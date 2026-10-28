// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <DPrintPreviewDialog>
#include <DPrintPreviewSettingInfo>
#include <DPushButton>
#include <DSuggestButton>
#include <QSemaphore>
#include <QSpinBox>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QTest>
#include <QThreadPool>
#include <QTimer>
#include <QtConcurrent>

DWIDGET_USE_NAMESPACE

class ut_DPrintPreviewDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        auto pool = QThreadPool::globalInstance();
        pool->waitForDone();
        oldThreadCount = pool->maxThreadCount();
        pool->setMaxThreadCount(1);
        // Hold printer construction until the test has exercised the loading state.
        blocker = QtConcurrent::run(pool, [this] {
            started.release();
            release.acquire();
        });
        started.acquire();
        target = new DPrintPreviewDialog;
        QObject::connect(target, QOverload<DPrinter *>::of(&DPrintPreviewDialog::paintRequested),
                         target, [this](DPrinter *printer) {
            ++paintCount;
            paintedPrinter = printer;
            paintedDocName = printer->docName();
            QPainter painter(printer);
            painter.drawText(QPointF(20, 20), QStringLiteral("Preview"));
        });
        QObject::connect(target, QOverload<DPrinter *, const QVector<int> &>::of(&DPrintPreviewDialog::paintRequested),
                         target, [this](DPrinter *printer, const QVector<int> &pages) {
            EXPECT_FALSE(pages.isEmpty());
            if (pages.isEmpty())
                return;
            ++paintCount;
            paintedPages = pages;
            QPainter painter(printer);
            for (int index = 0; index < pages.size(); ++index) {
                if (index)
                    printer->newPage();
                painter.drawText(QPointF(20, 20), QString::number(pages.at(index)));
            }
        });
    }

    void TearDown() override
    {
        delete target;
        unblock();
        QThreadPool::globalInstance()->waitForDone();
        QThreadPool::globalInstance()->setMaxThreadCount(oldThreadCount);
    }

    void unblock()
    {
        if (!unblocked) {
            release.release();
            unblocked = true;
        }
    }

    bool waitForPreview()
    {
        unblock();
        return QTest::qWaitFor([this] { return paintCount > 0; }, 10000);
    }

    DPrintPreviewDialog *target = nullptr;
    int paintCount = 0;
    QString paintedDocName;
    DPrinter *paintedPrinter = nullptr;
    QVector<int> paintedPages;
    int oldThreadCount = 0;
    bool unblocked = false;
    QSemaphore started;
    QSemaphore release;
    QFuture<void> blocker;
};

TEST_F(ut_DPrintPreviewDialog, LoadingWindowRemainsResponsive)
{
    target->show();
    bool eventDelivered = false;
    QTimer::singleShot(0, target, [&] { eventDelivered = true; });
    QTest::qWait(20);
    EXPECT_TRUE(eventDelivered);
    EXPECT_EQ(paintCount, 0);
    ASSERT_TRUE(waitForPreview());
}

TEST_F(ut_DPrintPreviewDialog, PendingDocumentAndSettingsArePreserved)
{
    target->setDocName(QStringLiteral("pending document"));
    EXPECT_EQ(target->docName(), QStringLiteral("pending document"));
    {
        DPrintPreviewCopiesInfo info;
        info.copies = 3;
        target->updateDialogSettingInfo(&info);
    } // The dialog must not retain the caller's pointer.
    // Installed settings plugins may deliberately reject this query.
    if (DPrintPreviewDialog::currentPlugin().isEmpty()) {
        QScopedPointer<DPrintPreviewSettingInfo> pending(target->createDialogSettingInfo(DPrintPreviewSettingInfo::PS_Copies));
        ASSERT_TRUE(pending);
        EXPECT_EQ(static_cast<DPrintPreviewCopiesInfo *>(pending.data())->copies, 3);
    }
    target->show();
    ASSERT_TRUE(waitForPreview());
    EXPECT_EQ(paintedDocName, QStringLiteral("pending document"));
    auto frame = target->findChild<QWidget *>(QStringLiteral("CopyCountFrame"));
    ASSERT_TRUE(frame);
    auto copies = frame->findChild<QSpinBox *>();
    ASSERT_TRUE(copies);
    EXPECT_EQ(copies->value(), 3);
}

TEST_F(ut_DPrintPreviewDialog, PendingPathAndAsyncPreviewArePreserved)
{
    QTemporaryFile file;
    ASSERT_TRUE(file.open());
    ASSERT_TRUE(target->setPrintFromPath(file.fileName()));
    EXPECT_EQ(target->printFromPath(), file.fileName());
    EXPECT_FALSE(target->setAsynPreview(-1));
    EXPECT_TRUE(target->setAsynPreview(4));
    EXPECT_TRUE(target->isAsynPreview());
    target->show();
    ASSERT_TRUE(waitForPreview());
    EXPECT_EQ(target->printFromPath(), file.fileName());
    EXPECT_TRUE(target->isAsynPreview());
}

TEST_F(ut_DPrintPreviewDialog, PendingAsyncImpositionWaitsForInitialPreview)
{
    ASSERT_TRUE(target->setAsynPreview(4));
    DPrintPreviewNUpPrintInfo info;
    info.enable = true;
    info.imposition = DPrintPreviewWidget::OneRowTwoCol;
    info.order = DPrintPreviewWidget::L2R_T2B;
    target->updateDialogSettingInfo(&info);
    target->show();
    ASSERT_TRUE(waitForPreview());
    auto preview = target->findChild<DPrintPreviewWidget *>();
    ASSERT_TRUE(preview);
    EXPECT_EQ(preview->imposition(), info.imposition);
    EXPECT_EQ(preview->pagesCount(), 2);
    EXPECT_EQ(paintedPages, QVector<int>({1, 2}));
}

TEST_F(ut_DPrintPreviewDialog, PendingAsyncPageRangeUsesDocumentPageCount)
{
    ASSERT_TRUE(target->setAsynPreview(10));
    DPrintPreviewPageRangeInfo info;
    info.rangeType = DPrintPreviewWidget::SelectPage;
    info.selectPages = QStringLiteral("2");
    target->updateDialogSettingInfo(&info);
    target->show();
    ASSERT_TRUE(waitForPreview());
    auto preview = target->findChild<DPrintPreviewWidget *>();
    ASSERT_TRUE(preview);
    EXPECT_EQ(preview->pageRangeMode(), info.rangeType);
    EXPECT_EQ(preview->pagesCount(), 1);
    EXPECT_EQ(preview->currentPage(), 2);
    EXPECT_EQ(paintedPages, QVector<int>({2}));
    auto printButton = target->findChild<DSuggestButton *>();
    ASSERT_TRUE(printButton);
    EXPECT_TRUE(printButton->isEnabled());
}

TEST_F(ut_DPrintPreviewDialog, CanDestroyWhileConstructionIsPending)
{
    target->show();
    QTest::qWait(20);
    delete target;
    target = nullptr;
    // Destruction must return even though the constructor task cannot run yet.
    EXPECT_FALSE(unblocked);
}

TEST_F(ut_DPrintPreviewDialog, CanExportPdfAfterStartup)
{
    target->show();
    ASSERT_TRUE(waitForPreview());
    ASSERT_TRUE(paintedPrinter);
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    const QString path = directory.filePath(QStringLiteral("preview.pdf"));
    paintedPrinter->setOutputFormat(QPrinter::PdfFormat);
    paintedPrinter->setOutputFileName(path);
    QPainter painter;
    ASSERT_TRUE(painter.begin(paintedPrinter));
    painter.drawText(QPointF(20, 20), QStringLiteral("PDF export after asynchronous startup"));
    ASSERT_TRUE(painter.end());
    QFile file(path);
    ASSERT_TRUE(file.open(QIODevice::ReadOnly));
    EXPECT_EQ(file.read(5), QByteArray("%PDF-"));
}

TEST_F(ut_DPrintPreviewDialog, CanCancelAndReopenWhileLoading)
{
    target->show();
    DPushButton *cancel = nullptr;
    for (auto button : target->findChildren<DPushButton *>()) {
        if (button->parentWidget()->objectName() == QStringLiteral("rightWidget")
            && button->text() == QCoreApplication::translate("DPrintPreviewDialogPrivate", "Cancel", "button")) {
            cancel = button;
            break;
        }
    }
    ASSERT_TRUE(cancel);
    ASSERT_TRUE(cancel->isEnabled());
    cancel->click();
    EXPECT_FALSE(target->isVisible());
    target->show();
    ASSERT_TRUE(waitForPreview());
    auto printer = paintedPrinter;
    target->reject();
    paintCount = 0;
    target->show();
    ASSERT_TRUE(waitForPreview());
    EXPECT_EQ(paintedPrinter, printer);
}

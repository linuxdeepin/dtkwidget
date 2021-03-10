#include <gtest/gtest.h>
#include <QTest>
#include <QSignalSpy>

#include "dprintpreviewwidget.h"
#include "dprintpreviewdialog.h"
#include "dprintpreviewwidget_p.h"
#include "dprintpreviewdialog_p.h"

#define DELAY_TIME 100

DWIDGET_USE_NAMESPACE

class TestPrintPreviewDialog : public DPrintPreviewDialog
{
public:
    explicit TestPrintPreviewDialog(QWidget *parent = nullptr)
        : DPrintPreviewDialog(parent)
    {
        connect(this, QOverload<DPrinter *>::of(&TestPrintPreviewDialog::paintRequested), this, &TestPrintPreviewDialog::slotPaintRequestSync);
        connect(this, QOverload<DPrinter *, const QVector<int> &>::of(&TestPrintPreviewDialog::paintRequested), this, &TestPrintPreviewDialog::slotPaintRequestAync);
    }

protected Q_SLOTS:
    void slotPaintRequestSync(DPrinter *printer);
    void slotPaintRequestAync(DPrinter *printer, const QVector<int> &pageRange);
};

void TestPrintPreviewDialog::slotPaintRequestSync(DPrinter *printer)
{
    enum { pageCount = 4 };

    QPainter painter;
    painter.begin(printer);
    for (int i = 0; i < pageCount; ++i) {
        const QRect f = printer->pageRect(QPrinter::DevicePixel).toRect();
        painter.fillRect(f, Qt::white);
        painter.drawText(f.center(), QString::fromLatin1("Page %1").arg(i + 1));
        if (i != pageCount - 1)
            printer->newPage();
    }
    painter.end();
}

void TestPrintPreviewDialog::slotPaintRequestAync(DPrinter *printer, const QVector<int> &pageRange)
{
    enum { pageCount = 5 };

    QPainter painter;
    painter.begin(printer);
    for (int i = 0; i < pageCount; ++i) {
        if (!pageRange.contains(i + 1))
            continue;

        const QRect f = printer->pageRect(QPrinter::DevicePixel).toRect();
        painter.fillRect(f, Qt::white);
        painter.drawText(f.center(), QString::fromLatin1("Page %1").arg(i + 1));
        if (i != pageCount - 1)
            printer->newPage();
    }
    painter.end();
}

class ut_DPrintPreviewWidget : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DPrintPreviewWidget *previewWidget = nullptr;
    TestPrintPreviewDialog *printDialog = nullptr;
};

void ut_DPrintPreviewWidget::SetUp()
{
    printDialog = new TestPrintPreviewDialog;
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    previewWidget = printDialog->findChild<DPrintPreviewWidget *>();
}

void ut_DPrintPreviewWidget::TearDown()
{
    delete printDialog;
}

TEST_F(ut_DPrintPreviewWidget, testForInit)
{
    // 测试窗口能否成功创建
    ASSERT_TRUE(previewWidget);
    ASSERT_TRUE(previewWidget->isVisible());

    // 测试部分初始化数据是否正常
    ASSERT_TRUE(previewWidget->d_func()->previewPrinter);
    ASSERT_EQ(previewWidget->d_func()->currentPageNumber, 1);
    ASSERT_EQ(previewWidget->d_func()->pageRangeMode, DPrintPreviewWidget::AllPage);
    ASSERT_EQ(previewWidget->d_func()->pages.count(), 4);
    ASSERT_EQ(previewWidget->d_func()->pictures.count(), 4);
    ASSERT_FALSE(previewWidget->d_func()->isAsynPreview);
    ASSERT_TRUE(previewWidget->d_func()->printFromPath.isEmpty());
}

TEST_F(ut_DPrintPreviewWidget, testForInitFunc)
{
    // 测试初始化属性获取函数是否正常输出
    ASSERT_EQ(previewWidget->pageRangeMode(), DPrintPreviewWidget::AllPage);
    ASSERT_EQ(previewWidget->pagesCount(), 4);
    ASSERT_EQ(previewWidget->currentPage(), 1);
    ASSERT_TRUE(previewWidget->turnPageAble());
    ASSERT_EQ(previewWidget->getColorMode(), previewWidget->d_func()->previewPrinter->colorMode());
    ASSERT_EQ(previewWidget->getScale(), 1);
    ASSERT_EQ(previewWidget->imposition(), DPrintPreviewWidget::One);
    ASSERT_EQ(previewWidget->imposition(), DPrintPreviewWidget::One);
    ASSERT_TRUE(previewWidget->printFromPath().isEmpty());
    ASSERT_FALSE(previewWidget->isAsynPreview());
    ASSERT_EQ(previewWidget->targetPageCount(4), 4);
    ASSERT_EQ(previewWidget->originPageCount(), 4);

    previewWidget->themeTypeChanged(DGuiApplicationHelper::DarkType);
    ASSERT_EQ(previewWidget->d_func()->scene->backgroundBrush().color(), QColor(0, 0, 0, 3));

    if (previewWidget->d_func()->previewPrinter->outputFormat() != DPrinter::NativeFormat) {
        ASSERT_TRUE(previewWidget->printerColorModel().isEmpty());
    }
}

#define TESTCASE_TEXT(COUNT) \
    QStringLiteral("测试 %1").arg(COUNT)

TEST_F(ut_DPrintPreviewWidget, testForSetFunc)
{
    // 测试属性设置函数设置后的属性是否正常
    DPrintPreviewWidgetPrivate *pview_d = previewWidget->d_func();
    ASSERT_TRUE(pview_d);

    previewWidget->setPageRange({1, 2});
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 1 << 2);
    previewWidget->setPageRange({1, 2});
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 1 << 2);
    previewWidget->setPageRange(2, 3);
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 2 << 3);
    previewWidget->setPageRange(3, 2);
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 2 << 3);

    previewWidget->setPageRangeALL();
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 1 << 2 << 3 << 4);

    previewWidget->setPageRangeMode(DPrintPreviewWidget::CurrentPage);
    ASSERT_EQ(pview_d->pageRangeMode, DPrintPreviewWidget::CurrentPage);
    previewWidget->setPageRangeMode(DPrintPreviewWidget::AllPage);
    ASSERT_EQ(pview_d->pageRangeMode, DPrintPreviewWidget::AllPage);

    previewWidget->setColorMode(DPrinter::Color);
    ASSERT_EQ(pview_d->colorMode, DPrinter::Color);

    previewWidget->setOrientation(DPrinter::Landscape);
    ASSERT_EQ(pview_d->previewPrinter->orientation(), DPrinter::Landscape);

    previewWidget->setScale(1.25);
    ASSERT_EQ(previewWidget->getScale(), 1.25);

    previewWidget->setWaterMarkType(1);
    ASSERT_EQ(pview_d->waterMark->getType(), 1);

    QFont font = previewWidget->font();
    font.setPointSize(18);
    previewWidget->setWaterMarkFont(font);
    ASSERT_EQ(pview_d->waterMark->getFont().pointSize(), font.pointSize());

    QImage testImage(QSize(20, 20), QImage::Format_ARGB32);
    testImage.fill(Qt::green);
    previewWidget->setWaterMargImage(testImage);
    ASSERT_EQ(pview_d->waterMark->sourceImage, testImage);

    previewWidget->setWaterMarkRotate(270);
    ASSERT_EQ(pview_d->waterMark->rotation(), 270);

    previewWidget->setWaterMarkScale(1.25);
    ASSERT_EQ(pview_d->waterMark->mScaleFactor, 1.25);

    previewWidget->setWaterMarkOpacity(0.78);
    ASSERT_EQ(pview_d->waterMark->opacity(), 0.78);

    previewWidget->setConfidentialWaterMark();
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), qApp->translate("DPrintPreviewWidget", "Confidential").toLocal8Bit());

    previewWidget->setDraftWaterMark();
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), qApp->translate("DPrintPreviewWidget", "Draft").toLocal8Bit());

    previewWidget->setSampleWaterMark();
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), qApp->translate("DPrintPreviewWidget", "Sample").toLocal8Bit());

    previewWidget->setCustomWaterMark(TESTCASE_TEXT(1));
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), TESTCASE_TEXT(1).toLocal8Bit());

    previewWidget->setTextWaterMark(TESTCASE_TEXT(2));
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), TESTCASE_TEXT(2).toLocal8Bit());

    previewWidget->setWaterMarkColor(Qt::cyan);
    ASSERT_EQ(pview_d->waterMark->color, QColor(Qt::cyan));

    previewWidget->setWaterMarkLayout(1);
    ASSERT_EQ(pview_d->waterMark->layout, 1);

    previewWidget->setImposition(DPrintPreviewWidget::OneRowTwoCol);
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::OneRowTwoCol);

    previewWidget->setWaterMarkType(1);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->type, WaterMark::Text);

    previewWidget->setWaterMargImage(testImage);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->sourceImage, testImage);

    previewWidget->setWaterMarkRotate(270);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->rotation(), 270);

    previewWidget->setWaterMarkScale(1.25);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->mScaleFactor, 1.25);

    previewWidget->setWaterMarkOpacity(0.78);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->opacity(), 0.78);

    previewWidget->setConfidentialWaterMark();
    ASSERT_STREQ(pview_d->numberUpPrintData->waterList.first()->text.toLocal8Bit(), qApp->translate("DPrintPreviewWidget", "Confidential").toLocal8Bit());

    previewWidget->setDraftWaterMark();
    ASSERT_STREQ(pview_d->numberUpPrintData->waterList.first()->text.toLocal8Bit(), qApp->translate("DPrintPreviewWidget", "Draft").toLocal8Bit());

    previewWidget->setSampleWaterMark();
    ASSERT_STREQ(pview_d->numberUpPrintData->waterList.first()->text.toLocal8Bit(), qApp->translate("DPrintPreviewWidget", "Sample").toLocal8Bit());

    previewWidget->setCustomWaterMark(TESTCASE_TEXT(1));
    ASSERT_STREQ(pview_d->numberUpPrintData->waterList.first()->text.toLocal8Bit(), TESTCASE_TEXT(1).toLocal8Bit());

    previewWidget->setTextWaterMark(TESTCASE_TEXT(2));
    ASSERT_STREQ(pview_d->numberUpPrintData->waterList.first()->text.toLocal8Bit(), TESTCASE_TEXT(2).toLocal8Bit());

    font.setPointSize(18);
    previewWidget->setWaterMarkFont(font);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->font.pointSize(), 18);

    previewWidget->setWaterMarkColor(Qt::cyan);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->color, QColor(Qt::cyan));

    previewWidget->setWaterMarkLayout(1);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->layout, 1);

    previewWidget->setOrder(DPrintPreviewWidget::R2L_T2B);
    ASSERT_EQ(pview_d->order, DPrintPreviewWidget::R2L_T2B);
    previewWidget->setOrder(DPrintPreviewWidget::Copy);
    ASSERT_EQ(pview_d->order, DPrintPreviewWidget::Copy);

    previewWidget->setPrintMode(DPrintPreviewWidget::PrintToImage);
    ASSERT_EQ(pview_d->printMode, DPrintPreviewWidget::PrintToImage);

    previewWidget->setCurrentPage(2);
    ASSERT_EQ(pview_d->currentPageNumber, 2);

    previewWidget->setAsynPreview(4);
    previewWidget->updatePreview();
    ASSERT_EQ(pview_d->isAsynPreview, true);
    previewWidget->setColorMode(DPrinter::Color);

    previewWidget->refreshBegin();
    ASSERT_EQ(pview_d->refreshMode, DPrintPreviewWidgetPrivate::RefreshDelay);
    previewWidget->refreshEnd();
    ASSERT_EQ(pview_d->refreshMode, DPrintPreviewWidgetPrivate::RefreshImmediately);

    ASSERT_EQ(previewWidget->originPageCount(), 4);
}

TEST_F(ut_DPrintPreviewWidget, testPrint)
{
    previewWidget->d_func()->previewPrinter->setOutputFileName("widget_test.pdf");
    previewWidget->setPrintMode(DPrintPreviewWidget::PrintToPrinter);
    previewWidget->setPrintFromPath("test_pdf");

    if (previewWidget->d_func()->previewPrinter->outputFormat() == DPrinter::NativeFormat) {
        previewWidget->print();

        previewWidget->setPrintFromPath("");
        previewWidget->print();
    }

    previewWidget->setPrintMode(DPrintPreviewWidget::PrintToPdf);
    previewWidget->print();
    ASSERT_TRUE(QFileInfo("widget_test.pdf").exists());

    previewWidget->d_func()->previewPrinter->setOutputFileName("widget_test.png");
    previewWidget->setPrintMode(DPrintPreviewWidget::PrintToImage);
    previewWidget->print();
    ASSERT_TRUE(QFileInfo("widget_test(1).png").exists());
}

class ut_DPrintPreviewWidgetTestParam : public testing::TestWithParam<int>
{
protected:
    void SetUp() override;
    void TearDown() override;

    DPrintPreviewWidget *previewWidget = nullptr;
    TestPrintPreviewDialog *printDialog = nullptr;
};

void ut_DPrintPreviewWidgetTestParam::SetUp()
{
    printDialog = new TestPrintPreviewDialog;
    previewWidget = printDialog->findChild<DPrintPreviewWidget *>();
}

void ut_DPrintPreviewWidgetTestParam::TearDown()
{
    delete printDialog;
}

INSTANTIATE_TEST_CASE_P(DPrintPreviewWidget, ut_DPrintPreviewWidgetTestParam, ::testing::Range(1, 5));

TEST_P(ut_DPrintPreviewWidgetTestParam, testUpdatePreview)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 多次调用重绘 测试函数是否崩溃
    previewWidget->updatePreview();
    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testAyncUpdatePreview)
{
    previewWidget->setAsynPreview(5);
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 多次调用异步的刷新预览函数 测试函数是否崩溃
    previewWidget->setCurrentPage(GetParam());
    previewWidget->updatePreview();
    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testTurnFront)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 多次向前切换当前页码 测试函数是否崩溃且页码是否正常
    int currentPage = 5 - GetParam() + 1;
    if (currentPage > 4) {
        currentPage = 4;
    } else if (currentPage < 0) {
        currentPage = 0;
    }

    previewWidget->setCurrentPage(currentPage);
    previewWidget->turnFront();

    ASSERT_EQ(previewWidget->currentPage(), currentPage - 1);
    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testTurnBack)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 多次向后切换当前页码 测试函数是否崩溃且页码是否正常
    int currentPage = GetParam();
    previewWidget->setCurrentPage(currentPage);
    previewWidget->turnBack();

    if (currentPage > 3) {
        currentPage = 3;
    }

    ASSERT_EQ(previewWidget->currentPage(), currentPage + 1);
    previewWidget->setCurrentPage(6);
    ASSERT_EQ(previewWidget->currentPage(), previewWidget->pagesCount());

    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testAsynTurnFront)
{
    // setAsynPreview() should be called before show().
    previewWidget->setAsynPreview(5);
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 异步方式切换当前页码 测试函数运行是否崩溃且当前页码是否正常
    int currentPage = 5 - GetParam() + 1;
    previewWidget->setCurrentPage(currentPage);
    previewWidget->pagesCount();

    previewWidget->turnFront();
    ASSERT_EQ(previewWidget->currentPage(), currentPage - 1);
    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testSyncNumberUp)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 同步模式下测试并打
    // 2-6 为并打所有的模式
    previewWidget->setImposition(DPrintPreviewWidget::Imposition(GetParam() + 1));
    previewWidget->setOrder(DPrintPreviewWidget::Order(GetParam()));

    // 测试并打模式下 总页码是否正常
    switch (previewWidget->imposition()) {
    case DPrintPreviewWidget::OneRowTwoCol:
        ASSERT_EQ(previewWidget->pagesCount(), 2);
        break;

    case DPrintPreviewWidget::TwoRowTwoCol:
        ASSERT_EQ(previewWidget->pagesCount(), 1);
        break;

    case DPrintPreviewWidget::TwoRowThreeCol:
        ASSERT_EQ(previewWidget->pagesCount(), 1);
        break;

    case DPrintPreviewWidget::ThreeRowThreeCol:
        ASSERT_EQ(previewWidget->pagesCount(), 1);
        break;

    case DPrintPreviewWidget::FourRowFourCol:
        // copy 模式为总页码
        ASSERT_EQ(previewWidget->pagesCount(), 4);
        break;

    default:
        break;
    }

    // 测试并打模式下 切换页码是否正常(未崩溃且页码正常)
    for (int i = 1; i <= previewWidget->pagesCount(); ++i) {
        previewWidget->setCurrentPage(i);
        ASSERT_EQ(previewWidget->currentPage(), i);
    }

    previewWidget->setImposition(DPrintPreviewWidget::One);
    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testAsynNumberUp)
{
    printDialog->setAsynPreview(5);
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    int param = GetParam();
    // 测试异步并打
    previewWidget->setImposition(DPrintPreviewWidget::Imposition(param + 1));
    previewWidget->setOrder(DPrintPreviewWidget::Order(param));

    // 测试并打模式下 总页码是否正常
    switch (previewWidget->imposition()) {
    case DPrintPreviewWidget::OneRowTwoCol:
        ASSERT_EQ(previewWidget->pagesCount(), 3);
        break;

    case DPrintPreviewWidget::TwoRowTwoCol:
        ASSERT_EQ(previewWidget->pagesCount(), 2);
        break;

    case DPrintPreviewWidget::TwoRowThreeCol:
        ASSERT_EQ(previewWidget->pagesCount(), 1);
        break;

    case DPrintPreviewWidget::ThreeRowThreeCol:
        ASSERT_EQ(previewWidget->pagesCount(), 1);
        break;

    case DPrintPreviewWidget::FourRowFourCol:
        // copy 模式为总页码
        ASSERT_EQ(previewWidget->pagesCount(), 5);
        break;

    default:
        break;
    }

    // 测试并打模式下 切换页码是否正常(未崩溃且页码正常)
    for (int i = 1; i <= previewWidget->pagesCount(); ++i) {
        previewWidget->setCurrentPage(i);
        ASSERT_EQ(previewWidget->currentPage(), i);
    }

    if (previewWidget->order() == DPrintPreviewWidget::Copy) {
        QSignalSpy orderSpy(previewWidget, SIGNAL(currentPageChanged(int)));
        previewWidget->setOrder(DPrintPreviewWidget::Order(param % 4));
        ASSERT_EQ(orderSpy.count(), 1);
    }

    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testSyncNumberUpWithWaterMark)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 测试同步 并打 + 水印同时存在 函数是否出现崩溃
    previewWidget->setImposition(DPrintPreviewWidget::Imposition(GetParam() + 1));
    previewWidget->setOrder(DPrintPreviewWidget::Order(GetParam()));

    // 一半添加文字水印 一半添加图片水印
    if (GetParam() % 2) {
        previewWidget->setWaterMarkType(1);
        previewWidget->setWaterMarkColor(Qt::darkCyan);
        previewWidget->setWaterMarkScale(1.25);
        previewWidget->setWaterMarkRotate(270);
        previewWidget->setWaterMarkOpacity(0.74);
        previewWidget->setTextWaterMark(TESTCASE_TEXT(1));
    } else {
        QImage testImage(QSize(30, 30), QImage::Format_ARGB32);
        testImage.fill(Qt::darkYellow);

        QPainter painter;
        painter.begin(&testImage);
        painter.setPen(Qt::white);
        painter.drawText(QRect({0, 0}, testImage.size()), Qt::AlignCenter, TESTCASE_TEXT(GetParam()));
        painter.end();

        previewWidget->setWaterMarkType(2);
        previewWidget->setWaterMarkScale(0.85);
        previewWidget->setWaterMarkRotate(256);
        previewWidget->setWaterMarkOpacity(0.47);
        previewWidget->setWaterMargImage(testImage);
        previewWidget->setWaterMarkLayout(1);
    }

    // 测试并打模式下 切换页码是否正常(未崩溃且页码正常)
    for (int i = 1; i <= previewWidget->pagesCount(); ++i) {
        previewWidget->setCurrentPage(i);
        ASSERT_EQ(previewWidget->currentPage(), i);
    }

    printDialog->close();
}

TEST_P(ut_DPrintPreviewWidgetTestParam, testAsynNumberUpWithWaterMark)
{
    printDialog->setAsynPreview(5);
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 测试同步 并打 + 水印同时存在 函数是否出现崩溃
    previewWidget->setImposition(DPrintPreviewWidget::Imposition(GetParam() + 1));
    previewWidget->setOrder(DPrintPreviewWidget::Order(GetParam()));

    // 一半case添加文字水印 另一半添加图片水印
    if (GetParam() % 2) {
        previewWidget->setWaterMarkType(1);
        previewWidget->setWaterMarkColor(Qt::darkCyan);
        previewWidget->setWaterMarkScale(1.25);
        previewWidget->setWaterMarkRotate(270);
        previewWidget->setWaterMarkOpacity(0.74);
        previewWidget->setTextWaterMark(TESTCASE_TEXT(1));
    } else {
        QImage testImage(QSize(30, 30), QImage::Format_ARGB32);
        testImage.fill(Qt::darkYellow);

        QPainter painter;
        painter.begin(&testImage);
        painter.setPen(Qt::white);
        painter.drawText(QRect({0, 0}, testImage.size()), Qt::AlignCenter, TESTCASE_TEXT(GetParam()));
        painter.end();

        previewWidget->setWaterMarkType(2);
        previewWidget->setWaterMarkScale(0.85);
        previewWidget->setWaterMarkRotate(256);
        previewWidget->setWaterMarkOpacity(0.47);
        previewWidget->setWaterMargImage(testImage);
        previewWidget->setWaterMarkLayout(1);
    }

    // 测试并打模式下 切换页码是否正常(未崩溃且页码正常)
    for (int i = 1; i <= previewWidget->pagesCount(); ++i) {
        previewWidget->setCurrentPage(i);
        ASSERT_EQ(previewWidget->currentPage(), i);
    }

    printDialog->close();
}

class ut_DPrintPreviewWidgetPrivate : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DPrintPreviewWidgetPrivate *pview_d = nullptr;
    TestPrintPreviewDialog *printDialog = nullptr;
};

void ut_DPrintPreviewWidgetPrivate::SetUp()
{
    printDialog = new TestPrintPreviewDialog;
    DPrintPreviewWidget *previewWidget = printDialog->findChild<DPrintPreviewWidget *>();
    pview_d = previewWidget->d_func();

    ASSERT_TRUE(pview_d);
}

void ut_DPrintPreviewWidgetPrivate::TearDown()
{
    delete printDialog;
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivateFuncUpdate)
{
    // 测试私有函数的重绘部分 生成场景的稳定性
    QSize paperSize = pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size();
    pview_d->generatePreviewPicture();
    pview_d->populateScene();
    ASSERT_TRUE(pview_d->background);
    ASSERT_EQ(pview_d->background->rect(), QRect(QPoint(0, 0), paperSize));
    ASSERT_EQ(pview_d->background->brush(), QBrush(Qt::white));
    ASSERT_FALSE(pview_d->pages.isEmpty());
    ASSERT_FALSE(pview_d->pictures.isEmpty());

    // 测试调用函数的稳定性
    pview_d->fitView();
    pview_d->generatePreview();

    // 构造一个并打的场景 测试重绘的稳定性
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    pview_d->generatePreview();

    // 构造异步场景 测试函数的稳定性
    pview_d->isAsynPreview = true;
    pview_d->asynPreviewTotalPage = 5;
    pview_d->populateScene();
    pview_d->fitView();
    pview_d->generatePreview();

    // 构造异步自定义页码场景
    pview_d->pageRange = {1, 2};
    pview_d->pageRangeMode = DPrintPreviewWidget::SelectPage;
    pview_d->generatePreview();
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivateFuncPages)
{
    // 测试私有类函数的页面部分
    pview_d->generatePreview();

    // 同步模式下测试页码范围
    pview_d->pageRange = {2, 3};
    pview_d->setPageRangeAll();
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 1 << 2 << 3 << 4);
    pview_d->setCurrentPage(3);
    ASSERT_EQ(pview_d->currentPageNumber, 3);

    // 异步模式下测试页码范围
    pview_d->setCurrentPage(1);
    ASSERT_EQ(pview_d->currentPageNumber, 1);
    pview_d->setCurrentPage(5);
    ASSERT_EQ(pview_d->currentPageNumber, 4);
    pview_d->isAsynPreview = true;
    pview_d->asynPreviewTotalPage = 5;
    pview_d->pageRange = {2, 3};
    pview_d->setPageRangeAll();
    ASSERT_EQ(pview_d->pageRange, QVector<int>() << 1 << 2 << 3 << 4 << 5);
    pview_d->setCurrentPage(3);
    ASSERT_EQ(pview_d->currentPageNumber, 3);

    // 测试不同情况下的页码是否正常
    ASSERT_EQ(pview_d->pagesCount(), 5);
    pview_d->imposition = DPrintPreviewWidget::OneRowTwoCol;
    ASSERT_EQ(pview_d->pagesCount(), 3);
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    ASSERT_EQ(pview_d->pagesCount(), 2);
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    ASSERT_EQ(pview_d->pagesCount(), 1);
    pview_d->imposition = DPrintPreviewWidget::FourRowFourCol;
    ASSERT_EQ(pview_d->pagesCount(), 1);

    pview_d->order = DPrintPreviewWidget::Copy;
    ASSERT_EQ(pview_d->pagesCount(), 5);

    pview_d->imposition = DPrintPreviewWidget::One;
    ASSERT_EQ(pview_d->targetPage(5), 5);
    pview_d->imposition = DPrintPreviewWidget::OneRowTwoCol;
    ASSERT_EQ(pview_d->targetPage(5), 3);
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    ASSERT_EQ(pview_d->targetPage(5), 2);
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    ASSERT_EQ(pview_d->targetPage(5), 1);
    pview_d->imposition = DPrintPreviewWidget::FourRowFourCol;
    ASSERT_EQ(pview_d->targetPage(5), 1);

    ASSERT_EQ(pview_d->index2page(0), 1);
    pview_d->pageRange = {2, 3};
    ASSERT_EQ(pview_d->index2page(1), 3);

    ASSERT_EQ(pview_d->page2index(3), 1);
    pview_d->pageRange = {4, 5};
    ASSERT_EQ(pview_d->page2index(4), 0);
    pview_d->setPageRangeAll();
    ASSERT_EQ(pview_d->page2index(1), 0);

    pview_d->imposition = DPrintPreviewWidget::One;
    ASSERT_EQ(pview_d->impositionPages(DPrintPreviewWidget::One), 1);
    pview_d->imposition = DPrintPreviewWidget::OneRowTwoCol;
    ASSERT_EQ(pview_d->impositionPages(DPrintPreviewWidget::OneRowTwoCol), 2);
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    ASSERT_EQ(pview_d->impositionPages(DPrintPreviewWidget::TwoRowTwoCol), 4);
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    ASSERT_EQ(pview_d->impositionPages(DPrintPreviewWidget::TwoRowThreeCol), 6);
    pview_d->imposition = DPrintPreviewWidget::ThreeRowThreeCol;
    ASSERT_EQ(pview_d->impositionPages(DPrintPreviewWidget::ThreeRowThreeCol), 9);
    pview_d->imposition = DPrintPreviewWidget::FourRowFourCol;
    ASSERT_EQ(pview_d->impositionPages(DPrintPreviewWidget::FourRowFourCol), 16);

    pview_d->order = DPrintPreviewWidget::R2L_T2B;
    pview_d->currentPageNumber = 1;
    ASSERT_EQ(pview_d->requestPages(1), QVector<int>() << 1 << 2 << 3 << 4 << 5);
    pview_d->order = DPrintPreviewWidget::Copy;
    ASSERT_EQ(pview_d->requestPages(2), QVector<int>() << 2);
    pview_d->order = DPrintPreviewWidget::T2B_L2R;
    ASSERT_EQ(pview_d->requestPages(1), QVector<int>() << 1 << 2 << 3 << 4 << 5);

    // 测试异步并打接口能否正常调用
    ASSERT_FALSE(pview_d->numberUpPrintData);
    pview_d->calculateNumberUpPage();
    ASSERT_TRUE(pview_d->numberUpPrintData);
    pview_d->calculateNumberPagePosition();
    ASSERT_FALSE(pview_d->numberUpPrintData->paintPoints.isEmpty());
    pview_d->updateNumberUpContent();
    ASSERT_TRUE(pview_d->numberUpPrintData->numberItem);

    pview_d->releaseImpositionData();
    ASSERT_FALSE(pview_d->numberUpPrintData);

    pview_d->calculateNumberUpPage();
    ASSERT_NE(pview_d->numberUpPrintData->rowCount, 0);
    ASSERT_NE(pview_d->numberUpPrintData->columnCount, 0);
    ASSERT_NE(pview_d->numberUpPrintData->scaleRatio, 1);
    pview_d->calculateCurrentNumberPage();
    // 当前pictures数量为4
    ASSERT_EQ(pview_d->numberUpPrintData->previewPictures.length(), 4);
    pview_d->displayWaterMarkItem();
    // 16版时 pictures数量为4 水印数量和pictures数量对齐
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.length(), 4);

    pview_d->releaseImpositionData();
    pview_d->isAsynPreview = false;
    pview_d->generatePreview();
    // 同步模式测试并打数据是否正常
    pview_d->calculateNumberUpPage();
    ASSERT_NE(pview_d->numberUpPrintData->rowCount, 0);
    ASSERT_NE(pview_d->numberUpPrintData->columnCount, 0);
    ASSERT_NE(pview_d->numberUpPrintData->scaleRatio, 1);
    pview_d->calculateCurrentNumberPage();
    // 当前pictures数量为4
    ASSERT_EQ(pview_d->numberUpPrintData->previewPictures.length(), 4);
    pview_d->displayWaterMarkItem();
    // 16版时 pictures数量为4 水印数量和pictures数量对齐
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.length(), 4);
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivateSyncPrint)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 指定模式生成pdf文件 不直接通过打印机打印
    pview_d->previewPrinter->setOutputFormat(QPrinter::PdfFormat);
    pview_d->previewPrinter->setOutputFileName("test_sync_no_water.pdf");
    pview_d->syncPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_sync_no_water.pdf").exists());

    pview_d->waterMark->setText(TESTCASE_TEXT(""));
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setLayoutType(WaterMark::Center);
    pview_d->waterMark->setColor(Qt::gray);

    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_sync_with_water.pdf");
    pview_d->syncPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_sync_with_water.pdf").exists());

    // 2x2 并打 函数功能和文件是否正常生成
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_sync_2x2_with_water.pdf");
    pview_d->syncPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_sync_2x2_with_water.pdf").exists());

    // 2x3 并打 当前页 函数功能和文件是否正常生成
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    pview_d->pageRangeMode = DPrintPreviewWidget::CurrentPage;
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_sync_2x2_currentpage_with_water.pdf");
    pview_d->syncPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_sync_2x2_currentpage_with_water.pdf").exists());

    // 2x3 并打 拷贝模式测试
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    pview_d->order = DPrintPreviewWidget::Copy;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_sync_2x3_with_water.pdf");
    pview_d->syncPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_sync_2x3_with_water.pdf").exists());
    printDialog->close();
    // The function passes if it doesn't crash.
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivateAsynPrint)
{
    printDialog->setAsynPreview(5);
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 构造一个需要打印的页面页码值
    pview_d->previewPages = pview_d->pageRange;
    pview_d->generatePreviewPicture();

    // 测试异步情况下的私有类打印函数
    pview_d->previewPrinter->setOutputFormat(QPrinter::PdfFormat);
    pview_d->previewPrinter->setOutputFileName("test_asyn_no_water.pdf");
    pview_d->asynPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_asyn_no_water.pdf").exists());

    pview_d->waterMark->setText("测试Asyn");
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setRotation(30);
    pview_d->waterMark->setLayoutType(WaterMark::Tiled);
    pview_d->waterMark->setColor(Qt::red);

    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_asyn_with_water.pdf");
    pview_d->asynPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_asyn_with_water.pdf").exists());

    // 2x2 并打 函数功能和文件是否正常生成
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_asyn_2x2_with_water.pdf");
    pview_d->asynPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_asyn_2x2_with_water.pdf").exists());

    // 2x3 并打 拷贝模式测试
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    pview_d->order = DPrintPreviewWidget::Copy;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_asyn_2x3_with_water.pdf");
    pview_d->asynPrint({0, 0}, pview_d->previewPrinter->pageRect(), pview_d->pageRange);
    ASSERT_TRUE(QFileInfo("test_asyn_2x3_with_water.pdf").exists());
    printDialog->close();
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivateSyncPrintToImage)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    pview_d->previewPrinter->setOutputFormat(QPrinter::PdfFormat);
    pview_d->previewPrinter->setOutputFileName("test_images_sync_no_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    // 另存为图片为异步方式 需要等待图片生成成功再测试
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_images_sync_no_water(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_no_water(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_no_water(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_no_water(4).png").exists());

    pview_d->waterMark->setText("测试Sync");
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setRotation(80);
    pview_d->waterMark->setLayoutType(WaterMark::Center);
    pview_d->waterMark->setColor(Qt::blue);

    pview_d->previewPrinter->setOutputFileName("test_images_sync_with_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    // 另存为图片为异步方式 需要等待图片生成成功再测试
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_images_sync_with_water(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_with_water(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_with_water(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_with_water(4).png").exists());

    // 2x2 并打 函数功能和文件是否正常生成
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_images_sync_2x2_with_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    QTest::qWait(DELAY_TIME);
    // 并打时 图片仅剩余一张
    ASSERT_TRUE(QFileInfo("test_images_sync_2x2_with_water(1).png").exists());

    // 2x3 并打 拷贝模式测试
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    pview_d->order = DPrintPreviewWidget::Copy;
    pview_d->scale = 0.85;
    pview_d->pageRangeMode = DPrintPreviewWidget::CurrentPage;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_images_sync_2x3_with_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_images_sync_2x3_with_water(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_2x3_with_water(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_2x3_with_water(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_sync_2x3_with_water(4).png").exists());
    printDialog->close();
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivateAsynPrintToImage)
{
    // 拷贝同步部分代码 添加异步属性直接测试
    printDialog->setAsynPreview(5);
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    // 构造一个需要打印的页面页码值
    pview_d->previewPages = pview_d->pageRange;
    pview_d->generatePreviewPicture();

    pview_d->previewPrinter->setOutputFormat(QPrinter::PdfFormat);
    pview_d->previewPrinter->setOutputFileName("test_images_asyn_no_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    // 另存为图片为异步方式 需要等待图片生成成功再测试
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_images_asyn_no_water(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_no_water(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_no_water(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_no_water(4).png").exists());

    pview_d->waterMark->setText("测试Asyn");
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setRotation(80);
    pview_d->waterMark->setLayoutType(WaterMark::Center);
    pview_d->waterMark->setColor(Qt::yellow);

    pview_d->previewPrinter->setOutputFileName("test_images_asyn_with_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    // 另存为图片为异步方式 需要等待图片生成成功再测试
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_images_asyn_with_water(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_with_water(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_with_water(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_with_water(4).png").exists());

    // 2x2 并打 函数功能和文件是否正常生成
    pview_d->imposition = DPrintPreviewWidget::TwoRowTwoCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_images_asyn_2x2_with_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    QTest::qWait(DELAY_TIME);
    // 并打时 图片仅剩余一张
    ASSERT_TRUE(QFileInfo("test_images_asyn_2x2_with_water(1).png").exists());

    // 2x3 并打 拷贝模式测试
    pview_d->imposition = DPrintPreviewWidget::TwoRowThreeCol;
    pview_d->order = DPrintPreviewWidget::Copy;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    pview_d->displayWaterMarkItem();
    ASSERT_FALSE(pview_d->generateWaterMarkImage().isNull());
    pview_d->previewPrinter->setOutputFileName("test_images_asyn_2x3_with_water.png");
    pview_d->printAsImage(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size(), pview_d->pageRange);
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_images_asyn_2x3_with_water(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_2x3_with_water(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_2x3_with_water(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_images_asyn_2x3_with_water(4).png").exists());
    printDialog->close();
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivatePrint)
{
    pview_d->generatePreview();
    pview_d->previewPrinter->setOutputFormat(QPrinter::PdfFormat);
    pview_d->previewPrinter->setOutputFileName("test_sync_print.pdf");

    // 同步直接调用打印函数
    pview_d->print();
    ASSERT_TRUE(QFileInfo("test_sync_print.pdf").exists());

    // 同步 + 当前页 + 缩放 直接调用打印函数
    pview_d->previewPrinter->setOutputFileName("test_currentpage_scale_sync_print.pdf");
    pview_d->pageRangeMode = DPrintPreviewWidget::CurrentPage;
    pview_d->scale = 0.5;
    pview_d->print();
    ASSERT_TRUE(QFileInfo("test_currentpage_scale_sync_print.pdf").exists());

    pview_d->isAsynPreview = true;
    pview_d->asynPreviewTotalPage = 4;
    pview_d->previewPrinter->setOutputFileName("test_asyn_print.pdf");

    // 异步直接调用打印函数
    pview_d->print();
    ASSERT_TRUE(QFileInfo("test_asyn_print.pdf").exists());

    // 异步另存为图片
    pview_d->pageRangeMode = DPrintPreviewWidget::AllPage;
    pview_d->scale = 1;
    pview_d->previewPrinter->setOutputFileName("test_asyn_print.png");

    pview_d->print(true);
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_asyn_print(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_asyn_print(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_asyn_print(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_asyn_print(4).png").exists());

    pview_d->isAsynPreview = false;
    pview_d->asynPreviewTotalPage = 0;
    pview_d->previewPrinter->setOutputFileName("test_sync_print.png");

    // 同步另存为图片
    pview_d->print(true);
    QTest::qWait(DELAY_TIME);
    ASSERT_TRUE(QFileInfo("test_sync_print(1).png").exists());
    ASSERT_TRUE(QFileInfo("test_sync_print(2).png").exists());
    ASSERT_TRUE(QFileInfo("test_sync_print(3).png").exists());
    ASSERT_TRUE(QFileInfo("test_sync_print(4).png").exists());
}

static bool testPixmapHasData(const QPixmap &pixmap)
{
    if (pixmap.isNull())
        return false;

    QImage image = pixmap.toImage();

    image.reinterpretAsFormat(QImage::Format_RGB32);
    const QRgb *bits = reinterpret_cast<const QRgb *>(image.constBits());
    const QRgb *end = bits + image.sizeInBytes() / sizeof(QRgb);
    return !std::all_of(bits, end, [](QRgb r) { return r == QColor(Qt::gray).rgb(); });
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPrivatePrintDrawUtil)
{
    pview_d->generatePreview();

    // 单页打印输出绘制函数
    QPixmap pixmap(pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution()).size());
    QPainter p(&pixmap);
    pixmap.fill(Qt::gray);

    // 无水印
    pview_d->printSinglePageDrawUtil(&p, {0, 0}, {0, 0}, QImage(), pview_d->pictures.first());
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);

    pview_d->waterMark->setText("测试水印");
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setRotation(80);
    pview_d->waterMark->setLayoutType(WaterMark::Center);
    pview_d->waterMark->setColor(Qt::yellow);

    //带水印
    pview_d->printSinglePageDrawUtil(&p, {0, 0}, {0, 0}, pview_d->generateWaterMarkImage(), pview_d->pictures.first());
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 多页打印输出绘制函数
    pixmap.fill(Qt::gray);
    pview_d->imposition = DPrintPreviewWidget::OneRowTwoCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();

    // 无水印
    pview_d->printMultiPageDrawUtil(&p, {0, 0}, QImage());
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);
    pview_d->displayWaterMarkItem();

    // 带水印
    const QImage &water = pview_d->generateWaterMarkImage();
    ASSERT_FALSE(water.isNull());
    pview_d->printMultiPageDrawUtil(&p, {0, 0}, water);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pview_d->scale = 1.25;
    pixmap.fill(Qt::gray);

    // 无水印 带缩放
    pview_d->printSinglePageDrawUtil(&p, {0, 0}, {0, 0}, {}, pview_d->pictures.back());
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);
    // 水印 带缩放
    pview_d->imposition = DPrintPreviewWidget::One;
    pview_d->printSinglePageDrawUtil(&p, {0, 0}, {0, 0}, pview_d->generateWaterMarkImage(), pview_d->pictures.back());
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);
    pview_d->imposition = DPrintPreviewWidget::FourRowFourCol;
    pview_d->calculateNumberUpPage();
    pview_d->calculateCurrentNumberPage();
    // 多页打印 缩放 无水印
    pview_d->printMultiPageDrawUtil(&p, {0, 0}, QImage());
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);
    pview_d->displayWaterMarkItem();

    // 带水印 缩放
    const QImage &water1 = pview_d->generateWaterMarkImage();
    ASSERT_FALSE(water1.isNull());
    pview_d->printMultiPageDrawUtil(&p, {0, 0}, water);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pview_d->releaseImpositionData();

    // 测试按照文件路径打印是否成功
    pview_d->pageRangeMode = DPrintPreviewWidget::CurrentPage;
    ASSERT_FALSE(pview_d->printerOptions().isEmpty());
    pview_d->pageRangeMode = DPrintPreviewWidget::SelectPage;
    pview_d->pageRange = {1, 2};
    ASSERT_FALSE(pview_d->printerOptions().isEmpty());
    pview_d->previewPrinter->setDuplex(DPrinter::DuplexAuto);
    pview_d->previewPrinter->setColorMode(DPrinter::GrayScale);
    ASSERT_FALSE(pview_d->printerOptions().isEmpty());
    pview_d->previewPrinter->setDuplex(DPrinter::DuplexLongSide);
    ASSERT_FALSE(pview_d->printerOptions().isEmpty());
    pview_d->previewPrinter->setDuplex(DPrinter::DuplexShortSide);
    ASSERT_FALSE(pview_d->printerOptions().isEmpty());
    pview_d->previewPrinter->setDuplex(DPrinter::DuplexNone);
    pview_d->previewPrinter->setColorMode(DPrinter::Color);
    ASSERT_FALSE(pview_d->printerOptions().isEmpty());

    if (pview_d->previewPrinter->outputFormat() == DPrinter::NativeFormat) {
        // 测试正常打印机能否正常输出打印
        pview_d->printByCups();
    }

    enum { TestPageCount = 3 };
    pview_d->pageCopyCount = TestPageCount;
    pview_d->isFirstPage = false;
    pview_d->isAsynPreview = true;
    QVector<int> testPages = pview_d->pageRange;
    pview_d->updatePageByPagePrintVector(pview_d->pageRange, pview_d->pictures);
    ASSERT_NE(testPages, pview_d->pageRange);
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testWaterItem)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    pview_d->waterMark->setText("test watermark!");
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setRotation(80);
    pview_d->waterMark->setLayoutType(WaterMark::Center);
    pview_d->waterMark->setColor(Qt::yellow);
    pview_d->waterMark->setFont(pview_d->q_func()->font());

    QRectF paperRect = pview_d->previewPrinter->pageLayout().fullRectPixels(pview_d->previewPrinter->resolution());
    pview_d->waterMark->setBoundingRect(paperRect);
    ASSERT_EQ(pview_d->waterMark->brect, paperRect);
    ASSERT_FALSE(pview_d->waterMark->twoPolygon.isEmpty());
    ASSERT_FALSE(pview_d->waterMark->boundingRect().isEmpty());
    ASSERT_FALSE(pview_d->waterMark->itemMaxPolygon().isEmpty());
    ASSERT_FALSE(pview_d->waterMark->itemMaxPolygon().isEmpty());
    ASSERT_FALSE(pview_d->waterMark->itemClipPath().isEmpty());

    // 测试绘制函数是否能绘制出数据
    QPixmap pixmap(paperRect.size().toSize());
    QPainter painter(&pixmap);
    pixmap.fill(Qt::gray);

    pview_d->waterMark->paint(&painter, nullptr, nullptr);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);
    pview_d->waterMark->updatePicture(&painter, true);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 测试平铺文字 并打属性是否正常
    pview_d->waterMark->setLayoutType(WaterMark::Tiled);
    pview_d->q_func()->setImposition(DPrintPreviewWidget::TwoRowTwoCol);
    pview_d->q_func()->setScale(0.5);
    pview_d->numberUpPrintData->waterList.first()->setScaleFactor(pview_d->numberUpPrintData->scaleRatio);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.length(), 4);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->mScaleFactor, pview_d->numberUpPrintData->scaleRatio);
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->text, "test watermark!");
    ASSERT_FALSE(pview_d->numberUpPrintData->waterList.first()->itemClipPath().isEmpty());

    // 测试paint函数
    pixmap.fill(Qt::gray);
    pview_d->numberUpPrintData->waterList.first()->paint(&painter, nullptr, nullptr);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 测试updatePicture函数
    pixmap.fill(Qt::gray);
    pview_d->numberUpPrintData->waterList.first()->updatePicture(&painter, true);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 平铺图片 并打数据
    QImage testImage({40, 40}, QImage::Format_ARGB32);
    testImage.fill(Qt::darkCyan);

    pview_d->numberUpPrintData->waterList.first()->setImage(testImage);
    pixmap.fill(Qt::gray);
    pview_d->numberUpPrintData->waterList.first()->updatePicture(&painter, true);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 并打图片 居中模式
    pview_d->numberUpPrintData->waterList.first()->setLayoutType(WaterMark::Center);
    pixmap.fill(Qt::gray);
    pview_d->numberUpPrintData->waterList.first()->updatePicture(&painter, true);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    pixmap.fill(Qt::gray);
    QStyleOptionGraphicsItem item;
    item.exposedRect = pview_d->numberUpPrintData->numberItem->boundingRect();
    pview_d->numberUpPrintData->numberItem->paint(&painter, &item, nullptr);
    ASSERT_TRUE(testPixmapHasData(pixmap));
}

TEST_F(ut_DPrintPreviewWidgetPrivate, testPageItem)
{
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    ASSERT_FALSE(pview_d->pages.isEmpty());
    PageItem *item = dynamic_cast<PageItem *>(pview_d->pages.first());

    ASSERT_TRUE(item);
    ASSERT_EQ(item->boundingRect(), item->brect);
    ASSERT_EQ(item->pageNumber(), item->pageNumber());

    QPixmap pixmap(item->boundingRect().size().toSize());
    QPainter painter(&pixmap);
    pixmap.fill(Qt::gray);

    // Page Item 没有绘制操作  只测试是否崩溃
    QStyleOptionGraphicsItem option;
    option.exposedRect = item->boundingRect();
    item->paint(&painter, &option, nullptr);

    ContentItem *content = item->content;
    ASSERT_TRUE(content);
    ASSERT_EQ(content->boundingRect(), content->brect);
    content->setRect(QRectF({10, 10}, content->boundingRect().size()));
    ASSERT_EQ(content->pos(), QPointF(10, 10));

    pixmap.fill(Qt::gray);
    // Content Item 绘制函数是否存在数据
    QStyleOptionGraphicsItem contentoption;
    contentoption.exposedRect = content->boundingRect();
    content->paint(&painter, &contentoption, nullptr);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    content->updateGrayContent();
    ASSERT_TRUE(content->grayPicture.size() > 0);

    // 非并打测试绘制函数
    pixmap.fill(Qt::gray);
    content->drawNumberUpPictures(&painter);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 并打测试绘制函数
    pview_d->q_func()->setImposition(DPrintPreviewWidget::TwoRowTwoCol);
    pixmap.fill(Qt::gray);
    content->drawNumberUpPictures(&painter);
    ASSERT_TRUE(testPixmapHasData(pixmap));

    // 测试灰度转换是否正常
    QPicture pic = content->grayscalePaint(*pview_d->pictures.first());
    ASSERT_TRUE(pic.size() > 0);
    QImage origin(QSize(40, 40), QImage::Format_ARGB32);
    origin.fill(Qt::yellow);

    ASSERT_TRUE(content->imageGrayscale(&origin).isGrayscale());
}

TEST_F(ut_DPrintPreviewWidgetPrivate, graphicsViewEvent)
{
    // 测试GraphicsView类中的鼠标事件是否正常
    GraphicsView *graphicsView = pview_d->graphicsView;
    QWheelEvent wheUpEvent({0, 0}, 15, Qt::NoButton, Qt::NoModifier);

    graphicsView->wheelEvent(&wheUpEvent);
    ASSERT_EQ(graphicsView->scaleRatio, 1 * PREVIEW_ENLARGE_RATIO);

    QMouseEvent pressEvent(QMouseEvent::MouseButtonPress, {10, 10}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    graphicsView->mousePressEvent(&pressEvent);
    ASSERT_EQ(graphicsView->dragMode(), GraphicsView::ScrollHandDrag);

    QMouseEvent releaseEvent(QMouseEvent::MouseButtonRelease, {20, 20}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    graphicsView->mouseReleaseEvent(&releaseEvent);
    ASSERT_EQ(graphicsView->dragMode(), GraphicsView::NoDrag);
    graphicsView->resetScale(false);
    ASSERT_EQ(graphicsView->scaleRatio, 1);

    QWheelEvent wheDownEvent({0, 0}, -15, Qt::NoButton, Qt::NoModifier);

    graphicsView->wheelEvent(&wheDownEvent);
    ASSERT_EQ(graphicsView->scaleRatio, 1 * PREVIEW_NARROW_RATIO);

    QTest::mouseClick(graphicsView->scaleResetButton, Qt::LeftButton, Qt::NoModifier);
    QTest::qWait(DELAY_TIME);
    ASSERT_EQ(graphicsView->scaleRatio, 1);
}

class ut_DPrintPreviewWidgetPrivateParam : public testing::TestWithParam<int>
{
protected:
    void SetUp() override;
    void TearDown() override;

    TestPrintPreviewDialog *printDialog = nullptr;
    DPrintPreviewWidgetPrivate *pview_d = nullptr;
};

INSTANTIATE_TEST_CASE_P(DPrintPreviewWidgetPrivate, ut_DPrintPreviewWidgetPrivateParam, ::testing::Range(1, 5));

void ut_DPrintPreviewWidgetPrivateParam::SetUp()
{
    printDialog = new TestPrintPreviewDialog;
    DPrintPreviewWidget *pview = printDialog->findChild<DPrintPreviewWidget *>();
    pview_d = pview->d_func();

    ASSERT_TRUE(pview_d);
}

void ut_DPrintPreviewWidgetPrivateParam::TearDown()
{
    delete printDialog;
}

TEST_P(ut_DPrintPreviewWidgetPrivateParam, TestPrivateNumberUpData)
{
    // 显示界面
    printDialog->show();
    QVERIFY(QTest::qWaitForWindowExposed(printDialog));

    int param = GetParam();
    pview_d->waterMark->setText("测试水印");
    pview_d->waterMark->setType(WaterMark::Text);
    pview_d->waterMark->setRotation(80);
    pview_d->waterMark->setLayoutType(WaterMark::Center);
    pview_d->waterMark->setColor(Qt::yellow);

    // 设置并打模式  让并打数据能够正常创建
    pview_d->q_func()->setImposition(DPrintPreviewWidget::Imposition(param));
    pview_d->q_func()->setOrder(DPrintPreviewWidget::Order(param - 1));

    ASSERT_TRUE(pview_d->numberUpPrintData);
    ASSERT_FALSE(pview_d->numberUpPrintData->updatePositions(1.39).isEmpty());
    pview_d->scale = 1.59;
    ASSERT_EQ(pview_d->numberUpPrintData->updatePositions(1.39).length(), pview_d->numberUpPrintData->rowCount * pview_d->numberUpPrintData->columnCount);

    // 测试函数能否正常执行
    pview_d->numberUpPrintData->setWaterMarksScale(1.45);

    //  测试拷贝数据是否正常
    pview_d->numberUpPrintData->copyWaterMarkProperties();
    ASSERT_TRUE(pview_d->numberUpPrintData->waterProperty);
    ASSERT_EQ(pview_d->numberUpPrintData->waterProperty->type, WaterMark::Text);
    ASSERT_EQ(pview_d->numberUpPrintData->waterProperty->text, "测试水印");
    ASSERT_EQ(pview_d->numberUpPrintData->waterProperty->rotation, 80);
    ASSERT_EQ(pview_d->numberUpPrintData->waterProperty->layout, WaterMark::Center);
    ASSERT_EQ(pview_d->numberUpPrintData->waterProperty->color, Qt::yellow);

    // 测试水印能否正常设置
    WaterMark *wm = new WaterMark;
    pview_d->numberUpPrintData->setWaterMarkOriginProperties(wm);
    ASSERT_EQ(wm->getType(), WaterMark::Text);
    ASSERT_EQ(wm->text, "测试水印");
    ASSERT_EQ(wm->rotation(), 80);
    ASSERT_EQ(wm->layout, WaterMark::Center);
    ASSERT_EQ(wm->color, Qt::yellow);
    delete wm;

    // 测试函数能否正常调用
    pview_d->numberUpPrintData->updateWaterMarks();
    pview_d->numberUpPrintData->setWaterMarkProperty([](WaterMark *wm) { wm->setText("change the text"); });
    ASSERT_EQ(pview_d->numberUpPrintData->waterList.first()->text, "change the text");

    pview_d->numberUpPrintData->resetData();
    ASSERT_EQ(pview_d->numberUpPrintData->scaleRatio, 1);
    ASSERT_EQ(pview_d->numberUpPrintData->rowCount, 0);
    ASSERT_EQ(pview_d->numberUpPrintData->columnCount, 0);
    ASSERT_EQ(pview_d->numberUpPrintData->pageStartPoint, QPointF(0, 0));
}

// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>

#include <QSignalSpy>
#include <DIconButton>
#include <DSuggestButton>
#include <DLineEdit>
#include <QButtonGroup>
#include <DSwitchButton>
#include <DFileChooserEdit>
#include <DSpinBox>
#include <DSlider>
#include <DPaletteHelper>
#include "dprintpreviewwidget.h"

#include "dprintpreviewdialog.h"
#include "dprintpreviewdialog_p.h"
#include "dprintpreviewwidget.h"
#include "dprintpreviewwidget_p.h"
#include "dlineedit_p.h"
#include "dalertcontrol_p.h"

#define MAX_PAGE_VALUE 4
#define NORMAL_MODERATE_TOP_BOTTRM 25.4
#define NORMAL_LEFT_RIGHT 31.8
#define MODERATE_LEFT_RIGHT 19.1

DWIDGET_USE_NAMESPACE

class TestPrintPreviewDialog : public DPrintPreviewDialog
{
public:
    TestPrintPreviewDialog(QWidget *parent = nullptr)
        : DPrintPreviewDialog(parent)
    {
        connect(this, QOverload<DPrinter *>::of(&TestPrintPreviewDialog::paintRequested), this, &TestPrintPreviewDialog::slotPaintRequest);
    }

protected Q_SLOTS:
    void slotPaintRequest(DPrinter *printer);
};

void TestPrintPreviewDialog::slotPaintRequest(DPrinter *printer)
{
    QPainter painter(printer);

    for (int i = 0; i < MAX_PAGE_VALUE; ++i) {
        if (i != 0)
            printer->newPage();

        painter.drawText(printer->pageRect().center(), QStringLiteral("Page %1").arg(i + 1));
    }
}

class ut_DPrintPreviewDialog : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    TestPrintPreviewDialog *testPrintDialog;
    DPrintPreviewDialogPrivate *test_dialog_d;
};

void ut_DPrintPreviewDialog::SetUp()
{
    testPrintDialog = new TestPrintPreviewDialog;
    testPrintDialog->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(testPrintDialog));
    test_dialog_d = testPrintDialog->d_func();
    test_dialog_d->printer->setOutputFormat(DPrinter::PdfFormat);
    ASSERT_TRUE(test_dialog_d);
}

void ut_DPrintPreviewDialog::TearDown()
{
    delete testPrintDialog;
}

TEST_F(ut_DPrintPreviewDialog, testForInit)
{
    // 测试保存PDF文件名称接口
    testPrintDialog->setDocName("test");
    ASSERT_STREQ(testPrintDialog->docName().toLocal8Bit(), "test");

    // 测试保存PDF文件路径接口
    testPrintDialog->setPrintFromPath(":/test.pdf");
    ASSERT_STREQ(testPrintDialog->printFromPath().toLocal8Bit(), ":/test.pdf");

    // 测试参数初始化
    ASSERT_TRUE(test_dialog_d->printer);
    ASSERT_EQ(test_dialog_d->printer->orientation(), QPrinter::Portrait);
    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::A4);
    ASSERT_EQ(test_dialog_d->printer->paperName(), "A4");

    ASSERT_EQ(test_dialog_d->totalPages, 4);
    ASSERT_EQ(test_dialog_d->fontSizeMore, false);
    ASSERT_EQ(test_dialog_d->lastPageRange, "");
    ASSERT_EQ(test_dialog_d->supportedDuplexFlag, false);
    ASSERT_EQ(test_dialog_d->supportedColorMode, true);
    ASSERT_EQ(test_dialog_d->isInited, true);
    ASSERT_EQ(test_dialog_d->strLengths, 0);
    ASSERT_EQ(test_dialog_d->isOnFocus, false);
    ASSERT_EQ(test_dialog_d->lastCusWatermarkText, "");
    ASSERT_EQ(test_dialog_d->isChecked, false);
    ASSERT_EQ(test_dialog_d->isEventHide, false);
    ASSERT_EQ(test_dialog_d->isInitBtnColor, true);
    ASSERT_EQ(test_dialog_d->waterColor, QColor("#6f6f6f"));
    ASSERT_EQ(test_dialog_d->typeChoice, 0);
    ASSERT_EQ(test_dialog_d->directChoice, 0);

    ASSERT_EQ(test_dialog_d->colorModeCombo->count(), 2);
    ASSERT_EQ(test_dialog_d->paperSizeCombo->count(), 7);
    ASSERT_EQ(test_dialog_d->paperSizeCombo->currentIndex(), 1);

    // 测试控件初始化
    ASSERT_TRUE(test_dialog_d->pview);
    ASSERT_TRUE(test_dialog_d->back);
    ASSERT_TRUE(test_dialog_d->basicsettingwdg);
    ASSERT_TRUE(test_dialog_d->advancesettingwdg);
    ASSERT_TRUE(test_dialog_d->watermarksettingwdg);
    ASSERT_TRUE(test_dialog_d->textWatermarkWdg);
    ASSERT_TRUE(test_dialog_d->picWatermarkWdg);
    ASSERT_TRUE(test_dialog_d->firstBtn);
    ASSERT_TRUE(test_dialog_d->prevPageBtn);
    ASSERT_TRUE(test_dialog_d->jumpPageEdit);
    ASSERT_TRUE(test_dialog_d->totalPageLabel);
    ASSERT_TRUE(test_dialog_d->originTotalPageLabel);
    ASSERT_TRUE(test_dialog_d->nextPageBtn);
    ASSERT_TRUE(test_dialog_d->lastBtn);
    ASSERT_TRUE(test_dialog_d->cancelBtn);
    ASSERT_TRUE(test_dialog_d->printBtn);
    ASSERT_TRUE(test_dialog_d->advanceBtn);
    ASSERT_TRUE(test_dialog_d->scrollarea);
    ASSERT_TRUE(test_dialog_d->printDeviceCombo);
    ASSERT_TRUE(test_dialog_d->copycountspinbox);
    ASSERT_TRUE(test_dialog_d->pageRangeCombo);
    ASSERT_TRUE(test_dialog_d->pageRangeEdit);
    ASSERT_TRUE(test_dialog_d->orientationgroup);
    ASSERT_TRUE(test_dialog_d->colorModeCombo);
    ASSERT_TRUE(test_dialog_d->marginsCombo);
    ASSERT_TRUE(test_dialog_d->marginTopSpin);
    ASSERT_TRUE(test_dialog_d->marginBottomSpin);
    ASSERT_TRUE(test_dialog_d->marginLeftSpin);
    ASSERT_TRUE(test_dialog_d->marginRightSpin);
    ASSERT_TRUE(test_dialog_d->scaleGroup);
    ASSERT_TRUE(test_dialog_d->scaleRateEdit);
    ASSERT_TRUE(test_dialog_d->duplexCombo);
    ASSERT_TRUE(test_dialog_d->duplexCheckBox);
    ASSERT_TRUE(test_dialog_d->paperSizeCombo);
    ASSERT_TRUE(test_dialog_d->paperSizeCombo);
    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo);
    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox);
    ASSERT_TRUE(test_dialog_d->directGroup);
    ASSERT_TRUE(test_dialog_d->printOrderGroup);
    ASSERT_TRUE(test_dialog_d->inorderCombo);
    ASSERT_TRUE(test_dialog_d->waterTextCombo);
    ASSERT_TRUE(test_dialog_d->waterColorBtn);
    ASSERT_TRUE(test_dialog_d->waterTextEdit);
    ASSERT_TRUE(test_dialog_d->fontCombo);
    ASSERT_TRUE(test_dialog_d->waterMarkBtn);
    ASSERT_TRUE(test_dialog_d->picPathEdit);
    ASSERT_TRUE(test_dialog_d->waterTypeGroup);
    ASSERT_TRUE(test_dialog_d->waterPosCombox);
    ASSERT_TRUE(test_dialog_d->inclinatBox);
    ASSERT_TRUE(test_dialog_d->waterSizeSlider);
    ASSERT_TRUE(test_dialog_d->sizeBox);
    ASSERT_TRUE(test_dialog_d->wmOpaSlider);
    ASSERT_TRUE(test_dialog_d->opaBox);
    ASSERT_EQ(test_dialog_d->marginOldValue.length(), 8);
    for (QVector<qreal>::iterator iter = test_dialog_d->marginOldValue.begin(); iter < test_dialog_d->marginOldValue.end(); iter++) {
        ASSERT_EQ(*iter, 0);
    }
    ASSERT_TRUE(test_dialog_d->spacer);
    ASSERT_TRUE(test_dialog_d->wmSpacer);
    ASSERT_TRUE(test_dialog_d->colorWidget);
    ASSERT_TRUE(test_dialog_d->printInOrderRadio);
    ASSERT_TRUE(test_dialog_d->inorderwdg);
    ASSERT_TRUE(test_dialog_d->pickColorWidget);
    ASSERT_EQ(test_dialog_d->firstBtn->isEnabled(), false);
    ASSERT_EQ(test_dialog_d->prevPageBtn->isEnabled(), false);
    ASSERT_EQ(test_dialog_d->nextPageBtn->isEnabled(), true);
    ASSERT_EQ(test_dialog_d->lastBtn->isEnabled(), true);
    ASSERT_EQ(test_dialog_d->spinboxTextCaches.begin(), test_dialog_d->spinboxTextCaches.end());
}

TEST_F(ut_DPrintPreviewDialog, testPaintRequested)
{
    ASSERT_EQ(test_dialog_d->totalPages, 4);
    ASSERT_EQ(test_dialog_d->marginOldValue.length(), 8);
    ASSERT_EQ(test_dialog_d->firstBtn->isEnabled(), false);
    ASSERT_EQ(test_dialog_d->prevPageBtn->isEnabled(), false);
    ASSERT_EQ(test_dialog_d->nextPageBtn->isEnabled(), true);
    ASSERT_EQ(test_dialog_d->lastBtn->isEnabled(), true);
}

TEST_F(ut_DPrintPreviewDialog, testSetfrmaeback)
{
    DFrame *printerFrame = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(printerFrame);
    DPalette pa = DPaletteHelper::instance()->palette(printerFrame);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *copycountFrame = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(copycountFrame);
    pa = DPaletteHelper::instance()->palette(copycountFrame);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *pageFrame = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(pageFrame);
    pa = DPaletteHelper::instance()->palette(pageFrame);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *colorframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(colorframe);
    pa = DPaletteHelper::instance()->palette(colorframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *marginsframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(marginsframe);
    pa = DPaletteHelper::instance()->palette(marginsframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *paperframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(paperframe);
    pa = DPaletteHelper::instance()->palette(paperframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *duplexframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(duplexframe);
    pa = DPaletteHelper::instance()->palette(duplexframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *sidebysideframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(sidebysideframe);
    pa = DPaletteHelper::instance()->palette(sidebysideframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *watermarkframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(watermarkframe);
    pa = DPaletteHelper::instance()->palette(watermarkframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *posframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(posframe);
    pa = DPaletteHelper::instance()->palette(posframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *inclinatframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(inclinatframe);
    pa = DPaletteHelper::instance()->palette(inclinatframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *sizeframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(sizeframe);
    pa = DPaletteHelper::instance()->palette(sizeframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *opaframe = new DFrame(test_dialog_d->basicsettingwdg);
    test_dialog_d->setfrmaeback(opaframe);
    pa = DPaletteHelper::instance()->palette(opaframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());
}

TEST_F(ut_DPrintPreviewDialog, testAdvanceBtn)
{
    ASSERT_TRUE(test_dialog_d->advanceBtn->isEnabled());
    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isHidden());

    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());
}

TEST_F(ut_DPrintPreviewDialog, testPrintDeviceCombo)
{
    ASSERT_TRUE(test_dialog_d->printDeviceCombo->isEnabled());

    // printDeviceCombo 选中 Print to PDF
    ASSERT_STREQ(test_dialog_d->printDeviceCombo->currentText().toLocal8Bit(), "Print to PDF");

    ASSERT_FALSE(test_dialog_d->copycountspinbox->isEnabled());
    ASSERT_EQ(test_dialog_d->copycountspinbox->value(), 1);
    ASSERT_EQ(test_dialog_d->duplexCheckBox->checkState(), Qt::Unchecked);
    ASSERT_FALSE(test_dialog_d->duplexCheckBox->isEnabled());
    ASSERT_STREQ(test_dialog_d->duplexCombo->currentText().toLocal8Bit(), "");
    ASSERT_FALSE(test_dialog_d->duplexCombo->isEnabled());
    ASSERT_TRUE(test_dialog_d->waterColorBtn->isEnabled());
    ASSERT_EQ(test_dialog_d->colorModeCombo->currentIndex(), 0);
    ASSERT_FALSE(test_dialog_d->colorModeCombo->isEnabled());
    ASSERT_FALSE(test_dialog_d->paperSizeCombo->signalsBlocked());
    ASSERT_EQ(test_dialog_d->paperSizeCombo->currentIndex(), 1);

    test_dialog_d->printDeviceCombo->setCurrentText("Save as Image");

    // printDeviceCombo 选中 Save as Image
    ASSERT_STREQ(test_dialog_d->printDeviceCombo->currentText().toLocal8Bit(), "Save as Image");

    ASSERT_FALSE(test_dialog_d->copycountspinbox->isEnabled());
    ASSERT_EQ(test_dialog_d->copycountspinbox->value(), 1);
    ASSERT_EQ(test_dialog_d->duplexCheckBox->checkState(), Qt::Unchecked);
    ASSERT_FALSE(test_dialog_d->duplexCheckBox->isEnabled());
    ASSERT_STREQ(test_dialog_d->duplexCombo->currentText().toLocal8Bit(), "");
    ASSERT_FALSE(test_dialog_d->duplexCombo->isEnabled());
    ASSERT_TRUE(test_dialog_d->waterColorBtn->isEnabled());
    ASSERT_EQ(test_dialog_d->colorModeCombo->currentIndex(), 0);
    ASSERT_FALSE(test_dialog_d->colorModeCombo->isEnabled());
    ASSERT_FALSE(test_dialog_d->paperSizeCombo->signalsBlocked());
    ASSERT_EQ(test_dialog_d->paperSizeCombo->currentIndex(), 1);

    // printDeviceCombo 选中 Print to PDF
    test_dialog_d->pageRangeCombo->setCurrentText("Select pages");

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");

    test_dialog_d->printDeviceCombo->setCurrentText("Print to PDF");

    ASSERT_STREQ(test_dialog_d->printDeviceCombo->currentText().toLocal8Bit(), "Print to PDF");
    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "All");
}

TEST_F(ut_DPrintPreviewDialog, testCancelBtn)
{
    ASSERT_TRUE(test_dialog_d->cancelBtn->isEnabled());
    ASSERT_TRUE(testPrintDialog->isVisible());

    // 点击 cancel 按钮
    test_dialog_d->cancelBtn->click();

    ASSERT_TRUE(testPrintDialog->isHidden());
}

TEST_F(ut_DPrintPreviewDialog, testPageRangeCombo)
{
    ASSERT_TRUE(test_dialog_d->pageRangeCombo->isEnabled());

    // pageRangeCombo 选中 ALL
    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "All");
    ASSERT_EQ(test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_EQ(test_dialog_d->pageRangeEdit->lineEdit()->placeholderText().toLocal8Bit(), "");
    ASSERT_EQ(test_dialog_d->pview->pageRangeMode(),
              static_cast<DPrintPreviewWidget::PageRange>(test_dialog_d->pageRangeCombo->currentIndex()));
    ASSERT_TRUE(test_dialog_d->printBtn->isEnabled());
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    // pageRangeCombo 选中 Current page
    test_dialog_d->pageRangeCombo->setCurrentText("Current page");

    ASSERT_EQ(test_dialog_d->pageRangeCombo->currentIndex(), 1);
    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Current page");
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_EQ(test_dialog_d->pageRangeEdit->lineEdit()->placeholderText().toLocal8Bit(), "");
    ASSERT_EQ(test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_EQ(test_dialog_d->pview->pageRangeMode(),
              static_cast<DPrintPreviewWidget::PageRange>(test_dialog_d->pageRangeCombo->currentIndex()));
    ASSERT_TRUE(test_dialog_d->printBtn->isEnabled());
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    // pageRangeCombo 选中 Select pages
    test_dialog_d->pageRangeCombo->setCurrentText("Select pages");

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");
    ASSERT_TRUE(test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_EQ(test_dialog_d->pageRangeEdit->lineEdit()->placeholderText().toLocal8Bit(), "For example, 1,3,5-7,11-15,18,21");
    ASSERT_EQ(test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_EQ(test_dialog_d->pview->pageRangeMode(),
              static_cast<DPrintPreviewWidget::PageRange>(test_dialog_d->pageRangeCombo->currentIndex()));
    ASSERT_FALSE(test_dialog_d->printBtn->isEnabled());
    ASSERT_TRUE(test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    test_dialog_d->pageRangeEdit->lineEdit()->setFocus();
    test_dialog_d->pageRangeEdit->lineEdit()->setText("2-3");
    test_dialog_d->pageRangeEdit->lineEdit()->clearFocus();

    ASSERT_STREQ(test_dialog_d->totalPageLabel->text().toLocal8Bit(), "2");

    // pageRangeCombo 再次选中 All
    test_dialog_d->pageRangeCombo->setCurrentText("All");

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "All");
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_STREQ(test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_TRUE(test_dialog_d->printBtn->isEnabled());
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    // pageRangeCombo 再次选中 Select pages
    test_dialog_d->pageRangeCombo->setCurrentText("Select pages");

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");
    ASSERT_TRUE(test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_STREQ(test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "2-3");
    ASSERT_TRUE(test_dialog_d->printBtn->isEnabled());
    ASSERT_FALSE(test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(test_dialog_d->totalPageLabel->text().toLocal8Bit(), "2");

    // 清除焦点，防止 TearDown 调用槽函数
    if (test_dialog_d->pageRangeEdit->lineEdit()->hasFocus())
        test_dialog_d->pageRangeEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->pageRangeEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testMarginsCombo)
{
    ASSERT_TRUE(test_dialog_d->marginsCombo->isEnabled());

    // marginsCombo 选中 Narrow(mm)
    ASSERT_STREQ(test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Narrow (mm)");
    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().top());
    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().left());
    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().right());
    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().bottom());

    // marginsCombo 选中 Normal(mm)
    test_dialog_d->marginsCombo->setCurrentText("Normal (mm)");

    ASSERT_STREQ(test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Normal (mm)");
    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), NORMAL_MODERATE_TOP_BOTTRM);
    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), NORMAL_LEFT_RIGHT);
    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), NORMAL_LEFT_RIGHT);
    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), NORMAL_MODERATE_TOP_BOTTRM);

    // marginsCombo 选中 Moderate(mm)
    test_dialog_d->marginsCombo->setCurrentText("Moderate (mm)");

    ASSERT_STREQ(test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Moderate (mm)");
    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), NORMAL_MODERATE_TOP_BOTTRM);
    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), MODERATE_LEFT_RIGHT);
    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), MODERATE_LEFT_RIGHT);
    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), NORMAL_MODERATE_TOP_BOTTRM);

    // marginsCombo 选中 Customize(mm)
    test_dialog_d->marginsCombo->setCurrentText("Customize (mm)");

    ASSERT_STREQ(test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Customize (mm)");
    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().top());
    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().left());
    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().right());
    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), test_dialog_d->printer->pageLayout().minimumMargins().bottom());
}

TEST_F(ut_DPrintPreviewDialog, testPrintBtn)
{
    ASSERT_TRUE(test_dialog_d->printBtn->isEnabled());
//    QTest::mouseClick(test_dialog_d->printBtn, Qt::LeftButton, Qt::NoModifier);
}

TEST_F(ut_DPrintPreviewDialog, testWaterColorBtn)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    ASSERT_TRUE(test_dialog_d->waterColorBtn->isEnabled());
    ASSERT_TRUE(test_dialog_d->colorWidget->isHidden());

    // 显示 PrintPickColorWidget
    test_dialog_d->_q_colorButtonCliked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->colorWidget));

    ASSERT_TRUE(test_dialog_d->colorWidget->isVisible());
}

TEST_F(ut_DPrintPreviewDialog, testColorModeCombo)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

//    pdf mode cannot be reached
//    ASSERT_FALSE(test_dialog_d->colorModeCombo->isEnabled());
//    ASSERT_EQ(test_dialog_d->colorModeCombo->count(), 2);
//    ASSERT_EQ(test_dialog_d->colorModeCombo->currentText(), "Color");
//    // TODO: colorModeCombo 选中 Color

//    test_dialog_d->colorModeCombo->setCurrentIndex(1);
//    ASSERT_EQ(test_dialog_d->colorModeCombo->currentText(), "Grayscale");
//    // TODO: colorModeCombo 选中 Grayscale
}

TEST_F(ut_DPrintPreviewDialog, testOrientationgroup)
{
    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();
    Q_FOREACH (QAbstractButton *button, test_dialog_d->orientationgroup->buttons()) {
        ASSERT_TRUE(button->isEnabled());
    }

    // orientationgroup 选中 printer_portrait
    test_dialog_d->_q_orientationChanged(0);

    ASSERT_EQ(test_dialog_d->orientationgroup->checkedId(), 0);
    ASSERT_EQ(pview_d->previewPrinter->orientation(), DPrinter::Portrait);

    // orientationgroup 选中 printer_landscape
    test_dialog_d->orientationgroup->button(1)->click();

    ASSERT_EQ(test_dialog_d->orientationgroup->checkedId(), 1);
    ASSERT_EQ(pview_d->previewPrinter->orientation(), DPrinter::Landscape);
    ASSERT_FALSE(test_dialog_d->originTotalPageLabel->isVisible());

    // 测试改变 orientationgroup 后，pageRangeCombo 是否为All
    test_dialog_d->pageRangeCombo->setCurrentText("Select pages");

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");

    // 开启并列打印
    test_dialog_d->sidebysideCheckBox->setChecked(true);

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo->isEnabled());

    test_dialog_d->_q_orientationChanged(1);

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "All");
    ASSERT_TRUE(test_dialog_d->originTotalPageLabel->isVisible());
}

TEST_F(ut_DPrintPreviewDialog, testWaterTextCombo)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();
    ASSERT_TRUE(test_dialog_d->waterTextCombo->isEnabled());

    // waterTextCombo 选中 Confidential
    ASSERT_STREQ(test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Confidential");
    ASSERT_FALSE(test_dialog_d->waterTextEdit->isEnabled());
    ASSERT_STREQ(test_dialog_d->waterTextEdit->text().toLocal8Bit(), "");
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), "Confidential");

    // waterTextCombo 选中 Draft
    test_dialog_d->waterTextCombo->setCurrentText("Draft");

    ASSERT_STREQ(test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Draft");
    ASSERT_FALSE(test_dialog_d->waterTextEdit->isEnabled());
    ASSERT_STREQ(test_dialog_d->waterTextEdit->text().toLocal8Bit(), "");
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), "Draft");

    // waterTextCombo 选中 Sample
    test_dialog_d->waterTextCombo->setCurrentText("Sample");

    ASSERT_STREQ(test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Sample");
    ASSERT_FALSE(test_dialog_d->waterTextEdit->isEnabled());
    ASSERT_STREQ(test_dialog_d->waterTextEdit->text().toLocal8Bit(), "");
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), "Sample");

    // waterTextCombo 选中 Custom
    test_dialog_d->waterTextCombo->setCurrentText("Custom");

    ASSERT_STREQ(test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Custom");
    ASSERT_TRUE(test_dialog_d->waterTextEdit->isEnabled());
}

TEST_F(ut_DPrintPreviewDialog, testInorderCombo)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_FALSE(test_dialog_d->inorderCombo->isEnabled());

    // inorderCombo 选中 Front to back
    ASSERT_EQ(test_dialog_d->inorderCombo->currentText().toLocal8Bit(), "Front to back");

    // inorderCombo 选中 Back to front
    test_dialog_d->inorderCombo->setCurrentText("Back to front");

    ASSERT_STREQ(test_dialog_d->inorderCombo->currentText().toLocal8Bit(), "Back to front");
}

TEST_F(ut_DPrintPreviewDialog, testWaterTextEdit)
{
    QLatin1String testStr("Test");
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    // waterTextCombo 选中 Custom
    test_dialog_d->waterTextCombo->setCurrentText("Custom");

    ASSERT_STREQ(test_dialog_d->waterTextEdit->text().toLocal8Bit(), "");

    // 测试 waterTextEdit
    test_dialog_d->waterTextEdit->setText(testStr);
    test_dialog_d->_q_customTextWatermarkFinished();

    ASSERT_STREQ(test_dialog_d->lastCusWatermarkText.toLocal8Bit(), "Test");

    // waterTextCombo 选中 Confidential
    test_dialog_d->waterTextCombo->setCurrentText("Confidential");

    ASSERT_STREQ(test_dialog_d->waterTextEdit->text().toLocal8Bit(), "");

    // waterTExtCombo 选中 Custom
    test_dialog_d->waterTextCombo->setCurrentText("Custom");

    ASSERT_STREQ(test_dialog_d->lastCusWatermarkText.toLocal8Bit(), "Test");
    ASSERT_STREQ(test_dialog_d->waterTextEdit->text().toLocal8Bit(), "Test");
    ASSERT_STREQ(pview_d->waterMark->text.toLocal8Bit(), "Test");

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->waterTextEdit->lineEdit()->hasFocus())
        test_dialog_d->waterTextEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->waterTextEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testPagePerSheetCombo)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    ASSERT_FALSE(test_dialog_d->pagePerSheetCombo->isEnabled());
    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isEnabled());

    // 开启并列打印
    test_dialog_d->sidebysideCheckBox->setChecked(true);

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo->isEnabled());

    // pagePerSheetCombo 选中 2 pages/sheet, 1×2
    ASSERT_STREQ(test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "2 pages/sheet, 1×2");
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::OneRowTwoCol);

    // pagePerSheetCombo 选中 4 pages/sheet, 2×2
    test_dialog_d->pagePerSheetCombo->setCurrentText("4 pages/sheet, 2×2");

    ASSERT_STREQ(test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "4 pages/sheet, 2×2");
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::TwoRowTwoCol);

    // pagePerSheetCombo 选中 6 pages/sheet, 2×3
    test_dialog_d->pagePerSheetCombo->setCurrentText("6 pages/sheet, 2×3");

    ASSERT_STREQ(test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "6 pages/sheet, 2×3");
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::TwoRowThreeCol);

    // pagePerSheetCombo 选中 9 pages/sheet, 3×3
    test_dialog_d->pagePerSheetCombo->setCurrentText("9 pages/sheet, 3×3");

    ASSERT_STREQ(test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "9 pages/sheet, 3×3");
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::ThreeRowThreeCol);

    // pagePerSheetCombo 选中 16 pages/sheet, 4×4
    test_dialog_d->pagePerSheetCombo->setCurrentText("16 pages/sheet, 4×4");

    ASSERT_STREQ(test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "16 pages/sheet, 4×4");
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::FourRowFourCol);
}

TEST_F(ut_DPrintPreviewDialog, testPicPathEdit)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    test_dialog_d->waterTypeGroup->button(1)->click();

    ASSERT_TRUE(test_dialog_d->picPathEdit->isEnabled());

    // picPathEdit 设置图片路径
    test_dialog_d->picPathEdit->setFocus();
    test_dialog_d->picPathEdit->setText(":/test.png");
    test_dialog_d->picPathEdit->clearFocus();

    test_dialog_d->waterMarkBtnClicked(false);
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    // 开启并列打印
    test_dialog_d->sidebysideCheckBox->setChecked(true);

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo->isEnabled());

    // pagePerSheetCombo 选中 2 pages/sheet, 1×2
    ASSERT_STREQ(test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "2 pages/sheet, 1×2");
    ASSERT_EQ(pview_d->imposition, DPrintPreviewWidget::OneRowTwoCol);

    Q_EMIT test_dialog_d->picPathEdit->fileChoosed(":/test.png");
}

TEST_F(ut_DPrintPreviewDialog, testSizeBox)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_FALSE(test_dialog_d->sizeBox->isHidden());

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_TRUE(test_dialog_d->sizeBox->isEnabled());
    ASSERT_FALSE(test_dialog_d->sizeBox->isHidden());
//    ASSERT_TRUE(test_dialog_d->sizeBox->isVisible());

    ASSERT_EQ(test_dialog_d->sizeBox->value(), 100);

    // sizeBox 测试 waterSizeSlider 值的改变
    test_dialog_d->sizeBox->setValue(30);

    ASSERT_STREQ(test_dialog_d->sizeBox->text().toLocal8Bit(), "30%");

    ASSERT_EQ(test_dialog_d->waterSizeSlider->value(), 30);
}

TEST_F(ut_DPrintPreviewDialog, testOpaBox)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    ASSERT_EQ(test_dialog_d->opaBox->value(), 30);

    // opaBox 测试 wmOpaSlider 值的改变
    test_dialog_d->wmOpaSlider->setValue(50);

    ASSERT_STREQ(test_dialog_d->opaBox->text().toLocal8Bit(), "50%");
    ASSERT_EQ(test_dialog_d->wmOpaSlider->value(), 50);
}

TEST_F(ut_DPrintPreviewDialog, testFontCombo)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    ASSERT_TRUE(test_dialog_d->fontCombo->isEnabled());

    QString fontComboCurrentText = test_dialog_d->fontCombo->currentText();

    // 如果fontCombo 选中的不是最后一个，就向下选择一个
    if (test_dialog_d->fontCombo->currentIndex() < test_dialog_d->fontCombo->count() - 1)
        test_dialog_d->fontCombo->setCurrentIndex(test_dialog_d->fontCombo->currentIndex() + 1);
    // 如果fontCombo 选中的不是第一个，就向上选择一个
    else if (test_dialog_d->fontCombo->currentIndex() > 0)
        test_dialog_d->fontCombo->setCurrentIndex(test_dialog_d->fontCombo->currentIndex() - 1);

    if (test_dialog_d->fontCombo->count() > 1) {
        ASSERT_STRNE(fontComboCurrentText.toLocal8Bit(), test_dialog_d->fontCombo->currentText().toLocal8Bit());
        ASSERT_STREQ(test_dialog_d->fontCombo->currentText().toLocal8Bit(), pview_d->waterMark->font.toString().split(",")[0].toLocal8Bit());
    }
}

TEST_F(ut_DPrintPreviewDialog, testPickColorWidget)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    ASSERT_TRUE(test_dialog_d->colorWidget->isHidden());
    ASSERT_TRUE(test_dialog_d->colorWidget->isEnabled());

    // 显示 colorWidget 菜单
    test_dialog_d->_q_colorButtonCliked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->colorWidget));

    ASSERT_TRUE(test_dialog_d->colorWidget->isVisible());

    // 测试 pickColorWidget
    test_dialog_d->_q_selectColorButton(QColor("#f9a825"));
    ASSERT_EQ(test_dialog_d->waterColor, QColor("#f9a825"));
}

TEST_F(ut_DPrintPreviewDialog, testWaterPosCombox)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    // waterPosCombox 选中 Center
    test_dialog_d->waterPosCombox->setCurrentText("Center");

    ASSERT_STREQ(test_dialog_d->waterPosCombox->currentText().toLocal8Bit(), "Center");
    ASSERT_EQ(pview_d->waterMark->layout, WaterMark::Center);

    // waterPosCombox 选中 Tile
    test_dialog_d->waterPosCombox->setCurrentText("Tile");

    ASSERT_STREQ(test_dialog_d->waterPosCombox->currentText().toLocal8Bit(), "Tile");
    ASSERT_EQ(pview_d->waterMark->layout, WaterMark::Tiled);
}

TEST_F(ut_DPrintPreviewDialog, testDirectGroup)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    Q_FOREACH (QAbstractButton *button, test_dialog_d->directGroup->buttons()) {
        ASSERT_FALSE(button->isEnabled());
    }
    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isEnabled());

    test_dialog_d->sidebysideCheckBox->setChecked(true);

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isChecked());
    Q_FOREACH (QAbstractButton *button, test_dialog_d->directGroup->buttons()) {
        ASSERT_TRUE(button->isEnabled());
    }

    for (int i =0; i<test_dialog_d->directGroup->buttons().count(); i++) {
        // 切换并打顺序
        test_dialog_d->directGroup->button(i)->click();

        ASSERT_TRUE(test_dialog_d->directGroup->button(i)->isChecked());
        ASSERT_EQ(test_dialog_d->directChoice, i);
        ASSERT_EQ(pview_d->order, i);
    }
}

TEST_F(ut_DPrintPreviewDialog, testInclinatBox)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    // 测试 inclinatBox 初始值
    ASSERT_EQ(test_dialog_d->inclinatBox->value(), 30);
    ASSERT_STREQ(test_dialog_d->inclinatBox->text().toLocal8Bit(), "30°");
    ASSERT_EQ(pview_d->waterMark->rotation(), 30);

    test_dialog_d->inclinatBox->setFocus();
    test_dialog_d->inclinatBox->lineEdit()->setText("123");
    Q_EMIT test_dialog_d->inclinatBox->lineEdit()->textEdited("123°");
    test_dialog_d->inclinatBox->clearFocus();

    ASSERT_EQ(pview_d->waterMark->rotation(), 123);

    // 测试 _d_setSpinboxDefaultValue 函数
    test_dialog_d->inclinatBox->setFocus();
    test_dialog_d->inclinatBox->lineEdit()->setText("");
    Q_EMIT test_dialog_d->inclinatBox->lineEdit()->textEdited("");
    test_dialog_d->inclinatBox->clearFocus();

    ASSERT_STREQ(test_dialog_d->inclinatBox->text().toLocal8Bit(), "123°");
    ASSERT_EQ(pview_d->waterMark->rotation(), 123);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->inclinatBox->lineEdit()->hasFocus())
        test_dialog_d->inclinatBox->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->inclinatBox->lineEdit()->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testWaterSizeSlider)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    // 测试 waterSizeSlider 初始值
    ASSERT_EQ(test_dialog_d->sizeBox->value(), 100);
    ASSERT_EQ(test_dialog_d->waterSizeSlider->value(), 100);
    ASSERT_EQ(pview_d->waterMark->mScaleFactor, 1);

    test_dialog_d->waterSizeSlider->setValue(30);

    ASSERT_EQ(test_dialog_d->sizeBox->value(), 30);
    ASSERT_EQ(pview_d->waterMark->mScaleFactor, 0.3);
}

TEST_F(ut_DPrintPreviewDialog, testWmOpaSlider)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    // 测试 wmOpaSlider 初始值
    ASSERT_EQ(test_dialog_d->opaBox->value(), 30);
    ASSERT_EQ(test_dialog_d->wmOpaSlider->value(), 30);
    ASSERT_EQ(pview_d->waterMark->opacity(), 0.3);

    test_dialog_d->wmOpaSlider->setValue(50);

    ASSERT_EQ(test_dialog_d->opaBox->value(), 50);
    ASSERT_EQ(pview_d->waterMark->opacity(), 0.5);
}

TEST_F(ut_DPrintPreviewDialog, testPrintOrderGroup)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_TRUE(test_dialog_d->printOrderGroup->buttons()[0]->isEnabled());
    ASSERT_FALSE(test_dialog_d->printOrderGroup->buttons()[1]->isEnabled());

    // pdf mode cannot be reached
    // TODO: printOrderGroup Collate pages

    // TODO: printOrderGroup Print pages in order
}

TEST_F(ut_DPrintPreviewDialog, testWaterMarkBtn)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    // 隐藏 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(false);

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isHidden());
}

TEST_F(ut_DPrintPreviewDialog, testWaterTypeGroup)
{
    // 显示 advanced 菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 显示 waterMark 菜单
    test_dialog_d->waterMarkBtnClicked(true);
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->watermarksettingwdg));

    ASSERT_TRUE(test_dialog_d->watermarksettingwdg->isVisible());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    // waterTypeGroup Text 选中 watermark
    ASSERT_TRUE(test_dialog_d->waterTypeGroup->button(0)->isEnabled());
    ASSERT_EQ(pview_d->waterMark->type, DPrintPreviewDialogPrivate::Type_Text);
    ASSERT_EQ(test_dialog_d->typeChoice, 0);

    // waterTypeGroup Image 选中 watermark
    ASSERT_TRUE(test_dialog_d->waterTypeGroup->button(1)->isEnabled());
    test_dialog_d->waterTypeGroup->button(1)->click();

    ASSERT_TRUE(test_dialog_d->picPathEdit->isEnabled());
    ASSERT_EQ(pview_d->waterMark->type, DPrintPreviewDialogPrivate::Type_Image);
    ASSERT_EQ(test_dialog_d->typeChoice, 1);
}

TEST_F(ut_DPrintPreviewDialog, testPageRangeEdit)
{
    DToolTip *alertToopTip;
    DAlertControl *alterControl;
    // 测试 pageRangeCombo 未选中 Select pages 时调用 _q_customPagesFinished
    Q_EMIT test_dialog_d->_q_customPagesFinished();

    ASSERT_STREQ(test_dialog_d->lastPageRange.toLocal8Bit(), "");

    test_dialog_d->pageRangeCombo->setCurrentText("Select pages");

    ASSERT_STREQ(test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");

    ASSERT_TRUE(test_dialog_d->pageRangeEdit->isEnabled());

    // 测试 pageRangeEdit 为空时且失去焦点的情况
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_TRUE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "Input page numbers please");

    // 测试 pageRangeEdit 正常输入且失去焦点的情况 1-2
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("1-2");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_FALSE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "");

    // 测试 pageRangeEdit 正常输入且失去焦点的情况 999-1
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("999-1");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_TRUE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "Maximum page number reached");

    // 测试 pageRangeEdit 正常输入且失去焦点的情况 2-1
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("2-1");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_FALSE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "");

    // 测试 pageRangeEdit 未输入完且失去焦点的情况 1,2
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("1,2");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_FALSE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "");

    // 测试 pageRangeEdit 未输入完且失去焦点的情况 1,9999
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("1,9999");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_TRUE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "Maximum page number reached");

    // 测试 pageRangeEdit 未输入完且失去焦点的情况 1-
    test_dialog_d->pageRangeEdit->setFocus();
    test_dialog_d->pageRangeEdit->setText("1-");
    test_dialog_d->pageRangeEdit->clearFocus();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_TRUE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "Input page numbers like this: 1,3,5-7,11-15,18,21");

    // 测试 pageRangeEdit 为空且没有 focus 的情况
    test_dialog_d->pageRangeEdit->setText("");
    Q_EMIT test_dialog_d->pageRangeEdit->editingFinished();

    alterControl = test_dialog_d->pageRangeEdit->d_func()->control;
    alertToopTip = alterControl->d_func()->tooltip;
    ASSERT_TRUE(alterControl->isAlert());
    if (alertToopTip && alterControl->isAlert())
        ASSERT_STREQ(alertToopTip->text().toLocal8Bit(), "Input page numbers like this: 1,3,5-7,11-15,18,21");

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->pageRangeEdit->lineEdit()->hasFocus())
        test_dialog_d->pageRangeEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->pageRangeEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testSidebysideCheckBox)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isEnabled());

    ASSERT_FALSE(test_dialog_d->pagePerSheetCombo->isEnabled());
    Q_FOREACH (QAbstractButton *button, test_dialog_d->directGroup->buttons()) {
        ASSERT_FALSE(button->isEnabled());
    }

    // sidebysideCheckBox checked
    test_dialog_d->sidebysideCheckBox->setChecked(true);

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isChecked());

    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo->isEnabled());
    Q_FOREACH (QAbstractButton *button, test_dialog_d->directGroup->buttons()) {
        ASSERT_TRUE(button->isEnabled());
    }
    ASSERT_FALSE(test_dialog_d->inorderwdg->isEnabled());
    ASSERT_FALSE(test_dialog_d->inorderCombo->isEnabled());
    ASSERT_TRUE(test_dialog_d->printOrderGroup->button(0)->isChecked());
    ASSERT_TRUE(test_dialog_d->directGroup->button(test_dialog_d->directChoice)->isChecked());
    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo->isEnabled());
    ASSERT_TRUE(test_dialog_d->originTotalPageLabel->isVisible());

    // sidebysideCheckBox not checked
    test_dialog_d->sidebysideCheckBox->setChecked(false);

    ASSERT_FALSE(test_dialog_d->pagePerSheetCombo->isEnabled());
    Q_FOREACH (QAbstractButton *button, test_dialog_d->directGroup->buttons()) {
        ASSERT_FALSE(button->isEnabled());
    }
    ASSERT_FALSE(test_dialog_d->pagePerSheetCombo->isEnabled());
    ASSERT_FALSE(test_dialog_d->originTotalPageLabel->isVisible());
}

TEST_F(ut_DPrintPreviewDialog, testJumpPageEdit_lineEdit)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 开启并列打印
    test_dialog_d->sidebysideCheckBox->setChecked(true);

    ASSERT_TRUE(test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_TRUE(test_dialog_d->pagePerSheetCombo->isEnabled());

    DPrintPreviewWidgetPrivate* pview_d = test_dialog_d->pview->d_func();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);

    test_dialog_d->jumpPageEdit->lineEdit()->setFocus();
    test_dialog_d->jumpPageEdit->lineEdit()->setText("2");
    test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 2);
    ASSERT_EQ(pview_d->currentPageNumber, 2);

    // 关闭并列打印
    test_dialog_d->sidebysideCheckBox->setChecked(false);

    ASSERT_FALSE(test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_FALSE(test_dialog_d->pagePerSheetCombo->isEnabled());

    test_dialog_d->jumpPageEdit->lineEdit()->setFocus();
    test_dialog_d->jumpPageEdit->lineEdit()->setText("0");
    test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_EQ(pview_d->currentPageNumber, 1);

    test_dialog_d->jumpPageEdit->lineEdit()->setFocus();
    test_dialog_d->jumpPageEdit->lineEdit()->setText("9999");
    test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), MAX_PAGE_VALUE);
    ASSERT_EQ(pview_d->currentPageNumber, MAX_PAGE_VALUE);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->jumpPageEdit->lineEdit()->hasFocus())
        test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->jumpPageEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testFirstBtn)
{
    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_FALSE(test_dialog_d->firstBtn->isEnabled());

    test_dialog_d->jumpPageEdit->lineEdit()->setFocus();
    test_dialog_d->jumpPageEdit->lineEdit()->setText("2");
    test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_EQ(test_dialog_d->pview->currentPage(), 2);

    ASSERT_TRUE(test_dialog_d->firstBtn->isEnabled());

    test_dialog_d->firstBtn->click();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_EQ(test_dialog_d->pview->currentPage(), 1);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->jumpPageEdit->lineEdit()->hasFocus())
        test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->jumpPageEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testPrevPageBtn)
{
    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_FALSE(test_dialog_d->prevPageBtn->isEnabled());

    test_dialog_d->jumpPageEdit->lineEdit()->setFocus();
    test_dialog_d->jumpPageEdit->lineEdit()->setText("2");
    test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_EQ(test_dialog_d->pview->currentPage(), 2);

    ASSERT_TRUE(test_dialog_d->prevPageBtn->isEnabled());

    test_dialog_d->prevPageBtn->click();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_EQ(test_dialog_d->pview->currentPage(), 1);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->jumpPageEdit->lineEdit()->hasFocus())
        test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->jumpPageEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testNextPageBtn)
{
    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_TRUE(test_dialog_d->nextPageBtn->isEnabled());

    test_dialog_d->nextPageBtn->click();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 2);
    ASSERT_EQ(test_dialog_d->pview->currentPage(), 2);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->jumpPageEdit->lineEdit()->hasFocus())
        test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->jumpPageEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testLastBtn)
{
    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_TRUE(test_dialog_d->lastBtn->isEnabled());

    test_dialog_d->lastBtn->click();

    ASSERT_EQ(test_dialog_d->jumpPageEdit->value(), MAX_PAGE_VALUE);
    ASSERT_EQ(test_dialog_d->pview->currentPage(), MAX_PAGE_VALUE);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->jumpPageEdit->lineEdit()->hasFocus())
        test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->jumpPageEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testPaperSizeCombo)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 测试初始值
    ASSERT_EQ(test_dialog_d->paperSizeCombo->currentIndex(), 1);
    ASSERT_STREQ(test_dialog_d->paperSizeCombo->currentText().toLocal8Bit(), "A4");

    // paperSizeCombo 选中 A3
    test_dialog_d->paperSizeCombo->setCurrentText("A3");

    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::A3);

    // paperSizeCombo 选中 A4
    test_dialog_d->paperSizeCombo->setCurrentText("A4");

    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::A4);

    // paperSizeCombo 选中 A5
    test_dialog_d->paperSizeCombo->setCurrentText("A5");

    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::A5);

    // paperSizeCombo 选中 B4
    test_dialog_d->paperSizeCombo->setCurrentText("B4");

    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::B4);

    // paperSizeCombo 选中 B5
    test_dialog_d->paperSizeCombo->setCurrentText("B5");

    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::B5);

    test_dialog_d->paperSizeCombo->setCurrentText("8K");

    // paperSizeCombo 选中 8K
    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::Custom);

    test_dialog_d->paperSizeCombo->setCurrentText("16K");

    // paperSizeCombo 选中 16K
    ASSERT_EQ(test_dialog_d->printer->pageSize(), QPrinter::Custom);
}

TEST_F(ut_DPrintPreviewDialog, testScaleRateEdit_lineEdit)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // 测试 scaleGroup 初始值
    ASSERT_EQ(test_dialog_d->scaleGroup->checkedId(), 1);

    // scaleGroup 选择 自定义比例
    test_dialog_d->scaleGroup->button(2)->click();

    ASSERT_EQ(test_dialog_d->scaleGroup->checkedId(), 2);
    ASSERT_EQ(test_dialog_d->pview->getScale(), 1);
    ASSERT_TRUE(test_dialog_d->scaleRateEdit->lineEdit()->isEnabled());

    // 测试 scaleRateEdit
    test_dialog_d->scaleRateEdit->lineEdit()->setFocus();
    test_dialog_d->scaleRateEdit->lineEdit()->setText("1");
    test_dialog_d->scaleRateEdit->lineEdit()->clearFocus();

    ASSERT_EQ(test_dialog_d->pview->getScale(), 0.1);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->scaleRateEdit->lineEdit()->hasFocus())
        test_dialog_d->scaleRateEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->scaleRateEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testScaleGroup)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    // scaleGroup 选中 Actual size
    test_dialog_d->scaleGroup->button(1)->click();

    ASSERT_EQ(test_dialog_d->scaleGroup->checkedId(), 1);
    ASSERT_EQ(test_dialog_d->pview->getScale(), 1);
    ASSERT_FALSE(test_dialog_d->scaleRateEdit->lineEdit()->isEnabled());

    // scaleGroup 选中 Scale
    test_dialog_d->scaleGroup->button(2)->click();

    ASSERT_EQ(test_dialog_d->scaleGroup->checkedId(), 2);
    ASSERT_EQ(test_dialog_d->pview->getScale(), 1);
    ASSERT_TRUE(test_dialog_d->scaleRateEdit->lineEdit()->isEnabled());

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->scaleRateEdit->lineEdit()->hasFocus())
        test_dialog_d->scaleRateEdit->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->scaleRateEdit->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testCopycountspinbox)
{
    ASSERT_FALSE(test_dialog_d->copycountspinbox->isEnabled());

    ASSERT_EQ(test_dialog_d->copycountspinbox->value(), 1);

    test_dialog_d->copycountspinbox->setValue(2);
    Q_EMIT test_dialog_d->copycountspinbox->lineEdit()->textEdited("2");

    ASSERT_EQ(test_dialog_d->copycountspinbox->value(), 2);

    test_dialog_d->copycountspinbox->lineEdit()->setText("");
    Q_EMIT test_dialog_d->copycountspinbox->lineEdit()->textEdited("");
    Q_EMIT test_dialog_d->copycountspinbox->editingFinished();

    ASSERT_EQ(test_dialog_d->copycountspinbox->value(), 2);

    // 清除焦点，防止TearDown调用槽函数
    if (test_dialog_d->copycountspinbox->lineEdit()->hasFocus())
        test_dialog_d->copycountspinbox->lineEdit()->clearFocus();

    ASSERT_FALSE(test_dialog_d->copycountspinbox->hasFocus());
}

TEST_F(ut_DPrintPreviewDialog, testMarginTopSpin)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), 0);

    test_dialog_d->marginTopSpin->setFocus();
    test_dialog_d->marginTopSpin->setValue(2);
    test_dialog_d->marginTopSpin->clearFocus();
    Q_EMIT test_dialog_d->marginTopSpin->lineEdit()->textEdited("2");
    Q_EMIT test_dialog_d->marginTopSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), 2);

    test_dialog_d->marginTopSpin->setFocus();
    test_dialog_d->marginTopSpin->lineEdit()->setText("");
    test_dialog_d->marginTopSpin->clearFocus();
    Q_EMIT test_dialog_d->marginTopSpin->lineEdit()->textEdited("");
    Q_EMIT test_dialog_d->marginTopSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginTopSpin->value(), 2);
}

TEST_F(ut_DPrintPreviewDialog, testMarginRightSpin)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), 0);

    test_dialog_d->marginRightSpin->setFocus();
    test_dialog_d->marginRightSpin->setValue(2);
    test_dialog_d->marginRightSpin->clearFocus();
    Q_EMIT test_dialog_d->marginRightSpin->lineEdit()->textEdited("2");
    Q_EMIT test_dialog_d->marginRightSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), 2);

    test_dialog_d->marginRightSpin->setFocus();
    test_dialog_d->marginRightSpin->lineEdit()->setText("");
    test_dialog_d->marginRightSpin->clearFocus();
    Q_EMIT test_dialog_d->marginRightSpin->lineEdit()->textEdited("");
    Q_EMIT test_dialog_d->marginRightSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginRightSpin->value(), 2);
}

TEST_F(ut_DPrintPreviewDialog, testMarginLeftSpin)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), 0);

    test_dialog_d->marginLeftSpin->setFocus();
    test_dialog_d->marginLeftSpin->setValue(2);
    test_dialog_d->marginLeftSpin->clearFocus();
    Q_EMIT test_dialog_d->marginLeftSpin->lineEdit()->textEdited("2");
    Q_EMIT test_dialog_d->marginLeftSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), 2);

    test_dialog_d->marginLeftSpin->setFocus();
    test_dialog_d->marginLeftSpin->lineEdit()->setText("");
    test_dialog_d->marginLeftSpin->clearFocus();
    Q_EMIT test_dialog_d->marginLeftSpin->lineEdit()->textEdited("");
    Q_EMIT test_dialog_d->marginLeftSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginLeftSpin->value(), 2);
}

TEST_F(ut_DPrintPreviewDialog, testMarginBottomSpin)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), 0);

    test_dialog_d->marginBottomSpin->setFocus();
    test_dialog_d->marginBottomSpin->setValue(2);
    test_dialog_d->marginBottomSpin->clearFocus();
    Q_EMIT test_dialog_d->marginBottomSpin->lineEdit()->textEdited("2");
    Q_EMIT test_dialog_d->marginBottomSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), 2);

    test_dialog_d->marginBottomSpin->setFocus();
    test_dialog_d->marginBottomSpin->lineEdit()->setText("");
    test_dialog_d->marginBottomSpin->clearFocus();
    Q_EMIT test_dialog_d->marginBottomSpin->lineEdit()->textEdited("");
    Q_EMIT test_dialog_d->marginBottomSpin->editingFinished();

    ASSERT_EQ(test_dialog_d->marginBottomSpin->value(), 2);
}

TEST_F(ut_DPrintPreviewDialog, testDuplexCheckBox)
{
    // 显示advanced菜单
    test_dialog_d->showadvancesetting();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(test_dialog_d->advancesettingwdg));

    ASSERT_TRUE(test_dialog_d->advancesettingwdg->isVisible());

    ASSERT_FALSE(test_dialog_d->duplexCheckBox->isEnabled());
    ASSERT_FALSE(test_dialog_d->duplexCheckBox->isChecked());
//    pdf mode cannot be reached
    // TODO: duplexCheckBox
}

TEST_F(ut_DPrintPreviewDialog, themeTypeChanged)
{
    DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::LightType);

    DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::DarkType);
}

TEST_F(ut_DPrintPreviewDialog, testPrivateUpdateSubControlSettings)
{
    test_dialog_d->waterMarkBtn->setChecked(true);
    QSignalSpy spy(test_dialog_d->inclinatBox, &DSpinBox::editingFinished);
    test_dialog_d->updateSubControlSettings(DPrintPreviewSettingInfo::PS_Watermark);
    if (DPrintPreviewDialog::currentPlugin().isEmpty()) {
        EXPECT_EQ(spy.count(), 0);
    } else {
        auto info = testPrintDialog->createDialogSettingInfo(DPrintPreviewSettingInfo::PS_Watermark);
        if (info) {
            EXPECT_EQ(spy.count(), 1);
            delete info;
        } else {
            EXPECT_EQ(spy.count(), 0);
        }
    }
}

TEST_F(ut_DPrintPreviewDialog, createDialogSettingInfo)
{
    if (!DPrintPreviewDialog::currentPlugin().isEmpty()) {
        return;
    }

    auto info = testPrintDialog->createDialogSettingInfo(DPrintPreviewSettingInfo::PS_Watermark);
    ASSERT_TRUE(info);
    EXPECT_EQ(info->type(), DPrintPreviewSettingInfo::PS_Watermark);
    delete info;

    info = testPrintDialog->createDialogSettingInfo(DPrintPreviewSettingInfo::PS_SettingsCount);
    ASSERT_FALSE(info);
}

TEST_F(ut_DPrintPreviewDialog, updateDialogSettingInfo)
{
    if (!DPrintPreviewDialog::currentPlugin().isEmpty()) {
        return;
    }

    test_dialog_d->waterMarkBtn->setChecked(true);
    auto info = testPrintDialog->createDialogSettingInfo(DPrintPreviewSettingInfo::PS_Watermark);
    auto watermarkInfo = dynamic_cast<DPrintPreviewWatermarkInfo *>(info);
    ASSERT_TRUE(watermarkInfo);
    EXPECT_EQ(watermarkInfo->opened, true);

    watermarkInfo->opened = false;
    bool newFlag = false;
    testPrintDialog->updateDialogSettingInfo(watermarkInfo);
    delete watermarkInfo;

    info = testPrintDialog->createDialogSettingInfo(DPrintPreviewSettingInfo::PS_Watermark);
    watermarkInfo = dynamic_cast<DPrintPreviewWatermarkInfo *>(info);
    ASSERT_TRUE(watermarkInfo);
    EXPECT_EQ(newFlag, watermarkInfo->opened);

    delete watermarkInfo;
}

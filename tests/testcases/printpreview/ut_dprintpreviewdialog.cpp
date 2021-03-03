#include <gtest/gtest.h>
#include <QTest>

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

#define DELAY_TIME 50
#define MAX_PAGE_VALUE 4

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

    DPrintPreviewDialog *printDialog;
    TestPrintPreviewDialog *testPrintDialog;
    DPrintPreviewDialogPrivate *dialog_d;
    DPrintPreviewDialogPrivate *test_dialog_d;
};

void ut_DPrintPreviewDialog::SetUp()
{
    printDialog = new DPrintPreviewDialog;
    testPrintDialog = new TestPrintPreviewDialog;
//    dialog_d = new DPrintPreviewDialogPrivate(testPrintDialog);
    printDialog->show();
    testPrintDialog->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(printDialog));
    dialog_d = printDialog->d_func();
    test_dialog_d = testPrintDialog->d_func();
    ASSERT_TRUE(dialog_d);
}

void ut_DPrintPreviewDialog::TearDown()
{
    delete printDialog;
    delete testPrintDialog;
}

TEST_F(ut_DPrintPreviewDialog, testForInit)
{
    // 测试保存PDF文件名称接口
    this->printDialog->setDocName("test");
    ASSERT_STREQ(this->printDialog->docName().toLocal8Bit(), "test");

    // 测试保存PDF文件路径接口
    this->printDialog->setPrintFromPath(":/test.pdf");
    ASSERT_STREQ(this->printDialog->printFromPath().toLocal8Bit(), ":/test.pdf");

    // 测试参数初始化
    ASSERT_TRUE(this->dialog_d->printer);
    ASSERT_EQ(this->dialog_d->printer->orientation(), QPrinter::Portrait);
    ASSERT_EQ(this->dialog_d->printer->pageSize(), QPrinter::A4);
    ASSERT_EQ(this->dialog_d->printer->paperName(), "A4");

    ASSERT_EQ(this->dialog_d->totalPages, 0);
    ASSERT_EQ(this->dialog_d->fontSizeMore, false);
    ASSERT_EQ(this->dialog_d->lastPageRange, "");
    ASSERT_EQ(this->dialog_d->supportedDuplexFlag, false);
    ASSERT_EQ(this->dialog_d->supportedColorMode, true);
    ASSERT_EQ(this->dialog_d->isInited, true);
    ASSERT_EQ(this->dialog_d->strLengths, 0);
    ASSERT_EQ(this->dialog_d->isOnFocus, false);
    ASSERT_EQ(this->dialog_d->lastCusWatermarkText, "");
    ASSERT_EQ(this->dialog_d->isChecked, false);
    ASSERT_EQ(this->dialog_d->isEventHide, false);
    ASSERT_EQ(this->dialog_d->isInitBtnColor, true);
    ASSERT_EQ(this->dialog_d->waterColor, QColor("#6f6f6f"));
    ASSERT_EQ(this->dialog_d->typeChoice, 0);
    ASSERT_EQ(this->dialog_d->directChoice, 0);

    ASSERT_EQ(this->dialog_d->colorModeCombo->count(), 2);
    ASSERT_EQ(this->dialog_d->paperSizeCombo->count(), 7);
    ASSERT_EQ(this->dialog_d->paperSizeCombo->currentIndex(), 1);

    // 测试控件初始化
    ASSERT_TRUE(this->dialog_d->pview);
    ASSERT_TRUE(this->dialog_d->back);
    ASSERT_TRUE(this->dialog_d->basicsettingwdg);
    ASSERT_TRUE(this->dialog_d->advancesettingwdg);
    ASSERT_TRUE(this->dialog_d->watermarksettingwdg);
    ASSERT_TRUE(this->dialog_d->textWatermarkWdg);
    ASSERT_TRUE(this->dialog_d->picWatermarkWdg);
    ASSERT_TRUE(this->dialog_d->firstBtn);
    ASSERT_TRUE(this->dialog_d->prevPageBtn);
    ASSERT_TRUE(this->dialog_d->jumpPageEdit);
    ASSERT_TRUE(this->dialog_d->totalPageLabel);
    ASSERT_TRUE(this->dialog_d->originTotalPageLabel);
    ASSERT_TRUE(this->dialog_d->nextPageBtn);
    ASSERT_TRUE(this->dialog_d->lastBtn);
    ASSERT_TRUE(this->dialog_d->cancelBtn);
    ASSERT_TRUE(this->dialog_d->printBtn);
    ASSERT_TRUE(this->dialog_d->advanceBtn);
    ASSERT_TRUE(this->dialog_d->scrollarea);
    ASSERT_TRUE(this->dialog_d->printDeviceCombo);
    ASSERT_TRUE(this->dialog_d->copycountspinbox);
    ASSERT_TRUE(this->dialog_d->pageRangeCombo);
    ASSERT_TRUE(this->dialog_d->pageRangeEdit);
    ASSERT_TRUE(this->dialog_d->orientationgroup);
    ASSERT_TRUE(this->dialog_d->colorModeCombo);
    ASSERT_TRUE(this->dialog_d->marginsCombo);
    ASSERT_TRUE(this->dialog_d->marginTopSpin);
    ASSERT_TRUE(this->dialog_d->marginBottomSpin);
    ASSERT_TRUE(this->dialog_d->marginLeftSpin);
    ASSERT_TRUE(this->dialog_d->marginRightSpin);
    ASSERT_TRUE(this->dialog_d->scaleGroup);
    ASSERT_TRUE(this->dialog_d->scaleRateEdit);
    ASSERT_TRUE(this->dialog_d->duplexCombo);
    ASSERT_TRUE(this->dialog_d->duplexCheckBox);
    ASSERT_TRUE(this->dialog_d->paperSizeCombo);
    ASSERT_TRUE(this->dialog_d->paperSizeCombo);
    ASSERT_TRUE(this->dialog_d->pagePerSheetCombo);
    ASSERT_TRUE(this->dialog_d->sidebysideCheckBox);
    ASSERT_TRUE(this->dialog_d->directGroup);
    ASSERT_TRUE(this->dialog_d->printOrderGroup);
    ASSERT_TRUE(this->dialog_d->inorderCombo);
    ASSERT_TRUE(this->dialog_d->waterTextCombo);
    ASSERT_TRUE(this->dialog_d->waterColorBtn);
    ASSERT_TRUE(this->dialog_d->waterTextEdit);
    ASSERT_TRUE(this->dialog_d->fontCombo);
    ASSERT_TRUE(this->dialog_d->waterMarkBtn);
    ASSERT_TRUE(this->dialog_d->picPathEdit);
    ASSERT_TRUE(this->dialog_d->waterTypeGroup);
    ASSERT_TRUE(this->dialog_d->waterPosCombox);
    ASSERT_TRUE(this->dialog_d->inclinatBox);
    ASSERT_TRUE(this->dialog_d->waterSizeSlider);
    ASSERT_TRUE(this->dialog_d->sizeBox);
    ASSERT_TRUE(this->dialog_d->wmOpaSlider);
    ASSERT_TRUE(this->dialog_d->opaBox);
    ASSERT_EQ(this->dialog_d->marginOldValue.length(), 8);
    for (QVector<qreal>::iterator iter = this->dialog_d->marginOldValue.begin(); iter < this->dialog_d->marginOldValue.end(); iter++) {
        ASSERT_EQ(*iter, 0);
    }
    ASSERT_TRUE(this->dialog_d->spacer);
    ASSERT_TRUE(this->dialog_d->wmSpacer);
    ASSERT_TRUE(this->dialog_d->colorWidget);
    ASSERT_TRUE(this->dialog_d->printInOrderRadio);
    ASSERT_TRUE(this->dialog_d->inorderwdg);
    ASSERT_TRUE(this->dialog_d->pickColorWidget);
    ASSERT_EQ(this->dialog_d->firstBtn->isEnabled(), false);
    ASSERT_EQ(this->dialog_d->prevPageBtn->isEnabled(), false);
    ASSERT_EQ(this->dialog_d->nextPageBtn->isEnabled(), false);
    ASSERT_EQ(this->dialog_d->lastBtn->isEnabled(), false);
    ASSERT_EQ(this->dialog_d->spinboxTextCaches.begin(), this->dialog_d->spinboxTextCaches.end());
}

TEST_F(ut_DPrintPreviewDialog, testPaintRequested)
{
    ASSERT_EQ(this->test_dialog_d->totalPages, 4);
    ASSERT_EQ(this->test_dialog_d->marginOldValue.length(), 8);
    ASSERT_EQ(this->test_dialog_d->firstBtn->isEnabled(), false);
    ASSERT_EQ(this->test_dialog_d->prevPageBtn->isEnabled(), false);
    ASSERT_EQ(this->test_dialog_d->nextPageBtn->isEnabled(), true);
    ASSERT_EQ(this->test_dialog_d->lastBtn->isEnabled(), true);
}

TEST_F(ut_DPrintPreviewDialog, setfrmaeback)
{
    DFrame *printerFrame = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(printerFrame);
    DPalette pa = DPaletteHelper::instance()->palette(printerFrame);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *copycountFrame = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(copycountFrame);
    pa = DPaletteHelper::instance()->palette(copycountFrame);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *pageFrame = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(pageFrame);
    pa = DPaletteHelper::instance()->palette(pageFrame);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *colorframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(colorframe);
    pa = DPaletteHelper::instance()->palette(colorframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *marginsframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(marginsframe);
    pa = DPaletteHelper::instance()->palette(marginsframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *paperframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(paperframe);
    pa = DPaletteHelper::instance()->palette(paperframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *duplexframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(duplexframe);
    pa = DPaletteHelper::instance()->palette(duplexframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *sidebysideframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(sidebysideframe);
    pa = DPaletteHelper::instance()->palette(sidebysideframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *watermarkframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(watermarkframe);
    pa = DPaletteHelper::instance()->palette(watermarkframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *posframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(posframe);
    pa = DPaletteHelper::instance()->palette(posframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *inclinatframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(inclinatframe);
    pa = DPaletteHelper::instance()->palette(inclinatframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *sizeframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(sizeframe);
    pa = DPaletteHelper::instance()->palette(sizeframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());

    DFrame *opaframe = new DFrame(this->dialog_d->basicsettingwdg);
    this->dialog_d->setfrmaeback(opaframe);
    pa = DPaletteHelper::instance()->palette(opaframe);

    ASSERT_EQ(pa.base().color(), pa.itemBackground().color());
}

TEST_F(ut_DPrintPreviewDialog, advanceBtn)
{
    ASSERT_TRUE(this->test_dialog_d->advanceBtn->isEnabled());
    ASSERT_TRUE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());
    this->test_dialog_d->showadvancesetting();
}

TEST_F(ut_DPrintPreviewDialog, printDeviceCombo)
{
    ASSERT_TRUE(this->test_dialog_d->printDeviceCombo->isEnabled());
    ASSERT_EQ(this->test_dialog_d->printDeviceCombo->currentIndex(), 0);
    ASSERT_STREQ(this->test_dialog_d->printDeviceCombo->currentText().toLocal8Bit(), "Print to PDF");
    // printDeviceCombo Print to PDF
    ASSERT_FALSE(this->test_dialog_d->copycountspinbox->isEnabled());
    ASSERT_EQ(this->test_dialog_d->copycountspinbox->value(), 1);
    ASSERT_EQ(this->test_dialog_d->duplexCheckBox->checkState(), Qt::Unchecked);
    ASSERT_FALSE(this->test_dialog_d->duplexCheckBox->isEnabled());
    ASSERT_STREQ(this->test_dialog_d->duplexCombo->currentText().toLocal8Bit(), "");
    ASSERT_FALSE(this->test_dialog_d->duplexCombo->isEnabled());
    ASSERT_TRUE(this->test_dialog_d->waterColorBtn->isEnabled());
    ASSERT_EQ(this->test_dialog_d->colorModeCombo->currentIndex(), 0);
    ASSERT_FALSE(this->test_dialog_d->colorModeCombo->isEnabled());

    // 选中 printDeviceCombo
    QTest::mouseClick(this->test_dialog_d->printDeviceCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->printDeviceCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->printDeviceCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->printDeviceCombo->currentIndex(), 1);
    ASSERT_STREQ(this->test_dialog_d->printDeviceCombo->currentText().toLocal8Bit(), "Save as Image");
    // printDeviceCombo Save as Image
    ASSERT_FALSE(this->test_dialog_d->copycountspinbox->isEnabled());
    ASSERT_EQ(this->test_dialog_d->copycountspinbox->value(), 1);
    ASSERT_EQ(this->test_dialog_d->duplexCheckBox->checkState(), Qt::Unchecked);
    ASSERT_FALSE(this->test_dialog_d->duplexCheckBox->isEnabled());
    ASSERT_STREQ(this->test_dialog_d->duplexCombo->currentText().toLocal8Bit(), "");
    ASSERT_FALSE(this->test_dialog_d->duplexCombo->isEnabled());
    ASSERT_TRUE(this->test_dialog_d->waterColorBtn->isEnabled());
    ASSERT_EQ(this->test_dialog_d->colorModeCombo->currentIndex(), 0);
    ASSERT_FALSE(this->test_dialog_d->colorModeCombo->isEnabled());
}

TEST_F(ut_DPrintPreviewDialog, cancelBtn)
{
    ASSERT_TRUE(this->test_dialog_d->cancelBtn->isEnabled());
    ASSERT_FALSE(this->testPrintDialog->isHidden());

    // 点击 cancel 按钮
    QTest::mouseClick(this->test_dialog_d->cancelBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_TRUE(this->testPrintDialog->isHidden());
}

TEST_F(ut_DPrintPreviewDialog, pageRangeCombo)
{
    ASSERT_TRUE(this->test_dialog_d->pageRangeCombo->isEnabled());
    ASSERT_EQ(this->test_dialog_d->pageRangeCombo->currentIndex(), 0);
    ASSERT_EQ(this->test_dialog_d->pageRangeCombo->count(), 3);
    ASSERT_STREQ(this->test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "All");
    // TODO: pageRangeCombo ALL
    ASSERT_FALSE(this->test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_EQ(this->test_dialog_d->pageRangeEdit->lineEdit()->placeholderText().toLocal8Bit(), "");
    ASSERT_EQ(this->test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_EQ(this->test_dialog_d->pview->pageRangeMode(),
              static_cast<DPrintPreviewWidget::PageRange>(this->test_dialog_d->pageRangeCombo->currentIndex()));
    ASSERT_TRUE(this->test_dialog_d->printBtn->isEnabled());
    ASSERT_FALSE(this->test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(this->test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    // 选中 pageRangeCombo
    QTest::mouseClick(this->test_dialog_d->pageRangeCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->pageRangeCombo->currentIndex(), 1);
    ASSERT_STREQ(this->test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Current page");
    // TODO: pageRangeCombo Current page
    ASSERT_FALSE(this->test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_EQ(this->test_dialog_d->pageRangeEdit->lineEdit()->placeholderText().toLocal8Bit(), "");
    ASSERT_EQ(this->test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_EQ(this->test_dialog_d->pview->pageRangeMode(),
              static_cast<DPrintPreviewWidget::PageRange>(this->test_dialog_d->pageRangeCombo->currentIndex()));
    ASSERT_TRUE(this->test_dialog_d->printBtn->isEnabled());
    ASSERT_FALSE(this->test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(this->test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    QTest::mouseClick(this->test_dialog_d->pageRangeCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->pageRangeCombo->currentIndex(), 2);
    ASSERT_STREQ(this->test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");
    // TODO: pageRangeCombo Select pages
    ASSERT_TRUE(this->test_dialog_d->pageRangeEdit->isEnabled());
    ASSERT_EQ(this->test_dialog_d->pageRangeEdit->lineEdit()->placeholderText().toLocal8Bit(), "For example, 1,3,5-7,11-15,18,21");
    ASSERT_EQ(this->test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "");
    ASSERT_EQ(this->test_dialog_d->pview->pageRangeMode(),
              static_cast<DPrintPreviewWidget::PageRange>(this->test_dialog_d->pageRangeCombo->currentIndex()));
    ASSERT_FALSE(this->test_dialog_d->printBtn->isEnabled());
    ASSERT_TRUE(this->test_dialog_d->pageRangeEdit->isAlert());
    ASSERT_STREQ(this->test_dialog_d->totalPageLabel->text().toLocal8Bit(), "4");

    QTest::mouseClick(this->test_dialog_d->pageRangeEdit->lineEdit(), Qt::LeftButton, Qt::NoModifier);
    QTest::keyClicks(this->test_dialog_d->pageRangeEdit->lineEdit(), "2-3");

    QTest::qWait(DELAY_TIME);

    QTest::keyClick(this->test_dialog_d->pageRangeEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->pageRangeEdit->lineEdit()->text().toLocal8Bit(), "2-3");

    ASSERT_STREQ(this->test_dialog_d->totalPageLabel->text().toLocal8Bit(), "2");

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->pageRangeEdit->lineEdit()->clearFocus();
}

TEST_F(ut_DPrintPreviewDialog, marginsCombo)
{
    ASSERT_TRUE(this->test_dialog_d->marginsCombo->isEnabled());
    ASSERT_EQ(this->test_dialog_d->marginsCombo->currentIndex(), 0);
    ASSERT_EQ(this->test_dialog_d->marginsCombo->count(), 4);

    // TODO: marginsCombo Narrow(mm)
    ASSERT_STREQ(this->test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Narrow (mm)");

    // 选中 marginsCombo
    QTest::mouseClick(this->test_dialog_d->marginsCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->marginsCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->marginsCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    // TODO: marginsCombo Normal(mm)
    ASSERT_STREQ(this->test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Normal (mm)");

    QTest::mouseClick(this->test_dialog_d->marginsCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->marginsCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->marginsCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    // TODO: marginsCombo Moderate(mm)
    ASSERT_STREQ(this->test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Moderate (mm)");

    QTest::mouseClick(this->test_dialog_d->marginsCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->marginsCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->marginsCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    // TODO: marginsCombo Customize(mm)
    ASSERT_STREQ(this->test_dialog_d->marginsCombo->currentText().toLocal8Bit(), "Customize (mm)");
}

TEST_F(ut_DPrintPreviewDialog, printBtn)
{
    ASSERT_TRUE(this->test_dialog_d->printBtn->isEnabled());
//    QTest::mouseClick(this->dialog_d->printBtn, Qt::LeftButton, Qt::NoModifier);
}

TEST_F(ut_DPrintPreviewDialog, waterColorBtn)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->waterColorBtn->isEnabled());
    ASSERT_TRUE(this->test_dialog_d->colorWidget->isHidden());

    QTest::mouseClick(this->test_dialog_d->waterColorBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->colorWidget->isHidden());
    // TODO: colorWidget
}

TEST_F(ut_DPrintPreviewDialog, colorModeCombo)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_FALSE(this->test_dialog_d->colorModeCombo->isEnabled());
    ASSERT_EQ(this->test_dialog_d->colorModeCombo->count(), 2);
    ASSERT_EQ(this->test_dialog_d->colorModeCombo->currentText(), "Color");
    // TODO: colorModeCombo Color

    this->test_dialog_d->colorModeCombo->setCurrentIndex(1);
    ASSERT_EQ(this->test_dialog_d->colorModeCombo->currentText(), "Grayscale");
    // TODO: colorModeCombo Grayscale
}

TEST_F(ut_DPrintPreviewDialog, orientationgroup)
{
    Q_FOREACH (QAbstractButton *button, this->test_dialog_d->orientationgroup->buttons()) {
        ASSERT_TRUE(button->isEnabled());
    }
    ASSERT_EQ(this->test_dialog_d->orientationgroup->checkedId(), 0);
    // TODO: orientationgroup printer_portrait

    QTest::mouseClick(this->test_dialog_d->orientationgroup->buttons()[1], Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->orientationgroup->checkedId(), 1);
    // TODO: orientationgroup printer_landscape
}

TEST_F(ut_DPrintPreviewDialog, waterTextCombo)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->waterTextCombo->isEnabled());
    ASSERT_EQ(this->test_dialog_d->waterTextCombo->count(), 4);
    ASSERT_STREQ(this->test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Confidential");
    // TODO: waterTextCombo Confidential
    ASSERT_FALSE(this->test_dialog_d->waterTextEdit->isEnabled());


    QTest::mouseClick(this->test_dialog_d->waterTextCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Draft");
    // TODO: waterTextCombo Draft
    ASSERT_FALSE(this->test_dialog_d->waterTextEdit->isEnabled());


    QTest::mouseClick(this->test_dialog_d->waterTextCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Sample");
    // TODO: waterTextCombo Sample
    ASSERT_FALSE(this->test_dialog_d->waterTextEdit->isEnabled());


    QTest::mouseClick(this->test_dialog_d->waterTextCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->waterTextCombo->currentText().toLocal8Bit(), "Custom");
    // TODO: waterTextCombo Custom
    ASSERT_TRUE(this->test_dialog_d->waterTextEdit->isEnabled());
}

TEST_F(ut_DPrintPreviewDialog, inorderCombo)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());


    ASSERT_FALSE(this->test_dialog_d->inorderCombo->isEnabled());
    ASSERT_EQ(this->test_dialog_d->inorderCombo->count(), 2);
    ASSERT_EQ(this->test_dialog_d->inorderCombo->currentIndex(), 0);
    ASSERT_EQ(this->test_dialog_d->inorderCombo->currentText().toLocal8Bit(), "Front to back");
    // TODO: inorderCombo Front to back

    this->test_dialog_d->inorderCombo->setCurrentIndex(1);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->inorderCombo->currentIndex(), 1);
    ASSERT_EQ(this->test_dialog_d->inorderCombo->currentText().toLocal8Bit(), "Back to front");
    // TODO: inorderCombo Back to front
}

TEST_F(ut_DPrintPreviewDialog, waterTextEdit)
{
    // 显示 advanced 菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示 waterMark 菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    // 选中 waterTextCombo
    QTest::mouseClick(this->test_dialog_d->waterTextCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterTextCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->waterTextEdit->text().toLocal8Bit(), "");
    QTest::mouseClick(this->test_dialog_d->waterTextEdit->lineEdit(), Qt::LeftButton, Qt::NoModifier);
    QTest::keyClicks(this->test_dialog_d->waterTextEdit->lineEdit(), "Test");

    QTest::qWait(DELAY_TIME);

    QTest::keyClick(this->test_dialog_d->pageRangeEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->waterTextEdit->lineEdit()->text().toLocal8Bit(), "Test");
    // TODO: waterTextEdit
}

TEST_F(ut_DPrintPreviewDialog, pagePerSheetCombo)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_FALSE(this->test_dialog_d->pagePerSheetCombo->isEnabled());
    ASSERT_TRUE(this->test_dialog_d->sidebysideCheckBox->isEnabled());

    QTest::mouseClick(this->test_dialog_d->sidebysideCheckBox, Qt::LeftButton, Qt::NoModifier,
                      QPoint(2, this->test_dialog_d->sidebysideCheckBox->height() / 2));

    QTest::qWait(DELAY_TIME);

    ASSERT_TRUE(this->test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_TRUE(this->test_dialog_d->pagePerSheetCombo->isEnabled());

    ASSERT_EQ(this->test_dialog_d->pagePerSheetCombo->count(), 5);
    ASSERT_STREQ(this->test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "2 pages/sheet, 1×2");
    // TODO: pagePerSheetCombo 2 pages/sheet, 1×2


    QTest::mouseClick(this->test_dialog_d->pagePerSheetCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "4 pages/sheet, 2×2");
    // TODO: pagePerSheetCombo 4 pages/sheet, 2×2

    QTest::mouseClick(this->test_dialog_d->pagePerSheetCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "6 pages/sheet, 2×3");
    // TODO: pagePerSheetCombo 6 pages/sheet, 2×3

    QTest::mouseClick(this->test_dialog_d->pagePerSheetCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "9 pages/sheet, 3×3");
    // TODO: pagePerSheetCombo 9 pages/sheet, 3×3

    QTest::mouseClick(this->test_dialog_d->pagePerSheetCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pagePerSheetCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STREQ(this->test_dialog_d->pagePerSheetCombo->currentText().toLocal8Bit(), "16 pages/sheet, 4×4");
    // TODO: pagePerSheetCombo 16 pages/sheet, 4×4
}

TEST_F(ut_DPrintPreviewDialog, picPathEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());


    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    QTest::mouseClick(this->test_dialog_d->waterTypeGroup->buttons()[1], Qt::LeftButton, Qt::NoModifier,
            QPoint(2, this->test_dialog_d->waterTypeGroup->buttons()[1]->height() / 2));

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->waterTypeGroup->checkedId(), 1);

    ASSERT_TRUE(this->test_dialog_d->picPathEdit->isEnabled());
    // TODO: picPathEdit
}

TEST_F(ut_DPrintPreviewDialog, sizeBox)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_FALSE(this->test_dialog_d->sizeBox->isHidden());
    ASSERT_TRUE(this->test_dialog_d->sizeBox->isEnabled());

    // TODO: sizeBox
    ASSERT_EQ(this->test_dialog_d->sizeBox->value(), 100);

    this->test_dialog_d->sizeBox->setValue(30);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->sizeBox->value(), 30);
    ASSERT_STREQ(this->test_dialog_d->sizeBox->text().toLocal8Bit(), "30%");

    ASSERT_EQ(this->test_dialog_d->waterSizeSlider->value(), 30);
}

TEST_F(ut_DPrintPreviewDialog, opaBox)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->opaBox->value(), 30);

    this->test_dialog_d->wmOpaSlider->setValue(50);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->opaBox->value(), 50);
    ASSERT_STREQ(this->test_dialog_d->opaBox->text().toLocal8Bit(), "50%");
}

TEST_F(ut_DPrintPreviewDialog, fontCombo)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->fontCombo->isEnabled());

    QString fontComboCurrentText = this->test_dialog_d->fontCombo->currentText();

    QTest::mouseClick(this->test_dialog_d->fontCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->fontCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->fontCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_STRNE(fontComboCurrentText.toLocal8Bit(), this->test_dialog_d->fontCombo->currentText().toLocal8Bit());
}

TEST_F(ut_DPrintPreviewDialog, pickColorWidget)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->colorWidget->isHidden());
    ASSERT_TRUE(this->test_dialog_d->colorWidget->isEnabled());


    // 显示colorWidget菜单
    QTest::mouseClick(this->test_dialog_d->waterColorBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->colorWidget->isHidden());

    // TODO: pickColorWidget
}

TEST_F(ut_DPrintPreviewDialog, waterPosCombox)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->waterPosCombox->count(), 2);
    ASSERT_STREQ(this->test_dialog_d->waterPosCombox->currentText().toLocal8Bit(), "Center");
    // TODO: waterPosCombox Center

    QTest::mouseClick(this->test_dialog_d->waterPosCombox, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterPosCombox, Qt::Key_Up, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->waterPosCombox, Qt::Key_Enter, Qt::NoModifier);

    ASSERT_STREQ(this->test_dialog_d->waterPosCombox->currentText().toLocal8Bit(), "Tile");
    //TODO: waterPosCombox Tile
}

TEST_F(ut_DPrintPreviewDialog, directGroup)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    Q_FOREACH (QAbstractButton *button, this->test_dialog_d->directGroup->buttons()) {
        ASSERT_FALSE(button->isEnabled());
    }
    ASSERT_TRUE(this->test_dialog_d->sidebysideCheckBox->isEnabled());

    QTest::mouseClick(this->test_dialog_d->sidebysideCheckBox, Qt::LeftButton, Qt::NoModifier,
                      QPoint(2, this->test_dialog_d->sidebysideCheckBox->height() / 2));

    QTest::qWait(DELAY_TIME);

    ASSERT_TRUE(this->test_dialog_d->sidebysideCheckBox->isChecked());
    ASSERT_EQ(this->test_dialog_d->directGroup->buttons().count(), 5);
    Q_FOREACH (QAbstractButton *button, this->test_dialog_d->directGroup->buttons()) {
        ASSERT_TRUE(button->isEnabled());
    }
    // TODO: directGroup button
}

TEST_F(ut_DPrintPreviewDialog, inclinatBox)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->inclinatBox->value(), 30);
    ASSERT_STREQ(this->test_dialog_d->inclinatBox->text().toLocal8Bit(), "30°");

    QTest::mouseClick(this->test_dialog_d->inclinatBox->lineEdit(), Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    this->test_dialog_d->inclinatBox->setValue(123);

    QTest::qWait(DELAY_TIME);

    QTest::keyClick(this->test_dialog_d->inclinatBox, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->inclinatBox->value(), 123);
    ASSERT_STREQ(this->test_dialog_d->inclinatBox->text().toLocal8Bit(), "123°");
//     TODO: inclinatBox

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->inclinatBox->lineEdit()->clearFocus();
}

TEST_F(ut_DPrintPreviewDialog, wmOpaSlider)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());


    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->opaBox->value(), 30);

    this->test_dialog_d->wmOpaSlider->setValue(50);

    ASSERT_EQ(this->test_dialog_d->opaBox->value(), 50);
    ASSERT_STREQ(this->test_dialog_d->opaBox->text().toLocal8Bit(), "50%");
}

TEST_F(ut_DPrintPreviewDialog, printOrderGroup)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->printOrderGroup->buttons()[0]->isEnabled());
    ASSERT_FALSE(this->test_dialog_d->printOrderGroup->buttons()[1]->isEnabled());

    // TODO: printOrderGroup Collate pages

    // TODO: printOrderGroup Print pages in order
}

TEST_F(ut_DPrintPreviewDialog, waterMarkBtn)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    // 隐藏waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_TRUE(this->test_dialog_d->watermarksettingwdg->isHidden());
}

TEST_F(ut_DPrintPreviewDialog, waterTypeGroup)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());


    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    // TODO: waterTypeGroup Text watermark

    QTest::mouseClick(this->test_dialog_d->waterTypeGroup->buttons()[1], Qt::LeftButton, Qt::NoModifier,
            QPoint(2, this->test_dialog_d->waterTypeGroup->buttons()[1]->height() / 2));

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->waterTypeGroup->checkedId(), 1);

    ASSERT_TRUE(this->test_dialog_d->picPathEdit->isEnabled());
}

TEST_F(ut_DPrintPreviewDialog, pageRangeEdit)
{
    // 选中 pageRangeCombo
    QTest::mouseClick(this->test_dialog_d->pageRangeCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->pageRangeCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->pageRangeCombo->currentIndex(), 2);
    ASSERT_STREQ(this->test_dialog_d->pageRangeCombo->currentText().toLocal8Bit(), "Select pages");

    ASSERT_TRUE(this->test_dialog_d->pageRangeEdit->isEnabled());

    // TODO: pageRangeEdit
    QTest::mouseClick(this->test_dialog_d->waterTextEdit->lineEdit(), Qt::LeftButton, Qt::NoModifier);
    QTest::keyClicks(this->test_dialog_d->waterTextEdit->lineEdit(), "1");
}

TEST_F(ut_DPrintPreviewDialog, sidebysideCheckBox)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->sidebysideCheckBox->isEnabled());

    ASSERT_FALSE(this->test_dialog_d->pagePerSheetCombo->isEnabled());
    Q_FOREACH (QAbstractButton *button, this->test_dialog_d->directGroup->buttons()) {
        ASSERT_FALSE(button->isEnabled());
    }

    QTest::mouseClick(this->test_dialog_d->sidebysideCheckBox, Qt::LeftButton, Qt::NoModifier,
                      QPoint(2, this->test_dialog_d->sidebysideCheckBox->height() / 2));

    QTest::qWait(DELAY_TIME);

    ASSERT_TRUE(this->test_dialog_d->sidebysideCheckBox->isChecked());

    ASSERT_TRUE(this->test_dialog_d->pagePerSheetCombo->isEnabled());
    Q_FOREACH (QAbstractButton *button, this->test_dialog_d->directGroup->buttons()) {
        ASSERT_TRUE(button->isEnabled());
    }
}

TEST_F(ut_DPrintPreviewDialog, jumpPageEdit_lineEdit)
{
    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);
    this->test_dialog_d->jumpPageEdit->lineEdit()->setText("2");

    QTest::keyClick(this->test_dialog_d->jumpPageEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 2);

    this->test_dialog_d->jumpPageEdit->lineEdit()->setText("0");

    QTest::keyClick(this->test_dialog_d->jumpPageEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 2);

    this->test_dialog_d->jumpPageEdit->lineEdit()->setText("12");

    QTest::keyClick(this->test_dialog_d->jumpPageEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    if (MAX_PAGE_VALUE < 12) {
        ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), MAX_PAGE_VALUE);
    } else {
        ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 12);
    }

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();
//     TODO: jumpPageEdit->lineEdit()
}

TEST_F(ut_DPrintPreviewDialog, pview)
{
    // TODO: pview 2 function
}

TEST_F(ut_DPrintPreviewDialog, firstBtn)
{
    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_FALSE(this->test_dialog_d->firstBtn->isEnabled());

    this->test_dialog_d->jumpPageEdit->lineEdit()->setText("2");

    QTest::keyClick(this->test_dialog_d->jumpPageEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 2);
    ASSERT_EQ(this->test_dialog_d->pview->currentPage(), 2);

    ASSERT_TRUE(this->test_dialog_d->firstBtn->isEnabled());

    QTest::mouseClick(this->test_dialog_d->firstBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();
}

TEST_F(ut_DPrintPreviewDialog, prevPageBtn)
{
    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_FALSE(this->test_dialog_d->prevPageBtn->isEnabled());

    this->test_dialog_d->jumpPageEdit->lineEdit()->setText("2");

    QTest::keyClick(this->test_dialog_d->jumpPageEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 2);
    ASSERT_EQ(this->test_dialog_d->pview->currentPage(), 2);

    ASSERT_TRUE(this->test_dialog_d->prevPageBtn->isEnabled());

    QTest::mouseClick(this->test_dialog_d->prevPageBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();
}

TEST_F(ut_DPrintPreviewDialog, nextPageBtn)
{
    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_TRUE(this->test_dialog_d->nextPageBtn->isEnabled());

    QTest::mouseClick(this->test_dialog_d->nextPageBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 2);

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();
}

TEST_F(ut_DPrintPreviewDialog, lastBtn)
{
    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), 1);
    ASSERT_TRUE(this->test_dialog_d->lastBtn->isEnabled());

    QTest::mouseClick(this->test_dialog_d->lastBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->jumpPageEdit->value(), MAX_PAGE_VALUE);

    // 清除焦点，防止TearDown调用槽函数
    this->test_dialog_d->jumpPageEdit->lineEdit()->clearFocus();
}

TEST_F(ut_DPrintPreviewDialog, paperSizeCombo)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->paperSizeCombo->count(), 7);
    ASSERT_EQ(this->test_dialog_d->paperSizeCombo->currentIndex(), 1);
    ASSERT_STREQ(this->test_dialog_d->paperSizeCombo->currentText().toLocal8Bit(), "A4");

    // 选中 pageRangeCombo
    QTest::mouseClick(this->test_dialog_d->paperSizeCombo, Qt::LeftButton, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->paperSizeCombo, Qt::Key_Down, Qt::NoModifier);
    QTest::keyClick(this->test_dialog_d->paperSizeCombo, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->paperSizeCombo->currentIndex(), 2);
    ASSERT_STREQ(this->test_dialog_d->paperSizeCombo->currentText().toLocal8Bit(), "A5");
    // TODO: paperSizeCombo
}

TEST_F(ut_DPrintPreviewDialog, scaleRateEdit_lineEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->scaleGroup->checkedId(), 1);

    QTest::mouseClick(this->test_dialog_d->scaleGroup->buttons()[1], Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->scaleGroup->checkedId(), 2);

    ASSERT_TRUE(this->test_dialog_d->scaleRateEdit->lineEdit()->isEnabled());

    this->test_dialog_d->scaleRateEdit->lineEdit()->setText("10");

    QTest::keyClick(this->test_dialog_d->scaleRateEdit->lineEdit(), Qt::Key_Enter, Qt::NoModifier);
    // TODO: scaleRateEdit->lineEdit()
}

TEST_F(ut_DPrintPreviewDialog, scaleGroup)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->scaleGroup->checkedId(), 1);
    // TODO: scaleGroup Actual size
    ASSERT_FALSE(this->test_dialog_d->scaleRateEdit->lineEdit()->isEnabled());

    QTest::mouseClick(this->test_dialog_d->scaleGroup->buttons()[1], Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->scaleGroup->checkedId(), 2);
    // TODO: scaleGroup Scale
    ASSERT_TRUE(this->test_dialog_d->scaleRateEdit->lineEdit()->isEnabled());
}

TEST_F(ut_DPrintPreviewDialog, copycountspinbox)
{
    ASSERT_FALSE(this->test_dialog_d->copycountspinbox->isEnabled());

    ASSERT_EQ(this->test_dialog_d->copycountspinbox->value(), 1);

    this->test_dialog_d->copycountspinbox->setValue(2);

    QTest::keyClick(this->test_dialog_d->copycountspinbox->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    ASSERT_EQ(this->test_dialog_d->copycountspinbox->value(), 2);

    // TODO: copycountspinbox
}

TEST_F(ut_DPrintPreviewDialog, marginTopSpin)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginTopSpin->value(), 0);

    this->test_dialog_d->marginTopSpin->setValue(2);
}

TEST_F(ut_DPrintPreviewDialog, marginRightSpin)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginRightSpin->value(), 0);

    this->test_dialog_d->marginRightSpin->setValue(2);
}

TEST_F(ut_DPrintPreviewDialog, marginLeftSpin)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginLeftSpin->value(), 0);

    this->test_dialog_d->marginLeftSpin->setValue(2);
}

TEST_F(ut_DPrintPreviewDialog, marginBottomSpin)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginBottomSpin->text().toLocal8Bit(), "0.00");

    this->test_dialog_d->marginBottomSpin->setValue(2);
}

TEST_F(ut_DPrintPreviewDialog, duplexCheckBox)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_FALSE(this->test_dialog_d->duplexCheckBox->isEnabled());
    ASSERT_FALSE(this->test_dialog_d->duplexCheckBox->isChecked());
    // TODO: duplexCheckBox
}

TEST_F(ut_DPrintPreviewDialog, themeTypeChanged)
{
    DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::LightType);

    DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::DarkType);
}

TEST_F(ut_DPrintPreviewDialog, marginTopSpin_lineEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_TRUE(this->test_dialog_d->marginTopSpin->isEnabled());

    ASSERT_EQ(this->test_dialog_d->marginTopSpin->value(), 0);

    this->test_dialog_d->marginTopSpin->setValue(2);

    QTest::keyClick(this->test_dialog_d->marginTopSpin->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    ASSERT_EQ(this->test_dialog_d->marginTopSpin->value(), 2);
    // TODO: marginTopSpin->lineEdit()
}

TEST_F(ut_DPrintPreviewDialog, marginRightSpin_lineEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginRightSpin->value(), 0);

    this->test_dialog_d->marginRightSpin->setValue(2);

    QTest::keyClick(this->test_dialog_d->marginRightSpin->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    ASSERT_EQ(this->test_dialog_d->marginRightSpin->value(), 2);
    // TODO: marginRightSpin->lineEdit()
}

TEST_F(ut_DPrintPreviewDialog, marginLeftSpin_lineEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginLeftSpin->value(), 0);

    this->test_dialog_d->marginLeftSpin->setValue(2);

    QTest::keyClick(this->test_dialog_d->marginLeftSpin->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    ASSERT_EQ(this->test_dialog_d->marginLeftSpin->value(), 2);
    // TODO: marginLeftSpin->lineEdit()
}

TEST_F(ut_DPrintPreviewDialog, marginBottomSpin_lineEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->marginBottomSpin->value(), 0);

    this->test_dialog_d->marginBottomSpin->setValue(2);

    QTest::keyClick(this->test_dialog_d->marginBottomSpin->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    ASSERT_EQ(this->test_dialog_d->marginBottomSpin->value(), 2);
    // TODO: marginBottomSpin->lineEdit()
}

TEST_F(ut_DPrintPreviewDialog, copycountspinbox_lineEdit)
{
    ASSERT_FALSE(this->test_dialog_d->copycountspinbox->isEnabled());

    ASSERT_EQ(this->test_dialog_d->copycountspinbox->value(), 1);

    this->test_dialog_d->copycountspinbox->setValue(2);

    QTest::keyClick(this->test_dialog_d->copycountspinbox->lineEdit(), Qt::Key_Enter, Qt::NoModifier);

    ASSERT_EQ(this->test_dialog_d->copycountspinbox->value(), 2);

    // TODO: copycountspinbox
}

TEST_F(ut_DPrintPreviewDialog, inclinatBox_lineEdit)
{
    // 显示advanced菜单
    QTest::mouseClick(this->test_dialog_d->advanceBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->advancesettingwdg->isHidden());

    // 显示waterMark菜单
    QTest::mouseClick(this->test_dialog_d->waterMarkBtn, Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_FALSE(this->test_dialog_d->watermarksettingwdg->isHidden());

    ASSERT_EQ(this->test_dialog_d->inclinatBox->value(), 30);
    ASSERT_STREQ(this->test_dialog_d->inclinatBox->text().toLocal8Bit(), "30°");

    QTest::mouseClick(this->test_dialog_d->inclinatBox->lineEdit(), Qt::LeftButton, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    this->test_dialog_d->inclinatBox->setValue(123);

    QTest::keyClick(this->test_dialog_d->inclinatBox, Qt::Key_Enter, Qt::NoModifier);

    QTest::qWait(DELAY_TIME);

    ASSERT_EQ(this->test_dialog_d->inclinatBox->value(), 123);
    ASSERT_STREQ(this->test_dialog_d->inclinatBox->text().toLocal8Bit(), "123°");
    // TODO: inclinatBox
}

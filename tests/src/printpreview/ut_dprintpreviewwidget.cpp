#include <gtest/gtest.h>
#include <QTest>

#include "dprintpreviewwidget.h"
#include "dprintpreviewdialog.h"

DWIDGET_USE_NAMESPACE

class ut_DPrintPreviewWidget : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DPrintPreviewWidget *previewWidget = nullptr;
};
void ut_DPrintPreviewWidget::SetUp()
{
    DPrintPreviewDialog printDialog;
    printDialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&printDialog));

    previewWidget = printDialog.findChild<DPrintPreviewWidget *>();
}

void ut_DPrintPreviewWidget::TearDown()
{
}

TEST_F(ut_DPrintPreviewWidget, testForInit)
{
    ASSERT_TRUE(previewWidget);
}

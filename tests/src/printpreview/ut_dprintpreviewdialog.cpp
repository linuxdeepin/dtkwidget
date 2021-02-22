#include <gtest/gtest.h>
#include <QTest>

#include "dprintpreviewwidget.h"
#include "dprintpreviewdialog.h"
#include "dprintpreviewdialog_p.h"

DWIDGET_USE_NAMESPACE

class ut_DPrintPreviewDialog : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DPrintPreviewDialog *printDialog;
    DPrintPreviewDialogPrivate *dialog_d;
};
void ut_DPrintPreviewDialog::SetUp()
{
    printDialog = new DPrintPreviewDialog;
    printDialog->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(printDialog));
    dialog_d = printDialog->d_func();
    ASSERT_TRUE(dialog_d);
}

void ut_DPrintPreviewDialog::TearDown()
{
    delete printDialog;
}

TEST_F(ut_DPrintPreviewDialog, testForInit)
{
    ASSERT_TRUE(dialog_d->printer);
}

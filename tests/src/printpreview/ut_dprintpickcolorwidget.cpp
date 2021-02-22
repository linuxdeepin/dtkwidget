#include <gtest/gtest.h>
#include <QTest>

#include "dprintpickcolorwidget.h"

DWIDGET_USE_NAMESPACE

class ut_DPrintColorPickWidget : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DPrintPickColorWidget *pickWidget;
};

void ut_DPrintColorPickWidget::SetUp()
{
    pickWidget = new DPrintPickColorWidget(nullptr);
    pickWidget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(pickWidget));
}

void ut_DPrintColorPickWidget::TearDown()
{
    delete pickWidget;
}

TEST_F(ut_DPrintColorPickWidget, testForInit)
{
    ASSERT_TRUE(pickWidget);
}

#include <gtest/gtest.h>
#include <QTest>
#include <DLineEdit>
#include <DIconButton>
#include <QSignalSpy>

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
    ASSERT_FALSE(pickWidget->btnlist.isEmpty());
    ASSERT_FALSE(pickWidget->colorList.isEmpty());
    ASSERT_TRUE(pickWidget->btnGroup);
    ASSERT_TRUE(pickWidget->valueLineEdit);
    ASSERT_TRUE(pickWidget->pickColorBtn);
    ASSERT_TRUE(pickWidget->rEdit);
    ASSERT_TRUE(pickWidget->gEdit);
    ASSERT_TRUE(pickWidget->bEdit);
    ASSERT_TRUE(pickWidget->colorLabel);
    ASSERT_TRUE(pickWidget->colorSlider);
}

TEST_F(ut_DPrintColorPickWidget, testFunction)
{
    // 测试函数调用是否出现异常
    pickWidget->setRgbEdit(Qt::yellow, false);
    ASSERT_EQ(pickWidget->rEdit->text(), QString("%1").arg(QColor(Qt::yellow).red()));
    ASSERT_EQ(pickWidget->gEdit->text(), QString("%1").arg(QColor(Qt::yellow).green()));
    ASSERT_EQ(pickWidget->bEdit->text(), QString("%1").arg(QColor(Qt::yellow).blue()));

    pickWidget->convertColor(Qt::darkCyan, true);
    ASSERT_EQ(pickWidget->valueLineEdit->text(), QColor(Qt::darkCyan).name().remove('#'));

    pickWidget->convertColor(Qt::blue, false);
    pickWidget->slotColorPick(QString::number(qApp->applicationPid()), QColor(Qt::darkMagenta).name());
    pickWidget->slotEditColor(QStringLiteral("FFF0F5"));
    QList<ColorButton *> btnList = pickWidget->btnlist;
    ASSERT_TRUE(std::all_of(btnList.begin(), btnList.end(), [](ColorButton *btn) { return !btn->isChecked(); }));
    pickWidget->slotEditColor(QStringLiteral("ff5d00"));
    ASSERT_FALSE(std::all_of(btnList.begin(), btnList.end(), [](ColorButton *btn) { return !btn->isChecked(); }));

    QSignalSpy pickedColorSpy(pickWidget->colorLabel, SIGNAL(pickedColor(QColor)));
    Q_EMIT pickWidget->colorLabel->pickedColor(Qt::gray);
    ASSERT_EQ(pickedColorSpy.count(), 1);

    ColorButton *colorBtn = pickWidget->btnlist.first();
    QSignalSpy selectButtonSpy(colorBtn, SIGNAL(selectColorButton(QColor)));
    Q_EMIT colorBtn->selectColorButton(Qt::blue);
    ASSERT_EQ(selectButtonSpy.count(), 1);

    QSignalSpy valueChangedSpy(pickWidget->colorSlider, SIGNAL(valueChanged(int)));
    Q_EMIT pickWidget->colorSlider->valueChanged(10);
    ASSERT_EQ(selectButtonSpy.count(), 1);
}

TEST_F(ut_DPrintColorPickWidget, testWidgetFunction)
{
    ColorLabel *label = pickWidget->colorLabel;
    ASSERT_TRUE(pickWidget->colorLabel);

    qreal h, s, v;
    h = 0;
    s = 0.78;
    v = 0.85;
    QColor color = label->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 60;
    color = label->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 120;
    color = label->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 180;
    color = label->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 240;
    color = label->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 300;
    color = label->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    label->setHue(100);
    ASSERT_EQ(label->m_hue, 100);

    QSignalSpy pickedColorSpy(label, SIGNAL(pickedColor(QColor)));
    label->pickColor({20, 20});
    ASSERT_TRUE(label->m_pickedColor.isValid());
    ASSERT_EQ(pickedColorSpy.count(), 1);
    ASSERT_EQ(pickedColorSpy.takeFirst().at(0).type(), QVariant::Color);

    QCursor cursor = label->pickColorCursor();
    ASSERT_FALSE(cursor.pixmap().isNull());

    // 测试函数正常执行且能成功渲染
    QPaintEvent labelPaint(label->rect());
    qApp->sendEvent(label, &labelPaint);
    QPixmap pixmap(50, 50);

    QPainter p(&pixmap);
    label->render(&p);
    ASSERT_FALSE(pixmap.isNull());

    QEvent labelLeave(QEvent::Leave);
    QMouseEvent labelMousePress(QMouseEvent::MouseButtonPress, {20, 20}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QMouseEvent labelMouseMove(QMouseEvent::MouseMove, {20, 20}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QMouseEvent labelMouseRelease(QMouseEvent::MouseButtonRelease, {20, 20}, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    qApp->sendEvent(label, &labelLeave);
    qApp->sendEvent(label, &labelMousePress);
    qApp->sendEvent(label, &labelMouseMove);
    qApp->sendEvent(label, &labelMouseRelease);

    ColorSlider *slider = pickWidget->colorSlider;
    ASSERT_FALSE(slider->m_backgroundImage.isNull());

    h = 0;
    color = slider->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 60;
    color = slider->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 120;
    color = slider->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 180;
    color = slider->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 240;
    color = slider->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    h = 300;
    color = slider->getColor(h, s, v);
    ASSERT_TRUE(color.isValid());

    pixmap.fill(Qt::transparent);
    QPaintEvent sliderPaint(slider->rect());
    qApp->sendEvent(slider, &sliderPaint);
    slider->render(&p);
    ASSERT_FALSE(pixmap.isNull());

    ColorButton *cButton = pickWidget->btnlist.first();
    QSignalSpy selectSpy(cButton, SIGNAL(selectColorButton(QColor)));
    QTest::mouseClick(cButton, Qt::LeftButton);
    ASSERT_EQ(selectSpy.count(), 1);
    ASSERT_EQ(selectSpy.takeFirst().at(0), cButton->m_color);

    pixmap.fill(Qt::transparent);
    QPaintEvent cbtnPaint(cButton->rect());
    qApp->sendEvent(cButton, &cbtnPaint);
    cButton->render(&p);
    ASSERT_FALSE(pixmap.isNull());
}

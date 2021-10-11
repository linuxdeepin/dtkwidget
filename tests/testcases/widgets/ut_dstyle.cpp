/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>

#include <DGuiApplicationHelper>
#include <DStyleOptionButton>
#include <QTest>
#include <QTextLayout>
#include <QPainterPath>

#include "dstyle.h"
DWIDGET_USE_NAMESPACE
class ut_DStyle : public testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

class MyWidget : public QWidget {
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {

    }
    virtual ~MyWidget(){}

    // QWidget interface
protected:
    void paintEvent(QPaintEvent */*event*/)
    {
        DStyleOptionButton option;
        QPainter painter(this);
        DStyle::drawPrimitive(this->style(), DStyle::PE_ItemBackground, &option, &painter);
        DStyle::drawPrimitive(this->style(), DStyle::PE_IconButtonPanel, &option, &painter);

        {
            DStyleOptionButtonBoxButton opt;
            DStyle::drawControl(this->style(), DStyle::CE_ButtonBoxButtonBevel, &opt, &painter);

            DDrawUtils::drawShadow(&painter, QRect(0, 0, 10, 10), 1, 1, Qt::red, 1, QPoint(0, 1));
            DDrawUtils::drawShadow(&painter, QRect(0, 0, 10, 10), QPainterPath(QPointF(1, 1)), Qt::red, 1, QPoint(0, 1));

            DDrawUtils::drawFork(&painter, QRectF(0, 0, 10, 10), Qt::red);
            DDrawUtils::drawIndicatorChecked(&painter, QRect(0, 0, 1, 1));
            DDrawUtils::drawArrow(&painter, QRectF(1, 1, 1, 1), Qt::red, Qt::UpArrow);
            DDrawUtils::drawPlus(&painter, QRectF(1, 1, 1, 1), Qt::red, 1);
            DDrawUtils::drawSubtract(&painter, QRectF(1, 1, 1, 1), Qt::red, 1);
        }
        {
            DDrawUtils::drawSelectElement(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawExpandElement(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawExpandElement(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawReduceElement(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawLockElement(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawUnlockElement(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowEnter(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowLeave(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowNext(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowPrev(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawCloseButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawIndicatorUnchecked(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawIndicatorChecked(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawDeleteButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawAddButton(&painter, QRectF(1, 1, 1, 1));

            DDrawUtils::drawTitleBarMenuButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawTitleBarMinButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawTitleBarMaxButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawTitleBarCloseButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawTitleBarNormalButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawTitleQuitFullButton(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowUp(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowDown(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowLeft(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowRight(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowBack(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawArrowForward(&painter, QRectF(1, 1, 1, 1));
            DDrawUtils::drawLineEditClearButton(&painter, QRectF(1, 1, 1, 1));
        }

        DStyle::drawControl(style(), DStyle::CE_IconButton, &option, &painter);

        DStyle::subElementRect(this->style(), DStyle::SE_IconButtonIcon, &option);

        QTextLayout textLayout("text");
        DStyle::viewItemTextLayout(textLayout, 1);
        QStyleOptionViewItem itemOpt;
        DStyle::viewItemSize(this->style(), &itemOpt, Qt::DisplayRole);
        QRect pxRect;
        QRect textRect;
        QRect checkRect;
        DStyle::viewItemLayout(this->style(), &itemOpt, &pxRect, &textRect, &checkRect, true);
        DStyle::viewItemDrawText(this->style(), &painter, &itemOpt, QRect(0, 0, 10, 10));
    }
};

class StyleWidget : public QWidget {
    Q_OBJECT

public:
    explicit StyleWidget(QWidget *parent = nullptr)
        : QWidget(parent),
          dstyle(new DStyle)
    {
    }
    virtual ~StyleWidget(){
        dstyle->deleteLater();
    }

    // QWidget interface
protected:
    void paintEvent(QPaintEvent */*event*/)
    {
        DStyleOptionButton option;
        QPainter painter(this);
        dstyle->drawPrimitive(DStyle::PE_ItemBackground, &option, &painter);
        dstyle->drawPrimitive(DStyle::PE_IndicatorArrowUp, &option, &painter);
        dstyle->drawPrimitive(DStyle::PE_IndicatorArrowDown, &option, &painter);
        dstyle->drawPrimitive(DStyle::PE_IndicatorArrowRight, &option, &painter);
        dstyle->drawPrimitive(DStyle::PE_IndicatorArrowLeft, &option, &painter);

        dstyle->drawControl(DStyle::CE_IconButton, &option, &painter);

        dstyle->subElementRect(DStyle::SE_IconButtonIcon, &option);
        dstyle->standardIcon(QStyle::SP_TitleBarMenuButton, &option);

        QStyleHintReturn hintReturn;
        ASSERT_EQ(dstyle->styleHint(QStyle::SH_Menu_SubMenuPopupDelay, &option, this, &hintReturn), 100);

        QBrush base(Qt::red);
        QBrush b1 = dstyle->generatedBrush(&option, base, QPalette::Normal, QPalette::NoRole);
        QBrush b2 = dstyle->generatedBrush(&option, base, QPalette::Normal, DPalette::ItemBackground);
        QBrush b3 = dstyle->generatedBrush(DStyle::SS_NormalState, base, QPalette::Normal, DPalette::ItemBackground);
        QPixmap p1 = dstyle->generatedIconPixmap(QIcon::Active, QPixmap(10, 10), &option);

        QStyleOptionViewItem itemOpt;
        dstyle->viewItemDrawText(&painter, &itemOpt, QRect(0, 0, 10, 10));

        dstyle->sizeFromContents(DStyle::CT_IconButton, &option, QSize(10, 10));

    }
    DStyle* dstyle = nullptr;
};

TEST_F(ut_DStyle, staticFunc)
{
    QColor base(Qt::red);
    DStyle::adjustColor(base);

    QColor substrate(Qt::red);
    QColor superstratum(Qt::yellow);
    DStyle::blendColor(substrate, superstratum);

    DStyle::setTooltipTextFormat(Qt::RichText);
    ASSERT_EQ(DStyle::tooltipTextFormat(), Qt::RichText);

    QWidget *widget = new MyWidget;
    widget->show();
    DStyle::setFrameRadius(widget, 1);
    ASSERT_EQ(widget->property("_d_dtk_frameRadius").toInt(), 1);

    DStyle::setFocusRectVisible(widget, true);
    ASSERT_EQ(widget->property("_d_dtk_noFocusRect").toBool(), false);

    DStyleOptionButton option;
    option.state |= DStyle::State_Enabled;
    ASSERT_EQ(DStyle::getState(&option), DStyle::SS_NormalState);

    DStyle::pixelMetric(widget->style(), DStyle::PM_FrameRadius, &option);

    DStyle::sizeFromContents(widget->style(), DStyle::CT_IconButton, &option, QSize(10, 10));
    DStyle::sizeFromContents(widget->style(), DStyle::CT_SwitchButton, &option, QSize(10, 10));

    DStyle::standardIcon(widget->style(), DStyle::SP_DecreaseElement);

    widget->deleteLater();
};

TEST_F(ut_DStyle, dstyleFunc)
{
    QWidget *widget = new StyleWidget;
    widget->setProperty("_d_dtk_newNotification", true);
    widget->show();

    widget->deleteLater();
};

class ut_DStylePainter : public testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

class PainterWidget : public QWidget {
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {

    }
    virtual ~PainterWidget(){}

    // QWidget interface
protected:
    void paintEvent(QPaintEvent */*event*/)
    {
        DStyleOptionButton option;
        DStylePainter painter(this);
        painter.drawPrimitive(QStyle::PE_Frame, option);
        painter.drawPrimitive(DStyle::PE_ItemBackground, option);
        painter.drawControl(QStyle::CE_PushButton, option);
        painter.drawControl(DStyle::CE_IconButton, option);
        QStyleOptionComplex cOpt;
        painter.drawComplexControl(QStyle::CC_SpinBox, cOpt);
        painter.drawItemText(QRect(0, 0, 1, 1), 0, DGuiApplicationHelper::instance()->applicationPalette(), true, "text");
        painter.drawItemPixmap(QRect(0, 0, 1, 1), 0, QPixmap(10, 10));
    }
};

TEST_F(ut_DStylePainter, painterWidget)
{
    QWidget *widget = new PainterWidget;
    widget->show();

    widget->deleteLater();
};

class ut_DStyledIconEngine : public testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

class IconEngineWidget : public QWidget {
    Q_OBJECT

public:
    explicit IconEngineWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
    }
    virtual ~IconEngineWidget(){
        if (engine) {
            delete engine;
            engine = nullptr;
        }
    }

    // QWidget interface
protected:
    void paintEvent(QPaintEvent */*event*/)
    {
        if (!engine) {
            auto drawFun = [](QPainter * painter, const QRectF &rect){
                painter->drawText(rect, "drawFun");
            };
            engine = new DStyledIconEngine(drawFun);
        }
        QPainter painter;
        engine->paint(&painter, DGuiApplicationHelper::instance()->applicationPalette(), QRect(1, 1, 1, 1));
        engine->paint(&painter, QRect(1, 1, 1, 1), QIcon::Normal, QIcon::On);

        DStyledIconEngine::drawIcon(QIcon::fromTheme("preferences-system"), &painter, QRect(1, 1, 1, 1));

        DStyleOptionIcon option;
        DStyle::drawPrimitive(this->style(), DStyle::PE_Icon, &option, &painter);

        QPixmap px = engine->pixmap(QSize(10, 10), QIcon::Normal, QIcon::On);
        ASSERT_FALSE(px.isNull());
        engine->setFrontRole(this, QPalette::WindowText);
        engine->setIconName("icon");
        engine->bindDrawFun([](QPainter * painter, const QRectF &rect){
            painter->drawText(rect, "drawFun2");
        });

        auto cloneEngine = engine->clone();
        ASSERT_TRUE(cloneEngine);
        delete cloneEngine;
    }
private:
    DStyledIconEngine *engine = nullptr;
};

TEST_F(ut_DStyledIconEngine, dStyledIconEngine)
{
    QWidget *widget = new IconEngineWidget;
    widget->show();

    widget->deleteLater();
};


#include "ut_dstyle.moc"

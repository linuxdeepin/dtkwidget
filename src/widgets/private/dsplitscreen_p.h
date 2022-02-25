/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DSPLITSCREEN_P_H
#define DSPLITSCREEN_P_H

#include <dtkwidget_global.h>

#include <DArrowRectangle>
#include <DGuiApplicationHelper>
#include <DIconButton>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DSplitScreenButton : public DIconButton
{
    Q_OBJECT
public:
    explicit DSplitScreenButton(DStyle::StandardPixmap sp, QWidget *parent = nullptr);

protected:
    void initStyleOption(DStyleOptionButton *option) const;
};

class DSplitScreenWidget : public DArrowRectangle
{
    Q_OBJECT
public:
    enum SplitScreenMode {
        SplitLeftHalf = 1,
        SplitRightHalf = 2,
        SplitFullScreen = 15
    };
    Q_ENUM(SplitScreenMode)

    explicit DSplitScreenWidget(DSplitScreenWidget::FloatMode mode, QWidget *parent = nullptr);

    void hide();
    void hideImmediately();
    void updateMaximizeButtonIcon(bool isMaximized);
    void setButtonsEnable(bool enable);

Q_SIGNALS:
    void maximizeButtonClicked();
    void leftSplitScreenButtonClicked();
    void rightSplitScreenButtonClicked();

private Q_SLOTS:
    void onThemeTypeChanged(DGuiApplicationHelper::ColorType ct);

protected:
    void init();
    void disabledByScreenGeometryAndWindowSize(QWidgetList w);
    bool eventFilter(QObject *o, QEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

private:
    DSplitScreenButton *leftSplitButton         = nullptr;
    DSplitScreenButton *rightSplitButton        = nullptr;
    DSplitScreenButton *maximizeButton          = nullptr;

    QWidget *contentWidget = nullptr;
    QBasicTimer hideTimer;
    DArrowRectangle::FloatMode floatMode;
    bool isMaxButtonPressAndHold = false;

    friend class DTitlebarPrivate;
    friend class DTitlebar;
};

DWIDGET_END_NAMESPACE
#endif // DSPLITSCREEN_P_H

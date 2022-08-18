// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSPLITSCREEN_P_H
#define DSPLITSCREEN_P_H

#include <dtkwidget_global.h>

#include <QTimer>

#include "DGuiApplicationHelper"
#include "dblureffectwidget.h"
#include "dflowlayout.h"
#include "dframe.h"
#include "DPushButton"

DWIDGET_BEGIN_NAMESPACE

class DSplitScreenPlaceholder :  public DPushButton
{
    Q_OBJECT
public:
    enum PositionFlag {
        Left = 1 << 0,
        Right = 1 << 1,
        Top = 1 << 2,
        Bottom = 1 << 3,
    };
    Q_DECLARE_FLAGS(Position, PositionFlag)
    Q_FLAG(Position)

    explicit DSplitScreenPlaceholder(Position position, QWidget *parent = nullptr);
    DSplitScreenPlaceholder::Position position() const;

    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    Position m_position;
    bool m_paintFocus = false;
};

class DSplitScreenCell : public DFrame
{
    Q_OBJECT
public:
    enum ModeFlag {
        TwoSplit = 1,
        ThreeSplit = 2,
        FourSplit = 4,
        SupportTwoSplit = TwoSplit,
        SupportFourSplit = ThreeSplit,
        PositionType = 1 << 4,
        Left = 1 << (PositionType + 1)
    };
    Q_DECLARE_FLAGS(Mode, ModeFlag)
    Q_FLAG(Mode)

    DSplitScreenCell(const Mode mode, QWidget *parent = nullptr);

Q_SIGNALS:
    void screenSelected(DSplitScreenCell::Mode type, DSplitScreenPlaceholder::Position position);
private Q_SLOTS:
    void onScreenPlaceholderSelected();
private:
    QVector<DSplitScreenPlaceholder::Position> positionsByScreenMode(const DSplitScreenCell::Mode mode);

    DFlowLayout *layout = nullptr;
    DSplitScreenCell::Mode m_type;
};

class DSplitScreenWidget : public DBlurEffectWidget
{
    Q_OBJECT
public:

    explicit DSplitScreenWidget(QWidget *parent = nullptr);

    void hide();
    void hideImmediately();
    static bool supportSplitScreenByWM(const QWidget *window);
    void show(const QPoint &pos);

private Q_SLOTS:
    void onThemeTypeChanged(DGUI_NAMESPACE::DGuiApplicationHelper::ColorType ct);
    void onScreenSelected(DSplitScreenCell::Mode type, DSplitScreenPlaceholder::Position position);

protected:
    void init();
    bool disableByScreenGeometryAndWindowSize() const;
    bool eventFilter(QObject *o, QEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

private:
    QBasicTimer hideTimer;
    bool isMaxButtonPressAndHold = false;

    friend class DTitlebarPrivate;
    friend class DTitlebar;
};

DWIDGET_END_NAMESPACE

Q_DECLARE_OPERATORS_FOR_FLAGS(DTK_WIDGET_NAMESPACE::DSplitScreenPlaceholder::Position)
Q_DECLARE_OPERATORS_FOR_FLAGS(DTK_WIDGET_NAMESPACE::DSplitScreenCell::Mode)
#endif // DSPLITSCREEN_P_H

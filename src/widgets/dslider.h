// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSLIDER_H
#define DSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QStyleOption>
#include <QPainter>

#include <dtkwidget_global.h>
#include <dobject.h>

DWIDGET_BEGIN_NAMESPACE

class DSliderPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSlider : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DSlider)
    D_DECLARE_PRIVATE(DSlider)
public:
    enum SliderIcons {
        LeftIcon,
        RightIcon
    };
    DSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);

    Qt::Orientation orientation() const;

    QSlider *slider();

    void setLeftIcon(const QIcon &left);
    void setRightIcon(const QIcon &right);

    void setIconSize(const QSize &size);

    void setMinimum(int min);
    int minimum() const;

    void setValue(int value);
    int value() const;

    void setPageStep(int pageStep);
    int pageStep() const;

    void setMaximum(int max);
    int maximum() const;

    void setLeftTicks(const QStringList &info);
    void setRightTicks(const QStringList &info);

    void setAboveTicks(const QStringList &info);
    void setBelowTicks(const QStringList &info);

    void setMarkPositions(QList<int> list);

    void setMouseWheelEnabled(bool enabled);

    void setTipValue(const QString &value);

    QSlider::TickPosition tickPosition() const;
    QSize sizeHint() const override;

    void setHandleVisible(bool b);
    bool handleVisible() const;

    void setEnabledAcrossStyle(bool enabled);

Q_SIGNALS:
    void valueChanged(int value);

    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();

    void rangeChanged(int min, int max);

    void actionTriggered(int action);
    void iconClicked(SliderIcons icon, bool checked);

protected:
    DSlider(DSliderPrivate &q, QWidget *parent);

    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
};

class SpecialSlider : public QSlider {
public:
    SpecialSlider(Qt::Orientation orientation, QWidget *parent = nullptr) : QSlider(orientation, parent) {
    }

    void paintEvent(QPaintEvent *ev) {
        Q_UNUSED(ev)
        QPainter p(this);
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        DSlider* dSlider = qobject_cast<DSlider *>(this->parent());

        if (!dSlider)
            return;

        if (dSlider->handleVisible())
            opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
        else
            opt.subControls = QStyle::SC_SliderGroove;

        style()->drawComplexControl(QStyle::CC_Slider, &opt, &p, parentWidget());
    }
};

DWIDGET_END_NAMESPACE

#endif // DSLIDER_H

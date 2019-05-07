/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DSTACKWIDGET_H
#define DSTACKWIDGET_H

#include <QWidget>
#include <QVariantAnimation>

#include "dobject.h"
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DStackWidget;
class DAbstractStackWidgetTransitionPrivate;
class DAbstractStackWidgetTransition : public QObject, public DTK_CORE_NAMESPACE::DObject
{
public:
    enum TransitionType{
        Push,
        Pop
    };

    struct TransitionInfo{
        TransitionType type;
        DStackWidget *stackWidget = nullptr;
        QWidget *oldWidget = nullptr;
        QWidget *newWidget = nullptr;
    };

    explicit DAbstractStackWidgetTransition(QObject *parent = 0);

    virtual void beginTransition(const TransitionInfo &info);
    virtual QVariantAnimation *animation() const;

protected:
    virtual void updateVariant(const QVariant& variant) = 0;

protected:
    explicit DAbstractStackWidgetTransition(DAbstractStackWidgetTransitionPrivate &dd,
                                            QObject *parent = 0);

    const TransitionInfo &info() const;

private:
    D_DECLARE_PRIVATE(DAbstractStackWidgetTransition)
};

class DSlideStackWidgetTransition : public DAbstractStackWidgetTransition
{
    Q_OBJECT

public:
    explicit DSlideStackWidgetTransition(QObject *parent = 0);

    void beginTransition(const TransitionInfo &info) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void updateVariant(const QVariant &variant) Q_DECL_OVERRIDE;
};

class DStackWidgetPrivate;
class DStackWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    ///busy is true if a transition is running, and false otherwise.
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged FINAL)
    ///The number of widgets currently pushed onto the stack.
    Q_PROPERTY(int depth READ depth NOTIFY depthChanged FINAL)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QWidget* currentWidget READ currentWidget NOTIFY currentWidgetChanged)
    Q_PROPERTY(DAbstractStackWidgetTransition* transition READ transition WRITE setTransition)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(QEasingCurve::Type animationType READ animationType WRITE setAnimationType)

public:
    explicit DStackWidget(QWidget *parent = 0);

    bool busy() const;
    int depth() const;

    int currentIndex() const;
    QWidget* currentWidget() const;

    DAbstractStackWidgetTransition* transition() const;
    int animationDuration() const;
    QEasingCurve::Type animationType() const;

public Q_SLOTS:
    int pushWidget(QWidget *widget, bool enableTransition = true);
    void insertWidget(int index, QWidget *widget, bool enableTransition = true);

    /// If widget is nullptr, all widgets up to the currentIndex+count widgets will be popped.
    /// If not specified, all widgets up to the depthOf(widget)+count widgets will be popped.
    void popWidget(QWidget *widget = nullptr, bool isDelete = true,
                  int count = 1, bool enableTransition = true);
    void clear();

    int indexOf(QWidget *widget) const;
    QWidget* getWidgetByIndex(int index) const;

    void setTransition(DAbstractStackWidgetTransition* transition);
    void setAnimationDuration(int animationDuration);
    void setAnimationType(QEasingCurve::Type animationType);

Q_SIGNALS:
    void busyChanged(bool busy);
    void depthChanged(int depth);

    void currentIndexChanged(int currentIndex);
    void currentWidgetChanged(QWidget* currentWidget);

    void widgetDepthChanged(QWidget *widget, int depth);

    void switchWidgetFinished();

protected:
    explicit DStackWidget(DStackWidgetPrivate &dd, QWidget *parent = 0);

    void setCurrentIndex(int currentIndex,
                         DAbstractStackWidgetTransition::TransitionType type = DAbstractStackWidgetTransition::Push,
                         bool enableTransition = true);
    void setCurrentWidget(QWidget* currentWidget,
                          DAbstractStackWidgetTransition::TransitionType type = DAbstractStackWidgetTransition::Push,
                          bool enableTransition = true);

private:
    Q_DISABLE_COPY(DStackWidget)
    D_DECLARE_PRIVATE(DStackWidget)
};

DWIDGET_END_NAMESPACE

#endif // DSTACKWIDGET_H

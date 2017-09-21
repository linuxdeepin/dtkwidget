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

#ifndef DSWITCHBUTTON_H
#define DSWITCHBUTTON_H

#include <QFrame>
#include <QPixmap>
#include <QVariantAnimation>
#include <QUrl>

#include "dtkwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE
class DSwitchButtonPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSwitchButton : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(double animationStartValue READ animationStartValue WRITE setAnimationStartValue)
    Q_PROPERTY(double animationEndValue READ animationEndValue WRITE setAnimationEndValue)
    Q_PROPERTY(QEasingCurve::Type animationType READ animationType WRITE setAnimationType)
    Q_PROPERTY(QString disabledImageSource READ disabledImageSource WRITE setDisabledImageSource)
    Q_PROPERTY(QString enabledImageSource READ enabledImageSource WRITE setEnabledImageSource)

public:
    explicit DSwitchButton(QWidget *parent = 0);

    bool checked() const;

    Q_DECL_DEPRECATED int animationDuration() const;
    Q_DECL_DEPRECATED QEasingCurve::Type animationType() const;
    Q_DECL_DEPRECATED QString backgroundImageSource() const;
    Q_DECL_DEPRECATED double animationStartValue() const;
    Q_DECL_DEPRECATED double animationEndValue() const;
    Q_DECL_DEPRECATED QString disabledImageSource() const;
    Q_DECL_DEPRECATED QString enabledImageSource() const;

    QSize sizeHint() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void setChecked(bool arg);

    Q_DECL_DEPRECATED void setAnimationDuration(int arg);
    Q_DECL_DEPRECATED void setAnimationType(QEasingCurve::Type arg);
    Q_DECL_DEPRECATED void setAnimationStartValue(double animationStartValue);
    Q_DECL_DEPRECATED void setAnimationEndValue(double animationEndValue);
    Q_DECL_DEPRECATED bool setDisabledImageSource(const QString &arg);
    Q_DECL_DEPRECATED bool setEnabledImageSource(const QString &arg);

Q_SIGNALS:
    void checkedChanged(bool arg);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DSwitchButton)
};

DWIDGET_END_NAMESPACE
#endif // DSWITCHBUTTON_H


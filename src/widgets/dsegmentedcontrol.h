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

#ifndef DSEGMENTEDCONTROL_H
#define DSEGMENTEDCONTROL_H

#include <QFrame>
#include <QHBoxLayout>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QList>
#include <QEasingCurve>

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSegmentedHighlight : public QToolButton
{
    Q_OBJECT

public:
    explicit DSegmentedHighlight(QWidget *parent = 0);
};

class DSegmentedControlPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSegmentedControl : public QFrame, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DSegmentedControl)

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(QEasingCurve::Type animationType READ animationType WRITE setAnimationType)
public:
    explicit DSegmentedControl(QWidget *parent = 0);

    int count() const;
    const DSegmentedHighlight *highlight() const;
    int currentIndex() const;
    QToolButton *at(int index) const;
    QString getText(int index) const;
    QIcon getIcon(int index) const;
    int animationDuration() const;
    int indexByTitle(const QString &title) const;

    QEasingCurve::Type animationType() const;

public Q_SLOTS:
    int addSegmented(const QString &title);
    int addSegmented(const QIcon &icon, const QString &title);
    void addSegmented(const QStringList &titleList);
    void addSegmented(const QList<QIcon> &iconList, const QStringList &titleList);
    void insertSegmented(int index, const QString &title);
    void insertSegmented(int index, const QIcon &icon, const QString &title);
    void removeSegmented(int index);
    void clear();
    bool setCurrentIndex(int currentIndex);
    bool setCurrentIndexByTitle(const QString &title);
    void setText(int index, const QString &title);
    void setIcon(int index, const QIcon &icon);
    void setAnimationDuration(int animationDuration);
    void setAnimationType(QEasingCurve::Type animationType);

private Q_SLOTS:
    void updateHighlightGeometry(bool animation = true);
    void buttonClicked();

Q_SIGNALS:
    void currentChanged(int index);
    void currentTitleChanged(QString title);
    void animationDurationChanged(int animationDuration);

protected:
    bool eventFilter(QObject *, QEvent *) override;
};

DWIDGET_END_NAMESPACE
#endif // DSEGMENTEDCONTROL_H

// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED DSegmentedHighlight : public QToolButton
{
    Q_OBJECT

public:
    explicit DSegmentedHighlight(QWidget *parent = 0);
};

class DSegmentedControlPrivate;
class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED_X("Use DButtonBox") DSegmentedControl : public QWidget, public DCORE_NAMESPACE::DObject
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
    void resizeEvent(QResizeEvent *event) override;
};

DWIDGET_END_NAMESPACE
#endif // DSEGMENTEDCONTROL_H

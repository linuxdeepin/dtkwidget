// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef DBOUNCEANIMATION_H
#define DBOUNCEANIMATION_H

#include <DObject>
#include <QObject>

class QPropertyAnimation;
class QAbstractScrollArea;
class DBounceAnimationPrivate;
class DBounceAnimation : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DBounceAnimation(QObject *parent = nullptr);

    void setAnimationTarget(QAbstractScrollArea *w);
    void setAniMationEnable(bool enable);

protected:
    bool eventFilter(QObject *o, QEvent *e) override;
    void bounceBack(Qt::Orientation orientation);

private:
    D_DECLARE_PRIVATE(DBounceAnimation)

};

#endif // DBOUNCEANIMATION_H

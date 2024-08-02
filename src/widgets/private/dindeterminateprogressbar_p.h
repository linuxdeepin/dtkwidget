// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DINDETERMINATEPROGRESSBAR_P_H
#define DINDETERMINATEPROGRESSBAR_P_H

#include <DObjectPrivate>
#include <DIndeterminateProgressbar>

#include <QTimer>

class QPropertyAnimation;
class DIndeterminateProgressbarPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DIndeterminateProgressbarPrivate(DIndeterminateProgressbar *qq);

    QWidget *m_sliderWidget;
    QTimer *m_timer;
    bool m_leftToRight;
    QWidget *m_spotWidget;
    QPropertyAnimation *m_animation;

private:
    D_DECLARE_PUBLIC(DIndeterminateProgressbar)
};

#endif // DINDETERMINATEPROGRESSBAR_P_H

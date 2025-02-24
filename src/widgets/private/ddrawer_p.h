// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDRAWER_P_H
#define DDRAWER_P_H

#include <DDrawer>
#include "dframe_p.h"

QT_BEGIN_NAMESPACE
class QBoxLayout;
class QVBoxLayout;
class QPropertyAnimation;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DBoxWidget;
class ContentBox;
class DDrawerPrivate : public DFramePrivate
{
public:
    DDrawerPrivate(DDrawer* qq);
    ~DDrawerPrivate();

    void init();
    void updateHeightDirect();

    QWidget *m_header = nullptr;
    QWidget *m_content = nullptr;
    DBoxWidget *m_boxWidget = nullptr;
    QVBoxLayout *m_headerLayout = nullptr;
    QBoxLayout *m_contentLayout = nullptr;
    ContentBox *m_contentLoader = nullptr;
    DHorizontalLine * m_hSeparator = nullptr;
    DHorizontalLine *m_bottom_separator = nullptr;
    QPropertyAnimation *m_animation = nullptr;
    bool m_enableAnimation = false;
    bool m_expand = false;
    bool m_reservedPadding[7];

    D_DECLARE_PUBLIC(DDrawer)
};

DWIDGET_END_NAMESPACE

#endif // DDRAWER_P_H

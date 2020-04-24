/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DDRAWER_P_H
#define DDRAWER_P_H

#include "ddrawer.h"
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

    QWidget *m_header = nullptr;
    QWidget *m_content = nullptr;
    DBoxWidget *m_boxWidget = nullptr;
    QVBoxLayout *m_headerLayout = nullptr;
    QBoxLayout *m_contentLayout = nullptr;
    ContentBox *m_contentLoader = nullptr;
    DHorizontalLine * m_hSeparator = nullptr;
    DHorizontalLine *m_bottom_separator = nullptr;
    QPropertyAnimation *m_animation = nullptr;
    bool m_expand = false;
    bool m_reservedPadding[7];

    D_DECLARE_PUBLIC(DDrawer)
};

DWIDGET_END_NAMESPACE

#endif // DDRAWER_P_H

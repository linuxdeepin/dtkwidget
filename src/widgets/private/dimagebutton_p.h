/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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

#ifndef DIMAGEBUTTON_P_H
#define DIMAGEBUTTON_P_H

#include "dimagebutton.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DImageButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DImageButtonPrivate(DImageButton *qq);
    ~DImageButtonPrivate();

    void updateIcon();
    void setState(DImageButton::State state);
    QPixmap loadPixmap(const QString &path);

    DImageButton::State m_state = DImageButton::Normal;

    bool m_isChecked = false;
    bool m_isCheckable = false;
    QString m_normalPic;
    QString m_hoverPic;
    QString m_pressPic;
    QString m_checkedPic;

    D_DECLARE_PUBLIC(DImageButton)
};

DWIDGET_END_NAMESPACE

#endif // DIMAGEBUTTON_P_H

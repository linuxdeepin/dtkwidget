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
#ifndef DARROWLINEDRAWER_H
#define DARROWLINEDRAWER_H

#include <DDrawer>

DWIDGET_BEGIN_NAMESPACE

class DBaseLine;
class DArrowLineDrawerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DArrowLineDrawer : public DDrawer
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DArrowLineDrawer)

public:
    explicit DArrowLineDrawer(QWidget *parent = nullptr);
    void setTitle(const QString &title);
    void setExpand(bool value);
    D_DECL_DEPRECATED DBaseLine *headerLine();

private:
    void setHeader(QWidget *header);
    void resizeEvent(QResizeEvent *e);
};

DWIDGET_END_NAMESPACE

#endif // DARROWLINEDRAWER_H

/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DFILEICONPROVIDER_H
#define DFILEICONPROVIDER_H

#include <DObject>
#include "dtkwidget_global.h"

#include <QFileIconProvider>

DWIDGET_BEGIN_NAMESPACE

class DFileIconProviderPrivate;
class DFileIconProvider : public QFileIconProvider, public DTK_CORE_NAMESPACE::DObject
{
public:
    DFileIconProvider();
    virtual ~DFileIconProvider() Q_DECL_OVERRIDE;

    static DFileIconProvider *globalProvider();

    QIcon icon(const QFileInfo &info) const Q_DECL_OVERRIDE;
    QIcon icon(const QFileInfo &info, const QIcon &feedback) const;

private:
    D_DECLARE_PRIVATE(DFileIconProvider)
    Q_DISABLE_COPY(DFileIconProvider)
};

DWIDGET_END_NAMESPACE

#endif // DFILEICONPROVIDER_H

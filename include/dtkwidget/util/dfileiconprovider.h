// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILEICONPROVIDER_H
#define DFILEICONPROVIDER_H

#include <DObject>
#include <dtkwidget_global.h>

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

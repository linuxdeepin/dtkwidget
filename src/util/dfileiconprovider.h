/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
    ~DFileIconProvider();

    static DFileIconProvider *globalProvider();

    QIcon icon(const QFileInfo &info) const Q_DECL_OVERRIDE;
    QIcon icon(const QFileInfo &info, const QIcon &feedback) const;

private:
    D_DECLARE_PRIVATE(DFileIconProvider)
    Q_DISABLE_COPY(DFileIconProvider)
};

DWIDGET_END_NAMESPACE

#endif // DFILEICONPROVIDER_H

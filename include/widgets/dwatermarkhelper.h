// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DWATERMARKHELPER_H
#define DWATERMARKHELPER_H
#include "dwatermarkwidget.h"

#include <dtkwidget_global.h>
#include <DObject>

#include <QObject>

DTK_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE
class DWaterMarkHelperPrivate;
class DWaterMarkHelper : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    static DWaterMarkHelper *instance();
    void registerWidget(QWidget *w);

    WaterMarkData data() const;
    void setData(const WaterMarkData &data);

protected:
    explicit DWaterMarkHelper(QObject *parent = nullptr);

private:
    D_DECLARE_PRIVATE(DWaterMarkHelper)
};

DWIDGET_END_NAMESPACE

#endif // DWATERMARKHELPER_H

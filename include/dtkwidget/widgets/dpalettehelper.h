// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPALETTEHELPER_H
#define DPALETTEHELPER_H

#include <dtkwidget_global.h>
#include <DPalette>
#include <DObject>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DPaletteHelperPrivate;
class DPaletteHelper : public QObject
    , public DCORE_NAMESPACE::DObject
{
    Q_OBJECT

public:
    static DPaletteHelper *instance();

    DPalette palette(const QWidget *widget, const QPalette &base = QPalette()) const;
    void setPalette(QWidget *widget, const DPalette &palette);
    void resetPalette(QWidget *widget);

private:
    DPaletteHelper(QObject *parent = nullptr);
    ~DPaletteHelper() override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    D_DECLARE_PRIVATE(DPaletteHelper)
};

DWIDGET_END_NAMESPACE

#endif // DPALETTEHELPER_H

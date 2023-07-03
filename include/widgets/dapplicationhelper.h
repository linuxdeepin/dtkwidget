// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAPPLICATIONHELPER_H
#define DAPPLICATIONHELPER_H

#include <dtkwidget_global.h>
#include <DGuiApplicationHelper>
#include <DPalette>

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)

DWIDGET_BEGIN_NAMESPACE

DGUI_USE_NAMESPACE

class D_DECL_DEPRECATED_X("Use DPaletteHelper") DApplicationHelper : public DGuiApplicationHelper
{
    Q_OBJECT

public:
    static DApplicationHelper *instance();

    DPalette palette(const QWidget *widget, const QPalette &base = QPalette()) const;
    void setPalette(QWidget *widget, const DPalette &palette);
    void resetPalette(QWidget *widget);

private:
    DApplicationHelper();
    ~DApplicationHelper();

    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *event) override;

    friend class _DApplicationHelper;
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATIONHELPER_H

#endif

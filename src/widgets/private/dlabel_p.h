// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLABEL_P_H
#define DLABEL_P_H

#include <DLabel>
#include <DStyle>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DLabelPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DLabel)

public:
    DLabelPrivate(DLabel *q);

    void init();
    static Qt::LayoutDirection textDirection(QLabelPrivate *d);
    static QRectF documentRect(QLabelPrivate *d);
    static QRectF layoutRect(QLabelPrivate *d);
    static void ensureTextLayouted(QLabelPrivate *d);

    DPalette::ColorType color = DPalette::NoType;
    Qt::TextElideMode elideMode = Qt::ElideNone;
};

DWIDGET_END_NAMESPACE

#endif // DLABEL_P_H

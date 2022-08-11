// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFRAME_P_H
#define DFRAME_P_H

#include <DStyle>
#include <DFrame>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE


class DFramePrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DFramePrivate(DFrame *qq);
    bool frameRounded;
    DPalette::ColorType backType;

    D_DECLARE_PUBLIC(DFrame)
};

DWIDGET_END_NAMESPACE

#endif // DFRAME_P_H

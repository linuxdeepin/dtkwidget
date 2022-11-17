// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dimageviewerlibs_p.h"

#include <QLibrary>

DLibFreeImage::DLibFreeImage()
{
    freeImage = new QLibrary("freeimage", "3");

    if (!freeImage->load()) {
        delete freeImage;
        freeImage = nullptr;
        return;
    }

#define INIT_FUNCTION(Name) Name = reinterpret_cast<decltype (Name)>(freeImage->resolve(#Name)); Q_ASSERT(Name)

    INIT_FUNCTION(FreeImage_Load);
    INIT_FUNCTION(FreeImage_Save);
    INIT_FUNCTION(FreeImage_GetFileType);
    INIT_FUNCTION(FreeImage_GetWidth);
    INIT_FUNCTION(FreeImage_GetHeight);
}

DLibFreeImage::~DLibFreeImage()
{
    if (freeImage) {
        delete freeImage;
    }
}

bool DLibFreeImage::isValid()
{
    return freeImage;
}

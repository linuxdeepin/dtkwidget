// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dhidpihelper.h"

#include <DIcon>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DHiDPIHelper
  \inmodule dtkwidget
  \brief The DHiDPIHelper class provides helper to help applications support hiDPI.
 */

/*!
  \brief DHiDPIHelper::loadNxPixmap loads the suitable @Nx image.
  \a fileName is the original resource file name.
  \return the hiDPI ready QPixmap.
 */
QPixmap DHiDPIHelper::loadNxPixmap(const QString &fileName)
{
    return DTK_GUI_NAMESPACE::DIcon::loadNxPixmap(fileName);
}

DWIDGET_END_NAMESPACE

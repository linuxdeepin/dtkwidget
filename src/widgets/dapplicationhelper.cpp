/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "dapplicationhelper.h"
#include "dpalettehelper.h"

#include <QApplication>

DWIDGET_BEGIN_NAMESPACE

class _DApplicationHelper {
public:
    static DGuiApplicationHelper *createHelper()
    {
        return new DApplicationHelper();
    }
};

__attribute__((constructor)) // 在库被加载时就执行此函数
static void init_createHelper ()
{
    if (!QApplication::instance() || qobject_cast<QApplication *>(QApplication::instance())) {
        DApplicationHelper::registerInstanceCreator(_DApplicationHelper::createHelper);
    }
}

class DApplicationHelperPrivate
{
public:
    DPaletteHelper *paletteHelper = DPaletteHelper::instance();
};

static DApplicationHelperPrivate *d = nullptr;

/*!
  \class Dtk::Widget::DApplicationHelper
  \inmodule dtkwidget
  \brief DApplicationHelper提供了一个修改的 DGuiApplicationHelper 类.
 */

/*!
  \brief DApplicationHelper::instance返回 DApplicationHelper 对象
 */
DApplicationHelper *DApplicationHelper::instance()
{
    return qobject_cast<DApplicationHelper*>(DGuiApplicationHelper::instance());
}

/*!
  \brief DApplicationHelper::palette返回调色板
  \a widget 控件
  \a base 调色板
  \return 调色板
 */
DPalette DApplicationHelper::palette(const QWidget *widget, const QPalette &base) const
{
    return d->paletteHelper->palette(widget, base);
}

/*!
  \brief DApplicationHelper::setPalette将调色板设置到控件
  \a widget 控件
  \a palette 调色板
 */
void DApplicationHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    d->paletteHelper->setPalette(widget, palette);
}

/*!
  \brief DApplicationHelper::resetPalette重置控件的调色板属性
  \a widget 控件
 */
void DApplicationHelper::resetPalette(QWidget *widget)
{
    d->paletteHelper->resetPalette(widget);
}

DApplicationHelper::DApplicationHelper()
{
    if (!d)
        d = new DApplicationHelperPrivate();
}

DApplicationHelper::~DApplicationHelper()
{
    if (d) {
        delete d;
        d = nullptr;
    }
}

bool DApplicationHelper::eventFilter(QObject *watched, QEvent *event)
{
    Q_ASSERT_X(false, Q_FUNC_INFO, "This function should not be called.");

    return DGuiApplicationHelper::eventFilter(watched, event);
}

bool DApplicationHelper::event(QEvent *event)
{
    return DGuiApplicationHelper::event(event);
}

DWIDGET_END_NAMESPACE

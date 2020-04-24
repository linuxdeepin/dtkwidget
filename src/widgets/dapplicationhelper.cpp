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
#include "dstyleoption.h"

#include <QWidget>

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
    DApplicationHelper::registerInstanceCreator(_DApplicationHelper::createHelper);
}

class DApplicationHelperPrivate
{
public:
    QHash<const QWidget*, DPalette> paletteCache;
};

static DApplicationHelperPrivate *d = nullptr;

/*!
 * \~chinese \class DApplicationHelper
 * \~chinese \brief DApplicationHelper提供了一个修改的 DGuiApplicationHelper 类
 */

/*!
 * \~chinese \brief DApplicationHelper::instance返回 DApplicationHelper 对象
 */
DApplicationHelper *DApplicationHelper::instance()
{
    return qobject_cast<DApplicationHelper*>(DGuiApplicationHelper::instance());
}

/*!
 * \~chinese \brief DApplicationHelper::palette返回调色板
 * \~chinese \param widget控件
 * \~chinese \param base调色板
 * \~chinese \return 调色板
 */
DPalette DApplicationHelper::palette(const QWidget *widget, const QPalette &base) const
{
    DPalette palette;

    if (!widget) {
        return applicationPalette();
    }

    do {
        // 先从缓存中取数据
        if (d->paletteCache.contains(widget)) {
            palette = d->paletteCache.value(widget);
            break;
        }

        if (QWidget *parent = widget->parentWidget()) {
            palette = this->palette(parent, base);
        } else {
            palette = applicationPalette();
        }

        // 判断widget对象有没有被设置过palette
        if (widget->testAttribute(Qt::WA_SetPalette)) {
            // 存在自定义palette时应该根据其自定义的palette获取对应色调的DPalette
            const QPalette &wp = widget->palette();

            // 判断控件自己的palette色调是否和要继承调色板色调一致
            if (toColorType(palette) != toColorType(wp)) {
                // 不一致时则fallback到标准的palette
                palette = standardPalette(toColorType(wp));
            }
        }

        // 缓存数据
        d->paletteCache.insert(widget, palette);
        // 关注控件palette改变的事件
        const_cast<QWidget*>(widget)->installEventFilter(const_cast<DApplicationHelper*>(this));
    } while (false);

    palette.QPalette::operator =(base.resolve() ? base : widget->palette());

    return palette;
}

/*!
 * \~chinese \brief DApplicationHelper::setPalette将调色板设置到控件
 * \~chinese \param widget控件
 * \~chinese \param palette调色板
 */
void DApplicationHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    d->paletteCache.insert(widget, palette);
    // 记录此控件被设置过palette
    widget->setProperty("_d_set_palette", true);
    widget->setPalette(palette);
}

/*!
 * \~chinese \brief DApplicationHelper::resetPalette重置控件的调色板属性
 * \~chinese \param widget控件
 */
void DApplicationHelper::resetPalette(QWidget *widget)
{
    // 清理数据
    d->paletteCache.remove(widget);
    widget->setProperty("_d_set_palette", QVariant());
    widget->setAttribute(Qt::WA_SetPalette, false);
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
    if (Q_UNLIKELY(event->type() == QEvent::PaletteChange)) {
        if (QWidget *widget = qobject_cast<QWidget*>(watched)) {
            if (!widget->property("_d_set_palette").toBool()) {
                // 清理缓存
                d->paletteCache.remove(widget);
            }
        }
    }

    return DGuiApplicationHelper::eventFilter(watched, event);
}

bool DApplicationHelper::event(QEvent *event)
{
    if (event->type() == QEvent::ApplicationFontChange) {
        DFontSizeManager::instance()->setFontGenericPixelSize(DFontSizeManager::fontPixelSize(qGuiApp->font()));
    }

    return DGuiApplicationHelper::event(event);
}

DWIDGET_END_NAMESPACE

/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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

#include <DObjectPrivate>
#include <DGuiApplicationHelper>

#include "dpalettehelper.h"
#include "dstyleoption.h"
#include "private/dpalettehelper_p.h"

DWIDGET_BEGIN_NAMESPACE

static DPaletteHelper *g_instance = nullptr;

DPaletteHelperPrivate::DPaletteHelperPrivate(DPaletteHelper *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{
}

DPaletteHelper::DPaletteHelper(QObject *parent)
    : QObject(parent)
    , DTK_CORE_NAMESPACE::DObject(*new DPaletteHelperPrivate(this))
{
}

DPaletteHelper::~DPaletteHelper()
{
    if (g_instance) {
        g_instance->deleteLater();
        g_instance = nullptr;
    }
}

DPaletteHelper *DPaletteHelper::instance()
{
    if (!g_instance) {
        g_instance = new DPaletteHelper;
    }

    return g_instance;
}

/*!
 * \~chinese \brief DPaletteHelper::palette返回调色板
 * \~chinese \param widget控件
 * \~chinese \param base调色板
 * \~chinese \return 调色板
 */
DPalette DPaletteHelper::palette(const QWidget *widget, const QPalette &base) const
{
    D_DC(DPaletteHelper);

    DPalette palette;

    if (!widget) {
        return DGuiApplicationHelper::instance()->applicationPalette();
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
            palette = DGuiApplicationHelper::instance()->applicationPalette();
        }

        // 判断widget对象有没有被设置过palette
        if (widget->testAttribute(Qt::WA_SetPalette)) {
            // 存在自定义palette时应该根据其自定义的palette获取对应色调的DPalette
            const QPalette &wp = widget->palette();

            // 判断控件自己的palette色调是否和要继承调色板色调一致
            if (DGuiApplicationHelper::instance()->toColorType(palette) != DGuiApplicationHelper::instance()->toColorType(wp)) {
                // 不一致时则fallback到标准的palette
                palette = DGuiApplicationHelper::instance()->standardPalette(DGuiApplicationHelper::instance()->toColorType(wp));
            }
        }

        // 缓存数据
        const_cast<DPaletteHelperPrivate *>(d)->paletteCache.insert(widget, palette);
        // 关注控件palette改变的事件
        const_cast<QWidget *>(widget)->installEventFilter(const_cast<DPaletteHelper *>(this));
    } while (false);

    palette.QPalette::operator=(base.resolve() ? base : widget->palette());

    return palette;
}

/*!
 * \~chinese \brief DPaletteHelper::setPalette将调色板设置到控件
 * \~chinese \param widget控件
 * \~chinese \param palette调色板
 */
void DPaletteHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    D_D(DPaletteHelper);

    d->paletteCache.insert(widget, palette);
    widget->installEventFilter(const_cast<DPaletteHelper *>(this));
    // 记录此控件被设置过palette
    widget->setProperty("_d_set_palette", true);
    widget->setPalette(palette);
}

/*!
 * \~chinese \brief DPaletteHelper::resetPalette重置控件的调色板属性
 * \~chinese \param widget控件
 */
void DPaletteHelper::resetPalette(QWidget *widget)
{
    D_D(DPaletteHelper);

    // 清理数据
    d->paletteCache.remove(widget);
    widget->setProperty("_d_set_palette", QVariant());
    widget->setAttribute(Qt::WA_SetPalette, false);
}

bool DPaletteHelper::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DPaletteHelper);

    if (Q_UNLIKELY(event->type() == QEvent::PaletteChange)) {
        if (QWidget *widget = qobject_cast<QWidget *>(watched)) {
            if (!widget->property("_d_set_palette").toBool()) {
                // 清理缓存
                d->paletteCache.remove(widget);
            }
        }
    } else if (Q_UNLIKELY(event->type() == QEvent::Destroy)) {
        if (QWidget *widget = qobject_cast<QWidget *>(watched)) {
            if (d->paletteCache.contains(widget)) {
                // 清理缓存
                d->paletteCache.remove(widget);
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

bool DPaletteHelper::event(QEvent *event)
{
    if (event->type() == QEvent::ApplicationFontChange) {
        DFontSizeManager::instance()->setFontGenericPixelSize(static_cast<quint16>(DFontSizeManager::fontPixelSize(qGuiApp->font())));
    }

    return QObject::event(event);
}

DWIDGET_END_NAMESPACE

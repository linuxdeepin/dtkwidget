/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dstyle.h"

#include <QStyleOption>
#include <QTextLayout>
#include <qmath.h>
#include <private/qfixed_p.h>

DWIDGET_BEGIN_NAMESPACE

inline static int adjustColorValue(int base, qint8 increment, int max = 255)
{
    return increment > 0 ? (max - base) * increment / 100.0 + base
                         : base * (1 + increment / 100.0);
}

QColor DStyle::adjustColor(const QColor &base,
                           qint8 hueFloat, qint8 saturationFloat, qint8 lightnessFloat,
                           qint8 redFloat, qint8 greenFloat, qint8 blueFloat, qint8 alphaFloat)
{
    // 按HSL格式调整
    int H, S, L, A;
    base.getHsl(&H, &S, &L, &A);

    H = H > 0 ? adjustColorValue(H, hueFloat, 359) : H;
    S = adjustColorValue(S, saturationFloat);
    L = adjustColorValue(L, lightnessFloat);
    A = adjustColorValue(A, alphaFloat);

    QColor new_color = QColor::fromHsl(H, S, L, A);

    // 按RGB格式调整
    int R, G, B;
    new_color.getRgb(&R, &G, &B);

    R = adjustColorValue(R, redFloat);
    G = adjustColorValue(G, greenFloat);
    B = adjustColorValue(B, blueFloat);

    new_color.setRgb(R, G, B, A);

    return new_color;
}

QColor DStyle::blendColor(const QColor &substrate, const QColor &superstratum)
{
    QColor c2 = superstratum.toRgb();

    if (c2.alpha() >= 255)
        return c2;

    QColor c1 = substrate.toRgb();
    qreal c1_weight = 1 - c2.alphaF();

    int r = c1_weight * c1.red() + c2.alphaF() * c2.red();
    int g = c1_weight * c1.green() + c2.alphaF() * c2.green();
    int b = c1_weight * c1.blue() + c2.alphaF() * c2.blue();

    return QColor(r, g, b, c1.alpha());
}

DStyle::DStyle()
{

}

static DStyle::StyleState getState(const QStyleOption *option)
{
    DStyle::StyleState state = DStyle::SS_NormalState;

    if (option->state.testFlag(DStyle::State_Sunken)) {
        state = DStyle::SS_PressState;
    } else if (option->state.testFlag(DStyle::State_MouseOver)) {
        state = DStyle::SS_HoverState;
    }

    return state;
}

static DStyle::StateFlags getFlags(const QStyleOption *option)
{
    DStyle::StateFlags flags = 0;

    if (option->state.testFlag(DStyle::State_On)) {
        flags |= DStyle::SS_CheckedFlag;
    }

    if (option->state.testFlag(DStyle::State_Selected)) {
        flags |= DStyle::SS_SelectedFlag;
    }

    if (option->state.testFlag(DStyle::State_HasFocus)) {
        flags |= DStyle::SS_FocusFlag;
    }

    return flags;
}

int DStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    if (static_cast<int>(m) == PM_FrameMargins) {
        int shadow_radius = pixelMetric(PM_ShadowRadius, opt, widget);
        int shadow_xoffset = pixelMetric(PM_ShadowHOffset, opt, widget);
        int shadow_yoffset = pixelMetric(PM_ShadowVOffset, opt, widget);

        int border_width = pixelMetric(PM_FocusBorderWidth, opt, widget);
        int border_spacing = pixelMetric(PM_FocusBorderSpacing, opt, widget);

        int margins = qMax((shadow_radius + qMax(shadow_xoffset, shadow_yoffset)) / 2, border_width + border_spacing);

        return margins;
    }

    return QCommonStyle::pixelMetric(m, opt, widget);
}

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    return generatedBrush(getState(option), option, base, cg, role);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    if (auto proxy = qobject_cast<const DStyle*>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, role, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, role, option);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role, const QStyleOption *option) const
{
    Q_UNUSED(flags)
    Q_UNUSED(base)
    Q_UNUSED(cg)
    Q_UNUSED(role)
    Q_UNUSED(option)

    return base;
}

QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    return generatedBrush(getState(option), option, base, cg, type);
}

QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    if (auto proxy = qobject_cast<const DStyle*>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, type, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, type, option);
}

QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType role, const QStyleOption *option) const
{
    Q_UNUSED(flags)
    Q_UNUSED(cg)
    Q_UNUSED(role)
    Q_UNUSED(option)

    return base;
}

#if QT_CONFIG(itemviews)
QSizeF DStyle::viewItemTextLayout(QTextLayout &textLayout, int lineWidth) const
{
    qreal height = 0;
    qreal widthUsed = 0;
    textLayout.beginLayout();
    while (true) {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;
        line.setLineWidth(lineWidth);
        line.setPosition(QPointF(0, height));
        height += line.height();
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
    }
    textLayout.endLayout();
    return QSizeF(widthUsed, height);
}

QSize DStyle::viewItemSize(const QStyleOptionViewItem *option, int role) const
{
    const QWidget *widget = option->widget;
    switch (role) {
    case Qt::CheckStateRole:
        if (option->features & QStyleOptionViewItem::HasCheckIndicator)
            return QSize(proxy()->pixelMetric(QStyle::PM_IndicatorWidth, option, widget),
                         proxy()->pixelMetric(QStyle::PM_IndicatorHeight, option, widget));
        break;
    case Qt::DisplayRole:
        if (option->features & QStyleOptionViewItem::HasDisplay) {
            QTextOption textOption;
            textOption.setWrapMode(QTextOption::WordWrap);
            QTextLayout textLayout(option->text, option->font);
            textLayout.setTextOption(textOption);
            const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
            const int textMargin = proxy()->pixelMetric(QStyle::PM_FocusFrameHMargin, option, widget) + 1;
            QRect bounds = option->rect;
            switch (option->decorationPosition) {
            case QStyleOptionViewItem::Left:
            case QStyleOptionViewItem::Right: {
                if (wrapText && bounds.isValid()) {
                    int width = bounds.width() - 2 * textMargin;
                    if (option->features & QStyleOptionViewItem::HasDecoration)
                        width -= option->decorationSize.width() + 2 * textMargin;
                    bounds.setWidth(width);
                } else
                    bounds.setWidth(QFIXED_MAX);
                break;
            }
            case QStyleOptionViewItem::Top:
            case QStyleOptionViewItem::Bottom:
                if (wrapText)
                    bounds.setWidth(bounds.isValid() ? bounds.width() - 2 * textMargin : option->decorationSize.width());
                else
                    bounds.setWidth(QFIXED_MAX);
                break;
            default:
                break;
            }

            if (wrapText && option->features & QStyleOptionViewItem::HasCheckIndicator)
                bounds.setWidth(bounds.width() - proxy()->pixelMetric(QStyle::PM_IndicatorWidth) - 2 * textMargin);

            const int lineWidth = bounds.width();
            const QSizeF size = viewItemTextLayout(textLayout, lineWidth);
            return QSize(qCeil(size.width()) + 2 * textMargin, qCeil(size.height()));
        }
        break;
    case Qt::DecorationRole:
        if (option->features & QStyleOptionViewItem::HasDecoration) {
            return option->decorationSize;
        }
        break;
    default:
        break;
    }

    return QSize(0, 0);
}

void DStyle::viewItemLayout(const QStyleOptionViewItem *opt,  QRect *pixmapRect, QRect *textRect, QRect *checkRect, bool sizehint) const
{
    Q_ASSERT(checkRect && pixmapRect && textRect);
    *pixmapRect = QRect(QPoint(0, 0), viewItemSize(opt, Qt::DecorationRole));
    *textRect = QRect(QPoint(0, 0), viewItemSize(opt, Qt::DisplayRole));
    *checkRect = QRect(QPoint(0, 0), viewItemSize(opt, Qt::CheckStateRole));

    const QWidget *widget = opt->widget;
    const bool hasCheck = checkRect->isValid();
    const bool hasPixmap = pixmapRect->isValid();
    const bool hasText = textRect->isValid();
    const bool hasMargin = (hasText | hasPixmap | hasCheck);
    const int frameHMargin = hasMargin ? proxy()->pixelMetric(QStyle::PM_FocusFrameHMargin, opt, widget) + 1 : 0;
    const int textMargin = hasText ? frameHMargin : 0;
    const int pixmapMargin = hasPixmap ? frameHMargin : 0;
    const int checkMargin = hasCheck ? frameHMargin : 0;
    const int x = opt->rect.left();
    const int y = opt->rect.top();
    int w, h;

    if (textRect->height() == 0 && (!hasPixmap || !sizehint)) {
        //if there is no text, we still want to have a decent height for the item sizeHint and the editor size
        textRect->setHeight(opt->fontMetrics.height());
    }

    QSize pm(0, 0);
    if (hasPixmap) {
        pm = pixmapRect->size();
        pm.rwidth() += 2 * pixmapMargin;
    }
    if (sizehint) {
        h = qMax(checkRect->height(), qMax(textRect->height(), pm.height()));
        if (opt->decorationPosition == QStyleOptionViewItem::Left
            || opt->decorationPosition == QStyleOptionViewItem::Right) {
            w = textRect->width() + pm.width();
        } else {
            w = qMax(textRect->width(), pm.width());
        }

        int cw = 0;
        QRect check;
        if (hasCheck) {
            cw = checkRect->width() + 2 * checkMargin;
            if (sizehint) w += cw;
            // 选择标识框优先放置在右边
            if (opt->direction == Qt::RightToLeft) {
                check.setRect(x, y, cw, h);
            } else {
                check.setRect(x + w - cw, y, cw, h);
            }
        }

        QRect display;
        QRect decoration;
        switch (opt->decorationPosition) {
        case QStyleOptionViewItem::Top: {
            if (hasPixmap)
                pm.setHeight(pm.height() + pixmapMargin); // add space
            h = sizehint ? textRect->height() : h - pm.height();

            if (opt->direction == Qt::RightToLeft) {
                decoration.setRect(x, y, w - cw, pm.height());
                display.setRect(x, y + pm.height(), w - cw, h);
            } else {
                decoration.setRect(x + cw, y, w - cw, pm.height());
                display.setRect(x + cw, y + pm.height(), w - cw, h);
            }
            break; }
        case QStyleOptionViewItem::Bottom: {
            if (hasText)
                textRect->setHeight(textRect->height() + textMargin); // add space
            h = sizehint ? textRect->height() + pm.height() : h;

            if (opt->direction == Qt::RightToLeft) {
                display.setRect(x, y, w - cw, textRect->height());
                decoration.setRect(x, y + textRect->height(), w - cw, h - textRect->height());
            } else {
                display.setRect(x + cw, y, w - cw, textRect->height());
                decoration.setRect(x + cw, y + textRect->height(), w - cw, h - textRect->height());
            }
            break; }
        case QStyleOptionViewItem::Left: {
            if (opt->direction == Qt::LeftToRight) {
                decoration.setRect(x + cw, y, pm.width(), h);
                display.setRect(decoration.right() + 1, y, w - pm.width() - cw, h);
            } else {
                display.setRect(x, y, w - pm.width() - cw, h);
                decoration.setRect(display.right() + 1, y, pm.width(), h);
            }
            break; }
        case QStyleOptionViewItem::Right: {
            if (opt->direction == Qt::LeftToRight) {
                display.setRect(x + cw, y, w - pm.width() - cw, h);
                decoration.setRect(display.right() + 1, y, pm.width(), h);
            } else {
                decoration.setRect(x, y, pm.width(), h);
                display.setRect(decoration.right() + 1, y, w - pm.width() - cw, h);
            }
            break; }
        default:
            qWarning("doLayout: decoration position is invalid");
            decoration = *pixmapRect;
            break;
        }

        *checkRect = check;
        *pixmapRect = decoration;
        *textRect = display;
    } else {
        w = opt->rect.width();
        h = opt->rect.height();

        *pixmapRect = QStyle::alignedRect(opt->direction, opt->decorationAlignment,
                                          pixmapRect->size(), opt->rect.adjusted(pixmapMargin, pixmapMargin, -pixmapMargin, -pixmapMargin));

        QRect display = opt->rect;

        switch (opt->decorationPosition) {
        case QStyleOptionViewItem::Top: {
            int residue_height = opt->rect.bottom() - pixmapRect->bottom();

            // 空间不够时，要改变图标的位置来腾出空间
            if (textRect->height() > residue_height) {
                pixmapRect->moveTop(qMax(0, pixmapRect->top() - textRect->height() + residue_height));
            }

            display.setTop(pixmapRect->bottom() + pixmapMargin);
            break;
        }
        case QStyleOptionViewItem::Bottom: {
            int residue_height = pixmapRect->top() - opt->rect.top();

            // 空间不够时，要改变图标的位置来腾出空间
            if (textRect->height() > residue_height) {
                pixmapRect->moveBottom(qMin(opt->rect.bottom(), pixmapRect->bottom() + textRect->height() - residue_height));
            }

            display.setBottom(pixmapRect->top() - pixmapMargin);
            break;
        }
        case QStyleOptionViewItem::Left:
        case QStyleOptionViewItem::Right:
            if (opt->decorationPosition == QStyleOptionViewItem::Left
                    && opt->direction == Qt::LeftToRight) {
                int residue_width = pixmapRect->left() - opt->rect.left();

                // 空间不够时，要改变图标的位置来腾出空间
                if (textRect->width() > residue_width) {
                    pixmapRect->moveLeft(qMax(opt->rect.left(), pixmapRect->left() - textRect->width() + residue_width));
                }

                display.setLeft(pixmapRect->right() + pixmapMargin);
            } else {
                int residue_width = opt->rect.right() - pixmapRect->left();

                // 空间不够时，要改变图标的位置来腾出空间
                if (textRect->width() > residue_width) {
                    pixmapRect->moveRight(qMin(opt->rect.right(), pixmapRect->right() + textRect->width() - residue_width));
                }

                display.setRight(pixmapRect->left() - pixmapMargin);
            }
        default:
            break;
        }

        display.adjust(checkMargin, 0, -checkMargin, 0);
        *checkRect = QStyle::alignedRect(opt->direction, Qt::AlignRight | Qt::AlignVCenter, checkRect->size(), display);

        // 减去margins
        display.adjust(textMargin, textMargin, -textMargin, -textMargin);
        *textRect = QStyle::alignedRect(opt->direction, opt->displayAlignment, textRect->size(), display);

        if (opt->features & QStyleOptionViewItem::HasCheckIndicator)
            display.setRight(checkRect->left() - checkMargin - textMargin);

        *textRect &= display;
    }
}
#endif

DWIDGET_END_NAMESPACE

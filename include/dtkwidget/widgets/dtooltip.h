// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTOOLTIP_H
#define DTOOLTIP_H

#include <DTipLabel>
#include <dobject.h>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DToolTip : public DTipLabel
{
    Q_OBJECT

public:
    enum ToolTipShowMode {
        NotShow,
        AlwaysShow,
        ShowWhenElided,
        Default
    };
    Q_ENUM(ToolTipShowMode)

    static void setToolTipTextFormat(Qt::TextFormat format);
    static Qt::TextFormat toolTipTextFormat();
    static void setToolTipShowMode(QWidget *widget, ToolTipShowMode mode);
    static ToolTipShowMode toolTipShowMode(const QWidget *widget);
    static QString wrapToolTipText(QString text, QTextOption option);
    static bool needUpdateToolTip(const QWidget *widget, bool showToolTip);
    static void setShowToolTip(QWidget *widget, bool showToolTip);

    explicit DToolTip(const QString &text, bool completionClose = true);

    QSize sizeHint() const override;
    void show(const QPoint &pos, int duration);
};

DWIDGET_END_NAMESPACE

Q_DECLARE_METATYPE(Dtk::Widget::DToolTip::ToolTipShowMode)
#endif // DTOOLTIP_H

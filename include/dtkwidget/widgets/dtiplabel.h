// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTIPLABEL_H
#define DTIPLABEL_H

#include <DLabel>

DWIDGET_BEGIN_NAMESPACE

class DTipLabelPrivate;
class LIBDTKWIDGETSHARED_EXPORT DTipLabel : public DLabel
{
    Q_OBJECT
    Q_DISABLE_COPY(DTipLabel)
    D_DECLARE_PRIVATE(DTipLabel)
public:
    DTipLabel(const QString &text = QString(), QWidget *parent = nullptr);
    ~DTipLabel();

    using QLabel::show;
    void show(const QPoint &pos);
    void setForegroundRole(DPalette::ColorType color);

protected:
    void initPainter(QPainter *painter) const override;
    void paintEvent(QPaintEvent *event) override;
};
DWIDGET_END_NAMESPACE

#endif // DTIPLABEL_H

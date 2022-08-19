// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLABEL_H
#define DLABEL_H

#include <dobject.h>
#include <dstyle.h>
#include <dtkwidget_global.h>

#include <QLabel>

DWIDGET_BEGIN_NAMESPACE

class DLabelPrivate;
class LIBDTKWIDGETSHARED_EXPORT DLabel : public QLabel, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DLabel)
    D_DECLARE_PRIVATE(DLabel)
public:
    explicit DLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    DLabel(const QString &text, QWidget *parent = nullptr);
    ~DLabel();

    void setForegroundRole(QPalette::ColorRole role);
    void setForegroundRole(DPalette::ColorType color);
    void setElideMode(Qt::TextElideMode elideMode);
    Qt::TextElideMode elideMode() const;

protected:
    DLabel(DLabelPrivate &dd, QWidget *parent = nullptr);

    void initPainter(QPainter *painter) const override;
    void paintEvent(QPaintEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DLABEL_H

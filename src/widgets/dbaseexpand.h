/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DBASEEXPAND_H
#define DBASEEXPAND_H

#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>

#include <DHorizontalLine>

#include "dtkwidget_global.h"
#include "dconstants.h"

DWIDGET_BEGIN_NAMESPACE

class ContentLoader : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int height READ height WRITE setFixedHeight)
public:
    explicit ContentLoader(QWidget *parent = nullptr) : QFrame(parent){}
};

class DBoxWidget;

class DBaseExpandPrivate;
class LIBDTKWIDGETSHARED_EXPORT DBaseExpand : public QWidget
{
    Q_OBJECT
public:
    explicit DBaseExpand(QWidget *parent = nullptr);
    ~DBaseExpand() override;

    void setHeader(QWidget *header);
    void setContent(QWidget *content, Qt::Alignment alignment = Qt::AlignHCenter);
    QWidget *getContent() const;
    void setHeaderHeight(int height);
    virtual void setExpand(bool value);
    bool expand() const;
    void setAnimationDuration(int duration);
    void setAnimationEasingCurve(QEasingCurve curve);
    void setSeparatorVisible(bool arg);
    void setExpandedSeparatorVisible(bool arg);

Q_SIGNALS:
    void expandChange(bool e);
    void sizeChanged(QSize s);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private:
    QScopedPointer<DBaseExpandPrivate> d_private;

    Q_DECLARE_PRIVATE_D(d_private, DBaseExpand)
};

DWIDGET_END_NAMESPACE

#endif // DBASEEXPAND_H

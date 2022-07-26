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
#ifndef DDRAWER_H
#define DDRAWER_H

#include <DFrame>

DWIDGET_BEGIN_NAMESPACE
class DDrawerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DDrawer : public DFrame
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DDrawer)

public:
    explicit DDrawer(QWidget *parent = nullptr);
    ~DDrawer() override;

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
    DDrawer(DDrawerPrivate &dd, QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
};

DWIDGET_END_NAMESPACE

#endif // DDRAWER_H

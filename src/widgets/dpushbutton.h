/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DICONBUTTON_H
#define DICONBUTTON_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>
#include <QPushButton>

DWIDGET_BEGIN_NAMESPACE

class DPushButtonPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPushButton : public QPushButton, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(QString normalIcon READ normalIcon WRITE setNormalIcon DESIGNABLE true)
    Q_PROPERTY(QString hoverIcon READ hoverIcon WRITE setHoverIcon DESIGNABLE true)
    Q_PROPERTY(QString pressIcon READ pressIcon WRITE setPressIcon DESIGNABLE true)

public:
    explicit DPushButton(QWidget *parent = 0);

    DPushButton(const QString &normalIcon, const QString &hoverIcon,
                const QString &pressIcon, QWidget *parent = 0);

    const QString normalIcon() const;
    const QString hoverIcon() const;
    const QString pressIcon() const;

public Q_SLOTS:
    void setNormalIcon(const QString &normalIcon);
    void setHoverIcon(const QString &hoverIcon);
    void setPressIcon(const QString &pressIcon);
    void setNormalIcon(const QIcon &normalIcon);
    void setHoverIcon(const QIcon &hoverIcon);
    void setPressIcon(const QIcon &pressIcon);

protected:
    void enterEvent(QEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DPushButton)
};

DWIDGET_END_NAMESPACE

#endif // DICONBUTTON_H

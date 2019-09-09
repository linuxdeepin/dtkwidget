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

#ifndef DLINEEDIT_H
#define DLINEEDIT_H

#include <QLineEdit>

#include "dtkwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DLineEditPrivate;
class DStyleOptionLineEdit;
class LIBDTKWIDGETSHARED_EXPORT DLineEdit : public QLineEdit, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DLineEdit)
    D_DECLARE_PRIVATE(DLineEdit)
    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)

public:
    DLineEdit(QWidget *parent = 0);

    void setAlert(bool isAlert);
    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void hideAlertMessage();

    void setLeftWidgets(const QList<QWidget *> &list);
    void setRightWidgets(const QList<QWidget *> &list);

    void setLeftWidgetsVisible(bool visible);
    void setRightWidgetsVisible(bool visible);

Q_SIGNALS:
    void alertChanged(bool alert) const;
    void focusChanged(bool onFocus) const;
    void sizeChanged(const QSize &size) const;

protected:
    DLineEdit(DLineEditPrivate &q, QWidget *parent);

    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

    friend class DStyleOptionLineEdit;
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_H

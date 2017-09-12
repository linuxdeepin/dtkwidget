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

#ifndef DOPTION_H
#define DOPTION_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QFrame>
#include <QHBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DOptionPrivate;
class LIBDTKWIDGETSHARED_EXPORT DOption : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DOption)
    D_DECLARE_PRIVATE(DOption)
    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(QString checkedIcon READ checkedIcon WRITE setCheckedIcon NOTIFY checkedIconChanged DESIGNABLE true)

public:
    DOption(QWidget *parent = nullptr);
    void setName(const QString &name);
    void setValue(const QString &value);
    void setIcon(const QString &icon);
    void setIcon(const QPixmap &icon);

    const QString value() const;
    const QString name() const;

    bool checked() const;
    void setChecked(bool checked);
    const QString checkedIcon() const;
    void setCheckedIcon(const QString &icon);

    QHBoxLayout *layout();

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void sizeChanged(const QSize &size) const;
    void checkedChanged(bool checked) const;
    void checkedIconChanged(const QString checkedIcon) const;
};

DWIDGET_END_NAMESPACE

#endif // DOPTION_H

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

#ifndef DOPTION_P_H
#define DOPTION_P_H

#include "doption.h"

#include <DObjectPrivate>

#include <QLabel>
#include <QHBoxLayout>
#include <QListWidgetItem>

DWIDGET_BEGIN_NAMESPACE

class DOptionPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DOption)

public:
    DOptionPrivate(DOption *q);

    void init();

    void setIcon(const QString& iconName);
    void setIcon(const QPixmap& icon);

    const QString name() const;
    const QString value() const;

    bool checked() const;
    void setChecked(bool checked);

    const QString checkedIcon() const;
    void setCheckedIcon(const QString &icon);

    void sizeChanged(QResizeEvent *e);

public Q_SLOTS:
    void _q_itemStateChanged(QListWidgetItem* item);

public:
    bool m_checked = false;
    QString m_optionValue;
    QString m_checkedIconName;

    QLabel *m_checkedIconLabel;
    QLabel *m_optionIcon;
    QLabel *m_optionName;

    QHBoxLayout *m_layout;
};

DWIDGET_END_NAMESPACE

#endif // DOPTION_P_H


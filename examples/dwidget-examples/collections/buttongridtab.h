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

#ifndef BUTTONGRIDTAB_H
#define BUTTONGRIDTAB_H

#include <QFrame>

class ButtonGridTab : public QFrame
{
    Q_OBJECT
public:
    explicit ButtonGridTab(QWidget *parent = 0);
    ~ButtonGridTab();
    void initData();
    void initUI();
    void initConnect();
Q_SIGNALS:

public Q_SLOTS:
    void handleClicked(QString label);
    void handleEnterEvent(QString label);
    void handleLeaveEvent(QString label);

    void handleEnterIndex(int index);
    void handleLeaveIndex(int index);
private:
    QStringList m_buttons;
};

#endif // BUTTONGRIDTAB_H

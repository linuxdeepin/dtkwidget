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

#ifndef DPASSWORDEDIT_P_H
#define DPASSWORDEDIT_P_H

#include "dlineedit_p.h"
#include "dpasswordedit.h"

DWIDGET_BEGIN_NAMESPACE

class DSuggestButton;
class DPasswordEditPrivate : DLineEditPrivate
{
public:
    DPasswordEditPrivate(DPasswordEdit *q);

    void init();

    void _q_toggleEchoMode();

protected:
    Q_DECLARE_PUBLIC(DPasswordEdit)

private:
    DSuggestButton * togglePasswordVisibleButton;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWORDEDIT_P_H


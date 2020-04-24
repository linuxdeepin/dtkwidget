/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "buttongroup.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

class ButtonGroupPrivate
{
public:
    ButtonGroupPrivate(ButtonGroup *parent) : q_ptr(parent){}

    QHBoxLayout *layout = nullptr;
    QButtonGroup *group  = nullptr;
    ButtonGroup *q_ptr;
    Q_DECLARE_PUBLIC(ButtonGroup)
};

ButtonGroup::ButtonGroup(QWidget *parent) :
    QFrame(parent), d_ptr(new ButtonGroupPrivate(this))
{
    Q_D(ButtonGroup);
    setObjectName("ButtonGroup");
    d->group = new QButtonGroup;
    d->layout = new QHBoxLayout(this);
    d->layout->setSpacing(0);
    d->layout->setMargin(0);

    connect(d->group,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased),
            this, [=](int){
        Q_EMIT buttonChecked(d->group->checkedId());
    });
}

ButtonGroup::~ButtonGroup()
{

}

void ButtonGroup::setCheckedButton(int id)
{
    Q_D(ButtonGroup);
    if (d->group->button(id)) {
        d->group->button(id)->setChecked(true);
    }
}

void ButtonGroup::setButtons(const QStringList &texts)
{
    Q_D(ButtonGroup);
    int i = 0;
    for (auto text: texts) {
        auto bt = new QPushButton(text);
        bt->setCheckable(true);

        if (texts.length() <= 1)
            bt->setObjectName("ButtonGroupSingle");
        else {
            if (i == 0)
                bt->setObjectName("ButtonGroupBegin");
            else if (i == texts.length() -1) {
                    bt->setObjectName("ButtonGroupEnd");
            } else {

                    bt->setObjectName("ButtonGroupMiddle");
            }
        }
        d->group->addButton(bt, i);
        d->layout->addWidget(bt);
        i++;
    }
    d->layout->addStretch();
}

// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
    d->layout->setContentsMargins(0, 0, 0, 0);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(d->group, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonReleased),
            this, [=](QAbstractButton *) {
              Q_EMIT buttonChecked(d->group->checkedId());
            });
#else
    connect(d->group,&QButtonGroup::buttonReleased,this, [=](QAbstractButton *){
        Q_EMIT buttonChecked(d->group->checkedId());
    });
#endif
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

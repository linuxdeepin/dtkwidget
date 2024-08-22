// SPDX-FileCopyrightText: 2019 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBUTTONBOX_P_H
#define DBUTTONBOX_P_H

#include <DButtonBox>
#include <DStyleOption>

#include <DObjectPrivate>

QT_BEGIN_NAMESPACE
class QBoxLayout;
class QVariantAnimation;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DButtonBoxPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DButtonBoxPrivate(DButtonBox *qq);
    void init();
    DStyleOptionButtonBoxButton::ButtonPosition getButtonPosition(const DButtonBoxButton *button) const;

    QButtonGroup *group;
    QBoxLayout *layout;

    int m_hoverId;
    int m_checkedId;
    int m_pressId;

    QVariantAnimation *m_hoverAnimation;
    QVariantAnimation *m_checkMoveAnimation;

    D_DECLARE_PUBLIC(DButtonBox)
};

DWIDGET_END_NAMESPACE

#endif // DBUTTONBOX_P_H

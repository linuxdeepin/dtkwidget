// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIMAGEBUTTON_P_H
#define DIMAGEBUTTON_P_H

#include <DImageButton>

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DImageButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DImageButtonPrivate(DImageButton *qq);
    ~DImageButtonPrivate();

    void updateIcon();
    void setState(DImageButton::State state);
    QPixmap loadPixmap(const QString &path);

    DImageButton::State m_state = DImageButton::Normal;

    bool m_isChecked = false;
    bool m_isCheckable = false;
    QString m_normalPic;
    QString m_hoverPic;
    QString m_pressPic;
    QString m_checkedPic;
    QString m_disabledPic;

    D_DECLARE_PUBLIC(DImageButton)
};

DWIDGET_END_NAMESPACE

#endif // DIMAGEBUTTON_P_H

#endif

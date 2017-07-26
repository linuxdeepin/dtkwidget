#ifndef DPUSHBUTTON_P_H
#define DPUSHBUTTON_P_H

#include "dpushbutton.h"
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DPushButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DPushButtonPrivate(DPushButton *qq);

    QIcon normalIcon;
    QIcon hoverIcon;
    QIcon pressIcon;

    QString normalIconPath;
    QString hoverIconPath;
    QString pressIconPath;

    D_DECLARE_PUBLIC(DPushButton)
};

DWIDGET_END_NAMESPACE

#endif // DPUSHBUTTON_P_H

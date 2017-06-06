#ifndef DPUSHBUTTON_P_H
#define DPUSHBUTTON_P_H

#include "dpushbutton.h"
#include <private/dobject_p.h>

DWIDGET_BEGIN_NAMESPACE

class DPushButtonPrivate : public DObjectPrivate
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

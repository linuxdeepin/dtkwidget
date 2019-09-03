#ifndef DFLOATINGWIDGET_P_H
#define DFLOATINGWIDGET_P_H

#include "dfloatingwidget.h"

#include <DObjectPrivate>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DFloatingWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DFloatingWidgetPrivate(DFloatingWidget *qq);
    ~DFloatingWidgetPrivate();

    void init();

    QHBoxLayout *layout = nullptr;

    D_DECLARE_PUBLIC(DFloatingWidget)
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGWIDGET_P_H

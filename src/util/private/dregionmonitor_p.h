#ifndef DREGIONMONITOR_P_H_DWIDGET
#define DREGIONMONITOR_P_H_DWIDGET

#include "dtkwidget_global.h"
#include "dobject_p.h"
#include <QRegion>
#include <DRegionMonitor>

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

typedef DGUI_NAMESPACE::DRegionMonitor GuiDRegionMonitor;

class DRegionMonitorPrivate : public DObjectPrivate
{
public:
    DRegionMonitorPrivate(DObject *qq, QObject *parent = nullptr);
    virtual ~DRegionMonitorPrivate();

    GuiDRegionMonitor *guiRegionMonitor;
};

DWIDGET_END_NAMESPACE

#endif // DREGIONMONITOR_P_H_DWIDGET

#ifndef DREGIONMONITOR_P_H
#define DREGIONMONITOR_P_H

#include "dtkwidget_global.h"
#include "dobject_p.h"
#include "../dregionmonitor.h"

#include <QRegion>

#include <com_deepin_api_xmousearea.h>

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

using XMousAreaInter = com::deepin::api::XMouseArea;

class DRegionMonitorPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DRegionMonitor)

public:
    DRegionMonitorPrivate(DRegionMonitor *q);
    ~DRegionMonitorPrivate();

    bool registered() const { return !registerKey.isEmpty(); }

    void init();
    void registerMonitorRegion();
    void unregisterMonitorRegion();

    void _q_ButtonPress(const int flag, const int x, const int y, const QString &key);
    void _q_ButtonRelease(const int flag, const int x, const int y, const QString &key);

    XMousAreaInter *mouseAreaInter;
    QRegion watchedRegion;
    QString registerKey;
};

DWIDGET_END_NAMESPACE

#endif // DREGIONMONITOR_P_H

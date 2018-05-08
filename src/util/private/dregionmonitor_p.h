#ifndef DREGIONMONITOR_P_H
#define DREGIONMONITOR_P_H

#include "dtkwidget_global.h"
#include "dobject_p.h"
#include "../dregionmonitor.h"

#include <QRegion>
#include <QScreen>

#include <com_deepin_api_xeventmonitor.h>

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

using XEventMonitor = ::com::deepin::api::XEventMonitor;

class DRegionMonitorPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DRegionMonitor)

public:
    DRegionMonitorPrivate(DRegionMonitor *q);
    ~DRegionMonitorPrivate();

    enum Flag{
        Motion = 1 << 0,
        Button = 1 << 1,
        Key    = 1 << 2
    };

    bool registered() const { return !registerKey.isEmpty(); }

    void init();
    void registerMonitorRegion();
    void unregisterMonitorRegion();

    void _q_ButtonPress(const int flag, const int x, const int y, const QString &key);
    void _q_ButtonRelease(const int flag, const int x, const int y, const QString &key);
    void _q_CursorMove(const int x, const int y, const QString &key);
    void _q_KeyPress(const QString &keyname, const int x, const int y, const QString &key);
    void _q_KeyRelease(const QString &keyname, const int x, const int y, const QString &key);

    const QPoint deviceScaledCoordinate(const QPoint &p, const double ratio) const;

    XEventMonitor *eventInter;
    QRegion watchedRegion;
    QString registerKey;
};

DWIDGET_END_NAMESPACE

#endif // DREGIONMONITOR_P_H

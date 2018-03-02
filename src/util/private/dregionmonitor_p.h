#ifndef DREGIONMONITOR_P_H
#define DREGIONMONITOR_P_H

#include "dtkwidget_global.h"
#include "dobject_p.h"
#include "../dregionmonitor.h"

#include <QRegion>
#include <QScreen>

#ifdef DFRAMEWORKDBUS_API_XEVENTMONITOR
#include <com_deepin_api_xeventmonitor.h>
#else
#include <com_deepin_api_xmousearea.h>
#endif

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

#ifdef DFRAMEWORKDBUS_API_XEVENTMONITOR
using XEventMonitor = ::com::deepin::api::XEventMonitor;
#else
using XMousAreaInter = ::com::deepin::api::XMouseArea;
#endif

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

#ifdef DFRAMEWORKDBUS_API_XEVENTMONITOR
    XEventMonitor *eventInter;
#else
    XMousAreaInter *eventInter;
#endif
    QRegion watchedRegion;
    QString registerKey;
};

DWIDGET_END_NAMESPACE

#endif // DREGIONMONITOR_P_H

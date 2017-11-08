#include "dregionmonitor.h"
#include "private/dregionmonitor_p.h"

#include <QObject>
#include <QDebug>
#include <QApplication>

DWIDGET_BEGIN_NAMESPACE

DRegionMonitor::DRegionMonitor(QObject *parent)
    : QObject(parent),
      DObject(*new DRegionMonitorPrivate(this))
{
    D_D(DRegionMonitor);

    d->init();
}

bool DRegionMonitor::registered() const
{
    D_DC(DRegionMonitor);

    return !d->registerKey.isEmpty();
}

QRegion DRegionMonitor::watchedRegion() const
{
    D_DC(DRegionMonitor);

    return d->watchedRegion;
}

void DRegionMonitor::registerRegion()
{
    if (registered())
    {
        qWarning() << "region already registered!";
        return;
    }

    D_D(DRegionMonitor);

    d->registerMonitorRegion();
}

void DRegionMonitor::unregisterRegion()
{
    D_D(DRegionMonitor);

    d->unregisterMonitorRegion();
}

void DRegionMonitor::setWatchedRegion(const QRegion &region)
{
    D_D(DRegionMonitor);

    d->watchedRegion = region;
    if (registered())
        d->registerMonitorRegion();
}

DRegionMonitorPrivate::DRegionMonitorPrivate(DRegionMonitor *q)
    : DObjectPrivate(q),

      mouseAreaInter(new XMousAreaInter("com.deepin.api.XMouseArea", "/com/deepin/api/XMouseArea", QDBusConnection::sessionBus()))
{
}

DRegionMonitorPrivate::~DRegionMonitorPrivate()
{
    if (registered())
        unregisterMonitorRegion();

    mouseAreaInter->deleteLater();
}

void DRegionMonitorPrivate::init()
{
    D_Q(DRegionMonitor);

    QObject::connect(mouseAreaInter, SIGNAL(ButtonPress(int,int,int,QString)), q, SLOT(_q_ButtonPress(const int, const int, const int, const QString&)));
    QObject::connect(mouseAreaInter, SIGNAL(ButtonRelease(int,int,int,QString)), q, SLOT(_q_ButtonRelease(const int, const int, const int, const QString&)));
}

void DRegionMonitorPrivate::registerMonitorRegion()
{
    if (registered())
        unregisterMonitorRegion();

    if (watchedRegion.isEmpty())
    {
        registerKey = mouseAreaInter->RegisterFullScreen();
    } else {
        const QRect r = watchedRegion.boundingRect();
        const int x1 = r.x();
        const int y1 = r.y();
        const int x2 = x1 + r.width();
        const int y2 = y1 + r.height();

        // TODO:
        const int flags = 2;

        registerKey = mouseAreaInter->RegisterArea(x1, y1, x2, y2, flags);
    }
}

void DRegionMonitorPrivate::unregisterMonitorRegion()
{
    if (registerKey.isEmpty())
        return;

    mouseAreaInter->UnregisterArea(registerKey);
    registerKey.clear();
}

void DRegionMonitorPrivate::_q_ButtonPress(const int flag, const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    D_Q(DRegionMonitor);

    emit q->buttonPress(QPoint(x, y) / qApp->devicePixelRatio(), flag);
}

void DRegionMonitorPrivate::_q_ButtonRelease(const int flag, const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    D_Q(DRegionMonitor);

    emit q->buttonRelease(QPoint(x, y) / qApp->devicePixelRatio(), flag);
}

#include "moc_dregionmonitor.cpp"

DWIDGET_END_NAMESPACE

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
    : DObjectPrivate(q)
#ifdef DFRAMEWORKDBUS_API_XEVENTMONITOR
    , eventInter(new XEventMonitor("com.deepin.api.XEventMonitor", "/com/deepin/api/XEventMonitor", QDBusConnection::sessionBus()))
#else
    , eventInter(new XMousAreaInter("com.deepin.api.XMouseArea", "/com/deepin/api/XMouseArea", QDBusConnection::sessionBus()))
#endif
{
}

DRegionMonitorPrivate::~DRegionMonitorPrivate()
{
    if (registered())
        unregisterMonitorRegion();

    eventInter->deleteLater();
}

void DRegionMonitorPrivate::init()
{
    D_Q(DRegionMonitor);

    QObject::connect(eventInter, SIGNAL(ButtonPress(int,int,int,QString)), q, SLOT(_q_ButtonPress(const int, const int, const int, const QString&)));
    QObject::connect(eventInter, SIGNAL(ButtonRelease(int,int,int,QString)), q, SLOT(_q_ButtonRelease(const int, const int, const int, const QString&)));
    QObject::connect(eventInter, SIGNAL(CursorMove(int,int,QString)), q, SLOT(_q_CursorMove(const int, const int, const QString&)));
    QObject::connect(eventInter, SIGNAL(KeyPress(QString,int,int,QString)), q, SLOT(_q_KeyPress(const QString&, const int, const int, const QString&)));
    QObject::connect(eventInter, SIGNAL(KeyRelease(QString,int,int,QString)), q, SLOT(_q_KeyRelease(const QString&, const int, const int, const QString&)));
}

void DRegionMonitorPrivate::registerMonitorRegion()
{
    if (registered())
        unregisterMonitorRegion();

    if (watchedRegion.isEmpty())
    {
        registerKey = eventInter->RegisterFullScreen();
    } else {
        const QRect r = watchedRegion.boundingRect();
        const int x1 = r.x();
        const int y1 = r.y();
        const int x2 = x1 + r.width();
        const int y2 = y1 + r.height();

        // TODO:
        const int flags = Motion | Button | Key;

        registerKey = eventInter->RegisterArea(x1, y1, x2, y2, flags);
    }
}

void DRegionMonitorPrivate::unregisterMonitorRegion()
{
    if (registerKey.isEmpty())
        return;

    eventInter->UnregisterArea(registerKey);
    registerKey.clear();
}

void DRegionMonitorPrivate::_q_ButtonPress(const int flag, const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    D_Q(DRegionMonitor);

    Q_EMIT q->buttonPress(deviceScaledCoordinate(QPoint(x, y), qApp->devicePixelRatio()), flag);
}

void DRegionMonitorPrivate::_q_ButtonRelease(const int flag, const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    D_Q(DRegionMonitor);

    Q_EMIT q->buttonRelease(deviceScaledCoordinate(QPoint(x, y), qApp->devicePixelRatio()), flag);
}

void DRegionMonitorPrivate::_q_CursorMove(const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    D_Q(DRegionMonitor);

    Q_EMIT q->cursorMove(deviceScaledCoordinate(QPoint(x, y), qApp->devicePixelRatio()));
}

void DRegionMonitorPrivate::_q_KeyPress(const QString &keyname, const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    Q_UNUSED(x);
    Q_UNUSED(y);

    D_Q(DRegionMonitor);

    Q_EMIT q->keyPress(keyname);
}

void DRegionMonitorPrivate::_q_KeyRelease(const QString &keyname, const int x, const int y, const QString &key)
{
    if (registerKey != key)
        return;

    Q_UNUSED(x);
    Q_UNUSED(y);

    D_Q(DRegionMonitor);

    Q_EMIT q->keyRelease(keyname);
}

const QPoint DRegionMonitorPrivate::deviceScaledCoordinate(const QPoint &p, const double ratio) const
{
    for (const auto *s : qApp->screens())
    {
        const QRect &g(s->geometry());
        const QRect realRect(g.topLeft(), g.size() * ratio);

        if (realRect.contains(p))
            return QPoint(realRect.topLeft() + (p - realRect.topLeft()) / ratio);
    }

    return p / ratio;
}

#include "moc_dregionmonitor.cpp"

DWIDGET_END_NAMESPACE

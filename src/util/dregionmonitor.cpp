#include "dregionmonitor.h"
#include "private/dregionmonitor_p.h"

DWIDGET_BEGIN_NAMESPACE
/*!
 * \~chinese \brief DRegionMonitor::DRegionMonitor 这个类已经废弃不用,请使用libdtkgui
 * \~chinese \param parent
 */
DRegionMonitor::DRegionMonitor(QObject *parent)
    : QObject(parent)
    , DObject(*new DRegionMonitorPrivate(this, this))
{
    D_D(DRegionMonitor);

    connect(d->guiRegionMonitor, &GuiDRegionMonitor::buttonPress, this, &DRegionMonitor::buttonPress);
    connect(d->guiRegionMonitor, &GuiDRegionMonitor::buttonRelease, this, &DRegionMonitor::buttonRelease);
    connect(d->guiRegionMonitor, &GuiDRegionMonitor::cursorMove, this, &DRegionMonitor::cursorMove);
    connect(d->guiRegionMonitor, &GuiDRegionMonitor::keyPress, this, &DRegionMonitor::keyPress);
    connect(d->guiRegionMonitor, &GuiDRegionMonitor::keyRelease, this, &DRegionMonitor::keyRelease);

    connect(d->guiRegionMonitor, &GuiDRegionMonitor::coordinateTypeChanged, this, [this](GuiDRegionMonitor::CoordinateType type) {
        this->coordinateTypeChanged(static_cast<CoordinateType>(type));
    });
}

bool DRegionMonitor::registered() const
{
    D_DC(DRegionMonitor);
    return d->guiRegionMonitor->registered();
}

QRegion DRegionMonitor::watchedRegion() const
{
    D_DC(DRegionMonitor);
    return d->guiRegionMonitor->watchedRegion();
}

DRegionMonitor::CoordinateType DRegionMonitor::coordinateType() const
{
    D_DC(DRegionMonitor);
    return static_cast<DRegionMonitor::CoordinateType>(d->guiRegionMonitor->coordinateType());
}

void DRegionMonitor::registerRegion()
{
    D_D(DRegionMonitor);
    d->guiRegionMonitor->registerRegion();
}

void DRegionMonitor::unregisterRegion()
{
    D_D(DRegionMonitor);
    d->guiRegionMonitor->unregisterRegion();
}

void DRegionMonitor::setWatchedRegion(const QRegion &region)
{
    D_D(DRegionMonitor);
    d->guiRegionMonitor->setWatchedRegion(region);
}

void DRegionMonitor::setCoordinateType(DRegionMonitor::CoordinateType type)
{
    D_D(DRegionMonitor);
    auto guiType = static_cast<GuiDRegionMonitor::CoordinateType>(type);
    d->guiRegionMonitor->setCoordinateType(guiType);
}

DRegionMonitorPrivate::DRegionMonitorPrivate(DObject *qq, QObject *parent /* = nullptr*/)
    : DObjectPrivate(qq)
    , guiRegionMonitor(new GuiDRegionMonitor(parent))
{
}

DRegionMonitorPrivate::~DRegionMonitorPrivate()
{
}

DWIDGET_END_NAMESPACE

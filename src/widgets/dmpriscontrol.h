#ifndef DMPRISCONTROL_H
#define DMPRISCONTROL_H

#include "dobject.h"
#include "dtkwidget_global.h"

#include <QFrame>

DWIDGET_BEGIN_NAMESPACE

class DMPRISControlPrivate;
class LIBDTKWIDGETSHARED_EXPORT DMPRISControl : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DMPRISControl)
    D_DECLARE_PRIVATE(DMPRISControl)

public:
    explicit DMPRISControl(QWidget *parent = 0);

    bool isWorking() const;

Q_SIGNALS:
    void mprisAcquired() const;
    void mprisChanged() const;
    void mprisLosted() const;

protected:
    D_PRIVATE_SLOT(void _q_onMetaDataChanged())
    D_PRIVATE_SLOT(void _q_onPlaybackStatusChanged())
    D_PRIVATE_SLOT(void _q_onPrevClicked())
    D_PRIVATE_SLOT(void _q_onPlayClicked())
    D_PRIVATE_SLOT(void _q_onPauseClicked())
    D_PRIVATE_SLOT(void _q_onNextClicked())
    D_PRIVATE_SLOT(void _q_loadMPRISPath(const QString &))
    D_PRIVATE_SLOT(void _q_removeMPRISPath(const QString &))
};

DWIDGET_END_NAMESPACE

#endif // DMPRISCONTROL_H

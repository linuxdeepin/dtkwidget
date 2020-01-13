#ifndef DFLOATINGMESSAGE_P_H
#define DFLOATINGMESSAGE_P_H

#include "dfloatingmessage.h"
#include "dfloatingwidget_p.h"

QT_BEGIN_NAMESPACE
class QTimer;
class QWidget;
class QHBoxLayout;
class DIconButton;
class QLabel;
class MessageLabel;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DFloatingMessagePrivate : public DFloatingWidgetPrivate
{
public:
    explicit DFloatingMessagePrivate(DFloatingMessage *qq);
    ~DFloatingMessagePrivate();

    void init();

    QTimer *timer = nullptr;
    QWidget *widget = nullptr;
    QHBoxLayout *hBoxLayout = nullptr;
    DIconButton *iconButton = nullptr;
    MessageLabel *labMessage = nullptr;
    QWidget *content = nullptr;
    DIconButton *closeButton = nullptr;
    DFloatingMessage::MessageType notifyType;

    D_DECLARE_PUBLIC(DFloatingMessage)
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGMESSAGE_P_H

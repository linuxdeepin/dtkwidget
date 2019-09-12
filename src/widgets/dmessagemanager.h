#ifndef DMESSAGEMANAGER_H
#define DMESSAGEMANAGER_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>

#include "dfloatingmessage.h"

DWIDGET_USE_NAMESPACE

class DMessageManager: public QObject
{
    Q_OBJECT

private:
    DMessageManager();                    //构造函数是私有的

public:
    static DMessageManager *instance();

    void sendMessage(QWidget *par, DFloatingMessage *floMsg);
    void sendMessage(QWidget *par, const QIcon &icon, const QString &message);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // DMESSAGEMANAGER_H

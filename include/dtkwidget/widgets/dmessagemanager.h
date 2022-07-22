#ifndef DMESSAGEMANAGER_H
#define DMESSAGEMANAGER_H

#include <QObject>
#include <QWidget>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DFloatingMessage;
class DMessageManager: public QObject
{
    Q_OBJECT

private:
    DMessageManager();                    //构造函数是私有的

public:
    static DMessageManager *instance();

    void sendMessage(QWidget *par, DFloatingMessage *floMsg);
    void sendMessage(QWidget *par, const QIcon &icon, const QString &message);
    bool setContentMargens(QWidget *par, const QMargins &margins);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DMESSAGEMANAGER_H

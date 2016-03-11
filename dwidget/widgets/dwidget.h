#ifndef DWIDGET_H
#define DWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <libdui_global.h>

DWIDGET_BEGIN_NAMESPACE

class DWidget : public QWidget, public DObject
{
    Q_OBJECT
public:
    explicit DWidget(QWidget *parent = 0);

    virtual void mouseMoveEvent(QMouseEvent *);

    virtual void removeLayout();
protected:
    explicit DWidget(DObjectPrivate &dd, QWidget *parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DWIDGET_H

#ifndef DBACKGROUNDGROUP_H
#define DBACKGROUNDGROUP_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DBackgroundGroup : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    using QWidget::QWidget;

protected:
    void paintEvent(QPaintEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DBACKGROUNDGROUP_H

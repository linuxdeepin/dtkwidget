#ifndef DWINDOW_H
#define DWINDOW_H

#include <QWidget>
#include <dobject.h>
#include <libdui_global.h>

#include "dwidget.h"

DWIDGET_BEGIN_NAMESPACE

class DWindowPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWindow : public DWidget
{
    Q_OBJECT
public:
    explicit DWindow(DWidget *parent = 0);

    void setContentLayout(QLayout *l);
    void setContentWidget(QWidget *w);

private:
    Q_DISABLE_COPY(DWindow)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOW_H

#ifndef DWIDGET_H
#define DWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <libdui_global.h>


#include <dx11widget.h>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DWidget : public DX11Widget
{
    Q_OBJECT
public:
    explicit DWidget(DWidget *parent = 0);

private:
    Q_DISABLE_COPY(DWidget)
};

DWIDGET_END_NAMESPACE


#endif // DWIDGET_H

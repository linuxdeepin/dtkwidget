#ifndef DBACKGROUNDGROUP_H
#define DBACKGROUNDGROUP_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DBackgroundGroupPrivate;
class LIBDTKWIDGETSHARED_EXPORT DBackgroundGroup : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DBackgroundGroup)

    Q_PROPERTY(QMargins itemMargins READ itemMargins WRITE setItemMargins)
public:
    explicit DBackgroundGroup(QLayout *layout, QWidget *parent = nullptr);

    QMargins itemMargins() const;

public Q_SLOTS:
    void setItemMargins(QMargins itemMargins);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    using QWidget::setLayout;
    using QWidget::setAutoFillBackground;
};

DWIDGET_END_NAMESPACE

#endif // DBACKGROUNDGROUP_H

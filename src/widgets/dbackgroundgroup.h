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
    Q_PROPERTY(bool useWidgetBackground READ useWidgetBackground WRITE setUseWidgetBackground NOTIFY useWidgetBackgroundChanged)

public:
    explicit DBackgroundGroup(QLayout *layout, QWidget *parent = nullptr);

    QMargins itemMargins() const;
    bool useWidgetBackground() const;

public Q_SLOTS:
    void setItemMargins(QMargins itemMargins);
    void setItemSpacing(int spacing);
    void setUseWidgetBackground(bool useWidgetBackground);

Q_SIGNALS:
    void useWidgetBackgroundChanged(bool useWidgetBackground);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    using QWidget::setLayout;
    using QWidget::setAutoFillBackground;
};

DWIDGET_END_NAMESPACE

#endif // DBACKGROUNDGROUP_H

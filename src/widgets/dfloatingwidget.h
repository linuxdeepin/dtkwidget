#ifndef DFLOATINGWIDGET_H
#define DFLOATINGWIDGET_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DStyleOptionFloatingWidget;
class DFloatingWidgetPrivate;
class DFloatingWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFloatingWidget)

public:
    explicit DFloatingWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setWidget(QWidget *widget);

protected:
    void paintEvent(QPaintEvent* e) override;

    using QWidget::setContentsMargins;
    using QWidget::setAutoFillBackground;

public:
    virtual void initStyleOption(DStyleOptionFloatingWidget *option) const;
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGWIDGET_H

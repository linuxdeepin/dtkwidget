#ifndef DTICKWIDGETPRIVATE_H
#define DTICKWIDGETPRIVATE_H

#include "dtickwidget.h"
#include <private/dobject_p.h>
#include <QHBoxLayout>
#include <QVariantAnimation>

DWIDGET_BEGIN_NAMESPACE

class DTickWidgetPrivate : public DObjectPrivate
{
public:
    explicit DTickWidgetPrivate(DTickWidget *qq);
    ~DTickWidgetPrivate();

    void init();
    void initDirection();

public:
    DTickWidget::Direction direction;
    int duration;
    QVariantAnimation *runAnimation;
    QWidget *content;

    D_DECLARE_PUBLIC(DTickWidget)
};

DWIDGET_END_NAMESPACE

#endif // DTICKWIDGETPRIVATE_H

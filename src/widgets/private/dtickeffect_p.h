#ifndef DTICKEFFECTPRIVATE_H
#define DTICKEFFECTPRIVATE_H

#include "dtickeffect.h"
#include <DObjectPrivate>
#include <QHBoxLayout>
#include <QVariantAnimation>

DWIDGET_BEGIN_NAMESPACE

class DTickEffectPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DTickEffectPrivate(DTickEffect *qq);
    ~DTickEffectPrivate();

    void init();
    void initDirection();

public:
    DTickEffect::Direction direction;
    int duration;
    QVariantAnimation *runAnimation;
    QWidget *content;

    D_DECLARE_PUBLIC(DTickEffect)
};

DWIDGET_END_NAMESPACE

#endif // DTICKEFFECTPRIVATE_H

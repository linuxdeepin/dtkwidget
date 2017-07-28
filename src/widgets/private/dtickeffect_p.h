/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

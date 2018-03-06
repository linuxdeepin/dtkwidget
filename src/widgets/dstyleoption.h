/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DSTYLEOPTION_H
#define DSTYLEOPTION_H

#include <dtkwidget_global.h>

#include <QStyleOption>

DWIDGET_BEGIN_NAMESPACE

class DStyleOption
{
public:
    virtual void init(QWidget *widget);
};

class DStyleOptionSuggestButton : public QStyleOptionButton, public DStyleOption
{
public:
    enum ButtonFeature {
        SuggestButton = (CommandLinkButton << 1)
    };

    void init(QWidget *widget) override;
};

class DStyleOptionLineEdit : public DStyleOption
{
public:
    enum LineEditFeature {
        Alert = 0x1,
        IconButton = 0x2
    };
    Q_DECLARE_FLAGS(LineEditFeatures, LineEditFeature)

    void init(QWidget *widget) override;

    LineEditFeatures features = 0;
    QRect iconButtonRect;
};

DWIDGET_END_NAMESPACE

#endif // DSTYLEOPTION_H

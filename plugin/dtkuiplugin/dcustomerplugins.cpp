/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     alex <wangpenga@uniontech.com>
 *
 * Maintainer: alex <wangpenga@uniontech.com>
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
#include "dcustomerplugins.h"

DCustomWidgets::DCustomWidgets(QObject *parent)
        : QObject(parent)
{
    widgets.append(new DFramePlugin(this));
    widgets.append(new DArrowLineDrawerPlugin(this));
    widgets.append(new DButtonBoxPlugin(this));
    widgets.append(new DColoredProgressBarPlugin(this));
    widgets.append(new DCrumbEditPlugin(this));
    widgets.append(new DDoubleSpinBoxPlugin(this));
    widgets.append(new DFileChooserEditPlugin(this));
    widgets.append(new DFloatingButtonPlugin(this));
    widgets.append(new DIpv4LineEditPlugin(this));
    widgets.append(new DKeySequenceEditPlugin(this));
    widgets.append(new DLabelPlugin(this));
    widgets.append(new DLineEditPlugin(this));
    widgets.append(new DPageIndicatorPlugin(this));

    widgets.append(new DPasswordEditPlugin(this));
    widgets.append(new DTabBarPlugin(this));
    widgets.append(new DWaterProgressPlugin(this));
    widgets.append(new DTextEditPlugin(this));
    widgets.append(new DSearchComboBoxPlugin(this));
    widgets.append(new DSpinBoxPlugin(this));
    widgets.append(new DSpinnerPlugin(this));
    widgets.append(new DToolButtonPlugin(this));
    widgets.append(new DSuggestButtonPlugin(this));
    widgets.append(new DSwitchButtonPlugin(this));
    widgets.append(new DWarningButtonPlugin(this));
    widgets.append(new DSearchEditPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> DCustomWidgets::customWidgets() const
{
    return widgets;
}

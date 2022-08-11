// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

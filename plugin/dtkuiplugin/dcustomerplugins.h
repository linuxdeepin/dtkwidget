// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCUSTOMERPLUGINS_H
#define DCUSTOMERPLUGINS_H

#include "dcustomermacrowidget.h"

class DCustomWidgets: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    DCustomWidgets(QObject *parent = nullptr);

    QList<QDesignerCustomWidgetInterface*> customWidgets() const Q_DECL_OVERRIDE;

private:
    QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // DCUSTOMERPLUGINS_H

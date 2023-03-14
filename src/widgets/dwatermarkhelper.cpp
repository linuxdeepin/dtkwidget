// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwatermarkhelper.h"

#include <QGuiApplication>
#include <QPaintEvent>
#include <QPainter>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

DTK_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#define WATER_TEXTSPACE 65
#define DEFAULT_FONTSIZE WATER_TEXTSPACE

class DWaterMaskHelper_ : public DWaterMarkHelper {};
Q_GLOBAL_STATIC(DWaterMaskHelper_, wmhGlobal)

class DWaterMarkHelperPrivate: public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DWaterMarkHelperPrivate(DWaterMarkHelper *parent)
        : DObjectPrivate(parent)
    {

    }

    void init();
    static void clean() {
        auto helper = DWaterMarkHelper::instance();
        helper->d_func()->widgetMap.clear();
        helper->deleteLater();
    }

    WaterMarkData data;
    static QMap<QWidget *, DWaterMarkWidget *> widgetMap;
};

void DWaterMarkHelperPrivate::init()
{    // 进程退出时销毁资源
    qAddPostRoutine(clean);
}

QMap<QWidget *, DWaterMarkWidget *>DWaterMarkHelperPrivate::widgetMap;

/*!
  \brief DWaterMarkHelper::instance
  The singleton object of DWaterMarkHelper is instantiated at the first call using the Q _ GLOBAL _ STATIC definition.
  \return
 */
DWaterMarkHelper *DWaterMarkHelper::instance()
{
    return wmhGlobal;
}

/*!
  \class Dtk::Widget::DWaterMaskHelper
  \inmodule dtkwidget
  \brief Direct instantiation of this object is not allowed
  \a parent
  \sa DWaterMaskHelper::instance
 */
DWaterMarkHelper::DWaterMarkHelper(QObject *parent)
    : QObject(parent)
    , DObject(*new DWaterMarkHelperPrivate(this))
{
    D_D(DWaterMarkHelper);
    d->init();
}

/*!
  \brief DWaterMarkHelper::registerWidget
  \a widget
 */
void DWaterMarkHelper::registerWidget(QWidget *widget)
{
    D_D(DWaterMarkHelper);

    if (widget && !d->widgetMap.contains(widget)) {
        DWaterMarkWidget *mark = new DWaterMarkWidget(widget);
        mark->setData(d->data);
        mark->setVisible(true);
        d->widgetMap.insert(widget, mark);

        connect(widget, &QObject::destroyed, this, [widget] {
            DWaterMarkHelper::instance()->d_func()->widgetMap.remove(widget);
        });
    }
}

/*!
  \brief get the current setting
 */
WaterMarkData DWaterMarkHelper::data() const
{
    D_DC(DWaterMarkHelper);
    return d->data;
}

/*!
  \brief set the current setting 
  \a data
 */
void DWaterMarkHelper::setData(const WaterMarkData &data)
{
    D_D(DWaterMarkHelper);
    d->data = data;

    auto helper = DWaterMarkHelper::instance();
    for (auto w : helper->d_func()->widgetMap.values()) {
        w->setData(data);
    }
}

DWIDGET_END_NAMESPACE

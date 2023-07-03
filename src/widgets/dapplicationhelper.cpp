// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dapplicationhelper.h"
#include "dpalettehelper.h"
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
#include <QApplication>

DWIDGET_BEGIN_NAMESPACE

class _DApplicationHelper {
public:
    static DGuiApplicationHelper *createHelper()
    {
        return new DApplicationHelper();
    }
};

__attribute__((constructor)) // This function is executed when the library is loaded
static void init_createHelper ()
{
    if (!QApplication::instance() || qobject_cast<QApplication *>(QApplication::instance())) {
        DApplicationHelper::registerInstanceCreator(_DApplicationHelper::createHelper);
    }
}

class DApplicationHelperPrivate
{
public:
    DPaletteHelper *paletteHelper = DPaletteHelper::instance();
};

static DApplicationHelperPrivate *d = nullptr;

/*!
  \class Dtk::Widget::DApplicationHelper
  \inmodule dtkwidget
  \brief `dApplicationHelper` provided a modified `DGuiApplicationHelper` 类.
 */

/*!
  \brief `DApplicationHelper::instance`return `DApplicationHelper` object
 */
DApplicationHelper *DApplicationHelper::instance()
{
    return qobject_cast<DApplicationHelper*>(DGuiApplicationHelper::instance());
}

/*!
  \brief `DApplicationHelper::palette` return a palette
  \a widget widget
  \a base Palette
  \return Palette
 */
DPalette DApplicationHelper::palette(const QWidget *widget, const QPalette &base) const
{
    return d->paletteHelper->palette(widget, base);
}

/*!
  \brief `DApplicationHelper::setPalette` set the palette to the control
  \a widget widget
  \a palette palette
 */
void DApplicationHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    d->paletteHelper->setPalette(widget, palette);
}

/*!
  \brief `DApplicationHelper::resetPalette` Reset the color panel attribute of the reset control
  \a widget widget
 */
void DApplicationHelper::resetPalette(QWidget *widget)
{
    d->paletteHelper->resetPalette(widget);
}

DApplicationHelper::DApplicationHelper()
{
    if (!d)
        d = new DApplicationHelperPrivate();
}

DApplicationHelper::~DApplicationHelper()
{
    if (d) {
        delete d;
        d = nullptr;
    }
}

bool DApplicationHelper::eventFilter(QObject *watched, QEvent *event)
{
    Q_ASSERT_X(false, Q_FUNC_INFO, "This function should not be called.");

    return DGuiApplicationHelper::eventFilter(watched, event);
}

bool DApplicationHelper::event(QEvent *event)
{
    return DGuiApplicationHelper::event(event);
}

DWIDGET_END_NAMESPACE
#endif

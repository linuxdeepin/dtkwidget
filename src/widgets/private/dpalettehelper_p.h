#ifndef DPALETTEHELPER_P_H
#define DPALETTEHELPER_P_H

#include "dpalettehelper.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DPaletteHelperPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DPaletteHelperPrivate(DPaletteHelper *qq);

    QHash<const QWidget *, DPalette> paletteCache;

    D_DECLARE_PUBLIC(DPaletteHelper)
};

DWIDGET_END_NAMESPACE

#endif // DPALETTEHELPER_P_H

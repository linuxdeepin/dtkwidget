#ifndef DHIDPIHELPER_H
#define DHIDPIHELPER_H

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DHiDPIHelper
{
public:
    static QPixmap loadNxPixmap(const QString &fileName);
};

DWIDGET_END_NAMESPACE

#endif // DHIDPIHELPER_H

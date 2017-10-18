#ifndef DHIDPIHELPER_H
#define DHIDPIHELPER_H

#include "dtkwidget_global.h"

DTK_BEGIN_NAMESPACE

class DHiDPIHelper
{
public:
    static QPixmap loadNxPixmap(const QString &fileName);
};

DTK_END_NAMESPACE

#endif // DHIDPIHELPER_H

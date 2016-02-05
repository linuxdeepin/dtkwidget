/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPASSWORDEDIT_P_H
#define DPASSWORDEDIT_P_H

#include "dlineedit_p.h"
#include "dpasswordedit.h"

DWIDGET_NAMESPACE_BEGIN

class DPasswordEditPrivate : DLineEditPrivate
{
public:
    DPasswordEditPrivate(DPasswordEdit *q);

    void init();

    void _q_toggleEchoMode();

protected:
    Q_DECLARE_PUBLIC(DPasswordEdit)
};

DWIDGET_NAMESPACE_END

#endif // DPASSWORDEDIT_P_H


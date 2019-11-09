#ifndef DKEYSEQUENCEEDIT_P_H
#define DKEYSEQUENCEEDIT_P_H

#include "dkeysequenceedit.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DKeyWidget;
class DKeySequenceEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DKeySequenceEdit)
public:
    DKeySequenceEditPrivate(DKeySequenceEdit *q);

    void init();
private:
    DKeyWidget *rightWidget;
};

DWIDGET_END_NAMESPACE

#endif // DKEYSEQUENCEEDIT_P_H

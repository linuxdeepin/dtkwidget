#ifndef DWARNINGBUTTON_H
#define DWARNINGBUTTON_H

#include <dtkwidget_global.h>
#include <DPushButton>

DWIDGET_BEGIN_NAMESPACE

class DWarningButton : public DPushButton
{
public:
    DWarningButton(QWidget *parent = nullptr);

protected:
    void initStyleOption(QStyleOptionButton *option) const;
    void paintEvent(QPaintEvent *e) override;
};

DWIDGET_END_NAMESPACE

#endif // DWARNINGBUTTON_H

#ifndef DCOMMANDLINKBUTTON_H
#define DCOMMANDLINKBUTTON_H

#include <dtkwidget_global.h>
#include <QAbstractButton>

DWIDGET_BEGIN_NAMESPACE
class DStyleOptionButton;

class DCommandLinkButton : public QAbstractButton
{
    Q_OBJECT

public:
    explicit DCommandLinkButton(const QString text, QWidget *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void initStyleOption(DStyleOptionButton *option) const;
    void paintEvent(QPaintEvent *e) override;
};

DWIDGET_END_NAMESPACE

#endif // DCOMMANDLINKBUTTON_H

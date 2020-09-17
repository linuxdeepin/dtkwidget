#ifndef MENUEXAMPLE_H
#define MENUEXAMPLE_H

#include <QWidget>

#include "dtkwidget_global.h"
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class QPropertyAnimation;
class QMenu;
class MenuExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit MenuExampleWindow(QWidget *parent = nullptr);
};

class DMenuExample : public ExampleWindowInterface
{
    Q_OBJECT
    Q_PROPERTY(QColor aColor READ getAColor WRITE setAColor)

    // ExampleWindowInterface interface
public:
    explicit DMenuExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;

    QColor getAColor();
    void setAColor(const QColor &color);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void restoreAnimation();

private:
    QPropertyAnimation *animation;
    QColor acolor;
    QPixmap pixmap;
    QMenu *leftMenu;
};

#endif // MENUEXAMPLE_H

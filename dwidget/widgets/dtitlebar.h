#ifndef DTITLEBAR_H
#define DTITLEBAR_H

#include <QWidget>
#include <dobject.h>
#include "libdui_global.h"

DWIDGET_BEGIN_NAMESPACE

class DTitlebarPrivate;
class DMenu;

///
/// \brief The DTitlebar class is an universal title bar on the top of windows.
///
class LIBDTKWIDGETSHARED_EXPORT DTitlebar : public QWidget , public DObject
{
    Q_OBJECT
public:
    explicit DTitlebar(QWidget *parent = 0);

    void setMenu(DMenu*);
    void setCustomWidget(QWidget *, bool fixCenterPos = false);
    void setWindowFlags(Qt::WindowFlags type);
    int buttonAreaWidth() const;
signals:
    void minimumClicked();
    void maximumClicked();
    void restoreClicked();
    void closeClicked();
    void optionClicked();

public slots:
    void setFixedHeight(int h);
    void setTitle(const QString& title);
    void setIcon(const QPixmap& icon);

private slots:
    void showMenu();

private:
    D_DECLARE_PRIVATE(DTitlebar)
};

DWIDGET_END_NAMESPACE

#endif // DTITLEBAR_H

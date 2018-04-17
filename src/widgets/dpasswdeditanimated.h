#ifndef DPASSWDEDITANIMATED_H
#define DPASSWDEDITANIMATED_H

#include "dimagebutton.h"
#include "dlabel.h"

#include <QLineEdit>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DPasswdEditAnimated : public QFrame
{
    Q_OBJECT

public:
    DPasswdEditAnimated(QWidget *parent);

private Q_SLOTS:
    void setKeyboardVisible(bool value);
    void setCapslockVisible(bool value);
    void setEchoMode(QLineEdit::EchoMode mode);
    void togglePasswdVisible();

private:
    DImageButton *m_keyboard;
    DImageButton *m_eye;
    QLineEdit *m_passwdEdit;
    DLabel *m_caps;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWDEDITANIMATED_H

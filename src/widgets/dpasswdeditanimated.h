#ifndef DPASSWDEDITANIMATED_H
#define DPASSWDEDITANIMATED_H

#include "dimagebutton.h"
#include "dlabel.h"
#include "darrowrectangle.h"

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
    void togglePasswdVisible();

public Q_SLOTS:
    void setEchoMode(QLineEdit::EchoMode mode);
    void showAlert(const QString &message);
    void hideAlert();

private:
    DImageButton *m_keyboard;
    DImageButton *m_eye;
    DImageButton *m_submit;

    DLabel *m_caps;
    DLabel *m_invalidMessage;

    QLineEdit *m_passwdEdit;

    DArrowRectangle *m_invalidTip;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWDEDITANIMATED_H

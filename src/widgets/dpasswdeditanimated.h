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

public Q_SLOTS:
    void setEchoMode(QLineEdit::EchoMode mode);
    void showAlert(const QString &message);
    void hideAlert();
    void showLoadSlider();
    void hideLoadSlider();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void setKeyboardVisible(bool value);
    void setCapslockVisible(bool value);
    void togglePasswdVisible();

private:
    DImageButton *m_keyboard;
    DImageButton *m_eye;
    DImageButton *m_submit;

    DLabel *m_caps;
    DLabel *m_invalidMessage;

    DArrowRectangle *m_invalidTip;
    QLineEdit *m_passwdEdit;

    bool m_isLoading;
    int m_loadSliderX;
    int m_timerID;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWDEDITANIMATED_H

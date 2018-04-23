#ifndef DPASSWDEDITANIMATED_H
#define DPASSWDEDITANIMATED_H

#include "dimagebutton.h"
#include "dlabel.h"
#include "darrowrectangle.h"

#include <QLineEdit>

class __Keyboard;
namespace com {
  namespace deepin {
    namespace daemon {
      namespace inputdevice {
        typedef __Keyboard Keyboard;
      }
    }
  }
}

DWIDGET_BEGIN_NAMESPACE

class DKeyboardMonitor;

class LIBDTKWIDGETSHARED_EXPORT DPasswdEditAnimated : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(bool alert READ alert NOTIFY alertChanged)
    Q_PROPERTY(bool editFocus READ editFocus NOTIFY editFocusChanged)

public:
    DPasswdEditAnimated(QWidget *parent);

Q_SIGNALS:
    void submit(const QString &input);
    void abort();
    void alertChanged(bool alert);
    void editFocusChanged(bool focus);

public Q_SLOTS:
    void setEchoMode(QLineEdit::EchoMode mode);
    void setKeyboardButtonEnable(bool value);
    void setCapslockIndicatorEnable(bool value);
    void setEyeButtonEnable(bool value);
    void setSubmitButtonEnable(bool value);
    void setLoadAnimEnable(bool value);
    void setSubmitIcon(const QString &normalPic, const QString &hoverPic, const QString &pressPic);
    void showAlert(const QString &message);
    void hideAlert();
    void abortAuth();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void resetKeyboardState();
    void resetCapslockState();
    void onEyeButtonClicked();
    void showLoadSlider();
    void hideLoadSlider();
    void inputDone();
    inline bool alert() { return m_invalidTip->isVisible(); }
    inline bool editFocus() { return hasFocus(); }

private:
    DImageButton *m_keyboard;
    DImageButton *m_eye;
    DImageButton *m_submit;

    DLabel *m_caps;
    DLabel *m_invalidMessage;

    DArrowRectangle *m_invalidTip;
    QLineEdit *m_passwdEdit;

    // to get capslock state
    DKeyboardMonitor *m_kbdMonitor;
    // to get user keyboard layout
    com::deepin::daemon::inputdevice::Keyboard *m_kbdInter;

    bool m_keyboardEnable;
    bool m_capsEnable;
    bool m_eyeEnable;
    bool m_submitEnable;
    bool m_loadAnimEnable;
    bool m_isLoading;
    int m_loadSliderX;
    int m_timerID;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWDEDITANIMATED_H

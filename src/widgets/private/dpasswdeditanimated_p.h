#ifndef DPASSWDEDITANIMATED_P_H
#define DPASSWDEDITANIMATED_P_H

#include "dpasswdeditanimated.h"
#include "dimagebutton.h"
#include "dlabel.h"
#include "darrowrectangle.h"
#include "dkeyboardmonitor.h"

#include <DObjectPrivate>
#include <QLineEdit>
#include <com_deepin_daemon_inputdevice_keyboard.h>

using KeyboardInter = com::deepin::daemon::inputdevice::Keyboard;

DWIDGET_BEGIN_NAMESPACE

class DPasswdEditAnimatedPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DPasswdEditAnimated)

public:
    DPasswdEditAnimatedPrivate(DPasswdEditAnimated *q);

    void init();

public Q_SLOTS:
    void _q_resetKeyboardState();
    void _q_resetCapslockState();
    void _q_onEyeButtonClicked();
    void _q_showLoadSlider();
    void _q_hideLoadSlider();
    void _q_inputDone();
    void _q_onKeyboardButtonClicked();

public:
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
    KeyboardInter *m_kbdInter;

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

#endif // DPASSWDEDITANIMATED_P_H

#include "dpasswdeditanimated.h"
#include "DThemeManager"

#include "dkeyboardmonitor.h"

#include <QHBoxLayout>
#include <QKeyEvent>

DWIDGET_BEGIN_NAMESPACE

DPasswdEditAnimated::DPasswdEditAnimated(QWidget *parent) : QFrame(parent)
{
    QHBoxLayout *mainHLayout = new QHBoxLayout(this);
    m_keyboard = new DImageButton;
    m_passwdEdit = new QLineEdit;
    m_caps = new DLabel;
    m_eye = new DImageButton;
    DImageButton *m_submit = new DImageButton;

    m_keyboard->setObjectName("KeyboardButton");
    m_passwdEdit->setObjectName("PasswdEdit");
    m_caps->setObjectName("Capslock");
    m_eye->setObjectName("EyeButton");
    m_submit->setObjectName("SubmitButton");

    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setFrame(false);

    m_caps->setPixmap(QPixmap(":/images/light/images/capslock-hover.svg"));

    mainHLayout->addWidget(m_keyboard);
    mainHLayout->addWidget(m_passwdEdit);
    mainHLayout->addWidget(m_caps);
    mainHLayout->addWidget(m_eye);
    mainHLayout->addWidget(m_submit);

    // TODO: init keyboard state
    m_keyboard->hide();

    DThemeManager::registerWidget(this);

    DKeyboardMonitor *dkm = DKeyboardMonitor::instance();
    dkm->start(QThread::LowestPriority);
    m_caps->setVisible(dkm->isCapslockOn());

    connect(dkm, &DKeyboardMonitor::capslockStatusChanged, this, &DPasswdEditAnimated::setCapslockVisible);
    connect(m_eye, &DImageButton::clicked, this, &DPasswdEditAnimated::togglePasswdVisible);
}

void DPasswdEditAnimated::setKeyboardVisible(bool value)
{
    m_keyboard->setVisible(value);
}

void DPasswdEditAnimated::setCapslockVisible(bool value)
{
    m_caps->setVisible(value);
}

void DPasswdEditAnimated::setEchoMode(QLineEdit::EchoMode mode)
{
    m_passwdEdit->setEchoMode(mode);
}

void DPasswdEditAnimated::togglePasswdVisible()
{
    if (m_passwdEdit->echoMode() == QLineEdit::Password) {
        this->setEchoMode(QLineEdit::Normal);
        return;
    }
    this->setEchoMode(QLineEdit::Password);
}

DWIDGET_END_NAMESPACE

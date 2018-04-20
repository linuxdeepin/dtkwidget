#include "dpasswdeditanimated.h"
#include "dkeyboardmonitor.h"

#include "DThemeManager"
#include <QHBoxLayout>
#include <com_deepin_daemon_inputdevice_keyboard.h>

using KeyboardInter = com::deepin::daemon::inputdevice::Keyboard;

DWIDGET_BEGIN_NAMESPACE

DPasswdEditAnimated::DPasswdEditAnimated(QWidget *parent) : QFrame(parent)
{
    QHBoxLayout *mainHLayout = new QHBoxLayout(this);

    m_keyboard = new DImageButton;
    m_passwdEdit = new QLineEdit;
    m_caps = new DLabel;
    m_eye = new DImageButton;
    m_submit = new DImageButton;
    m_invalidMessage = new DLabel;
    m_invalidTip = new DArrowRectangle(DArrowRectangle::ArrowTop, this);

    m_keyboardEnable = true;
    m_capsEnable = true;
    m_eyeEnable = true;
    m_submitEnable = true;
    m_isLoading = false;
    m_loadSliderX = 0;
    m_timerID = 0;

    m_keyboard->setObjectName("KeyboardButton");
    m_passwdEdit->setObjectName("PasswdEdit");
    m_caps->setObjectName("Capslock");
    m_eye->setObjectName("EyeButton");
    m_submit->setObjectName("SubmitButton");
    m_invalidTip->setObjectName("InvalidTip");

    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setFrame(false);

    m_caps->setPixmap(QPixmap(":/images/light/images/capslock-hover.svg"));

    m_invalidTip->hide();

    mainHLayout->addWidget(m_keyboard, 0, Qt::AlignLeft);
    mainHLayout->addWidget(m_passwdEdit, 1, Qt::AlignLeft);
    mainHLayout->addWidget(m_caps, 0, Qt::AlignRight);
    mainHLayout->addWidget(m_eye, 0, Qt::AlignRight);
    mainHLayout->addWidget(m_submit, 0, Qt::AlignRight);

    DThemeManager::registerWidget(this, QStringList("alert"));

    m_kbdMonitor = DKeyboardMonitor::instance();
    m_kbdMonitor->start(QThread::LowestPriority);
    resetCapslockState();

    m_kbdInter = new KeyboardInter(QString("com.deepin.daemon.SystemInfo"),
                                                QString("/com/deepin/daemon/InputDevice/Keyboard"),
                                                QDBusConnection::sessionBus(), this);
    resetKeyboardState();

    connect(m_kbdMonitor, &DKeyboardMonitor::capslockStatusChanged, this, &DPasswdEditAnimated::resetCapslockState);
    connect(m_kbdInter, &KeyboardInter::UserLayoutListChanged, this, &DPasswdEditAnimated::resetKeyboardState);
    connect(m_eye, &DImageButton::clicked, this, &DPasswdEditAnimated::onEyeButtonClicked);
    connect(m_passwdEdit, &QLineEdit::returnPressed, this, &DPasswdEditAnimated::inputDone);
    connect(m_submit, &DImageButton::clicked, this, &DPasswdEditAnimated::inputDone);
}

void DPasswdEditAnimated::setKeyboardButtonEnable(bool value)
{
    if (m_keyboardEnable != value) {
        m_keyboardEnable = value;
        resetKeyboardState();
    }
}

void DPasswdEditAnimated::setCapslockIndicatorEnable(bool value)
{
    if (m_capsEnable != value) {
        m_capsEnable = value;
        resetCapslockState();
    }
}

void DPasswdEditAnimated::setEyeButtonEnable(bool value)
{
    if (m_eyeEnable != value) {
        m_eyeEnable = value;
        m_eye->setVisible(m_eyeEnable);
    }
}

void DPasswdEditAnimated::setSubmitButtonEnable(bool value)
{
    if (m_submitEnable != value) {
        m_submitEnable = value;
        m_submit->setVisible(m_submitEnable);
    }
}

void DPasswdEditAnimated::setEchoMode(QLineEdit::EchoMode mode)
{
    m_passwdEdit->setEchoMode(mode);
}

void DPasswdEditAnimated::onEyeButtonClicked()
{
    if (m_passwdEdit->echoMode() == QLineEdit::Password) {
        this->setEchoMode(QLineEdit::Normal);
        return;
    }
    this->setEchoMode(QLineEdit::Password);
}

void DPasswdEditAnimated::inputDone()
{
    QString input = m_passwdEdit->text();
    if (input.length() > 0) {
        showLoadSlider();
        Q_EMIT submit(input);
    }
}

void DPasswdEditAnimated::showAlert(const QString &message)
{
    hideLoadSlider();

    m_passwdEdit->selectAll();

    m_invalidMessage->setText(message);
    m_invalidMessage->adjustSize();

    if (!m_invalidTip->isVisible()) {
        m_invalidTip->setContent(m_invalidMessage);
        m_invalidTip->adjustSize();
        QPoint pos = mapToGlobal(m_passwdEdit->pos());
        m_invalidTip->move(pos.x() + m_invalidTip->width() / 2, pos.y() + 20);
        m_invalidTip->setArrowX(20);
        m_invalidTip->setVisible(true);
        setAlert(true);
    }

}

void DPasswdEditAnimated::hideAlert()
{
    if (m_invalidTip->isVisible()) {
        m_invalidTip->setVisible(false);
        setAlert(false);
    }

}

void DPasswdEditAnimated::setSubmitIcon(const QString &normalPic, const QString &hoverPic, const QString &pressPic)
{
    m_submit->setNormalPic(normalPic);
    m_submit->setHoverPic(hoverPic);
    m_submit->setPressPic(pressPic);
}

void DPasswdEditAnimated::showLoadSlider()
{
    if (!m_isLoading) {
        m_isLoading = true;
        m_loadSliderX = 0;
        m_timerID = startTimer(5);
    }
}

void DPasswdEditAnimated::hideLoadSlider()
{
    if (m_timerID != 0 && m_isLoading) {
        killTimer(m_timerID);
        m_isLoading = false;
        m_timerID = 0;
    }
}

void DPasswdEditAnimated::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    update();
}

void DPasswdEditAnimated::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (m_isLoading) {
        QPainter painter(this);
    //    painter.setOpacity(0.8);
        QLinearGradient grad(m_loadSliderX, height() / 2, 40 + m_loadSliderX, height() / 2);
        grad.setColorAt(0.0, Qt::transparent);
        grad.setColorAt(1.0, Qt::white);
        painter.fillRect(m_loadSliderX, 1, 40, height() - 2, grad);

        m_loadSliderX = m_loadSliderX + 1;
        if (m_loadSliderX >= width()) {
            m_loadSliderX = 0;
        }
    }
}

void DPasswdEditAnimated::resetKeyboardState()
{
    if (m_keyboardEnable) {
        if (m_kbdInter->userLayoutList().length() > 1) {
            m_keyboard->setVisible(true);
            return;
        }
    }
    m_keyboard->setVisible(false);
}

void DPasswdEditAnimated::resetCapslockState()
{
    if (m_capsEnable) {
        if (m_kbdMonitor->isCapslockOn()) {
            m_caps->setVisible(true);
            return;
        }
    }
    m_caps->setVisible(false);
}

DWIDGET_END_NAMESPACE

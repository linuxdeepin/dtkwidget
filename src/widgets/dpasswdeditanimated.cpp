#include "dpasswdeditanimated.h"
#include "DThemeManager"

#include "dkeyboardmonitor.h"

#include <QHBoxLayout>

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

void DPasswdEditAnimated::showAlert(const QString &message)
{
    m_invalidMessage->setText(message);
    m_invalidMessage->adjustSize();

    if (!m_invalidTip->isVisible()) {
        m_invalidTip->setContent(m_invalidMessage);
        m_invalidTip->adjustSize();
        QPoint pos = mapToGlobal(m_passwdEdit->pos());
        m_invalidTip->move(pos.x() + m_invalidTip->width() / 2, pos.y() + 20);
        m_invalidTip->setArrowX(20);
        m_invalidTip->setVisible(true);
        setProperty("alert", true);
    }

    // qss will not work unless reset StyleSheet
    setStyleSheet(styleSheet());
}

void DPasswdEditAnimated::hideAlert()
{
    if (m_invalidTip->isVisible()) {
        m_invalidTip->setVisible(false);
        setProperty("alert", false);
    }

    // qss will not work unless reset StyleSheet
    setStyleSheet(styleSheet());
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

DWIDGET_END_NAMESPACE

#include "dpasswdeditanimated.h"
#include "dkeyboardmonitor.h"
#include "private/dpasswdeditanimated_p.h"

#include "DThemeManager"
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

DPasswdEditAnimatedPrivate::DPasswdEditAnimatedPrivate(DPasswdEditAnimated *q)
    : DTK_CORE_NAMESPACE::DObjectPrivate(q)
{
}

void DPasswdEditAnimatedPrivate::init()
{
    D_Q(DPasswdEditAnimated);

    QHBoxLayout *mainHLayout = new QHBoxLayout(q);

    m_keyboard = new DImageButton;
    m_passwdEdit = new QLineEdit;
    m_caps = new DLabel;
    m_eye = new DImageButton;
    m_submit = new DImageButton;
    m_invalidMessage = new DLabel;
    m_invalidTip = new DArrowRectangle(DArrowRectangle::ArrowTop, q);

    m_keyboardEnable = true;
    m_capsEnable = true;
    m_eyeEnable = true;
    m_submitEnable = true;
    m_loadAnimEnable = true;
    m_isLoading = false;
    m_loadSliderX = 0;
    m_timerID = 0;

    m_keyboard->setObjectName("KeyboardButton");
    m_passwdEdit->setObjectName("PasswdEdit");
    m_caps->setObjectName("Capslock");
    m_eye->setObjectName("EyeButton");
    m_submit->setObjectName("SubmitButton");
    m_invalidMessage->setObjectName("InvalidMessage");
    m_invalidTip->setObjectName("InvalidTip");

    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setFrame(false);
    m_passwdEdit->installEventFilter(q);
    q->setFocusPolicy(Qt::StrongFocus);
    q->setFocusProxy(m_passwdEdit);

    m_caps->setPixmap(QPixmap(":/images/light/images/capslock-hover.svg"));

    m_invalidTip->hide();

    mainHLayout->addWidget(m_keyboard, 0, Qt::AlignLeft);
    mainHLayout->addWidget(m_passwdEdit, 1, Qt::AlignLeft);
    mainHLayout->addWidget(m_caps, 0, Qt::AlignRight);
    mainHLayout->addWidget(m_eye, 0, Qt::AlignRight);
    mainHLayout->addWidget(m_submit, 0, Qt::AlignRight);

    mainHLayout->setContentsMargins(5, 5, 5, 5);
    q->setLayout(mainHLayout);

    DThemeManager::registerWidget(q, QStringList("alert") << "editFocus");

    m_kbdMonitor = DKeyboardMonitor::instance();
    m_kbdMonitor->start(QThread::LowestPriority);
    _q_resetCapslockState();

    m_kbdInter = new KeyboardInter(QString("com.deepin.daemon.SystemInfo"),
                                                QString("/com/deepin/daemon/InputDevice/Keyboard"),
                                                QDBusConnection::sessionBus(), q);
    _q_resetKeyboardState();

    q->connect(m_kbdMonitor, SIGNAL(capslockStatusChanged(bool)), q, SLOT(_q_resetCapslockState()));
    q->connect(m_kbdInter, SIGNAL(UserLayoutListChanged(const QStringList &)), q, SLOT(_q_resetKeyboardState()));
    q->connect(m_eye, SIGNAL(clicked()), q, SLOT(_q_onEyeButtonClicked()));
    q->connect(m_passwdEdit, SIGNAL(returnPressed()), q, SLOT(_q_inputDone()));
    q->connect(m_passwdEdit, SIGNAL(selectionChanged()), q, SLOT(hideAlert()));
    q->connect(m_submit, SIGNAL(clicked()), q, SLOT(_q_inputDone()));
    q->connect(m_keyboard, SIGNAL(clicked()), q, SLOT(_q_onKeyboardButtonClicked()));
}

void DPasswdEditAnimatedPrivate::_q_onEyeButtonClicked()
{
    D_Q(DPasswdEditAnimated);

    if (m_passwdEdit->echoMode() == QLineEdit::Password) {
        q->setEchoMode(QLineEdit::Normal);
        return;
    }
    q->setEchoMode(QLineEdit::Password);
}

void DPasswdEditAnimatedPrivate::_q_inputDone()
{
    D_Q(DPasswdEditAnimated);

    QString input = m_passwdEdit->text();
    if (input.length() > 0) {
        _q_showLoadSlider();
        Q_EMIT q->submit(input);
    }
}

void DPasswdEditAnimatedPrivate::_q_onKeyboardButtonClicked()
{
    D_Q(DPasswdEditAnimated);

    Q_EMIT q->keyboardButtonClicked();
}

void DPasswdEditAnimatedPrivate::_q_showLoadSlider()
{
    D_Q(DPasswdEditAnimated);

    if (m_loadAnimEnable) {
        if (!m_isLoading) {
            m_isLoading = true;
            m_loadSliderX = 0;
            m_timerID = q->startTimer(5);
        }
    }
}

void DPasswdEditAnimatedPrivate::_q_hideLoadSlider()
{
    D_Q(DPasswdEditAnimated);

    if (m_timerID != 0 && m_isLoading) {
        q->killTimer(m_timerID);
        m_isLoading = false;
        m_timerID = 0;
        q->update();
    }
}

void DPasswdEditAnimatedPrivate::_q_resetKeyboardState()
{
    if (m_keyboardEnable) {
        if (m_kbdInter->userLayoutList().length() > 1) {
            m_keyboard->setVisible(true);
            return;
        }
    }
    m_keyboard->setVisible(false);
}

void DPasswdEditAnimatedPrivate::_q_resetCapslockState()
{
    if (m_capsEnable) {
        if (m_kbdMonitor->isCapslockOn()) {
            m_caps->setVisible(true);
            return;
        }
    }
    m_caps->setVisible(false);
}

DPasswdEditAnimated::DPasswdEditAnimated(QWidget *parent) : QFrame(parent),
    DTK_CORE_NAMESPACE::DObject(*new DPasswdEditAnimatedPrivate(this))
{
    D_D(DPasswdEditAnimated);

    d->init();
}

void DPasswdEditAnimated::setKeyboardButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_keyboardEnable != value) {
        d->m_keyboardEnable = value;
        d->_q_resetKeyboardState();
    }
}

void DPasswdEditAnimated::setCapslockIndicatorEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_capsEnable != value) {
        d->m_capsEnable = value;
        d->_q_resetCapslockState();
    }
}

void DPasswdEditAnimated::setEyeButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_eyeEnable != value) {
        d->m_eyeEnable = value;
        d->m_eye->setVisible(d->m_eyeEnable);
    }
}

void DPasswdEditAnimated::setSubmitButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_submitEnable != value) {
        d->m_submitEnable = value;
        d->m_submit->setVisible(d->m_submitEnable);
    }
}

void DPasswdEditAnimated::setLoadAnimEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    d->m_loadAnimEnable = value;
}

void DPasswdEditAnimated::setEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DPasswdEditAnimated);

    d->m_passwdEdit->setEchoMode(mode);
}

bool DPasswdEditAnimated::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DPasswdEditAnimated);

    if (watched == d->m_passwdEdit) {
        switch (event->type()) {
        case QEvent::FocusIn:
            Q_EMIT editFocusChanged(true);
            break;
        case QEvent::FocusOut:
            Q_EMIT editFocusChanged(false);
            break;
        default:
            break;
        }
    }
    return false;
}

void DPasswdEditAnimated::showAlert(const QString &message)
{
    D_D(DPasswdEditAnimated);

    d->_q_hideLoadSlider();

    d->m_invalidMessage->setText(message);
    d->m_invalidMessage->adjustSize();

    d->m_passwdEdit->selectAll();
    d->m_passwdEdit->setFocus();

    if (!d->m_invalidTip->isVisible()) {
        d->m_invalidTip->setContent(d->m_invalidMessage);
        d->m_invalidTip->adjustSize();
        QPoint pos = mapToGlobal(d->m_passwdEdit->pos());
        d->m_invalidTip->move(pos.x() + d->m_invalidTip->width() / 2, pos.y() + 20);
        d->m_invalidTip->setArrowX(20);
        d->m_invalidTip->setVisible(true);
        Q_EMIT alertChanged(true);
    }

}

void DPasswdEditAnimated::hideAlert()
{
    D_D(DPasswdEditAnimated);

    if (d->m_invalidTip->isVisible()) {
        d->m_invalidTip->setVisible(false);
        Q_EMIT alertChanged(false);
    }

}

void DPasswdEditAnimated::abortAuth()
{
    D_D(DPasswdEditAnimated);

    if (d->m_isLoading) {
        d->_q_hideLoadSlider();
        Q_EMIT abort();
    }
}

bool DPasswdEditAnimated::alert()
{
    D_D(DPasswdEditAnimated);

    return d->m_invalidTip->isVisible();
}

void DPasswdEditAnimated::setSubmitIcon(const QString &normalPic, const QString &hoverPic, const QString &pressPic)
{
    D_D(DPasswdEditAnimated);

    d->m_submit->setNormalPic(normalPic);
    d->m_submit->setHoverPic(hoverPic);
    d->m_submit->setPressPic(pressPic);
}

void DPasswdEditAnimated::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    update();
}

void DPasswdEditAnimated::paintEvent(QPaintEvent *event)
{
    D_D(DPasswdEditAnimated);

    QFrame::paintEvent(event);

    if (d->m_isLoading) {
        QPainter painter(this);
    //    painter.setOpacity(0.8);
        QLinearGradient grad(d->m_loadSliderX, height() / 2, 40 + d->m_loadSliderX, height() / 2);
        grad.setColorAt(0.0, Qt::transparent);
        grad.setColorAt(1.0, Qt::gray);
        painter.fillRect(d->m_loadSliderX, 1, 40, height() - 2, grad);

        d->m_loadSliderX = d->m_loadSliderX + 1;
        if (d->m_loadSliderX >= width()) {
            d->m_loadSliderX = 0;
        }
    }
}

#include "moc_dpasswdeditanimated.cpp"

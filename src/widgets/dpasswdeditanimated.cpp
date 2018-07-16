#include "dpasswdeditanimated.h"
#include "dkeyboardmonitor.h"
#include "private/dpasswdeditanimated_p.h"

#include "DThemeManager"
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

LoadSlider::LoadSlider(QWidget *parent) : QWidget(parent),
    m_loadSliderColor(Qt::gray)
{
}

void LoadSlider::setLoadSliderColor(const QColor &color)
{
    m_loadSliderColor = color;
    update();
}

void LoadSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//    painter.setOpacity(0.8);
    QLinearGradient grad(0, height() / 2, width(), height() / 2);
    grad.setColorAt(0.0, Qt::transparent);
    grad.setColorAt(1.0, m_loadSliderColor);
    painter.fillRect(0, 1, width(), height() - 2, grad);

    QWidget::paintEvent(event);
}

QColor LoadSlider::loadSliderColor() const
{
    return m_loadSliderColor;
}

DPasswdEditAnimatedPrivate::DPasswdEditAnimatedPrivate(DPasswdEditAnimated *q)
    : DTK_CORE_NAMESPACE::DObjectPrivate(q)
{
}

void DPasswdEditAnimatedPrivate::init()
{
    D_Q(DPasswdEditAnimated);

    QHBoxLayout *mainHLayout = new QHBoxLayout(q);
    QVBoxLayout *passwdVBLayout = new QVBoxLayout;

    m_keyboard = new DImageButton;
    m_passwdEdit = new QLineEdit;
    m_caps = new DLabel;
    m_eye = new DImageButton;
    m_submit = new DImageButton;
    m_invalidTip = new DArrowRectangle(DArrowRectangle::ArrowTop, q);
    m_invalidMessage = new DLabel(m_invalidTip);

    m_loadSlider = new LoadSlider(q);
    m_loadSlider->hide();
    m_loadSliderAnim = new QPropertyAnimation(m_loadSlider, "pos", m_loadSlider);
    m_loadSliderAnim->setDuration(1000);
    m_loadSliderAnim->setLoopCount(-1);
    m_loadSliderAnim->setEasingCurve(QEasingCurve::Linear);

    m_capsEnable = true;
    m_eyeEnable = true;
    m_submitEnable = true;
    m_loadAnimEnable = true;
    m_isLoading = false;

    m_keyboard->setObjectName("KeyboardButton");
    m_passwdEdit->setObjectName("PasswdEdit");
    m_caps->setObjectName("Capslock");
    m_eye->setObjectName("EyeButton");
    m_submit->setObjectName("SubmitButton");
    m_invalidMessage->setObjectName("InvalidMessage");
    m_invalidTip->setObjectName("InvalidTip");

    m_keyboard->setStyleSheet("background-color: transparent;");
    m_passwdEdit->setStyleSheet("background-color: transparent;");
    m_caps->setStyleSheet("background-color: transparent;");
    m_eye->setStyleSheet("background-color: transparent;");
    m_submit->setStyleSheet("background-color: transparent;");

    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setFrame(false);
    m_passwdEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_passwdEdit->installEventFilter(q);
    q->setFocusPolicy(Qt::StrongFocus);
    q->setFocusProxy(m_passwdEdit);

    m_caps->setPixmap(QPixmap(":/images/light/images/capslock-hover.svg"));

    m_invalidMessage->hide();
    m_invalidTip->hide();

    // fix QLineEdit do not expand width in QHBoxLayout
    passwdVBLayout->setMargin(0);
    passwdVBLayout->setContentsMargins(0, 0, 0, 0);
    passwdVBLayout->setSpacing(0);
    passwdVBLayout->addWidget(m_passwdEdit);

    mainHLayout->addWidget(m_keyboard, 0, Qt::AlignLeft);
    mainHLayout->addLayout(passwdVBLayout, 1);
    mainHLayout->addWidget(m_caps, 0, Qt::AlignRight);
    mainHLayout->addWidget(m_eye, 0, Qt::AlignRight);
    mainHLayout->addWidget(m_submit, 0, Qt::AlignRight);

    mainHLayout->setContentsMargins(5, 5, 5, 5);
    q->setLayout(mainHLayout);

    DThemeManager::registerWidget(q, QStringList("alert") << "editFocus");

    m_kbdMonitor = DKeyboardMonitor::instance();
    m_kbdMonitor->start(QThread::LowestPriority);
    _q_resetCapslockState();

    q->connect(m_kbdMonitor, SIGNAL(capslockStatusChanged(bool)), q, SLOT(_q_resetCapslockState()));
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

    q->hideAlert();

    QString input = m_passwdEdit->text();
    if (input.length() > 0) {
        _q_showLoadSlider();
        Q_EMIT q->submit(input);
    }
}

void DPasswdEditAnimatedPrivate::_q_onKeyboardButtonClicked()
{
    D_Q(DPasswdEditAnimated);

    q->hideAlert();

    Q_EMIT q->keyboardButtonClicked();
}

void DPasswdEditAnimatedPrivate::_q_showLoadSlider()
{
    D_Q(DPasswdEditAnimated);

    if (m_loadAnimEnable) {
        if (!m_isLoading) {
            m_isLoading = true;
            m_loadSlider->show();
            m_loadSlider->setGeometry(0, 0, LoadSliderWidth, q->height());
            m_loadSliderAnim->setStartValue(QPoint(0 - LoadSliderWidth, 0));
            m_loadSliderAnim->setEndValue(QPoint(q->width(), 0));
            m_loadSliderAnim->start();
        }
    }
}

void DPasswdEditAnimatedPrivate::_q_hideLoadSlider()
{
    D_Q(DPasswdEditAnimated);

    if (m_isLoading) {
        m_isLoading = false;
        m_loadSliderAnim->stop();
        m_loadSlider->hide();
    }
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

QLineEdit *DPasswdEditAnimated::lineEdit()
{
    D_D(DPasswdEditAnimated);

    return d->m_passwdEdit;
}
QLabel *DPasswdEditAnimated::invalidMessage()
{
    D_D(DPasswdEditAnimated);

    return d->m_invalidMessage;
}

QPropertyAnimation *DPasswdEditAnimated::loadingAnimation()
{
    D_D(DPasswdEditAnimated);

    return d->m_loadSliderAnim;
}

void DPasswdEditAnimated::setKeyboardButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    d->m_keyboard->setVisible(value);
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

    d->m_passwdEdit->selectAll();
    d->m_passwdEdit->setFocus();

    if (!d->m_invalidTip->isVisible()) {
        // focus BUG
        //d->m_invalidTip->setContent(d->m_invalidMessage);
        //d->m_invalidTip->adjustSize();

        QPoint pos = mapToGlobal(rect().bottomLeft());
        QFontMetrics fm((QFont()));
        d->m_invalidTip->setGeometry(pos.x(), pos.y() + 5, fm.width(message) + 20, fm.height() + 20);
        d->m_invalidMessage->move(10, 15);

        d->m_invalidTip->setArrowX(20);
        d->m_invalidMessage->show();
        d->m_invalidTip->QWidget::show();

        Q_EMIT alertChanged(true);
    }
}

void DPasswdEditAnimated::hideAlert()
{
    D_D(DPasswdEditAnimated);

    if (d->m_invalidTip->isVisible()) {
        d->m_invalidMessage->hide();
        d->m_invalidTip->hide();

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

QColor DPasswdEditAnimated::loadingEffectColor()
{
    D_D(DPasswdEditAnimated);

    return d->m_loadSlider->loadSliderColor();
}

void DPasswdEditAnimated::setLoadingEffectColor(const QColor &color)
{
    D_D(DPasswdEditAnimated);

    d->m_loadSlider->setLoadSliderColor(color);
}

#include "moc_dpasswdeditanimated.cpp"

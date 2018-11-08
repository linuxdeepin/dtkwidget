#include "dpasswdeditanimated.h"
#include "dkeyboardmonitor.h"
#include "private/dpasswdeditanimated_p.h"

#include "DThemeManager"
#include <QHBoxLayout>

DWIDGET_BEGIN_NAMESPACE

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
    m_alterBeforeHide = false;

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
    m_passwdEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

/**
 * \~chinese \class DPasswdEditAnimated
 * \~chinese \brief 带有动画的密码输入框。
 *
 * \~chinese DPasswdEditAnimated 一个带有动画，大小写标识，键盘布局按钮的密码输入框。
 *
 * \~chinese 动画的效果是一个半透明滑块从左向右滑动，
 * \~chinese 可以通过 DPasswdEditAnimated::setLoadingEffectColor 来设置滑块的颜色。
 * \~chinese 也可以通过 DPasswdEditAnimated::loadingAnimation 拿到 QPropertyAnimation 对象并设置其属性，
 * \~chinese 以改变动画的一些行为。
 */

// ========================PROPERTY START========================
/**
 * \~chinese \property DPasswdEditAnimated::editFocus
 * \~chinese \brief 获取焦点的状态
 */
// ========================PROPERTY END========================

// ========================SIGNAL START========================
/**
 * \~chinese \fn DPasswdEditAnimated::submit
 * \~chinese \brief 提交输入完成数据，一般是因为按下了回车或者点击了提交按钮。
 * \~chinese \param input 输入的字符串。
 */

/**
 * \~chinese \fn DPasswdEditAnimated::abort
 * \~chinese \brief 过程被终止。
 */

/**
 * \~chinese \fn DPasswdEditAnimated::alertChanged
 * \~chinese \brief 属性 DPasswdEditAnimated::alert 属性发生改变。
 */

/**
 * \~chinese \fn DPasswdEditAnimated::editFocusChanged
 * \~chinese \brief 属性 DPasswdEditAnimated::editFocus 发生改变。
 */

/**
 * \~chinese \fn DPasswdEditAnimated::keyboardButtonClicked
 * \~chinese \brief 键盘布局按钮被点击。
 */
// ========================SIGNAL END========================

/**
 * \~chinese \brief 构造一个 DPasswdEditAnimated 实例。
 * \~chinese \param parent 为创建对象的父控件。
 */
DPasswdEditAnimated::DPasswdEditAnimated(QWidget *parent) : QFrame(parent),
    DTK_CORE_NAMESPACE::DObject(*new DPasswdEditAnimatedPrivate(this))
{
    D_D(DPasswdEditAnimated);

    d->init();
}

/**
 * \~chinese \brief 获取输入框对象。
 *
 * \~chinese 拿到QLineEdit类型的输入框对象后可对其进行一些常规设置，如设置最大输入长度等。
 * \~chinese \return 输入框对象。
 */
QLineEdit *DPasswdEditAnimated::lineEdit()
{
    D_D(DPasswdEditAnimated);

    return d->m_passwdEdit;
}

/**
 * \~chinese \brief 获取警告信息。
 *
 * \~chinese 可以用获取到对象来设置要显示的警告信息。
 * \~chinese \return 警告信息对象。
 */
QLabel *DPasswdEditAnimated::invalidMessage()
{
    D_D(DPasswdEditAnimated);

    return d->m_invalidMessage;
}

/**
 * \~chinese \brief 获取动画对象。
 *
 * \~chinese 可以用获取到的对象设置动画的属性。
 * \~chinese \return 动画对象。
 */
QPropertyAnimation *DPasswdEditAnimated::loadingAnimation()
{
    D_D(DPasswdEditAnimated);

    return d->m_loadSliderAnim;
}

/**
 * \~chinese \brief 设置是否需要键盘布局按钮。
 *
 * \~chinese \param value 为true表示需要，反之则反。
 */
void DPasswdEditAnimated::setKeyboardButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    d->m_keyboard->setVisible(value);
}

/**
 * \~chinese \brief 设置是否需要大小写指示器。
 *
 * \~chinese \param value 为true表示需要，反之则反。
 */
void DPasswdEditAnimated::setCapslockIndicatorEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_capsEnable != value) {
        d->m_capsEnable = value;
        d->_q_resetCapslockState();
    }
}

/**
 * \~chinese \brief 设置是否需要切换密码隐藏状态的按钮。
 *
 * \~chinese \param value 为true表示需要，反之则反。
 */
void DPasswdEditAnimated::setEyeButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_eyeEnable != value) {
        d->m_eyeEnable = value;
        d->m_eye->setVisible(d->m_eyeEnable);
    }
}

/**
 * \~chinese \brief 设置是否需要提交按钮。
 *
 * \~chinese \param value 为true表示需要，反之则反。
 */
void DPasswdEditAnimated::setSubmitButtonEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    if (d->m_submitEnable != value) {
        d->m_submitEnable = value;
        d->m_submit->setVisible(d->m_submitEnable);
    }
}

/**
 * \~chinese \brief 设置是否需要动画功能。
 *
 * \~chinese \param value 为true表示需要，反之则反。
 */
void DPasswdEditAnimated::setLoadAnimEnable(bool value)
{
    D_D(DPasswdEditAnimated);

    d->m_loadAnimEnable = value;
}

/**
 * \~chinese \brief 设置是密码的显示状态。
 *
 * \~chinese \param value 显示状态。
 *
 * \sa QLineEdit::EchoMode
 */
void DPasswdEditAnimated::setEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DPasswdEditAnimated);

    d->m_passwdEdit->setEchoMode(mode);
}

/** \reimp */
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

/*!
 * \reimp
 */
void DPasswdEditAnimated::showEvent(QShowEvent *event)
{
    D_D(DPasswdEditAnimated);

    // show alert message if it is visible when widget is hidden
    if (d->m_alterBeforeHide) {
        showAlert(d->m_invalidMessage->text());
    }

    QFrame::showEvent(event);
}

/*!
 * \reimp
 */
void DPasswdEditAnimated::hideEvent(QHideEvent *event)
{
    D_D(DPasswdEditAnimated);

    // remember alert visible before hide
    d->m_alterBeforeHide = alert();

    hideAlert();

    QFrame::hideEvent(event);
}

/**
 * \~chinese \brief 显示警告信息。
 *
 * \~chinese 弹出一个警告信息提示框。
 * \~chinese \param message 要显示的信息。
 */
void DPasswdEditAnimated::showAlert(const QString &message)
{
    D_D(DPasswdEditAnimated);

    d->_q_hideLoadSlider();

    d->m_invalidMessage->setText(message);

    d->m_passwdEdit->selectAll();
    d->m_passwdEdit->setFocus();

    if (isVisible() == false) {
        d->m_alterBeforeHide = true;
        return;
    }

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

/**
 * \~chinese \brief 隐藏警告提示框。
 */
void DPasswdEditAnimated::hideAlert()
{
    D_D(DPasswdEditAnimated);

    if (d->m_invalidTip->isVisible()) {
        d->m_invalidTip->setVisible(false);

        Q_EMIT alertChanged(false);
    }
}

/**
 * \~chinese \brief 终止认证过程，同时会关闭警告提示框和停止动画。
 */
void DPasswdEditAnimated::abortAuth()
{
    D_D(DPasswdEditAnimated);

    if (d->m_isLoading) {
        d->_q_hideLoadSlider();
        Q_EMIT abort();
    }
}

/**
 * \~chinese \property DPasswdEditAnimated::alert
 * \~chinese \brief 警告信息的显示状态。
 * \~chinese \return true 正在显示警告信息，反之则反。
 */
bool DPasswdEditAnimated::alert()
{
    D_D(DPasswdEditAnimated);

    return d->m_invalidTip->isVisible();
}

/**
 * \~chinese \brief 设置提交按钮的图标。
 *
 * \~chinese 参数可以是文件系统中的文件路径，也可以是资源系统中的路径。
 * \~chinese \param normalPic 正常状态下的图标。
 * \~chinese \param hoverPic 鼠标悬浮状态下的图标。
 * \~chinese \param pressPic 鼠标按下状态下的图标。
 */
void DPasswdEditAnimated::setSubmitIcon(const QString &normalPic, const QString &hoverPic, const QString &pressPic)
{
    D_D(DPasswdEditAnimated);

    d->m_submit->setNormalPic(normalPic);
    d->m_submit->setHoverPic(hoverPic);
    d->m_submit->setPressPic(pressPic);
}

/**
 * \~chinese \property DPasswdEditAnimated::loadingEffectColor
 * \~chinese \brief 动画的颜色。
 * \~chinese \sa DPasswdEditAnimated::setLoadingEffectColor
 */
QColor DPasswdEditAnimated::loadingEffectColor()
{
    D_D(DPasswdEditAnimated);

    return d->m_loadSlider->loadSliderColor();
}

/**
 * \~chinese \brief 设置动画的颜色。
 *
 * \~chinese \param color 动画的颜色。
 */
void DPasswdEditAnimated::setLoadingEffectColor(const QColor &color)
{
    D_D(DPasswdEditAnimated);

    d->m_loadSlider->setLoadSliderColor(color);
}

DWIDGET_END_NAMESPACE

#include "moc_dpasswdeditanimated.cpp"

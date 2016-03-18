#include "dwindows.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>

#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

#include <dobject.h>
#include <private/dobject_p.h>
#include <dgraphicsgloweffect.h>
#include <dtitlebar.h>
DWIDGET_BEGIN_NAMESPACE

const int WindowGlowRadius = 8;
const int WindowsRadius = 4;

const QColor BorderColor = QColor(216, 216, 216);
const QColor ShadowColor = QColor(23, 23, 23, 128);
const QColor BackgroundTopColor = QColor(250, 251, 252);
const QColor BackgroundBottonColor = QColor(250, 251, 252);
const QColor TipsBorderColor = QColor(255, 255, 255, 255 * 0.2);
const QColor TipsBackground = QColor(0, 0, 0);

class DWindowPrivate: public DObjectPrivate
{
protected:
    DWindowPrivate(DWindow *qq);

private:
    void init();

    int     m_GlowRadius;
    int     m_Radius;
    QPoint  m_LastMousePos;
    bool    m_MousePressed;
    DGraphicsGlowEffect *m_Shadow;

    QVBoxLayout *rootLayout;
    DTitlebar   *titlebar;
    DWidget     *contentWidget;
    Qt::WindowFlags dwindowFlags;
    D_DECLARE_PUBLIC(DWindow)
};

DWindowPrivate::DWindowPrivate(DWindow *qq): DObjectPrivate(qq)
{

}

void DWindowPrivate::init()
{
    D_Q(DWindow);

    dwindowFlags = Qt::Window | Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint |
                   Qt::WindowCloseButtonHint | Qt::WindowFullscreenButtonHint;
    m_GlowRadius = WindowGlowRadius;
    m_Radius = WindowsRadius;
    m_MousePressed = false;
    m_Shadow = nullptr;

    rootLayout = new QVBoxLayout;
    rootLayout->setMargin(0);
    rootLayout->setSpacing(0);

    titlebar = new DTitlebar;
    contentWidget = new DWidget;
    rootLayout->addWidget(titlebar);
    rootLayout->addWidget(contentWidget);

    q->connect(titlebar, &DTitlebar::closeClicked, q, &DWindow::close);
    q->connect(titlebar, &DTitlebar::maximumClicked, q, &DWindow::showMaximized);
    q->connect(titlebar, &DTitlebar::restoreClicked, q, &DWindow::showNormal);
    q->connect(titlebar, &DTitlebar::minimumClicked, q, &DWindow::showMinimized);

    q->setLayout(rootLayout);
}

DWindow::DWindow(DWidget *parent) :
    DWidget(*new DWindowPrivate(this), parent)
{
    D_D(DWindow);
    d->init();

    QWidget::setAttribute(Qt::WA_TranslucentBackground, true);
    QWidget::setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

#ifdef Q_OS_WIN
    // call this in windows platform
    setShadowHits();
#endif
    // Set Titlebar flag
    setWindowFlags(windowFlags());
}


Qt::WindowFlags DWindow::windowFlags()
{
    D_D(DWindow);
    return d->dwindowFlags;
}

///
/// \brief setWindowFlags
/// \param type
///
void DWindow::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DWindow);
    d->dwindowFlags = type;
    d->titlebar->setWindowFlags(type);
}

void DWindow::setTitleFixedHeight(int h) {
    D_D(DWindow);
    d->titlebar->setFixedHeight(h);
}

void DWindow::setTitle(const QString &t)
{
    D_D(DWindow);
    d->titlebar->setTitle(t);
}

void DWindow::setIcon(const QPixmap &icon)
{
    D_D(DWindow);
    d->titlebar->setIcon(icon);
}

void DWindow::setContentLayout(QLayout *l)
{
    D_D(DWindow);
    d->contentWidget->removeLayout();
    l->setMargin(l->margin()+10);
    d->contentWidget->setLayout(l);
}

void DWindow::setContentWidget(QWidget *w)
{
    D_D(DWindow);
    QHBoxLayout *l = new QHBoxLayout;
    l->setMargin(10);
    l->setSpacing(0);
    l->addWidget(w);
    setContentLayout(l);
}

void DWindow::setShadowHits()
{
    D_D(DWindow);
#ifdef Q_OS_LINUX
    const int maxInterLength = 40;
    QString glowRadiusString = QString("%1").arg(d->m_GlowRadius);
    unsigned char value[maxInterLength];
    memset(value, 0, maxInterLength);
    memcpy(value, glowRadiusString.toStdString().c_str(), glowRadiusString.size());

    Atom shadowAtom;

    shadowAtom = XInternAtom(QX11Info::display(), "DEEPIN_WINDOW_SHADOW", False);
    if (shadowAtom == None) {
        qWarning() << "Failed to create atom with name DEEPIN_WINDOW_SHADOW";
        return;
    }

    XChangeProperty(QX11Info::display(),
                    this->winId(),
                    shadowAtom,
                    XA_STRING,
                    WindowGlowRadius,
                    PropModeReplace,
                    value,
                    glowRadiusString.size());
#endif

    d->m_Shadow = new DGraphicsGlowEffect(this);
    d->m_Shadow->setOffset(0, 0);
    d->m_Shadow->setBlurRadius(d->m_GlowRadius);
    this->setGraphicsEffect(d->m_Shadow);
}

DWindow::~DWindow()
{

}

void DWindow::showEvent(QShowEvent *e)
{
    D_D(DWindow);
    if (!d->m_Shadow) {
        setShadowHits();
    }
    QWidget::showEvent(e);
}

void DWindow::paintEvent(QPaintEvent *)
{
    D_D(DWindow);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect().marginsRemoved(QMargins(d->m_GlowRadius, d->m_GlowRadius, d->m_GlowRadius, d->m_GlowRadius));
    QPoint topLeft(rect.x(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
    QPainterPath border;
    border.addRoundedRect(rect, d->m_Radius, d->m_Radius);

    QLinearGradient linearGradient(topLeft, QPoint(topLeft.x(), bottomRight.y()));
    linearGradient.setColorAt(0.0, BackgroundTopColor);
    linearGradient.setColorAt(0.2, BackgroundBottonColor);
    linearGradient.setColorAt(1.0, BackgroundBottonColor);

    QPen borderPen(BorderColor);
    painter.setBrush(QBrush(linearGradient));
    painter.strokePath(border, borderPen);
    painter.fillPath(border, QBrush(linearGradient));
}

DWIDGET_END_NAMESPACE

// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddrawer.h"
#include "dboxwidget.h"
#include "private/ddrawer_p.h"

#include <QPropertyAnimation>
#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

class ContentBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int height READ height WRITE setFixedHeight)
public:
    explicit ContentBox(QWidget *parent = nullptr)
        : QWidget(parent)
    {
    }
};

DDrawerPrivate::DDrawerPrivate(DDrawer *qq)
    : DFramePrivate(qq)
{

}

DDrawerPrivate::~DDrawerPrivate()
{
}

void DDrawerPrivate::init()
{
    DDrawer *qq = q_func();
    qq->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    QVBoxLayout *mainLayout = new QVBoxLayout(qq);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_headerLayout = new QVBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setAlignment(Qt::AlignCenter);

    m_hSeparator = new DHorizontalLine();
    m_bottom_separator = new DHorizontalLine;
    m_bottom_separator->hide();
    m_hSeparator->setAccessibleName("DDrawerHorizontalSeparator");
    m_bottom_separator->setAccessibleName("DDrawerBottomSeparator");

    qq->connect(qq, &DDrawer::expandChange, m_bottom_separator, &QWidget::setVisible);

    m_contentLoader = new ContentBox();
    m_contentLoader->setFixedHeight(0); // default to not expanded.
    m_contentLoader->setAccessibleName("DDrawerContentBox");

    m_boxWidget = new DVBoxWidget;
    m_boxWidget->setAccessibleName("DDrawerVBoxWidget");
    m_contentLayout = m_boxWidget->layout();

    QVBoxLayout *layout_contentLoader = new QVBoxLayout(m_contentLoader);

    layout_contentLoader->setContentsMargins(0, 0, 0, 0);
    layout_contentLoader->setSpacing(0);
    layout_contentLoader->addWidget(m_boxWidget);
    layout_contentLoader->addStretch();

    m_animation = new QPropertyAnimation(m_contentLoader, "height", qq);
    m_animation->setDuration(400);
    m_animation->setEasingCurve(QEasingCurve::InQuad);
    qq->connect(m_animation, &QPropertyAnimation::valueChanged, qq, [this, qq] {
        if (!m_enableAnimation)
            return;
        qq->setFixedHeight(qq->sizeHint().height());
    });

    mainLayout->addLayout(m_headerLayout);
    mainLayout->addWidget(m_hSeparator);
    mainLayout->addWidget(m_contentLoader);
    mainLayout->addWidget(m_bottom_separator);

    qq->setLayout(mainLayout);

    qq->connect(m_boxWidget, &DBoxWidget::sizeChanged, qq, [this] {
        if (!m_enableAnimation) {
            updateHeightDirect();
            return;
        }
        if (m_expand) {
            int endHeight = 0;
            endHeight = m_boxWidget->height();

            m_animation->setStartValue(m_contentLoader->height());
            m_animation->setEndValue(endHeight);
            m_animation->stop();
            m_animation->start();
        }
    });
}

void DDrawerPrivate::updateHeightDirect()
{
    D_Q(DDrawer);
    m_contentLoader->setFixedHeight(m_expand ? m_boxWidget->height() : 0);
    q->setFixedHeight(q->sizeHint().height());
}

/*!
  \class Dtk::Widget::DDrawer
  \inmodule dtkwidget
  \brief A beautifully developed control.
  
  using DDrawer Class can create a controllable control with an animation effect. This control contains the upper and lower parts. The above control is the title control. This control will always show.:: SETEXPAND to set the visibility of the content control.Use DDRAWER :: SetHeader and DDRAWER :: SetContent to set the title control and content control respectively.
  \sa DHeaderLine
 */

/*!
  \fn void DDrawer::expandChange(bool e)
  \brief Content control visibility signal
  \a e For TRUE, the content control becomes visible, but vice versa
 */

/*!
  \brief Get DDRAWER :: DDRAWER example
  \a parent As the parent control part of the example
 */
DDrawer::DDrawer(QWidget *parent)
    : DDrawer(*new DDrawerPrivate(this), parent)
{

}

DDrawer::~DDrawer()
{

}

/*!
  \brief Set the title control
  The title control will always be displayed in the layout
  \a header Title control
 */
void DDrawer::setHeader(QWidget *header)
{
    Q_D(DDrawer);
    if (!header) {
        return;
    }

    QLayoutItem *child;
    while ((child = d->m_headerLayout->takeAt(0)) != nullptr) {
        delete child;
    }

    d->m_headerLayout->addWidget(header);
    d->m_header = header;
}

/*!
  \brief Set the content control
  Content control is hidden by default, call ddrawer :: setexpand settings for its visibility
  \a content Content controlent control
  \a alignment The alignment method of content control in the layout method of content control in the layout method of content control in the layout method of content control in the layout method of content control in the layout method of content control in the layout method of content control in the layout
 */
void DDrawer::setContent(QWidget *content, Qt::Alignment alignment)
{
    Q_D(DDrawer);

    if (!content) {
        return;
    }

    QLayoutItem *child;
    while ((child = d->m_contentLayout->takeAt(0)) != nullptr) {
        delete child;
    }

    d->m_contentLayout->addWidget(content, 1, alignment);
    d->m_contentLayout->addStretch(1);
    d->m_content = content;
}

/*!
  \brief Get the content control object
  \return Content control object
 */
QWidget *DDrawer::getContent() const
{
    Q_D(const DDrawer);

    return d->m_content;
}

/*!
  \brief Set the height of the title control.

  \a height Specified height
 */
void DDrawer::setHeaderHeight(int height)
{
    Q_D(DDrawer);

    if (d->m_header) {
        d->m_header->setFixedHeight(height);
    }
}

/*!
  \brief Set the visibility of the content control.

  \a value For TRUE, the content control can be seen, but vice versant control can be seen, but vice versa
 */
void DDrawer::setExpand(bool value)
{
    Q_D(DDrawer);

    if (d->m_expand == value) {
        return;
    }

    d->m_expand = value;
    Q_EMIT expandChange(value);

    // Disable animation to update height when widget is invisible by default.
    static const bool disableAnimation(qEnvironmentVariableIsSet("D_DTK_DISABLE_ANIMATIONS"));
    d->m_enableAnimation = !disableAnimation && isVisible();

    if (!d->m_enableAnimation) {
        d->updateHeightDirect();
        return;
    }

    if (value) {
        d->m_animation->setStartValue(0);
        d->m_animation->setEndValue(d->m_boxWidget->height());
    } else {
        d->m_animation->setStartValue(d->m_boxWidget->height());
        d->m_animation->setEndValue(0);
    }

    d->m_animation->stop();
    d->m_animation->start();
}

/*!
  \brief Visible to obtain the current content control.

  \return Visible of the current content control
 */
bool DDrawer::expand() const
{
    Q_D(const DDrawer);
    return d->m_expand;
}

/*!
  \brief Set the time for the visibility of the content control when the animation time is changed.

  \a duration Specify the animation time
 */
void DDrawer::setAnimationDuration(int duration)
{
    Q_D(DDrawer);
    d->m_animation->setDuration(duration);
}

/*!
  \brief setTheVisibilityOfTheContentControlWhenTheAnimationStyleIsChangedOfTheContentControlWhenTheAnimationStyleIsChanged
  \a curve Specify animation style
 */
void DDrawer::setAnimationEasingCurve(QEasingCurve curve)
{
    Q_D(DDrawer);
    d->m_animation->setEasingCurve(curve);
}

/*!
  \brief Set the segmentation line between the title control and the content control.

  \a arg For ture, the segmentation line is displayed, but the instead does not display
 */
void DDrawer::setSeparatorVisible(bool arg)
{
    Q_D(DDrawer);
    d->m_hSeparator->setVisible(arg);
}

/*!
  \brief Set the segmentation line under the content control.

  \a arg For ture, the segmentation line is displayed, but the instead does not display
 */
void DDrawer::setExpandedSeparatorVisible(bool arg)
{
    Q_D(DDrawer);

    if (arg) {
        connect(this, &DDrawer::expandChange, d->m_bottom_separator, &QWidget::setVisible);
        d->m_bottom_separator->show();
    } else {
        disconnect(this, &DDrawer::expandChange, d->m_bottom_separator, &QWidget::setVisible);
        d->m_bottom_separator->hide();
    }
}

DDrawer::DDrawer(DDrawerPrivate &dd, QWidget *parent)
    : DFrame(dd, parent)
{
    dd.init();
}

void DDrawer::resizeEvent(QResizeEvent *e)
{
    Q_D(DDrawer);

    if (d->m_content) {
        d->m_content->setFixedWidth(e->size().width());
    }
    DFrame::resizeEvent(e);
}

DWIDGET_END_NAMESPACE

#include "ddrawer.moc"

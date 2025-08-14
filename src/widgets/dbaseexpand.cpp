// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dbaseexpand.h"
#include "dthememanager.h"
#include "dboxwidget.h"

#include <QResizeEvent>
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
DWIDGET_BEGIN_NAMESPACE

class DBaseExpandPrivate
{
public:
    DBaseExpandPrivate(DBaseExpand *qq);
    ~DBaseExpandPrivate();

    QWidget *m_header = nullptr;
    QWidget *m_content = nullptr;
    DBoxWidget *m_boxWidget = nullptr;
    QVBoxLayout *m_headerLayout = nullptr;
    QBoxLayout *m_contentLayout = nullptr;
    ContentLoader *m_contentLoader = nullptr;
    DHorizontalLine *m_hSeparator = nullptr;
    DHorizontalLine *m_bottom_separator = nullptr;
    QPropertyAnimation *m_animation = nullptr;
    bool m_expand = false;
    bool m_reservedPadding[7];

    DBaseExpand *q_ptr;
};

DBaseExpandPrivate::DBaseExpandPrivate(DBaseExpand *qq)
    : q_ptr(qq)
{
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
    m_hSeparator->setAccessibleName("DBaseExpandHorizontalSeparator");
    m_bottom_separator->setAccessibleName("DBaseExpandBottomSeparator");

    qq->connect(qq, &DBaseExpand::expandChange, m_bottom_separator, &QWidget::setVisible);

    m_contentLoader = new ContentLoader();
    m_contentLoader->setFixedHeight(0); // default to not expanded.
    m_contentLoader->setAccessibleName("DBaseExpandContentLoader");

    m_boxWidget = new DVBoxWidget;
    m_contentLayout = m_boxWidget->layout();
    m_boxWidget->setAccessibleName("DBaseExpandVBoxWidget");

    QVBoxLayout *layout_contentLoader = new QVBoxLayout(m_contentLoader);

    layout_contentLoader->setContentsMargins(0, 0, 0, 0);
    layout_contentLoader->setSpacing(0);
    layout_contentLoader->addWidget(m_boxWidget);
    layout_contentLoader->addStretch();

    m_animation = new QPropertyAnimation(m_contentLoader, "height", qq);
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::InSine);
    qq->connect(m_animation, &QPropertyAnimation::valueChanged, qq, [qq] {
        qq->setFixedHeight(qq->sizeHint().height());
    });

    mainLayout->addLayout(m_headerLayout);
    mainLayout->addWidget(m_hSeparator);
    mainLayout->addWidget(m_contentLoader);
    mainLayout->addWidget(m_bottom_separator);

    qq->setLayout(mainLayout);

    qq->connect(m_boxWidget, &DBoxWidget::sizeChanged, qq, [this] {
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

DBaseExpandPrivate::~DBaseExpandPrivate()
{
}

/*!
@~english
  @class Dtk::Widget::DBaseExpand
  @brief A beautiful expandable widget.
  
  Using the DBaseExpand class, you can create an expandable widget with animation effects. This widget contains the upper and lower parts. The upper widget is a title widget，which will always be displayed. The lower widget is a content widget, which defaults to not being displayed.
  Call DBaseExpand::setExpand to set the visibility of the content widget. Use DBaseExpand::setHeader and DBaseExpand::setContent to set the title widget and content widget respectively.

  @sa DHeaderLine
  
  @image html DBaseExpand.gif
 */

/*!
@~english
  @fn void DBaseExpand::expandChange(bool e)
  @brief content widget visibility signal
  @param[in] e If "true", the content widget becomes visible, otherwise, the content widget becomes invisible.
*/

/*!
@~english
  @brief get DBaseExpand::DBaseExpand instance
  @param[in] parent as the parent widget part of the example
 */
DBaseExpand::DBaseExpand(QWidget *parent)
    : QWidget(parent)
    , d_private(new DBaseExpandPrivate(this))
{

}

DBaseExpand::~DBaseExpand()
{

}

/*!
@~english
  @brief set the title widget
  The title widget will always be displayed in the layout.
  @param[in] header title widget
 */
void DBaseExpand::setHeader(QWidget *header)
{
    Q_D(DBaseExpand);
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
@~english
  @brief set the content widget
  Content widget is hidden by default, call dbaseexpand::setexpand to set its visibility.
  @param[in] content content widget
  @param[in] alignment content widget's alignment method in layout
 */
void DBaseExpand::setContent(QWidget *content, Qt::Alignment alignment)
{
    Q_D(DBaseExpand);

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
@~english
  @brief get the content widget object
  @return content widget object
 */

QWidget *DBaseExpand::getContent() const
{
    Q_D(const DBaseExpand);

    return d->m_content;
}

/*!
@~english
  @brief set the height of the title widget
  
  @param[in] height specified height
 */
void DBaseExpand::setHeaderHeight(int height)
{
    Q_D(DBaseExpand);

    if (d->m_header) {
        d->m_header->setFixedHeight(height);
    }
}

/*!
@~english
  @brief set the visibility of the content widget
  
  @param[in] value If "true", the content widget is visible, otherwise, the content widget is not visible.
 */
void DBaseExpand::setExpand(bool value)
{
    Q_D(DBaseExpand);

    if (d->m_expand == value) {
        return;
    }

    d->m_expand = value;
    Q_EMIT expandChange(value);

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
@~english
  @brief get the visiblity of the current content widget
  
  @return visiblity of the current content widget
 */
bool DBaseExpand::expand() const
{
    Q_D(const DBaseExpand);
    return d->m_expand;
}

/*!
@~english
  @brief set the duration for the visibility of the content widget
  
  @param[in] duration specified animation duration
 */
void DBaseExpand::setAnimationDuration(int duration)
{
    Q_D(DBaseExpand);
    d->m_animation->setDuration(duration);
}

/*!
@~english
  @brief set the visibility of the content widget when the animation style is changed
  
  @param[in] curve specified animation style
 */
void DBaseExpand::setAnimationEasingCurve(QEasingCurve curve)
{
    Q_D(DBaseExpand);
    d->m_animation->setEasingCurve(curve);
}

/*!
@~english
  @brief set whether to allow the segmentation line between the title widget and the content widget
  
  @param[in] arg If "true", the segmentation line is visible, otherwise, the segmentation line is not visible.
 */
void DBaseExpand::setSeparatorVisible(bool arg)
{
    Q_D(DBaseExpand);
    d->m_hSeparator->setVisible(arg);
}

/*!
@~english
  @brief set whether to allow the segmentation line under the content widget
  
  @param[in] arg If "true", the segmentation line is visible, otherwise, the segmentation line is not visible.
 */
void DBaseExpand::setExpandedSeparatorVisible(bool arg)
{
    Q_D(DBaseExpand);

    if (arg) {
        connect(this, &DBaseExpand::expandChange, d->m_bottom_separator, &QWidget::setVisible);
        d->m_bottom_separator->show();
    } else {
        disconnect(this, &DBaseExpand::expandChange, d->m_bottom_separator, &QWidget::setVisible);
        d->m_bottom_separator->hide();
    }
}

void DBaseExpand::resizeEvent(QResizeEvent *e)
{
    Q_D(DBaseExpand);

    if (d->m_content) {
        d->m_content->setFixedWidth(e->size().width());
    }
    QWidget::resizeEvent(e);
}

DWIDGET_END_NAMESPACE
#endif

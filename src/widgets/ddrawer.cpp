/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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

    layout_contentLoader->setMargin(0);
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

/*!
  \class Dtk::Widget::DDrawer
  \inmodule dtkwidget
  \brief 一个美观的可展开的控件.
  
  使用 DDrawer 类可以创建一个可展开的带有展开动画效果的控件，这个控件包含上下两部分，上面的控件为标题控件，这个控件会始终显示，下面的控件为内容控件，默认为不会显示，调用 DDrawer::setExpand 设置内容控件的可见性。使用 DDrawer::setHeader 和 DDrawer::setContent 设置分别设置标题控件和内容控件。
  
  \sa DHeaderLine
 */

/*!
  \fn void DDrawer::expandChange(bool e)
  \brief 内容控件可见性发生改变的信号
  \a e 为 true 表示内容控件变为了可见，反之则反
 */

/*!
  \brief 获取 DDrawer::DDrawer 实例
  \a parent 作为实例的父控件
 */
DDrawer::DDrawer(QWidget *parent)
    : DDrawer(*new DDrawerPrivate(this), parent)
{

}

DDrawer::~DDrawer()
{

}

/*!
  \brief 设置标题控件
  标题控件会始终显示在布局里
  \a header 标题控件
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
  \brief 设置内容控件
  内容控件默认是隐藏的，调用 DDrawer::setExpand 设置其可见性
  \a content 内容控件
  \a alignment 内容控件在布局中的对齐方式
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
  \brief 获取内容控件对象
  \return 内容控件对象
 */
QWidget *DDrawer::getContent() const
{
    Q_D(const DDrawer);

    return d->m_content;
}

/*!
  \brief 设置标题控件的高度.

  \a height 指定的高度
 */
void DDrawer::setHeaderHeight(int height)
{
    Q_D(DDrawer);

    if (d->m_header) {
        d->m_header->setFixedHeight(height);
    }
}

/*!
  \brief 设置内容控件的可见性.

  \a value 为 true 则内容控件可见，反之则反
 */
void DDrawer::setExpand(bool value)
{
    Q_D(DDrawer);

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
  \brief 获取当前内容控件的可见性.

  \return 当前内容控件的可见性
 */
bool DDrawer::expand() const
{
    Q_D(const DDrawer);
    return d->m_expand;
}

/*!
  \brief 设置内容控件的可见性改变时动画的时间.

  \a duration 指定动画时间
 */
void DDrawer::setAnimationDuration(int duration)
{
    Q_D(DDrawer);
    d->m_animation->setDuration(duration);
}

/*!
  \brief 设置内容控件的可见性改变时动画的样式.
  \a curve 指定动画样式
 */
void DDrawer::setAnimationEasingCurve(QEasingCurve curve)
{
    Q_D(DDrawer);
    d->m_animation->setEasingCurve(curve);
}

/*!
  \brief 设置是否允许标题控件与内容控件之间的分割线.

  \a arg 为 ture 则显示分割线，反之则反
 */
void DDrawer::setSeparatorVisible(bool arg)
{
    Q_D(DDrawer);
    d->m_hSeparator->setVisible(arg);
}

/*!
  \brief 设置是否允许内容控件下的分割线.

  \a arg 为 ture 则显示分割线，反之则反
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

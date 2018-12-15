/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dbaseexpand.h"
#include "dthememanager.h"
#include "dboxwidget.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DBaseExpand
 * \~chinese \brief 一个美观的可展开的控件
 *
 * \~chinese 使用 DBaseExpand 类可以创建一个可展开的带有展开动画效果的控件，这个控件包含上下两部分，上面的控件为标题控件，这个控件会始终显示，下面的控件为内容控件，默认为不会显示，调用 DBaseExpand::setExpand 设置内容控件的可见性。使用 DBaseExpand::setHeader 和 DBaseExpand::setContent 设置分别设置标题控件和内容控件。
 *
 * \~chinese \sa DHeaderLine
 *
 * \~chinese \image html DBaseExpand.gif
 */

/**
 * \~chinese \fn DBaseExpand::expandChange
 * \~chinese \brief 内容控件可见性发生改变的信号
 * \~chinese \param e 为 true 表示内容控件变为了可见，反之则反
 */

/*!
 * \~chinese \brief 获取 DBaseExpand::DBaseExpand 实例
 * \~chinese \param parent 作为实例的父控件
 */
DBaseExpand::DBaseExpand(QWidget *parent) : QWidget(parent)
{
    DThemeManager::registerWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_headerLayout = new QVBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_headerLayout->setAlignment(Qt::AlignCenter);

    m_hSeparator = new DSeparatorHorizontal();
    m_bottom_separator = new DSeparatorHorizontal;
    m_bottom_separator->hide();

    connect(this, &DBaseExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);

    m_contentLoader = new ContentLoader();
    m_contentLoader->setFixedHeight(0); // default to not expanded.

    m_boxWidget = new DVBoxWidget;
    m_contentLayout = m_boxWidget->layout();

    QVBoxLayout *layout_contentLoader = new QVBoxLayout(m_contentLoader);

    layout_contentLoader->setMargin(0);
    layout_contentLoader->setSpacing(0);
    layout_contentLoader->addWidget(m_boxWidget);
    layout_contentLoader->addStretch();

    m_animation = new QPropertyAnimation(m_contentLoader, "height");
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::InSine);
    connect(m_animation, &QPropertyAnimation::valueChanged, this, [this] {
        setFixedHeight(sizeHint().height());
    });

    mainLayout->addLayout(m_headerLayout);
    mainLayout->addWidget(m_hSeparator);
    mainLayout->addWidget(m_contentLoader);
    mainLayout->addWidget(m_bottom_separator);

    setLayout(mainLayout);

    connect(m_boxWidget, &DBoxWidget::sizeChanged, this, [this] {
        if (m_expand)
        {
            int endHeight = 0;
            endHeight = m_boxWidget->height();

            m_animation->setStartValue(m_contentLoader->height());
            m_animation->setEndValue(endHeight);
            m_animation->stop();
            m_animation->start();
        }
    });
}

DBaseExpand::~DBaseExpand()
{
    if (m_headerLayout) {
        m_headerLayout->deleteLater();
    }
    if (m_contentLayout) {
        m_contentLayout->deleteLater();
    }
    if (m_contentLoader) {
        m_contentLoader->deleteLater();
    }
    if (m_animation) {
        m_animation->deleteLater();
    }
}

/**
 * \~chinese \brief 设置标题控件
 * \~chinese 标题控件会始终显示在布局里
 * \~chinese \param header 标题控件
 */
void DBaseExpand::setHeader(QWidget *header)
{
    if (!header) {
        return;
    }

    QLayoutItem *child;
    while ((child = m_headerLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_headerLayout->addWidget(header);
    m_header = header;
}

/**
 * \~chinese \property DBaseExpand::getContent
 * \~chinese \brief 获取内容控件对象
 * \~chinese \return 内容控件对象
 */

/**
 * \~chinese \brief 设置内容控件
 * \~chinese 内容控件默认是隐藏的，调用 DBaseExpand::setExpand 设置其可见性
 * \~chinese \param content 内容控件
 * \~chinese \param alignment 内容控件在布局中的对齐方式
 */
void DBaseExpand::setContent(QWidget *content, Qt::Alignment alignment)
{
    if (!content) {
        return;
    }

    QLayoutItem *child;
    while ((child = m_contentLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_contentLayout->addWidget(content, 1, alignment);
    m_contentLayout->addStretch(1);
    m_content = content;
}

/**
 * \~chinese \brief 设置标题控件的高度
 * \~chinese
 * \~chinese \param height 指定的高度
 */
void DBaseExpand::setHeaderHeight(int height)
{
    if (m_header) {
        m_header->setFixedHeight(height);
    }
}

/**
 * \~chinese \brief 设置内容控件的可见性
 * \~chinese
 * \~chinese \param value 为 true 则内容控件可见，反之则反
 */
void DBaseExpand::setExpand(bool value)
{
    if (m_expand == value) {
        return;
    }

    m_expand = value;
    Q_EMIT expandChange(value);

    if (value) {
        m_animation->setStartValue(0);
        m_animation->setEndValue(m_boxWidget->height());
    } else {
        m_animation->setStartValue(m_boxWidget->height());
        m_animation->setEndValue(0);
    }

    m_animation->stop();
    m_animation->start();
}

/**
 * \~chinese \brief 获取当前内容控件的可见性
 * \~chinese
 * \~chinese \return 当前内容控件的可见性
 */
bool DBaseExpand::expand() const
{
    return m_expand;
}

/**
 * \~chinese \brief 设置内容控件的可见性改变时动画的时间
 * \~chinese
 * \~chinese \param duration 指定动画时间
 */
void DBaseExpand::setAnimationDuration(int duration)
{
    m_animation->setDuration(duration);
}

/**
 * \~chinese \brief 设置内容控件的可见性改变时动画的样式
 * \~chinese
 * \~chinese \param curve 指定动画样式
 */
void DBaseExpand::setAnimationEasingCurve(QEasingCurve curve)
{
    m_animation->setEasingCurve(curve);
}

/**
 * \~chinese \brief 设置是否允许标题控件与内容控件之间的分割线
 * \~chinese
 * \~chinese \param arg 为 ture 则显示分割线，反之则反
 */
void DBaseExpand::setSeparatorVisible(bool arg)
{
    m_hSeparator->setVisible(arg);
}

/**
 * \~chinese \brief 设置是否允许内容控件下的分割线
 * \~chinese
 * \~chinese \param arg 为 ture 则显示分割线，反之则反
 */
void DBaseExpand::setExpandedSeparatorVisible(bool arg)
{
    if (arg) {
        connect(this, &DBaseExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);
        m_bottom_separator->show();
    } else {
        disconnect(this, &DBaseExpand::expandChange, m_bottom_separator, &DSeparatorHorizontal::setVisible);
        m_bottom_separator->hide();
    }
}

void DBaseExpand::resizeEvent(QResizeEvent *e)
{
    if (m_content) {
        m_content->setFixedWidth(e->size().width());
    }
    QWidget::resizeEvent(e);
}


DWIDGET_END_NAMESPACE

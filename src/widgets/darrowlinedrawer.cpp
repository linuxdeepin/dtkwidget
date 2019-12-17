/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "darrowlinedrawer.h"
#include "dheaderline.h"
#include "diconbutton.h"
#include "private/ddrawer_p.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

namespace HeaderLine {
    class ArrowHeaderLine : public DHeaderLine
    {
        Q_OBJECT
    public:
        ArrowHeaderLine(QWidget *parent = nullptr);
        void setExpand(bool value);

    Q_SIGNALS:
        void mousePress();

    protected:
        void mousePressEvent(QMouseEvent *);

    private:
        void reverseArrowDirection();
        bool m_isExpanded = false;
        DIconButton *m_arrowButton = nullptr;
    };

    ArrowHeaderLine::ArrowHeaderLine(QWidget *parent) :
        DHeaderLine(parent)
    {
        m_arrowButton = new DIconButton(DStyle::SP_ReduceElement, this);
        m_arrowButton->setFlat(true);
        setExpand(false);
        connect(m_arrowButton, &DIconButton::clicked, this, &ArrowHeaderLine::mousePress);
        setContent(m_arrowButton);
        setFixedHeight(EXPAND_HEADER_HEIGHT);
    }

    void ArrowHeaderLine::setExpand(bool value)
    {
        if (value) {
            m_arrowButton->setIcon(DStyle::SP_ExpandElement);
        } else {
            m_arrowButton->setIcon(DStyle::SP_ReduceElement);
        }
        m_isExpanded = value;
    }

    void ArrowHeaderLine::mousePressEvent(QMouseEvent *)
    {
        Q_EMIT mousePress();
    }

    void ArrowHeaderLine::reverseArrowDirection()
    {
        setExpand(!m_isExpanded);
    }
}
using namespace HeaderLine;

class DArrowLineDrawerPrivate : public DDrawerPrivate
{
public:
    D_DECLARE_PUBLIC(DArrowLineDrawer)
    explicit DArrowLineDrawerPrivate(DDrawer *qq)
        : DDrawerPrivate(qq) {

    }

    ArrowHeaderLine *headerLine = nullptr;
};

/**
 * \~chinese \class DArrowLineDrawer
 * \~chinese \brief 一个美观的可展开的控件
 * \~chinese
 * \~chinese DArrowLineDrawer 继承自 DDrawer 并提供了了 ArrowHeaderLine (一个带有箭头标示的按钮)作为其固定的标题控件，也就是说跟 DDrawer 相比省去了提供标题控件的步骤，只需要提供内容控件即可，如果需要自定义标题控件应该使用 DDrawer 类。
 * \~chinese \sa  DDrawer
 */

/**
 * \~chinese \brief 构造一个 DArrowLineDrawer 实例
 * \~chinese
 * \~chinese \param parent 为实例的父控件
 */
DArrowLineDrawer::DArrowLineDrawer(QWidget *parent)
    : DDrawer(*new DArrowLineDrawerPrivate(this), parent)
{
    D_D(DArrowLineDrawer);
    d->headerLine = new ArrowHeaderLine(this);
    d->headerLine->setExpand(expand());
    connect(d->headerLine, &ArrowHeaderLine::mousePress, [=]{
        setExpand(!expand());
    });
    setHeader(d->headerLine);
}

/**
 * \~chinese \brief 设置标题要显示的文字
 * \~chinese
 * \~chinese \param title 标题内容
 */
void DArrowLineDrawer::setTitle(const QString &title)
{
    D_D(DArrowLineDrawer);
    d->headerLine->setTitle(title);
}

/**
 * \~chinese \brief 设置是否展开以显示内容控件
 * \~chinese
 * \~chinese \param value 为 true 即为显示，反之则反
 */
void DArrowLineDrawer::setExpand(bool value)
{
    D_D(DArrowLineDrawer);
    //Header's arrow direction change here
    d->headerLine->setExpand(value);
    DDrawer::setExpand(value);
}

/**
 * \~chinese \brief 获取标题控件
 * \~chinese \return 标题控件
 * \~chinese \sa ArrowHeaderLine DHeaderLine DBaseLine
 */
DBaseLine *DArrowLineDrawer::headerLine()
{
    D_D(DArrowLineDrawer);
    return d->headerLine;
}

void DArrowLineDrawer::setHeader(QWidget *header)
{
    DDrawer::setHeader(header);
}

void DArrowLineDrawer::resizeEvent(QResizeEvent *e)
{
    D_D(DArrowLineDrawer);
    d->headerLine->setFixedWidth(e->size().width());

    DDrawer::resizeEvent(e);
}

DWIDGET_END_NAMESPACE

#include "darrowlinedrawer.moc"

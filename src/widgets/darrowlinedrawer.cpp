// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
        void mousePressEvent(QMouseEvent *) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void changeEvent(QEvent *e) override;

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
        m_arrowButton->setAccessibleName("ArrowHeaderArrowButton");
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

    void ArrowHeaderLine::mouseMoveEvent(QMouseEvent *event)
    {
        //屏蔽掉鼠标移动事件
        event->accept();
    }

    void ArrowHeaderLine::changeEvent(QEvent *e)
    {
        if (e->type() == QEvent::FontChange)
            setFixedHeight(qMax(EXPAND_HEADER_HEIGHT, this->fontMetrics().height()));

        return DHeaderLine::changeEvent(e);
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

/*!
@~english
  \class Dtk::Widget::DArrowLineDrawer
  \inmodule dtkwidget
  @brief A beautiful expandable control.
  
  DArrowLineDrawer inherited from DDrawer and provide ArrowHeaderLine (a button marked with an arrow)as its fixed title control，that is to say, compared with DDrawer, the step of providing title control is omitted，you only need to provide content controls，if you need a custom title control, you should use the DDrawer class.
  \sa  DDrawer
 */

/*!
@~english
  @brief Construct a DArrowLineDrawer example
  
  \a parent is the parent control of the instance
 */
DArrowLineDrawer::DArrowLineDrawer(QWidget *parent)
    : DDrawer(*new DArrowLineDrawerPrivate(this), parent)
{
    D_D(DArrowLineDrawer);
    d->headerLine = new ArrowHeaderLine(this);
    d->headerLine->setExpand(expand());
    d->headerLine->setAccessibleName("DArrowLineDrawerHeaderLine");
    connect(d->headerLine, &ArrowHeaderLine::mousePress, [=]{
        setExpand(!expand());
    });
    setHeader(d->headerLine);
}

/*!
@~english
  @brief Set the text for the title to display
  
  \a title Title content
 */
void DArrowLineDrawer::setTitle(const QString &title)
{
    D_D(DArrowLineDrawer);
    d->headerLine->setTitle(title);
}

/*!
@~english
  @brief Sets whether to expand to display the content control
  
  \a value If it is true, it will be displayed, and vice versa.
 */
void DArrowLineDrawer::setExpand(bool value)
{
    D_D(DArrowLineDrawer);
    //Header's arrow direction change here
    d->headerLine->setExpand(value);
    DDrawer::setExpand(value);
}

/*!
@~english
  @brief Get the title control
  \return Title control
  \sa DHeaderLine DBaseLine
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
DBaseLine *DArrowLineDrawer::headerLine()
{
    D_D(DArrowLineDrawer);
    return d->headerLine;
}
#endif

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

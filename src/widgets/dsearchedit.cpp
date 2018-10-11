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

#include "dsearchedit.h"
#include "dthememanager.h"

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPropertyAnimation>
#include <QDebug>
#include <QEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QTimer>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DSearchEdit
 * \~chinese \brief DSearchEdit 类提供了专门用来进行搜索的输入框控件。
 * \~chinese
 * \~chinese 相比于一般的输入框控件，DSearchEdit 提供了搜索按钮指示，用户使用起来会更加自然。
 * \~chinese 如图示：
 * \~chinese \image html searchedit.png
 * \~chinese
 * \~chinese \warning DSearchEdit 与 QLineEdit、DLineEdit 没有继承关系，功能不兼容。
 */


/*!
 * \~chinese \fn DSearchEdit::textChanged
 * \~chinese \brief 信号在用户对输入框内容做了修改后触发。
 */

/*!
 * \~chinese \fn DSearchEdit::returnPressed
 * \~chinese \brief 信号在用户输入完成敲击回车时触发。
 */

/*!
 * \~chinese \fn DSearchEdit::editingFinished
 * \~chinese \brief 信号在用户敲击Enter或者回车键，以及输入框失去焦点是触发。
 * \~chinese \see QLineEdit::editingFinished。
 */

/*!
 * \~chinese \fn DSearchEdit::focusOut
 * \~chinese \brief 信号在输入框失去焦点时触发。
 */

/*!
 * \~chinese \fn DSearchEdit::focusIn
 * \~chinese \brief 信号在输入框获得焦点时触发。
 */


/*!
 * \~chinese \fn DSearchEdit::setText
 * \~chinese \brief 设置输入框内容。
 * \~chinese \param text 为要设置的内容。
 */


/*!
 * \~chinese \fn DSearchEdit::clear
 * \~chinese \brief 清空输入框内容。
 */


/*!
 * \~chinese \fn DSearchEdit::setPlaceHolder
 * \~chinese \brief 为输入框设置占位内容，用来提示用户，如显示“搜索快捷键”。
 * \~chinese \param text 为占位内容。
 *
 */


/*!
 * \~chinese \brief DSearchEdit::DSearchEdit 是 DSearchEdit 类的构造函数。
 * \~chinese \param parent 指定了控件的父控件。
 */
DSearchEdit::DSearchEdit(QWidget *parent)
    : QFrame(parent)
{
    DThemeManager::registerWidget(this);

    initInsideFrame();

    m_searchBtn = new QLabel;
    m_searchBtn->setObjectName("SearchIcon");
    m_searchBtn->setFixedSize(16, 16);
    m_clearBtn = new DImageButton;
    m_clearBtn->setObjectName("ClearIcon");
    m_clearBtn->hide();
    m_edt = new QLineEdit;
    m_edt->setObjectName("Edit");
    m_placeHolder = new QLabel;
    m_placeHolder->setObjectName("PlaceHolder");

    m_animation = new QPropertyAnimation(m_edt, "minimumWidth");

    m_size = QSize(m_searchBtn->sizeHint().width() + m_edt->sizeHint().width() + m_clearBtn->sizeHint().width() + 6,
                   qMax(m_searchBtn->sizeHint().height(), m_edt->sizeHint().height()));
    m_edt->setFixedWidth(0);
    m_edt->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout(m_insideFrame);
    layout->addStretch();
    layout->addWidget(m_searchBtn);
    layout->setAlignment(m_searchBtn, Qt::AlignCenter);
    layout->addWidget(m_placeHolder);
    layout->setAlignment(m_placeHolder, Qt::AlignCenter);
    layout->addWidget(m_edt);
    layout->setAlignment(m_edt, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(m_clearBtn);
    layout->setAlignment(m_clearBtn, Qt::AlignCenter);
    layout->setSpacing(0);
    layout->setContentsMargins(3, 0, 3, 0);

    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(m_clearBtn, &DImageButton::clicked, m_edt, static_cast<void (QLineEdit::*)()>(&QLineEdit::setFocus));
    connect(m_clearBtn, &DImageButton::clicked, this, &DSearchEdit::clear);
    connect(m_edt, &QLineEdit::textChanged, [this] {m_clearBtn->setVisible(!m_edt->text().isEmpty());});
    connect(m_edt, &QLineEdit::textChanged, this, &DSearchEdit::textChanged, Qt::DirectConnection);
    connect(m_edt, &QLineEdit::editingFinished, this, &DSearchEdit::editingFinished, Qt::DirectConnection);
    connect(m_edt, &QLineEdit::returnPressed, this, &DSearchEdit::returnPressed, Qt::DirectConnection);
//    connect(m_searchBtn, &DImageButton::clicked, this, &DSearchEdit::toEditMode);
}

DSearchEdit::~DSearchEdit()
{
    m_animation->deleteLater();
}

/*!
 * \~chinese \brief DSearchEdit::text 用于获取输入框中的内容。
 * \~chinese \return 输入框中用户输入的内容。
 */
const QString DSearchEdit::text() const
{
    return m_edt->text();
}

void DSearchEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        return QFrame::mousePressEvent(e);

    toEditMode();

    e->accept();
}

void DSearchEdit::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
}

bool DSearchEdit::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_edt && e->type() == QEvent::FocusOut && m_edt->text().isEmpty()) {
        auto fe = dynamic_cast<QFocusEvent *>(e);
        if (fe && fe->reason() != Qt::PopupFocusReason) {
            m_animation->stop();
            m_animation->setStartValue(m_edt->width());
            m_animation->setEndValue(0);
            m_animation->setEasingCurve(m_hideCurve);
            m_animation->start();

            connect(m_animation, &QPropertyAnimation::finished, m_placeHolder, &QLabel::show);
        }
    }

    if (o == m_edt) {
        if (e->type() == QEvent::FocusOut) {
            Q_EMIT focusOut();
        }

        if (e->type() == QEvent::FocusIn) {
            Q_EMIT focusIn();
        }
    }

    return QFrame::eventFilter(o, e);
}

/*!
 * \~chinese \brief DSearchEdit::getLineEdit 用于获取控件中封装的 QLineEdit 实例，
 * \~chinese 使用者可以通过 QLineEdit 提供的函数对输入框进行进一步细节设置，如果输入规则等。
 * \~chinese \return 控件中封装的 QLineEdit 实例
 */
QLineEdit *DSearchEdit::getLineEdit() const
{
    return m_edt;
}

void DSearchEdit::toEditMode()
{
    // already in edit mode
    if (!m_placeHolder->isVisible()) {
        return;
    }

    disconnect(m_animation, &QPropertyAnimation::finished, m_placeHolder, &QLabel::show);

    m_animation->stop();
    m_animation->setStartValue(0);
    m_animation->setEndValue(m_size.width() - m_searchBtn->width() - 6); // left + right margins = 6
    m_animation->setEasingCurve(m_showCurve);
    m_animation->start();

    m_placeHolder->hide();
    m_edt->setFocus();
}

//Bypassing the problem here
//qss can't draw box-shadow
void DSearchEdit::initInsideFrame()
{
    m_insideFrame = new QFrame(this);
    m_insideFrame->raise();
    m_insideFrame->setObjectName("DEditInsideFrame");
    QHBoxLayout *insideLayout = new QHBoxLayout(this);
    insideLayout->setContentsMargins(0, 0, 0, 1);
    insideLayout->setSpacing(0);
    insideLayout->addWidget(m_insideFrame);
}

void DSearchEdit::resizeEvent(QResizeEvent *e)
{
    m_size = e->size();
    m_edt->setFixedHeight(m_size.height());
}

bool DSearchEdit::event(QEvent *e)
{
    if (e->type() == QEvent::FocusIn) {
        const QFocusEvent *event = static_cast<QFocusEvent*>(e);

        if (event->reason() == Qt::TabFocusReason
                || event->reason() == Qt::BacktabFocusReason
                || event->reason() == Qt::OtherFocusReason
                || event->reason() == Qt::ShortcutFocusReason) {
            toEditMode();
        }
    }

    return QFrame::event(e);
}

DWIDGET_END_NAMESPACE

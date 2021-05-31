/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
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

#include "dfontcombobox.h"

#include "private/dcombobox_p.h"
#include <DObjectPrivate>

#include <QListView>
#include <QTableView>
#include <QDesktopWidget>
#include <QEvent>
#include <QApplication>

DWIDGET_BEGIN_NAMESPACE

class DFontComboBoxPrivate : public DComboBoxPrivate
{
    D_DECLARE_PUBLIC(DFontComboBox)
public:
    explicit DFontComboBoxPrivate(DFontComboBox* q)
      : DComboBoxPrivate(q)
      , impl(new QFontComboBox())
    {
    }

    virtual ~DFontComboBoxPrivate() override;

    // DFontComboBox的具体实现采用QFontComboBox的实现
    QFontComboBox* impl;
};

DFontComboBoxPrivate::~DFontComboBoxPrivate()
{
    impl->deleteLater();
}

/*!
 * \~chinese \class DFontComboBox
 * \~chinese \brief DFontComboBox 字体选择下拉列表
 * \~chinese \li DFontComboBox 提供了系统字体选择功能
 *
 * \~chinese \note\row 代码示例
 * \~chinese \note\row DFontComboBox *fontComboBox = new DFontComboBox();
 */

/*!
 * \chinese \brief DFontComboBox 构造函数
 * \chinese \param parent参数被发送到 DComboBox 构造函数
 */
DFontComboBox::DFontComboBox(QWidget *parent)
    : DComboBox(*new DFontComboBoxPrivate(this), parent)
{
    setModel(d_func()->impl->model());
    setItemDelegate(d_func()->impl->itemDelegate());

    connect(this, SIGNAL(currentIndexChanged(QString)),
            d_func()->impl, SIGNAL(currentIndexChanged(QString)));
    connect(d_func()->impl, SIGNAL(currentFontChanged(const QFont &)),
            this, SIGNAL(currentFontChanged(const QFont &)));
}

DFontComboBox::~DFontComboBox()
{
}

/*!
 * \~chinese \brief 与QFontComboBox::setWritingSystem相同
 */
void DFontComboBox::setWritingSystem(QFontDatabase::WritingSystem script)
{
    D_D(DFontComboBox);
    d->impl->setWritingSystem(script);
}

/*!
 * \~chinese \brief 与QFontComboBox::writingSystem相同
 */
QFontDatabase::WritingSystem DFontComboBox::writingSystem() const
{
    D_DC(DFontComboBox);
    return d->impl->writingSystem();
}

/*!
 * \~chinese \brief 与QFontComboBox::setFontFilters相同
 */
void DFontComboBox::setFontFilters(QFontComboBox::FontFilters filters)
{
    D_DC(DFontComboBox);
    return d->impl->setFontFilters(filters);
}

/*!
 * \~chinese \brief 与QFontComboBox::fontFilters相同
 */
QFontComboBox::FontFilters DFontComboBox::fontFilters() const
{
    D_DC(DFontComboBox);
    return d->impl->fontFilters();
}

/*!
 * \~chinese \brief 与QFontComboBox::currentFont相同
 */
QFont DFontComboBox::currentFont() const
{
    D_DC(DFontComboBox);
    return d->impl->currentFont();
}

/*!
 * \~chinese \brief 与QFontComboBox::sizeHint相同
 */
QSize DFontComboBox::sizeHint() const
{
    D_DC(DFontComboBox);
    return d->impl->sizeHint();
}

/*!
 * \~chinese \brief 与QFontComboBox::setCurrentFont相同
 */
void DFontComboBox::setCurrentFont(const QFont &f)
{
    D_DC(DFontComboBox);
    d->impl->setCurrentFont(f);
}

/*!
 * \~chinese \brief 与QFontComboBox::event相同
 */
bool DFontComboBox::event(QEvent *e)
{
    if (e->type() == QEvent::Resize) {
        QListView *lview = qobject_cast<QListView*>(view());
        if (lview) {
            lview->window()->setFixedWidth(qMin(width() * 5 / 3,
                               QApplication::desktop()->availableGeometry(lview).width()));
        }
    }
    return DComboBox::event(e);
}

DWIDGET_END_NAMESPACE

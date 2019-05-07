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

#include "dbuttonlist.h"
#include "dthememanager.h"
#include "dconstants.h"
#include <QListWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QPoint>
#include <QResizeEvent>
#include <QEvent>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

DIconButton::DIconButton(const QString &Icon, const QString &text, QWidget *parent):
    QPushButton(text, parent),
    m_icon(Icon),
    m_text(text)
{
    initIconLabel();
    initConnect();
}

void DIconButton::initConnect(){
    connect(this, SIGNAL(toggled(bool)), m_iconLabel, SLOT(setVisible(bool)));
}

void DIconButton::initIconLabel(){
    m_iconLabel = new QLabel(this);
    setIconLeftMargin(20);
    m_iconLabel->hide();
    QImage image(m_icon);
    m_iconLabel->setPixmap(QPixmap::fromImage(image));
    m_iconLabel->setFixedSize(image.size());
}

void DIconButton::setIconLeftMargin(int leftMargin){
    m_iconLabel->move(leftMargin, y());
}

void DIconButton::resizeEvent(QResizeEvent *event){
    int height = event->size().height();
    m_iconLabel->move(m_iconLabel->x(), (height - m_iconLabel->height())/ 2 );
    QPushButton::resizeEvent(event);
}

void DIconButton::setIconLabel(const QString &icon){
    m_icon = icon;
    QImage image(m_icon);
    m_iconLabel->setPixmap(QPixmap::fromImage(image));
    m_iconLabel->setFixedSize(image.size());
}

void DIconButton::hideIconLabel(){
    m_iconLabel->hide();
}

void DIconButton::updateStyle(){
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void DIconButton::enterEvent(QEvent *event){
    Q_EMIT mouseEntered(text());
    QPushButton::enterEvent(event);
}

void DIconButton::leaveEvent(QEvent *event){
    Q_EMIT mouseLeaved(text());
    QPushButton::leaveEvent(event);
}

/*!
 * \class DButtonList
 *
 * \~chinese \brief 已废弃，请考虑配合 QButtonGroup 使用 QRadioButton 来替代。
 * \~english \brief Deprecated. Consider use QRadioButton with QButtonGroup instead.
 *
 * \~chinese \deprecated 请考虑配合 QButtonGroup 使用 QRadioButton 来替代。
 * \~english \deprecated Consider use QRadioButton with QButtonGroup instead.
*/

DButtonList::DButtonList(QWidget *parent) : QListWidget(parent)
{
    DThemeManager::registerWidget(this);

    setSelectionMode(DButtonList::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(ScrollPerItem);
    setResizeMode(Adjust);

    setItemSize(200, BUTTON_HEIGHT);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);

    initConnect();
}


void DButtonList::initMargins(int leftMargin, int rightMargin, int imageLeftMargin){
    m_leftMargin = leftMargin;
    m_rightMargin = rightMargin;
    m_imageLeftMargin = imageLeftMargin;
}

DButtonList::~DButtonList()
{

}

void DButtonList::initConnect(){
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setButtonChecked(int)));
}

void DButtonList::setItemWidth(int width){
    setGridSize(QSize(width, gridSize().height()));
    setFixedWidth(gridSize().width());
}

void DButtonList::setItemHeight(int height){
    setGridSize(QSize(gridSize().width(), height));
}

void DButtonList::setItemSize(int width, int height){
    setItemSize(QSize(width, height));
}

void DButtonList::setItemSize(QSize size){
    setGridSize(size);
    setFixedWidth(gridSize().width());

    for(int i=0; i< count(); i++){
        itemWidget(item(i))->setFixedHeight(size.height());
    }
}

void DButtonList::addButton(const QString &label){
    int index = count();
    addButton(label, index);
}

void DButtonList::addButton(const QString &label, int index){
    DIconButton* button = new DIconButton(":/images/dark/images/tick_hover.png", label, this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setCheckable(true);
    connect(button, SIGNAL(mouseEntered(QString)), this, SIGNAL(buttonMouseEntered(QString)));
    connect(button, SIGNAL(mouseLeaved(QString)), this, SIGNAL(buttonMouseLeaved(QString)));
    QFrame* borderFrame = new QFrame;
    borderFrame->setObjectName("BorderFrame");
    QVBoxLayout* borderLayout = new QVBoxLayout;
    borderLayout->addWidget(button);
    borderLayout->setSpacing(0);
    borderLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
    borderFrame->setLayout(borderLayout);

    m_buttonGroup->addButton(button, index);
    QListWidgetItem* item = new QListWidgetItem(this);
    addItem(item);
    setItemWidget(item, borderFrame);

    setItemSize(gridSize());

}


void DButtonList::addButtons(const QStringList &listLabels){
    for(int i= 0; i< listLabels.length(); i++) {
        addButton(listLabels.at(i), i);
    }
}

void DButtonList::setButtonChecked(int id){
   if (id < m_buttonGroup->buttons().length()){
       DIconButton* button = reinterpret_cast<DIconButton*>(m_buttonGroup->button(id));
       button->setChecked(true);
       if (m_buttonGroup->buttons().length() == 1){
            button->setProperty("state", "OnlyOne");
            button->hideIconLabel();
       }else{
            button->setProperty("state", "normal");
       }
       button->updateStyle();
       Q_EMIT buttonCheckedIndexChanged(id);
       Q_EMIT buttonChecked(button->text());
   }
}

void DButtonList::checkButtonByIndex(int index){
    if (index < m_buttonGroup->buttons().length()){
        DIconButton* button = reinterpret_cast<DIconButton*>(m_buttonGroup->button(index));
        button->click();
    }
}

DIconButton* DButtonList::getButtonByIndex(int index){
    if (index < m_buttonGroup->buttons().length()){
        DIconButton* button = reinterpret_cast<DIconButton*>(m_buttonGroup->button(index));
        return button;
    }else{
        qWarning() << "There is no this index:" << index;
    }
    return NULL;
}

void DButtonList::clear(){
    Q_FOREACH (QAbstractButton* button, m_buttonGroup->buttons()) {
//        qDebug() << static_cast<IconButton*>(button)->text();
        static_cast<DIconButton*>(button)->disconnect();
        m_buttonGroup->removeButton(static_cast<DIconButton*>(button));
    }
    QListWidget::clear();
}


DWIDGET_END_NAMESPACE

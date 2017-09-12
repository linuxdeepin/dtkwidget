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

#include "imagebuttongridtab.h"
#include "dbuttongrid.h"

#include <QHBoxLayout>
#include <QDebug>

DWIDGET_USE_NAMESPACE

ImageButtonGridTab::ImageButtonGridTab(QWidget *parent) : QFrame(parent)
{
    initData();
    initUI();
    initConnect();
    setStyleSheet("background-color: rgba(52, 52, 52, 0.9)");
}

ImageButtonGridTab::~ImageButtonGridTab()
{

}

void ImageButtonGridTab::initData(){
    for(int i= 0; i<4; i++){
        QMap<QString, QString> imageInfo;
        imageInfo.insert("key", QString("image%1").arg(QString::number(i)));
        imageInfo.insert("name", QString("image%1").arg(QString::number(i)));
        imageInfo.insert("url", ":/images/wallpaper/1.jpg");
        m_imageInfos.append(imageInfo);
    }
}

void ImageButtonGridTab::initUI(){
    DButtonGrid* buttonGridGroup = new DButtonGrid(2, 2, this);
    buttonGridGroup->setItemSize(100, 80);
    buttonGridGroup->addImageButtons(m_imageInfos, true);
    buttonGridGroup->addImageButtons(m_imageInfos, false);
    connect(buttonGridGroup, SIGNAL(buttonChecked(QString)), this, SLOT(handleClicked(QString)));
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(buttonGridGroup, 0, Qt::AlignCenter);
    layout->setContentsMargins(10, 0, 0, 0);
    setLayout(layout);
    buttonGridGroup->checkButtonByIndex(2);
}


void ImageButtonGridTab::handleClicked(QString label){
    qDebug() << label << "clicked";
}

void ImageButtonGridTab::initConnect(){

}


/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "cameraform.h"
#include "ui_cameraform.h"

#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QMediaRecorder>
#include <QMediaMetaData>
#include <QVideoProbe>
#include <QCameraImageCapture>
#include <QDebug>

CameraForm::CameraForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraForm)
{
    ui->setupUi(this);

    camera = new QCamera(QCameraInfo::defaultCamera(), this);
    viewfinder = new DVideoWidget(ui->widget);
    viewfinder->resize(150,150);
    viewfinder->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);

    viewfinder->move(ui->widget->width()/2-viewfinder->width()/2, ui->widget->height()/2-viewfinder->height()/2);
    viewfinder->setSource(camera);
}

CameraForm::~CameraForm()
{
    camera->stop();
    delete ui;
}

void CameraForm::on_horizontalSlider_valueChanged(int value)
{
    viewfinder->setScale(1+value/50.0);
}

void CameraForm::on_pushButton_clicked()
{
    ui->label->setPixmap(viewfinder->capture());
}

void CameraForm::on_pushButton_2_clicked()
{
    viewfinder->setMirroredHorizontal(!viewfinder->mirroredHorizontal());
}

void CameraForm::on_pushButton_3_clicked()
{
    viewfinder->setRound(!viewfinder->round());
}

void CameraForm::on_pushButton_4_clicked()
{
    if(camera->status() == QCamera::ActiveStatus) {
        camera->stop();
        ui->pushButton_4->setText("start");
    } else {
        camera->start();
        ui->pushButton_4->setText("stop");
    }
}

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

#ifndef CAMERAFORM_H
#define CAMERAFORM_H

#include <QWidget>
#include <QVideoFrame>

#include <DVideoWidget>

DWIDGET_USE_NAMESPACE

namespace Ui {
class CameraForm;
}

class QCamera;
class QCameraViewfinder;

class CameraForm : public QWidget
{
    Q_OBJECT

public:
    explicit CameraForm(QWidget *parent = nullptr);
    ~CameraForm();

private Q_SLOTS:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::CameraForm *ui;
    QCamera *camera;
    DVideoWidget *viewfinder;
};

#endif // FORM_H

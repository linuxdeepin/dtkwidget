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

#ifndef DABSTRACTDIALOG_H
#define DABSTRACTDIALOG_H

#include <QDialog>
#include <QPoint>

#include <DObject>

#include "dtkwidget_global.h"

class QMouseEvent;
class QPushButton;
class QResizeEvent;

DWIDGET_BEGIN_NAMESPACE

class DAbstractDialogPrivate;
class DAbstractDialog : public QDialog, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(DisplayPostion displayPostion READ displayPostion WRITE setDisplayPostion)

public:
    enum DisplayPostion {
        DisplayCenter,
        DisplayTopRight
    };

    Q_ENUMS(DisplayPostion)

    DAbstractDialog(QWidget *parent = 0);

    QColor backgroundColor() const;
    QColor borderColor() const;
    DisplayPostion displayPostion() const;

public Q_SLOTS:
    void moveToCenter();
    void moveToTopRight();
    void moveToCenterByRect(const QRect &rect);
    void moveToTopRightByRect(const QRect &rect);

    void setBackgroundColor(QColor backgroundColor);
    void setBorderColor(QColor borderColor);
    void setDisplayPostion(DisplayPostion displayPostion);

Q_SIGNALS:
    void sizeChanged(QSize size);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    DAbstractDialog(DAbstractDialogPrivate &dd, QWidget *parent = 0);

private:
    D_DECLARE_PRIVATE(DAbstractDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTDIALOG_H

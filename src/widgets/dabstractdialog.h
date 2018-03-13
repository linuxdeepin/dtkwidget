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
class LIBDTKWIDGETSHARED_EXPORT DAbstractDialog : public QDialog, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(DisplayPosition displayPosition READ displayPosition WRITE setDisplayPosition)

public:
    /**
     * @brief The DisplayPosition enum contains the position options that can be specified by all dialogs.
     */
    enum DisplayPosition {
        Center, /*!< display this dialog in the center of the screen */
        TopRight /*!< display this dialog in the top right of the screen */
    };
    enum DisplayPostion {
        DisplayCenter = Center, /*!< display this dialog in the center of the screen */
        DisplayTopRight = TopRight /*!< display this dialog in the top right of the screen */
    };

    Q_ENUMS(DisplayPosition)
    Q_ENUMS(DisplayPostion)

    DAbstractDialog(QWidget *parent = 0);

    QColor backgroundColor() const;
    QColor borderColor() const;
    DisplayPosition displayPosition() const;
    D_DECL_DEPRECATED DisplayPostion displayPostion() const;

    void move(const QPoint &pos);
    void setGeometry(const QRect &rect);

public Q_SLOTS:
    void moveToCenter();
    void moveToTopRight();
    void moveToCenterByRect(const QRect &rect);
    void moveToTopRightByRect(const QRect &rect);

    void setBackgroundColor(QColor backgroundColor);
    void setBorderColor(QColor borderColor);
    void setDisplayPosition(DisplayPosition displayPosition);
    D_DECL_DEPRECATED void setDisplayPostion(DisplayPostion displayPosition);

Q_SIGNALS:
    /**
     * @brief sizeChanged is emitted when the size of this dialog changed.
     * @param size is the target size.
     */
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

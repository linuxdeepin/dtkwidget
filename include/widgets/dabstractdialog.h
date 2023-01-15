// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DABSTRACTDIALOG_H
#define DABSTRACTDIALOG_H

#include <QDialog>
#include <QPoint>

#include <DObject>

#include <dtkwidget_global.h>

class QMouseEvent;
class QPushButton;
class QResizeEvent;

DWIDGET_BEGIN_NAMESPACE

class DAbstractDialogPrivate;
class LIBDTKWIDGETSHARED_EXPORT DAbstractDialog : public QDialog, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(DisplayPosition displayPosition READ displayPosition WRITE setDisplayPosition)

public:
    enum DisplayPosition {
        Center,/*!<@~english display this dialog in the center of the screen */
        TopRight/*!<@~english display this dialog in the top right of the screen */
    };
    enum DisplayPostion { // This is wrong, but keep it for compatibility
        DisplayCenter = Center,
        DisplayTopRight = TopRight
    };

    Q_ENUMS(DisplayPosition)
    Q_ENUMS(DisplayPostion)

    DAbstractDialog(QWidget *parent = nullptr);
    DAbstractDialog(bool blurIfPossible, QWidget *parent = nullptr);

    DisplayPosition displayPosition() const;

    void move(const QPoint &pos);
    inline void move(int x, int y)
    { move(QPoint(x, y));}

    void setGeometry(const QRect &rect);
    inline void setGeometry(int x, int y, int width, int height)
    { setGeometry(QRect(x, y, width, height));}

public Q_SLOTS:
    void moveToCenter();
    void moveToTopRight();
    void moveToCenterByRect(const QRect &rect);
    void moveToTopRightByRect(const QRect &rect);

    void setDisplayPosition(DisplayPosition displayPosition);

Q_SIGNALS:
    /**
    @~english
     * \brief sizeChanged is emitted when the size of this dialog changed.
     * \a size is the target size.
     */
    void sizeChanged(QSize size);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) override;

protected:
    DAbstractDialog(DAbstractDialogPrivate &dd, QWidget *parent = nullptr);

private:
    D_DECLARE_PRIVATE(DAbstractDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTDIALOG_H

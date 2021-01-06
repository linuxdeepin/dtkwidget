/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     linxun <linxun@uniontech.com>
*
* Maintainer: lixnun <linxun@uniontech.com>
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

#ifndef DPRINTPICKCOLORWIDGET_H
#define DPRINTPICKCOLORWIDGET_H
#include "qdbusinterface.h"
#include "dfloatingwidget.h"
#include "dlabel.h"

#include <DWidget>
#include <DPushButton>

class QVBoxLayout;
DWIDGET_BEGIN_NAMESPACE
class DIconButton;
class DLineEdit;
class DLabel;
class DSlider;

class ColorButton : public DPushButton
{
    Q_OBJECT
public:
    ColorButton(QColor color, QWidget *parent = nullptr);
Q_SIGNALS:
    void selectColorButton(QColor color);
    void btnIsChecked(bool checked);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QColor m_color;
    bool m_flag = false;
    bool m_checked = false;
};
class ColorLabel : public DLabel
{
    Q_OBJECT
public:
    ColorLabel(DWidget *parent = nullptr);
    ~ColorLabel();

    //h∈(0, 360), s∈(0, 1), v∈(0, 1)
    QColor getColor(qreal h, qreal s, qreal v);
    void setHue(int hue);

    void pickColor(QPoint pos);
    QCursor pickColorCursor();

Q_SIGNALS:
    void clicked();
    void pickedColor(QColor color);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QCursor m_lastCursor;
    int m_hue = 0;
    bool m_pressed;
    QColor m_pickedColor;
    QPoint m_clickedPos;
    QPoint m_tipPoint;
};
class ColorSlider : public QSlider
{
    Q_OBJECT
public:
    ColorSlider(QWidget *parent = nullptr);
    ~ColorSlider();

    //h∈(0, 360), s∈(0, 1), v∈(0, 1)
    QColor getColor(qreal h, qreal s, qreal v);

protected:
    void paintEvent(QPaintEvent *ev);

private:
    int m_value;
    QImage m_backgroundImage;
};
class DPrintPickColorWidget : public DWidget
{
    Q_OBJECT
public:
    DPrintPickColorWidget(QWidget *parent);
    ~DPrintPickColorWidget();
    void initUI();
    void initConnection();
    void setRgbEdit(QColor color, bool btnColor = false);
    void convertColor(QColor color, bool btnColor = false);
Q_SIGNALS:
    void selectColorButton(QColor color);
    void signalColorChanged(QColor color);
public Q_SLOTS:
    void slotColorPick(QString uuid, QString colorName);
    void slotEditColor(QString str);

private:
    QList<ColorButton *> btnlist;
    QList<QColor> colorList;
    QButtonGroup *btnGroup;
    DSlider *alphaSlider;
    DLabel *alphaLabel;
    DLineEdit *valueLineEdit;
    DPushButton *expandButton;
    DIconButton *pickColorBtn;
    DWidget *expandWidget;
    QDBusInterface *pinterface;
    DLineEdit *rEdit;
    DLineEdit *gEdit;
    DLineEdit *bEdit;
    ColorLabel *colorLabel;
    ColorSlider *colorSlider;
};
DWIDGET_END_NAMESPACE
#endif // DPRINTPICKCOLORWIDGET_H

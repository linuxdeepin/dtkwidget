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

#ifndef DSEARCHEDIT_H
#define DSEARCHEDIT_H

#include <QFrame>
#include <QSize>
#include <QLineEdit>
#include <QPropertyAnimation>

#include "dtkwidget_global.h"
#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSearchEdit : public QFrame
{
    Q_OBJECT
public:
    explicit DSearchEdit(QWidget *parent = 0);
    ~DSearchEdit();

    QSize sizeHint() const {return m_size;}
    QSize minimumSizeHint() const {return m_size;}
    const QString text() const;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

    inline void setAniDuration(const int duration) {m_animation->setDuration(duration);}
    inline void setAniShowCurve(const QEasingCurve curve) {m_showCurve = curve;}
    inline void setAniHideCurve(const QEasingCurve curve) {m_hideCurve = curve;}

    QLineEdit *getLineEdit() const;

public Q_SLOTS:
    void setText(const QString & text) {if (m_edt) m_edt->setText(text);}
    inline void clear() {m_edt->clear();}
    inline void setPlaceHolder(const QString &text) {m_placeHolder->setText(text);}

Q_SIGNALS:
    void textChanged();
    void returnPressed();
    void editingFinished();
    void focusOut();
    void focusIn();

private Q_SLOTS:
    void toEditMode();

private:
    void initInsideFrame();
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *e);

private:
    QSize m_size;
    QLineEdit *m_edt;
    QLabel *m_searchBtn;
    DImageButton *m_clearBtn;
    QLabel *m_placeHolder;
    QFrame *m_insideFrame = NULL;

    QPropertyAnimation *m_animation;
    QEasingCurve m_showCurve = QEasingCurve::OutCubic;
    QEasingCurve m_hideCurve = QEasingCurve::InCubic;
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHEDIT_H

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

#ifndef DBUTTONLIST_H
#define DBUTTONLIST_H

#include <QListWidget>
#include <QPushButton>
#include "dtkwidget_global.h"

class QButtonGroup;
class QLabel;
class QPoint;
class QResizeEvent;
class QEvent;

DWIDGET_BEGIN_NAMESPACE

class DButtonListIconButton : public QPushButton
{
    Q_OBJECT
public:
    DButtonListIconButton(const QString &Icon, const QString &text, QWidget *parent = Q_NULLPTR);

    void initIconLabel();
    void initConnect();
    void setIconLeftMargin(int leftMargin);
    void setIconLabel(const QString &icon);
    void hideIconLabel();
    void updateStyle();

Q_SIGNALS:
    void mouseEntered(QString label);
    void mouseLeaved(QString label);

protected:
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QLabel *m_iconLabel;
    QString m_icon;
    QString m_text;
};

class LIBDTKWIDGETSHARED_EXPORT DButtonList : public QListWidget
{
    Q_OBJECT
public:
    Q_DECL_DEPRECATED explicit DButtonList(QWidget *parent = Q_NULLPTR);
    ~DButtonList();

    void initMargins(int leftMargin, int rightMargin, int imageLeftMargin);
    DButtonListIconButton *getButtonByIndex(int index);

public Q_SLOTS:
    void addButton(const QString &label);
    void addButton(const QString &label, int index);
    void addButtons(const QStringList &listLabels);
    void setItemHeight(int height);
    void setItemWidth(int width);
    void setItemSize(int width, int height);
    void setItemSize(QSize size);
    void checkButtonByIndex(int index);
    void clear();

private Q_SLOTS:
    void setButtonChecked(int id);

Q_SIGNALS:
    void buttonChecked(QString label);
    void buttonCheckedIndexChanged(int index);
    void buttonMouseEntered(QString label);
    void buttonMouseLeaved(QString label);

private:
    QButtonGroup *m_buttonGroup = NULL;

    void initConnect();

    int m_leftMargin = 10;
    int m_rightMargin = 10;
    int m_imageLeftMargin = 10;
};


DWIDGET_END_NAMESPACE

#endif // DBUTTONLIST_H

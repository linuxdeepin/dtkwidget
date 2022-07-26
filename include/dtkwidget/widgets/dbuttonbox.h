/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DBUTTONBOX_H
#define DBUTTONBOX_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DStyle>

#include <QAbstractButton>

DWIDGET_BEGIN_NAMESPACE

class DStyleOptionButtonBoxButton;
class DButtonBoxButtonPrivate;
class DButtonBoxButton : public QAbstractButton, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DButtonBoxButton)

public:
    explicit DButtonBoxButton(const QString &text, QWidget *parent = nullptr);
    DButtonBoxButton(const QIcon& icon, const QString &text = QString(), QWidget *parent = nullptr);
    DButtonBoxButton(QStyle::StandardPixmap iconType = static_cast<QStyle::StandardPixmap>(-1),
                     const QString &text = QString(), QWidget *parent = nullptr);
    DButtonBoxButton(DStyle::StandardPixmap iconType = static_cast<DStyle::StandardPixmap>(-1),
                     const QString &text = QString(), QWidget *parent = nullptr);

    void setIcon(const QIcon &icon);
    void setIcon(QStyle::StandardPixmap iconType);
    void setIcon(DStyle::StandardPixmap iconType);

    QSize iconSize() const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const override;

private:
    void initStyleOption(DStyleOptionButtonBoxButton *option) const;

    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent *e) override;
};

class DButtonBoxPrivate;
class DButtonBox : public QWidget, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DButtonBox)

public:
    explicit DButtonBox(QWidget *parent = nullptr);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    void setButtonList(const QList<DButtonBoxButton*> &list, bool checkable);
    QList<QAbstractButton*> buttonList() const;

    QAbstractButton * checkedButton() const;
    // no setter on purpose!

    QAbstractButton *button(int id) const;
    void setId(QAbstractButton *button, int id);
    int id(QAbstractButton *button) const;
    int checkedId() const;

Q_SIGNALS:
    void buttonClicked(QAbstractButton *);
    void buttonPressed(QAbstractButton *);
    void buttonReleased(QAbstractButton *);
    void buttonToggled(QAbstractButton *, bool);

private:
    void paintEvent(QPaintEvent *e) override;

    friend class DButtonBoxButton;
};

DWIDGET_END_NAMESPACE

#endif // DBUTTONBOX_H

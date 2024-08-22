// SPDX-FileCopyrightText: 2019 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBUTTONBOX_H
#define DBUTTONBOX_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DStyle>
#include <DDciIcon>

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
    DButtonBoxButton(const DDciIcon &dciIcon, const QString &text = QString(), QWidget *parent = nullptr);

    void setIcon(const QIcon &icon);
    void setIcon(QStyle::StandardPixmap iconType);
    void setIcon(DStyle::StandardPixmap iconType);

    void setIcon(const DDciIcon &icon);
    DDciIcon dciIcon() const;

    QSize iconSize() const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const override;

private:
    void initStyleOption(DStyleOptionButtonBoxButton *option) const;

    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent *e) override;

    friend class DButtonBox;
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

protected:
    bool eventFilter(QObject *o, QEvent *e) override;

private:
    void paintEvent(QPaintEvent *e) override;

    friend class DButtonBoxButton;
};

DWIDGET_END_NAMESPACE

#endif // DBUTTONBOX_H

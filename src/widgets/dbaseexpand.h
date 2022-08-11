// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBASEEXPAND_H
#define DBASEEXPAND_H

#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>

#include <DHorizontalLine>

#include <dtkwidget_global.h>
#include <dconstants.h>

DWIDGET_BEGIN_NAMESPACE

class D_DECL_DEPRECATED ContentLoader : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int height READ height WRITE setFixedHeight)
public:
    explicit ContentLoader(QWidget *parent = nullptr) : QFrame(parent){}
};

class DBoxWidget;

class DBaseExpandPrivate;
class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED_X("Use DDrawer") DBaseExpand : public QWidget
{
    Q_OBJECT
public:
    explicit DBaseExpand(QWidget *parent = nullptr);
    ~DBaseExpand() override;

    void setHeader(QWidget *header);
    void setContent(QWidget *content, Qt::Alignment alignment = Qt::AlignHCenter);
    QWidget *getContent() const;
    void setHeaderHeight(int height);
    virtual void setExpand(bool value);
    bool expand() const;
    void setAnimationDuration(int duration);
    void setAnimationEasingCurve(QEasingCurve curve);
    void setSeparatorVisible(bool arg);
    void setExpandedSeparatorVisible(bool arg);

Q_SIGNALS:
    void expandChange(bool e);
    void sizeChanged(QSize s);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private:
    QScopedPointer<DBaseExpandPrivate> d_private;

    Q_DECLARE_PRIVATE_D(d_private, DBaseExpand)
};

DWIDGET_END_NAMESPACE

#endif // DBASEEXPAND_H

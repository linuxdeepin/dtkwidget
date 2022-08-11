// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDRAWER_H
#define DDRAWER_H

#include <DFrame>

DWIDGET_BEGIN_NAMESPACE
class DDrawerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DDrawer : public DFrame
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DDrawer)

public:
    explicit DDrawer(QWidget *parent = nullptr);
    ~DDrawer() override;

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
    DDrawer(DDrawerPrivate &dd, QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
};

DWIDGET_END_NAMESPACE

#endif // DDRAWER_H

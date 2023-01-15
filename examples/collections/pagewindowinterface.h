// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PAGEWINDOWINTERFACE_H
#define PAGEWINDOWINTERFACE_H

#include <QWidget>
#include <QMouseEvent>

class ExampleWindowInterface;

class PageWindowInterface : public QWidget
{
public:
    explicit PageWindowInterface(QWidget *parent);
    virtual ~PageWindowInterface() override;

public:
    virtual void initPageWindow();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    virtual QWidget *doLayout(ExampleWindowInterface *pExample);
    void addExampleWindow(ExampleWindowInterface *pExample);

private:
    QList<ExampleWindowInterface *> m_exampleList;
};

#endif // PAGEWINDOWINTERFACE_H

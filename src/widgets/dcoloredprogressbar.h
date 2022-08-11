// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <dtkwidget_global.h>
#include <DObject>

#include <QProgressBar>

DWIDGET_BEGIN_NAMESPACE

class DColoredProgressBarPrivate;
class LIBDTKWIDGETSHARED_EXPORT DColoredProgressBar : public QProgressBar, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DColoredProgressBar(QWidget *parent = nullptr);
    void addThreshold(int threshold, QBrush brush);
    void removeThreshold(int threshold);
    QList<int> thresholds() const;

protected:
    void paintEvent(QPaintEvent *) override;

private:
    D_DECLARE_PRIVATE(DColoredProgressBar)
};

DWIDGET_END_NAMESPACE

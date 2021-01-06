/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chengyulong <chengyulong@uniontech.com>
*
* Maintainer: chengyulong <chengyulong@uniontech.com>
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

#ifndef DPRINTPREVIEWDIALOG_H
#define DPRINTPREVIEWDIALOG_H

#include <DDialog>
#include "dprintpreviewwidget.h"

DWIDGET_BEGIN_NAMESPACE
class DPrintPreviewDialogPrivate;
class DPrintPreviewDialog : public DDialog
{
    Q_OBJECT
public:
    explicit DPrintPreviewDialog(QWidget *parent = nullptr);
    ~DPrintPreviewDialog() override;

Q_SIGNALS:
    void paintRequested(DPrinter *printer);
    void paintRequested(DPrinter *printer, const QVector<int> &pageRange);

private:
    D_DECLARE_PRIVATE(DPrintPreviewDialog)
    D_PRIVATE_SLOT(void _q_printerChanged(int))
    D_PRIVATE_SLOT(void _q_pageRangeChanged(int))
    D_PRIVATE_SLOT(void _q_pageMarginChanged(int))
    D_PRIVATE_SLOT(void _q_ColorModeChange(int))
    D_PRIVATE_SLOT(void _q_startPrint(bool))
    D_PRIVATE_SLOT(void _q_orientationChanged(int))
    D_PRIVATE_SLOT(void _q_customPagesFinished())
    D_PRIVATE_SLOT(void _q_marginspinChanged(double))
    D_PRIVATE_SLOT(void _q_marginEditFinished())
    D_PRIVATE_SLOT(void _q_currentPageSpinChanged(int))
    D_PRIVATE_SLOT(void _q_checkStateChanged(int))
    D_PRIVATE_SLOT(void _q_textWaterMarkModeChanged(int))
    D_PRIVATE_SLOT(void _q_customTextWatermarkFinished())
    D_PRIVATE_SLOT(void _q_colorButtonCliked(bool))
    D_PRIVATE_SLOT(void _q_selectColorButton(QColor))
    D_PRIVATE_SLOT(void _q_pagePersheetComboIndexChanged(int))
    D_PRIVATE_SLOT(void _q_printOrderComboIndexChanged(int))

public:
    virtual bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void setDocName(const QString &);
    QString docName() const;

    bool setPrintFromPath(const QString &path = QString());
    QString printFromPath() const;

    bool setAsynPreview(int totalPage);
    bool isAsynPreview() const;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_H

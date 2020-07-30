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
    ~DPrintPreviewDialog();



Q_SIGNALS:
    void paintRequested(DPrinter *printer);

private:
    D_DECLARE_PRIVATE(DPrintPreviewDialog)
    D_PRIVATE_SLOT(void _q_printerChanged(int))
    D_PRIVATE_SLOT(void _q_pageRangeChanged(int))
    D_PRIVATE_SLOT(void _q_pageMarginChanged(int))
    D_PRIVATE_SLOT(void _q_ColorModeChange(int))
    D_PRIVATE_SLOT(void _q_startPrint(bool))
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_H

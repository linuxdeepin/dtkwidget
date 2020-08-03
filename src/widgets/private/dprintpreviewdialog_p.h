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

#ifndef DPRINTPREVIEWDIALOG_P_H
#define DPRINTPREVIEWDIALOG_P_H

#include "dprintpreviewdialog.h"
#include "ddialog_p.h"
#include <DWidget>
#include <DPushButton>
#include <DComboBox>
#include <DRadioButton>
#include <private/qprintdevice_p.h>
class QVBoxLayout;
class QButtonGroup;
class DScrollArea;
class QPrinter;
DWIDGET_BEGIN_NAMESPACE
class DFrame;
class DIconButton;
class DLineEdit;
class DLabel;
class DSpinBox;
class DSwitchButton;
class DCommandLinkButton;
class DSuggestButton;
class DDoubleSpinBox;
class DPrintPreviewDialogPrivate : public DDialogPrivate
{
public:
    explicit DPrintPreviewDialogPrivate(DPrintPreviewDialog *qq);
    void startup();
    void initui();
    void initleft(QVBoxLayout *layout);
    void initright(QVBoxLayout *layout);
    void initbasicui();
    void initadvanceui();
    void initdata();
    void initconnections();
    void setfrmaeback(DWidget *frame);
    void showadvancesetting();
    void setupPrinter();
    void setScaling(int index);
    void test();
    void updateSetteings(int index);
    QVector<int> checkDuplication(QVector<int> data);

    void setEnable(const int &value, DComboBox *combox); //控件可用

    void _q_printerChanged(int index);
    void _q_pageRangeChanged(int index);
    void _q_pageMarginChanged(int index);
    void _q_ColorModeChange(int index);
    void _q_startPrint(bool clicked);
    void _q_orientationChanged(int index);
    void _q_customPagesFinished();
    void _q_marginTimerOut();
    void _q_marginspinChanged(double);

    //printer
    DPrinter *printer;
    QPrintDevice m_currentPrintDevice;
    int totalPages = 0;

    //control
    DPrintPreviewWidget *pview;
    DWidget *basicsettingwdg;
    DWidget *advancesettingwdg;
    DIconButton *firstBtn;
    DIconButton *prevPageBtn;
    DSpinBox *jumpPageEdit;
    DLabel *totalPageLabel;
    DIconButton *nextPageBtn;
    DIconButton *lastBtn;
    DPushButton *cancelBtn;
    DSuggestButton *printBtn;
    DPushButton *advanceBtn;
    DScrollArea *scrollarea;
    DComboBox *printDeviceCombo;
    DSpinBox *copycountspinbox;
    DComboBox *pageRangeCombo;
    DLineEdit *pageRangeEdit;
    QButtonGroup *orientationgroup;
    DComboBox *colorModeCombo;
    DComboBox *marginsCombo;
    DDoubleSpinBox *marginTopSpin;
    DDoubleSpinBox *marginBottomSpin;
    DDoubleSpinBox *marginLeftSpin;
    DDoubleSpinBox *marginRightSpin;
    QButtonGroup *scaleGroup;
    DSpinBox *scaleRateEdit;
    DComboBox *paperSizeCombo;
    DSwitchButton *duplexSwitchBtn;
    DComboBox *pagePerSheetCombo;
    DIconButton *lrtbBtn; //左右上下
    DIconButton *rltbBtn; //右左上下
    DIconButton *tblrBtn; //上下左右
    DIconButton *tbrlBtn; //上下右左
    QButtonGroup *printOrderGroup; //打印顺序
    DComboBox *inorderCombo;
    DComboBox *waterTypeCombo; //添加水印类型
    DIconButton *waterColorBtn;
    DLineEdit *waterTextEdit;
    QTimer *marginTimer;

    Q_DECLARE_PUBLIC(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_P_H

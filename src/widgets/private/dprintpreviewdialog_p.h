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
#include <DCheckBox>
#include <DComboBox>
#include <DRadioButton>

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
    void marginsLayout(bool adapted);
    void initdata();
    void initconnections();
    void setfrmaeback(DFrame *frame);
    void showadvancesetting();
    void setupPrinter();
    void setScaling(int index);
    void judgeSupportedAttributes(const QString &lastPaperSize);
    void setMininumMargins();
    void themeTypeChange(DGuiApplicationHelper::ColorType themeType);
    void setPageIsLegal(bool islegal);
    QVector<int> checkDuplication(QVector<int> data);

    void setEnable(const int &value, DComboBox *combox); //控件可用
    void setTurnPageBtnStatus();

    void _q_printerChanged(int index);
    void _q_pageRangeChanged(int index);
    void _q_pageMarginChanged(int index);
    void _q_ColorModeChange(int index);
    void _q_startPrint(bool clicked);
    void _q_orientationChanged(int index);
    void _q_customPagesFinished();
    void _q_marginTimerOut();
    void _q_marginspinChanged(double);
    void _q_marginEditFinished();
    void _q_currentPageSpinChanged(int value);
    void _q_checkStateChanged(int state);

    //printer
    DPrinter *printer = nullptr;
    int totalPages = 0;
    bool fontSizeMore = false;
    QString lastPageRange = "";
    bool supportedDuplexFlag = false;
    bool supportedColorMode = false;
    bool isInited = false;
    bool marginsControl = false;

    //control
    DPrintPreviewWidget *pview = nullptr;
    DWidget *basicsettingwdg = nullptr;
    DWidget *advancesettingwdg = nullptr;
    DIconButton *firstBtn = nullptr;
    DIconButton *prevPageBtn = nullptr;
    DSpinBox *jumpPageEdit = nullptr;
    DLabel *totalPageLabel = nullptr;
    DIconButton *nextPageBtn = nullptr;
    DIconButton *lastBtn = nullptr;
    DPushButton *cancelBtn = nullptr;
    DSuggestButton *printBtn = nullptr;
    DPushButton *advanceBtn = nullptr;
    DScrollArea *scrollarea = nullptr;
    DComboBox *printDeviceCombo = nullptr;
    DSpinBox *copycountspinbox = nullptr;
    DComboBox *pageRangeCombo = nullptr;
    DLineEdit *pageRangeEdit = nullptr;
    QButtonGroup *orientationgroup = nullptr;
    DComboBox *colorModeCombo = nullptr;
    DComboBox *marginsCombo = nullptr;
    DDoubleSpinBox *marginTopSpin = nullptr;
    DDoubleSpinBox *marginBottomSpin = nullptr;
    DDoubleSpinBox *marginLeftSpin = nullptr;
    DDoubleSpinBox *marginRightSpin = nullptr;
    QButtonGroup *scaleGroup = nullptr;
    DSpinBox *scaleRateEdit = nullptr;
    DComboBox *duplexCombo = nullptr;
    DCheckBox *duplexCheckBox = nullptr;
    DComboBox *paperSizeCombo = nullptr;
    DComboBox *pagePerSheetCombo = nullptr;
    DIconButton *lrtbBtn = nullptr; //左右上下
    DIconButton *rltbBtn = nullptr; //右左上下
    DIconButton *tblrBtn = nullptr; //上下左右
    DIconButton *tbrlBtn = nullptr; //上下右左
    QButtonGroup *printOrderGroup = nullptr; //打印顺序
    DComboBox *inorderCombo = nullptr;
    DComboBox *waterTypeCombo = nullptr; //添加水印类型
    DIconButton *waterColorBtn = nullptr;
    DLineEdit *waterTextEdit = nullptr;
    QVector<qreal> marginOldValue= {}; // 记录margin自定义时的旧值  如果旧值和新值一致，就不需要刷新，top left right bottom
    Q_DECLARE_PUBLIC(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_P_H

#ifndef DPRINTPREVIEWDIALOG_P_H
#define DPRINTPREVIEWDIALOG_P_H

#include "dprintpreviewdialog.h"
#include "ddialog_p.h"
#include <DWidget>
#include <DPushButton>
#include <DComboBox>
#include <DRadioButton>
#include "QtPrintSupport/private/qprintdevice_p.h"
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
class DDoubleSpinBox;
class DSuggestButton;
class DPrintPreviewDialogPrivate : public DDialogPrivate
{
public:
    explicit DPrintPreviewDialogPrivate(DPrintPreviewDialog *qq);
    void startup(DPrinter *printer = nullptr);
    void initui();
    void initleft(QVBoxLayout *layout);
    void initright(QVBoxLayout *layout);
    void initbasicui();
    void initadvanceui();
    void initdata();
    void initconnections();
    void setfrmaeback(DWidget *frame);
    void showadvancesetting();

    void test();

    void setEnable(const int &value, DComboBox *combox); //控件可用

    //printer
    DPrinter *printer;
    bool ownPrinter;

    //control
    DPrintPreviewWidget *pview;
    DWidget *basicsettingwdg;
    DWidget *advancesettingwdg;
    DIconButton *firstBtn;
    DIconButton *prevPageBtn;
    DLineEdit *jumpPageEdit;
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
    DSpinBox *fromeSpin;
    DSpinBox *toSpin;
    QButtonGroup *orientationgroup;
    DComboBox *colorModeCombo;
    DComboBox *marginsCombo;
    DDoubleSpinBox *marginTopSpin;
    DSpinBox *marginBottomSpin;
    DSpinBox *marginLeftSpin;
    DSpinBox *marginRightSpin;
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

    QPrintDevice m_currentPrintDevice;
    Q_DECLARE_PUBLIC(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_P_H

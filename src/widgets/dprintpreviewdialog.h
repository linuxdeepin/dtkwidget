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
    explicit DPrintPreviewDialog(DPrinter *printer, QWidget *parent = nullptr);
    ~DPrintPreviewDialog();
public Q_SLOTS:
    void printerChanged(int index);
    void slotPageRangeCombox(int value);
    void slotPageMarginCombox(int value);
    void slotStartPrint(bool clicked);

Q_SIGNALS:
    void paintRequested(DPrinter *printer);

private:
    D_DECLARE_PRIVATE(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_H

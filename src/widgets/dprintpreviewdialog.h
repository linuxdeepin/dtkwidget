#ifndef DPRINTPREVIEWDIALOG_H
#define DPRINTPREVIEWDIALOG_H

#include "ddialog.h"

class QPrinter;
DWIDGET_BEGIN_NAMESPACE
class DPrintPreviewDialogPrivate;
class DPrintPreviewDialog : public DDialog
{
    Q_OBJECT
public:
    explicit DPrintPreviewDialog(QWidget *parent = nullptr);
    explicit DPrintPreviewDialog(QPrinter *printer, QWidget *parent = nullptr);
    ~DPrintPreviewDialog();

public Q_SLOTS:

    void printerChanged(int index);
    void slotPageRangeCombox(int value);
    void slotPageMarginCombox(int value);

Q_SIGNALS:
    void paintRequested(QPrinter *printer);

private:
    D_DECLARE_PRIVATE(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_H

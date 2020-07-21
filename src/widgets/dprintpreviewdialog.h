#ifndef DPRINTPREVIEWDIALOG_H
#define DPRINTPREVIEWDIALOG_H

#include "ddialog.h"

DWIDGET_BEGIN_NAMESPACE

class DPrintPreviewDialogPrivate;
class DPrintPreviewDialog : public DDialog
{
    Q_OBJECT
public:
    explicit DPrintPreviewDialog(QWidget *parent = nullptr);

public Q_SLOTS:
    void showAdvanceSetting();

Q_SIGNALS:

private:
    D_DECLARE_PRIVATE(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_H

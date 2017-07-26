#pragma once

#include <QPointer>
#include <QScopedPointer>

#include <DSettings>

#include "dabstractdialog.h"

DWIDGET_BEGIN_NAMESPACE

class DSettingsDialogPrivate;
class DSettingsDialog : public DAbstractDialog
{
    Q_OBJECT
public:
    DSettingsDialog(QWidget *parent = 0);
    ~DSettingsDialog();

public Q_SLOTS:
    void updateSettings(DTK_CORE_NAMESPACE::DSettings *settings);

private:
    QScopedPointer<DSettingsDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DSettingsDialog)
};

DWIDGET_END_NAMESPACE

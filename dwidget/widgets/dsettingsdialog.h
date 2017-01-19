#pragma once

#include <QPointer>
#include <QScopedPointer>

#include "dabstractdialog.h"

class Settings;

DWIDGET_BEGIN_NAMESPACE

class DSettingsDialogPrivate;
class DSettingsDialog : public DAbstractDialog
{
public:
    DSettingsDialog(QWidget *parent = 0);
    ~DSettingsDialog();

public slots:
    void updateSettings(QPointer<Settings> settings);

private:
    QScopedPointer<DSettingsDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DSettingsDialog)
};


DWIDGET_END_NAMESPACE

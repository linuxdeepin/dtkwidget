// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPRINTPREVIEWDIALOG_H
#define DPRINTPREVIEWDIALOG_H
#ifndef DTK_PRINTSUPPORT_PLUGIN
#define DTK_PRINTSUPPORT_PLUGIN
#endif

#include <DDialog>
#include <dprintpreviewwidget.h>

DWIDGET_BEGIN_NAMESPACE
class DPrintPreviewDialogPrivate;
class DPrintPreviewDialog : public DDialog
{
    Q_OBJECT
public:
    explicit DPrintPreviewDialog(QWidget *parent = nullptr);
    ~DPrintPreviewDialog() override;

    static void setPluginMimeData(const QVariant &mimeData);
    static QVariant pluginMimeData();

    static bool setCurrentPlugin(const QString &pluginName);
    static QString currentPlugin();

    static QStringList availablePlugins();
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
    D_PRIVATE_SLOT(void _q_spinboxValueEmptyChecked(const QString &))
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
    void timerEvent(QTimerEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_H

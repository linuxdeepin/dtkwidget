#ifndef DPRINTPREVIEWWIDGET_H
#define DPRINTPREVIEWWIDGET_H

#include <dtkwidget_global.h>
#include <DObject>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsItem>

#define private protected
#include <QPrinter>
#undef private

DWIDGET_BEGIN_NAMESPACE

class DPrintPreviewWidgetPrivate;

class DPrinter : public QPrinter
{
public:
    explicit DPrinter(PrinterMode mode = ScreenResolution);
    ~DPrinter() {}

    void setPreviewMode(bool isPreview);

    QList<const QPicture *> getPrinterPages();

private:
};

class LIBDTKWIDGETSHARED_EXPORT DPrintPreviewWidget : public QWidget
    , public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DPrintPreviewWidget(DPrinter *printer, QWidget *parent = nullptr);

    void setVisible(bool visible) override;

public Q_SLOTS:
    void updatePreview();

Q_SIGNALS:
    void paintRequested(DPrinter *printer);
    void previewChanged();

private:
    D_DECLARE_PRIVATE(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_H

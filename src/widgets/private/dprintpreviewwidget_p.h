#ifndef DPRINTPREVIEWWIDGET_P_H
#define DPRINTPREVIEWWIDGET_P_H

#include "dprintpreviewwidget.h"
#include <DObjectPrivate>
#include <QGraphicsScene>
#include <QGraphicsView>

DWIDGET_BEGIN_NAMESPACE

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr)
        : QGraphicsView(parent)
    {
    }
Q_SIGNALS:
    void resized();

protected:
    void resizeEvent(QResizeEvent *e) override
    {
        {
            QGraphicsView::resizeEvent(e);
        }
        Q_EMIT resized();
    }

    void showEvent(QShowEvent *e) override
    {
        QGraphicsView::showEvent(e);
        Q_EMIT resized();
    }
};

class DPrintPreviewWidgetPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    explicit DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq);

    void init();
    void populateScene();
    void generatePreview();
    void fitView();

    GraphicsView *graphicsView;
    QGraphicsScene *scene;

    QList<const QPicture *> pictures;
    QList<QGraphicsItem *> pages;

    DPrinter *previewPrinter;

    Q_DECLARE_PUBLIC(DPrintPreviewWidget)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWWIDGET_P_H

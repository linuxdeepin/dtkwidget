#ifndef DCLIPEFFECTWIDGET_H
#define DCLIPEFFECTWIDGET_H

#include "dwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DClipEffectWidgetPrivate;
class DClipEffectWidget : public QWidget, public DObject
{
    Q_OBJECT

    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)

public:
    explicit DClipEffectWidget(QWidget *parent);

    void setClipPath(const QPainterPath &path);

    QMargins margins() const;

public slots:
    void setMargins(QMargins margins);

signals:
    void marginsChanged(QMargins margins);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DClipEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DCLIPEFFECTWIDGET_H

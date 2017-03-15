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
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath NOTIFY clipPathChanged)

public:
    explicit DClipEffectWidget(QWidget *parent);

    QMargins margins() const;
    QPainterPath clipPath() const;

public slots:
    void setMargins(QMargins margins);
    void setClipPath(const QPainterPath &path);

signals:
    void marginsChanged(QMargins margins);
    void clipPathChanged(QPainterPath clipPath);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DClipEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DCLIPEFFECTWIDGET_H

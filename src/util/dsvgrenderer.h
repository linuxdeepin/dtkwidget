#ifndef DSVGRENDERER_H
#define DSVGRENDERER_H

#include <QObject>
#include <QRectF>

#include <dtkwidget_global.h>
#include <dobject.h>

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DSvgRendererPrivate;
class DSvgRenderer : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_PROPERTY(QRectF viewBox READ viewBoxF WRITE setViewBox)
public:
    DSvgRenderer(QObject *parent = Q_NULLPTR);
    DSvgRenderer(const QString &filename, QObject *parent = Q_NULLPTR);
    DSvgRenderer(const QByteArray &contents, QObject *parent = Q_NULLPTR);
    ~DSvgRenderer();

    bool isValid() const;

    QSize defaultSize() const;

    QRect viewBox() const;
    QRectF viewBoxF() const;
    void setViewBox(const QRect &viewbox);
    void setViewBox(const QRectF &viewbox);

    QRectF boundsOnElement(const QString &id) const;
    bool elementExists(const QString &id) const;

public Q_SLOTS:
    bool load(const QString &filename);
    bool load(const QByteArray &contents);
    void render(QPainter *p);
    void render(QPainter *p, const QRectF &bounds);

    void render(QPainter *p, const QString &elementId,
                const QRectF &bounds=QRectF());

private:
    D_DECLARE_PRIVATE(DSvgRenderer)
};

DWIDGET_END_NAMESPACE

#endif // DSVGRENDERER_H

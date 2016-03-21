#ifndef DABSTRACTWIDGET_H
#define DABSTRACTWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <libdui_global.h>

DWIDGET_BEGIN_NAMESPACE

class DX11WidgetPrivate;

class LIBDTKWIDGETSHARED_EXPORT DX11Widget : public QWidget, public DObject
{
    Q_OBJECT
public:
    explicit DX11Widget(DX11Widget *parent = 0);
    explicit DX11Widget(DObjectPrivate &dd, DX11Widget *parent = 0);

    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int shadowWidth READ shadowWidth WRITE setShadowWidth)
    Q_PROPERTY(int titleHeight READ titleHeight WRITE setTitleFixedHeight)
    Q_PROPERTY(QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage)

    Qt::WindowFlags windowFlags();
    void setWindowFlags(Qt::WindowFlags type);

    void setTitle(const QString &);
    void setIcon(const QPixmap &icon);
    int titleHeight() const;
    void setTitleFixedHeight(int h);

    QLayout *layout() const;
    void setLayout(QLayout *);

    int radius() const;
    void setRadius(int r);

    int shadowWidth() const;
    void setShadowWidth(int w);

    const QPixmap &backgroundImage() const;
    void setBackgroundImage(const QPixmap &);

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;

    void setFixedSize(const QSize &);
    void setFixedSize(int w, int h);
//    void setFixedWidth(int w);
//    void setFixedHeight(int h);

    void resize(const QSize &);

    void removeLayout();

    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);

public slots:
    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void showNormal();

protected:
    void setShadow();

private:
    D_DECLARE_PRIVATE(DX11Widget)
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTWIDGET_H

/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DABSTRACTWIDGET_H
#define DABSTRACTWIDGET_H

#include <QWidget>
#include <dobject.h>
#include <dtkwidget_global.h>

class QMenu;

DWIDGET_BEGIN_NAMESPACE

class DBorderlessWidgetPrivate;
class DTitlebar;

class LIBDTKWIDGETSHARED_EXPORT DBorderlessWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    enum DecorationFlag {
        ShowTitlebarSeparator = 0x01
    };
    Q_DECLARE_FLAGS(DecorationFlags, DecorationFlag)
    Q_FLAGS(DecorationFlags)

    explicit DBorderlessWidget(QWidget *parent = 0);

    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int shadowWidth READ shadowWidth WRITE setShadowWidth)
    Q_PROPERTY(int border READ border WRITE setBorder)
    Q_PROPERTY(int titlebarHeight READ titlebarHeight WRITE setTitlebarFixedHeight)
    Q_PROPERTY(QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(QPoint shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged)

    DecorationFlags decorationFlags();
    void setDecorationFlags(DecorationFlags flags);

    Qt::WindowFlags windowFlags();
    void setWindowFlags(Qt::WindowFlags type);

    void setTitle(const QString &);
    void setTitleIcon(const QPixmap &icon);

    DTitlebar *titlebar() const;

    void setTitlebarMenu(QMenu *);
    QMenu *titleBarMenu() const;
    void setTitlebarWidget(QWidget *, bool fixCenterPos = false);
    void setTitlebarWidget(QWidget *w, Qt::AlignmentFlag wflag, bool fixCenterPos = false);
    int titlebarHeight() const;
    void setTitlebarFixedHeight(int h);

    QLayout *layout() const;
    void setLayout(QLayout *);

    int radius() const;
    void setRadius(int r);

    int shadowWidth() const;
    void setShadowWidth(int w);

    int border() const;
    void setBorder(int b);

    const QPixmap &backgroundImage() const;
    void setBackgroundImage(const QPixmap &srcPixmap);

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;

    void setFixedSize(const QSize &);
    void setFixedSize(int w, int h);
    void setFixedWidth(int w);
//    void setFixedHeight(int h);

    void resize(int width, int height);
    void resize(const QSize &);

    void removeLayout();

    void adjustSize();

    void move(int x, int y);
    void move(const QPoint &);

    QRect frameGeometry() const;
    const QRect &geometry() const;
    QRect normalGeometry() const;

    int x() const;
    int y() const;
    QPoint pos() const;
    QSize frameSize() const;
    QSize size() const;
    int width() const;
    int height() const;
    QRect rect() const;
    QRect childrenRect() const;
    QRegion childrenRegion() const;

    QSize minimumSize() const;
    QSize maximumSize() const;
    int minimumWidth() const;
    int minimumHeight() const;
    int maximumWidth() const;
    int maximumHeight() const;
    void setMinimumSize(const QSize &);
    void setMinimumSize(int minw, int minh);
    void setMaximumSize(const QSize &);
    void setMaximumSize(int maxw, int maxh);
    void setMinimumWidth(int minw);
    void setMinimumHeight(int minh);
    void setMaximumWidth(int maxw);
    void setMaximumHeight(int maxh);

    QColor backgroundColor() const;
    QColor shadowColor() const;
    QPoint shadowOffset() const;

protected:
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *)Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *)Q_DECL_OVERRIDE;
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void showNormal();

    void moveWindow(Qt::MouseButton botton);
    void toggleMaximizedWindow();

    void setBackgroundColor(QColor backgroundColor);
    void setShadowColor(QColor shadowColor);
    void setShadowOffset(QPoint shadowOffset);

protected:
    explicit DBorderlessWidget(DBorderlessWidgetPrivate &dd, QWidget *parent = 0);

    virtual void drawShadowPixmap();

Q_SIGNALS:
    void optionClicked();
    void backgroundColorChanged(QColor backgroundColor);
    void shadowColorChanged(QColor shadowColor);
    void shadowOffsetChanged(QPoint shadowOffset);

private:
    D_PRIVATE_SLOT(void _q_onTitleBarMousePressed(Qt::MouseButtons) const)

    D_DECLARE_PRIVATE(DBorderlessWidget)
};

class FilterMouseMove : public QObject
{
    Q_OBJECT

public:
    explicit FilterMouseMove(QObject *object = nullptr);
    ~FilterMouseMove();

    bool eventFilter(QObject *obj, QEvent *event);

//private:
    QWidget *m_rootWidget = nullptr;
};

DWIDGET_END_NAMESPACE

#endif // DABSTRACTWIDGET_H

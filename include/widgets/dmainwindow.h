// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DMAINWINDOW_H
#define DMAINWINDOW_H

#include <dtkwidget_global.h>
#include <DFloatingMessage>
#include <DObject>

#include <QMainWindow>
#include <QPainterPath>

DWIDGET_BEGIN_NAMESPACE

class DMainWindowPrivate;
class DTitlebar;
class LIBDTKWIDGETSHARED_EXPORT DMainWindow : public QMainWindow, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(int windowRadius READ windowRadius WRITE setWindowRadius NOTIFY windowRadiusChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int shadowRadius READ shadowRadius WRITE setShadowRadius NOTIFY shadowRadiusChanged)
    Q_PROPERTY(QPoint shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath NOTIFY clipPathChanged)
    Q_PROPERTY(QRegion frameMask READ frameMask WRITE setFrameMask NOTIFY frameMaskChanged)
    Q_PROPERTY(QMargins frameMargins READ frameMargins NOTIFY frameMarginsChanged)
    Q_PROPERTY(bool translucentBackground READ translucentBackground WRITE setTranslucentBackground NOTIFY translucentBackgroundChanged)
    Q_PROPERTY(bool enableSystemResize READ enableSystemResize WRITE setEnableSystemResize NOTIFY enableSystemResizeChanged)
    Q_PROPERTY(bool enableSystemMove READ enableSystemMove WRITE setEnableSystemMove NOTIFY enableSystemMoveChanged)
    Q_PROPERTY(bool enableBlurWindow READ enableBlurWindow WRITE setEnableBlurWindow NOTIFY enableBlurWindowChanged)
    Q_PROPERTY(bool autoInputMaskByClipPath READ autoInputMaskByClipPath WRITE setAutoInputMaskByClipPath NOTIFY autoInputMaskByClipPathChanged)
    Q_PROPERTY(bool titlebarShadowEnabled READ titlebarShadowIsEnabled WRITE setTitlebarShadowEnabled)

public:
    explicit DMainWindow(QWidget *parent = 0);

    DTitlebar *titlebar() const;

    void setSidebarWidget(QWidget *widget);
    QWidget * sidebarWidget();

    int  sidebarWidth() const;
    void setSidebarWidth(int width);

    D_DECL_DEPRECATED_X("Please use sidebarVisible") bool sidebarVisble() const;
    bool sidebarVisible() const ;
    void setSidebarVisible(bool visible);

    bool sidebarExpanded() const;
    void setSidebarExpanded(bool expended);

    bool isDXcbWindow() const;

    int windowRadius() const;

    int borderWidth() const;
    QColor borderColor() const;

    int shadowRadius() const;
    QPoint shadowOffset() const;
    QColor shadowColor() const;

    QPainterPath clipPath() const;
    QRegion frameMask() const;
    QMargins frameMargins() const;

    bool translucentBackground() const;
    bool enableSystemResize() const;
    bool enableSystemMove() const;
    bool enableBlurWindow() const;
    bool autoInputMaskByClipPath() const;

    bool titlebarShadowIsEnabled() const;

public Q_SLOTS:
    void setWindowRadius(int windowRadius);

    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);

    void setShadowRadius(int shadowRadius);
    void setShadowOffset(const QPoint &shadowOffset);
    void setShadowColor(const QColor &shadowColor);

    void setClipPath(const QPainterPath &clipPath);
    void setFrameMask(const QRegion &frameMask);

    void setTranslucentBackground(bool translucentBackground);
    void setEnableSystemResize(bool enableSystemResize);
    void setEnableSystemMove(bool enableSystemMove);
    void setEnableBlurWindow(bool enableBlurWindow);
    void setAutoInputMaskByClipPath(bool autoInputMaskByClipPath);

    // TODO: remove it if there is an batter sulotion
#ifdef Q_OS_MAC
    void setWindowFlags(Qt::WindowFlags type);
#endif

    void sendMessage(const QIcon &icon, const QString &message);
    void sendMessage(DFloatingMessage *message);

    void setTitlebarShadowEnabled(bool titlebarShadowEnabled);

Q_SIGNALS:
    void windowRadiusChanged();
    void borderWidthChanged();
    void borderColorChanged();
    void shadowRadiusChanged();
    void shadowOffsetChanged();
    void shadowColorChanged();
    void clipPathChanged();
    void frameMaskChanged();
    void frameMarginsChanged();
    void translucentBackgroundChanged();
    void enableSystemResizeChanged();
    void enableSystemMoveChanged();
    void enableBlurWindowChanged();
    void autoInputMaskByClipPathChanged();
    void sidebarVisbleChanged(bool visible);
    void sidebarExpanedChanged(bool expaned);

protected:
    DMainWindow(DMainWindowPrivate &dd, QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    D_DECLARE_PRIVATE(DMainWindow)
    D_PRIVATE_SLOT(void _q_autoShowFeatureDialog())
};

DWIDGET_END_NAMESPACE

#endif // DMAINWINDOW_H

// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include "dtitlebarsettingsimpl.h"

#include <dtkwidget_global.h>
#include <DObject>
#include <DBlurEffectWidget>
#include <DIconButton>

#include <QHBoxLayout>
#include <QPointer>

class QPushButton;
class QLabel;

DWIDGET_BEGIN_NAMESPACE

class DTitlebarEditPanel;
class DFlowLayout;
class DIconButton;

class PlaceHoderWidget: public QWidget
{
    Q_OBJECT
public:
    explicit PlaceHoderWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

class DragDropWidget : public DIconButton
{
    Q_OBJECT
public:
    explicit DragDropWidget(const QString &id = "", QWidget *parent = nullptr);
    virtual~DragDropWidget() Q_DECL_OVERRIDE;

    void setButtonIcon(const QIcon &icon, const QSize &size = QSize(36, 36));
    QString id() const;
    void setScreenShotedView(QWidget *view);
    void screenShot();

protected:
    virtual void onIgnoreAction();
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void startDrag(const QPoint &pos);
    void gobackDrag(const QPixmap &pixmap, const QPoint &pos);

protected:
    QString m_mimeDataFormat;
    DTitlebarEditPanel *m_titleBarEditPanel = nullptr;
    int m_index = -1;
    QPixmap m_pixmap;
    QString m_id; // m_id is key when Type is TITLEBAR_TYPE, m_id is toolId when Type is SELECTZONE_TYPE

private:
    QPoint m_startDrag;
    bool m_isClicked = false;
    QPointer<QWidget> m_view = nullptr;
};

class TitlebarZoneWidget : public DragDropWidget
{
public:
    explicit TitlebarZoneWidget(const QString &id = "", QWidget *parent = nullptr);
    void onIgnoreAction() Q_DECL_OVERRIDE;
};

class SelectionZoneWidget : public DragDropWidget
{
public:
    explicit SelectionZoneWidget(const QString &id = "", QWidget *parent = nullptr);
};

class DefaultZoneWidget : public DragDropWidget
{
public:
    explicit DefaultZoneWidget(const QString &id = "", QWidget *parent = nullptr);
};

class IconTextWidget : public QWidget {
    Q_OBJECT
public:
    explicit IconTextWidget(DragDropWidget *m_iconWidget, const QString &toolId, QWidget *parent = nullptr);
    ~IconTextWidget();

    void setContent(const QIcon &icon, const QString &text, const QSize &size = QSize(36, 36));
    void setIconSize(const QSize &size);

private:
    QString m_toolId;
    DragDropWidget *m_iconWidget;
    QLabel *m_titleLabel;
};

class DCollapseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DCollapseWidget(DTitlebarSettingsImpl *settings, QWidget *parent = nullptr);
    virtual~DCollapseWidget() Q_DECL_OVERRIDE;

    void removeAll();
    void reloadWidgets();
    void removePlaceHolder();
    virtual void addWidget(const QString &key, int index);
    void removeWidget(int index);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void collapse();
    void expand();
    void updateMinimumValue();
    void initExpandButton();

protected:
    DTitlebarSettingsImpl *m_settingsImpl = nullptr;
    QHBoxLayout *m_mainHLayout;
    QVector<QPair<QString, QWidget*>> m_viewsInMenu;
    DIconButton *m_expandButton = nullptr;
    QPointer<QWidget> m_placeHolder = nullptr;

private:
    int m_minimumWidth = 0;
};

class DTitlebarCustomWidget: public DCollapseWidget
{
    Q_OBJECT
public:
    explicit DTitlebarCustomWidget(DTitlebarSettingsImpl *settings, QWidget *parent = nullptr);
    bool editMode() const;
    void setEditMode(bool isEditMode);
    QWidget *widget(const int index) const;
    void addWidget(const QString &key, int index) Q_DECL_OVERRIDE;
    void appendDefaultWidget(const QString &toolId);
    void insertPlaceHolder(int index, const QSize &size);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_isEditMode = false;
};

class DTitlebarEditPanel : public DCollapseWidget
{
    Q_OBJECT
public:
    explicit DTitlebarEditPanel(DTitlebarSettingsImpl *settings, DTitlebarCustomWidget *customWidget, QWidget *parent = nullptr);
    void updateCustomWidget(bool isEditMode = true);
    void updateScreenShotedViews();
    void addWidget(const QString &key, int index) Q_DECL_OVERRIDE;
    bool isFixedTool(const int index);
    bool dropped() const;
    void setDropped(bool isDropped);
    void doStartScreenShot();
    void replaceOldView(const QString &toolId);
    
Q_SIGNALS:
    void addingToolView(const QString &key, const int pos);
    void removedToolView(const QString &key, const int pos);
    void movedToolView(const QString &key, const int pos);
    void resetToolView();
    void startScreenShot();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    bool containsTool(const QString &toolId);
    void handleTitlebarZoneWidgetMoveEvent(QDropEvent *event);
    void handleSelectionZoneWidgetMoveEvent(QDropEvent *event);
    void handleDefaultWidgetDropEvent(QDropEvent *event);
    void handleTitlebarZoneWidgetDropEvent(QDropEvent *event);
    void handleSelectionZoneWidgetDropEvent(QDropEvent *event);
    void positionPlaceHolder(const QPoint &pos, const QPoint &hotSpot, const QSize &size);

private:
    bool m_isDropped = true;
    DTitlebarCustomWidget *m_customWidget;
};

class DToolbarEditPanel : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit DToolbarEditPanel(DTitlebarSettingsImpl *settingsImpl, QWidget *parent = Q_NULLPTR);
    void addWidgetToSelectionZone(const QString &id);
    void setDefaultView(const QPixmap &pixmap, const QSize &size);
    void removeAll();

Q_SIGNALS:
    void confirmBtnClicked();

private Q_SLOTS:
    void onConfirmBtnClicked();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void init();

private:
    DTitlebarSettingsImpl *m_settingsImpl = nullptr;
    QWidget *m_selectZoneView;
    DFlowLayout *m_flowLayout;
    IconTextWidget *m_defaultToolBarWidget;
    QPushButton *m_confirmBtn;
};

DWIDGET_END_NAMESPACE

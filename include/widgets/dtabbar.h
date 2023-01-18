// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTABBAR_H
#define DTABBAR_H

#include <QTabBar>

#include <dtkwidget_global.h>
#include <dobject.h>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DTabBarPrivate;
class DTabBar : public QWidget, public DObject
{
    Q_OBJECT

    Q_PROPERTY(bool visibleAddButton READ visibleAddButton WRITE setVisibleAddButton)
    Q_PROPERTY(QTabBar::Shape shape READ shape WRITE setShape)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(bool drawBase READ drawBase WRITE setDrawBase)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
    Q_PROPERTY(Qt::TextElideMode elideMode READ elideMode WRITE setElideMode)
    Q_PROPERTY(bool usesScrollButtons READ usesScrollButtons WRITE setUsesScrollButtons)
    Q_PROPERTY(bool tabsClosable READ tabsClosable WRITE setTabsClosable)
    Q_PROPERTY(QTabBar::SelectionBehavior selectionBehaviorOnRemove READ selectionBehaviorOnRemove WRITE setSelectionBehaviorOnRemove)
    Q_PROPERTY(bool expanding READ expanding WRITE setExpanding)
    Q_PROPERTY(bool movable READ isMovable WRITE setMovable)
    Q_PROPERTY(bool dragable READ isDragable WRITE setDragable)
    Q_PROPERTY(bool documentMode READ documentMode WRITE setDocumentMode)
    Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide)
    Q_PROPERTY(bool changeCurrentOnDrag READ changeCurrentOnDrag WRITE setChangeCurrentOnDrag)
    Q_PROPERTY(int startDragDistance READ startDragDistance WRITE setStartDragDistance)
    // on drag enter
    Q_PROPERTY(QColor maskColor READ maskColor WRITE setMaskColor)
    // on inserted tab from mime data
    Q_PROPERTY(QColor flashColor READ flashColor WRITE setFlashColor)

public:
    explicit DTabBar(QWidget *parent = 0);

    void setTabMinimumSize(int index, const QSize &size);
    void setTabMaximumSize(int index, const QSize &size);

    bool visibleAddButton() const;

    QTabBar::Shape shape() const;
    void setShape(QTabBar::Shape shape);

    int addTab(const QString &text);
    int addTab(const QIcon &icon, const QString &text);

    int insertTab(int index, const QString &text);
    int insertTab(int index, const QIcon&icon, const QString &text);

    void removeTab(int index);
    void moveTab(int from, int to);

    bool isTabEnabled(int index) const;
    void setTabEnabled(int index, bool);

    QString tabText(int index) const;
    void setTabText(int index, const QString &text);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon &icon);

    Qt::TextElideMode elideMode() const;
    void setElideMode(Qt::TextElideMode mode);

#ifndef QT_NO_TOOLTIP
    void setTabToolTip(int index, const QString &tip);
    QString tabToolTip(int index) const;
#endif

#ifndef QT_NO_WHATSTHIS
    void setTabWhatsThis(int index, const QString &text);
    QString tabWhatsThis(int index) const;
#endif

    void setTabData(int index, const QVariant &data);
    QVariant tabData(int index) const;

    QRect tabRect(int index) const;
    int tabAt(const QPoint &pos) const;

    int currentIndex() const;
    int count() const;

    void setDrawBase(bool drawTheBase);
    bool drawBase() const;

    QSize iconSize() const;
    void setIconSize(const QSize &size);

    bool usesScrollButtons() const;
    void setUsesScrollButtons(bool useButtons);

    bool tabsClosable() const;
    void setTabsClosable(bool closable);

    void setTabButton(int index, QTabBar::ButtonPosition position, QWidget *widget);
    QWidget *tabButton(int index, QTabBar::ButtonPosition position) const;

    QTabBar::SelectionBehavior selectionBehaviorOnRemove() const;
    void setSelectionBehaviorOnRemove(QTabBar::SelectionBehavior behavior);

    bool expanding() const;
    void setExpanding(bool enabled);

    bool isMovable() const;
    void setMovable(bool movable);

    bool isDragable() const;
    void setDragable(bool dragable);

    bool documentMode() const;
    void setDocumentMode(bool set);

    bool autoHide() const;
    void setAutoHide(bool hide);

    bool changeCurrentOnDrag() const;
    void setChangeCurrentOnDrag(bool change);

    int startDragDistance() const;

    QColor maskColor() const;
    QColor flashColor() const;

    QWindow *dragIconWindow() const;

    void setEnabledEmbedStyle(bool enable);

    void setTabLabelAlignment(Qt::Alignment alignment);

Q_SIGNALS:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
    void tabIsInserted(int index);
    void tabIsRemoved(int index);
    void tabBarClicked(int index);
    void tabBarDoubleClicked(int index);
    void tabAddRequested();
    void tabReleaseRequested(int index);
    void tabDroped(int index, Qt::DropAction action, QObject *target);
    void dragActionChanged(Qt::DropAction action);
    void dragStarted();
    void dragEnd(Qt::DropAction action);

public Q_SLOTS:
    void setCurrentIndex(int index);
    void setVisibleAddButton(bool visibleAddButton);
    void setStartDragDistance(int startDragDistance);

    void setMaskColor(QColor maskColor);
    void setFlashColor(QColor flashColor);

    void startDrag(int index);
    void stopDrag(Qt::DropAction action);

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    void startTabFlash(int index);

    virtual void paintTab(QPainter *painter, int index, const QStyleOptionTab &option) const;

    virtual QPixmap createDragPixmapFromTab(int index, const QStyleOptionTab &option, QPoint *hotspot) const;
    virtual QMimeData *createMimeDataFromTab(int index, const QStyleOptionTab &option) const;
    virtual bool canInsertFromMimeData(int index, const QMimeData *source) const;
    virtual void insertFromMimeData(int index, const QMimeData *source);
    virtual void insertFromMimeDataOnDragEnter(int index, const QMimeData *source);

    virtual void tabInserted(int index);
    virtual void tabLayoutChange();
    virtual void tabRemoved(int index);

    virtual QSize tabSizeHint(int index) const;
    virtual QSize minimumTabSizeHint(int index) const;
    virtual QSize maximumTabSizeHint(int index) const;

private:
    DTabBarPrivate* d_func();
    const DTabBarPrivate* d_func() const;
    friend class DTabBarPrivate;
};

DWIDGET_END_NAMESPACE

#endif // DTABBAR_H

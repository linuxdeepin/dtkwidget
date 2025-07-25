// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtitlebareditpanel.h"
#include "dtitlebarsettings.h"

#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPropertyAnimation>
#include <QPainter>
#include <QBitmap>

#include <DIconTheme>
#include <DListView>
#include <DHorizontalLine>
#include <DIconButton>
#include <dflowlayout.h>
#include <DPlatformWindowHandle>
#include "dtitlebar.h"

DWIDGET_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE

#define SPACING 10

static const char* TitlebarZoneDataFormat = "titlebarZoneWidget";
static const char* SelectionZoneDataFormat = "selectionZoneWidget";
static const char* DefaultZoneDataFormat = "defaultZoneWidget";

static QBitmap bitmapOfMask(const QSize &size, const qreal radius)
{
    QBitmap bitMap(size);
    bitMap.fill(Qt::color0);

    QPainter painter(&bitMap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(bitMap.rect(), radius, radius);

    return bitMap;
}

PlaceHoderWidget::PlaceHoderWidget(QWidget *parent)
    : QWidget(parent)
{

}

void PlaceHoderWidget::paintEvent(QPaintEvent *event)
{
    QColor color;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        color = QColor(0, 0, 0, qRound(255 * 0.06));
    } else {
        color = QColor(255, 255, 255, qRound(255 * 0.06));
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);
    QPen pen(color);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRoundedRect(this->rect().adjusted(1, 1, -1, -1), 8.0, 8.0);

    QWidget::paintEvent(event);
}

DragDropWidget::DragDropWidget(const QString &id, QWidget *parent)
    : DIconButton(parent)
    , m_id(id)
{
}

DragDropWidget::~DragDropWidget()
{

}

void DragDropWidget::setButtonIcon(const QIcon &icon, const QSize &size)
{
    this->setIcon(icon);
    this->setIconSize(size);
}

QString DragDropWidget::id() const
{
    return m_id;
}

void DragDropWidget::setScreenShotedView(QWidget *view)
{
    m_view = view;
}

void DragDropWidget::screenShot()
{
    if (m_view && m_view->size().width() > 0) {
        this->setFixedSize(m_view->size());
        auto pixmap = m_view->grab(m_view->rect());
        pixmap.setMask(bitmapOfMask(pixmap.size(), 8));
        this->setButtonIcon(pixmap, m_view->size());
    }
}

void DragDropWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isClicked = true;
    }
}

void DragDropWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isClicked)
        startDrag(event->pos());
}

void DragDropWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_isClicked = false;
}

void DragDropWidget::startDrag(const QPoint &pos)
{
    m_startDrag = mapToGlobal(this->pos());

    QPoint hotSpot = pos;
    QPixmap pixmap(this->grab());
    pixmap.setMask(bitmapOfMask(pixmap.size(), 8));
    m_pixmap = pixmap;
    int index = -1;
    if (DTitlebarEditPanel *panel = qobject_cast<DTitlebarEditPanel *>(this->parentWidget())) {
        index = panel->layout()->indexOf(this);
        m_titleBarEditPanel = panel;
        m_index = index;
        if (panel->isFixedTool(index)) {
            return;
        }
    }

    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << m_id << hotSpot << this->size() << index;

    mimeData->setData(m_mimeDataFormat, itemData);
    
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);
    
    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
    if (dropAction == Qt::IgnoreAction) {
        onIgnoreAction();
    }
}

void DragDropWidget::onIgnoreAction()
{
    gobackDrag(m_pixmap, QCursor::pos());
}

void DragDropWidget::gobackDrag(const QPixmap &pixmap, const QPoint &pos)
{
    QLabel *widget = new QLabel();
    widget->setAttribute(Qt::WA_TranslucentBackground);
    widget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    widget->setFixedSize(pixmap.size());
    widget->setPixmap(pixmap);
    widget->move(pos);
    widget->show();
    
    auto currentXAni = new QPropertyAnimation(widget, "pos");
    const int AnimationTime = 250;
    currentXAni->setEasingCurve(QEasingCurve::OutCubic);
    currentXAni->setDuration(AnimationTime);
    currentXAni->setStartValue(pos);
    currentXAni->setEndValue(m_startDrag);
    currentXAni->setDirection(QAbstractAnimation::Forward);
    currentXAni->start();
    connect(currentXAni, &QPropertyAnimation::finished, currentXAni, &QWidget::deleteLater);
    connect(currentXAni, &QPropertyAnimation::finished, widget, &QWidget::deleteLater);
}

TitlebarZoneWidget::TitlebarZoneWidget(const QString &id, QWidget *parent)
    : DragDropWidget(id, parent)
{
    m_mimeDataFormat = TitlebarZoneDataFormat;
}

void TitlebarZoneWidget::onIgnoreAction()
{
    if (!m_titleBarEditPanel->dropped()) {
        Q_EMIT m_titleBarEditPanel->removedToolView(m_id, m_index);
        m_titleBarEditPanel->removePlaceHolder();
        m_titleBarEditPanel->updateCustomWidget();
        m_titleBarEditPanel->updateScreenShotedViews();
        m_titleBarEditPanel->setDropped(true);
        this->deleteLater();
    }
}

SelectionZoneWidget::SelectionZoneWidget(const QString &id, QWidget *parent)
    : DragDropWidget(id, parent)
{
    m_mimeDataFormat = SelectionZoneDataFormat;
}

DefaultZoneWidget::DefaultZoneWidget(const QString &id, QWidget *parent)
    : DragDropWidget(id, parent)
{
    m_mimeDataFormat = DefaultZoneDataFormat;
}

IconTextWidget::IconTextWidget(DragDropWidget *iconWidget, const QString &toolId, QWidget *parent)
    : QWidget(parent)
    , m_toolId(toolId)
    , m_iconWidget(iconWidget)
    , m_titleLabel(new QLabel)
{
}

IconTextWidget::~IconTextWidget()
{

}

void IconTextWidget::setContent(const QIcon &icon, const QString &text, const QSize &size)
{
    m_iconWidget->setFixedSize(size);
    m_iconWidget->setButtonIcon(icon, size);
    m_titleLabel->setText(text);
    m_titleLabel->setAlignment(Qt::AlignHCenter);
    DFontSizeManager *fontManager = DFontSizeManager::instance();
    fontManager->bind(m_titleLabel, DFontSizeManager::T10, QFont::Medium);

    if (layout())
        return;

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->setSpacing(0);
    mainVLayout->setContentsMargins(0, 0, 0, 0);
    mainVLayout->addWidget(m_iconWidget, 0, Qt::AlignHCenter);
    mainVLayout->addSpacing(6);
    mainVLayout->addWidget(m_titleLabel, 0, Qt::AlignHCenter);
}

void IconTextWidget::setIconSize(const QSize &size)
{
    m_iconWidget->setFixedSize(size);
}

DCollapseWidget::DCollapseWidget(DTitlebarSettingsImpl *settings, QWidget *parent)
    : QWidget(parent)
    , m_settingsImpl(settings)
    , m_mainHLayout(new QHBoxLayout(this))
    , m_placeHolder(new PlaceHoderWidget)
{
    m_placeHolder->setObjectName("placeHolder");
    m_mainHLayout->setSpacing(SPACING);
}

DCollapseWidget::~DCollapseWidget()
{

}

void DCollapseWidget::removeAll()
{
    QLayoutItem *item = nullptr;
    while ((item = m_mainHLayout->takeAt(0)) != nullptr) {
        if (auto w = item->widget()) {
            if (w->objectName() != "placeHolder") {
                delete item->widget();
                delete item;
            }
        }
    }
    removePlaceHolder();
}

void DCollapseWidget::removePlaceHolder()
{
    if (m_placeHolder && m_placeHolder->isVisible()) {
        m_mainHLayout->removeWidget(m_placeHolder);
        m_placeHolder->hide();
    }
}

void DCollapseWidget::reloadWidgets()
{
    removeAll();
    for (auto key : m_settingsImpl->keys()) {
        addWidget(key, -1);
    }
}

void DCollapseWidget::addWidget(const QString &key, int index)
{
    Q_UNUSED(key)
    Q_UNUSED(index)
}

void DCollapseWidget::removeWidget(int index)
{
    if (auto item = m_mainHLayout->takeAt(index)) {
        if (auto w = item->widget()) {
            w->hide();
        }
    }
}

void DCollapseWidget::resizeEvent(QResizeEvent *event)
{
    updateMinimumValue();

    if (width() < m_minimumWidth) {
        collapse();
    } else {
        expand();
    }

    QWidget::resizeEvent(event);
}

void DCollapseWidget::updateMinimumValue()
{
    int minimum = 0;

    for (int i = 0; i < m_mainHLayout->count(); ++i) {
        auto item = m_mainHLayout->itemAt(i);
        if (auto spacerItem = item->spacerItem()) {
            if (spacerItem->sizePolicy().horizontalPolicy() == QSizePolicy::Fixed) {
                auto baseInterface = m_settingsImpl->tool(m_settingsImpl->findKeyByPos(i));
                if (auto spacerInterface = qobject_cast<DTitleBarSpacerInterface *>(baseInterface)) {
                    minimum += spacerInterface->size() + SPACING;
                    qDebug() << "+" << spacerInterface->size() + SPACING;
                }
            }
        } else {
            auto w = item->widget();
            if (!w || w->sizePolicy().horizontalPolicy() == QSizePolicy::Expanding)
                continue;
            minimum += w->width();
            qDebug() << "+" << w->width();
            if (auto dragDropWidget = qobject_cast<DragDropWidget *>(w)) {
               if (m_settingsImpl->isSpacerTool(m_settingsImpl->findKeyByPos(i)) && !m_settingsImpl->isStrecherTool(m_settingsImpl->findKeyByPos(i))) {
                   minimum += SPACING;
                   qDebug() << "+" << SPACING;
               }
            }
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    minimum += 2 * m_mainHLayout->margin();
#else
    minimum += m_mainHLayout->contentsMargins().right() + m_mainHLayout->contentsMargins().left();
#endif
    m_minimumWidth = minimum;
}

void DCollapseWidget::initExpandButton()
{
    m_expandButton = new DIconButton;
    m_expandButton->setObjectName("expandButton");
    m_expandButton->setFixedSize(36, 36);
    m_expandButton->setIconSize(QSize(36, 36));
    m_expandButton->setIcon(DIconTheme::findQIcon("fold"));
    m_expandButton->setFlat(false);
    m_mainHLayout->insertWidget(m_mainHLayout->count(), m_expandButton);
    connect(m_expandButton, &QPushButton::clicked, this, [this] {
        QMenu menu(m_expandButton);
        for (auto view : m_viewsInMenu) {
            DTitlebarToolBaseInterface* i = m_settingsImpl->tool(view.first);
            auto interface = qobject_cast<DTitleBarToolInterface *>(i);
            if (!interface) continue;
            QAction *action = new QAction(interface->description());
            connect(action, &QAction::triggered, interface, &DTitleBarToolInterface::triggered);
            menu.addAction(action);
        }
        menu.move(this->mapToGlobal(m_expandButton->pos()).x(), this->mapToGlobal(m_expandButton->pos()).y() + m_expandButton->height());
        menu.exec();
    });
}

void DCollapseWidget::collapse()
{
    if (m_mainHLayout->count() == 0)
        return;

    int index = m_mainHLayout->count() - 1;
    if (m_expandButton && m_expandButton->isVisible()) {
        --index;
    }

    if (auto item = m_mainHLayout->takeAt(index)) {
        if (auto spacerItem = item->spacerItem()) { // 如果是spacer，只存数据，不处理expand按钮
            QPair<QString, QWidget*> tmp{m_settingsImpl->findKeyByPos(index), nullptr};
            m_viewsInMenu.append(tmp);
            qDebug() << "collapse:" << m_viewsInMenu;
            return;
        } else {
            if (auto w = item->widget()) {
                w->hide();
                QPair<QString, QWidget*> tmp{m_settingsImpl->findKeyByPos(index), w};
                m_viewsInMenu.append(tmp);
            }
        }
        qDebug() << "collapse:" << m_viewsInMenu;
    }

    if (!m_expandButton) {
        initExpandButton();
    }

    if (!m_expandButton->isVisible() && m_mainHLayout->indexOf(m_expandButton) == -1) {
        m_mainHLayout->insertWidget(m_mainHLayout->count(), m_expandButton);
        m_expandButton->show();
    }
}

void DCollapseWidget::expand()
{
    if (m_viewsInMenu.isEmpty())
        return;
    auto view = m_viewsInMenu.constLast();
    if (!view.second) {
        if (this->width() >= m_minimumWidth + SPACING) {
            m_viewsInMenu.takeLast();
            int index = m_mainHLayout->indexOf(m_expandButton);
            if (m_settingsImpl->isStrecherTool(view.first)) {
                m_mainHLayout->insertStretch(index, 0);
            } else {
                auto tool = m_settingsImpl->tool(view.first);
                if (auto spacerInter = qobject_cast<DTitleBarSpacerInterface *>(tool)) {
                    auto spacingSize = spacerInter->size();
                    m_mainHLayout->insertSpacing(index, spacingSize);
                }
            }
        }
    } else {
        if (this->width() >= m_minimumWidth + view.second->width() + SPACING) {
            qDebug() << "expand" << m_viewsInMenu.count();
            auto w = m_viewsInMenu.takeLast();
            int index = m_mainHLayout->indexOf(m_expandButton);
            m_mainHLayout->insertWidget(index, view.second);
            view.second->show();
        }
    }
    qDebug() << "expand:" << m_viewsInMenu;

    if (m_viewsInMenu.isEmpty()) {
        m_mainHLayout->removeWidget(m_expandButton);
        m_expandButton->hide();
    }
}

DTitlebarCustomWidget::DTitlebarCustomWidget(DTitlebarSettingsImpl *settings, QWidget *parent)
    : DCollapseWidget(settings, parent)
{
}

bool DTitlebarCustomWidget::editMode() const
{
    return m_isEditMode;
}

void DTitlebarCustomWidget::setEditMode(bool isEditMode)
{
    m_isEditMode = isEditMode;
}

QWidget *DTitlebarCustomWidget::widget(const int index) const
{
    if (auto item = m_mainHLayout->itemAt(index))
        return item->widget();
    return nullptr;
}

void DTitlebarCustomWidget::addWidget(const QString &key, int index)
{
    auto tool = m_settingsImpl->tool(key);
    if (!tool) {
        return;
    }
    const bool isSpacer = DTitlebarSettingsImpl::isSpacerTool(tool);
    if (isSpacer) {
        auto spacerInterface = qobject_cast<DTitleBarSpacerInterface *>(tool);
        if (!spacerInterface) {
            return;
        }
        if (m_isEditMode) {
            auto view = spacerInterface->createPlaceholderView();
            m_mainHLayout->insertWidget(index, view);
        } else {
            const auto spacingSize = spacerInterface->size();
            if (spacingSize < 0) {
                m_mainHLayout->insertStretch(index, 0);
            } else {
                m_mainHLayout->insertSpacing(index, spacingSize + SPACING);
            }
        }
    } else {
        auto toolInterface = qobject_cast<DTitleBarToolInterface *>(tool);
        if (!toolInterface) {
            return;
        }
        QWidget *view = toolInterface->createView();
        m_mainHLayout->insertWidget(index, view);
    }
}

void DTitlebarCustomWidget::appendDefaultWidget(const QString &toolId)
{
    auto tool = m_settingsImpl->toolById(toolId);
    if (!tool)
        return;

    const bool isSpacer = DTitlebarSettingsImpl::isSpacerTool(tool);
    if (isSpacer) {
        auto spacerInterface = qobject_cast<DTitleBarSpacerInterface *>(tool);
        if (!spacerInterface) {
            return;
        }
        auto spacingSize = spacerInterface->size();
        if (spacingSize < 0) {
            m_mainHLayout->insertStretch(-1, 1);
        } else {
            m_mainHLayout->insertSpacing(-1, spacingSize + SPACING);
        }
    } else {
        auto toolInterface = qobject_cast<DTitleBarToolInterface *>(tool);
        if (!toolInterface) {
            return;
        }
        QWidget *view = toolInterface->createView();
        m_mainHLayout->insertWidget(-1, view);
    }
}

void DTitlebarCustomWidget::insertPlaceHolder(int index, const QSize &size)
{
    m_placeHolder->setFixedSize(size);
    m_mainHLayout->insertWidget(index, m_placeHolder);
    m_placeHolder->show();
}

void DTitlebarCustomWidget::resizeEvent(QResizeEvent *event)
{
    if (event->size() != event->oldSize() && m_isEditMode)
        m_settingsImpl->adjustDisplayView();
}

DTitlebarEditPanel::DTitlebarEditPanel(DTitlebarSettingsImpl *settings, DTitlebarCustomWidget *customWidget, QWidget *parent)
    : DCollapseWidget(settings, parent)
    , m_customWidget(customWidget)
{
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    connect(this, &DTitlebarEditPanel::startScreenShot, this, &DTitlebarEditPanel::doStartScreenShot, Qt::QueuedConnection);
}

void DTitlebarEditPanel::updateCustomWidget(bool isEditMode)
{
    m_customWidget->setEditMode(isEditMode);
    m_customWidget->reloadWidgets();
}

void DTitlebarEditPanel::updateScreenShotedViews()
{
    for (int i = 0; i < m_mainHLayout->count(); ++i) {
        auto w = m_mainHLayout->itemAt(i)->widget();
        auto btn = qobject_cast<DragDropWidget*> (w);
        if (btn) {
            btn->setScreenShotedView(m_customWidget->widget(i));
        }
    }
    Q_EMIT startScreenShot();
}

void DTitlebarEditPanel::addWidget(const QString &key, int index)
{
    DragDropWidget *w = new TitlebarZoneWidget(key);
    if (m_settingsImpl->isSpacerTool(key)) {
        auto spacerInterface = qobject_cast<DTitleBarSpacerInterface *>(m_settingsImpl->tool(key));
        if (!spacerInterface) {
            return;
        }
        if (spacerInterface->size() == -1) {
            w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        } else {
            qDebug() << "size" << spacerInterface->size();
            w->setFixedWidth(spacerInterface->size());
        }
    }
    m_mainHLayout->insertWidget(index, w);
}

bool DTitlebarEditPanel::isFixedTool(const int index)
{
    return m_settingsImpl->isFixedTool(index);
}

bool DTitlebarEditPanel::dropped() const
{
    return m_isDropped;
}

void DTitlebarEditPanel::setDropped(bool isDropped)
{
    m_isDropped = isDropped;
}

void DTitlebarEditPanel::doStartScreenShot()
{
    for (int i = 0; i < m_mainHLayout->count(); ++i) {
        auto w = m_mainHLayout->itemAt(i)->widget();
        auto btn = qobject_cast<DragDropWidget*> (w);
        if (btn) {
            btn->screenShot();
        }
    }
}

void DTitlebarEditPanel::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
    if (event->mimeData()->hasFormat(TitlebarZoneDataFormat)) {
        if (!m_isDropped) {
            return;
        }
        QByteArray itemData = event->mimeData()->data(TitlebarZoneDataFormat);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QString key;
        QPoint hotSpot;
        QSize size;
        int index;
        dataStream >> key >> hotSpot >> size >> index;
        if (auto item = m_mainHLayout->takeAt(index)) {
            if (auto w = qobject_cast<DragDropWidget*>(item->widget())) {
                w->hide();
                m_customWidget->removeWidget(index);
                m_isDropped = false;
                Q_EMIT startScreenShot();
            }
        }
    }
}

void DTitlebarEditPanel::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(TitlebarZoneDataFormat)) {
        handleTitlebarZoneWidgetMoveEvent(event);
    } else if (event->mimeData()->hasFormat(SelectionZoneDataFormat)) {
        handleSelectionZoneWidgetMoveEvent(event);
    } else {
        event->accept(); // default zone
    }
}

void DTitlebarEditPanel::dragLeaveEvent(QDragLeaveEvent *event)
{
    removePlaceHolder();
    m_customWidget->removePlaceHolder();
    QWidget::dragLeaveEvent(event);
}

void DTitlebarEditPanel::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(DefaultZoneDataFormat)) {
        handleDefaultWidgetDropEvent(event);
    } else if (event->mimeData()->hasFormat(TitlebarZoneDataFormat)) {
        handleTitlebarZoneWidgetDropEvent(event);
    } else {
        handleSelectionZoneWidgetDropEvent(event);
    }
}

bool DTitlebarEditPanel::containsTool(const QString &toolId)
{
    for (int i = 0; i < m_mainHLayout->count(); ++i) {
        auto w = m_mainHLayout->itemAt(i)->widget();
        auto view = qobject_cast<DragDropWidget *>(w);
        if (view && m_settingsImpl->toolId(view->id()) == toolId) {
            return true;
        }
    }
    return false;
}

void DTitlebarEditPanel::replaceOldView(const QString &toolId)
{
    for (int i = 0; i < m_mainHLayout->count(); ++i) {
        auto w = m_mainHLayout->itemAt(i)->widget();
        auto view = qobject_cast<DragDropWidget *>(w);
        if (view && m_settingsImpl->toolId(view->id()) == toolId) {
            m_mainHLayout->takeAt(i);
            int index = m_mainHLayout->indexOf(m_placeHolder);
            m_mainHLayout->insertWidget(index, w);
            removePlaceHolder();
            Q_EMIT movedToolView(view->id(), index);
            updateCustomWidget();
            updateScreenShotedViews();
            break;
        }
    }
}

void DTitlebarEditPanel::handleTitlebarZoneWidgetMoveEvent(QDropEvent *event)
{
    removePlaceHolder();
    m_customWidget->removePlaceHolder();

    QByteArray itemData = event->mimeData()->data(TitlebarZoneDataFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString key;
    QPoint hotSpot;
    QSize size;
    dataStream >> key >> hotSpot >> size;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    positionPlaceHolder(event->position().toPoint(), hotSpot, size);
#else
    positionPlaceHolder(event->pos(), hotSpot, size);
#endif
    Q_EMIT startScreenShot();
}

void DTitlebarEditPanel::handleSelectionZoneWidgetMoveEvent(QDropEvent *event)
{
    removePlaceHolder();
    m_customWidget->removePlaceHolder();

    QByteArray itemData = event->mimeData()->data(SelectionZoneDataFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString key;
    QPoint hotSpot;
    QSize size;
    dataStream >> key >> hotSpot >> size;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    positionPlaceHolder(event->position().toPoint(), hotSpot, size);
#else
    positionPlaceHolder(event->pos(), hotSpot, size);
#endif
    Q_EMIT startScreenShot();
}

void DTitlebarEditPanel::handleDefaultWidgetDropEvent(QDropEvent *event)
{
    Q_EMIT resetToolView();
    updateCustomWidget();
    reloadWidgets();
    updateScreenShotedViews();
    event->accept();
}

void DTitlebarEditPanel::handleTitlebarZoneWidgetDropEvent(QDropEvent *event)
{
    QByteArray itemData = event->mimeData()->data(TitlebarZoneDataFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString id;
    QPoint hotSpot;
    QSize size;
    int type;
    int index;
    dataStream >> id >> hotSpot >> size >> type >> index;
    if (m_mainHLayout->indexOf(m_placeHolder) != -1) { // 调整位置
        auto w = qobject_cast<QWidget*> (event->source());
        if (m_settingsImpl->isFixedTool(id)) {
            event->ignore();
            return;
        }
        if (w) {
            m_mainHLayout->replaceWidget(m_placeHolder, w);
            auto tool = m_settingsImpl->tool(id);
            const bool isSpacer = DTitlebarSettingsImpl::isSpacerTool(tool);
            if (isSpacer && qobject_cast<DTitleBarSpacerInterface *>(tool)->size() == -1) {
                m_mainHLayout->setStretchFactor(w, 1);
            }
            m_placeHolder->hide();
            w->show();
            Q_EMIT movedToolView(id, m_mainHLayout->indexOf(w));
            updateCustomWidget();
            updateScreenShotedViews();
            m_isDropped = true;
            event->accept();
        }
    }
}

void DTitlebarEditPanel::handleSelectionZoneWidgetDropEvent(QDropEvent *event)
{
    QByteArray itemData = event->mimeData()->data(SelectionZoneDataFormat);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    QString id;
    dataStream >> id;

    if (!m_settingsImpl->isSpacerToolById(id) && containsTool(id)) {
        replaceOldView(id);
    } else {
        int index = m_mainHLayout->indexOf(m_placeHolder);
        Q_EMIT addingToolView(id, index);
        updateCustomWidget();
        this->addWidget(m_settingsImpl->findKeyByPos(index), index);
        removePlaceHolder();
        updateScreenShotedViews();
    }
    event->accept();
}

void DTitlebarEditPanel::positionPlaceHolder(const QPoint &pos, const QPoint &hotSpot, const QSize &size)
{
    int newIndex = -1;
    QWidget *child = childAt(pos);
    if (!child) {
        for (int i = 0; i < m_mainHLayout->count(); ++i) {
            auto w = m_mainHLayout->itemAt(i)->widget();
            if (pos.x() < w->pos().x()) {
                if (qobject_cast<DragDropWidget*>(w)) {
                    newIndex = i;
                    break;
                }
            }
        }
    } else if (qobject_cast<DragDropWidget*>(child)) {
        newIndex = m_mainHLayout->indexOf(child);
        if (pos.x() - hotSpot.x() + size.width() / 2 > child->pos().x() + (child->width() / 2)) {
            ++newIndex;
        }
    } else if (qobject_cast<PlaceHoderWidget*>(child)) {
        return;
    } else { // strech
        newIndex = m_mainHLayout->count();
    }

    if (newIndex == -1) {
        newIndex = m_mainHLayout->count();
    }
    if (newIndex != -1) {
        m_mainHLayout->insertWidget(newIndex, m_placeHolder);
        m_customWidget->insertPlaceHolder(newIndex, size);
        m_placeHolder->setFixedSize(size);
        m_placeHolder->show();
    }
}

bool DTitlebarEditPanel::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}

void DTitlebarEditPanel::resizeEvent(QResizeEvent *event)
{
    if (event->size() != event->oldSize())
        Q_EMIT startScreenShot();
}

DToolbarEditPanel::DToolbarEditPanel(DTitlebarSettingsImpl *settingsImpl, QWidget *parent)
    : DBlurEffectWidget(parent),
      m_settingsImpl(settingsImpl),
      m_selectZoneView(new QWidget),
      m_flowLayout(new DFlowLayout(m_selectZoneView)),
      m_defaultToolBarWidget(new IconTextWidget(new DefaultZoneWidget, "default")),
      m_confirmBtn(new QPushButton)
{
    init();
}

void DToolbarEditPanel::addWidgetToSelectionZone(const QString &id)
{
    auto tool = m_settingsImpl->toolById(id);
    Q_ASSERT(tool);

    IconTextWidget * customWidget = new IconTextWidget(new SelectionZoneWidget(id), id, m_selectZoneView);
    customWidget->setContent(DIconTheme::findQIcon(tool->iconName()), tool->description());
    m_flowLayout->addWidget(customWidget);
}

void DToolbarEditPanel::setDefaultView(const QPixmap &pixmap, const QSize &size)
{
    m_defaultToolBarWidget->setContent(QIcon(pixmap), tr("Default toolset"), size);
    m_defaultToolBarWidget->setIconSize(size);
}

void DToolbarEditPanel::removeAll()
{
    QLayoutItem *item = nullptr;
    while ((item = m_flowLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void DToolbarEditPanel::onConfirmBtnClicked()
{
    Q_EMIT confirmBtnClicked();
    this->close();
}

void DToolbarEditPanel::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        onConfirmBtnClicked();
    }
    DBlurEffectWidget::keyPressEvent(event);
}

void DToolbarEditPanel::init()
{
    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    
    QLabel *selectZoneToolTipLabel = new QLabel(tr("Drag your favorite items into the toolbar"));
    QLabel *defaultZoneToolTipLabel = new QLabel(tr("Drag below items into the toolbar to restore defaults"));
    m_selectZoneView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_flowLayout->setSpacing(30);

    m_confirmBtn->setFixedSize(126, 36);
    m_confirmBtn->setText(tr("Confirm"));
    
    mainVLayout->setSpacing(0);
    mainVLayout->setContentsMargins(10, 0, 10, 0);
    mainVLayout->addSpacing(21);
    mainVLayout->addWidget(selectZoneToolTipLabel, 0, Qt::AlignCenter);
    mainVLayout->addSpacing(12);
    mainVLayout->addWidget(m_selectZoneView);
    mainVLayout->addSpacing(20);
    mainVLayout->addWidget(new DHorizontalLine);
    mainVLayout->addSpacing(12);
    mainVLayout->addWidget(defaultZoneToolTipLabel, 0, Qt::AlignCenter);
    mainVLayout->addSpacing(12);
    mainVLayout->addWidget(m_defaultToolBarWidget, 0, Qt::AlignLeft);
    mainVLayout->addSpacing(10);
    mainVLayout->addWidget(new DHorizontalLine);
    mainVLayout->addSpacing(10);
    mainVLayout->addWidget(m_confirmBtn, 0, Qt::AlignRight);
    mainVLayout->addSpacing(10);

    setMouseTracking(true);

    connect(m_confirmBtn, &QPushButton::clicked, this, &DToolbarEditPanel::onConfirmBtnClicked);
}

DWIDGET_END_NAMESPACE

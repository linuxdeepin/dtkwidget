// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtitlebarsettingsimpl.h"
#include "ddialog.h"
#include "dtitlebareditpanel.h"
#include "diconbutton.h"

#include <QDebug>
#include <QPointer>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QWidget>
#include <QUuid>

#include <DObjectPrivate>
#include "dflowlayout.h"
#include "dtitlebar.h"

DWIDGET_BEGIN_NAMESPACE
DCORE_USE_NAMESPACE

static const QString SettingsTools(u8"tools");
static const QString SettingsAlignment(u8"alignment");
static const QString SettingsKey(u8"key");
static const QString SettingsFixed(u8"fixed");
static const QString SettingsCount(u8"count");
static const QString SettingsSpacingSize(u8"spacingSize");
static const QString SettingsSpacerId(u8"builtin/spacer");
static const QString SettingsStretchId(u8"builtin/stretch");

DTitlebarDataStore::DTitlebarDataStore(QObject *parent)
    : QObject(parent)
    , m_settingsGroupName("dtitlebar-settings")
    , m_settingsGroupNameSubGroup(QString("%1/%2").arg(m_settingsGroupName))
{
}

DTitlebarDataStore::~DTitlebarDataStore()
{
    save();
    qDeleteAll(m_instances);
}

DTitlebarDataStore *DTitlebarDataStore::instance()
{
    static DTitlebarDataStore *dataStore = nullptr;
    if (!dataStore) {
        dataStore = new DTitlebarDataStore;
    }
    return dataStore;
}

bool DTitlebarDataStore::load()
{
    const auto root = metaRoot();
    if (root.isEmpty())
        return false;

    m_isValid = true;

    if (root.contains(SettingsSpacingSize)) {
        m_spacingSize = root[SettingsSpacingSize].toInt();
    }

    const auto cachePos = positionsFromCache();
    if (!cachePos.isEmpty()) {
        for (auto item: cachePos) {
            const auto data = item.toMap();
            const auto &id = data["toolId"].toString();
            const auto &key = data["key"].toString();
            const auto &fixed = data["fixed"].toBool();
            const auto instance = createInstance(id, key);
            instance->isFixed = fixed;
            m_instances << instance;
        }
    } else {
        const auto metaPos = toolInstancesFromToolMeta(root);
        for (int i = 0; i < metaPos.count(); i++) {
            const auto item = metaPos[i];
            const auto instance = createInstance(item.toolId);
            instance->isFixed = item.isFixed;
            m_instances << instance;
        }
    }
    return true;
}

ToolInstance *DTitlebarDataStore::createInstance(const QString &id)
{
    return createInstance(id, QUuid::createUuid().toString());
}

ToolInstance *DTitlebarDataStore::createInstance(const QString &id, const QString &key)
{
    auto instance = new ToolInstance();
    instance->key = key;
    instance->toolId = id;
    return instance;
}

ToolInstance *DTitlebarDataStore::getInstance(const QString &key) const
{
    if (isInvalid())
        return nullptr;
    for (int i = 0; i < m_instances.count(); i++) {
        if (m_instances[i]->key == key)
            return m_instances[i];
    }
    return nullptr;
}

bool DTitlebarDataStore::load(const QString &path)
{
    m_filePath = path;
    return load();
}

void DTitlebarDataStore::save()
{
    if (m_isValid) {
        savePositionsToCache();
    }
}

void DTitlebarDataStore::clear()
{
    clearCache();
    qDeleteAll(m_instances);
    m_instances.clear();
}

void DTitlebarDataStore::reset()
{
    clear();
    load();
}

bool DTitlebarDataStore::isValid() const
{
    return m_isValid;
}

QString DTitlebarDataStore::findKeyByPos(const int pos) const
{
    if (isInvalid())
        return QString();

    if (m_instances.count() <= pos || pos < 0)
        return QString();

    return m_instances[pos]->key;
}

QStringList DTitlebarDataStore::defaultIds() const
{
    return positionsFromToolMeta();
}

QStringList DTitlebarDataStore::keys() const
{
    if (isInvalid())
        return QStringList();
    QStringList positions;
    for (auto item: m_instances) {
        positions << item->key;
    }
    return positions;
}

QString DTitlebarDataStore::key(const int pos)
{
    if (isInvalid())
        return QString();

    if (m_instances.count() <= pos || pos < 0)
        return QString();

    return m_instances[pos]->key;
}

QStringList DTitlebarDataStore::toolIds() const
{
    QStringList positions;
    for (auto item: m_instances) {
        positions << item->toolId;
    }
    return positions;
}

QString DTitlebarDataStore::toolId(const QString &key) const
{
    for (int i = 0; i < m_instances.count(); i++) {
        if (m_instances[i]->key == key)
            return m_instances[i]->toolId;
    }
    return QString();
}

void DTitlebarDataStore::removeAllNotExistIds(const QStringList &ids)
{
    for (int i = m_instances.count() - 1; i >= 0; i--) {
        auto instance = m_instances[i];
        if (ids.contains(instance->toolId))
            continue;

        qDebug() << QString("Don't exit the id for %1.").arg(instance->toolId);
        m_instances.remove(i);
        delete instance;
    }
}

int DTitlebarDataStore::position(const QString &key) const
{
    const auto instance = getInstance(key);
    if (!instance)
        return -1;

    return m_instances.indexOf(instance);
}

bool DTitlebarDataStore::contains(const QString &key) const
{
    return getInstance(key);
}

bool DTitlebarDataStore::isExistTheId(const QString &id) const
{
    if (isInvalid())
        return false;
    for (const auto item: m_instances) {
        if (item->toolId == id)
            return true;
    }
    return false;
}

int DTitlebarDataStore::spacingSize() const
{
    return m_spacingSize;
}

QString DTitlebarDataStore::insert(const QString &id, const int pos)
{
    if (isInvalid())
        return QString();

    const int index = pos == -1 ? m_instances.count() : pos;

    const auto instance = createInstance(id);
    m_instances.insert(index, instance);
    return instance->key;
}

void DTitlebarDataStore::remove(const QString &key)
{
    if (!contains(key))
        return;

    remove(position(key));
}

void DTitlebarDataStore::remove(const int pos)
{
    if (isInvalid())
        return;

    if (pos < 0 || pos >= m_instances.count())
        return;

    auto instance = m_instances.takeAt(pos);
    delete instance;
}

bool DTitlebarDataStore::isFixed(const QString &key) const
{
    if (auto instance = getInstance(key)) {
        return instance->isFixed;
    }
    return false;
}

bool DTitlebarDataStore::isFixed(const int pos) const
{
    if (pos < 0 || pos >= m_instances.count())
        return false;

    return m_instances[pos]->isFixed;
}

bool DTitlebarDataStore::isInvalid() const
{
    if (!m_isValid)
        qWarning() << "TitleBarDataStore is invalid.";
    return !m_isValid;
}

QStringList DTitlebarDataStore::positionsFromToolMeta(const QJsonObject &root) const
{
    QStringList metaPos;
    for (auto item : toolInstancesFromToolMeta(root)) {
        metaPos << item.toolId;
    }

    return metaPos;
}

QList<ToolInstance> DTitlebarDataStore::toolInstancesFromToolMeta(const QJsonObject &root) const
{
    QList<ToolInstance> results;
    const auto &tools = root[SettingsTools].toArray();
    for (int i = 0; i < tools.size(); i++) {
        const auto item = tools[i];
        const auto id = item[SettingsKey].toString();
        int count = acceptCountField(id) ? countFromToolMeta(root, i) : 1;

        for (int j = 0; j < count; j++) {
            ToolInstance ins;
            ins.toolId = id;
            ins.isFixed = fixedFromToolMeta(root, i);
            results << ins;
        }
    }

    ToolInstance stretchInstance;
    stretchInstance.toolId = SettingsStretchId;
    stretchInstance.isFixed = true;
    const QString &aligment = alignmentFromToolMeta(root);
    if (aligment == "right") {
        results.prepend(stretchInstance);
    } else {
        results << stretchInstance;
    }

    return results;
}

bool DTitlebarDataStore::fixedFromToolMeta(const QJsonObject &root, const int index) const
{
    const auto &tools = root[SettingsTools].toArray();
    if (index < 0 || index >= tools.count())
        return false;

    const QJsonObject &item = tools[index].toObject();
    if (!item.contains(SettingsFixed))
        return false;

    return item[SettingsFixed].toBool();
}

int DTitlebarDataStore::countFromToolMeta(const QJsonObject &root, const int index) const
{
    const auto &tools = root[SettingsTools].toArray();
    if (index < 0 || index >= tools.count())
        return 0;

    const QJsonObject &item = tools[index].toObject();
    if (!item.contains(SettingsCount))
        return 1;

    return item[SettingsCount].toInt();
}

QString DTitlebarDataStore::alignmentFromToolMeta(const QJsonObject &root) const
{
    if (!root.contains(SettingsAlignment))
        return "left";
    return root[SettingsAlignment].toString();
}

QStringList DTitlebarDataStore::positionsFromToolMeta() const
{
    const auto root = metaRoot();
    return positionsFromToolMeta(root);
}

QJsonObject DTitlebarDataStore::metaRoot() const
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed on open file: \"%s\", error message: \"%s\"",
                 qPrintable(file.fileName()), qPrintable(file.errorString()));
        return QJsonObject();
    }

    QJsonParseError error;
    auto document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning("Failed on parse file: %s", qPrintable(error.errorString()));
        return QJsonObject();
    }

    return document.object();
}

QVariantList DTitlebarDataStore::positionsFromCache()
{
    QVariantList positions;
    QSettings settings;
    const int size = settings.beginReadArray(m_settingsGroupNameSubGroup.arg("positions"));
    for (int  i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        QVariantMap data;
        data["key"] = settings.value("key");
        data["toolId"] = settings.value("toolId");
        data["fixed"] = settings.value("fixed");
        positions << data;
    }
    settings.endArray();
    return positions;
}

void DTitlebarDataStore::savePositionsToCache()
{
    QSettings settings;
    settings.beginWriteArray(m_settingsGroupNameSubGroup.arg("positions"));
    for (int i = 0; i < m_instances.size(); i++) {
        const auto item = m_instances[i];
        settings.setArrayIndex(i);
        settings.setValue("key", item->key);
        settings.setValue("toolId", item->toolId);
        settings.setValue("fixed", item->isFixed);
    }
    settings.endArray();
}

void DTitlebarDataStore::clearCache()
{
    QSettings settings;
    settings.beginGroup(m_settingsGroupName);
    settings.remove("");
    settings.endGroup();
}

bool DTitlebarDataStore::acceptCountField(const QString &id) const
{
    const  QStringList countToolIds {
        SettingsSpacerId
    };
    return countToolIds.contains(id);
}

void DTitlebarDataStore::move(const QString &key, const int pos)
{
    if (isInvalid())
        return;

    if (!contains(key))
        return;

    m_instances.move(position(key), pos);
}

QString DTitlebarDataStore::add(const QString &id)
{
    return insert(id, -1);
}

DTitlebarToolFactory::DTitlebarToolFactory(QObject *parent)
    : QObject(parent)
{
}

DTitlebarToolFactory::~DTitlebarToolFactory()
{
    m_tools.clear();
}

void DTitlebarToolFactory::add(DTitlebarToolBaseInterface *tool)
{
    bool exist = false;
    for (const auto &item : std::as_const(m_tools)) {
        if (item.tool->id() == tool->id()) {
            exist =  true;
            break;
        }
    }
    if (exist) {
        qWarning() << "The tool already exist in factory, tool key: " << tool->id();
        return;
    }
    m_tools[tool->id()] = ToolWrapper{tool};
}

void DTitlebarToolFactory::remove(const QString &id)
{
    m_tools.remove(id);
}

void DTitlebarToolFactory::setTools(const QList<DTitlebarToolBaseInterface *> &tools)
{
    m_tools.clear();
    for (auto tool : std::as_const(tools))
        m_tools[tool->id()] = ToolWrapper{tool};
}

DTitlebarToolBaseInterface *DTitlebarToolFactory::tool(const QString &id) const
{
    if (!contains(id))
        return nullptr;

    return m_tools[id].tool.data();
}

QList<DTitlebarToolBaseInterface *> DTitlebarToolFactory::tools() const
{
    QList<DTitlebarToolBaseInterface *> result;
    for (auto item : m_tools.values())
        result << item.tool.data();

    return result;
}

bool DTitlebarToolFactory::contains(const QString &id) const
{
    return m_tools.contains(id);
}

QStringList DTitlebarToolFactory::toolIds() const
{
    return m_tools.keys();
}

ReloadSignal *ReloadSignal::instance()
{
    static ReloadSignal *reloadSignal = nullptr;
    if (!reloadSignal) {
        reloadSignal = new ReloadSignal;
    }
    return reloadSignal;
}

class ToolSpacer: public QWidget {
public:
    explicit ToolSpacer(QWidget *parent = nullptr) : QWidget(parent)
    {
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

void ToolSpacer::paintEvent(QPaintEvent *event)
{
    QColor color;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        color =  QColor(65, 77, 104);
    } else {
        color = QColor(192, 198, 212);
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setPen(QColor(213, 217, 221));
    painter.drawRoundedRect(this->rect().adjusted(1, 1, -1, -1), 8.0, 8.0);

    painter.setRenderHints(QPainter::Antialiasing, false);

    QPen pen(color);
    painter.setPen(color);
    painter.setBrush(color);
    painter.drawLine(QPoint(rect().x() + 4, height() / 2 - 4), QPoint(rect().x() + 4, height() / 2 + 4));
    painter.drawLine(QPoint(width() - 4 - 1, height() / 2 - 4), QPoint(width() - 4 -1, height() / 2 + 4));

    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.drawLine(QPoint(rect().x() + 4 + 2, height() / 2), QPoint(width() - 4 - 2, height() / 2));

    QWidget::paintEvent(event);
}

class DTitleBarToolSpacer : public DTitleBarSpacerInterface
{
public:
    DTitleBarToolSpacer(DTitlebarDataStore *dataStore)
        : m_dataStore(dataStore)
    {
    }
    inline virtual QString id() const override { return SettingsSpacerId; }
    virtual QString description() override
    {
        return "builtin/spacer";
    }
    virtual QString iconName() override
    {
        return "spacer_fixed";
    }
    virtual QWidget *createPlaceholderView() override
    {
        auto view = new ToolSpacer();
        view->setFixedWidth(size());
        return view;
    }
    virtual int size() const override;
private:
    const DTitlebarDataStore *m_dataStore = nullptr;
};

int DTitleBarToolSpacer::size() const
{
    if (!m_dataStore || m_dataStore->spacingSize() == -1)
        return 30;
    return m_dataStore->spacingSize();
}

class ToolStretch: public QWidget {
public:
    explicit ToolStretch(QWidget *parent = nullptr) : QWidget(parent)
    {
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

void ToolStretch::paintEvent(QPaintEvent *event)
{
    QColor color;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        color =  QColor(65, 77, 104);
    } else {
        color = QColor(192, 198, 212);
    }
    QPainter painter(this);
    painter.setPen(QColor(213, 217, 221));
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.drawRoundedRect(this->rect().adjusted(1, 1, -1, -1), 8.0, 8.0);

    painter.setRenderHints(QPainter::Antialiasing, false);

    QPen pen(color);
    painter.setPen(color);
    painter.setBrush(color);
    QPolygon leftTriangle;
    leftTriangle.setPoints(3, rect().x() + 4, height() / 2, rect().x() + 4 + 4, height() / 2 - 4, rect().x() + 4 + 4, height() / 2 + 4);
    painter.drawPolygon(leftTriangle);

    QPolygon rightTriangle;
    rightTriangle.setPoints(3, width() - 4, height() / 2, width() - 4 - 4, height() / 2 - 4, width() - 4 - 4, height() / 2 + 4);
    painter.drawPolygon(rightTriangle);

    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.drawLine(QPoint(rect().x() + 4 + 6, height() / 2), QPoint(width() - 4 - 6, height() / 2));

    QWidget::paintEvent(event);
}

class DTitleBarToolStretch : public DTitleBarSpacerInterface
{
public:
    inline virtual QString id() const override { return SettingsStretchId; }
    virtual QString description() override
    {
        return "builtin/stretch";
    }
    virtual QString iconName() override
    {
        return "spacer_stretch";
    }
    virtual QWidget *createPlaceholderView() override
    {
        auto view = new ToolStretch();
        view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        return view;
    }
    virtual int size() const override;
};

int DTitleBarToolStretch::size() const
{
    return -1;
}

class DTitlebarSettingsImplPrivate: public DCORE_NAMESPACE::DObjectPrivate {
    D_DECLARE_PUBLIC(DTitlebarSettingsImpl)
public:
    DTitlebarSettingsImplPrivate(DTitlebarSettingsImpl *qq)
        : DObjectPrivate(qq)
        , dataStore(DTitlebarDataStore::instance())
    {
    }

    void initDisplayView()
    {
        D_Q(DTitlebarSettingsImpl);

        displayView = new DTitlebarEditPanel(q, customView);
        displayView->setAutoFillBackground(true);
        displayView->setBackgroundRole(QPalette::Base);
        QObject::connect(displayView, SIGNAL(addingToolView(const QString &, const int)),
                q, SLOT(_q_addingToolView(const QString &, const int)));
        QObject::connect(displayView, SIGNAL(removedToolView(const QString &, const int)),
                q, SLOT(_q_removedToolView(const QString &, const int)));
        QObject::connect(displayView, SIGNAL(resetToolView()),
                q, SLOT(_q_resetToolView()));
        QObject::connect(displayView, SIGNAL(movedToolView(const QString &, const int)),
                q, SLOT(_q_movedToolView(const QString &, const int)));
    }

    bool load(const QString &path)
    {
        if (!dataStore->isValid()) {
            if (!dataStore->load(path))
                return false;
        }

        // remove not existed tool.
        dataStore->removeAllNotExistIds(factory.toolIds());

        D_Q(DTitlebarSettingsImpl);
        QObject::connect(ReloadSignal::instance(), SIGNAL(reload()), q, SLOT(_q_onReload()));
        loadCustomView(false);
        return true;
    }

    void loadCustomView(bool isEditMode)
    {
        D_Q(DTitlebarSettingsImpl);

        if (!customView) {
            customView = new DTitlebarCustomWidget(q);
        }

        customView->setEditMode(isEditMode);
        customView->removeAll();

        // load tool from cache.
        for (auto key : dataStore->keys()) {
            customView->addWidget(key, -1);
        }
        customView->show();
    }

    void loadSelectZoneView()
    {
        toolsEditPanel->removeAll();
        for (auto id : factory.toolIds()) {
            toolsEditPanel->addWidgetToSelectionZone(id);
        }
    }

    void loadDefaultZoneView()
    {
        D_Q(DTitlebarSettingsImpl);

        DTitlebarCustomWidget *defaultTitleBarEditPanel = new DTitlebarCustomWidget(q);
        const QSize &size = QSize(toolsEditPanel->minimumWidth(), customView->height());
        defaultTitleBarEditPanel->setFixedSize(size);
        for (auto id : dataStore->defaultIds()) {
            defaultTitleBarEditPanel->appendDefaultWidget(id);
        }
        const QPixmap &pixmap = defaultTitleBarEditPanel->grab().scaled(size.width(), size.height() * 70 / 100);
        toolsEditPanel->setDefaultView(pixmap, QSize(size.width(), size.height() * 70 / 100));
        defaultTitleBarEditPanel->deleteLater();
    }

    void loadDisplayView()
    {
        if (!displayView) {
            initDisplayView();
        }

        displayView->removeAll();
        for (auto key : dataStore->keys()) {
            displayView->addWidget(key, -1);
        }

        displayView->updateScreenShotedViews();
        adjustDisplayView();
    }

    void adjustDisplayView()
    {
        if (displayView) {
            displayView->setParent(customView->parentWidget());
            displayView->setFixedSize(customView->size());
            displayView->move(customView->pos());
            displayView->raise();
            Q_EMIT displayView->startScreenShot();
            displayView->show();
        }
    }

    void addBuiltinTools()
    {
        auto spacer = new DTitleBarToolSpacer(dataStore);
        factory.add(spacer);
        auto stretch = new DTitleBarToolStretch();
        factory.add(stretch);
    }

    void showEditPanel()
    {
        loadCustomView(true); // load the real view in title bar
        loadDisplayView(); // load the editable view in title bar
        loadSelectZoneView();
        loadDefaultZoneView();

        toolsEditPanel->show();
        toolsEditPanel->setFocus();
    }

    void removeTool(const QString &key)
    {
        factory.remove(key);
        if (!dataStore->contains(key)) {
            qDebug() << "The tool doesn't exist in factory, tool key: " << key;
            return;
        }
        dataStore->remove(key);
    }

    void _q_addingToolView(const QString &id, const int pos)
    {
        D_QC(DTitlebarSettingsImpl);
        qDebug() << Q_FUNC_INFO << id << pos;
        if (!factory.contains(id))
            return;

        if (!q->isSpacerToolById(id)) {
            if (dataStore->isExistTheId(id))
                return;
        }
        dataStore->insert(id, pos);
        dataStore->save();
        Q_EMIT ReloadSignal::instance()->reload();
    }

    void _q_removedToolView(const QString &key, const int pos)
    {
        qDebug() << Q_FUNC_INFO << key << pos;
        dataStore->remove(key);
        dataStore->save();
        Q_EMIT ReloadSignal::instance()->reload();
    }

    void _q_movedToolView(const QString &key, const int pos)
    {
        qDebug() << Q_FUNC_INFO << key << pos;
        dataStore->move(key, pos);
        dataStore->save();
        Q_EMIT ReloadSignal::instance()->reload();
    }

    void _q_resetToolView()
    {
        qDebug() << Q_FUNC_INFO;
        dataStore->reset();
        Q_EMIT ReloadSignal::instance()->reload();
    }

    void _q_confirmBtnClicked()
    {
        qDebug() << Q_FUNC_INFO << this;
        dataStore->save();
        customView->setEditMode(false);
        customView->reloadWidgets();
        displayView->setVisible(false);
    }

    void _q_onReload()
    {
        qDebug() << Q_FUNC_INFO << this;
        loadCustomView(customView->editMode());
    }

    QWidget *tryCreateToolsEditPanel()
    {
        D_Q(DTitlebarSettingsImpl);
        if (!toolsEditPanel) {
            toolsEditPanel = new DToolbarEditPanel(q);
            QObject::connect(toolsEditPanel.data(), SIGNAL(confirmBtnClicked()),
                    q, SLOT(_q_confirmBtnClicked()));
        }
        return toolsEditPanel;
    }

    DTitlebarToolFactory factory;
    DTitlebarDataStore *dataStore = nullptr;
    DTitlebarCustomWidget *customView = nullptr;
    DTitlebarEditPanel *displayView = nullptr;

    QPointer<DToolbarEditPanel> toolsEditPanel = nullptr;
};

DTitlebarSettingsImpl::DTitlebarSettingsImpl(QObject *parent)
    : QObject(parent)
    , DObject(*new DTitlebarSettingsImplPrivate(this))
{
}

DTitlebarSettingsImpl::~DTitlebarSettingsImpl()
{
}

void DTitlebarSettingsImpl::setTools(const QList<DTitlebarToolBaseInterface *> &tools)
{
    D_D(DTitlebarSettingsImpl);
    d->factory.setTools(tools);
    d->addBuiltinTools();
}

void DTitlebarSettingsImpl::addTool(DTitlebarToolBaseInterface *tool)
{
    D_D(DTitlebarSettingsImpl);
    d->factory.add(tool);
}

DTitlebarToolBaseInterface *DTitlebarSettingsImpl::tool(const QString &key) const
{
    D_DC(DTitlebarSettingsImpl);
    auto id = d->dataStore->toolId(key);
    return d->factory.tool(id);
}

DTitlebarToolBaseInterface *DTitlebarSettingsImpl::toolById(const QString &id) const
{
    D_DC(DTitlebarSettingsImpl);
    return d->factory.tool(id);
}

QStringList DTitlebarSettingsImpl::keys() const
{
    D_DC(DTitlebarSettingsImpl);
    return d->dataStore->keys();
}

QString DTitlebarSettingsImpl::findKeyByPos(const int pos) const
{
    D_DC(DTitlebarSettingsImpl);
    return d->dataStore->findKeyByPos(pos);
}

QString DTitlebarSettingsImpl::toolId(const QString &key) const
{
    D_DC(DTitlebarSettingsImpl);
    return d->dataStore->toolId(key);
}

void DTitlebarSettingsImpl::removeTool(const QString &key)
{
    D_D(DTitlebarSettingsImpl);
    d->removeTool(key);
}

bool DTitlebarSettingsImpl::isSpacerTool(const DTitlebarToolBaseInterface *tool)
{
    return qobject_cast<const DTitleBarSpacerInterface *>(tool);
}

bool DTitlebarSettingsImpl::isSpacerTool(const QString &key) const
{
    D_DC(DTitlebarSettingsImpl);
    const auto id = d->dataStore->toolId(key);
    return isSpacerToolById(id);
}

bool DTitlebarSettingsImpl::isStrecherTool(const QString &key) const
{
    D_DC(DTitlebarSettingsImpl);
    const auto id = d->dataStore->toolId(key);
    if (auto tool = qobject_cast<DTitleBarSpacerInterface *>(d->factory.tool(id)))
        return tool->size() < 0;

    return false;
}

bool DTitlebarSettingsImpl::isSpacerToolById(const QString &id) const
{
    D_DC(DTitlebarSettingsImpl);
    return isSpacerTool(d->factory.tool(id));
}

bool DTitlebarSettingsImpl::isFixedTool(const QString &key) const
{
    D_DC(DTitlebarSettingsImpl);
    return d->dataStore->isFixed(key);
}

bool DTitlebarSettingsImpl::isFixedTool(const int pos) const
{
    D_DC(DTitlebarSettingsImpl);
    return d->dataStore->isFixed(pos);
}

bool DTitlebarSettingsImpl::load(const QString &path)
{
    D_D(DTitlebarSettingsImpl);
    return d->load(path);
}

QWidget *DTitlebarSettingsImpl::toolsView() const
{
    D_DC(DTitlebarSettingsImpl);
    return d->customView;
}

QWidget *DTitlebarSettingsImpl::toolsEditPanel() const
{
    D_DC(DTitlebarSettingsImpl);
    return const_cast<DTitlebarSettingsImplPrivate *>(d)->tryCreateToolsEditPanel();
}

bool DTitlebarSettingsImpl::hasEditPanel() const
{
    D_DC(DTitlebarSettingsImpl);
    return d->toolsEditPanel != nullptr;
}

void DTitlebarSettingsImpl::adjustDisplayView()
{
    D_D(DTitlebarSettingsImpl);
    d->adjustDisplayView();
}

void DTitlebarSettingsImpl::showEditPanel()
{
    D_D(DTitlebarSettingsImpl);
    d->showEditPanel();
}

bool DTitlebarSettingsImpl::isValid() const
{
    D_DC(DTitlebarSettingsImpl);
    return d->dataStore->isValid();
}

void DTitlebarSettingsImpl::clearCache()
{
    D_D(DTitlebarSettingsImpl);
    d->dataStore->clear();
}

DWIDGET_END_NAMESPACE

#include "moc_dtitlebarsettingsimpl.cpp"

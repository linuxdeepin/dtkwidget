// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include "dtkwidget_global.h"
#include "dtitlebarsettings.h"
#include <DObjectPrivate>
#include <DObject>
#include <QSharedPointer>
#include <QVector>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DTitlebarToolBaseInterface;

struct ToolInstance
{
    QString key;
    QString toolId;
    bool isFixed = false;
};
class DTitlebarDataStore : public QObject
{
    Q_OBJECT

public:
    explicit DTitlebarDataStore(QObject *parent = nullptr);
    virtual ~DTitlebarDataStore() override;

    static DTitlebarDataStore *instance();
    bool load(const QString &path);
    void save();
    void clear();
    void reset();
    bool isValid() const;

    QString findKeyByPos(const int pos) const;
    QStringList defaultIds() const;
    QStringList keys() const;
    QString key(const int pos);
    QStringList toolIds() const;
    QString toolId(const QString &key) const;
    void  removeAllNotExistIds(const QStringList &ids);
    int position(const QString &key) const;
    bool contains(const QString &key) const;
    bool isExistTheId(const QString &id) const;
    int spacingSize() const;

    void move(const QString &key, const int pos);
    QString add(const QString &id);
    QString insert(const QString &id, const int pos);
    void remove(const QString &key);
    void remove(const int pos);

    bool isFixed(const QString &key) const;
    bool isFixed(const int pos) const;

private:
    bool load();
    ToolInstance *createInstance(const QString &id);
    ToolInstance *createInstance(const QString &id, const QString &key);
    ToolInstance *getInstance(const QString &key) const;
    bool isInvalid() const;
    QStringList positionsFromToolMeta(const QJsonObject &root) const;
    QList<ToolInstance> toolInstancesFromToolMeta(const QJsonObject &root) const;
    bool fixedFromToolMeta(const QJsonObject &root, const int index) const;
    int countFromToolMeta(const QJsonObject &root, const int index) const;
    QString alignmentFromToolMeta(const QJsonObject &root) const;
    QStringList positionsFromToolMeta() const;
    QJsonObject metaRoot() const;
    QVariantList positionsFromCache();
    void savePositionsToCache();
    void clearCache();
    bool acceptCountField(const QString &id) const;

private:
    QString m_settingsGroupName;
    QString m_settingsGroupNameSubGroup;
    QVector<ToolInstance *> m_instances;
    int m_spacingSize = -1;
    bool m_isValid = false;
    QString m_filePath;
};

struct ToolWrapper
{
    explicit ToolWrapper(DTitlebarToolBaseInterface *t = nullptr)
        : tool(t)
    {
    }
    QSharedPointer<DTitlebarToolBaseInterface> tool = nullptr;
};

class DTitlebarToolFactory : public QObject
{
    Q_OBJECT
public:
    explicit DTitlebarToolFactory(QObject *parent = nullptr);
    virtual ~DTitlebarToolFactory() override;

    void add(DTitlebarToolBaseInterface *tool);
    void remove(const QString &id);
    void setTools(const QList<DTitlebarToolBaseInterface *> &tools);
    DTitlebarToolBaseInterface *tool(const QString &id) const;
    QList<DTitlebarToolBaseInterface *> tools() const;
    bool contains(const QString &id) const;

    QStringList toolIds() const;

private:
    QMap<QString, ToolWrapper> m_tools;
};

class ReloadSignal : public QObject
{
    Q_OBJECT
public:
    static ReloadSignal *instance();
Q_SIGNALS:
    void reload();
private:
    ReloadSignal() = default;
    ~ReloadSignal() = default;
};

class DTitlebarSettingsImplPrivate;

class DTitlebarSettingsImpl : public QObject, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DTitlebarSettingsImpl(QObject *parent = nullptr);
    virtual ~DTitlebarSettingsImpl() override;

    void setTools(const QList<DTitlebarToolBaseInterface *> &tools);
    void addTool(DTitlebarToolBaseInterface *tool);
    DTitlebarToolBaseInterface *tool(const QString &key) const;
    DTitlebarToolBaseInterface *toolById(const QString &id) const;
    QStringList keys() const;
    QString findKeyByPos(const int pos) const;
    QString toolId(const QString &key) const;
    void removeTool(const QString &key);
    static bool isSpacerTool(const DTitlebarToolBaseInterface *tool);
    bool isSpacerTool(const QString &key) const;
    bool isStrecherTool(const QString &key) const;
    bool isSpacerToolById(const QString &id) const;
    bool isFixedTool(const QString &key) const;
    bool isFixedTool(const int pos) const;
    bool load(const QString &path);

    QWidget *toolsView() const;
    QWidget *toolsEditPanel() const;
    bool hasEditPanel() const;
    void adjustDisplayView();
    void showEditPanel();
    bool isValid() const;
    void clearCache();

private:
    D_DECLARE_PRIVATE(DTitlebarSettingsImpl)

    D_PRIVATE_SLOT(void _q_addingToolView(const QString &, const int))
    D_PRIVATE_SLOT(void _q_removedToolView(const QString &, const int))
    D_PRIVATE_SLOT(void _q_movedToolView(const QString &, const int))
    D_PRIVATE_SLOT(void _q_resetToolView())
    D_PRIVATE_SLOT(void _q_confirmBtnClicked())
    D_PRIVATE_SLOT(void _q_onReload())
};

DWIDGET_END_NAMESPACE

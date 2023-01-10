// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <DLineEdit>
#include <QTest>

#include "dtitlebar.h"
#include "private/dtitlebarsettingsimpl.h"

DWIDGET_USE_NAMESPACE

static const QString dataFilePath = ":/data/titlebar-settings.json";

class ut_DTitlebarDataStore : public testing::Test
{
protected:
    virtual void TearDown() override;
};

void ut_DTitlebarDataStore::TearDown()
{
    // clear cache.
    DTitlebarDataStore dataStore;
    dataStore.clear();
}

TEST_F(ut_DTitlebarDataStore, loadAndSave)
{
    QStringList settingsTools({"builtin/search-tool",
                                     "test-tool"});
    {
        DTitlebarDataStore dataStore;
        dataStore.clear();
        ASSERT_TRUE(dataStore.load(dataFilePath));
        ASSERT_EQ(dataStore.toolIds(), settingsTools << "builtin/stretch");
    }
    {
        DTitlebarDataStore dataStore;
        dataStore.load(dataFilePath);
        dataStore.move("test-tool", 0);
        ASSERT_EQ(dataStore.defaultIds(), settingsTools);
    }
}

TEST_F(ut_DTitlebarDataStore, addAndRemove)
{
    {
        QStringList settingsTools({"builtin/search-tool",
                                   "test-tool",
                                  "builtin/stretch",
                                  "test-tool2"});
        DTitlebarDataStore dataStore;
        dataStore.load(dataFilePath);
        dataStore.add("test-tool2");
        ASSERT_EQ(dataStore.toolIds(), settingsTools);
    }
    {
        QStringList settingsTools({"builtin/search-tool",
                                   "test-tool",
                                  "builtin/stretch",
                                  "test-tool2"});
        DTitlebarDataStore dataStore;
        dataStore.load(dataFilePath);
        ASSERT_EQ(dataStore.toolIds(), settingsTools);
    }
    {
        QStringList settingsTools({"builtin/search-tool",
                                   "test-tool",
                                  "builtin/stretch",
                                  "test-tool2"});
        DTitlebarDataStore dataStore;
        dataStore.load(dataFilePath);
        const auto key = dataStore.keys().at(0);
        const auto toolId = dataStore.toolId(key);
        dataStore.remove(key);
        ASSERT_EQ(toolId, "builtin/search-tool");
        settingsTools.removeAll(toolId);
        ASSERT_EQ(dataStore.toolIds().size(), settingsTools.size());
    }
}

class TitleBarToolTest : public DTitleBarToolInterface {
public:
    virtual QWidget *createView() override
    {
        auto view = new DLineEdit();
        connect(view, &DLineEdit::textEdited, this, [this](const QString &) {
            actionExecuted = true;
        });
        return view;
    }
    virtual QString id() const override
    {
        return "test-tool";
    }
    virtual QString description() override
    {
        return "test tool";
    }
    virtual QString iconName() override
    {
        return "test-icon";
    }
    bool actionExecuted = false;
};

class TitleBarToolTest2 : public TitleBarToolTest {
public:
    virtual QString id() const override
    {
        return "test-tool2";
    }
    virtual QString description() override
    {
        return "test tool2";
    }
};

class ut_DTitleBarToolInterface : public testing::Test
{
};

TEST_F(ut_DTitleBarToolInterface, base)
{
    auto tool = new TitleBarToolTest();
    auto view = qobject_cast<DLineEdit *>(tool->createView());
    ASSERT_TRUE(view);
    ASSERT_FALSE(tool->actionExecuted);
    view->textEdited("text");
    ASSERT_TRUE(tool->actionExecuted);

    ASSERT_EQ(tool->id(), QString("test-tool"));
    ASSERT_EQ(tool->description(), QString("test tool"));
    ASSERT_EQ(tool->iconName(), QString("test-icon"));

    tool->deleteLater();
    view->deleteLater();
}

class ut_DTitlebarToolFactory : public testing::Test
{
};

TEST_F(ut_DTitlebarToolFactory, base)
{
    DTitlebarToolFactory factory;
    auto tool = new TitleBarToolTest();
    factory.add(tool);
    ASSERT_EQ(factory.toolIds(), QStringList{"test-tool"});
    ASSERT_TRUE(factory.contains("test-tool"));
    ASSERT_EQ(factory.tool("test-tool"), tool);
}

class ut_DTitleBarSettings : public testing::Test
{
protected:
    virtual void TearDown() override;
};

void ut_DTitleBarSettings::TearDown()
{
    // clear cache.
    DTitlebarSettingsImpl settings;
    settings.clearCache();
}

TEST_F(ut_DTitleBarSettings, base)
{
    DTitlebarSettingsImpl settings;
    settings.addTool(new TitleBarToolTest());
    settings.addTool(new TitleBarToolTest2());
    settings.load(dataFilePath);
}

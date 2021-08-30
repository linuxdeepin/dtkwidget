/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>
#include <DFontSizeManager>
#include <DKeySequenceEdit>
#include <DSettingsOption>
#include <DSwitchButton>
#include <QJsonObject>
#include <QWidget>
#include <private/settings/contenttitle.h>

#include "dsettingswidgetfactory.h"
DWIDGET_USE_NAMESPACE
class ut_DSettingsWidgetFactory : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DSettingsWidgetFactory();
        target->registerWidget("registerWidget", std::bind(&ut_DSettingsWidgetFactory::widgetCreateHandler, this, std::placeholders::_1));
        QJsonObject opt;
        opt["key"] = "register";
        opt["type"] = "registerWidget";
        registerOption = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
        if (registerOption) {
            registerOption->deleteLater();
        }
    }
    DSettingsWidgetFactory *target = nullptr;
    QWidget *widgetCreateHandler(QObject *)
    {
        createdWidget = new QWidget();
        return createdWidget;
    }
    QWidget *createdWidget = nullptr;
    DTK_CORE_NAMESPACE::DSettingsOption *registerOption = nullptr;

};

TEST_F(ut_DSettingsWidgetFactory, createStandardItem)
{
    QWidget *rightWidget = new QWidget();
    auto result = target->createStandardItem(QByteArray(), registerOption, rightWidget);
    ASSERT_EQ(result.first, nullptr);
    ASSERT_EQ(result.second, rightWidget);
    result.first->deleteLater();
    result.second->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, createWidget)
{
    auto widget = target->createWidget(registerOption);
    ASSERT_EQ(widget, createdWidget);
    widget->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, createWidget2)
{
    QByteArray translateContext("translateContext");
    QWidget *result = target->createWidget(translateContext, registerOption);
    ASSERT_EQ(registerOption->property("_d_DSettingsWidgetFactory_translateContext").toByteArray(), translateContext);
    result->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, registerWidget)
{
    QWidget *parent = new QWidget();
    auto widgetCreateHandler = [parent](QObject *){
        return new QWidget(parent);
    };
    target->registerWidget("registerWidget2", widgetCreateHandler);
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "registerWidget2";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);;
    auto result = target->createWidget(option);

    ASSERT_EQ(result->parent(), parent);
    parent->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, registerWidget2)
{
    QWidget *parent = new QWidget();
    auto itemCreateHandler = [parent](QObject *){
        return QPair<QWidget*, QWidget*>(new QWidget(parent), new QWidget(parent));
    };
    target->registerWidget("registerWidget2", itemCreateHandler);

    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "registerWidget2";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    auto result = target->createItem(option);
    ASSERT_EQ(result.first->parent(), parent);
    ASSERT_EQ(result.second->parent(), parent);
    parent->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, shortcut)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "shortcut";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    auto result = target->createItem(option);
    ASSERT_EQ(result.first, nullptr);
    auto rightWidget = qobject_cast<DKeySequenceEdit*>(result.second);
    ASSERT_EQ(rightWidget->objectName(), "OptionShortcutEdit");

    rightWidget->editingFinished(QKeySequence(QKeySequence::Copy));
    rightWidget->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, checkbox)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "checkbox";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    auto result = target->createItem(option);
    ASSERT_EQ(result.first->objectName(), "OptionCheckbox");
    ASSERT_EQ(result.second, nullptr);
    result.first->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, switchbutton)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "switchbutton";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    auto result = target->createItem(option);
    ASSERT_EQ(result.first, nullptr);
    QWidget *content = result.second->findChild<DSwitchButton*>();
    ASSERT_TRUE(content);
    ASSERT_EQ(content->objectName(), "OptionDSwitchButton");
    result.second->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, title1)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "title1";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    auto result = target->createItem(option);
    auto firstWidget = qobject_cast<ContentTitle*>(result.first);
    ASSERT_EQ(firstWidget->font().pixelSize(), DFontSizeManager::instance()->t4().pixelSize());
    ASSERT_EQ(result.second, nullptr);
    firstWidget->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, title2)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "title2";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);
    auto result = target->createItem(option);
    auto firstWidget = qobject_cast<ContentTitle*>(result.first);
    ASSERT_EQ(firstWidget->font().pixelSize(), DFontSizeManager::instance()->t5().pixelSize());
    ASSERT_EQ(result.second, nullptr);
    firstWidget->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, createUnsupportHandle)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "registerWidget2";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);;
    auto result = target->createWidget(option);
    ASSERT_EQ(result->objectName(), "OptionUnsupport");
    result->deleteLater();
    option->deleteLater();
};

TEST_F(ut_DSettingsWidgetFactory, lineedit)
{
    QJsonObject opt;
    opt["key"] = "register2";
    opt["type"] = "lineedit";
    auto option = DTK_CORE_NAMESPACE::DSettingsOption::fromJson("register", opt);;
    auto result = target->createItem(option);
    ASSERT_EQ(result.first, nullptr);
    ASSERT_EQ(result.second->objectName(), "OptionLineEdit");
    result.second->deleteLater();
    option->deleteLater();
};



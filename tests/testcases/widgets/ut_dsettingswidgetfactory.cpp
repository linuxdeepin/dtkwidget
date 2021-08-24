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
#include <DSettingsOption>
#include <QJsonObject>
#include <QWidget>

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
};

TEST_F(ut_DSettingsWidgetFactory, createWidget)
{
    auto widget = target->createWidget(registerOption);
    ASSERT_EQ(widget, createdWidget);
};

TEST_F(ut_DSettingsWidgetFactory, createWidget2)
{
    QByteArray translateContext("translateContext");
    target->createWidget(translateContext, registerOption);
    ASSERT_EQ(registerOption->property("_d_DSettingsWidgetFactory_translateContext").toByteArray(), translateContext);
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

};

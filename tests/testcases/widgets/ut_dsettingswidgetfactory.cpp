// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

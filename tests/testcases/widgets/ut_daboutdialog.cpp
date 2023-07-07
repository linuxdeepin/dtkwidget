// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "daboutdialog.h"
DWIDGET_USE_NAMESPACE
class ut_DAboutDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        widget = new DAboutDialog();
    }
    void TearDown() override
    {
        if (widget) {
            delete widget;
            widget = nullptr;
        }
    }
    DAboutDialog *widget = nullptr;
};

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0) // dtk 6.0 no longer support
TEST_F(ut_DAboutDialog, setAcknowledgementLink)
{
    widget->setAcknowledgementLink(QString{"setAcknowledgementLink"});
};
#endif

TEST_F(ut_DAboutDialog, setAcknowledgementVisible)
{
    widget->setAcknowledgementVisible(true);
};

TEST_F(ut_DAboutDialog, setCompanyLogo)
{
    widget->setCompanyLogo(QPixmap(50, 50));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    ASSERT_EQ(widget->companyLogo()->size(), QSize(50, 50));
#else
    ASSERT_EQ(widget->companyLogo().size(), QSize(50, 50));
#endif
};

TEST_F(ut_DAboutDialog, setDescription)
{
    widget->setDescription("setDescription");
    ASSERT_EQ(widget->description(), "setDescription");
};

TEST_F(ut_DAboutDialog, setLicense)
{
    widget->setLicense("setLicense");
    ASSERT_EQ(widget->license(), "setLicense");
};

TEST_F(ut_DAboutDialog, setProductIcon)
{
    widget->setProductIcon(QIcon::fromTheme("preferences-system"));
};

TEST_F(ut_DAboutDialog, setProductName)
{
    widget->setProductName("setProductName");
    ASSERT_EQ(widget->productName(), "setProductName");
};

TEST_F(ut_DAboutDialog, setVersion)
{
    widget->setVersion("setVersion");
    ASSERT_EQ(widget->version(), "setVersion");
};

TEST_F(ut_DAboutDialog, setWebsiteLink)
{
    widget->setWebsiteLink("setWebsiteLink");
    ASSERT_EQ(widget->websiteLink(), "setWebsiteLink");
};

TEST_F(ut_DAboutDialog, setWebsiteName)
{
    widget->setWebsiteName("setWebsiteName");
    ASSERT_EQ(widget->websiteName(), "setWebsiteName");
};

TEST_F(ut_DAboutDialog, setWindowTitle)
{
    widget->setWindowTitle("setWindowTitle");
    ASSERT_EQ(widget->windowTitle(), "setWindowTitle");
};

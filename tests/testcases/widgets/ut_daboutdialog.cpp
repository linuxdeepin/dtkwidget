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

TEST_F(ut_DAboutDialog, setAcknowledgementLink)
{
    widget->setAcknowledgementLink("setAcknowledgementLink");
    ASSERT_EQ(widget->acknowledgementLink(), "setAcknowledgementLink");
};

TEST_F(ut_DAboutDialog, setAcknowledgementVisible)
{
    widget->setAcknowledgementVisible(true);
};

TEST_F(ut_DAboutDialog, setCompanyLogo)
{
    widget->setCompanyLogo(QPixmap(50, 50));
    ASSERT_EQ(widget->companyLogo()->size(), QSize(50, 50));
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


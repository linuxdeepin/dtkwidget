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
#include <QPushButton>
#include <QPixmap>
#include <QDebug>

#include "ddialog.h"
#include "private/ddialog_p.h"

DWIDGET_USE_NAMESPACE
class ut_DDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DDialog();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DDialog *target = nullptr;
};

TEST_F(ut_DDialog, addButton)
{
    target->addButton("addButtons");
    ASSERT_EQ(target->buttonCount(), 1);

};

TEST_F(ut_DDialog, addButtons2)
{
    target->addButtons({"addButtons"});
    ASSERT_EQ(target->buttonCount(), 1);
    ASSERT_EQ(target->getButtons().size(), 1);
};

TEST_F(ut_DDialog, addContent)
{
    QWidget *content = new QWidget();
    target->addContent(content);
    ASSERT_EQ(target->getContent(0), content);
    ASSERT_EQ(target->contentCount(), 1);
};

TEST_F(ut_DDialog, clearButtons)
{
    target->addButtons({"addButtons"});
    ASSERT_EQ(target->buttonCount(), 1);
    target->clearButtons();
    ASSERT_EQ(target->buttonCount(), 0);
};

TEST_F(ut_DDialog, clearContents)
{
    QWidget *content = new QWidget();
    target->addContent(content);
    target->clearContents(true);
    ASSERT_EQ(target->getContents().size(), 0);
    ASSERT_EQ(target->contentCount(), 0);
};

TEST_F(ut_DDialog, getButton)
{
    target->addButtons({"getButton"});
    ASSERT_EQ(target->getButton(0)->text(), "getButton");
};

TEST_F(ut_DDialog, getButtonIndexByText)
{
    target->addButtons({"getButtonIndexByText"});
    ASSERT_EQ(target->getButtonIndexByText("getButtonIndexByText"), 0);
};

TEST_F(ut_DDialog, insertButton)
{
    target->insertButton(0, "insertButton");
    ASSERT_EQ(target->buttonCount(), 1);
};

TEST_F(ut_DDialog, insertButton2)
{
    QPushButton* button = new QPushButton();
    target->insertButton(0, button);
    ASSERT_EQ(target->buttonCount(), 1);
};

TEST_F(ut_DDialog, insertButtons)
{
    target->insertButtons(0, {"insertButtons"});
    ASSERT_EQ(target->buttonCount(), 1);
};

TEST_F(ut_DDialog, insertContent)
{
    QWidget *content = new QWidget();
    target->insertContent(0, content);
    ASSERT_EQ(target->contentCount(), 1);
};

TEST_F(ut_DDialog, removeButton)
{
    target->addButtons({"removeButton"});
    target->removeButton(0);
    ASSERT_EQ(target->buttonCount(), 0);
};

TEST_F(ut_DDialog, removeButton2)
{
    target->addButtons({"removeButton"});
    target->removeButton(target->getButton(0));
    ASSERT_EQ(target->buttonCount(), 0);
};

TEST_F(ut_DDialog, removeButtonByText)
{
    target->addButtons({"removeButtonByText"});
    target->removeButtonByText("removeButtonByText");
    ASSERT_EQ(target->buttonCount(), 0);
};

TEST_F(ut_DDialog, removeContent)
{
    QWidget *content = new QWidget();
    target->addContent(content);
    target->removeContent(content, true);
    ASSERT_EQ(target->contentCount(), 0);
};

TEST_F(ut_DDialog, setButtonIcon)
{
    target->addButtons({"setButtonIcon"});
    target->setButtonIcon(0, QIcon::fromTheme("preferences-system"));
    ASSERT_EQ(target->getButton(0)->icon().name(), QIcon::fromTheme("preferences-system").name());
};

TEST_F(ut_DDialog, setButtonText)
{
    target->addButtons({"setButtonText"});
    target->setButtonText(0, "setButtonText");
    ASSERT_EQ(target->getButton(0)->text(), "setButtonText");
};

TEST_F(ut_DDialog, setCloseButtonVisible)
{
    target->setCloseButtonVisible(true);
    ASSERT_EQ(target->closeButtonVisible(), true);
};

TEST_F(ut_DDialog, setContentLayoutContentsMargins)
{
    QMargins margin;
    margin.setLeft(10);

    target->setContentLayoutContentsMargins(margin);
    ASSERT_EQ(target->contentLayoutContentsMargins(), margin);
};

TEST_F(ut_DDialog, setIcon)
{
    target->setIcon(QIcon::fromTheme("preferences-system"));
    ASSERT_EQ(target->icon().name(), QIcon::fromTheme("preferences-system").name());
};

TEST_F(ut_DDialog, setMessage)
{
    target->setMessage("setMessage");
    ASSERT_EQ(target->message(), "setMessage");
};

TEST_F(ut_DDialog, setOnButtonClickedClose)
{
    target->setOnButtonClickedClose(true);
    ASSERT_EQ(target->onButtonClickedClose(), true);
};

TEST_F(ut_DDialog, setTextFormat)
{
    target->setTextFormat(Qt::AutoText);
    ASSERT_EQ(target->textFormat(), Qt::AutoText);
};

TEST_F(ut_DDialog, setTitle)
{
    target->setTitle("setTitle");
    ASSERT_EQ(target->title(), "setTitle");
};

TEST_F(ut_DDialog, setWordWrapMessage)
{
    target->setWordWrapMessage(true);
};

TEST_F(ut_DDialog, setWordWrapTitle)
{
    target->setWordWrapTitle(true);
};

TEST_F(ut_DDialog, supplement)
{
    target->d_func()->getContentLayout();
    ASSERT_TRUE(target->d_func()->getScreen() != nullptr);

    const QString tagContent = "<tag>xxx</tag>yyy";
    ASSERT_EQ(target->d_func()->trimTag(tagContent), "xxxyyy");
    // TODO: 返回的值好像不正确
    ASSERT_EQ(target->d_func()->scanTags(tagContent)[1], "");

    target->d_func()->elideString(tagContent, target->fontMetrics(), target->width());
    target->d_func()->_q_onButtonClicked();
    target->d_func()->_q_defaultButtonTriggered();

    // test DDialog param constructor
    const QString &title = "test";
    const QString &message = "Test Msg";
    QWidget *widget = new QWidget;

    DDialog dialog(title, message, widget);
    dialog.iconPixmap();
    dialog.insertButton(0, "button0", false, DDialog::ButtonType::ButtonNormal);
    dialog.insertButton(1, "button1", false, DDialog::ButtonType::ButtonWarning);
    dialog.insertButton(2, "button2", false, DDialog::ButtonType::ButtonRecommend);

    dialog.setDefaultButton(0);
    dialog.setSpacing(10);
    dialog.insertSpacing(2, 20);
    dialog.clearSpacing();

    QSize expectSize(30,30);
    dialog.setIcon(QIcon::fromTheme("icon_Window"), expectSize);
    dialog.setIconPixmap(QIcon::fromTheme("icon_Window").pixmap(expectSize));

    widget->deleteLater();
}



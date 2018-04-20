/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "inputtab.h"
#include "dshortcutedit.h"
#include "dfilechooseredit.h"
#include "dipv4lineedit.h"
#include "dspinbox.h"
#include "dcrumbedit.h"

#include <dthememanager.h>

#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QCheckBox>

#ifdef Q_OS_LINUX
#include "dpasswdeditanimated.h"
#endif

InputTab::InputTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("InputTab{background-color: #252627;}");
    setFocusPolicy(Qt::ClickFocus);

    DTK_WIDGET_NAMESPACE::DPasswordEdit *pwdEdit = new DTK_WIDGET_NAMESPACE::DPasswordEdit(this);
    pwdEdit->setText("password");
    pwdEdit->move(20, 20);
    pwdEdit->setContextMenuPolicy(Qt::DefaultContextMenu);

    DTK_WIDGET_NAMESPACE::DPasswordEdit *pwdEdit2 = new DTK_WIDGET_NAMESPACE::DPasswordEdit(this);
    pwdEdit2->setText("password");
    pwdEdit2->setEchoMode(DTK_WIDGET_NAMESPACE::DPasswordEdit::Normal);
    pwdEdit2->setAlert(true);
    pwdEdit2->move(20, 50);
    connect(pwdEdit2, &DTK_WIDGET_NAMESPACE::DPasswordEdit::focusChanged, [](bool focus) {qDebug() << "focus: " << focus;});
    connect(pwdEdit2, &DTK_WIDGET_NAMESPACE::DPasswordEdit::textChanged, [](const QString &text) {qDebug() << "text: " << text;});

    DTK_WIDGET_NAMESPACE::DSearchEdit *searchEdit = new DTK_WIDGET_NAMESPACE::DSearchEdit(this);
    //searchEdit->setSearchIcon(":/images/button.png");
    //searchEdit->setFixedWidth(300);
    searchEdit->move(20, 120);
    searchEdit->setPlaceHolder("Tes");
    Dtk::Widget::DThemeManager::instance()->setTheme(searchEdit, "dark");

    DTK_WIDGET_NAMESPACE::DLineEdit *lineEdit = new DTK_WIDGET_NAMESPACE::DLineEdit(this);
    lineEdit->setText("Test Alert Message");
    lineEdit->move(20, 180);
//    lineEdit->setAlert(true);
//    lineEdit->setFixedSize(200, 30);
    connect(lineEdit, &DTK_WIDGET_NAMESPACE::DLineEdit::focusChanged, [](bool focus) {qDebug() << "focus: " << focus;});
    QTimer::singleShot(2000, nullptr, [=] {lineEdit->showAlertMessage("Test Alert Message !!");});

    DTK_WIDGET_NAMESPACE::DFileChooserEdit *fileChooser = new DTK_WIDGET_NAMESPACE::DFileChooserEdit(this);
    fileChooser->move(150, 180);
    fileChooser->setDialogDisplayPosition(DTK_WIDGET_NAMESPACE::DFileChooserEdit::CurrentMonitorCenter);

    DTK_WIDGET_NAMESPACE::DLineEdit *lineEditAlert = new DTK_WIDGET_NAMESPACE::DLineEdit(this);
    lineEditAlert->setText("AlertLineEdit");
    lineEditAlert->setAlert(true);
    lineEditAlert->setFixedSize(200, 30);
    lineEditAlert->move(20, 230);
    lineEditAlert->setValidator(new QRegExpValidator(QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){0,3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?")));

    DTK_WIDGET_NAMESPACE::DShortcutEdit *shortcutEdit = new DTK_WIDGET_NAMESPACE::DShortcutEdit(this);
    shortcutEdit->move(20, 300);

    DTK_WIDGET_NAMESPACE::DIpv4LineEdit *ipv4 = new DTK_WIDGET_NAMESPACE::DIpv4LineEdit(this);

    ipv4->move(300, 20);

    connect(ipv4, &DTK_WIDGET_NAMESPACE::DIpv4LineEdit::textChanged, this, [this, ipv4](const QString &text) {
        qDebug() << "text changed:" <<text << ipv4->text();
    });

    ipv4->setText("192.168.31.55");

    DTK_WIDGET_NAMESPACE::DSpinBox *spinbox = new DTK_WIDGET_NAMESPACE::DSpinBox(this);

    spinbox->resize(100, 22);

    spinbox->move(300, 50);


    DTK_WIDGET_NAMESPACE::DCrumbEdit* crumbEdit = new DTK_WIDGET_NAMESPACE::DCrumbEdit(this);
    crumbEdit->resize(100, 50);
//    crumbEdit->setDualClickCreateCrumb(true);
//    crumbEdit->setCrumbReadOnly(true);
    crumbEdit->move(300, 100);

//    searchEdit->setFocus();
    lineEditAlert->setFocus();

#ifdef Q_OS_LINUX
    DTK_WIDGET_NAMESPACE::DPasswdEditAnimated *passwdEA = new DTK_WIDGET_NAMESPACE::DPasswdEditAnimated(this);
    passwdEA->move(500, 20);
    QCheckBox *checkButton1 = new QCheckBox("show alert", this);
    checkButton1->setStyleSheet("color: red");
    checkButton1->move(700, 20);
    connect(checkButton1, &QCheckBox::clicked, [=](bool checked){
            if (checked) passwdEA->showAlert("button clicked!!!");
            else passwdEA->hideAlert();
        });

    QCheckBox *checkButton2 = new QCheckBox("keyboard enable", this);
    checkButton2->setStyleSheet("color: red");
    checkButton2->setChecked(true);
    checkButton2->move(700, 40);
    connect(checkButton2, &QCheckBox::clicked, [=](bool checked){
            if (checked) passwdEA->setKeyboardButtonEnable(true);
            else passwdEA->setKeyboardButtonEnable(false);
        });

    QCheckBox *checkButton3 = new QCheckBox("capslock enable", this);
    checkButton3->setStyleSheet("color: red");
    checkButton3->setChecked(true);
    checkButton3->move(700, 60);
    connect(checkButton3, &QCheckBox::clicked, [=](bool checked){
            if (checked) passwdEA->setCapslockIndicatorEnable(true);
            else passwdEA->setCapslockIndicatorEnable(false);
        });

    QCheckBox *checkButton4 = new QCheckBox("eye enable", this);
    checkButton4->setStyleSheet("color: red");
    checkButton4->setChecked(true);
    checkButton4->move(700, 80);
    connect(checkButton4, &QCheckBox::clicked, [=](bool checked){
            if (checked) passwdEA->setEyeButtonEnable(true);
            else passwdEA->setEyeButtonEnable(false);
        });

    QCheckBox *checkButton5 = new QCheckBox("submit enable", this);
    checkButton5->setStyleSheet("color: red");
    checkButton5->setChecked(true);
    checkButton5->move(700, 100);
    connect(checkButton5, &QCheckBox::clicked, [=](bool checked){
            if (checked) passwdEA->setSubmitButtonEnable(true);
            else passwdEA->setSubmitButtonEnable(false);
        });

#endif
}

/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "inputtab.h"
#include "dshortcutedit.h"
#include "dfilechooseredit.h"
#include "dipv4lineedit.h"
#include "dspinbox.h"
#include "dcrumbedit.h"
#include "dalertcontrol.h"

#include <QComboBox>
#include <dthememanager.h>

#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QCheckBox>
#include <QVariantAnimation>
#include <QEasingCurve>


InputTab::InputTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("InputTab{background-color: #252627;}");
    setFocusPolicy(Qt::ClickFocus);

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    DTK_WIDGET_NAMESPACE::DPasswordEdit *pwdEdit = new DTK_WIDGET_NAMESPACE::DPasswordEdit(this);
    pwdEdit->setText("password");
    leftLayout->addWidget(pwdEdit);
    pwdEdit->setContextMenuPolicy(Qt::DefaultContextMenu);

    DTK_WIDGET_NAMESPACE::DPasswordEdit *pwdEdit2 = new DTK_WIDGET_NAMESPACE::DPasswordEdit(this);
    pwdEdit2->setText("password");
    pwdEdit2->setEchoMode(QLineEdit::Normal);

    pwdEdit2->showAlertMessage("this is an alert message...");
    leftLayout->addWidget(pwdEdit2);
    connect(pwdEdit2, &DTK_WIDGET_NAMESPACE::DPasswordEdit::focusChanged, [](bool focus) {qDebug() << "focus: " << focus;});
    connect(pwdEdit2, &DTK_WIDGET_NAMESPACE::DPasswordEdit::textChanged, [=](const QString &text) {
        qDebug() << "text: " << text << pwdEdit2->alertMessageAlignment();
        if (pwdEdit2->isAlert()) {
            pwdEdit2->setAlertMessageAlignment(Qt::AlignLeft);
        } else if (pwdEdit2->isEchoMode()) {
            pwdEdit2->setAlertMessageAlignment(Qt::AlignRight);
        } else {
            pwdEdit2->setAlertMessageAlignment(Qt::AlignCenter);
        }

        pwdEdit2->showAlertMessage("this is an alert message..."+text, nullptr);
        pwdEdit2->setAlert(!pwdEdit2->isAlert());
    });

    QComboBox *combo = new QComboBox(this);
    combo->setEditable(true);
    combo->addItem("left");
    combo->addItem("right");
    combo->addItem("center");
    combo->addItem("item");
    DTK_WIDGET_NAMESPACE::DAlertControl *ac = new DTK_WIDGET_NAMESPACE::DAlertControl(combo/*->lineEdit()*/, combo);
    ac->setMessageAlignment(Qt::AlignRight);

    layout->addWidget(combo, 0, Qt::AlignTop);
    combo->setMinimumWidth(150);

    QVariantAnimation *ma = new QVariantAnimation(combo);
    connect(ma, &QVariantAnimation::valueChanged, [combo](const QVariant &value){
        QPoint p = combo->pos();
        p.setX(value.toInt());
        combo->move(p);
    });
    ma->setDuration(6000);
    ma->setStartValue(550);
    ma->setEndValue(700);

    connect(combo, &QComboBox::editTextChanged,  [=](const QString &text){
        //ac->showAlertMessage("this is an alert message...");
        if (text == "left") {
            ac->setMessageAlignment(Qt::AlignLeft);
        } else if (text == "right") {
            ac->setMessageAlignment(Qt::AlignRight);
        } else if (text == "center") {
            ac->setMessageAlignment(Qt::AlignCenter);
        }

        ac->showAlertMessage("this is an alert message..."+text, combo, 10000);
        ac->setAlert(!ac->isAlert());

        combo->move(550, 150);
        ma->start();
    });


    DTK_WIDGET_NAMESPACE::DSearchEdit *searchEdit = new DTK_WIDGET_NAMESPACE::DSearchEdit(this);
    //searchEdit->setSearchIcon(":/images/button.png");
    //searchEdit->setFixedWidth(300);
    leftLayout->addWidget(searchEdit);
    searchEdit->setPlaceHolder("Tes");
    //    Dtk::Widget::DThemeManager::instance()->setTheme(searchEdit, "dark");

    DTK_WIDGET_NAMESPACE::DLineEdit *lineEdit = new DTK_WIDGET_NAMESPACE::DLineEdit(this);
    lineEdit->setText("Test Alert Message");
    leftLayout->addWidget(lineEdit);
    //    lineEdit->setAlert(true);
    //    lineEdit->setFixedSize(200, 30);
    connect(lineEdit, &DTK_WIDGET_NAMESPACE::DLineEdit::focusChanged, [](bool focus) {qDebug() << "focus: " << focus;});
    QTimer::singleShot(2000, nullptr, [=] {lineEdit->showAlertMessage("Test Alert Message !!");});

    DTK_WIDGET_NAMESPACE::DFileChooserEdit *fileChooser = new DTK_WIDGET_NAMESPACE::DFileChooserEdit(this);
    rightLayout->addWidget(fileChooser);
    fileChooser->setDialogDisplayPosition(DTK_WIDGET_NAMESPACE::DFileChooserEdit::CurrentMonitorCenter);

    DTK_WIDGET_NAMESPACE::DLineEdit *lineEditAlert = new DTK_WIDGET_NAMESPACE::DLineEdit(this);
    lineEditAlert->setText("AlertLineEdit");
    lineEditAlert->setAlert(true);
    lineEditAlert->setFixedSize(200, 30);
    leftLayout->addWidget(lineEditAlert);
    lineEditAlert->lineEdit()->setValidator(new QRegExpValidator(QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){0,3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?")));

    //    DTK_WIDGET_NAMESPACE::DShortcutEdit *shortcutEdit = new DTK_WIDGET_NAMESPACE::DShortcutEdit(this);
    //    shortcutEdit->move(20, 300);

    DTK_WIDGET_NAMESPACE::DIpv4LineEdit *ipv4 = new DTK_WIDGET_NAMESPACE::DIpv4LineEdit(this);

    rightLayout->addWidget(ipv4);

    connect(ipv4, &DTK_WIDGET_NAMESPACE::DIpv4LineEdit::textChanged, this, [ipv4](const QString &text) {
        qDebug() << "text changed:" <<text << ipv4->text();
    });

    ipv4->setText("192.168.31.55");

    DTK_WIDGET_NAMESPACE::DSpinBox *spinbox = new DTK_WIDGET_NAMESPACE::DSpinBox(this);

    spinbox->resize(100, 22);

    rightLayout->addWidget(spinbox);

    DTK_WIDGET_NAMESPACE::DCrumbEdit* crumbEdit = new DTK_WIDGET_NAMESPACE::DCrumbEdit(this);
    crumbEdit->resize(100, 50);
//    crumbEdit->setDualClickCreateCrumb(true);
//    crumbEdit->setCrumbReadOnly(true);
    rightLayout->addWidget(crumbEdit);

    //    searchEdit->setFocus();
    lineEditAlert->setFocus();
}

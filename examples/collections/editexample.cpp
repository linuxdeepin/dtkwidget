// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "editexample.h"

#include <DSearchEdit>
#include <DLineEdit>
#include <DIpv4LineEdit>
#include <DPasswordEdit>
#include <DFileChooserEdit>
#include <DSpinBox>
#include <DTextEdit>
#include <DCrumbEdit>
#include <DKeySequenceEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QShortcut>
#include <QDebug>

DWIDGET_USE_NAMESPACE

EditExampleWindow::EditExampleWindow(QWidget *parent)
    : PageWindowInterface(parent)
{
    addExampleWindow(new DSearchEditExample(this));
    addExampleWindow(new DLineEditExample(this));
    addExampleWindow(new DIpv4LineEditExample(this));
    addExampleWindow(new DPasswordEditExample(this));
    addExampleWindow(new DFileChooserEditExample(this));
    addExampleWindow(new DSpinBoxExample(this));
    addExampleWindow(new DTextEditExample(this));
    addExampleWindow(new DCrumbTextFormatExample(this));
    addExampleWindow(new DKeySequenceEditExample(this));
}

DSearchEditExample::DSearchEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DSearchEdit *edit = new DSearchEdit(this);
    edit->setFixedWidth(340);
    edit->lineEdit()->setClearButtonEnabled(true);
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DSearchEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DSearchEditExample::getTitleName() const
{
    return "DSearchEdit";
}

QString DSearchEditExample::getDescriptionInfo() const
{
    return "用户工具栏或者主要区域的搜索框。配\n"
           "合一起使用的会有补全菜单。输入文字\n"
           "后一定会有清除按钮";
}

int DSearchEditExample::getFixedHeight() const
{
    return 632;
}

DLineEditExample::DLineEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DLineEdit *edit = new DLineEdit(this);
    edit->setFixedWidth(340);
    edit->lineEdit()->setPlaceholderText("选填");
    edit->lineEdit()->setClearButtonEnabled(true);
    connect(edit->lineEdit(), &QLineEdit::textChanged, [edit](const QString & text) {
        if (text.size() < 2) {
            edit->setAlert(true);
            edit->showAlertMessage("字符不能少于2个");
        } else {
            edit->setAlert(false);
        }
    });
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DLineEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DLineEditExample::getTitleName() const
{
    return "DLineEdit";
}

QString DLineEditExample::getDescriptionInfo() const
{
    return "普通的单行文本输入框，一般有输入字\n"
           "数限制，但是字数限制需要根据实际情\n"
           "况来约定。\n"
           "输入内容后输入尾部有清空按钮。";
}

int DLineEditExample::getFixedHeight() const
{
    return 632;
}

DIpv4LineEditExample::DIpv4LineEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DIpv4LineEdit *edit = new DIpv4LineEdit(this);
    edit->setFixedWidth(340);
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DIpv4LineEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DIpv4LineEditExample::getTitleName() const
{
    return "DIpv4LineEdit";
}

QString DIpv4LineEditExample::getDescriptionInfo() const
{
    return "比较特殊的IP地址输入框，用的较少";
}

int DIpv4LineEditExample::getFixedHeight() const
{
    return 602;
}

DPasswordEditExample::DPasswordEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DPasswordEdit *edit = new DPasswordEdit(this);
    edit->setFixedWidth(340);
    edit->setText("0123456789");
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DPasswordEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(380, 230);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DPasswordEditExample::getTitleName() const
{
    return "DPasswordEdit";
}

QString DPasswordEditExample::getDescriptionInfo() const
{
    return "常见的密码输入框";
}

int DPasswordEditExample::getFixedHeight() const
{
    return 436;
}

DFileChooserEditExample::DFileChooserEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DFileChooserEdit *edit = new DFileChooserEdit(this);
    edit->setFixedWidth(340);
    edit->lineEdit()->setClearButtonEnabled(true);
    edit->setText("~/.ssh/ssh_keygin.key");
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DFileChooserEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DFileChooserEditExample::getTitleName() const
{
    return "DFileChooserEdit";
}

QString DFileChooserEditExample::getDescriptionInfo() const
{
    return "普通的文件选择输入框";
}

int DFileChooserEditExample::getFixedHeight() const
{
    return 632;
}

DSpinBoxExample::DSpinBoxExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DSpinBox *plusMinus = new DSpinBox(this);
    plusMinus->setFixedWidth(340);
    plusMinus->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    QHBoxLayout *upDownLayout = new QHBoxLayout();
    DSpinBox *upDown = new DSpinBox(this);
    upDown->setFixedWidth(248);
    upDown->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    upDown->setEnabledEmbedStyle(true);
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DSpinBox.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    upDownLayout->addStretch();
    upDownLayout->addWidget(upDown);
    upDownLayout->addSpacing(92);
    upDownLayout->addStretch();

    mainLayout->addWidget(plusMinus, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(10);
    mainLayout->addItem(upDownLayout);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DSpinBoxExample::getTitleName() const
{
    return "DSpinBox";
}

QString DSpinBoxExample::getDescriptionInfo() const
{
    return "常见的数值输入框，只能输入数字字\n"
           "符，且多数时候只能是整数，上面三个\n"
           "性质上是一样的，根据实际情况需要选\n"
           "择即可。";
}

int DSpinBoxExample::getFixedHeight() const
{
    return 814;
}

DTextEditExample::DTextEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DTextEdit *edit = new DTextEdit(this);
    edit->setFixedSize(340, 144);
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DTextEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DTextEditExample::getTitleName() const
{
    return "DTextEdit";
}

QString DTextEditExample::getDescriptionInfo() const
{
    return "多行文本输入框，在网页上常见，应用\n"
           "内使用相对较少。";
}

int DTextEditExample::getFixedHeight() const
{
    return 740;
}

DCrumbTextFormatExample::DCrumbTextFormatExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    DCrumbEdit *edit = new DCrumbEdit(this);
    edit->setFixedSize(340, 70);

    DCrumbTextFormat first = edit->makeTextFormat();
    first.setText("人物");
    DCrumbTextFormat second = edit->makeTextFormat();
    second.setText("儿童");
    DCrumbTextFormat third = edit->makeTextFormat();
    third.setText("照片");
    edit->insertCrumb(first);
    edit->insertCrumb(second);
    edit->insertCrumb(third);

    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DCrumbEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(300, 708);

    mainLayout->addWidget(edit, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

QString DCrumbTextFormatExample::getTitleName() const
{
    return "DCrumbEdit";
}

QString DCrumbTextFormatExample::getDescriptionInfo() const
{
    return "标签输入框，用的情况不多，目前主要"
           "\n是文管使用";
}

int DCrumbTextFormatExample::getFixedHeight() const
{
    return 948;
}

DKeySequenceEditExample::DKeySequenceEditExample(QWidget *parent)
    : ExampleWindowInterface(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *keyHLayout = new QHBoxLayout();
    QHBoxLayout *closeHLayout1 = new QHBoxLayout();
    QHBoxLayout *closeHLayout2 = new QHBoxLayout();
    setLayout(mainLayout);
    mainLayout->addLayout(keyHLayout);
    mainLayout->addLayout(closeHLayout1);
    mainLayout->addLayout(closeHLayout2);

    QLabel *keyLabel = new QLabel("切换键盘布局", this);
    keyLabel->setFixedSize(108, 19);
    keyLabel->setAlignment(Qt::AlignLeft);
    DKeySequenceEdit *keyEdit = new DKeySequenceEdit(this);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    keyEdit->setKeySequence(QKeySequence(Qt::CTRL + Qt::SHIFT));
#else
    keyEdit->setKeySequence(QKeySequence(Qt::CTRL | Qt::SHIFT));
#endif
    QLabel *closeLabel1 = new QLabel("关闭窗口", this);
    closeLabel1->setFixedSize(72, 19);
    closeLabel1->setAlignment(Qt::AlignLeft);
    DKeySequenceEdit *closeEdit1 = new DKeySequenceEdit(this);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    closeEdit1->setKeySequence(QKeySequence(Qt::ALT | Qt::Key_F4));
#else
    closeEdit1->setKeySequence(QKeySequence(Qt::ALT + Qt::Key_F4));
#endif
    QLabel *closeLabel2 = new QLabel("关闭窗口", this);
    closeLabel2->setFixedSize(72, 19);
    closeLabel2->setAlignment(Qt::AlignLeft);
    DKeySequenceEdit *closeEdit2 = new DKeySequenceEdit(this);
    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("://images/example/DKeySequenceEdit.png"));
    label->setScaledContents(true);
    label->setFixedSize(550, 426);

    keyHLayout->addStretch();
    keyHLayout->addWidget(keyLabel);
    keyHLayout->addSpacing(220);
    keyHLayout->addWidget(keyEdit, 0, Qt::AlignRight);
    keyHLayout->addStretch();

    closeHLayout1->addStretch();
    closeHLayout1->addWidget(closeLabel1);
    closeHLayout1->addSpacing(260);
    closeHLayout1->addWidget(closeEdit1, 0, Qt::AlignRight);
    closeHLayout1->addStretch();

    closeHLayout2->addStretch();
    closeHLayout2->addWidget(closeLabel2);
    closeHLayout2->addSpacing(260);
    closeHLayout2->addWidget(closeEdit2, 0, Qt::AlignRight);
    closeHLayout2->addStretch();

    mainLayout->addSpacing(76);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);

    QShortcut *shortCut = new QShortcut(this);
    shortCut->setKey(QKeySequence(keyEdit->keySequence()));
    connect(shortCut, &QShortcut::activated, this, [shortCut](){
        qWarning() << "shorcut is trigger" << shortCut->key();
    });
    connect(keyEdit, &DKeySequenceEdit::editingFinished, this, [shortCut](QKeySequence key){
        shortCut->setKey(key);
    });
}

QString DKeySequenceEditExample::getTitleName() const
{
    return "DKeySequenceEdit";
}

QString DKeySequenceEditExample::getDescriptionInfo() const
{
    return "应用设置和控制中心部分设置快捷键的\n"
           "地方。";
}

int DKeySequenceEditExample::getFixedHeight() const
{
    return 724;
}

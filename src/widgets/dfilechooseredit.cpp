// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfilechooseredit.h"
#include "private/dfilechooseredit_p.h"

#include <DStyleHelper>
#include <DSuggestButton>

#include <QScreen>
#include <QGuiApplication>
#include <private/qguiapplication_p.h>
#include <QWindow>
DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DFileChooserEdit
  \inmodule dtkwidget
  \brief 带有选择文件按钮的文本编辑框.
  
  本控件基本与 DLineEdit 相同，但同时在编辑框的右侧提供了一个按钮，点击按钮将会出现一个选择文件的对话框，当在对话框中选择完毕点击确定之后，选择的结果将会出现在文本编辑框中。
  另外还提供了设置对话框出现的位置，选择文件的类型，或设置文件名过滤器的方法以定制控件的功能。
  
  \sa DLineEdit QFileDialog
 */

/*!
  \enum Dtk::Widget::DFileChooserEdit::DialogDisplayPosition
  \brief 这个枚举保存了对话框可以出现的位置
  
  \value FollowParentWindow
  跟随父窗口
  
  \value CurrentMonitorCenter
  鼠标所在的显示器的中心
 */

// =========================Signals begin=========================
/*!
  \fn void DFileChooserEdit::fileChoosed(const QString &fileName)
  \brief 这个信号在文件被选择且点击了对话框的确认按钮之后被调用
  \a fileName 被选中的文件名，包含其绝对路径
 */

/*!
  \fn void DFileChooserEdit::dialogOpened()
  \brief 这个信号在对话框即将显示时被调用
  \note 注意，此时对话框并没有显示
 */

/*!
  \fn void DFileChooserEdit::dialogClosed(int code)
  \brief 这信号在对话框关闭时被调用，无论对话框是被点击了确认还是取消，都会调用本信号
  \a code 对话框的返回码，返回码表示了对话框是因为点击了取消还是确认而关闭的
  \sa QDialog::DialogCode
 */
// =========================Signals end=========================

/*!
  \brief 获取 DFileChooserEdit 的一个实例
  \a parent 作为实例的父控件
 */
DFileChooserEdit::DFileChooserEdit(QWidget *parent)
    : DLineEdit(*new DFileChooserEditPrivate(this), parent)
{
    D_D(DFileChooserEdit);

    d->init();
}

/*!
  \brief 这个属性保存文件选择对话框将会出现的位置
  
  可选值为枚举 DFileChooserEdit::DialogDisplayPosition 中的值
  
  Getter: DFileChooserEdit::dialogDisplayPosition , Setter: DFileChooserEdit::setDialogDisplayPosition
  
  \sa DFileChooserEdit::DialogDisplayPosition
 */
DFileChooserEdit::DialogDisplayPosition DFileChooserEdit::dialogDisplayPosition() const
{
    D_DC(DFileChooserEdit);

    return d->dialogDisplayPosition;
}

/*!
  \brief 设置对话框显示位置.

  \a dialogDisplayPosition 对话框的显示位置.

  \sa DFileChooserEdit::dialogDisplayPosition
 */
void DFileChooserEdit::setDialogDisplayPosition(DFileChooserEdit::DialogDisplayPosition dialogDisplayPosition)
{
    D_D(DFileChooserEdit);

    d->dialogDisplayPosition = dialogDisplayPosition;
}

void DFileChooserEdit::setFileDialog(QFileDialog *fileDialog)
{
    D_D(DFileChooserEdit);

    d->dialog = fileDialog;
}

QFileDialog *DFileChooserEdit::fileDialog() const
{
    D_DC(DFileChooserEdit);

    return d->dialog;
}

void DFileChooserEdit::initDialog()
{
    D_D(DFileChooserEdit);

    if (d->dialog) {
        return;
    }

    d->dialog = new QFileDialog(this);
    d->dialog->setAcceptMode(QFileDialog::AcceptOpen);
    d->dialog->setFileMode(QFileDialog::ExistingFile);
}

/*!
  \brief 设置文件选择模式
  \a mode 要使用的模式
  \sa DFileChooserEdit::fileMode
 */
void DFileChooserEdit::setFileMode(QFileDialog::FileMode mode)
{
    D_D(DFileChooserEdit);

    if (!d->dialog) {
        initDialog();
    }
    d->dialog->setFileMode(mode);
}

/*!
  \brief 获取对话框选择文件模式
  
  有多种类型的选择模式，也就是说对话框可以有多种显示或行为，例如选择单个文件，选择多个文件亦或选择一个目录等，详细可以查阅：QFileDialog::FileMode
  \return 返回但前的选择模式
  \sa QFileDialog::FileMode
  \note 目前本控件只支持选择单个文件，即便调用 DFileChooserEdit::setFileMode 设置了选择模式，当有多个文件在对话框中被选中时，取其第一个作为选择结果
 */
QFileDialog::FileMode DFileChooserEdit::fileMode() const
{
    D_DC(DFileChooserEdit);

    if (!d->dialog) {
        return QFileDialog::FileMode::AnyFile;
    }
    return d->dialog->fileMode();
}

/*!
  \brief 设置文件名过滤器
  \a filters 要使用的文件名过滤器组成的列表
  \sa DFileChooserEdit::nameFilters
 */
void  DFileChooserEdit::setNameFilters(const QStringList &filters)
{
    D_D(DFileChooserEdit);

    if (!d->dialog) {
        initDialog();
    }
    d->dialog->setNameFilters(filters);
}

/*!
  \brief 文件名过滤器
  
  默认此选项为空，即所有文件都可以被选择，当文件名过滤器被设置后，则只有文件名与过滤器匹配的文件可以被选择，
  例如：设置了"*.txt"，则表示只有后缀名为"txt"的文件可以被选择，
  或者同时设置了多个过滤器：QStringList() << "text file (*.txt)" << "picture file (*.png);
  则会在文件选择对话框的下方出现设置的多个过滤选项，只是需要注意，一次只能使用一个过滤选项，也就是说不能同时即允许选择txt文件又允许选择png文件
  \return 返回当前的文件名过滤器组成的列表
  \sa DFileChooserEdit::setNameFilters
 */
QStringList DFileChooserEdit::nameFilters() const
{
    D_DC(DFileChooserEdit);

    if (!d->dialog) {
        return QStringList();
    }
    return d->dialog->nameFilters();
}

void DFileChooserEdit::setDirectoryUrl(const QUrl &directory)
{
    D_D(DFileChooserEdit);

    if (!d->dialog) {
        initDialog();
    }
    d->dialog->setDirectoryUrl(directory);
}

QUrl DFileChooserEdit::directoryUrl()
{
    D_D(DFileChooserEdit);

    if (!d->dialog) {
        initDialog();
    }
    return d->dialog->directoryUrl();
}


DFileChooserEditPrivate::DFileChooserEditPrivate(DFileChooserEdit *q)
    : DLineEditPrivate(q)
{
}

void DFileChooserEditPrivate::init()
{
    D_Q(DFileChooserEdit);

    QList<QWidget *> list;
    DSuggestButton *btn = new DSuggestButton(nullptr);
    btn->setAccessibleName("DFileChooserEditSuggestButton");
    btn->setIcon(DStyleHelper(q->style()).standardIcon(DStyle::SP_SelectElement, nullptr));
    btn->setIconSize(QSize(24, 24));

    q->setDialogDisplayPosition(DFileChooserEdit::DialogDisplayPosition::CurrentMonitorCenter);

    list.append(btn);

    q->setRightWidgets(list);
    q->setClearButtonEnabled(true);

    q->connect(btn, SIGNAL(clicked()), q, SLOT(_q_showFileChooserDialog()));
}

void DFileChooserEditPrivate::_q_showFileChooserDialog()
{
    D_Q(DFileChooserEdit);

    if (!dialog) {
        q->initDialog();
    }

    if (dialogDisplayPosition == DFileChooserEdit::CurrentMonitorCenter) {
        QPoint pos = QCursor::pos();

        for (QScreen *screen : qApp->screens()) {
            if (screen->geometry().contains(pos)) {
                QRect rect = dialog->geometry();
                rect.moveCenter(screen->geometry().center());
                dialog->move(rect.topLeft());
                break;
            }
        }
    }

    q->dialogOpened();

    if (!dialog) {
        qWarning("init filedialog failed!!");
        return;
    }

    // 多次打开时有时会出现 filedialog 不显示的问题 exec 前确保没显示否则不调用 helper->show
    if (dialog->isVisible())
        dialog->setVisible(false);

    int code = dialog->exec();

    if (code == QDialog::Accepted && !dialog->selectedFiles().isEmpty()) {
        const QString fileName = dialog->selectedFiles().first();

        q->setText(fileName);
        Q_EMIT q->fileChoosed(fileName);
    }

    // exec 后如果 filedialog 还阻塞了应用就要隐藏掉
    if (qApp->modalWindow() == dialog->windowHandle())
        QGuiApplicationPrivate::hideModalWindow(dialog->windowHandle());

    q->dialogClosed(code);
}

DWIDGET_END_NAMESPACE

#include "moc_dfilechooseredit.cpp"

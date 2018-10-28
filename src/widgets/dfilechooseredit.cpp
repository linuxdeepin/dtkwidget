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

#include "dfilechooseredit.h"
#include "dthememanager.h"
#include "private/dfilechooseredit_p.h"

#include <QDebug>
#include <QFileDialog>
#include <QScreen>
#include <QGuiApplication>

DWIDGET_BEGIN_NAMESPACE


/*!
 * \~chinese \class DFileChooserEdit
 * \~chinese \brief 带有选择文件按钮的文本编辑框
 *
 * \~chinese 本控件基本与 DLineEdit 相同，但同时在编辑框的右侧提供了一个按钮，点击按钮将会出现一个选择文件的对话框，当在对话框中选择完毕点击确定之后，选择的结果将会出现在文本编辑框中。
 * \~chinese 另外还提供了设置对话框出现的位置，选择文件的类型，或设置文件名过滤器的方法以定制控件的功能。
 *
 * \~chinese \sa DLineEdit QFileDialog
 */

/*!
 * \~chinese \enum DFileChooserEdit::DialogDisplayPosition
 * \~chinese \brief 这个枚举保存了对话框可以出现的位置
 *
 * \~chinese \var DFileChooserEdit::DialogDisplayPosition DFileChooserEdit::FollowParentWindow
 * \~chinese 跟随父窗口
 *
 * \~chinese \var DFileChooserEdit::DialogDisplayPosition DFileChooserEdit::CurrentMonitorCenter
 * \~chinese 鼠标所在的显示器的中心
 */

// =========================Signals begin=========================
/*!
 * \~chinese \fn DFileChooserEdit::fileChoosed
 * \~chinese \brief 这个信号在文件被选择且点击了对话框的确认按钮之后被调用
 * \~chinese \param fileName 被选中的文件名，包含其绝对路径
 */

/*!
 * \~chinese \fn DFileChooserEdit::dialogOpened
 * \~chinese \brief 这个信号在对话框即将显示时被调用
 * \~chinese \note 注意，此时对话框并没有显示
 */

/*!
 * \~chinese \fn DFileChooserEdit::dialogClosed
 * \~chinese \brief 这信号在对话框关闭时被调用，无论对话框是被点击了确认还是取消，都会调用本信号
 * \~chinese \param 对话框的返回码，返回码表示了对话框是因为点击了取消还是确认而关闭的
 * \~chinese \sa QDialog::DialogCode
 */
// =========================Signals end=========================

/*!
 * \~chinese \brief 获取 DFileChooserEdit 的一个实例
 * \~chinese \param parent 作为实例的父控件
 */
DFileChooserEdit::DFileChooserEdit(QWidget *parent)
    : DLineEdit(*new DFileChooserEditPrivate(this), parent)
{
    DThemeManager::registerWidget(this);
    D_D(DFileChooserEdit);

    d->init();
}

/*!
 * \~chinese \property DFileChooserEdit::dialogDisplayPosition
 * \~chinese \brief 这个属性保存文件选择对话框将会出现的位置
 *
 * \~chinese 可选值为枚举 DFileChooserEdit::DialogDisplayPosition 中的值
 *
 * \~chinese Getter: DFileChooserEdit::dialogDisplayPosition , Setter: DFileChooserEdit::setDialogDisplayPosition
 *
 * \~chinese \sa DFileChooserEdit::DialogDisplayPosition
 */
DFileChooserEdit::DialogDisplayPosition DFileChooserEdit::dialogDisplayPosition() const
{
    D_DC(DFileChooserEdit);

    return d->dialogDisplayPosition;
}

/*!
 * \~chinese \sa DFileChooserEdit::dialogDisplayPosition
 */
void DFileChooserEdit::setDialogDisplayPosition(DFileChooserEdit::DialogDisplayPosition dialogDisplayPosition)
{
    D_D(DFileChooserEdit);

    d->dialogDisplayPosition = dialogDisplayPosition;
}

/*!
 * \~chinese \brief 设置文件选择模式
 * \~chinese \param mode 要使用的模式
 * \~chinese \sa DFileChooserEdit::fileMode
 */
void DFileChooserEdit::setFileMode(QFileDialog::FileMode mode)
{
    D_D(DFileChooserEdit);
    d->fileMode = mode;
}

/*!
 * \~chinese \brief 获取对话框选择文件模式
 *
 * \~chinese 有多种类型的选择模式，也就是说对话框可以有多种显示或行为，例如选择单个文件，选择多个文件亦或选择一个目录等，详细可以查阅：QFileDialog::FileMode
 * \~chinese \return 返回但前的选择模式
 * \~chinese \sa QFileDialog::FileMode
 * \~chinese \note 目前本控件只支持选择单个文件，即便调用 DFileChooserEdit::setFileMode 设置了选择模式，当有多个文件在对话框中被选中时，取其第一个作为选择结果
 */
QFileDialog::FileMode DFileChooserEdit::fileMode() const
{
    D_DC(DFileChooserEdit);
    return d->fileMode;
}

/*!
 * \~chinese \brief 设置文件名过滤器
 * \~chinese \param filters 要使用的文件名过滤器组成的列表
 * \~chinese \sa DFileChooserEdit::nameFilters
 */
void  DFileChooserEdit::setNameFilters(const QStringList &filters)
{
    D_D(DFileChooserEdit);
    d->nameFilters = filters;
}

/*!
 * \~chinese \brief 文件名过滤器
 *
 * \~chinese 默认此选项为空，即所有文件都可以被选择，当文件名过滤器被设置后，则只有文件名与过滤器匹配的文件可以被选择，
 * \~chinese 例如：设置了"*.txt"，则表示只有后缀名为"txt"的文件可以被选择，
 * \~chinese 或者同时设置了多个过滤器：QStringList() << "text file (*.txt)" << "picture file (*.png);
 * \~chinese 则会在文件选择对话框的下方出现设置的多个过滤选项，只是需要注意，一次只能使用一个过滤选项，也就是说不能同时即允许选择txt文件又允许选择png文件
 * \~chinese \return 返回当前的文件名过滤器组成的列表
 * \~chinese \sa DFileChooserEdit::setNameFilters
 */
QStringList DFileChooserEdit::nameFilters() const
{
    D_DC(DFileChooserEdit);
    return d->nameFilters;
}


DFileChooserEditPrivate::DFileChooserEditPrivate(DFileChooserEdit *q)
    : DLineEditPrivate(q)
{
}

void DFileChooserEditPrivate::init()
{
    D_Q(DFileChooserEdit);

    q->setTextMargins(0, 0, 24, 0);
    q->setReadOnly(true);
    q->setIconVisible(true);
    q->connect(q, SIGNAL(iconClicked()), q, SLOT(_q_showFileChooserDialog()));
}

void DFileChooserEditPrivate::_q_showFileChooserDialog()
{
    D_Q(DFileChooserEdit);

    QFileDialog dialog(q);

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(fileMode);
    dialog.setNameFilters(nameFilters);

    if (dialogDisplayPosition == DFileChooserEdit::CurrentMonitorCenter) {
        QPoint pos = QCursor::pos();

        for (QScreen *screen : qApp->screens()) {
            if (screen->geometry().contains(pos)) {
                QRect rect = dialog.geometry();
                rect.moveCenter(screen->geometry().center());
                dialog.move(rect.topLeft());
                break;
            }
        }
    }

    q->dialogOpened();

    int code = dialog.exec();


    if (code == QDialog::Accepted && !dialog.selectedFiles().isEmpty()) {
        const QString fileName = dialog.selectedFiles().first();

        q->setText(fileName);
        Q_EMIT q->fileChoosed(fileName);
    }

    q->dialogClosed(code);
}

DWIDGET_END_NAMESPACE

#include "moc_dfilechooseredit.cpp"

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

#ifndef DFILECHOOSEREDIT_H
#define DFILECHOOSEREDIT_H

#include "dlineedit.h"
#include <QFileDialog>

DWIDGET_BEGIN_NAMESPACE

class DFileChooserEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DFileChooserEdit : public DLineEdit
{
    Q_OBJECT

    Q_ENUMS(DialogDisplayPosition)
    Q_PROPERTY(DialogDisplayPosition dialogDisplayPosition READ dialogDisplayPosition WRITE setDialogDisplayPosition)

public:
    enum DialogDisplayPosition{
        FollowParentWindow,
        CurrentMonitorCenter
    };

    DFileChooserEdit(QWidget *parent = nullptr);

    DialogDisplayPosition dialogDisplayPosition() const;

    void setFileMode(QFileDialog::FileMode mode);
    QFileDialog::FileMode fileMode() const;

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;

public Q_SLOTS:
    void setDialogDisplayPosition(DialogDisplayPosition dialogDisplayPosition);

Q_SIGNALS:
    void fileChoosed(const QString &fileName);
    void dialogOpened();
    void dialogClosed(int code);

protected:
    Q_DISABLE_COPY(DFileChooserEdit)
    D_DECLARE_PRIVATE(DFileChooserEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_showFileChooserDialog())
};

DWIDGET_END_NAMESPACE

#endif // DFILECHOOSEREDIT_H

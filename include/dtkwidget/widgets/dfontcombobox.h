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
#ifndef DFONTCOMBOBOX_H
#define DFONTCOMBOBOX_H

#include <dtkwidget_global.h>
#include <DComboBox>
#include <QFontComboBox>

QT_REQUIRE_CONFIG(fontcombobox);

DWIDGET_BEGIN_NAMESPACE

class DFontComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DFontComboBox : public DComboBox
{
    Q_OBJECT
    Q_PROPERTY(QFontDatabase::WritingSystem writingSystem READ writingSystem WRITE setWritingSystem)
    Q_PROPERTY(QFontComboBox::FontFilters fontFilters READ fontFilters WRITE setFontFilters)
    Q_PROPERTY(QFont currentFont READ currentFont WRITE setCurrentFont NOTIFY currentFontChanged)

public:
    explicit DFontComboBox(QWidget *parent = nullptr);
    virtual ~DFontComboBox() override;

    void setWritingSystem(QFontDatabase::WritingSystem);
    QFontDatabase::WritingSystem writingSystem() const;

    void setFontFilters(QFontComboBox::FontFilters filters);
    QFontComboBox::FontFilters fontFilters() const;

    QFont currentFont() const;
    virtual QSize sizeHint() const override;

public Q_SLOTS:
    void setCurrentFont(const QFont &f);

Q_SIGNALS:
    void currentFontChanged(const QFont &f);

protected:
    virtual bool event(QEvent *e) override;

private:
    Q_DISABLE_COPY(DFontComboBox)
    D_DECLARE_PRIVATE(DFontComboBox)
};

DWIDGET_END_NAMESPACE

#endif // DFONTCOMBOBOX_H

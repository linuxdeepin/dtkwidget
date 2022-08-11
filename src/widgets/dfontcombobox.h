// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

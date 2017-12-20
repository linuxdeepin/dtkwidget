/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DCRUMBEDIT_H
#define DCRUMBEDIT_H

#include <QTextEdit>

#include "dobject.h"
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DCrumbTextFormat : public QTextCharFormat
{
public:
    DCrumbTextFormat();

    QColor tagColor() const;
    void setTagColor(const QColor &color);
    QString text() const;
    void setText(const QString &text);
    QColor textColor() const;
    void setTextColor(const QColor &color);
    QBrush background() const;
    void setBackground(const QBrush &background);

protected:
    DCrumbTextFormat(int objectType);
    explicit DCrumbTextFormat(const QTextFormat &fmt);
    friend class CrumbObjectInterface;
    friend class DCrumbEdit;
    friend class DCrumbEditPrivate;
};

class DCrumbEditPrivate;
class DCrumbEdit : public QTextEdit, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool crumbReadOnly READ crumbReadOnly WRITE setCrumbReadOnly)

public:
    enum CrumbType {
        black = Qt::black,
        white = Qt::white,
        darkGray = Qt::darkGray,
        gray = Qt::gray,
        lightGray = Qt::lightGray,
        red = Qt::red,
        green = Qt::green,
        blue = Qt::blue,
        cyan = Qt::cyan,
        magenta = Qt::magenta,
        yellow = Qt::yellow,
        darkRed = Qt::darkRed,
        darkGreen = Qt::darkGreen,
        darkBlue = Qt::darkBlue,
        darkCyan = Qt::darkCyan,
        darkMagenta = Qt::darkMagenta,
        darkYellow = Qt::darkYellow
    };

    explicit DCrumbEdit(QWidget *parent = 0);

    bool insertCrumb(const DCrumbTextFormat &format, int pos = -1);
    bool insertCrumb(const QString &text, int pos = -1);
    bool appendCrumb(const DCrumbTextFormat &format);
    bool appendCrumb(const QString &text);

    bool containCrumb(const QString &text) const;
    QStringList crumbList() const;

    DCrumbTextFormat crumbTextFormat(const QString &text) const;
    DCrumbTextFormat makeTextFormat() const;
    DCrumbTextFormat makeTextFormat(CrumbType type) const;

    bool crumbReadOnly() const;

Q_SIGNALS:
    void crumbAdded(const QString &text);
    void crumbRemoved(const QString &text);
    void crumbListChanged();

public Q_SLOTS:
    void setCrumbReadOnly(bool crumbReadOnly);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    QMimeData *createMimeDataFromSelection() const override;
    bool canInsertFromMimeData(const QMimeData *source) const override;
    void insertFromMimeData(const QMimeData *source) override;

private:
    using QTextEdit::setDocument;
    using QTextEdit::document;
    using QTextEdit::setText;
    using QTextEdit::setHtml;
    using QTextEdit::setPlaceholderText;
    using QTextEdit::insertPlainText;
    using QTextEdit::insertHtml;
    using QTextEdit::append;

    D_DECLARE_PRIVATE(DCrumbEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_onDocumentLayoutChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_onCurrentPositionChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_onTextChanged())
};

DWIDGET_END_NAMESPACE

#endif // DCRUMBEDIT_H

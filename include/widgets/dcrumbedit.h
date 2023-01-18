// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCRUMBEDIT_H
#define DCRUMBEDIT_H

#include <QTextEdit>

#include <DObject>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DCrumbTextFormat : public QTextCharFormat
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
    int backgroundRadius() const;
    void setBackgroundRadius(int radius);

protected:
    DCrumbTextFormat(int objectType);
    explicit DCrumbTextFormat(const QTextFormat &fmt);
    friend class CrumbObjectInterface;
    friend class DCrumbEdit;
    friend class DCrumbEditPrivate;
};

class DCrumbEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DCrumbEdit : public QTextEdit, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool crumbReadOnly READ crumbReadOnly WRITE setCrumbReadOnly)
    Q_PROPERTY(int crumbRadius READ crumbRadius WRITE setCrumbRadius)
    Q_PROPERTY(QString splitter READ splitter WRITE setSplitter)
    Q_PROPERTY(bool dualClickMakeCrumb READ dualClickMakeCrumb WRITE setDualClickMakeCrumb)

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

    bool dualClickMakeCrumb() const Q_DECL_NOEXCEPT;
    bool crumbReadOnly() const;
    int crumbRadius() const;
    QString splitter() const;

Q_SIGNALS:
    void crumbAdded(const QString &text);
    void crumbRemoved(const QString &text);
    void crumbListChanged();

public Q_SLOTS:
    void setCrumbReadOnly(bool crumbReadOnly);
    void setCrumbRadius(int crumbRadius);
    void setSplitter(const QString &splitter);

    void setDualClickMakeCrumb(bool flag) Q_DECL_NOEXCEPT;

protected:
    bool event(QEvent *e) override;
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

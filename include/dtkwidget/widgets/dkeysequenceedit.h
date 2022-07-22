#ifndef DKEYSEQUENCEEDIT_H
#define DKEYSEQUENCEEDIT_H

#include <dobject.h>
#include <dtkwidget_global.h>

#include <QKeySequenceEdit>
#include <QLineEdit>

DWIDGET_BEGIN_NAMESPACE

class DKeySequenceEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DKeySequenceEdit : public QLineEdit, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DKeySequenceEdit)
    D_DECLARE_PRIVATE(DKeySequenceEdit)

public:
    explicit DKeySequenceEdit(QWidget *parent = nullptr);

    void clear();
    bool setKeySequence(const QKeySequence &keySequence);
    QKeySequence keySequence();
    void ShortcutDirection(Qt::AlignmentFlag alig);

    QString getKeySequence(QKeySequence sequence);

Q_SIGNALS:
    void editingFinished(const QKeySequence &keySequence);
    void keySequenceChanged(const QKeySequence &keySequence);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent *e) override;
};

DWIDGET_END_NAMESPACE

#endif // DKEYSEQUENCEEDIT_H



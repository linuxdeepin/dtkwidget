#include "DApplication"
#include "dkeysequenceedit.h"
#include "dstyle.h"

#include "private/dkeysequenceedit_p.h"

#include <QDebug>
#include <QKeyEvent>
#include <DPalette>
#include <DFrame>
#include <DTipLabel>

#include <QVBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DKeyLabel : public DFrame
{
public:
    DKeyLabel(QString text, QWidget *parent = nullptr)
        : DFrame(parent)
    {
        label = new DTipLabel(text);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(layout->contentsMargins().left(), 0, layout->contentsMargins().right(), 0);
        layout->setSpacing(0);
        label->setForegroundRole(DPalette::LightLively);
        layout->addWidget(label);
    }
private:
    DTipLabel *label;
};

class DKeyWidget : public QWidget
{
public:
    DKeyWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        label = new DTipLabel(qApp->translate("DKeySequenceEdit", "Enter a new shortcut"));

        layout->addWidget(label);
        setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    }

    void setKeyVisible(bool visible)    //true 隐藏文字 false显示文字
    {
        label->setVisible(!visible);
        keyHide(!visible);
        fastMode = visible;
    }

    bool isFastMode()
    {
        return fastMode;
    }

    void keyHide(bool hide)
    {
        for (int i = 0; i < labelList.count(); i++) {
            labelList.at(i)->setVisible(!hide);
        }
    }

    void keyDelete()
    {
        for (int i = 0; i < labelList.count(); i++) {
            labelList.at(i)->deleteLater();
        }
        labelList.clear();
    }

    void setKeyName(const QStringList &keyList);
private:
    DTipLabel *label;

    QList<DKeyLabel *> labelList;
    bool fastMode = false; //true 显示快捷键 false 显示文字简介
protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);

        if (fastMode) {
            setKeyVisible(false);
        }
    }

    void focusOutEvent(QFocusEvent *event) override
    {
        Q_UNUSED(event)

        if (fastMode) {
            return;
        } else {
            setKeyVisible(true);
            for (int i = 0; i < labelList.count(); i++) {
                labelList.at(i)->setVisible(true);
            }
            if (labelList.isEmpty())
                setKeyVisible(false);
        }
    }
};

DKeySequenceEdit::DKeySequenceEdit(QWidget *parent)
    : QLineEdit(parent)
    , DObject(*new DKeySequenceEditPrivate(this))
{
    Q_D(DKeySequenceEdit);

    d->init();
}

void DKeySequenceEdit::clear()
{
    D_D(DKeySequenceEdit);

    d->rightWidget->keyDelete();
    d->rightWidget->setKeyVisible(false);
}

bool DKeySequenceEdit::setKeySequence(const QKeySequence &keySequence)
{
    D_D(DKeySequenceEdit);

    QStringList keyText;
    keyText << keySequence.toString().split("+", QString::SkipEmptyParts);

    if (keyText.isEmpty()) {
        return false;
    }

    d->rightWidget->setKeyName(keyText);
    d->sequencekey = keySequence;
    Q_EMIT keySequenceChanged(keySequence);
    return true;
}

QKeySequence DKeySequenceEdit::keySequence()
{
    D_D(DKeySequenceEdit);
    return d->sequencekey;
}

void DKeySequenceEdit::ShortcutDirection(Qt::AlignmentFlag alig)
{
    if (alig == Qt::AlignLeft || alig == Qt::AlignRight) {
       layout()->setAlignment(alig);
       setAlignment(alig == Qt::AlignLeft ? Qt::AlignRight : Qt::AlignLeft);
    }
}

void DKeySequenceEdit::keyPressEvent(QKeyEvent *e)
{
    D_D(DKeySequenceEdit);

    if (d->rightWidget->isFastMode()) {
        return;
    }

    int nextKey = e->key();

    if (nextKey == Qt::Key_Control
            || nextKey == Qt::Key_Shift
            || nextKey == Qt::Key_Alt
            || nextKey == Qt::Key_unknown
            || nextKey == Qt::Key_Meta) {
        return;
    }

    QString modifiers = QKeySequence(e->modifiers()).toString();
    QString key =  QKeySequence(e->key()).toString();

    QKeySequence sequence(modifiers + key);
    bool flags = setKeySequence(sequence);
    if (!flags)
        return;

    d_func()->sequencekey = sequence;
    Q_EMIT editingFinished(sequence);
}

DKeySequenceEditPrivate::DKeySequenceEditPrivate(DKeySequenceEdit *q)
    : DObjectPrivate(q)
{
}

void DKeySequenceEditPrivate::init()
{
    Q_Q(DKeySequenceEdit);

    QHBoxLayout *layout = new QHBoxLayout(q);
    rightWidget = new DKeyWidget(q);

    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(rightWidget, 0, Qt::AlignRight);

    q->setReadOnly(true);
    q->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void DKeyWidget::setKeyName(const QStringList &keyList)
{
    keyDelete();

    for (QString key : keyList) {
        DKeyLabel *label = new DKeyLabel(key);
        layout()->addWidget(label);
        labelList.append(label);
    }
    setKeyVisible(true);
}

DWIDGET_END_NAMESPACE

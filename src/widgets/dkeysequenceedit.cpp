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
        label = new DLabel(text);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(layout->contentsMargins().left(), 0, layout->contentsMargins().right(), 0);
        layout->setSpacing(0);
        label->setForegroundRole(QPalette::Highlight);
        layout->addWidget(label);
    }
private:
    DLabel *label;
};

class DKeyWidget : public QWidget
{
public:
    DKeyWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        label = new DTipLabel(qApp->translate("DKeySequenceEdit", "Enter a new shortcut"));
        layout->setContentsMargins(layout->contentsMargins().left(), 0, layout->contentsMargins().right(), 0);
        layout->setAlignment(Qt::AlignVCenter);

        layout->addWidget(label);
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
        parentWidget()->setFocus();
        if (fastMode) {
            setKeyVisible(false);
        }
        QWidget::mousePressEvent(event);
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

/*!
 * \~chinese \brief DKeySequenceEdit::DKeySequenceEdit 一个快捷键编辑展示的控件
 * \~chinese \param parent
 */
DKeySequenceEdit::DKeySequenceEdit(QWidget *parent)
    : QLineEdit(parent)
    , DObject(*new DKeySequenceEditPrivate(this))
{
    Q_D(DKeySequenceEdit);

    d->init();
}

/*!
 * \~chinese \brief DKeySequenceEdit::clear 清空内容
 */
void DKeySequenceEdit::clear()
{
    D_D(DKeySequenceEdit);

    d->rightWidget->keyDelete();
    d->rightWidget->setKeyVisible(false);
    d->sequencekey = QKeySequence();
}

/*!
 * \~chinese \brief DKeySequenceEdit::setKeySequence 设定快捷键
 * \~chinese \param keySequence 快捷键
 * \~chinese \return true设定成功　false设定失败
 */
bool DKeySequenceEdit::setKeySequence(const QKeySequence &keySequence)
{
    D_D(DKeySequenceEdit);

    QStringList keyText;
    keyText << keySequence.toString().split("+", QString::SkipEmptyParts);

    if (keySequence.toString().contains("++")) {
        keyText << "+";
    }

    if (keyText.isEmpty() || (keyText.count() > 4)) {
        return false;
    }

    for (int i = 0; i < keyText.count(); ++i) {
        keyText[i] = d->replaceWriting(keyText[i]);
    }

    d->rightWidget->setKeyName(keyText);
    d->sequencekey = keySequence;
    Q_EMIT keySequenceChanged(keySequence);
    return true;
}

/*!
 * \~chinese \brief DKeySequenceEdit::keySequence 返回设定的快捷键
 * \~chinese \return
 */
QKeySequence DKeySequenceEdit::keySequence()
{
    D_D(DKeySequenceEdit);
    return d->sequencekey;
}

/*!
 * \~chinese \brief DKeySequenceEdit::ShortcutDirection　快捷键文本展示方向
 * \~chinese \param alig 设定方向的枚举值
 */
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
        return QLineEdit::keyPressEvent(e);
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

bool DKeySequenceEdit::event(QEvent *e)
{
    D_D(DKeySequenceEdit);
    if (e->type() == QEvent::FocusOut) {
        if (!d->sequencekey.isEmpty())
            d->rightWidget->setKeyVisible(true);
    }
    return QLineEdit::event(e);
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

    copywritingList.insert("PgUp", "PageUp");
    copywritingList.insert("PgDown", "PageDown");
    copywritingList.insert("Return", "Enter");
    copywritingList.insert("Meta", "Super");
}

QString DKeySequenceEditPrivate::replaceWriting(QString copywriting)
{
    return copywritingList.value(copywriting, copywriting);
}

void DKeyWidget::setKeyName(const QStringList &keyList)
{
    keyDelete();

    for (QString key : keyList) {
        DKeyLabel *label = new DKeyLabel(key);
        label->setMinimumHeight(24);
        layout()->addWidget(label);
        labelList.append(label);
    }
    setKeyVisible(true);
}

DWIDGET_END_NAMESPACE

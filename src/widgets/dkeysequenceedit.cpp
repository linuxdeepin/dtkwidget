// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DApplication"
#include "dkeysequenceedit.h"
#include "dstyle.h"
#include "dsizemode.h"

#include "private/dkeysequenceedit_p.h"

#include <QDebug>
#include <QKeyEvent>
#include <DPalette>
#include <DFrame>
#include <DTipLabel>
#include <DGuiApplicationHelper>
#include <DPaletteHelper>
#include <DStyle>

#include <QVBoxLayout>
#include "private/qkeymapper_p.h"

DWIDGET_BEGIN_NAMESPACE

class DKeyLabel : public DFrame
{
public:
    DKeyLabel(QString text, QWidget *parent = nullptr)
        : DFrame(parent)
    {
        label = new DLabel(text);
        label->setAccessibleName("DKeyLabelTextLabel");
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(layout->contentsMargins().left(), 0, layout->contentsMargins().right(), 0);
        layout->setSpacing(0);
        label->setForegroundRole(QPalette::ButtonText);
        layout->addWidget(label);
    }

    void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)
        QStyleOptionFrame opt;
        initStyleOption(&opt);
        QPainter p(this);
        drawShadow(&p, event->rect() - contentsMargins(), QColor(0, 0, 0, 20));

        opt.features |= QStyleOptionFrame::Rounded;

        const DPalette &dp = DPaletteHelper::instance()->palette(this);

        if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
            p.setBackground(QColor(255, 255, 255));
        } else {
            QColor bgColor(109, 109, 109);
            if ((opt.state & QStyle::State_Active) == 0) {
                auto inactive_mask_color = dp.color(QPalette::Window);
                inactive_mask_color.setAlphaF(0.6);
                bgColor = DGuiApplicationHelper::blendColor(bgColor, inactive_mask_color);
            }
            p.setBackground(bgColor);
        }

        p.setPen(QPen(dp.frameBorder(), opt.lineWidth));
        style()->drawControl(QStyle::CE_ShapedFrame, &opt, &p, this);
    }

    void drawShadow(QPainter *p, const QRect &rect, const QColor &color) const
    {
        DStyle dstyle;
        int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);
        int shadow_xoffset = dstyle.pixelMetric(DStyle::PM_ShadowHOffset);
        int shadow_yoffset = dstyle.pixelMetric(DStyle::PM_ShadowVOffset);

        QRect shadow = rect;
        QPoint pointOffset(rect.center().x() + shadow_xoffset, rect.center().y() + shadow_yoffset);
        shadow.moveCenter(pointOffset);

        p->setBrush(color);
        p->setPen(Qt::NoPen);
        p->setRenderHint(QPainter::Antialiasing);
        p->drawRoundedRect(shadow, frame_radius, frame_radius);
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
    void changeEvent(QEvent *event) override
    {
        if (event->type() == QEvent::StyleChange) {
            for (auto item : labelList) {
                item->setMinimumHeight(DSizeModeHelper::element(18, 24));
            }
        }
        return QWidget::changeEvent(event);
    }
};

/*!
  \brief DKeySequenceEdit::DKeySequenceEdit 一个快捷键编辑展示的控件
  \a parent
 */
DKeySequenceEdit::DKeySequenceEdit(QWidget *parent)
    : QLineEdit(parent)
    , DObject(*new DKeySequenceEditPrivate(this))
{
    Q_D(DKeySequenceEdit);

    d->init();
}

/*!
  \brief DKeySequenceEdit::clear 清空内容
 */
void DKeySequenceEdit::clear()
{
    D_D(DKeySequenceEdit);

    d->rightWidget->keyDelete();
    d->rightWidget->setKeyVisible(false);
    d->sequencekey = QKeySequence();
}

/*!
  \brief DKeySequenceEdit::setKeySequence 设定快捷键
  \a keySequence 快捷键
  \return true设定成功　false设定失败
 */
bool DKeySequenceEdit::setKeySequence(const QKeySequence &keySequence)
{
    D_D(DKeySequenceEdit);
    QString writing = getKeySequence(keySequence);
    QStringList keyText;
    keyText << writing.split("+", Qt::SkipEmptyParts);

    if (writing.contains("++")) {
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
  \brief DKeySequenceEdit::keySequence 返回设定的快捷键
  \return
 */
QKeySequence DKeySequenceEdit::keySequence()
{
    D_D(DKeySequenceEdit);
    return d->sequencekey;
}

/*!
  \brief DKeySequenceEdit::ShortcutDirection　快捷键文本展示方向
  \a alig 设定方向的枚举值
 */
void DKeySequenceEdit::ShortcutDirection(Qt::AlignmentFlag alig)
{
    if (alig == Qt::AlignLeft || alig == Qt::AlignRight) {
       layout()->setAlignment(alig);
       setAlignment(alig == Qt::AlignLeft ? Qt::AlignRight : Qt::AlignLeft);
    }
}

/*!
  \brief DKeySequenceEdit::getKeySequence 将Qt快捷键文案转换为Dtk文案
  \return Dtk文案
 */
QString DKeySequenceEdit::getKeySequence(QKeySequence sequence)
{
    D_D(DKeySequenceEdit);
    return d->replaceWriting(sequence.toString());
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

    if (e->modifiers() & Qt::ShiftModifier) {
        auto possibleKeys = QKeyMapper::possibleKeys(e);
        int pkTotal = possibleKeys.count();
        if (!pkTotal)
            return;
        bool found = false;
        for (int i = 0; i < possibleKeys.size(); ++i) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (possibleKeys.at(i).toCombined() - nextKey == static_cast<int>(e->modifiers())
                || (possibleKeys.at(i).toCombined() == nextKey && e->modifiers() == Qt::ShiftModifier)) {
                nextKey = possibleKeys.at(i).toCombined();
#else
            if (static_cast<int>(possibleKeys.at(i)) - nextKey == static_cast<int>(e->modifiers())
                || (possibleKeys.at(i) == nextKey && e->modifiers() == Qt::ShiftModifier)) {
                nextKey = possibleKeys.at(i);
#endif
                found = true;
                break;
            }
        }
        // Use as fallback
        if (!found)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            nextKey = possibleKeys.first().toCombined();
#else
            nextKey = possibleKeys.first();
#endif
    }

    QString modifiers = QKeySequence(e->modifiers()).toString();
    QString key =  QKeySequence(nextKey).toString();

    QKeySequence sequence(modifiers + key);
    bool flags = setKeySequence(sequence);
    if (!flags)
        return;

    d->sequencekey = sequence;
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

    rightWidget->setAccessibleName("DKeySequenceEditKeyWidget");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(rightWidget, 0, Qt::AlignRight);

    q->setReadOnly(true);

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
        label->setAccessibleName(QString("DKeyWidgetKeyLabelAt").append(key));
        label->setMinimumHeight(DSizeModeHelper::element(18, 24));
        layout()->addWidget(label);
        labelList.append(label);
    }
    setKeyVisible(true);
}

DWIDGET_END_NAMESPACE

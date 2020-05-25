#ifndef DTEXTEDIT_H
#define DTEXTEDIT_H

#include <QTextEdit>

#include <dtkwidget_global.h>
#include <DObject>

QT_BEGIN_NAMESPACE
class QContextMenuEvent;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE
class DTextEditPrivate;
class DTextEdit : public QTextEdit, public DCORE_NAMESPACE::DObject
{
public:
    explicit DTextEdit(QWidget *parent = nullptr);
    explicit DTextEdit(const QString& text, QWidget* parent = nullptr);

public:
    bool speechToTextIsEnabled() const;
    void setSpeechToTextEnabled(bool enable);

    bool textToSpeechIsEnabled() const;
    void setTextToSpeechEnabled(bool enable);

    bool textToTranslateIsEnabled() const;
    void setTextToTranslateEnabled(bool enable);

protected:
    bool event(QEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    D_DECLARE_PRIVATE(DTextEdit)
};

DWIDGET_END_NAMESPACE

#endif // DTEXTEDIT_H

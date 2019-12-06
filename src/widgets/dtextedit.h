#ifndef DTEXTEDIT_H
#define DTEXTEDIT_H

#include <QTextEdit>

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE
class DTextEditPrivate;
class DTextEdit : public QTextEdit, public DCORE_NAMESPACE::DObject
{
public:
    explicit DTextEdit(QWidget *parent = nullptr);
    explicit DTextEdit(const QString& text, QWidget* parent = nullptr);

protected:
    bool event(QEvent *e) override;

private:
    D_DECLARE_PRIVATE(DTextEdit)
};

DWIDGET_END_NAMESPACE

#endif // DTEXTEDIT_H

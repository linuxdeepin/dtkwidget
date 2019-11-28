#ifndef DSPINNER_H
#define DSPINNER_H

#include <QScopedPointer>
#include <QWidget>

#include <DObject>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DSpinnerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSpinner : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DSpinner(QWidget *parent = 0);
    ~DSpinner();

    bool isPlaying() const;

public Q_SLOTS:
    void start();
    void stop();
    void setBackgroundColor(QColor color);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *e) override;

private:
    D_DECLARE_PRIVATE(DSpinner)
};

DWIDGET_END_NAMESPACE

#endif

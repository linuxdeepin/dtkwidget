#ifndef DSPINNER_H
#define DSPINNER_H

#include <QScopedPointer>
#include <QWidget>

#include "dtkwidget_global.h"

class DSpinnerPrivate;
class DSpinner : public QWidget
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

private:
    QScopedPointer<DSpinnerPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DSpinner)
};

#endif

#ifndef DPAGEINDICATOR_H
#define DPAGEINDICATOR_H

#include "dwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DPageIndicatorPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPageIndicator : public QWidget, public DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DPageIndicator)
    D_DECLARE_PRIVATE(DPageIndicator)
    Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor DESIGNABLE true)
    Q_PROPERTY(QColor secondaryPointColor READ secondaryPointColor WRITE setSecondaryPointColor DESIGNABLE true)

public:
    explicit DPageIndicator(QWidget *parent = 0);

    int pageCount() const;
    void setPageCount(const int count);

    void nextPage();
    void previousPage();
    void setCurrentPage(const int index);
    int currentPageIndex() const;

    QColor pointColor() const;
    void setPointColor(QColor color);

    QColor secondaryPointColor() const;
    void setSecondaryPointColor(QColor color);

protected:
    void paintEvent(QPaintEvent *e) override;
};

DWIDGET_END_NAMESPACE

#endif // DPAGEINDICATOR_H

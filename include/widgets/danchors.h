// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DANCHORS_H
#define DANCHORS_H


#include <QObject>
#include <QPointer>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QWidget>

#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DAnchorsBase;
struct DAnchorInfo {
    DAnchorInfo(DAnchorsBase *b, const Qt::AnchorPoint &t):
        base(b),
        type(t)
    {
    }

    DAnchorsBase *base;
    Qt::AnchorPoint type;
    const DAnchorInfo *targetInfo = NULL;

    bool operator==(const DAnchorInfo *info) const
    {
        return info == targetInfo;
    }

    bool operator==(const DAnchorInfo &info) const
    {
        return &info == targetInfo;
    }

    bool operator!=(const DAnchorInfo *info) const
    {
        return info != targetInfo;
    }

    bool operator!=(const DAnchorInfo &info) const
    {
        return &info != targetInfo;
    }

    const DAnchorInfo &operator=(const DAnchorInfo *info)
    {
        targetInfo = info;

        return *this;
    }
};

class DAnchorsBasePrivate;
class DEnhancedWidget;
class DAnchorsBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget *target READ target CONSTANT)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(const DAnchorsBase *anchors READ anchors)
    Q_PROPERTY(const DAnchorInfo *top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(const DAnchorInfo *bottom READ bottom WRITE setBottom NOTIFY bottomChanged)
    Q_PROPERTY(const DAnchorInfo *left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(const DAnchorInfo *right READ right WRITE setRight NOTIFY rightChanged)
    Q_PROPERTY(const DAnchorInfo *horizontalCenter READ horizontalCenter WRITE setHorizontalCenter NOTIFY horizontalCenterChanged)
    Q_PROPERTY(const DAnchorInfo *verticalCenter READ verticalCenter WRITE setVerticalCenter NOTIFY verticalCenterChanged)
    Q_PROPERTY(QWidget *fill READ fill WRITE setFill NOTIFY fillChanged)
    Q_PROPERTY(QWidget *centerIn READ centerIn WRITE setCenterIn NOTIFY centerInChanged)
    Q_PROPERTY(int margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(int horizontalCenterOffset READ horizontalCenterOffset WRITE setHorizontalCenterOffset NOTIFY horizontalCenterOffsetChanged)
    Q_PROPERTY(int verticalCenterOffset READ verticalCenterOffset WRITE setVerticalCenterOffset NOTIFY verticalCenterOffsetChanged)
    Q_PROPERTY(bool alignWhenCentered READ alignWhenCentered WRITE setAlignWhenCentered NOTIFY alignWhenCenteredChanged)

public:
    explicit DAnchorsBase(QWidget *w);
    ~DAnchorsBase();

    enum AnchorError {
        NoError,
        Conflict,
        TargetInvalid,
        PointInvalid,
        LoopBind
    };

    QWidget *target() const;
    DEnhancedWidget *enhancedWidget() const;
    bool enabled() const;
    const DAnchorsBase *anchors() const;
    const DAnchorInfo *top() const;
    const DAnchorInfo *bottom() const;
    const DAnchorInfo *left() const;
    const DAnchorInfo *right() const;
    const DAnchorInfo *horizontalCenter() const;
    const DAnchorInfo *verticalCenter() const;
    QWidget *fill() const;
    QWidget *centerIn() const;
    int margins() const;
    int topMargin() const;
    int bottomMargin() const;
    int leftMargin() const;
    int rightMargin() const;
    int horizontalCenterOffset() const;
    int verticalCenterOffset() const;
    int alignWhenCentered() const;
    AnchorError errorCode() const;
    QString errorString() const;
    bool isBinding(const DAnchorInfo *info) const;

    static bool setAnchor(QWidget *w, const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point);
    static void clearAnchors(const QWidget *w);
    static DAnchorsBase *getAnchorBaseByWidget(const QWidget *w);

public Q_SLOTS:
    void setEnabled(bool enabled);
    bool setAnchor(const Qt::AnchorPoint &p, QWidget *target, const Qt::AnchorPoint &point);
    bool setTop(const DAnchorInfo *top);
    bool setBottom(const DAnchorInfo *bottom);
    bool setLeft(const DAnchorInfo *left);
    bool setRight(const DAnchorInfo *right);
    bool setHorizontalCenter(const DAnchorInfo *horizontalCenter);
    bool setVerticalCenter(const DAnchorInfo *verticalCenter);
    bool setFill(QWidget *fill);
    bool setCenterIn(QWidget *centerIn);
    bool setFill(DAnchorsBase *fill);
    bool setCenterIn(DAnchorsBase *centerIn);
    void setMargins(int margins);
    void setTopMargin(int topMargin);
    void setBottomMargin(int bottomMargin);
    void setLeftMargin(int leftMargin);
    void setRightMargin(int rightMargin);
    void setHorizontalCenterOffset(int horizontalCenterOffset);
    void setVerticalCenterOffset(int verticalCenterOffset);
    void setAlignWhenCentered(bool alignWhenCentered);

    void setTop(int arg, Qt::AnchorPoint point);
    void setBottom(int arg, Qt::AnchorPoint point);
    void setLeft(int arg, Qt::AnchorPoint point);
    void setRight(int arg, Qt::AnchorPoint point);
    void setHorizontalCenter(int arg, Qt::AnchorPoint point);
    void setVerticalCenter(int arg, Qt::AnchorPoint point);

    void moveTop(int arg);
    void moveBottom(int arg);
    void moveLeft(int arg);
    void moveRight(int arg);
    void moveHorizontalCenter(int arg);
    void moveVerticalCenter(int arg);
    void moveCenter(const QPoint &arg);

private Q_SLOTS:
    void updateVertical();
    void updateHorizontal();
    void updateFill();
    void updateCenterIn();

Q_SIGNALS:
    void enabledChanged(bool enabled);
    void topChanged(const DAnchorInfo *top);
    void bottomChanged(const DAnchorInfo *bottom);
    void leftChanged(const DAnchorInfo *left);
    void rightChanged(const DAnchorInfo *right);
    void horizontalCenterChanged(const DAnchorInfo *horizontalCenter);
    void verticalCenterChanged(const DAnchorInfo *verticalCenter);
    void fillChanged(QWidget *fill);
    void centerInChanged(QWidget *centerIn);
    void marginsChanged(int margins);
    void topMarginChanged(int topMargin);
    void bottomMarginChanged(int bottomMargin);
    void leftMarginChanged(int leftMargin);
    void rightMarginChanged(int rightMargin);
    void horizontalCenterOffsetChanged(int horizontalCenterOffset);
    void verticalCenterOffsetChanged(int verticalCenterOffset);
    void alignWhenCenteredChanged(bool alignWhenCentered);

protected:
    void init(QWidget *w);

private:
    DAnchorsBase(QWidget *w, bool);

    QExplicitlySharedDataPointer<DAnchorsBasePrivate> d_ptr;

    Q_DECLARE_PRIVATE(DAnchorsBase)
};

template<class T>
class DAnchors : public DAnchorsBase
{
public:
    inline DAnchors(): DAnchorsBase((QWidget*)NULL), m_widget(NULL) {}
    inline DAnchors(T *w): DAnchorsBase(w), m_widget(w) {}
    inline DAnchors(const DAnchors &me): DAnchorsBase(me.m_widget), m_widget(me.m_widget) {}

    inline T &operator=(const DAnchors &me)
    {
        m_widget = me.m_widget;
        init(m_widget);
        return *m_widget;
    }
    inline T &operator=(T *w)
    {
        m_widget = w;
        init(w);
        return *m_widget;
    }
    inline T *widget() const
    {
        return m_widget;
    }
    inline T *operator ->() const
    {
        return m_widget;
    }
    inline T &operator *() const
    {
        return *m_widget;
    }
    inline operator T *() const
    {
        return m_widget;
    }
    inline operator T &() const
    {
        return *m_widget;
    }

private:
    T *m_widget;
};

DWIDGET_END_NAMESPACE

#endif // DANCHORS_H

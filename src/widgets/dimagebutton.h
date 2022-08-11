// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIMAGEBUTTON_H
#define DIMAGEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE
class DImageButtonPrivate;
class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED_X("Use DIconButton") DImageButton : public QLabel, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(QString normalPic READ getNormalPic WRITE setNormalPic DESIGNABLE true)
    Q_PROPERTY(QString hoverPic READ getHoverPic WRITE setHoverPic DESIGNABLE true)
    Q_PROPERTY(QString pressPic READ getPressPic WRITE setPressPic DESIGNABLE true)
    Q_PROPERTY(QString checkedPic READ getCheckedPic WRITE setCheckedPic DESIGNABLE true)
    Q_PROPERTY(QString disabledPic READ getDisabledPic WRITE setDisabledPic DESIGNABLE true)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)

public:
    DImageButton(QWidget *parent = 0);

    DImageButton(const QString &normalPic, const QString &hoverPic,
                 const QString &pressPic, QWidget *parent = 0);

    DImageButton(const QString &normalPic, const QString &hoverPic,
                 const QString &pressPic, const QString &checkedPic, QWidget *parent = 0);

    ~DImageButton();

    void setEnabled(bool enabled);
    void setDisabled(bool disabled);

    void setChecked(bool flag);
    void setCheckable(bool flag);
    bool isChecked() const;
    bool isCheckable() const;

    void setNormalPic(const QString &normalPic);
    void setHoverPic(const QString &hoverPic);
    void setPressPic(const QString &pressPic);
    void setCheckedPic(const QString &checkedPic);
    void setDisabledPic(const QString &disabledPic);

    const QString getNormalPic() const;
    const QString getHoverPic() const;
    const QString getPressPic() const;
    const QString getCheckedPic() const;
    const QString getDisabledPic() const;

    enum State {
        Normal,
        Hover,
        Press,
        Checked,
        Disabled
    };

    void setState(State state);
    State getState() const;

Q_SIGNALS:
    void clicked();
    void checkedChanged(bool checked);
    void stateChanged();

protected:
    DImageButton(DImageButtonPrivate &q, QWidget *parent);
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DImageButton)
    D_DECLARE_PRIVATE(DImageButton)
};

DWIDGET_END_NAMESPACE

#endif // DIMAGEBUTTON_H

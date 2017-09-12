/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIMAGEBUTTON_H
#define DIMAGEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DImageButton : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString normalPic READ getNormalPic WRITE setNormalPic DESIGNABLE true)
    Q_PROPERTY(QString hoverPic READ getHoverPic WRITE setHoverPic DESIGNABLE true)
    Q_PROPERTY(QString pressPic READ getPressPic WRITE setPressPic DESIGNABLE true)
    Q_PROPERTY(QString checkedPic READ getCheckedPic WRITE setCheckedPic DESIGNABLE true)

public:
    DImageButton(QWidget * parent=0);

    DImageButton(const QString & normalPic, const QString & hoverPic,
                 const QString & pressPic, QWidget *parent = 0);

    DImageButton(const QString & normalPic, const QString & hoverPic,
                 const QString & pressPic, const QString & checkedPic, QWidget * parent = 0);

    ~DImageButton();

    void setChecked(bool flag);
    void setCheckable(bool flag);
    bool isChecked();
    bool isCheckable();

    void setNormalPic(const QString & normalPic);
    void setHoverPic(const QString & hoverPic);
    void setPressPic(const QString & pressPic);
    void setCheckedPic(const QString & checkedPic);

    inline const QString getNormalPic() const {return m_normalPic;}
    inline const QString getHoverPic() const {return m_hoverPic;}
    inline const QString getPressPic() const {return m_pressPic;}
    inline const QString getCheckedPic() const {return m_checkedPic;}

    enum State {Normal, Hover, Press, Checked};

    State getState() const;

Q_SIGNALS:
    void clicked();
    void stateChanged();

protected:
    void enterEvent(QEvent * event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void updateIcon();
    void setState(State state);
    QPixmap loadPixmap(const QString &path);

private:

    State m_state = Normal;

    bool m_isChecked = false;
    bool m_isCheckable = false;
    QString m_normalPic;
    QString m_hoverPic;
    QString m_pressPic;
    QString m_checkedPic;
};

DWIDGET_END_NAMESPACE

#endif // DIMAGEBUTTON_H

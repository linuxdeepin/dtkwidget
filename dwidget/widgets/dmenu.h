#ifndef DMENU_H
#define DMENU_H

#include <QObject>
#include <DObject>
#include "libdui_global.h"
#include <QMenu>

DWIDGET_BEGIN_NAMESPACE
class DMenuPrivate;
class DMenuItem;
class DMenu;

class LIBDTKWIDGETSHARED_EXPORT DMenu:  public QObject, public DObject
{
    Q_OBJECT
public:
    DMenu(DMenu *parent = nullptr);

    const QVariantMap toVariantMap() const;

    void attatch(QWidget *);

    void addItem(const QString &, const QString &text);
    void addItem(DMenuItem *);

    DMenuItem *getItem(const QString &);

    void show(const QPoint &pos);

Q_SIGNALS:
    void itemInvoked(const QString &itemId, bool checked);

private:
    void onMenuUnregistered();

    D_DECLARE_PRIVATE(DMenu)
};

class LIBDTKWIDGETSHARED_EXPORT DMenuItem: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString itemID READ itemID WRITE setItemID NOTIFY itemIDChanged)
    Q_PROPERTY(QString itemIcon READ itemIcon WRITE setItemIcon NOTIFY itemIconChanged)
    Q_PROPERTY(QString itemIconHover READ itemIconHover WRITE setItemIconHover NOTIFY itemIconHoverChanged)
    Q_PROPERTY(QString itemIconInactive READ itemIconInactive WRITE setItemIconInactive NOTIFY itemIconInactiveChanged)
    Q_PROPERTY(QString itemText READ itemText WRITE setItemText NOTIFY itemTextChanged)
    Q_PROPERTY(QString itemExtra READ itemExtra WRITE setItemExtra NOTIFY itemExtraChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
    Q_PROPERTY(bool checked READ checked WRITE setChecked NOTIFY checkedChanged)
    Q_PROPERTY(DMenu *itemSubMenu READ itemSubMenu WRITE setItemSubMenu NOTIFY itemSubMenuChanged)

public:
    DMenuItem(DMenu *parent);
    DMenuItem(const QString &, const QString &, DMenu *parent);

    const QVariantMap toVariantMap() const;

    bool checked() const { return m_checked; }
    bool isActive() const { return m_isActive; }
    QString itemExtra() const { return m_itemExtra; }
    QString itemText() const { return m_itemText; }
    QString itemIconInactive() const { return m_itemIconInactive; }
    QString itemIconHover() const { return m_itemIconHover; }
    QString itemIcon() const { return m_itemIcon; }
    QString itemID() const { return m_itemID; }
    DMenu *itemSubMenu() const {return m_itemSubMenu;}

public slots:
    void setItemSubMenu(DMenu *menu)
    {
        m_itemSubMenu = menu;
        m_itemSubMenu->setParent(parent());
    }

    void setChecked(bool checked)
    {
        if (m_checked == checked) {
            return;
        }

        m_checked = checked;
        emit checkedChanged(checked);
    }

    void setIsActive(bool isActive)
    {
        if (m_isActive == isActive) {
            return;
        }

        m_isActive = isActive;
        emit isActiveChanged(isActive);
    }

    void setItemExtra(QString itemExtra)
    {
        if (m_itemExtra == itemExtra) {
            return;
        }

        m_itemExtra = itemExtra;
        emit itemExtraChanged(itemExtra);
    }

    void setItemText(QString itemText)
    {
        if (m_itemText == itemText) {
            return;
        }

        m_itemText = itemText;
        emit itemTextChanged(itemText);
    }

    void setItemIconInactive(QString itemIconInactive)
    {
        if (m_itemIconInactive == itemIconInactive) {
            return;
        }

        m_itemIconInactive = itemIconInactive;
        emit itemIconInactiveChanged(itemIconInactive);
    }

    void setItemIconHover(QString itemIconHover)
    {
        if (m_itemIconHover == itemIconHover) {
            return;
        }

        m_itemIconHover = itemIconHover;
        emit itemIconHoverChanged(itemIconHover);
    }

    void setItemIcon(QString itemIcon)
    {
        if (m_itemIcon == itemIcon) {
            return;
        }

        m_itemIcon = itemIcon;
        emit itemIconChanged(itemIcon);
    }

    void setItemID(QString itemID)
    {
        if (m_itemID == itemID) {
            return;
        }

        m_itemID = itemID;
        emit itemIDChanged(itemID);
    }

signals:
    void checkedChanged(bool checked);
    void isActiveChanged(bool isActive);
    void itemExtraChanged(QString itemExtra);
    void itemTextChanged(QString itemText);
    void itemIconInactiveChanged(QString itemIconInactive);
    void itemIconHoverChanged(QString itemIconHover);
    void itemIconChanged(QString itemIcon);
    void itemIDChanged(QString itemID);
    void itemSubMenuChanged(DMenu *);

private:
    bool    m_checked;
    bool    m_isActive;
    QString m_itemExtra;
    QString m_itemText;
    QString m_itemIconInactive;
    QString m_itemIconHover;
    QString m_itemIcon;
    QString m_itemID;
    DMenu   *m_itemSubMenu;
};

DWIDGET_END_NAMESPACE

#endif // DMENU_H

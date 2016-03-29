#include "dmenu.h"

#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDebug>
#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>

#include "dbusmenu.h"
#include "dbusmenumanager.h"

#include <private/dobject_p.h>

#define MenuManager_service "com.deepin.menu"
#define MenuManager_path "/com/deepin/menu"

DWIDGET_BEGIN_NAMESPACE

DMenuItem::DMenuItem(DMenu *parent) : DMenuItem("Undefined", "Undefined", parent)
{
}

DMenuItem::DMenuItem(const QString &itemID, const QString &itemText, DMenu *parent) : QObject(parent)
{
    m_itemID    = itemID;
    m_itemText  = itemText;
    m_isActive  = true;
    m_checked   = false;
    m_itemSubMenu = nullptr;
}

const QVariantMap DMenuItem::toVariantMap() const
{
    QVariantMap menuItem;
    menuItem["itemId"]              = m_itemID;
    menuItem["itemIcon"]            = m_itemIcon;
    menuItem["itemIconHover"]       = m_itemIconHover;
    menuItem["itemIconInactive"]    = m_itemIconInactive;
    menuItem["itemText"]            = m_itemText;
    menuItem["itemExtra"]           = m_itemExtra;
    menuItem["isActive"]            = m_isActive;
    menuItem["checked"]             = m_checked;
    if (m_itemSubMenu) {
        menuItem["itemSubMenu"]     = m_itemSubMenu->toVariantMap();
    }
    return menuItem;
}


class DMenuPrivate: public DObjectPrivate
{
protected:
    DMenuPrivate(DMenu *qq);

private:
    DMenu                   *parent;
    MenumanagerInterface    *menuManager;
    MenuInterface           *menu;
    QVariantMap             menuVariant;
    QVector<DMenuItem *>    menuItems;
    QWidget                 *attatch;

    D_DECLARE_PUBLIC(DMenu)
};

DMenuPrivate::DMenuPrivate(DMenu *qq): DObjectPrivate(qq)
{
}

DMenu::DMenu(DMenu *parent): QObject(parent), DObject(* new DMenuPrivate(this))
{
    D_D(DMenu);
    d->parent = parent;
    d->menuManager = new MenumanagerInterface(MenuManager_service,
            MenuManager_path,
            QDBusConnection::sessionBus(),
            this);
    d->menu = nullptr;
    d->menuVariant["x"] = 0;
    d->menuVariant["y"] = 0;
    d->menuVariant["isDockMenu"] = false;
    d->menuVariant["menuJsonContent"] = "";
}

void DMenu::attatch(QWidget *)
{
    D_D(DMenu);
}

void DMenu::addItem(const QString &id, const QString &text)
{
    addItem(new DMenuItem(id, text, this));
}

void DMenu::addItem(DMenuItem *item)
{
    D_D(DMenu);
    d->menuItems.push_back(item);
}

DMenuItem *DMenu::getItem(const QString &itemID)
{
    D_D(DMenu);
    for (auto item : d->menuItems) {
        if (item->itemID() == itemID) {
            return item;
        }
    }
    return nullptr;
}

void DMenu::onMenuUnregistered()
{
    D_D(DMenu);
    if (d->menu) {
        d->menu->deleteLater();
        d->menu = nullptr;
    }
}

const QVariantMap DMenu::toVariantMap() const
{
    D_DC(DMenu);
    QVariantList items;
    QVariantMap menuJsonContent;
    QVariantMap menu = d->menuVariant;

    for (auto item : d->menuItems) {
        items.push_back(item->toVariantMap());
    }

    menuJsonContent["items"] = items;
    QJsonDocument menuJsonContentDoc = QJsonDocument::fromVariant(menuJsonContent);
    auto menuJsonContentStr = menuJsonContentDoc.toJson();
    menu["menuJsonContent"] = menuJsonContentStr;
    return menu;
}

void DMenu::show(const QPoint &pos)
{
    D_D(DMenu);
    if (d->parent) {
        qWarning() << "Only top DMenu can be show";
        return;
    }

    if (d->menu) {
        qWarning() << "Another menu is active";
        return;
    }
    d->menuVariant["x"] = pos.x();
    d->menuVariant["y"] = pos.y();

    QVariantMap content = this->toVariantMap();

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(toVariantMap());
    auto menuStr = jsonDoc.toJson();

    QDBusPendingReply<QDBusObjectPath> reply = d->menuManager->RegisterMenu();
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << "Call deepin-menu dbus failed: " << reply.error();
    }
    QString menuPath = reply.value().path();
    d->menu = new MenuInterface(MenuManager_service, menuPath, QDBusConnection::sessionBus(), this);
    d->menu->ShowMenu(menuStr);

    connect(d->menu, &MenuInterface::MenuUnregistered,
            this, &DMenu::onMenuUnregistered);

    connect(d->menu, &MenuInterface::ItemInvoked,
            this, &DMenu::itemInvoked);

}

DWIDGET_END_NAMESPACE

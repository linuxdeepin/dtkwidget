// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFEATUREDISPLAYDIALOG_H
#define DFEATUREDISPLAYDIALOG_H

#include <DDialog>

class QLabel;
DWIDGET_BEGIN_NAMESPACE

class DFeatureItemPrivate;
class DFeatureItem :public QObject,  public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DFeatureItem(const QIcon &icon = QIcon(), const QString &name = QString(),
                          const QString &description = QString(), QObject *parent = nullptr);
    ~DFeatureItem() override;

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

private:
    D_DECLARE_PRIVATE(DFeatureItem)
};

class DFeatureDisplayDialogPrivate;
class DFeatureDisplayDialog : public DDialog
{
    Q_OBJECT
public:
    explicit DFeatureDisplayDialog(QWidget *parent = nullptr);
    ~DFeatureDisplayDialog() override;

    void setTitle(const QString &title);
    void addItem(DFeatureItem *item);
    void removeItem(DFeatureItem* item);
    void addItems(QList<DFeatureItem*> items);
    void clearItems();
    void setLinkButtonVisible(bool isVisible);
    void setLinkUrl(const QString &url);
    void show();
    bool isEmpty() const;

private:
    D_DECLARE_PRIVATE(DFeatureDisplayDialog)
    D_PRIVATE_SLOT(void _q_toggleLinkBtn())

};

DWIDGET_END_NAMESPACE

#endif // DFEATUREDISPLAYDIALOG_H

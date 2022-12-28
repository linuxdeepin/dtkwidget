// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFEATUREDISPLAYDIALOG_P_H
#define DFEATUREDISPLAYDIALOG_P_H

#include "dfeaturedisplaydialog.h"
#include "ddialog_p.h"

DWIDGET_BEGIN_NAMESPACE
class DCommandLinkButton;

class DFeatureItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DFeatureItemWidget(const QIcon &icon = QIcon(), const QString &name = QString(), const QString &description = QString(), QWidget *parent = nullptr);
    virtual ~DFeatureItemWidget() override;

    void setDescriptionLabelWidth(const int width);
    int descriptionLabelWidth();

private:
    QLabel *m_iconLabel = nullptr;
    QLabel *m_featureNameLabel = nullptr;
    QLabel *m_featureDescriptionLabel = nullptr;
};

class DFeatureItemPrivate: public Core::DObjectPrivate
{
public:
    explicit DFeatureItemPrivate(Core::DObject *qq, const QIcon &icon,
                                 const QString &name = QString(), const QString &description = QString());
    ~DFeatureItemPrivate() override;

    QIcon m_icon;
    QString m_name;
    QString m_description;
};

class DFeatureDisplayDialogPrivate : public DDialogPrivate
{
protected:
    explicit DFeatureDisplayDialogPrivate(DFeatureDisplayDialog *qq);

private:
    void init();
    void addFeatureItem(const QIcon &icon, const QString &name, const QString &description);
    int getDescriptionMaxWidth();
    void updateItemWidth();
    void createWidgetItems();
    void deleteItems();
    void clearLayout();

private:
    QLabel *m_title = nullptr;
    QVBoxLayout *m_vBoxLayout = nullptr;
    DCommandLinkButton *m_linkBtn = nullptr;
    QString m_linkUrl;
    QList<QPointer<DFeatureItem>> m_featureItems;

    Q_DECLARE_PUBLIC(DFeatureDisplayDialog)

private:
    void _q_toggleLinkBtn();
};

DWIDGET_END_NAMESPACE

#endif // DFEATUREDISPLAYDIALOG_P_H

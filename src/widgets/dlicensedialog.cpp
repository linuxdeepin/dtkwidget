// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "private/dlicensedialog_p.h"
#include "private/dabstractdialogprivate_p.h"

#include <DFontSizeManager>
#include <DIconButton>
#include <DTitlebar>
#include <DListView>
#include <DHorizontalLine>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedLayout>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

DCORE_USE_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

const QString SystemLicenseDir = "/usr/share/spdx-licenses";
const QString CustomLicenseDir = "/usr/share/custom-licenses";
const QString SystemConfigDir = "/usr/share/deepin/credits";

class DLicenseDialog::DComponentInfoPrivate : public DObjectPrivate
{
public:
    QString name;
    QString version;
    QString copyRight;
    QString licenseName;

protected:
    explicit DComponentInfoPrivate(DLicenseDialog::DComponentInfo *qq)
        : DObjectPrivate(qq)
    {
    }

private:
    Q_DECLARE_PUBLIC(DLicenseDialog::DComponentInfo)
};

DLicenseDialog::DComponentInfo::DComponentInfo(DObject * parent)
    : DObject(*new DLicenseDialog::DComponentInfoPrivate(this), parent)
{

}

DLicenseDialog::DComponentInfo::~DComponentInfo()
{
}

QString DLicenseDialog::DComponentInfo::name() const
{
    return d_func()->name;
}

QString DLicenseDialog::DComponentInfo::version() const
{
    return d_func()->version;
}

QString DLicenseDialog::DComponentInfo::copyRight() const
{
    return d_func()->copyRight;
}

QString DLicenseDialog::DComponentInfo::licenseName() const
{
    return d_func()->licenseName;
}

class DLicenseDialogPrivate : public DAbstractDialogPrivate
{
public:
    explicit DLicenseDialogPrivate(DLicenseDialog *qq);
    ~DLicenseDialogPrivate();

private:
    void init();
    void addComponentItem(const DLicenseDialog::DComponentInfo *DComponentInfo);
    bool loadLicense();
    bool loadFile(const QString &file);
    bool loadContent(const QByteArray &content);
    QByteArray licenseContent(const QString &licenseName);
    void clear();

    DTitlebar          *titleBar = nullptr;
    DIconButton        *backwardBtn = nullptr;
    DListView          *listView = nullptr;
    QStandardItemModel *listModel = nullptr;
    QStackedLayout     *stackedLayout = nullptr;
    QLabel             *componentNameLabel = nullptr;
    QLabel             *componentVersionLabel = nullptr;
    QLabel             *copyRightLabel = nullptr;
    QLabel             *licenseContentLabel = nullptr;
    QScrollArea        *scrollArea = nullptr;
    bool isValid = false;
    DLicenseDialog::DComponentInfos componentInfos;

private:
    Q_DECLARE_PUBLIC(DLicenseDialog)
};

DLicenseDialogPrivate::DLicenseDialogPrivate(DLicenseDialog *qq)
    : DAbstractDialogPrivate(qq)
    , backwardBtn(new DIconButton(QStyle::SP_ArrowBack))
    , listView(new DListView)
    , listModel(new QStandardItemModel)
    , stackedLayout(new QStackedLayout)
    , componentNameLabel(new QLabel)
    , componentVersionLabel(new QLabel)
    , copyRightLabel(new QLabel)
    , licenseContentLabel(new QLabel)
    , scrollArea(new QScrollArea)
{
}

DLicenseDialogPrivate::~DLicenseDialogPrivate()
{
    clear();
}

void DLicenseDialogPrivate::init()
{
    D_Q(DLicenseDialog);
    q->setFixedSize(900, 800);

    titleBar = new DTitlebar();
    titleBar->setAccessibleName("DLicenseDialogTitleBar");
    titleBar->setMenuVisible(false);
    titleBar->setBackgroundTransparent(true);
    titleBar->setTitle(QObject::tr("Open-Source Software"));
    titleBar->addWidget(backwardBtn, Qt::AlignLeft | Qt::AlignVCenter);

    backwardBtn->setVisible(false);

    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setSelectionMode(QAbstractItemView::NoSelection);
    listView->setSpacing(0);
    listView->setItemSpacing(0);
    listView->setModel(listModel);
    listView->setAlternatingRowColors(true);

    DFontSizeManager *fontManager =  DFontSizeManager::instance();
    fontManager->bind(componentNameLabel, DFontSizeManager::T4, QFont::Bold);
    fontManager->bind(componentVersionLabel, DFontSizeManager::T6, QFont::DemiBold);
    fontManager->bind(copyRightLabel, DFontSizeManager::T6, QFont::DemiBold);

    licenseContentLabel->setWordWrap(true);

    QWidget *licenseWidget = new QWidget;
    QVBoxLayout *licenseLayout = new QVBoxLayout(licenseWidget);
    licenseLayout->setSpacing(0);
    licenseLayout->setMargin(20);
    licenseLayout->addWidget(componentNameLabel);
    licenseLayout->addSpacing(16);
    licenseLayout->addWidget(new DHorizontalLine);
    licenseLayout->addSpacing(16);
    licenseLayout->addWidget(componentVersionLabel);
    licenseLayout->addWidget(copyRightLabel);
    licenseLayout->addSpacing(40);
    licenseLayout->addWidget(licenseContentLabel);
    licenseLayout->addStretch(0);

    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->viewport()->setAutoFillBackground(false);
    scrollArea->setContentsMargins(QMargins(0,0,0,0));
    scrollArea->viewport()->setContentsMargins(QMargins(0,0,0,0));
    scrollArea->setWidget(licenseWidget);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    licenseWidget->setAutoFillBackground(false);

    stackedLayout->addWidget(listView);
    stackedLayout->addWidget(scrollArea);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->addWidget(titleBar, 0, Qt::AlignTop);
    mainLayout->addLayout(stackedLayout);

    q->setLayout(mainLayout);
    q->setFocusPolicy(Qt::ClickFocus);
    q->setFocus();

    QObject::connect(stackedLayout, &QStackedLayout::currentChanged, q, [this](int index) {
        backwardBtn->setVisible(index != 0);
    });
    QObject::connect(backwardBtn, &QAbstractButton::clicked, q, [this]{
        scrollArea->horizontalScrollBar()->setValue(0);
        scrollArea->verticalScrollBar()->setValue(0);
        stackedLayout->setCurrentIndex(0);
    });
    QObject::connect(listView, &QAbstractItemView::clicked, q, [this](const QModelIndex &index) {
        const auto &components = componentInfos;
        if (components.size() <= index.row() || index.row() < 0)
            return;

        auto componentInfo = components.at(index.row());

        componentNameLabel->setText(componentInfo->name());
        componentVersionLabel->setText(componentInfo->version());
        copyRightLabel->setText(componentInfo->copyRight());
        licenseContentLabel->setText(licenseContent(componentInfo->licenseName()));
        stackedLayout->setCurrentIndex(1);
    });
}

void DLicenseDialogPrivate::addComponentItem(const DLicenseDialog::DComponentInfo *componentInfo)
{
    D_Q(DLicenseDialog);
    auto pItem = new DStandardItem(componentInfo->name());
    pItem->setEditable(false);
    QSize size(12, 12);
    DViewItemAction *enterAction = new DViewItemAction(Qt::AlignVCenter, size, size, true);
    enterAction->setIcon(DStyle::standardIcon(q->style(), DStyle::SP_ArrowEnter));
    pItem->setActionList(Qt::RightEdge, DViewItemActionList() << enterAction);
    listModel->appendRow(pItem);
    const auto index = pItem->index();
    QObject::connect(enterAction, &DViewItemAction::triggered, enterAction, [this, index] {
        Q_EMIT listView->clicked(index);
    });
}

bool DLicenseDialogPrivate::loadLicense()
{
    QString file(QString("%1/%2.json").arg(SystemConfigDir).arg(qApp->applicationName()));
    isValid = loadFile(file);

    if (isValid) {
        listModel->clear();
        for (auto component : componentInfos) {
            addComponentItem(component);
        }
    }
    return isValid;
}

bool DLicenseDialogPrivate::loadFile(const QString &file)
{
    QFile jsonFile(file);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Failed on open file: \"%1\", error message: \"%2\"").arg(
            qPrintable(jsonFile.fileName()), qPrintable(jsonFile.errorString()));
        return false;
    }
    return loadContent(jsonFile.readAll());
}

bool DLicenseDialogPrivate::loadContent(const QByteArray &content)
{
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(content, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "When loading the license, parseJson failed:" << qPrintable(error.errorString());
        return false;
    }
    if (!jsonDoc.isArray()) {
        qWarning() << "When loading the license, parseJson failed: it is not a JSON array";
        return false;
    }

    clear();
    QJsonArray array = jsonDoc.array();
    for (const QJsonValue &value : array) {
        if (!value.isObject()) {
            qWarning() << "When loading the license, parseJson failed: it is not a JSON object!";
            return false;
        }
        DLicenseDialog::DComponentInfo *componentInfo = new DLicenseDialog::DComponentInfo;
        QJsonObject obj = value.toObject();
        QJsonValue name = obj.value("name");
        QJsonValue version = obj.value("version");
        QJsonValue copyright = obj.value("copyright");
        QJsonValue license = obj.value("license");
        if (!name.isString() || !version.isString()
            || !copyright.isString() || !license.isString()) {
            qWarning() << "When loading the license, parseJson failed: it is not a string!";
            return false;
        }
        componentInfo->d_func()->name = name.toString();
        componentInfo->d_func()->version = version.toString();
        componentInfo->d_func()->copyRight = copyright.toString();
        componentInfo->d_func()->licenseName = license.toString();
        componentInfos.append(componentInfo);
    }
    return true;
}

QByteArray DLicenseDialogPrivate::licenseContent(const QString &licenseName)
{
    QByteArray content;
    QStringList dirs{CustomLicenseDir, SystemLicenseDir};
    for (const QString &dir : dirs) {
        QFile file(QString("%1/%2.txt").arg(dir).arg(licenseName));
        if (!file.exists())
            continue;
        if (file.open(QIODevice::ReadOnly)) {
            content = file.readAll();
            file.close();
            break;
        }
    }
    if (content.isEmpty()) {
        qWarning() << QString("License content is empty when getting license content!");
    }
    return content;
}

void DLicenseDialogPrivate::clear()
{
    qDeleteAll(componentInfos);
    componentInfos.clear();
}

DLicenseDialog::DLicenseDialog(QWidget *parent)
    : DAbstractDialog(*new DLicenseDialogPrivate(this), parent)
{
    Q_D(DLicenseDialog);
    d->init();
}

DLicenseDialog::~DLicenseDialog()
{
}

bool DLicenseDialog::load()
{
    D_D(DLicenseDialog);
    return d->loadLicense();
}

bool DLicenseDialog::isValid() const
{
    D_DC(DLicenseDialog);
    return d->isValid;
}

void DLicenseDialog::hideEvent(QHideEvent *)
{
    D_D(DLicenseDialog);
    d->backwardBtn->setVisible(false);
    d->stackedLayout->setCurrentIndex(0);
    d->scrollArea->horizontalScrollBar()->setValue(0);
    d->scrollArea->verticalScrollBar()->setValue(0);
}
DWIDGET_END_NAMESPACE

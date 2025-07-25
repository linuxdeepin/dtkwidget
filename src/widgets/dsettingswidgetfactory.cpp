// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsettingswidgetfactory.h"

#include <QDebug>
#include <QMap>
#include <QFrame>
#include <QLabel>
#include <QEvent>
#include <QCheckBox>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QCoreApplication>
#include <QFormLayout>

#include <DSettingsOption>
#include <DSwitchButton>
#include <DFontSizeManager>
#include <DKeySequenceEdit>
#include <DSuggestButton>
#include <DButtonBox>
#include <DTipLabel>
#include <DDialog>

#include "private/settings/shortcutedit.h"
#include "private/settings/buttongroup.h"
#include "private/settings/combobox.h"
#include "private/settings/contenttitle.h"

DWIDGET_BEGIN_NAMESPACE

#define SHORTCUT_VALUE "shortcut_null"

class DSettingsWidgetFactoryPrivate;

class KeySequenceEdit : public DKeySequenceEdit
{
public:
    KeySequenceEdit(DTK_CORE_NAMESPACE::DSettingsOption *opt, QWidget *parent = nullptr)
        : DKeySequenceEdit(parent)
    {
        m_poption = opt;
    }
    DTK_CORE_NAMESPACE::DSettingsOption *option()
    {
        return m_poption;
    }
private:
    DTK_CORE_NAMESPACE::DSettingsOption *m_poption = nullptr;
};

static QMap<QString, KeySequenceEdit *> shortcutMap;

class ChangeDDialog : public DDialog
{
public:
    ChangeDDialog(QString key, KeySequenceEdit *edit, QString text = QString())
    {
        QPushButton *cancel = new QPushButton(qApp->translate("DSettingsDialog", "Cancel"));
        DSuggestButton *replace = new DSuggestButton(qApp->translate("DSettingsDialog", "Replace"));

        QString str = qApp->translate("DSettingsDialog", "This shortcut conflicts with %1, click on Add to make this shortcut effective immediately")
                      .arg(QString("<span style=\"color: rgba(255, 90, 90, 1);\">%1 %2</span>").arg(text).arg(QString("[%1]").arg(edit->getKeySequence(key))));
        setMessage(str);
        cancel->setAccessibleName("ChangeDDialogCancelButton");
        replace->setAccessibleName("ChangeDDialogReplaceButton");
        insertButton(1, cancel);
        insertButton(1, replace);
        connect(replace, &DSuggestButton::clicked, [ = ] {  //替换
            auto value = shortcutMap.value(key);
            value->option()->setValue(SHORTCUT_VALUE);
            shortcutMap.remove(key);

            edit->option()->setValue(key);
        });
        connect(cancel, &QPushButton::clicked, [ = ] {  //取消
            cancelSettings(edit);
        });
        connect(this, &DDialog::closed, [ = ] {
            cancelSettings(edit);
        });
        connect(this, &DDialog::rejected, [ = ] {
            cancelSettings(edit);
        });
    }
private:
    void cancelSettings(KeySequenceEdit *edit)
    {
        if (shortcutMap.key(edit).isEmpty()) {  //第一次被设置
            edit->clear();
        } else {
            edit->setKeySequence(edit->option()->value().toString());
        }
    }
};

/*!
  \class Dtk::Widget::DSettingsWidgetFactory
  \inmodule dtkwidget
  \brief DSettingsWidgetFactory是一个用于构造Deepin风格的设置对话框的控件.

  对于每种控件，均由一个id和对应的构造函数来组成，DSettingsWidgetFactory内置了许多基础的控件，包括：
  checkbox/lineedit/combobox/spinbutton/buttongroup/radiogroup/slider等。
  
 */

/*!
  \typedef DSettingsWidgetFactory::WidgetCreateHandler
  \brief 自定义的控件构建函数,一般接受一个 Dtk::Core::DSettingsOption 作为参数.
 */

#define PRIVATE_PROPERTY_translateContext "_d_DSettingsWidgetFactory_translateContext"

static inline QString tr(const QByteArray &translateContext, const char *sourceText)
{
    if (translateContext.isEmpty()) {
        return QObject::tr(sourceText);
    }

    return qApp->translate(translateContext, sourceText);
}

/*!
  \brief DSettingsWidgetFactory::createTwoColumWidget 返回一个水平布局的控件，
  左边为option的名称标签，右边为设置的自定义控件rightWidget.

  \a option 设置项，包含改配置的名称
  \a rightWidget 自定义控件
  \return 生成的水平布局配置项控件
  \sa Dtk::Widget::DSettingsWidgetFactory::createTwoColumWidget(const QByteArray &translateContext, Dtk::Core::DSettingsOption *option, QWidget *rightWidget)
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
QWidget *DSettingsWidgetFactory::createTwoColumWidget(DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget)
{
    return createTwoColumWidget(QByteArray(), option, rightWidget);
}
#endif

/*!
  \brief DSettingsWidgetFactory::createTwoColumWidget 返回一个水平布局的控件，
  左边为option的名称标签，右边为设置的自定义控件rightWidget, 可以通过translateContext设置翻译上下文.

  \a translateContext 自定义的翻译上下文，参考 QCoreApplication::translate();
  \a option 设置项，包含改配置的名称
  \a rightWidget 自定义控件
  \return 生成的水平布局配置项控件
  \sa Dtk::Widget::DSettingsWidgetFactory::createTwoColumWidget(Dtk::Core::DSettingsOption *option, QWidget *rightWidget)
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
QWidget *DSettingsWidgetFactory::createTwoColumWidget(const QByteArray &translateContext, DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget)
{
    auto optionFrame = new QWidget;
    optionFrame->setObjectName("OptionFrame");
    optionFrame->setAccessibleName("OptionFrame");

    auto optionLayout = new QFormLayout(optionFrame);
    optionLayout->setContentsMargins(0, 0, 0, 0);
    optionLayout->setSpacing(0);

    rightWidget->setMinimumWidth(240);

    if (!option->name().isEmpty()) {
        auto trName = DWIDGET_NAMESPACE::tr(translateContext, option->name().toStdString().c_str());
        optionLayout->addRow(trName, rightWidget);
    } else {
        optionLayout->addWidget(rightWidget);
    }

    return  optionFrame;
}
#endif

QPair<QWidget *, QWidget *> DSettingsWidgetFactory::createStandardItem(const QByteArray &translateContext, Core::DSettingsOption *option, QWidget *rightWidget)
{
    const QString label(DWIDGET_NAMESPACE::tr(translateContext, option->name().toLocal8Bit().constData()));

    if (label.isEmpty())
        return qMakePair(nullptr, rightWidget);

    return qMakePair(new QLabel(label), rightWidget);
}

QPair<QWidget *, QWidget *> createShortcutEditOptionHandle(DSettingsWidgetFactoryPrivate *p, QObject *opt)
{
    static DSettingsWidgetFactoryPrivate *pFlg = nullptr;
    if (pFlg != p) {
        shortcutMap.clear();
        pFlg = p;
    }

    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new KeySequenceEdit(option);

    rightWidget->setObjectName("OptionShortcutEdit");
    rightWidget->setAccessibleName("OptionShortcutEdit");
    rightWidget->ShortcutDirection(Qt::AlignLeft);

    auto optionValue = option->value();
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();

    option->connect(rightWidget, &KeySequenceEdit::editingFinished, [ = ](const QKeySequence & sequence) {

        QString keyseq = sequence.toString();
        if (shortcutMap.value(sequence.toString()) == rightWidget) //键位于自己相同
            return;

        if (shortcutMap.value(keyseq)) {
            ChangeDDialog frame(keyseq, rightWidget, rightWidget->text());
            frame.setAccessibleName("ChangeDDialog");
            frame.exec();
        } else {
            shortcutMap.remove(shortcutMap.key(rightWidget));
            shortcutMap.insert(keyseq, rightWidget);
            option->setValue(keyseq);
        }
    });

    auto updateWidgetValue = [ = ](const QVariant & optionValue, DTK_CORE_NAMESPACE::DSettingsOption * opt) {
        QKeySequence sequence(optionValue.toString());
        QString keyseq = sequence.toString();

        if (shortcutMap.value(keyseq)) {
            return;
        }

        if (rightWidget->setKeySequence(sequence)) {
            shortcutMap.insert(keyseq, rightWidget);
            opt->setValue(keyseq);
        }
    };
    updateWidgetValue(optionValue, option);

    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged, rightWidget, [ = ](const QVariant & value) {

        if (value.toString() == SHORTCUT_VALUE) {
            rightWidget->clear();
            return;
        }
        QKeySequence sequence(value.toString());
        QString keyseq = sequence.toString();

        shortcutMap.remove(shortcutMap.key(rightWidget));

        if (rightWidget->setKeySequence(sequence)) {    //设置快捷键

            shortcutMap.insert(keyseq, rightWidget);
            option->setValue(keyseq);
        }
    });

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, rightWidget);
}

QPair<QWidget *, QWidget *> createCheckboxOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto value = option->data("text").toString();
    auto trName = DWIDGET_NAMESPACE::tr(translateContext, value.toStdString().c_str());

    auto rightWidget = new QCheckBox(trName);

    rightWidget->setObjectName("OptionCheckbox");
    rightWidget->setAccessibleName("OptionCheckbox");
    rightWidget->setChecked(option->value().toBool());

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    option->connect(rightWidget, &QCheckBox::checkStateChanged,
#else
    option->connect(rightWidget, &QCheckBox::stateChanged,
#endif
    option, [ = ](int status) {
        option->setValue(status == Qt::Checked);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setChecked(value.toBool());
        rightWidget->update();
    });

    return qMakePair(rightWidget, nullptr);
}

QPair<QWidget *, QWidget *> createLineEditOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto value = option->data("text").toString();
    auto trName = DWIDGET_NAMESPACE::tr(translateContext, value.toStdString().c_str());
    auto rightWidget = new QLineEdit(trName);
    rightWidget->setObjectName("OptionLineEdit");
    rightWidget->setAccessibleName("OptionLineEdit");
    rightWidget->setText(option->value().toString());

    option->connect(rightWidget, &QLineEdit::editingFinished,
    option, [ = ]() {
        option->setValue(rightWidget->text());
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setText(value.toString());
        rightWidget->update();
    });

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, rightWidget);
}

QPair<QWidget *, QWidget *> createComboBoxOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new ComboBox();
    rightWidget->setFocusPolicy(Qt::StrongFocus);
    rightWidget->setObjectName("OptionLineEdit");
    rightWidget->setAccessibleName("OptionComboBox");

    auto initComboxList = [ = ](const QStringList & data) {
        for (auto item : data) {
            auto trName = DWIDGET_NAMESPACE::tr(translateContext, item.toStdString().c_str());
            rightWidget->addItem(trName);
        }
        auto current = option->value().toInt();
        rightWidget->setCurrentIndex(current);
        option->connect(rightWidget, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
        option, [ = ](int index) {
            option->setValue(index);
        });

        option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
        rightWidget, [ = ](const QVariant & value) {
            rightWidget->setCurrentIndex(value.toInt());
        });
    };

    auto initComboxMap = [ = ](const QMap<QString, QVariant> &data) {
        auto keys = data.value("keys").toStringList();
        auto values = data.value("values").toStringList();

        for (int i = 0; i < keys.length(); ++i) {
            auto trName = DWIDGET_NAMESPACE::tr(translateContext, values.value(i).toStdString().c_str());
            rightWidget->addItem(trName, keys.value(i));
        }

        auto currentData = option->value().toString();
        auto currentIndex = rightWidget->findData(currentData);
        rightWidget->setCurrentIndex(currentIndex);

        option->connect(rightWidget, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
        option, [ = ](int index) {
            option->setValue(keys.value(index));
        });

        option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
        rightWidget, [ = ](const QVariant & value) {
            auto currentIndex = rightWidget->findData(value.toString());
            rightWidget->setCurrentIndex(currentIndex);
        });
    };

    auto updateData = [ = ](const QString & dataType, const QVariant & data) {
        if ("items" != dataType) {
            return;
        }

        rightWidget->clear();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if (data.type() == QVariant::StringList) {
#else
        if (data.typeId() == QMetaType::Type::QStringList) {
#endif
            initComboxList(data.toStringList());
        }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if (data.type() == QVariant::Map) {
#else
        if (data.typeId() == QMetaType::Type::QVariantMap) {
#endif
            initComboxMap(data.toMap());
        }
        rightWidget->update();
    };

    auto initData = option->data("items");
    updateData("items", initData);

    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::dataChanged,
    rightWidget, [ = ](const QString & dataType, const QVariant & value) {
        updateData(dataType, value);
    });

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, rightWidget);
}

QPair<QWidget *, QWidget *> createButtonGroupOptionHandle(QObject *opt)
{
    QList<DButtonBoxButton *> btnList;

    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto items = option->data("items").toStringList();
    for (const auto &item : items) {
        auto btn = new DButtonBoxButton(item);
        btnList.append(btn);
    }

    auto rightWidget = new DButtonBox();
    rightWidget->setObjectName("OptionButtonBox");
    rightWidget->setAccessibleName("OptionButtonBox");
    rightWidget->setButtonList(btnList, true);
    rightWidget->setMaximumWidth(60 * btnList.count());
    btnList.at(option->value().toInt())->setChecked(true);

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    option->connect(rightWidget, &DButtonBox::buttonClicked, option, [option, rightWidget ] (QAbstractButton * btn) {
        int index = rightWidget->buttonList().indexOf(btn);
        option->setValue(index);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
                    rightWidget, [ = ](const QVariant & value) {
        int index = value.toInt();
        btnList.at(index)->setChecked(true);
        rightWidget->update();
    });

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, rightWidget);
}

QPair<QWidget *, QWidget *> createRadioGroupOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto items = option->data("items").toStringList();

    auto rightWidget = new QGroupBox;
    rightWidget->setContentsMargins(0, 0, 0, 0);
    rightWidget->setObjectName("OptionRadioGroup");
    rightWidget->setAccessibleName("OptionRadioGroup");
    rightWidget->setAlignment(Qt::AlignLeft);
    rightWidget->setFlat(true);
    rightWidget->setMinimumHeight(24 * items.length() + 8);

    auto rgLayout = new QVBoxLayout;
    rgLayout->setContentsMargins(0, 0, 0, 0);
    auto index = 0;
    QList<QRadioButton *> buttonList;
    for (auto item : items)  {
        auto rb = new QRadioButton(DWIDGET_NAMESPACE::tr(translateContext, item.toStdString().c_str()));
        // fix 加大字体后显示截断，不应该 fixedheight
        rb->setMinimumHeight(24);
        rb->setProperty("_dtk_widget_settings_radiogroup_index", index);
        rb->setAccessibleName(QString("OptionRadioButtonAt").append(QString::number(items.indexOf(item) + 1)));
        rgLayout->addWidget(rb);
        ++index;

        option->connect(rb, &QRadioButton::clicked,
        option, [ = ](int) {
            auto index = rb->property("_dtk_widget_settings_radiogroup_index").toInt();
            option->setValue(index);
        });
        buttonList << rb;
    }
    rightWidget->setLayout(rgLayout);

    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ buttonList ](const QVariant & value) {
        auto index = value.toInt();
        if (buttonList.length() > index) {
            buttonList.value(index)->setChecked(true);
        }
    });

    index = option->value().toInt();
    if (buttonList.length() > index) {
        buttonList.value(index)->setChecked(true);
    }

    return qMakePair(rightWidget, nullptr);
}

QPair<QWidget *, QWidget *> createSpinButtonOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QSpinBox();
    rightWidget->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    rightWidget->setObjectName("OptionDSpinBox");
    rightWidget->setAccessibleName("OptionDSpinBox");
    if (option->data("max").isValid()) {
        rightWidget->setMaximum(option->data("max").toInt());
    }
    if (option->data("min").isValid()) {
        rightWidget->setMinimum(option->data("min").toInt());
    }

    // The default range is 0-99. so we setMaximum/setMinimum first
    rightWidget->setValue(option->value().toInt());

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();

    option->connect(rightWidget, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
    option, [ = ](int value) {
        option->setValue(value);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setValue(value.toInt());
        rightWidget->update();
    });

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, rightWidget);
}

QPair<QWidget *, QWidget *> createSliderOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QSlider();
    rightWidget->setObjectName("OptionQSlider");
    rightWidget->setAccessibleName("OptionQSlider");
    rightWidget->setOrientation(Qt::Horizontal);
    rightWidget->setMaximum(option->data("max").toInt());
    rightWidget->setMinimum(option->data("min").toInt());
    rightWidget->setValue(option->value().toInt());

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();

    option->connect(rightWidget, &QSlider::valueChanged,
    option, [ = ](int value) {
        rightWidget->blockSignals(true);
        option->setValue(value);
        rightWidget->blockSignals(false);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setValue(value.toInt());
        rightWidget->update();
    });

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, rightWidget);
}

QPair<QWidget *, QWidget *> createSwitchButton(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new DSwitchButton();
    rightWidget->setObjectName("OptionDSwitchButton");
    rightWidget->setAccessibleName("OptionDSwitchButton");
    rightWidget->setChecked(option->value().toBool());

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();

    option->connect(rightWidget, &DSwitchButton::checkedChanged,
    option, [ = ](bool value) {
        rightWidget->blockSignals(true);
        option->setValue(value);
        rightWidget->blockSignals(false);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setChecked(value.toBool());
        rightWidget->update();
    });

    QWidget *widget = new  QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    widget->setAccessibleName("OptionDSwitchButtonWidget");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(rightWidget, 0, Qt::AlignRight);

    return DSettingsWidgetFactory::createStandardItem(translateContext, option, widget);
}

QPair<QWidget *, QWidget *> createTitle1(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto trName = DWIDGET_NAMESPACE::tr(translateContext, option->value().toString().toLocal8Bit().constData());

    auto title = new ContentTitle;
    title->setTitle(trName);
    title->setAccessibleName(trName);
    title->label()->setForegroundRole(QPalette::BrightText);
    DFontSizeManager::instance()->bind(title, DFontSizeManager::T4, QFont::Medium);

    return qMakePair(title, nullptr);
}

QPair<QWidget *, QWidget *> createTitle2(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto trName = DWIDGET_NAMESPACE::tr(translateContext, option->value().toString().toLocal8Bit().constData());

    auto title = new ContentTitle;
    title->setAccessibleName(trName);
    title->setTitle(trName);
    DFontSizeManager::instance()->bind(title, DFontSizeManager::T5, QFont::Medium);

    return qMakePair(title, nullptr);
}

QWidget *createUnsupportHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QLabel();
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionUnsupport");
    rightWidget->setAccessibleName("OptionUnsupport");
    rightWidget->setText("Unsupport option type: " + option->viewType());
    rightWidget->setWordWrap(true);

    return  rightWidget;
}

class DSettingsWidgetFactoryPrivate
{
public:
    DSettingsWidgetFactoryPrivate(DSettingsWidgetFactory *parent) : q_ptr(parent)
    {
        itemCreateHandles.insert("checkbox", createCheckboxOptionHandle);
        itemCreateHandles.insert("lineedit", createLineEditOptionHandle);
        itemCreateHandles.insert("combobox", createComboBoxOptionHandle);
        itemCreateHandles.insert("shortcut", std::bind(createShortcutEditOptionHandle, this, std::placeholders::_1));
        itemCreateHandles.insert("spinbutton", createSpinButtonOptionHandle);
        itemCreateHandles.insert("buttongroup", createButtonGroupOptionHandle);
        itemCreateHandles.insert("radiogroup", createRadioGroupOptionHandle);
        itemCreateHandles.insert("slider", createSliderOptionHandle);
        itemCreateHandles.insert("switchbutton", createSwitchButton);
        itemCreateHandles.insert("title1", createTitle1);
        itemCreateHandles.insert("title2", createTitle2);
    }

    QMap<QString, std::function<DSettingsWidgetFactory::WidgetCreateHandler> > widgetCreateHandles;
    QMap<QString, std::function<DSettingsWidgetFactory::ItemCreateHandler> > itemCreateHandles;

    DSettingsWidgetFactory *q_ptr;
    Q_DECLARE_PUBLIC(DSettingsWidgetFactory)
};

DSettingsWidgetFactory::DSettingsWidgetFactory(QObject *parent) :
    QObject(parent), dd_ptr(new DSettingsWidgetFactoryPrivate(this))
{

}

DSettingsWidgetFactory::~DSettingsWidgetFactory()
{

}

/*!
  \brief DSettingsWidgetFactory::registerWidget 向Factory注册一种类型viewType。注册该类型后，可以通过在DSettings的DOption中，设置type为对应的viewType，这样Factory在构建viewType对应的控件时，会调用handler所指定的方法来构建.

  \a viewType 控件类型，建议为全局唯一的字符串，且不要和内部类型重复，参考 Dtk::Widget::DSettingsWidgetFactory
  \a handler 自定义的控件构建函数， 参考 Dtk::Widget::DSettingsWidgetFactory::WidgetCreateHandler
 */
void DSettingsWidgetFactory::registerWidget(const QString &viewType, std::function<WidgetCreateHandler> handler)
{
    Q_D(DSettingsWidgetFactory);
    d->widgetCreateHandles.insert(viewType, handler);
}

void DSettingsWidgetFactory::registerWidget(const QString &viewType, std::function<DSettingsWidgetFactory::ItemCreateHandler> handler)
{
    Q_D(DSettingsWidgetFactory);
    d->itemCreateHandles.insert(viewType, handler);
}

/*!
  \brief DSettingsWidgetFactory::createWidget 创建一个option对应的配置控件，一般是需要自定义控件时使用，可以通过translateContext设置翻译上下文.

  \a option 需要构建的配置项
  \return 根据option的type属性创建的配置控件
  \sa Dtk::Widget::DSettingsWidgetFactory::createWidget(const QByteArray &translateContext, QPointer<Dtk::Core::DSettingsOption> option)
 */
QWidget *DSettingsWidgetFactory::createWidget(QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option)
{
    return createWidget(QByteArray(), option);
}
/*!
  \brief DSettingsWidgetFactory::createWidget 创建一个option对应的配置控件，一般是需要自定义控件时使用.

  \a translateContext 自定义的翻译上下文，参考 QCoreApplication::translate();
  \a option 需要构建的配置项
  \return 根据option的type属性创建的配置控件
  \sa Dtk::Widget::DSettingsWidgetFactory::createWidget(QPointer<Dtk::Core::DSettingsOption> option)
 */
QWidget *DSettingsWidgetFactory::createWidget(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option)
{
    option->setProperty(PRIVATE_PROPERTY_translateContext, translateContext);

    Q_D(DSettingsWidgetFactory);
    auto handle = d->widgetCreateHandles.value(option->viewType());
    if (handle) {
        return handle(option.data());
    } else {
        return createUnsupportHandle(option.data());
    }
}

QPair<QWidget *, QWidget *> DSettingsWidgetFactory::createItem(QPointer<Core::DSettingsOption> option) const
{
    return createItem(QByteArray(), option);
}

QPair<QWidget *, QWidget *> DSettingsWidgetFactory::createItem(const QByteArray &translateContext, QPointer<Core::DSettingsOption> option) const
{
    option->setProperty(PRIVATE_PROPERTY_translateContext, translateContext);

    Q_D(const DSettingsWidgetFactory);
    auto handle = d->itemCreateHandles.value(option->viewType());
    if (handle) {
        return handle(option.data());
    }

    return qMakePair(nullptr, nullptr);
}

DWIDGET_END_NAMESPACE

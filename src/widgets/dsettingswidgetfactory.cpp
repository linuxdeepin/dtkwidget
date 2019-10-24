/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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

#include <DSettingsOption>

#include "private/settings/shortcutedit.h"
#include "private/settings/buttongroup.h"
#include "private/settings/combobox.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DSettingsWidgetFactory
 * \~chinese \brief DSettingsWidgetFactory是一个用于构造Deepin风格的设置对话框的控件。\n
 * \~chinese 对于每种控件，均由一个id和对应的构造函数来组成，DSettingsWidgetFactory内置了许多基础的控件，包括：\n
 * \~chinese checkbox/lineedit/combobox/spinbutton/buttongroup/radiogroup/slider等。
 * \~chinese
 */

/*!
 * \~chinese \typedef DSettingsWidgetFactory::WidgetCreateHandler
 * \~chinese \brief 自定义的控件构建函数,一般接受一个 Dtk::Core::DSettingsOption 作为参数。\n
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
 * \~chinese \brief DSettingsWidgetFactory::createTwoColumWidget 返回一个水平布局的控件，
 * \~chinese 左边为option的名称标签，右边为设置的自定义控件rightWidget
 * \~chinese \param option 设置项，包含改配置的名称
 * \~chinese \param rightWidget 自定义控件
 * \~chinese \return 生成的水平布局配置项控件
 * \~chinese \sa Dtk::Widget::DSettingsWidgetFactory::createTwoColumWidget(const QByteArray &translateContext, Dtk::Core::DSettingsOption *option, QWidget *rightWidget)
 */
QWidget *DSettingsWidgetFactory::createTwoColumWidget(DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget)
{
    return createTwoColumWidget(QByteArray(), option, rightWidget);
}

/*!
 * \~chinese \brief DSettingsWidgetFactory::createTwoColumWidget 返回一个水平布局的控件，
 * \~chinese 左边为option的名称标签，右边为设置的自定义控件rightWidget, 可以通过translateContext设置翻译上下文。
 * \~chinese \param translateContext 自定义的翻译上下文，参考 QCoreApplication::translate();
 * \~chinese \param option 设置项，包含改配置的名称
 * \~chinese \param rightWidget 自定义控件
 * \~chinese \return 生成的水平布局配置项控件
 * \~chinese \sa Dtk::Widget::DSettingsWidgetFactory::createTwoColumWidget(Dtk::Core::DSettingsOption *option, QWidget *rightWidget)
 */
QWidget *DSettingsWidgetFactory::createTwoColumWidget(const QByteArray &translateContext, DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget)
{
    auto optionFrame = new QFrame;
    optionFrame->setMinimumHeight(30);
    optionFrame->setObjectName("OptionFrame");
//    optionFrame->setStyleSheet("QFrame{border: 1px solid red;}");

    auto optionLayout = new QGridLayout(optionFrame);
    optionLayout->setContentsMargins(0, 0, 0, 0);
    optionLayout->setSpacing(0);

    rightWidget->setMinimumWidth(240);

    if (!option->name().isEmpty()) {
        optionLayout->setColumnMinimumWidth(0, 110);
        optionLayout->setHorizontalSpacing(20);
        optionLayout->setColumnStretch(0, 10);
        optionLayout->setColumnStretch(1, 100);

        auto trName = DWIDGET_NAMESPACE::tr(translateContext, option->name().toStdString().c_str());
        auto labelWidget = new QLabel(trName);
        labelWidget->setContentsMargins(5, 0, 0, 0);
        labelWidget->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        labelWidget->setObjectName("OptionLabel");
        optionLayout->addWidget(labelWidget, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
        optionLayout->addWidget(rightWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    } else {
        optionLayout->setColumnMinimumWidth(0, 5);
        optionLayout->addWidget(rightWidget, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }

    return  optionFrame;
}

QWidget *createShortcutEditOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);

    auto rightWidget = new ShortcutEdit();
    rightWidget->setObjectName("OptionShortcutEdit");

    auto updateWidgetValue = [rightWidget](const QVariant & optionValue) {
        switch (optionValue.type()) {
        case QVariant::List:
        case QVariant::StringList: {
            QStringList keyseqs = optionValue.toStringList();
            if (keyseqs.length() == 2) {
                auto modifier = static_cast<Qt::KeyboardModifiers>(keyseqs.value(0).toInt());
                auto key = static_cast<Qt::Key>(keyseqs.value(1).toInt());
                rightWidget->setShortCut(modifier, key);
            }
            break;
        }
        case QVariant::String: {
            rightWidget->setShortCut(optionValue.toString());
            break;
        }
        default:
            qCritical() << "unknown variant type" << optionValue;
        }
    };

    auto optionValue = option->value();
    updateWidgetValue(optionValue);

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);

    // keep raw value type
    switch (optionValue.type()) {
    case QVariant::List:
    case QVariant::StringList: {
        option->connect(rightWidget, &ShortcutEdit::shortcutChanged,
        option, [ = ](Qt::KeyboardModifiers modifier, Qt::Key key) {
            QStringList keyseqs;
            keyseqs << QString("%1").arg(modifier) << QString("%1").arg(key);
            option->setValue(keyseqs);
        });
        break;
    }
    case QVariant::String: {
        option->connect(rightWidget, &ShortcutEdit::shortcutStringChanged,
        option, [ = ](const QString & seqString) {
            option->setValue(seqString);
        });
        break;
    }
    default:
        qCritical() << "unknown variant type" << optionValue;
    }

    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        updateWidgetValue(value);
        rightWidget->update();
    });

    return  optionWidget;
}

QWidget *createCheckboxOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto value = option->data("text").toString();
    auto trName = DWIDGET_NAMESPACE::tr(translateContext, value.toStdString().c_str());

//    auto checkboxFrame = new QWidget;
//    auto checkboxLayout = new QHBoxLayout(checkboxFrame);
//    checkboxLayout->setSpacing(0);
//    checkboxLayout->setContentsMargins(0, 0, 0, 0);
    auto rightWidget = new QCheckBox(trName);
//    auto checkboxLabel = new QLabel(trName);
//    checkboxLabel->setWordWrap(true);
//    checkboxLabel->setMinimumWidth(320);
//    checkboxLayout->addWidget(rightWidget);
//    checkboxLayout->addSpacing(5);
//    checkboxLayout->addWidget(checkboxLabel);
//    checkboxLayout->addStretch();

    rightWidget->setObjectName("OptionCheckbox");
    rightWidget->setChecked(option->value().toBool());

//    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, checkboxFrame);

    option->connect(rightWidget, &QCheckBox::stateChanged,
    option, [ = ](int status) {
        option->setValue(status == Qt::Checked);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setChecked(value.toBool());
        rightWidget->update();
    });

    return rightWidget;
//    return  optionWidget;
}

QWidget *createLineEditOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto value = option->data("text").toString();
    auto trName = DWIDGET_NAMESPACE::tr(translateContext, value.toStdString().c_str());
    auto rightWidget = new QLineEdit(trName);
    rightWidget->setObjectName("OptionLineEdit");
    rightWidget->setText(option->value().toString());

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);

    option->connect(rightWidget, &QLineEdit::editingFinished,
    option, [ = ]() {
        option->setValue(rightWidget->text());
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setText(value.toString());
        rightWidget->update();
    });

    return  optionWidget;
}

QWidget *createComboBoxOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new ComboBox();
    rightWidget->setFocusPolicy(Qt::StrongFocus);
    rightWidget->setObjectName("OptionLineEdit");
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);

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
        if (data.type() == QVariant::StringList) {
            initComboxList(data.toStringList());
        }
        if (data.type() == QVariant::Map) {
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

    return  optionWidget;
}

QWidget *createButtonGroupOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new ButtonGroup();
    rightWidget->setObjectName("OptionButtonGroup");

    auto items = option->data("items").toStringList();
    rightWidget->setButtons(items);
    rightWidget->setCheckedButton(0);

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);
    rightWidget->setParent(optionWidget);

    option->connect(rightWidget, &ButtonGroup::buttonChecked,
    option, [ = ](int id) {
        option->setValue(id);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setCheckedButton(value.toInt());
        rightWidget->update();
    });
    return  optionWidget;
}

QWidget *createRadioGroupOptionHandle(QObject *opt)
{
    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto items = option->data("items").toStringList();

    auto rightWidget = new QGroupBox;
    rightWidget->setContentsMargins(0, 0, 0, 0);
    rightWidget->setObjectName("OptionRadioGroup");
    rightWidget->setAlignment(Qt::AlignLeft);
    rightWidget->setFixedHeight(24 * items.length() + 8);

    auto rgLayout = new QVBoxLayout;
    rgLayout->setContentsMargins(0, 0, 0, 0);
    auto index = 0;
    QList<QRadioButton *> buttonList;
    for (auto item : items)  {
        auto rb = new QRadioButton(DWIDGET_NAMESPACE::tr(translateContext, item.toStdString().c_str()));
        rb->setFixedHeight(24);
        rb->setProperty("_dtk_widget_settings_radiogroup_index", index);
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

//    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);
//    rightWidget->setParent(optionWidget);

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

    rightWidget->setStyleSheet("QGroupBox{border: 0px solid red;}");
//    optionWidget->setFixedHeight(24 * items.length() + 8);
//    return  optionWidget;
    return rightWidget;
}

QWidget *createSpinButtonOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QSpinBox();
    rightWidget->setObjectName("OptionDSpinBox");
    rightWidget->setValue(option->value().toInt());

    if (option->data("max").isValid()) {
        rightWidget->setMaximum(option->data("max").toInt());
    }
    if (option->data("min").isValid()) {
        rightWidget->setMinimum(option->data("min").toInt());
    }

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);

    option->connect(rightWidget, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged),
    option, [ = ](int value) {
        option->setValue(value);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setValue(value.toInt());
        rightWidget->update();
    });
    return  optionWidget;
}

QWidget *createSliderOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QSlider();
    rightWidget->setObjectName("OptionQSlider");
    rightWidget->setOrientation(Qt::Horizontal);
    rightWidget->setMaximum(option->data("max").toInt());
    rightWidget->setMinimum(option->data("min").toInt());
    rightWidget->setValue(option->value().toInt());

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);

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
    return  optionWidget;
}

QWidget *createUnsupportHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QLabel();
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionUnsupport");
    rightWidget->setText("Unsupport option type: " + option->viewType());

    auto translateContext = opt->property(PRIVATE_PROPERTY_translateContext).toByteArray();
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(translateContext, option, rightWidget);

//    optionWidget->setStyleSheet("QFrame{border: 1px solid red;}");
    return  optionWidget;
}

class DSettingsWidgetFactoryPrivate
{
public:
    DSettingsWidgetFactoryPrivate(DSettingsWidgetFactory *parent) : q_ptr(parent)
    {
        widgetCreateHandles.insert("checkbox", createCheckboxOptionHandle);
        widgetCreateHandles.insert("lineedit", createLineEditOptionHandle);
        widgetCreateHandles.insert("combobox", createComboBoxOptionHandle);
        widgetCreateHandles.insert("shortcut", createShortcutEditOptionHandle);
        widgetCreateHandles.insert("spinbutton", createSpinButtonOptionHandle);
        widgetCreateHandles.insert("buttongroup", createButtonGroupOptionHandle);
        widgetCreateHandles.insert("radiogroup", createRadioGroupOptionHandle);
        widgetCreateHandles.insert("slider", createSliderOptionHandle);
    }

    QMap<QString, std::function<DSettingsWidgetFactory::WidgetCreateHandler> > widgetCreateHandles;

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
 * \~chinese \brief DSettingsWidgetFactory::registerWidget 向Factory注册一种类型viewType。注册该类型后，可以通过在DSettings的DOption中，设置type为对应的viewType，这样Factory在构建viewType对应的控件时，会调用handler所指定的方法来构建。
 * \~chinese \param viewType 控件类型，建议为全局唯一的字符串，且不要和内部类型重复，参考 Dtk::Widget::DSettingsWidgetFactory
 * \~chinese \param handler 自定义的控件构建函数， 参考 Dtk::Widget::DSettingsWidgetFactory::WidgetCreateHandler
 */
void DSettingsWidgetFactory::registerWidget(const QString &viewType, std::function<WidgetCreateHandler> handler)
{
    Q_D(DSettingsWidgetFactory);
    d->widgetCreateHandles.insert(viewType, handler);
}

/*!
 * \~chinese \brief DSettingsWidgetFactory::createWidget 创建一个option对应的配置控件，一般是需要自定义控件时使用，可以通过translateContext设置翻译上下文。
 * \~chinese \param option 需要构建的配置项
 * \~chinese \return 根据option的type属性创建的配置控件
 * \~chinese \sa Dtk::Widget::DSettingsWidgetFactory::createWidget(const QByteArray &translateContext, QPointer<Dtk::Core::DSettingsOption> option)
 */
QWidget *DSettingsWidgetFactory::createWidget(QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option)
{
    return createWidget(QByteArray(), option);
}
/*!
 * \~chinese \brief DSettingsWidgetFactory::createWidget 创建一个option对应的配置控件，一般是需要自定义控件时使用,
 * \~chinese \param translateContext 自定义的翻译上下文，参考 QCoreApplication::translate();
 * \~chinese \param option 需要构建的配置项
 * \~chinese \return 根据option的type属性创建的配置控件
 * \~chinese \sa Dtk::Widget::DSettingsWidgetFactory::createWidget(QPointer<Dtk::Core::DSettingsOption> option)
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

DWIDGET_END_NAMESPACE

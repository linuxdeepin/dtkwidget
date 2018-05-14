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

#include <DSettingsOption>

#include "private/settings/shortcutedit.h"
#include "private/settings/buttongroup.h"
#include "private/settings/combobox.h"

DWIDGET_BEGIN_NAMESPACE

QWidget *DSettingsWidgetFactory::createTwoColumWidget(DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget)
{
    auto optionFrame = new QFrame;
    optionFrame->setMinimumHeight(30);
    optionFrame->setObjectName("OptionFrame");
//    optionFrame->setStyleSheet("QFrame{border: 1px solid red;}");

    auto optionLayout = new QGridLayout(optionFrame);
    optionLayout->setContentsMargins(0, 0, 0, 0);
    optionLayout->setSpacing(0);

    rightWidget->setMinimumWidth(240);
    rightWidget->setStyleSheet("QWidget{font-size: 12px;}");

    if (!option->name().isEmpty()) {
        optionLayout->setColumnMinimumWidth(0, 110);
        optionLayout->setHorizontalSpacing(20);
        optionLayout->setColumnStretch(0, 10);
        optionLayout->setColumnStretch(1, 100);

        auto trName = QObject::tr(option->name().toStdString().c_str());
        auto labelWidget = new QLabel(trName);
        labelWidget->setContentsMargins(5, 0, 0, 0);
        labelWidget->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        labelWidget->setMinimumWidth(150);
        labelWidget->setFixedWidth(160);
        labelWidget->setObjectName("OptionLabel");
        labelWidget->setStyleSheet("#OptionLabel{font-size: 12px; }");
        optionLayout->addWidget(labelWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
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

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);

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
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto value = option->data("text").toString();
    auto trName = QObject::tr(value.toStdString().c_str());

    auto checkboxFrame = new QWidget;
    auto checkboxLayout = new QHBoxLayout(checkboxFrame);
    checkboxLayout->setSpacing(0);
    checkboxLayout->setContentsMargins(0, 0, 0, 0);
    auto rightWidget = new QCheckBox("");
    auto checkboxLabel = new QLabel(trName);
    checkboxLabel->setWordWrap(true);
    checkboxLabel->setMinimumWidth(320);
    checkboxLayout->addWidget(rightWidget);
    checkboxLayout->addSpacing(5);
    checkboxLayout->addWidget(checkboxLabel);
    checkboxLayout->addStretch();

    rightWidget->setObjectName("OptionCheckbox");
    rightWidget->setChecked(option->value().toBool());

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, checkboxFrame);

    option->connect(rightWidget, &QCheckBox::stateChanged,
    option, [ = ](int status) {
        option->setValue(status == Qt::Checked);
    });
    option->connect(option, &DTK_CORE_NAMESPACE::DSettingsOption::valueChanged,
    rightWidget, [ = ](const QVariant & value) {
        rightWidget->setChecked(value.toBool());
        rightWidget->update();
    });

    return  optionWidget;
}

QWidget *createLineEditOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto value = option->data("text").toString();
    auto trName = QObject::tr(value.toStdString().c_str());
    auto rightWidget = new QLineEdit(trName);
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionLineEdit");
    rightWidget->setText(option->value().toString());

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);

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
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new ComboBox();
    rightWidget->setFocusPolicy(Qt::StrongFocus);
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionLineEdit");
    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);

    auto initComboxList = [ = ](const QStringList & data) {
        for (auto item : data) {
            auto trName = QObject::tr(item.toStdString().c_str());
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
            auto trName = QObject::tr(values.value(i).toStdString().c_str());
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
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionButtonGroup");

    auto items = option->data("items").toStringList();
    rightWidget->setButtons(items);
    rightWidget->setCheckedButton(0);

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);
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
        auto rb = new QRadioButton(QObject::tr(item.toStdString().c_str()));
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

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);
    rightWidget->setParent(optionWidget);

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

    rightWidget->setStyleSheet("QGroupBox{border: none;} QRadioButton{font-size:12px;}");
    return  optionWidget;
}

QWidget *createSpinButtonOptionHandle(QObject *opt)
{
    auto option = qobject_cast<DTK_CORE_NAMESPACE::DSettingsOption *>(opt);
    auto rightWidget = new QSpinBox();
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionDSpinBox");
    rightWidget->setValue(option->value().toInt());

    if (option->data("max").isValid()) {
        rightWidget->setMaximum(option->data("max").toInt());
    }
    if (option->data("min").isValid()) {
        rightWidget->setMinimum(option->data("min").toInt());
    }

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);

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
    rightWidget->setFixedHeight(24);
    rightWidget->setObjectName("OptionQSlider");
    rightWidget->setOrientation(Qt::Horizontal);
    rightWidget->setMaximum(option->data("max").toInt());
    rightWidget->setMinimum(option->data("min").toInt());
    rightWidget->setValue(option->value().toInt());

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);

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

    auto optionWidget = DSettingsWidgetFactory::createTwoColumWidget(option, rightWidget);

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
    QObject(parent), d_ptr(new DSettingsWidgetFactoryPrivate(this))
{

}

DSettingsWidgetFactory::~DSettingsWidgetFactory()
{

}

void DSettingsWidgetFactory::registerWidget(const QString &viewType, std::function<WidgetCreateHandler> handler)
{
    Q_D(DSettingsWidgetFactory);
    d->widgetCreateHandles.insert(viewType, handler);
}

QWidget *DSettingsWidgetFactory::createWidget(QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option)
{
    Q_D(DSettingsWidgetFactory);
    auto handle = d->widgetCreateHandles.value(option->viewType());
    if (handle) {
        return handle(option.data());
    } else {
        return createUnsupportHandle(option.data());
    }
}

DWIDGET_END_NAMESPACE

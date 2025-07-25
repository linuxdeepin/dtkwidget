// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCUSTOMERMACROWIDGET_H
#define DCUSTOMERMACROWIDGET_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

// 定义键位组合操作符，兼容DTK5和DTK6
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define DTKEY_OPERATOR |
#else
#define DTKEY_OPERATOR +
#endif

#include <DFrame>
#include <DArrowLineDrawer>
#include <DButtonBox>
#include <DColoredProgressBar>
#include <DCrumbEdit>
#include <DDoubleSpinBox>
#include <DFileChooserEdit>
#include <DFloatingButton>
#include <DIpv4LineEdit>
#include <DKeySequenceEdit>
#include <DLabel>
#include <DLineEdit>
#include <DPageIndicator>
#include <DProgressBar>

#include <DPasswordEdit>
#include <DTabBar>
#include <DWaterProgress>
#include <DShadowLine>
#include <DTextEdit>
#include <DSearchComboBox>
#include <DSpinBox>
#include <DSegmentedHighlight>
#include <DSpinner>
#include <DToolButton>
#include <DSegmentedControl>
#include <DSuggestButton>
#include <DSwitchButton>
#include <DWarningButton>
#include <DSearchEdit>

// not added
// #include <DHorizontalLine>
// #include <DVerticalLine>

// TODO: setText problem, add in v5.6
#include <DIconButton>

DWIDGET_USE_NAMESPACE

#define IS_SAME(A, B) std::is_same<A, B>::value

#define GENERATE_CUSTOMER_WIDGET(UPPER_NAME, WIDTH, HEIGHT)                                                 \
class UPPER_NAME ## Plugin : public QObject, public QDesignerCustomWidgetInterface                          \
{                                                                                                           \
    Q_OBJECT                                                                                                \
    Q_INTERFACES(QDesignerCustomWidgetInterface)                                                            \
public:                                                                                                     \
    explicit UPPER_NAME ## Plugin(QObject *parent = nullptr)                                                \
        : QObject(parent)                                                                                   \
        , m_initialized(false)                                                                              \
    {                                                                                                       \
    }                                                                                                       \
                                                                                                            \
    QString name() const override {                                                                         \
        return QStringLiteral("Dtk::Widget::" # UPPER_NAME);                                                \
    }                                                                                                       \
                                                                                                            \
    QString group() const override {                                                                        \
        return QStringLiteral("Dtk Widgets");                                                               \
    }                                                                                                       \
                                                                                                            \
    QString toolTip() const override {                                                                      \
        return QStringLiteral("A widgets for dtk gui components.");                                         \
    }                                                                                                       \
                                                                                                            \
    QString whatsThis() const override {                                                                    \
        return toolTip();                                                                                   \
    }                                                                                                       \
                                                                                                            \
    QString includeFile() const override {                                                                  \
        return QStringLiteral("" # UPPER_NAME);                                                             \
    }                                                                                                       \
                                                                                                            \
    QIcon icon() const override {                                                                           \
        return QIcon(QStringLiteral(":/images/" # UPPER_NAME) + ".png");                                    \
    }                                                                                                       \
                                                                                                            \
    bool isContainer() const override {                                                                     \
        return false;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    UPPER_NAME *createWidget(QWidget *parent) override {                                                    \
        if (IS_SAME(UPPER_NAME, DFrame)) {                                                                  \
             return reinterpret_cast<UPPER_NAME *>(new DFrame(parent));                                     \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DLabel))                                                                    \
            return reinterpret_cast<UPPER_NAME *>(new DLabel("This is DLabel.", parent));                   \
        if (IS_SAME(UPPER_NAME, DColoredProgressBar)) {                                                     \
            DColoredProgressBar *w = new DColoredProgressBar(parent);                                       \
            w->setValue(50);                                                                                \
            w->setAlignment(Qt::AlignCenter);                                                               \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DFloatingButton)) {                                                         \
            DFloatingButton *w = new DFloatingButton(parent);                                               \
            w->setIcon(QIcon::fromTheme("lock"));                                                           \
            w->setFixedSize(WIDTH, HEIGHT);                                                                 \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DButtonBox)) {                                                              \
            DButtonBox *w = new DButtonBox(parent);                                                         \
            w->resize(WIDTH, HEIGHT);                                                                       \
            w->setToolTip("ButtonBox");                                                                     \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DIpv4LineEdit)) {                                                           \
            DIpv4LineEdit *w = new DIpv4LineEdit(parent);                                                   \
            w->resize(WIDTH, HEIGHT);                                                                       \
            w->setText("255.255.255.255");                                                                  \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DKeySequenceEdit)) {                                                        \
            DKeySequenceEdit *w = new DKeySequenceEdit(parent);                                             \
            w->setKeySequence(QKeySequence(Qt::CTRL DTKEY_OPERATOR Qt::Key_A)); /* 用+号不要用逗号 */                      \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DWaterProgress)) {                                                          \
            DWaterProgress *w = new DWaterProgress(parent);                                                 \
            w->setValue(50);                                                                                \
            w->start();                                                                                     \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DToolButton)) {                                                             \
            DToolButton *w = new DToolButton(parent);                                                       \
            w->setIcon(QIcon::fromTheme("unlock"));                                                         \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DSpinner)) {                                                                \
            DSpinner *w = new DSpinner(parent);                                                             \
            w->start();                                                                                     \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DSuggestButton)) {                                                          \
            DSuggestButton *w = new DSuggestButton("suggest", parent);                                      \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DWarningButton)) {                                                          \
            DWarningButton *w = new DWarningButton(parent);                                                 \
            w->setText("warning");                                                                          \
            return reinterpret_cast<UPPER_NAME *>(w);                                                       \
        }                                                                                                   \
                                                                                                            \
        return new UPPER_NAME(parent);                                                                      \
    }                                                                                                       \
                                                                                                            \
    bool isInitialized() const override { return m_initialized;}                                            \
    void initialize(QDesignerFormEditorInterface */*core*/) override {                                      \
        if (m_initialized)                                                                                  \
            return;                                                                                         \
        m_initialized = true;                                                                               \
    }                                                                                                       \
    QString domXml() const override {                                                                       \
        QString property;                                                                                   \
        if (IS_SAME(UPPER_NAME, DColoredProgressBar)) {                                                     \
            property = R"(<property name="value"><number>50</number></property>)";                          \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DProgressBar)) {                                                            \
            property = R"(<property name="value"><number>50</number></property>)";                          \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DLineEdit)) {                                                               \
            property = R"(<property name="text"><number>50</number></property>)";                           \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DIpv4LineEdit)) {                                                           \
            property = R"(<property name="text"><string>255.255.255.255</string></property>)";              \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DWaterProgress)) {                                                          \
            property = R"(<property name="value"><number>50</number></property>)";                          \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DTextEdit)) {                                                               \
            property = R"(<property name="text"><string>这是一个富文本编辑框</string></property>)";            \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DSuggestButton)) {                                                          \
            property = R"(<property name="text"><string>suggest</string></property>)";                      \
        }                                                                                                   \
        if (IS_SAME(UPPER_NAME, DWarningButton)) {                                                          \
            property = R"(<property name="text"><string>warning</string></property>)";                      \
        }                                                                                                   \
        return QStringLiteral("                                                                             \
            <ui language=\"c++\" displayname=\"" # UPPER_NAME "\">                                          \
                <widget class=\"Dtk::Widget::" # UPPER_NAME "\" name=\"Dtk::Widget::" # UPPER_NAME "\">     \
                    <property name=\"geometry\">                                                            \
                        <rect>                                                                              \
                            <x>0</x>                                                                        \
                            <y>0</y>                                                                        \
                            <width>%1</width>                                                               \
                            <height>%2</height>                                                             \
                        </rect>                                                                             \
                    </property>                                                                             \
                    %3                                                                                      \
                </widget>                                                                                   \
            </ui>").arg(WIDTH).arg(HEIGHT).arg(property);                                                   \
    }                                                                                                       \
                                                                                                            \
private:                                                                                                    \
    bool m_initialized;                                                                                     \
    char __align[7];                                                                                        \
}

GENERATE_CUSTOMER_WIDGET(DFrame,                320, 240);

GENERATE_CUSTOMER_WIDGET(DArrowLineDrawer,      160, 35);
GENERATE_CUSTOMER_WIDGET(DButtonBox,            110, 35);

GENERATE_CUSTOMER_WIDGET(DCrumbEdit,            200, 92);
GENERATE_CUSTOMER_WIDGET(DTextEdit,             200, 92);       // TODO: 大小固定

GENERATE_CUSTOMER_WIDGET(DFloatingButton,       40,  40);

GENERATE_CUSTOMER_WIDGET(DPageIndicator,        160, 35);

GENERATE_CUSTOMER_WIDGET(DDoubleSpinBox,        200, 40);
GENERATE_CUSTOMER_WIDGET(DSpinBox,              200, 40);

GENERATE_CUSTOMER_WIDGET(DFileChooserEdit,      200, 40);
GENERATE_CUSTOMER_WIDGET(DKeySequenceEdit,      200, 40);
GENERATE_CUSTOMER_WIDGET(DIpv4LineEdit,         210, 35);

GENERATE_CUSTOMER_WIDGET(DLabel,                200, 30);
GENERATE_CUSTOMER_WIDGET(DLineEdit,             200, 30);

GENERATE_CUSTOMER_WIDGET(DProgressBar,          200, 30);
GENERATE_CUSTOMER_WIDGET(DColoredProgressBar,   200, 30);

GENERATE_CUSTOMER_WIDGET(DPasswordEdit,         200, 40);
GENERATE_CUSTOMER_WIDGET(DTabBar,               40,  40);
GENERATE_CUSTOMER_WIDGET(DWaterProgress,        100, 100);       //  TODO: 大小被写死了： 100*100
GENERATE_CUSTOMER_WIDGET(DSearchComboBox,       200, 40);
GENERATE_CUSTOMER_WIDGET(DSpinner,              40,  40);
GENERATE_CUSTOMER_WIDGET(DToolButton,           40,  40);
GENERATE_CUSTOMER_WIDGET(DSuggestButton,        100, 40);
GENERATE_CUSTOMER_WIDGET(DWarningButton,        100, 40);
GENERATE_CUSTOMER_WIDGET(DSwitchButton,         75,  32);
GENERATE_CUSTOMER_WIDGET(DSearchEdit,           200, 40);

//GENERATE_CUSTOMER_WIDGET(DShadowLine,           200, 40);     // 看不到效果
//GENERATE_CUSTOMER_WIDGET(DSegmentedControl,     200, 40);
//GENERATE_CUSTOMER_WIDGET(DSegmentedHighlight,   200, 40);

#endif // DCUSTOMERMACROWIDGET_H

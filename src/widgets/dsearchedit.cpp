// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsearchedit.h"
#include "dpalette.h"
#include "dstyle.h"
#include "private/dsearchedit_p.h"
#include "diconbutton.h"

#include <DSysInfo>
#include <DGuiApplicationHelper>
#undef ENABLE_AI

#ifdef ENABLE_AI
// 讯飞语言相关
#include "session_interface.h"
#include "iat_interface.h"
#endif

#include <QAction>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QAction>
#include <QTimer>
#include <QCoreApplication>
#include <QToolButton>
#include <QDBusPendingCallWatcher>
#ifndef DTK_NO_MULTIMEDIA
#include <QAudioInput>
#include <QAudioDeviceInfo>
#endif

DWIDGET_BEGIN_NAMESPACE
DCORE_USE_NAMESPACE

#ifdef ENABLE_AI
class VoiceDevice : public QIODevice
{
    Q_OBJECT
public:
    using QIODevice::QIODevice;
    ~VoiceDevice()
    {
        if (isOpen())
            close();
    }

    bool open(OpenMode mode) override
    {
        if (mode != WriteOnly)
            return false;

        com::iflytek::aiservice::session session("com.iflytek.aiservice", "/", QDBusConnection::sessionBus());
        QDBusObjectPath path = session.createSession(qApp->arguments().first().split("/").last(), "iat");

        if (session.lastError().type() != QDBusError::NoError)
            return false;

        m_iat = new com::iflytek::aiservice::iat("com.iflytek.aiservice", path.path(), QDBusConnection::sessionBus(), this);

        QVariantMap param_dict;
        param_dict[QString("accent")] = QVariant(QString("mandarin"));
        param_dict[QString("ptt")] = QVariant(QString("0"));
        const QString &json_data = m_iat->startIat(param_dict);
        QJsonDocument document = QJsonDocument::fromJson(json_data.toLocal8Bit());

        if (document.object().value("status").toInt(-1) != 0) {
            m_iat->deleteLater();
            m_iat = nullptr;
            return false;
        }

        // 清理旧的数据
        m_message.clear();

        connect(m_iat, &ComIflytekAiserviceIatInterface::onEnd, this, &VoiceDevice::onEnd);
        connect(m_iat, &ComIflytekAiserviceIatInterface::onResult, this, &VoiceDevice::onResult);

        return QIODevice::open(mode);
    }

    void close() override
    {
        if (m_iat) {
            m_iat->stopIat();
            m_iat->deleteLater();
            m_iat = nullptr;
        }

        QIODevice::close();
    }

    qint64 readData(char *, qint64) override
    {
        return 0;
    }

    qint64 writeData(const char *data, qint64 len) override
    {
        m_iat->putAudio(QByteArray(data, len), false);

        return len;
    }

    Q_SLOT void onResult(const QString &json)
    {
        QJsonDocument document = QJsonDocument::fromJson(json.toLocal8Bit());

        if (!document.isObject()) {
            return;
        }

        document = QJsonDocument::fromJson(document["text"].toString().toLocal8Bit());
        const QJsonArray &words = document["ws"].toArray();
        bool replace = document["pgs"].toString() == "rpl";

        if (replace) {
            m_message.clear();
        }

        for (const QJsonValue &v : words) {
            const QJsonArray &cw = v["cw"].toArray();

            for (const QJsonValue &v : cw) {
                m_message.append(v["w"].toString());
            }
        }

        Q_EMIT voiceReply(m_message);
    }

    Q_SLOT void onError(const QString &error)
    {
        qDebug() << error;
        Q_EMIT voiceReply(QString());
    }

    Q_SLOT void onEnd()
    {
        close();
        Q_EMIT finished();
    }

Q_SIGNALS:
    void voiceReply(QString text);
    void finished();

private:
    com::iflytek::aiservice::iat *m_iat = nullptr;
    QString m_message;
};
#endif

/*!
  \class Dtk::Widget::DSearchEdit
  \inmodule dtkwidget
  \brief DSearchEdit 类提供了专门用来进行搜索的输入框控件.
  
  相比于一般的输入框控件，DSearchEdit 提供了搜索按钮指示，用户使用起来会更加自然。
  如图示：
  \image html searchedit.png
  
  \warning DSearchEdit 与 QLineEdit、DLineEdit 没有继承关系，功能不兼容。
 */

/*!
  \brief DSearchEdit::DSearchEdit 是 DSearchEdit 类的构造函数.
  \a parent 指定了控件的父控件。
 */
DSearchEdit::DSearchEdit(QWidget *parent)
    : DLineEdit(*new DSearchEditPrivate(this), parent)
{
    D_D(DSearchEdit);

    d->init();
    setSpeechToTextEnabled(false);

    //平板模式下屏蔽搜索框右键菜单
    if (DGuiApplicationHelper::isTabletEnvironment()) {
        d->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

DSearchEdit::~DSearchEdit()
{

}

/*!
  \brief DSearchEdit::setPlaceHolder 设置灰色的占位符文本
  \a placeHolder 占位符文本内容
 */
void DSearchEdit::setPlaceHolder(QString placeHolder)
{
    Q_D(DSearchEdit);
    if (d->placeHolder == placeHolder)
        return;
    d->placeHolder = placeHolder;
    d->label->setText(placeHolder);
}

/*!
  \brief DSearchEdit::placeHolder
  \return 返回占位符文本内容
 */
QString DSearchEdit::placeHolder() const
{
    D_DC(DSearchEdit);

    return d->placeHolder;
}

void DSearchEdit::clear()
{
    lineEdit()->clear();
}

/*!
  \brief DSearchEdit::clearEdit 清除内容，退出编辑状态
 */
void DSearchEdit::clearEdit()
{
    D_D(DSearchEdit);

    lineEdit()->clear();
    d->_q_toEditMode(false);

    if (lineEdit()->hasFocus()) {
        lineEdit()->clearFocus();
    }
}

bool DSearchEdit::isVoiceInput() const
{
#ifndef DTK_NO_MULTIMEDIA
    D_DC(DSearchEdit);
#ifdef ENABLE_AI
    return d->voiceInput && d->voiceInput->state() == QAudio::ActiveState;
#endif // 
#else
    return false;
#endif
}

void DSearchEdit::setPlaceholderText(const QString &text)
{
    D_D(DSearchEdit);
    d->placeholderText = text;
    if (lineEdit()->hasFocus())
        lineEdit()->setPlaceholderText(text);
}

QString DSearchEdit::placeholderText() const
{
    return d_func()->placeholderText;
}

DSearchEditPrivate::DSearchEditPrivate(DSearchEdit *q)
    : DLineEditPrivate(q)
    , action(nullptr)
    , iconWidget(nullptr)
    , label(nullptr)
{
}

DSearchEditPrivate::~DSearchEditPrivate()
{

}

void DSearchEditPrivate::init()
{
    D_Q(DSearchEdit);
    label = new QLabel;
    DIconButton *iconbtn = new DIconButton(DStyle::SP_IndicatorSearch);

    iconbtn->setFlat(true);
    iconbtn->setFocusPolicy(Qt::NoFocus);
    iconbtn->setAttribute(Qt::WA_TransparentForMouseEvents);
    iconbtn->setAccessibleName("DSearchEditIconButton");

    placeHolder = qApp->translate("DSearchEdit", "Search");

    action = new QAction(q);
    action->setObjectName("_d_search_leftAction");
    action->setIcon(QIcon::fromTheme("search_indicator"));
    q->lineEdit()->addAction(action, QLineEdit::LeadingPosition);
    action->setVisible(false);
    iconbtn->setIconSize(QSize(20, 20));

    DPalette pe;
    QStyleOption opt;
    QColor color = DStyleHelper(q->style()).getColor(&opt, pe, DPalette::TextTips);
    pe.setColor(DPalette::TextTips, color);

    label->setPalette(pe);
    label->setText(placeHolder);
    label->setAccessibleName("DSearchEditPlaceHolderLabel");

    q->connect(q, SIGNAL(focusChanged(bool)), q, SLOT(_q_toEditMode(bool)));
    q->connect(q, &DLineEdit::textChanged, q, [ = ](QString text) {
        if (!text.isEmpty())
            _q_toEditMode(false);
    });

    QHBoxLayout *layout = new QHBoxLayout(q->lineEdit());

    iconWidget = new QWidget;
    iconWidget->setObjectName("iconWidget");
    iconWidget->setAccessibleName("DSearchEditIconWidget");
    QHBoxLayout *center_layout = new QHBoxLayout(iconWidget);
    center_layout->setMargin(0);
    center_layout->setSpacing(6);

    layout->setMargin(0);
    layout->setSpacing(0);

    center_layout->addWidget(iconbtn, 0, Qt::AlignVCenter);
    center_layout->addWidget(label, 0, Qt::AlignCenter);
    center_layout->addSpacing(12 / qApp->devicePixelRatio());
    layout->addWidget(iconWidget, 0, Qt::AlignCenter);

    QAction* clearAction = q->lineEdit()->findChild<QAction *>(QLatin1String("_q_qlineeditclearaction"));

    if (clearAction != nullptr) {
        QList<QToolButton *> list = q->lineEdit()->findChildren<QToolButton *>();

        for (int i = 0; i < list.count(); i++) {
            if (list.at(i)->defaultAction() == clearAction) {
                QToolButton *clearBtn = list.at(i);
                //屏蔽lineedit清除按钮的槽函数,_q_clearFocus()获得有效的判断条件
                q->disconnect(clearBtn, SIGNAL(clicked()), q->lineEdit(), nullptr);
                q->connect(clearBtn, SIGNAL(clicked()), q, SLOT(_q_clearFocus()));
            }
        }
    }

#ifdef ENABLE_AI
    voiceAction = new QAction(q);
    voiceAction->setIcon(QIcon::fromTheme("button_voice"));
    voiceAction->setCheckable(true);
    voiceAction->setEnabled(false);
    lineEdit->addAction(voiceAction, QLineEdit::TrailingPosition);

    q->connect(voiceAction, SIGNAL(triggered(bool)), q, SLOT(_q_onVoiceActionTrigger(bool)));
    // 语音输入按钮
    QDBusInterface testSpeechToText("com.iflytek.aiassistant",
                               "/aiassistant/iat",
                               "com.iflytek.aiassistant.iat",
                               QDBusConnection::sessionBus());
    // 测试听写接口是否开启
    QDBusPendingCall call = testSpeechToText.asyncCall("getIatEnable");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, q);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, q, [this](QDBusPendingCallWatcher *pWatcher) {
        QDBusPendingReply<bool> reply = *pWatcher;
        if (reply.isError()) {
            qWarning() << "getIatEnable is ERROR";
        } else {
            voiceAction->setEnabled(reply.argumentAt<0>());
        }
        pWatcher->deleteLater();
    });
#endif
}

void DSearchEditPrivate::_q_toEditMode(bool focus)
{
    D_Q(DSearchEdit);

    if (focus  || !q->lineEdit()->text().isEmpty()) {
        action->setVisible(true);
        iconWidget->setVisible(false);
        lineEdit->setPlaceholderText(placeholderText);
    } else {
        action->setVisible(false);
        iconWidget->setVisible(true);
        lineEdit->setPlaceholderText(QString());
    }

#ifdef ENABLE_AI
    //焦点消失，清除语音check
    if (voiceAction) {
        voiceAction->setChecked(false);
        _q_onVoiceActionTrigger(false);
    }
#endif
}

void DSearchEditPrivate::_q_onVoiceActionTrigger(bool checked)
{
#if (!defined DTK_NO_MULTIMEDIA) && (defined ENABLE_AI)
    if (checked) {
        voiceAction->setIcon(QIcon::fromTheme("button_voice_active"));

        if (!voiceInput) {
            QAudioFormat format;
            format.setSampleRate(16000);
            format.setChannelCount(1);
            format.setSampleSize(16);
            format.setSampleType(QAudioFormat::SignedInt);
            format.setByteOrder(QAudioFormat::LittleEndian);
            format.setCodec("audio/pcm");

            D_Q(DSearchEdit);
            voiceInput = new QAudioInput(format, q);
            voiceIODevice = new VoiceDevice(voiceInput);

            q->connect(voiceIODevice, &VoiceDevice::voiceReply, q, [q, this](const QString & text) {
                q->setText(text);
            }, Qt::QueuedConnection);

            q->connect(voiceIODevice, &VoiceDevice::finished, q, [q, this] {
                // 自动结束录制
                voiceAction->setChecked(false);
                _q_onVoiceActionTrigger(false);
                Q_EMIT q->voiceInputFinished();
            });

            q->connect(voiceInput, &QAudioInput::stateChanged, q, &DSearchEdit::voiceChanged);
        }

        if (voiceIODevice->open(QIODevice::WriteOnly))
            voiceInput->start(voiceIODevice);
    } else {
        voiceAction->setIcon(QIcon::fromTheme("button_voice"));

        if (voiceInput) {
            voiceInput->stop();
            voiceIODevice->close();
            voiceInput->deleteLater();
            voiceInput = nullptr;
        }
    }
#else
    Q_UNUSED(checked)
#endif
}

void DSearchEditPrivate::_q_clearFocus()
{
    Q_Q(DSearchEdit);

    if (!q->text().isEmpty()) {
        q->clearEdit();
    }
    Q_EMIT q->searchAborted();
}

DWIDGET_END_NAMESPACE

#include "moc_dsearchedit.cpp"
#include "dsearchedit.moc"

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
#include <DIconTheme>
#undef ENABLE_AI

#ifdef ENABLE_AI
//iFlytek related
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
DGUI_USE_NAMESPACE

constexpr int ANI_DURATION = 200;
constexpr int HIDE_CURSOR_MARGIN = -4;

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

        // clear old massage
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
@~english
  \class Dtk::Widget::DSearchEdit
  \inmodule dtkwidget
  @brief The DSearchEdit class provides input box widget specifically used for searching
  
  @details The DSearchEdit class inherits from the DLineEdit class.
  Compared to regular input box widgets, DSearchEdit provides search button instructions，thus users can use it more naturally.
  As shown in the figure：

  @image html ../images/DSearchEdit.png
 */

/*!
@~english
  @brief DSearchEdit constructor
  @param[in] parent is the parent widget。
 */
DSearchEdit::DSearchEdit(QWidget *parent)
    : DLineEdit(*new DSearchEditPrivate(this), parent)
{
    D_D(DSearchEdit);

    d->init();
    setSpeechToTextEnabled(false);

    //hide the right-click menu for searching box in tablet environment
    if (DGuiApplicationHelper::isTabletEnvironment()) {
        d->lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

DSearchEdit::~DSearchEdit()
{

}

/*!
  @brief set gray placeholder text
  @param[in] placeHolder is the content of placeholder text
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
@~english
  @brief return the placeholder text
  @return placeholder text content
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
@~english
  @brief clear the input and exit editing status
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
    , animation(nullptr)
{
    if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_SEARCH) {
        animation = new QPropertyAnimation;
        animation->setPropertyName("pos");
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setDuration(ANI_DURATION);
    }
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
    action->setIcon(DIconTheme::findQIcon("search_indicator"));
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
    center_layout->setContentsMargins(0, 0, 0, 0);
    center_layout->setSpacing(6);

    layout->setContentsMargins(0, 0, 0, 0);
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
                //Block the private slots of the lineedit clear button,_q_clearFocus() gains effective jugding condition
                q->disconnect(clearBtn, SIGNAL(clicked()), q->lineEdit(), nullptr);
                q->connect(clearBtn, SIGNAL(clicked()), q, SLOT(_q_clearFocus()));
            }
        }
    }

#ifdef ENABLE_AI
    voiceAction = new QAction(q);
    voiceAction->setIcon(DIconTheme::findQIcon("button_voice"));
    voiceAction->setCheckable(true);
    voiceAction->setEnabled(false);
    lineEdit->addAction(voiceAction, QLineEdit::TrailingPosition);

    q->connect(voiceAction, SIGNAL(triggered(bool)), q, SLOT(_q_onVoiceActionTrigger(bool)));
    // voice input button
    QDBusInterface testSpeechToText("com.iflytek.aiassistant",
                               "/aiassistant/iat",
                               "com.iflytek.aiassistant.iat",
                               QDBusConnection::sessionBus());
    // test whether the dictation interface is enabled
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

    if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_SEARCH) {
        if (animation->state() == QPropertyAnimation::Running)
            return;

        auto textMargins = q->lineEdit()->textMargins();
        QMargins marginsInAnimation(HIDE_CURSOR_MARGIN, 0, 0, 0);

        if (!animation->parent())
            animation->setParent(iconWidget);

        animation->setTargetObject(iconWidget);
        animation->setStartValue(QPoint(q->lineEdit()->geometry().center().x() - iconWidget->width() / 2, iconWidget->pos().y()));
        animation->setEndValue(QPoint(10, iconWidget->pos().y()));

        q->connect(animation, &QPropertyAnimation::finished, q, [q, this, textMargins]() {
            q->lineEdit()->setTextMargins(textMargins);
            if (animation->direction() == QPropertyAnimation::Direction::Forward) {
                iconWidget->setVisible(false);
                action->setVisible(true);
                lineEdit->setPlaceholderText(placeholderText);
            } else {
                iconWidget->setVisible(true);
                lineEdit->setPlaceholderText(QString());
                iconWidget->move(QPoint(q->lineEdit()->geometry().center().x() - iconWidget->width() / 2, iconWidget->pos().y()));
            }
        });

        if (!q->lineEdit()->text().isEmpty())
            return;

        if (focus) {
            animation->setDirection(QPropertyAnimation::Direction::Forward);
        } else {
            action->setVisible(false);
            animation->setDirection(QPropertyAnimation::Direction::Backward);
        }

        iconWidget->setVisible(true);
        q->lineEdit()->setTextMargins(marginsInAnimation);
        animation->start();
    } else {
        if (focus  || !q->lineEdit()->text().isEmpty()) {
            action->setVisible(true);
            iconWidget->setVisible(false);
            lineEdit->setPlaceholderText(placeholderText);
        } else {
            action->setVisible(false);
            iconWidget->setVisible(true);
            lineEdit->setPlaceholderText(QString());
        }

    }

#ifdef ENABLE_AI
    //Focus disappears, clear voice check
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
        voiceAction->setIcon(DIconTheme::findQIcon("button_voice_active"));

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
                // end the recording automatically
                voiceAction->setChecked(false);
                _q_onVoiceActionTrigger(false);
                Q_EMIT q->voiceInputFinished();
            });

            q->connect(voiceInput, &QAudioInput::stateChanged, q, &DSearchEdit::voiceChanged);
        }

        if (voiceIODevice->open(QIODevice::WriteOnly))
            voiceInput->start(voiceIODevice);
    } else {
        voiceAction->setIcon(DIconTheme::findQIcon("button_voice"));

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

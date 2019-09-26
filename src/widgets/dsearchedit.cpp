/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dsearchedit.h"
#include "dpalette.h"
#include "dstyle.h"
#include "private/dsearchedit_p.h"
#include "diconbutton.h"

#ifdef ENABLE_XFYUN
// 讯飞语言相关
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#endif

#include <QAction>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QAction>
#include <QAudioInput>
#include <QTimer>
#include <QAudioDeviceInfo>

DWIDGET_BEGIN_NAMESPACE

#ifdef ENABLE_XFYUN
class VoiceDevice : public QIODevice
{
    Q_OBJECT
public:
    using QIODevice::QIODevice;
    ~VoiceDevice() {
        if (isOpen())
            close();
    }

    bool open(OpenMode mode) override
    {
        if (mode != WriteOnly)
            return false;

        // 先登录
        int            ret                  =    MSP_SUCCESS;
        const char* login_params            =    "appid = 5720401f, work_dir = ."; // 登录参数，appid与msc库绑定,请勿随意改动

        /* 用户登录 */
        ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，均传NULL即可，第三个参数是登录参数

        if (MSP_SUCCESS != ret) {
            qDebug("MSPLogin failed , Error code %d.\n", ret);
            return false; //登录失败，退出登录
        }

#define BUFFER_SIZE 4096 * 4
#define FRAME_LEN 640
#define HINTS_SIZE 100

//        char            hints[HINTS_SIZE]               =    {NULL}; //hints为结束本次会话的原因描述，由用户自定义
        int             errcode               =    MSP_SUCCESS;
        /*
        * sub:                  请求业务类型
        * domain:               领域
        * language:             语言
        * accent:               方言
        * sample_rate:          音频采样率
        * result_type:          识别结果格式
        * result_encoding:      结果编码格式
        *
        * 详细参数说明请参阅《iFlytek MSC Reference Manual》
        */
        const char* session_begin_params = "sub = iat, domain = iat, language = zh_ch, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8";

        // 启动讯飞语言解析的会话
        xfyunSessionID = QISRSessionBegin(NULL, session_begin_params, &errcode); //听写不需要语法，第一个参数为NULL

        if (MSP_SUCCESS != errcode) {
            xfyunSessionID = nullptr;
            qWarning("QISRSessionBegin failed! error code: %d\n", errcode);
            return false;
        }

        aud_stat = MSP_AUDIO_SAMPLE_FIRST; // 重置语音状态

        return QIODevice::open(mode);
    }

    void close() override
    {
        if (xfyunSessionID) {
            QISRSessionEnd(xfyunSessionID, nullptr);
            xfyunSessionID = nullptr;
        }

        MSPLogout();
        QIODevice::close();
    }

    qint64 readData(char *, qint64 ) override
    {
        return 0;
    }

    qint64 writeData(const char *data, qint64 len) override
    {
        unsigned int    total_len                       =    0;
        int             ep_stat                         =    MSP_EP_LOOKING_FOR_SPEECH;        //端点检测
        int             rec_stat                        =    MSP_REC_STATUS_SUCCESS;           //识别状态
        int ret = 0;

        ret = QISRAudioWrite(xfyunSessionID, (const void *)data, len, aud_stat, &ep_stat, &rec_stat);
        aud_stat = MSP_AUDIO_SAMPLE_CONTINUE; // 更新音频状态

        if (MSP_SUCCESS != ret) {
            qDebug("QISRAudioWrite failed! error code: %d\n", ret);
            // 结束录制
            return 0;
        }

        if (MSP_REC_STATUS_SUCCESS == rec_stat) { //已经有部分听写结果
            int             errcode                         =    MSP_SUCCESS;
            const char *rslt = QISRGetResult(xfyunSessionID, &rec_stat, 0, &errcode);

            if (MSP_SUCCESS != errcode) {
                qDebug("QISRGetResult failed! error code: %d\n", errcode);
                // 结束录制
                return 0;
            }

            if (NULL != rslt) {
                unsigned int rslt_len = strlen(rslt);
                total_len += rslt_len;

                if (total_len >= BUFFER_SIZE) {
                    qDebug("no enough buffer for rec_result !\n");
                    // 结束录制
                }

                Q_EMIT voiceReply(QString::fromUtf8(rslt));
            }
        }

        if (MSP_EP_AFTER_SPEECH == ep_stat) {
            qDebug() << "ep_stat" << ep_stat;
            Q_EMIT voiceReply(QString());
        }

        return len;
    }

Q_SIGNALS:
    void voiceReply(QString text);

private:
    const char *xfyunSessionID = nullptr;
    int aud_stat = MSP_AUDIO_SAMPLE_FIRST; //音频状态
};
#endif

/*!
 * \~chinese \class DSearchEdit
 * \~chinese \brief DSearchEdit 类提供了专门用来进行搜索的输入框控件。
 * \~chinese
 * \~chinese 相比于一般的输入框控件，DSearchEdit 提供了搜索按钮指示，用户使用起来会更加自然。
 * \~chinese 如图示：
 * \~chinese \image html searchedit.png
 * \~chinese
 * \~chinese \warning DSearchEdit 与 QLineEdit、DLineEdit 没有继承关系，功能不兼容。
 */

/*!
 * \~chinese \brief DSearchEdit::DSearchEdit 是 DSearchEdit 类的构造函数。
 * \~chinese \param parent 指定了控件的父控件。
 */
DSearchEdit::DSearchEdit(QWidget *parent)
    : DLineEdit(*new DSearchEditPrivate(this), parent)
{
    D_D(DSearchEdit);

    d->init();
}

DSearchEdit::~DSearchEdit()
{

}

void DSearchEdit::setPlaceHolder(QString placeHolder)
{
    Q_D(DSearchEdit);

    d->placeHolder = placeHolder;
}

QString DSearchEdit::placeHolder() const
{
    D_DC(DSearchEdit);

    return d->placeHolder;
}

DSearchEditPrivate::DSearchEditPrivate(DSearchEdit *q)
    : DLineEditPrivate(q)
    , action(nullptr)
{
}

DSearchEditPrivate::~DSearchEditPrivate()
{

}

void DSearchEditPrivate::init()
{
    D_Q(DSearchEdit);

    label = new QLabel;
    placeHolder = DSearchEdit::tr("Search");

    action = new QAction(q);
    action->setIcon(DStyleHelper(q->style()).standardIcon(DStyle::SP_IndicatorSearch, nullptr));

    DPalette pe;
    QStyleOption opt;
    QColor color = DStyleHelper(q->style()).getColor(&opt, pe, DPalette::TextTips);
    pe.setColor(DPalette::TextTips, color);

    label->setPalette(pe);
    label->setText(placeHolder);

    q->setFocusPolicy(Qt::ClickFocus);

    q->connect(q, SIGNAL(focusChanged(bool)), q, SLOT(_q_toEditMode(bool)));

    QHBoxLayout *layout = new QHBoxLayout(q->lineEdit());

    layout->addWidget(label, 0, Qt::AlignCenter);

#ifdef ENABLE_XFYUN
    // 语音输入按钮
    voiceAction = new QAction(q);
    voiceAction->setIcon(QIcon(":/images/voice.svg"));
    voiceAction->setCheckable(true);
    lineEdit->addAction(voiceAction, QLineEdit::TrailingPosition);

    q->connect(voiceAction, SIGNAL(triggered(bool)), q, SLOT(_q_onVoiceActionTrigger(bool)));
#endif
}

void DSearchEditPrivate::_q_toEditMode(bool focus)
{
    D_Q(DSearchEdit);

    if (focus) {
        q->lineEdit()->addAction(action, QLineEdit::LeadingPosition);
        label->setVisible(false);
    } else {
        q->lineEdit()->removeAction(action);

        if (!q->lineEdit()->text().isEmpty()) {
            label->setVisible(false);
        } else {
            label->setVisible(true);
        }
    }
}

void DSearchEditPrivate::_q_onVoiceActionTrigger(bool checked)
{
#ifdef ENABLE_XFYUN
    if (checked) {
        voiceAction->setIcon(QIcon(":/images/voice_activate.svg"));

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

            q->connect(voiceIODevice, &VoiceDevice::voiceReply, q, [q, this] (const QString &text) {
                // 自动结束录制
                voiceAction->setChecked(false);
                _q_onVoiceActionTrigger(false);

                if (!text.isEmpty()) {
                    q->setText(text);
                    Q_EMIT q->voiceInputFinished();
                }
            }, Qt::QueuedConnection);
        }

        if (voiceIODevice->open(QIODevice::WriteOnly))
            voiceInput->start(voiceIODevice);
    } else {
        voiceAction->setIcon(QIcon(":/images/voice.svg"));

        if (voiceInput) {
            voiceInput->stop();
            voiceIODevice->close();
            voiceInput->deleteLater();
            voiceInput = nullptr;
        }
    }
#endif
}

DWIDGET_END_NAMESPACE

#include "moc_dsearchedit.cpp"
#include "dsearchedit.moc"

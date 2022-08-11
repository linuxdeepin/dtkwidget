// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dmpriscontrol.h"
#include "private/dmpriscontrol_p.h"
#include "private/mpris/dmprismonitor.h"

#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QTimer>
#include <QScrollBar>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DMPRISControl
  \inmodule dtkwidget
  \brief DMPRISControl 封装了MPRIS协议，提供了上一曲、下一曲、暂停播放、恢复播放和显示歌曲名和封面的功能.
 */

/*!
  \fn void DMPRISControl::mprisAcquired() const
  \brief 当第一次注册MPRIS接口时会发出该信号
  \fn void DMPRISControl::mprisChanged() const
  \brief 当MPRIS接口列表发生变化时会发出该信号
  \fn void DMPRISControl::mprisLosted() const
  \brief 当MPRIS接口中有退出的服务时会发出该信号
 */

/*!
  \brief DMPRISControl::DMPRISControl 的构造函数
  
  \a parent 初始化QFrame的构造函数
 */
DMPRISControl::DMPRISControl(QWidget *parent)
    : QFrame(parent),
      DObject(*new DMPRISControlPrivate(this))
{
    D_D(DMPRISControl);

    d->init();
}

/*!
  \brief 返回当前是否在工作状态
  
  \return true
  \return false
 */
bool DMPRISControl::isWorking() const
{
    D_DC(DMPRISControl);

    return d->m_mprisInter;
}

/*!
  \brief 设置是否显示封面
  
  \a visible
 */
void DMPRISControl::setPictureVisible(bool visible)
{
    D_D(DMPRISControl);

    d->m_pictureVisible = visible;
    d->m_picture->setVisible(visible);
}

/*!
  \brief 设置封面大小
  
  \a size
 */
void DMPRISControl::setPictureSize(const QSize &size)
{
    D_D(DMPRISControl);

    d->m_picture->setFixedSize(size);
}

void DMPRISControl::showEvent(QShowEvent *event)
{
    D_D(DMPRISControl);

    d->_q_onPlaybackStatusChanged();

    QFrame::showEvent(event);
}

DMPRISControlPrivate::DMPRISControlPrivate(DMPRISControl *q)
    : DObjectPrivate(q), m_mprisInter(nullptr)
{
}

void DMPRISControlPrivate::init()
{
    D_Q(DMPRISControl);

    m_mprisMonitor    = new DMPRISMonitor(q);
    m_titleScrollArea = new QScrollArea(q);
    m_title           = new QLabel;
    m_picture         = new QLabel;
    m_pictureVisible  = true;
    m_controlWidget   = new QWidget;
    m_prevBtn         = new DFloatingButton(m_controlWidget);
    m_playBtn         = new DFloatingButton(m_controlWidget);
    m_nextBtn         = new DFloatingButton(m_controlWidget);
    m_tickEffect      = new DTickEffect(m_title, m_title);

    m_prevBtn->setIcon(QIcon::fromTheme(":/assets/images/play_previous.svg"));
    m_prevBtn->setAccessibleName("DMPRISControlPrevFloatingButton");
    m_playBtn->setIcon(QIcon::fromTheme(":/assets/images/play_start.svg"));
    m_playBtn->setAccessibleName("DMPRISControlPlayFloatingButton");
    m_nextBtn->setIcon(QIcon::fromTheme(":/assets/images/play_next.svg"));
    m_nextBtn->setAccessibleName("DMPRISControlNextFloatingButton");
    m_prevBtn->setBackgroundRole(DPalette::Button);
    m_playBtn->setBackgroundRole(DPalette::Button);
    m_nextBtn->setBackgroundRole(DPalette::Button);
    m_prevBtn->setAutoExclusive(true);
    m_playBtn->setAutoExclusive(true);
    m_nextBtn->setAutoExclusive(true);

    m_title->setAlignment(Qt::AlignCenter);
    m_title->setAccessibleName("DMPRISControlTitleLabel");
    m_picture->setFixedSize(200, 200);
    m_picture->setAccessibleName("DMPRISControlPictureLabel");
    m_prevBtn->setObjectName("PrevBtn");
    m_playBtn->setObjectName("PlayBtn");
    m_nextBtn->setObjectName("NextBtn");

    m_tickEffect->setDirection(DTickEffect::RightToLeft);
    m_tickEffect->setFixedPixelMove(50);

    m_titleScrollArea->setWidget(m_title);
    m_titleScrollArea->setObjectName("scrollarea");
    m_titleScrollArea->setWidgetResizable(true);
    m_titleScrollArea->setFocusPolicy(Qt::NoFocus);
    m_titleScrollArea->setFrameStyle(QFrame::NoFrame);
    m_titleScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    m_titleScrollArea->setContentsMargins(0, 0, 0, 0);
    m_titleScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_titleScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_titleScrollArea->setStyleSheet("background-color:transparent;");
    m_titleScrollArea->setAccessibleName("DMPRISControlScrollArea");
    m_titleScrollArea->viewport()->setAccessibleName("DMPRISControlScrollAreaViewPort");

#ifdef QT_DEBUG
    m_title->setText("MPRIS Title");

    m_nextBtn->setIcon(QIcon::fromTheme(":/assets/images/arrow_right_normal.png"));
    m_playBtn->setIcon(QIcon::fromTheme(":/assets/images/arrow_right_white.png"));
    m_prevBtn->setIcon(QIcon::fromTheme(":/assets/images/arrow_left_normal.png"));
#endif

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_prevBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_playBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_nextBtn);
    controlLayout->setContentsMargins(0, 5, 0, 0);
    m_controlWidget->setLayout(controlLayout);
    m_controlWidget->setAccessibleName("DMPRISControlCentralWidget");

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_titleScrollArea);
    centralLayout->addWidget(m_picture);
    centralLayout->setAlignment(m_picture, Qt::AlignCenter);
//    centralLayout->addLayout(controlLayout);
    centralLayout->addWidget(m_controlWidget);
    centralLayout->setMargin(0);

    q->setLayout(centralLayout);

    q->connect(m_mprisMonitor, SIGNAL(mprisAcquired(const QString &)), q, SLOT(_q_loadMPRISPath(const QString &)));
    q->connect(m_mprisMonitor, SIGNAL(mprisLost(const QString &)), q, SLOT(_q_removeMPRISPath(const QString &)));
    q->connect(m_prevBtn, SIGNAL(clicked()), q, SLOT(_q_onPrevClicked()));

    q->connect(m_playBtn, &DFloatingButton::clicked, q, [q, this] {
        if (m_clickedStatus)
            return;
        m_clickedStatus = true;
        QTimer::singleShot(100, q, SLOT(_q_onPlayClicked()));
    });

    q->connect(m_nextBtn, SIGNAL(clicked()), q, SLOT(_q_onNextClicked()));

    m_mprisMonitor->init();
}

void DMPRISControlPrivate::_q_onPrevClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Previous();
    m_prevBtn->setFocus();
}

void DMPRISControlPrivate::_q_onPlayClicked()
{
    m_clickedStatus = false;
    if (!m_mprisInter)
        return;

    if (m_playStatus) {
        m_mprisInter->Pause();
    } else {
        m_mprisInter->Play();
    }

    m_playBtn->setFocus();
}

void DMPRISControlPrivate::_q_onPauseClicked()
{
    //
}

void DMPRISControlPrivate::_q_onNextClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Next();
    m_nextBtn->setFocus();
}

void DMPRISControlPrivate::_q_onMetaDataChanged()
{
    if (!m_mprisInter) return;

    const auto &   meta        = m_mprisInter->metadata();
    const QString &title       = meta.value("xesam:title").toString();
    const QString &artist      = meta.value("xesam:artist").toString();
    const QUrl &   pictureUrl  = meta.value("mpris:artUrl").toString();
    const QSize &  pictureSize = m_picture->size();
    const QPixmap &picture     = QPixmap(pictureUrl.toLocalFile())
                                 .scaled(pictureSize, Qt::IgnoreAspectRatio);

    if (title.isEmpty()) {
        m_title->clear();
        m_tickEffect->stop();
    }
    else {
        if (artist.isEmpty()) {
            m_title->setText(title);
        }
        else {
            m_title->setText(QString("%1 - %2").arg(title).arg(artist));
        }
        m_title->adjustSize();
        m_tickEffect->play();
    }

    m_picture->setPixmap(picture);
    m_picture->setVisible(m_pictureVisible && !picture.isNull());
}

void DMPRISControlPrivate::_q_onPlaybackStatusChanged()
{
    if (!m_mprisInter)
        return;

    const QString stat = m_mprisInter->playbackStatus();
#ifdef QT_DEBUG
    if (stat == "Playing") {
        m_playStatus = true;
        m_playBtn->setIcon(QIcon::fromTheme(":/assets/images/arrow_right_white.png"));
    } else {
        m_playStatus = false;
        m_playBtn->setIcon(QIcon::fromTheme(":/assets/images/arrow_left_white.png"));
    }
#else
    if (stat == "Playing") {
        m_playStatus = true;
        m_playBtn->setIcon(QIcon::fromTheme(":/assets/images/play_pause.svg"));
    } else {
        m_playStatus = false;
        m_playBtn->setIcon(QIcon::fromTheme(":/assets/images/play_start.svg"));
    }
#endif
}

void DMPRISControlPrivate::_q_loadMPRISPath(const QString &path)
{
    D_Q(DMPRISControl);

    DBusMPRIS *newMpris = new DBusMPRIS(path, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), q);

    // 此属性判断是否支持使用MPRIS控制 真表示能控制 假则忽略这个dbus接口
    if (!newMpris->canShowInUI()) {
        newMpris->deleteLater();
        return;
    }

    const bool hasOld = m_mprisInter;
    m_lastPath = path;

    // save paths
    if (!m_mprisPaths.contains(path))
        m_mprisPaths.append(path);

    if (m_mprisInter)
        m_mprisInter->deleteLater();

    m_mprisInter = newMpris;

    m_controlWidget->setVisible(m_mprisInter->canControl());

    q->connect(m_mprisInter, SIGNAL(MetadataChanged(QVariantMap)), q, SLOT(_q_onMetaDataChanged()));
    q->connect(m_mprisInter, SIGNAL(PlaybackStatusChanged(QString)), q, SLOT(_q_onPlaybackStatusChanged()));
    q->connect(m_mprisInter, SIGNAL(CanControlChanged(bool)), q, SLOT(_q_onCanControlChanged(bool)));

    _q_onMetaDataChanged();
    _q_onPlaybackStatusChanged();

    if (hasOld)
        Q_EMIT q->mprisChanged();
    else
        Q_EMIT q->mprisAcquired();
}

void DMPRISControlPrivate::_q_removeMPRISPath(const QString &path)
{
    D_QC(DMPRISControl);

    m_mprisPaths.removeOne(path);

    if (m_lastPath != path)
        return;

    if (!m_mprisInter)
        return;

    if (!m_mprisPaths.isEmpty())
        return _q_loadMPRISPath(m_mprisPaths.last());

    m_mprisInter->deleteLater();
    m_mprisInter = nullptr;

    Q_EMIT q->mprisLosted();
}

void DMPRISControlPrivate::_q_onCanControlChanged(bool canControl)
{
    m_controlWidget->setVisible(canControl);
}

DWIDGET_END_NAMESPACE

#include "moc_dmpriscontrol.cpp"

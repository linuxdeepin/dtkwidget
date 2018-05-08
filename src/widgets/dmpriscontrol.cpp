/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dmpriscontrol.h"
#include "private/dmpriscontrol_p.h"
#include "private/mpris/dmprismonitor.h"

#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

DMPRISControl::DMPRISControl(QWidget *parent)
    : QFrame(parent),
      DObject(*new DMPRISControlPrivate(this))
{
    D_D(DMPRISControl);

    d->init();
}

bool DMPRISControl::isWorking() const
{
    D_DC(DMPRISControl);

    return d->m_mprisInter;
}

void DMPRISControl::setPictureVisible(bool visible)
{
    D_D(DMPRISControl);

    d->m_pictureVisible = visible;
    d->m_picture->setVisible(visible);
}

void DMPRISControl::setPictureSize(const QSize &size)
{
    D_D(DMPRISControl);

    d->m_picture->setFixedSize(size);
}

DMPRISControlPrivate::DMPRISControlPrivate(DMPRISControl *q)
    : DObjectPrivate(q),

      m_mprisInter(nullptr)
{
}

void DMPRISControlPrivate::init()
{
    D_Q(DMPRISControl);

    m_mprisMonitor = new DMPRISMonitor(q);

    m_title = new QLabel;
    m_title->setAlignment(Qt::AlignCenter);
    m_picture = new QLabel;
    m_picture->setFixedSize(200, 200);
    m_pictureVisible = true;
    m_controlWidget = new QWidget;
    m_prevBtn = new DImageButton;
    m_prevBtn->setObjectName("PrevBtn");
    m_pauseBtn = new DImageButton;
    m_pauseBtn->setObjectName("PauseBtn");
    m_playBtn = new DImageButton;
    m_playBtn->setObjectName("PlayBtn");
    m_nextBtn = new DImageButton;
    m_nextBtn->setObjectName("NextBtn");

#ifdef QT_DEBUG
    m_title->setText("MPRIS Title");
    m_nextBtn->setNormalPic("://images/arrow_right_normal.png");
    m_pauseBtn->setNormalPic("://images/arrow_left_white.png");
    m_playBtn->setNormalPic("://images/arrow_right_white.png");
    m_prevBtn->setNormalPic("://images/arrow_left_normal.png");
#endif

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_prevBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_pauseBtn);
    controlLayout->addWidget(m_playBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_nextBtn);
    controlLayout->setContentsMargins(0, 10, 0, 0);
    m_controlWidget->setLayout(controlLayout);


    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_title);
    centralLayout->addWidget(m_picture);
    centralLayout->setAlignment(m_picture, Qt::AlignCenter);
//    centralLayout->addLayout(controlLayout);
    centralLayout->addWidget(m_controlWidget);
    centralLayout->setMargin(0);

    q->setLayout(centralLayout);

    q->connect(m_mprisMonitor, SIGNAL(mprisAcquired(const QString &)), q, SLOT(_q_loadMPRISPath(const QString &)));
    q->connect(m_mprisMonitor, SIGNAL(mprisLost(const QString &)), q, SLOT(_q_removeMPRISPath(const QString &)));
    q->connect(m_prevBtn, SIGNAL(clicked()), q, SLOT(_q_onPrevClicked()));
    q->connect(m_pauseBtn, SIGNAL(clicked()), q, SLOT(_q_onPauseClicked()));
    q->connect(m_playBtn, SIGNAL(clicked()), q, SLOT(_q_onPlayClicked()));
    q->connect(m_nextBtn, SIGNAL(clicked()), q, SLOT(_q_onNextClicked()));

    m_mprisMonitor->init();
}

void DMPRISControlPrivate::_q_onPrevClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Previous();
}

void DMPRISControlPrivate::_q_onPlayClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Play();
}

void DMPRISControlPrivate::_q_onPauseClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Pause();
}

void DMPRISControlPrivate::_q_onNextClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Next();
}

void DMPRISControlPrivate::_q_onMetaDataChanged()
{
    if (!m_mprisInter)
        return;

    const auto &meta = m_mprisInter->metadata();
    const QString &title = meta.value("xesam:title").toString();
    const QString &artist = meta.value("xesam:artist").toString();
    const QUrl &pictureUrl = meta.value("mpris:artUrl").toString();
    const QSize &pictureSize = m_picture->size();
    const QPixmap &picture = QPixmap(pictureUrl.toLocalFile()).scaled(pictureSize, Qt::IgnoreAspectRatio);

    if (title.isEmpty())
        m_title->clear();
    else
    {
        if (artist.isEmpty())
            m_title->setText(title);
        else
            m_title->setText(QString("%1 - %2").arg(title).arg(artist));
    }

    m_picture->setPixmap(picture);
    m_picture->setVisible(m_pictureVisible && !picture.isNull());
}

void DMPRISControlPrivate::_q_onPlaybackStatusChanged()
{
    const QString stat = m_mprisInter->playbackStatus();

    if (stat == "Playing")
    {
        m_pauseBtn->setVisible(true);
        m_playBtn->setVisible(false);
    } else {
        m_pauseBtn->setVisible(false);
        m_playBtn->setVisible(true);
    }
}

void DMPRISControlPrivate::_q_loadMPRISPath(const QString &path)
{
    D_Q(DMPRISControl);

    const bool hasOld = m_mprisInter;
    m_lastPath = path;

    // save paths
    if (!m_mprisPaths.contains(path))
        m_mprisPaths.append(path);

    if (m_mprisInter)
        m_mprisInter->deleteLater();

    m_mprisInter = new DBusMPRIS(path, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), q);

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
    D_Q(DMPRISControl);

    m_controlWidget->setVisible(canControl);
}

#include "moc_dmpriscontrol.cpp"

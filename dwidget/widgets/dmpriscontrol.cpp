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
    m_prevBtn = new DImageButton;
    m_prevBtn->setObjectName("PrevBtn");
    m_pauseBtn = new DImageButton;
    m_pauseBtn->setObjectName("PauseBtn");
    m_nextBtn = new DImageButton;
    m_nextBtn->setObjectName("NextBtn");

#ifdef QT_DEBUG
    m_title->setText("MPRIS Title");
    m_nextBtn->setNormalPic("://images/arrow_right_normal.png");
    m_pauseBtn->setNormalPic("://images/arrow_left_white.png");
    m_prevBtn->setNormalPic("://images/arrow_left_normal.png");
#endif

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_prevBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_pauseBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(m_nextBtn);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_title);
    centralLayout->addLayout(controlLayout);

    q->setLayout(centralLayout);

    q->connect(m_mprisMonitor, SIGNAL(mprisAcquired(const QString &)), q, SLOT(_q_loadMPRISPath(const QString &)));
    q->connect(m_mprisMonitor, SIGNAL(mprisLost(const QString &)), q, SLOT(_q_removeMPRISPath(const QString &)));
    q->connect(m_prevBtn, SIGNAL(clicked()), q, SLOT(_q_onPrevClicked()));
    q->connect(m_pauseBtn, SIGNAL(clicked()), q, SLOT(_q_onPauseClicked()));
    q->connect(m_nextBtn, SIGNAL(clicked()), q, SLOT(_q_onNextClicked()));

    m_mprisMonitor->init();
}

void DMPRISControlPrivate::_q_onPrevClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->Previous();
}

void DMPRISControlPrivate::_q_onPauseClicked()
{
    if (!m_mprisInter)
        return;

    m_mprisInter->PlayPause();
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

    const auto meta = m_mprisInter->metadata();
    if (meta.contains("xesam:title"))
        m_title->setText(meta.value("xesam:title").toString());
    else
        m_title->clear();
}

void DMPRISControlPrivate::_q_loadMPRISPath(const QString &path)
{
    D_Q(DMPRISControl);

    const bool hasOld = m_mprisInter;
    m_lastPath = path;

    if (m_mprisInter)
        m_mprisInter->deleteLater();

    m_mprisInter = new DBusMPRIS(path, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), q);

    q->connect(m_mprisInter, SIGNAL(MetadataChanged(QVariantMap)), q, SLOT(_q_onMetaDataChanged()));

    _q_onMetaDataChanged();

    if (hasOld)
        emit q->mprisChanged();
    else
        emit q->mprisAcquired();
}

void DMPRISControlPrivate::_q_removeMPRISPath(const QString &path)
{
    D_QC(DMPRISControl);

    if (m_lastPath != path)
        return;

    if (!m_mprisInter)
        return;

    m_mprisInter->deleteLater();
    m_mprisInter = nullptr;

    emit q->mprisLosted();
}

#include "moc_dmpriscontrol.cpp"

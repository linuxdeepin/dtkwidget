/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "indicatortab.h"

#include "dspinner.h"
#include "dpicturesequenceview.h"

#include <QDebug>
#include <QString>
#include <QBoxLayout>

DWIDGET_USE_NAMESPACE

IndicatorTab::IndicatorTab(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("IndicatorTab{background-color: #252627;}");

    auto layout = new QVBoxLayout(this);

    auto *spinnerLayout = new QHBoxLayout();

    auto addSpinner = [ = ](int size, QColor color) {
        auto spinner = new DSpinner;
        spinner->setFixedSize(size, size);
        spinner->setBackgroundColor(color);
        spinnerLayout->addWidget(spinner);
        spinner->start();
    };

    addSpinner(16, Qt::transparent);
    addSpinner(16, Qt::blue);
    addSpinner(32, Qt::green);
    addSpinner(32, Qt::yellow);
    addSpinner(64, Qt::white);
    addSpinner(64, Qt::black);

    spinnerLayout->addStretch();

    auto *loadingContainerLayout = new QHBoxLayout();
    DLoadingIndicator *loadingIndicator = new DLoadingIndicator(this);
    loadingIndicator->setImageSource(QPixmap(":/images/loading_indicator.png"));
    loadingIndicator->setLoading(true);
    loadingIndicator->setAniDuration(2000);
    loadingIndicator->setAniEasingCurve(QEasingCurve::OutInCubic);

    DLoadingIndicator *loadingIndicator2 = new DLoadingIndicator(this);
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(":/images/reload_normal.png"));
    label->setAttribute(Qt::WA_TranslucentBackground);
    loadingIndicator2->setFixedSize(label->sizeHint());
    loadingIndicator2->setWidgetSource(label);
    loadingIndicator2->setSmooth(true);
    loadingIndicator2->setLoading(true);

    loadingContainerLayout->addWidget(loadingIndicator);
    loadingContainerLayout->addWidget(loadingIndicator2);
    loadingContainerLayout->addStretch();

    QStringList seq;
    for (int i(1); i != 91; ++i) {
        seq.append(QString(":/images/Spinner/Spinner%1.png").arg(i, 2, 10, QChar('0')));
    }

    QStringList seq2;
    for (int i(1); i != 91; ++i) {
        seq2.append(QString(":/images/eLoading/eLoading_%1.png").arg(i, 2, 10, QChar('0')));
    }

    DPictureSequenceView *seqView = new DPictureSequenceView(this);
    seqView->setPictureSequence(":/images/Spinner/Spinner%1.png", QPair<int, int>(1, 91), 2);
    seqView->setFixedSize(50, 50);
    seqView->setSpeed(20);

    DPictureSequenceView *seqView2 = new DPictureSequenceView(this);
    seqView2->setPictureSequence(seq);
    seqView2->setFixedSize(50, 50);
    seqView2->setSpeed(33);

    DPictureSequenceView *seqView3 = new DPictureSequenceView(this);
    seqView3->setPictureSequence(seq);
    seqView3->setFixedSize(50, 50);
    seqView3->setSpeed(66);

    DPictureSequenceView *seqView4 = new DPictureSequenceView(this);
    seqView4->setPictureSequence(seq2);
    seqView4->setFixedSize(50, 50);
    seqView4->setSpeed(20);

    DPictureSequenceView *seqView5 = new DPictureSequenceView(this);
    seqView5->setPictureSequence(seq2);
    seqView5->setFixedSize(50, 50);
    seqView5->setSpeed(33);

    DPictureSequenceView *seqView6 = new DPictureSequenceView(this);
    seqView6->setPictureSequence(seq2);
    seqView6->setFixedSize(50, 50);
    seqView6->setSpeed(66);

    auto seqViewLayout = new QHBoxLayout;
    seqViewLayout->addWidget(seqView);
    seqViewLayout->addWidget(seqView2);
    seqViewLayout->addWidget(seqView3);
    seqViewLayout->addWidget(seqView4);
    seqViewLayout->addWidget(seqView5);
    seqViewLayout->addWidget(seqView6);
    seqViewLayout->addStretch();


    DPictureSequenceView *seqViewOnce = new DPictureSequenceView(this);
    seqViewOnce->setPictureSequence(seq);
    seqViewOnce->setFixedSize(50, 50);
    seqViewOnce->setSpeed(33);
    seqViewOnce->setSingleShot(true);

    auto seqOnceViewLayout = new QHBoxLayout;
    seqOnceViewLayout->addWidget(seqViewOnce);
    seqOnceViewLayout->addStretch();

    layout->addLayout(spinnerLayout);
    layout->addLayout(loadingContainerLayout);
    layout->addLayout(seqViewLayout);
    layout->addLayout(seqOnceViewLayout);
    layout->addStretch();

    seqView->play();
    seqView2->play();
    seqView3->play();
    seqView4->play();
    seqView5->play();
    seqView6->play();

    setLayout(layout);
}

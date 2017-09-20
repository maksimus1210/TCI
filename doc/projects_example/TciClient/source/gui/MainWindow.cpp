// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  pPlotter(new QCustomPlot)
{
    setupUi(this);

    lePlotter->addWidget(pPlotter);
    pPlotter->addGraph();

    m_spectrumBuffer.resize(SpectrumSize);
    m_window.resize(SpectrumSize);
    m_signal.resize(SpectrumSize);
    m_magnitude.resize(SpectrumSize);

    connect(pbConnect, &QPushButton::toggled, this, &MainWindow::onConnect);
    connect(&m_tciClient, &TciClient::openStatusChanged, this, &MainWindow::onConnectStatus);
    connect(&m_tciClient, &TciClient::message, teLog, &QTextEdit::append);

    connect(&m_tciClient.trxState(), &TciTrxState::started, [=](){ pbStart->setChecked(true);  });
    connect(&m_tciClient.trxState(), &TciTrxState::stoped , [=](){ pbStart->setChecked(false); });
    connect(pbStart, &QPushButton::toggled, [=](bool state) {
        if (state)
            m_tciClient.trxState().start();
        else
            m_tciClient.trxState().stop();
    });

    connect(&m_tciClient.trxState(), &TciTrxState::vfoLimitsChanged, [=](quint64 min, quint64 max) {
        sbRx1DdsFreq->setMinimum(min);
        sbRx1DdsFreq->setMaximum(max);

        sbRx2DdsFreq->setMinimum(min);
        sbRx2DdsFreq->setMaximum(max);

        sbRx1FilterBFreq->setMinimum(-static_cast<double>(max));
        sbRx1FilterBFreq->setMaximum( max);

        sbRx2FilterBFreq->setMinimum(-static_cast<double>(max));
        sbRx2FilterBFreq->setMaximum( max);
    });

    connect(&m_tciClient.trxState(), &TciTrxState::ifLimitsChanged, [=](int min, int max) {
        sbRx1FilterAFreq->setMinimum(min);
        sbRx1FilterAFreq->setMaximum(max);

        sbRx2FilterAFreq->setMinimum(min);
        sbRx2FilterAFreq->setMaximum(max);
    });

    connect(&m_tciClient.trxState(), &TciTrxState::ddsFreqChanged, [=](quint32 trx, double value) {
        if (trx == 0u)
            sbRx1DdsFreq->setValue(value);
        else if (trx == 1u)
            sbRx2DdsFreq->setValue(value);
    });

    connect(sbRx1DdsFreq, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
        m_tciClient.trxState().setDdsFreq(0u, value);
    });
    connect(sbRx2DdsFreq, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
        m_tciClient.trxState().setDdsFreq(1u, value);
    });


    connect(&m_tciClient.trxState(), &TciTrxState::ifFreqChanged, [=](quint32 trx, quint32 channel, double value) {
        if (trx == 0u) {
            if (channel == 0u)
                sbRx1FilterAFreq->setValue(value);
            else if (channel == 1u)
                sbRx1FilterBFreq->setValue(value);
        }
        else if (trx == 1u) {
            if (channel == 0u)
                sbRx2FilterAFreq->setValue(value);
            else if (channel == 1u)
                sbRx2FilterBFreq->setValue(value);
        }
    });
    connect(sbRx1FilterAFreq, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
        m_tciClient.trxState().setIfFreq(0u, 0u, value);
    });
    connect(sbRx1FilterBFreq, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
        m_tciClient.trxState().setIfFreq(0u, 1u, value);
    });
    connect(sbRx2FilterAFreq, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
        m_tciClient.trxState().setIfFreq(1u, 0u, value);
    });
    connect(sbRx2FilterBFreq, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
        m_tciClient.trxState().setIfFreq(1u, 1u, value);
    });


    m_c1 = connect(cbRx1Mode, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &text) {
        m_tciClient.trxState().setModulation(0u, text.toLower());
    });
    m_c2 = connect(cbRx2Mode, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &text) {
        m_tciClient.trxState().setModulation(1u, text.toLower());
    });


    connect(&m_tciClient.trxState(), &TciTrxState::modulationsListChanged, [=](const QStringList &list) {
        disconnect(m_c1);
        disconnect(m_c2);

        cbRx1Mode->clear();
        cbRx2Mode->clear();

        for (auto &text : list) {
            cbRx1Mode->addItem(text.toUpper());
            cbRx2Mode->addItem(text.toUpper());
        }

        m_c1 = connect(cbRx1Mode, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &text) {
            m_tciClient.trxState().setModulation(0u, text.toLower());
        });
        m_c2 = connect(cbRx2Mode, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &text) {
            m_tciClient.trxState().setModulation(1u, text.toLower());
        });
    });


    connect(&m_tciClient.trxState(), &TciTrxState::modulationChanged, [=](quint32 trx, const QString &mode) {
        int t_index = indefOf(cbRx1Mode, mode);
        if (t_index >= 0) {
            if (trx == 0u)
                cbRx1Mode->setCurrentIndex(t_index);
            else if (trx == 1u)
                cbRx2Mode->setCurrentIndex(t_index);
        }
    });


    connect(&m_tciClient.trxState(), &TciTrxState::iqOutSampleRateChanged, [=](quint32 value) {
        if (value == 48000u)
            cbIqSampleRate->setCurrentIndex(0);
        else if (value == 96000u)
            cbIqSampleRate->setCurrentIndex(1);
        else if (value == 192000u)
            cbIqSampleRate->setCurrentIndex(2);
    });

    connect(&m_tciClient.trxState(), &TciTrxState::iqStartChanged, [=](quint32 trx, bool state) {
        if (trx == 0u)
            pbRx1StartIQ->setChecked(state);
        else if (trx == 1u)
            pbRx2StartIQ->setChecked(state);
    });


    connect(pbRx1StartAudio, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setAudioStart(0u, state);
    });
    connect(pbRx2StartAudio, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setAudioStart(1u, state);
    });


    connect(pbRx1StartIQ, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setIqStart(0u, state);
    });
    connect(pbRx2StartIQ, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setIqStart(1u, state);
    });
    connect(cbIqSampleRate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        m_tciClient.trxState().setIqOutSampleRate(48000 << index);
    });

    connect(pbRxEnable, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setRxEnable(1u, state);
    });
    connect(&m_tciClient.trxState(), &TciTrxState::rxEnableChanged, [=](quint32 trx, bool state) {
        if (trx == 1u)
            pbRxEnable->setChecked(state);
    });

    connect(&m_tciClient.trxState(), &TciTrxState::txEnableChanged, [=](quint32 trx, bool state) {
        if (trx == 0u)
            gbTx1->setEnabled(state);
        else if (trx == 1u)
            gbTx2->setEnabled(state);
    });

    connect(&m_tciClient.trxState(), &TciTrxState::trxChanged, [=](quint32 trx, bool state) {
        if (trx == 0u)
            pbTx1->setChecked(state);
        else if (trx == 1u)
            pbTx2->setChecked(state);
    });
    connect(pbTx1, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setTrx(0u, state);
    });
    connect(pbTx2, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setTrx(1u, state);
    });


    connect(pbRx1CwMakros, &QPushButton::clicked, [=]() {
        if (!leRx1CwMakros->text().isEmpty())
            m_tciClient.trxState().setMacros(0u, leRx1CwMakros->text());

        leRx1CwMakros->clear();
    });
    connect(pbRx2CwMakros, &QPushButton::clicked, [=]() {
        if (!leRx2CwMakros->text().isEmpty())
            m_tciClient.trxState().setMacros(1u, leRx2CwMakros->text());

        leRx2CwMakros->clear();
    });


    connect(&m_tciClient.trxState(), &TciTrxState::cwSpeedChanged, [=](quint32 value) {
        sbCwSpeed->setValue(value);
    });
    connect(sbCwSpeed, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
        m_tciClient.trxState().setCwSpeed(value);
    });


    connect(&m_tciClient.trxState(), &TciTrxState::cwMacrosDelayChanged, [=](quint32 value) {
        sbCwDelay->setValue(value);
    });
    connect(sbCwDelay, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
        m_tciClient.trxState().setCwMacrosDelay(value);
    });

    connect(pbSendSpot, &QPushButton::clicked, [=]() {
        if (leSpotCallsign->text().isEmpty())
            return;

        m_tciClient.trxState().setSpot(leSpotCallsign->text(),
                                       cbRx1Mode->currentText(),
                                       sbRx1DdsFreq->value() + sbRx1FilterAFreq->value(),
                                       Qt::red,
                                       QStringLiteral("Demo text"));

        leSpotCallsign->clear();
    });

    connect(pbDeleteSpot, &QPushButton::clicked, [=]() {
        if (leSpotCallsign->text().isEmpty())
            return;

        m_tciClient.trxState().setDeleteSpot(leSpotCallsign->text());
        leSpotCallsign->clear();
    });

    connect(pbRx1Rit, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setRitEnable(0u, state);
    });
    connect(pbRx2Rit, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setRitEnable(1u, state);
    });
    connect(&m_tciClient.trxState(), &TciTrxState::ritEnableChanged, [=](quint32 trx, bool state) {
        if (trx == 0u)
            pbRx1Rit->setChecked(state);
        else if (trx == 1u)
            pbRx2Rit->setChecked(state);
    });


    connect(pbRx1Xit, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setXitEnable(0u, state);
    });
    connect(pbRx2Xit, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setXitEnable(1u, state);
    });
    connect(&m_tciClient.trxState(), &TciTrxState::xitEnableChanged, [=](quint32 trx, bool state) {
        if (trx == 0u)
            pbRx1Xit->setChecked(state);
        else if (trx == 1u)
            pbRx2Xit->setChecked(state);
    });


    connect(pbRx1Split, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setSplitEnable(0u, state);
    });
    connect(pbRx2Split, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setSplitEnable(1u, state);
    });
    connect(&m_tciClient.trxState(), &TciTrxState::splitEnableChanged, [=](quint32 trx, bool state) {
        if (trx == 0u)
            pbRx1Split->setChecked(state);
        else if (trx == 1u)
            pbRx2Split->setChecked(state);
    });


    connect(&m_tciClient.trxState(), &TciTrxState::ritOffsetChanged, [=](quint32 trx, int value) {
        if (trx == 0)
            sbRx1RitOffset->setValue(value);
        else if (trx == 1)
            sbRx2RitOffset->setValue(value);
    });
    connect(sbRx1RitOffset, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](int value) {
        m_tciClient.trxState().setRitOffset(0u, value);
    });
    connect(sbRx2RitOffset, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](int value) {
        m_tciClient.trxState().setRitOffset(1u, value);
    });


    connect(&m_tciClient.trxState(), &TciTrxState::xitOffsetChanged, [=](quint32 trx, int value) {
        if (trx == 0)
            sbRx1XitOffset->setValue(value);
        else if (trx == 1)
            sbRx2XitOffset->setValue(value);
    });
    connect(sbRx1XitOffset, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](int value) {
        m_tciClient.trxState().setXitOffset(0u, value);
    });
    connect(sbRx2XitOffset, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](int value) {
        m_tciClient.trxState().setXitOffset(1u, value);
    });

    connect(&m_tciClient.trxState(), &TciTrxState::channelEnableChanged, [=](quint32 trx, quint32 channel, bool state) {
        if (trx == 0 && channel == 1u)
            pbRx1SubRx->setChecked(state);
        else if (trx == 1)
            pbRx2SubRx->setChecked(state);
    });
    connect(pbRx1SubRx, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setChannelEnable(0u, 1u, state);
    });
    connect(pbRx2SubRx, &QPushButton::toggled, [=](bool state) {
        m_tciClient.trxState().setChannelEnable(1u, 1u, state);
    });



    connect(pbCwMessageSend, &QPushButton::clicked, [=]() {
        if (lePathCallsign->text().isEmpty() || leCwMessageAfter->text().isEmpty())
            return;

        m_tciClient.trxState().setCwMessage(0u, leCwMessageBefore->text(), lePathCallsign->text(), leCwMessageAfter->text());

        lePathCallsign->clear();
        leCwMessageBefore->clear();
        leCwMessageAfter->clear();
    });

    connect(pbCwMessageCallsign, &QPushButton::clicked, [=]() {
        if (leCallsign->text().isEmpty())
            return;

        m_tciClient.trxState().addCwMessageCallsign(leCallsign->text());

        leCallsign->clear();
    });


    connect(pbCwTerminate, &QPushButton::clicked, &m_tciClient.trxState(), &TciTrxState::setCwMacrosStop);
    connect(&m_tciClient, &TciClient::readyReadIq, this, &MainWindow::onReadIq);
    connect(&m_timer    , &QTimer::timeout, this, &MainWindow::onUpdateSpectrum);



    m_timer.start(100);
}

void MainWindow::onConnect(bool state)
{
    pbRx1StartAudio->setChecked(false);
    pbRx2StartAudio->setChecked(false);

    if (state) {
        QUrl t_url("ws://" + leAddress->text() + ":" + QString::number(sbPort->value()));
        m_tciClient.open(t_url);
    }
    else {
        m_tciClient.close();
    }
}

void MainWindow::onConnectStatus(bool state)
{
    if (state) {
        sbStatus->showMessage(tr("Connected."), 2000);
    }
    else {
        pbConnect->setChecked(false);
        sbStatus->showMessage(tr("Disconnected."), 2000);
    }
}

int MainWindow::indefOf(QComboBox *p, const QString &text)
{
    if (p == nullptr)
        return -1;

    for (int i = 0; i < p->count(); ++i) {
        if (p->itemText(i) == text.toUpper())
            return i;
    }

    return -1;
}

void MainWindow::onUpdateSpectrum()
{
    int t_spectrumWidth = 48000 << cbIqSampleRate->currentIndex();
    pPlotter->xAxis->setRange(-t_spectrumWidth/2, t_spectrumWidth/2);
    pPlotter->yAxis->setRange(-140, 0);

    if (!m_spectrumBuffer.readyRead())
        return;

    m_spectrumBuffer.readAll(m_signal);
    m_window.process(m_signal.data(), m_signal.length());
    Spectrum::fft(m_signal.data(), m_signal.length(), true);
    Spectrum::magnitude(m_signal.data(), m_magnitude.data(), m_signal.length());

    QVector<double> t_x(m_signal.length()), t_y(m_signal.length());

    double t_xBegin = -t_spectrumWidth/2.0;
    double t_xStep  = static_cast<double>(t_spectrumWidth)/m_signal.length();

    int t_index = SpectrumSize/2;
    int t_mask  = SpectrumSize - 1;

    for (int i = 0; i < m_signal.length(); ++i, t_xBegin += t_xStep) {
        t_x[i] = t_xBegin;
        t_y[i] = m_magnitude[t_index++&t_mask];
    }

    pPlotter->graph(0)->setData(t_x, t_y);

    pPlotter->replot();
}

void MainWindow::onReadIq()
{
    auto t_vector = m_tciClient.iqData();
    m_spectrumBuffer.write(t_vector);
}




























// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "TciTrxState.h"

namespace ExpertElectronics {

static const QString CmdStart("start");
static const QString CmdPramp("preamp");
static const QString CmdDds("dds");
static const QString CmdIf("if");
static const QString CmdTrx("trx");
static const QString CmdMode("mode");
static const QString CmdIqStart("iq_start");
static const QString CmdAudioStart("audio_start");
static const QString CmdRxEnable("rx_enable");
static const QString CmdTxEnable("tx_enable");
static const QString CmdRxChannel("rx_channel");
static const QString CmdRitEnable("rit_enable");
static const QString CmdXitEnable("xit_enable");
static const QString CmdSplitEnable("split_enable");
static const QString CmdRitOffset("rit_offset");
static const QString CmdXitOffset("xit_offset");
static const QString CmdRxFilterLow("rx_filter_l");
static const QString CmdRxFilterHigh("rx_filter_h");

TciTrxState::TciTrxState(QObject *parent) :
  QObject(parent),
  m_trxCount(2u),
  m_channelsCount(2u),
  m_vfoMin(0u),
  m_vfoMax(30000000u),
  m_ifMin(-24000),
  m_ifMax(24000),
  m_iqOutSampleRate(48000u),
  m_cwSpeed(30u),
  m_cwDelay(50u)
{
    m_state[CmdStart]           = false;
    m_state[CmdPramp]           = 0;
    m_state[CmdDds + ":0"]      = 1800000.0;
    m_state[CmdIf + ":0,0"]     = 0.0;
    m_state[CmdIf + ":0,1"]     = 0.0;
    m_state[CmdTrx + ":0"]      = false;
    m_state[CmdRxEnable + ":0"] = true;
    m_state[CmdTxEnable + ":0"] = false;
}

void TciTrxState::setTrxCount(quint32 value)
{
    m_trxCount = qMax(value, 1u);
}
quint32 TciTrxState::trxCount() const
{
    return m_trxCount;
}

void TciTrxState::setChannelsCount(quint32 value)
{
    m_channelsCount = qMax(value, 2u);
}
quint32 TciTrxState::channelsCount() const
{
    return m_channelsCount;
}

void TciTrxState::setRxEnable(quint32 trx, bool state)
{
    if ((trx == 0u) || (trx >= m_trxCount))
        return;

    QString t_key = CmdRxEnable + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit rxEnableChanged(trx, state);
}
bool TciTrxState::rxEnable(quint32 trx) const
{
    if (trx == 0u)
        return true;
    else if (trx >= m_trxCount)
        return false;

    QString t_key = CmdRxEnable + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setTxEnable(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdTxEnable + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit txEnableChanged(trx, state);
}
bool TciTrxState::txEnable(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdTxEnable + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setVfoLimits(quint64 min, quint64 max)
{
    if ((m_vfoMin == min) && (m_vfoMax == max))
        return;

    m_vfoMin = min;
    m_vfoMax = max;

    emit vfoLimitsChanged(m_vfoMin, m_vfoMax);
}
qint64 TciTrxState::vfoMin() const
{
    return m_vfoMin;
}
qint64 TciTrxState::vfoMax() const
{
    return m_vfoMax;
}

void TciTrxState::setIfLimits(int min, int max)
{
    if ((m_ifMin == min) && (m_ifMax == max))
        return;

    m_ifMin = min;
    m_ifMax = max;

    emit ifLimitsChanged(min, max);
}
qint64 TciTrxState::ifMin() const
{
    return m_ifMin;
}
qint64 TciTrxState::ifMax() const
{
    return m_ifMax;
}

void TciTrxState::setModulationsList(const QStringList &list)
{
    if (m_mudulations == list)
        return;

    m_mudulations = list;
    emit modulationsListChanged(list);
}
QStringList TciTrxState::modulationsList() const
{
    return m_mudulations;
}

void TciTrxState::start()
{
    if (m_state[CmdStart].toBool())
        return;

    m_state[CmdStart] = true;
    emit started();
}
void TciTrxState::stop()
{
    if (!m_state[CmdStart].toBool())
        return;

    m_state[CmdStart] = false;
    emit stoped();
}

bool TciTrxState::isStart() const
{
    return m_state[CmdStart].toBool();
}

void TciTrxState::setPreamp(int value)
{
    if (m_state[CmdPramp].toInt() == value)
        return;

    m_state[CmdPramp] = value;
    emit preampChanged(value);
}
int TciTrxState::preamp() const
{
    return m_state[CmdPramp].toInt();
}

void TciTrxState::setDdsFreq(quint32 trx, double freq)
{
    if ((trx >= m_trxCount) || (freq < 0.0))
        return;

    QString t_key = CmdDds + ":" + QString::number(trx);
    if (static_cast<qint64>(m_state[t_key].toDouble(0.0)) == static_cast<qint64>(freq))
        return;

    m_state[t_key] = freq;
    emit ddsFreqChanged(trx, freq);
}
double TciTrxState::ddsFreq(quint32 trx)
{
    if (trx >= m_trxCount)
        return 0.0;

    return m_state[CmdDds + ":" + QString::number(trx)].toDouble(0.0);
}

void TciTrxState::setIfFreq(quint32 trx, quint32 channel, double freq)
{
    if ((trx >= m_trxCount) || (channel >= m_channelsCount))
        return;

    QString t_key = CmdIf + ":" + QString::number(trx) + "," + QString::number(channel);
    if (static_cast<qint64>(m_state[t_key].toDouble(0.0)) == static_cast<qint64>(freq))
        return;

    m_state[t_key] = freq;
    emit ifFreqChanged(trx, channel, freq);
}
double TciTrxState::ifFreq(quint32 trx, quint32 channel)
{
    if ((trx >= m_trxCount) || (channel >= m_channelsCount))
        return 0.0;

    return m_state[CmdIf + ":" + QString::number(trx) + "," + QString::number(channel)].toDouble(0.0);
}

void TciTrxState::setTrx(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdTrx + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit trxChanged(trx, state);
}
bool TciTrxState::trx(quint32 trx)
{
    if (trx >= m_trxCount)
        return false;

    return m_state[CmdTrx + ":" + QString::number(trx)].toBool(false);
}

void TciTrxState::setModulation(quint32 trx, const QString &mode)
{
    if (trx >= m_trxCount)
        return;
    else if (!m_mudulations.contains(mode.toLower()))
        return;

    QString t_key  = CmdMode + ":" + QString::number(trx);
    if (m_state[t_key].toString() == mode.toLower())
        return;

    m_state[t_key] = mode.toLower();
    emit modulationChanged(trx, mode.toLower());
}
QString TciTrxState::modulation(quint32 trx)
{
    if (trx >= m_trxCount)
        return m_mudulations.first();

    QString t_key = CmdMode + ":" + QString::number(trx);
    return m_state[t_key].toString(m_mudulations.first());
}

void TciTrxState::setIqOutSampleRate(quint32 value)
{
    if (m_iqOutSampleRate == value)
        return;

    m_iqOutSampleRate = value;
    emit iqOutSampleRateChanged(value);

}
quint32 TciTrxState::iqOutSampleRate() const
{
    return m_iqOutSampleRate;
}


void TciTrxState::setIqStart(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdIqStart + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit iqStartChanged(trx, state);
}
bool TciTrxState::iqStart(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdIqStart + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setAudioStart(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdAudioStart + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit audioStartChanged(trx, state);
}
bool TciTrxState::audioStart(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdAudioStart + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setMacros(quint32 trx, const QString &text)
{
    emit cwMacros(trx, text);
}

void TciTrxState::setCwMacrosStop()
{
    emit cwMakrosStop();
}

void TciTrxState::setCwSpeed(quint32 value)
{
    if (m_cwSpeed == value)
        return;

    m_cwSpeed = value;
    emit cwSpeedChanged(m_cwSpeed);
}
quint32 TciTrxState::cwSpeed() const noexcept
{
    return m_cwSpeed;
}

void TciTrxState::setCwMacrosDelay(quint32 value)
{
    if (m_cwDelay == value)
        return;

    m_cwDelay = qBound(10u, value, 1000u);
    emit cwMacrosDelayChanged(m_cwDelay);
}
quint32 TciTrxState::cwMacrosDelay() const noexcept
{
    return m_cwDelay;
}

void TciTrxState::setSpot(const QString &callsign,
             const QString &mode,
             quint64 freq,
             const QColor &color,
             const QString &text)
{
    emit spot(callsign, mode, freq, color, text);
}

void TciTrxState::setDeleteSpot(const QString &callsign)
{
    emit deleteSpot(callsign);
}

void TciTrxState::setRitEnable(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdRitEnable + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit ritEnableChanged(trx, state);
}
bool TciTrxState::ritEnable(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdRitEnable + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setXitEnable(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdXitEnable + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit xitEnableChanged(trx, state);
}
bool TciTrxState::xitEnable(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdXitEnable + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setSplitEnable(quint32 trx, bool state)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdSplitEnable + ":" + QString::number(trx);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit splitEnableChanged(trx, state);
}
bool TciTrxState::splitEnable(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdSplitEnable + ":" + QString::number(trx);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setRitOffset(quint32 trx, int value)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdRitOffset + ":" + QString::number(trx);
    if (m_state[t_key].toInt(0) == value)
        return;

    m_state[t_key] = value;
    emit ritOffsetChanged(trx, value);
}
int TciTrxState::ritOffset(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdRitOffset + ":" + QString::number(trx);
    return m_state[t_key].toInt(0);
}

void TciTrxState::setXitOffset(quint32 trx, int value)
{
    if (trx >= m_trxCount)
        return;

    QString t_key = CmdXitOffset + ":" + QString::number(trx);
    if (m_state[t_key].toInt(0) == value)
        return;

    m_state[t_key] = qBound(-50000, value, 50000);
    emit xitOffsetChanged(trx, value);
}
int TciTrxState::xitOffset(quint32 trx) const
{
    if (trx >= m_trxCount)
        return false;

    QString t_key = CmdXitOffset + ":" + QString::number(trx);
    return m_state[t_key].toInt(0);
}

void TciTrxState::setChannelEnable(quint32 trx, quint32 channel, bool state)
{
    if (trx >= m_trxCount)
        return;

    if (channel == 0u)
        return;

    QString t_key = CmdRxChannel + ":" + QString::number(trx) + "," + QString::number(channel);
    if (m_state[t_key].toBool(false) == state)
        return;

    m_state[t_key] = state;
    emit channelEnableChanged(trx, channel, state);
}
bool TciTrxState::channelEnable(quint32 trx, quint32 channel) const
{
    if (trx >= m_trxCount)
        return false;

    if (channel == 0u)
        return true;

    QString t_key = CmdRxChannel + ":" + QString::number(trx) + "," + QString::number(channel);
    return m_state[t_key].toBool(false);
}

void TciTrxState::setRxFilter(quint32 trx, int low, int high)
{
    if (trx >= m_trxCount)
        return;

    QString t_keyL = CmdRxFilterLow  + ":" + QString::number(trx);
    QString t_keyH = CmdRxFilterHigh + ":" + QString::number(trx);

    if ((m_state[t_keyL].toInt(0) == low) && (m_state[t_keyH].toInt(0) == high))
        return;

    m_state[t_keyL] = low;
    m_state[t_keyH] = high;

    emit rxFilterChanged(trx, low, high);
}
bool TciTrxState::rxFilter(quint32 trx, int &low, int &high)
{
    if (trx >= m_trxCount)
        return false;

    QString t_keyL = CmdRxFilterLow  + ":" + QString::number(trx);
    QString t_keyH = CmdRxFilterHigh + ":" + QString::number(trx);

    low  = m_state[t_keyL].toInt(0);
    high = m_state[t_keyH].toInt(0);

    return true;
}


void TciTrxState::setCwMessage(quint32 trx, const QString &before, const QString &callsign, const QString &after)
{
    emit cwMessage(trx, before, callsign, after);
}
void TciTrxState::addCwMessageCallsign(const QString &callsign)
{
    emit cwMessageCallsign(callsign);
}




}  // namespace ExpertElectronics




















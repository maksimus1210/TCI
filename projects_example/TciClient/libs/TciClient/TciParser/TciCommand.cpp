// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "TciCommand.h"

namespace ExpertElectronics {

//=================================================================================================
// START

TciCommandStart::TciCommandStart(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::started, [=](){ status(); });
}
TciCommandStart::~TciCommandStart()
{
    disconnect(m_c);
}
bool TciCommandStart::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (current.toLower() == QStringLiteral("start")) {
            m_control.start();
            list.removeOne(current);
            t_ok = true;
        }
    }
    return t_ok;
}
void TciCommandStart::status()
{
    if (m_control.isStart())
        emit message(QStringLiteral("start;"));
}


//=================================================================================================
// STOP

TciCommandStop::TciCommandStop(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::stoped, [=](){ status(); });
}
TciCommandStop::~TciCommandStop()
{
    disconnect(m_c);
}
bool TciCommandStop::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (current.toLower() == QStringLiteral("stop")) {
            m_control.stop();
            list.removeOne(current);
            t_ok = true;
        }
    }
    return t_ok;
}
void TciCommandStop::status()
{
    if (!m_control.isStart())
        emit message(QStringLiteral("stop;"));
}

//=================================================================================================
// DDS
TciCommandDds::TciCommandDds(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::ddsFreqChanged, [=](quint32 trx, double freq)
    { Q_UNUSED(freq) currentStatus(trx); });
}
TciCommandDds::~TciCommandDds()
{
    disconnect(m_c);
}
bool TciCommandDds::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("dds"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("dds"))
            continue;

        if (t_list.count() == 2) {
            t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
            if (t_list.count() != 2)
                continue;

            int t_trx = t_list.first().toInt(&t_ok);
            if (t_ok) {
                qint64 t_freq = t_list.last().toLongLong(&t_ok);
                if (t_ok)
                    m_control.setDdsFreq(t_trx, static_cast<double>(t_freq));
            }

        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandDds::status()
{
    currentStatus();
}
void TciCommandDds::currentStatus(int trx)
{
    if (trx < 0) {
        // высылаем все DDS частоты
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("dds:") + QString::number(i) +
                         QStringLiteral(",") +
                         QString::number(static_cast<qint64>(m_control.ddsFreq(i))) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("dds:") + QString::number(trx) +
                         QStringLiteral(",") +
                         QString::number(static_cast<qint64>(m_control.ddsFreq(trx))) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// IF
TciCommandIf::TciCommandIf(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::ifFreqChanged, [=](quint32 trx, quint32 channel, double freq)
    { Q_UNUSED(freq) currentStatus(trx, channel); });
}
TciCommandIf::~TciCommandIf()
{
    disconnect(m_c);
}
bool TciCommandIf::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("if"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if ((t_list.count() != 2))
            continue;
        else if (t_list.first().toLower() != QStringLiteral("if"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 3) {
            int t_trx = t_list.at(0).toInt(&t_ok);
            if (t_ok) {
                int t_channel = t_list.at(1).toInt(&t_ok);
                if (t_ok) {
                    qint64 t_freq = t_list.at(2).toLongLong(&t_ok);
                    if (t_ok)
                        m_control.setIfFreq(t_trx, t_channel, static_cast<double>(t_freq));
                }
            }
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandIf::status()
{
    currentStatus();
}
void TciCommandIf::currentStatus(int trx, int channel)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            if (channel < 0) {
                for (quint32 j = 0u; j < m_control.channelsCount(); ++j)
                    emit message(command(i, j));
            }
            else {
                if (channel < m_control.channelsCount())
                    emit message(command(i, channel));
            }
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            if (channel < 0) {
                for (quint32 j = 0u; j < m_control.channelsCount(); ++j)
                    emit message(command(trx, j));
            }
            else {
                if (channel < m_control.channelsCount())
                    emit message(command(trx, channel));
            }
        }
    }
}
QString TciCommandIf::command(int trx, int channel)
{
    return (QStringLiteral("if:") + QString::number(trx) +
            QStringLiteral(",") + QString::number(channel) +
            QStringLiteral(",") +
            QString::number(static_cast<qint64>(m_control.ifFreq(trx, channel))) +
            QStringLiteral(";"));
}


//=================================================================================================
// TRX
TciCommandTrx::TciCommandTrx(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::trxChanged, [=](quint32 trx, bool state)
    { Q_UNUSED(state) currentStatus(trx); });
}
TciCommandTrx::~TciCommandTrx()
{
    disconnect(m_c);
}
bool TciCommandTrx::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("trx"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first() != QStringLiteral("trx"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                if (t_list.last().toLower() == QStringLiteral("true"))
                    m_control.setTrx(t_trx, true);
                else if (t_list.last().toLower() == QStringLiteral("false"))
                    m_control.setTrx(t_trx, false);
            }
        }
        else if (t_list.count() == 1) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandTrx::status()
{
    currentStatus();
}
void TciCommandTrx::currentStatus(int trx)
{
    if (trx < 0) {
        // высылаем состояние TRX
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("trx:") + QString::number(i) +
                         QStringLiteral(",") +
                         (m_control.trx(i) ?  QStringLiteral("true") : QStringLiteral("false")) +
                          QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("trx:") + QString::number(trx) +
                         QStringLiteral(",") +
                         (m_control.trx(trx) ?  QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// VFO_LIMITS
TciCommandVfoLimits::TciCommandVfoLimits(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
}
bool TciCommandVfoLimits::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("vfo_limits"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("vfo_limits"))
            continue;

        if (t_list.count() == 2) {
            t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
            if (t_list.count() != 2)
                continue;

            quint64 t_low = t_list.first().toULongLong(&t_ok);
            if (!t_ok)
                continue;

            quint64 t_high = t_list.last().toULongLong(&t_ok);
            if (!t_ok)
                continue;

            m_control.setVfoLimits(t_low, t_high);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}

//=================================================================================================
// IF_LIMITS
TciCommandIfLimits::TciCommandIfLimits(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
}
bool TciCommandIfLimits::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("if_limits"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("if_limits"))
            continue;

        if (t_list.count() == 2) {
            t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
            if (t_list.count() != 2)
                continue;

            int t_low = t_list.first().toInt(&t_ok);
            if (!t_ok)
                continue;

            int t_high = t_list.last().toInt(&t_ok);
            if (!t_ok)
                continue;

            m_control.setIfLimits(t_low, t_high);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}

//=================================================================================================
// MODULATIONS_LIST
TciCommandModulationsList::TciCommandModulationsList(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
}
bool TciCommandModulationsList::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("modulations_list"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() != 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("modulations_list"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 0)
            continue;

        m_control.setModulationsList(t_list);

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}

//=================================================================================================
// MODULATION
TciCommandModulation::TciCommandModulation(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::modulationChanged, [=](quint32 trx, QString mode)
    { Q_UNUSED(mode) currentStatus(trx); });
}
TciCommandModulation::~TciCommandModulation()
{
    disconnect(m_c);
}
bool TciCommandModulation::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("modulation"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() != 2)
            continue;
        else if (t_list.first() != QStringLiteral("modulation"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            int t_trx = t_list.first().toInt(&t_ok);
            if (t_ok)
                m_control.setModulation(t_trx, t_list.last());
        }
        else if (t_list.count() == 1) {
            int t_trx = t_list.first().toInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }

    return t_ok;
}
void TciCommandModulation::status()
{
    currentStatus();
}
void TciCommandModulation::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i)
            emit message(QStringLiteral("modulation:") + QString::number(i) + "," + m_control.modulation(i) + ";");
    }
    else {
        if (m_control.trxCount() > trx)
            emit message(QStringLiteral("modulation:") + QString::number(trx) + "," + m_control.modulation(trx) + ";");
    }
}


//=================================================================================================
// IQ_SAMPLERATE
TciCommandIqSampleRate::TciCommandIqSampleRate(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::iqOutSampleRateChanged, [=]() { status(); });
}
TciCommandIqSampleRate::~TciCommandIqSampleRate()
{
    disconnect(m_c);
}
bool TciCommandIqSampleRate::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("iq_samplerate"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first() != QStringLiteral("iq_samplerate"))
            continue;

        if (t_list.count() == 2) {
            quint32 t_value = t_list.last().toUInt(&t_ok);
            if (t_ok) {
                if ((t_value == 48000u) || (t_value == 96000u) || (t_value == 192000u))
                    m_control.setIqOutSampleRate(t_value);
            }
        }
        else if (t_list.count() == 1) {
            status();
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandIqSampleRate::status()
{
    emit message(QStringLiteral("iq_samplerate:") + QString::number(m_control.iqOutSampleRate()) + ";");
}


//=================================================================================================
// IQ_START
TciCommandIqStart::TciCommandIqStart(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::iqStartChanged, [=](quint32 trx, bool state) { Q_UNUSED(state); onStatus(trx); });
}
TciCommandIqStart::~TciCommandIqStart()
{
    disconnect(m_c);
}
bool TciCommandIqStart::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("iq_start"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first() != QStringLiteral("iq_start"))
            continue;

        if (t_list.count() == 2) {
            quint32 t_trx = t_list.last().toUInt(&t_ok);
            if (t_ok)
                m_control.setIqStart(t_trx, true);
        }
        else if (t_list.count() == 1) {
            status();
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandIqStart::status()
{
    onStatus();
}

void TciCommandIqStart::onStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i)
            if (m_control.iqStart(i))
                emit message(QStringLiteral("iq_start:") + QString::number(i) + ";");
    }
    else {
        if (trx < m_control.trxCount()) {
            if (m_control.iqStart(trx))
                emit message(QStringLiteral("iq_start:") + QString::number(trx) + ";");
        }
    }
}

//=================================================================================================
// IQ_STOP
TciCommandIqStop::TciCommandIqStop(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&control, &TciTrxState::iqStartChanged, [=](quint32 trx, bool state) { Q_UNUSED(state); onStatus(trx); });
}
TciCommandIqStop::~TciCommandIqStop()
{
    disconnect(m_c);
}
bool TciCommandIqStop::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("iq_stop"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first() != QStringLiteral("iq_stop"))
            continue;

        if (t_list.count() == 2) {
            quint32 t_trx = t_list.last().toUInt(&t_ok);
            if (t_ok)
                m_control.setIqStart(t_trx, false);
        }
        else if (t_list.count() == 1) {
            status();
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandIqStop::status()
{
    onStatus();
}

void TciCommandIqStop::onStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i)
            if (!m_control.iqStart(i))
                emit message(QStringLiteral("iq_stop:") + QString::number(i) + ";");
    }
    else {
        if (trx < m_control.trxCount()) {
            if (!m_control.iqStart(trx))
                emit message(QStringLiteral("iq_stop:") + QString::number(trx) + ";");
        }
    }
}

//=================================================================================================
// AUDIO_START
TciCommandAudioStart::TciCommandAudioStart(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    m_c = connect(&control, &TciTrxState::audioStartChanged, [=](quint32 trx, bool state)
    { Q_UNUSED(state); currentStatus(trx); });
}
TciCommandAudioStart::~TciCommandAudioStart()
{
    disconnect(m_c);
}
bool TciCommandAudioStart::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}
void TciCommandAudioStart::status()
{
    currentStatus();
}
void TciCommandAudioStart::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            if (m_control.audioStart(i))
                emit message(QStringLiteral("audio_start:") + QString::number(i) + ";");
        }
    }
    else {
        if (trx < m_control.trxCount()) {
            if (m_control.audioStart(trx))
                emit message(QStringLiteral("audio_start:") + QString::number(trx) + ";");
        }
    }
}

//=================================================================================================
// AUDIO_STOP
TciCommandAudioStop::TciCommandAudioStop(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    m_c = connect(&control, &TciTrxState::audioStartChanged, [=](quint32 trx, bool state)
    { Q_UNUSED(state); currentStatus(trx); });
}
TciCommandAudioStop::~TciCommandAudioStop()
{
    disconnect(m_c);
}
bool TciCommandAudioStop::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}
void TciCommandAudioStop::status()
{
    currentStatus();
}
void TciCommandAudioStop::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            if (!m_control.audioStart(i))
                emit message(QStringLiteral("audio_stop:") + QString::number(i) + ";");
        }
    }
    else {
        if (trx < m_control.trxCount()) {
            if (!m_control.audioStart(trx))
                emit message(QStringLiteral("audio_stop:") + QString::number(trx) + ";");
        }
    }
}

//=================================================================================================
// RX_ENABLE
TciCommandRxEnable::TciCommandRxEnable(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c1 = connect(&control, &TciTrxState::rxEnableChanged, [=](quint32 trx, bool state)
    { Q_UNUSED(state); currentStatus(trx); });
}
TciCommandRxEnable::~TciCommandRxEnable()
{
    disconnect(m_c1);
}
bool TciCommandRxEnable::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("rx_enable"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first() != QStringLiteral("rx_enable"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                if (t_list.last().toLower() == QStringLiteral("true"))
                    m_control.setRxEnable(t_trx, true);
                else if (t_list.last().toLower() == QStringLiteral("false"))
                    m_control.setRxEnable(t_trx, false);
            }
        }
        else if (t_list.count() == 1) {
            quint32 t_trx = t_list.last().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandRxEnable::status()
{
    currentStatus();
}
void TciCommandRxEnable::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("rx_enable:") + QString::number(i) +
                         QStringLiteral(",") +
                         (m_control.rxEnable(i) ?  QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("rx_enable:") + QString::number(trx) +
                         QStringLiteral(",") +
                         (m_control.rxEnable(trx) ?  QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// TX_ENABLE
TciCommandTxEnable::TciCommandTxEnable(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
}
bool TciCommandTxEnable::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("tx_enable"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first() != QStringLiteral("tx_enable"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                if (t_list.last().toLower() == QStringLiteral("true"))
                    m_control.setTxEnable(t_trx, true);
                else if (t_list.last().toLower() == QStringLiteral("false"))
                    m_control.setTxEnable(t_trx, false);
            }
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}


//=================================================================================================
// CW_MACROS
TciCommandCwMacros::TciCommandCwMacros(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    m_c = connect(&m_control, &TciTrxState::cwMacros, [=](quint32 trx, const QString &text) {
        QString t_text = text.toUpper();

        // symbol ':' replace to '^'
        // symbol ',' replace to '~'
        // symbol ';' replace to '*'
        t_text.replace(":", "^");
        t_text.replace(",", "~");
        t_text.replace(";", "*");

        emit message(QStringLiteral("cw_macros:") + QString::number(trx) + QStringLiteral(",") + t_text + QStringLiteral(";"));
    });
}
TciCommandCwMacros::~TciCommandCwMacros()
{
    disconnect(m_c);
}
bool TciCommandCwMacros::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}


//=================================================================================================
// CW_MSG
TciCommandCwMessage::TciCommandCwMessage(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control),
  m_regExpCallsign("[^A-Za-z0-9$]")
{
    m_parser.attach(this);

    m_c1 = connect(&m_control, &TciTrxState::cwMessage, [=](quint32 trx, const QString &before, const QString &callsign, const QString &after) {
        QString t_text1 = before.toUpper();
        QString t_text2 = after.toUpper();

        // symbol ':' replace to '^'
        // symbol ',' replace to '~'
        // symbol ';' replace to '*'
        t_text1.replace(":", "^");
        t_text1.replace(",", "~");
        t_text1.replace(";", "*");

        t_text2.replace(":", "^");
        t_text2.replace(",", "~");
        t_text2.replace(";", "*");

        QString t_callsign = callsign.toUpper();
        t_callsign.remove(m_regExpCallsign);

        if (t_text1.isEmpty())
            t_text1 = QStringLiteral("_");

        if (t_text2.isEmpty())
            t_text2 = QStringLiteral("_");

        emit message(QStringLiteral("cw_msg:") + QString::number(trx) + QStringLiteral(",") + t_text1+
                                                                        QStringLiteral(",") + t_callsign +
                                                                        QStringLiteral(",") + t_text2 +
                                                                        QStringLiteral(";"));
    });

    m_c2 = connect(&m_control, &TciTrxState::cwMessageCallsign, [=](const QString &callsign) {
        QString t_text = callsign;
        t_text.remove(m_regExpCallsign);

        emit message(QStringLiteral("cw_msg:") + callsign + QStringLiteral(";"));
    });
}
TciCommandCwMessage::~TciCommandCwMessage()
{
    disconnect(m_c1);
    disconnect(m_c2);
}
bool TciCommandCwMessage::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}


//=================================================================================================
// CW_MAKROS_STOP
TciCommandCwMacrosStop::TciCommandCwMacrosStop(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    m_c = connect(&m_control, &TciTrxState::cwMakrosStop, [=]() {
        emit message(QStringLiteral("cw_macros_stop;"));
    });
}
TciCommandCwMacrosStop::~TciCommandCwMacrosStop()
{
    disconnect(m_c);
}
bool TciCommandCwMacrosStop::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}


//=================================================================================================
// CW_SPEED
TciCommandCwSpeed::TciCommandCwSpeed(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    m_c = connect(&m_control, &TciTrxState::cwSpeedChanged, [=]() { status(); });

}
TciCommandCwSpeed::~TciCommandCwSpeed()
{
    disconnect(m_c);
}
bool TciCommandCwSpeed::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("cw_macros_speed"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() != 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("cw_macros_speed"))
            continue;

            quint32 t_wpm = t_list.last().toUInt(&t_ok);
            if (t_ok)
                m_control.setCwSpeed(qBound(1u, t_wpm, 60u));

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandCwSpeed::status()
{
    emit message(QStringLiteral("cw_macros_speed:") + QString::number(m_control.cwSpeed()) + QStringLiteral(";"));
}


//=================================================================================================
// CW_MACROS_DELAY
TciCommandCwMacrosDelay::TciCommandCwMacrosDelay(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::cwMacrosDelayChanged, [=]()
          { status(); });

}
TciCommandCwMacrosDelay::~TciCommandCwMacrosDelay()
{
    disconnect(m_c);
}
bool TciCommandCwMacrosDelay::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("cw_macros_delay"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("cw_macros_delay"))
            continue;

        if (t_list.count() == 2) {
            quint32 t_delay = t_list.last().toUInt(&t_ok);
            if (t_ok)
                m_control.setCwMacrosDelay(qBound(10u, t_delay, 1000u));
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandCwMacrosDelay::status()
{
    emit message(QStringLiteral("cw_macros_delay:") + QString::number(m_control.cwMacrosDelay()) + QStringLiteral(";"));
}


//=================================================================================================
// SPOT
TciCommandSpot::TciCommandSpot(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    connect(&m_control, &TciTrxState::spot, this, &TciCommandSpot::onSpot);
}
bool TciCommandSpot::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}
void TciCommandSpot::onSpot(const QString &callsign,
                            const QString &mode,
                            quint64 freq,
                            const QColor &color,
                            const QString &text)
{

    emit message(QStringLiteral("spot:") +
                 callsign + QStringLiteral(",") +
                 mode + QStringLiteral(",") +
                 QString::number(freq) + QStringLiteral(",") +
                 QString::number(color.rgb()) + QStringLiteral(",") +
                 text + QStringLiteral(";"));
}

//=================================================================================================
// SPOT
TciCommandSpotDelete::TciCommandSpotDelete(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);

    connect(&m_control, &TciTrxState::deleteSpot, this, &TciCommandSpotDelete::onSpot);
}
bool TciCommandSpotDelete::process(QStringList &list)
{
    Q_UNUSED(list);
    return false;
}
void TciCommandSpotDelete::onSpot(const QString &callsign)
{
    emit message(QStringLiteral("spot_delete:") +
                 callsign + QStringLiteral(";"));
}


//=================================================================================================
// Команда RIT_ENABLE
TciCommandRitEnable::TciCommandRitEnable(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::ritEnableChanged, [=](quint32 trx, bool state)
          { Q_UNUSED(state); currentStatus(trx); });

}
TciCommandRitEnable::~TciCommandRitEnable()
{
    disconnect(m_c);
}
bool TciCommandRitEnable::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("rit_enable"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 3)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("rit_enable"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                if (t_list.last().toLower() == QStringLiteral("true"))
                    m_control.setRitEnable(t_trx, true);
                else if (t_list.last().toLower() == QStringLiteral("false"))
                    m_control.setRitEnable(t_trx, false);
            }
        }
        else {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandRitEnable::status()
{
    currentStatus();
}
void TciCommandRitEnable::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("rit_enable:") + QString::number(i) + QStringLiteral(",") +
                         (m_control.ritEnable(i) ? QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("rit_enable:") + QString::number(trx) + QStringLiteral(",") +
                         (m_control.ritEnable(trx) ? QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
}

//=================================================================================================
// XIT_ENABLE
TciCommandXitEnable::TciCommandXitEnable(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::xitEnableChanged, [=](quint32 trx, bool state)
          { Q_UNUSED(state); currentStatus(trx); });

}
TciCommandXitEnable::~TciCommandXitEnable()
{
    disconnect(m_c);
}
bool TciCommandXitEnable::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("xit_enable"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 3)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("xit_enable"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                if (t_list.last().toLower() == QStringLiteral("true"))
                    m_control.setXitEnable(t_trx, true);
                else if (t_list.last().toLower() == QStringLiteral("false"))
                    m_control.setXitEnable(t_trx, false);
            }
        }
        else {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandXitEnable::status()
{
    currentStatus();
}
void TciCommandXitEnable::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("xit_enable:") + QString::number(i) + QStringLiteral(",") +
                         (m_control.xitEnable(i) ? QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("xit_enable:") + QString::number(trx) + QStringLiteral(",") +
                         (m_control.xitEnable(trx) ? QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// SPLIT_ENABLE
TciCommandSplitEnable::TciCommandSplitEnable(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::splitEnableChanged, [=](quint32 trx, bool state)
          { Q_UNUSED(state); currentStatus(trx); });

}
TciCommandSplitEnable::~TciCommandSplitEnable()
{
    disconnect(m_c);
}
bool TciCommandSplitEnable::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("split_enable"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 3)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("split_enable"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                if (t_list.last().toLower() == QStringLiteral("true"))
                    m_control.setSplitEnable(t_trx, true);
                else if (t_list.last().toLower() == QStringLiteral("false"))
                    m_control.setSplitEnable(t_trx, false);
            }
        }
        else {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandSplitEnable::status()
{
    currentStatus();
}
void TciCommandSplitEnable::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("split_enable:") + QString::number(i) + QStringLiteral(",") +
                         (m_control.splitEnable(i) ? QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("split_enable:") + QString::number(trx) + QStringLiteral(",") +
                         (m_control.splitEnable(trx) ? QStringLiteral("true") : QStringLiteral("false")) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// RIT_OFFSET
TciCommandRitOffset::TciCommandRitOffset(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::ritOffsetChanged, [=](quint32 trx, int value)
          { Q_UNUSED(value); currentStatus(trx); });

}
TciCommandRitOffset::~TciCommandRitOffset()
{
    disconnect(m_c);
}
bool TciCommandRitOffset::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("rit_offset"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 3)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("rit_offset"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                int t_value = t_list.last().toInt(&t_ok);
                if (t_ok)
                    m_control.setRitOffset(t_trx, t_value);
            }
        }
        else {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandRitOffset::status()
{
    currentStatus();
}
void TciCommandRitOffset::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("rit_offset:") + QString::number(i) + QStringLiteral(",") +
                         QString::number(m_control.ritOffset(i)) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("rit_offset:") + QString::number(trx) + QStringLiteral(",") +
                         QString::number(m_control.ritOffset(trx)) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// XIT_OFFSET
TciCommandXitOffset::TciCommandXitOffset(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::xitOffsetChanged, [=](quint32 trx, int value)
          { Q_UNUSED(value); currentStatus(trx); });

}
TciCommandXitOffset::~TciCommandXitOffset()
{
    disconnect(m_c);
}
bool TciCommandXitOffset::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("xit_offset"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 3)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("xit_offset"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                int t_value = t_list.last().toInt(&t_ok);
                if (t_ok)
                    m_control.setXitOffset(t_trx, t_value);
            }
        }
        else {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandXitOffset::status()
{
    currentStatus();
}
void TciCommandXitOffset::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            emit message(QStringLiteral("xit_offset:") + QString::number(i) + QStringLiteral(",") +
                         QString::number(m_control.xitOffset(i)) +
                         QStringLiteral(";"));
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            emit message(QStringLiteral("xit_offset:") + QString::number(trx) + QStringLiteral(",") +
                         QString::number(m_control.xitOffset(trx)) +
                         QStringLiteral(";"));
        }
    }
}


//=================================================================================================
// RX_CHANNEL
TciCommandRxChannelEnable::TciCommandRxChannelEnable(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c = connect(&m_control, &TciTrxState::channelEnableChanged, [=](quint32 trx, quint32 channel, bool state)
          { Q_UNUSED(state); currentStatus(trx, channel); });


}
TciCommandRxChannelEnable::~TciCommandRxChannelEnable()
{
    disconnect(m_c);
}
bool TciCommandRxChannelEnable::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("rx_channel_enable"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 2)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("rx_channel_enable"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 3) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                quint32 t_channel = t_list.at(1).toUInt(&t_ok);
                if (t_ok)
                    m_control.setChannelEnable(t_trx, t_channel, t_list.last().toLower() == QStringLiteral("true"));
            }
        }
        else if (t_list.count() == 2) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                quint32 t_channel = t_list.last().toUInt(&t_ok);
                if (t_ok)
                    currentStatus(t_trx, t_channel);
            }
        }
        else if (t_list.count() == 1) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandRxChannelEnable::status()
{
    currentStatus();
}
void TciCommandRxChannelEnable::currentStatus(int trx, int channel)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i) {
            if (channel < 0) {
                for (quint32 j = 0u; j < m_control.channelsCount(); ++j)
                    emit message(command(i, j));
            }
            else {
                if (channel < m_control.channelsCount())
                    emit message(command(i, channel));
            }
        }
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount()) {
            if (channel < 0) {
                for (quint32 j = 0u; j < m_control.channelsCount(); ++j)
                    emit message(command(trx, j));
            }
            else {
                if (channel < m_control.channelsCount())
                    emit message(command(trx, channel));
            }
        }
    }
}
QString TciCommandRxChannelEnable::command(int trx, int channel)
{
    return (QStringLiteral("rx_channel_enable:") + QString::number(trx) +
            QStringLiteral(",") + QString::number(channel) +
            QStringLiteral(",") +
            (m_control.channelEnable(trx, channel) ? QStringLiteral("true") : QStringLiteral("false")) +
            QStringLiteral(";"));
}



//=================================================================================================
// RX_FILTER
TciCommandRxFilter::TciCommandRxFilter(TciParserAbstract &parser, TciTrxState &control, QObject *parent) :
  TciParserCommandAbsract(parent),
  m_parser(parser),
  m_control(control)
{
    m_parser.attach(this);
    m_c1 = connect(&m_control, &TciTrxState::rxFilterChanged, [=](quint32 trx, int, int)
    { currentStatus(trx); });

}
TciCommandRxFilter::~TciCommandRxFilter()
{
    disconnect(m_c1);
}
bool TciCommandRxFilter::process(QStringList &list)
{
    bool t_ok = false;
    QListIterator<QString> i(list);
    while (i.hasNext()) {
        QString current = i.next();
        if (!current.contains(QStringLiteral("rx_filer_band"), Qt::CaseInsensitive))
            continue;

        QStringList t_list = current.split(QStringLiteral(":"), QString::SkipEmptyParts);
        if (t_list.count() > 3)
            continue;
        else if (t_list.first().toLower() != QStringLiteral("rx_filer_band"))
            continue;

        t_list = t_list.last().split(QStringLiteral(","), QString::SkipEmptyParts);
        if (t_list.count() == 3) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok) {
                int t_low = t_list.at(1).toInt(&t_ok);
                if (t_ok) {
                    int t_high = t_list.last().toInt(&t_ok);
                    if (t_ok) {
                        if (t_low > t_high)
                            std::swap(t_low, t_high);

                        m_control.setRxFilter(t_trx, t_low, t_high);
                    }
                }
            }
        }
        else if (t_list.count() == 1) {
            quint32 t_trx = t_list.first().toUInt(&t_ok);
            if (t_ok)
                currentStatus(t_trx);
        }

        list.removeOne(current);
        t_ok = true;
    }
    return t_ok;
}
void TciCommandRxFilter::status()
{
    currentStatus();
}
void TciCommandRxFilter::currentStatus(int trx)
{
    if (trx < 0) {
        for (quint32 i = 0u; i < m_control.trxCount(); ++i)
            emit message(command(i));
    }
    else {
        if (static_cast<quint32>(trx) < m_control.trxCount())
            emit message(command(trx));
    }
}
QString TciCommandRxFilter::command(int trx)
{
    int t_low = 0, t_high = 0;
    m_control.rxFilter(trx, t_low, t_high);

    return (QStringLiteral("rx_filer_band:") + QString::number(trx) +
            QStringLiteral(",") + QString::number(t_low)  +
            QStringLiteral(",") + QString::number(t_high) +
            QStringLiteral(";"));
}


}  // namespace ExpertElectronics




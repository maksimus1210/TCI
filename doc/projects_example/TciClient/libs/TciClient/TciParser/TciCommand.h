// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TCICOMMAND_H
#define TCICOMMAND_H

#include "TciParserCommandAbsract.h"
#include "TciParserAbstract.h"
#include "../TciTrxState/TciTrxState.h"

namespace ExpertElectronics {

class TciCommandStart : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandStart(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandStart();
    bool process(QStringList &strings) override;
    void status() override;

private:
    TciCommandStart() = delete;
    Q_DISABLE_COPY(TciCommandStart)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandStop : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandStop(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandStop();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandStop() = delete;
    Q_DISABLE_COPY(TciCommandStop)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandDds : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandDds(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandDds();
    bool process(QStringList &string) override;
    void status() override;

private slots:
    void currentStatus(int trx = -1);

private:
    TciCommandDds() = delete;
    Q_DISABLE_COPY(TciCommandDds)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandIf : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandIf(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandIf();
    bool process(QStringList &string) override;
    void status() override;

private slots:
    void currentStatus(int trx = -1, int channel = -1);

private:
    TciCommandIf() = delete;
    Q_DISABLE_COPY(TciCommandIf)

    QString command(int trx, int channel);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandTrx : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandTrx(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandTrx();
    bool process(QStringList &string) override;
    void status() override;

private slots:
    void currentStatus(int trx = -1);

private:
    TciCommandTrx() = delete;
    Q_DISABLE_COPY(TciCommandTrx)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandVfoLimits : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandVfoLimits(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    bool process(QStringList &string) override;

private:
    TciCommandVfoLimits() = delete;
    Q_DISABLE_COPY(TciCommandVfoLimits)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
};

class TciCommandIfLimits : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandIfLimits(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    bool process(QStringList &list) override;

private:
    TciCommandIfLimits() = delete;
    Q_DISABLE_COPY(TciCommandIfLimits)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
};

class TciCommandModulationsList : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandModulationsList(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    bool process(QStringList &list) override;

private:
    TciCommandModulationsList() = delete;
    Q_DISABLE_COPY(TciCommandModulationsList)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
};

class TciCommandModulation : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandModulation(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandModulation();
    bool process(QStringList &list) override;
    void status() override;

private slots:
    void currentStatus(int trx = -1);

private:
    TciCommandModulation() = delete;
    Q_DISABLE_COPY(TciCommandModulation)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandIqSampleRate : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandIqSampleRate(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandIqSampleRate();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandIqSampleRate() = delete;
    Q_DISABLE_COPY(TciCommandIqSampleRate)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandIqStart : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandIqStart(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandIqStart();
    bool process(QStringList &string) override;
    void status() override;

private slots:
    void onStatus(int trx = -1);

private:
    TciCommandIqStart() = delete;
    Q_DISABLE_COPY(TciCommandIqStart)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandIqStop : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandIqStop(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandIqStop();
    bool process(QStringList &string) override;
    void status() override;

private slots:
    void onStatus(int trx = -1);

private:
    TciCommandIqStop() = delete;
    Q_DISABLE_COPY(TciCommandIqStop)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandAudioStart final : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandAudioStart(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandAudioStart();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandAudioStart() = delete;
    Q_DISABLE_COPY(TciCommandAudioStart)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandAudioStop final : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandAudioStop(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandAudioStop();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandAudioStop() = delete;
    Q_DISABLE_COPY(TciCommandAudioStop)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandRxEnable : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandRxEnable(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandRxEnable();
    bool process(QStringList &string) override;
    void status() override;

private slots:
    void currentStatus(int trx = -1);

private:
    TciCommandRxEnable() = delete;
    Q_DISABLE_COPY(TciCommandRxEnable)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c1;
};

class TciCommandTxEnable : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandTxEnable(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    bool process(QStringList &string) override;

private:
    TciCommandTxEnable() = delete;
    Q_DISABLE_COPY(TciCommandTxEnable)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
};

class TciCommandCwMacros : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandCwMacros(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandCwMacros();
    bool process(QStringList &list) override;

private:
    TciCommandCwMacros() = delete;
    Q_DISABLE_COPY(TciCommandCwMacros)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandCwMacrosStop : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandCwMacrosStop(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandCwMacrosStop();
    bool process(QStringList &list) override;

private:
    TciCommandCwMacrosStop() = delete;
    Q_DISABLE_COPY(TciCommandCwMacrosStop)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandCwSpeed : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandCwSpeed(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandCwSpeed();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandCwSpeed() = delete;
    Q_DISABLE_COPY(TciCommandCwSpeed)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandCwMacrosDelay : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandCwMacrosDelay(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandCwMacrosDelay();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandCwMacrosDelay() = delete;
    Q_DISABLE_COPY(TciCommandCwMacrosDelay)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandSpot : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandSpot(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    bool process(QStringList &string) override;

private slots:
    void onSpot(const QString &callsign,
                const QString &mode,
                quint64 freq,
                const QColor &color,
                const QString &text);

private:
    TciCommandSpot() = delete;
    Q_DISABLE_COPY(TciCommandSpot)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
};

class TciCommandSpotDelete : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandSpotDelete(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    bool process(QStringList &string) override;

private slots:
    void onSpot(const QString &callsign);

private:
    TciCommandSpotDelete() = delete;
    Q_DISABLE_COPY(TciCommandSpotDelete)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
};

class TciCommandRitEnable : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandRitEnable(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandRitEnable();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandRitEnable() = delete;
    Q_DISABLE_COPY(TciCommandRitEnable)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandXitEnable : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandXitEnable(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandXitEnable();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandXitEnable() = delete;
    Q_DISABLE_COPY(TciCommandXitEnable)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandSplitEnable : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandSplitEnable(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandSplitEnable();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandSplitEnable() = delete;
    Q_DISABLE_COPY(TciCommandSplitEnable)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandRitOffset : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandRitOffset(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandRitOffset();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandRitOffset() = delete;
    Q_DISABLE_COPY(TciCommandRitOffset)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandXitOffset : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandXitOffset(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandXitOffset();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandXitOffset() = delete;
    Q_DISABLE_COPY(TciCommandXitOffset)

    void currentStatus(int trx = -1);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandRxChannelEnable : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandRxChannelEnable(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandRxChannelEnable();
    bool process(QStringList &string) override;
    void status() override;

private:
    TciCommandRxChannelEnable() = delete;
    Q_DISABLE_COPY(TciCommandRxChannelEnable)

    void currentStatus(int trx = -1, int channel = -1);
    QString command(int trx, int channel);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c;
};

class TciCommandRxFilter final : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandRxFilter(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandRxFilter();
    bool process(QStringList &string) override;
    void status() override;

signals:
    void valueChanged(quint32 trx, int low, int high);

private:
    TciCommandRxFilter() = delete;
    Q_DISABLE_COPY(TciCommandRxFilter)

    void currentStatus(int trx = -1);
    QString command(int trx);

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QMetaObject::Connection m_c1;
};

class TciCommandCwMessage final : public TciParserCommandAbsract
{
    Q_OBJECT
public:
    TciCommandCwMessage(TciParserAbstract &parser, TciTrxState &state, QObject *parent = nullptr);
    ~TciCommandCwMessage();
    bool process(QStringList &list) override;

private:
    TciCommandCwMessage() = delete;
    Q_DISABLE_COPY(TciCommandCwMessage)

private:
    TciParserAbstract &m_parser;
    TciTrxState       &m_control;
    QRegExp            m_regExpCallsign;
    QMetaObject::Connection m_c1;
    QMetaObject::Connection m_c2;
};


}  // namespace ExpertElectronics

#endif // TCICOMMAND_H



















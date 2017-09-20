// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TCITRXSTATE_H
#define TCITRXSTATE_H

#include <QtCore>
#include <QColor>
#include <atomic>

namespace ExpertElectronics {

class TciTrxState : public QObject
{
    Q_OBJECT

public:
    explicit TciTrxState(QObject *parent = nullptr);

    void setTrxCount(quint32 value);

    quint32 trxCount() const;

    void setChannelsCount(quint32 value);

    quint32 channelsCount() const;

    void setVfoLimits(quint64 min, quint64 max);

    qint64 vfoMin() const;

    qint64 vfoMax() const;

    void setIfLimits(int min, int max);

    qint64 ifMin() const;

    qint64 ifMax() const;

    void setModulationsList(const QStringList &list);

    QStringList modulationsList() const;

    void setIqOutSampleRate(quint32 value);

    quint32 iqOutSampleRate() const;

    void setAudioStart(quint32 trx, bool state);

    bool audioStart(quint32 trx) const;

    void setIqStart(quint32 trx, bool state);

    bool iqStart(quint32 trx) const;

    quint32 cwSpeed() const noexcept;

    quint32 cwMacrosDelay() const noexcept;

    bool ritEnable(quint32 trx) const;

    bool xitEnable(quint32 trx) const;

    bool splitEnable(quint32 trx) const;

    int ritOffset(quint32 trx) const;

    int xitOffset(quint32 trx) const;

    bool channelEnable(quint32 trx, quint32 channel) const;

    bool rxFilter(quint32 trx, int &low, int &high);

public slots:
    void setRxEnable(quint32 trx, bool state);

    bool rxEnable(quint32 trx) const;

    void setTxEnable(quint32 trx, bool state);

    bool txEnable(quint32 trx) const;

    void start();

    void stop();

    bool isStart() const;

    void setPreamp(int value);

    int preamp() const;

    void setDdsFreq(quint32 trx, double freq);

    double ddsFreq(quint32 trx);

    void setIfFreq(quint32 trx, quint32 channel, double freq);

    double ifFreq(quint32 trx, quint32 channel);

    void setTrx(quint32 trx, bool state);

    bool trx(quint32 trx);

    void setModulation(quint32 trx, const QString &mode);

    QString modulation(quint32 trx);

    void setMacros(quint32 trx, const QString &text);

    void setCwMacrosStop();

    void setCwSpeed(quint32 value);

    void setCwMacrosDelay(quint32 value);

    void setSpot(const QString &callsign,
                 const QString &mode,
                 quint64 freq,
                 const QColor &color,
                 const QString &text);

    void setDeleteSpot(const QString &callsign);

    void setRitEnable(quint32 trx, bool state);

    void setXitEnable(quint32 trx, bool state);

    void setSplitEnable(quint32 trx, bool state);

    void setRitOffset(quint32 trx, int value);

    void setXitOffset(quint32 trx, int value);

    void setChannelEnable(quint32 trx, quint32 channel, bool state);

    void setRxFilter(quint32 trx, int low, int high);

    void setCwMessage(quint32 trx, const QString &before, const QString &callsign, const QString &after);

    void addCwMessageCallsign(const QString &callsign);

signals:
    void started();
    void stoped();

    void preampChanged(int value);

    void ddsFreqChanged(quint32 trx, double value);

    void ifFreqChanged(quint32 trx, quint32 channel, double value);

    void trxChanged(quint32 trx, bool state);

    void vfoLimitsChanged(quint64 min, quint64 max);

    void ifLimitsChanged(int min, int max);

    void modulationsListChanged(QStringList list);

    void modulationChanged(quint32 trx, QString mode);

    void iqOutSampleRateChanged(quint32 value);

    void iqStartChanged(quint32 trx, bool state);

    void audioStartChanged(quint32 trx, bool state);

    void rxEnableChanged(quint32 trx, bool state);

    void txEnableChanged(quint32 trx, bool state);

    void cwMacros(quint32 trx, QString text);

    void cwMakrosStop();

    void cwSpeedChanged(quint32 value);

    void cwMacrosDelayChanged(quint32 value);

    void spot(QString callsign, QString mode, quint64 freq, QColor color, QString text);
    void deleteSpot(QString callsign);

    void ritEnableChanged(quint32 trx, bool state);

    void xitEnableChanged(quint32 trx, bool state);

    void splitEnableChanged(quint32 trx, bool state);

    void ritOffsetChanged(quint32 trx, int value);

    void xitOffsetChanged(quint32 trx, int value);

    void channelEnableChanged(quint32 trx, quint32 channel, bool state);

    void rxFilterChanged(quint32 trx, int low, int high);

    void cwMessage(quint32 trx, QString before, QString callsign, QString after);

    void cwMessageCallsign(const QString &callsign);

private:
    Q_DISABLE_COPY(TciTrxState)

private:
    quint32 m_trxCount;
    quint32 m_channelsCount;
    quint64 m_vfoMin;
    quint64 m_vfoMax;
    int     m_ifMin;
    int     m_ifMax;
    quint32 m_iqOutSampleRate;
    quint32 m_cwSpeed;
    quint32 m_cwDelay;
    QJsonObject m_state;
    QStringList m_mudulations;
};

}  // namespace ExpertElectronics

#endif // TCITRXSTATE_H

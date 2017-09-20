// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef TCICLIENT_H
#define TCICLIENT_H

#include <QtCore>
#include <QtWebSockets>

#include "TciTrxState/TciTrxState.h"
#include "TciParser/TciParser.h"
#include "TciParser/TciCommand.h"

typedef float REAL;

typedef struct
{
    float re;
    float im;
}COMPLEX;

namespace ExpertElectronics {

class TciClient : public QObject
{
    Q_OBJECT

    static constexpr quint32 IqHeaderSize = 16u*sizeof(quint32);
    static constexpr quint32 IqBufferSize = 2048u;


    typedef enum
    {
        IqInt16 = 0,
        IqInt24,
        IqInt34,
        IqFloat32,
        IqFloat64
    }IqDataType;

    typedef enum
    {
        IqStream = 0,
        RxAudioStream,
        TxAudioStream,
        TxChrono,
    }StreamType;

    typedef struct
    {
        quint32 receiver;
        quint32 sampleRate;
        quint32 format;
        quint32 codec;
        quint32 crc;
        quint32 length;
        quint32 type;
        quint32 reserv[9];
        float   data[4096];
    }DataStream;

public:
    explicit TciClient(QObject *parent = nullptr);
    ~TciClient();

    void open(const QUrl &url);
    void close();
    bool isOpen() const;

    TciTrxState& trxState();

    QVector<COMPLEX> iqData();

signals:
    void openStatusChanged(bool);
    void message(QString);
    void readyReadIq();

private slots:
    void onConnected();
    void onClosed();

    void onSendMessage(const QString &message);

    void onBinaryReceived(const QByteArray &data);

private:
    bool       m_open;
    QWebSocket m_webSocket;

    TciTrxState m_trxState;
    TciParser   m_parser;

    QByteArray t_iqData;
    QVector<COMPLEX> m_signal;
};

}  // namespace ExpertElectronics

#endif // TCICLIENT_H

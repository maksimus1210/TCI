// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "tciclient.h"

namespace ExpertElectronics {

TciClient::TciClient(QObject *parent) :
  QObject(parent),
  m_open(false)
{
    t_iqData.reserve(sizeof(IqStream));
    m_signalIQ.reserve(IqBufferSize);
    m_signalAudio.reserve(IqBufferSize);

    m_parser.attach(new TciCommandStart(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandStop(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandDds(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIf(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandVfo(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandTrx(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandVfoLimits(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIfLimits(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandModulationsList(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandModulation(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIqSampleRate(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandAudioSampleRate(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIqStart(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIqStop( m_parser, m_trxState, this));
    m_parser.attach(new TciCommandRxEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandTxEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandCwMacros(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandCwMacrosStop(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandCwSpeed(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandCwMacrosDelay(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSpot(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSpotDelete(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandRitEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandXitEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSplitEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandRitOffset(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandXitOffset(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandRxChannelEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandRxFilter(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandCwMessage(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandAudioStart(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandAudioStop(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSetInFocus(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSpotClear(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandVolume(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSqlEnable(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandSqlLevel(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandTune(m_parser, m_trxState, this));

    connect(&m_webSocket, &QWebSocket::connected   , this, &TciClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &TciClient::onClosed);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, [=](const QString &text) {
        emit message(text);
        m_parser.processString(text);
    });

    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &TciClient::onBinaryReceived);
    connect(&m_parser, &TciParser::message, this, &TciClient::onSendMessage);
}
TciClient::~TciClient()
{
    close();
}

void TciClient::open(const QUrl &url)
{
    if (m_open)
        return;

    m_webSocket.open(url);
}

void TciClient::close()
{
    if (!m_open)
        return;

    m_webSocket.close();
}

bool TciClient::isOpen() const
{
    return m_open;
}

TciTrxState& TciClient::trxState()
{
    return m_trxState;
}

QVector<COMPLEX> TciClient::iqData() const
{
    return m_signalIQ;
}

QVector<COMPLEX> TciClient::audioData() const
{
    return m_signalAudio;
}

void TciClient::sendText(const QString &text)
{
    onSendMessage(text);
}

bool TciClient::sendTxAudio(const QVector<COMPLEX> &src)
{
    t_txAudioData.resize(IqHeaderSize + 2*src.size()*sizeof (float));
    DataStream *pStream = reinterpret_cast<DataStream*>(t_txAudioData.data());

    pStream->receiver   = 0;
    pStream->sampleRate = 48000;
    pStream->format     = IqFloat32;
    pStream->codec      = 0u;
    pStream->crc        = 0u;
    pStream->type       = TxAudioStream;
    pStream->length     = 2*src.size();

    for (quint32 i = 0u, j = 0u; i < pStream->length; ++j) {
        pStream->data[i++] = src[j].re;
        pStream->data[i++] = src[j].im;
    }

    return m_webSocket.sendBinaryMessage(t_txAudioData) == t_txAudioData.size();
}

void TciClient::onConnected()
{
    m_open = true;
    emit openStatusChanged(m_open);
}

void TciClient::onClosed()
{
    m_open = false;
    emit openStatusChanged(m_open);
}

void TciClient::onSendMessage(const QString &message)
{
    m_webSocket.sendTextMessage(message);
}

void TciClient::onBinaryReceived(const QByteArray &data)
{
    t_iqData = data;
    DataStream *pStream = reinterpret_cast<DataStream*>(t_iqData.data());

    if (pStream->receiver != 0u)
        return;

    if (pStream->type == IqStream) {
        m_signalIQ.resize(pStream->length/2u);
        for (quint32 i = 0u, j = 0u; i < pStream->length; ++j) {
            m_signalIQ[j].re = pStream->data[i++];
            m_signalIQ[j].im = pStream->data[i++];
        }
        emit readyReadIq();
    }
    else if (pStream->type == RxAudioStream) {
        m_signalAudio.resize(pStream->length/2u);
        for (quint32 i = 0u, j = 0u; i < pStream->length; ++j) {
            m_signalAudio[j].re = pStream->data[i++];
            m_signalAudio[j].im = pStream->data[i++];
        }
        emit readyReadAudio();
    }
    else if (pStream->type == TxChrono) {
        qDebug() << Q_FUNC_INFO << __LINE__ << m_txTimer.elapsed();
        m_txTimer.start();
        emit chronoTxSignal(pStream->length);
    }
}




}  // namespace ExpertElectronics


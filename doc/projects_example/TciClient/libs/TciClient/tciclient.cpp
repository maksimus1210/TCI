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
    m_signal.reserve(IqBufferSize);

    m_parser.attach(new TciCommandStart( m_parser, m_trxState, this));
    m_parser.attach(new TciCommandStop(  m_parser, m_trxState, this));
    m_parser.attach(new TciCommandDds(   m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIf(    m_parser, m_trxState, this));
    m_parser.attach(new TciCommandTrx(   m_parser, m_trxState, this));
    m_parser.attach(new TciCommandVfoLimits(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIfLimits(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandModulationsList(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandModulation(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIqSampleRate(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIqStart(m_parser, m_trxState, this));
    m_parser.attach(new TciCommandIqStop( m_parser, m_trxState, this));
    m_parser.attach(new TciCommandRxEnable( m_parser, m_trxState, this));
    m_parser.attach(new TciCommandTxEnable( m_parser, m_trxState, this));
    m_parser.attach(new TciCommandCwMacros( m_parser, m_trxState, this));
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
    m_parser.attach(new TciCommandAudioStop( m_parser, m_trxState, this));

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

QVector<COMPLEX> TciClient::iqData()
{
    return m_signal;
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
        m_signal.resize(pStream->length/2u);
        for (quint32 i = 0u, j = 0u; i < pStream->length; ++j) {
            m_signal[j].re = pStream->data[i++];
            m_signal[j].im = pStream->data[i++];
        }
        emit readyReadIq();
    }
}




}  // namespace ExpertElectronics
















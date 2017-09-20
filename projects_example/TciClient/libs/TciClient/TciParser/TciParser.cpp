// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "TciParser.h"

namespace ExpertElectronics {

TciParser::TciParser(QObject *parent) :
  TciParserAbstract(parent)
{

}

void TciParser::processString(const QString &string)
{
    if (string.isEmpty())
        return;

    QStringList t_list = string.split(";", QString::SkipEmptyParts);
    for (auto p : m_list) {
        if (p->process(t_list)) {
            if (t_list.isEmpty())
                break;
        }
    }
}

void TciParser::attach(TciParserCommandAbsract *p)
{
    if (m_list.contains(p) || (p == nullptr))
        return;

    m_list.append(p);
    connect(p, &TciParserCommandAbsract::message, this, &TciParser::message);
}

void TciParser::sendStatus()
{
    for (auto p : m_list)
        p->status();
}

}  // namespace ExpertElectronics

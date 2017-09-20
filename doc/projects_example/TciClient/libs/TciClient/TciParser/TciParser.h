// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TCIPARSER_H
#define TCIPARSER_H

#include <QtCore>
#include "TciParserAbstract.h"

namespace ExpertElectronics {

class TciParser : public TciParserAbstract
{
    Q_OBJECT

public:
    explicit TciParser(QObject *parent = nullptr);

    void processString(const QString &string) override;

    void attach(TciParserCommandAbsract *p) override;

    void sendStatus();

private:
    Q_DISABLE_COPY(TciParser)

private:
    QList<TciParserCommandAbsract*> m_list;
};

}  // namespace ExpertElectronics

#endif // TCIPARSER_H

// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TCIPARSERABSTRACT_H
#define TCIPARSERABSTRACT_H

#include <QtCore>
#include "TciParserCommandAbsract.h"

namespace ExpertElectronics {

class TciParserAbstract : public QObject
{
    Q_OBJECT

public:
    explicit TciParserAbstract(QObject *parent = nullptr) : QObject(parent) {}

    virtual ~TciParserAbstract() {}

    virtual void processString(const QString &string) = 0;

    virtual void attach(TciParserCommandAbsract *p) = 0;

signals:
    void message(QString string);

private:
    Q_DISABLE_COPY(TciParserAbstract)
};

}  // namespace ExpertElectronics

#endif // TCIPARSERABSTRACT_H

// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TCIPARSERCOMMANDABSRACT_H
#define TCIPARSERCOMMANDABSRACT_H

#include <QtCore>

namespace ExpertElectronics {

class TciParserCommandAbsract : public QObject
{
    Q_OBJECT

public:
    TciParserCommandAbsract(QObject *parent = nullptr) : QObject(parent) {}

    virtual ~TciParserCommandAbsract() {}

    virtual bool process(QStringList &string) = 0;

    virtual void status() {};

signals:
    void message(QString string);

private:
    Q_DISABLE_COPY(TciParserCommandAbsract)
};

}  // namespace ExpertElectronics

#endif // TCIPARSERCOMMANDABSRACT_H

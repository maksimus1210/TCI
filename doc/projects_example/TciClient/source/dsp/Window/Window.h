// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WINDOW_H
#define WINDOW_H

#include <math.h>
#include <QtCore>

#include "../dsp.h"

class Window
{
public:
    Window();

    void resize(int len);
    int size();

    void process(COMPLEX *pSrcDst, int len);

private:
    void calcWindow();

private:
    QVector<REAL> m_data;
};

#endif // WINDOW_H

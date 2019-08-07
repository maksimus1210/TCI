// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QThread>
#include <QMutex>
#include <math.h>

#include "../Window/Window.h"


class Spectrum
{
public:
    static bool fft(COMPLEX *pSrcDst, quint32 len, bool directFFT = true);
    static bool magnitude(COMPLEX *pSrc, REAL *pDst, quint32 len);
};

#endif // SPECTRUM_H

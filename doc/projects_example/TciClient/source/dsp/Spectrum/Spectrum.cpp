// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Spectrum.h"

#define IS_TWO_POW(x) ((!((x)&((x)-1)))&&((x)>1))

bool Spectrum::fft(COMPLEX *pSrcDst, quint32 len, bool directFFT)
{
    if ((pSrcDst == nullptr) || (len > 16384) || (len < 1) || !IS_TWO_POW(len))
        return false;

    int  i, j, n, k, io, ie, in, nn;
    REAL ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

    static const REAL Rcoef[14] =
    {  -1.0000000000000000,  0.0000000000000000,  0.7071067811865475,
        0.9238795325112867,  0.9807852804032304,  0.9951847266721969,
        0.9987954562051724,  0.9996988186962042,  0.9999247018391445,
        0.9999811752826011,  0.9999952938095761,  0.9999988234517018,
        0.9999997058628822,  0.9999999264657178
    };
    static const REAL Icoef[14] =
    {    0.0000000000000000, -1.0000000000000000, -0.7071067811865474,
        -0.3826834323650897, -0.1950903220161282, -0.0980171403295606,
        -0.0490676743274180, -0.0245412285229122, -0.0122715382857199,
        -0.0061358846491544, -0.0030679567629659, -0.0015339801862847,
        -0.0007669903187427, -0.0003834951875714
    };

    nn = len >> 1;
    ie = len;
    int LogN = qRound(log10((double)len)/log10(2.0));

    for (n = 1; n <= LogN; ++n) {
        rw = Rcoef[LogN - n];
        iw = Icoef[LogN - n];

        if (!directFFT)
            iw = -iw;

        in = ie >> 1;
        ru = 1.0F;
        iu = 0.0F;

        for (j = 0; j < in; ++j) {
            for (i = j; i < len; i+=ie) {
                io       = i + in;
                rtp      = pSrcDst[i].re + pSrcDst[io].re;
                itp      = pSrcDst[i].im + pSrcDst[io].im;
                rtq      = pSrcDst[i].re - pSrcDst[io].re;
                itq      = pSrcDst[i].im - pSrcDst[io].im;
                pSrcDst[io].re = rtq*ru - itq*iu;
                pSrcDst[io].im = itq*ru + rtq*iu;
                pSrcDst[i].re  = rtp;
                pSrcDst[i].im  = itp;
            }

            sr = ru;
            ru = ru*rw - iu*iw;
            iu = iu*rw + sr*iw;
        }

        ie >>= 1;
    }

    for (j = i = 1; i < len; ++i) {
        if (i < j) {
            io       = i - 1;
            in       = j - 1;
            rtp      = pSrcDst[in].re;
            itp      = pSrcDst[in].im;
            pSrcDst[in].re = pSrcDst[io].re;
            pSrcDst[in].im = pSrcDst[io].im;
            pSrcDst[io].re = rtp;
            pSrcDst[io].im = itp;
        }

        k = nn;

        while (k < j) {
            j   = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    if (!directFFT)
        return true;

    rw = 1.0/len;

    for (i = 0; i < len; ++i) {
        pSrcDst[i].re *= rw;
        pSrcDst[i].im *= rw;
    }

    return true;
}

bool Spectrum::magnitude(COMPLEX *pSrc, REAL *pDst, quint32 len)
{
    if ((pSrc == nullptr) || (pDst == nullptr) || (len == 0u))
        return false;

    for (int i = 0u; i < len; ++i)
        pDst[i] = 10.0f*log10(pSrc[i].re*pSrc[i].re + pSrc[i].im*pSrc[i].im);

    return true;
}

























// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Window.h"

#define FROM_DB(X) (pow(10, X/20.0))
#define TWO_PI   ( 6.28318530717958647692 )

Window::Window()
{
    m_data.resize(1024);
    calcWindow();
}

void Window::resize(int len)
{
    m_data.resize(len);
    calcWindow();
}
int Window::size()
{
    return m_data.size();
}

void Window::process(COMPLEX *pSrcDst, int len)
{
    if (m_data.size() != len || pSrcDst == nullptr)
        return;

    for (int i = 0; i < len; ++i) {
        pSrcDst[i].re *= m_data[i];
        pSrcDst[i].im *= m_data[i];
    }
}

void Window::calcWindow()
{
    // WINDOW BLACKMAN-NUTTALL
    double sn = m_data.size() - 1.0;
    double gain = FROM_DB(-8.8);
    for(int j = 0; j < m_data.size(); ++j)
        m_data[j] = (0.3635819 - 0.4891775*cos(TWO_PI*j/sn) + 0.1365995*cos(2.0*TWO_PI*j/sn) - 0.0106411*cos(3.0*TWO_PI*j/sn))/gain;
}

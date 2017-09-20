// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPECTRUMRINGBUFFER_H
#define SPECTRUMRINGBUFFER_H

#include <atomic>
#include <QtCore>
#include <QtGlobal>

namespace ExpertElectronics {

using namespace std;

template <typename T>
class SpectrumRingBuffer
{
public:
    explicit SpectrumRingBuffer(quint32 t_size = 0);

    void resize(quint32 t_size);
    quint32 size() const noexcept;

    void clear() noexcept;

    void fill(T value);

    bool readyRead() const noexcept;

    bool readyWrite() const noexcept;

    bool readAll(QVector<T> &dst) noexcept;
    bool readAll(T *pDst, quint32 len) noexcept;

    bool write(const QVector<T> &src) noexcept;
    bool write(const T *pSrc, quint32 len) noexcept;

private:
    SpectrumRingBuffer(const SpectrumRingBuffer &) = delete;
    SpectrumRingBuffer &operator=(const SpectrumRingBuffer &) = delete;

    inline int pow2Next(int d)
    {
        --d;
        d |= d >> 1;
        d |= d >> 2;
        d |= d >> 4;
        d |= d >> 8;
        d |= d >> 16;
        ++d;
        return d;
    }

private:
    quint32 m_size;
    quint32 m_readPtr;
    quint32 m_writePtr;
    quint32 m_mask;

    atomic_bool      m_readyRead;
    atomic<quint32> m_bytesForRead;
    atomic<quint32> m_bytesForWrite;

    QVector<T> m_buffer;
    QMutex     m_mutex;
};


template <typename T>
SpectrumRingBuffer<T>::SpectrumRingBuffer(quint32 t_size) :
  m_size(t_size),
  m_readPtr(0),
  m_writePtr(0),
  m_mask(0),
  m_readyRead(false),
  m_bytesForRead(0),
  m_bytesForWrite(0)
{
    m_buffer.resize(pow2Next(t_size));

    m_size = m_buffer.size();
    m_mask = m_size - 1;
    m_bytesForWrite = m_size;

}

template <typename T>
void SpectrumRingBuffer<T>::resize(quint32 t_size)
{
    if (m_size == t_size)
        return;

    m_mutex.lock();
        m_bytesForRead  = 0;
        m_bytesForWrite = 0;

        m_buffer.resize(pow2Next(t_size));

        m_size = m_buffer.size();
        m_mask = m_size - 1;
        m_bytesForWrite = m_size;
    m_mutex.unlock();
}

template <typename T>
quint32 SpectrumRingBuffer<T>::size() const noexcept
{
    return m_size;
}

template <typename T>
void SpectrumRingBuffer<T>::clear() noexcept
{
    m_mutex.lock();
        m_bytesForRead  = 0;
        m_bytesForWrite = 0;
        m_readPtr       = 0;
        m_writePtr      = 0;
        m_readyRead     = false;
        m_bytesForWrite = m_size;
    m_mutex.unlock();
}

template <typename T>
void SpectrumRingBuffer<T>::fill(T value)
{
    m_mutex.lock();
        m_buffer.fill(value);
    m_mutex.unlock();
}

template <typename T>
bool SpectrumRingBuffer<T>::readyRead() const noexcept
{
    return m_readyRead;
}

template <typename T>
bool SpectrumRingBuffer<T>::readyWrite() const noexcept
{
    return m_size != 0u;
}

template <typename T>
bool SpectrumRingBuffer<T>::readAll(QVector<T> & dst) noexcept
{
    if ((static_cast<quint32>(dst.size()) != m_size) || !m_readyRead)
        return false;

    m_mutex.lock();
        m_readyRead = false;
        m_readPtr   = m_writePtr;

        for (T &data : dst)
            data = m_buffer[m_readPtr++&m_mask];
    m_mutex.unlock();

    return true;
}

template <typename T>
bool SpectrumRingBuffer<T>::readAll(T *pDst, quint32 len) noexcept
{
    if ((len != m_size) || !m_readyRead)
        return false;

    m_mutex.lock();
        m_readyRead = false;
        m_readPtr   = m_writePtr;

        for (quint32 i = 0; i < len; ++i)
            pDst[i] = m_buffer[m_readPtr++&m_mask];
    m_mutex.unlock();

    return true;
}

template <typename T>
bool SpectrumRingBuffer<T>::write(const QVector<T> & src) noexcept
{
    if ((src.size() == 0) && (m_size == 0u))
        return false;

    m_mutex.lock();
        for (T &data : const_cast<QVector<T> &>(src))
            m_buffer[m_writePtr++&m_mask] = data;
        m_writePtr &= m_mask;

        m_bytesForRead = qMin<quint32>(m_bytesForRead + src.size(), m_size);
        m_readyRead    = m_bytesForRead == m_size;
    m_mutex.unlock();

    return true;
}

template <typename T>
bool SpectrumRingBuffer<T>::write(const T *pSrc, quint32 len) noexcept
{
    if ((len == 0u) && (m_size == 0u))
        return false;

    m_mutex.lock();
        for (quint32 i = 0; i < len; ++i)
            m_buffer[m_writePtr++&m_mask] = pSrc[i];
        m_writePtr &= m_mask;

        m_bytesForRead = qMin<quint32>(m_bytesForRead + len, m_size);
        m_readyRead    = m_bytesForRead == m_size;
    m_mutex.unlock();

    return true;
}

}

#endif // SPECTRUMRINGBUFFER_H

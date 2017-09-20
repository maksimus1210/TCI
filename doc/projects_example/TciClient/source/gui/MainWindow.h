// Copyright (c) 2017 Expert Electronics
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <TciClient>
#include <QCustomPlot>

#include "../dsp/spectrumringbuffer.h"
#include "../dsp/Window/Window.h"
#include "../dsp/Spectrum/Spectrum.h"
#include "ui_MainWindow.h"

using namespace ExpertElectronics;

class MainWindow : public QMainWindow, protected Ui::MainWindow
{
    Q_OBJECT

    static constexpr quint32 SpectrumSize = 4096;

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onConnect(bool state);
    void onConnectStatus(bool state);

    void onReadIq();
    void onUpdateSpectrum();

private:
    int indefOf(QComboBox *p, const QString &text);

private:
    QCustomPlot *pPlotter;
    TciClient m_tciClient;

    QMetaObject::Connection m_c1;
    QMetaObject::Connection m_c2;

    QTimer m_timer;

    SpectrumRingBuffer<COMPLEX> m_spectrumBuffer;
    Window m_window;
    QVector<COMPLEX> m_signal;
    QVector<REAL>    m_magnitude;
};

#endif // MAINWINDOW_H

/******************************************************************************
 * SpeechMarker:  Qt-based GUI application for marking wav files
 * Copyright (C) 2015-2018 Pavel Paramonov <pa.pawka@gmail.com>

 * SpeechMarker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * SpeechMarker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with SpeechMarker; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QVector>
#include <QProgressBar>

#include <QtMultimedia/QAudio>
#include <QByteArray>
#include <QBuffer>

#include "workerwavfilereader.h"

namespace Ui {
    class MainWindow;
}

class RenderArea;
class QAudioOutput;

#define defaultLabel "??"
#define zoomCoeff 1.5


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = 0);
    ~MainWindow();
public slots:
    void pBtnLoadWavClicked();
    void pBtnSaveMarkersClicked();
    void pBtnLoadMarkersClicked();
    void pBtnPlaceMarkClicked();
    void pBtnRemoveMarkClicked();
    void pBtnZoomInClicked();
    void pBtnZoomOutClicked();
    void pBtnPlayClicked();
    void pBtnStopClicked();
    void cBxMarkTypeCurrentIndexChanged(const QString & text);
    void cBxIntervalsCurrentIndexChanged(int index);
    void edMarkerPositionTextEdited(const QString &newText);
    void graphAreaMarkerPositionChanged(int newPosition);
    void sBarPlotScrollerValueChanged(int value);
    void prBarOpenWavProgressValueChanged(int value);
    void prBarOpenWavProgressMaxValueChanged(int value);
    void processWavReaderResult(wavReaderErrCode errCode, QString wavFileName);
    void drawPrecalculatedArray();
    void AudioOutputStateChanged(QAudio::State newState);
    void audioNotifyProcess();
    void startPlayback(int byteOffset);

private:
    Ui::MainWindow *ui;
    // GIU members:
    RenderArea  *graphArea;
    QPushButton *pBtnLoadWav, *pBtnSaveMarkers, *pBtnLoadMarkers, *pBtnPlaceMark, *pBtnZoomIn, *pBtnZoomOut, *pBtnRemoveMark, *pBtnPlay, *pBtnStop;
    QComboBox   *cBxMarkType, *cBxWindowSize, *cBxIntervals;
    QLabel      *lbCurrentWavFile, *lbWavFileSamplRate, *lbWavFileBitsPerSample, *lbMarkerPosition, *lbSamplesInWav;
    QLineEdit   *edCurrentWavFile, *edWavFileSamplRate, *edWavFileBitsPerSample, *edMarkerPosition, *edSamplesInWav;
    QVBoxLayout *vBoxLayMarksSettings, *vBoxLayRenderControl;
    QHBoxLayout *hBoxLayControlButtons, *hBoxLayMarkerPosition, *hBoxLayWavFileLabel, *hBoxLayMain, *hBoxPlayStop;
    QScrollBar  *sBarPlotScroller;
    QProgressBar *prBarOpenWavProgress;
    // Other members:
    QVector<int>    vectSamples, vectMarks;
    QVector<QString> vectLabels;
    int markerPosition;
    int visibleSamplesCnt;
    wavHeader wavFileHeader;
    QAudioOutput* audio;
    QByteArray byteArrRawWav;
    QBuffer wavDataToPlay;
    bool *previousEnabledState;
    // Private functions:
};

#endif // MAINWINDOW_H

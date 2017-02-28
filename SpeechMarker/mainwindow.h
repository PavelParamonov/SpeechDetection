#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
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

class RenderArea;
class QAudioOutput;

#define defaultLabel "??"
#define zoomCoeff 1.5


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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

private:
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

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

class RenderArea;

#define defaultLabel "??"
#define zoomCoeff 1.5

struct wavHeader {
    char chunkID[4];
    quint32 chunkSize;
    char format[4];
    char subchunk1ID[4];
    quint32 subchunk1Size;
    quint16 audioFormat;
    quint16 numChannels;
    quint32 sampleRate;
    quint32 byteRate;
    quint16 blockAlign;
    quint16 bitsPerSample;
    char subchunk2ID[4];
    quint32 subchunk2Size;
};

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
    void cBxMarkTypeCurrentIndexChanged(const QString & text);
    void cBxIntervalsCurrentIndexChanged(int index);
    void edMarkerPositionTextEdited(const QString &newText);
    void graphAreaMarkerPositionChanged(int newPosition);
    void sBarPlotScrollerValueChanged(int value);
    void prBarOpenWavProgressValueChanged(int value);

private:
    // GIU members:
    RenderArea  *graphArea;
    QPushButton *pBtnLoadWav, *pBtnSaveMarkers, *pBtnLoadMarkers, *pBtnPlaceMark, *pBtnZoomIn, *pBtnZoomOut, *pBtnRemoveMark;
    QComboBox   *cBxMarkType, *cBxWindowSize, *cBxIntervals;
    QLabel      *lbCurrentWavFile, *lbWavFileSamplRate, *lbWavFileBitsPerSample, *lbMarkerPosition, *lbSamplesInWav;
    QLineEdit   *edCurrentWavFile, *edWavFileSamplRate, *edWavFileBitsPerSample, *edMarkerPosition, *edSamplesInWav;
    QVBoxLayout *vBoxLayMarksSettings, *vBoxLayRenderControl;
    QHBoxLayout *hBoxLayControlButtons, *hBoxLayMarkerPosition, *hBoxLayWavFileLabel, *hBoxLayMain;
    QScrollBar  *sBarPlotScroller;
    // Other members:
    QVector<int>    vectSamples, vectMarks;
    QVector<QString> vectLabels;
    int markerPosition;
    int visibleSamplesCnt;
    wavHeader wavFileHeader;
    QProgressBar *prBarOpenWavProgress;
};

#endif // MAINWINDOW_H

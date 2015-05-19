#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

class RenderArea;

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

private:
    // GIU members:
    RenderArea  *graphArea;
    QPushButton *pBtnLoadWav, *pBtnSaveMarkers, *pBtnLoadMarkers, *pBtnPlaceMark, *pBtnZoomIn, *pBtnZoomOut;
    QComboBox   *cBxMarkType, *cBxWindowSize;
    QLabel      *lbCurrentWavFile, *lbWavFileSamplRate, *lbWavFileBitsPerSample;
    QLineEdit   *edCurrentWavFile, *edWavFileSamplRate, *edWavFileBitsPerSample;
    QVBoxLayout *vBoxLayMarksSettings, *vBoxLayRenderControl;
    QHBoxLayout *hBoxLayControlButtons, *hBoxLayWavFileLabel, *hBoxLayMain;
    // Other members:
    //QVector     *vectSamples, *vectMarks;
};

#endif // MAINWINDOW_H

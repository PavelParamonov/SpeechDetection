#include "mainwindow.h"
#include "renderarea.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    graphArea = new RenderArea;
    pBtnLoadWav = new QPushButton("Load Wav File");
    pBtnSaveMarkers = new QPushButton("Save markers");
    pBtnLoadMarkers = new QPushButton("Load markers");
    pBtnPlaceMark = new QPushButton("Place mark");
    pBtnZoomIn = new QPushButton("Zoom in");
    pBtnZoomOut = new QPushButton("Zoom Out");
    lbCurrentWavFile = new QLabel("Wav File:");
    edCurrentWavFile = new QLineEdit();
    cBxMarkType = new QComboBox();
    cBxMarkType->addItem("Silence");
    cBxMarkType->addItem("Speech");
    cBxWindowSize = new QComboBox();
    cBxWindowSize->addItem("1 sample");
    cBxWindowSize->addItem("10 ms");
    cBxWindowSize->addItem("16 ms");
    cBxWindowSize->addItem("20 ms");

    hBoxLayControlButtons = new QHBoxLayout();
    hBoxLayControlButtons->addWidget(pBtnLoadWav);
    hBoxLayControlButtons->addWidget(pBtnLoadMarkers);
    hBoxLayControlButtons->addWidget(pBtnSaveMarkers);

    vBoxLayMarksSettings = new QVBoxLayout();
    vBoxLayMarksSettings->addWidget(pBtnZoomIn);
    vBoxLayMarksSettings->addWidget(pBtnZoomOut);
    vBoxLayMarksSettings->addWidget(cBxMarkType);
    vBoxLayMarksSettings->addWidget(cBxWindowSize);
    vBoxLayMarksSettings->addWidget(pBtnPlaceMark);

    hBoxLayWavFileLabel = new QHBoxLayout();
    hBoxLayWavFileLabel->addWidget(lbCurrentWavFile);
    hBoxLayWavFileLabel->addWidget(edCurrentWavFile);

    vBoxLayRenderControl = new QVBoxLayout();
    vBoxLayRenderControl->addWidget(graphArea);
    vBoxLayRenderControl->addLayout(hBoxLayControlButtons);
    vBoxLayRenderControl->addLayout(hBoxLayWavFileLabel);

    hBoxLayMain = new QHBoxLayout();
    hBoxLayMain->addLayout(vBoxLayRenderControl);
    hBoxLayMain->addLayout(vBoxLayMarksSettings);
    setLayout(hBoxLayMain);

}

void MainWindow::pBtnLoadWavClicked()
{
//    QString wavFileName("D:\My_Documents\Pasha_Docs\GitHub\SpeechDetection\SpeechMarker\example.wav");
//    QFile wavFile(wavFileName);
//    wavFile.open(QIODevice::ReadOnly);
//    wavHeader wavFileHeader;
//    QDataStream in(&wavFile);
//    in >> wavFileHeader;
//    wavFile.close();
//    edCurrentWavFile->setText(wavFileName);
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
}

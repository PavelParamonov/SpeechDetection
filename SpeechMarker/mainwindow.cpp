#include "mainwindow.h"
#include "renderarea.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    graphArea = new RenderArea(0);
    pBtnLoadWav = new QPushButton("Load Wav File");
    pBtnSaveMarkers = new QPushButton("Save markers");
    pBtnLoadMarkers = new QPushButton("Load markers");
    pBtnPlaceMark = new QPushButton("Place mark");
    pBtnZoomIn = new QPushButton("Zoom in");
    pBtnZoomOut = new QPushButton("Zoom Out");
    lbCurrentWavFile = new QLabel("Wav File:");
    lbWavFileSamplRate = new QLabel("Sampling Rate:");
    lbWavFileBitsPerSample = new QLabel("Bits per sample:");
    edCurrentWavFile = new QLineEdit();
    edWavFileSamplRate = new QLineEdit();
    edWavFileSamplRate->setMaximumWidth(80);
    edWavFileBitsPerSample = new QLineEdit();
    edWavFileBitsPerSample->setMaximumWidth(60);
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
    hBoxLayWavFileLabel->addWidget(lbWavFileSamplRate);
    hBoxLayWavFileLabel->addWidget(edWavFileSamplRate);
    hBoxLayWavFileLabel->addWidget(lbWavFileBitsPerSample);
    hBoxLayWavFileLabel->addWidget(edWavFileBitsPerSample);

    vBoxLayRenderControl = new QVBoxLayout();
    vBoxLayRenderControl->addWidget(graphArea);
    vBoxLayRenderControl->addLayout(hBoxLayControlButtons);
    vBoxLayRenderControl->addLayout(hBoxLayWavFileLabel);

    hBoxLayMain = new QHBoxLayout();
    hBoxLayMain->addLayout(vBoxLayRenderControl);
    hBoxLayMain->addLayout(vBoxLayMarksSettings);
    setLayout(hBoxLayMain);

    setWindowTitle("Speech Marker");

    connect(pBtnLoadWav, SIGNAL(clicked()), this, SLOT(pBtnLoadWavClicked()));

}

void MainWindow::pBtnLoadWavClicked()
{
//    For Windows:
    QString wavFileName("D:\\My_Documents\\Pasha_Docs\\GitHub\\SpeechDetection\\SpeechMarker\\example.wav");
//  For Linux:
    //    QString wavFileName("/home/pavel/dev/SpeechDetection/SpeechMarker/example.wav");
    QFile wavFile(wavFileName);
    if (!wavFile.exists()) {
        edCurrentWavFile->setText("File doesn't exist");
    }
    else {
      wavFile.open(QIODevice::ReadOnly);
      wavHeader wavFileHeader;
      QDataStream inFile(&wavFile);
      inFile.readRawData(wavFileHeader.chunkID, 4);
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.chunkSize), sizeof(wavFileHeader.chunkSize));
      inFile.readRawData(wavFileHeader.format, 4);
      inFile.readRawData(wavFileHeader.subchunk1ID, 4);
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.subchunk1Size), sizeof(wavFileHeader.subchunk1Size));
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.audioFormat), sizeof(wavFileHeader.audioFormat));
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.numChannels), sizeof(wavFileHeader.numChannels));
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.sampleRate), sizeof(wavFileHeader.sampleRate));
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.byteRate), sizeof(wavFileHeader.byteRate));
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.blockAlign), sizeof(wavFileHeader.blockAlign));
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.bitsPerSample), sizeof(wavFileHeader.bitsPerSample));
      inFile.readRawData(wavFileHeader.subchunk2ID, 4);
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.subchunk2Size), sizeof(wavFileHeader.subchunk2Size));
      wavFile.close();
      edCurrentWavFile->setText(wavFileName);
      edWavFileSamplRate->setText(QString::number(wavFileHeader.sampleRate));
      edWavFileBitsPerSample->setText(QString::number(wavFileHeader.bitsPerSample));
    }
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
}

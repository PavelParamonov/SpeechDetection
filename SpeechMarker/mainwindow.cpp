#include "mainwindow.h"
#include "renderarea.h"
#include <QFile>
#include <QtMath>

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
    lbMarkerPosition = new QLabel("Marker position: ");
    edMarkerPosition = new QLineEdit();
    lbSamplesInWav = new QLabel("Samples in file:");
    edSamplesInWav = new QLineEdit();
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

    hBoxLayMarkerPosition = new QHBoxLayout();
    hBoxLayMarkerPosition ->addWidget(lbMarkerPosition);
    hBoxLayMarkerPosition ->addWidget(edMarkerPosition);
    hBoxLayMarkerPosition ->addWidget(lbSamplesInWav);
    hBoxLayMarkerPosition ->addWidget(edSamplesInWav);

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
    vBoxLayRenderControl->addLayout(hBoxLayMarkerPosition);
    vBoxLayRenderControl->addLayout(hBoxLayControlButtons);
    vBoxLayRenderControl->addLayout(hBoxLayWavFileLabel);

    hBoxLayMain = new QHBoxLayout();
    hBoxLayMain->addLayout(vBoxLayRenderControl);
    hBoxLayMain->addLayout(vBoxLayMarksSettings);
    setLayout(hBoxLayMain);

    setWindowTitle("Speech Marker");

    connect(pBtnLoadWav, SIGNAL(clicked()), this, SLOT(pBtnLoadWavClicked()));
    connect(edMarkerPosition, SIGNAL(textChanged(QString)), this, SLOT(edMarkerPositionTextEdited(QString)));

}

void MainWindow::edMarkerPositionTextEdited(const QString &newText)
{
    bool ok = false;
    unsigned int uIntMarkerPosition = newText.toUInt(&ok);
    if(ok) {
        graphArea->setMarkerPosition(uIntMarkerPosition);
        graphArea->update();
    }
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
      int bytesRead=0;
      bytesRead += inFile.readRawData(wavFileHeader.chunkID, 4);
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.chunkSize), sizeof(wavFileHeader.chunkSize));
      bytesRead += inFile.readRawData(wavFileHeader.format, 4);
      bytesRead += inFile.readRawData(wavFileHeader.subchunk1ID, 4);
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.subchunk1Size), sizeof(wavFileHeader.subchunk1Size));
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.audioFormat), sizeof(wavFileHeader.audioFormat));
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.numChannels), sizeof(wavFileHeader.numChannels));
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.sampleRate), sizeof(wavFileHeader.sampleRate));
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.byteRate), sizeof(wavFileHeader.byteRate));
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.blockAlign), sizeof(wavFileHeader.blockAlign));
      bytesRead += inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.bitsPerSample), sizeof(wavFileHeader.bitsPerSample));
      inFile.skipRawData(wavFileHeader.subchunk1Size +
                         sizeof(wavFileHeader.chunkID) +
                         sizeof(wavFileHeader.chunkSize) +
                         sizeof(wavFileHeader.format) +
                         sizeof(wavFileHeader.subchunk1ID) +
                         sizeof(wavFileHeader.subchunk1Size) -
                         bytesRead);
      inFile.readRawData(wavFileHeader.subchunk2ID, 4);
      inFile.readRawData(reinterpret_cast<char *>(&wavFileHeader.subchunk2Size), sizeof(wavFileHeader.subchunk2Size));
      vectSamples.clear();
      vectSamples.resize(wavFileHeader.subchunk2Size/(wavFileHeader.bitsPerSample/8));
      for(int i=0; i<vectSamples.length();i++) {
          switch (wavFileHeader.bitsPerSample/8) {
          case 1:
              unsigned char charBuffer;
              inFile.readRawData(reinterpret_cast<char *>(&charBuffer), sizeof(charBuffer));
              vectSamples.data()[i] = static_cast<int>(charBuffer);
              break;
          case 2:
              signed short shortBuffer;
              inFile.readRawData(reinterpret_cast<char *>(&shortBuffer), sizeof(shortBuffer));
              vectSamples.data()[i] = static_cast<int>(shortBuffer);
              break;
          }
      }
      wavFile.close();
      edCurrentWavFile->setText(wavFileName);
      edWavFileSamplRate->setText(QString::number(wavFileHeader.sampleRate));
      edWavFileBitsPerSample->setText(QString::number(wavFileHeader.bitsPerSample));
      edSamplesInWav->setText(QString::number(vectSamples.length()));
      graphArea->setNewSamples(vectSamples, static_cast<unsigned int>(qPow(2, wavFileHeader.bitsPerSample-1)));
      graphArea->update();
    }
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
}

#include "mainwindow.h"
#include "renderarea.h"
#include <QFile>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    markerPosition = 1;
    graphArea = new RenderArea(&vectSamples, &vectMarks, &markerPosition);
    pBtnLoadWav = new QPushButton("Load Wav File");
    pBtnSaveMarkers = new QPushButton("Save markers");
    pBtnSaveMarkers->setEnabled(false);
    pBtnLoadMarkers = new QPushButton("Load markers");
    pBtnLoadMarkers->setEnabled(false);
    pBtnPlaceMark = new QPushButton("Place mark");
    pBtnPlaceMark->setEnabled(false);
    pBtnZoomIn = new QPushButton("Zoom in");
    pBtnZoomIn->setEnabled(false);
    pBtnZoomOut = new QPushButton("Zoom Out");
    pBtnZoomOut->setEnabled(false);
    lbCurrentWavFile = new QLabel("Wav File:");
    lbWavFileSamplRate = new QLabel("Sample Rate:");
    lbWavFileBitsPerSample = new QLabel("Bits per sample:");
    edCurrentWavFile = new QLineEdit();
    edCurrentWavFile->setEnabled(false);
    edCurrentWavFile->setReadOnly(true);
    edWavFileSamplRate = new QLineEdit();
    edWavFileSamplRate->setEnabled(false);
    edWavFileSamplRate->setReadOnly(true);
    edWavFileSamplRate->setMaximumWidth(80);
    lbMarkerPosition = new QLabel("Marker position: ");
    edMarkerPosition = new QLineEdit();
    edMarkerPosition->setEnabled(false);
    lbSamplesInWav = new QLabel("Samples in file:");
    edSamplesInWav = new QLineEdit();
    edSamplesInWav->setEnabled(false);
    edSamplesInWav->setReadOnly(true);
    edWavFileBitsPerSample = new QLineEdit();
    edWavFileBitsPerSample->setEnabled(false);
    edWavFileBitsPerSample->setReadOnly(true);
    edWavFileBitsPerSample->setMaximumWidth(60);
    cBxMarkType = new QComboBox();
    cBxMarkType->addItem("Silence");
    cBxMarkType->addItem("Speech");
    cBxMarkType->setEnabled(false);
    cBxWindowSize = new QComboBox();
    cBxWindowSize->setEnabled(false);
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
    connect(pBtnPlaceMark, SIGNAL(clicked()), this, SLOT(pBtnPlaceMarkClicked()));
    connect(edMarkerPosition, SIGNAL(textChanged(QString)), this, SLOT(edMarkerPositionTextEdited(QString)));

}

void MainWindow::edMarkerPositionTextEdited(const QString &newText)
{
    bool ok = false;
    unsigned int uIntMarkerPosition = newText.toUInt(&ok);
    if(ok && (uIntMarkerPosition>0) && (uIntMarkerPosition<=vectSamples.length())) {
        // Set normal color:
        edMarkerPosition->setStyleSheet("QLineEdit{background: white;}");
        markerPosition = uIntMarkerPosition;
        graphArea->updatePlot();
    }
    else {
        // Change color to disturbingly red:
        edMarkerPosition->setStyleSheet("QLineEdit{background: red;}");
    }
}

void MainWindow::pBtnPlaceMarkClicked()
{
    pBtnSaveMarkers->setEnabled(true);
//    unsigned int uIntMarkerPosition = edMarkerPosition->text().toInt();
    vectMarks.append(markerPosition);
//    graphArea->samplesVectorPtr()->append(markerPosition);
}

void MainWindow::pBtnSaveMarkersClicked()
{

}

void MainWindow::pBtnLoadWavClicked()
{
//    For Windows:
//    QString wavFileName("D:\\My_Documents\\Pasha_Docs\\GitHub\\SpeechDetection\\SpeechMarker\\example.wav");
//  For Linux:
    QString wavFileName("/home/pavel/dev/SpeechDetection/SpeechMarker/example.wav");
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
      // Type wav file information:
      edCurrentWavFile->setText(wavFileName);
      edWavFileSamplRate->setText(QString::number(wavFileHeader.sampleRate));
      edWavFileBitsPerSample->setText(QString::number(wavFileHeader.bitsPerSample));
      edSamplesInWav->setText(QString::number(vectSamples.length()));
      // Initial marker position:
      markerPosition = 1;
      edMarkerPosition->setText("1");
      // Unlock all necessary GUI elements:
      edMarkerPosition->setEnabled(true);
      edSamplesInWav->setEnabled(true);
      pBtnLoadMarkers->setEnabled(true);
      edCurrentWavFile->setEnabled(true);
      edWavFileSamplRate->setEnabled(true);
      edWavFileBitsPerSample->setEnabled(true);
      pBtnZoomIn->setEnabled(true);
      pBtnZoomOut->setEnabled(true);
      cBxMarkType->setEnabled(true);
      cBxWindowSize->setEnabled(true);
      pBtnPlaceMark->setEnabled(true);
      // Visualize samples:
      graphArea->setSampleMaxValue(static_cast<unsigned int>(qPow(2, wavFileHeader.bitsPerSample-1)));
      graphArea->updatePlot();
      // Clear all previusly set marks:
      vectMarks.clear();
    }
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
}

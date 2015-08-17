#include "mainwindow.h"
#include "renderarea.h"
#include <QFile>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    markerPosition = 0;
    graphArea = new RenderArea(&vectSamples, &vectMarks, &markerPosition);
    graphArea->setEnabled(false);
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
    cBxIntervals = new QComboBox();
//    cBxIntervals ->addItem(defaultLabel);
    cBxIntervals ->setMinimumWidth(120);
    cBxIntervals->setEnabled(false);
    edSamplesInWav->setEnabled(false);
    edSamplesInWav->setReadOnly(true);
    edWavFileBitsPerSample = new QLineEdit();
    edWavFileBitsPerSample->setEnabled(false);
    edWavFileBitsPerSample->setReadOnly(true);
    edWavFileBitsPerSample->setMaximumWidth(60);
    cBxMarkType = new QComboBox();
    cBxMarkType->addItem(defaultLabel);
    cBxMarkType->addItem("SL");
    cBxMarkType->addItem("SP");
    cBxMarkType->setMinimumWidth(80);
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
    hBoxLayMarkerPosition ->addWidget(cBxIntervals);
    hBoxLayMarkerPosition-> addWidget(cBxMarkType);

    hBoxLayControlButtons = new QHBoxLayout();
    hBoxLayControlButtons->addWidget(pBtnLoadWav);
    hBoxLayControlButtons->addWidget(pBtnLoadMarkers);
    hBoxLayControlButtons->addWidget(pBtnSaveMarkers);

    vBoxLayMarksSettings = new QVBoxLayout();
    vBoxLayMarksSettings->addWidget(pBtnZoomIn);
    vBoxLayMarksSettings->addWidget(pBtnZoomOut);
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
    connect(graphArea, SIGNAL(markerPositionChanged(int)), this, SLOT(graphAreaMarkerPositionChanged(int)));
    //connect(cBxIntervals, static_cast<void(QComboBox::*)(int)>);
    connect(cBxIntervals, SIGNAL(currentIndexChanged(int)), this, SLOT(cBxIntervalsCurrentIndexChanged(int)));
    connect(cBxMarkType, SIGNAL(currentIndexChanged(QString)), this, SLOT(cBxMarkTypeCurrentIndexChanged(QString)));
}

void MainWindow::graphAreaMarkerPositionChanged(int newPosition)
{
    markerPosition = newPosition;
    edMarkerPosition->setText(QString::number(markerPosition));
    graphArea->updatePlot();
}

void MainWindow::edMarkerPositionTextEdited(const QString &newText)
{
    bool ok = false;
    int uIntMarkerPosition = newText.toUInt(&ok);
    if(ok && (uIntMarkerPosition>=0) && (uIntMarkerPosition<vectSamples.length())) {
        // Set normal color:
        edMarkerPosition->setStyleSheet("QLineEdit{background: white;}");
        // Unlock "Place marker button"
        pBtnPlaceMark->setEnabled(true);
        markerPosition = uIntMarkerPosition;
        graphArea->updatePlot();
    }
    else {
        // Change color to disturbingly red:
        edMarkerPosition->setStyleSheet("QLineEdit{background: red;}");
        pBtnPlaceMark->setEnabled(false);
    }
}

void MainWindow::pBtnPlaceMarkClicked()
{
    pBtnSaveMarkers->setEnabled(true);
    if(!vectMarks.contains(markerPosition)) {
        int i=0;
        while((i<vectMarks.length()) && (markerPosition>vectMarks.data()[i])) {i++;}
        QString currentIntervalLabel = vectLabels.data()[i-1];  // Because number of labels = (number of marks - 1)
        vectLabels.remove(i-1,1);
        vectLabels.insert(i-1, currentIntervalLabel);
        vectLabels.insert(i, currentIntervalLabel);
        vectMarks.insert(i, markerPosition);
        cBxIntervals->removeItem(i-1);
        cBxIntervals->insertItem(i-1, QString::number(i==0? 0:vectMarks.data()[i-1]) + "-" + QString::number(vectMarks.data()[i]));
        cBxIntervals->insertItem(i, QString::number(vectMarks.data()[i]) + "-" + QString::number(i==vectMarks.length()? vectSamples.length()-1:vectMarks.data()[i+1]));
    }
}

void MainWindow::cBxMarkTypeCurrentIndexChanged(const QString & text)
{
    int currInterval = cBxIntervals->currentIndex();
    vectLabels[currInterval] = text;
}

void MainWindow::cBxIntervalsCurrentIndexChanged(int index)
{
    if(index != -1) {
        cBxMarkType->setCurrentIndex(cBxMarkType->findText(vectLabels[index]));
        graphArea->setSelectedInterval(index);
        graphArea->updatePlot();
    }
}

void MainWindow::pBtnSaveMarkersClicked()
{

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
      // Type wav file information:
      edCurrentWavFile->setText(wavFileName);
      edWavFileSamplRate->setText(QString::number(wavFileHeader.sampleRate));
      edWavFileBitsPerSample->setText(QString::number(wavFileHeader.bitsPerSample));
      edSamplesInWav->setText(QString::number(vectSamples.length()));
      // Add default label that cover the whole wav:
      vectLabels.clear();
      vectLabels.append(defaultLabel);
      // Clear all previusly set marks:
      vectMarks.clear();
      // Add two marks, namely the first and the last sample (since we have only one label that covers the whole wav):
      vectMarks.append(0);
      vectMarks.append(vectSamples.length()-1);
      // When wav file is opened we have one interval from begining to the end:
      cBxIntervals->addItem(QString::number(vectMarks[0]) + "-" + QString::number(vectMarks[1]));
      // Initial marker position:
      markerPosition = 0;
      edMarkerPosition->setText(QString::number(markerPosition));
      // Unlock all necessary GUI elements:
      edMarkerPosition->setEnabled(true);
      edSamplesInWav->setEnabled(true);
      pBtnLoadMarkers->setEnabled(true);
      edCurrentWavFile->setEnabled(true);
      edWavFileSamplRate->setEnabled(true);
      edWavFileBitsPerSample->setEnabled(true);
      pBtnZoomIn->setEnabled(true);
      pBtnZoomOut->setEnabled(true);
      cBxIntervals->setEnabled(true);
      cBxMarkType->setEnabled(true);
      cBxWindowSize->setEnabled(true);
      pBtnPlaceMark->setEnabled(true);
      // Visualize samples:
      graphArea->setSampleMaxValue(static_cast<unsigned int>(qPow(2, wavFileHeader.bitsPerSample-1)));
      graphArea->setEnabled(true);
      graphArea->updatePlot();
    }
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
}

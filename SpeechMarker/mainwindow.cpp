#include "mainwindow.h"
#include "renderarea.h"
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include <QList>
#include <QThread>


#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    visibleSamplesCnt = 0;
    previousEnabledState = 0;
    markerPosition = 0;
    graphArea = new RenderArea(&vectSamples, &vectMarks, &vectLabels, &markerPosition);
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
    pBtnRemoveMark = new QPushButton("Remove mark");
    pBtnRemoveMark->setEnabled(false);
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
    sBarPlotScroller = new QScrollBar(Qt::Horizontal);
    sBarPlotScroller->setMinimum(0);
    sBarPlotScroller->setMaximum(0);

    // Progress bar construction:
    prBarOpenWavProgress = new QProgressBar();
    prBarOpenWavProgress->setOrientation(Qt::Horizontal);

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
    vBoxLayMarksSettings->addWidget(pBtnRemoveMark);

    hBoxLayWavFileLabel = new QHBoxLayout();
    hBoxLayWavFileLabel->addWidget(lbCurrentWavFile);
    hBoxLayWavFileLabel->addWidget(edCurrentWavFile);
    hBoxLayWavFileLabel->addWidget(lbWavFileSamplRate);
    hBoxLayWavFileLabel->addWidget(edWavFileSamplRate);
    hBoxLayWavFileLabel->addWidget(lbWavFileBitsPerSample);
    hBoxLayWavFileLabel->addWidget(edWavFileBitsPerSample);

    vBoxLayRenderControl = new QVBoxLayout();
    vBoxLayRenderControl->addWidget(graphArea);
    vBoxLayRenderControl->addWidget(sBarPlotScroller);
    vBoxLayRenderControl->addLayout(hBoxLayMarkerPosition);
    vBoxLayRenderControl->addLayout(hBoxLayControlButtons);
    vBoxLayRenderControl->addLayout(hBoxLayWavFileLabel);
    vBoxLayRenderControl->addWidget(prBarOpenWavProgress);

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
    connect(pBtnSaveMarkers, SIGNAL(clicked()), this, SLOT(pBtnSaveMarkersClicked()));
    connect(pBtnLoadMarkers, SIGNAL(clicked()), this, SLOT(pBtnLoadMarkersClicked()));
    connect(pBtnZoomIn, SIGNAL(clicked()), this, SLOT(pBtnZoomInClicked()));
    connect(pBtnZoomOut, SIGNAL(clicked()), this, SLOT(pBtnZoomOutClicked()));
    connect(pBtnRemoveMark, SIGNAL(clicked()), this, SLOT(pBtnRemoveMarkClicked()));
    connect(sBarPlotScroller, SIGNAL(valueChanged(int)), this, SLOT(sBarPlotScrollerValueChanged(int)));
    connect(graphArea, SIGNAL(stepsOfPrecalculation(int)), this, SLOT(prBarOpenWavProgressValueChanged(int)));
    connect(graphArea, SIGNAL(precalculatedArraysReady()), this, SLOT(drawPrecalculatedArray()));

    qRegisterMetaType<wavReaderErrCode>();
}

void MainWindow::sBarPlotScrollerValueChanged(int value)
{
    int leftVisibleBorder = value;
    int rightVisibleBorder = value + visibleSamplesCnt - 1;
    graphArea->setVisibleBorders(leftVisibleBorder, rightVisibleBorder);
    graphArea->updatePlot();
}

void MainWindow::prBarOpenWavProgressValueChanged(int value)
{
    prBarOpenWavProgress->setValue(value);
}

void MainWindow::prBarOpenWavProgressMaxValueChanged(int value)
{
    prBarOpenWavProgress->setMinimum(0);
    prBarOpenWavProgress->setMaximum(value-1);
    prBarOpenWavProgress->setValue(0);
}

void MainWindow::graphAreaMarkerPositionChanged(int newPosition)
{
    markerPosition = newPosition;
    // Changing of text also invokes graphArea update, so no need
    // to call it manually.
    edMarkerPosition->setText(QString::number(markerPosition));
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
        pBtnRemoveMark->setEnabled(true);
    }
}

void MainWindow::pBtnRemoveMarkClicked()
{
    int i = vectMarks.indexOf(markerPosition);
    if((i != -1) && (i != 0) && (i != vectMarks.length()-1)) {
        QString currentIntervalLabel = vectLabels.data()[i-1];
        vectLabels.remove(i);
        vectMarks.remove(i);
        cBxIntervals->removeItem(i-1);
        cBxIntervals->removeItem(i-1);
        cBxIntervals->insertItem(i-1, QString::number(vectMarks.data()[i-1]) + "-" + QString::number(vectMarks.data()[i]));
    }
    // We can't remove 2 fundamental marks (the beginning and the end):
    if(vectMarks.length() == 2)
        pBtnRemoveMark->setEnabled(false);
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
    //    For Windows:
    //QString labelsFileName("D:\\My_Documents\\Pasha_Docs\\GitHub\\SpeechDetection\\SpeechMarker\\labels_example.txt");
    QString labelsFileName = QFileDialog::getSaveFileName(this, "Save Marks", QDir::currentPath(), "*.txt");
    if(!labelsFileName.isEmpty()) {
        QFile labelsFile(labelsFileName);
        if(labelsFile.open(QIODevice::WriteOnly)) {
            QTextStream outfile(&labelsFile);
            for(int i=0; i<vectMarks.length(); i++) {
                outfile << static_cast<qint32>(vectMarks[i]);
                if(i < vectLabels.length())
                    outfile << " "
                            << vectLabels[i]
                               << " ";
                else
                    outfile << "";
            }
            labelsFile.close();
        }
        else
        {
            // Some message on open failure
            QMessageBox::critical(this, "SpeechMarker error", "Could not open labels file " + labelsFileName + " for writing!");
        }
    }
}

void MainWindow::pBtnLoadMarkersClicked()
{
    //    For Windows:
//    QString labelsFileName("D:\\My_Documents\\Pasha_Docs\\GitHub\\SpeechDetection\\SpeechMarker\\labels_example.txt");
    QString labelsFileName = QFileDialog::getOpenFileName(this, "Save Marks", QDir::currentPath(), "*.txt");
    if(!labelsFileName.isEmpty()) {
        QFile labelsFile(labelsFileName);
        if(!labelsFile.exists()) {
            QMessageBox::critical(this, "SpeechMarker error", "Could not open labels file " + labelsFileName + ". File doesn't exist!");
        }
        else {
            if(labelsFile.open(QIODevice::ReadOnly)) {
                // Block signals from cBxIntervals and edMarkerPosition to prevent excessive updates of graphArea:
                cBxIntervals->blockSignals(true);
                edMarkerPosition->blockSignals(true);
                // Clear all marks and labels:
                vectMarks.clear();
                vectLabels.clear();
                cBxIntervals->clear();
                int tmp_mark = 0;
                QString tmp_label;
                QTextStream infile(&labelsFile);
                while(!infile.atEnd()) {
                    infile >> tmp_mark;
                    infile >> tmp_label;
                    vectMarks.append(tmp_mark);
                    if(!tmp_label.isEmpty())
                        vectLabels.append(tmp_label);
                }
                // Fill in intervals list with newly read marks:
                for(int i=0; i < vectLabels.length(); i++) {
                    cBxIntervals->addItem(QString::number(vectMarks[i]) + "-" + QString::number(vectMarks[i+1]));
                }
                markerPosition = 0;
                edMarkerPosition->setText(QString::number(markerPosition));
                // Unblock signals emission and force update of graphArea:
                cBxIntervals->blockSignals(false);
                edMarkerPosition->blockSignals(false);
                graphArea->updatePlot();
                labelsFile.close();
            }
            else {
                QMessageBox::critical(this, "SpeechMarker error", "Could not open labels file " + labelsFileName + " for reading.");
            }
        }
    }
}

void MainWindow::pBtnZoomInClicked()
{
    visibleSamplesCnt = visibleSamplesCnt / zoomCoeff;
    visibleSamplesCnt = visibleSamplesCnt < 100? 100 : visibleSamplesCnt;
    int leftVisibleBorder = markerPosition - visibleSamplesCnt/2;
    if(leftVisibleBorder < 0) {
        leftVisibleBorder = 0;
    }
    int rightVisibleBorder = leftVisibleBorder + visibleSamplesCnt-1;
    if(rightVisibleBorder >= vectSamples.length()) {
        rightVisibleBorder = vectSamples.length()-1;
        leftVisibleBorder = rightVisibleBorder - (visibleSamplesCnt-1);
    }
    visibleSamplesCnt = rightVisibleBorder - leftVisibleBorder + 1;
    graphArea->setVisibleBorders(leftVisibleBorder, rightVisibleBorder);
    // Set new parameters for scroller:
    sBarPlotScroller->blockSignals(true);   // Block signal emission to prevent repetitive actions
    sBarPlotScroller->setMaximum(vectSamples.length() - visibleSamplesCnt);
    sBarPlotScroller->setValue(leftVisibleBorder);
    sBarPlotScroller->setSingleStep(qFloor(static_cast<double>(wavFileHeader.sampleRate)*0.01));
    sBarPlotScroller->blockSignals(false);
    graphArea->updatePlot();
}

void MainWindow::pBtnZoomOutClicked()
{
    visibleSamplesCnt = visibleSamplesCnt * zoomCoeff;
    visibleSamplesCnt = (visibleSamplesCnt > vectSamples.length())? vectSamples.length() : visibleSamplesCnt;
    int leftVisibleBorder = markerPosition - visibleSamplesCnt/2;
    if(leftVisibleBorder < 0) {
        leftVisibleBorder = 0;
    }
    int rightVisibleBorder = leftVisibleBorder + visibleSamplesCnt-1;
    if(rightVisibleBorder >= vectSamples.length()) {
        rightVisibleBorder = vectSamples.length()-1;
        leftVisibleBorder = rightVisibleBorder - (visibleSamplesCnt-1);
    }
    visibleSamplesCnt = rightVisibleBorder - leftVisibleBorder + 1;
    graphArea->setVisibleBorders(leftVisibleBorder, rightVisibleBorder);
    // Set new parameters for scroller:
    sBarPlotScroller->blockSignals(true);   // Block signal emission to prevent repetitive actions
    sBarPlotScroller->setMaximum(vectSamples.length() - visibleSamplesCnt);
    sBarPlotScroller->setValue(leftVisibleBorder);
    sBarPlotScroller->setSingleStep(qFloor(static_cast<double>(wavFileHeader.sampleRate)*0.01));
    sBarPlotScroller->blockSignals(false);
    graphArea->updatePlot();
}

void MainWindow::pBtnLoadWavClicked()
{
//    For Windows:
//    QString wavFileName("D:\\My_Documents\\Pasha_Docs\\GitHub\\SpeechDetection\\SpeechMarker\\example.wav");
//    For Linux:
//    QString wavFileName("/home/pavel/dev/SpeechDetection/SpeechMarker/example.wav");
    // --- Disable all widgets:
    QList<QWidget *> allWidgets = this->findChildren<QWidget *>();
    if(previousEnabledState!=0)
        delete []previousEnabledState;
    previousEnabledState = new bool[allWidgets.size()];
    for (int i=0; i<allWidgets.size(); i++) {
        previousEnabledState[i] = allWidgets[i]->isEnabled();
        allWidgets[i]->setEnabled(false);
    }
    // ---
    QString wavFileName = QFileDialog::getOpenFileName(this, "Save Marks", QDir::currentPath(), "*.wav");
    graphArea->setState(INACTIVE, "Reading wav file...");
    graphArea->updatePlot();
    WorkerWavFileReader *worker = new WorkerWavFileReader(&wavFileHeader, &vectSamples, wavFileName);
    QThread *new_thread = new QThread;
    worker->moveToThread(new_thread);
    connect(new_thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), new_thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(new_thread, SIGNAL(finished()), new_thread, SLOT(deleteLater()));
    connect(worker, SIGNAL(processResult(wavReaderErrCode, QString)), this, SLOT(processWavReaderResult(wavReaderErrCode, QString)));
    connect(worker, SIGNAL(bytesAlreadyRead(int)), this, SLOT(prBarOpenWavProgressValueChanged(int)));
    connect(worker, SIGNAL(samplesInWavToRead(int)), this, SLOT(prBarOpenWavProgressMaxValueChanged(int)));
    new_thread->start();
}

void MainWindow::processWavReaderResult(wavReaderErrCode errCode, QString wavFileName)
{
    QList<QWidget *> allWidgets = this->findChildren<QWidget *>();
    switch (errCode) {
    case FILENOTEXIST:
        // Restore previuos enable state for every widget:
        for (int i=0; i<allWidgets.size(); i++) {
            allWidgets[i]->setEnabled(previousEnabledState[i]);
        }
        graphArea->setState(ACTIVEDRAWING);
        break;
    case WAVEMPTY:
        // Restore previuos enable state for every widget:
        for (int i=0; i<allWidgets.size(); i++) {
            allWidgets[i]->setEnabled(previousEnabledState[i]);
        }
        graphArea->setState(ACTIVEDRAWING);
        break;
    case READSUCC:
        visibleSamplesCnt = vectSamples.length();
        prBarOpenWavProgress->setMinimum(0);
        prBarOpenWavProgress->setMaximum(8);
        prBarOpenWavProgress->setValue(0);
        graphArea->setVisibleBorders(0, vectSamples.length()-1);
        graphArea->setState(INACTIVE, "Precalculating waveforms for visualization...");
        //-------
        // We prepare arrays of signal extrema for fast drawing:
        graphArea->preparePrecalculatedArrays();
        // Then MainWidget waits for signal precalculatedArraysReady() from graphArea which is connected to slot drawPrecalculatedArray()
        //-------------------------------------------------------------------------------------------------------------------------------
        graphArea->setSampleMaxValue(static_cast<unsigned int>(qPow(2, wavFileHeader.bitsPerSample-1)));
        // Block signals from edMarkerPosition and cBxIntervals to prevent excessive updates of graphArea:
        edMarkerPosition->blockSignals(true);
        cBxIntervals->blockSignals(true);
        edWavFileSamplRate->setText(QString::number(wavFileHeader.sampleRate));
        edWavFileBitsPerSample->setText(QString::number(wavFileHeader.bitsPerSample));
        edSamplesInWav->setText(QString::number(vectSamples.length()));
        // Add default label that covers the whole wav:
        vectLabels.clear();
        vectLabels.append(defaultLabel);
        // Clear all previously set marks:
        vectMarks.clear();
        // Add two marks, namely the first and the last sample (since we have only one label that covers the whole wav):
        vectMarks.append(0);
        vectMarks.append(vectSamples.length()-1);
        cBxIntervals->clear();
        // When wav file is opened we have one interval from begining to the end:
        cBxIntervals->addItem(QString::number(vectMarks[0]) + "-" + QString::number(vectMarks[1]));
        // Initial marker position:
        markerPosition = 0;
        edMarkerPosition->setText(QString::number(markerPosition));
        // Type wav file information:
        edCurrentWavFile->setText(wavFileName);
        break;
    default:
        break;
    }
    graphArea->updatePlot();
}

void MainWindow::drawPrecalculatedArray()
{
    // Unlock all GUI elements:
    QList<QWidget *> allWidgets = this->findChildren<QWidget *>();
    foreach (QWidget *childWidget, allWidgets) {
        childWidget->setEnabled(true);
    }
    // Disable "Remove Mark" button because there are no marks yet:
    pBtnRemoveMark->setEnabled(false);
    // ---
    sBarPlotScroller->setMinimum(0);
    sBarPlotScroller->setMaximum(0);
    // Unblock signals emission from edMarkerPosition and cBxIntervals and force update for graphArea:
    edMarkerPosition->blockSignals(false);
    cBxIntervals->blockSignals(false);
    graphArea->setEnabled(true);
    graphArea->setState(ACTIVEDRAWING);
    graphArea->updatePlot();
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
}

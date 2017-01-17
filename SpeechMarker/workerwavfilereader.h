#ifndef WORKERWAVFILEREADER_H
#define WORKERWAVFILEREADER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QFile>
#include "mainwindow.h"

class WorkerWavFileReader : public QObject
{
    Q_OBJECT
public:
    explicit WorkerWavFileReader(wavHeader *headerPtr, QVector<int> *vectSamplesPtr, QString &inputFileName, QObject *parent = 0);
public slots:
    void process();
signals:
    void finished();
    void updateCurrentWavFileName(QString);
private:
    wavHeader *prtWavHeader;
    QVector<int> *ptrVectSamples;
    QString wavFileName;
};

#endif // WORKERWAVFILEREADER_H

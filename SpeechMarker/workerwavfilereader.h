#ifndef WORKERWAVFILEREADER_H
#define WORKERWAVFILEREADER_H

#include <QObject>
#include <QString>
#include <QVector>

#define BYTES_READ_CNT_SIGNAL 32768

enum wavReaderErrCode {READSUCC, WAVEMPTY, FILENOTEXIST};

Q_DECLARE_METATYPE(wavReaderErrCode)

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

class QByteArray;

class WorkerWavFileReader : public QObject
{
    Q_OBJECT
public:
    explicit WorkerWavFileReader(wavHeader *headerPtr, QVector<int> *vectSamplesPtr, QByteArray *rawWavBytes, QString &inputFileName, QObject *parent = 0);
public slots:
    void process();
signals:
    void finished();
    void processResult(wavReaderErrCode errCode, QString);
    void samplesInWavToRead(int samplesCnt);
    void bytesAlreadyRead(int bytesCnt);
private:
    wavHeader *prtWavHeader;
    QVector<int> *ptrVectSamples;
    QByteArray *ptrByteArr;
    QString wavFileName;
};

#endif // WORKERWAVFILEREADER_H

/******************************************************************************
 * SpeechMarker:  Qt-based GUI application for marking wav files
 * Copyright (C) 2015-2018 Pavel Paramonov <pa.pawka@gmail.com>

 * SpeechMarker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * SpeechMarker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with SpeechMarker; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
******************************************************************************/

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

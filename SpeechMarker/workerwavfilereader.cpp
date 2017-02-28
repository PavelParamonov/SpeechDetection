#include "workerwavfilereader.h"
#include <QFile>
#include <QDataStream>
#include <QByteArray>

WorkerWavFileReader::WorkerWavFileReader(wavHeader *headerPtr, QVector<int> *vectSamplesPtr, QByteArray *rawWavBytes, QString &inputFileName, QObject *parent) : QObject(parent)
{
    prtWavHeader=headerPtr; ptrVectSamples=vectSamplesPtr; wavFileName = inputFileName; ptrByteArr = rawWavBytes;
}

void WorkerWavFileReader::process()
{
    if(!wavFileName.isEmpty()) {
        QFile wavFile(wavFileName);
        if (!wavFile.exists()) {
            emit processResult(FILENOTEXIST, QString("File doesn't exist"));
        }
        else {
            wavFile.open(QIODevice::ReadOnly);
            QDataStream inFile(&wavFile);
            int bytesRead=0;
            bytesRead += inFile.readRawData(prtWavHeader->chunkID, 4);
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->chunkSize), sizeof(prtWavHeader->chunkSize));
            bytesRead += inFile.readRawData(prtWavHeader->format, 4);
            bytesRead += inFile.readRawData(prtWavHeader->subchunk1ID, 4);
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->subchunk1Size), sizeof(prtWavHeader->subchunk1Size));
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->audioFormat), sizeof(prtWavHeader->audioFormat));
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->numChannels), sizeof(prtWavHeader->numChannels));
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->sampleRate), sizeof(prtWavHeader->sampleRate));
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->byteRate), sizeof(prtWavHeader->byteRate));
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->blockAlign), sizeof(prtWavHeader->blockAlign));
            bytesRead += inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->bitsPerSample), sizeof(prtWavHeader->bitsPerSample));
            inFile.skipRawData(prtWavHeader->subchunk1Size +
                               sizeof(prtWavHeader->chunkID) +
                               sizeof(prtWavHeader->chunkSize) +
                               sizeof(prtWavHeader->format) +
                               sizeof(prtWavHeader->subchunk1ID) +
                               sizeof(prtWavHeader->subchunk1Size) -
                               bytesRead);
            inFile.readRawData(prtWavHeader->subchunk2ID, 4);
            inFile.readRawData(reinterpret_cast<char *>(&prtWavHeader->subchunk2Size), sizeof(prtWavHeader->subchunk2Size));
            ptrVectSamples->clear();
            ptrVectSamples->resize(prtWavHeader->subchunk2Size/(prtWavHeader->bitsPerSample/8));
            // reading raw bytes:
            ptrByteArr->clear();
            ptrByteArr->resize(prtWavHeader->subchunk2Size);
            bytesRead = inFile.readRawData(ptrByteArr->data(), ptrByteArr->length());
            // ------------------
            emit samplesInWavToRead(ptrVectSamples->length());
            for(int i=0; i<ptrVectSamples->length();i++) {
                switch (prtWavHeader->bitsPerSample/8) {
                case 1:
                    // Could be done simplier! I just copied it from uShort version lower:
                    unsigned char charBuffer;
                    charBuffer = *(reinterpret_cast<unsigned short *>(ptrByteArr->data()+ i*prtWavHeader->bitsPerSample/8));
                    ptrVectSamples->data()[i] = static_cast<int>(charBuffer);
                    break;
                case 2:
                    signed short shortBuffer;
                    shortBuffer = *(reinterpret_cast<signed short *>(ptrByteArr->data()+ i*prtWavHeader->bitsPerSample/8));
                    ptrVectSamples->data()[i] = static_cast<int>(shortBuffer);
                    break;
                }
                if((!(i % BYTES_READ_CNT_SIGNAL)) || (i == ptrVectSamples->length()-1))
                    emit bytesAlreadyRead(i);
            }
            wavFile.close();
            emit processResult(READSUCC, wavFileName);
        }
    }
    else
        emit processResult(WAVEMPTY, QString("Wav file is empty"));
    emit finished();
}

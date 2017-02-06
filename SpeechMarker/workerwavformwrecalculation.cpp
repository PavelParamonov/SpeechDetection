#include "workerwavformwrecalculation.h"
#include <QVector>
#include <QPair>
#include <QtMath>
#include <QMutex>

WorkerWavformPrecalculation::WorkerWavformPrecalculation(const QVector<int> *vectSamples, QVector<QPair<int, int> > *vectExtrema, int samplesPerPixel, QMutex *mutex, QObject *parent) : QObject(parent), ptrVectSamples(vectSamples)
{
    ptrVectExtrema = vectExtrema;
    ptrMutex = mutex;
    this->samplesPerPixel = samplesPerPixel;
}

void WorkerWavformPrecalculation::process()
{
    ptrMutex->lock();
    int vectLen = qCeil(ptrVectSamples->length() / static_cast<double>(samplesPerPixel));
    ptrVectExtrema->resize(vectLen);
    for(int i=0; i < ptrVectExtrema->length(); i++) {
        ptrVectExtrema->data()[i].first = i*samplesPerPixel;     // Index of the 1-st extremum for the i-th window
        ptrVectExtrema->data()[i].second = i*samplesPerPixel;    // Index of the 2-nd extremum for the i-th window
        for(int j=i*samplesPerPixel + 1; (j < (i+1)*samplesPerPixel) && (j < ptrVectSamples->length()); j++) {
            // Find index of a minimal number in the i-th window:
            if(ptrVectSamples->data()[j] < ptrVectSamples->data()[ptrVectExtrema->data()[i].first])
                ptrVectExtrema->data()[i].first = j;
            // Find index of a maximal number in the i-th window:
            if(ptrVectSamples->data()[j] > ptrVectSamples->data()[ptrVectExtrema->data()[i].second])
                ptrVectExtrema->data()[i].second = j;
        }
        // Correct order for first and second if necessary:
        if(ptrVectExtrema->data()[i].first > ptrVectExtrema->data()[i].second)
            std::swap(ptrVectExtrema->data()[i].first, ptrVectExtrema->data()[i].second);
    }
    ptrMutex->unlock();
    emit finished();
}

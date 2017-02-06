#ifndef WORKERWAVFORMPRECALCULATION_H
#define WORKERWAVFORMPRECALCULATION_H

#include <QObject>

class QMutex;

class WorkerWavformPrecalculation : public QObject
{
    Q_OBJECT
public:
    explicit WorkerWavformPrecalculation(const QVector<int> *vectSamples, QVector<QPair<int, int> > *vectExtrema, int samplesPerPixel, QMutex *mutex, QObject *parent = 0);

signals:
    void finished();
public slots:
    void process();
private:
    const QVector<int> *ptrVectSamples;
    QVector<QPair<int, int> > *ptrVectExtrema;
    int samplesPerPixel;
    QMutex *ptrMutex;
};

#endif // WORKERWAVFORMPRECALCULATION_H

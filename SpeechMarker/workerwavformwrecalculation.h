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

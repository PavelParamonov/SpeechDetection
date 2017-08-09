# SpeechDetection

All that belongs to speech detection project.

## About

Currently only one part of this project is in active development - SpeechMarker. It is a Qt-based marking application that visualizes .wav file (single channel), so that user could select regions of interest and label these regions. It is already usable (see screenshots). However, many features are waiting to be implemented.
![SpeechMarker screenshot 1](/SpeechMarker/data/screenshots/sc_1.PNG)
![SpeechMarker screenshot 2](/SpeechMarker/data/screenshots/sc_2.PNG)
It would also be great to start development of SpeechDetect - (maybe Qt-based) application for automatic detection of .wav regions that contain speech. Don't know when I'll start it though.

## Features

* Wav files reading
* Waveform zooming
* Easy labeling of selected regions
* Audio playback
* Labels loading and saving
* Plain text format for storing labels

## Build

As a Qt project:
1. Clone repository: `git clone git://github.com/PavelParamonov/SpeechDetection.git`
2. Create "Empty qmake project" with Qt Creator
3. Add all source files into project
4. Add into project file (.pro):
```
QT += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = SpeechMarker
TEMPLATE = app
```

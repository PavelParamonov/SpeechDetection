#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pBtnLoadWav = new QPushButton("Load Wav File", this);
    pBtnSaveMarkers = new QPushButton("Save markers", this);
    pBtnLoadMarkers = new QPushButton("Load markers", this);
    pBtnPlaceMark = new QPushButton("Place mark", this);
    pBtnZoomIn = new QPushButton("Zoom in", this);
    pBtnZoomOut = new QPushButton("Zoom Out", this);
}

MainWindow::~MainWindow()
{
    delete pBtnLoadWav;
    delete pBtnSaveMarkers;
    delete ui;
}

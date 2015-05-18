#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // GIU members:
    Ui::MainWindow *ui;
    QPushButton *pBtnLoadWav, *pBtnSaveMarkers, *pBtnLoadMarkers, *pBtnPlaceMark, *pBtnZoomIn, *pBtnZoomOut;
    QComboBox   *cBxMarkType, *cBxWindowSize;
    QLabel      *lbCurrentWavFile;
    QLineEdit   *edCurrentWavFile;
    QVBoxLayout *vBoxLayMarksSettings;
    QHBoxLayout *hBoxLayControlButtons;
    // Other members:
    //QVector     *vectSamples, *vectMarks;
};

#endif // MAINWINDOW_H

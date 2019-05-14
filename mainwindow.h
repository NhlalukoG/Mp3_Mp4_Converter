#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QStack>
#include <QPainter>
#include <QRadialGradient>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QThreadPool>
#include <converter.h>

/*For image preview*/
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

/*AutoComplete*/
#include <QCompleter>
#include <QDirModel>
#include "countfiles.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_vidsDirBrowser_clicked();
    void on_saveToBrowser_clicked();
    void on_pushButton_clicked();
    void on_getImage_clicked();
    void writeoutput(QString text);
    void on_pushButton_2_clicked();
    void on_tobrowse_editingFinished();
    void counterDone(int mp3, int mp4);

private:
    Ui::MainWindow *ui;
    QFileDialog *x;
    QTemporaryFile *temp,*temp2;
    void paintEvent(QPaintEvent *);
    void countFilesAndDirRecursive();
    int mp3count,mp4count;
    QStack<QDir> stack;
    QMessageBox error;
    QThreadPool *pool;
    Converter *conv;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *img;

    QCompleter *fscompleter;
    QDirModel *fsdircompleter;

    CountFIles *files;

};

#endif // MAINWINDOW_H

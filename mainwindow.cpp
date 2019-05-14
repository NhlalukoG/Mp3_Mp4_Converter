#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Multi Mp3 to Mp4 Convert");
    x = new QFileDialog(this,Qt::Dialog);
    x->setFileMode(QFileDialog::DirectoryOnly);
    stack = QStack<QDir>();

    // Error message box
    error.setIcon(QMessageBox::Warning);
    error.setStandardButtons(QMessageBox::Ok);
    x->setDirectory(QDir::home());

    pool = new QThreadPool(this);
    pool->setMaxThreadCount(1);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    /*AutoComplete for manual path editing*/
    fscompleter = new QCompleter(this);
    fsdircompleter = new QDirModel(fscompleter);
    fscompleter->setModel(fsdircompleter);

    ui->tobrowse->setCompleter(fscompleter);
    ui->image_loc->setCompleter(fscompleter);
    ui->saveto->setCompleter(fscompleter);

    /*File counter*/
    files = new CountFIles(stack,mp3count,mp4count);

    connect(files,SIGNAL(done(int,int)),this,SLOT(counterDone(int,int)));

    conv = new Converter();
    connect(conv,SIGNAL(output(QString)),this,SLOT(writeoutput(QString)));
}

MainWindow::~MainWindow()
{
    /*Memory cleaning*/
    delete files;
    delete conv;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(ui->image_loc->text().length() > 0 && QFile(ui->image_loc->text()).exists()){

        ui->graphicsView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    }
}
void MainWindow::countFilesAndDirRecursive(){
    if(files->isRunning()){
        ui->plainTextEdit->appendPlainText("Terminating current process");
       files->terminate();
    }
    /*Lets update the values*/
    files->setStack(stack);
    files->start();
    ui->plainTextEdit->appendPlainText("counting");
}
void MainWindow::on_vidsDirBrowser_clicked()
{
    x->exec();
    if(!x->directory().isReadable()){
        error.setText("Cannot read from selected directory");
        error.exec();
        ui->tobrowse->setText("");
        ui->mp4count->setText("");
        ui->mp3count->setText("");
    }else{
        ui->tobrowse->setText(x->directory().absolutePath().replace(" ","\\ "));
        stack.clear();
        stack.push(x->directory());
        countFilesAndDirRecursive();
        ui->mp3count->setText(QString::number(mp3count));
        ui->mp4count->setText(QString::number(mp4count));
    }
}

void MainWindow::on_saveToBrowser_clicked()
{
    x->exec();
    if(!x->directory().isReadable()){
        error.setText("Cannot write to selected directory, please select another directory");
        error.exec();
        ui->saveto->setText("");
    }else{
        ui->saveto->setText(x->directory().absolutePath());
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->tobrowse->text() == ""){
        // No browse directory
        error.setText("please select input directory");
        ui->tobrowse->setStyleSheet("background-color:red");
        if(ui->saveto->text() == ""){
            ui->saveto->setStyleSheet("background-color:red");
        }
        if(ui->image_loc->text() == ""){
            ui->image_loc->setStyleSheet("background-color:red");
        }
        error.exec();
        return;
    }
    if(ui->saveto->text() == ""){
        error.setText("please select input directory");
        ui->saveto->setStyleSheet("background-color:red");
        if(ui->image_loc->text() == ""){
            ui->image_loc->setStyleSheet("background-color:red");
        }
        error.exec();
        return;
    }

    // Testing done, now to convert
    if(ui->tomp4->isChecked()){
        if(ui->image_loc->text() == ""){
            error.setText("Please select image or video to encode with your mp3");
            error.exec();
            ui->image_loc->setStyleSheet("background-color:red");
            return;
        }
        conv->setData(stack,ui->saveto->text(),ui->image_loc->text(),"tomp4","/home/nhlaluko/Desktop/Stuff/ffmpeg");
        conv->start();
    }else{
        conv->setData(stack,ui->saveto->text(),"tomp3","/home/nhlaluko/Desktop/Stuff/ffmpeg");
        conv->start();
    }
}
void MainWindow::on_getImage_clicked()
{
    ui->image_loc->setText(QFileDialog::getOpenFileName(this,tr("Open Image"), x->directory().absolutePath(), tr("Image Files (*.png *.jpg *.bmp)")));
    // We have to check if image or video
    img = new QGraphicsPixmapItem(QPixmap(ui->image_loc->text()));
    scene->clear();
    scene->addItem(img);
    ui->graphicsView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox confirm(this);
    confirm.setText("Are you sure you want to cancel the running operation?");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirm.setIcon(QMessageBox::Question);

    if (confirm.exec() == QMessageBox::No) return;
    if(conv != nullptr){
        if(conv->isRunning()){
            conv->terminate();
            ui->plainTextEdit->appendHtml("<hr>");
            ui->plainTextEdit->appendPlainText("Cancelled");
        }
    }
}
void MainWindow::writeoutput(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}
void MainWindow::on_tobrowse_editingFinished()
{
    stack.clear(); // About to insert a new base directory
    QDir x((ui->tobrowse->text()));
    if(!x.isReadable()){
        error.setText("Cannot read from selected directory");
        error.exec();
        ui->tobrowse->setText("");
        ui->mp4count->setText("");
        ui->mp3count->setText("");
    }else{
        mp3count = 0;
        mp4count = 0;
        ui->tobrowse->setText(x.absolutePath().replace(" ","\\ "));
        stack.push(x);
        countFilesAndDirRecursive();
        ui->mp3count->setText(QString::number(mp3count));
        ui->mp4count->setText(QString::number(mp4count));
    }
}

void MainWindow::counterDone(int mp3, int mp4)
{
    mp3count = mp3;
    mp4count = mp4;
    ui->mp3count->setText(QString::number(mp3));
    ui->mp4count->setText(QString::number(mp4));
}

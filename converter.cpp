#include "converter.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
Converter::Converter()
{
    running = false;
}
void Converter::setData(QStack<QDir> base, QString saveto, QString operation, QString binary)
{
    stack = base;
    this->tobrowse = base.top().absolutePath();
    this->saveto = saveto;
    this->operation = operation;
    this->binary = binary;
}
void Converter::setData(QStack<QDir> base, QString saveto,QString img_loc, QString operation,QString binary)
{
    stack = base;
    this->tobrowse = base.top().absolutePath();
    this->saveto = saveto;
    this->operation = operation;
    this->binary = binary;
    this->img_loc = img_loc;
}

void Converter::run()
{
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(readStdOut()));
    convert();
    emit output("\n***********Done*********\n");
    delete process;
}

Converter::~Converter()
{
}

bool Converter::isRunning()
{
    return running;
}

void Converter::convert(){
    QString savecurrto,base;
    emit output("Starting\nOperation: " + operation);
    if(stack.isEmpty()){
        emit output("No directory to scan");
        return;
    }
    running = true;
    while(!stack.isEmpty()){
       QDir dir(stack.pop());
        foreach(QFileInfo x,dir.entryInfoList()){
            if ((x.baseName() == "") || (x.baseName() == ".") || (x.baseName() == "..")) continue;
            if (x.isDir()){
                stack.push(QDir(x.absoluteFilePath()));
            }else{
                if(operation == "tomp4"){
                    /* Convert mp3 to mp4 */
                    if(x.suffix() != "mp3")
                        continue; /*Not an mp3*/
                    base = saveto + x.absolutePath().replace(tobrowse,"");
                    /*Create Base Directory*/
                    QDir current(base);
                    savecurrto = base + "/" + x.baseName() + ".mp4";
                    if(QFile(savecurrto).exists()){
                        emit output("\n" + savecurrto + " already exists, checking for completion...\n");
                        if(QFile(savecurrto).size() >= QFile(x.absoluteFilePath()).size()){
                            emit output(savecurrto + " converted successfully, skiping.\n");
                            continue;
                        }
                    }
                    if(!current.exists()){
                        qDebug() << "making " << base;
                        current.mkpath(base);
                        current.mkdir(base);
                    }
                    emit output("***** " + x.fileName() + "*****");
                    args << "-y" << "-loop" << QString::number(1) << "-framerate" << QString::number(1) << "-i" << img_loc.replace(" ","\ ").replace("'","\'") << "-i" << x.absoluteFilePath().replace(" ","\ ").replace("'","\'") << "-c:v" << "libx264" << "-preset" << "veryslow" << "-crf" << QString::number(0) << "-c:a" << "copy" << "-shortest" << savecurrto.replace(" ","\ ").replace("'","\'");
                    process->start(binary,args);
                    process->waitForFinished(60000);
                    args.clear();
                }else{
                    /*Operation == tomp3*/
                    /* Convert mp3 to mp4 */
                    if(x.suffix() != "mp4")
                        continue; /*Not an mp3*/
                    base = saveto + x.absolutePath().replace(tobrowse,"");
                    /*Create Base Directory*/
                    QDir current(base);
                    savecurrto = base + "/" + x.baseName() + ".mp3";
                    if(QFile(savecurrto).exists()){
                        emit output("\n" + savecurrto + " already exists, checking for completion...\n");
                        if(QFile(savecurrto).size() >= QFile(x.absoluteFilePath()).size()){
                            emit output(savecurrto + " converted successfully, skiping.\n");
                            continue;
                        }
                    }
                    if(!current.exists()){
                        qDebug() << "making " << base;
                        current.mkpath(base);
                        current.mkdir(base);
                    }
                    if(x.suffix() == "mp4" || x.suffix() == "mkv" || x.suffix() == "avi"){
                        // convert here
                        qDebug() << "converting to mp3...";
                        args << "-i" << x.absoluteFilePath().replace(" ","\ ").replace("'","\'") << "-b:a" << "192K" << "-vn" << savecurrto.replace(" ","\ ").replace("'","\'");
                        process->start(binary,args);
                        process->waitForFinished(60000);
                    }else{
                        /* Not an mp4 */
                        continue;
                    }
                }
            }
        }
    }
    // delete process;
    running = false;
}
void Converter::readStdOut()
{
    //ui->plainTextEdit->insertPlainText(QString(process->readAll()));
    //ui->plainTextEdit->appendPlainText(QString(process->readAll()));
    QByteArray x = process->readAll();
    if(!x.isEmpty()){

        emit output(x);
    }
}

#ifndef CONVERTER_H
#define CONVERTER_H
#include <QRunnable>
#include <QObject>
#include <QProcess>
#include <QDir>
#include <QStack>
#include <QPointer>
#include <QThread>
class Converter : public QThread
{
    Q_OBJECT
public:
    Converter();
    void setData(QStack<QDir> base, QString saveto, QString operation, QString binary);
    void setData(QStack<QDir> base, QString saveto,QString img_loc, QString operation, QString binary);
    void run();
    ~Converter();
    bool isRunning();
private:
    void convert();
    //QStack<QDir> *stack;
    QStack<QDir> stack;
    QStringList args;
    bool running = false;
private slots:
    void readStdOut();
signals:
    void output(QString text);
protected:
    QProcess *process;
    QString fname,tobrowse,saveto,img_loc,operation,binary;
};

#endif // CONVERTER_H

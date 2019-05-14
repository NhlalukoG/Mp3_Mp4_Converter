#ifndef COUNTFILES_H
#define COUNTFILES_H

#include <QObject>
#include <QStack>
#include <QThread>
#include <QDir>
class CountFIles : public QThread
{
    Q_OBJECT
public:
    CountFIles(QStack<QDir> &base, int &mp3count, int &mp4count);
    void run();
    void setStack(QStack<QDir> &base);
protected:
    QStack<QDir> stack;
    int mp3,mp4;
signals:
    void done(int mp3,int mp4);
};

#endif // COUNTFILES_H

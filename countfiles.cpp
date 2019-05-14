#include "countfiles.h"
#include <QDebug>
CountFIles::CountFIles(QStack<QDir> &base, int &mp3count, int &mp4count)
{
    stack = base;
    mp3 = mp3count;
    mp4 = mp4count;
}

void CountFIles::run()
{
    while(!stack.isEmpty()){
       QDir dir(stack.pop());
        foreach(QFileInfo x,dir.entryInfoList()){
            if ((x.baseName() == "") || (x.baseName() == ".") || (x.baseName() == "..")) continue;
            if (x.isDir()){
                stack.push(QDir(x.absoluteFilePath()));
            }else{
                if(x.suffix() == "mp3"){
                     mp3++;
                }else if(x.suffix() == "mp4" || x.suffix() == "mkv" || x.suffix() == "avi"){
                    mp4++;
                }
            }
        }
    }
    emit done(mp3,mp4);
}

void CountFIles::setStack(QStack<QDir> &base)
{
    mp3 = 0; mp4 = 0;
    stack = base;
}

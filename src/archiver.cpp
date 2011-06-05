#include "archiver.h"
#include <QDataStream>

Archiver::Archiver()
{
    startCom = false;
    startDeC = false;
}

void Archiver::Decmpress()
{
//ololo
}

void Archiver::Decompress(QString _filename, QString _where )
{
    filename = _filename;
    where = _where;
    emit DStarted(true);
    startDeC = true;
}

void Archiver::Compress(QString _filename, QString _where)
{
    filename = _filename;
    where = _where;
    emit CStarted(true);
    startCom = true;
}

void Archiver::run()
{
    linesCount = 0;
    if(startDeC)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;
        if(where == "/")
            where = filename.remove(filename.count()-5,5);
        QFile write(where);
        if (!write.open(QIODevice::WriteOnly))
            return;
        QDataStream read(&file);

        quint32 dataCount;
        read >> dataCount;

        for(int i = 0; i < dataCount; i++)
        {
            QByteArray data;
            read >> data;
            write.write(LZ77::Decompress(data));
        }
        emit DEnd(true);
        startDeC=false;
        file.close();
        write.close();
    }
    if(startCom)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;
        if(where == "/")
            where = filename+".kani";
        QFile write(where);
        if (!write.open(QIODevice::WriteOnly))
            return;
        QDataStream wrs(&write);

        while(!file.atEnd())
        {
            linesCount++;
            file.readLine();
        }
        wrs << linesCount;
        file.close();
        if (!file.open(QIODevice::ReadOnly))
            return;
        while(!file.atEnd())
        {
            QByteArray data = file.readLine();
            wrs << LZ77::Compress(data);
        }
        emit CEnd(true);
        startCom = false;
        file.close();
        write.close();

    }
    filename.clear();
    where.clear();
}

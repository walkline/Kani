#include "archiver.h"
#include <QDataStream>
#include <QDebug>

Archiver::Archiver()
{
    startCom = false;
    startDeC = false;
    isArchive = false;
    fileSize = new quint32[FILES_LIMIT];
}

void Archiver::clear()
{
    Archiver();
    files.clear();
    dirs.clear();
    for(int i = 0; i < FILES_LIMIT; ++i)
        fileSize[i] = 0;
}

void Archiver::Decompress(QString _where )
{
    where = _where;
    emit DStarted(true);
    startDeC = true;
}

void Archiver::Compress(QString _where)
{
    where = _where;
    emit CStarted(true);
    startCom = true;
}

void Archiver::AddDir(QString name)
{
    dirs << name;
}

void Archiver::AddFiles(QStringList f)
{
    files += f;
    for(int i = 0; i < files.count(); ++i)
    {
        QFile file(files.at(i));
        if(!file.open(QIODevice::ReadOnly))
            return;
        fileSize[i] = file.size();
    }
}

void Archiver::AddFile(QString name, bool _isArchive)
{
    files << name;
    isArchive = _isArchive;
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly))
        return;
    fileSize[files.count() - 1] = file.size();
}

void Archiver::WriteInfo()
{
    QDataStream wrs(&write);
    wrs << dirs.count() << files.count();
//    wrs << dirs;
    linesC = new quint32[files.count()];
    readTimes = new quint32[files.count()];
    for(int i = 0; i < files.count(); ++i)
    {
        QFile file(files.at(i));
        if (!file.open(QIODevice::ReadOnly))
            return;
        int lines = 0;
        while(!file.atEnd())
        {
            lines++;
            file.readLine();
        }
        linesC[i] = lines;

        float readT = lines*1.0 / READ_LINES;
        if(readT > (int) readT)
            readT++;
        readTimes[i] = (quint32) readT;
        qDebug() << "Read Times " << readTimes[i];
        wrs << files.at(i) << fileSize[i] << readTimes[i];
        file.close();
    }
}

void Archiver::ReadInfo()
{

    readS >> dCount >> fCount;
    //readS >> dirs;
    readTimes = new quint32[fCount];
    for(int i = 0; i < fCount; ++i)
    {
        quint32 readT, size;
        QString str;
        readS >> str >> size >> readT;
        files << str;
        readTimes[i] = readT;
        fileSize[i] = size;
    }
}


bool Archiver::isActive()
{
    return (files.count())?true:false;
}

quint32* Archiver::getFilesSize()
{
    if(!isArchive)
        return fileSize;
    QFile file(files.at(0));
    if(!file.open(QIODevice::ReadOnly))
        return 0;
    QDataStream _readS;
    _readS.setDevice(&file);

    quint32 *sizef = new quint32[FILES_LIMIT];
    _readS >> dCount >> fCount;
    //readS >> dirs;
    for(int i = 0; i < fCount; ++i)
    {
        quint32 readT, size;
        QString str;
        _readS >> str >> size >> readT;
        sizef[i] = size;
        qDebug() << size;
    }
    file.close();
    return sizef;
}

QStringList Archiver::getFiles()
{
    if(!isArchive)
        return files;
    QFile file(files.at(0));
    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "cant open file";
    QDataStream _readS;
    _readS.setDevice(&file);

    QStringList ls;
    _readS >> dCount >> fCount;
    //readS >> dirs;
    for(int i = 0; i < fCount; ++i)
    {
        quint32 readT, size;
        QString str;
        _readS >> str >> size >> readT;
        ls << str;
    }
    file.close();
    return ls;
}

void Archiver::DelFile(int index)
{
    if(!index && files.count() == 1)
    {
        clear();
        return;
    }
    QStringList str;
    for(int i = 0; i < index; ++i)
        str << files.at(i);
    for(int i = index + 1; i < files.count(); ++i)
    {
        fileSize[i - 1] = fileSize[i];
        str << files.at(i);
    }
    files = str;
}

void Archiver::run()
{
    linesCount = 0;
    if(startDeC)
    {
        if(!isArchive)
            return;

        QFile read(files.at(0));
        files.clear();
        if(!read.open(QIODevice::ReadOnly))
            return;
        readS.setDevice(&read);

        ReadInfo();

        qDebug()<<files;
        qDebug()<<"files.count()"<<files.count();
        for(int fileIterator = 0; fileIterator < files.count(); ++fileIterator)
        {
            QFile write(files.at(fileIterator));
            if (!write.open(QIODevice::WriteOnly))
                return;
            for(int i = 0; i < readTimes[fileIterator]; ++i)
            {
                QByteArray data;
                readS >> data;
                qDebug()<<data;

                write.write(LZ77::Decompress(data));
            }
            write.close();
        }

        read.close();

        emit DEnd(true);
        startDeC=false;
    }
    if(startCom)
    {
        if(where == "/")
            where = files.at(0)+".kani";

        write.setFileName(where);
        if (!write.open(QIODevice::WriteOnly))
            return;
        QDataStream wrs(&write);

        WriteInfo();

        for(int fileIterator = 0; fileIterator < files.count(); ++fileIterator)
        {
            QFile file(files.at(fileIterator));
            if (!file.open(QIODevice::ReadOnly))
                return;
            int _readTimes = 0;
            int _readLines = 0;
            QByteArray data;
            while(!file.atEnd())
            {
                _readLines++;
                data += file.readLine();
                if(_readLines == READ_LINES)
                {
                    _readLines = 0;
                    _readTimes++;
                    qDebug()<<data;
                    wrs << LZ77::Compress(data);
                    data.clear();
                }
            }
            qDebug() << "if("<<_readTimes<<" < "<<readTimes[fileIterator]<<")";
            if(_readTimes < readTimes[fileIterator])
            {
                qDebug()<<data;
                wrs << LZ77::Compress(data);
            }
            file.close();
        }

        emit CEnd(true);
        startCom = false;
        write.close();
    }
    where.clear();
}

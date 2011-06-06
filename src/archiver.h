#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "lz77.h"
#include <QStringList>
#include <QObject>
#include <QTextBrowser>
#include <QThread>
#include <QFile>

#define READ_LINES 100
#define FILES_LIMIT 200

/*TODO:
 * 1. реализировать многофайловое архивирование. Переписать.
 */

/* Использует отдельный поток.
 * Использование:
   Archiver::Compress(filename, where); // Archiver::Decompress(filename, where);
   Archiver::run();
 *
 */
class Archiver : public QThread
{
    Q_OBJECT
private:
    QStringList files;
    QStringList dirs;
    quint32 *fileSize;
    //for decompress
    int fCount;
    int dCount;
    //get info from the file
    void Decmpress();
    bool startDeC;
    bool startCom;
    quint32 *linesC;
    quint32 *readTimes;

    bool isArchive;

    QFile write;
    QFile read;
    QDataStream readS;

    void WriteInfo();
    void ReadInfo();

    //Decompress
    QString where;

public:
    Archiver();
    void AddFiles(QStringList f);
    void DelFile(int index);
    void AddFile(QString name, bool _isArchive = false);
    void AddDirs(QStringList d){ dirs += d; }
    void AddDir(QString name);
    void Decompress(QString _where = "/");
    //TODO: добавить параметр количество считуемых строк.
    void Compress(QString _where = "/");

    void clear();

    bool isLoadArchive(){ return isArchive; }
    bool isActive();
    QStringList getFiles();
    quint32* getFilesSize();

    //unused
    qint32 linesCount;

protected:
    //QThread
    void run();

signals:
    void DStarted(bool);
    void DEnd(bool);
    void CStarted(bool);
    void CEnd(bool);

};

#endif // ARCHIVER_H

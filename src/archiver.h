#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "lz77.h"
#include <QStringList>
#include <QObject>
#include <QTextBrowser>
#include <QThread>
#include <QFile>

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
    //get info from the file
    void Decmpress();
    bool startDeC;
    bool startCom;


    //Decompress
    QString filename;
    QString where;

public:
    Archiver();
    void AddFiles(QStringList f){ files += f; }
    void AddDirs(QStringList d){ dirs += d; }
    void Decompress(QString _filename, QString _where = "/");
    //TODO: добавить параметр количество считуемых строк.
    void Compress(QString _filename, QString _where = "/");

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

#ifndef LZ77_H
#define LZ77_H

#include <QtGui/QApplication>
#include <QBuffer>
#include <QObject>
#include <QDebug>

//Класс реализирует зжатие и розжатие данных алгоритмом LZ77
//Использовать зачастую статические методы
//  QByteArray stuff = LZ77::Decompress( someCompressedData );
//  QByteArray compressedData = LZ77::Compress( someData );
class LZ77
{
public:
    LZ77();
    void InsertNode( int r );
    void DeleteNode( int p );
    void InitTree();

    static int GetLz77Offset( const QByteArray &data );

    static quint32 GetDecompressedSize( const QByteArray &data );

    QByteArray Compr( const QByteArray &ba );

    static QByteArray Decompress( const QByteArray &compressed, int offset );

    static QByteArray Decompress( const QByteArray &compressed );
    static QByteArray Compress( const QByteArray &ba );

private:
    int lson[ 4097 ];
    int rson[ 4353 ];
    int dad[ 4097 ];
    quint16 text_buf[ 4113 ];
    int match_position;
    int match_length;
    int textsize;
    int codesize;
};

#endif // LZ77_H

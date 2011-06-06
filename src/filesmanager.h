#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QWidget>
#include <QDebug>
#include <QTableWidget>
#include <QFileDialog>
#include "archiver.h"

namespace Ui {
    class FilesManager;
}

class FilesManager : public QWidget
{
    Q_OBJECT

private:
    Archiver *arch;
    bool isArchiver;

    QTableWidget *data;
    void UpdateData();
public:
    explicit FilesManager(QWidget *parent = 0);
    ~FilesManager();
    void addArchiver(Archiver *_arch);

private slots:
    void on_addButton_clicked();

    void on_dellButton_clicked();

    void on_openButton_clicked();

    void on_archButton_clicked();

    void on_newButton_clicked();

private:
    Ui::FilesManager *ui;
};

#endif // FILESMANAGER_H

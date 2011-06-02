#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filesmanager.h"
#include <QMovie>
#include "archiver.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QStringList files;
    bool haveFile;
    FilesManager fm;
    QMovie *movie;
    bool isCLoad;
    bool isDLoad;
    Archiver arch;

private slots:
    void on_pathPushButton_clicked();
    void on_haxButton_clicked();
    void on_pathLineEdit_returnPressed();
    void CompLoadStart();
    void CompLoadStop();
    void DecompLoadStart();
    void DecompLoadStop();
};

#endif // MAINWINDOW_H

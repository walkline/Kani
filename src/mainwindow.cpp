#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

QString css = "<span style=\" font-size:7pt; color:#ca6149; text-transform:uppercase;\">";
//not crossplatform!!!!
QString loadingImg = ":/img/ajax-loader.gif";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    haveFile = false;
    movie = new QMovie(loadingImg);
    CompLoadStop();
    DecompLoadStop();
    connect(&arch,SIGNAL(DStarted(bool)),this,SLOT(DecompLoadStart()));
    connect(&arch,SIGNAL(DEnd(bool)),this,SLOT(DecompLoadStop()));
    connect(&arch,SIGNAL(CStarted(bool)),this,SLOT(CompLoadStart()));
    connect(&arch,SIGNAL(CEnd(bool)),this,SLOT(CompLoadStop()));

    ui->textBrowser->clear();
    ui->textBrowser->append("<p align=center style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:7pt; color:#ca6149; text-transform:uppercase;\">For CRABS ONLY.</span>");
}
void MainWindow::CompLoadStop()
{
    ui->eyeLeft->setStyleSheet("background-image: url(:/img/eyeleft.jpg);");
    ui->eyeLeft->clear();
    isCLoad = false;
    if(!isDLoad)
        movie->stop();
    ui->textBrowser->append(css+"Process completed."+QString::number(arch.linesCount));
}

void MainWindow::CompLoadStart()
{
    ui->eyeLeft->setStyleSheet("");
    ui->eyeLeft->setMovie(movie);
    movie->start();
    isCLoad = true;
    ui->textBrowser->append(css+"Compress started.");
}
void MainWindow::DecompLoadStop()
{
    ui->eyeRight->setStyleSheet("background-image: url(:/img/eyeright.jpg);");
    ui->eyeRight->clear();
    isDLoad = false;
    if(!isCLoad)
        movie->stop();
    ui->textBrowser->append(css+"Process completed.");
}

void MainWindow::DecompLoadStart()
{
    ui->eyeRight->setStyleSheet("");
    ui->eyeRight->setMovie(movie);
    movie->start();
    isDLoad = true;
    ui->textBrowser->append(css+"Decompress started.");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pathLineEdit_returnPressed()
{
    on_haxButton_clicked();
}

void MainWindow::on_haxButton_clicked()
{
    if(haveFile)
    {
        if(ui->pathLineEdit->text().contains(".kani"))
        {
            arch.clear();
            arch.AddFile(ui->pathLineEdit->text(),true);
            arch.Decompress();
            arch.start();
        } else
        {
            arch.clear();
            arch.AddFile(ui->pathLineEdit->text());
        //    arch.AddFile("/home/walkline/lol/lz77.cpp");
            arch.Compress();
            arch.start();
        }
        haveFile = false;
        files.clear();
        return;
    }
    if(QFile::exists(ui->pathLineEdit->text()))
    {
        haveFile = true;
        on_haxButton_clicked();
    } else
        ui->textBrowser->append(css+"cant find file");
}

void MainWindow::on_pathPushButton_clicked()
{
    QFileDialog dial;
    dial.setFileMode(QFileDialog::Directory);
    files=dial.getOpenFileNames(this,tr("Select files"));
    qDebug()<<files;
//    if(files.count())
//        haveFile = true;
//    fm.show();
}

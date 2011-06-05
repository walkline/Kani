#include "filesmanager.h"
#include "ui_filesmanager.h"

FilesManager::FilesManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesManager)
{
    ui->setupUi(this);
    ui->dataTable->setColumnCount(3);
    QStringList ls;
    ls << "Name" << "Size" << "---";
    ui->dataTable->setHorizontalHeaderLabels(ls);
    ui->dataTable->setRowCount(3);
    ui->dataTable->setColumnWidth(0, 209);
    ui->dataTable->verticalHeader()->hide();
//    ui->dataTable->clearSpans();
}

FilesManager::~FilesManager()
{
    delete ui;
}

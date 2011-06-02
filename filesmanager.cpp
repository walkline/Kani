#include "filesmanager.h"
#include "ui_filesmanager.h"

FilesManager::FilesManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesManager)
{
    ui->setupUi(this);
}

FilesManager::~FilesManager()
{
    delete ui;
}

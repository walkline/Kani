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
    ui->dataTable->setColumnWidth(0, 199);
    ui->dataTable->verticalHeader()->hide();
    ui->dataTable->setTextElideMode(Qt::ElideMiddle);

    arch = new Archiver;
    data = new QTableWidget;
    data = ui->dataTable;
    isArchiver = false;
    UpdateData();
}

void FilesManager::addArchiver(Archiver *_arch)
{
    arch = _arch;
    isArchiver = true;
    arch->clear();
    UpdateData();
}

void FilesManager::UpdateData()
{
    QStringList files = arch->getFiles();
    quint32* size = new quint32[FILES_LIMIT];
    size = arch->getFilesSize();
    data->setRowCount(files.count());
    double totalsize = 0;
    for(int i = 0; i < files.size(); ++i)
    {
        QTableWidgetItem *name = new QTableWidgetItem(files.at(i));
        data->setItem(i,0,name);
        QString str = QString::number(size[i]) + " byte";
        QTableWidgetItem *_size = new QTableWidgetItem(str);
        _size->setTextAlignment(Qt::AlignCenter);
        totalsize += size[i];
        data->setItem(i,1,_size);
    }
    ui->sizeLabel->setText(QString::number(totalsize/1024)+" kb");
    ui->filesLabel->setText(QString::number(files.count()));
}

FilesManager::~FilesManager()
{
    delete ui;
}

void FilesManager::on_addButton_clicked()
{
    QStringList list;
    list = QFileDialog::getOpenFileNames(this,tr("Select files"));
    if(list.count())
        arch->AddFiles(list);
    UpdateData();
}

void FilesManager::on_dellButton_clicked()
{
    int row = data->currentRow();
    if(row == -1)
        row = 0;
    arch->DelFile(row);
    UpdateData();
}

void FilesManager::on_openButton_clicked()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "/",
                                                tr("Kani Archive (*.kani)"));
    if(name.contains(".kani"))
    {
        arch->clear();
        arch->AddFile(name,true);
    }
    UpdateData();
}

void FilesManager::on_archButton_clicked()
{
    if(arch->isLoadArchive())
    {
        arch->Decompress();
        arch->start();
    } else {
        arch->Compress();
        arch->start();
    }
}

void FilesManager::on_newButton_clicked()
{
    arch->clear();
    UpdateData();
}

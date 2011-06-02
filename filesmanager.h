#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QWidget>

namespace Ui {
    class FilesManager;
}

class FilesManager : public QWidget
{
    Q_OBJECT

public:
    explicit FilesManager(QWidget *parent = 0);
    ~FilesManager();

private:
    Ui::FilesManager *ui;
};

#endif // FILESMANAGER_H

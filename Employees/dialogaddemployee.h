#ifndef DIALOGADDEMPLOYEE_H
#define DIALOGADDEMPLOYEE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QDateTime>

#include <database.h>
#include <employee.h>
#include <manager.h>
#include <sales.h>

namespace Ui {
class DialogAddEmployee;
}

class DialogAddEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEmployee(int row = -1, QWidget *parent = 0);
    ~DialogAddEmployee();

signals:
    void signalReady();

private slots:
    void on_buttonBox_accepted();
    void updateButtons(int row);

private:
    Ui::DialogAddEmployee       *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;

private:
    void setupModel();
    void createUI();
    void accept();

public:
    void salary(); 
    double workerSalary(WorkerBase *base);
};

#endif // DIALOGADDEMPLOYEE_H

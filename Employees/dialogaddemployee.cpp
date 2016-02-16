#include "dialogaddemployee.h"
#include "ui_dialogaddemployee.h"

DialogAddEmployee::DialogAddEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEmployee)
{
    ui->setupUi(this);
}

DialogAddEmployee::~DialogAddEmployee()
{
    delete ui;
}

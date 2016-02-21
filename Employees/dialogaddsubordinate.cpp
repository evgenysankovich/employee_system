#include "dialogaddsubordinate.h"
#include "ui_dialogaddsubordinate.h"

DialogAddSubordinate::DialogAddSubordinate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddSubordinate)
{
    ui->setupUi(this);
}

DialogAddSubordinate::~DialogAddSubordinate()
{
    delete ui;
}

QString DialogAddSubordinate::surnameSubordinate()
{
    return ui->surnameSubordinatelineEdit->text();
}

QString DialogAddSubordinate::surnameBoss()
{
    return ui->surnameBosslineEdit->text();
}

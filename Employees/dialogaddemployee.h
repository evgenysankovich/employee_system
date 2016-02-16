#ifndef DIALOGADDEMPLOYEE_H
#define DIALOGADDEMPLOYEE_H

#include <QDialog>

namespace Ui {
class DialogAddEmployee;
}

class DialogAddEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEmployee(QWidget *parent = 0);
    ~DialogAddEmployee();

private:
    Ui::DialogAddEmployee *ui;
};

#endif // DIALOGADDEMPLOYEE_H

#ifndef DIALOGADDSUBORDINATE_H
#define DIALOGADDSUBORDINATE_H

#include <QDialog>
//#include <QString>

namespace Ui {
class DialogAddSubordinate;
}

class DialogAddSubordinate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddSubordinate(QWidget *parent = 0);
    ~DialogAddSubordinate();

private:
    Ui::DialogAddSubordinate *ui;

public:
    QString surnameSubordinate();
    QString surnameBoss();
};

#endif // DIALOGADDSUBORDINATE_H

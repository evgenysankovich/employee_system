#include "workerbase.h"

WorkerBase::WorkerBase(QObject *parent) : QObject(parent)
{

}

WorkerBase::~WorkerBase()
{

}

/* Метод для нахождения времени работы сотрудника
 * */
int WorkerBase::timeWorkYear(QString hireDate, QString currentDate)
{    
    QDateTime hDate = QDateTime::fromString(hireDate,"dd.MM.yyyy");
    QDateTime cDate = QDateTime::fromString(currentDate,"dd.MM.yyyy");
    QDateTime tempDate;
    tempDate.setTime_t(cDate.toTime_t()-hDate.toTime_t());
    tempDate = tempDate.addYears(-1970);
    if (tempDate.toString("yy").toInt()) {
        return tempDate.toString("yy").toInt();
    } else {
        return 0;
    }
}

/* Метод для подсчета зарплаты без подчиненных
 * */
double WorkerBase::salaryWithoutSubordinate(QString baseSalary,int timeWork, double percentYear, int maxYear)
{
    if (timeWork > 0) {
        if (timeWork > maxYear) {
            timeWork = maxYear;
        }
        return baseSalary.toDouble() + baseSalary.toDouble() *timeWork * percentYear;
    } else {
        return baseSalary.toDouble();
    }
}


#include "workerbase.h"

WorkerBase::WorkerBase(QObject *parent) : QObject(parent)
{

}

WorkerBase::~WorkerBase()
{

}

int WorkerBase::timeWorkYear(QDateTime hireDate, QDateTime currentDate = QDateTime::currentDateTime())
{
    QDateTime tempDate;
    tempDate.setTime_t(currentDate.toTime_t()-hireDate.toTime_t());
    tempDate = tempDate.addYears(-1970);
    if (tempDate.toString("yy").toInt()) {
        return tempDate.toString("yy").toInt();
    } else {
        return 0;
    }
}


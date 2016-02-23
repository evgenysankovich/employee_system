#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <QObject>
#include <database.h>

class WorkerBase : public QObject
{
    Q_OBJECT
public:
    explicit WorkerBase(QObject *parent = 0);
    virtual ~WorkerBase();

signals:

public slots:

public:
    int timeWorkYear(QString hireDate, QString currentDate);
    double salaryWithoutSubordinate(QString baseSalary,int timeWork, double percentYear, int maxYear);
    virtual double getPercentYear()=0;
    virtual int getMaxYear()=0;
};

#endif // WORKERBASE_H

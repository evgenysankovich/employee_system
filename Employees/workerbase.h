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
    int timeWorkYear(QDateTime, QDateTime);
    virtual double salary() = 0;
    virtual double recursionSubordinate(double) = 0;

};

#endif // WORKERBASE_H

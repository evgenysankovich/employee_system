#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <workerbase.h>

class Employee: public WorkerBase {
private:
    double percentYear = 0.03;
    int maxYear = 10;

public:
    Employee();
    ~Employee();

    double getPercentYear();
    int getMaxYear();
};

#endif // EMPLOYEE_H

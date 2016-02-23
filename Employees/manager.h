#ifndef MANAGER_H
#define MANAGER_H

#include <workerbase.h>

class Manager: public WorkerBase {
private:
    const double percentYear = 0.05;
    const double percentSubordinate = 0.005;
    const int maxYear = 8;

public:
    Manager();
    ~Manager();

    double getPercentYear();
    double getPercentSubordinate();
    int getMaxYear();
};

#endif // MANAGER_H

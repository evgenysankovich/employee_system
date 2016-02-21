#ifndef MANAGER_H
#define MANAGER_H

#include <workerbase.h>

class Manager: public WorkerBase {
private:
    double percentYear = 0.05;
    double percentSubordinate = 0.005;
    int maxYear = 8;

public:
    Manager();
    ~Manager();

    double getPercentYear();
//    double getPercentSubordinate();
    int getMaxYear();
    double firstLevelSubordinate(QString id);
};

#endif // MANAGER_H

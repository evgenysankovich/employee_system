#ifndef SALES_H
#define SALES_H

#include <workerbase.h>

class Sales: public WorkerBase {
private:
    const double percentYear = 0.01;
    const double percentSubordinate = 0.003;
    const int maxYear = 35;

public:
    Sales();
    ~Sales();

    double getPercentYear();
    double getPercentSubordinate();
    int getMaxYear();
};

#endif // SALES_H

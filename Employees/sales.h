#ifndef SALES_H
#define SALES_H

#include <workerbase.h>

class Sales: public WorkerBase {
private:
    double percentYear = 0.01;
    double percentSubordinate = 0.003;
    int maxYear = 35;

public:
    Sales();
    ~Sales();

    double getPercentYear();
    double getPercentSubordinate();
    int getMaxYear();    
};

#endif // SALES_H

#include "manager.h"

Manager::Manager()
{

}

Manager::~Manager()
{

}

double Manager::getPercentYear()
{
    return percentYear;
}

//double Manager::getPercentSubordinate()
//{
//    return percentSubordinate;
//}

int Manager::getMaxYear()
{
    return maxYear;
}

double Manager::firstLevelSubordinate(QString &id)
{    
    QString sqlIdBoss = QString ("SELECT " EMPLOYEE_SALARY
                                 " FROM " EMPLOYEE
                                 " WHERE " EMPLOYEE_ID_BOSS " ='%1';")
            .arg(id);
    qDebug()<<sqlIdBoss;
    QSqlQuery query;
    if(!query.exec(sqlIdBoss)) {
        qDebug() << "dont find";
        qDebug() << query.lastError().text();
    }
    double salary = 0;
    QSqlRecord rec = query.record();
    while (query.next()) {
        salary += percentSubordinate * query.value(rec.indexOf(EMPLOYEE_SALARY)).toDouble();
    }
    return salary;
}

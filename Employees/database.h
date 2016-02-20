#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QSqlRecord>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "ExampleDataBase"
#define DATABASE_NAME       "employee.db"

#define EMPLOYEE                "EMPLOYEE"
#define EMPLOYEE_ID_BOSS        "ID_BOSS"
#define EMPLOYEE_SURNAME        "SURNAME"
#define EMPLOYEE_HIRE_DATE      "HIRE_DATE"
#define EMPLOYEE_BASE_SALARY    "BASE_SALARY"
#define EMPLOYEE_SALARY         "SALARY"
#define EMPLOYEE_TYPE           "TYPE"
#define EMPLOYEE_LOGIN          "LOGIN"
#define EMPLOYEE_PASSWORD       "PASSWORD"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();
    bool inserIntoEmployeeTable(const QVariantList &data);
    QString findData(QString, QString);

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createEmployeeTable();
};

#endif // DATABASE_H

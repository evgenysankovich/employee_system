#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

/* Методы для подключения к базе данных
 * */
void DataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile(DATABASE_NAME).exists()){
        qDebug()<<"Restore DB:";
        this->restoreDataBase();
    } else {
        qDebug()<<"Open DataBase:";
        this->openDataBase();
    }
}

/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createEmployeeTable()){
            qDebug()<<"restoreDataBase: false";
            return false;
        } else {
            qDebug()<<"restoreDataBase: true";
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        qDebug()<<"Cannot open database:" <<db.lastError();
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase()
{
    db.close();
}

/* Метод для создания таблицы устройств в базе данных
 * */
bool DataBase::createEmployeeTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " EMPLOYEE " ("
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            EMPLOYEE_ID_BOSS        " VARCHAR(255), "
                            EMPLOYEE_SURNAME        " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_HIRE_DATE      " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_BASE_SALARY    " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_SALARY         " VARCHAR(50), "
                            EMPLOYEE_TYPE           " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_LOGIN          " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_PASSWORD       " VARCHAR(50)     NOT NULL "
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << EMPLOYEE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

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
//                            EMPLOYEE_ID_BOSS        " VARCHAR(255)    NOT NULL, "
                            EMPLOYEE_SURNAME        " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_HIRE_DATE      " VARCHAR(50)     NOT NULL, "
                            EMPLOYEE_BASE_SALARY    " VARCHAR(50)     NOT NULL, "
//                            EMPLOYEE_SALARY         " VARCHAR(50)     NOT NULL, "
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

/* Метод для вставки записи в таблицу устройств
 * */
bool DataBase::inserIntoDeviceTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " EMPLOYEE " ( " EMPLOYEE_SURNAME ", "
                                              EMPLOYEE_HIRE_DATE ", "
                                              EMPLOYEE_BASE_SALARY", "
//                                              EMPLOYEE_SALARY", "
                                              EMPLOYEE_TYPE", "
                                              EMPLOYEE_LOGIN", "
                                              EMPLOYEE_PASSWORD " ) "
                  "VALUES (:SURNAME, :HIRE_DATE, :BASE_SALARY, :TYPE, :LOGIN, :PASSWORD )");
    query.bindValue(":SURNAME",         data[0].toString());
    query.bindValue(":HIRE_DATE",       data[1].toString());
    query.bindValue(":BASE_SALARY",     data[2].toString());
//    query.bindValue(":SALARY",          data[3].toString());    //Пересчет зарплаты
    query.bindValue(":TYPE",            data[3].toString());
    query.bindValue(":LOGIN",           data[4].toString());
    query.bindValue(":PASSWORD",        data[5].toString());
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << EMPLOYEE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

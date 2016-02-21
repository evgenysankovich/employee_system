#include "dialogaddemployee.h"
#include "ui_dialogaddemployee.h"

DialogAddEmployee::DialogAddEmployee(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEmployee)
{
    ui->setupUi(this);
//    this->setWindowTitle("Добавить работника");

    /* Метода для инициализации модели,
     * из которой будут транслироваться данные
     * */
    setupModel();

    /* Если строка не задана, то есть равна -1,
     * тогда диалог работает по принципу создания новой записи.
     * А именно, в модель вставляется новая строка и работа ведётся с ней.
     * */
    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
    /* В противном случае диалог настраивается на заданную запись
     * */
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }

    createUI();
}

DialogAddEmployee::~DialogAddEmployee()
{
    delete ui;
}

/* Метод настройки модели данных и mapper
 * */
void DialogAddEmployee::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable(EMPLOYEE);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Инициализируем mapper и привязываем
     * поля данных к объектам LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
//    mapper->addMapping("0");
    mapper->addMapping(ui->surnameLineEdit, 2);
    mapper->addMapping(ui->hireDateLineEdit, 3);
    mapper->addMapping(ui->baseSalaryLineEdit, 4);
    mapper->addMapping(ui->typeEmployeeComboBox, 6);
    mapper->addMapping(ui->loginLineEdit, 7);
    mapper->addMapping(ui->passwordLineEdit, 8);
    /* Ручное подтверждение изменения данных
     * через mapper
     * */

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    qDebug() << ui->surnameLineEdit->text();
    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
}

/* Метод для установки валидатора на поле ввода IP и MAC адресов
 * */
void DialogAddEmployee::createUI()
{
//    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
//    QRegExp ipRegex ("^" + ipRange
//                     + "\\." + ipRange
//                     + "\\." + ipRange
//                     + "\\." + ipRange + "$");
//    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
//    ui->IPAddressLineEdit->setValidator(ipValidator);

//    QString macRange = "(?:[0-9A-Fa-f][0-9A-Fa-f])";
//    QRegExp macRegex ("^" + macRange
//                      + "\\:" + macRange
//                      + "\\:" + macRange
//                      + "\\:" + macRange
//                      + "\\:" + macRange
//                      + "\\:" + macRange + "$");
//    QRegExpValidator *macValidator = new QRegExpValidator(macRegex, this);
//    ui->MACLineEdit->setValidator(macValidator);
}

void DialogAddEmployee::on_buttonBox_accepted()
{
    /* SQL-запрос для проверки существования записи
     * с такими же учетными данными.
     * Если запись не существует или находится лишь индекс
     * редактируемой в данный момент записи,
     * то диалог позволяет вставку записи в таблицу данных
     * */
    QSqlQuery query;
    QString str = QString("SELECT EXISTS (SELECT " EMPLOYEE_SURNAME " FROM " EMPLOYEE
                              " WHERE ( " EMPLOYEE_SURNAME " = '%1' )"                          
                              " AND id NOT LIKE '%2' )")
                .arg(ui->surnameLineEdit->text(),
                     model->data(model->index(mapper->currentIndex(),0), Qt::DisplayRole).toString());

    query.prepare(str);
    query.exec();
    query.next();

    /* Если запись существует, то диалог вызывает
     * предупредительное сообщение
     * */
    if(query.value(0) != 0){
        QMessageBox::information(this, "Ошибка ФИО", "Такой человек уже существует");

    /* В противном случае производится вставка новых данных в таблицу
     * и диалог завершается с передачей сигнала для обновления
     * таблицы в главном окне
     * */
    } else {
        QString sqlFindId = QString ("SELECT ID FROM " EMPLOYEE
                                     " WHERE " EMPLOYEE_SURNAME " ='%1'")
                .arg(ui->surnameLineEdit->text());
//        qDebug() << "find id: "<<sqlFindId;
        mapper->submit();
        model->submitAll();
        workerSalary(findData(sqlFindId,"ID"));
        insertBossId();
        emit signalReady();

        this->close();
    }
}

void DialogAddEmployee::accept()
{
    qDebug() << ui->surnameLineEdit->text();
}

/* Метод изменения состояния активности кнопок пролистывания
 * */
void DialogAddEmployee::updateButtons(int row)
{
    /* В том случае, если мы достигаем одного из крайних (самый первый или
     * самый последний) из индексов в таблице данных,
     * то мы изменяем состояние соответствующей кнопки на
     * состояние неактивна
     * */
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}

/* Подсчет зарплаты в зависимости от должности сотрудника
 * */
void DialogAddEmployee::workerSalary(QString id)
{
    double salary;
    QSqlQuery query;
    QString typeWorker = QString ("SELECT " EMPLOYEE_TYPE " FROM " EMPLOYEE
                                  " WHERE ID ='%1'")
            .arg(id);
    typeWorker = findData(typeWorker,EMPLOYEE_TYPE);
    if(typeWorker == "Employee") {
        salary = employeeSalary(id);
    } else if(typeWorker == "Manager") {
        salary = managerSalary(id);
    } else if(typeWorker == "Sales") {
        salary = salesSalary(id);
    }

    QString employeeSurname = QString ("SELECT " EMPLOYEE_SURNAME " FROM " EMPLOYEE
                                       " WHERE ID ='%1';")
            .arg(id);
    QString updateSalary = QString ("UPDATE " EMPLOYEE
                           " SET " EMPLOYEE_SALARY " ='%1'"
                           " WHERE " EMPLOYEE_SURNAME " ='%2'")
            .arg(QString::number(salary),findData(employeeSurname,EMPLOYEE_SURNAME));
    if(!query.exec(updateSalary)){
        qDebug() << "error insert into " << EMPLOYEE;
        qDebug() << query.lastError().text();
    }
    QString idBoss = QString ("SELECT " EMPLOYEE_ID_BOSS " FROM " EMPLOYEE
                              " WHERE ID ='%1';")
            .arg(id);
    int idBossInt = findData(idBoss,EMPLOYEE_ID_BOSS).toInt();
    if (idBossInt) {
        qDebug() <<"check";
        workerSalary(QString::number(idBossInt,10));
    }
}

/* Подсчет зарплаты для Employee
 * */
double DialogAddEmployee::employeeSalary(QString &id)
{
    Employee employee;
    QString baseSalary = QString ("SELECT " EMPLOYEE_BASE_SALARY " FROM " EMPLOYEE
                            " WHERE ID ='%1';")
            .arg(id);
    QString hireDate = QString ("SELECT " EMPLOYEE_HIRE_DATE " FROM " EMPLOYEE
                                " WHERE ID ='%1';")
                .arg(id);
    return employee.salaryWithoutSubordinate(findData(baseSalary,EMPLOYEE_BASE_SALARY),
                                             employee.timeWorkYear(findData(hireDate,EMPLOYEE_HIRE_DATE),QDateTime::currentDateTime().toString("dd.MM.yyyy")),
                                             employee.getPercentYear(),
                                             employee.getMaxYear());
}

/* Подсчет зарплаты для Manager
 * */
double DialogAddEmployee::managerSalary(QString &id)
{
    Manager manager;
    QString baseSalary = QString ("SELECT " EMPLOYEE_BASE_SALARY " FROM " EMPLOYEE
                            " WHERE ID ='%1';")
            .arg(id);
    QString hireDate = QString ("SELECT " EMPLOYEE_HIRE_DATE " FROM " EMPLOYEE
                                " WHERE ID ='%1';")
                .arg(id);

    double salary = manager.salaryWithoutSubordinate(findData(baseSalary,EMPLOYEE_BASE_SALARY),
                                                     manager.timeWorkYear(findData(hireDate,EMPLOYEE_HIRE_DATE),QDateTime::currentDateTime().toString("dd.MM.yyyy")),
                                                     manager.getPercentYear(),
                                                     manager.getMaxYear());
    salary += manager.firstLevelSubordinate(id);
    return salary;
}

/* Подсчет зарплаты для Salary
 * */
double DialogAddEmployee::salesSalary(QString &id)
{

}

/* Метод для нахождения и возвращения значения выбранного поля (column)
 * */
QString DialogAddEmployee::findData(QString sqlQuery, QString column)
{
    QSqlQuery query;
    if(!query.exec(sqlQuery)) {
        qDebug()<<"search failed";
        qDebug()<<query.lastError().text();
    }
    QSqlRecord rec = query.record();
    query.next();
    return query.value(rec.indexOf(column)).toString();
}

void DialogAddEmployee::insertBossId()
{
    QSqlQuery query;
    QString findIdFromName = QString ("SELECT ID FROM " EMPLOYEE
                                      " WHERE " EMPLOYEE_SURNAME " ='%1'")
            .arg(ui->bossLineEdit->text());
    QString updateIdBoss = QString ("UPDATE " EMPLOYEE
                           " SET " EMPLOYEE_ID_BOSS " ='%1'"
                           " WHERE " EMPLOYEE_SURNAME " ='%2'")
            .arg(findData(findIdFromName,"ID"),ui->surnameLineEdit->text());

    if(!query.exec(updateIdBoss)){
        qDebug() << "error insert into " << EMPLOYEE;
        qDebug() << query.lastError().text();
    }
}


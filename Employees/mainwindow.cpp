#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Тестовое задание");
    /* Первым делом необходимо создать объект для работы с базой данных
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();



    /* Инициализируем модели для представления данных
     * с заданием названий колонок
     * */
    this->setupModel(EMPLOYEE,
                     QStringList() << "id"
                     << "ID Начальника"
                     << "ФИО"
                     << "Дата приема"
                     << "Базовая ставка"
                     << "Зарплата"
                     << "Должность"
                     << "Имя пользователя"
                     << "Пароль"
                     );

    /* Инициализируем внешний вид таблицы с данными
         * */
    this->createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_5_clicked()
{
    this->close();
}

void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * */
    modelEmployee = new QSqlTableModel(this);
    modelEmployee->setTable(tableName);
    modelEmployee->select();
    modelEmployee->setEditStrategy(QSqlTableModel::OnFieldChange);
    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelEmployee->columnCount(); i++, j++){
        modelEmployee->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI()
{
    ui->EmployeeTableView->setModel(modelEmployee);     // Устанавливаем модель на TableView
    ui->EmployeeTableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->EmployeeTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    // Устанавливаем режим выделения лишь одной строки в таблице
    ui->EmployeeTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->EmployeeTableView->resizeColumnsToContents();
    ui->EmployeeTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->EmployeeTableView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->EmployeeTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord(QModelIndex)));
}

/* Метод для активации диалога добавления записей
 * */
void MainWindow::on_AddEmployeeButton_clicked()
{
    createUI();
    DialogAddEmployee *addEmployeeDialog = new DialogAddEmployee();
    connect(addEmployeeDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addEmployeeDialog->setWindowTitle(trUtf8("Добавить подчиненного"));
    addEmployeeDialog->exec();
//    addEmployeeDialog-
//    addEmployeeDialog->close();
}

/* Слот обновления модели представления данных
 * */
void MainWindow::slotUpdateModels()
{    
    modelEmployee->select();
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void MainWindow::slotEditRecord(QModelIndex index)
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    DialogAddEmployee *addEmployeeDialog = new DialogAddEmployee(index.row());
    connect(addEmployeeDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addEmployeeDialog->setWindowTitle(trUtf8("Редактировать подчиненного"));
    addEmployeeDialog->exec();
}

void MainWindow::on_ClearButton_clicked()
{
    ui->EmployeeTableView->setModel(NULL);     // Устанавливаем модель на TableView
}

void MainWindow::on_ShowEmployeeButton_clicked()
{
    createUI();
}

void MainWindow::insertBossId(QString surnameSubordinate, QString surnameBoss)
{
    QSqlQuery query;
    QString findIdBoss = QString ("SELECT ID FROM " EMPLOYEE
                                      " WHERE " EMPLOYEE_SURNAME " ='%1'")
            .arg(surnameBoss);
    QString updateSubordinateIdBoss = QString ("UPDATE " EMPLOYEE
                           " SET " EMPLOYEE_ID_BOSS " ='%1'"
                           " WHERE " EMPLOYEE_SURNAME " ='%2'")
            .arg(findData(findIdBoss,"ID"),surnameSubordinate);
    if(!query.exec(updateSubordinateIdBoss)){
        qDebug() << "error insert into " << EMPLOYEE;
        qDebug() << query.lastError().text();
    }
    slotUpdateModels();
}

/* Подсчет зарплаты в зависимости от должности сотрудника
 * */
void MainWindow::workerSalary(QString id)
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
double MainWindow::employeeSalary(QString &id)
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
double MainWindow::managerSalary(QString &id)
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
double MainWindow::salesSalary(QString &id)
{

}

/* Метод для нахождения и возвращения значения выбранного поля (column)
 * */
QString MainWindow::findData(QString sqlQuery, QString column)
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

void MainWindow::on_addSubordinateButton_clicked()
{
    QString s1,s2;
    DialogAddSubordinate *dialogAddSubordinate = new DialogAddSubordinate();
    dialogAddSubordinate->setWindowTitle("Добавить подчиненного");
    if (dialogAddSubordinate->exec() == QDialog::Accepted) {
        insertBossId(dialogAddSubordinate->surnameSubordinate(),dialogAddSubordinate->surnameBoss());
    }
    delete dialogAddSubordinate;
}

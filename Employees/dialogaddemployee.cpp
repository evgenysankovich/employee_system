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

/* Метод для установки валидатора на полях ввода
 * */
void DialogAddEmployee::createUI()
{
    QRegExp surnameRegex ("^[а-яА-ЯёЁa-zA-Z. -]+");
    QRegExpValidator *surVal = new QRegExpValidator(surnameRegex, this);
    ui->surnameLineEdit->setValidator(surVal);

    QRegExp baseSalaryRegex ("\\-?\\d+(\\.\\d{0,})?");
    QRegExpValidator *baseSalaryVal = new QRegExpValidator(baseSalaryRegex, this);
    ui->baseSalaryLineEdit->setValidator(baseSalaryVal);
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
        mapper->submit();
        model->submitAll();
        salary();
        emit signalReady();
        this->close();
    }
}

void DialogAddEmployee::accept()
{
    qDebug() << ui->surnameLineEdit->text();
}

void DialogAddEmployee::salary()
{
    double salary;
    QSqlQuery query;

    switch (ui->typeEmployeeComboBox->currentIndex()) {
    case 0:
    {
        Employee emp;
        salary = workerSalary(&emp);
    }
        break;
    case 1:
    {
        Manager manager;
        salary = workerSalary(&manager);
    }
        break;
    case 2:
    {
        Sales sales;
        salary = workerSalary(&sales);
    }
        break;
    default:
        break;
    }
    QString str = QString ("UPDATE " EMPLOYEE
                           " SET " EMPLOYEE_SALARY " ='%1'"
                                                   " WHERE " EMPLOYEE_SURNAME " ='%2'")
            .arg(QString::number(salary),ui->surnameLineEdit->text());
    if(!query.exec(str)){
        qDebug() << "error insert into " << EMPLOYEE;
        qDebug() << query.lastError().text();
    }
}

double DialogAddEmployee::workerSalary(WorkerBase *base)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    return base->salaryWithoutSubordinate(ui->baseSalaryLineEdit->text(),
                                          base->timeWorkYear(ui->hireDateLineEdit->text(),currentTime.toString("dd.MM.yyyy")),
                                          base->getPercentYear(),
                                          base->getMaxYear());
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






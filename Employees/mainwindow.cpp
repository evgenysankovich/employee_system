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
  //  ui->EmployeeTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->EmployeeTableView->horizontalHeader()->setStretchLastSection(true);

//    connect(ui->EmployeeTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord(QModelIndex)));
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
    //addEmployeeDialog->setWindowTitle(trUtf8("Добавить Устройство"));
    addEmployeeDialog->exec();
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
//    addDeviceEmployee->setWindowTitle(trUtf8("Редактировать Устройство"));
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

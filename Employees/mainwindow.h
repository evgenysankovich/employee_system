#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

#include <database.h>
#include <dialogaddemployee.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_5_clicked();
    void on_AddEmployeeButton_clicked();
    void slotUpdateModels();
    void slotEditRecord(QModelIndex index);

private:
    Ui::MainWindow              *ui;
    DataBase                    *db;
    QSqlTableModel              *modelEmployee;

private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};

#endif // MAINWINDOW_H

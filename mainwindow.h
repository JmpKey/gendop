#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>

#include <dialogyear.h>
#include <dialoggroup.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_year_triggered();
    void on_action_group_triggered();

    // user slots:
    void onOpenDialogYear();
    void receiveDataYear(const QString& year);
    void onOpenDialogGroup();
    void receiveDataGroup(const QString& year, const QString& group);



private:
    Ui::MainWindow *ui;
    QWidget* widget;
    QSqlQueryModel* model;
    QSqlDatabase db;
    bool flagd = false;

    void db_init();
    void show_model();
    void clear_model();
    void clear_all_elem_model();
    void show_message(const QString& text);
    void add_year_db_w(const QString& year);
    void add_group_db_w(const QString& year, const QString& group_name);

};
#endif // MAINWINDOW_H

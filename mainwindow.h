#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

// win zone
#include "dialogyear.h"
#include "dialoggroup.h"
#include "dialogteacher.h"

// module zone
#include "databasemanager.h"

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
    void on_action_teacher_triggered();

    // user slots:
    void onOpenDialogYear();
    void receiveDataYear(const QString& year);
    void onOpenDialogGroup();
    void receiveDataGroup(const QString& year, const QString& group);
    void onOpenDialogTeacher();
    void receiveDataTeacher(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr);

private:
    Ui::MainWindow *ui;
    QWidget* widget;
    QSqlQueryModel* model;
    DataBaseManager* db_manager;
    QSqlDatabase db;
    bool flagd = false;

    void db_init();
    void show_model();
    void clear_model();
    void show_message(const QString& text);

};
#endif // MAINWINDOW_H

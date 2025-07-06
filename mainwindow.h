#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <adaptation.h>

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
    void on_action_student_triggered();
    void on_action_del_obj_triggered();
    void on_action_comiss_triggered();
    void on_action_search_triggered();

    // user slots:
    void onOpenDialogYear();
    void receiveDataYear(const QString& year, const QString& years);
    void onOpenDialogGroup();
    void receiveDataGroup(const QString& year, const QString& group);
    void onOpenDialogTeacher();
    void receiveDataTeacher(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr, const QString& dolznost_rod);
    void onOpenDialogStudent();
    void receiveDataStudent(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment, const QString& nzakl, const QString& date_zakl);
    void receiveDataChoseGroup(const QString& id_c_group);
    void receiveDataChoseTeacher(const QString& id_c_teacher);
    void onOpenDialogComiss();
    void receiveDataChoseComiss(const QString& id_year, const QString& id_ruk, const QString& podl);
    void onOpenDialogSearch();
    void receiveDataSearch(const QString& search);

    void on_action_sr_math_triggered();

    void on_action_gendoc_triggered();

private:
    Ui::MainWindow *ui;
    QWidget* widget;
    QSqlQueryModel* model = nullptr;
    DataBaseManager* db_manager;
    QSqlDatabase db;
    bool flagd = false;

    void db_init();
    void show_model();
    void clear_model();
    void show_message(const QString& text);

};
#endif // MAINWINDOW_H

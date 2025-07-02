#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>
#include <QSqlQueryModel>

class DataBaseManager
{
private:
    QSqlDatabase& db;

    // tmp memory
    QString id_c_group;
    QString id_c_teacher;

public:
    DataBaseManager(QSqlDatabase& database);

    QSqlQueryModel* getStudentsModel();
    void add_year_db_w(const QString& year);
    void add_group_db_w(const QString& year, const QString& group_name);
    void add_teacher_db_w(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr);
    void add_student_db_w(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment);
    QStringList getLastGroups();
    QStringList getTeacher();
    QStringList getYear();
    void set_id_c_group(const QString& _id_c_group);
    void set_id_c_teacher(const QString& _id_c_teacher);
    void add_comiss_db_w(const QString& id_year, const QString& id_ruk, const QString& podl, const QString& com_chief, const QString& com_senior, const QString& com_teacher);
    QString findByDolznost(const QString& dolznost);
    void delete_student_db_w(const QString& id_student);
    QSqlQueryModel* searchStudents(const QString& searchString);
    QString calculateAverages();
};

#endif // DATABASEMANAGER_H

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>
#include <QSqlQueryModel>

class DataBaseManager
{
private:
    QSqlDatabase& db;

public:
    DataBaseManager(QSqlDatabase& database);

    QSqlQueryModel* getStudentsModel();
    void add_year_db_w(const QString& year);
    void add_group_db_w(const QString& year, const QString& group_name);
    void add_teacher_db_w(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr);

};

#endif // DATABASEMANAGER_H

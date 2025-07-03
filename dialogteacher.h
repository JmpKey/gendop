#ifndef DIALOGTEACHER_H
#define DIALOGTEACHER_H

#include <QDialog>

namespace Ui {
class DialogTeacher;
}

class DialogTeacher : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTeacher(const QString &dolzn1, const QString &dolzn2, const QString &dolzn3, QWidget *parent = nullptr);
    ~DialogTeacher();

private slots:
    void on_pb_teacher_clicked();

signals:
    void sendDataTeacher(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr);

private:
    Ui::DialogTeacher *ui;
};

#endif // DIALOGTEACHER_H

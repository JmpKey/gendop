#ifndef DIALOGSTUDENT_H
#define DIALOGSTUDENT_H

#include <QDialog>

namespace Ui {
class DialogStudent;
}

class DialogStudent : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStudent(QWidget* parent = nullptr);
    ~DialogStudent();

private slots:
    void on_pb_student_clicked();

signals:
    void sendDataStudent(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment, const QString& nzakl, const QString& date_zakl);

private:
    Ui::DialogStudent* ui;
};

#endif // DIALOGSTUDENT_H

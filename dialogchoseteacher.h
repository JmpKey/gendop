#ifndef DIALOGCHOSETEACHER_H
#define DIALOGCHOSETEACHER_H

#include <QDialog>

namespace Ui {
class DialogChoseTeacher;
}

class DialogChoseTeacher : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoseTeacher(const QStringList& teachers, QWidget* parent = nullptr);
    ~DialogChoseTeacher();

private slots:
    void on_pb_c_teacher_clicked();

signals:
    void sendDataChoseTeacher(const QString& id_c_teacher);

private:
    Ui::DialogChoseTeacher* ui;
};

#endif // DIALOGCHOSETEACHER_H

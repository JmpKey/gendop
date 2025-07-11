#include "dialogchoseteacher.h"
#include "ui_dialogchoseteacher.h"

DialogChoseTeacher::DialogChoseTeacher(const QStringList& teachers, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogChoseTeacher)
{
    ui->setupUi(this);

    connect(ui->pb_c_teacher, &QPushButton::clicked, this, &DialogChoseTeacher::on_pb_c_teacher_clicked);

    for (const QString& teacher : teachers) {
        ui->cb_teacher->addItem(teacher);
    }
}

DialogChoseTeacher::~DialogChoseTeacher()
{
    delete ui;
}

void DialogChoseTeacher::on_pb_c_teacher_clicked()
{
    QString selectedString = ui->cb_teacher->currentText();
    QString numberString = selectedString.section(' ', 0, 0);

    if (!numberString.isEmpty()) {
        qDebug() << "DialogChoseTeacher:" << numberString;
        emit sendDataChoseTeacher(numberString);
        accept();
    } else {
        qDebug() << "Error DialogChoseTeacher: The numeric part of the string could not be extracted.";
    }
}


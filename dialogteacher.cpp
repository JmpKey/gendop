#include "dialogteacher.h"
#include "ui_dialogteacher.h"

DialogTeacher::DialogTeacher(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogTeacher)
{
    ui->setupUi(this);

    connect(ui->pb_teacher, &QPushButton::clicked, this, &DialogTeacher::on_pb_teacher_clicked);
}

DialogTeacher::~DialogTeacher()
{
    delete ui;
}

void DialogTeacher::on_pb_teacher_clicked()
{
    if (ui->le_fio->text() != "" && ui->le_dolzn->text() != "" && ui->le_dolzn_min->text() != "" && ui->le_fio_pr->text() != "") {
        emit sendDataTeacher(ui->le_fio->text(), ui->le_dolzn->text(), ui->le_dolzn_min->text(), ui->le_fio_pr->text());
        accept();
    }
}


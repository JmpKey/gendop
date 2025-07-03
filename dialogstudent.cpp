#include "dialogstudent.h"
#include "ui_dialogstudent.h"

DialogStudent::DialogStudent(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogStudent)
{
    ui->setupUi(this);

    connect(ui->pb_student, &QPushButton::clicked, this, &DialogStudent::on_pb_student_clicked);
}

DialogStudent::~DialogStudent()
{
    delete ui;
}

void DialogStudent::on_pb_student_clicked()
{
    if (ui->le_fio->text() != "" && ui->le_fio_rod->text() != "" && ui->le_orig1->text() != "" && ui->le_orig2->text() != "" && ui->le_tema->text() != "" && ui->te_comment->toPlainText() != "" && ui->le_nzakl->text() != "" && ui->le_date_zakl->text() != "") {
        if(ui->le_orig1->text() == "-" || ui->le_orig2->text() != "-") {
            emit sendDataStudent(ui->le_fio->text(), ui->le_fio_rod->text(), "0", "0", ui->le_tema->text(), ui->cb_dopusk->currentText(), ui->te_comment->toPlainText(), ui->le_nzakl->text(), ui->le_date_zakl->text());
        }
        else {
            emit sendDataStudent(ui->le_fio->text(), ui->le_fio_rod->text(), ui->le_orig1->text(), ui->le_orig2->text(), ui->le_tema->text(), ui->cb_dopusk->currentText(), ui->te_comment->toPlainText(), ui->le_nzakl->text(), ui->le_date_zakl->text());
        }
        accept();
    }
}


#include "dialogstudent.h"
#include "ui_dialogstudent.h"

DialogStudent::DialogStudent(QWidget* parent)
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
    QString orig1, orig2 = "";
    if(ui->le_orig1->text() == "-") { orig1 = "0"; }
    else { orig1 = ui->le_orig1->text(); }
    if(ui->le_orig2->text() == "-") { orig2 = "0"; }
    else { orig2 = ui->le_orig2->text(); }

    if (ui->le_fio->text() != "" && ui->le_fio_rod->text() != "" && ui->le_orig1->text() != "" && ui->le_orig2->text() != "" && ui->le_tema->text() != "" && ui->te_comment->toPlainText() != "" && ui->le_nzakl->text() != "" && ui->le_date_zakl->text() != "") {
                emit sendDataStudent(ui->le_fio->text(), ui->le_fio_rod->text(), orig1, orig2, ui->le_tema->text(), ui->cb_dopusk->currentText(), ui->te_comment->toPlainText(), ui->le_nzakl->text(), ui->le_date_zakl->text());
        accept();
    }
}


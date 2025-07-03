#include "dialogteacher.h"
#include "ui_dialogteacher.h"

DialogTeacher::DialogTeacher(const QString &dolzn1, const QString &dolzn2, const QString &dolzn3, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogTeacher)
{
    ui->setupUi(this);

    connect(ui->pb_teacher, &QPushButton::clicked, this, &DialogTeacher::on_pb_teacher_clicked);

    ui->cb_dolzn->addItem(dolzn1);
    ui->cb_dolzn->addItem(dolzn2);
    ui->cb_dolzn->addItem(dolzn3);
}

DialogTeacher::~DialogTeacher()
{
    delete ui;
}

void DialogTeacher::on_pb_teacher_clicked()
{
    if (ui->le_fio->text() != "" && ui->cb_dolzn->currentText() != "" && ui->le_dolzn_min->text() != "" && ui->le_fio_pr->text() != "") {
        emit sendDataTeacher(ui->le_fio->text(), ui->cb_dolzn->currentText(), ui->le_dolzn_min->text(), ui->le_fio_pr->text());
        accept();
    }
}


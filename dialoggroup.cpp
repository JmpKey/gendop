#include "dialoggroup.h"
#include "ui_dialoggroup.h"

DialogGroup::DialogGroup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogGroup)
{
    ui->setupUi(this);

    connect(ui->pb_group, &QPushButton::clicked, this, &DialogGroup::on_pb_group_clicked);
}

DialogGroup::~DialogGroup()
{
    delete ui;
}

void DialogGroup::on_pb_group_clicked()
{
    if (ui->le_group->text() != "" && ui->le_group_year->text() != "") {
        bool ok;
        int year = ui->le_group_year->text().toInt(&ok);

        if (ok)
        {
            if(year >= 1990 && year <= 3000) {
                emit sendDataGroup(QString::number(year), ui->le_group->text());
                accept();
            }
        } else {
            qDebug() << "Error group";
            ui->le_group_year->clear();
            ui->le_group_year->setFocus();
        }
    }
}


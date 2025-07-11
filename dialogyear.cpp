#include "dialogyear.h"
#include "ui_dialogyear.h"

DialogYear::DialogYear(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogYear)
{
    ui->setupUi(this);

    connect(ui->pb_year, &QPushButton::clicked, this, &DialogYear::on_pb_year_clicked);
}

DialogYear::~DialogYear()
{
    delete ui;
}

void DialogYear::on_pb_year_clicked()
{
    if (ui->le_year->text() != "") {
        bool ok;
        int year = ui->le_year->text().toInt(&ok);

        if (ok)
        {
            if(year >= 1990 && year <= 3000) {
                emit sendDataYear(QString::number(year), ui->le_years->text());
                accept();
            }
        } else {
            qDebug() << "Error year";
            ui->le_year->clear();
            ui->le_year->setFocus();
        }
    }
}


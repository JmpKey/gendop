#include "dialogchosecomis.h"
#include "ui_dialogchosecomis.h"

DialogChoseComis::DialogChoseComis(const QStringList& years, const QStringList& teachers, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogChoseComis)
{
    ui->setupUi(this);

    connect(ui->pb_comis, &QPushButton::clicked, this, &DialogChoseComis::on_pb_comis_clicked);

    for (const QString& year : years) {
        ui->cb_year->addItem(year);
    }

    for (const QString& teacher : teachers) {
        ui->cb_ruk->addItem(teacher);
    }
}

DialogChoseComis::~DialogChoseComis()
{
    delete ui;
}

void DialogChoseComis::on_pb_comis_clicked()
{
    QString selectedStringY = ui->cb_year->currentText();
    QString numberStringY = selectedStringY.section(' ', 0, 0);

    QString selectedStringT = ui->cb_ruk->currentText();
    QString numberStringT = selectedStringT.section(' ', 0, 0);

    if (!numberStringY.isEmpty() && !numberStringT.isEmpty()) {
        if (ui->le_podl->text() != "") {
            qDebug() << "DialogChoseComis year:" << numberStringY;
            qDebug() << "DialogChoseComis teacher:" << numberStringT;
            emit sendDataChoseComis(numberStringY, numberStringT, ui->le_podl->text());
            accept();
        }
    } else {
        qDebug() << "Error DialogChoseComis: The numeric part of the string could not be extracted.";
    }
}


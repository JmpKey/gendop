#include "dialogchosegroup.h"
#include "ui_dialogchosegroup.h"

DialogChoseGroup::DialogChoseGroup(const QStringList& groups, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogChoseGroup)
{
    ui->setupUi(this);

    connect(ui->pb_c_group, &QPushButton::clicked, this, &DialogChoseGroup::on_pb_c_group_clicked);

    for (const QString& group : groups) {
        ui->cb_group->addItem(group);
    }
}

DialogChoseGroup::~DialogChoseGroup()
{
    delete ui;
}

void DialogChoseGroup::on_pb_c_group_clicked()
{
    QString selectedString = ui->cb_group->currentText();
    QString numberString = selectedString.section(' ', 0, 0);

    if (!numberString.isEmpty()) {
        qDebug() << "DialogChoseGroup:" << numberString;
        emit sendDataChoseGroup(numberString);
        accept();
    } else {
        qDebug() << "Error DialogChoseGroup: The numeric part of the string could not be extracted.";
    }
}


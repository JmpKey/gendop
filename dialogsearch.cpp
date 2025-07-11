#include "dialogsearch.h"
#include "ui_dialogsearch.h"

DialogSearch::DialogSearch(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogSearch)
{
    ui->setupUi(this);

    connect(ui->pb_search, &QPushButton::clicked, this, &DialogSearch::on_pb_search_clicked);
}

DialogSearch::~DialogSearch()
{
    delete ui;
}

void DialogSearch::on_pb_search_clicked()
{
    if (ui->le_search->text() != "") {
        emit sendDataSearch(ui->le_search->text());
        accept();
    }
}


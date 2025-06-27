#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Code from Windows 2x
    widget = new QWidget();
    widget->setLayout(ui->gridLayout);
    setCentralWidget(widget);

    connect(ui->action_year, &QAction::triggered, this, &MainWindow::onOpenDialogYear);
    connect(ui->action_group, &QAction::triggered, this, &MainWindow::onOpenDialogGroup);
    connect(ui->action_teacher, &QAction::triggered, this, &MainWindow::onOpenDialogTeacher);

    db_init();
}

MainWindow::~MainWindow()
{
    if (db.isOpen()) { db.close(); }

    if (db_manager) { delete db_manager; db_manager = nullptr; }

    if (model) { delete model; model = nullptr; }

    if (widget) { delete widget; widget = nullptr; }

    if (ui) { delete ui; ui = nullptr; }

}

void MainWindow::db_init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString pathDB = QDir::currentPath() + QString("/db/db");
    db.setDatabaseName(pathDB);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Error db", "Failed to open database:\n" + db.lastError().text(), QMessageBox::Ok);
        return;
    }

    db_manager = new DataBaseManager(db);
    show_model();
}

void MainWindow::show_model()
{
    clear_model();
    model = db_manager->getStudentsModel();
    if (model == nullptr) {
        QMessageBox::critical(this, "Ошибка", "Ошибка получения данных из базы данных", QMessageBox::Ok);
        return;
    }

    model->setHeaderData(1, Qt::Horizontal, "ФИО студента");
    model->setHeaderData(2, Qt::Horizontal, "Группа");
    model->setHeaderData(3, Qt::Horizontal, "Год");
    model->setHeaderData(4, Qt::Horizontal, "ФИО преподавателя");
    model->setHeaderData(5, Qt::Horizontal, "Оригинальность 1");
    model->setHeaderData(6, Qt::Horizontal, "Оригинальность 2");
    model->setHeaderData(7, Qt::Horizontal, "Тема");
    model->setHeaderData(8, Qt::Horizontal, "Допуск");
    model->setHeaderData(9, Qt::Horizontal, "Комментарий");

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

}

void MainWindow::clear_model() {
    if (model != nullptr) {
        ui->tableView->setModel(nullptr);
        delete model;
        model = nullptr;
    }
}


//QSqlDatabase MainWindow::getDatabase() const { return db; }

void MainWindow::on_action_year_triggered() {}

void MainWindow::onOpenDialogYear() {
    DialogYear win_year(this);
    connect(&win_year, &DialogYear::sendDataYear, this, &MainWindow::receiveDataYear);
    win_year.exec();
    flagd = false;
}

void MainWindow::receiveDataYear(const QString& year)
{
    if(!flagd) { flagd = true; return; }
    db_manager->add_year_db_w(year);
}

void MainWindow::show_message(const QString& text) {
    QMessageBox msg;
    msg.setText(text);
    msg.exec();
}

void MainWindow::on_action_group_triggered() {}

void MainWindow::onOpenDialogGroup() {
    DialogGroup win_group(this);
    connect(&win_group, &DialogGroup::sendDataGroup, this, &MainWindow::receiveDataGroup);
    win_group.exec();
    flagd = false;
}

void MainWindow::receiveDataGroup(const QString& year, const QString& group)
{
    if(!flagd) { flagd = true; return; }
    db_manager->add_group_db_w(year, group);
}

void MainWindow::on_action_teacher_triggered() {}

void MainWindow::onOpenDialogTeacher() {
    DialogTeacher win_teacher(this);
    connect(&win_teacher, &DialogTeacher::sendDataTeacher, this, &MainWindow::receiveDataTeacher);
    win_teacher.exec();
    flagd = false;
}

void MainWindow::receiveDataTeacher(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr)
{
    if(!flagd) { flagd = true; return; }
    db_manager->add_teacher_db_w(fio_t, dolznost_t, dolznost_ts, fio_tpr);
}

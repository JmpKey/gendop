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

    db_init();
}

MainWindow::~MainWindow()
{
    db.close();
    delete widget;
    delete ui;
}

void MainWindow::db_init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString pathDB = QDir::currentPath() + QString("/db/db");
    db.setDatabaseName(pathDB);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Error db", "Failed to open database:\n" + db.lastError().text(), QMessageBox::Ok);
    }

    show_model();
}

void MainWindow::show_model()
{
    model = new QSqlQueryModel();
    model->setQuery(
        "SELECT "
        "  s.id_student, "
        "  s.fio AS ФИО_студента, "
        "  lg.group_name AS Группа, "
        "  y.year AS Год, "
        "  t.fio AS ФИО_преподавателя, "
        "  s.origin1, "
        "  s.origin2, "
        "  s.tema, "
        "  s.dopusk, "
        "  s.comment "
        "FROM students s "
        "INNER JOIN last_groups lg ON s.students_id_last_group = lg.id_last_group "
        "INNER JOIN year y ON lg.group_year = y.id_year "
        "INNER JOIN teachers t ON s.students_id_prep = t.id_prep;");

    if (model->lastError().isValid()) {
        QMessageBox::critical(nullptr, "Error", "Request execution error:\n" + model->lastError().text(), QMessageBox::Ok);
        db.close();
    }

    // Устанавливаем заголовки
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

void MainWindow::clear_all_elem_model() { ui->tableView->model()->removeRows(0, ui->tableView->model()->rowCount()); model->clear(); }

void MainWindow::clear_model() {
    if(model != nullptr) {
        clear_all_elem_model();
        delete model;
        model = nullptr;
        ui->tableView->setModel(nullptr);
    }
}

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
    add_year_db_w(year);
}

void MainWindow::show_message(const QString& text) {
    QMessageBox msg;
    msg.setText(text);
    msg.exec();
}

void MainWindow::add_year_db_w(const QString& year) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    db.transaction();

    if (!query.exec("SELECT MAX(id_year) FROM year")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();
        return;
    }

    int nextId = 0;
    if (query.next()) {
        nextId = query.value(0).toInt() + 1;
    }

    query.prepare("INSERT INTO year (id_year, year) VALUES (:id_year, :year)");
    query.bindValue(":id_year", nextId);
    query.bindValue(":year", year);

    if (!query.exec()) {
        qDebug() << "Error inserting year:" << year << query.lastError().text();
        db.rollback();
        return;
    }

    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback();
        return;
    }
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
    add_group_db_w(year, group);
}

void MainWindow::add_group_db_w(const QString& year, const QString& group_name) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    db.transaction();

    query.prepare("SELECT id_year FROM year WHERE year = :year");
    query.bindValue(":year", year);

    if (!query.exec()) {
        qDebug() << "Error getting id_year from year table:" << query.lastError().text();
        db.rollback();
        return;
    }

    int yearId = -1;
    if (query.next()) {
        yearId = query.value(0).toInt();
    } else {
        qDebug() << "Year not found in year table:" << year;
        db.rollback();
        return;
    }

    if (!query.exec("SELECT MAX(id_last_group) FROM last_groups")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();
        return;
    }

    int nextId = 0;
    if (query.next()) {
        nextId = query.value(0).toInt() + 1;
    }

    query.prepare("INSERT INTO last_groups (id_last_group, group_name, group_year) VALUES (:id_last_group, :group_name, :group_year)");
    query.bindValue(":id_last_group", nextId);
    query.bindValue(":group_name", group_name);
    query.bindValue(":group_year", yearId);

    if (!query.exec()) {
        qDebug() << "Error inserting into last_groups:" << query.lastError().text();
        db.rollback();
        return;
    }

    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback();
        return;
    }

    qDebug() << "Successfully added group:" << group_name << "with year:" << year;
}

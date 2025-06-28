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
    connect(ui->action_student, &QAction::triggered, this, &MainWindow::onOpenDialogStudent);

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

void MainWindow::on_action_student_triggered() {}

void MainWindow::onOpenDialogStudent() {
    // тут должно вызываться окно с опросом выбранной группы
    DialogChoseGroup win_chose_g(db_manager->getLastGroups(), this);
    connect(&win_chose_g, &DialogChoseGroup::sendDataChoseGroup, this, &MainWindow::receiveDataChoseGroup);
    win_chose_g.exec();

    DialogChoseTeacher win_chose_t(db_manager->getTeacher(), this);
    connect(&win_chose_t, &DialogChoseTeacher::sendDataChoseTeacher, this, &MainWindow::receiveDataChoseTeacher);
    win_chose_t.exec();

    DialogStudent win_student(this);
    connect(&win_student, &DialogStudent::sendDataStudent, this, &MainWindow::receiveDataStudent);
    win_student.exec();
    flagd = false;
}

void MainWindow::receiveDataChoseGroup(const QString& id_c_group) {
    // тут id выбранной группы передаётся в db_manager для связывания
    db_manager->set_id_c_group(id_c_group);
}

void MainWindow::receiveDataChoseTeacher(const QString& id_c_teacher) {
    // тут id выбранного студента передаётся в db_manager для связывания
    db_manager->set_id_c_teacher(id_c_teacher);
}

void MainWindow::receiveDataStudent(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment)
{
    if(!flagd) { flagd = true; return; }


    //db_manager->add_teacher_db_w(fio_t, dolznost_t, dolznost_ts, fio_tpr);
}


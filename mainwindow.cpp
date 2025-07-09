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
    connect(ui->action_comiss, &QAction::triggered, this, &MainWindow::onOpenDialogComiss);
    connect(ui->action_search, &QAction::triggered, this, &MainWindow::onOpenDialogSearch);

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

    model->setHeaderData(0, Qt::Horizontal, "id");
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

void MainWindow::receiveDataYear(const QString& year, const QString& years)
{
    if(!flagd) { flagd = true; return; }
    db_manager->add_year_db_w(year, years);
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
    FConfigManager configReader(QDir::currentPath() + QString("/conf/teachers.conf"));
    QVector<QString> configValues = configReader.readConfig();

    DialogTeacher win_teacher(configValues[0], configValues[1], configValues[2], this);
    connect(&win_teacher, &DialogTeacher::sendDataTeacher, this, &MainWindow::receiveDataTeacher);
    win_teacher.exec();
    flagd = false;
}

void MainWindow::receiveDataTeacher(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr, const QString& dolznost_rod)
{
    if(!flagd) { flagd = true; return; }
    QString curDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    db_manager->add_teacher_db_w(fio_t, dolznost_t, dolznost_ts, fio_tpr, curDate, dolznost_rod);
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

void MainWindow::receiveDataStudent(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment, const QString& nzakl, const QString& date_zakl)
{
    if(!flagd) { flagd = true; return; }
    db_manager->add_student_db_w(fio_s, fio_rod, orig_1, orig_2, tema, dopusk, comment, nzakl, date_zakl);
    show_model();
}

void MainWindow::on_action_del_obj_triggered()
{
    // Получаем текущий индекс выбранной ячейки
    QModelIndex index = ui->tableView->currentIndex();

    // Проверяем, что индекс действителен
    if (!index.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите строку для удаления.");
        return;
    }

    // Запрашиваем подтверждение удаления
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение удаления", "Вы уверены, что хотите удалить выбранный объект?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Получаем значение id из первой колонки (или другой колонки, если необходимо)
        QVariant idValue = model->data(model->index(index.row(), 0));

        // Преобразуем QVariant в QString
        QString id = idValue.toString();

        // Выводим ID в отладочный вывод
        qDebug() << "See ID:" << id;

        db_manager->delete_student_db_w(id);

        show_model();
    }
}

void MainWindow::on_action_comiss_triggered() {}

void MainWindow::onOpenDialogComiss() {
    DialogChoseComis win_chose_c(db_manager->getYear(), db_manager->getTeacher(), this);
    connect(&win_chose_c, &DialogChoseComis::sendDataChoseComis, this, &MainWindow::receiveDataChoseComiss);
    win_chose_c.exec();
    flagd = false;
}

void MainWindow::receiveDataChoseComiss(const QString& id_year, const QString& id_ruk, const QString& podl) {
    if(!flagd) { flagd = true; return; }

    FConfigManager configReader(QDir::currentPath() + QString("/conf/teachers.conf"));
    QVector<QString> configValues = configReader.readConfig();
    db_manager->add_comiss_db_w(id_year, id_ruk, podl, configValues[0], configValues[1], configValues[2]);
}

void MainWindow::on_action_search_triggered() {}

void MainWindow::onOpenDialogSearch() {
    DialogSearch win_search(this);
    connect(&win_search, &DialogSearch::sendDataSearch, this, &MainWindow::receiveDataSearch);
    win_search.exec();
    flagd = false;
}

void MainWindow::receiveDataSearch(const QString& search)
{
    if(!flagd) { flagd = true; return; }
    clear_model();
    model = db_manager->searchStudents(search);
    if (model == nullptr) {
        QMessageBox::critical(this, "Ошибка", "Ошибка получения данных из базы данных", QMessageBox::Ok);
        return;
    }

    model->setHeaderData(0, Qt::Horizontal, "id");
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
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}


void MainWindow::on_action_sr_math_triggered()
{
    show_message(db_manager->calculateAverages());
}


void MainWindow::on_action_gendoc_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();

    // Проверяем, что индекс действителен
    if (!index.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите строку для генерации.");
        return;
    }

    QVariant idValue = model->data(model->index(index.row(), 0));

    // Преобразуем QVariant в QString
    QString id = idValue.toString();

    // Выводим ID в отладочный вывод
    qDebug() << "See ID:" << id;

    DocxManager microd;
    microd.unZipDocxWrite((db_manager->getStudentEdit(db_manager->getStudentData(id))));
}


void MainWindow::on_action_update_triggered()
{
    show_model();
}


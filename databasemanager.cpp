#include "databasemanager.h"

DataBaseManager::DataBaseManager(QSqlDatabase& database) : db(database) {}

QSqlQueryModel* DataBaseManager::getStudentsModel() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(
        "SELECT "
        " s.id_student, "
        " s.fio AS ФИО_студента, "
        " lg.group_name AS Группа, "
        " y.year AS Год, "
        " t.fio AS ФИО_преподавателя, "
        " s.origin1, "
        " s.origin2, "
        " s.tema, "
        " s.dopusk, "
        " s.comment "
        "FROM students s "
        "INNER JOIN last_groups lg ON s.students_id_last_group = lg.id_last_group "
        "INNER JOIN year y ON lg.group_year = y.id_year "
        "INNER JOIN teachers t ON s.students_id_prep = t.id_prep;", db); // db передаётся в запрос

    if (model->lastError().isValid()) {
        // Обработка ошибки - лучше выбросить исключение или вернуть nullptr, а не закрывать базу данных здесь
        qWarning() << "Database error:" << model->lastError().text();
        delete model; // Удаляем модель, если произошла ошибка
        return nullptr; // Возвращаем nullptr, указывающий на ошибку
    }

    // Установка заголовков - лучше сделать это в MainWindow, для большей гибкости
    return model;
}

void DataBaseManager::add_year_db_w(const QString& year, const QString& years) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    // 1. Начинаем транзакцию
    db.transaction();

    // 2. Получаем максимальный ID из таблицы.
    if (!query.exec("SELECT MAX(id_year) FROM year")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    int nextId = 0; // По умолчанию ID будет 0, если таблица пуста
    if (query.next()) {
        // Получаем максимальный ID из результата запроса
        nextId = query.value(0).toInt() + 1; // Увеличиваем на 1 для следующего ID
    }

    // 3. Вставляем новую запись с явно указанным ID.
    query.prepare("INSERT INTO year (id_year, year, years) VALUES (:id_year, :year, :years)");
    query.bindValue(":id_year", nextId);
    query.bindValue(":year", year);
    query.bindValue(":years", years);

    if (!query.exec()) {
        qDebug() << "Error inserting year:" << year << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    // 4. Подтверждаем транзакцию
    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback(); // Откатываем, если commit не удался (очень редко)
        return;
    }
}

void DataBaseManager::add_group_db_w(const QString& year, const QString& group_name) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    // 1. Начинаем транзакцию
    db.transaction();

    // 2. Получаем id_year для данного year из таблицы year.
    query.prepare("SELECT id_year FROM year WHERE year = :year");
    query.bindValue(":year", year);

    if (!query.exec()) {
        qDebug() << "Error getting id_year from year table:" << query.lastError().text();
        db.rollback();
        return;
    }

    int yearId = -1; // Значение по умолчанию, если год не найден
    if (query.next()) {
        yearId = query.value(0).toInt();
    } else {
        qDebug() << "Year not found in year table:" << year;
        db.rollback();
        return; // Год не найден, откатываем транзакцию и выходим
    }

    // 3. Получаем максимальный ID из таблицы.
    if (!query.exec("SELECT MAX(id_last_group) FROM last_groups")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    int nextId = 0; // По умолчанию ID будет 0, если таблица пуста
    if (query.next()) {
        // Получаем максимальный ID из результата запроса
        nextId = query.value(0).toInt() + 1; // Увеличиваем на 1 для следующего ID
    }

    // 4. Если id_year получен успешно, добавляем запись в last_groups.
    query.prepare("INSERT INTO last_groups (id_last_group, group_name, group_year) VALUES (:id_last_group, :group_name, :group_year)");
    query.bindValue(":id_last_group", nextId);
    query.bindValue(":group_name", group_name);
    query.bindValue(":group_year", yearId); // Используем полученный id_year

    if (!query.exec()) {
        qDebug() << "Error inserting into last_groups:" << query.lastError().text();
        db.rollback();
        return;
    }

    // 5. Подтверждаем транзакцию
    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback();
        return;
    }

    qDebug() << "Successfully added group:" << group_name << "with year:" << year;
}

void DataBaseManager::add_teacher_db_w(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr, QString curDate, const QString& dolznost_rod) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    // 1. Начинаем транзакцию
    db.transaction();

    // 2. Получаем максимальный ID из таблицы.
    if (!query.exec("SELECT MAX(id_prep) FROM teachers")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    int nextId = 0; // По умолчанию ID будет 0, если таблица пуста
    if (query.next()) {
        // Получаем максимальный ID из результата запроса
        nextId = query.value(0).toInt() + 1; // Увеличиваем на 1 для следующего ID
    }

    // 3. Если id_year получен успешно, добавляем запись в last_groups.
    // INSERT INTO teachers (id_prep, fio, dolznost, dolznost_sokr, fio_dolz_pri) VALUES (:id_prep, :fio, :dolznost, :dolznost_sokr, :fio_dolz_pri)
    query.prepare("INSERT INTO teachers (id_prep, fio, dolznost, dolznost_sokr, fio_dolz_pri, date, dolznost_rod) VALUES (:id_prep, :fio, :dolznost, :dolznost_sokr, :fio_dolz_pri, :date, :dolznost_rod)");
    query.bindValue(":id_prep", nextId);
    query.bindValue(":fio", fio_t);
    query.bindValue(":dolznost", dolznost_t); // Используем полученный id_year
    query.bindValue(":dolznost_sokr", dolznost_ts);
    query.bindValue(":fio_dolz_pri", fio_tpr);
    query.bindValue(":date", curDate);
    query.bindValue(":dolznost_rod", dolznost_rod);

    if (!query.exec()) {
        qDebug() << "Error inserting into teachers:" << query.lastError().text();
        db.rollback();
        return;
    }

    // 4. Подтверждаем транзакцию
    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback();
        return;
    }

    qDebug() << "Successfully added teacher:" << fio_t;
}

QStringList DataBaseManager::getLastGroups() {
    QStringList result;

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return result;
    }

    QSqlQuery query("SELECT id_last_group, group_name FROM last_groups");

    if (!query.exec()) {
        qWarning() << "Query execution failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        result << QString("%1 %2").arg(id).arg(name);
    }

    return result;
}

QStringList DataBaseManager::getTeacher() {
    QStringList result;

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return result;
    }

    QSqlQuery query("SELECT id_prep, fio FROM teachers");

    if (!query.exec()) {
        qWarning() << "Query execution failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        result << QString("%1 %2").arg(id).arg(name);
    }

    return result;
}

void DataBaseManager::set_id_c_group(const QString& _id_c_group) { id_c_group = _id_c_group; }

void DataBaseManager::set_id_c_teacher(const QString& _id_c_teacher) { id_c_teacher = _id_c_teacher; }

void DataBaseManager::add_student_db_w(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment, const QString& nzakl, const QString& date_zakl)
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    // 1. Начинаем транзакцию
    db.transaction();

    // 2. Получаем максимальный ID из таблицы.
    if (!query.exec("SELECT MAX(id_student) FROM students")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    int nextId = 0; // По умолчанию ID будет 0, если таблица пуста
    if (query.next()) {
        // Получаем максимальный ID из результата запроса
        nextId = query.value(0).toInt() + 1; // Увеличиваем на 1 для следующего ID
    }

    // 3. Если id_year получен успешно, добавляем запись в last_groups.
    // INSERT INTO teachers (id_prep, fio, dolznost, dolznost_sokr, fio_dolz_pri) VALUES (:id_prep, :fio, :dolznost, :dolznost_sokr, :fio_dolz_pri)
    query.prepare("INSERT INTO students (id_student, fio, fio_rod, origin1, origin2, tema, dopusk, comment, students_id_last_group, students_id_prep, nzakl, date_zakl) VALUES (:id_student, :fio, :fio_rod, :origin1, :origin2, :tema, :dopusk, :comment, :students_id_last_group, :students_id_prep, :nzakl, :date_zakl)");
    query.bindValue(":id_student", nextId);
    query.bindValue(":fio", fio_s);
    query.bindValue(":fio_rod", fio_rod);
    query.bindValue(":origin1", orig_1);
    query.bindValue(":origin2", orig_2);
    query.bindValue(":tema", tema);
    query.bindValue(":dopusk", dopusk);
    query.bindValue(":comment", comment);
    query.bindValue(":students_id_last_group", id_c_group);
    query.bindValue(":students_id_prep", id_c_teacher);
    query.bindValue(":nzakl", nzakl);
    query.bindValue(":date_zakl", date_zakl);

    if (!query.exec()) {
        qDebug() << "Error inserting into student:" << query.lastError().text();
        db.rollback();
        return;
    }

    // 4. Подтверждаем транзакцию
    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback();
        return;
    }

    qDebug() << "Successfully added student:" << fio_s;
}

QStringList DataBaseManager::getYear() {
    QStringList result;

    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return result;
    }

    QSqlQuery query("SELECT id_year, year FROM year");

    if (!query.exec()) {
        qWarning() << "Query execution failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        result << QString("%1 %2").arg(id).arg(name);
    }

    return result;
}

void DataBaseManager::add_comiss_db_w(const QString& id_year, const QString& id_ruk, const QString& podl, const QString& com_chief, const QString& com_senior, const QString& com_teacher) {
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db);

    // 1. Начинаем транзакцию
    db.transaction();

    // тут год и руководитель с остальными должны вызваться
    QString fio_ruk = "";
    QString dolznost_ruk = "";

    // Подготовка SQL-запроса
    query.prepare("SELECT fio, dolznost FROM teachers WHERE id_prep = :id");
    query.bindValue(":id", id_ruk);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }

    // Получение результата
    if (query.next()) {
        fio_ruk = query.value(0).toString(); // Получаем значение поля fio
        dolznost_ruk = query.value(1).toString(); // Получаем значение поля dolznost
    } else {
        qDebug() << "Запись не найдена для id:" << id_ruk;
    }

    qDebug() << "Запись dolznost_ruk:" << dolznost_ruk;

    QString com1; // первый член комисси
    QString com2; // второй член комисси

    if(dolznost_ruk == com_chief) { com1 = findByDolznost(com_teacher); com2 = findByDolznost(com_senior); }
    if(dolznost_ruk == com_senior) { com1 = findByDolznost(com_chief); com2 = findByDolznost(com_teacher); }
    if(dolznost_ruk == com_teacher) { com1 = findByDolznost(com_chief); com2 = findByDolznost(com_senior); }
    else {}

    // тут когда мы собрали всех тороих можно записывать по порядку в БД: com1 com2 и руководитель

    // 2. Получаем максимальный ID из таблицы.
    if (!query.exec("SELECT MAX(id_comiss) FROM comission")) {
        qDebug() << "Error getting max id:" << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    int nextId = 0; // По умолчанию ID будет 0, если таблица пуста
    if (query.next()) {
        // Получаем максимальный ID из результата запроса
        nextId = query.value(0).toInt() + 1; // Увеличиваем на 1 для следующего ID
    }

    // 3. Вставляем новую запись с явно указанным ID.
    query.prepare("INSERT INTO comission (id_comiss, predsedatel, post_g_comiss, vari_g_comissi, podl_g_comiss, comission_id_year) VALUES (:id_comiss, :predsedatel, :post_g_comiss, :vari_g_comissi, :podl_g_comiss, :comission_id_year)");
    query.bindValue(":id_comiss", nextId);
    query.bindValue(":predsedatel", com1);
    query.bindValue(":post_g_comiss", com2);
    query.bindValue(":vari_g_comissi", fio_ruk);
    query.bindValue(":podl_g_comiss", podl);
    query.bindValue(":comission_id_year", id_year);

    if (!query.exec()) {
        qDebug() << "Error inserting comission:" << nextId << query.lastError().text();
        db.rollback();  // Откатываем транзакцию в случае ошибки
        return;
    }

    // 4. Подтверждаем транзакцию
    if (!db.commit()) {
        qDebug() << "Error committing transaction:" << db.lastError().text();
        db.rollback(); // Откатываем, если commit не удался (очень редко)
        return;
    }
}

QString DataBaseManager::findByDolznost(const QString& dolznost) {
    // в этот метод из условия перенаправляем нужную должность. И в условии получаем
    QString fio;

    QSqlQuery query(db);

    // Используем параметризованный запрос для предотвращения SQL-инъекций
    query.prepare("SELECT fio FROM teachers WHERE dolznost = :dolznost");
    query.bindValue(":dolznost", dolznost);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return fio; // Возвращаем пустую строку в случае ошибки
    }

    while (query.next()) {
        fio = query.value(0).toString(); // Получаем ФИО
    }    

    return fio;
}

void DataBaseManager::delete_student_db_w(const QString& id_student) {
    if (!db.isOpen()) {
        qDebug() << "Ошибка: База данных не открыта!";
        return;
    }

    // Начинаем транзакцию
    if (!db.transaction()) {
        qDebug() << "Ошибка при начале транзакции:" << db.lastError().text();
        return;
    }

    // Создаем запрос
    QSqlQuery query;
    query.prepare("DELETE FROM students WHERE id_student = :id_student");
    query.bindValue(":id_student", id_student); // Привязываем значение параметра

    // Выполняем запрос
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении студента:" << query.lastError().text();
        // Откатываем транзакцию в случае ошибки
        db.rollback();
        return;
    }

    // Подтверждаем транзакцию
    if (!db.commit()) {
        qDebug() << "Ошибка при подтверждении транзакции:" << db.lastError().text();
        return;
    }

    qDebug() << "Студент с id" << id_student << "успешно удален.";
}

QSqlQueryModel* DataBaseManager::searchStudents(const QString& searchString) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Подготовка SQL-запроса с использованием LIKE для поиска
    QString queryStr = QString(
        "SELECT "
        " s.id_student, "
        " s.fio AS ФИО_студента, "
        " lg.group_name AS Группа, "
        " y.year AS Год, "
        " t.fio AS ФИО_преподавателя, "
        " s.origin1, "
        " s.origin2, "
        " s.tema, "
        " s.dopusk, "
        " s.comment "
        "FROM students s "
        "INNER JOIN last_groups lg ON s.students_id_last_group = lg.id_last_group "
        "INNER JOIN year y ON lg.group_year = y.id_year "
        "INNER JOIN teachers t ON s.students_id_prep = t.id_prep "
        "WHERE s.fio LIKE :searchString OR lg.group_name LIKE :searchString OR t.fio LIKE :searchString"
        );

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":searchString", "%" + searchString + "%"); // Используем подстановочный знак для поиска

    // Выполнение запроса
    if (!query.exec()) {
        qWarning() << "Ошибка выполнения запроса:" << query.lastError().text();
        delete model; // Удаляем модель в случае ошибки
        return nullptr; // Возвращаем nullptr, указывающий на ошибку
    }

    // Устанавливаем результаты запроса в модель
    model->setQuery(std::move(query)); // Используем std::move для передачи запроса

    if (model->lastError().isValid()) {
        qWarning() << "Ошибка в модели:" << model->lastError().text();
        delete model; // Удаляем модель в случае ошибки
        return nullptr; // Возвращаем nullptr, указывающий на ошибку
    }

    return model; // Возвращаем модель с результатами поиска
}

QString DataBaseManager::calculateAverages() {
    QSqlQuery query;
    QString retSr;

    // SQL-запрос для вычисления средних значений и получения имени группы
    QString sql = R"(
        SELECT
            g.id_last_group,
            g.group_name,
            AVG(CAST(s.origin1 AS FLOAT)) AS average_origin1,
            AVG(CAST(s.origin2 AS FLOAT)) AS average_origin2
        FROM
            students s
        JOIN
            last_groups g ON s.students_id_last_group = g.id_last_group
        GROUP BY
            g.id_last_group, g.group_name;
    )";

    if (!query.exec(sql)) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return "";
    }

    // Обработка результатов
    while (query.next()) {
        QVariant groupId = query.value(0);
        QVariant groupName = query.value(1);
        QVariant averageOrigin1 = query.value(2);
        QVariant averageOrigin2 = query.value(3);

        qDebug() << "Группа ID:" << groupId.toString()
                 << "Имя группы:" << groupName.toString()
                 << "Среднее origin1:" << averageOrigin1.toDouble()
                 << "Среднее origin2:" << averageOrigin2.toDouble();

        retSr += "Группа " + groupName.toString() + " " + "Среднее origin1: " + averageOrigin1.toString() + " " + "Среднее origin2: " + averageOrigin2.toString() + "\n";
    }

    return retSr;
}

QVector<QString> DataBaseManager::getComissionMembers(const QString& groupName) {
    QVector<QString> members;

    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return members; // Return an empty vector if the database is not open
    }

    QSqlQuery query(db);

    // Construct the SQL query to retrieve the comission members based on the group name.
    // First, find the year associated with the group name. Then, use that year's ID to
    // find the corresponding comission members.

    QString sqlQuery = QString("SELECT c.predsedatel, c.post_g_comiss, c.vari_g_comissi, c.podl_g_comiss "
                               "FROM comission c "
                               "JOIN year y ON c.comission_id_year = y.id_year "
                               "JOIN last_groups lg ON y.id_year = lg.group_year "
                               "WHERE lg.group_name = :groupName");


    query.prepare(sqlQuery);
    query.bindValue(":groupName", groupName);


    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return members; // Return an empty vector if the query fails
    }

    // If the query returns at least one row, extract the comission members.
    if (query.first()) {
        members.append(query.value("predsedatel").toString());
        members.append(query.value("post_g_comiss").toString());
        members.append(query.value("vari_g_comissi").toString());
        members.append(query.value("podl_g_comiss").toString());
    } else {
        qDebug() << "No comission members found for group name: " << groupName;
    }

    return members;
}

QVector<QString> DataBaseManager::getStudentData(const QString& studentId) {
    QVector<QString> result;

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return result; // Возвращаем пустой вектор, если не удалось подключиться.
    }

    // 2. Формирование SQL-запроса
    QString queryStr = QString(
        "SELECT "
        "  s.nzakl, "
        "  s.date_zakl, "
        "  s.students_id_prep, "
        "  lg.group_name, "
        "  t.dolznost_rod, "
        "  t.fio_dolz_pri, "
        "  s.fio_rod, "
        "  s.tema, "
        "  s.origin1, "
        "  s.origin2, "
        "  s.comment, "
        "  s.dopusk "
        "FROM students AS s "
            "LEFT JOIN teachers AS t ON s.students_id_prep = t.id_prep "
            "LEFT JOIN last_groups AS lg ON s.students_id_last_group = lg.id_last_group "
            "WHERE "
            " s.id_student = :studentId;");

    // 3. Подготовка запроса
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":studentId", studentId);

    // 4. Выполнение запроса
    if (!query.exec()) { // !!!!!!!!!1
        qDebug() << "SQL query failed:" << query.lastError().text();
        return result; // Возвращаем пустой вектор при ошибке выполнения запроса
    }

    // 5. Обработка результата (если он есть)
    if (query.next()) {
        result.append(query.value(0).toString());  // s.nzakl
        result.append(query.value(1).toString());  // s.date_zakl
        result.append(query.value(2).toString());  // s.students_id_prep
        result.append(query.value(3).toString());  // lg.group_name
        result.append(query.value(4).toString());  // t.dolznost_rod
        result.append(query.value(5).toString());  // t.fio_dolz_pri
        result.append(query.value(6).toString());  // s.fio_rod
        result.append(query.value(7).toString());  // s.tema
        result.append(query.value(8).toString());  // s.origin1
        result.append(query.value(9).toString());  // s.origin2
        result.append(query.value(10).toString()); // s.comment
        result.append(query.value(11).toString()); // s.dopusk
    } else {
        qDebug() << "No data found for student ID:" << studentId;
    }

    return result;
}

QVector<QString> DataBaseManager::getTeacherInfo(const QString& fio) {
    QVector<QString> result;

    // Проверяем, подключена ли база данных
    if (!db.isValid()) {
        qDebug() << "Ошибка: база данных не подключена.";
        return result; // Возвращаем пустой вектор в случае ошибки
    }

    // Формируем SQL-запрос
    QString queryText = "SELECT dolznost_rod, fio_dolz_pri FROM teachers WHERE fio = :fio";
    QSqlQuery query(db);
    query.prepare(queryText);
    query.bindValue(":fio", fio);

    // Выполняем запрос
    if (!query.exec()) {
        qDebug() << "Ошибка при выполнении запроса: " << query.lastError().text();
        qDebug() << "Запрос: " << query.lastQuery();
        return result; // Возвращаем пустой вектор в случае ошибки
    }

    // Обрабатываем результат
    if (query.next()) {
        result.append(query.value("dolznost_rod").toString());
        result.append(query.value("fio_dolz_pri").toString());
    } else {
        qDebug() << "Преподаватель с ФИО '" << fio << "' не найден.";
    }

    return result;
}

QString DataBaseManager::getTeacherDolzn(const QString& fio) {
    QString result;

    // Проверяем, подключена ли база данных
    if (!db.isValid()) {
        qDebug() << "Ошибка: база данных не подключена.";
        return result; // Возвращаем пустой вектор в случае ошибки
    }

    // Формируем SQL-запрос
    QString queryText = "SELECT dolznost FROM teachers WHERE fio = :fio";
    QSqlQuery query(db);
    query.prepare(queryText);
    query.bindValue(":fio", fio);

    // Выполняем запрос
    if (!query.exec()) {
        qDebug() << "Ошибка при выполнении запроса: " << query.lastError().text();
        qDebug() << "Запрос: " << query.lastQuery();
        return result; // Возвращаем пустой вектор в случае ошибки
    }

    // Обрабатываем результат
    if (query.next()) {
        result = query.value("dolznost").toString();
    } else {
        qDebug() << "Преподаватель с ФИО '" << fio << "' не найден.";
    }

    return result;
}

QVector<QString> DataBaseManager::getStudentEdit(const QVector<QString>& student) {
    QVector<QString>nextVec;

    // сбор всех данных для генерации
    nextVec.append(student[0]); // nzakl v
    QStringList parts = student[1].split('.'); // date_zakl 2 - 4 v
    nextVec.append(parts[0]);
    nextVec.append(parts[1]);
    nextVec.append(parts[2]);

    QVector<QString>comiss = getComissionMembers(student[3]);
    nextVec.append(comiss[0]); // predsedatel - fio v
    nextVec.append(getTeacherDolzn(comiss[0])); // predsedatel - dolznost v
    nextVec.append(comiss[1]); // post_g_comiss - fio v
    nextVec.append(getTeacherDolzn(comiss[1])); // post_g_comiss - dolznost v
    nextVec.append(comiss[2]); // vari_g_comissi - fio v
    nextVec.append(getTeacherDolzn(comiss[2])); // vari_g_comissi - dolznost v

    nextVec.append(student[6]); // s.fio_rod v
    nextVec.append(student[3]); // lg.group_name v
    nextVec.append(student[7]); // s.tema v

    //ркуоводитель
    QVector<QString>ruk = getTeacherInfo(comiss[2]); // vari_g_comissi - dolznost_rod, fio_dolz_pri v
    nextVec.append(ruk[0]);
    nextVec.append(ruk[1]);

    if(student[8] == "0") { nextVec.append("-"); } // s.origin1 v
    else { nextVec.append(student[8]); }

    if(student[9] == "0") { nextVec.append("-"); } // s.origin2 v
    else { nextVec.append(student[9]); }

    nextVec.append(student[10]); // s.comment v

    if(student[11] == "+") { nextVec.append("допустить работу к защите"); } // s.dopusk v
    else { nextVec.append("отправить на доработку"); }

    nextVec.append(nextVec[4]); // predsedatel - fio v
    nextVec.append(nextVec[6]); // post_g_comiss - fio v
    nextVec.append(nextVec[8]); //  vari_g_comissi - fio v
    //nextVec.append(comiss[3]); // podl_g_comiss - fio ?

    return nextVec;
}

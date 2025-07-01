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

void DataBaseManager::add_year_db_w(const QString& year) {
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
    query.prepare("INSERT INTO year (id_year, year) VALUES (:id_year, :year)");
    query.bindValue(":id_year", nextId);
    query.bindValue(":year", year);

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

void DataBaseManager::add_teacher_db_w(const QString& fio_t, const QString& dolznost_t, const QString& dolznost_ts, const QString& fio_tpr) {
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
    query.prepare("INSERT INTO teachers (id_prep, fio, dolznost, dolznost_sokr, fio_dolz_pri) VALUES (:id_prep, :fio, :dolznost, :dolznost_sokr, :fio_dolz_pri)");
    query.bindValue(":id_prep", nextId);
    query.bindValue(":fio", fio_t);
    query.bindValue(":dolznost", dolznost_t); // Используем полученный id_year
    query.bindValue(":dolznost_sokr", dolznost_ts);
    query.bindValue(":fio_dolz_pri", fio_tpr);

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

void DataBaseManager::add_student_db_w(const QString& fio_s, const QString& fio_rod, const QString& orig_1, const QString& orig_2, const QString& tema, const QString& dopusk, const QString& comment)
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
    query.prepare("INSERT INTO students (id_student, fio, fio_rod, origin1, origin2, tema, dopusk, comment, students_id_last_group, students_id_prep) VALUES (:id_student, :fio, :fio_rod, :origin1, :origin2, :tema, :dopusk, :comment, :students_id_last_group, :students_id_prep)");
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

void DataBaseManager::add_comiss_db_w(const QString& id_year, const QString& id_ruk, const QString& podl) {
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

    if(dolznost_ruk == "заведующий кафедрой ИС") { com1 = findByDolznost("доцент кафедры ИС"); com2 = findByDolznost("старший преподаватель кафедры ИС"); }
    if(dolznost_ruk == "старший преподаватель кафедры ИС") { com1 = findByDolznost("заведующий кафедрой ИС"); com2 = findByDolznost("доцент кафедры ИС"); }
    if(dolznost_ruk == "доцент кафедры ИС") { com1 = findByDolznost("заведующий кафедрой ИС"); com2 = findByDolznost("старший преподаватель кафедры ИС"); }
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

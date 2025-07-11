#include "docxmanager.h"

DocxManager::DocxManager() {}

void DocxManager::replacePlaceholderInXml(const QString& xmlFilePath, const QString& placeholder, const QString& replacement)  {
    // Читаем файл document.xml
    QFile xmlFile(xmlFilePath);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл:" << xmlFilePath;
        return;
    }

    QString xmlContent = xmlFile.readAll();
    xmlFile.close();

    // Используем регулярное выражение для замены плейсхолдера
    QRegularExpression regex(placeholder);
    xmlContent.replace(regex, replacement);

    // Записываем изменения обратно в файл
    if (!xmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для записи:" << xmlFilePath;
        return;
    }

    xmlFile.write(xmlContent.toUtf8());
    xmlFile.close();

    qDebug() << "Замена завершена в файле:" << xmlFilePath;
}

QString DocxManager::getUniqueFileName(const QString& basePath, const QString& fileName) {
    QString newFileName = fileName;
    QString fullPath = QDir(basePath).filePath(newFileName);
    int counter = 1;

    // Проверяем, существует ли файл
    while (QFile::exists(fullPath)) {
        // Извлекаем имя файла и расширение
        QFileInfo fileInfo(fullPath);
        QString nameWithoutExt = fileInfo.baseName();
        QString extension = fileInfo.suffix();

        // Формируем новое имя файла
        newFileName = QString("%1%2.%3").arg(nameWithoutExt).arg(counter).arg(extension);
        fullPath = QDir(basePath).filePath(newFileName);
        counter++;
    }

    return newFileName;
}

QString DocxManager::copyFile(const QString& sourcePath, const QString& destinationPath) {
    QFile sourceFile(sourcePath);
    if (!sourceFile.exists()) {
        std::cerr << "Source file does not exist." << std::endl;
        return "";
    }

    QString destinationFileName = getUniqueFileName(destinationPath, QFileInfo(sourcePath).fileName());
    QString destinationFullPath = QDir(destinationPath).filePath(destinationFileName);

    if (sourceFile.copy(destinationFullPath)) {
        std::cout << "File copied to: " << destinationFullPath.toStdString() << std::endl;
        return destinationFullPath;
    } else {
        std::cerr << "Failed to copy file." << std::endl;
        return "";
    }
}

void DocxManager::callSh(const QString& fileName, const QString& scriptPath) {
    QProcess process;

    QString workingDir = QCoreApplication::applicationDirPath() + "/template";

    // Устанавливаем рабочую директорию
    process.setWorkingDirectory(workingDir);

    // Запускаем процесс
    process.start(scriptPath, QStringList() << fileName);

    if (!process.waitForStarted()) {
        qDebug() << "Ошибка при запуске скрипта:" << process.errorString();
        return;
    }

    // Ожидание завершения процесса
    if (!process.waitForFinished()) {
        qDebug() << "Ошибка при завершении скрипта:" << process.errorString();
        return;
    }

    // Получение вывода скрипта
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    qDebug() << "Вывод:" << output;
    if (!errorOutput.isEmpty()) {
        qDebug() << "Ошибка:" << errorOutput;
    }
}

void DocxManager::copyFileToDirU(const QString& sourceDir, const QString& destDir, const QString& newName) {
    QDir dir(sourceDir);
    if (!dir.exists()) {
        qDebug() << "Source directory does not exist.";
        return;
    }

    // Установим фильтры для нужных файлов
    QStringList filters;
    filters << "*.docx" << "*.odt" << "*.pdf";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);

    QFileInfoList fileList = dir.entryInfoList();
    for (const QFileInfo& fileInfo : fileList) {
        QString fileExtension = fileInfo.suffix(); // Получаем расширение файла
        QString destFilePath = QDir(destDir).filePath(newName + "." + fileExtension); // Создаем новое имя файла

        if (QFile::copy(fileInfo.filePath(), destFilePath)) {
            qDebug() << "Copied:" << fileInfo.filePath() << "to" << destFilePath;
        } else {
            qDebug() << "Failed to copy:" << fileInfo.filePath() << "to" << destFilePath;
        }
    }
}

void DocxManager::removeFileOnDir(const QString& directoryPath) {
    QDir directory(directoryPath);

    if (!directory.exists()) {
        qDebug() << "Директория не существует: " << directoryPath;
        return;
    }

    QDirIterator it(directoryPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        QString fileExtension = fileInfo.suffix().toLower(); // Получаем расширение в нижнем регистре

        if (fileExtension == "docx" || fileExtension == "odt" || fileExtension == "pdf") {
            QFile file(filePath);
            if (file.remove()) {
                qDebug() << "Удален файл: " << filePath;
            } else {
                qDebug() << "Не удалось удалить файл: " << filePath << " Ошибка: " << file.errorString();
            }
        }
    }
}

void DocxManager::removeTmpDir(const QString& tmpFile) {
    // удаление директории
    QDir dir("template/" + tmpFile);
    if (!dir.exists()) {
        qDebug() << "Директория не существует: " << tmpFile;
    }

    if (!dir.removeRecursively()) {
        qDebug() << "Не удалось рекурсивно удалить директорию: " << tmpFile;
    }
}

void DocxManager::unZipDocxWrite(const QVector<QString>& lze_dmitriy, const QString& userDir) {
    QString fileNameToCopy = "tem.docx";

    QString sourceFilePath = QDir("in/").filePath(fileNameToCopy);

    // получаем шаблон и вызываем распаковщик
    QString fullname = copyFile(sourceFilePath, "template/");

    int lastSlashIndex = fullname.lastIndexOf('/');

    QString fileName;
    if (lastSlashIndex != -1) {
        fileName = fullname.mid(lastSlashIndex + 1);
    } else {
        fileName = "";
    }

    callSh(fileName, "./unzipf.sh");

    // меняем содержимое
    QString pointer[22] = {"{{n}}", "{{d1}}", "{{d2}}", "{{d3}}", "{{t1}}", "{{dt1}}", "{{t2}}", "{{dt2}}", "{{t3}}", "{{dt3}}", "{{sn}}", "{{gn}}", "{{tem}}", "{{rukd}}", "{{ruk}}", "{{r1}}", "{{r2}}", "{{com}}", "{{rez}}", "{{pt1}}", "{{pt2}}", "{{pt3}}"};


    fileName.chop(5);
    QString xmlFilePath = "template/" + fileName + "/word/document.xml";

    unsigned char i = 0;
    foreach (const QString& elem, lze_dmitriy) {
        replacePlaceholderInXml(xmlFilePath, pointer[i], elem);
        i++;
    }

    // собираем docx
    callSh(fileName, "./zipf.sh");
    callSh(fileName, "./convert.sh");

    copyFileToDirU("template/", userDir, lze_dmitriy[10]);
    removeFileOnDir("template/");

    removeTmpDir(fileName);
}


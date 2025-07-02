#include "fconfigmanager.h"

FConfigManager::FConfigManager(const QString &filePath) : filePath(filePath) {}

QVector<QString> FConfigManager::readConfig() {
    QVector<QString> result;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open the config file.");
        return result;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) {
            continue; // Пропускаем пустые строки и комментарии
        }

        // Используем правильное регулярное выражение для извлечения содержимого скобок
        QRegularExpression re(R"(\[([^\]]+)\])");
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            QStringList texts = match.captured(1).split(',');
            for (const QString &text : texts) {
                if (result.size() >= 3) {
                    qWarning("More than 3 entries found in brackets. Ignoring additional entries.");
                    break; // Прекращаем добавление, если уже 3 записи
                }
                result.append(text.trimmed());
            }
        }
    }

    file.close();
    return result;
}

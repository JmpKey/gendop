#ifndef FCONFIGMANAGER_H
#define FCONFIGMANAGER_H

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QRegularExpression>

class FConfigManager
{
public:
    FConfigManager(const QString& filePath);

    QVector<QString> readConfig();

private:
    QString filePath;
};

#endif // FCONFIGMANAGER_H

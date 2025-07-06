#ifndef DOCXMANAGER_H
#define DOCXMANAGER_H

#include <iostream>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QDir>

class DocxManager
{
public:
    DocxManager();

    void replacePlaceholderInXml(const QString& xmlFilePath, const QString& placeholder, const QString& replacement);
    void unZipDocxWrite(const QVector<QString>& lze_dmitriy);
    QString getUniqueFileName(const QString& basePath, const QString& fileName);
    QString copyFile(const QString& sourcePath, const QString& destinationPath);
    void callSh(const QString& fileName, const QString& scriptPath);
};

#endif // DOCXMANAGER_H

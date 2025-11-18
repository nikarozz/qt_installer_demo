#include "resource_extractor.h"

#include <QFile>
#include <QTemporaryFile>
#include <QDir>
#include <QDebug>

QString ResourceExtractor::extractResourceToTempFile(const QString& resourcePath,
                                                     QString* errorMessage)
{
    QFile inFile(resourcePath);
    if (!inFile.exists()) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Ресурс не найден: %1").arg(resourcePath);
        }
        return {};
    }

    if (!inFile.open(QIODevice::ReadOnly)) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Не удалось открыть ресурс: %1").arg(inFile.errorString());
        }
        return {};
    }

    QTemporaryFile tempFile(QDir::tempPath() + QDir::separator() + "installerXXXXXX.deb");
    tempFile.setAutoRemove(false);
    if (!tempFile.open()) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Не удалось создать временный файл: %1").arg(tempFile.errorString());
        }
        return {};
    }

    QByteArray data = inFile.readAll();
    if (tempFile.write(data) != data.size()) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("Не удалось записать данные во временный файл");
        }
        return {};
    }

    tempFile.flush();
    QString path = tempFile.fileName();
    tempFile.close();

    qDebug() << "Ресурс" << resourcePath << "извлечён во временный файл" << path;
    return path;
}

#include "installer_service.h"
#include "resource_extractor.h"
#include "default_process_runner.h"

#include <QDebug>

InstallerService::InstallerService(QObject* parent,
                                   std::unique_ptr<IProcessRunner> processRunner,
                                   ResourceExtractorFunc resourceExtractor)
    : QObject(parent)
{
    if (processRunner) {
        m_process_runner_ = std::move(processRunner);
    } else {
        m_process_runner_ = std::make_unique<DefaultProcessRunner>();
    }

    if (resourceExtractor) {
        m_resource_extractor_ = std::move(resourceExtractor);
    } else {
        m_resource_extractor_ = [](const QString& path, QString* error) -> QString {
            return ResourceExtractor::extractResourceToTempFile(path, error);
        };
    }
}

void InstallerService::installPackage(const PackageDescriptor& pkg)
{
    qDebug() << "Запуск установки пакета:" << pkg.id;
    emit installationStarted(pkg);

    if (pkg.resourcePaths.isEmpty()) {
        const QString msg = QStringLiteral("У пакета '%1' не указаны файлы ресурсов.").arg(pkg.id);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    QStringList tempFiles;
    tempFiles.reserve(pkg.resourcePaths.size());

    for (const QString& resPath : pkg.resourcePaths) {
        QString error;
        const QString tempFilePath = m_resource_extractor_(resPath, &error);
        if (tempFilePath.isEmpty()) {
            const QString msg = QStringLiteral("Не удалось извлечь пакет '%1' из ресурса %2: %3")
                                    .arg(pkg.id, resPath, error);
            qWarning() << msg;
            emit installationFinished(pkg, false, msg);
            return;
        }
        qDebug() << "Ресурс" << resPath << "извлечён во временный файл" << tempFilePath;
        tempFiles << tempFilePath;
    }

    QString program = QStringLiteral("env");
    QStringList args;
    args << QStringLiteral("DEBIAN_FRONTEND=noninteractive");
    args << QStringLiteral("dpkg");
    args << QStringLiteral("-i");
    args << tempFiles;

    ProcessResult res = m_process_runner_->run(program, args);

    if (!res.stdOut.isEmpty())
        emit installationOutput(res.stdOut);
    if (!res.stdErr.isEmpty())
        emit installationOutput(res.stdErr);

    if (res.failedToStart) {
        const QString msg = QStringLiteral("Не удалось запустить dpkg: %1").arg(res.stdErr);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    if (res.exitCode != 0) {
        const QString msg = QStringLiteral("dpkg завершился с кодом %1: %2")
                                .arg(res.exitCode)
                                .arg(res.stdErr);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    emit installationFinished(pkg, true, QString());
}

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

/*
void InstallerService::fixDependencies(const PackageDescriptor& pkg)
{
    qDebug() << "Попытка исправить зависимости через apt-get -f install для пакета:" << pkg.id;

    emit installationOutput(
        QStringLiteral("Запуск 'apt-get -f install -y' для устранения проблем с зависимостями..."));

    QString program = QStringLiteral("apt-get");
    QStringList args;
    args << QStringLiteral("-f") << QStringLiteral("install") << QStringLiteral("-y");

    ProcessResult resFix = m_process_runner_->run(program, args);

    if (!resFix.stdOut.isEmpty())
        emit installationOutput(resFix.stdOut);
    if (!resFix.stdErr.isEmpty())
        emit installationOutput(resFix.stdErr);

    if (resFix.failedToStart) {
        const QString msg = QStringLiteral("Не удалось запустить apt-get: %1").arg(resFix.stdErr);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    if (resFix.exitCode != 0) {
        const QString msg = QStringLiteral("apt-get -f install завершился с кодом %1: %2")
                                .arg(resFix.exitCode)
                                .arg(resFix.stdErr);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    QString error;
    const QString tempFilePath = m_resource_extractor_(pkg.resourcePath, &error);
    if (tempFilePath.isEmpty()) {
        const QString msg = QStringLiteral("После apt-get не удалось извлечь пакет повторно: %1").arg(error);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    QString dpkgProgram = QStringLiteral("dpkg");
    QStringList dpkgArgs;
    dpkgArgs << QStringLiteral("-i") << tempFilePath;

    ProcessResult resDpkg = m_process_runner_->run(dpkgProgram, dpkgArgs);

    if (!resDpkg.stdOut.isEmpty())
        emit installationOutput(resDpkg.stdOut);
    if (!resDpkg.stdErr.isEmpty())
        emit installationOutput(resDpkg.stdErr);

    if (resDpkg.failedToStart) {
        const QString msg = QStringLiteral("Не удалось повторно запустить dpkg: %1").arg(resDpkg.stdErr);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    if (resDpkg.exitCode != 0) {
        const QString msg = QStringLiteral("После apt-get dpkg всё равно завершился с кодом %1: %2")
                                .arg(resDpkg.exitCode)
                                .arg(resDpkg.stdErr);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    emit installationFinished(pkg, true, QString());
}
*/

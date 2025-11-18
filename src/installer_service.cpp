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

    QString error;
    const QString tempFilePath = m_resource_extractor_(pkg.resourcePath, &error);
    if (tempFilePath.isEmpty()) {
        const QString msg = QStringLiteral("Не удалось извлечь пакет: %1").arg(error);
        qWarning() << msg;
        emit installationFinished(pkg, false, msg);
        return;
    }

    QString program = QStringLiteral("dpkg");
    QStringList args;
    args << QStringLiteral("-i") << tempFilePath;

    ProcessResult res = m_process_runner_->run(program, args);

    bool success = false;
    QString errorMessage;

    if (res.failedToStart) {
        errorMessage = QStringLiteral("Не удалось запустить dpkg: %1").arg(res.stdErr);
    } else if (res.exitCode != 0) {
        errorMessage = QStringLiteral("dpkg завершился с кодом %1: %2")
                           .arg(res.exitCode)
                           .arg(res.stdErr);
    } else {
        success = true;
    }

    if (!res.stdOut.isEmpty()) {
        emit installationOutput(res.stdOut);
    }
    if (!res.stdErr.isEmpty()) {
        emit installationOutput(res.stdErr);
    }

    emit installationFinished(pkg, success, errorMessage);
}

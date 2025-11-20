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
    emit installationStarted(pkg);

    const auto files = pkg.resourcePaths;
    const int total = files.size();
    if (total == 0) {
        const QString msg = QStringLiteral("Нет файлов для установки для пакета '%1'")
                                .arg(pkg.displayName);
        emit installationFinished(pkg, false, msg);
        return;
    }

    int step = 100 / total;
    if (step <= 0)
        step = 1;

    int currentPercent = 0;
    emit installationProgress(pkg, currentPercent);

    for (int i = 0; i < total; ++i) {
        const QString& resPath = files.at(i);

        QString error;
        const QString tempFile = m_resource_extractor_(resPath, &error);
        if (tempFile.isEmpty()) {
            emit installationOutput(
                QStringLiteral("Не удалось извлечь ресурс: %1 (%2)")
                    .arg(resPath, error));
            const QString msg =
                QStringLiteral("Ошибка извлечения ресурса %1: %2").arg(resPath, error);
            emit installationFinished(pkg, false, msg);
            return;
        }

        emit installationOutput(
            QStringLiteral("Установка файла %1 (%2 из %3)...")
                .arg(resPath)
                .arg(i + 1)
                .arg(total));

        auto result = m_process_runner_->run(
            QStringLiteral("dpkg"),
            QStringList() << "-i" << tempFile);

        if (!result.stdOut.isEmpty())
            emit installationOutput(result.stdOut);
        if (!result.stdErr.isEmpty())
            emit installationOutput(result.stdErr);

        if (result.failedToStart) {
            const QString msg =
                QStringLiteral("Не удалось запустить dpkg для файла %1: %2")
                    .arg(tempFile, result.stdErr);
            emit installationFinished(pkg, false, msg);
            return;
        }

        if (result.exitCode != 0) {
            const QString msg =
                QStringLiteral("dpkg завершился с кодом %1 при установке %2")
                    .arg(result.exitCode)
                    .arg(tempFile);
            emit installationFinished(pkg, false, msg);
            return;
        }

        if (i == total - 1) {
            currentPercent = 100;
        } else {
            currentPercent += step;
            if (currentPercent > 99)
                currentPercent = 99;
        }

        emit installationProgress(pkg, currentPercent);
    }

    emit installationFinished(pkg, true, QString());
}


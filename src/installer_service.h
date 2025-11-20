#pragma once
#include <QObject>
#include <memory>
#include <functional>
#include <QString>

#include "package_descriptor.h"
#include "i_process_runner.h"

using ResourceExtractorFunc = std::function<QString(const QString&, QString*)>;

/**
 * @brief Сервис установки пакетов.
 *
 * Отвечает за:
 *  - извлечение .deb-файлов из ресурсов;
 *  - последовательный запуск dpkg для каждого файла;
 *  - эмит сигналов о ходе и результате установки.
 */
class InstallerService : public QObject
{
    Q_OBJECT
public:
    explicit InstallerService(QObject* parent = nullptr,
                              std::unique_ptr<IProcessRunner> processRunner = nullptr,
                              ResourceExtractorFunc resourceExtractor = ResourceExtractorFunc());

    void installPackage(const PackageDescriptor& pkg);

signals:
    void installationStarted(const PackageDescriptor& pkg);
    void installationFinished(const PackageDescriptor& pkg, bool success, const QString& errorMessage);
    void installationOutput(const QString& line);
    void installationProgress(const PackageDescriptor& pkg, int percent);

private:
    std::unique_ptr<IProcessRunner> m_process_runner_;
    ResourceExtractorFunc m_resource_extractor_;
};

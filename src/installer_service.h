#pragma once
#include <QObject>
#include <memory>
#include <functional>
#include <QString>

#include "package_descriptor.h"
#include "i_process_runner.h"

using ResourceExtractorFunc = std::function<QString(const QString&, QString*)>;

class InstallerService : public QObject
{
    Q_OBJECT
public:
    explicit InstallerService(QObject* parent = nullptr,
                              std::unique_ptr<IProcessRunner> processRunner = nullptr,
                              ResourceExtractorFunc resourceExtractor = ResourceExtractorFunc());

    void installPackage(const PackageDescriptor& pkg);
    //void fixDependencies(const PackageDescriptor& pkg);

signals:
    void installationStarted(const PackageDescriptor& pkg);
    void installationFinished(const PackageDescriptor& pkg, bool success, const QString& errorMessage);
    void installationOutput(const QString& line);   
    // dependencyProblemsDetected(const PackageDescriptor& pkg, const QString& details);

private:
    std::unique_ptr<IProcessRunner> m_process_runner_;
    ResourceExtractorFunc m_resource_extractor_;
};

#pragma once

#include <QString>

class ResourceExtractor
{
public:
    static QString extractResourceToTempFile(const QString& resourcePaths,
                                             QString* errorMessage = nullptr);
};

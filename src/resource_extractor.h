#pragma once

#include <QString>

class ResourceExtractor
{
public:
    static QString extractResourceToTempFile(const QString& resourcePath,
                                             QString* errorMessage = nullptr);
};

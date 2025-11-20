#pragma once

#include <QString>

/**
 * @brief Извлекает ресурс во временный файл.
 */
class ResourceExtractor
{
public:
    static QString extractResourceToTempFile(const QString& resourcePaths,
                                             QString* errorMessage = nullptr);
};

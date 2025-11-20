#pragma once

#include "package_descriptor.h"
#include <vector>

/**
 * @brief Репозиторий доступных пакетов для установщика.
 *
 * Инкапсулирует список пакетов, встроенных в приложение
 * (greetings, sl, slapd) и их привязку к ресурсам Qt.
 */
class PackageRepository
{
public:
    static std::vector<PackageDescriptor> availablePackages();
};

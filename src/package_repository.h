#pragma once

#include "package_descriptor.h"
#include <vector>

class PackageRepository
{
public:
    static std::vector<PackageDescriptor> availablePackages();
};

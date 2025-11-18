#include "package_repository.h"

std::vector<PackageDescriptor> PackageRepository::availablePackages()
{
    std::vector<PackageDescriptor> pkgs;

    pkgs.push_back(PackageDescriptor{
        QStringLiteral("postgresql"),
        QStringLiteral("PostgreSQL"),
        QStringLiteral("СУБД PostgreSQL"),
        QStringLiteral(":/packages/postgresql.deb"),
        PackageType::Rpm
    });

    pkgs.push_back(PackageDescriptor{
        QStringLiteral("slapd"),
        QStringLiteral("OpenLDAP (slapd)"),
        QStringLiteral("Сервер каталогов OpenLDAP (slapd)"),
        QStringLiteral(":/packages/slapd.deb"),
        PackageType::Deb
    });

    pkgs.push_back(PackageDescriptor{
        QStringLiteral("wine"),
        QStringLiteral("Wine"),
        QStringLiteral("Wine — запуск Windows-приложений"),
        QStringLiteral(":/packages/wine.deb"),
        PackageType::Deb
    });

    return pkgs;
}

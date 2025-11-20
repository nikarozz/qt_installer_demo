#include "package_repository.h"

std::vector<PackageDescriptor> PackageRepository::availablePackages()
{
    std::vector<PackageDescriptor> pkgs;

    pkgs.push_back(PackageDescriptor{
        QStringLiteral("greetings"),
        QStringLiteral("Greetings"),
        QStringLiteral("Small self-assembled deb package - greetings"),
        QStringList{QStringLiteral(":/packages/greetings.deb")},
        PackageType::Deb
    });

    pkgs.push_back(PackageDescriptor{
        QStringLiteral("sl"),
        QStringLiteral("Steam Locomotive"),
        QStringLiteral("Steam Locomotive – get train in shell"),
        QStringList{QStringLiteral(":/packages/sl_5.02-1_amd64.deb")},
        PackageType::Deb
    });

    pkgs.push_back(PackageDescriptor{
        QStringLiteral("slapd"),
        QStringLiteral("Slapd"),
        QStringLiteral("Stand-alone LDAP Daemon"),
        QStringList{
            QStringLiteral(":/packages/slapd/libodbc2_2.3.12-1ubuntu0.24.04.1_amd64.deb"),
            QStringLiteral(":/packages/slapd/ldap-utils_2.6.7+dfsg-1~exp1ubuntu8.2_amd64.deb"),
            QStringLiteral(":/packages/slapd/slapd_2.6.7+dfsg-1~exp1ubuntu8.2_amd64.deb")
        },
        PackageType::Deb
    });

    return pkgs;
}

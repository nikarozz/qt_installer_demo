#pragma once

#include <QString>
#include <QStringList>
#include <QMetaType>

enum class PackageType {
    Deb,
    // Rpm
};

struct PackageDescriptor {
    QString id;
    QString displayName;
    QString description;
    QStringList resourcePaths;
    PackageType type;
};

Q_DECLARE_METATYPE(PackageDescriptor)

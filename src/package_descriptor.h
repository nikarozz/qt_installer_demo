#pragma once

#include <QString>
#include <QMetaType>

enum class PackageType {
    Deb,
    Rpm
};

struct PackageDescriptor {
    QString id;
    QString displayName;
    QString description;
    QString resourcePath;
    PackageType type;
};

Q_DECLARE_METATYPE(PackageDescriptor)

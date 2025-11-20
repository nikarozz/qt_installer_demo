#pragma once

#include <QString>
#include <QStringList>
#include <QMetaType>

enum class PackageType {
    Deb,
    // Rpm
};

/**
 * @brief Описание устанавливаемого пакета.
 *
 * Хранит идентификатор, отображаемое имя, описание и
 * список путей к .deb-файлам в ресурсах Qt.
 */
struct PackageDescriptor {
    QString id;
    QString displayName;
    QString description;
    QStringList resourcePaths;
    PackageType type;
};

Q_DECLARE_METATYPE(PackageDescriptor)

#pragma once

#include <QString>
#include <QStringList>

/**
 * @brief Результат выполнения внешнего процесса.
 */
struct ProcessResult {
    int exitCode = -1;
    QString stdOut;
    QString stdErr;
    bool failedToStart = false;
};

/**
 * @brief Интерфейс абстракции над запуском внешних процессов.
 *
 * Позволяет подменять реализацию при
 * сохранении общей логики в InstallerService.
 */
class IProcessRunner
{
public:
    virtual ProcessResult run(const QString& program,
                              const QStringList& args) = 0;

    virtual ~IProcessRunner() = default;
};

#pragma once

#include <QString>
#include <QStringList>

struct ProcessResult {
    int exitCode = -1;
    QString stdOut;
    QString stdErr;
    bool failedToStart = false;
};

class IProcessRunner
{
public:
    virtual ProcessResult run(const QString& program,
                              const QStringList& args) = 0;

    virtual ~IProcessRunner() = default;
};

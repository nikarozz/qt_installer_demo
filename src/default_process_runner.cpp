#include "default_process_runner.h"

#include <QProcess>
#include <QDebug>

ProcessResult DefaultProcessRunner::run(const QString& program,
                                  const QStringList& args)
{
    ProcessResult result;

    QProcess proc;
    proc.setProgram(program);
    proc.setArguments(args);

    qDebug() << "Запуск процесса:" << program << args;

    proc.start();
    if (!proc.waitForStarted()) {
        result.failedToStart = true;
        result.stdErr = QStringLiteral("Процесс не запустился: %1").arg(proc.errorString());
        qWarning() << result.stdErr;
        return result;
    }

    proc.closeWriteChannel();

    if (!proc.waitForFinished(-1)) {
        result.failedToStart = true;
        result.stdErr = QStringLiteral("Процесс не завершился корректно: %1").arg(proc.errorString());
        qWarning() << result.stdErr;
        return result;
    }

    result.exitCode = proc.exitCode();
    result.stdOut = QString::fromLocal8Bit(proc.readAllStandardOutput());
    result.stdErr = QString::fromLocal8Bit(proc.readAllStandardError());

    qDebug() << "Процесс завершился, код:" << result.exitCode;
    if (!result.stdOut.isEmpty()) {
        qDebug() << "stdout:" << result.stdOut;
    }
    if (!result.stdErr.isEmpty()) {
        qDebug() << "stderr:" << result.stdErr;
    }

    return result;
}

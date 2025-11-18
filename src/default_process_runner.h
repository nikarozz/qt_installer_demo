#pragma once

#include "i_process_runner.h"

class DefaultProcessRunner : public IProcessRunner
{
public:
    ProcessResult run(const QString& program,
                      const QStringList& args) override;

    ~DefaultProcessRunner() override = default;
};

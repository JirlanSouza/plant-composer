#pragma once

#include "domain/ilogger_factory.h"
#include <spdlog/spdlog.h>

namespace adapters::shared {
    class SpdlogLoggerFactory : public domain::ILoggerFactory {
    public:
        SpdlogLoggerFactory();
        std::shared_ptr<domain::Ilogger> getLogger(const std::string &name) override;
    };
}

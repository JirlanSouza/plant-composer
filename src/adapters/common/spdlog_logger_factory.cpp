#include "shared/spdlog_logger_factory.h"
#include "spdlog_logger.h"

namespace adapters::shared {
    SpdlogLoggerFactory::SpdlogLoggerFactory() {
        spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v%$");
    }

    std::shared_ptr<domain::Ilogger> SpdlogLoggerFactory::getLogger(const std::string &name) {
        auto spdlog_instance = spdlog::get(name);
        if (!spdlog_instance) {
            spdlog_instance = spdlog::stdout_color_mt(name);
        }
        return std::make_shared<SpdlogLogger>(spdlog_instance);
    }
}

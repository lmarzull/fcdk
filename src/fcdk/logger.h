//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#ifndef __FCDK_LOGGER_H__
#define __FCDK_LOGGER_H__


#include <cstdint>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <filesystem>


namespace fcdk::log {

//------------------------------------------------------------------------------
// Really temporary configuration file, waiting to have yaml/ini/xml 
// configuration possibilities
//
// spdlog.stdout_logger.sink=Console
// spdlog.stdout_logger.output=stdout
// spdlog.stdout_logger.color=false
// 
// spdlog.stderr_logger.sink=Console
// spdlog.stderr_logger.output=stderr
// spdlog.stderr_logger.color=true
// 
// spdlog.basic.sink=File
// spdlog.basic.filename=/tmp/file.log
// spdlog.basic.truncate=true
// 
// spdlog.rotating.sink=RotatingFile
// spdlog.rotating.filename=/tmp/rotating.log
// spdlog.rotating.max_size=3
// spdlog.rotating.max_files=2
// spdlog.rotating.truncate=true
// 
// spdlog.daily.sink=DailyFile
// spdlog.daily.filename=/tmp/daily.log
// spdlog.daily.hour_min_24=12:00
// spdlog.daily.max_size=100
// spdlog.daily.truncate=true
//------------------------------------------------------------------------------
void  initialize(const std::filesystem::path& config_file);


} // fcdk::log


#define LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
#define LOG_DEBUG(...)    spdlog::debug(__VA_ARGS__)
#define LOG_INFO(...)     spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)    spdlog::err(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)


#define __DO_LOGGER__(logger, loglevel, ...) \
  do {\
    ::spdlog::get(logger)->log(::spdlog::level::loglevel, __VA_ARGS__); \
  } while(0)


#define LOGGER_TRACE(logger, ...)     __DO_LOGGER__(logger, trace, __VA_ARGS__)
#define LOGGER_DEBUG(logger, ...)     __DO_LOGGER__(logger, debug, __VA_ARGS__)
#define LOGGER_INFO(logger, ...)      __DO_LOGGER__(logger, info, __VA_ARGS__)
#define LOGGER_WARN(logger, ...)      __DO_LOGGER__(logger, warn, __VA_ARGS__)
#define LOGGER_ERROR(logger, ...)     __DO_LOGGER__(logger, err, __VA_ARGS__)
#define LOGGER_CRITICAL(logger, ...)  __DO_LOGGER__(logger, critical, __VA_ARGS__)


#endif  // #ifndef __FCDK_LOGGER_H__

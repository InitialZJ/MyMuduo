#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

#include "noncopyable.h"

enum LoggerLevel { INFO, ERROR, FATAL, DEBUG };

class Logger {
 public:
  static Logger& Instance();
  void setLogLevel(int level);
  void log(std::string msg);

 private:
  Logger() = default;
  int level_;
};

#define LOG_INFO(LogmsgFormat, ...)                   \
  do {                                                \
    Logger& logger = Logger::Instance();              \
    logger.setLogLevel(INFO);                         \
    char buf[1024] = {0};                             \
    snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
    logger.log(buf);                                  \
  } while (0)
#define LOG_ERROR(LogmsgFormat, ...)                  \
  do {                                                \
    Logger& logger = Logger::Instance();              \
    logger.setLogLevel(ERROR);                        \
    char buf[1024] = {0};                             \
    snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
    logger.log(buf);                                  \
  } while (0)
#define LOG_FATAL(LogmsgFormat, ...)                  \
  do {                                                \
    Logger& logger = Logger::Instance();              \
    logger.setLogLevel(FATAL);                        \
    char buf[1024] = {0};                             \
    snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
    logger.log(buf);                                  \
    exit(-1);                                         \
  } while (0)
#define LOG_DEBUG(LogmsgFormat, ...)                  \
  do {                                                \
    Logger& logger = Logger::Instance();              \
    logger.setLogLevel(DEBUG);                        \
    char buf[1024] = {0};                             \
    snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
    logger.log(buf);                                  \
  } while (0)

#endif  // !LOGGER_H_

#include "Logger.h"

#include <iostream>

#include "Timestamp.h"

Logger& Logger::Instance() {
  static Logger logger;
  return logger;
}

void Logger::setLogLevel(int level) { level_ = level; }

void Logger::log(std::string msg) {
  switch (level_) {
    case INFO:
      std::cout << "[INFO]";
      break;
    case ERROR:
      std::cout << "[ERROR]";
      break;
    case FATAL:
      std::cout << "[FATAL]";
      break;
    case DEBUG:
      std::cout << "[DEBUG]";
      break;
    default:
      break;
  }
  std::cout << TimeStamp::now().toString() << " : " << msg << std::endl;
}

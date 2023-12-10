#include "Timestamp.h"

#include <time.h>

TimeStamp::TimeStamp(int64_t microSecondSinceEpoch)
    : microSecondSinceEpoch_(microSecondSinceEpoch) {}

TimeStamp TimeStamp::now() { return TimeStamp(time(NULL)); }

std::string TimeStamp::toString() const {
  char buf[128] = {0};
  tm* tm_time = localtime(&microSecondSinceEpoch_);
  snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d", tm_time->tm_year + 1900,
           tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour,
           tm_time->tm_min, tm_time->tm_sec);
  return buf;
}

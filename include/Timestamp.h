#ifndef Timestamp_H_
#define Timestamp_H_

#include <iostream>
#include <string>

/**
 * @brief muduo的部分TimeStamp方法
 */
class TimeStamp {
 public:
  TimeStamp() = default;
  explicit TimeStamp(int64_t microSecondSinceEpoch);  // 禁止隐式转换
  static TimeStamp now();
  std::string toString() const;

 private:
  int64_t microSecondSinceEpoch_;
};

#endif  // !Timestamp_H_

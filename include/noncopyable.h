#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

/**
 * @brief 派生类可以正常构造和析构，但是不能拷贝构造和赋值
 */
namespace muduo {
class noncopyable {
 public:
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
}  // namespace muduo

#endif  // !NONCOPYABLE_H_

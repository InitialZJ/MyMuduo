#ifndef CURRENT_THREAD_H_
#define CURRENT_THREAD_H_

#include <sys/syscall.h>
#include <unistd.h>

namespace CurrentThread {
extern __thread int
    t_cachedTid;  // 通过__thread修饰的变量，在线程中地址都不一样，每个线程一份，互不干扰

void cacheTid();

inline int tid() {
  if (__builtin_expect(t_cachedTid == 0, 0)) {
    // 告诉编译器t_cachedTid == 0的概率很小
    cacheTid();
  }
  return t_cachedTid;
}

}  // namespace CurrentThread

#endif  // !CURRENT_THREAD_H_

#include <iostream>
#include <atomic>
#include <thread>

int importantData;
std::atomic_flag lock = ATOMIC_FLAG_INIT; // 초기화해주는게 좋은 코드

void func(int);

int main() {
  std::thread t1(func, 1); // 쓰레드에서 실행할 함수명, 파라미터들(여러개도 가능)
  std::thread t2(func, 2);
  std::thread t3(func, 3);

  t1.join(), t2.join(), t3.join(); // join() 필수!!

  std::cout << "fin! value: " << importantData << '\n';
}

void func(int id) {
  for (int i = 0; i < 5; i++) {
    while (lock.test_and_set(std::memory_order_acquire)) ; // spinning, busy-waiting
    importantData++;
    std::cout << "Now thread " << id << " running! value: " << importantData << '\n';
    lock.clear(std::memory_order_release); // release lock
    std::this_thread::yield(); // 다른 쓰레드가 실행되도록
  }
}

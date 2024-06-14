#include <iostream>
#include <atomic>
#include <thread>

int importantData;
std::atomic_flag lock;
// C++20부터 `ATOMIC_FLAG_INIT`은 deprecated!! C++20부터 std::atmoic_flag의 생성자는 자동으로 clear state로 초기화해준다!!

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

/*
  ref: https://en.cppreference.com/w/cpp/atomic/ATOMIC_FLAG_INIT
  
  This is the only way to initialize std::atomic_flag to a definite value: the value held after any other initialization is unspecified.
	(until C++20)

  This macro is no longer needed since default constructor of std::atomic_flag initializes it to clear state. It is kept for the compatibility with C.
	(since C++20)
*/

#include <mutex>
#include <thread>
#include <deque>
#include <chrono>
#include <iostream>
#include <condition_variable>

/*
 * Condition Variable Example
 * What: Synchronization primitive in C++ that allows threads to
 *  wait for certain conditions to be met before proceeding.
 * When: Avoid busy-waiting and improve efficiency in multi-threaded programs.
 * How: Use std::condition_variable along with std::mutex to block threads
 *      until they are notified.
 */

std::deque<int>         q;
std::mutex              mu;
std::condition_variable cond;

#define CONDITION_VAR 1

// Producer
void function_1() {
    int count = 10;

    while (count > 0) {
        std::unique_lock<std::mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
#ifdef CONDITION_VAR
        cond.notify_one(); // Notify one waiting thread
#endif
        std::this_thread::sleep_for(std::chrono::seconds(1));
        count--;
    }
}

// Consumer
void function_2() {
    int data = 0;

    while (data != 1) {
        std::unique_lock<std::mutex> locker(mu);
#ifdef CONDITION_VAR
        // Wait until notified and queue is not empty
        //cond.wait(locker);                              // Bad: Spurious wakeup
        cond.wait(locker, []{ return !q.empty(); });    // Good: Predicate to avoid spurious wakeup
#else
        if (!q.empty())
#endif
        {
            data = q.back();
            q.pop_back();
            locker.unlock();
            std::cout << "T2 got a value from T1: " << data << std::endl;
        }
    }
}

int main() {
    std::cout << "Starting" << std::endl;

    std::thread t1(function_1);
    std::thread t2(function_2);

    t1.join();
    t2.join();

    return 0;
}
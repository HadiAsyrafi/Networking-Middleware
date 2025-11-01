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

/*
 * Learning Note:
 *
 * 0000000000412df0 <std::condition_variable::wait(std::unique_lock<std::mutex>&)>:
 * 412df0:	f3 0f 1e fa          	endbr64                                 # Intel CET feature (similar to ARM BTI)
 * 412df4:	48 83 ec 08          	sub    $0x8,%rsp
 * 412df8:	48 8b 36             	mov    (%rsi),%rsi
 * 412dfb:	e8 00 d2 be ff       	callq  0 <_nl_current_LC_CTYPE>         # Likely pthread_cond_wait
 * 412e00:	85 c0                	test   %eax,%eax
 * 412e02:	75 05                	jne    412e09 <std::condition_variable::wait(std::unique_lock<std::mutex>&)+0x19>
 * 412e04:	48 83 c4 08          	add    $0x8,%rsp
 * 412e08:	c3                   	retq
 * 412e09:	e8 a2 54 ff ff       	callq  4082b0 <std::terminate()>
 * 412e0e:	66 90                	xchg   %ax,%ax
 *
 * Note: pthread_cond_wait
 *  Block a thread until either:
 *  - Another thread performs a signal or broadcast on the condition variable
 *  - A signal is delivered to the thread
 *  - The thread is cancelled
 *
 * Note: Intel Control-flow Enforcement Technology (CET)
 * - JOP or ROP attacks can be particularly hard to detect or prevent
 *      - Because it uses existing code running from executable memory
 *      - In a creative way to change program behavior
 *      - Using "gadgets" (small code sequences ending in control transfer instructions)
 * - CET
 *      - ROP protection:
 *          - Enables the OS to create a shadow-stack
 *          - Stores the original copy of the return addresses during CALL
 *      - COP / JOP protection:
 *          - Using Indirect Branch Tracking (IBT)
 *          - ENDBRANCH: Mark valid indirect CALL/JMP instructions
 *          - How?
 *              - CPU state machine moves from IDLE to WAIT_FOR_ENDBRANCH when JMP/CALL is seen
 *              - The next instruction in the program stream, must be endbr32/64
 */

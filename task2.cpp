#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

long long counter = 0;
std::mutex mtx;
const int increments = 1000000000; // 10^9 increments per thread

void increment_without_protection()
{
    for (int i = 0; i < increments; ++i)
        ++counter;
}

void increment_with_mutex()
{
    for (int i = 0; i < increments; ++i)
    {
        mtx.lock();
        ++counter;
        mtx.unlock();
    }
}


std::atomic<long long> atomic_counter(0);

void increment_atomic()
{
    for (int i = 0; i < increments; ++i)
        ++atomic_counter;
}

int main()
{
    std::thread t1, t2;
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time;
    
    // Без синхронізації
    counter = 0;
    t1 = std::thread(increment_without_protection);
    t2 = std::thread(increment_without_protection);
    t1.join();
    t2.join();
    end_time = std::chrono::steady_clock::now();
    std::cout << "Without synchronization: " << counter << ", Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms\n";

    // З м'ютексом
    start_time = std::chrono::steady_clock::now();
    counter = 0;
    t1 = std::thread(increment_with_mutex);
    t2 = std::thread(increment_with_mutex);
    t1.join();
    t2.join();
    end_time = std::chrono::steady_clock::now();
    std::cout << "With mutex: " << counter << ", Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms\n";

    // З atomic
    start_time = std::chrono::steady_clock::now();
    atomic_counter = 0;
    t1 = std::thread(increment_atomic);
    t2 = std::thread(increment_atomic);
    t1.join();
    t2.join();
    end_time = std::chrono::steady_clock::now();
    std::cout << "With atomic: " << atomic_counter << ", Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms\n";

    return 0;
}

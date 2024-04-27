#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<long long> global_counter(0);
const int total_increments = 1000000000; // 10^9 increments in total
const int num_threads = 2; // Using two threads

void partial_increment(int increments_per_thread)
{
    long long local_count = 0;
    for (int i = 0; i < increments_per_thread; ++i)
        ++local_count;

    global_counter += local_count;
}

int main()
{
    int increments_per_thread = total_increments / num_threads;
    std::vector<std::thread> threads;

    auto start_time = std::chrono::steady_clock::now();
    for (int i = 0; i < num_threads; ++i)
        threads.push_back(std::thread(partial_increment, increments_per_thread));


    for (auto& t : threads)
        t.join();

    auto end_time = std::chrono::steady_clock::now();

    std::cout << "Final counter value: " << global_counter << std::endl;
    std::cout << "Execution time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() 
              << " ms" << std::endl;

    return 0;
}

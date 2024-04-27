#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int counter = 0;
std::mutex mtx;
std::condition_variable cv;
bool turn = true; // True if it's time for the first thread to increment

void synchronized_increment(bool my_turn, int limit)
{
    std::unique_lock<std::mutex> lock(mtx);
    for (int i = 0; i < limit; ++i)
    {
        cv.wait(lock, [&]() { return turn == my_turn; });
        ++counter;
        turn = !turn; // Toggle turn
        cv.notify_all(); // Wake up the other thread
    }
}

int main()
{
    std::thread t1(synchronized_increment, true, 1000);
    std::thread t2(synchronized_increment, false, 1000);

    t1.join();
    t2.join();

    std::cout << "Final counter value: " << counter << std::endl;

    return 0;
}

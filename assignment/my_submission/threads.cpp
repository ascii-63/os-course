#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int x, y, z;
bool ready = false;
std::mutex mtx;
std::condition_variable cv;

void childThread()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []
            { return ready; });

    z = x + y;

    ready = false;
    cv.notify_one();
}

int main()
{
    std::thread child(childThread);

    std::cout << "Enter x value: ";
    std::cin >> x;
    std::cout << "Enter y value: ";
    std::cin >> y;

    ready = true;
    cv.notify_one();

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []
            { return !ready; });

    std::cout << "z = " << z << std::endl;

    child.join();

    return 0;
}

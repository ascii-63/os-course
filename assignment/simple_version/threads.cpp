#include <iostream>
#include <thread>

int x, y, z;
bool ready = false;

void childThread()
{
    while (!ready)
    {
        // Wait for the main thread to set 'ready' to true
    }

    z = x + y;
    ready = false;
}

int main()
{
    std::thread child(childThread);

    std::cout << "Enter x value: ";
    std::cin >> x;
    std::cout << "Enter y value: ";
    std::cin >> y;

    ready = true;

    while (ready)
    {
        // Wait for the child thread to set 'ready' to false
    }

    std::cout << "z = " << z << std::endl;

    child.join();

    return 0;
}

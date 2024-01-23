// child.cpp
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

int main() {
    while (true) {
        auto now = chrono::system_clock::now();
        auto now_c = chrono::system_clock::to_time_t(now);
        cout << "The child is talking at " << ctime(&now_c);
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}


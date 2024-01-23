// mainThread.cpp
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

void ChildThread() {
    while (true) {
        // time
        auto now = chrono::system_clock::now();
        auto now_c = chrono::system_clock::to_time_t(now);
        cout << "[ThreadID: " << this_thread::get_id() << "] + "<< "[System time] " << ctime(&now_c);
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    // create thread
    thread childThread(ChildThread);

    while (true) {
        auto now = chrono::system_clock::now();
        auto now_c = chrono::system_clock::to_time_t(now);
        cout << "[ThreadID: " << this_thread::get_id() << "] + "<< "[System time] " << ctime(&now_c);
        this_thread::sleep_for(chrono::seconds(1));
    }

    childThread.join(); // Ensure the child thread completes before exiting

    return 0;
}


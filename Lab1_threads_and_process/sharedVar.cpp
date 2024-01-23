// sharedVar.cpp
#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

// create one atomic variable
atomic<int> shared_var(0);

void ChildThread() {
    while (true) {
        shared_var.fetch_sub(1);
        printf("Share_var sub: %d\n", shared_var.load());
        // cout << "Share_var sub: " << shared_var.load() << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    thread childThread(ChildThread);

    while (true) {
        shared_var.fetch_add(1);
        printf("Share_var add: %d\n", shared_var.load());
        // cout << "Share_var add: " << shared_var.load() << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    childThread.join(); // Ensure the child thread completes before exiting

    return 0;
}


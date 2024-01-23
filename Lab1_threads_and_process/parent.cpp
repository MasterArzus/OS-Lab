// parent.cpp
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;
int main() {
    // if will fork new threads
    pid_t pid = fork();

    if (pid == -1) {
        cerr << "Error creating child process\n";
        return 1;
    }

    if (pid == 0) {
        // Child process
        execl("./child", "child", nullptr);
    }
    else {
        // Parent process
        while (true) {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);
            cout << "The parent is talking at " << ctime(&now_c);
            this_thread::sleep_for(chrono::seconds(1));
        }
        // Wait for the child process to finish
        wait(nullptr);
    }
    return 0;
}

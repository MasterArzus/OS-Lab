#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <queue>

using namespace std;

// Define semaphores and variables
sem_t barber_sem, customer_sem, mutex_sem;
queue<int> waiting_customers;
int seat_num; // Number of seats in the waiting room

void barber(int haircut_time) {
    while (true) {
        sem_wait(&customer_sem);
        sem_wait(&mutex_sem);

        int customer_id = waiting_customers.front();
        waiting_customers.pop();

        // sleep(haircut_time);
        this_thread::sleep_for(chrono::seconds(haircut_time));
        sem_post(&mutex_sem);
        sem_post(&barber_sem);
    }
}

void customer(int customer_id, int arrival_time) {
    //sleep(arrival_time);
    this_thread::sleep_for(chrono::seconds(arrival_time));
    sem_wait(&mutex_sem);

    if (waiting_customers.size() < seat_num) {
        waiting_customers.push(customer_id);

        printf("customer %d: there are %ld customers in front waiting for a haircut\n", customer_id, waiting_customers.size()-1);
        sem_post(&mutex_sem);
        sem_post(&customer_sem);
        sem_wait(&barber_sem);
        printf("customer %d: finished haircut\n", customer_id);
    } else {
        printf("customer %d: no more empty chairs, customer leaves\n", customer_id);
        sem_post(&mutex_sem);
    }
}

int main() {
    int n, barber_count, haircut_time;
    cout << "Input cus num, barber num, seat num, haircut time. \n";
    cin >> n >> barber_count >> seat_num >> haircut_time;

    sem_init(&barber_sem, 0, 0);
    sem_init(&customer_sem, 0, 0);
    sem_init(&mutex_sem, 0, 1);

    thread barber_threads[barber_count];
    for (int i = 0; i < barber_count; ++i) {
        barber_threads[i] = thread(barber, haircut_time);
    }

    vector<int> input;
    vector<thread> customer_threads;
    for (int i = 0; i < n; ++i) {
        int customer_id, arrival_time;
        // cout << "Input cus id, arrival_time. \n";
        cin >> customer_id >> arrival_time;
        input.push_back(customer_id);
        input.push_back(arrival_time);
        // customer_threads.push_back(thread(customer, customer_id, arrival_time));
    }

    for (int i = 0; i < n; ++i) {
        customer_threads.push_back(thread(customer, input[2 * i], input[2 * i + 1]));
    }

    for (int i = 0; i < n; ++i) {
        customer_threads[i].join();
    }

    for (int i = 0; i < barber_count; ++i) {
        sem_post(&customer_sem); // Notify barber threads to finish
    }

    for (int i = 0; i < barber_count; ++i) {
        barber_threads[i].join();
    }

    sem_destroy(&barber_sem);
    sem_destroy(&customer_sem);
    sem_destroy(&mutex_sem);

    return 0;
}



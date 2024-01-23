#include <iostream>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
#include <chrono>

using namespace std;

sem_t x, y, z, write_sem, read_sem; // x, y controls reader_count, writer_count
int reader_count = 0;
int writer_count = 0;
int priority_order; // 1 for readers, 2 for writers


void reader(int process_id, int start_time, int duration) {
    sleep(start_time);
   // while (true) {
        sem_wait(&z);
        sem_wait(&read_sem);
        sem_wait(&x);

        reader_count++;
        printf("reader %d waiting to read \n", process_id);
        //if (reader_count == 1 && priority_order == 2) {
        //    sem_wait(&write_sem);
        //}
        if (reader_count == 1) {
            sem_wait(&write_sem);
        }

        sem_post(&x);
        sem_post(&read_sem);
        sem_post(&z);

        printf("reader %d start to read \n", process_id);
        sleep(duration);
        //this_thread::sleep_for(chrono::seconds(duration));
        printf("reader %d ends reading \n", process_id);

        sem_wait(&x);
        reader_count--;

        if (reader_count == 0) {
            sem_post(&write_sem);
        }

        sem_post(&x);
        // this_thread::sleep_for(chrono::seconds(15 - duration));
   // }
}

void writer(int process_id, int start_time, int duration) {
    sleep(start_time);
    // this_thread::sleep_for(chrono::seconds(start_time));

    // while (true) {
        sem_wait(&y);
        writer_count++;

        //if (writer_count == 1 && priority_order == 1) {
        //    sem_wait(&read_sem);
        //}
        if (writer_count == 1) {
            sem_wait(&read_sem);
        }

        sem_post(&y);
        printf("writer %d waiting to write \n", process_id);
        sem_wait(&write_sem);
        printf("writer %d start to write \n", process_id);
        sleep(duration);
        //this_thread::sleep_for(chrono::seconds(duration));
        printf("writer %d ends writing \n", process_id);

        writer_count--;

        if (writer_count == 0) {
            sem_post(&read_sem);
        }

        sem_post(&write_sem);
        //this_thread::sleep_for(chrono::milliseconds(200));
    //}
}

int main() {
    cout << "Enter priority and total process: \n";
    cin >> priority_order;

    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);
    sem_init(&z, 0, 1);
    sem_init(&write_sem, 0, 1);
    sem_init(&read_sem, 0, 1);

    int total_processes;
    cin >> total_processes;

    thread* processes = new thread[total_processes];

    cout << "Enter each process: \n";
    vector<int> input_num;
    vector<char> input_char;
    for (int i = 0; i < total_processes; ++i) {
        int process_id, start_time, duration;
        char type;
        cin >> process_id >> type >> start_time >> duration;

        input_num.push_back(process_id);
        input_num.push_back(start_time);
        input_num.push_back(duration);
        input_char.push_back(type);
    }

    for (int i = 0; i < total_processes; ++i) {

        if (input_char[i] == 'R') {
            processes[i] = thread(reader, input_num[3*i], input_num[3 * i + 1], input_num[3 * i + 2]);
        }
        else {
            processes[i] = thread(writer, input_num[3 * i], input_num[3 * i + 1], input_num[3 * i + 2]);
        }
    }

    for (int i = 0; i < total_processes; ++i) {
        processes[i].join();
    }

    sem_destroy(&x);
    sem_destroy(&y);
    sem_destroy(&z);
    sem_destroy(&write_sem);
    sem_destroy(&read_sem);

    delete[] processes;

    return 0;
}


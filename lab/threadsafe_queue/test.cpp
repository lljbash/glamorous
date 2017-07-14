#include <iostream>
#include <thread>
#include <cstdlib>

#include "threadsafe_queue.h"

threadsafe_queue<int> q;
std::mutex mutex;

void threadA() {
    int result = 0;
    for (int i = 0; i < 100; i++) {
        // if (q.try_pop(result) == true) {
        //     std::cout << "pop : " << result << std::endl;
        // } else {
        //     std::cout << "pop : failed" << std::endl;
        // }
        q.wait_and_pop(result);
        // std::cout << "pop : " << result << std::endl;
        std::lock_guard<std::mutex> lock(mutex);
        printf("pop : %d\n", result);
        fflush(stdout);
    }
}

void threadB() {
    for (int i = 0; i < 100; i++) {
        while (rand() % 10000 != 0);
        q.push(i);
        // std::cout << "push : " << i << std::endl;
        std::lock_guard<std::mutex> lock(mutex);
        printf("push : %d\n", i);
        fflush(stdout);
    }
}

int main(int argc, char const *argv[]) {
    srand(123);
    std::thread tA(threadA);
    std::thread tB(threadB);
    tA.join();
    tB.join();
    return 0;
}

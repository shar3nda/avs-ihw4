#include <pthread.h>
#include <mutex>
#include <vector>
#include <queue>
#include "work.cpp"

int main() {
#ifdef WRONG_CPP_VER
    printf("Стандарт C++ старше c++20. Запустите программу с -std=c++20, иначе вывод может быть в некорректном порядке.\n\n");
#endif

    std::srand(std::time(nullptr));
    pthread_mutex_init(&mutex, nullptr);
    for (size_t i = 0; i < 3; ++i) {
        tasks.push_back(std::queue<Task>());
    }
    std::vector<pthread_t> workers = std::vector<pthread_t>(3);
    for (size_t i = 0; i < 3; ++i) {
        pthread_create(&workers[i], nullptr, doWork, (void *) (i));
    }
    for (size_t i = 0; i < 3; ++i) {
        pthread_join(workers[i], nullptr);
    }
    return 0;
}


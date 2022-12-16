#include <iostream>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <sstream>
#include <iomanip>
#include <syncstream>

std::string tm() {
    const auto now = std::chrono::system_clock::now();
    const auto now_as_time_t = std::chrono::system_clock::to_time_t(now);
    const auto now_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream now_ss;
    now_ss << std::put_time(std::localtime(&now_as_time_t), "[%T") << '.' << std::setfill('0')
           << std::setw(3) << now_ms.count() << "] ";
    return now_ss.str();
}

#define dbgout std::osyncstream(std::cout) << tm()

enum task_type {
    WRONG, OK, TO_CHECK
};

struct Task {
 public:
    Task(task_type type, int from_id, int task_id) {
        this->type = type;
        this->from_id = from_id;
        this->task_id = task_id;
    }
    Task() {}

    task_type type = {};
    int from_id = {};
    int task_id = {};
};

std::vector<std::queue<Task>> tasks;
pthread_mutex_t mutex;

void *doWork(void *void_worker_id) {
    int worker_id = (size_t) (void_worker_id);
    int self_task_id = 1;

    while (true) {
        dbgout << "Программист " << worker_id << " решает задачу №" << self_task_id << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 1000));
        dbgout << "Программист " << worker_id << " решил задачу №" << self_task_id << "\n";
        // в очередь прогера to_id добавить задачу
        int sent_to_id = rand() % 2;
        if (worker_id == 0 || worker_id == 1 && sent_to_id == 1) {
            ++sent_to_id;
        }
        pthread_mutex_lock(&mutex);
        tasks[sent_to_id].emplace(TO_CHECK, worker_id, self_task_id);
        pthread_mutex_unlock(&mutex);

        dbgout << "Программист " << worker_id << " отправил задачу №" << self_task_id
               << " на ревью программисту " << sent_to_id << "\n";

        while (true) {
            if (!tasks[worker_id].empty()) {
                pthread_mutex_lock(&mutex);
                Task task = tasks[worker_id].front();
                tasks[worker_id].pop();
                pthread_mutex_unlock(&mutex);

                if (task.type == OK) {
                    ++self_task_id;
                    break;
                } else if (task.type == WRONG) {
                    dbgout << "Программист " << worker_id << " перерешивает задачу №"
                           << self_task_id << "\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 1000));
                    pthread_mutex_lock(&mutex);
                    tasks[task.from_id].push(Task(TO_CHECK, worker_id, self_task_id));
                    pthread_mutex_unlock(&mutex);
                    dbgout << "Программист " << worker_id << " повторно отправил задачу №"
                           << self_task_id << " на ревью программисту " << task.from_id << "\n";
                } else {
                    dbgout << "Программист " << worker_id << " проверяет задачу №" << task.task_id
                           << " программиста " << task.from_id << "\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10000 + 1000));
                    int is_correct = rand() % 2;
                    dbgout << "Программист " << worker_id << " проверил задачу №" << task.task_id
                           << " программиста " << task.from_id << ". Вердикт: "
                           << (is_correct ? "OK" : "WRONG") << "\n";
                    pthread_mutex_lock(&mutex);
                    tasks[task.from_id].push(Task(is_correct ? OK : WRONG,
                                                  worker_id,
                                                  self_task_id));
                    pthread_mutex_unlock(&mutex);
                    if (!is_correct) {
                        dbgout << "Программист " << worker_id << " отправил задачу №"
                               << self_task_id << " на переписывание программисту " << task.from_id
                               << "\n";
                    }
                }
            }
        }
    }
}

int main() {
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


#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include "task.cpp"
#include "cli_utils.cpp"

/// Вектор с очередью заданий для i-го программиста в tasks[i]
std::vector<std::queue<Task>> tasks;
/// Мьютекс
pthread_mutex_t mutex;

/**
 * Основной цикл для программиста
 * @param void_worker_id id программиста
 * @return
 */
[[noreturn]] void *doWork(void *void_worker_id);
void rewriteTask(int &worker_id, int &self_task_id, const Task &task);
void reviewTask(int &worker_id, int &self_task_id, const Task &task);
void addTask(const int to, const Task task);


[[noreturn]] void *doWork(void *void_worker_id) {
    int worker_id = (size_t) void_worker_id;
    int self_task_id = 1;

    while (true) {
        dbgout << "Программист " << worker_id << " решает задачу №" << self_task_id << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 1000));
        int sent_to_id = rand() % 2;
        if (worker_id == 0 || worker_id == 1 && sent_to_id == 1) {
            ++sent_to_id;
        }
        pthread_mutex_lock(&mutex);
        tasks[sent_to_id].emplace(TO_CHECK, worker_id, self_task_id);
        pthread_mutex_unlock(&mutex);

        dbgout << "Программист " << worker_id << " решил задачу №" << self_task_id
               << " и отправил ее на ревью программисту " << sent_to_id << "\n";

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
                    rewriteTask(worker_id, self_task_id, task);
                } else {
                    reviewTask(worker_id, self_task_id, task);
                }
            }
        }
    }
}
void rewriteTask(int &worker_id, int &self_task_id, const Task &task) {
    dbgout << "Программист " << worker_id << " перерешивает задачу №" << self_task_id << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 1000));
    pthread_mutex_lock(&mutex);
    tasks[task.from_id].push(Task(TO_CHECK, worker_id, self_task_id));
    pthread_mutex_unlock(&mutex);
    dbgout << "Программист " << worker_id << " повторно отправил задачу №" << self_task_id
           << " на ревью программисту " << task.from_id << "\n";
}
void reviewTask(int &worker_id, int &self_task_id, const Task &task) {
    dbgout << "Программист " << worker_id << " проверяет задачу №" << task.task_id
           << " программиста " << task.from_id << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10000 + 1000));
    int is_correct = rand() % 2;
    dbgout << "Программист " << worker_id << " проверил задачу №" << task.task_id
           << " программиста " << task.from_id << ". Вердикт: " << (is_correct ? "OK" : "WRONG")
           << "\n";
    addTask(task.from_id, Task(is_correct ? OK : WRONG, worker_id, task.task_id));
}
void addTask(const int to, const Task task) {
    pthread_mutex_lock(&mutex);
    tasks[to].push(task);
    pthread_mutex_unlock(&mutex);
}
/// Тип задания - неверная программа, верная программа, программа на проверку
enum task_type {
    WRONG, OK, TO_CHECK
};

/// Структура "Задание"
struct Task {
 public:
    Task(task_type type, int from_id, int task_id) {
        this->type = type;
        this->from_id = from_id;
        this->task_id = task_id;
    }
    Task() {}

    /// Тип задания
    task_type type = {};
    /// От кого
    int from_id = {};
    /// Порядковый номер задания
    int task_id = {};
};
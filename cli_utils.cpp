#include <chrono>
#include <iomanip>
#include <iostream>


// Макрос для вывода в синхронизированный поток с добавлением времени вывода.
// Библиотека syncstream доступна только начиная с c++20 - делаем проверку
#if __cplusplus >= 202002L
#include <syncstream>
#define dbgout std::osyncstream(std::cout) << colors[worker_id] << tm() << reset_colors
#else
// < c++20 - используем cout
#define WRONG_CPP_VER
#define dbgout std::cout << tm() << ' '
#endif

/// Строки с ANSI-последовательностями для цветов - зеленый, голубой, пурпурный
const char *colors[] = {"\033[30;42m", "\033[30;44m", "\033[30;45m"};
/// Строка, возвращающая исходные цвета
const char *reset_colors = "\033[0m ";

/**
 * Функция возвращает строку с текущим временем в формате "[H%M%S.%MS]" (например, [15:10:11.452])
 * @return Отформатированная строка
 */
std::string tm() {
    const auto now = std::chrono::system_clock::now();
    const auto now_as_time_t = std::chrono::system_clock::to_time_t(now);
    const auto now_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream now_ss;
    now_ss << std::put_time(std::localtime(&now_as_time_t), "[%T") << '.' << std::setfill('0')
           << std::setw(3) << now_ms.count() << "]";
    return now_ss.str();
}
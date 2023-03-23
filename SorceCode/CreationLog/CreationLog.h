#ifndef SORSECODE_CREATIONLOG_CREATIONLOG_H_
#define SORSECODE_CREATIONLOG_CREATIONLOG_H_

#include <string.h> // Библиотека для работы с строками

#include <chrono> // Библиотека для работы с временем
#include <fstream> // Библиотека для работы с файлами
#include <iostream> // Функции для работы с вводом-выводом

class Logger {
 private:
  std::fstream file_;

 public:
    /*
        *   Создает конечную точку для связи и 
        *   возвращает дескриптор сокета
        *
        *   Incoming:
        *       domain - Домен/адресса связи в пределах 
        *                которого будет осуществлятся связь
    */
    Logger(const char* path);

    // Деструктор класса логированя
    ~Logger();

    /*
    *   Создает запись в файле логов
    *
    *   Incoming:
    *       buf - данные которые нужно записать в логи
    */
    void CreateLog(const char* buf);
};  // class Logger

#endif // SORSECODE_CREATIONLOG_CREATIONLOG_H_

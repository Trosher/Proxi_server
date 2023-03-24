#ifndef SORCECODE_SERVER_SERVER_H_
#define SORCECODE_SERVER_SERVER_H_

#include <algorithm> //набор функций для выполнения алгоритмических операций 
                    //над контейнерами и над другими последовательностями
#include <vector> // Реализация динамического массива
#include <csignal> // Макросы сигналов и функции их отработки
#include <fstream> // набор классов, методов и функций, которые предоставляют 
                  // интерфейс для чтения/записи данных из/в файл
#include <memory>
#include <utility>

#include "../NetProcessing/NetProcessing.h" // Подключение обёрток функций сокетов
#include "../CreationLog/CreationLog.h" // Подключение функционала для логирования
constexpr size_t BUFF_SIZE = 256;

constexpr char SQL_QUERY_1 = '\x03';
constexpr char SQL_QUERY_2 = '\x16';

// Флаг выключения сервера
bool shutdown_server_ = false;

namespace net_protocol {
    class Server {
        public:
            /*
            *   Конструктор класса сервер
            *   Создает fd для прослушивающего сокета
            *    
            *   Настраивает этот сокет на прослушивание с очередью из 10 клиентов
            *    
            *   Создаёт порт, который сервер будет использовать 
            *   в качестве прослушивающего для подключения нового клиента
            */
            Server(int proxi_port, int port_bd, char *ip_bd);

            // Диструктор класса сервр
            ~Server();

            /*
            *   Функция проверки наличия новых подключений
            *   Основной метод, вызывающий poll()
            *   При наличии хотя бы одного подключения вызывает 
            *   функцию контроля дискрипторов
            */ 
            void CheckingConnectionRequests();

            /*
            *   Функция обработки сигнала Ctrl + C or Ctrl + Z
            *   
            *   Incoming:
            *       signum - входной сигнал
            */ 
            static void SigHandler(int signum);

        private:
            /*
            *   Функция разрыва соиденения между клиентом и базой данных
            *   а так же дальнейшее удаление не нужных fd
            *   
            *   Incoming:
            *       i - Индекс удаляемого дискриптора
            *       ItIsUserFd - Является ли данный дискриптор юзерским
            */
            void Disconnect_(const size_t &i, const bool &ItIsUserFd);

            /*
            *   Передача сообщения от пользователя к DB
            *   
            *   Incoming:
            *       i - Индекс расматриваемого дискриптора
            *       ItIsUserFd - Является ли данный дискриптор юзерским
            */
            void FromUser_(const size_t &i);

            /*
            *   Передача сообщения от DB к пользователю
            *   
            *   Incoming:
            *       i - Индекс расматриваемого дискриптора
            *       ItIsUserFd - Является ли данный дискриптор юзерским
            */
            void ToUser_(const size_t &i);

            // Функция создающая зависимость между декрипторами клиента и Db
            void ConnectingClientAndDB_();
            
            /*
            *   Функция проверки сотояния дискрипторов
            *   При наличии запросов в дескрипторах вызывает сответствующие функции
            */ 
            void DescriptorController_();

            // Инициализация и настройка порта для подключения клиентов
            void InitListenPorts_();
            
            // Функция добавления новых юзеров
            void AddNewUsers_();

            // Буфер запросов клиента
            char buf_[BUFF_SIZE];

            // Вектор подключёных дискрипторов
            std::vector<pollfd> fds_;

            // Счётчик подключёных дескрипторов
            size_t fds_caunter_;

            // Порт прокси
            const int PROXI_PORT_;

            // Порт базы данных
            const int DB_PORT_;

            // ip базы данных
            const char *IP_DB_;

            // Создание логера для записи логов
            std::unique_ptr<Logger> logger_;
    }; // class Server
} // namespace net_protocol

#endif // SORCECODE_SERVER_SERVER_H_
#ifndef SORCECODE_SERVER_SERVER_H_
#define SORCECODE_SERVER_SERVER_H_

#include <algorithm> //набор функций для выполнения алгоритмических операций 
                    //над контейнерами и над другими последовательностями
#include <vector> // Реализация динамического массива
#include <csignal> // Макросы сигналов и функции их отработки
#include <fstream> // набор классов, методов и функций, которые предоставляют 
                  // интерфейс для чтения/записи данных из/в файл

#include "../Client/Client.h" // Определение класса клиента
constexpr size_t BUFF_SIZE = 255;
constexpr int SERVER_PORT = 85;
// Флаг выключения сервера
// bool shutdown_server_ = false;

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
            *   Incoming:
            *       signum - входной сигнал
            */ 
            static void SigHandler(int signum);

        private:
            
            const int PROXI_PORT_, BD_PORT_, MAX_USERS;
            const char *IP_BD_;

            // Функция создающая зависимость между декрипторами клиента и Db
            void ConnectingClientAndDB();
            
            /*
            *   Функция проверки сотояния дискрипторов
            *   При наличии запросов в дескрипторах вызывает сответствующие функции
            */ 
            void DescriptorController_();

            // Инициализация и настройка порта для подключения клиентов
            void InitListenPorts_();
            
            // Функция добавления нового юзера
            void AddNewUser_();
            // Вектор подключёных клиентов
            std::vector<Client> fds_clients_;
            // Буфер запросов клиента
            char buf_[BUFF_SIZE];
            // Вектор подключёных дискрипторов
            std::vector<pollfd> fds_;
            // Счётчик подключёных дескрипторов
            int fds_caunter_;

    }; // class Server
} // namespace net_protocol

#endif // SORCECODE_SERVER_SERVER_H_
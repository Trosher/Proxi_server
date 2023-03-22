#ifndef SORCECODE_CLIENT_CLIENT_H_
#define SORCECODE_CLIENT_CLIENT_H_

#include "../NetProcessing/NetProcessing.h"

namespace net_protocol {
class Client {
    public:
        /*
        *
        */
        Client(pollfd* client_poll, pollfd* db_poll);
        // Деструктор клиента
        ~Client();
        // Получения дескриптора соиденения между клиентом и прокси
        int GetFd();
        // Получения дескриптора соиденения между прокси и DB
        int GetDBFd();
        /*
        *   Установка дискриптора для соеденения прокси и DB
        *   Incoming:
        *       new_fd - Новый дескриптор для седенения
        */
        std::pair<short, short> GetEvents();
        /*
        *   Установка дискриптора для соеденения клиента и прокси
        *   Incoming:
        *       new_fd - Новый дескриптор для седенения
        */
        void SetFd(int new_fd);
        /*
        *   Установка дискриптора для соеденения прокси и DB
        *   Incoming:
        *       new_fd - Новый дескриптор для седенения
        */
        void SetDBFd(int new_fd);
    private:
        // Соеденение между клиентом и прокси
        pollfd* c_fd_;
        // Соеденение между прокси и DB
        pollfd* db_fd_;
}; // class Client
} // net_protocol 

#endif // SORCECODE_CLIENT_CLIENT_H_
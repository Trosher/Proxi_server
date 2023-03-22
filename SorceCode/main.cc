#include "Server/Server.h"

int main() {
    std::vector<net_protocol::Client> aga;
    pollfd listening_server_fds_;
    listening_server_fds_.fd = 1;
    std::cout << listening_server_fds_.fd << std::endl;
    net_protocol::Client aga3(&listening_server_fds_, &listening_server_fds_);
    aga.push_back(aga3);
    int aga2 = aga[0].GetFd();
    std::cout << aga2 << std::endl;
    delete aga[0];
    int aga12 = aga[0].GetFd();
    return 0;
}
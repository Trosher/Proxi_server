#include "Server.h"

net_protocol::Server::Server(int proxi_port, int port_bd, char *ip_bd, int max_users)
            : fds_caunter_(0),
            PROXI_PORT_(proxi_port),
            BD_PORT_(port_bd),
            IP_BD_(ip_bd) {
    InitListenPorts();
}

void net_protocol::Server::InitListenPorts_() {
    int listening = -1;
    listening = net_protocol::NetProcessing::Socket(AF_INET, SOCK_STREAM, 0);
    net_protocol::NetProcessing::MakeSocketReuseable(listening);
    net_protocol::NetProcessing::SetNonBlockingSocket(listening);
    auto addr_size = net_protocol::NetProcessing::InintAddr(PROXI_PORT_);
    net_protocol::NetProcessing::Bind(listening, (struct sockaddr*)&addr_size, sizeof(addr_size));
    net_protocol::NetProcessing::Listen(listening, 50);
    pollfd fds_buf_;
    fds_buf_.fd = listening;
    fds_buf_.events = POLL_IN;
    fds_.push_back(fds_buf_);
}

net_protocol::Server::~Server() {
    for (auto it : fds_)
        close(it.fd);
}

void net_protocol::Server::ConnectingClientAndDB() {
    auto addr = net_protocol::NetHandler::NetProcessing(DB_PORT);
    int db_fd = net_protocol::NetHandler::Socket(AF_INET, SOCK_STREAM, 0);
    net_protocol::NetHandler::IPConverter(AF_INET, IP_BD_, &addr.sin_addr);
    net_protocol::NetHandler::Connect(db_fd, (sockaddr*)&addr, sizeof(addr));
    pollfd fds_buf_;
    fds_buf_.fd = new_fd;
    fds_buf_.events = POLLIN;
    fds_.push_back(fds_buf_);
    std::cout << "Added new pair of client and DB" << std::endl;
}

void net_protocol::Server::AddNewUser_() {
    int new_fd = 0;
    std::cout << "\nGot the request to add new user\n";
    do {
        new_fd = accept(fds_[0].fd, NULL, NULL);
        if (new_fd < 0) {
            if (errno != EWOULDBLOCK) {
                perror("ERROR: Accept failed \n");
                shutdown_server_ = true;
            }
            break;
        }
        pollfd fds_buf_;
        fds_buf_.fd = new_fd;
        fds_buf_.events = POLLIN;
        fds_.push_back(fds_buf_);
        ConnectingClientAndDB();
    } while (new_fd != -1);
}
void net_protocol::Server::DescriptorController_() {
    if (fds_[0].revents == POLLIN) {
        AddNewUsers_();
    }
    
}

void net_protocol::Server::CheckingConnectionRequests() {
    int status = 0;
    do {
        std::cout << "poll primal event\n";
        pollfd *lisen_fds_ = fds_.data();
        status = poll(lisen_fds_, fds_caunter_, -1);
        if (status < 0) {
            perror("ERROR: Poll fails while waiting for the request \n");
            break;
        } else if (status == 0) {
            perror("ERROR: Poll timed out \n");
            break;
        }
        DescriptorController();
    } while (!shutdown_server_);
}

void net_protocol::Server::SigHandler(int /*signum*/) {
    std::cout << "\nSig exit" << std::endl;
    shutdown_server_ = true;
}

int main(int argc, char** argv) {
    if (argc != 5 && atoi(argv[1]) > 0 && atoi(argv[2]) > 0) {
        std::cout << "ERROR: The arguments passed when the server is turned on are incorrect\n\
        Example of valid arguments: 85 (Port proxi) 5432 (Port Db) 0.0.0.0 (IP Db)\n\
        100 (Max Users)" << std::endl;
        return -1;
    }
    std::cout << "If you want to stop the server use this: Ctrt+C or Ctrl+Z" << std::endl;
    signal(SIGINT, net_protocol::Server::SigHandler);
    signal(SIGTSTP, net_protocol::Server::SigHandler);
    net_protocol::Server server(atoi(argv[1]), atoi(argv[2]), argv[3]);
    server.CheckingConnectionRequests();
    return 0;
}
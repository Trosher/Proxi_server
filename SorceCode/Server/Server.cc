#include "Server.h"

net_protocol::Server::Server(int proxi_port, int port_bd, char *ip_bd)
            : fds_caunter_(0),
            PROXI_PORT_(proxi_port),
            DB_PORT_(port_bd),
            IP_DB_(ip_bd),
            logger_(new Logger("Log.txt")) {
    InitListenPorts_();
}

void net_protocol::Server::InitListenPorts_() {
    int listening = -1;
    listening = net_protocol::NetProcessing::Socket(AF_INET, SOCK_STREAM, 0);
    net_protocol::NetProcessing::MakeSocketReuseable(listening);
    net_protocol::NetProcessing::SetNonBlockingSocket(listening);
    auto addr_size = net_protocol::NetProcessing::InitAddr(PROXI_PORT_);
    net_protocol::NetProcessing::Bind(listening, (struct sockaddr*)&addr_size, sizeof(addr_size));
    net_protocol::NetProcessing::Listen(listening, 50);
    pollfd fds_buf_;
    fds_buf_.fd = listening;
    fds_buf_.events = POLL_IN;
    fds_.emplace_back(fds_buf_);
    fds_caunter_++;
}

net_protocol::Server::~Server() {
    for (auto it : fds_)
        close(it.fd);
}

void net_protocol::Server::ConnectingClientAndDB_() {
    auto addr = net_protocol::NetProcessing::InitAddr(DB_PORT_);
    int db_fd = net_protocol::NetProcessing::Socket(AF_INET, SOCK_STREAM, 0);
    net_protocol::NetProcessing::IPConverter(AF_INET, IP_DB_, &addr.sin_addr);
    net_protocol::NetProcessing::Connect(db_fd, (sockaddr*)&addr, sizeof(addr));
    pollfd fds_buf_;
    fds_buf_.fd = db_fd;
    fds_buf_.events = POLLIN;
    fds_.emplace_back(fds_buf_);
    fds_caunter_++;
    std::cout << "Added new pair of client and DB" << std::endl;
}

void net_protocol::Server::AddNewUsers_() {
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
        fds_.emplace_back(fds_buf_);
        fds_caunter_++;
        ConnectingClientAndDB_();
    } while (new_fd != -1);
}

void net_protocol::Server::Disconnect_(const size_t &i, const bool &ItIsUserFd) {
    if (ItIsUserFd) {
        close(fds_[i].fd);
        close(fds_[i+1].fd);
        fds_.erase(fds_.begin() + i, fds_.begin() + i + 2);
    } else {
        close(fds_[i-1].fd);
        close(fds_[i].fd);
        fds_.erase(fds_.begin() + i - 1, fds_.begin() + i + 1);
    }
    fds_caunter_-=2;
}

void net_protocol::Server::FromUser_(const size_t &i) {
    char buf[BUFF_SIZE]{};
    bool close_connection = false;
    auto status = net_protocol::NetProcessing::Read(fds_[i].fd, buf, BUFF_SIZE);
    if (status.second) 
        close_connection = true;
    if (buf[4] == 3 || buf[4] == 22) {
        try {
            logger_->CreateLog(buf + 5);
        } catch (std::invalid_argument const& ex) {
            std::cout << ex.what();
        }
    }
    status = net_protocol::NetProcessing::Write(fds_[i+1].fd, buf, status.first);
    if (status.second || close_connection) 
        Disconnect_(i, 1); 
}

void net_protocol::Server::ToUser_(const size_t &i) {
    char buf[BUFF_SIZE]{};
    bool close_connection = false;
    auto status = net_protocol::NetProcessing::Read(fds_[i].fd, buf, BUFF_SIZE);
    if (status.second) 
        close_connection = true;
    status = net_protocol::NetProcessing::Write(fds_[i-1].fd, buf, status.first);
    if (status.second || close_connection) 
        Disconnect_(i, 0);
}

void net_protocol::Server::DescriptorController_() {
    if (fds_[0].revents == POLLIN) {
        AddNewUsers_();
    }
    for (size_t i = 1; i < fds_caunter_; i++) {
        bool ItIsUserFd = i % 2;
        short StatusEvent = fds_[i].revents;
        if (StatusEvent == 0) {
            continue;
        } else if (StatusEvent == SIGTSTP) {
            Disconnect_(i, ItIsUserFd);
            i--;
        } else if (StatusEvent == POLLIN) {
            if (ItIsUserFd) {
                FromUser_(i);
            } else {
                ToUser_(i);
            }
        }
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
        DescriptorController_();
    } while (!shutdown_server_);
}

void net_protocol::Server::SigHandler(int /*signum*/) {
    std::cout << "\nSig exit" << std::endl;
    shutdown_server_ = true;
}

int main(int argc, char** argv) {
    if (argc != 4 && atoi(argv[1]) > 0 && atoi(argv[2]) > 0) {
        std::cout << "ERROR: The arguments passed when the server is turned on are incorrect\n\
        Example of valid arguments: 85 (Port proxi) 5432 (Port Db) 0.0.0.0 (IP Db)" << std::endl;
        return -1;
    }
    std::cout << "If you want to stop the server use this: Ctrt+C or Ctrl+Z" << std::endl;
    signal(SIGINT, net_protocol::Server::SigHandler);
    signal(SIGTSTP, net_protocol::Server::SigHandler);
    net_protocol::Server server(atoi(argv[1]), atoi(argv[2]), argv[3]);
    server.CheckingConnectionRequests();
    return 0;
}
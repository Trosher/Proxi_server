#include "Client.h"

net_protocol::Client::Client(pollfd* client_poll, pollfd* db_poll)
    : c_fd_(client_poll),
      db_fd_(db_poll) {}

net_protocol::Client::~Client() {}

void net_protocol::Client::SetFd(int new_fd) { c_fd_->fd = new_fd; }

int net_protocol::Client::GetFd() { return c_fd_->fd; }

void net_protocol::Client::SetDBFd(int new_fd) { db_fd_->fd = new_fd; }

int net_protocol::Client::GetDBFd() { return db_fd_->fd; }

std::pair<short, short> net_protocol::Client::GetEvents() {
  return {c_fd_->revents, db_fd_->revents};
}
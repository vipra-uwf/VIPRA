
#include "tcp_host.hpp"

#include <stdio.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "vipra/logging/logging.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Output {

VIPRA_REGISTER_MODULE(TCPHost, Output)

// void TCPHost::initialize()
// {
//     // TODO(tyler): Initialize Godot server connection here.

//     VIPRA::Log::debug("Initializing TCPHost Output Module.");
//     _client_id = create_server();

//     if (_client_id == -1) {
//         VIPRA_MODULE_ERROR("Failed to create TCP server.");
//     }

// }

void TCPHost::write(std::filesystem::path const& outputDir)
{
    // Close socket here.
    close_server(_server_fd, _client_id);
}

// Technically, we could send data over TCP each timestep.
void TCPHost::timestep_update(VIPRA::timestep /*unused*/,
                                       VIPRA::delta_t /*unused*/,
                                       VIPRA::State const& state)
{
    std::string json_data = "[";
    // Get state.velocities and state.positions each timestep.
    // Compile the state and velocities into a JSON string.
    for ( size_t i = 0; i < state.positions.size(); ++i ) {
        json_data += "{ \"position\": [ " + std::to_string(state.positions[i].x) + ", " +
                     std::to_string(state.positions[i].y) + ", " + std::to_string(state.positions[i].z) +
                     " ], \"velocity\": [ " + std::to_string(state.velocities[i].x) + ", " +
                     std::to_string(state.velocities[i].y) + ", " + std::to_string(state.velocities[i].z) +
                     " ] }";

        if ( i != state.positions.size() - 1 ) json_data += ',';
    }

    // TODO(tyler): If there are any issues, retry or close the server.

    json_data += "]";

    int result = send_data(_client_id, json_data);
    if (result == -1) {
        VIPRA_MODULE_ERROR("Failed to send data over TCP. Closing server.");
    }
}

int TCPHost::send_data(int client_id, const std::string& data) 
{
    ssize_t total_sent = 0;
    ssize_t data_length = data.size();
    const char* data_ptr = data.c_str();

    while (total_sent < data_length) {
        ssize_t sent = send(client_id, data_ptr + total_sent, data_length - total_sent, 0);
        if (sent == -1) {
            return -1;
        }
        total_sent += sent;
    }

    return total_sent;
}

int TCPHost::create_server() 
{
    VIPRA::Log::debug("Creating TCP Server on {}:{}", _host, _port);
    // TODO(tyler): Implement server creation logic here.
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1) {
        VIPRA_MODULE_ERROR("Failed to create socket for server.");
        return -1;
    }

    VIPRA::Log::debug("Socket created for TCP server.");

    int reuse = 1;
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        VIPRA_MODULE_ERROR("Failed to set socket options for TCP server.");
        close(_server_fd);
        return -1;
    }

    VIPRA::Log::debug("Socket options set for TCP server.");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_addr.s_addr = inet_addr(HOST);
    server_addr.sin_port = htons(PORT);

    if (bind(_server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        VIPRA_MODULE_ERROR("Failed to bind TCP server to port {}.", PORT);
        close(_server_fd);
        return -1;
    }

    VIPRA::Log::debug("TCP Server bound to port {}.", PORT);

    if ((listen(_server_fd, 3)) < 0) {
        VIPRA_MODULE_ERROR("Failed to listen on TCP server socket.");
        close(_server_fd);
        return -1;
    }

    VIPRA::Log::debug("TCP Server listening for connections.");

    int client_fd = accept(_server_fd, NULL, NULL);
    if (client_fd < 0) {
        VIPRA_MODULE_ERROR("Failed to accept connection on TCP server.");
        close(_server_fd);
        return -1;
    }

    VIPRA::Log::debug("TCP Server created and client connected.");

    return client_fd;
} 

void close_server(int server_fd, int client_fd) 
{
    close(client_fd);
    close(server_fd);
}
}  // namespace VIPRA::Output
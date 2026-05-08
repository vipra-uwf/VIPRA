#pragma once

#include <cstdio>

// #include <nlohmann/json.hpp>

#include "vipra/macros/output.hpp"
#include "vipra/modules/output.hpp"

#include "vipra/macros/errors.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

// TODO(tyler): We need to start a server somewhere, but not sure how we can initialize that as a module.

// TODO(tyler):  should these be registered parameters?
#define PORT 5000
#define BUFFER_SIZE 1024
#define HOST "127.0.0.1"

namespace VIPRA::Output {
/**
 * @brief TCP output module for writing trajectories with a TCP connection.
 * 
 */
class TCPHost : public VIPRA::Modules::Module<TCPHost>,
                         public VIPRA::Modules::Output {
 public:
  VIPRA_MODULE_NAME("TCPHost")
  VIPRA_MODULE_TYPE(Output)

  VIPRA_REGISTER_PARAMS(
    VIPRA_PARAM("host", _host), //TODO(tyler): Add any additional parameters needed for TCP connection. Shouldn't need filename, but could use host or port.
    VIPRA_PARAM("port", _port)
  ) 

  VIPRA_OUTPUT_RESET override { }

  // void initialize() override;
  void write(std::filesystem::path const& outputDir) override;
  void timestep_update(VIPRA::timestep /*unused*/, VIPRA::delta_t /*unused*/,
                       VIPRA::State const& state) override;


 private:
  std::string                          _host;
  unsigned int                         _port;
  int                                  _client_id;
  int                                  _server_fd;
  int create_server();
  void close_server(int server_fd, int client_fd);
  int send_data(int client_id, const std::string& data);


};
}  // namespace VIPRA::Output
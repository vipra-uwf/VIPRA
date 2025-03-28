#pragma once

#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>

namespace VIPRA::Util {
inline void append_to_file(std::string const& filepath, std::string_view output)
{
  std::ofstream file(filepath, std::ios_base::app);

  if ( ! file.is_open() ) throw std::runtime_error("Unable to open file: " + filepath);

  file << output;
}
}  // namespace VIPRA::Util
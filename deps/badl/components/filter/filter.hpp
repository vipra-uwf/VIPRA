#pragma once

namespace BADL {
template <typename... stimulus_ts>
class AttentionFilter {
 public:
  template <typename stimulus_t>
  auto gate(stimulus_t const& stimulus) -> bool
  {
    // TODO(Rolland): figure this out
    return true;
  }

 private:
};
}  // namespace BADL
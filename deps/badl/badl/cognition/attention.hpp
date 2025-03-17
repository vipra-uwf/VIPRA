

#include "badl/components/base_component.hpp"
#include "badl/components/component.hpp"

namespace BADL {
template <typename... filter_ts>
class Attention : public BADL::Component,
                  public BADL::BaseComponent<Attention<filter_ts...>> {
 public:
  template <typename stimulus_t>
  [[nodiscard]] auto gate() -> bool
  {
    return true;
  }

 private:
  std::tuple<filter_ts...> _components;
};
}  // namespace BADL
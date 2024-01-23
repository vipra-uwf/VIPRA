#pragma once

#include <tuple>

#include "vipra/concepts/model.hpp"

namespace VIPRA {
template <Concepts::ModelModule... model_ts>
class Model {
  VIPRA_MODULE_TYPE(MODEL);

 public:
 private:
  std::tuple<model_ts...> _models;
};

CHECK_MODULE(ModelModule, Model<Concepts::DummyModel>);
}  // namespace VIPRA
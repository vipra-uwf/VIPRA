#pragma once

#define BADL_QUERY_GRAMMAR(queries...)                                         \
  [[nodiscard]] auto get_activation_grammar() const                            \
      -> std::vector<                                                          \
          std::tuple<std::string, std::string_view, BADL::QueryFunc>> override \
  {                                                                            \
    return {queries};                                                          \
  }

#define BADL_ACTIVATION_GRAMMAR(activations...)                       \
  [[nodiscard]] auto get_activation_grammar()                         \
      const -> std::vector<std::tuple<std::string, std::string_view,  \
                                      BADL::ActivationFunc>> override \
  {                                                                   \
    return {activations};                                             \
  }

#define BADL_ACTIVATION(name, grammar, method) \
  std::make_tuple(name, grammar, &method)

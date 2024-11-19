
#define VIPRA_COLLISION_UPDATE                                         \
  void update(VIPRA::Modules::Pedestrians const& pedset,               \
              VIPRA::Modules::Map const&         map,                  \
              VIPRA::Modules::Goals const& goals, VIPRA::State& state, \
              VIPRA::delta_t deltaT, VIPRA::timestep timestep)

#define VIPRA_COLLISION_INIT_STEP                            \
  void initialize(VIPRA::Modules::Pedestrians const& pedset, \
                  VIPRA::Modules::Map const&         map,    \
                  VIPRA::Modules::Goals const&       goals,  \
                  VIPRA::Random::Engine&             engine)

#pragma once

#include "vipra/modules/map.hpp"
#include "vipra/modules/model.hpp"

#include "vipra/macros/model.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/macros/performance.hpp"

#include "force_field.hpp"

#include "vipra/types/float.hpp"

namespace Model {
class SocialForce : public VIPRA::Modules::Module<SocialForce>, public VIPRA::Modules::Model {
    public:
        VIPRA_MODULE_NAME("SocialForce")
        VIPRA_MODULE_TYPE(Model)

        VIPRA_REGISTER_PARAMS(FORCE_FIELD_PARAMS)
    
        VIPRA_MODEL_RESET {}

        VIPRA_MODEL_INIT_STEP
        {
            // Initialize any necessary data structures or parameters here.
        }

        VIPRA_MODEL_TIMESTEP
        {
            // Put code for the timestep update here.
            update_state(pedset, map, goals, state, deltaT, timestep);
        }

    private:
        void update_state(VIPRA::Modules::Pedestrians const& pedset,
            VIPRA::Modules::Map const& map,
            VIPRA::Modules::Goals const& goals, 
            VIPRA::State& state,
            VIPRA::delta_t deltaT,
            VIPRA::timestep timestep);
        
        void update_positions(VIPRA::Modules::Map const& map, 
            VIPRA::State& state, 
            VIPRA::idx pedIdx, 
            VIPRA::f3d coord, 
            VIPRA::delta_t deltaT);

        // Additional helper functions go here.
        ADDITIONAL_SOCIAL_FORCE_FUNCTIONS

        // Functions for calculating the propulsion and repulsion forces.
        PROPULSION_FORCE
        REPULSION_FORCE
        FORCE_FIELD
        UPDATE_PED
    };



}   // namespace Model


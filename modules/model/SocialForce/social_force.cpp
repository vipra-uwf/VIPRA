#include "social_force.hpp"

VIPRA_REGISTER_MODULE(Model::SocialForce, Model)

void Model::SocialForce::update_state(VIPRA::Modules::Pedestrians const& pedset,
                                VIPRA::Modules::Map const& map,
                                VIPRA::Modules::Goals const& goals, 
                                VIPRA::State& state,
                                VIPRA::delta_t deltaT,
                                VIPRA::timestep timestep)
{
    VIPRA::size const pedCnt = pedset.num_pedestrians();
    auto const&       velocities = pedset.all_velocities();
    auto const&       coords = pedset.all_coords();

    auto const mean_mass = 1.0;

    for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
        if ( goals.is_goal_met(pedIdx) ) {
            state.velocities[pedIdx] = VIPRA::f3d{};
            continue;
        }

        VIPRA::f3d const   coord = coords[pedIdx];
        VIPRA::f3d const   velocity = velocities[pedIdx];

        VIPRA::f3d propulsionForce{};
        VIPRA::f3d repulsionForce{};
        VIPRA::f3d forceField{};

        // TODO: Here's the problem. If propulsion, repulsion, or force_field require additional parameters, 
        // we need to define those here. However, that would make this significantly less likely to be plug-and-play. 

        // If PROPULSION_FORCE, REPULSION_FORCE, and FORCE_FIELD are defined, use them here, otherwise, use the default.
        #ifdef PROPULSION_FORCE
            propulsionForce = propulsion(pedset, map, goals, state, deltaT, timestep, pedIdx);
        #else
            VIPRA::f3d const propulsion = (((direction * desiredSpeed * beta - velocity) * mass) / reactionTime);
        #endif

        #ifdef REPULSION_FORCE
            repulsionForce = repulsion(pedset, map, goals, state, deltaT, timestep, pedIdx);   
        #else
            VIPRA::f3d const repulsion = _config.frictionCoef * (velocity - ((velocity.dot(direction)) * direction));
        #endif

        #ifdef FORCE_FIELD
            forceField = force_field(pedset, map, goals, state, deltaT, pedIdx, timestep, propulsionForce, repulsionForce);
        #else
            VIPRA::f3d const forceField = propulsion - repulsion;
        #endif

        // TODO:  Handle Collision status

        update_ped(pedset, map, goals, state, deltaT, timestep, pedIdx, forceField);

        // state.velocities[pedIdx] = ((forceField / mean_mass) * deltaT) + velocity;
        // // TODO(tyler): Add Acceleration here. This should work, right? Should also create a state for this as well.
        // // state.accelerationsp[pedIdx] = velocity - state.velocities[pedIdx-1]/deltaT;
        // update_positions(map, state, pedIdx, coord, deltaT);
        // // state.positions[pedIdx] = coord + (state.velocities[pedIdx] * deltaT);

    }
}

void Model::SocialForce::update_positions(VIPRA::Modules::Map const& map, VIPRA::State& state, VIPRA::idx pedIdx, VIPRA::f3d current_position, VIPRA::delta_t deltaT) {
    VIPRA::f3d new_position = current_position + (state.velocities[pedIdx] * deltaT);

    VIPRA::f3d dimensions = map.get_dimensions();

    if (!map.collision(new_position) && new_position.x > 0.0 && new_position.y > 0.0 && new_position.x < dimensions.x && new_position.y < dimensions.y) {
        state.positions[pedIdx] = new_position;
    } else {
        // If there is a collision, we can either set the velocity to zero or try to find a new position that is not in collision.
        // For now, we will just set the velocity to zero and keep the position the same.
        state.velocities[pedIdx] = VIPRA::f3d{};
        state.positions[pedIdx] = current_position;
    }
}
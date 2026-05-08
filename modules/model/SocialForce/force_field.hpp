#pragma once
#include "vipra/geometry/f3d.hpp"
#include "vipra/modules/goals.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/modules/model.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/macros/model.hpp"

#define FORCE_FIELD_PARAMS

#define REPULSION_FORCE \
VIPRA::f3d repulsion(VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::Modules::Goals const& goals, VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep, VIPRA::idx pedIdx) { \
VIPRA::f3d total_repulsion{0.0, 0.0, 0.0}; \
VIPRA::f3d pos_alpha = pedset.ped_coords(pedIdx); \
VIPRA::f3d vel_alpha = pedset.ped_velocity(pedIdx); \
VIPRA::f_pnt A = 2000.0; \
VIPRA::f_pnt B = 0.08; \
VIPRA::f_pnt lambda = 0.5; \
for (VIPRA::idx beta = 0; beta < pedset.num_pedestrians(); ++beta) { \
  if (beta == pedIdx) continue; \
  VIPRA::f3d pos_beta = pedset.ped_coords(beta); \
  VIPRA::f3d vel_beta = pedset.ped_velocity(beta); \
  VIPRA::f3d force_beta = compute_repulsion_force_between_peds(pos_alpha, pos_beta, vel_alpha, vel_beta, A, B, lambda, deltaT); \
  total_repulsion = vector_add(total_repulsion, force_beta); \
} \
return total_repulsion;}


#define PROPULSION_FORCE \
VIPRA::f3d propulsion(VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::Modules::Goals const& goals, VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep, VIPRA::idx pedIdx) { \
VIPRA::f3d pos = pedset.ped_coords(pedIdx); \
VIPRA::f3d vel = pedset.ped_velocity(pedIdx); \
VIPRA::f3d goal = goals.current_goal(pedIdx); \
VIPRA::f3d direction = vector_subtract(goal, pos); \
VIPRA::f3d e_alpha = normalize_vector(direction); \
VIPRA::f_pnt v0 = 1.34; \
VIPRA::f_pnt tau = 0.5; \
VIPRA::f3d desired_velocity = vector_scale(e_alpha, v0); \
VIPRA::f3d velocity_diff = vector_subtract(desired_velocity, vel); \
VIPRA::f3d propulsion_force = vector_scale(velocity_diff, 1.0 / tau); \
return propulsion_force;}


#define FORCE_FIELD \
VIPRA::f3d force_field(VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::Modules::Goals const& goals, VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep, VIPRA::idx pedIdx, VIPRA::f3d propulsion, VIPRA::f3d repulsion) { \
VIPRA::f3d total_force = vector_add(propulsion, repulsion); \
return total_force;}


#define UPDATE_PED \
void update_ped(VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::Modules::Goals const& goals, VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep, VIPRA::idx pedIdx, VIPRA::f3d force) { \
VIPRA::f3d acceleration = force; \
VIPRA::f3d current_velocity = state.velocities[pedIdx]; \
VIPRA::f3d new_velocity = vector_add(current_velocity, vector_scale(acceleration, deltaT)); \
state.velocities[pedIdx] = new_velocity; \
VIPRA::f3d current_position = state.positions[pedIdx]; \
VIPRA::f3d new_position = vector_add(current_position, vector_scale(new_velocity, deltaT)); \
state.positions[pedIdx] = new_position;}


#define ADDITIONAL_SOCIAL_FORCE_FUNCTIONS \
VIPRA::f3d normalize_vector(VIPRA::f3d const& vec) {\
VIPRA::f_pnt mag = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); \
if (mag < 1e-12) { \
  return VIPRA::f3d{0.0, 0.0, 0.0}; \
} \
return VIPRA::f3d{vec.x / mag, vec.y / mag, vec.z / mag}; \
}\
VIPRA::f_pnt vector_magnitude(VIPRA::f3d const& vec) {\
return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); \
}\
VIPRA::f3d vector_subtract(VIPRA::f3d const& a, VIPRA::f3d const& b) {\
return VIPRA::f3d{a.x - b.x, a.y - b.y, a.z - b.z}; \
}\
VIPRA::f3d vector_add(VIPRA::f3d const& a, VIPRA::f3d const& b) {\
return VIPRA::f3d{a.x + b.x, a.y + b.y, a.z + b.z}; \
}\
VIPRA::f3d vector_scale(VIPRA::f3d const& vec, VIPRA::f_pnt scalar) {\
return VIPRA::f3d{vec.x * scalar, vec.y * scalar, vec.z * scalar}; \
}\
VIPRA::f_pnt compute_angle_weight(VIPRA::f3d const& direction, VIPRA::f3d const& to_neighbor, VIPRA::f_pnt lambda) {\
VIPRA::f3d dir_norm = normalize_vector(direction); \
VIPRA::f3d to_norm = normalize_vector(to_neighbor); \
VIPRA::f_pnt cos_phi = dir_norm.x * to_norm.x + dir_norm.y * to_norm.y + dir_norm.z * to_norm.z; \
if (cos_phi < -1.0) cos_phi = -1.0; \
if (cos_phi > 1.0) cos_phi = 1.0; \
VIPRA::f_pnt w = lambda + (1.0 - lambda) * (1.0 + cos_phi) / 2.0; \
return w; \
}\
VIPRA::f_pnt compute_b_parameter(VIPRA::f3d const& d_vec, VIPRA::f3d const& y_vec) {\
VIPRA::f_pnt d_mag = vector_magnitude(d_vec); \
VIPRA::f3d d_minus_y = vector_subtract(d_vec, y_vec); \
VIPRA::f_pnt d_minus_y_mag = vector_magnitude(d_minus_y); \
VIPRA::f_pnt y_mag = vector_magnitude(y_vec); \
VIPRA::f_pnt sum_term = d_mag + d_minus_y_mag; \
VIPRA::f_pnt diff_term = y_mag * y_mag; \
VIPRA::f_pnt b_val = 0.5 * std::sqrt(sum_term * sum_term - diff_term); \
return b_val; \
}\
VIPRA::f3d compute_repulsion_force_between_peds(VIPRA::f3d const& pos_alpha, VIPRA::f3d const& pos_beta, VIPRA::f3d const& vel_alpha, VIPRA::f3d const& vel_beta, VIPRA::f_pnt A, VIPRA::f_pnt B, VIPRA::f_pnt lambda, VIPRA::delta_t deltaT) {\
VIPRA::f3d d_vec = vector_subtract(pos_beta, pos_alpha); \
VIPRA::f_pnt d_mag = vector_magnitude(d_vec); \
if (d_mag < 1e-12) { \
  return VIPRA::f3d{0.0, 0.0, 0.0}; \
} \
VIPRA::f3d rel_vel = vector_subtract(vel_beta, vel_alpha); \
VIPRA::f3d y_vec = vector_scale(rel_vel, deltaT); \
VIPRA::f_pnt b = compute_b_parameter(d_vec, y_vec); \
if (b < 1e-12) { \
  return VIPRA::f3d{0.0, 0.0, 0.0}; \
} \
VIPRA::f3d d_minus_y = vector_subtract(d_vec, y_vec); \
VIPRA::f_pnt d_minus_y_mag = vector_magnitude(d_minus_y); \
VIPRA::f3d d_norm = normalize_vector(d_vec); \
VIPRA::f3d d_minus_y_norm = (d_minus_y_mag > 1e-12) ? normalize_vector(d_minus_y) : VIPRA::f3d{0.0, 0.0, 0.0}; \
VIPRA::f3d avg_direction = vector_scale(vector_add(d_norm, d_minus_y_norm), 0.5); \
VIPRA::f_pnt magnitude = A * std::exp(-b / B) * (d_mag + d_minus_y_mag) / (2.0 * b); \
VIPRA::f3d force_direction = vector_scale(avg_direction, magnitude); \
VIPRA::f_pnt angle_weight = compute_angle_weight(normalize_vector(vel_alpha), d_vec, lambda); \
VIPRA::f3d final_force = vector_scale(force_direction, angle_weight); \
return final_force; \
}\


/* Notes: 
Parameters assumed for implementation: A = 2000.0 N (interaction strength for pedestrian-pedestrian repulsion), B = 0.08 m (interaction range), lambda = 0.5 (anisotropic parameter for field of view), tau = 0.5 s (relaxation time for propulsion), v0 = 1.34 m/s (desired velocity from equation 1). These are typical values from social force models. The pedestrian-wall repulsion forces (f_alpha_i from equation 3) are not implemented as the Map class does not provide obstacle positions or geometries directly accessible for vectorial force computation. Equation 7 is a metric for trajectory evaluation and not a force, so it is not implemented. The stochastic term xi_alpha(t) from equation 2 is not implemented as no random number generation facilities or noise parameters were specified. */
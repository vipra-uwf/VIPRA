#pragma once

#ifdef BADL_BUILD_MACRO_ON

#define BADL_STIMULI_TYPES Sight, Sound
#define BADL_AGENT_THOUGHT_PROCESS_COMPONENTS Behaviors
#define BADL_AGENT_ACTUATOR_COMPONENTS Legs, Mouth, Brain
#define BADL_AGENT_ATTENTION_COMPONENTS Filter
#define BADL_AGENT_INTERNAL_STATE_COMPONENTS Beliefs, Memory
#define BADL_AGENT_SENSOR_COMPONENTS Eyes, Ears
#define BADL_PROGRAM_INTERFACE_INCLUDE @BADL_PROGRAM_INTERFACE_INCLUDE@

#else

#define BADL_STIMULI_TYPES
#define BADL_AGENT_COMPONENTS
#define BADL_AGENT_THOUGHT_PROCESS_COMPONENTS
#define BADL_AGENT_ACTUATOR_COMPONENTS
#define BADL_AGENT_ATTENTION_COMPONENTS
#define BADL_AGENT_INTERNAL_STATE_COMPONENTS
#define BADL_AGENT_SENSOR_COMPONENTS
#define BADL_PROGRAM_INTERFACE_INCLUDE

#endif
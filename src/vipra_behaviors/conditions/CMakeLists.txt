

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  condition.cpp
  sub_conditions/subcondition_attribute.cpp
  sub_conditions/subcondition_elapsed_time.cpp
  sub_conditions/subcondition_enter.cpp
  sub_conditions/subcondition_event_ending.cpp
  sub_conditions/subcondition_event_occurring.cpp
  sub_conditions/subcondition_event_starting.cpp
  sub_conditions/subcondition_event.cpp
  sub_conditions/subcondition_exists.cpp
  sub_conditions/subcondition_in_location.cpp
  sub_conditions/subcondition_leave.cpp
  sub_conditions/subcondition_spatial.cpp
)
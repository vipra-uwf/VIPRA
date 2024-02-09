grammar targets;

import common, directions, values, lexer_rules;

target:
  self |
  other (','? modifier)*
;

self:
  SELF
;

other:
  nearest_type
;

nearest_type:
  NEAREST id_list
;

modifier:
  direction |
  distance |
  location_modifier
;

location_modifier:
  IN LOC_NAME
;
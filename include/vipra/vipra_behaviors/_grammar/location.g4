grammar location;

import values, lexer_rules;

location:
  LOCATION ':' location_attribute*
;

location_attribute:
  loc_name |
  loc_dimensions
;

loc_name:
  NAME ':' ID
;

loc_dimensions:
  DIMENSIONS ':'
  ((loc_center loc_lengths loc_rotation) |
  (loc_center loc_rotation loc_lengths) |
  (loc_lengths loc_center loc_rotation) |
  (loc_lengths loc_rotation loc_center) |
  (loc_rotation loc_center loc_lengths) |
  (loc_rotation loc_lengths loc_center))
;

loc_center:
  CENTER ':' value_coord
;

loc_lengths:
  LENGTHS ':' value_coord
;

loc_rotation:
  ROTATION ':' value_numeric
;
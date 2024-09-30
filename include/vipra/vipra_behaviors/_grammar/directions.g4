grammar directions;

import values, lexer_rules;

distance:
  WITHIN value_numeric ('M'|'m')
;

direction:
  front |
  behind
;

front:
  IN FRONT
;

behind:
  BEHIND
;
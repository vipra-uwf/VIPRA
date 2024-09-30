grammar event;

import lexer_rules, condition;

event:
  EVENT ':' event_attribute*
;

event_attribute:
  event_name |
  event_start |
  event_end
;

event_name:
  NAME ':' ID
;

event_start:
  START ':' condition
;

event_end:
  END ':' condition
;

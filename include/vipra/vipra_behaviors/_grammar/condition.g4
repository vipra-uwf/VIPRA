grammar condition;

import lexer_rules, values, time, attributes, targets;

condition:
  unary |
  unary AND condition |
  unary OR condition
  ;

unary:
  primary
  ;

primary:
  '(' condition ')' |
  sub_condition
  ;

sub_condition:
  condition_Time_Elapsed_From_Event |
  condition_Event_Occurred |
  condition_Event_Occurring |
  condition_Event_Starting |
  condition_Event_Ending |
  condition_Spatial |
  condition_Enter_Location |
  condition_Exit_Location |
  condition_Inside_Location |
  condition_Attribute |
  condition_Exists
  ;

condition_Time_Elapsed_From_Event:
  value_numeric SECONDS AFTER EVNT
  ;

condition_Event_Occurred:
  EVNT HAS NOT? OCCURRED
  ;

condition_Event_Occurring:
  EVNT NOT? OCCURRING
  ;

condition_Event_Starting:
  EVNT STARTING
  ;

condition_Event_Ending:
  EVNT ENDING
  ;

condition_Spatial:
  TARGET WITHIN value_numeric ('m'|'M')
  ;

condition_Enter_Location:
  ENTER LOC_NAME
  ;

condition_Exit_Location:
  LEAVE LOC_NAME
  ;

condition_Inside_Location:
  IN LOC_NAME
  ;

condition_Attribute:
  TARGET? attribute IS (NOT?) attr_value
  ;

condition_Exists:
  EXISTS ':' ID (','? modifier)* WHOSE attribute IS (NOT?) attr_value
  ;
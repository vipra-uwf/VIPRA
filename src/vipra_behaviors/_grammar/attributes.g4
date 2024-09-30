grammar attributes;

import lexer_rules, values;


attribute:
  POSITION  |
  VELOCITY  |
  GOAL      |
  end_goal  |
  STATE     |
  LOCATION  |
  STATUS
  ;

attr_value:
  value_numeric |
  value_coord   |
  STATE_VAL |
  LOC_NAME |
  towards
  ;

end_goal:
  END GOAL
  ;

towards:
  TOWARDS (LOC_NAME | attribute)
;
grammar values;

import lexer_rules;

value_numeric:
  value_number |
  value_float |
  value_range |
  value_random
  ;

value_number:
  NUMBER
  ;

value_float:
  FLOAT
  ;

value_range:
  float_range |
  number_range
  ;

value_random:
  random_float |
  random_number
  ;

random_number:
  RANDOM number_range
  ;

random_float:
  RANDOM float_range
  ;

float_range:
  FLOAT TO FLOAT
  ;

number_range:
  NUMBER TO NUMBER
  ;

value_coord:
  '{' value_numeric (',' value_numeric (',' value_numeric)?)? '}'
  ;
grammar time;

import lexer_rules, values;

duration:
  value_numeric (SECONDS | SECOND)
  ;
grammar declarations;

import lexer_rules, values;

declaration:
  decl_Ped_State |
  decl_Ped
;

decl_Ped_State:
  STATES ':' ID (COMMA? ID)*;

decl_Ped:
  TYPES ':' ID (COMMA? ID)*;
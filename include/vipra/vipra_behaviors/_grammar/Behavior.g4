grammar Behavior;

import lexer_rules, location, event, selector, condition, action, declarations;

program: 
  decl_Ped '.' 
  statement+;

statement:
  event '.' |
  location '.' |
  declaration '.' |
  ped_Selector '.' |
  action '.'
  ;
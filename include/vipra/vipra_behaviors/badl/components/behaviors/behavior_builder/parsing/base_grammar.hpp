#pragma once

const char* const FUNC_CALL_GRAMMAR = R"(
start <- (Type / word)*
Type <- (String / Int / Float)
String <- '%str'
Int <- '%int'
Float <- '%flt'
word <- ('a'i / 'b'i / 'c'i / 'd'i / 'e'i / 'f'i / 'g'i / 'h'i / 'i'i / 'j'i / 'k'i / 'l'i / 'm'i / 'n'i / 'o'i / 'p'i / 'q'i / 'r'i / 's'i / 't'i / 'u'i / 'v'i / 'w'i / 'x'i / 'y'i / 'z'i)+
%whitespace <- [ \t]*
)";

const char* const BASE_DSL_GRAMMAR = R"(
  start <- Behavior+
  Behavior <- 'behavior'i ':' Component+
  Component <- (Action / Condition)+
  Action <- 'action'i ':' #actuator_calls#
  Condition <- 'condition'i ':' #query_calls#
  Flt <- '-'?Pos_Int'.'Pos_Int
  Int <- Pos_Int/Neg_Int
  Neg_Int <- '-'Pos_Int
  Pos_Int <- ('0'/'1'/'2'/'3'/'4'/'5'/'6'/'7'/'8'/'9')+
  ID <- ('a'i / 'b'i / 'c'i / 'd'i / 'e'i / 'f'i / 'g'i / 'h'i / 'i'i / 'j'i / 'k'i / 'l'i / 'm'i / 'n'i / 'o'i / 'p'i / 'q'i / 'r'i / 's'i / 't'i / 'u'i / 'v'i / 'w'i / 'x'i / 'y'i / 'z'i)+
  %whitespace <- [ \t\r\n]*
)";

// Condition <- 'condition'i ':' #conditions#
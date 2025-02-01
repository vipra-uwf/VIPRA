#pragma once

// TODO(rolland): add in substitution for strings to ids. like %id

const char* const FUNC_CALL_GRAMMAR = R"(
start <- (Type / Word / Punc / Num)*
Num <- < [0-9]+ >
Word <- < [a-z]+ >
Punc <- < [,.;:]+ >
%whitespace <- [ \t]*

Type <- (String / Int / Float)
String <- '%str'
Int <- '%int'
Float <- '%flt'
)";

const char* const BASE_DSL_GRAMMAR = R"(
start <- Behavior
Num <- < [0-9]+ >
ID <- < [a-z]+ >
Punc <- < [,.;:]+ >
%whitespace <- [ \t\r\n]*

Int <- < '-'? [0-9]+ >

Flt <- < Int'.'Num >
  
Behavior <- 'behavior'i ':' Component+
Component <- (Action / Condition)

Action <- 'action'i ':' #activation_calls#
Condition <- 'condition'i ':' #query_calls#
)";

// Condition <- 'condition'i ':' #conditions#
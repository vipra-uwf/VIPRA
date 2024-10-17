
// Generated from Behavior.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  BehaviorLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, COMMENT = 9, LINE_COMMENT = 10, WS = 11, COMMA = 12, COLON = 13, 
    STATE_VAL = 14, EVNT = 15, LOC_NAME = 16, AN = 17, AT = 18, ADD = 19, 
    AND = 20, ARE = 21, AFTER = 22, ALWAYS = 23, AROUND = 24, ACTION = 25, 
    BEHIND = 26, BOTTOM = 27, CONSIDER = 28, CIRCLE = 29, CENTER = 30, CONDITION = 31, 
    DECIMAL = 32, DIRECTION = 33, DURATION = 34, END = 35, ENDS = 36, ENVIRONMENT = 37, 
    ENTER = 38, EXIT = 39, EXISTS = 40, EVENT = 41, EXACTLY = 42, EVERY = 43, 
    EVERYONE = 44, ENDING = 45, FOR = 46, FROM = 47, FRONT = 48, FASTER = 49, 
    GIVEN = 50, GOAL = 51, GROUP = 52, HAS = 53, IN = 54, INTEGER = 55, 
    INITIAL = 56, IS = 57, IF = 58, LENGTH = 59, LENGTHS = 60, LEFT = 61, 
    LEAVE = 62, DIMENSIONS = 63, LOCATION = 64, MAX = 65, METERS = 66, NAME = 67, 
    NEAREST = 68, NORMAL = 69, NONE = 70, NOT = 71, OBSTACLE = 72, OCCUR = 73, 
    OCCURRED = 74, OCCURRING = 75, OF = 76, ON = 77, OR = 78, PEDESTRIAN = 79, 
    PEDESTRIANS = 80, POINTS = 81, POSSIBLE = 82, POSITION = 83, RADIUS = 84, 
    RANDOM = 85, REQUIRED = 86, RECTANGLE = 87, RIGHT = 88, RESPONSE = 89, 
    ROTATION = 90, SCALE = 91, SELF = 92, SELECT = 93, SELECTOR = 94, SET = 95, 
    SECOND = 96, SECONDS = 97, STATUS = 98, STARTING = 99, SLOWER = 100, 
    SHAPE = 101, STATES = 102, START = 103, STATE = 104, STARTS = 105, SPEED = 106, 
    SUBTRACT = 107, STIMULUS = 108, TARGET = 109, TO = 110, TOWARDS = 111, 
    TOP = 112, THE = 113, THAT = 114, THEN = 115, THEIR = 116, THERES = 117, 
    TYPES = 118, TYPE = 119, UNCONDITIONALLY = 120, VALUE = 121, VELOCITY = 122, 
    WHEN = 123, WHILE = 124, WHOSE = 125, WIDTH = 126, WILL = 127, WITH = 128, 
    WITHIN = 129, ID = 130, FLOAT = 131, NUMBER = 132
  };

  explicit BehaviorLexer(antlr4::CharStream *input);

  ~BehaviorLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};


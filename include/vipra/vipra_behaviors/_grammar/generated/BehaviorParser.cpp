
// Generated from Behavior.g4 by ANTLR 4.13.2

#include "BehaviorVisitor.h"

#include "BehaviorParser.h"

using namespace antlrcpp;

using namespace antlr4;

namespace {

struct BehaviorParserStaticData final {
  BehaviorParserStaticData(std::vector<std::string> ruleNames, std::vector<std::string> literalNames,
                           std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)),
        literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames)
  {
  }

  BehaviorParserStaticData(const BehaviorParserStaticData&) = delete;
  BehaviorParserStaticData(BehaviorParserStaticData&&) = delete;
  BehaviorParserStaticData& operator=(const BehaviorParserStaticData&) = delete;
  BehaviorParserStaticData& operator=(BehaviorParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA>       decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string>      ruleNames;
  const std::vector<std::string>      literalNames;
  const std::vector<std::string>      symbolicNames;
  const antlr4::dfa::Vocabulary       vocabulary;
  antlr4::atn::SerializedATNView      serializedATN;
  std::unique_ptr<antlr4::atn::ATN>   atn;
};

::antlr4::internal::OnceFlag behaviorParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<BehaviorParserStaticData> behaviorParserStaticData = nullptr;

void behaviorParserInitialize()
{
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if ( behaviorParserStaticData != nullptr ) {
    return;
  }
#else
  assert(behaviorParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<BehaviorParserStaticData>(
    std::vector<std::string>{
      "program", "statement", "location", "location_attribute", "loc_name", 
      "loc_dimensions", "loc_center", "loc_lengths", "loc_rotation", "value_numeric", 
      "value_number", "value_float", "value_range", "value_random", "random_number", 
      "random_float", "float_range", "number_range", "value_coord", "event", 
      "event_attribute", "event_name", "event_start", "event_end", "condition", 
      "unary", "primary", "sub_condition", "condition_Time_Elapsed_From_Event", 
      "condition_Event_Occurred", "condition_Event_Occurring", "condition_Event_Starting", 
      "condition_Event_Ending", "condition_Spatial", "condition_Enter_Location", 
      "condition_Exit_Location", "condition_Inside_Location", "condition_Attribute", 
      "condition_Exists", "duration", "attribute", "attr_value", "end_goal", 
      "towards", "target", "self", "other", "nearest_type", "modifier", 
      "location_modifier", "id_list", "distance", "direction", "front", 
      "behind", "ped_Selector", "selector_attribute", "selector_type", "selector_selector", 
      "selector_from", "selector_required", "selector", "selector_Percent", 
      "selector_Exactly_N_Random", "selector_Everyone", "selector_Location", 
      "group", "action", "action_attribute", "action_stimulus", "action_response", 
      "action_duration", "action_target", "sub_action", "action_atom", "set_atom", 
      "scale_atom", "set_objective_atom", "declaration", "decl_Ped_State", 
      "decl_Ped"
    },
    std::vector<std::string>{
      "", "'.'", "'{'", "'}'", "'('", "')'", "'m'", "'M'", "'%'", "", "", 
      "", "','", "':'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "COMMENT", "LINE_COMMENT", "WS", 
      "COMMA", "COLON", "STATE_VAL", "EVNT", "LOC_NAME", "AN", "AT", "ADD", 
      "AND", "ARE", "AFTER", "ALWAYS", "AROUND", "ACTION", "BEHIND", "BOTTOM", 
      "CONSIDER", "CIRCLE", "CENTER", "CONDITION", "DECIMAL", "DIRECTION", 
      "DURATION", "END", "ENDS", "ENVIRONMENT", "ENTER", "EXIT", "EXISTS", 
      "EVENT", "EXACTLY", "EVERY", "EVERYONE", "ENDING", "FOR", "FROM", 
      "FRONT", "FASTER", "GIVEN", "GOAL", "GROUP", "HAS", "IN", "INTEGER", 
      "INITIAL", "IS", "IF", "LENGTH", "LENGTHS", "LEFT", "LEAVE", "DIMENSIONS", 
      "LOCATION", "MAX", "METERS", "NAME", "NEAREST", "NORMAL", "NONE", 
      "NOT", "OBSTACLE", "OCCUR", "OCCURRED", "OCCURRING", "OF", "ON", "OR", 
      "PEDESTRIAN", "PEDESTRIANS", "POINTS", "POSSIBLE", "POSITION", "RADIUS", 
      "RANDOM", "REQUIRED", "RECTANGLE", "RIGHT", "RESPONSE", "ROTATION", 
      "SCALE", "SELF", "SELECT", "SELECTOR", "SET", "SECOND", "SECONDS", 
      "STATUS", "STARTING", "SLOWER", "SHAPE", "STATES", "START", "STATE", 
      "STARTS", "SPEED", "SUBTRACT", "STIMULUS", "TARGET", "TO", "TOWARDS", 
      "TOP", "THE", "THAT", "THEN", "THEIR", "THERES", "TYPES", "TYPE", 
      "UNCONDITIONALLY", "VALUE", "VELOCITY", "WHEN", "WHILE", "WHOSE", 
      "WIDTH", "WILL", "WITH", "WITHIN", "ID", "FLOAT", "NUMBER"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,132,633,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,2,77,7,
  	77,2,78,7,78,2,79,7,79,2,80,7,80,1,0,1,0,1,0,4,0,166,8,0,11,0,12,0,167,
  	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,185,8,
  	1,1,2,1,2,1,2,5,2,190,8,2,10,2,12,2,193,9,2,1,3,1,3,3,3,197,8,3,1,4,1,
  	4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,
  	1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,229,8,5,1,6,1,6,1,6,1,
  	6,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,3,9,247,8,9,1,10,1,
  	10,1,11,1,11,1,12,1,12,3,12,255,8,12,1,13,1,13,3,13,259,8,13,1,14,1,14,
  	1,14,1,15,1,15,1,15,1,16,1,16,1,16,1,16,1,17,1,17,1,17,1,17,1,18,1,18,
  	1,18,1,18,1,18,1,18,3,18,281,8,18,3,18,283,8,18,1,18,1,18,1,19,1,19,1,
  	19,5,19,290,8,19,10,19,12,19,293,9,19,1,20,1,20,1,20,3,20,298,8,20,1,
  	21,1,21,1,21,1,21,1,22,1,22,1,22,1,22,1,23,1,23,1,23,1,23,1,24,1,24,1,
  	24,1,24,1,24,1,24,1,24,1,24,1,24,3,24,321,8,24,1,25,1,25,1,26,1,26,1,
  	26,1,26,1,26,3,26,330,8,26,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,
  	27,1,27,1,27,3,27,343,8,27,1,28,1,28,1,28,1,28,1,28,1,29,1,29,1,29,3,
  	29,353,8,29,1,29,1,29,1,30,1,30,3,30,359,8,30,1,30,1,30,1,31,1,31,1,31,
  	1,32,1,32,1,32,1,33,1,33,1,33,1,33,1,33,1,34,1,34,1,34,1,35,1,35,1,35,
  	1,36,1,36,1,36,1,37,3,37,384,8,37,1,37,1,37,1,37,3,37,389,8,37,1,37,1,
  	37,1,38,1,38,1,38,1,38,3,38,397,8,38,1,38,5,38,400,8,38,10,38,12,38,403,
  	9,38,1,38,1,38,1,38,1,38,3,38,409,8,38,1,38,1,38,1,39,1,39,1,39,1,40,
  	1,40,1,40,1,40,1,40,1,40,1,40,3,40,423,8,40,1,41,1,41,1,41,1,41,1,41,
  	1,41,3,41,431,8,41,1,42,1,42,1,42,1,43,1,43,1,43,3,43,439,8,43,1,44,1,
  	44,1,44,3,44,444,8,44,1,44,5,44,447,8,44,10,44,12,44,450,9,44,3,44,452,
  	8,44,1,45,1,45,1,46,1,46,1,47,1,47,1,47,1,48,1,48,1,48,3,48,464,8,48,
  	1,49,1,49,1,49,1,50,1,50,3,50,471,8,50,1,50,5,50,474,8,50,10,50,12,50,
  	477,9,50,1,51,1,51,1,51,1,51,1,52,1,52,3,52,485,8,52,1,53,1,53,1,53,1,
  	54,1,54,1,55,1,55,1,55,5,55,495,8,55,10,55,12,55,498,9,55,1,56,1,56,1,
  	56,1,56,3,56,504,8,56,1,57,1,57,1,57,1,57,1,58,1,58,1,58,1,58,1,59,1,
  	59,1,59,1,59,1,60,1,60,1,61,1,61,1,61,1,61,3,61,524,8,61,1,62,1,62,1,
  	62,1,63,1,63,1,64,1,64,1,65,1,65,1,65,1,66,1,66,1,67,1,67,1,67,1,67,1,
  	67,1,67,5,67,544,8,67,10,67,12,67,547,9,67,1,68,1,68,1,68,1,68,3,68,553,
  	8,68,1,69,1,69,1,69,1,69,1,70,1,70,1,70,1,70,1,71,1,71,1,71,1,71,1,72,
  	1,72,1,72,1,72,1,73,1,73,1,73,5,73,574,8,73,10,73,12,73,577,9,73,1,74,
  	1,74,1,74,3,74,582,8,74,1,75,1,75,3,75,586,8,75,1,75,1,75,1,75,1,75,1,
  	76,1,76,3,76,594,8,76,1,76,1,76,1,76,1,77,1,77,1,77,1,77,1,77,1,77,1,
  	78,1,78,3,78,607,8,78,1,79,1,79,1,79,1,79,3,79,613,8,79,1,79,5,79,616,
  	8,79,10,79,12,79,619,9,79,1,80,1,80,1,80,1,80,3,80,625,8,80,1,80,5,80,
  	628,8,80,10,80,12,80,631,9,80,1,80,0,0,81,0,2,4,6,8,10,12,14,16,18,20,
  	22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,
  	68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,
  	112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,
  	148,150,152,154,156,158,160,0,4,1,0,6,7,1,0,96,97,2,0,79,79,130,130,2,
  	0,79,80,130,130,634,0,162,1,0,0,0,2,184,1,0,0,0,4,186,1,0,0,0,6,196,1,
  	0,0,0,8,198,1,0,0,0,10,202,1,0,0,0,12,230,1,0,0,0,14,234,1,0,0,0,16,238,
  	1,0,0,0,18,246,1,0,0,0,20,248,1,0,0,0,22,250,1,0,0,0,24,254,1,0,0,0,26,
  	258,1,0,0,0,28,260,1,0,0,0,30,263,1,0,0,0,32,266,1,0,0,0,34,270,1,0,0,
  	0,36,274,1,0,0,0,38,286,1,0,0,0,40,297,1,0,0,0,42,299,1,0,0,0,44,303,
  	1,0,0,0,46,307,1,0,0,0,48,320,1,0,0,0,50,322,1,0,0,0,52,329,1,0,0,0,54,
  	342,1,0,0,0,56,344,1,0,0,0,58,349,1,0,0,0,60,356,1,0,0,0,62,362,1,0,0,
  	0,64,365,1,0,0,0,66,368,1,0,0,0,68,373,1,0,0,0,70,376,1,0,0,0,72,379,
  	1,0,0,0,74,383,1,0,0,0,76,392,1,0,0,0,78,412,1,0,0,0,80,422,1,0,0,0,82,
  	430,1,0,0,0,84,432,1,0,0,0,86,435,1,0,0,0,88,451,1,0,0,0,90,453,1,0,0,
  	0,92,455,1,0,0,0,94,457,1,0,0,0,96,463,1,0,0,0,98,465,1,0,0,0,100,468,
  	1,0,0,0,102,478,1,0,0,0,104,484,1,0,0,0,106,486,1,0,0,0,108,489,1,0,0,
  	0,110,491,1,0,0,0,112,503,1,0,0,0,114,505,1,0,0,0,116,509,1,0,0,0,118,
  	513,1,0,0,0,120,517,1,0,0,0,122,523,1,0,0,0,124,525,1,0,0,0,126,528,1,
  	0,0,0,128,530,1,0,0,0,130,532,1,0,0,0,132,535,1,0,0,0,134,537,1,0,0,0,
  	136,552,1,0,0,0,138,554,1,0,0,0,140,558,1,0,0,0,142,562,1,0,0,0,144,566,
  	1,0,0,0,146,570,1,0,0,0,148,581,1,0,0,0,150,583,1,0,0,0,152,591,1,0,0,
  	0,154,598,1,0,0,0,156,606,1,0,0,0,158,608,1,0,0,0,160,620,1,0,0,0,162,
  	163,3,160,80,0,163,165,5,1,0,0,164,166,3,2,1,0,165,164,1,0,0,0,166,167,
  	1,0,0,0,167,165,1,0,0,0,167,168,1,0,0,0,168,1,1,0,0,0,169,170,3,38,19,
  	0,170,171,5,1,0,0,171,185,1,0,0,0,172,173,3,4,2,0,173,174,5,1,0,0,174,
  	185,1,0,0,0,175,176,3,156,78,0,176,177,5,1,0,0,177,185,1,0,0,0,178,179,
  	3,110,55,0,179,180,5,1,0,0,180,185,1,0,0,0,181,182,3,134,67,0,182,183,
  	5,1,0,0,183,185,1,0,0,0,184,169,1,0,0,0,184,172,1,0,0,0,184,175,1,0,0,
  	0,184,178,1,0,0,0,184,181,1,0,0,0,185,3,1,0,0,0,186,187,5,64,0,0,187,
  	191,5,13,0,0,188,190,3,6,3,0,189,188,1,0,0,0,190,193,1,0,0,0,191,189,
  	1,0,0,0,191,192,1,0,0,0,192,5,1,0,0,0,193,191,1,0,0,0,194,197,3,8,4,0,
  	195,197,3,10,5,0,196,194,1,0,0,0,196,195,1,0,0,0,197,7,1,0,0,0,198,199,
  	5,67,0,0,199,200,5,13,0,0,200,201,5,130,0,0,201,9,1,0,0,0,202,203,5,63,
  	0,0,203,228,5,13,0,0,204,205,3,12,6,0,205,206,3,14,7,0,206,207,3,16,8,
  	0,207,229,1,0,0,0,208,209,3,12,6,0,209,210,3,16,8,0,210,211,3,14,7,0,
  	211,229,1,0,0,0,212,213,3,14,7,0,213,214,3,12,6,0,214,215,3,16,8,0,215,
  	229,1,0,0,0,216,217,3,14,7,0,217,218,3,16,8,0,218,219,3,12,6,0,219,229,
  	1,0,0,0,220,221,3,16,8,0,221,222,3,12,6,0,222,223,3,14,7,0,223,229,1,
  	0,0,0,224,225,3,16,8,0,225,226,3,14,7,0,226,227,3,12,6,0,227,229,1,0,
  	0,0,228,204,1,0,0,0,228,208,1,0,0,0,228,212,1,0,0,0,228,216,1,0,0,0,228,
  	220,1,0,0,0,228,224,1,0,0,0,229,11,1,0,0,0,230,231,5,30,0,0,231,232,5,
  	13,0,0,232,233,3,36,18,0,233,13,1,0,0,0,234,235,5,60,0,0,235,236,5,13,
  	0,0,236,237,3,36,18,0,237,15,1,0,0,0,238,239,5,90,0,0,239,240,5,13,0,
  	0,240,241,3,18,9,0,241,17,1,0,0,0,242,247,3,20,10,0,243,247,3,22,11,0,
  	244,247,3,24,12,0,245,247,3,26,13,0,246,242,1,0,0,0,246,243,1,0,0,0,246,
  	244,1,0,0,0,246,245,1,0,0,0,247,19,1,0,0,0,248,249,5,132,0,0,249,21,1,
  	0,0,0,250,251,5,131,0,0,251,23,1,0,0,0,252,255,3,32,16,0,253,255,3,34,
  	17,0,254,252,1,0,0,0,254,253,1,0,0,0,255,25,1,0,0,0,256,259,3,30,15,0,
  	257,259,3,28,14,0,258,256,1,0,0,0,258,257,1,0,0,0,259,27,1,0,0,0,260,
  	261,5,85,0,0,261,262,3,34,17,0,262,29,1,0,0,0,263,264,5,85,0,0,264,265,
  	3,32,16,0,265,31,1,0,0,0,266,267,5,131,0,0,267,268,5,110,0,0,268,269,
  	5,131,0,0,269,33,1,0,0,0,270,271,5,132,0,0,271,272,5,110,0,0,272,273,
  	5,132,0,0,273,35,1,0,0,0,274,275,5,2,0,0,275,282,3,18,9,0,276,277,5,12,
  	0,0,277,280,3,18,9,0,278,279,5,12,0,0,279,281,3,18,9,0,280,278,1,0,0,
  	0,280,281,1,0,0,0,281,283,1,0,0,0,282,276,1,0,0,0,282,283,1,0,0,0,283,
  	284,1,0,0,0,284,285,5,3,0,0,285,37,1,0,0,0,286,287,5,41,0,0,287,291,5,
  	13,0,0,288,290,3,40,20,0,289,288,1,0,0,0,290,293,1,0,0,0,291,289,1,0,
  	0,0,291,292,1,0,0,0,292,39,1,0,0,0,293,291,1,0,0,0,294,298,3,42,21,0,
  	295,298,3,44,22,0,296,298,3,46,23,0,297,294,1,0,0,0,297,295,1,0,0,0,297,
  	296,1,0,0,0,298,41,1,0,0,0,299,300,5,67,0,0,300,301,5,13,0,0,301,302,
  	5,130,0,0,302,43,1,0,0,0,303,304,5,103,0,0,304,305,5,13,0,0,305,306,3,
  	48,24,0,306,45,1,0,0,0,307,308,5,35,0,0,308,309,5,13,0,0,309,310,3,48,
  	24,0,310,47,1,0,0,0,311,321,3,50,25,0,312,313,3,50,25,0,313,314,5,20,
  	0,0,314,315,3,48,24,0,315,321,1,0,0,0,316,317,3,50,25,0,317,318,5,78,
  	0,0,318,319,3,48,24,0,319,321,1,0,0,0,320,311,1,0,0,0,320,312,1,0,0,0,
  	320,316,1,0,0,0,321,49,1,0,0,0,322,323,3,52,26,0,323,51,1,0,0,0,324,325,
  	5,4,0,0,325,326,3,48,24,0,326,327,5,5,0,0,327,330,1,0,0,0,328,330,3,54,
  	27,0,329,324,1,0,0,0,329,328,1,0,0,0,330,53,1,0,0,0,331,343,3,56,28,0,
  	332,343,3,58,29,0,333,343,3,60,30,0,334,343,3,62,31,0,335,343,3,64,32,
  	0,336,343,3,66,33,0,337,343,3,68,34,0,338,343,3,70,35,0,339,343,3,72,
  	36,0,340,343,3,74,37,0,341,343,3,76,38,0,342,331,1,0,0,0,342,332,1,0,
  	0,0,342,333,1,0,0,0,342,334,1,0,0,0,342,335,1,0,0,0,342,336,1,0,0,0,342,
  	337,1,0,0,0,342,338,1,0,0,0,342,339,1,0,0,0,342,340,1,0,0,0,342,341,1,
  	0,0,0,343,55,1,0,0,0,344,345,3,18,9,0,345,346,5,97,0,0,346,347,5,22,0,
  	0,347,348,5,15,0,0,348,57,1,0,0,0,349,350,5,15,0,0,350,352,5,53,0,0,351,
  	353,5,71,0,0,352,351,1,0,0,0,352,353,1,0,0,0,353,354,1,0,0,0,354,355,
  	5,74,0,0,355,59,1,0,0,0,356,358,5,15,0,0,357,359,5,71,0,0,358,357,1,0,
  	0,0,358,359,1,0,0,0,359,360,1,0,0,0,360,361,5,75,0,0,361,61,1,0,0,0,362,
  	363,5,15,0,0,363,364,5,99,0,0,364,63,1,0,0,0,365,366,5,15,0,0,366,367,
  	5,45,0,0,367,65,1,0,0,0,368,369,5,109,0,0,369,370,5,129,0,0,370,371,3,
  	18,9,0,371,372,7,0,0,0,372,67,1,0,0,0,373,374,5,38,0,0,374,375,5,16,0,
  	0,375,69,1,0,0,0,376,377,5,62,0,0,377,378,5,16,0,0,378,71,1,0,0,0,379,
  	380,5,54,0,0,380,381,5,16,0,0,381,73,1,0,0,0,382,384,5,109,0,0,383,382,
  	1,0,0,0,383,384,1,0,0,0,384,385,1,0,0,0,385,386,3,80,40,0,386,388,5,57,
  	0,0,387,389,5,71,0,0,388,387,1,0,0,0,388,389,1,0,0,0,389,390,1,0,0,0,
  	390,391,3,82,41,0,391,75,1,0,0,0,392,393,5,40,0,0,393,394,5,13,0,0,394,
  	401,5,130,0,0,395,397,5,12,0,0,396,395,1,0,0,0,396,397,1,0,0,0,397,398,
  	1,0,0,0,398,400,3,96,48,0,399,396,1,0,0,0,400,403,1,0,0,0,401,399,1,0,
  	0,0,401,402,1,0,0,0,402,404,1,0,0,0,403,401,1,0,0,0,404,405,5,125,0,0,
  	405,406,3,80,40,0,406,408,5,57,0,0,407,409,5,71,0,0,408,407,1,0,0,0,408,
  	409,1,0,0,0,409,410,1,0,0,0,410,411,3,82,41,0,411,77,1,0,0,0,412,413,
  	3,18,9,0,413,414,7,1,0,0,414,79,1,0,0,0,415,423,5,83,0,0,416,423,5,122,
  	0,0,417,423,5,51,0,0,418,423,3,84,42,0,419,423,5,104,0,0,420,423,5,64,
  	0,0,421,423,5,98,0,0,422,415,1,0,0,0,422,416,1,0,0,0,422,417,1,0,0,0,
  	422,418,1,0,0,0,422,419,1,0,0,0,422,420,1,0,0,0,422,421,1,0,0,0,423,81,
  	1,0,0,0,424,431,3,18,9,0,425,431,3,36,18,0,426,431,5,14,0,0,427,431,5,
  	16,0,0,428,431,3,86,43,0,429,431,3,80,40,0,430,424,1,0,0,0,430,425,1,
  	0,0,0,430,426,1,0,0,0,430,427,1,0,0,0,430,428,1,0,0,0,430,429,1,0,0,0,
  	431,83,1,0,0,0,432,433,5,35,0,0,433,434,5,51,0,0,434,85,1,0,0,0,435,438,
  	5,111,0,0,436,439,5,16,0,0,437,439,3,80,40,0,438,436,1,0,0,0,438,437,
  	1,0,0,0,439,87,1,0,0,0,440,452,3,90,45,0,441,448,3,92,46,0,442,444,5,
  	12,0,0,443,442,1,0,0,0,443,444,1,0,0,0,444,445,1,0,0,0,445,447,3,96,48,
  	0,446,443,1,0,0,0,447,450,1,0,0,0,448,446,1,0,0,0,448,449,1,0,0,0,449,
  	452,1,0,0,0,450,448,1,0,0,0,451,440,1,0,0,0,451,441,1,0,0,0,452,89,1,
  	0,0,0,453,454,5,92,0,0,454,91,1,0,0,0,455,456,3,94,47,0,456,93,1,0,0,
  	0,457,458,5,68,0,0,458,459,3,100,50,0,459,95,1,0,0,0,460,464,3,104,52,
  	0,461,464,3,102,51,0,462,464,3,98,49,0,463,460,1,0,0,0,463,461,1,0,0,
  	0,463,462,1,0,0,0,464,97,1,0,0,0,465,466,5,54,0,0,466,467,5,16,0,0,467,
  	99,1,0,0,0,468,475,7,2,0,0,469,471,5,12,0,0,470,469,1,0,0,0,470,471,1,
  	0,0,0,471,472,1,0,0,0,472,474,7,2,0,0,473,470,1,0,0,0,474,477,1,0,0,0,
  	475,473,1,0,0,0,475,476,1,0,0,0,476,101,1,0,0,0,477,475,1,0,0,0,478,479,
  	5,129,0,0,479,480,3,18,9,0,480,481,7,0,0,0,481,103,1,0,0,0,482,485,3,
  	106,53,0,483,485,3,108,54,0,484,482,1,0,0,0,484,483,1,0,0,0,485,105,1,
  	0,0,0,486,487,5,54,0,0,487,488,5,48,0,0,488,107,1,0,0,0,489,490,5,26,
  	0,0,490,109,1,0,0,0,491,492,5,94,0,0,492,496,5,13,0,0,493,495,3,112,56,
  	0,494,493,1,0,0,0,495,498,1,0,0,0,496,494,1,0,0,0,496,497,1,0,0,0,497,
  	111,1,0,0,0,498,496,1,0,0,0,499,504,3,114,57,0,500,504,3,116,58,0,501,
  	504,3,118,59,0,502,504,3,120,60,0,503,499,1,0,0,0,503,500,1,0,0,0,503,
  	501,1,0,0,0,503,502,1,0,0,0,504,113,1,0,0,0,505,506,5,119,0,0,506,507,
  	5,13,0,0,507,508,3,100,50,0,508,115,1,0,0,0,509,510,5,93,0,0,510,511,
  	5,13,0,0,511,512,3,122,61,0,512,117,1,0,0,0,513,514,5,47,0,0,514,515,
  	5,13,0,0,515,516,3,132,66,0,516,119,1,0,0,0,517,518,5,86,0,0,518,121,
  	1,0,0,0,519,524,3,124,62,0,520,524,3,126,63,0,521,524,3,128,64,0,522,
  	524,3,130,65,0,523,519,1,0,0,0,523,520,1,0,0,0,523,521,1,0,0,0,523,522,
  	1,0,0,0,524,123,1,0,0,0,525,526,3,20,10,0,526,527,5,8,0,0,527,125,1,0,
  	0,0,528,529,3,20,10,0,529,127,1,0,0,0,530,531,5,44,0,0,531,129,1,0,0,
  	0,532,533,5,54,0,0,533,534,5,16,0,0,534,131,1,0,0,0,535,536,7,3,0,0,536,
  	133,1,0,0,0,537,538,5,25,0,0,538,539,5,4,0,0,539,540,5,130,0,0,540,541,
  	5,5,0,0,541,545,5,13,0,0,542,544,3,136,68,0,543,542,1,0,0,0,544,547,1,
  	0,0,0,545,543,1,0,0,0,545,546,1,0,0,0,546,135,1,0,0,0,547,545,1,0,0,0,
  	548,553,3,140,70,0,549,553,3,138,69,0,550,553,3,142,71,0,551,553,3,144,
  	72,0,552,548,1,0,0,0,552,549,1,0,0,0,552,550,1,0,0,0,552,551,1,0,0,0,
  	553,137,1,0,0,0,554,555,5,31,0,0,555,556,5,13,0,0,556,557,3,48,24,0,557,
  	139,1,0,0,0,558,559,5,89,0,0,559,560,5,13,0,0,560,561,3,146,73,0,561,
  	141,1,0,0,0,562,563,5,34,0,0,563,564,5,13,0,0,564,565,3,78,39,0,565,143,
  	1,0,0,0,566,567,5,109,0,0,567,568,5,13,0,0,568,569,3,88,44,0,569,145,
  	1,0,0,0,570,575,3,148,74,0,571,572,5,12,0,0,572,574,3,148,74,0,573,571,
  	1,0,0,0,574,577,1,0,0,0,575,573,1,0,0,0,575,576,1,0,0,0,576,147,1,0,0,
  	0,577,575,1,0,0,0,578,582,3,150,75,0,579,582,3,154,77,0,580,582,3,152,
  	76,0,581,578,1,0,0,0,581,579,1,0,0,0,581,580,1,0,0,0,582,149,1,0,0,0,
  	583,585,5,95,0,0,584,586,5,109,0,0,585,584,1,0,0,0,585,586,1,0,0,0,586,
  	587,1,0,0,0,587,588,3,80,40,0,588,589,5,110,0,0,589,590,3,82,41,0,590,
  	151,1,0,0,0,591,593,5,91,0,0,592,594,5,109,0,0,593,592,1,0,0,0,593,594,
  	1,0,0,0,594,595,1,0,0,0,595,596,3,80,40,0,596,597,3,82,41,0,597,153,1,
  	0,0,0,598,599,5,95,0,0,599,600,5,51,0,0,600,601,5,110,0,0,601,602,5,85,
  	0,0,602,603,5,16,0,0,603,155,1,0,0,0,604,607,3,158,79,0,605,607,3,160,
  	80,0,606,604,1,0,0,0,606,605,1,0,0,0,607,157,1,0,0,0,608,609,5,102,0,
  	0,609,610,5,13,0,0,610,617,5,130,0,0,611,613,5,12,0,0,612,611,1,0,0,0,
  	612,613,1,0,0,0,613,614,1,0,0,0,614,616,5,130,0,0,615,612,1,0,0,0,616,
  	619,1,0,0,0,617,615,1,0,0,0,617,618,1,0,0,0,618,159,1,0,0,0,619,617,1,
  	0,0,0,620,621,5,118,0,0,621,622,5,13,0,0,622,629,5,130,0,0,623,625,5,
  	12,0,0,624,623,1,0,0,0,624,625,1,0,0,0,625,626,1,0,0,0,626,628,5,130,
  	0,0,627,624,1,0,0,0,628,631,1,0,0,0,629,627,1,0,0,0,629,630,1,0,0,0,630,
  	161,1,0,0,0,631,629,1,0,0,0,46,167,184,191,196,228,246,254,258,280,282,
  	291,297,320,329,342,352,358,383,388,396,401,408,422,430,438,443,448,451,
  	463,470,475,484,496,503,523,545,552,575,581,585,593,606,612,617,624,629
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for ( size_t i = 0; i < count; i++ ) {
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  behaviorParserStaticData = std::move(staticData);
}

}  // namespace

BehaviorParser::BehaviorParser(TokenStream* input)
    : BehaviorParser(input, antlr4::atn::ParserATNSimulatorOptions())
{
}

BehaviorParser::BehaviorParser(TokenStream* input, const antlr4::atn::ParserATNSimulatorOptions& options)
    : Parser(input)
{
  BehaviorParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *behaviorParserStaticData->atn,
                                             behaviorParserStaticData->decisionToDFA,
                                             behaviorParserStaticData->sharedContextCache, options);
}

BehaviorParser::~BehaviorParser() { delete _interpreter; }

const atn::ATN& BehaviorParser::getATN() const { return *behaviorParserStaticData->atn; }

std::string BehaviorParser::getGrammarFileName() const { return "Behavior.g4"; }

const std::vector<std::string>& BehaviorParser::getRuleNames() const
{
  return behaviorParserStaticData->ruleNames;
}

const dfa::Vocabulary& BehaviorParser::getVocabulary() const { return behaviorParserStaticData->vocabulary; }

antlr4::atn::SerializedATNView BehaviorParser::getSerializedATN() const
{
  return behaviorParserStaticData->serializedATN;
}

//----------------- ProgramContext ------------------------------------------------------------------

BehaviorParser::ProgramContext::ProgramContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Decl_PedContext* BehaviorParser::ProgramContext::decl_Ped()
{
  return getRuleContext<BehaviorParser::Decl_PedContext>(0);
}

std::vector<BehaviorParser::StatementContext*> BehaviorParser::ProgramContext::statement()
{
  return getRuleContexts<BehaviorParser::StatementContext>();
}

BehaviorParser::StatementContext* BehaviorParser::ProgramContext::statement(size_t i)
{
  return getRuleContext<BehaviorParser::StatementContext>(i);
}

size_t BehaviorParser::ProgramContext::getRuleIndex() const { return BehaviorParser::RuleProgram; }

std::any BehaviorParser::ProgramContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::ProgramContext* BehaviorParser::program()
{
  ProgramContext* _localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, BehaviorParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(162);
    decl_Ped();
    setState(163);
    match(BehaviorParser::T__0);
    setState(165); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(164);
      statement();
      setState(167); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ( _la == BehaviorParser::ACTION

    || _la == BehaviorParser::EVENT || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 18014674461130753) != 0));
   
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

BehaviorParser::StatementContext::StatementContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::EventContext* BehaviorParser::StatementContext::event()
{
  return getRuleContext<BehaviorParser::EventContext>(0);
}

BehaviorParser::LocationContext* BehaviorParser::StatementContext::location()
{
  return getRuleContext<BehaviorParser::LocationContext>(0);
}

BehaviorParser::DeclarationContext* BehaviorParser::StatementContext::declaration()
{
  return getRuleContext<BehaviorParser::DeclarationContext>(0);
}

BehaviorParser::Ped_SelectorContext* BehaviorParser::StatementContext::ped_Selector()
{
  return getRuleContext<BehaviorParser::Ped_SelectorContext>(0);
}

BehaviorParser::ActionContext* BehaviorParser::StatementContext::action()
{
  return getRuleContext<BehaviorParser::ActionContext>(0);
}

size_t BehaviorParser::StatementContext::getRuleIndex() const { return BehaviorParser::RuleStatement; }

std::any BehaviorParser::StatementContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::StatementContext* BehaviorParser::statement()
{
  StatementContext* _localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, BehaviorParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(184);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::EVENT: {
        enterOuterAlt(_localctx, 1);
        setState(169);
        event();
        setState(170);
        match(BehaviorParser::T__0);
        break;
      }

      case BehaviorParser::LOCATION: {
        enterOuterAlt(_localctx, 2);
        setState(172);
        location();
        setState(173);
        match(BehaviorParser::T__0);
        break;
      }

      case BehaviorParser::STATES:
      case BehaviorParser::TYPES: {
        enterOuterAlt(_localctx, 3);
        setState(175);
        declaration();
        setState(176);
        match(BehaviorParser::T__0);
        break;
      }

      case BehaviorParser::SELECTOR: {
        enterOuterAlt(_localctx, 4);
        setState(178);
        ped_Selector();
        setState(179);
        match(BehaviorParser::T__0);
        break;
      }

      case BehaviorParser::ACTION: {
        enterOuterAlt(_localctx, 5);
        setState(181);
        action();
        setState(182);
        match(BehaviorParser::T__0);
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LocationContext ------------------------------------------------------------------

BehaviorParser::LocationContext::LocationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::LocationContext::LOCATION()
{
  return getToken(BehaviorParser::LOCATION, 0);
}

tree::TerminalNode* BehaviorParser::LocationContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

std::vector<BehaviorParser::Location_attributeContext*> BehaviorParser::LocationContext::location_attribute()
{
  return getRuleContexts<BehaviorParser::Location_attributeContext>();
}

BehaviorParser::Location_attributeContext* BehaviorParser::LocationContext::location_attribute(size_t i)
{
  return getRuleContext<BehaviorParser::Location_attributeContext>(i);
}

size_t BehaviorParser::LocationContext::getRuleIndex() const { return BehaviorParser::RuleLocation; }

std::any BehaviorParser::LocationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLocation(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::LocationContext* BehaviorParser::location()
{
  LocationContext* _localctx = _tracker.createInstance<LocationContext>(_ctx, getState());
  enterRule(_localctx, 4, BehaviorParser::RuleLocation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(186);
    match(BehaviorParser::LOCATION);
    setState(187);
    match(BehaviorParser::COLON);
    setState(191);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ( _la == BehaviorParser::DIMENSIONS

    || _la == BehaviorParser::NAME) {
      setState(188);
      location_attribute();
      setState(193);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Location_attributeContext ------------------------------------------------------------------

BehaviorParser::Location_attributeContext::Location_attributeContext(ParserRuleContext* parent,
                                                                     size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Loc_nameContext* BehaviorParser::Location_attributeContext::loc_name()
{
  return getRuleContext<BehaviorParser::Loc_nameContext>(0);
}

BehaviorParser::Loc_dimensionsContext* BehaviorParser::Location_attributeContext::loc_dimensions()
{
  return getRuleContext<BehaviorParser::Loc_dimensionsContext>(0);
}

size_t BehaviorParser::Location_attributeContext::getRuleIndex() const
{
  return BehaviorParser::RuleLocation_attribute;
}

std::any BehaviorParser::Location_attributeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLocation_attribute(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Location_attributeContext* BehaviorParser::location_attribute()
{
  Location_attributeContext* _localctx = _tracker.createInstance<Location_attributeContext>(_ctx, getState());
  enterRule(_localctx, 6, BehaviorParser::RuleLocation_attribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(196);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(194);
        loc_name();
        break;
      }

      case BehaviorParser::DIMENSIONS: {
        enterOuterAlt(_localctx, 2);
        setState(195);
        loc_dimensions();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Loc_nameContext ------------------------------------------------------------------

BehaviorParser::Loc_nameContext::Loc_nameContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Loc_nameContext::NAME() { return getToken(BehaviorParser::NAME, 0); }

tree::TerminalNode* BehaviorParser::Loc_nameContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

tree::TerminalNode* BehaviorParser::Loc_nameContext::ID() { return getToken(BehaviorParser::ID, 0); }

size_t BehaviorParser::Loc_nameContext::getRuleIndex() const { return BehaviorParser::RuleLoc_name; }

std::any BehaviorParser::Loc_nameContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLoc_name(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Loc_nameContext* BehaviorParser::loc_name()
{
  Loc_nameContext* _localctx = _tracker.createInstance<Loc_nameContext>(_ctx, getState());
  enterRule(_localctx, 8, BehaviorParser::RuleLoc_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(198);
    match(BehaviorParser::NAME);
    setState(199);
    match(BehaviorParser::COLON);
    setState(200);
    match(BehaviorParser::ID);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Loc_dimensionsContext ------------------------------------------------------------------

BehaviorParser::Loc_dimensionsContext::Loc_dimensionsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Loc_dimensionsContext::DIMENSIONS()
{
  return getToken(BehaviorParser::DIMENSIONS, 0);
}

tree::TerminalNode* BehaviorParser::Loc_dimensionsContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::Loc_centerContext* BehaviorParser::Loc_dimensionsContext::loc_center()
{
  return getRuleContext<BehaviorParser::Loc_centerContext>(0);
}

BehaviorParser::Loc_lengthsContext* BehaviorParser::Loc_dimensionsContext::loc_lengths()
{
  return getRuleContext<BehaviorParser::Loc_lengthsContext>(0);
}

BehaviorParser::Loc_rotationContext* BehaviorParser::Loc_dimensionsContext::loc_rotation()
{
  return getRuleContext<BehaviorParser::Loc_rotationContext>(0);
}

size_t BehaviorParser::Loc_dimensionsContext::getRuleIndex() const
{
  return BehaviorParser::RuleLoc_dimensions;
}

std::any BehaviorParser::Loc_dimensionsContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLoc_dimensions(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Loc_dimensionsContext* BehaviorParser::loc_dimensions()
{
  Loc_dimensionsContext* _localctx = _tracker.createInstance<Loc_dimensionsContext>(_ctx, getState());
  enterRule(_localctx, 10, BehaviorParser::RuleLoc_dimensions);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(202);
    match(BehaviorParser::DIMENSIONS);
    setState(203);
    match(BehaviorParser::COLON);
    setState(228);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      setState(204);
      loc_center();
      setState(205);
      loc_lengths();
      setState(206);
      loc_rotation();
      break;
    }

    case 2: {
      setState(208);
      loc_center();
      setState(209);
      loc_rotation();
      setState(210);
      loc_lengths();
      break;
    }

    case 3: {
      setState(212);
      loc_lengths();
      setState(213);
      loc_center();
      setState(214);
      loc_rotation();
      break;
    }

    case 4: {
      setState(216);
      loc_lengths();
      setState(217);
      loc_rotation();
      setState(218);
      loc_center();
      break;
    }

    case 5: {
      setState(220);
      loc_rotation();
      setState(221);
      loc_center();
      setState(222);
      loc_lengths();
      break;
    }

    case 6: {
      setState(224);
      loc_rotation();
      setState(225);
      loc_lengths();
      setState(226);
      loc_center();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Loc_centerContext ------------------------------------------------------------------

BehaviorParser::Loc_centerContext::Loc_centerContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Loc_centerContext::CENTER()
{
  return getToken(BehaviorParser::CENTER, 0);
}

tree::TerminalNode* BehaviorParser::Loc_centerContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

BehaviorParser::Value_coordContext* BehaviorParser::Loc_centerContext::value_coord()
{
  return getRuleContext<BehaviorParser::Value_coordContext>(0);
}

size_t BehaviorParser::Loc_centerContext::getRuleIndex() const { return BehaviorParser::RuleLoc_center; }

std::any BehaviorParser::Loc_centerContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLoc_center(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Loc_centerContext* BehaviorParser::loc_center()
{
  Loc_centerContext* _localctx = _tracker.createInstance<Loc_centerContext>(_ctx, getState());
  enterRule(_localctx, 12, BehaviorParser::RuleLoc_center);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(230);
    match(BehaviorParser::CENTER);
    setState(231);
    match(BehaviorParser::COLON);
    setState(232);
    value_coord();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Loc_lengthsContext ------------------------------------------------------------------

BehaviorParser::Loc_lengthsContext::Loc_lengthsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Loc_lengthsContext::LENGTHS()
{
  return getToken(BehaviorParser::LENGTHS, 0);
}

tree::TerminalNode* BehaviorParser::Loc_lengthsContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

BehaviorParser::Value_coordContext* BehaviorParser::Loc_lengthsContext::value_coord()
{
  return getRuleContext<BehaviorParser::Value_coordContext>(0);
}

size_t BehaviorParser::Loc_lengthsContext::getRuleIndex() const { return BehaviorParser::RuleLoc_lengths; }

std::any BehaviorParser::Loc_lengthsContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLoc_lengths(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Loc_lengthsContext* BehaviorParser::loc_lengths()
{
  Loc_lengthsContext* _localctx = _tracker.createInstance<Loc_lengthsContext>(_ctx, getState());
  enterRule(_localctx, 14, BehaviorParser::RuleLoc_lengths);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
    match(BehaviorParser::LENGTHS);
    setState(235);
    match(BehaviorParser::COLON);
    setState(236);
    value_coord();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Loc_rotationContext ------------------------------------------------------------------

BehaviorParser::Loc_rotationContext::Loc_rotationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Loc_rotationContext::ROTATION()
{
  return getToken(BehaviorParser::ROTATION, 0);
}

tree::TerminalNode* BehaviorParser::Loc_rotationContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::Value_numericContext* BehaviorParser::Loc_rotationContext::value_numeric()
{
  return getRuleContext<BehaviorParser::Value_numericContext>(0);
}

size_t BehaviorParser::Loc_rotationContext::getRuleIndex() const { return BehaviorParser::RuleLoc_rotation; }

std::any BehaviorParser::Loc_rotationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLoc_rotation(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Loc_rotationContext* BehaviorParser::loc_rotation()
{
  Loc_rotationContext* _localctx = _tracker.createInstance<Loc_rotationContext>(_ctx, getState());
  enterRule(_localctx, 16, BehaviorParser::RuleLoc_rotation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(238);
    match(BehaviorParser::ROTATION);
    setState(239);
    match(BehaviorParser::COLON);
    setState(240);
    value_numeric();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Value_numericContext ------------------------------------------------------------------

BehaviorParser::Value_numericContext::Value_numericContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Value_numberContext* BehaviorParser::Value_numericContext::value_number()
{
  return getRuleContext<BehaviorParser::Value_numberContext>(0);
}

BehaviorParser::Value_floatContext* BehaviorParser::Value_numericContext::value_float()
{
  return getRuleContext<BehaviorParser::Value_floatContext>(0);
}

BehaviorParser::Value_rangeContext* BehaviorParser::Value_numericContext::value_range()
{
  return getRuleContext<BehaviorParser::Value_rangeContext>(0);
}

BehaviorParser::Value_randomContext* BehaviorParser::Value_numericContext::value_random()
{
  return getRuleContext<BehaviorParser::Value_randomContext>(0);
}

size_t BehaviorParser::Value_numericContext::getRuleIndex() const
{
  return BehaviorParser::RuleValue_numeric;
}

std::any BehaviorParser::Value_numericContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitValue_numeric(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Value_numericContext* BehaviorParser::value_numeric()
{
  Value_numericContext* _localctx = _tracker.createInstance<Value_numericContext>(_ctx, getState());
  enterRule(_localctx, 18, BehaviorParser::RuleValue_numeric);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(246);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(242);
      value_number();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(243);
      value_float();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(244);
      value_range();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(245);
      value_random();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Value_numberContext ------------------------------------------------------------------

BehaviorParser::Value_numberContext::Value_numberContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Value_numberContext::NUMBER()
{
  return getToken(BehaviorParser::NUMBER, 0);
}

size_t BehaviorParser::Value_numberContext::getRuleIndex() const { return BehaviorParser::RuleValue_number; }

std::any BehaviorParser::Value_numberContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitValue_number(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Value_numberContext* BehaviorParser::value_number()
{
  Value_numberContext* _localctx = _tracker.createInstance<Value_numberContext>(_ctx, getState());
  enterRule(_localctx, 20, BehaviorParser::RuleValue_number);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(248);
    match(BehaviorParser::NUMBER);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Value_floatContext ------------------------------------------------------------------

BehaviorParser::Value_floatContext::Value_floatContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Value_floatContext::FLOAT() { return getToken(BehaviorParser::FLOAT, 0); }

size_t BehaviorParser::Value_floatContext::getRuleIndex() const { return BehaviorParser::RuleValue_float; }

std::any BehaviorParser::Value_floatContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitValue_float(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Value_floatContext* BehaviorParser::value_float()
{
  Value_floatContext* _localctx = _tracker.createInstance<Value_floatContext>(_ctx, getState());
  enterRule(_localctx, 22, BehaviorParser::RuleValue_float);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(250);
    match(BehaviorParser::FLOAT);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Value_rangeContext ------------------------------------------------------------------

BehaviorParser::Value_rangeContext::Value_rangeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Float_rangeContext* BehaviorParser::Value_rangeContext::float_range()
{
  return getRuleContext<BehaviorParser::Float_rangeContext>(0);
}

BehaviorParser::Number_rangeContext* BehaviorParser::Value_rangeContext::number_range()
{
  return getRuleContext<BehaviorParser::Number_rangeContext>(0);
}

size_t BehaviorParser::Value_rangeContext::getRuleIndex() const { return BehaviorParser::RuleValue_range; }

std::any BehaviorParser::Value_rangeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitValue_range(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Value_rangeContext* BehaviorParser::value_range()
{
  Value_rangeContext* _localctx = _tracker.createInstance<Value_rangeContext>(_ctx, getState());
  enterRule(_localctx, 24, BehaviorParser::RuleValue_range);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(254);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::FLOAT: {
        enterOuterAlt(_localctx, 1);
        setState(252);
        float_range();
        break;
      }

      case BehaviorParser::NUMBER: {
        enterOuterAlt(_localctx, 2);
        setState(253);
        number_range();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Value_randomContext ------------------------------------------------------------------

BehaviorParser::Value_randomContext::Value_randomContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Random_floatContext* BehaviorParser::Value_randomContext::random_float()
{
  return getRuleContext<BehaviorParser::Random_floatContext>(0);
}

BehaviorParser::Random_numberContext* BehaviorParser::Value_randomContext::random_number()
{
  return getRuleContext<BehaviorParser::Random_numberContext>(0);
}

size_t BehaviorParser::Value_randomContext::getRuleIndex() const { return BehaviorParser::RuleValue_random; }

std::any BehaviorParser::Value_randomContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitValue_random(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Value_randomContext* BehaviorParser::value_random()
{
  Value_randomContext* _localctx = _tracker.createInstance<Value_randomContext>(_ctx, getState());
  enterRule(_localctx, 26, BehaviorParser::RuleValue_random);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(258);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(256);
      random_float();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(257);
      random_number();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Random_numberContext ------------------------------------------------------------------

BehaviorParser::Random_numberContext::Random_numberContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Random_numberContext::RANDOM()
{
  return getToken(BehaviorParser::RANDOM, 0);
}

BehaviorParser::Number_rangeContext* BehaviorParser::Random_numberContext::number_range()
{
  return getRuleContext<BehaviorParser::Number_rangeContext>(0);
}

size_t BehaviorParser::Random_numberContext::getRuleIndex() const
{
  return BehaviorParser::RuleRandom_number;
}

std::any BehaviorParser::Random_numberContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitRandom_number(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Random_numberContext* BehaviorParser::random_number()
{
  Random_numberContext* _localctx = _tracker.createInstance<Random_numberContext>(_ctx, getState());
  enterRule(_localctx, 28, BehaviorParser::RuleRandom_number);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(260);
    match(BehaviorParser::RANDOM);
    setState(261);
    number_range();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Random_floatContext ------------------------------------------------------------------

BehaviorParser::Random_floatContext::Random_floatContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Random_floatContext::RANDOM()
{
  return getToken(BehaviorParser::RANDOM, 0);
}

BehaviorParser::Float_rangeContext* BehaviorParser::Random_floatContext::float_range()
{
  return getRuleContext<BehaviorParser::Float_rangeContext>(0);
}

size_t BehaviorParser::Random_floatContext::getRuleIndex() const { return BehaviorParser::RuleRandom_float; }

std::any BehaviorParser::Random_floatContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitRandom_float(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Random_floatContext* BehaviorParser::random_float()
{
  Random_floatContext* _localctx = _tracker.createInstance<Random_floatContext>(_ctx, getState());
  enterRule(_localctx, 30, BehaviorParser::RuleRandom_float);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(263);
    match(BehaviorParser::RANDOM);
    setState(264);
    float_range();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Float_rangeContext ------------------------------------------------------------------

BehaviorParser::Float_rangeContext::Float_rangeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

std::vector<tree::TerminalNode*> BehaviorParser::Float_rangeContext::FLOAT()
{
  return getTokens(BehaviorParser::FLOAT);
}

tree::TerminalNode* BehaviorParser::Float_rangeContext::FLOAT(size_t i)
{
  return getToken(BehaviorParser::FLOAT, i);
}

tree::TerminalNode* BehaviorParser::Float_rangeContext::TO() { return getToken(BehaviorParser::TO, 0); }

size_t BehaviorParser::Float_rangeContext::getRuleIndex() const { return BehaviorParser::RuleFloat_range; }

std::any BehaviorParser::Float_rangeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitFloat_range(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Float_rangeContext* BehaviorParser::float_range()
{
  Float_rangeContext* _localctx = _tracker.createInstance<Float_rangeContext>(_ctx, getState());
  enterRule(_localctx, 32, BehaviorParser::RuleFloat_range);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(266);
    match(BehaviorParser::FLOAT);
    setState(267);
    match(BehaviorParser::TO);
    setState(268);
    match(BehaviorParser::FLOAT);
   
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Number_rangeContext ------------------------------------------------------------------

BehaviorParser::Number_rangeContext::Number_rangeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

std::vector<tree::TerminalNode*> BehaviorParser::Number_rangeContext::NUMBER()
{
  return getTokens(BehaviorParser::NUMBER);
}

tree::TerminalNode* BehaviorParser::Number_rangeContext::NUMBER(size_t i)
{
  return getToken(BehaviorParser::NUMBER, i);
}

tree::TerminalNode* BehaviorParser::Number_rangeContext::TO() { return getToken(BehaviorParser::TO, 0); }

size_t BehaviorParser::Number_rangeContext::getRuleIndex() const { return BehaviorParser::RuleNumber_range; }

std::any BehaviorParser::Number_rangeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitNumber_range(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Number_rangeContext* BehaviorParser::number_range()
{
  Number_rangeContext* _localctx = _tracker.createInstance<Number_rangeContext>(_ctx, getState());
  enterRule(_localctx, 34, BehaviorParser::RuleNumber_range);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(270);
    match(BehaviorParser::NUMBER);
    setState(271);
    match(BehaviorParser::TO);
    setState(272);
    match(BehaviorParser::NUMBER);
   
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Value_coordContext ------------------------------------------------------------------

BehaviorParser::Value_coordContext::Value_coordContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

std::vector<BehaviorParser::Value_numericContext*> BehaviorParser::Value_coordContext::value_numeric()
{
  return getRuleContexts<BehaviorParser::Value_numericContext>();
}

BehaviorParser::Value_numericContext* BehaviorParser::Value_coordContext::value_numeric(size_t i)
{
  return getRuleContext<BehaviorParser::Value_numericContext>(i);
}

std::vector<tree::TerminalNode*> BehaviorParser::Value_coordContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::Value_coordContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::Value_coordContext::getRuleIndex() const { return BehaviorParser::RuleValue_coord; }

std::any BehaviorParser::Value_coordContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitValue_coord(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Value_coordContext* BehaviorParser::value_coord()
{
  Value_coordContext* _localctx = _tracker.createInstance<Value_coordContext>(_ctx, getState());
  enterRule(_localctx, 36, BehaviorParser::RuleValue_coord);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(274);
    match(BehaviorParser::T__1);
    setState(275);
    value_numeric();
    setState(282);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::COMMA) {
      setState(276);
      match(BehaviorParser::COMMA);
      setState(277);
      value_numeric();
      setState(280);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == BehaviorParser::COMMA) {
        setState(278);
        match(BehaviorParser::COMMA);
        setState(279);
        value_numeric();
      }
    }
    setState(284);
    match(BehaviorParser::T__2);
   
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EventContext ------------------------------------------------------------------

BehaviorParser::EventContext::EventContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::EventContext::EVENT() { return getToken(BehaviorParser::EVENT, 0); }

tree::TerminalNode* BehaviorParser::EventContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

std::vector<BehaviorParser::Event_attributeContext*> BehaviorParser::EventContext::event_attribute()
{
  return getRuleContexts<BehaviorParser::Event_attributeContext>();
}

BehaviorParser::Event_attributeContext* BehaviorParser::EventContext::event_attribute(size_t i)
{
  return getRuleContext<BehaviorParser::Event_attributeContext>(i);
}

size_t BehaviorParser::EventContext::getRuleIndex() const { return BehaviorParser::RuleEvent; }

std::any BehaviorParser::EventContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitEvent(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::EventContext* BehaviorParser::event()
{
  EventContext* _localctx = _tracker.createInstance<EventContext>(_ctx, getState());
  enterRule(_localctx, 38, BehaviorParser::RuleEvent);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(286);
    match(BehaviorParser::EVENT);
    setState(287);
    match(BehaviorParser::COLON);
    setState(291);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ( _la == BehaviorParser::END || _la == BehaviorParser::NAME

    || _la == BehaviorParser::START) {
      setState(288);
      event_attribute();
      setState(293);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_attributeContext ------------------------------------------------------------------

BehaviorParser::Event_attributeContext::Event_attributeContext(ParserRuleContext* parent,
                                                               size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Event_nameContext* BehaviorParser::Event_attributeContext::event_name()
{
  return getRuleContext<BehaviorParser::Event_nameContext>(0);
}

BehaviorParser::Event_startContext* BehaviorParser::Event_attributeContext::event_start()
{
  return getRuleContext<BehaviorParser::Event_startContext>(0);
}

BehaviorParser::Event_endContext* BehaviorParser::Event_attributeContext::event_end()
{
  return getRuleContext<BehaviorParser::Event_endContext>(0);
}

size_t BehaviorParser::Event_attributeContext::getRuleIndex() const
{
  return BehaviorParser::RuleEvent_attribute;
}

std::any BehaviorParser::Event_attributeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitEvent_attribute(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Event_attributeContext* BehaviorParser::event_attribute()
{
  Event_attributeContext* _localctx = _tracker.createInstance<Event_attributeContext>(_ctx, getState());
  enterRule(_localctx, 40, BehaviorParser::RuleEvent_attribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(297);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(294);
        event_name();
        break;
      }

      case BehaviorParser::START: {
        enterOuterAlt(_localctx, 2);
        setState(295);
        event_start();
        break;
      }

      case BehaviorParser::END: {
        enterOuterAlt(_localctx, 3);
        setState(296);
        event_end();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_nameContext ------------------------------------------------------------------

BehaviorParser::Event_nameContext::Event_nameContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Event_nameContext::NAME() { return getToken(BehaviorParser::NAME, 0); }

tree::TerminalNode* BehaviorParser::Event_nameContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

tree::TerminalNode* BehaviorParser::Event_nameContext::ID() { return getToken(BehaviorParser::ID, 0); }

size_t BehaviorParser::Event_nameContext::getRuleIndex() const { return BehaviorParser::RuleEvent_name; }

std::any BehaviorParser::Event_nameContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitEvent_name(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Event_nameContext* BehaviorParser::event_name()
{
  Event_nameContext* _localctx = _tracker.createInstance<Event_nameContext>(_ctx, getState());
  enterRule(_localctx, 42, BehaviorParser::RuleEvent_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(299);
    match(BehaviorParser::NAME);
    setState(300);
    match(BehaviorParser::COLON);
    setState(301);
    match(BehaviorParser::ID);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_startContext ------------------------------------------------------------------

BehaviorParser::Event_startContext::Event_startContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Event_startContext::START() { return getToken(BehaviorParser::START, 0); }

tree::TerminalNode* BehaviorParser::Event_startContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

BehaviorParser::ConditionContext* BehaviorParser::Event_startContext::condition()
{
  return getRuleContext<BehaviorParser::ConditionContext>(0);
}

size_t BehaviorParser::Event_startContext::getRuleIndex() const { return BehaviorParser::RuleEvent_start; }

std::any BehaviorParser::Event_startContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitEvent_start(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Event_startContext* BehaviorParser::event_start()
{
  Event_startContext* _localctx = _tracker.createInstance<Event_startContext>(_ctx, getState());
  enterRule(_localctx, 44, BehaviorParser::RuleEvent_start);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(303);
    match(BehaviorParser::START);
    setState(304);
    match(BehaviorParser::COLON);
    setState(305);
    condition();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_endContext ------------------------------------------------------------------

BehaviorParser::Event_endContext::Event_endContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Event_endContext::END() { return getToken(BehaviorParser::END, 0); }

tree::TerminalNode* BehaviorParser::Event_endContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

BehaviorParser::ConditionContext* BehaviorParser::Event_endContext::condition()
{
  return getRuleContext<BehaviorParser::ConditionContext>(0);
}

size_t BehaviorParser::Event_endContext::getRuleIndex() const { return BehaviorParser::RuleEvent_end; }

std::any BehaviorParser::Event_endContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitEvent_end(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Event_endContext* BehaviorParser::event_end()
{
  Event_endContext* _localctx = _tracker.createInstance<Event_endContext>(_ctx, getState());
  enterRule(_localctx, 46, BehaviorParser::RuleEvent_end);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(307);
    match(BehaviorParser::END);
    setState(308);
    match(BehaviorParser::COLON);
    setState(309);
    condition();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionContext ------------------------------------------------------------------

BehaviorParser::ConditionContext::ConditionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::UnaryContext* BehaviorParser::ConditionContext::unary()
{
  return getRuleContext<BehaviorParser::UnaryContext>(0);
}

tree::TerminalNode* BehaviorParser::ConditionContext::AND() { return getToken(BehaviorParser::AND, 0); }

BehaviorParser::ConditionContext* BehaviorParser::ConditionContext::condition()
{
  return getRuleContext<BehaviorParser::ConditionContext>(0);
}

tree::TerminalNode* BehaviorParser::ConditionContext::OR() { return getToken(BehaviorParser::OR, 0); }

size_t BehaviorParser::ConditionContext::getRuleIndex() const { return BehaviorParser::RuleCondition; }

std::any BehaviorParser::ConditionContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::ConditionContext* BehaviorParser::condition()
{
  ConditionContext* _localctx = _tracker.createInstance<ConditionContext>(_ctx, getState());
  enterRule(_localctx, 48, BehaviorParser::RuleCondition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(320);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(311);
      unary();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(312);
      unary();
      setState(313);
      match(BehaviorParser::AND);
      setState(314);
      condition();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(316);
      unary();
      setState(317);
      match(BehaviorParser::OR);
      setState(318);
      condition();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryContext ------------------------------------------------------------------

BehaviorParser::UnaryContext::UnaryContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::PrimaryContext* BehaviorParser::UnaryContext::primary()
{
  return getRuleContext<BehaviorParser::PrimaryContext>(0);
}

size_t BehaviorParser::UnaryContext::getRuleIndex() const { return BehaviorParser::RuleUnary; }

std::any BehaviorParser::UnaryContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitUnary(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::UnaryContext* BehaviorParser::unary()
{
  UnaryContext* _localctx = _tracker.createInstance<UnaryContext>(_ctx, getState());
  enterRule(_localctx, 50, BehaviorParser::RuleUnary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(322);
    primary();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryContext ------------------------------------------------------------------

BehaviorParser::PrimaryContext::PrimaryContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::ConditionContext* BehaviorParser::PrimaryContext::condition()
{
  return getRuleContext<BehaviorParser::ConditionContext>(0);
}

BehaviorParser::Sub_conditionContext* BehaviorParser::PrimaryContext::sub_condition()
{
  return getRuleContext<BehaviorParser::Sub_conditionContext>(0);
}

size_t BehaviorParser::PrimaryContext::getRuleIndex() const { return BehaviorParser::RulePrimary; }

std::any BehaviorParser::PrimaryContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitPrimary(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::PrimaryContext* BehaviorParser::primary()
{
  PrimaryContext* _localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
  enterRule(_localctx, 52, BehaviorParser::RulePrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(329);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case BehaviorParser::T__3: {
        enterOuterAlt(_localctx, 1);
        setState(324);
        match(BehaviorParser::T__3);
        setState(325);
        condition();
        setState(326);
        match(BehaviorParser::T__4);
        break;
      }

      case BehaviorParser::EVNT:
      case BehaviorParser::END:
      case BehaviorParser::ENTER:
      case BehaviorParser::EXISTS:
      case BehaviorParser::GOAL:
      case BehaviorParser::IN:
      case BehaviorParser::LEAVE:
      case BehaviorParser::LOCATION:
      case BehaviorParser::POSITION:
      case BehaviorParser::RANDOM:
      case BehaviorParser::STATUS:
      case BehaviorParser::STATE:
      case BehaviorParser::TARGET:
      case BehaviorParser::VELOCITY:
      case BehaviorParser::FLOAT:
      case BehaviorParser::NUMBER: {
        enterOuterAlt(_localctx, 2);
        setState(328);
        sub_condition();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Sub_conditionContext ------------------------------------------------------------------

BehaviorParser::Sub_conditionContext::Sub_conditionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Condition_Time_Elapsed_From_EventContext*
BehaviorParser::Sub_conditionContext::condition_Time_Elapsed_From_Event()
{
  return getRuleContext<BehaviorParser::Condition_Time_Elapsed_From_EventContext>(0);
}

BehaviorParser::Condition_Event_OccurredContext*
BehaviorParser::Sub_conditionContext::condition_Event_Occurred()
{
  return getRuleContext<BehaviorParser::Condition_Event_OccurredContext>(0);
}

BehaviorParser::Condition_Event_OccurringContext*
BehaviorParser::Sub_conditionContext::condition_Event_Occurring()
{
  return getRuleContext<BehaviorParser::Condition_Event_OccurringContext>(0);
}

BehaviorParser::Condition_Event_StartingContext*
BehaviorParser::Sub_conditionContext::condition_Event_Starting()
{
  return getRuleContext<BehaviorParser::Condition_Event_StartingContext>(0);
}

BehaviorParser::Condition_Event_EndingContext* BehaviorParser::Sub_conditionContext::condition_Event_Ending()
{
  return getRuleContext<BehaviorParser::Condition_Event_EndingContext>(0);
}

BehaviorParser::Condition_SpatialContext* BehaviorParser::Sub_conditionContext::condition_Spatial()
{
  return getRuleContext<BehaviorParser::Condition_SpatialContext>(0);
}

BehaviorParser::Condition_Enter_LocationContext*
BehaviorParser::Sub_conditionContext::condition_Enter_Location()
{
  return getRuleContext<BehaviorParser::Condition_Enter_LocationContext>(0);
}

BehaviorParser::Condition_Exit_LocationContext*
BehaviorParser::Sub_conditionContext::condition_Exit_Location()
{
  return getRuleContext<BehaviorParser::Condition_Exit_LocationContext>(0);
}

BehaviorParser::Condition_Inside_LocationContext*
BehaviorParser::Sub_conditionContext::condition_Inside_Location()
{
  return getRuleContext<BehaviorParser::Condition_Inside_LocationContext>(0);
}

BehaviorParser::Condition_AttributeContext* BehaviorParser::Sub_conditionContext::condition_Attribute()
{
  return getRuleContext<BehaviorParser::Condition_AttributeContext>(0);
}

BehaviorParser::Condition_ExistsContext* BehaviorParser::Sub_conditionContext::condition_Exists()
{
  return getRuleContext<BehaviorParser::Condition_ExistsContext>(0);
}

size_t BehaviorParser::Sub_conditionContext::getRuleIndex() const
{
  return BehaviorParser::RuleSub_condition;
}

std::any BehaviorParser::Sub_conditionContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSub_condition(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Sub_conditionContext* BehaviorParser::sub_condition()
{
  Sub_conditionContext* _localctx = _tracker.createInstance<Sub_conditionContext>(_ctx, getState());
  enterRule(_localctx, 54, BehaviorParser::RuleSub_condition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(342);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(331);
      condition_Time_Elapsed_From_Event();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(332);
      condition_Event_Occurred();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(333);
      condition_Event_Occurring();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(334);
      condition_Event_Starting();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(335);
      condition_Event_Ending();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(336);
      condition_Spatial();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(337);
      condition_Enter_Location();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(338);
      condition_Exit_Location();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(339);
      condition_Inside_Location();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(340);
      condition_Attribute();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(341);
      condition_Exists();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Time_Elapsed_From_EventContext ------------------------------------------------------------------

BehaviorParser::Condition_Time_Elapsed_From_EventContext::Condition_Time_Elapsed_From_EventContext(
    ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Value_numericContext*
BehaviorParser::Condition_Time_Elapsed_From_EventContext::value_numeric()
{
  return getRuleContext<BehaviorParser::Value_numericContext>(0);
}

tree::TerminalNode* BehaviorParser::Condition_Time_Elapsed_From_EventContext::SECONDS()
{
  return getToken(BehaviorParser::SECONDS, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Time_Elapsed_From_EventContext::AFTER()
{
  return getToken(BehaviorParser::AFTER, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Time_Elapsed_From_EventContext::EVNT()
{
  return getToken(BehaviorParser::EVNT, 0);
}

size_t BehaviorParser::Condition_Time_Elapsed_From_EventContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Time_Elapsed_From_Event;
}

std::any BehaviorParser::Condition_Time_Elapsed_From_EventContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Time_Elapsed_From_Event(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Time_Elapsed_From_EventContext* BehaviorParser::condition_Time_Elapsed_From_Event()
{
  Condition_Time_Elapsed_From_EventContext* _localctx =
      _tracker.createInstance<Condition_Time_Elapsed_From_EventContext>(_ctx, getState());
  enterRule(_localctx, 56, BehaviorParser::RuleCondition_Time_Elapsed_From_Event);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(344);
    value_numeric();
    setState(345);
    match(BehaviorParser::SECONDS);
    setState(346);
    match(BehaviorParser::AFTER);
    setState(347);
    match(BehaviorParser::EVNT);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Event_OccurredContext ------------------------------------------------------------------

BehaviorParser::Condition_Event_OccurredContext::Condition_Event_OccurredContext(ParserRuleContext* parent,
                                                                                 size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurredContext::EVNT()
{
  return getToken(BehaviorParser::EVNT, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurredContext::HAS()
{
  return getToken(BehaviorParser::HAS, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurredContext::OCCURRED()
{
  return getToken(BehaviorParser::OCCURRED, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurredContext::NOT() {
  return getToken(BehaviorParser::NOT, 0);
}


size_t BehaviorParser::Condition_Event_OccurredContext::getRuleIndex() const {
  return BehaviorParser::RuleCondition_Event_Occurred;
}

std::any BehaviorParser::Condition_Event_OccurredContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Event_Occurred(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Event_OccurredContext* BehaviorParser::condition_Event_Occurred()
{
  Condition_Event_OccurredContext* _localctx =
      _tracker.createInstance<Condition_Event_OccurredContext>(_ctx, getState());
  enterRule(_localctx, 58, BehaviorParser::RuleCondition_Event_Occurred);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(349);
    match(BehaviorParser::EVNT);
    setState(350);
    match(BehaviorParser::HAS);
    setState(352);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::NOT) {
      setState(351);
      match(BehaviorParser::NOT);
    }
    setState(354);
    match(BehaviorParser::OCCURRED);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Event_OccurringContext ------------------------------------------------------------------

BehaviorParser::Condition_Event_OccurringContext::Condition_Event_OccurringContext(ParserRuleContext* parent,
                                                                                   size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurringContext::EVNT()
{
  return getToken(BehaviorParser::EVNT, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurringContext::OCCURRING()
{
  return getToken(BehaviorParser::OCCURRING, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_OccurringContext::NOT() {
  return getToken(BehaviorParser::NOT, 0);
}


size_t BehaviorParser::Condition_Event_OccurringContext::getRuleIndex() const {
  return BehaviorParser::RuleCondition_Event_Occurring;
}

std::any BehaviorParser::Condition_Event_OccurringContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Event_Occurring(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Event_OccurringContext* BehaviorParser::condition_Event_Occurring()
{
  Condition_Event_OccurringContext* _localctx =
      _tracker.createInstance<Condition_Event_OccurringContext>(_ctx, getState());
  enterRule(_localctx, 60, BehaviorParser::RuleCondition_Event_Occurring);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(356);
    match(BehaviorParser::EVNT);
    setState(358);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::NOT) {
      setState(357);
      match(BehaviorParser::NOT);
    }
    setState(360);
    match(BehaviorParser::OCCURRING);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Event_StartingContext ------------------------------------------------------------------

BehaviorParser::Condition_Event_StartingContext::Condition_Event_StartingContext(ParserRuleContext* parent,
                                                                                 size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Event_StartingContext::EVNT()
{
  return getToken(BehaviorParser::EVNT, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_StartingContext::STARTING()
{
  return getToken(BehaviorParser::STARTING, 0);
}

size_t BehaviorParser::Condition_Event_StartingContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Event_Starting;
}

std::any BehaviorParser::Condition_Event_StartingContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Event_Starting(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Event_StartingContext* BehaviorParser::condition_Event_Starting()
{
  Condition_Event_StartingContext* _localctx =
      _tracker.createInstance<Condition_Event_StartingContext>(_ctx, getState());
  enterRule(_localctx, 62, BehaviorParser::RuleCondition_Event_Starting);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(362);
    match(BehaviorParser::EVNT);
    setState(363);
    match(BehaviorParser::STARTING);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Event_EndingContext ------------------------------------------------------------------

BehaviorParser::Condition_Event_EndingContext::Condition_Event_EndingContext(ParserRuleContext* parent,
                                                                             size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Event_EndingContext::EVNT()
{
  return getToken(BehaviorParser::EVNT, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Event_EndingContext::ENDING()
{
  return getToken(BehaviorParser::ENDING, 0);
}

size_t BehaviorParser::Condition_Event_EndingContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Event_Ending;
}

std::any BehaviorParser::Condition_Event_EndingContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Event_Ending(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Event_EndingContext* BehaviorParser::condition_Event_Ending()
{
  Condition_Event_EndingContext* _localctx =
      _tracker.createInstance<Condition_Event_EndingContext>(_ctx, getState());
  enterRule(_localctx, 64, BehaviorParser::RuleCondition_Event_Ending);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(365);
    match(BehaviorParser::EVNT);
    setState(366);
    match(BehaviorParser::ENDING);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_SpatialContext ------------------------------------------------------------------

BehaviorParser::Condition_SpatialContext::Condition_SpatialContext(ParserRuleContext* parent,
                                                                   size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_SpatialContext::TARGET()
{
  return getToken(BehaviorParser::TARGET, 0);
}

tree::TerminalNode* BehaviorParser::Condition_SpatialContext::WITHIN()
{
  return getToken(BehaviorParser::WITHIN, 0);
}

BehaviorParser::Value_numericContext* BehaviorParser::Condition_SpatialContext::value_numeric()
{
  return getRuleContext<BehaviorParser::Value_numericContext>(0);
}

size_t BehaviorParser::Condition_SpatialContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Spatial;
}

std::any BehaviorParser::Condition_SpatialContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Spatial(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_SpatialContext* BehaviorParser::condition_Spatial()
{
  Condition_SpatialContext* _localctx = _tracker.createInstance<Condition_SpatialContext>(_ctx, getState());
  enterRule(_localctx, 66, BehaviorParser::RuleCondition_Spatial);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(368);
    match(BehaviorParser::TARGET);
    setState(369);
    match(BehaviorParser::WITHIN);
    setState(370);
    value_numeric();
    setState(371);
    _la = _input->LA(1);
    if (!(_la == BehaviorParser::T__5

    || _la == BehaviorParser::T__6)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Enter_LocationContext ------------------------------------------------------------------

BehaviorParser::Condition_Enter_LocationContext::Condition_Enter_LocationContext(ParserRuleContext* parent,
                                                                                 size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Enter_LocationContext::ENTER()
{
  return getToken(BehaviorParser::ENTER, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Enter_LocationContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

size_t BehaviorParser::Condition_Enter_LocationContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Enter_Location;
}

std::any BehaviorParser::Condition_Enter_LocationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Enter_Location(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Enter_LocationContext* BehaviorParser::condition_Enter_Location()
{
  Condition_Enter_LocationContext* _localctx =
      _tracker.createInstance<Condition_Enter_LocationContext>(_ctx, getState());
  enterRule(_localctx, 68, BehaviorParser::RuleCondition_Enter_Location);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(373);
    match(BehaviorParser::ENTER);
    setState(374);
    match(BehaviorParser::LOC_NAME);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Exit_LocationContext ------------------------------------------------------------------

BehaviorParser::Condition_Exit_LocationContext::Condition_Exit_LocationContext(ParserRuleContext* parent,
                                                                               size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Exit_LocationContext::LEAVE()
{
  return getToken(BehaviorParser::LEAVE, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Exit_LocationContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

size_t BehaviorParser::Condition_Exit_LocationContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Exit_Location;
}

std::any BehaviorParser::Condition_Exit_LocationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Exit_Location(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Exit_LocationContext* BehaviorParser::condition_Exit_Location()
{
  Condition_Exit_LocationContext* _localctx =
      _tracker.createInstance<Condition_Exit_LocationContext>(_ctx, getState());
  enterRule(_localctx, 70, BehaviorParser::RuleCondition_Exit_Location);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(376);
    match(BehaviorParser::LEAVE);
    setState(377);
    match(BehaviorParser::LOC_NAME);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_Inside_LocationContext ------------------------------------------------------------------

BehaviorParser::Condition_Inside_LocationContext::Condition_Inside_LocationContext(ParserRuleContext* parent,
                                                                                   size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_Inside_LocationContext::IN()
{
  return getToken(BehaviorParser::IN, 0);
}

tree::TerminalNode* BehaviorParser::Condition_Inside_LocationContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

size_t BehaviorParser::Condition_Inside_LocationContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Inside_Location;
}

std::any BehaviorParser::Condition_Inside_LocationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Inside_Location(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_Inside_LocationContext* BehaviorParser::condition_Inside_Location()
{
  Condition_Inside_LocationContext* _localctx =
      _tracker.createInstance<Condition_Inside_LocationContext>(_ctx, getState());
  enterRule(_localctx, 72, BehaviorParser::RuleCondition_Inside_Location);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(379);
    match(BehaviorParser::IN);
    setState(380);
    match(BehaviorParser::LOC_NAME);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_AttributeContext ------------------------------------------------------------------

BehaviorParser::Condition_AttributeContext::Condition_AttributeContext(ParserRuleContext* parent,
                                                                       size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::AttributeContext* BehaviorParser::Condition_AttributeContext::attribute()
{
  return getRuleContext<BehaviorParser::AttributeContext>(0);
}

tree::TerminalNode* BehaviorParser::Condition_AttributeContext::IS()
{
  return getToken(BehaviorParser::IS, 0);
}

BehaviorParser::Attr_valueContext* BehaviorParser::Condition_AttributeContext::attr_value()
{
  return getRuleContext<BehaviorParser::Attr_valueContext>(0);
}

tree::TerminalNode* BehaviorParser::Condition_AttributeContext::TARGET()
{
  return getToken(BehaviorParser::TARGET, 0);
}

tree::TerminalNode* BehaviorParser::Condition_AttributeContext::NOT()
{
  return getToken(BehaviorParser::NOT, 0);
}

size_t BehaviorParser::Condition_AttributeContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Attribute;
}

std::any BehaviorParser::Condition_AttributeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Attribute(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_AttributeContext* BehaviorParser::condition_Attribute()
{
  Condition_AttributeContext* _localctx =
      _tracker.createInstance<Condition_AttributeContext>(_ctx, getState());
  enterRule(_localctx, 74, BehaviorParser::RuleCondition_Attribute);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(383);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::TARGET) {
      setState(382);
      match(BehaviorParser::TARGET);
    }
    setState(385);
    attribute();
    setState(386);
    match(BehaviorParser::IS);

    setState(388);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::NOT) {
      setState(387);
      match(BehaviorParser::NOT);
    }
    setState(390);
    attr_value();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_ExistsContext ------------------------------------------------------------------

BehaviorParser::Condition_ExistsContext::Condition_ExistsContext(ParserRuleContext* parent,
                                                                 size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::EXISTS()
{
  return getToken(BehaviorParser::EXISTS, 0);
}

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::ID() { return getToken(BehaviorParser::ID, 0); }

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::WHOSE()
{
  return getToken(BehaviorParser::WHOSE, 0);
}

BehaviorParser::AttributeContext* BehaviorParser::Condition_ExistsContext::attribute()
{
  return getRuleContext<BehaviorParser::AttributeContext>(0);
}

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::IS() { return getToken(BehaviorParser::IS, 0); }

BehaviorParser::Attr_valueContext* BehaviorParser::Condition_ExistsContext::attr_value()
{
  return getRuleContext<BehaviorParser::Attr_valueContext>(0);
}

std::vector<BehaviorParser::ModifierContext*> BehaviorParser::Condition_ExistsContext::modifier()
{
  return getRuleContexts<BehaviorParser::ModifierContext>();
}

BehaviorParser::ModifierContext* BehaviorParser::Condition_ExistsContext::modifier(size_t i)
{
  return getRuleContext<BehaviorParser::ModifierContext>(i);
}

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::NOT()
{
  return getToken(BehaviorParser::NOT, 0);
}

std::vector<tree::TerminalNode*> BehaviorParser::Condition_ExistsContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::Condition_ExistsContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::Condition_ExistsContext::getRuleIndex() const
{
  return BehaviorParser::RuleCondition_Exists;
}

std::any BehaviorParser::Condition_ExistsContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitCondition_Exists(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Condition_ExistsContext* BehaviorParser::condition_Exists()
{
  Condition_ExistsContext* _localctx = _tracker.createInstance<Condition_ExistsContext>(_ctx, getState());
  enterRule(_localctx, 76, BehaviorParser::RuleCondition_Exists);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(392);
    match(BehaviorParser::EXISTS);
    setState(393);
    match(BehaviorParser::COLON);
    setState(394);
    match(BehaviorParser::ID);
    setState(401);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 18014398576594944) != 0) || _la == BehaviorParser::WITHIN) {
      setState(396);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == BehaviorParser::COMMA) {
        setState(395);
        match(BehaviorParser::COMMA);
      }
      setState(398);
      modifier();
      setState(403);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(404);
    match(BehaviorParser::WHOSE);
    setState(405);
    attribute();
    setState(406);
    match(BehaviorParser::IS);

    setState(408);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::NOT) {
      setState(407);
      match(BehaviorParser::NOT);
    }
    setState(410);
    attr_value();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DurationContext ------------------------------------------------------------------

BehaviorParser::DurationContext::DurationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Value_numericContext* BehaviorParser::DurationContext::value_numeric()
{
  return getRuleContext<BehaviorParser::Value_numericContext>(0);
}

tree::TerminalNode* BehaviorParser::DurationContext::SECONDS()
{
  return getToken(BehaviorParser::SECONDS, 0);
}

tree::TerminalNode* BehaviorParser::DurationContext::SECOND() { return getToken(BehaviorParser::SECOND, 0); }

size_t BehaviorParser::DurationContext::getRuleIndex() const { return BehaviorParser::RuleDuration; }

std::any BehaviorParser::DurationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitDuration(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::DurationContext* BehaviorParser::duration()
{
  DurationContext* _localctx = _tracker.createInstance<DurationContext>(_ctx, getState());
  enterRule(_localctx, 78, BehaviorParser::RuleDuration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(412);
    value_numeric();
    setState(413);
    _la = _input->LA(1);
    if ( ! (_la == BehaviorParser::SECOND

            || _la == BehaviorParser::SECONDS) ) {
      _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AttributeContext ------------------------------------------------------------------

BehaviorParser::AttributeContext::AttributeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::AttributeContext::POSITION()
{
  return getToken(BehaviorParser::POSITION, 0);
}

tree::TerminalNode* BehaviorParser::AttributeContext::VELOCITY()
{
  return getToken(BehaviorParser::VELOCITY, 0);
}

tree::TerminalNode* BehaviorParser::AttributeContext::GOAL() { return getToken(BehaviorParser::GOAL, 0); }

BehaviorParser::End_goalContext* BehaviorParser::AttributeContext::end_goal()
{
  return getRuleContext<BehaviorParser::End_goalContext>(0);
}

tree::TerminalNode* BehaviorParser::AttributeContext::STATE() { return getToken(BehaviorParser::STATE, 0); }

tree::TerminalNode* BehaviorParser::AttributeContext::LOCATION()
{
  return getToken(BehaviorParser::LOCATION, 0);
}

tree::TerminalNode* BehaviorParser::AttributeContext::STATUS() { return getToken(BehaviorParser::STATUS, 0); }

size_t BehaviorParser::AttributeContext::getRuleIndex() const { return BehaviorParser::RuleAttribute; }

std::any BehaviorParser::AttributeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAttribute(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::AttributeContext* BehaviorParser::attribute()
{
  AttributeContext* _localctx = _tracker.createInstance<AttributeContext>(_ctx, getState());
  enterRule(_localctx, 80, BehaviorParser::RuleAttribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(422);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::POSITION: {
        enterOuterAlt(_localctx, 1);
        setState(415);
        match(BehaviorParser::POSITION);
        break;
      }

      case BehaviorParser::VELOCITY: {
        enterOuterAlt(_localctx, 2);
        setState(416);
        match(BehaviorParser::VELOCITY);
        break;
      }

      case BehaviorParser::GOAL: {
        enterOuterAlt(_localctx, 3);
        setState(417);
        match(BehaviorParser::GOAL);
        break;
      }

      case BehaviorParser::END: {
        enterOuterAlt(_localctx, 4);
        setState(418);
        end_goal();
        break;
      }

      case BehaviorParser::STATE: {
        enterOuterAlt(_localctx, 5);
        setState(419);
        match(BehaviorParser::STATE);
        break;
      }

      case BehaviorParser::LOCATION: {
        enterOuterAlt(_localctx, 6);
        setState(420);
        match(BehaviorParser::LOCATION);
        break;
      }

      case BehaviorParser::STATUS: {
        enterOuterAlt(_localctx, 7);
        setState(421);
        match(BehaviorParser::STATUS);
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Attr_valueContext ------------------------------------------------------------------

BehaviorParser::Attr_valueContext::Attr_valueContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Value_numericContext* BehaviorParser::Attr_valueContext::value_numeric()
{
  return getRuleContext<BehaviorParser::Value_numericContext>(0);
}

BehaviorParser::Value_coordContext* BehaviorParser::Attr_valueContext::value_coord()
{
  return getRuleContext<BehaviorParser::Value_coordContext>(0);
}

tree::TerminalNode* BehaviorParser::Attr_valueContext::STATE_VAL()
{
  return getToken(BehaviorParser::STATE_VAL, 0);
}

tree::TerminalNode* BehaviorParser::Attr_valueContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

BehaviorParser::TowardsContext* BehaviorParser::Attr_valueContext::towards()
{
  return getRuleContext<BehaviorParser::TowardsContext>(0);
}

BehaviorParser::AttributeContext* BehaviorParser::Attr_valueContext::attribute() {
  return getRuleContext<BehaviorParser::AttributeContext>(0);
}

size_t BehaviorParser::Attr_valueContext::getRuleIndex() const {
  return BehaviorParser::RuleAttr_value;
}

std::any BehaviorParser::Attr_valueContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAttr_value(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Attr_valueContext* BehaviorParser::attr_value()
{
  Attr_valueContext* _localctx = _tracker.createInstance<Attr_valueContext>(_ctx, getState());
  enterRule(_localctx, 82, BehaviorParser::RuleAttr_value);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(430);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::RANDOM:
      case BehaviorParser::FLOAT:
      case BehaviorParser::NUMBER: {
        enterOuterAlt(_localctx, 1);
        setState(424);
        value_numeric();
        break;
      }

      case BehaviorParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(425);
        value_coord();
        break;
      }

      case BehaviorParser::STATE_VAL: {
        enterOuterAlt(_localctx, 3);
        setState(426);
        match(BehaviorParser::STATE_VAL);
        break;
      }

      case BehaviorParser::LOC_NAME: {
        enterOuterAlt(_localctx, 4);
        setState(427);
        match(BehaviorParser::LOC_NAME);
        break;
      }

      case BehaviorParser::TOWARDS: {
        enterOuterAlt(_localctx, 5);
        setState(428);
        towards();
        break;
      }

      case BehaviorParser::END:
      case BehaviorParser::GOAL:
      case BehaviorParser::LOCATION:
      case BehaviorParser::POSITION:
      case BehaviorParser::STATUS:
      case BehaviorParser::STATE:
      case BehaviorParser::VELOCITY: {
        enterOuterAlt(_localctx, 6);
        setState(429);
        attribute();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- End_goalContext ------------------------------------------------------------------

BehaviorParser::End_goalContext::End_goalContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::End_goalContext::END() { return getToken(BehaviorParser::END, 0); }

tree::TerminalNode* BehaviorParser::End_goalContext::GOAL() { return getToken(BehaviorParser::GOAL, 0); }

size_t BehaviorParser::End_goalContext::getRuleIndex() const { return BehaviorParser::RuleEnd_goal; }

std::any BehaviorParser::End_goalContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitEnd_goal(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::End_goalContext* BehaviorParser::end_goal()
{
  End_goalContext* _localctx = _tracker.createInstance<End_goalContext>(_ctx, getState());
  enterRule(_localctx, 84, BehaviorParser::RuleEnd_goal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(432);
    match(BehaviorParser::END);
    setState(433);
    match(BehaviorParser::GOAL);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TowardsContext ------------------------------------------------------------------

BehaviorParser::TowardsContext::TowardsContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::TowardsContext::TOWARDS() { return getToken(BehaviorParser::TOWARDS, 0); }

tree::TerminalNode* BehaviorParser::TowardsContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

BehaviorParser::AttributeContext* BehaviorParser::TowardsContext::attribute()
{
  return getRuleContext<BehaviorParser::AttributeContext>(0);
}

size_t BehaviorParser::TowardsContext::getRuleIndex() const { return BehaviorParser::RuleTowards; }

std::any BehaviorParser::TowardsContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitTowards(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::TowardsContext* BehaviorParser::towards()
{
  TowardsContext* _localctx = _tracker.createInstance<TowardsContext>(_ctx, getState());
  enterRule(_localctx, 86, BehaviorParser::RuleTowards);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(435);
    match(BehaviorParser::TOWARDS);
    setState(438);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::LOC_NAME: {
        setState(436);
        match(BehaviorParser::LOC_NAME);
        break;
      }

      case BehaviorParser::END:
      case BehaviorParser::GOAL:
      case BehaviorParser::LOCATION:
      case BehaviorParser::POSITION:
      case BehaviorParser::STATUS:
      case BehaviorParser::STATE:
      case BehaviorParser::VELOCITY: {
        setState(437);
        attribute();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TargetContext ------------------------------------------------------------------

BehaviorParser::TargetContext::TargetContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::SelfContext* BehaviorParser::TargetContext::self()
{
  return getRuleContext<BehaviorParser::SelfContext>(0);
}

BehaviorParser::OtherContext* BehaviorParser::TargetContext::other()
{
  return getRuleContext<BehaviorParser::OtherContext>(0);
}

std::vector<BehaviorParser::ModifierContext*> BehaviorParser::TargetContext::modifier()
{
  return getRuleContexts<BehaviorParser::ModifierContext>();
}

BehaviorParser::ModifierContext* BehaviorParser::TargetContext::modifier(size_t i)
{
  return getRuleContext<BehaviorParser::ModifierContext>(i);
}

std::vector<tree::TerminalNode*> BehaviorParser::TargetContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::TargetContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::TargetContext::getRuleIndex() const { return BehaviorParser::RuleTarget; }

std::any BehaviorParser::TargetContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitTarget(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::TargetContext* BehaviorParser::target()
{
  TargetContext* _localctx = _tracker.createInstance<TargetContext>(_ctx, getState());
  enterRule(_localctx, 88, BehaviorParser::RuleTarget);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(451);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::SELF: {
        enterOuterAlt(_localctx, 1);
        setState(440);
        self();
        break;
      }

      case BehaviorParser::NEAREST: {
        enterOuterAlt(_localctx, 2);
        setState(441);
        other();
        setState(448);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 18014398576594944) != 0) || _la == BehaviorParser::WITHIN) {
          setState(443);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == BehaviorParser::COMMA) {
            setState(442);
            match(BehaviorParser::COMMA);
          }
          setState(445);
          modifier();
          setState(450);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelfContext ------------------------------------------------------------------

BehaviorParser::SelfContext::SelfContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::SelfContext::SELF() { return getToken(BehaviorParser::SELF, 0); }

size_t BehaviorParser::SelfContext::getRuleIndex() const { return BehaviorParser::RuleSelf; }

std::any BehaviorParser::SelfContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelf(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::SelfContext* BehaviorParser::self()
{
  SelfContext* _localctx = _tracker.createInstance<SelfContext>(_ctx, getState());
  enterRule(_localctx, 90, BehaviorParser::RuleSelf);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(453);
    match(BehaviorParser::SELF);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OtherContext ------------------------------------------------------------------

BehaviorParser::OtherContext::OtherContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Nearest_typeContext* BehaviorParser::OtherContext::nearest_type()
{
  return getRuleContext<BehaviorParser::Nearest_typeContext>(0);
}

size_t BehaviorParser::OtherContext::getRuleIndex() const { return BehaviorParser::RuleOther; }

std::any BehaviorParser::OtherContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitOther(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::OtherContext* BehaviorParser::other()
{
  OtherContext* _localctx = _tracker.createInstance<OtherContext>(_ctx, getState());
  enterRule(_localctx, 92, BehaviorParser::RuleOther);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(455);
    nearest_type();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Nearest_typeContext ------------------------------------------------------------------

BehaviorParser::Nearest_typeContext::Nearest_typeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Nearest_typeContext::NEAREST()
{
  return getToken(BehaviorParser::NEAREST, 0);
}

BehaviorParser::Id_listContext* BehaviorParser::Nearest_typeContext::id_list()
{
  return getRuleContext<BehaviorParser::Id_listContext>(0);
}

size_t BehaviorParser::Nearest_typeContext::getRuleIndex() const { return BehaviorParser::RuleNearest_type; }

std::any BehaviorParser::Nearest_typeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitNearest_type(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Nearest_typeContext* BehaviorParser::nearest_type()
{
  Nearest_typeContext* _localctx = _tracker.createInstance<Nearest_typeContext>(_ctx, getState());
  enterRule(_localctx, 94, BehaviorParser::RuleNearest_type);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(457);
    match(BehaviorParser::NEAREST);
    setState(458);
    id_list();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModifierContext ------------------------------------------------------------------

BehaviorParser::ModifierContext::ModifierContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::DirectionContext* BehaviorParser::ModifierContext::direction()
{
  return getRuleContext<BehaviorParser::DirectionContext>(0);
}

BehaviorParser::DistanceContext* BehaviorParser::ModifierContext::distance()
{
  return getRuleContext<BehaviorParser::DistanceContext>(0);
}

BehaviorParser::Location_modifierContext* BehaviorParser::ModifierContext::location_modifier()
{
  return getRuleContext<BehaviorParser::Location_modifierContext>(0);
}

size_t BehaviorParser::ModifierContext::getRuleIndex() const { return BehaviorParser::RuleModifier; }

std::any BehaviorParser::ModifierContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitModifier(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::ModifierContext* BehaviorParser::modifier()
{
  ModifierContext* _localctx = _tracker.createInstance<ModifierContext>(_ctx, getState());
  enterRule(_localctx, 96, BehaviorParser::RuleModifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(463);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(460);
      direction();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(461);
      distance();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(462);
      location_modifier();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Location_modifierContext ------------------------------------------------------------------

BehaviorParser::Location_modifierContext::Location_modifierContext(ParserRuleContext* parent,
                                                                   size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Location_modifierContext::IN() { return getToken(BehaviorParser::IN, 0); }

tree::TerminalNode* BehaviorParser::Location_modifierContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

size_t BehaviorParser::Location_modifierContext::getRuleIndex() const
{
  return BehaviorParser::RuleLocation_modifier;
}

std::any BehaviorParser::Location_modifierContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitLocation_modifier(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Location_modifierContext* BehaviorParser::location_modifier()
{
  Location_modifierContext* _localctx = _tracker.createInstance<Location_modifierContext>(_ctx, getState());
  enterRule(_localctx, 98, BehaviorParser::RuleLocation_modifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(465);
    match(BehaviorParser::IN);
    setState(466);
    match(BehaviorParser::LOC_NAME);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Id_listContext ------------------------------------------------------------------

BehaviorParser::Id_listContext::Id_listContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

std::vector<tree::TerminalNode*> BehaviorParser::Id_listContext::ID()
{
  return getTokens(BehaviorParser::ID);
}

tree::TerminalNode* BehaviorParser::Id_listContext::ID(size_t i) { return getToken(BehaviorParser::ID, i); }

std::vector<tree::TerminalNode*> BehaviorParser::Id_listContext::PEDESTRIAN()
{
  return getTokens(BehaviorParser::PEDESTRIAN);
}

tree::TerminalNode* BehaviorParser::Id_listContext::PEDESTRIAN(size_t i)
{
  return getToken(BehaviorParser::PEDESTRIAN, i);
}

std::vector<tree::TerminalNode*> BehaviorParser::Id_listContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::Id_listContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::Id_listContext::getRuleIndex() const { return BehaviorParser::RuleId_list; }

std::any BehaviorParser::Id_listContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitId_list(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Id_listContext* BehaviorParser::id_list()
{
  Id_listContext* _localctx = _tracker.createInstance<Id_listContext>(_ctx, getState());
  enterRule(_localctx, 100, BehaviorParser::RuleId_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(468);
    _la = _input->LA(1);
    if ( ! (_la == BehaviorParser::PEDESTRIAN

            || _la == BehaviorParser::ID) ) {
      _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(475);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(470);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == BehaviorParser::COMMA) {
          setState(469);
          match(BehaviorParser::COMMA);
        }
        setState(472);
        _la = _input->LA(1);
        if ( ! (_la == BehaviorParser::PEDESTRIAN

                || _la == BehaviorParser::ID) ) {
          _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      setState(477);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DistanceContext ------------------------------------------------------------------

BehaviorParser::DistanceContext::DistanceContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::DistanceContext::WITHIN() { return getToken(BehaviorParser::WITHIN, 0); }

BehaviorParser::Value_numericContext* BehaviorParser::DistanceContext::value_numeric()
{
  return getRuleContext<BehaviorParser::Value_numericContext>(0);
}

size_t BehaviorParser::DistanceContext::getRuleIndex() const { return BehaviorParser::RuleDistance; }

std::any BehaviorParser::DistanceContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitDistance(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::DistanceContext* BehaviorParser::distance()
{
  DistanceContext* _localctx = _tracker.createInstance<DistanceContext>(_ctx, getState());
  enterRule(_localctx, 102, BehaviorParser::RuleDistance);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(478);
    match(BehaviorParser::WITHIN);
    setState(479);
    value_numeric();
    setState(480);
    _la = _input->LA(1);
    if (!(_la == BehaviorParser::T__5

    || _la == BehaviorParser::T__6)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DirectionContext ------------------------------------------------------------------

BehaviorParser::DirectionContext::DirectionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::FrontContext* BehaviorParser::DirectionContext::front()
{
  return getRuleContext<BehaviorParser::FrontContext>(0);
}

BehaviorParser::BehindContext* BehaviorParser::DirectionContext::behind()
{
  return getRuleContext<BehaviorParser::BehindContext>(0);
}

size_t BehaviorParser::DirectionContext::getRuleIndex() const { return BehaviorParser::RuleDirection; }

std::any BehaviorParser::DirectionContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitDirection(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::DirectionContext* BehaviorParser::direction()
{
  DirectionContext* _localctx = _tracker.createInstance<DirectionContext>(_ctx, getState());
  enterRule(_localctx, 104, BehaviorParser::RuleDirection);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(484);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::IN: {
        enterOuterAlt(_localctx, 1);
        setState(482);
        front();
        break;
      }

      case BehaviorParser::BEHIND: {
        enterOuterAlt(_localctx, 2);
        setState(483);
        behind();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FrontContext ------------------------------------------------------------------

BehaviorParser::FrontContext::FrontContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::FrontContext::IN() { return getToken(BehaviorParser::IN, 0); }

tree::TerminalNode* BehaviorParser::FrontContext::FRONT() { return getToken(BehaviorParser::FRONT, 0); }

size_t BehaviorParser::FrontContext::getRuleIndex() const { return BehaviorParser::RuleFront; }

std::any BehaviorParser::FrontContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitFront(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::FrontContext* BehaviorParser::front()
{
  FrontContext* _localctx = _tracker.createInstance<FrontContext>(_ctx, getState());
  enterRule(_localctx, 106, BehaviorParser::RuleFront);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(486);
    match(BehaviorParser::IN);
    setState(487);
    match(BehaviorParser::FRONT);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BehindContext ------------------------------------------------------------------

BehaviorParser::BehindContext::BehindContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::BehindContext::BEHIND() { return getToken(BehaviorParser::BEHIND, 0); }

size_t BehaviorParser::BehindContext::getRuleIndex() const { return BehaviorParser::RuleBehind; }

std::any BehaviorParser::BehindContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitBehind(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::BehindContext* BehaviorParser::behind()
{
  BehindContext* _localctx = _tracker.createInstance<BehindContext>(_ctx, getState());
  enterRule(_localctx, 108, BehaviorParser::RuleBehind);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(489);
    match(BehaviorParser::BEHIND);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Ped_SelectorContext ------------------------------------------------------------------

BehaviorParser::Ped_SelectorContext::Ped_SelectorContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Ped_SelectorContext::SELECTOR()
{
  return getToken(BehaviorParser::SELECTOR, 0);
}

tree::TerminalNode* BehaviorParser::Ped_SelectorContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

std::vector<BehaviorParser::Selector_attributeContext*>
BehaviorParser::Ped_SelectorContext::selector_attribute()
{
  return getRuleContexts<BehaviorParser::Selector_attributeContext>();
}

BehaviorParser::Selector_attributeContext* BehaviorParser::Ped_SelectorContext::selector_attribute(size_t i)
{
  return getRuleContext<BehaviorParser::Selector_attributeContext>(i);
}

size_t BehaviorParser::Ped_SelectorContext::getRuleIndex() const { return BehaviorParser::RulePed_Selector; }

std::any BehaviorParser::Ped_SelectorContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitPed_Selector(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Ped_SelectorContext* BehaviorParser::ped_Selector()
{
  Ped_SelectorContext* _localctx = _tracker.createInstance<Ped_SelectorContext>(_ctx, getState());
  enterRule(_localctx, 110, BehaviorParser::RulePed_Selector);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(491);
    match(BehaviorParser::SELECTOR);
    setState(492);
    match(BehaviorParser::COLON);
    setState(496);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == BehaviorParser::FROM || ((((_la - 86) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 86)) & 8589934721) != 0)) {
      setState(493);
      selector_attribute();
      setState(498);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_attributeContext ------------------------------------------------------------------

BehaviorParser::Selector_attributeContext::Selector_attributeContext(ParserRuleContext* parent,
                                                                     size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Selector_typeContext* BehaviorParser::Selector_attributeContext::selector_type()
{
  return getRuleContext<BehaviorParser::Selector_typeContext>(0);
}

BehaviorParser::Selector_selectorContext* BehaviorParser::Selector_attributeContext::selector_selector()
{
  return getRuleContext<BehaviorParser::Selector_selectorContext>(0);
}

BehaviorParser::Selector_fromContext* BehaviorParser::Selector_attributeContext::selector_from()
{
  return getRuleContext<BehaviorParser::Selector_fromContext>(0);
}

BehaviorParser::Selector_requiredContext* BehaviorParser::Selector_attributeContext::selector_required()
{
  return getRuleContext<BehaviorParser::Selector_requiredContext>(0);
}

size_t BehaviorParser::Selector_attributeContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_attribute;
}

std::any BehaviorParser::Selector_attributeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_attribute(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_attributeContext* BehaviorParser::selector_attribute()
{
  Selector_attributeContext* _localctx = _tracker.createInstance<Selector_attributeContext>(_ctx, getState());
  enterRule(_localctx, 112, BehaviorParser::RuleSelector_attribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(503);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::TYPE: {
        enterOuterAlt(_localctx, 1);
        setState(499);
        selector_type();
        break;
      }

      case BehaviorParser::SELECT: {
        enterOuterAlt(_localctx, 2);
        setState(500);
        selector_selector();
        break;
      }

      case BehaviorParser::FROM: {
        enterOuterAlt(_localctx, 3);
        setState(501);
        selector_from();
        break;
      }

      case BehaviorParser::REQUIRED: {
        enterOuterAlt(_localctx, 4);
        setState(502);
        selector_required();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_typeContext ------------------------------------------------------------------

BehaviorParser::Selector_typeContext::Selector_typeContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Selector_typeContext::TYPE() { return getToken(BehaviorParser::TYPE, 0); }

tree::TerminalNode* BehaviorParser::Selector_typeContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::Id_listContext* BehaviorParser::Selector_typeContext::id_list()
{
  return getRuleContext<BehaviorParser::Id_listContext>(0);
}

size_t BehaviorParser::Selector_typeContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_type;
}

std::any BehaviorParser::Selector_typeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_type(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_typeContext* BehaviorParser::selector_type()
{
  Selector_typeContext* _localctx = _tracker.createInstance<Selector_typeContext>(_ctx, getState());
  enterRule(_localctx, 114, BehaviorParser::RuleSelector_type);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(505);
    match(BehaviorParser::TYPE);
    setState(506);
    match(BehaviorParser::COLON);
    setState(507);
    id_list();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_selectorContext ------------------------------------------------------------------

BehaviorParser::Selector_selectorContext::Selector_selectorContext(ParserRuleContext* parent,
                                                                   size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Selector_selectorContext::SELECT()
{
  return getToken(BehaviorParser::SELECT, 0);
}

tree::TerminalNode* BehaviorParser::Selector_selectorContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::SelectorContext* BehaviorParser::Selector_selectorContext::selector()
{
  return getRuleContext<BehaviorParser::SelectorContext>(0);
}

size_t BehaviorParser::Selector_selectorContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_selector;
}

std::any BehaviorParser::Selector_selectorContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_selector(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_selectorContext* BehaviorParser::selector_selector()
{
  Selector_selectorContext* _localctx = _tracker.createInstance<Selector_selectorContext>(_ctx, getState());
  enterRule(_localctx, 116, BehaviorParser::RuleSelector_selector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(509);
    match(BehaviorParser::SELECT);
    setState(510);
    match(BehaviorParser::COLON);
    setState(511);
    selector();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_fromContext ------------------------------------------------------------------

BehaviorParser::Selector_fromContext::Selector_fromContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Selector_fromContext::FROM() { return getToken(BehaviorParser::FROM, 0); }

tree::TerminalNode* BehaviorParser::Selector_fromContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::GroupContext* BehaviorParser::Selector_fromContext::group()
{
  return getRuleContext<BehaviorParser::GroupContext>(0);
}

size_t BehaviorParser::Selector_fromContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_from;
}

std::any BehaviorParser::Selector_fromContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_from(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_fromContext* BehaviorParser::selector_from()
{
  Selector_fromContext* _localctx = _tracker.createInstance<Selector_fromContext>(_ctx, getState());
  enterRule(_localctx, 118, BehaviorParser::RuleSelector_from);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(513);
    match(BehaviorParser::FROM);
    setState(514);
    match(BehaviorParser::COLON);
    setState(515);
    group();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_requiredContext ------------------------------------------------------------------

BehaviorParser::Selector_requiredContext::Selector_requiredContext(ParserRuleContext* parent,
                                                                   size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Selector_requiredContext::REQUIRED()
{
  return getToken(BehaviorParser::REQUIRED, 0);
}

size_t BehaviorParser::Selector_requiredContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_required;
}

std::any BehaviorParser::Selector_requiredContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_required(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_requiredContext* BehaviorParser::selector_required()
{
  Selector_requiredContext* _localctx = _tracker.createInstance<Selector_requiredContext>(_ctx, getState());
  enterRule(_localctx, 120, BehaviorParser::RuleSelector_required);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(517);
    match(BehaviorParser::REQUIRED);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectorContext ------------------------------------------------------------------

BehaviorParser::SelectorContext::SelectorContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Selector_PercentContext* BehaviorParser::SelectorContext::selector_Percent()
{
  return getRuleContext<BehaviorParser::Selector_PercentContext>(0);
}

BehaviorParser::Selector_Exactly_N_RandomContext* BehaviorParser::SelectorContext::selector_Exactly_N_Random()
{
  return getRuleContext<BehaviorParser::Selector_Exactly_N_RandomContext>(0);
}

BehaviorParser::Selector_EveryoneContext* BehaviorParser::SelectorContext::selector_Everyone()
{
  return getRuleContext<BehaviorParser::Selector_EveryoneContext>(0);
}

BehaviorParser::Selector_LocationContext* BehaviorParser::SelectorContext::selector_Location()
{
  return getRuleContext<BehaviorParser::Selector_LocationContext>(0);
}

size_t BehaviorParser::SelectorContext::getRuleIndex() const { return BehaviorParser::RuleSelector; }

std::any BehaviorParser::SelectorContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::SelectorContext* BehaviorParser::selector()
{
  SelectorContext* _localctx = _tracker.createInstance<SelectorContext>(_ctx, getState());
  enterRule(_localctx, 122, BehaviorParser::RuleSelector);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(523);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(519);
      selector_Percent();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(520);
      selector_Exactly_N_Random();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(521);
      selector_Everyone();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(522);
      selector_Location();
      break;
    }

      default:
        break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_PercentContext ------------------------------------------------------------------

BehaviorParser::Selector_PercentContext::Selector_PercentContext(ParserRuleContext* parent,
                                                                 size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Value_numberContext* BehaviorParser::Selector_PercentContext::value_number()
{
  return getRuleContext<BehaviorParser::Value_numberContext>(0);
}

size_t BehaviorParser::Selector_PercentContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_Percent;
}

std::any BehaviorParser::Selector_PercentContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_Percent(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_PercentContext* BehaviorParser::selector_Percent()
{
  Selector_PercentContext* _localctx = _tracker.createInstance<Selector_PercentContext>(_ctx, getState());
  enterRule(_localctx, 124, BehaviorParser::RuleSelector_Percent);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(525);
    value_number();
    setState(526);
    match(BehaviorParser::T__7);
   
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_Exactly_N_RandomContext ------------------------------------------------------------------

BehaviorParser::Selector_Exactly_N_RandomContext::Selector_Exactly_N_RandomContext(ParserRuleContext* parent,
                                                                                   size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Value_numberContext* BehaviorParser::Selector_Exactly_N_RandomContext::value_number()
{
  return getRuleContext<BehaviorParser::Value_numberContext>(0);
}

size_t BehaviorParser::Selector_Exactly_N_RandomContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_Exactly_N_Random;
}

std::any BehaviorParser::Selector_Exactly_N_RandomContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_Exactly_N_Random(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_Exactly_N_RandomContext* BehaviorParser::selector_Exactly_N_Random()
{
  Selector_Exactly_N_RandomContext* _localctx =
      _tracker.createInstance<Selector_Exactly_N_RandomContext>(_ctx, getState());
  enterRule(_localctx, 126, BehaviorParser::RuleSelector_Exactly_N_Random);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(528);
    value_number();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_EveryoneContext ------------------------------------------------------------------

BehaviorParser::Selector_EveryoneContext::Selector_EveryoneContext(ParserRuleContext* parent,
                                                                   size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Selector_EveryoneContext::EVERYONE()
{
  return getToken(BehaviorParser::EVERYONE, 0);
}

size_t BehaviorParser::Selector_EveryoneContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_Everyone;
}

std::any BehaviorParser::Selector_EveryoneContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_Everyone(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_EveryoneContext* BehaviorParser::selector_Everyone()
{
  Selector_EveryoneContext* _localctx = _tracker.createInstance<Selector_EveryoneContext>(_ctx, getState());
  enterRule(_localctx, 128, BehaviorParser::RuleSelector_Everyone);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(530);
    match(BehaviorParser::EVERYONE);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selector_LocationContext ------------------------------------------------------------------

BehaviorParser::Selector_LocationContext::Selector_LocationContext(ParserRuleContext* parent,
                                                                   size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Selector_LocationContext::IN() { return getToken(BehaviorParser::IN, 0); }

tree::TerminalNode* BehaviorParser::Selector_LocationContext::LOC_NAME()
{
  return getToken(BehaviorParser::LOC_NAME, 0);
}

size_t BehaviorParser::Selector_LocationContext::getRuleIndex() const
{
  return BehaviorParser::RuleSelector_Location;
}

std::any BehaviorParser::Selector_LocationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSelector_Location(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Selector_LocationContext* BehaviorParser::selector_Location()
{
  Selector_LocationContext* _localctx = _tracker.createInstance<Selector_LocationContext>(_ctx, getState());
  enterRule(_localctx, 130, BehaviorParser::RuleSelector_Location);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(532);
    match(BehaviorParser::IN);
    setState(533);
    match(BehaviorParser::LOC_NAME);
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupContext ------------------------------------------------------------------

BehaviorParser::GroupContext::GroupContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::GroupContext::ID() { return getToken(BehaviorParser::ID, 0); }

tree::TerminalNode* BehaviorParser::GroupContext::PEDESTRIAN()
{
  return getToken(BehaviorParser::PEDESTRIAN, 0);
}

tree::TerminalNode* BehaviorParser::GroupContext::PEDESTRIANS()
{
  return getToken(BehaviorParser::PEDESTRIANS, 0);
}

size_t BehaviorParser::GroupContext::getRuleIndex() const { return BehaviorParser::RuleGroup; }

std::any BehaviorParser::GroupContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitGroup(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::GroupContext* BehaviorParser::group()
{
  GroupContext* _localctx = _tracker.createInstance<GroupContext>(_ctx, getState());
  enterRule(_localctx, 132, BehaviorParser::RuleGroup);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(535);
    _la = _input->LA(1);
    if (!(((((_la - 79) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 79)) & 2251799813685251) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ActionContext ------------------------------------------------------------------

BehaviorParser::ActionContext::ActionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::ActionContext::ACTION() { return getToken(BehaviorParser::ACTION, 0); }

tree::TerminalNode* BehaviorParser::ActionContext::ID() { return getToken(BehaviorParser::ID, 0); }

tree::TerminalNode* BehaviorParser::ActionContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

std::vector<BehaviorParser::Action_attributeContext*> BehaviorParser::ActionContext::action_attribute()
{
  return getRuleContexts<BehaviorParser::Action_attributeContext>();
}

BehaviorParser::Action_attributeContext* BehaviorParser::ActionContext::action_attribute(size_t i)
{
  return getRuleContext<BehaviorParser::Action_attributeContext>(i);
}

size_t BehaviorParser::ActionContext::getRuleIndex() const { return BehaviorParser::RuleAction; }

std::any BehaviorParser::ActionContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::ActionContext* BehaviorParser::action()
{
  ActionContext* _localctx = _tracker.createInstance<ActionContext>(_ctx, getState());
  enterRule(_localctx, 134, BehaviorParser::RuleAction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(537);
    match(BehaviorParser::ACTION);
    setState(538);
    match(BehaviorParser::T__3);
    setState(539);
    match(BehaviorParser::ID);
    setState(540);
    match(BehaviorParser::T__4);
    setState(541);
    match(BehaviorParser::COLON);
    setState(545);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ( _la == BehaviorParser::CONDITION

            || _la == BehaviorParser::DURATION || _la == BehaviorParser::RESPONSE

    || _la == BehaviorParser::TARGET) {
      setState(542);
      action_attribute();
      setState(547);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Action_attributeContext ------------------------------------------------------------------

BehaviorParser::Action_attributeContext::Action_attributeContext(ParserRuleContext* parent,
                                                                 size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Action_responseContext* BehaviorParser::Action_attributeContext::action_response()
{
  return getRuleContext<BehaviorParser::Action_responseContext>(0);
}

BehaviorParser::Action_stimulusContext* BehaviorParser::Action_attributeContext::action_stimulus()
{
  return getRuleContext<BehaviorParser::Action_stimulusContext>(0);
}

BehaviorParser::Action_durationContext* BehaviorParser::Action_attributeContext::action_duration()
{
  return getRuleContext<BehaviorParser::Action_durationContext>(0);
}

BehaviorParser::Action_targetContext* BehaviorParser::Action_attributeContext::action_target()
{
  return getRuleContext<BehaviorParser::Action_targetContext>(0);
}

size_t BehaviorParser::Action_attributeContext::getRuleIndex() const
{
  return BehaviorParser::RuleAction_attribute;
}

std::any BehaviorParser::Action_attributeContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction_attribute(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Action_attributeContext* BehaviorParser::action_attribute()
{
  Action_attributeContext* _localctx = _tracker.createInstance<Action_attributeContext>(_ctx, getState());
  enterRule(_localctx, 136, BehaviorParser::RuleAction_attribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(552);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::RESPONSE: {
        enterOuterAlt(_localctx, 1);
        setState(548);
        action_response();
        break;
      }

      case BehaviorParser::CONDITION: {
        enterOuterAlt(_localctx, 2);
        setState(549);
        action_stimulus();
        break;
      }

      case BehaviorParser::DURATION: {
        enterOuterAlt(_localctx, 3);
        setState(550);
        action_duration();
        break;
      }

      case BehaviorParser::TARGET: {
        enterOuterAlt(_localctx, 4);
        setState(551);
        action_target();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Action_stimulusContext ------------------------------------------------------------------

BehaviorParser::Action_stimulusContext::Action_stimulusContext(ParserRuleContext* parent,
                                                               size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Action_stimulusContext::CONDITION()
{
  return getToken(BehaviorParser::CONDITION, 0);
}

tree::TerminalNode* BehaviorParser::Action_stimulusContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::ConditionContext* BehaviorParser::Action_stimulusContext::condition()
{
  return getRuleContext<BehaviorParser::ConditionContext>(0);
}

size_t BehaviorParser::Action_stimulusContext::getRuleIndex() const
{
  return BehaviorParser::RuleAction_stimulus;
}

std::any BehaviorParser::Action_stimulusContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction_stimulus(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Action_stimulusContext* BehaviorParser::action_stimulus()
{
  Action_stimulusContext* _localctx = _tracker.createInstance<Action_stimulusContext>(_ctx, getState());
  enterRule(_localctx, 138, BehaviorParser::RuleAction_stimulus);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(554);
    match(BehaviorParser::CONDITION);
    setState(555);
    match(BehaviorParser::COLON);
    setState(556);
    condition();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Action_responseContext ------------------------------------------------------------------

BehaviorParser::Action_responseContext::Action_responseContext(ParserRuleContext* parent,
                                                               size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Action_responseContext::RESPONSE()
{
  return getToken(BehaviorParser::RESPONSE, 0);
}

tree::TerminalNode* BehaviorParser::Action_responseContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::Sub_actionContext* BehaviorParser::Action_responseContext::sub_action()
{
  return getRuleContext<BehaviorParser::Sub_actionContext>(0);
}

size_t BehaviorParser::Action_responseContext::getRuleIndex() const
{
  return BehaviorParser::RuleAction_response;
}

std::any BehaviorParser::Action_responseContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction_response(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Action_responseContext* BehaviorParser::action_response()
{
  Action_responseContext* _localctx = _tracker.createInstance<Action_responseContext>(_ctx, getState());
  enterRule(_localctx, 140, BehaviorParser::RuleAction_response);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(558);
    match(BehaviorParser::RESPONSE);
    setState(559);
    match(BehaviorParser::COLON);
    setState(560);
    sub_action();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Action_durationContext ------------------------------------------------------------------

BehaviorParser::Action_durationContext::Action_durationContext(ParserRuleContext* parent,
                                                               size_t             invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Action_durationContext::DURATION()
{
  return getToken(BehaviorParser::DURATION, 0);
}

tree::TerminalNode* BehaviorParser::Action_durationContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::DurationContext* BehaviorParser::Action_durationContext::duration()
{
  return getRuleContext<BehaviorParser::DurationContext>(0);
}

size_t BehaviorParser::Action_durationContext::getRuleIndex() const
{
  return BehaviorParser::RuleAction_duration;
}

std::any BehaviorParser::Action_durationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction_duration(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Action_durationContext* BehaviorParser::action_duration()
{
  Action_durationContext* _localctx = _tracker.createInstance<Action_durationContext>(_ctx, getState());
  enterRule(_localctx, 142, BehaviorParser::RuleAction_duration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(562);
    match(BehaviorParser::DURATION);
    setState(563);
    match(BehaviorParser::COLON);
    setState(564);
    duration();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Action_targetContext ------------------------------------------------------------------

BehaviorParser::Action_targetContext::Action_targetContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Action_targetContext::TARGET()
{
  return getToken(BehaviorParser::TARGET, 0);
}

tree::TerminalNode* BehaviorParser::Action_targetContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

BehaviorParser::TargetContext* BehaviorParser::Action_targetContext::target()
{
  return getRuleContext<BehaviorParser::TargetContext>(0);
}

size_t BehaviorParser::Action_targetContext::getRuleIndex() const
{
  return BehaviorParser::RuleAction_target;
}

std::any BehaviorParser::Action_targetContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction_target(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Action_targetContext* BehaviorParser::action_target()
{
  Action_targetContext* _localctx = _tracker.createInstance<Action_targetContext>(_ctx, getState());
  enterRule(_localctx, 144, BehaviorParser::RuleAction_target);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(566);
    match(BehaviorParser::TARGET);
    setState(567);
    match(BehaviorParser::COLON);
    setState(568);
    target();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Sub_actionContext ------------------------------------------------------------------

BehaviorParser::Sub_actionContext::Sub_actionContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

std::vector<BehaviorParser::Action_atomContext*> BehaviorParser::Sub_actionContext::action_atom()
{
  return getRuleContexts<BehaviorParser::Action_atomContext>();
}

BehaviorParser::Action_atomContext* BehaviorParser::Sub_actionContext::action_atom(size_t i)
{
  return getRuleContext<BehaviorParser::Action_atomContext>(i);
}

std::vector<tree::TerminalNode*> BehaviorParser::Sub_actionContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::Sub_actionContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::Sub_actionContext::getRuleIndex() const { return BehaviorParser::RuleSub_action; }

std::any BehaviorParser::Sub_actionContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSub_action(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Sub_actionContext* BehaviorParser::sub_action()
{
  Sub_actionContext* _localctx = _tracker.createInstance<Sub_actionContext>(_ctx, getState());
  enterRule(_localctx, 146, BehaviorParser::RuleSub_action);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(570);
    action_atom();
    setState(575);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == BehaviorParser::COMMA) {
      setState(571);
      match(BehaviorParser::COMMA);
      setState(572);
      action_atom();
      setState(577);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Action_atomContext ------------------------------------------------------------------

BehaviorParser::Action_atomContext::Action_atomContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Set_atomContext* BehaviorParser::Action_atomContext::set_atom()
{
  return getRuleContext<BehaviorParser::Set_atomContext>(0);
}

BehaviorParser::Set_objective_atomContext* BehaviorParser::Action_atomContext::set_objective_atom() {
  return getRuleContext<BehaviorParser::Set_objective_atomContext>(0);
}

BehaviorParser::Scale_atomContext* BehaviorParser::Action_atomContext::scale_atom() {
  return getRuleContext<BehaviorParser::Scale_atomContext>(0);
}

size_t BehaviorParser::Action_atomContext::getRuleIndex() const { return BehaviorParser::RuleAction_atom; }

std::any BehaviorParser::Action_atomContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitAction_atom(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Action_atomContext* BehaviorParser::action_atom()
{
  Action_atomContext* _localctx = _tracker.createInstance<Action_atomContext>(_ctx, getState());
  enterRule(_localctx, 148, BehaviorParser::RuleAction_atom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(581);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(578);
      set_atom();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(579);
      set_objective_atom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(580);
      scale_atom();
      break;
    }

    default:
      break;
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Set_atomContext ------------------------------------------------------------------

BehaviorParser::Set_atomContext::Set_atomContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Set_atomContext::SET() { return getToken(BehaviorParser::SET, 0); }

BehaviorParser::AttributeContext* BehaviorParser::Set_atomContext::attribute()
{
  return getRuleContext<BehaviorParser::AttributeContext>(0);
}

tree::TerminalNode* BehaviorParser::Set_atomContext::TO() { return getToken(BehaviorParser::TO, 0); }

BehaviorParser::Attr_valueContext* BehaviorParser::Set_atomContext::attr_value()
{
  return getRuleContext<BehaviorParser::Attr_valueContext>(0);
}

tree::TerminalNode* BehaviorParser::Set_atomContext::TARGET() { return getToken(BehaviorParser::TARGET, 0); }

size_t BehaviorParser::Set_atomContext::getRuleIndex() const { return BehaviorParser::RuleSet_atom; }

std::any BehaviorParser::Set_atomContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitSet_atom(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Set_atomContext* BehaviorParser::set_atom()
{
  Set_atomContext* _localctx = _tracker.createInstance<Set_atomContext>(_ctx, getState());
  enterRule(_localctx, 150, BehaviorParser::RuleSet_atom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(583);
    match(BehaviorParser::SET);
    setState(585);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::TARGET) {
      setState(584);
      match(BehaviorParser::TARGET);
    }
    setState(587);
    attribute();
    setState(588);
    match(BehaviorParser::TO);
    setState(589);
    attr_value();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Scale_atomContext ------------------------------------------------------------------

BehaviorParser::Scale_atomContext::Scale_atomContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Scale_atomContext::SCALE() { return getToken(BehaviorParser::SCALE, 0); }

BehaviorParser::AttributeContext* BehaviorParser::Scale_atomContext::attribute()
{
  return getRuleContext<BehaviorParser::AttributeContext>(0);
}

BehaviorParser::Attr_valueContext* BehaviorParser::Scale_atomContext::attr_value()
{
  return getRuleContext<BehaviorParser::Attr_valueContext>(0);
}

tree::TerminalNode* BehaviorParser::Scale_atomContext::TARGET()
{
  return getToken(BehaviorParser::TARGET, 0);
}

size_t BehaviorParser::Scale_atomContext::getRuleIndex() const { return BehaviorParser::RuleScale_atom; }

std::any BehaviorParser::Scale_atomContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitScale_atom(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Scale_atomContext* BehaviorParser::scale_atom()
{
  Scale_atomContext* _localctx = _tracker.createInstance<Scale_atomContext>(_ctx, getState());
  enterRule(_localctx, 152, BehaviorParser::RuleScale_atom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(591);
    match(BehaviorParser::SCALE);
    setState(593);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == BehaviorParser::TARGET) {
      setState(592);
      match(BehaviorParser::TARGET);
    }
    setState(595);
    attribute();
    setState(596);
    attr_value();
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Set_objective_atomContext ------------------------------------------------------------------

BehaviorParser::Set_objective_atomContext::Set_objective_atomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* BehaviorParser::Set_objective_atomContext::SET() {
  return getToken(BehaviorParser::SET, 0);
}

tree::TerminalNode* BehaviorParser::Set_objective_atomContext::GOAL() {
  return getToken(BehaviorParser::GOAL, 0);
}

tree::TerminalNode* BehaviorParser::Set_objective_atomContext::TO() {
  return getToken(BehaviorParser::TO, 0);
}

tree::TerminalNode* BehaviorParser::Set_objective_atomContext::RANDOM() {
  return getToken(BehaviorParser::RANDOM, 0);
}

tree::TerminalNode* BehaviorParser::Set_objective_atomContext::LOC_NAME() {
  return getToken(BehaviorParser::LOC_NAME, 0);
}


size_t BehaviorParser::Set_objective_atomContext::getRuleIndex() const {
  return BehaviorParser::RuleSet_objective_atom;
}


std::any BehaviorParser::Set_objective_atomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor))
    return parserVisitor->visitSet_objective_atom(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Set_objective_atomContext* BehaviorParser::set_objective_atom() {
  Set_objective_atomContext *_localctx = _tracker.createInstance<Set_objective_atomContext>(_ctx, getState());
  enterRule(_localctx, 154, BehaviorParser::RuleSet_objective_atom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(598);
    match(BehaviorParser::SET);
    setState(599);
    match(BehaviorParser::GOAL);
    setState(600);
    match(BehaviorParser::TO);
    setState(601);
    match(BehaviorParser::RANDOM);
    setState(602);
    match(BehaviorParser::LOC_NAME);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

BehaviorParser::DeclarationContext::DeclarationContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

BehaviorParser::Decl_Ped_StateContext* BehaviorParser::DeclarationContext::decl_Ped_State()
{
  return getRuleContext<BehaviorParser::Decl_Ped_StateContext>(0);
}

BehaviorParser::Decl_PedContext* BehaviorParser::DeclarationContext::decl_Ped()
{
  return getRuleContext<BehaviorParser::Decl_PedContext>(0);
}

size_t BehaviorParser::DeclarationContext::getRuleIndex() const { return BehaviorParser::RuleDeclaration; }

std::any BehaviorParser::DeclarationContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::DeclarationContext* BehaviorParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 156, BehaviorParser::RuleDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(606);
    _errHandler->sync(this);
    switch ( _input->LA(1) ) {
      case BehaviorParser::STATES: {
        enterOuterAlt(_localctx, 1);
        setState(604);
        decl_Ped_State();
        break;
      }

      case BehaviorParser::TYPES: {
        enterOuterAlt(_localctx, 2);
        setState(605);
        decl_Ped();
        break;
      }

      default:
        throw NoViableAltException(this);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Decl_Ped_StateContext ------------------------------------------------------------------

BehaviorParser::Decl_Ped_StateContext::Decl_Ped_StateContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Decl_Ped_StateContext::STATES()
{
  return getToken(BehaviorParser::STATES, 0);
}

tree::TerminalNode* BehaviorParser::Decl_Ped_StateContext::COLON()
{
  return getToken(BehaviorParser::COLON, 0);
}

std::vector<tree::TerminalNode*> BehaviorParser::Decl_Ped_StateContext::ID()
{
  return getTokens(BehaviorParser::ID);
}

tree::TerminalNode* BehaviorParser::Decl_Ped_StateContext::ID(size_t i)
{
  return getToken(BehaviorParser::ID, i);
}

std::vector<tree::TerminalNode*> BehaviorParser::Decl_Ped_StateContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::Decl_Ped_StateContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::Decl_Ped_StateContext::getRuleIndex() const
{
  return BehaviorParser::RuleDecl_Ped_State;
}

std::any BehaviorParser::Decl_Ped_StateContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitDecl_Ped_State(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Decl_Ped_StateContext* BehaviorParser::decl_Ped_State() {
  Decl_Ped_StateContext *_localctx = _tracker.createInstance<Decl_Ped_StateContext>(_ctx, getState());
  enterRule(_localctx, 158, BehaviorParser::RuleDecl_Ped_State);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(608);
    match(BehaviorParser::STATES);
    setState(609);
    match(BehaviorParser::COLON);
    setState(610);
    match(BehaviorParser::ID);
    setState(617);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == BehaviorParser::COMMA || _la == BehaviorParser::ID) {
      setState(612);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == BehaviorParser::COMMA) {
        setState(611);
        match(BehaviorParser::COMMA);
      }
      setState(614);
      match(BehaviorParser::ID);
      setState(619);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Decl_PedContext ------------------------------------------------------------------

BehaviorParser::Decl_PedContext::Decl_PedContext(ParserRuleContext* parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState)
{
}

tree::TerminalNode* BehaviorParser::Decl_PedContext::TYPES() { return getToken(BehaviorParser::TYPES, 0); }

tree::TerminalNode* BehaviorParser::Decl_PedContext::COLON() { return getToken(BehaviorParser::COLON, 0); }

std::vector<tree::TerminalNode*> BehaviorParser::Decl_PedContext::ID()
{
  return getTokens(BehaviorParser::ID);
}

tree::TerminalNode* BehaviorParser::Decl_PedContext::ID(size_t i) { return getToken(BehaviorParser::ID, i); }

std::vector<tree::TerminalNode*> BehaviorParser::Decl_PedContext::COMMA()
{
  return getTokens(BehaviorParser::COMMA);
}

tree::TerminalNode* BehaviorParser::Decl_PedContext::COMMA(size_t i)
{
  return getToken(BehaviorParser::COMMA, i);
}

size_t BehaviorParser::Decl_PedContext::getRuleIndex() const { return BehaviorParser::RuleDecl_Ped; }

std::any BehaviorParser::Decl_PedContext::accept(tree::ParseTreeVisitor* visitor)
{
  if ( auto parserVisitor = dynamic_cast<BehaviorVisitor*>(visitor) )
    return parserVisitor->visitDecl_Ped(this);
  else
    return visitor->visitChildren(this);
}

BehaviorParser::Decl_PedContext* BehaviorParser::decl_Ped() {
  Decl_PedContext *_localctx = _tracker.createInstance<Decl_PedContext>(_ctx, getState());
  enterRule(_localctx, 160, BehaviorParser::RuleDecl_Ped);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(620);
    match(BehaviorParser::TYPES);
    setState(621);
    match(BehaviorParser::COLON);
    setState(622);
    match(BehaviorParser::ID);
    setState(629);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == BehaviorParser::COMMA || _la == BehaviorParser::ID) {
      setState(624);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == BehaviorParser::COMMA) {
        setState(623);
        match(BehaviorParser::COMMA);
      }
      setState(626);
      match(BehaviorParser::ID);
      setState(631);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
  }
  catch ( RecognitionException& e ) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void BehaviorParser::initialize()
{
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  behaviorParserInitialize();
#else
  ::antlr4::internal::call_once(behaviorParserOnceFlag, behaviorParserInitialize);
#endif
}


// Generated from Behavior.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "BehaviorVisitor.h"


/**
 * This class provides an empty implementation of BehaviorVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  BehaviorBaseVisitor : public BehaviorVisitor {
public:

  virtual std::any visitProgram(BehaviorParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(BehaviorParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocation(BehaviorParser::LocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocation_attribute(BehaviorParser::Location_attributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoc_name(BehaviorParser::Loc_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoc_dimensions(BehaviorParser::Loc_dimensionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoc_center(BehaviorParser::Loc_centerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoc_lengths(BehaviorParser::Loc_lengthsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoc_rotation(BehaviorParser::Loc_rotationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_numeric(BehaviorParser::Value_numericContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_number(BehaviorParser::Value_numberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_float(BehaviorParser::Value_floatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_range(BehaviorParser::Value_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_random(BehaviorParser::Value_randomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRandom_number(BehaviorParser::Random_numberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRandom_float(BehaviorParser::Random_floatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFloat_range(BehaviorParser::Float_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber_range(BehaviorParser::Number_rangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue_coord(BehaviorParser::Value_coordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent(BehaviorParser::EventContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_attribute(BehaviorParser::Event_attributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_name(BehaviorParser::Event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_start(BehaviorParser::Event_startContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_end(BehaviorParser::Event_endContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition(BehaviorParser::ConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary(BehaviorParser::UnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimary(BehaviorParser::PrimaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSub_condition(BehaviorParser::Sub_conditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Time_Elapsed_From_Event(BehaviorParser::Condition_Time_Elapsed_From_EventContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Event_Occurred(BehaviorParser::Condition_Event_OccurredContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Event_Occurring(BehaviorParser::Condition_Event_OccurringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Event_Starting(BehaviorParser::Condition_Event_StartingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Event_Ending(BehaviorParser::Condition_Event_EndingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Spatial(BehaviorParser::Condition_SpatialContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Enter_Location(BehaviorParser::Condition_Enter_LocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Exit_Location(BehaviorParser::Condition_Exit_LocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Inside_Location(BehaviorParser::Condition_Inside_LocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Attribute(BehaviorParser::Condition_AttributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition_Exists(BehaviorParser::Condition_ExistsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDuration(BehaviorParser::DurationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute(BehaviorParser::AttributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttr_value(BehaviorParser::Attr_valueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEnd_goal(BehaviorParser::End_goalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTowards(BehaviorParser::TowardsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTarget(BehaviorParser::TargetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelf(BehaviorParser::SelfContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOther(BehaviorParser::OtherContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNearest_type(BehaviorParser::Nearest_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitModifier(BehaviorParser::ModifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocation_modifier(BehaviorParser::Location_modifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitId_list(BehaviorParser::Id_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDistance(BehaviorParser::DistanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDirection(BehaviorParser::DirectionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFront(BehaviorParser::FrontContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBehind(BehaviorParser::BehindContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPed_Selector(BehaviorParser::Ped_SelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_attribute(BehaviorParser::Selector_attributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_type(BehaviorParser::Selector_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_selector(BehaviorParser::Selector_selectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_from(BehaviorParser::Selector_fromContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_required(BehaviorParser::Selector_requiredContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector(BehaviorParser::SelectorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_Percent(BehaviorParser::Selector_PercentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_Exactly_N_Random(BehaviorParser::Selector_Exactly_N_RandomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_Everyone(BehaviorParser::Selector_EveryoneContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelector_Location(BehaviorParser::Selector_LocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroup(BehaviorParser::GroupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction(BehaviorParser::ActionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction_attribute(BehaviorParser::Action_attributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction_stimulus(BehaviorParser::Action_stimulusContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction_response(BehaviorParser::Action_responseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction_duration(BehaviorParser::Action_durationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction_target(BehaviorParser::Action_targetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSub_action(BehaviorParser::Sub_actionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAction_atom(BehaviorParser::Action_atomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSet_atom(BehaviorParser::Set_atomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitScale_atom(BehaviorParser::Scale_atomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration(BehaviorParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDecl_Ped_State(BehaviorParser::Decl_Ped_StateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDecl_Ped(BehaviorParser::Decl_PedContext *ctx) override {
    return visitChildren(ctx);
  }


};


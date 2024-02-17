
// Generated from Behavior.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "BehaviorParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by BehaviorParser.
 */
class  BehaviorVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by BehaviorParser.
   */
    virtual std::any visitProgram(BehaviorParser::ProgramContext *context) = 0;

    virtual std::any visitStatement(BehaviorParser::StatementContext *context) = 0;

    virtual std::any visitLocation(BehaviorParser::LocationContext *context) = 0;

    virtual std::any visitLocation_attribute(BehaviorParser::Location_attributeContext *context) = 0;

    virtual std::any visitLoc_name(BehaviorParser::Loc_nameContext *context) = 0;

    virtual std::any visitLoc_dimensions(BehaviorParser::Loc_dimensionsContext *context) = 0;

    virtual std::any visitLoc_center(BehaviorParser::Loc_centerContext *context) = 0;

    virtual std::any visitLoc_lengths(BehaviorParser::Loc_lengthsContext *context) = 0;

    virtual std::any visitLoc_rotation(BehaviorParser::Loc_rotationContext *context) = 0;

    virtual std::any visitValue_numeric(BehaviorParser::Value_numericContext *context) = 0;

    virtual std::any visitValue_number(BehaviorParser::Value_numberContext *context) = 0;

    virtual std::any visitValue_float(BehaviorParser::Value_floatContext *context) = 0;

    virtual std::any visitValue_range(BehaviorParser::Value_rangeContext *context) = 0;

    virtual std::any visitValue_random(BehaviorParser::Value_randomContext *context) = 0;

    virtual std::any visitRandom_number(BehaviorParser::Random_numberContext *context) = 0;

    virtual std::any visitRandom_float(BehaviorParser::Random_floatContext *context) = 0;

    virtual std::any visitFloat_range(BehaviorParser::Float_rangeContext *context) = 0;

    virtual std::any visitNumber_range(BehaviorParser::Number_rangeContext *context) = 0;

    virtual std::any visitValue_coord(BehaviorParser::Value_coordContext *context) = 0;

    virtual std::any visitEvent(BehaviorParser::EventContext *context) = 0;

    virtual std::any visitEvent_attribute(BehaviorParser::Event_attributeContext *context) = 0;

    virtual std::any visitEvent_name(BehaviorParser::Event_nameContext *context) = 0;

    virtual std::any visitEvent_start(BehaviorParser::Event_startContext *context) = 0;

    virtual std::any visitEvent_end(BehaviorParser::Event_endContext *context) = 0;

    virtual std::any visitCondition(BehaviorParser::ConditionContext *context) = 0;

    virtual std::any visitUnary(BehaviorParser::UnaryContext *context) = 0;

    virtual std::any visitPrimary(BehaviorParser::PrimaryContext *context) = 0;

    virtual std::any visitSub_condition(BehaviorParser::Sub_conditionContext *context) = 0;

    virtual std::any visitCondition_Time_Elapsed_From_Event(BehaviorParser::Condition_Time_Elapsed_From_EventContext *context) = 0;

    virtual std::any visitCondition_Event_Occurred(BehaviorParser::Condition_Event_OccurredContext *context) = 0;

    virtual std::any visitCondition_Event_Occurring(BehaviorParser::Condition_Event_OccurringContext *context) = 0;

    virtual std::any visitCondition_Event_Starting(BehaviorParser::Condition_Event_StartingContext *context) = 0;

    virtual std::any visitCondition_Event_Ending(BehaviorParser::Condition_Event_EndingContext *context) = 0;

    virtual std::any visitCondition_Spatial(BehaviorParser::Condition_SpatialContext *context) = 0;

    virtual std::any visitCondition_Enter_Location(BehaviorParser::Condition_Enter_LocationContext *context) = 0;

    virtual std::any visitCondition_Exit_Location(BehaviorParser::Condition_Exit_LocationContext *context) = 0;

    virtual std::any visitCondition_Inside_Location(BehaviorParser::Condition_Inside_LocationContext *context) = 0;

    virtual std::any visitCondition_Attribute(BehaviorParser::Condition_AttributeContext *context) = 0;

    virtual std::any visitCondition_Exists(BehaviorParser::Condition_ExistsContext *context) = 0;

    virtual std::any visitDuration(BehaviorParser::DurationContext *context) = 0;

    virtual std::any visitAttribute(BehaviorParser::AttributeContext *context) = 0;

    virtual std::any visitAttr_value(BehaviorParser::Attr_valueContext *context) = 0;

    virtual std::any visitEnd_goal(BehaviorParser::End_goalContext *context) = 0;

    virtual std::any visitTowards(BehaviorParser::TowardsContext *context) = 0;

    virtual std::any visitTarget(BehaviorParser::TargetContext *context) = 0;

    virtual std::any visitSelf(BehaviorParser::SelfContext *context) = 0;

    virtual std::any visitOther(BehaviorParser::OtherContext *context) = 0;

    virtual std::any visitNearest_type(BehaviorParser::Nearest_typeContext *context) = 0;

    virtual std::any visitModifier(BehaviorParser::ModifierContext *context) = 0;

    virtual std::any visitLocation_modifier(BehaviorParser::Location_modifierContext *context) = 0;

    virtual std::any visitId_list(BehaviorParser::Id_listContext *context) = 0;

    virtual std::any visitDistance(BehaviorParser::DistanceContext *context) = 0;

    virtual std::any visitDirection(BehaviorParser::DirectionContext *context) = 0;

    virtual std::any visitFront(BehaviorParser::FrontContext *context) = 0;

    virtual std::any visitBehind(BehaviorParser::BehindContext *context) = 0;

    virtual std::any visitPed_Selector(BehaviorParser::Ped_SelectorContext *context) = 0;

    virtual std::any visitSelector_attribute(BehaviorParser::Selector_attributeContext *context) = 0;

    virtual std::any visitSelector_type(BehaviorParser::Selector_typeContext *context) = 0;

    virtual std::any visitSelector_selector(BehaviorParser::Selector_selectorContext *context) = 0;

    virtual std::any visitSelector_from(BehaviorParser::Selector_fromContext *context) = 0;

    virtual std::any visitSelector_required(BehaviorParser::Selector_requiredContext *context) = 0;

    virtual std::any visitSelector(BehaviorParser::SelectorContext *context) = 0;

    virtual std::any visitSelector_Percent(BehaviorParser::Selector_PercentContext *context) = 0;

    virtual std::any visitSelector_Exactly_N_Random(BehaviorParser::Selector_Exactly_N_RandomContext *context) = 0;

    virtual std::any visitSelector_Everyone(BehaviorParser::Selector_EveryoneContext *context) = 0;

    virtual std::any visitSelector_Location(BehaviorParser::Selector_LocationContext *context) = 0;

    virtual std::any visitGroup(BehaviorParser::GroupContext *context) = 0;

    virtual std::any visitAction(BehaviorParser::ActionContext *context) = 0;

    virtual std::any visitAction_attribute(BehaviorParser::Action_attributeContext *context) = 0;

    virtual std::any visitAction_stimulus(BehaviorParser::Action_stimulusContext *context) = 0;

    virtual std::any visitAction_response(BehaviorParser::Action_responseContext *context) = 0;

    virtual std::any visitAction_duration(BehaviorParser::Action_durationContext *context) = 0;

    virtual std::any visitAction_target(BehaviorParser::Action_targetContext *context) = 0;

    virtual std::any visitSub_action(BehaviorParser::Sub_actionContext *context) = 0;

    virtual std::any visitAction_atom(BehaviorParser::Action_atomContext *context) = 0;

    virtual std::any visitSet_atom(BehaviorParser::Set_atomContext *context) = 0;

    virtual std::any visitScale_atom(BehaviorParser::Scale_atomContext *context) = 0;

    virtual std::any visitDeclaration(BehaviorParser::DeclarationContext *context) = 0;

    virtual std::any visitDecl_Ped_State(BehaviorParser::Decl_Ped_StateContext *context) = 0;

    virtual std::any visitDecl_Ped(BehaviorParser::Decl_PedContext *context) = 0;


};


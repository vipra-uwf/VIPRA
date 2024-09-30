grammar selector;

import lexer_rules, values, common;

ped_Selector:
    SELECTOR ':' selector_attribute*
;

selector_attribute:
    selector_type |
    selector_selector |
    selector_from |
    selector_required
;

selector_type:
    TYPE ':' id_list
;

selector_selector:
    SELECT ':' selector
;

selector_from:
    FROM ':' group
;

selector_required:
    REQUIRED
;

selector:
    selector_Percent |
    selector_Exactly_N_Random |
    selector_Everyone |
    selector_Location
;

selector_Percent:
    value_number '%'
;

selector_Exactly_N_Random:
    value_number
;

selector_Everyone:
    EVERYONE
;

selector_Location:
    IN LOC_NAME
;

group:
    (ID | PEDESTRIAN | PEDESTRIANS )
;
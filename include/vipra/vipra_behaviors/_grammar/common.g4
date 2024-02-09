grammar common;

import lexer_rules;

id_list:
    ((ID | PEDESTRIAN) (COMMA? (ID | PEDESTRIAN))*);
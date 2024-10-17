lexer grammar lexer_rules;

COMMENT : '/*' .*? '*/'    -> skip;
LINE_COMMENT: '//' ~[\r\n]*    -> skip;
WS : [ \t\r\n]+ -> channel(HIDDEN);

fragment A:[aA];
fragment B:[bB];
fragment C:[cC];
fragment D:[dD];
fragment E:[eE];
fragment F:[fF];
fragment G:[gG];
fragment H:[hH];
fragment I:[iI];
fragment J:[jJ];
fragment K:[kK];
fragment L:[lL];
fragment M:[mM];
fragment N:[nN];
fragment O:[oO];
fragment P:[pP];
fragment Q:[qQ];
fragment R:[rR];
fragment S:[sS];
fragment T:[tT];
fragment U:[uU];
fragment V:[vV];
fragment W:[wW];
fragment X:[xX];
fragment Y:[yY];
fragment Z:[zZ];

COMMA: ',';
COLON: ':';

// -- Keep These In This Area ------------

STATE_VAL: '#'[a-zA-Z_\-]+;
EVNT: '!'[a-zA-Z_\-]+;
LOC_NAME: '@'[a-zA-Z_\-]+;

// ---------------------------------------

AN: A | A N;
AT: A T;
ADD: A D D;
AND: A N D;
ARE: A R E;
AFTER: A F T E R;
ALWAYS: A L W A Y S;
AROUND: A R O U N D;
ACTION: A C T I O N;

BEHIND: B E H I N D;
BOTTOM: B O T T O M;

CONSIDER: C O N S I D E R;
CIRCLE: C I R L E;
CENTER: C E N T E R;
CONDITION: C O N D I T I O N;

DECIMAL: D E C I M A L;
DIRECTION: D I R E C T I O N;
DURATION: D U R A T I O N;

END: E N D;
ENDS: E N D S;
ENVIRONMENT: E N V I R O N M E N T;
ENTER: E N T E R;
EXIT: E X I T;
EXISTS: E X I S T S;
EVENT: E V E N T;
EXACTLY: E X A C T L Y;
EVERY: E V E R Y;
EVERYONE: E V E R Y O N E;
ENDING: E N D I N G;

FOR: F O R;
FROM: F R O M;
FRONT: F R O N T;
FASTER: F A S T E R;

GIVEN: G I V E N;
GOAL: G O A L;
GROUP: G R O U P;

HAS: H A S;

IN: I N;
INTEGER: I N T E G E R;
INITIAL: I N I T I A L;
IS: I S;
IF: I F;

LENGTH: L E N G T H;
LENGTHS: L E N G T H S;
LEFT: L E F T;
LEAVE: L E A V E;
DIMENSIONS: D I M E N S I O N S;
LOCATION: L O C A T I O N;

MAX: M A X;
METERS: M E T E R S;

NAME: N A M E;
NEAREST: N E A R E S T;
NORMAL: N O R M A L;
NONE: N O N E;
NOT: N O T;

OBSTACLE: O B S T A C L E;
OCCUR: O C C U R;
OCCURRED: O C C U R R E D;
OCCURRING: O C C U R R I N G;
OF: O F;
ON: O N;
OR: O R;

PEDESTRIAN: P E D E S T R I A N;
PEDESTRIANS: P E D E S T R I A N S;
POINTS: P O I N T S;
POSSIBLE: P O S S I B L E;
POSITION: P O S I T I O N;

RADIUS: R A D I U S;
RANDOM: R A N D O M;
REQUIRED: R E Q U I R E D;
RECTANGLE: R E C T A N G L E;
RIGHT: R I G H T;
RESPONSE: R E S P O N S E;
ROTATION: R O T A T I O N;

SCALE: S C A L E;
SELF: S E L F;
SELECT: S E L E C T;
SELECTOR: S E L E C T O R;
SET: S E T;
SECOND: S E C O N D;
SECONDS: S E C O N D S;
STATUS: S T A T U S;
STARTING: S T A R T I N G;
SLOWER: S L O W E R;
SHAPE: S H A P E;
STATES: S T A T E S;
START: S T A R T;
STATE: S T A T E;
STARTS: S T A R T S;
SPEED: S P E E D;
SUBTRACT: S U B T R A C T;
STIMULUS: S T I M U L U S;

TARGET: T A R G E T;
TO: T O;
TOWARDS: T O W A R D S;
TOP: T O P;
THE: T H E;
THAT: T H A T;
THEN: T H E N;
THEIR: T H E I R;
THERES: T H E R E S;
TYPES: T Y P E S;
TYPE: T Y P E;

UNCONDITIONALLY: U N C O N D I T I O N A L L Y;

VALUE: V A L U E;
VELOCITY: V E L O C I T Y;

WHEN: W H E N;
WHILE: W H I L E;
WHOSE: W H O S E;
WIDTH: W I D T H;
WILL: W I L L;
WITH: W I T H;
WITHIN: W I T H I N;


// ---------- Keep The Following At The End Of The File ------------

ID: [a-zA-Z_]+[a-zA-Z_\-]*;
FLOAT: '-'?[0-9]+'.'[0-9]+;
NUMBER : '-'?[0-9]+;
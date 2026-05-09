/* 
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
  *  Distribution,  Product Version 5.7, and is subject to the Cadence
  *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
  *  this file constitutes your acceptance of the terms of the LEF/DEF
  *  Open Source License and an agreement to abide by its  terms.   If
  *  you  don't  agree  with  this, you must remove this and any other
  *  files which are part of the distribution and  destroy any  copies
  *  made.
  *
  *     For updates, support, or to become part of the LEF/DEF Community,
  *  check www.openeda.org for details.
  *
  *  Error message number:
  *  5000 - def reader, defrReader.cpp
  *  5500 - lex.cpph, yyerror
  *  6000 - def parser, error, lex.cpph, def.y (CALLBACK & CHKERR)
  *    6000 - out of memory
  *    6005 - defiAssertion.cpp
  *    6010 - defiBlockage.cpp
  *    6020 - defiComponent.cpp
  *    6030 - defiFPC.cpp
  *    6040 - defiFill.cpp
  *    6050 - defiGroup.cpp
  *    6060 - defiIOTiming.cpp
  *    6070 - defiMisc.cpp
  *    6080 - defiNet.cpp
  *    6090 - defiNonDefault.cpp
  *    6100 - defiPartition.cpp
  *    6110 - defiPinCap.cpp
  *    6120 - defiPinProp.cpp
  *    6130 - defiRegion.cpp
  *    6140 - defiRowTrack.cpp
  *    6150 - defiScanchain.cpp
  *    6160 - defiSlot.cpp
  *    6170 - defiTimingDisable.cpp
  *    6180 - defiVia.cpp
  *  6500 - def parser, error, def.y
  *  7000 - def parser, warning, lex.cpph
  *  7500 - def parser, warning, lef.y
  *  8000 - def parser, info, lex.cpph
  *  9000 - def writer
  */

 
 %{
 #include <stdlib.h>
 #include <string.h>
 #include <malloc.h>
 #include "defrReader.h"
 #include "lex.h"
 #include "defiUser.h"
 #include "defrCallBacks.h"
 
 #define YYDEBUG 1         /* this is temp fix for pcr 755132 */
 
 extern int dumb_mode; /* Read next N tokens in dumb mode
 */
extern int no_num; /* No numbers for the next N tokens
 */
extern int real_num; /* Next token will be a real number
 */
extern int by_is_keyword;
 extern int bit_is_keyword;
 extern int do_is_keyword;
 extern int new_is_keyword;
 extern int nondef_is_keyword;
 extern int step_is_keyword;
 extern int mustjoin_is_keyword;
 extern int routed_is_keyword;
 extern int cover_is_keyword;
 extern int fixed_is_keyword;
 extern int orient_is_keyword;
 extern char* History_text;
 extern int parsing_property;
 extern int doneDesign;
 
 extern int errors; /* from lex.cpph
 */
extern int nlines; /* from lex.cpph
 */
extern int defInPropDef; /* from lex.cpph, tracking if insided propDef
 */

 extern char* defrFileName;
 
 double save_x;
 double save_y;
 
 static double defVersionNum = 5.7; /* default is 5.7
 */

 static int defIgnoreVersion = 0; /* ignore checking version number
 */
/* this is redundant, since def does not have
 */
/* either 5.3 or 5.4 code
 */

 static int hasVer = 0; /* keep track VERSION is in the file
 */
static int hasNameCase = 0; /* keep track NAMESCASESENSITIVE is in the file
 */
static int hasDivChar = 0; /* keep track DIVIDERCHAR is in the file
 */
static int hasBusBit = 0; /* keep track BUSBITCHARS is in the file
 */
static int hasDes = 0; /* keep track DESIGN is in the file
 */
static int regTypeDef; /* keep track that region type is defined
 */
static int aOxide = 0; /* keep track for oxide
 */
static int netOsnet = 0; /* net = 1 & snet = 2
 */
static int viaRule = 0; /* keep track the viarule has called first
 */
static int hasPort = 0; /* keep track is port defined in a Pin
 */

 static int needNPCbk = 0; /* if cbk for net path is needed
 */
static int needSNPCbk = 0; /* if cbk for snet path is needed
 */
static double xStep, yStep;
 static int hasDoStep;
 static int hasBlkLayerComp; /* only 1 BLOCKAGE/LAYER/COMP
 */
static int hasBlkLayerSpac; /* only 1 BLOCKAGE/LAYER/SPACING
 */
static int hasBlkPlaceComp; /* only 1 BLOCKAGE/PLACEMENT/COMP
 */

 /* the following variables to keep track the number of warnings printed.
 */
static int assertionWarnings = 0;
 static int blockageWarnings = 0;
 static int caseSensitiveWarnings = 0;
 static int componentWarnings = 0;
 static int constraintWarnings = 0;
 static int defaultCapWarnings = 0;
 static int fillWarnings = 0;
 static int gcellGridWarnings = 0;
 static int iOTimingWarnings = 0;
 static int netWarnings = 0;
 static int nonDefaultWarnings = 0;
 static int pinExtWarnings = 0;
 static int pinWarnings = 0;
 static int regionWarnings = 0;
 static int rowWarnings = 0;
 static int scanchainWarnings = 0;
 static int sNetWarnings = 0;
 static int stylesWarnings = 0;
 static int trackWarnings = 0;
 static int unitsWarnings = 0;
 static int versionWarnings = 0;
 static int viaWarnings = 0;
 
 int names_case_sensitive = 1; /* always true in 5.6
 */
int reader_case_sensitive = 0; /* save what the reader prefer
 */
int defRetVal;
 
 int shield = FALSE; /* To identify if the path is shield for 5.3
 */

 static char defPropDefType; /* save the current type of the property
 */
static char* shieldName; /* to hold the shieldNetName
 */
static char* rowName; /* to hold the rowName for message
 */

 int bitsNum; /* Scanchain Bits value
 */
char *warningMsg;
 char *defMsg;
 
 /* From def_keywords.cpp */

 extern char* ringCopy(const char* string);
 
 
 /* Macro to describe how we handle a callback.
 * If the function was set then call it.
 * If the function returns non zero then there was an error
 * so call the error routine and exit.
 */

 #define CALLBACK(func, typ, data) \
 if (!errors) {\
 if (func) { \
 if ((defRetVal = (*func)(typ, data, defrUserData)) == PARSE_OK) { \
 } else if (defRetVal == STOP_PARSE) { \
 	 return defRetVal; \
 } else { \
 defError(6010, "Error in callback"); \
 	 return defRetVal; \
 } \
 } \
 }
 
 #define CHKERR() \
 if (errors > 20) {\
 defError(6011, "Too many syntax errors"); \
 errors = 0; \
 return 1; \
 }
 
 #define CHKPROPTYPE(propType, propName, name) \
 if (propType == 'N') { \
 warningMsg = (char*)defMalloc(strlen(propName)+strlen(name)+35); \
 sprintf(warningMsg, "PropName %s is not defined for %s.", \
 propName, name); \
 defWarning(7010, warningMsg); \
 defFree(warningMsg); \
 }
 
 double convert_defname2num(char *versionName)
 {
 char majorNm[80];
 char minorNm[80];
 char *subMinorNm = NULL;
 char *p1;
 char *versionNm = strdup(versionName);
 
 double major = 0, minor = 0, subMinor = 0;
 double version;
 
 sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
 if ((p1 = strchr(minorNm, '.'))) {
 subMinorNm = p1+1;
 *p1 = '\0';
 }
 major = atof(majorNm);
 minor = atof(minorNm);
 if (subMinorNm)
 subMinor = atof(subMinorNm);
 
 version = major;
 
 if (minor > 0)
 version = major + minor/10;
 
 if (subMinor > 0)
 version = version + subMinor/1000;
 
 free(versionNm);
 return version;
 }
 
 int numIsInt (char* volt) {
 if (strchr(volt, '.')) /* a floating point
 */
return 0;
 else
 return 1;
 }
 
 int defValidNum(int values) {
 char *outMsg;
 switch (values) {
 case 100:
 case 200:
 case 1000:
 case 2000:
 return 1;
 case 10000:
 case 20000:
 if (defVersionNum < 5.6) {
 if (defrUnitsCbk) {
 if (unitsWarnings++ < defrUnitsWarnings) {
 outMsg = (char*)defMalloc(1000);
 sprintf (outMsg,
 "Error found when processing DEF file '%s'\nUnit %d is a 5.6 or later syntax\nYour def file is not defined as 5.6.",
 defrFileName, values);
 defError(6501, outMsg);
 defFree(outMsg);
 }
 }
 return 0;
 } else
 return 1;
 }
 if (defrUnitsCbk) {
 if (unitsWarnings++ < defrUnitsWarnings) {
 outMsg = (char*)defMalloc(10000);
 sprintf (outMsg,
 "The value %d defined for DEF UNITS DISTANCE MICRON is invalid\n. Correct value is 100, 200, 1000, 2000, 10000, or 20000.", values);
 defError(6502, outMsg);
 defFree(outMsg);
 CHKERR();
 }
 }
 return 0;
 }
 
 #define FIXED 1
 #define COVER 2
 #define PLACED 3
 #define UNPLACED 4
 %}
 
 %union {
 double dval ;
 int integer ;
 char * string ;
 int keyword ; /* really just a nop */

 struct defpoint pt;
 defTOKEN *tk;
 }
 
 %token <string> QSTRING
 %token <string> T_STRING SITE_PATTERN
 %token <dval> NUMBER
 %token <keyword> K_HISTORY K_NAMESCASESENSITIVE
 %token <keyword> K_DESIGN K_VIAS K_TECH K_UNITS K_ARRAY K_FLOORPLAN
 %token <keyword> K_SITE K_CANPLACE K_CANNOTOCCUPY K_DIEAREA
 %token <keyword> K_PINS
 %token <keyword> K_DEFAULTCAP K_MINPINS K_WIRECAP
 %token <keyword> K_TRACKS K_GCELLGRID
 %token <keyword> K_DO K_BY K_STEP K_LAYER K_ROW K_RECT
 %token <keyword> K_COMPS K_COMP_GEN K_SOURCE K_WEIGHT K_EEQMASTER
 %token <keyword> K_FIXED K_COVER K_UNPLACED K_PLACED K_FOREIGN K_REGION
 %token <keyword> K_REGIONS
 %token <keyword> K_NETS K_START_NET K_MUSTJOIN K_ORIGINAL K_USE K_STYLE
 %token <keyword> K_PATTERN K_PATTERNNAME K_ESTCAP K_ROUTED K_NEW
 %token <keyword> K_SNETS K_SHAPE K_WIDTH K_VOLTAGE K_SPACING K_NONDEFAULTRULE
 %token <keyword> K_NONDEFAULTRULES
 %token <keyword> K_N K_S K_E K_W K_FN K_FE K_FS K_FW
 %token <keyword> K_GROUPS K_GROUP K_SOFT K_MAXX K_MAXY K_MAXHALFPERIMETER
 %token <keyword> K_CONSTRAINTS K_NET K_PATH K_SUM K_DIFF
 %token <keyword> K_SCANCHAINS K_START K_FLOATING K_ORDERED K_STOP K_IN K_OUT
 %token <keyword> K_RISEMIN K_RISEMAX K_FALLMIN K_FALLMAX K_WIREDLOGIC
 %token <keyword> K_MAXDIST
 %token <keyword> K_ASSERTIONS
 %token <keyword> K_DISTANCE K_MICRONS
 %token <keyword> K_END
 %token <keyword> K_IOTIMINGS K_RISE K_FALL K_VARIABLE K_SLEWRATE K_CAPACITANCE
 %token <keyword> K_DRIVECELL K_FROMPIN K_TOPIN K_PARALLEL
 %token <keyword> K_TIMINGDISABLES K_THRUPIN K_MACRO
 %token <keyword> K_PARTITIONS K_TURNOFF
 %token <keyword> K_FROMCLOCKPIN K_FROMCOMPPIN K_FROMIOPIN
 %token <keyword> K_TOCLOCKPIN K_TOCOMPPIN K_TOIOPIN
 %token <keyword> K_SETUPRISE K_SETUPFALL K_HOLDRISE K_HOLDFALL
 %token <keyword> K_VPIN K_SUBNET K_XTALK K_PIN K_SYNTHESIZED
 %token <keyword> K_DEFINE K_DEFINES K_DEFINEB K_IF K_THEN K_ELSE K_FALSE K_TRUE
 %token <keyword> K_EQ K_NE K_LE K_LT K_GE K_GT K_OR K_AND K_NOT
 %token <keyword> K_SPECIAL K_DIRECTION K_RANGE
 %token <keyword> K_FPC K_HORIZONTAL K_VERTICAL K_ALIGN K_MIN K_MAX K_EQUAL
 %token <keyword> K_BOTTOMLEFT K_TOPRIGHT K_ROWS K_TAPER K_TAPERRULE
 %token <keyword> K_VERSION K_DIVIDERCHAR K_BUSBITCHARS
 %token <keyword> K_PROPERTYDEFINITIONS K_STRING K_REAL K_INTEGER K_PROPERTY
 %token <keyword> K_BEGINEXT K_ENDEXT K_NAMEMAPSTRING K_ON K_OFF K_X K_Y
 %token <keyword> K_COMPONENT
 %token <keyword> K_PINPROPERTIES K_TEST
 %token <keyword> K_COMMONSCANPINS K_SNET K_COMPONENTPIN K_REENTRANTPATHS
 %token <keyword> K_SHIELD K_SHIELDNET K_NOSHIELD
 %token <keyword> K_ANTENNAPINPARTIALMETALAREA K_ANTENNAPINPARTIALMETALSIDEAREA
 %token <keyword> K_ANTENNAPINGATEAREA K_ANTENNAPINDIFFAREA
 %token <keyword> K_ANTENNAPINMAXAREACAR K_ANTENNAPINMAXSIDEAREACAR
 %token <keyword> K_ANTENNAPINPARTIALCUTAREA K_ANTENNAPINMAXCUTCAR
 %token <keyword> K_SIGNAL K_POWER K_GROUND K_CLOCK K_TIEOFF K_ANALOG K_SCAN
 %token <keyword> K_RESET K_RING K_STRIPE K_FOLLOWPIN K_IOWIRE K_COREWIRE
 %token <keyword> K_BLOCKWIRE K_FILLWIRE K_BLOCKAGEWIRE K_PADRING K_BLOCKRING
 %token <keyword> K_BLOCKAGES K_PLACEMENT K_SLOTS K_FILLS K_PUSHDOWN
 %token <keyword> K_NETLIST K_DIST K_USER K_TIMING K_BALANCED K_STEINER K_TRUNK
 %token <keyword> K_FIXEDBUMP K_FENCE K_FREQUENCY K_GUIDE K_MAXBITS
 %token <keyword> K_PARTITION K_TYPE K_ANTENNAMODEL K_DRCFILL
 %token <keyword> K_OXIDE1 K_OXIDE2 K_OXIDE3 K_OXIDE4
 %token <keyword> K_CUTSIZE K_CUTSPACING K_DESIGNRULEWIDTH K_DIAGWIDTH
 %token <keyword> K_ENCLOSURE K_HALO K_GROUNDSENSITIVITY
 %token <keyword> K_HARDSPACING K_LAYERS K_MINCUTS K_NETEXPR
 %token <keyword> K_OFFSET K_ORIGIN K_ROWCOL K_STYLES
 %token <keyword> K_POLYGON K_PORT K_SUPPLYSENSITIVITY K_VIA K_VIARULE K_WIREEXT
 %token <keyword> K_EXCEPTPGNET K_FILLWIREOPC K_OPC K_PARTIAL K_ROUTEHALO
 %type <pt> pt opt_paren
 %type <integer> comp_net_list subnet_opt_syn
 %type <integer> orient
 %type <integer> placement_status
 %type <string> net_type subnet_type track_start use_type shape_type source_type
 %type <string> pattern_type netsource_type
 %type <tk> path paths new_path
 %type <string> risefall opt_pin opt_pattern pin_layer_opt
 %type <string> vpin_status opt_plus track_type region_type
 %type <string> h_or_v turnoff_setup turnoff_hold
 %type <integer> conn_opt partition_maxbits
 
 %%
 
 def_file: version_stmt case_sens_stmt rules end_design
 ;
 
 version_stmt: /* empty */

 | K_VERSION { dumb_mode = 1; no_num = 1; } T_STRING ';'
 {
 defVersionNum = convert_defname2num($3);
 if (defVersionNum > 5.7) {
 char temp[120];
 sprintf(temp,
 "Def parser 5.7 does not support def file with version %s. Parser stops executions.",
 $3);
 defError(6503, temp);
 return 1;
 }
 if (defrVersionStrCbk) {
 CALLBACK(defrVersionStrCbk, defrVersionStrCbkType, $3);
 } else if (defrVersionCbk) {
 CALLBACK(defrVersionCbk, defrVersionCbkType, defVersionNum);
 }
 if (defVersionNum > 5.3 && defVersionNum < 5.4)
 defIgnoreVersion = 1;
 if (defVersionNum < 5.6) /* default to false before 5.6
 */
names_case_sensitive = reader_case_sensitive;
 else
 names_case_sensitive = 1;
 hasVer = 1;
 doneDesign = 0;
 }
 
 case_sens_stmt: /* empty */

 | K_NAMESCASESENSITIVE K_ON ';'
 {
 if (defVersionNum < 5.6) {
 names_case_sensitive = 1;
 if (defrCaseSensitiveCbk)
 CALLBACK(defrCaseSensitiveCbk, defrCaseSensitiveCbkType,
 names_case_sensitive);
 hasNameCase = 1;
 } else
 if (defrCaseSensitiveCbk) /* write error only if cbk is set */

 if (caseSensitiveWarnings++ < defrCaseSensitiveWarnings)
 defWarning(7011, "NAMESCASESENSITIVE statement is obsolete in version 5.6 and later.\nThe DEF parser will ignore this statement.");
 }
 | K_NAMESCASESENSITIVE K_OFF ';'
 {
 if (defVersionNum < 5.6) {
 names_case_sensitive = 0;
 if (defrCaseSensitiveCbk)
 CALLBACK(defrCaseSensitiveCbk, defrCaseSensitiveCbkType,
 names_case_sensitive);
 hasNameCase = 1;
 } else {
 if (defrCaseSensitiveCbk) { /* write error only if cbk is set */

 if (caseSensitiveWarnings++ < defrCaseSensitiveWarnings) {
 defError(6504, "Def parser version 5.7 and later does not support NAMESCASESENSITIVE OFF.\nEither remove this optional construct or set it to ON.");
 CHKERR();
 }
 }
 }
 }
 
 rules: /* empty */

 | rules rule
 | error
 ;
 
 rule: design_section | assertions_section | blockage_section | comps_section
 | constraint_section | extension_section | fill_section
 | floorplan_contraints_section | groups_section | iotiming_section
 | nets_section | nondefaultrule_section | partitions_section
 | pin_props_section | regions_section | scanchains_section
 | slot_section | snets_section | styles_section | timingdisables_section
 | via_section
 ;
 
 design_section: array_name | bus_bit_chars | canplace | cannotoccupy |
 design_name | die_area | divider_char |
 floorplan_name | gcellgrid | history |
 pin_cap_rule | pin_rule | prop_def_section |
 row_rule | site | tech_name | tracks_rule | units
 ;
 
 design_name: K_DESIGN {dumb_mode = 1; no_num = 1; } T_STRING ';'
 	 {
 if (defrDesignCbk)
 CALLBACK(defrDesignCbk, defrDesignStartCbkType, $3);
 hasDes = 1;
 }
 
 end_design: K_END K_DESIGN
 	 {
 doneDesign = 1;
 if (defrDesignEndCbk)
 CALLBACK(defrDesignEndCbk, defrDesignEndCbkType, 0);
 /* 11/16/2001 - pcr 408334
 */
/* Return 1 if there is any errors during parsing
 */
if (errors)
 return 1;
 
 if (!hasVer)
 defWarning(7012, "VERSION is a required statement on DEF file.\nWithout VERSION defined, the DEF file is technically illegal.\nRefer the LEF/DEF Language Reference manual on how to define this statement.");
 if (!hasNameCase && defVersionNum < 5.6)
 defWarning(7013, "NAMESCASESENSITIVE is a required statement on DEF file with version 5.6 and earlier.\nWithout NAMESCASESENSITIVE defined, the DEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
 if (!hasBusBit && defVersionNum < 5.6)
 defWarning(7014, "BUSBITCHARS is a required statement on DEF file with version 5.6 and earlier.\nWithout BUSBITCHARS defined, the DEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
 if (!hasDivChar && defVersionNum < 5.6)
 defWarning(7015, "DIVIDERCHAR is a required statement on DEF file with version 5.6 and earlier.\nWithout DIVIDECHAR defined, the DEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
 if (!hasDes)
 defWarning(7016, "DESIGN is a required statement on DEF file.");
 hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 hasDes = 0;
 
 assertionWarnings = 0;
 blockageWarnings = 0;
 caseSensitiveWarnings = 0;
 componentWarnings = 0;
 constraintWarnings = 0;
 defaultCapWarnings = 0;
 gcellGridWarnings = 0;
 iOTimingWarnings = 0;
 netWarnings = 0;
 nonDefaultWarnings = 0;
 pinExtWarnings = 0;
 pinWarnings = 0;
 regionWarnings = 0;
 rowWarnings = 0;
 scanchainWarnings = 0;
 sNetWarnings = 0;
 stylesWarnings = 0;
 trackWarnings = 0;
 unitsWarnings = 0;
 versionWarnings = 0;
 viaWarnings = 0;
 }
 
 tech_name: K_TECH { dumb_mode = 1; no_num = 1; } T_STRING ';'
 {
 if (defrTechnologyCbk)
 CALLBACK(defrTechnologyCbk, defrTechNameCbkType, $3);
 }
 
 array_name: K_ARRAY {dumb_mode = 1; no_num = 1;} T_STRING ';'
 	 {
 if (defrArrayNameCbk)
 CALLBACK(defrArrayNameCbk, defrArrayNameCbkType, $3);
 }
 
 floorplan_name: K_FLOORPLAN { dumb_mode = 1; no_num = 1; } T_STRING ';'
 	 {
 if (defrFloorPlanNameCbk)
 CALLBACK(defrFloorPlanNameCbk, defrFloorPlanNameCbkType, $3);
 }
 
 history: K_HISTORY
 {
 if (defrHistoryCbk)
 CALLBACK(defrHistoryCbk, defrHistoryCbkType, History_text);
 }
 
 prop_def_section: K_PROPERTYDEFINITIONS
 	 {
 parsing_property = 1;
 defInPropDef = 1; /* set flag as inside propertydefinitions */

 if (defrPropDefStartCbk)
 CALLBACK(defrPropDefStartCbk, defrPropDefStartCbkType, 0);
 }
 property_defs K_END K_PROPERTYDEFINITIONS
 	 {
 if (defrPropDefEndCbk)
 CALLBACK(defrPropDefEndCbk, defrPropDefEndCbkType, 0);
 real_num = 0; /* just want to make sure it is reset */

 parsing_property = 0;
 defInPropDef = 0; /* reset flag */

 }
 
 property_defs: /* empty */

 | property_defs property_def
 { }
 
 property_def: K_DESIGN {dumb_mode = 1; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "design",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrDesignProp),
 $3,
defPropDefType) ;
 }
 | K_NET { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "net",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrNetProp),
 $3,
defPropDefType) ;
 }
 | K_SNET { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "specialnet",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrSNetProp),
 $3,
defPropDefType) ;
 }
 | K_REGION { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "region",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrRegionProp),
 $3,
defPropDefType) ;
 }
 | K_GROUP { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "group",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrGroupProp),
 $3,
defPropDefType) ;
 }
 | K_COMPONENT { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "component",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrCompProp),
 $3,
defPropDefType) ;
 }
 | K_ROW { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "row",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrRowProp),
 $3,
defPropDefType) ;
 }
 | K_PIN { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "pin",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrPinDefProp),
 $3,
defPropDefType) ;
 }
 | K_COMPONENTPIN
 { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "componentpin",
$3) ;
 		CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrCompPinProp),
 $3,
defPropDefType) ;
 }
 | K_NONDEFAULTRULE
 { dumb_mode = 1 ; no_num = 1; defiProp_clear( &(defrProp) ) ;
 }
 T_STRING property_type_and_val ';'
 {
 if (defVersionNum < 5.6) {
 if (nonDefaultWarnings++ < defrNonDefaultWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "NONDEFAULTRULE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6505, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 } else {
 if (defrPropCbk) {
 defiProp_setPropType( &(defrProp),
 "nondefaultrule",
$3) ;
 		 CALLBACK(defrPropCbk, defrPropCbkType, &defrProp);
 }
 defiPropType_setPropType( &(defrNDefProp),
 $3,
defPropDefType) ;
 }
 }
 | error ';' { yyerrok; yyclearin;}
 
 property_type_and_val: K_INTEGER { real_num = 0 } opt_range opt_num_val
 {
 if (defrPropCbk) defiProp_setPropInteger( &(defrProp) ) ;
 defPropDefType = 'I';
 }
 | K_REAL { real_num = 1 } opt_range opt_num_val
 {
 if (defrPropCbk) defiProp_setPropReal( &(defrProp) ) ;
 defPropDefType = 'R';
 real_num = 0;
 }
 | K_STRING
 {
 if (defrPropCbk) defiProp_setPropString( &(defrProp) ) ;
 defPropDefType = 'S';
 }
 | K_STRING QSTRING
 {
 if (defrPropCbk) defiProp_setPropQString( &(defrProp),
 $2) ;
 defPropDefType = 'Q';
 }
 | K_NAMEMAPSTRING T_STRING
 {
 if (defrPropCbk) defiProp_setPropNameMapString( &(defrProp),
 $2) ;
 defPropDefType = 'S';
 }
 
 opt_num_val: /* empty */

 | NUMBER
 { if (defrPropCbk) defiProp_setNumber( &(defrProp),
 $1) ;
 }
 
 units: K_UNITS K_DISTANCE K_MICRONS NUMBER ';'
 	 {
 if (defrUnitsCbk) {
 if (defValidNum((int)$4))
 CALLBACK(defrUnitsCbk, defrUnitsCbkType, $4);
 }
 }
 
 divider_char: K_DIVIDERCHAR QSTRING ';'
 	 {
 if (defrDividerCbk)
 CALLBACK(defrDividerCbk, defrDividerCbkType, $2);
 hasDivChar = 1;
 }
 
 bus_bit_chars: K_BUSBITCHARS QSTRING ';'
 	 {
 if (defrBusBitCbk)
 CALLBACK(defrBusBitCbk, defrBusBitCbkType, $2);
 hasBusBit = 1;
 }
 
 site: K_SITE { dumb_mode = 1; no_num = 1; defiSite_clear( &(defrSite) ) ;
 }
 T_STRING NUMBER NUMBER orient
 K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER ';'
 	 {
 if (defrSiteCbk) {
 defiSite_setName( &(defrSite),
 $3) ;
 defiSite_setLocation( &(defrSite),
 $4,
$5) ;
 defiSite_setOrient( &(defrSite),
 $6) ;
 defiSite_setDo( &(defrSite),
 $8,
$10,
$12,
$13) ;
 	 CALLBACK(defrSiteCbk, defrSiteCbkType, &(defrSite));
 }
 	 }
 
 canplace: K_CANPLACE {dumb_mode = 1;no_num = 1; } T_STRING NUMBER NUMBER
 orient K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER ';'
 {
 if (defrCanplaceCbk) {
 defiSite_setName( &(defrCanplace),
 $3) ;
 defiSite_setLocation( &(defrCanplace),
 $4,
$5) ;
 defiSite_setOrient( &(defrCanplace),
 $6) ;
 defiSite_setDo( &(defrCanplace),
 $8,
$10,
$12,
$13) ;
 		CALLBACK(defrCanplaceCbk, defrCanplaceCbkType,
 		&(defrCanplace));
 }
 }
 cannotoccupy: K_CANNOTOCCUPY {dumb_mode = 1;no_num = 1; } T_STRING NUMBER NUMBER
 orient K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER ';'
 {
 if (defrCannotOccupyCbk) {
 defiSite_setName( &(defrCannotOccupy),
 $3) ;
 defiSite_setLocation( &(defrCannotOccupy),
 $4,
$5) ;
 defiSite_setOrient( &(defrCannotOccupy),
 $6) ;
 defiSite_setDo( &(defrCannotOccupy),
 $8,
$10,
$12,
$13) ;
 		CALLBACK(defrCannotOccupyCbk, defrCannotOccupyCbkType,
 &(defrCannotOccupy));
 }
 }
 
 orient: K_N {$$ = 0;}
 | K_W {$$ = 1;}
 | K_S {$$ = 2;}
 | K_E {$$ = 3;}
 | K_FN {$$ = 4;}
 | K_FW {$$ = 5;}
 | K_FS {$$ = 6;}
 | K_FE {$$ = 7;}
 
 die_area: K_DIEAREA
 	 {
 if (!defrGeomPtr) { /* set up for more than 4 points
 */
defrGeomPtr =
 (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 	 }
 firstPt nextPt otherPts ';'
 {
 	 if (defrDieAreaCbk) {
 defiBox_addPoint( &(defrDieArea),
 defrGeomPtr) ;
 CALLBACK(defrDieAreaCbk, defrDieAreaCbkType, &(defrDieArea));
 }
 }
 
 /* 8/31/2001 - This is obsolete in 5.4 */

 pin_cap_rule: start_def_cap pin_caps end_def_cap
 { }
 
 start_def_cap: K_DEFAULTCAP NUMBER
 	{
 if (defVersionNum < 5.4) {
 if (defrDefaultCapCbk)
 CALLBACK(defrDefaultCapCbk, defrDefaultCapCbkType, ROUND($2));
 } else {
 if (defrDefaultCapCbk) /* write error only if cbk is set */

 if (defaultCapWarnings++ < defrDefaultCapWarnings)
 defWarning(7017, "DEFAULTCAP statement is obsolete in version 5.4 and earlier.\nThe DEF parser will ignore this statement.");
 }
 }
 
 pin_caps: /* empty */

 | pin_caps pin_cap
 ;
 
 pin_cap: K_MINPINS NUMBER K_WIRECAP NUMBER ';'
 	 {
 if (defVersionNum < 5.4) {
 	 if (defrPinCapCbk) {
 	 defiPinCap_setPin( &(defrPinCap),
 ROUND($2)) ;
 	 defiPinCap_setCap( &(defrPinCap),
 $4) ;
 	 CALLBACK(defrPinCapCbk, defrPinCapCbkType, &(defrPinCap));
 	 }
 }
 	 }
 
 end_def_cap: K_END K_DEFAULTCAP
 { }
 
 pin_rule: start_pins pins end_pins
 { }
 
 start_pins: K_PINS NUMBER ';'
 	 {
 if (defrStartPinsCbk)
 CALLBACK(defrStartPinsCbk, defrStartPinsCbkType, ROUND($2));
 }
 
 pins: /* empty */

 | pins pin
 ;
 
 pin: '-' {dumb_mode = 1; no_num = 1; } T_STRING '+' K_NET
 	 {dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrPinCbk || defrPinExtCbk) {
 defiPin_Setup( &(defrPin),
 $3,
$7) ;
 }
 hasPort = 0;
 }
 pin_options ';'
 {
 if (defrPinCbk)
 CALLBACK(defrPinCbk, defrPinCbkType, &defrPin);
 }
 
 pin_options: /* empty */

 | pin_options pin_option
 
 pin_option: '+' K_SPECIAL
 {
 if (defrPinCbk)
 defiPin_setSpecial( &(defrPin) ) ;
 }
 
 | extension_stmt
 {
 if (defrPinExtCbk)
 CALLBACK(defrPinExtCbk, defrPinExtCbkType, History_text);
 }
 
 | '+' K_DIRECTION T_STRING
 {
 if (defrPinCbk || defrPinExtCbk)
 defiPin_setDirection( &(defrPin),
 $3) ;
 }
 
 | '+' K_NETEXPR QSTRING
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "NETEXPR statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6506, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk)
 defiPin_setNetExpr( &(defrPin),
 $3) ;
 
 }
 }
 
 | '+' K_SUPPLYSENSITIVITY { dumb_mode = 1; } T_STRING
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "SUPPLYSENSITIVITY statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6507, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk)
 defiPin_setSupplySens( &(defrPin),
 $4) ;
 }
 }
 
 | '+' K_GROUNDSENSITIVITY { dumb_mode = 1; } T_STRING
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "GROUNDSENSITIVITY statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6508, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk)
 defiPin_setGroundSens( &(defrPin),
 $4) ;
 }
 }
 
 | '+' K_USE use_type
 {
 if (defrPinCbk || defrPinExtCbk) defiPin_setUse( &(defrPin),
 $3) ;
 }
 | '+' K_PORT /* 5.7 */

 {
 if (defVersionNum < 5.7) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "PORT in PINS is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6555, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addPort( &(defrPin) ) ;
 hasPort = 1;
 }
 }
 
 | '+' K_LAYER { dumb_mode = 1; } T_STRING
 {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortLayer( &(defrPin),
 $4) ;
 else
 defiPin_addLayer( &(defrPin),
 $4) ;
 }
 }
 pin_layer_spacing_opt pt pt
 {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortLayerPts( &(defrPin),
 $7.x,
$7.y,
$8.x,
$8.y) ;
 else
 defiPin_addLayerPts( &(defrPin),
 $7.x,
$7.y,
$8.x,
$8.y) ;
 }
 }
 
 | '+' K_POLYGON { dumb_mode = 1; } T_STRING
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "POLYGON statement is a version 5.6 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6509, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortPolygon( &(defrPin),
 $4) ;
 else
 defiPin_addPolygon( &(defrPin),
 $4) ;
 }
 }
 if (!defrGeomPtr) {
 defrGeomPtr =
 (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else { /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 }
 pin_poly_spacing_opt firstPt nextPt nextPt otherPts
 {
 if (defVersionNum >= 5.6) { /* only add if 5.6 or beyond
 */
if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortPolygonPts( &(defrPin),
 defrGeomPtr) ;
 else
 defiPin_addPolygonPts( &(defrPin),
 defrGeomPtr) ;
 }
 }
 }
 | '+' K_VIA { dumb_mode = 1; } T_STRING '(' NUMBER NUMBER ')' /* 5.7
  */
{
 if (defVersionNum < 5.7) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "PIN VIA statement is a version 5.7 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6556, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortVia( &(defrPin),
 $4,
$6,
$7) ;
 else
 defiPin_addVia( &(defrPin),
 $4,
$6,
$7) ;
 }
 }
 }
 
 | placement_status pt orient
 {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_setPortPlacement( &(defrPin),
 $1,
$2.x,
$2.y,
$3) ;
 else
 defiPin_setPlacement( &(defrPin),
 $1,
$2.x,
$2.y,
$3) ;
 }
 }
 
 /* The following is 5.4 syntax */

 | '+' K_ANTENNAPINPARTIALMETALAREA NUMBER pin_layer_opt
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINPARTIALMETALAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6510, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinPartialMetalArea( &(defrPin),
 (int)$3,$4) ;
 }
 | '+' K_ANTENNAPINPARTIALMETALSIDEAREA NUMBER pin_layer_opt
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINPARTIALMETALSIDEAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6511, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinPartialMetalSideArea( &(defrPin),
 (int)$3,$4) ;
 }
 | '+' K_ANTENNAPINGATEAREA NUMBER pin_layer_opt
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINGATEAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6512, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinGateArea( &(defrPin),
 (int)$3,$4) ;
 }
 | '+' K_ANTENNAPINDIFFAREA NUMBER pin_layer_opt
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINDIFFAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6513, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinDiffArea( &(defrPin),
 (int)$3,$4) ;
 }
 | '+' K_ANTENNAPINMAXAREACAR NUMBER K_LAYER {dumb_mode=1;} T_STRING
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINMAXAREACAR statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6514, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinMaxAreaCar( &(defrPin),
 (int)$3,$6) ;
 }
 | '+' K_ANTENNAPINMAXSIDEAREACAR NUMBER K_LAYER {dumb_mode=1;}
 T_STRING
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINMAXSIDEAREACAR statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6515, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinMaxSideAreaCar( &(defrPin),
 (int)$3,$6) ;
 }
 | '+' K_ANTENNAPINPARTIALCUTAREA NUMBER pin_layer_opt
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINPARTIALCUTAREA statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6516, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinPartialCutArea( &(defrPin),
 (int)$3,$4) ;
 }
 | '+' K_ANTENNAPINMAXCUTCAR NUMBER K_LAYER {dumb_mode=1;} T_STRING
 {
 if (defVersionNum <= 5.3) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAPINMAXCUTCAR statement is a version 5.4 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6517, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAPinMaxCutCar( &(defrPin),
 (int)$3,$6) ;
 }
 | '+' K_ANTENNAMODEL pin_oxide
 { /* 5.5 syntax */

 if (defVersionNum < 5.5) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6518, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 }
 
 pin_layer_spacing_opt: /* empty */

 | K_SPACING NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "SPACING statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6519, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortLayerSpacing( &(defrPin),
 (int)$2) ;
 else
 defiPin_addLayerSpacing( &(defrPin),
 (int)$2) ;
 }
 }
 }
 | K_DESIGNRULEWIDTH NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "DESIGNRULEWIDTH statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6520, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortLayerDesignRuleWidth( &(defrPin),
 (int)$2) ;
 else
 defiPin_addLayerDesignRuleWidth( &(defrPin),
 (int)$2) ;
 }
 }
 }
 
 pin_poly_spacing_opt: /* empty */

 | K_SPACING NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "SPACING statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6521, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortPolySpacing( &(defrPin),
 (int)$2) ;
 else
 defiPin_addPolySpacing( &(defrPin),
 (int)$2) ;
 }
 }
 }
 | K_DESIGNRULEWIDTH NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrPinCbk || defrPinExtCbk) {
 if ((pinWarnings++ < defrPinWarnings) &&
 (pinWarnings++ < defrPinExtWarnings)) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "DESIGNRULEWIDTH statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6522, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrPinCbk || defrPinExtCbk) {
 if (hasPort)
 defiPin_addPortPolyDesignRuleWidth( &(defrPin),
 (int)$2) ;
 else
 defiPin_addPolyDesignRuleWidth( &(defrPin),
 (int)$2) ;
 }
 }
 }
 
 pin_oxide: K_OXIDE1
 { aOxide = 1;
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAntennaModel( &(defrPin),
 aOxide) ;
 }
 | K_OXIDE2
 { aOxide = 2;
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAntennaModel( &(defrPin),
 aOxide) ;
 }
 | K_OXIDE3
 { aOxide = 3;
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAntennaModel( &(defrPin),
 aOxide) ;
 }
 | K_OXIDE4
 { aOxide = 4;
 if (defrPinCbk || defrPinExtCbk)
 defiPin_addAntennaModel( &(defrPin),
 aOxide) ;
 }
 
 use_type: K_SIGNAL
 { $$ = (char*)"SIGNAL"; }
 | K_POWER
 { $$ = (char*)"POWER"; }
 | K_GROUND
 { $$ = (char*)"GROUND"; }
 | K_CLOCK
 { $$ = (char*)"CLOCK"; }
 | K_TIEOFF
 { $$ = (char*)"TIEOFF"; }
 | K_ANALOG
 { $$ = (char*)"ANALOG"; }
 | K_SCAN
 { $$ = (char*)"SCAN"; }
 | K_RESET
 { $$ = (char*)"RESET"; }
 
 pin_layer_opt:
 /* empty */

 { $$ = (char*)""; }
 | K_LAYER {dumb_mode=1;} T_STRING
 { $$ = $3; }
 
 end_pins: K_END K_PINS
 	{
 if (defrPinEndCbk)
 CALLBACK(defrPinEndCbk, defrPinEndCbkType, 0);
 }
 
 row_rule: K_ROW {dumb_mode = 2; no_num = 2; } T_STRING T_STRING NUMBER NUMBER
 orient
 {
 if (defrRowCbk) {
 rowName = $3;
 defiRow_setup( &(defrRow),
 $3,
$4,
$5,
$6,
$7) ;
 }
 }
 row_do_option
 row_options ';'
 {
 if (defrRowCbk)
 	 CALLBACK(defrRowCbk, defrRowCbkType, &defrRow);
 }
 
 row_do_option: /* empty */

 {
 if (defVersionNum < 5.6) {
 if (defrRowCbk) {
 if (rowWarnings++ < defrRowWarnings) {
 defError(6523, "The DO statement which is required in ROW statement is missing.");
 CHKERR();
 }
 }
 }
 }
 | K_DO NUMBER K_BY NUMBER row_step_option
 {
 /* 06/05/2002 - pcr 448455 */

 /* Check for 1 and 0 in the correct position */

 /* 07/26/2002 - Commented out due to pcr 459218 */

 if (hasDoStep) {
 /* 04/29/2004 - pcr 695535 */

 /* changed the testing */

 if ((($4 == 1) && (yStep == 0)) ||
 (($2 == 1) && (xStep == 0))) {
 /* do nothing */

 } else
 if (defVersionNum < 5.6) {
 if (defrRowCbk) {
 if (rowWarnings++ < defrRowWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf(defMsg,
 "ROW %s: Both spaceX and spaceY values were specified. The correct syntax for ROW DO statement is \"DO numX BY 1 STEP spaceX 0 | DO 1 BY numY STEP 0 spaceY\". Only the space direction for the appropriate row orientation was retained.", rowName);
 defWarning(7018, defMsg);
 defFree(defMsg);
 }
 }
 }
 }
 /* pcr 459218 - Error if at least numX or numY does not equal 1 */

 if (($2 != 1) && ($4 != 1)) {
 if (defrRowCbk) {
 if (rowWarnings++ < defrRowWarnings) {
 defError(6524, "Syntax error, correct syntax is either \"DO 1 BY num... or DO numX BY 1...\"");
 CHKERR();
 }
 }
 }
 if (defrRowCbk)
 defiRow_setDo( &(defrRow),
 ROUND($2),ROUND($4),xStep,yStep) ;
 }
 
 row_step_option: /* empty */

 {
 hasDoStep = 0;
 }
 | K_STEP NUMBER NUMBER
 {
 hasDoStep = 1;
 defiRow_setHasDoStep( &(defrRow) ) ;
 xStep = $2;
 yStep = $3;
 }
 
 row_options: /* empty */

 | row_options row_option
 ;
 
 row_option : '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1;}
 row_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 
 row_prop_list : /* empty */

 | row_prop_list row_prop
 ;
 
 row_prop : T_STRING NUMBER
 {
 if (defrRowCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrRowProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "ROW");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiRow_addNumProperty( &(defrRow),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrRowCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrRowProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "ROW");
 defiRow_addProperty( &(defrRow),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrRowCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrRowProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "ROW");
 defiRow_addProperty( &(defrRow),
 $1,
$2,
propTp) ;
 }
 }
 
 tracks_rule: track_start NUMBER
 {
 if (defrTrackCbk) {
 defiTrack_setup( &(defrTrack),
 $1) ;
 }
 }
 K_DO NUMBER K_STEP NUMBER track_layer_statement ';'
 {
 if (($5 <= 0) && (defVersionNum >= 5.4)) {
 if (defrTrackCbk)
 if (trackWarnings++ < defrTrackWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "The DO number %g in TRACK is invalid.\nThe number value has to be greater than 0", $5);
 defError(6525, defMsg);
 defFree(defMsg);
 }
 }
 if ($7 < 0) {
 if (defrTrackCbk)
 if (trackWarnings++ < defrTrackWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "The STEP number %g in TRACK is invalid.\nThe number value has to be greater than 0", $7);
 defError(6526, defMsg);
 defFree(defMsg);
 }
 }
 if (defrTrackCbk) {
 defiTrack_setDo( &(defrTrack),
 ROUND($2),ROUND($5),$7) ;
 	 CALLBACK(defrTrackCbk, defrTrackCbkType, &defrTrack);
 }
 }
 
 track_start: K_TRACKS track_type
 {
 $$ = $2;
 }
 
 track_type: K_X
 { $$ = (char*)"X";}
 | K_Y
 { $$ = (char*)"Y";}
 
 track_layer_statement: /* empty */

 | K_LAYER { dumb_mode = 1000; } track_layer track_layers
 { dumb_mode = 0; }
 
 track_layers: /* empty */

 | track_layer track_layers
 ;
 
 track_layer: T_STRING
 {
 if (defrTrackCbk)
 defiTrack_addLayer( &(defrTrack),
 $1) ;
 }
 
 gcellgrid: K_GCELLGRID track_type NUMBER
 K_DO NUMBER K_STEP NUMBER ';'
 	{
 if ($5 <= 0) {
 if (defrGcellGridCbk)
 if (gcellGridWarnings++ < defrGcellGridWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "The DO number %g in GCELLGRID is invalid.\nThe number value has to be greater than 0", $5);
 defError(6527, defMsg);
 defFree(defMsg);
 }
 }
 if ($7 < 0) {
 if (defrGcellGridCbk)
 if (gcellGridWarnings++ < defrGcellGridWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "The STEP number %g in GCELLGRID is invalid.\nThe number value has to be greater than 0", $7);
 defError(6528, defMsg);
 defFree(defMsg);
 }
 }
 	 if (defrGcellGridCbk) {
 	 defiGcellGrid_setup( &(defrGcellGrid),
 $2,
ROUND($3),ROUND($5),$7) ;
 	 CALLBACK(defrGcellGridCbk, defrGcellGridCbkType, &defrGcellGrid);
 	 }
 	}
 
 extension_section: K_BEGINEXT
 {
 if (defrExtensionCbk)
 CALLBACK(defrExtensionCbk, defrExtensionCbkType, History_text);
 }
 
 extension_stmt: '+' K_BEGINEXT
 { }
 
 via_section: via via_declarations via_end
 ;
 
 via: K_VIAS NUMBER ';'
 {
 if (defrViaStartCbk)
 CALLBACK(defrViaStartCbk, defrViaStartCbkType, ROUND($2));
 }
 
 via_declarations: /* empty */

 | via_declarations via_declaration
 ;
 
 via_declaration: '-' {dumb_mode = 1;no_num = 1; } T_STRING
 {
 if (defrViaCbk) defiVia_setup( &(defrVia),
 $3) ;
 viaRule = 0;
 }
 layer_stmts ';'
 {
 if (defrViaCbk)
 CALLBACK(defrViaCbk, defrViaCbkType, &defrVia);
 defiVia_clear( &(defrVia) ) ;
 }
 
 layer_stmts: /* empty */

 | layer_stmts layer_stmt
 ;
 
 layer_stmt: '+' K_RECT {dumb_mode = 1;no_num = 1; } T_STRING pt pt
 {
 if (defrViaCbk)
 defiVia_addLayer( &(defrVia),
 $4,
$5.x,
$5.y,
$6.x,
$6.y) ;
 }
 | '+' K_POLYGON { dumb_mode = 1; } T_STRING
 {
 if (defVersionNum < 5.6) {
 if (defrViaCbk) {
 if (viaWarnings++ < defrViaWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "POLYGON statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6529, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (!defrGeomPtr) {
 defrGeomPtr =
 (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else { /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 }
 firstPt nextPt nextPt otherPts
 {
 if (defVersionNum >= 5.6) { /* only add if 5.6 or beyond
 */
if (defrViaCbk)
 defiVia_addPolygon( &(defrVia),
 $4,
defrGeomPtr) ;
 }
 }
 | '+' K_PATTERNNAME {dumb_mode = 1;no_num = 1; } T_STRING
 {
 if (defVersionNum < 5.6) {
 if (defrViaCbk)
 defiVia_addPattern( &(defrVia),
 $4) ;
 } else
 if (defrViaCbk)
 if (viaWarnings++ < defrViaWarnings)
 defWarning(7019, "PATTERNNAME statement is obsolete in version 5.6 and later.\nDEF parser will ignore this statement.");
 }
 | '+' K_VIARULE {dumb_mode = 1;no_num = 1; } T_STRING
 '+' K_CUTSIZE NUMBER NUMBER
 '+' K_LAYERS {dumb_mode = 3;no_num = 1; } T_STRING T_STRING T_STRING
 '+' K_CUTSPACING NUMBER NUMBER
 '+' K_ENCLOSURE NUMBER NUMBER NUMBER NUMBER
 {
 viaRule = 1;
 if (defVersionNum < 5.6) {
 if (defrViaCbk) {
 if (viaWarnings++ < defrViaWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "VIARULE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6529, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrViaCbk)
 defiVia_addViaRule( &(defrVia),
 $4,
(int)$7,(int)$8,$12,$13,$14,(int)$17,(int)$18,(int)$21,(int)$22,(int)$23,(int)$24) ;
 }
 }
 | layer_viarule_opts
 | extension_stmt
 {
 if (defrViaExtCbk)
 CALLBACK(defrViaExtCbk, defrViaExtCbkType, History_text);
 }
 
 layer_viarule_opts: '+' K_ROWCOL NUMBER NUMBER
 {
 if (!viaRule) {
 if (defrViaCbk) {
 if (viaWarnings++ < defrViaWarnings) {
 defError (6530, "The ROWCOL statement has to be located in a VIARULE statement");
 CHKERR();
 }
 }
 } else if (defrViaCbk)
 defiVia_addRowCol( &(defrVia),
 (int)$3,(int)$4) ;
 }
 | '+' K_ORIGIN NUMBER NUMBER
 {
 if (!viaRule) {
 if (defrViaCbk) {
 if (viaWarnings++ < defrViaWarnings) {
 defError (6531, "The ORIGIN statement has to be located in a VIARULE statement");
 CHKERR();
 }
 }
 } else if (defrViaCbk)
 defiVia_addOrigin( &(defrVia),
 (int)$3,(int)$4) ;
 }
 | '+' K_OFFSET NUMBER NUMBER NUMBER NUMBER
 {
 if (!viaRule) {
 if (defrViaCbk) {
 if (viaWarnings++ < defrViaWarnings) {
 defError (6532, " The OFFSET statement has to be located in a VIARULE statement");
 CHKERR();
 }
 }
 } else if (defrViaCbk)
 defiVia_addOffset( &(defrVia),
 (int)$3,(int)$4,(int)$5,(int)$6) ;
 }
 | '+' K_PATTERN {dumb_mode = 1;no_num = 1; } T_STRING
 {
 if (!viaRule) {
 if (defrViaCbk) {
 if (viaWarnings++ < defrViaWarnings) {
 defError (6533, " The PATTERN statement has to be located in a VIARULE statement");
 CHKERR();
 }
 }
 } else if (defrViaCbk)
 defiVia_addCutPattern( &(defrVia),
 $4) ;
 }
 
 firstPt: pt
 { defiGeometries_startList( defrGeomPtr,
 $1.x,
$1.y) ;
 }
 
 nextPt: pt
 { defiGeometries_addToList( defrGeomPtr,
 $1.x,
$1.y) ;
 }
 
 otherPts: /* empty */

 | nextPt otherPts
 ;
 
 pt: '(' NUMBER NUMBER ')'
 {
 save_x = $2;
 save_y = $3;
 $$.x = ROUND($2);
 $$.y = ROUND($3);
 }
 | '(' '*' NUMBER ')'
 {
 save_y = $3;
 $$.x = ROUND(save_x);
 $$.y = ROUND($3);
 }
 | '(' NUMBER '*' ')'
 {
 save_x = $2;
 $$.x = ROUND($2);
 $$.y = ROUND(save_y);
 }
 | '(' '*' '*' ')'
 {
 $$.x = ROUND(save_x);
 $$.y = ROUND(save_y);
 }
 
 via_end: K_END K_VIAS
 	{
 if (defrViaEndCbk)
 CALLBACK(defrViaEndCbk, defrViaEndCbkType, 0);
 }
 
 regions_section: regions_start regions_stmts K_END K_REGIONS
 	{
 if (defrRegionEndCbk)
 CALLBACK(defrRegionEndCbk, defrRegionEndCbkType, 0);
 }
 
 regions_start: K_REGIONS NUMBER ';'
 	{
 if (defrRegionStartCbk)
 CALLBACK(defrRegionStartCbk, defrRegionStartCbkType, ROUND($2));
 }
 
 regions_stmts: /* empty */

 | regions_stmts regions_stmt
 {}
 
 regions_stmt: '-' { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrRegionCbk)
 defiRegion_setup( &(defrRegion),
 $3) ;
 regTypeDef = 0;
 }
 rect_list region_options ';'
 { CALLBACK(defrRegionCbk, defrRegionCbkType, &defrRegion); }
 
 rect_list :
 pt pt
 { if (defrRegionCbk)
 	 defiRegion_addRect( &(defrRegion),
 $1.x,
$1.y,
$2.x,
$2.y) ;
 }
 | rect_list pt pt
 { if (defrRegionCbk)
 	 defiRegion_addRect( &(defrRegion),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 ;
 
 region_options: /* empty */

 | region_options region_option
 ;
 
 region_option : '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
 region_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 | '+' K_TYPE region_type /* 5.4.1 */

 {
 if (regTypeDef) {
 if (defrRegionCbk) {
 if (regionWarnings++ < defrRegionWarnings) {
 defError(6534, "The TYPE statement has already defined in the REGION statement");
 CHKERR();
 }
 }
 }
 if (defrRegionCbk) defiRegion_setType( &(defrRegion),
 $3) ;
 regTypeDef = 1;
 }
 ;
 
 region_prop_list : /* empty */

 | region_prop_list region_prop
 ;
 
 region_prop : T_STRING NUMBER
 {
 if (defrRegionCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrRegionProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "REGION");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiRegion_addNumProperty( &(defrRegion),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrRegionCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrRegionProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "REGION");
 defiRegion_addProperty( &(defrRegion),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrRegionCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrRegionProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "REGION");
 defiRegion_addProperty( &(defrRegion),
 $1,
$2,
propTp) ;
 }
 }
 
 region_type: K_FENCE
 { $$ = (char*)"FENCE"; }
 | K_GUIDE
 { $$ = (char*)"GUIDE"; }
 
 comps_section: start_comps comps_rule end_comps
 ;
 
 start_comps: K_COMPS NUMBER ';'
 {
 if (defrComponentStartCbk)
 CALLBACK(defrComponentStartCbk, defrComponentStartCbkType,
 ROUND($2));
 }
 
 comps_rule: /* empty */

 | comps_rule comp
 ;
 
 comp: comp_start comp_options ';'
 	 {
 if (defrComponentCbk)
 CALLBACK(defrComponentCbk, defrComponentCbkType, &defrComponent);
 }
 
 comp_start: comp_id_and_name comp_net_list
 {
 dumb_mode = 0;
 no_num = 0;
 /* The net connections were added to the array in
 * reverse order so fix the order now */

 /* Do not need to reverse the order since the storing is
 in sequence now 08/07/2001
 if (defrComponentCbk)
 defiComponent_reverseNetOrder( &(defrComponent) ) ;
 */

 }
 
 comp_id_and_name: '-' {dumb_mode = 1000000000; no_num = 10000000; }
 T_STRING T_STRING
 {
 if (defrComponentCbk)
 defiComponent_IdAndName( &(defrComponent),
 $3,
$4) ;
 }
 
 comp_net_list: /* empty */

 { }
 | comp_net_list '*'
 {
 if (defrComponentCbk)
 defiComponent_addNet( &(defrComponent),
 "*") ;
 }
 | comp_net_list T_STRING
 {
 if (defrComponentCbk)
 defiComponent_addNet( &(defrComponent),
 $2) ;
 }
 
 comp_options: /* empty */

 | comp_options comp_option
 ;
 
 comp_option: comp_generate | comp_source | comp_type | weight |
 comp_foreign | comp_region | comp_eeq | comp_halo |
 comp_routehalo | comp_property | comp_extension_stmt
 ;
 
 comp_extension_stmt: extension_stmt
 {
 if (defrComponentCbk)
 CALLBACK(defrComponentExtCbk, defrComponentExtCbkType,
 History_text);
 }
 
 comp_eeq: '+' K_EEQMASTER {dumb_mode=1; no_num = 1; } T_STRING
 {
 if (defrComponentCbk)
 defiComponent_setEEQ( &(defrComponent),
 $4) ;
 }
 
 comp_generate: '+' K_COMP_GEN { dumb_mode = 2; no_num = 2; } T_STRING
 opt_pattern
 {
 if (defrComponentCbk)
 defiComponent_setGenerate( &(defrComponent),
 $4,
$5) ;
 }
 opt_pattern :
 /* empty */

 { $$ = (char*)""; }
 | T_STRING
 { $$ = $1; }
 
 comp_source: '+' K_SOURCE source_type
 {
 if (defrComponentCbk)
 defiComponent_setSource( &(defrComponent),
 $3) ;
 }
 
 source_type: K_NETLIST
 { $$ = (char*)"NETLIST"; }
 | K_DIST
 { $$ = (char*)"DIST"; }
 | K_USER
 { $$ = (char*)"USER"; }
 | K_TIMING
 { $$ = (char*)"TIMING"; }
 
 
 comp_region:
 	comp_region_start comp_pnt_list
 { }
 | comp_region_start T_STRING
 {
 	 if (defrComponentCbk)
 	 defiComponent_setRegionName( &(defrComponent),
 $2) ;
 	}
 
 comp_pnt_list: pt pt
 	{
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5) {
 if (defrComponentCbk)
 	 defiComponent_setRegionBounds( &(defrComponent),
 $1.x,
$1.y,
$2.x,
$2.y) ;
 }
 else
 defWarning(7020, "REGION pt pt statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 	}
 | comp_pnt_list pt pt
 	{
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5) {
 	 if (defrComponentCbk)
 	 defiComponent_setRegionBounds( &(defrComponent),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 else
 defWarning(7020, "REGION pt pt statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 	}
 
 comp_halo: '+' K_HALO /* 5.7 */

 {
 if (defVersionNum < 5.6) {
 if (defrComponentCbk) {
 if (componentWarnings++ < defrComponentWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "HALO statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6529, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 }
 halo_soft NUMBER NUMBER NUMBER NUMBER
 {
 if (defrComponentCbk)
 defiComponent_setHalo( &(defrComponent),
 (int)$5,(int)$6,(int)$7,(int)$8) ;
 }
 
 halo_soft: /* empty */
/* 5.7 */

 | K_SOFT
 {
 if (defVersionNum < 5.7) {
 if (defrComponentCbk) {
 if (componentWarnings++ < defrComponentWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "HALO SOFT is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6550, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrComponentCbk)
 defiComponent_setHaloSoft( &(defrComponent) ) ;
 }
 }
 
 /* 5.7 */

 comp_routehalo: '+' K_ROUTEHALO NUMBER { dumb_mode = 2; no_num = 2; } T_STRING T_STRING
 {
 if (defVersionNum < 5.7) {
 if (defrComponentCbk) {
 if (componentWarnings++ < defrComponentWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "ROUTEHALO is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6551, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrComponentCbk)
 defiComponent_setRouteHalo( &(defrComponent),
 $3,
$5,
$6) ;
 }
 }
 
 comp_property: '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
 comp_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 
 comp_prop_list: comp_prop
 | comp_prop_list comp_prop
 ;
 
 comp_prop: T_STRING NUMBER
 {
 if (defrComponentCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrCompProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "COMPONENT");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiComponent_addNumProperty( &(defrComponent),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrComponentCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrCompProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "COMPONENT");
 defiComponent_addProperty( &(defrComponent),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrComponentCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrCompProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "COMPONENT");
 defiComponent_addProperty( &(defrComponent),
 $1,
$2,
propTp) ;
 }
 }
 
 comp_region_start: '+' K_REGION
 { dumb_mode = 1; no_num = 1; }
 
 comp_foreign: '+' K_FOREIGN { dumb_mode = 1; no_num = 1; } T_STRING
 opt_paren orient
 {
 if (defVersionNum < 5.6) {
 if (defrComponentCbk) {
 defiComponent_setForeignName( &(defrComponent),
 $4) ;
 defiComponent_setForeignLocation( &(defrComponent),
 $5.x,
$5.y,
$6) ;
 }
 } else
 if (defrComponentCbk)
 if (componentWarnings++ < defrComponentWarnings)
 defWarning(7021, "FOREIGN statement is obsolete in version 5.6 and later.\nThe DEF parser will ignore this statement.");
 }
 
 opt_paren:
 pt
 	 { $$ = $1; }
 | NUMBER NUMBER
 	 { $$.x = ROUND($1); $$.y = ROUND($2); }
 
 comp_type: placement_status pt orient
 {
 if (defrComponentCbk) {
 defiComponent_setPlacementStatus( &(defrComponent),
 $1) ;
 defiComponent_setPlacementLocation( &(defrComponent),
 $2.x,
$2.y,
$3) ;
 }
 }
 | '+' K_UNPLACED
 {
 if (defrComponentCbk)
 defiComponent_setPlacementStatus( &(defrComponent),
 DEFI_COMPONENT_UNPLACED) ;
 defiComponent_setPlacementLocation( &(defrComponent),
 -1,
-1,
-1) ;
 }
 | '+' K_UNPLACED pt orient
 {
 if (defVersionNum < 5.4) { /* PCR 495463 */

 if (defrComponentCbk) {
 defiComponent_setPlacementStatus( &(defrComponent),
 DEFI_COMPONENT_UNPLACED) ;
 defiComponent_setPlacementLocation( &(defrComponent),
 $3.x,
$3.y,
$4) ;
 }
 } else {
 if (componentWarnings++ < defrComponentWarnings)
 defWarning(7022, "In the COMPONENT UNPLACED statement, point & orient are invalid in version 5.4 and later.\nDEF parser will ignore this statement.");
 /* Should not be called. Def parser should ignore the statement
 if (defrComponentCbk)
 defiComponent_setPlacementStatus( &(defrComponent),
 DEFI_COMPONENT_UNPLACED) ;
 defiComponent_setPlacementLocation( &(defrComponent),
 $3.x,
$3.y,
$4) ;
 */

 }
 }
 
 placement_status: '+' K_FIXED
 { $$ = DEFI_COMPONENT_FIXED; }
 | '+' K_COVER
 { $$ = DEFI_COMPONENT_COVER; }
 | '+' K_PLACED
 { $$ = DEFI_COMPONENT_PLACED; }
 
 weight: '+' K_WEIGHT NUMBER
 {
 if (defrComponentCbk)
 defiComponent_setWeight( &(defrComponent),
 ROUND($3)) ;
 }
 
 end_comps: K_END K_COMPS
 {
 if (defrComponentCbk)
 CALLBACK(defrComponentEndCbk, defrComponentEndCbkType, 0);
 }
 
 nets_section: start_nets net_rules end_nets
 ;
 
 start_nets: K_NETS NUMBER ';'
 	{
 if (defrNetStartCbk)
 CALLBACK(defrNetStartCbk, defrNetStartCbkType, ROUND($2));
 netOsnet = 1;
 }
 
 net_rules: /* empty */

 | net_rules one_net
 ;
 
 one_net: net_and_connections net_options ';'
 	{
 if (defrNetCbk)
 CALLBACK(defrNetCbk, defrNetCbkType, &defrNet);
 }
 /* 
** net_and_connections: net_start {dumb_mode = 1000000000; no_num = 10000000;}
 ** net_connections
 ** wmd -- this can be used to replace
 ** | '(' K_PIN {dumb_mode = 1; no_num = 1;} T_STRING conn_opt ')' (???)
 */

 net_and_connections: net_start
 {dumb_mode = 0; no_num = 0; }
 
 /* pcr 235555 & 236210 */

 net_start: '-' {dumb_mode = 1000000000; no_num = 10000000; nondef_is_keyword = TRUE; mustjoin_is_keyword = TRUE;} net_name
 
 net_name: T_STRING
 {
 /* 9/22/1999 */

 /* this is shared by both net and special net */

 if ((defrNetCbk && (netOsnet==1)) || (defrSNetCbk && (netOsnet==2)))
 defiNet_setName( &(defrNet),
 $1) ;
 if (defrNetNameCbk)
 CALLBACK(defrNetNameCbk, defrNetNameCbkType, $1);
 } net_connections
 | K_MUSTJOIN '(' T_STRING {dumb_mode = 1; no_num = 1;} T_STRING ')'
 {
 if ((defrNetCbk && (netOsnet==1)) || (defrSNetCbk && (netOsnet==2)))
 defiNet_addMustPin( &(defrNet),
 $3,
$5,
0) ;
 dumb_mode = 3;
 no_num = 3;
 }
 
 net_connections: /* empty */

 | net_connections net_connection
 ;
 
 net_connection: '(' T_STRING {dumb_mode = 1000000000; no_num = 10000000;}
 T_STRING conn_opt ')'
 {
 /* 9/22/1999 */

 /* since the code is shared by both net & special net, */

 /* need to check on both flags */

 if ((defrNetCbk && (netOsnet==1)) || (defrSNetCbk && (netOsnet==2)))
 defiNet_addPin( &(defrNet),
 $2,
$4,
$5) ;
 /* 1/14/2000 - pcr 289156 */

 /* reset dumb_mode & no_num to 3 , just in case */

 /* the next statement is another net_connection */

 dumb_mode = 3;
 no_num = 3;
 }
 | '(' '*' {dumb_mode = 1; no_num = 1;} T_STRING conn_opt ')'
 {
 if ((defrNetCbk && (netOsnet==1)) || (defrSNetCbk && (netOsnet==2)))
 defiNet_addPin( &(defrNet),
 "*",
$4,
$5) ;
 dumb_mode = 3;
 no_num = 3;
 }
 | '(' K_PIN {dumb_mode = 1; no_num = 1;} T_STRING conn_opt ')'
 {
 if ((defrNetCbk && (netOsnet==1)) || (defrSNetCbk && (netOsnet==2)))
 defiNet_addPin( &(defrNet),
 "PIN",
$4,
$5) ;
 dumb_mode = 3;
 no_num = 3;
 }
 
 conn_opt: /* empty */

 { $$ = 0; }
 | extension_stmt
 {
 if (defrNetConnectionExtCbk)
 	 CALLBACK(defrNetConnectionExtCbk, defrNetConnectionExtCbkType,
 History_text);
 $$ = 0;
 }
 | '+' K_SYNTHESIZED
 { $$ = 1; }
 
 /* These are all the optional fields for a net that go after the '+' */

 net_options: /* empty */

 | net_options net_option
 ;
 
 net_option: '+' net_type
 {
 if (defrNetCbk) defiNet_addWire( &(defrNet),
 $2,
NULL) ;
 }
 paths
 {
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 nondef_is_keyword = FALSE;
 mustjoin_is_keyword = FALSE;
 step_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 needNPCbk = 0;
 }
 
 | '+' K_SOURCE netsource_type
 { if (defrNetCbk) defiNet_setSource( &(defrNet),
 $3) ;
 }
 
 | '+' K_FIXEDBUMP
 {
 if (defVersionNum < 5.5) {
 if (defrNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "FIXEDBUMP statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6530, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrNetCbk) defiNet_setFixedbump( &(defrNet) ) ;
 }
 
 | '+' K_FREQUENCY { real_num = 1; } NUMBER
 {
 if (defVersionNum < 5.5) {
 if (defrNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "FREQUENCY statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6530, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrNetCbk) defiNet_setFrequency( &(defrNet),
 $4) ;
 real_num = 0;
 }
 
 | '+' K_ORIGINAL {dumb_mode = 1; no_num = 1;} T_STRING
 { if (defrNetCbk) defiNet_setOriginal( &(defrNet),
 $4) ;
 }
 
 | '+' K_PATTERN pattern_type
 { if (defrNetCbk) defiNet_setPattern( &(defrNet),
 $3) ;
 }
 
 | '+' K_WEIGHT NUMBER
 { if (defrNetCbk) defiNet_setWeight( &(defrNet),
 ROUND($3)) ;
 }
 
 | '+' K_XTALK NUMBER
 { if (defrNetCbk) defiNet_setXTalk( &(defrNet),
 ROUND($3)) ;
 }
 
 | '+' K_ESTCAP NUMBER
 { if (defrNetCbk) defiNet_setCap( &(defrNet),
 $3) ;
 }
 
 | '+' K_USE use_type
 { if (defrNetCbk) defiNet_setUse( &(defrNet),
 $3) ;
 }
 
 | '+' K_STYLE NUMBER
 { if (defrNetCbk) defiNet_setStyle( &(defrNet),
 (int)$3) ;
 }
 
 | '+' K_NONDEFAULTRULE { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrNetCbk && defrNetNonDefaultRuleCbk) {
 /* User wants a callback on nondefaultrule */

 CALLBACK(defrNetNonDefaultRuleCbk,
 defrNetNonDefaultRuleCbkType, $4);
 }
 /* Still save data in the class */

 if (defrNetCbk) defiNet_setNonDefaultRule( &(defrNet),
 $4) ;
 }
 
 | vpin_stmt
 
 | '+' K_SHIELDNET { dumb_mode = 1; no_num = 1; } T_STRING
 { if (defrNetCbk) defiNet_addShieldNet( &(defrNet),
 $4) ;
 }
 
 | '+' K_NOSHIELD { dumb_mode = 1; no_num = 1; }
 { /* since the parser still support 5.3 and earlier, can't */

 /* move NOSHIELD in net_type */

 if (defVersionNum < 5.4) { /* PCR 445209 */

 if (defrNetCbk) defiNet_addNoShield( &(defrNet),
 "") ;
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 shield = TRUE; /* save the path info in the shield paths */

 } else
 if (defrNetCbk) defiNet_addWire( &(defrNet),
 "NOSHIELD",
NULL) ;
 }
 paths
 {
 if (defVersionNum < 5.4) { /* PCR 445209 */

 shield = FALSE;
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 nondef_is_keyword = FALSE;
 mustjoin_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 } else {
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 nondef_is_keyword = FALSE;
 mustjoin_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 }
 needNPCbk = 0;
 }
 
 | '+' K_SUBNET
 { dumb_mode = 1; no_num = 1;
 if (defrNetCbk) {
 defrSubnet = (defiSubnet*)defMalloc(sizeof(defiSubnet));
 defiSubnet_Init( defrSubnet ) ;
 }
 }
 T_STRING {
 if (defrNetCbk && defrNetSubnetNameCbk) {
 /* User wants a callback on Net subnetName */

 CALLBACK(defrNetSubnetNameCbk, defrNetSubnetNameCbkType, $4);
 }
 /* Still save the subnet name in the class */

 if (defrNetCbk) {
 defiSubnet_setName( defrSubnet,
 $4) ;
 }
 }
 comp_names {
 routed_is_keyword = TRUE;
 fixed_is_keyword = TRUE;
 cover_is_keyword = TRUE;
 } subnet_options {
 if (defrNetCbk) {
 defiNet_addSubnet( &(defrNet),
 defrSubnet) ;
 defrSubnet = 0;
 routed_is_keyword = FALSE;
 fixed_is_keyword = FALSE;
 cover_is_keyword = FALSE;
 }
 }
 
 | '+' K_PROPERTY {dumb_mode = 10000000; parsing_property = 1; }
 net_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 
 | extension_stmt
 {
 if (defrNetExtCbk)
 CALLBACK(defrNetExtCbk, defrNetExtCbkType, History_text);
 }
 
 net_prop_list: net_prop
 | net_prop_list net_prop
 ;
 
 net_prop: T_STRING NUMBER
 {
 if (defrNetCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrNetProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "NET");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiNet_addNumProp( &(defrNet),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrNetCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrNetProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "NET");
 defiNet_addProp( &(defrNet),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrNetCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrNetProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "NET");
 defiNet_addProp( &(defrNet),
 $1,
$2,
propTp) ;
 }
 }
 
 netsource_type: K_NETLIST
 { $$ = (char*)"NETLIST"; }
 | K_DIST
 { $$ = (char*)"DIST"; }
 | K_USER
 { $$ = (char*)"USER"; }
 | K_TIMING
 { $$ = (char*)"TIMING"; }
 | K_TEST
 { $$ = (char*)"TEST"; }
 
 vpin_stmt: vpin_begin vpin_layer_opt pt pt
 {
 /* vpin_options may have to deal with orient */

 orient_is_keyword = TRUE;
 }
 vpin_options
 { if (defrNetCbk)
 defiNet_addVpinBounds( &(defrNet),
 $3.x,
$3.y,
$4.x,
$4.y) ;
 orient_is_keyword = FALSE;
 }
 
 vpin_begin: '+' K_VPIN {dumb_mode = 1; no_num = 1;} T_STRING
 { if (defrNetCbk) defiNet_addVpin( &(defrNet),
 $4) ;
 }
 
 vpin_layer_opt: /* empty */

 | K_LAYER {dumb_mode=1;} T_STRING
 { if (defrNetCbk) defiNet_addVpinLayer( &(defrNet),
 $3) ;
 }
 
 vpin_options: /* empty */

 | vpin_status pt orient
 { if (defrNetCbk) defiNet_addVpinLoc( &(defrNet),
 $1,
$2.x,
$2.y,
$3) ;
 }
 
 vpin_status: K_PLACED
 { $$ = (char*)"PLACED"; }
 | K_FIXED
 { $$ = (char*)"FIXED"; }
 | K_COVER
 { $$ = (char*)"COVER"; }
 
 net_type: K_FIXED
 { $$ = (char*)"FIXED"; dumb_mode = 1; }
 | K_COVER
 { $$ = (char*)"COVER"; dumb_mode = 1; }
 | K_ROUTED
 { $$ = (char*)"ROUTED"; dumb_mode = 1; }
 
 paths:
 path /* not necessary to do partial callback for net yet
 */
{ if (defrNeedPathData && defrNetCbk)
 pathIsDone(shield, 0, netOsnet, &needNPCbk);
 }
 | paths new_path
 { }
 
 new_path: K_NEW { dumb_mode = 1; } path
 { if (defrNeedPathData && defrNetCbk)
 pathIsDone(shield, 0, netOsnet, &needNPCbk);
 }
 
 path: T_STRING
 {
 /* 9/26/2002 - pcr 449514
 Check if $1 is equal to TAPER or TAPERRULE, layername
 is missing */

 if ((strcmp($1, "TAPER") == 0) || (strcmp($1, "TAPERRULE") == 0)) {
 if (defrNeedPathData && defrNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defError(6531, "layerName which is required in path is missing");
 CHKERR();
 }
 }
 /* Since there is already error, the next token is insignificant */

 dumb_mode = 1; no_num = 1;
 } else {
 if (defrNeedPathData && defrNetCbk)
 defiPath_addLayer( defrPath, $1) ;
 dumb_mode = 0; no_num = 0;
 }
 }
 opt_taper_style_s path_pt
 { dumb_mode = 1000000000; by_is_keyword = TRUE; do_is_keyword = TRUE;
 /* 
dumb_mode = 1; by_is_keyword = TRUE; do_is_keyword = TRUE;
 */

 new_is_keyword = TRUE; step_is_keyword = TRUE;
 orient_is_keyword = TRUE; }
 path_item_list
 { dumb_mode = 0; }
 
 path_item_list: /* empty */

 | path_item_list path_item
 ;
 
 path_item:
 T_STRING
 {
 if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2)))) {
 if (strcmp($1, "TAPER") == 0)
 defiPath_setTaper( defrPath ) ;
 else
 defiPath_addVia( defrPath,
 $1) ;
 }
 }
 | T_STRING orient
 { if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2)))) {
 defiPath_addVia( defrPath,
 $1) ;
 defiPath_addViaRotation( defrPath,
 $2) ;
 }
 }
 | T_STRING K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
 {
 if (defVersionNum < 5.5) {
 if (defrNeedPathData && defrSNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "VIA DO statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6532, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (($3 == 0) || ($5 == 0)) {
 if (defrNeedPathData && defrSNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defError(6533, "Either the numX or numY in the VIA DO statement has the value 0.\nThis is illegal.");
 CHKERR();
 }
 }
 }
 if (defrNeedPathData &&
 (defrSNetCbk && (netOsnet==2))) {
 defiPath_addVia( defrPath,
 $1) ;
 defiPath_addViaData( defrPath,
 (int)$3,(int)$5,(int)$7,(int)$8) ;
 } else
 if (defrNeedPathData && (defrNetCbk && (netOsnet == 1))) {
 if (netWarnings++ < defrNetWarnings) {
 defError(6567, "The VIA DO statement is defined in the NET statement and is invalid.\nRemove this statement from your DEF file and try again.");
 CHKERR();
 }
 }
 }
 | T_STRING orient K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
 {
 if (defVersionNum < 5.5) {
 if (defrNeedPathData && defrSNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "VIA DO statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6532, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (($4 == 0) || ($6 == 0)) {
 if (defrNeedPathData && defrSNetCbk) {
 if (netWarnings++ < defrNetWarnings) {
 defError(6533, "Either the numX or numY in the VIA DO statement has the value 0.\nThis is illegal.");
 CHKERR();
 }
 }
 }
 if (defrNeedPathData &&
 (defrSNetCbk && (netOsnet==2))) {
 defiPath_addVia( defrPath,
 $1) ;
 defiPath_addViaRotation( defrPath,
 $2) ;
 defiPath_addViaData( defrPath,
 (int)$4,(int)$6,(int)$8,(int)$9) ;
 } else
 if (defrNeedPathData && (defrNetCbk && (netOsnet == 1))) {
 if (netWarnings++ < defrNetWarnings) {
 defError(6567, "The VIA DO statement is defined in the NET statement and is invalid.\nRemove this statement from your DEF file and try again.");
 CHKERR();
 }
 }
 }
 | path_pt
 { /* reset dumb_mode to 1 just incase the next token is a via of the path
 */
/* 2/5/2004 - pcr 686781
 */
dumb_mode = 100000; by_is_keyword = TRUE; do_is_keyword = TRUE;
 new_is_keyword = TRUE; step_is_keyword = TRUE;
 orient_is_keyword = TRUE; }
 
 
 path_pt :
 '(' NUMBER NUMBER ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addPoint( defrPath,
 ROUND($2),ROUND($3)) ;
 	save_x = $2;
 	save_y = $3;
 }
 | '(' '*' NUMBER ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addPoint( defrPath,
 ROUND(save_x),ROUND($3)) ;
 	save_y = $3;
 }
 | '(' NUMBER '*' ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addPoint( defrPath,
 ROUND($2),ROUND(save_y)) ;
 	save_x = $2;
 }
 | '(' '*' '*' ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addPoint( defrPath,
 ROUND(save_x),ROUND(save_y)) ;
 }
 | '(' NUMBER NUMBER NUMBER ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addFlushPoint( defrPath,
 ROUND($2),ROUND($3),ROUND($4)) ;
 	save_x = $2;
 	save_y = $3;
 }
 | '(' '*' NUMBER NUMBER ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addFlushPoint( defrPath,
 ROUND(save_x),ROUND($3),ROUND($4)) ;
 	save_y = $3;
 }
 | '(' NUMBER '*' NUMBER ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addFlushPoint( defrPath,
 ROUND($2),ROUND(save_y),ROUND($4)) ;
 	save_x = $2;
 }
 | '(' '*' '*' NUMBER ')'
 {
 	if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 	 defiPath_addFlushPoint( defrPath,
 ROUND(save_x),ROUND(save_y),ROUND($4)) ;
 }
 
 opt_taper_style_s: /* empty */

 | opt_taper_style_s opt_taper_style
 ;
 opt_taper_style: opt_style
 | opt_taper
 ;
 
 opt_taper: K_TAPER
 { if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 defiPath_setTaper( defrPath ) ;
 }
 | K_TAPERRULE { dumb_mode = 1; } T_STRING
 { if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 defiPath_addTaperRule( defrPath,
 $3) ;
 }
 
 opt_style: K_STYLE NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrNeedPathData && (defrNetCbk || defrSNetCbk)) {
 if (netWarnings++ < defrNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "STYLE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6534, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else
 if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 defiPath_addStyle( defrPath,
 (int)$2) ;
 }
 
 opt_spaths: /* empty */

 | opt_spaths opt_shape_style
 ;
 
 opt_shape_style:
 '+' K_SHAPE shape_type
 { if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 defiPath_addShape( defrPath,
 $3) ;
 }
 | '+' K_STYLE NUMBER
 { if (defVersionNum < 5.6) {
 if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2)))) {
 if (netWarnings++ < defrNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "STYLE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6534, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrNeedPathData && ((defrNetCbk && (netOsnet==1)) ||
 (defrSNetCbk && (netOsnet==2))))
 defiPath_addStyle( defrPath,
 (int)$3) ;
 }
 }
 
 end_nets: K_END K_NETS
 	 {
 CALLBACK(defrNetEndCbk, defrNetEndCbkType, 0);
 netOsnet = 0;
 }
 
 shape_type: K_RING
 { $$ = (char*)"RING"; }
 | K_STRIPE
 { $$ = (char*)"STRIPE"; }
 | K_FOLLOWPIN
 { $$ = (char*)"FOLLOWPIN"; }
 | K_IOWIRE
 { $$ = (char*)"IOWIRE"; }
 | K_COREWIRE
 { $$ = (char*)"COREWIRE"; }
 | K_BLOCKWIRE
 { $$ = (char*)"BLOCKWIRE"; }
 | K_FILLWIRE
 { $$ = (char*)"FILLWIRE"; }
 | K_FILLWIREOPC /* 5.7 */

 {
 if (defVersionNum < 5.7) {
 if (defrNeedPathData) {
 if (fillWarnings++ < defrFillWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "FILLWIREOPC is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6552, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 $$ = (char*)"FILLWIREOPC";
 }
 | K_DRCFILL
 { $$ = (char*)"DRCFILL"; }
 | K_BLOCKAGEWIRE
 { $$ = (char*)"BLOCKAGEWIRE"; }
 | K_PADRING
 { $$ = (char*)"PADRING"; }
 | K_BLOCKRING
 { $$ = (char*)"BLOCKRING"; }
 
 snets_section : start_snets snet_rules end_snets
 ;
 
 snet_rules: /* empty */

 | snet_rules snet_rule
 ;
 
 snet_rule: net_and_connections snet_options ';'
 	{ CALLBACK(defrSNetCbk, defrSNetCbkType, &defrNet); }
 
 snet_options: /* empty */

 | snet_options snet_option
 ;
 
 snet_option: snet_width | snet_voltage |
 snet_spacing | snet_other_option
 ;
 
 snet_other_option: '+' net_type
 {
 if (defrSNetCbk) { /* PCR 902306 */

 defMsg = (char*)defMalloc(1024);
 sprintf(defMsg, "The SPECIAL NET statement, with type %s, does not have any net statement defined.\nThe DEF parser will ignore this statemnet.", $2);
 defWarning(7023, defMsg);
 defFree(defMsg);
 }
 }
 | '+' net_type
 {
 if (defrSNetCbk) defiNet_addWire( &(defrNet),
 $2,
NULL) ;
 }
 spaths
 {
 /* 7/17/2003 - Fix for pcr 604848, add a callback for each wire
 */
if (defrSNetWireCbk) {
 CALLBACK(defrSNetWireCbk, defrSNetWireCbkType, &defrNet);
 defiNet_freeWire( &(defrNet) ) ;
 }
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 needSNPCbk = 0;
 }
 
 | '+' K_SHIELD { dumb_mode = 1; no_num = 1; } T_STRING
 { shieldName = $4; }
 shield_layer
 
 | '+' K_POLYGON { dumb_mode = 1; } T_STRING
 {
 if (defVersionNum < 5.6) {
 if (defrSNetCbk) {
 if (sNetWarnings++ < defrSNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "POLYGON statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6535, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (!defrGeomPtr) {
 defrGeomPtr =
 (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else { /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 }
 firstPt nextPt nextPt otherPts
 {
 if (defVersionNum >= 5.6) { /* only add if 5.6 or beyond
 */
if (defrSNetCbk) {
 /* needSNPCbk will indicate that it has reach the max
 */
/* memory and if user has set partialPathCBk, def parser
 */
/* will make the callback.
 */
/* This will improve performance
 */
/* This construct is only in specialnet
 */
defiNet_addPolygon( &(defrNet),
 $4,
defrGeomPtr,
&needSNPCbk) ;
 if (needSNPCbk && defrSNetPartialPathCbk) {
 CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
 &defrNet);
 defiNet_clearRectPolyNPath( &(defrNet) ) ;
 }
 }
 }
 }
 
 | '+' K_RECT { dumb_mode = 1; } T_STRING pt pt
 {
 if (defVersionNum < 5.6) {
 if (defrSNetCbk) {
 if (sNetWarnings++ < defrSNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "RECT statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6536, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrSNetCbk) {
 /* needSNPCbk will indicate that it has reach the max
 */
/* memory and if user has set partialPathCBk, def parser
 */
/* will make the callback.
 */
/* This will improve performance
 */
/* This construct is only in specialnet
 */
defiNet_addRect( &(defrNet),
 $4,
$5.x,
$5.y,
$6.x,
$6.y,
&needSNPCbk) ;
 if (needSNPCbk && defrSNetPartialPathCbk) {
 CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
 &defrNet);
 defiNet_clearRectPolyNPath( &(defrNet) ) ;
 }
 }
 }
 
 | '+' K_SOURCE source_type
 { if (defrSNetCbk) defiNet_setSource( &(defrNet),
 $3) ;
 }
 
 | '+' K_FIXEDBUMP
 { if (defrSNetCbk) defiNet_setFixedbump( &(defrNet) ) ;
 }
 
 | '+' K_FREQUENCY NUMBER
 { if (defrSNetCbk) defiNet_setFrequency( &(defrNet),
 $3) ;
 }
 
 | '+' K_ORIGINAL {dumb_mode = 1; no_num = 1;} T_STRING
 { if (defrSNetCbk) defiNet_setOriginal( &(defrNet),
 $4) ;
 }
 
 | '+' K_PATTERN pattern_type
 { if (defrSNetCbk) defiNet_setPattern( &(defrNet),
 $3) ;
 }
 
 | '+' K_WEIGHT NUMBER
 { if (defrSNetCbk) defiNet_setWeight( &(defrNet),
 ROUND($3)) ;
 }
 
 | '+' K_ESTCAP NUMBER
 {
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5)
 if (defrSNetCbk) defiNet_setCap( &(defrNet),
 $3) ;
 else
 defWarning(7024, "ESTCAP statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 }
 
 | '+' K_USE use_type
 { if (defrSNetCbk) defiNet_setUse( &(defrNet),
 $3) ;
 }
 
 | '+' K_STYLE NUMBER
 { if (defrSNetCbk) defiNet_setStyle( &(defrNet),
 (int)$3) ;
 }
 
 | '+' K_PROPERTY {dumb_mode = 10000000; parsing_property = 1; }
 snet_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 
 | extension_stmt
 { CALLBACK(defrNetExtCbk, defrNetExtCbkType, History_text); }
 
 shield_layer: /* empty */
/* PCR 902306 */

 {
 if (defrSNetCbk) {
 defMsg = (char*)defMalloc(1024);
 sprintf(defMsg, "The SPECIAL NET SHIELD statement, does not have any shield net statement defined.\nThe DEF parser will ignore this statemnet.");
 defWarning(7025, defMsg);
 defFree(defMsg);
 }
 }
 |
 { /* since the parser still supports 5.3 and earlier, */

 /* can't just move SHIELD in net_type */

 if (defVersionNum < 5.4) { /* PCR 445209 */

 if (defrSNetCbk) defiNet_addShield( &(defrNet),
 shieldName) ;
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 shield = TRUE; /* save the path info in the shield paths */

 } else
 if (defrSNetCbk) defiNet_addWire( &(defrNet),
 "SHIELD",
shieldName) ;
 }
 spaths
 {
 /* 7/17/2003 - Fix for pcr 604848, add a callback for each wire
 */
if (defrSNetWireCbk) {
 CALLBACK(defrSNetWireCbk, defrSNetWireCbkType, &defrNet);
 if (defVersionNum < 5.4)
 defiNet_freeShield( &(defrNet) ) ;
 else
 defiNet_freeWire( &(defrNet) ) ;
 }
 if (defVersionNum < 5.4) { /* PCR 445209 */

 shield = FALSE;
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 nondef_is_keyword = FALSE;
 mustjoin_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 } else {
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 }
 needSNPCbk = 0;
 }
 
 snet_width: '+' K_WIDTH { dumb_mode = 1; } T_STRING NUMBER
 {
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5)
 if (defrSNetCbk) defiNet_setWidth( &(defrNet),
 $4,
$5) ;
 else
 defWarning(7026, "WIDTH statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 }
 
 snet_voltage: '+' K_VOLTAGE { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (numIsInt($4)) {
 if (defrSNetCbk) defiNet_setVoltage( &(defrNet),
 atoi($4)) ;
 } else {
 if (defrSNetCbk) {
 if (sNetWarnings++ < defrSNetWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "The value %s for statement VOLTAGE is not an integer.\nThe syntax requires an integer in units of millivolts", $4);
 defError(6537, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 }
 
 snet_spacing: '+' K_SPACING { dumb_mode = 1; } T_STRING NUMBER
 {
 if (defrSNetCbk) defiNet_setSpacing( &(defrNet),
 $4,
$5) ;
 }
 opt_snet_range
 {
 }
 
 snet_prop_list: snet_prop
 | snet_prop_list snet_prop
 ;
 
 snet_prop: T_STRING NUMBER
 {
 if (defrSNetCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrSNetProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "SPECIAL NET");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiNet_addNumProp( &(defrNet),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrSNetCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrSNetProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "SPECIAL NET");
 defiNet_addProp( &(defrNet),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrSNetCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrSNetProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "SPECIAL NET");
 defiNet_addProp( &(defrNet),
 $1,
$2,
propTp) ;
 }
 }
 
 opt_snet_range: /* nothing */

 | K_RANGE NUMBER NUMBER
 {
 if (defrSNetCbk) defiNet_setRange( &(defrNet),
 $2,
$3) ;
 }
 
 opt_range: /* nothing */

 | K_RANGE NUMBER NUMBER
 { defiProp_setRange( &(defrProp),
 $2,
$3) ;
 }
 
 pattern_type: K_BALANCED
 { $$ = (char*)"BALANCED"; }
 | K_STEINER
 { $$ = (char*)"STEINER"; }
 | K_TRUNK
 { $$ = (char*)"TRUNK"; }
 | K_WIREDLOGIC
 { $$ = (char*)"WIREDLOGIC"; }
 
 spaths:
 spath
 {
 if (defrNeedPathData && defrSNetCbk) {
 if (needSNPCbk && defrSNetPartialPathCbk) {
 /* require a callback before proceed because needSNPCbk must be
 */
/* set to 1 from the previous pathIsDone and user has registered
 */
/* a callback routine.
 */
CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
 &defrNet);
 needSNPCbk = 0; /* reset the flag
 */
pathIsDone(shield, 1, netOsnet, &needSNPCbk);
 } else
 pathIsDone(shield, 0, netOsnet, &needSNPCbk);
 }
 }
 | spaths snew_path
 { }
 
 snew_path: K_NEW { dumb_mode = 1; } spath
 { if (defrNeedPathData && defrSNetCbk) {
 if (needSNPCbk && defrSNetPartialPathCbk) {
 /* require a callback before proceed because needSNPCbk must be
 */
/* set to 1 from the previous pathIsDone and user has registered
 */
/* a callback routine.
 */
CALLBACK(defrSNetPartialPathCbk, defrSNetPartialPathCbkType,
 &defrNet);
 needSNPCbk = 0; /* reset the flag
 */
pathIsDone(shield, 1, netOsnet, &needSNPCbk);
 /* reset any poly or rect in special wiring statement
 */
defiNet_clearRectPolyNPath( &(defrNet) ) ;
 } else
 pathIsDone(shield, 0, netOsnet, &needSNPCbk);
 }
 }
 
 spath: T_STRING
 { if (defrNeedPathData && defrSNetCbk)
     defiPath_addLayer( defrPath, $1) ;
 dumb_mode = 0; no_num = 0;
 }
 width opt_spaths path_pt
 /* 
{ dumb_mode = 1; new_is_keyword = TRUE; }
 */

 { dumb_mode = 1000000000; by_is_keyword = TRUE; do_is_keyword = TRUE;
 new_is_keyword = TRUE; step_is_keyword = TRUE;
 orient_is_keyword = TRUE; }
 
 path_item_list
 { dumb_mode = 0; }
 
 width: NUMBER
 { if (defrNeedPathData && defrSNetCbk)
 defiPath_addWidth( defrPath,
 ROUND($1)) ;
 }
 
 start_snets: K_SNETS NUMBER ';'
 {
 if (defrSNetStartCbk)
 CALLBACK(defrSNetStartCbk, defrSNetStartCbkType, ROUND($2));
 netOsnet = 2;
 }
 
 end_snets: K_END K_SNETS
 {
 if (defrSNetEndCbk)
 CALLBACK(defrSNetEndCbk, defrSNetEndCbkType, 0);
 netOsnet = 0;
 }
 
 groups_section: groups_start group_rules groups_end
 ;
 
 groups_start: K_GROUPS NUMBER ';'
 {
 if (defrGroupsStartCbk)
 CALLBACK(defrGroupsStartCbk, defrGroupsStartCbkType, ROUND($2));
 }
 
 group_rules: /* empty */

 | group_rules group_rule
 ;
 
 group_rule: start_group group_members group_options ';'
 {
 if (defrGroupCbk)
 CALLBACK(defrGroupCbk, defrGroupCbkType, &defrGroup);
 }
 
 start_group: '-' { dumb_mode = 1; no_num = 1; } T_STRING
 {
 dumb_mode = 1000000000;
 no_num = 1000000000;
 /* dumb_mode is automatically turned off at the first
 * + in the options or at the ; at the end of the group */

 if (defrGroupCbk) defiGroup_setup( &(defrGroup),
 $3) ;
 if (defrGroupNameCbk)
 CALLBACK(defrGroupNameCbk, defrGroupNameCbkType, $3);
 }
 
 group_members:
 | group_members group_member
 { }
 
 group_member: T_STRING
 {
 /* if (defrGroupCbk) defiGroup_addMember( &(defrGroup),
 $1) ;
 */

 if (defrGroupMemberCbk)
 CALLBACK(defrGroupMemberCbk, defrGroupMemberCbkType, $1);
 }
 
 group_options: /* empty */

 | group_options group_option
 ;
 
 group_option: '+' K_SOFT group_soft_options
 { }
 | '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
 group_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 | '+' K_REGION { dumb_mode = 1; no_num = 1; } group_region
 { }
 | extension_stmt
 {
 if (defrGroupMemberCbk)
 CALLBACK(defrGroupExtCbk, defrGroupExtCbkType, History_text);
 }
 
 group_region: pt pt
 {
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5) {
 if (defrGroupCbk)
 defiGroup_addRegionRect( &(defrGroup),
 $1.x,
$1.y,
$2.x,
$2.y) ;
 }
 else
 defWarning(7027, "GROUP REGION pt pt statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 }
 | T_STRING
 { if (defrGroupCbk)
 defiGroup_setRegionName( &(defrGroup),
 $1) ;
 }
 
 group_prop_list : /* empty */

 | group_prop_list group_prop
 ;
 
 group_prop : T_STRING NUMBER
 {
 if (defrGroupCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrGroupProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "GROUP");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiGroup_addNumProperty( &(defrGroup),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrGroupCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrGroupProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "GROUP");
 defiGroup_addProperty( &(defrGroup),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrGroupCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrGroupProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "GROUP");
 defiGroup_addProperty( &(defrGroup),
 $1,
$2,
propTp) ;
 }
 }
 
 group_soft_options: /* empty */

 | group_soft_options group_soft_option
 { }
 
 group_soft_option: K_MAXX NUMBER
 {
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5)
 if (defrGroupCbk) defiGroup_setMaxX( &(defrGroup),
 ROUND($2)) ;
 else
 defWarning(7028, "GROUP SOFT MAXX statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 }
 | K_MAXY NUMBER
 {
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5)
 if (defrGroupCbk) defiGroup_setMaxY( &(defrGroup),
 ROUND($2)) ;
 else
 defWarning(7029, "GROUP SOFT MAXY statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 }
 | K_MAXHALFPERIMETER NUMBER
 {
 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 if (defVersionNum < 5.5)
 if (defrGroupCbk) defiGroup_setPerim( &(defrGroup),
 ROUND($2)) ;
 else
 defWarning(7030, "GROUP SOFT MAXHALFPERIMETER statement is obsolete in version 5.5 and later.\nThe DEF parser will ignore this statement.");
 }
 
 groups_end: K_END K_GROUPS
 {
 if (defrGroupsEndCbk)
 CALLBACK(defrGroupsEndCbk, defrGroupsEndCbkType, 0);
 }
 
 /* 8/31/2001 - This is obsolete in 5.4 */

 assertions_section: assertions_start constraint_rules assertions_end
 ;
 
 /* 8/31/2001 - This is obsolete in 5.4 */

 constraint_section: constraints_start constraint_rules constraints_end
 ;
 
 assertions_start: K_ASSERTIONS NUMBER ';'
 {
 if ((defVersionNum < 5.4) && (defrAssertionsStartCbk)) {
 CALLBACK(defrAssertionsStartCbk, defrAssertionsStartCbkType,
 	 ROUND($2));
 } else {
 if (defrAssertionCbk)
 if (assertionWarnings++ < defrAssertionWarnings)
 defWarning(7031, "ASSERTIONS statement is obsolete in version 5.4 and later.\nThe DEF parser will ignore this statement.");
 }
 if (defrAssertionCbk)
 defiAssertion_setAssertionMode( &(defrAssertion) ) ;
 }
 
 constraints_start: K_CONSTRAINTS NUMBER ';'
 {
 if ((defVersionNum < 5.4) && (defrConstraintsStartCbk)) {
 CALLBACK(defrConstraintsStartCbk, defrConstraintsStartCbkType,
 ROUND($2));
 } else {
 if (defrConstraintCbk)
 if (constraintWarnings++ < defrConstraintWarnings)
 defWarning(7032, "CONSTRAINTS statement is obsolete in version 5.4 and later.\nThe DEF parser will ignore this statement.");
 }
 if (defrConstraintCbk)
 defiAssertion_setConstraintMode( &(defrAssertion) ) ;
 }
 
 constraint_rules: /* empty */

 | constraint_rules constraint_rule
 ;
 
 constraint_rule: operand_rule
 | wiredlogic_rule
 {
 if ((defVersionNum < 5.4) && (defrConstraintCbk || defrAssertionCbk)) {
 if (defiAssertion_isConstraint( &(defrAssertion) )) CALLBACK(defrConstraintCbk, defrConstraintCbkType, &defrAssertion) ;
 if (defiAssertion_isAssertion( &(defrAssertion) )) CALLBACK(defrAssertionCbk, defrAssertionCbkType, &defrAssertion) ;
 }
 }
 
 operand_rule: '-' operand delay_specs ';'
 {
 if ((defVersionNum < 5.4) && (defrConstraintCbk || defrAssertionCbk)) {
 if (defiAssertion_isConstraint( &(defrAssertion) )) CALLBACK(defrConstraintCbk, defrConstraintCbkType, &defrAssertion) ;
 if (defiAssertion_isAssertion( &(defrAssertion) )) CALLBACK(defrAssertionCbk, defrAssertionCbkType, &defrAssertion) ;
 }
 
 /* reset all the flags and everything
 */
defiAssertion_clear( &(defrAssertion) ) ;
 }
 
 operand: K_NET { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_addNet( &(defrAssertion),
 $3) ;
 }
 | K_PATH {dumb_mode = 4; no_num = 4;} T_STRING T_STRING T_STRING T_STRING
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_addPath( &(defrAssertion),
 $3,
$4,
$5,
$6) ;
 }
 | K_SUM '(' operand_list ')'
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setSum( &(defrAssertion) ) ;
 }
 | K_DIFF '(' operand_list ')'
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setDiff( &(defrAssertion) ) ;
 }
 
 operand_list: operand
 | operand_list operand
 { }
 | operand_list ',' operand
 
 wiredlogic_rule: '-' K_WIREDLOGIC { dumb_mode = 1; no_num = 1; } T_STRING
 opt_plus K_MAXDIST NUMBER ';'
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setWiredlogic( &(defrAssertion),
 $4,
$7) ;
 }
 
 opt_plus:
 /* empty */

 { $$ = (char*)""; }
 | '+'
 { $$ = (char*)"+"; }
 
 delay_specs: /* empty */

 | delay_specs delay_spec
 ;
 
 delay_spec: '+' K_RISEMIN NUMBER
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setRiseMin( &(defrAssertion),
 $3) ;
 }
 | '+' K_RISEMAX NUMBER
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setRiseMax( &(defrAssertion),
 $3) ;
 }
 | '+' K_FALLMIN NUMBER
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setFallMin( &(defrAssertion),
 $3) ;
 }
 | '+' K_FALLMAX NUMBER
 {
 if (defrConstraintCbk || defrAssertionCbk)
 defiAssertion_setFallMax( &(defrAssertion),
 $3) ;
 }
 
 constraints_end: K_END K_CONSTRAINTS
 { if ((defVersionNum < 5.4) && defrConstraintsEndCbk) {
 CALLBACK(defrConstraintsEndCbk, defrConstraintsEndCbkType, 0);
 } else {
 if (defrConstraintsEndCbk) {
 if (constraintWarnings++ < defrConstraintWarnings)
 defWarning(7032, "CONSTRAINTS statement is obsolete in version 5.4 and later.\nThe DEF parser will ignore this statement.");
 }
 }
 }
 
 assertions_end: K_END K_ASSERTIONS
 { if ((defVersionNum < 5.4) && defrAssertionsEndCbk) {
 CALLBACK(defrAssertionsEndCbk, defrAssertionsEndCbkType, 0);
 } else {
 if (defrAssertionsEndCbk) {
 if (assertionWarnings++ < defrAssertionWarnings)
 defWarning(7031, "ASSERTIONS statement is obsolete in version 5.4 and later.\nThe DEF parser will ignore this statement.");
 }
 }
 }
 
 scanchains_section: scanchain_start scanchain_rules scanchain_end
 ;
 
 scanchain_start: K_SCANCHAINS NUMBER ';'
 { if (defrScanchainsStartCbk)
 CALLBACK(defrScanchainsStartCbk, defrScanchainsStartCbkType,
 ROUND($2));
 }
 
 scanchain_rules: /* empty */

 | scanchain_rules scan_rule
 {}
 
 scan_rule: start_scan scan_members ';'
 {
 if (defrScanchainCbk)
 CALLBACK(defrScanchainCbk, defrScanchainCbkType, &defrScanchain);
 }
 
 start_scan: '-' {dumb_mode = 1; no_num = 1;} T_STRING
 {
 if (defrScanchainCbk)
 defiScanchain_setName( &(defrScanchain),
 $3) ;
 bit_is_keyword = TRUE;
 }
 
 scan_members:
 | scan_members scan_member
 ;
 
 opt_pin :
 /* empty */

 { $$ = (char*)""; }
 | T_STRING
 { $$ = $1; }
 
 scan_member: '+' K_START {dumb_mode = 2; no_num = 2;} T_STRING opt_pin
 { if (defrScanchainCbk)
 defiScanchain_setStart( &(defrScanchain),
 $4,
$5) ;
 }
 | '+' K_FLOATING { dumb_mode = 1; no_num = 1; } floating_inst_list
 { dumb_mode = 0; no_num = 0; }
 | '+' K_ORDERED
 {
 dumb_mode = 1;
 no_num = 1;
 if (defrScanchainCbk)
 defiScanchain_addOrderedList( &(defrScanchain) ) ;
 }
 ordered_inst_list
 { dumb_mode = 0; no_num = 0; }
 | '+' K_STOP {dumb_mode = 2; no_num = 2; } T_STRING opt_pin
 { if (defrScanchainCbk)
 defiScanchain_setStop( &(defrScanchain),
 $4,
$5) ;
 }
 | '+' K_COMMONSCANPINS { dumb_mode = 10; no_num = 10; } opt_common_pins
 { dumb_mode = 0; no_num = 0; }
 | '+' K_PARTITION { dumb_mode = 1; no_num = 1; } T_STRING /* 5.5 */

 partition_maxbits
 {
 if (defVersionNum < 5.5) {
 if (defrScanchainCbk) {
 if (scanchainWarnings++ < defrScanchainWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "PARTITION statement is a version 5.5 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6538, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 }
 if (defrScanchainCbk)
 defiScanchain_setPartition( &(defrScanchain),
 $4,
$5) ;
 }
 | extension_stmt
 {
 if (defrScanChainExtCbk)
 CALLBACK(defrScanChainExtCbk, defrScanChainExtCbkType, History_text);
 }
 
 opt_common_pins: /* empty */

 { }
 | '(' T_STRING T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_setCommonIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_setCommonOut( &(defrScanchain),
 $3) ;
 	}
 }
 | '(' T_STRING T_STRING ')' '(' T_STRING T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_setCommonIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_setCommonOut( &(defrScanchain),
 $3) ;
 	 if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
 	 defiScanchain_setCommonIn( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
 	 defiScanchain_setCommonOut( &(defrScanchain),
 $7) ;
 	}
 }
 
 floating_inst_list: /* empty */

 | floating_inst_list one_floating_inst
 ;
 
 one_floating_inst: T_STRING
 {
 	dumb_mode = 1000;
 	no_num = 1000;
 	if (defrScanchainCbk)
 	 defiScanchain_addFloatingInst( &(defrScanchain),
 $1) ;
 }
 floating_pins
 { dumb_mode = 1; no_num = 1; }
 
 floating_pins: /* empty */

 { }
 | '(' T_STRING T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_addFloatingIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_addFloatingOut( &(defrScanchain),
 $3) ;
 else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
 bitsNum = atoi($3);
 defiScanchain_setFloatingBits( &(defrScanchain),
 bitsNum) ;
 }
 	}
 }
 | '(' T_STRING T_STRING ')' '(' T_STRING T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_addFloatingIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_addFloatingOut( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
 bitsNum = atoi($3);
 	 defiScanchain_setFloatingBits( &(defrScanchain),
 bitsNum) ;
 }
 	 if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
 	 defiScanchain_addFloatingIn( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
 	 defiScanchain_addFloatingOut( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
 bitsNum = atoi($7);
 	 defiScanchain_setFloatingBits( &(defrScanchain),
 bitsNum) ;
 }
 	}
 }
 | '(' T_STRING T_STRING ')' '(' T_STRING T_STRING ')' '(' T_STRING
 T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_addFloatingIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_addFloatingOut( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
 bitsNum = atoi($3);
 	 defiScanchain_setFloatingBits( &(defrScanchain),
 bitsNum) ;
 }
 	 if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
 	 defiScanchain_addFloatingIn( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
 	 defiScanchain_addFloatingOut( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
 bitsNum = atoi($7);
 	 defiScanchain_setFloatingBits( &(defrScanchain),
 bitsNum) ;
 }
 	 if (strcmp($10, "IN") == 0 || strcmp($10, "in") == 0)
 	 defiScanchain_addFloatingIn( &(defrScanchain),
 $11) ;
 	 else if (strcmp($10, "OUT") == 0 || strcmp($10, "out") == 0)
 	 defiScanchain_addFloatingOut( &(defrScanchain),
 $11) ;
 	 else if (strcmp($10, "BITS") == 0 || strcmp($10, "bits") == 0) {
 bitsNum = atoi($11);
 	 defiScanchain_setFloatingBits( &(defrScanchain),
 bitsNum) ;
 }
 	}
 }
 
 ordered_inst_list: /* empty */

 | ordered_inst_list one_ordered_inst
 ;
 
 one_ordered_inst: T_STRING
 { dumb_mode = 1000; no_num = 1000;
 	if (defrScanchainCbk)
 	 defiScanchain_addOrderedInst( &(defrScanchain),
 $1) ;
 }
 ordered_pins
 { dumb_mode = 1; no_num = 1; }
 
 ordered_pins: /* empty */

 { }
 | '(' T_STRING T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_addOrderedIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_addOrderedOut( &(defrScanchain),
 $3) ;
 else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
 bitsNum = atoi($3);
 defiScanchain_setOrderedBits( &(defrScanchain),
 bitsNum) ;
 }
 	}
 }
 | '(' T_STRING T_STRING ')' '(' T_STRING T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_addOrderedIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_addOrderedOut( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
 bitsNum = atoi($3);
 defiScanchain_setOrderedBits( &(defrScanchain),
 bitsNum) ;
 }
 	 if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
 	 defiScanchain_addOrderedIn( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
 	 defiScanchain_addOrderedOut( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
 bitsNum = atoi($7);
 defiScanchain_setOrderedBits( &(defrScanchain),
 bitsNum) ;
 }
 	}
 }
 | '(' T_STRING T_STRING ')' '(' T_STRING T_STRING ')' '(' T_STRING
 T_STRING ')'
 {
 	if (defrScanchainCbk) {
 	 if (strcmp($2, "IN") == 0 || strcmp($2, "in") == 0)
 	 defiScanchain_addOrderedIn( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "OUT") == 0 || strcmp($2, "out") == 0)
 	 defiScanchain_addOrderedOut( &(defrScanchain),
 $3) ;
 	 else if (strcmp($2, "BITS") == 0 || strcmp($2, "bits") == 0) {
 bitsNum = atoi($3);
 	 defiScanchain_setOrderedBits( &(defrScanchain),
 bitsNum) ;
 }
 	 if (strcmp($6, "IN") == 0 || strcmp($6, "in") == 0)
 	 defiScanchain_addOrderedIn( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "OUT") == 0 || strcmp($6, "out") == 0)
 	 defiScanchain_addOrderedOut( &(defrScanchain),
 $7) ;
 	 else if (strcmp($6, "BITS") == 0 || strcmp($6, "bits") == 0) {
 bitsNum = atoi($7);
 	 defiScanchain_setOrderedBits( &(defrScanchain),
 bitsNum) ;
 }
 	 if (strcmp($10, "IN") == 0 || strcmp($10, "in") == 0)
 	 defiScanchain_addOrderedIn( &(defrScanchain),
 $11) ;
 	 else if (strcmp($10, "OUT") == 0 || strcmp($10, "out") == 0)
 	 defiScanchain_addOrderedOut( &(defrScanchain),
 $11) ;
 	 else if (strcmp($10, "BITS") == 0 || strcmp($10, "bits") == 0) {
 bitsNum = atoi($11);
 	 defiScanchain_setOrderedBits( &(defrScanchain),
 bitsNum) ;
 }
 	}
 }
 
 partition_maxbits: /* empty */

 { $$ = -1; }
 | K_MAXBITS NUMBER
 { $$ = ROUND($2); }
 
 scanchain_end: K_END K_SCANCHAINS
 {
 if (defrScanchainsEndCbk)
 CALLBACK(defrScanchainsEndCbk, defrScanchainsEndCbkType, 0);
 bit_is_keyword = FALSE;
 dumb_mode = 0; no_num = 0;
 }
 
 /* 8/31/2001 - This is obsolete in 5.4 */

 iotiming_section: iotiming_start iotiming_rules iotiming_end
 ;
 
 iotiming_start: K_IOTIMINGS NUMBER ';'
 {
 if (defVersionNum < 5.4 && defrIOTimingsStartCbk) {
 CALLBACK(defrIOTimingsStartCbk, defrIOTimingsStartCbkType, ROUND($2));
 } else {
 if (defrIOTimingsStartCbk)
 if (iOTimingWarnings++ < defrIOTimingWarnings)
 defWarning(7035, "IOTIMINGS statement is obsolete in version 5.4 and later.\nThe DEF parser will ignore this statement.");
 }
 }
 
 iotiming_rules: /* empty */

 | iotiming_rules iotiming_rule
 { }
 
 iotiming_rule: start_iotiming iotiming_members ';'
 {
 if (defVersionNum < 5.4 && defrIOTimingCbk)
 CALLBACK(defrIOTimingCbk, defrIOTimingCbkType, &defrIOTiming);
 }
 
 start_iotiming: '-' '(' {dumb_mode = 2; no_num = 2; } T_STRING T_STRING ')'
 {
 if (defrIOTimingCbk)
 defiIOTiming_setName( &(defrIOTiming),
 $4,
$5) ;
 }
 
 iotiming_members:
 | iotiming_members iotiming_member
 ;
 
 iotiming_member:
 '+' risefall K_VARIABLE NUMBER NUMBER
 {
 if (defrIOTimingCbk)
 defiIOTiming_setVariable( &(defrIOTiming),
 $2,
$4,
$5) ;
 }
 | '+' risefall K_SLEWRATE NUMBER NUMBER
 {
 if (defrIOTimingCbk)
 defiIOTiming_setSlewRate( &(defrIOTiming),
 $2,
$4,
$5) ;
 }
 | '+' K_CAPACITANCE NUMBER
 {
 if (defrIOTimingCbk)
 defiIOTiming_setCapacitance( &(defrIOTiming),
 $3) ;
 }
 | '+' K_DRIVECELL {dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrIOTimingCbk)
 defiIOTiming_setDriveCell( &(defrIOTiming),
 $4) ;
 } iotiming_drivecell_opt
 /* |'+' K_FROMPIN {dumb_mode = 1; no_num = 1; } T_STRING*/

 /* 
| K_FROMPIN {dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrIOTimingCbk)
 defiIOTiming_setFrom( &(defrIOTiming),
 $3) ;
 }
 K_TOPIN {dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrIOTimingCbk)
 defiIOTiming_setTo( &(defrIOTiming),
 $7) ;
 }
 */

 /* |'+' K_PARALLEL NUMBER*/

 /* 
| K_PARALLEL NUMBER
 {
 if (defrIOTimingCbk)
 defiIOTiming_setParallel( &(defrIOTiming),
 $2) ;
 }
 */

 | extension_stmt
 {
 if (defVersionNum < 5.4 && defrIoTimingsExtCbk)
 CALLBACK(defrIoTimingsExtCbk, defrIoTimingsExtCbkType, History_text);
 }
 
 iotiming_drivecell_opt: iotiming_frompin
 K_TOPIN {dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrIOTimingCbk)
 defiIOTiming_setTo( &(defrIOTiming),
 $4) ;
 }
 iotiming_parallel
 
 iotiming_frompin: /* empty */

 | K_FROMPIN {dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrIOTimingCbk)
 defiIOTiming_setFrom( &(defrIOTiming),
 $3) ;
 }
 
 iotiming_parallel: /* empty */

 | K_PARALLEL NUMBER
 {
 if (defrIOTimingCbk)
 defiIOTiming_setParallel( &(defrIOTiming),
 $2) ;
 }
 
 risefall: K_RISE { $$ = (char*)"RISE"; } | K_FALL { $$ = (char*)"FALL"; }
 
 iotiming_end: K_END K_IOTIMINGS
 {
 if (defVersionNum < 5.4 && defrIOTimingsEndCbk)
 CALLBACK(defrIOTimingsEndCbk, defrIOTimingsEndCbkType, 0);
 }
 
 floorplan_contraints_section: fp_start fp_stmts K_END K_FPC
 {
 if (defrFPCEndCbk)
 CALLBACK(defrFPCEndCbk, defrFPCEndCbkType, 0);
 }
 
 fp_start: K_FPC NUMBER ';'
 {
 if (defrFPCStartCbk)
 CALLBACK(defrFPCStartCbk, defrFPCStartCbkType, ROUND($2));
 }
 
 fp_stmts: /* empty */

 | fp_stmts fp_stmt
 {}
 
 fp_stmt: '-' { dumb_mode = 1; no_num = 1; } T_STRING h_or_v
 { if (defrFPCCbk) defiFPC_setName( &(defrFPC),
 $3,
$4) ;
 }
 constraint_type constrain_what_list ';'
 { if (defrFPCCbk) CALLBACK(defrFPCCbk, defrFPCCbkType, &defrFPC); }
 
 h_or_v: K_HORIZONTAL
 { $$ = (char*)"HORIZONTAL"; }
 | K_VERTICAL
 { $$ = (char*)"VERTICAL"; }
 
 constraint_type: K_ALIGN
 { if (defrFPCCbk) defiFPC_setAlign( &(defrFPC) ) ;
 }
 | K_MAX NUMBER
 { if (defrFPCCbk) defiFPC_setMax( &(defrFPC),
 $2) ;
 }
 | K_MIN NUMBER
 { if (defrFPCCbk) defiFPC_setMin( &(defrFPC),
 $2) ;
 }
 | K_EQUAL NUMBER
 { if (defrFPCCbk) defiFPC_setEqual( &(defrFPC),
 $2) ;
 }
 
 constrain_what_list: /* empty */

 | constrain_what_list constrain_what
 ;
 
 constrain_what: '+' K_BOTTOMLEFT
 { if (defrFPCCbk) defiFPC_setDoingBottomLeft( &(defrFPC) ) ;
 }
 row_or_comp_list
 | '+' K_TOPRIGHT
 { if (defrFPCCbk) defiFPC_setDoingTopRight( &(defrFPC) ) ;
 }
 row_or_comp_list
 ;
 
 row_or_comp_list: /* empty */

 | row_or_comp_list row_or_comp
 
 row_or_comp: '(' K_ROWS {dumb_mode = 1; no_num = 1; } T_STRING ')'
 { if (defrFPCCbk) defiFPC_addRow( &(defrFPC),
 $4) ;
 }
 | '(' K_COMPS {dumb_mode = 1; no_num = 1; } T_STRING ')'
 { if (defrFPCCbk) defiFPC_addComps( &(defrFPC),
 $4) ;
 }
 
 timingdisables_section: timingdisables_start timingdisables_rules
 timingdisables_end
 ;
 
 timingdisables_start: K_TIMINGDISABLES NUMBER ';'
 {
 if (defrTimingDisablesStartCbk)
 CALLBACK(defrTimingDisablesStartCbk, defrTimingDisablesStartCbkType,
 ROUND($2));
 }
 
 timingdisables_rules: /* empty */

 | timingdisables_rules timingdisables_rule
 {}
 
 timingdisables_rule: '-' K_FROMPIN { dumb_mode = 2; no_num = 2; } T_STRING
 T_STRING K_TOPIN { dumb_mode = 2; no_num = 2; } T_STRING T_STRING ';'
 {
 if (defrTimingDisableCbk) {
 defiTimingDisable_setFromTo( &(defrTimingDisable),
 $4,
$5,
$8,
$9) ;
 CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
 &defrTimingDisable);
 }
 }
 | '-' K_THRUPIN {dumb_mode = 2; no_num = 2; } T_STRING T_STRING ';'
 {
 if (defrTimingDisableCbk) {
 defiTimingDisable_setThru( &(defrTimingDisable),
 $4,
$5) ;
 CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
 &defrTimingDisable);
 }
 }
 | '-' K_MACRO {dumb_mode = 1; no_num = 1;} T_STRING td_macro_option ';'
 {
 if (defrTimingDisableCbk) {
 defiTimingDisable_setMacro( &(defrTimingDisable),
 $4) ;
 CALLBACK(defrTimingDisableCbk, defrTimingDisableCbkType,
 &defrTimingDisable);
 }
 }
 | '-' K_REENTRANTPATHS ';'
 { if (defrTimingDisableCbk)
 defiTimingDisable_setReentrantPathsFlag( &(defrTimingDisable) ) ;
 }
 
 
 td_macro_option: K_FROMPIN {dumb_mode = 1; no_num = 1;} T_STRING K_TOPIN
 {dumb_mode=1; no_num = 1;} T_STRING
 {
 if (defrTimingDisableCbk)
 defiTimingDisable_setMacroFromTo( &(defrTimingDisable),
 $3,
$6) ;
 }
 | K_THRUPIN {dumb_mode=1; no_num = 1;} T_STRING
 {
 if (defrTimingDisableCbk)
 defiTimingDisable_setMacroThru( &(defrTimingDisable),
 $3) ;
 }
 
 timingdisables_end: K_END K_TIMINGDISABLES
 {
 if (defrTimingDisablesEndCbk)
 CALLBACK(defrTimingDisablesEndCbk, defrTimingDisablesEndCbkType, 0);
 }
 
 
 partitions_section: partitions_start partition_rules partitions_end
 ;
 
 partitions_start: K_PARTITIONS NUMBER ';'
 {
 if (defrPartitionsStartCbk)
 CALLBACK(defrPartitionsStartCbk, defrPartitionsStartCbkType,
 ROUND($2));
 }
 
 partition_rules: /* empty */

 | partition_rules partition_rule
 { }
 
 partition_rule: start_partition partition_members ';'
 {
 if (defrPartitionCbk)
 CALLBACK(defrPartitionCbk, defrPartitionCbkType, &defrPartition);
 }
 
 start_partition: '-' { dumb_mode = 1; no_num = 1; } T_STRING turnoff
 {
 if (defrPartitionCbk)
 defiPartition_setName( &(defrPartition),
 $3) ;
 }
 
 turnoff: /* empty */

 | K_TURNOFF turnoff_setup turnoff_hold
 {
 if (defrPartitionCbk)
 defiPartition_addTurnOff( &(defrPartition),
 $2,
$3) ;
 }
 
 turnoff_setup: /* empty */

 { $$ = (char*)" "; }
 | K_SETUPRISE
 { $$ = (char*)"R"; }
 | K_SETUPFALL
 { $$ = (char*)"F"; }
 
 turnoff_hold: /* empty */

 { $$ = (char*)" "; }
 | K_HOLDRISE
 { $$ = (char*)"R"; }
 | K_HOLDFALL
 { $$ = (char*)"F"; }
 
 partition_members: /* empty */

 | partition_members partition_member
 ;
 
 partition_member: '+' K_FROMCLOCKPIN {dumb_mode=2; no_num = 2;}
 T_STRING T_STRING risefall minmaxpins
 {
 if (defrPartitionCbk)
 defiPartition_setFromClockPin( &(defrPartition),
 $4,
$5) ;
 }
 | '+' K_FROMCOMPPIN {dumb_mode=2; no_num = 2; }
 T_STRING T_STRING risefallminmax2_list
 {
 if (defrPartitionCbk)
 defiPartition_setFromCompPin( &(defrPartition),
 $4,
$5) ;
 }
 | '+' K_FROMIOPIN {dumb_mode=1; no_num = 1; } T_STRING
 risefallminmax1_list
 {
 if (defrPartitionCbk)
 defiPartition_setFromIOPin( &(defrPartition),
 $4) ;
 }
 | '+' K_TOCLOCKPIN {dumb_mode=2; no_num = 2; }
 T_STRING T_STRING risefall minmaxpins
 {
 if (defrPartitionCbk)
 defiPartition_setToClockPin( &(defrPartition),
 $4,
$5) ;
 }
 | '+' K_TOCOMPPIN {dumb_mode=2; no_num = 2; }
 T_STRING T_STRING risefallminmax2_list
 {
 if (defrPartitionCbk)
 defiPartition_setToCompPin( &(defrPartition),
 $4,
$5) ;
 }
 | '+' K_TOIOPIN {dumb_mode=1; no_num = 2; } T_STRING risefallminmax1_list
 {
 if (defrPartitionCbk)
 defiPartition_setToIOPin( &(defrPartition),
 $4) ;
 }
 | extension_stmt
 {
 if (defrPartitionsExtCbk)
 CALLBACK(defrPartitionsExtCbk, defrPartitionsExtCbkType,
 History_text);
 }
 
 minmaxpins: min_or_max_list K_PINS
 { dumb_mode = 1000000000; no_num = 10000000; } pin_list
 { dumb_mode = 0; no_num = 0; }
 
 min_or_max_list: /* empty */

 | min_or_max_list min_or_max_member
 { }
 
 min_or_max_member: K_MIN NUMBER NUMBER
 {
 if (defrPartitionCbk)
 defiPartition_setMin( &(defrPartition),
 $2,
$3) ;
 }
 | K_MAX NUMBER NUMBER
 {
 if (defrPartitionCbk)
 defiPartition_setMax( &(defrPartition),
 $2,
$3) ;
 }
 
 pin_list: /* empty */

 | pin_list T_STRING
 { if (defrPartitionCbk) defiPartition_addPin( &(defrPartition),
 $2) ;
 }
 
 risefallminmax1_list: /* empty */

 | risefallminmax1_list risefallminmax1
 
 risefallminmax1: K_RISEMIN NUMBER
 { if (defrPartitionCbk) defiPartition_addRiseMin( &(defrPartition),
 $2) ;
 }
 | K_FALLMIN NUMBER
 { if (defrPartitionCbk) defiPartition_addFallMin( &(defrPartition),
 $2) ;
 }
 | K_RISEMAX NUMBER
 { if (defrPartitionCbk) defiPartition_addRiseMax( &(defrPartition),
 $2) ;
 }
 | K_FALLMAX NUMBER
 { if (defrPartitionCbk) defiPartition_addFallMax( &(defrPartition),
 $2) ;
 }
 
 risefallminmax2_list:
 risefallminmax2
 | risefallminmax2_list risefallminmax2
 ;
 
 risefallminmax2: K_RISEMIN NUMBER NUMBER
 { if (defrPartitionCbk)
 defiPartition_addRiseMinRange( &(defrPartition),
 $2,
$3) ;
 }
 | K_FALLMIN NUMBER NUMBER
 { if (defrPartitionCbk)
 defiPartition_addFallMinRange( &(defrPartition),
 $2,
$3) ;
 }
 | K_RISEMAX NUMBER NUMBER
 { if (defrPartitionCbk)
 defiPartition_addRiseMaxRange( &(defrPartition),
 $2,
$3) ;
 }
 | K_FALLMAX NUMBER NUMBER
 { if (defrPartitionCbk)
 defiPartition_addFallMaxRange( &(defrPartition),
 $2,
$3) ;
 }
 
 partitions_end: K_END K_PARTITIONS
 { if (defrPartitionsEndCbk)
 CALLBACK(defrPartitionsEndCbk, defrPartitionsEndCbkType, 0); }
 
 comp_names: /* empty */

 | comp_names comp_name
 { }
 
 comp_name: '(' {dumb_mode=2; no_num = 2; } T_STRING
 T_STRING subnet_opt_syn ')'
 {
 /* note that the first T_STRING could be the keyword VPIN */

 if (defrNetCbk)
 defiSubnet_addPin( defrSubnet,
 $3,
$4,
$5) ;
 }
 
 subnet_opt_syn: /* empty */

 { $$ = 0; }
 | '+' K_SYNTHESIZED
 { $$ = 1; }
 
 subnet_options: /* empty */

 | subnet_options subnet_option
 
 subnet_option: subnet_type
 {
 if (defrNetCbk) defiSubnet_addWire( defrSubnet,
 $1) ;
 }
 paths
 {
 by_is_keyword = FALSE;
 do_is_keyword = FALSE;
 new_is_keyword = FALSE;
 step_is_keyword = FALSE;
 orient_is_keyword = FALSE;
 needNPCbk = 0;
 }
 | K_NONDEFAULTRULE { dumb_mode = 1; no_num = 1; } T_STRING
 { if (defrNetCbk) defiSubnet_setNonDefault( defrSubnet,
 $3) ;
 }
 
 subnet_type: K_FIXED
 { $$ = (char*)"FIXED"; dumb_mode = 1; }
 | K_COVER
 { $$ = (char*)"COVER"; dumb_mode = 1; }
 | K_ROUTED
 { $$ = (char*)"ROUTED"; dumb_mode = 1; }
 | K_NOSHIELD
 { $$ = (char*)"NOSHIELD"; dumb_mode = 1; }
 
 pin_props_section: begin_pin_props pin_prop_list end_pin_props ;
 
 begin_pin_props: K_PINPROPERTIES NUMBER opt_semi
 { if (defrPinPropStartCbk)
 CALLBACK(defrPinPropStartCbk, defrPinPropStartCbkType, ROUND($2)); }
 
 opt_semi:
 /* empty */

 { }
 | ';'
 { }
 
 end_pin_props: K_END K_PINPROPERTIES
 { if (defrPinPropEndCbk)
 CALLBACK(defrPinPropEndCbk, defrPinPropEndCbkType, 0); }
 
 pin_prop_list: /* empty */

 | pin_prop_list pin_prop_terminal
 ;
 
 pin_prop_terminal: '-' { dumb_mode = 2; no_num = 2; } T_STRING T_STRING
 { if (defrPinPropCbk) defiPinProp_setName( &(defrPinProp),
 $3,
$4) ;
 }
 pin_prop_options ';'
 { if (defrPinPropCbk) {
 CALLBACK(defrPinPropCbk, defrPinPropCbkType, &defrPinProp);
 /* reset the property number
 */
defiPinProp_clear( &(defrPinProp) ) ;
 }
 }
 
 pin_prop_options : /* empty */

 | pin_prop_options pin_prop ;
 
 pin_prop: '+' K_PROPERTY { dumb_mode = 10000000; parsing_property = 1; }
 pin_prop_name_value_list
 { dumb_mode = 0; parsing_property = 0; }
 
 pin_prop_name_value_list : /* empty */

 | pin_prop_name_value_list pin_prop_name_value
 ;
 
 pin_prop_name_value : T_STRING NUMBER
 {
 if (defrPinPropCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrCompPinProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "PINPROPERTIES");
 /* For backword compatibility, also set the string value */

 /* We will use a temporary string to store the number.
 * The string space is borrowed from the ring buffer
 * in the lexer. */

 sprintf(str, "%g", $2);
 defiPinProp_addNumProperty( &(defrPinProp),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrPinPropCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrCompPinProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "PINPROPERTIES");
 defiPinProp_addProperty( &(defrPinProp),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrPinPropCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrCompPinProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "PINPROPERTIES");
 defiPinProp_addProperty( &(defrPinProp),
 $1,
$2,
propTp) ;
 }
 }
 
 blockage_section: blockage_start blockage_defs blockage_end ;
 
 blockage_start: K_BLOCKAGES NUMBER ';'
 { if (defrBlockageStartCbk)
 CALLBACK(defrBlockageStartCbk, defrBlockageStartCbkType, ROUND($2)); }
 
 blockage_end: K_END K_BLOCKAGES
 { if (defrBlockageEndCbk)
 CALLBACK(defrBlockageEndCbk, defrBlockageEndCbkType, 0); }
 
 blockage_defs: /* empty */

 | blockage_defs blockage_def
 ;
 
 blockage_def: blockage_rule rectPoly_blockage rectPoly_blockage_rules
 ';'
 {
 if (defrBlockageCbk) {
 CALLBACK(defrBlockageCbk, defrBlockageCbkType, &defrBlockage);
 defiBlockage_clear( &(defrBlockage) ) ;
 }
 }
 
 blockage_rule: '-' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrBlockageCbk) {
 if (defiBlockage_hasPlacement( &(defrBlockage) )!=0) { if (blockageWarnings++ < defrBlockageWarnings) { defError(6539, "The BLOCKAGE statement has both the LAYER & the PLACEMENT statements defined.\nThis is illegal.") ;
 CHKERR();
 }
 }
 defiBlockage_setLayer( &(defrBlockage),
 $4) ;
 defiBlockage_clearPoly( &(defrBlockage) ) ;
 /* free poly, if any
 */
}
 hasBlkLayerComp = 0;
 hasBlkLayerSpac = 0;
 }
 layer_blockage_rules
 /* 10/29/2001 - enhancement */

 | '-' K_PLACEMENT
 {
 if (defrBlockageCbk) {
 if (defiBlockage_hasLayer( &(defrBlockage) )!=0) { if (blockageWarnings++ < defrBlockageWarnings) { defError(6539, "The BLOCKAGE statement has both the LAYER & the PLACEMENT statements defined.") ;
 CHKERR();
 }
 }
 defiBlockage_setPlacement( &(defrBlockage) ) ;
 defiBlockage_clearPoly( &(defrBlockage) ) ;
 }
 hasBlkPlaceComp = 0;
 }
 placement_comp_rule
 
 layer_blockage_rules: /* empty */

 | layer_blockage_rules layer_blockage_rule
 ;
 
 layer_blockage_rule: '+' K_SPACING NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "SPACING statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6540, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else if (hasBlkLayerSpac) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6541, "A SPACING statement is defined in the LAYER statement,\nbut there is already either a SPACING statement or DESIGNRULEWIDTH statement has defined in the layer.\nThis is illegal");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setSpacing( &(defrBlockage),
 (int)$3) ;
 hasBlkLayerSpac = 1;
 }
 }
 | '+' K_DESIGNRULEWIDTH NUMBER
 {
 if (defVersionNum < 5.6) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6541, "A SPACING statement is defined in the LAYER statement,\nbut there is already either a SPACING statement or DESIGNRULEWIDTH statement has defined in the layer.\nThis is illegal");
 CHKERR();
 }
 }
 } else if (hasBlkLayerSpac) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6541, "A SPACING statement is defined in the LAYER statement,\nbut there is already either a SPACING statement or DESIGNRULEWIDTH statement has defined in the layer.\nThis is illegal");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setDesignRuleWidth( &(defrBlockage),
 (int)$3) ;
 hasBlkLayerSpac = 1;
 }
 }
 | comp_blockage_rule
 
 comp_blockage_rule:
 /* 06/20/2001 - pcr 383204 = pcr 378102 */

 '+' K_COMPONENT { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (hasBlkLayerComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setComponent( &(defrBlockage),
 $4) ;
 hasBlkLayerComp = 1;
 }
 }
 /* 8/30/2001 - pcr 394394 */

 | '+' K_SLOTS
 {
 if (hasBlkLayerComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setSlots( &(defrBlockage) ) ;
 hasBlkLayerComp = 1;
 }
 }
 | '+' K_FILLS
 {
 if (hasBlkLayerComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setFills( &(defrBlockage) ) ;
 hasBlkLayerComp = 1;
 }
 }
 | '+' K_PUSHDOWN
 {
 if (hasBlkLayerComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setPushdown( &(defrBlockage) ) ;
 hasBlkLayerComp = 1;
 }
 }
 | '+' K_EXCEPTPGNET /* 5.7 */

 {
 if (defVersionNum < 5.7) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "EXCEPTPGNET is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6549, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (hasBlkLayerComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6542, "The BLOCKAGES COMPONENT statement already has either COMPONENT, SLOTS, FILLS, PUSHDOWN or EXCEPTPGNET defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setExceptpgnet( &(defrBlockage) ) ;
 hasBlkLayerComp = 1;
 }
 }
 }
 
 placement_comp_rule: /* empty */

 /* 10/29/2001 - enhancement */

 | '+' K_COMPONENT { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (hasBlkPlaceComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setComponent( &(defrBlockage),
 $4) ;
 hasBlkPlaceComp = 1;
 }
 }
 | '+' K_PUSHDOWN
 {
 if (hasBlkPlaceComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setPushdown( &(defrBlockage) ) ;
 hasBlkPlaceComp = 1;
 }
 }
 | '+' K_SOFT /* 5.7
  */
{
 if (defVersionNum < 5.7) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "PLACEMENT SOFT is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6547, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (hasBlkPlaceComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setSoft( &(defrBlockage) ) ;
 hasBlkPlaceComp = 1;
 }
 }
 }
 | '+' K_PARTIAL NUMBER /* 5.7
  */
{
 if (defVersionNum < 5.7) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "PARTIAL is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6548, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (hasBlkPlaceComp) {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6543, "The BLOCKAGES PLACEMNET statement already has either COMPONENT, PUSHDOWN, SOFT or PARTIAL defined.\nOnly one of these statement is allowed per LAYER.");
 CHKERR();
 }
 }
 } else {
 if (defrBlockageCbk)
 defiBlockage_setPartial( &(defrBlockage),
 $3) ;
 hasBlkPlaceComp = 1;
 }
 }
 }
 
 rectPoly_blockage_rules: /* empty */

 | rectPoly_blockage_rules rectPoly_blockage
 ;
 
 rectPoly_blockage: K_RECT pt pt
 {
 if (defrBlockageCbk)
 defiBlockage_addRect( &(defrBlockage),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 | K_POLYGON
 {
 if (defrBlockageCbk) {
 if (!defrGeomPtr) {
 defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 }
 firstPt nextPt nextPt otherPts
 {
 if (defrBlockageCbk) {
 if (defVersionNum >= 5.6) { /* only 5.6 and beyond
 */
if /* only in layer
 */
(defiBlockage_hasLayer( &(defrBlockage) )) { if (defrBlockageCbk) defiBlockage_addPolygon( &(defrBlockage),
 defrGeomPtr) ;
 } else {
 if (defrBlockageCbk) {
 if (blockageWarnings++ < defrBlockageWarnings) {
 defError(6544, "A POLYGON statement is defined in the BLOCKAGE statement,\nbut it is not defined in the BLOCKAGE LAYER statement.\nThis is illegal");
 CHKERR();
 }
 }
 }
 }
 }
 }
 
 /* 8/31/2001 - 5.4 enhancement */

 slot_section: slot_start slot_defs slot_end ;
 
 slot_start: K_SLOTS NUMBER ';'
 { if (defrSlotStartCbk)
 CALLBACK(defrSlotStartCbk, defrSlotStartCbkType, ROUND($2)); }
 
 slot_end: K_END K_SLOTS
 { if (defrSlotEndCbk)
 CALLBACK(defrSlotEndCbk, defrSlotEndCbkType, 0); }
 
 slot_defs: /* empty */

 | slot_defs slot_def
 ;
 
 slot_def: slot_rule geom_slot_rules ';'
 {
 if (defrSlotCbk) {
 CALLBACK(defrSlotCbk, defrSlotCbkType, &defrSlot);
 defiSlot_clear( &(defrSlot) ) ;
 }
 }
 
 slot_rule: '-' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrSlotCbk) {
 defiSlot_setLayer( &(defrSlot),
 $4) ;
 defiSlot_clearPoly( &(defrSlot) ) ;
 /* free poly, if any
 */
}
 } geom_slot
 
 geom_slot_rules: /* empty */

 | geom_slot_rules geom_slot
 ;
 
 geom_slot: K_RECT pt pt
 {
 if (defrSlotCbk)
 defiSlot_addRect( &(defrSlot),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 | K_POLYGON
 {
 if (!defrGeomPtr) {
 defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 firstPt nextPt nextPt otherPts
 {
 if (defVersionNum >= 5.6) { /* only 5.6 and beyond
 */
if (defrSlotCbk)
 defiSlot_addPolygon( &(defrSlot),
 defrGeomPtr) ;
 }
 }
 
 /* 8/31/2001 - 5.4 enhancement */

 fill_section: fill_start fill_defs fill_end ;
 
 fill_start: K_FILLS NUMBER ';'
 { if (defrFillStartCbk)
 CALLBACK(defrFillStartCbk, defrFillStartCbkType, ROUND($2)); }
 
 fill_end: K_END K_FILLS
 { if (defrFillEndCbk)
 CALLBACK(defrFillEndCbk, defrFillEndCbkType, 0); }
 
 fill_defs: /* empty */

 | fill_defs fill_def
 ;
 
 fill_def: fill_rule geom_fill_rules ';'
 {
 if (defrFillCbk) {
 CALLBACK(defrFillCbk, defrFillCbkType, &defrFill);
 defiFill_clear( &(defrFill) ) ;
 }
 }
 | '-' K_VIA { dumb_mode = 1; no_num = 1; } T_STRING /* 5.7
  */
{
 if (defrFillCbk) {
 defiFill_setVia( &(defrFill),
 $4) ;
 defiFill_clearPts( &(defrFill) ) ;
 if (!defrGeomPtr) {
 defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 }
 fill_via_opc firstPt otherPts ';'
 {
 if (defrFillCbk) {
 defiFill_addPts( &(defrFill),
 defrGeomPtr) ;
 CALLBACK(defrFillCbk, defrFillCbkType, &defrFill);
 defiFill_clear( &(defrFill) ) ;
 }
 }
 
 fill_rule: '-' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrFillCbk) {
 defiFill_setLayer( &(defrFill),
 $4) ;
 defiFill_clearPoly( &(defrFill) ) ;
 /* free poly, if any
 */
}
 } fill_layer_opc geom_fill /* 5.7
  */

 geom_fill_rules: /* empty */

 | geom_fill_rules geom_fill
 ;
 
 geom_fill: K_RECT pt pt
 {
 if (defrFillCbk)
 defiFill_addRect( &(defrFill),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 | K_POLYGON
 {
 if (!defrGeomPtr) {
 defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 firstPt nextPt nextPt otherPts
 {
 if (defVersionNum >= 5.6) { /* only 5.6 and beyond
 */
if (defrFillCbk)
 defiFill_addPolygon( &(defrFill),
 defrGeomPtr) ;
 } else {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "POLYGON statement in FILLS LAYER is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6564, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 
 /* 5.7
  */
fill_layer_opc: /* empty */
{}
 | '+' K_OPC
 {
 if (defVersionNum < 5.7) {
 if (defrFillCbk) {
 if (fillWarnings++ < defrFillWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "LAYER OPC is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6553, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrFillCbk)
 defiFill_setLayerOpc( &(defrFill) ) ;
 }
 }
 
 /* 5.7
  */
fill_via_opc: /* empty */
{}
 | '+' K_OPC
 {
 if (defVersionNum < 5.7) {
 if (defrFillCbk) {
 if (fillWarnings++ < defrFillWarnings) {
 defMsg = (char*)defMalloc(10000);
 sprintf (defMsg,
 "VIA OPC is a version 5.7 or later syntax.\nYour def file is defined with version %g.", defVersionNum);
 defError(6554, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else {
 if (defrFillCbk)
 defiFill_setViaOpc( &(defrFill) ) ;
 }
 }
 
 /* 11/17/2003 - 5.6 enhancement */

 nondefaultrule_section: nondefault_start nondefault_def nondefault_defs
 nondefault_end ;
 
 nondefault_start: K_NONDEFAULTRULES NUMBER ';'
 {
 if (defVersionNum < 5.6) {
 if (defrNonDefaultStartCbk) {
 if (nonDefaultWarnings++ < defrNonDefaultWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "NONDEFAULTRULE statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6545, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else if (defrNonDefaultStartCbk)
 CALLBACK(defrNonDefaultStartCbk, defrNonDefaultStartCbkType,
 ROUND($2));
 }
 
 nondefault_end: K_END K_NONDEFAULTRULES
 { if (defrNonDefaultEndCbk)
 CALLBACK(defrNonDefaultEndCbk, defrNonDefaultEndCbkType, 0); }
 
 nondefault_defs: /* empty */

 | nondefault_defs nondefault_def
 ;
 
 nondefault_def: '-' { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_clear( &(defrNonDefault) ) ;
 defiNonDefault_setName( &(defrNonDefault),
 $3) ;
 }
 }
 nondefault_options ';'
 { if (defrNonDefaultCbk)
 CALLBACK(defrNonDefaultCbk, defrNonDefaultCbkType, &defrNonDefault); }
 
 nondefault_options: /* empty */

 | nondefault_options nondefault_option
 ;
 
 nondefault_option: '+' K_HARDSPACING
 {
 if (defrNonDefaultCbk)
 defiNonDefault_setHardspacing( &(defrNonDefault) ) ;
 }
 | '+' K_LAYER { dumb_mode = 1; no_num = 1; } T_STRING
 K_WIDTH NUMBER
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addLayer( &(defrNonDefault),
 $4) ;
 defiNonDefault_addWidth( &(defrNonDefault),
 $6) ;
 }
 }
 nondefault_layer_options
 | '+' K_VIA { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addVia( &(defrNonDefault),
 $4) ;
 }
 }
 | '+' K_VIARULE { dumb_mode = 1; no_num = 1; } T_STRING
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addViaRule( &(defrNonDefault),
 $4) ;
 }
 }
 | '+' K_MINCUTS { dumb_mode = 1; no_num = 1; } T_STRING NUMBER
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addMinCuts( &(defrNonDefault),
 $4,
(int)$5) ;
 }
 }
 | nondefault_prop_opt
 ;
 
 nondefault_layer_options: /* empty */

 | nondefault_layer_options nondefault_layer_option
 
 nondefault_layer_option:
 K_DIAGWIDTH NUMBER
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addDiagWidth( &(defrNonDefault),
 $2) ;
 }
 }
 | K_SPACING NUMBER
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addSpacing( &(defrNonDefault),
 $2) ;
 }
 }
 | K_WIREEXT NUMBER
 {
 if (defrNonDefaultCbk) {
 defiNonDefault_addWireExt( &(defrNonDefault),
 $2) ;
 }
 }
 ;
 
 nondefault_prop_opt: '+' K_PROPERTY { dumb_mode = 10000; parsing_property = 1; }
 nondefault_prop_list
 { dumb_mode = 0; parsing_property = 0; }
 
 nondefault_prop_list: /* empty */

 | nondefault_prop_list nondefault_prop
 ;
 
 nondefault_prop: T_STRING NUMBER
 {
 if (defrNonDefaultCbk) {
 char propTp;
 char* str = ringCopy(" ");
 propTp = defiPropType_propType( &(defrNDefProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "NONDEFAULTRULE");
 sprintf(str, "%g", $2);
 defiNonDefault_addNumProperty( &(defrNonDefault),
 $1,
$2,
str,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (defrNonDefaultCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrNDefProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "NONDEFAULTRULE");
 defiNonDefault_addProperty( &(defrNonDefault),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (defrNonDefaultCbk) {
 char propTp;
 propTp = defiPropType_propType( &(defrNDefProp),
 $1) ;
 CHKPROPTYPE(propTp, $1, "NONDEFAULTRULE");
 defiNonDefault_addProperty( &(defrNonDefault),
 $1,
$2,
propTp) ;
 }
 }
 
 /* 12/2/2003 - 5.6 enhancement */

 styles_section: styles_start styles_rules styles_end ;
 
 styles_start: K_STYLES NUMBER ';'
 {
 if (defVersionNum < 5.6) {
 if (defrStylesStartCbk) {
 if (stylesWarnings++ < defrStylesWarnings) {
 defMsg = (char*)defMalloc(1000);
 sprintf (defMsg,
 "STYLES statement is a version 5.6 and later syntax.\nYour def file is defined with version %g", defVersionNum);
 defError(6546, defMsg);
 defFree(defMsg);
 CHKERR();
 }
 }
 } else if (defrStylesStartCbk)
 CALLBACK(defrStylesStartCbk, defrStylesStartCbkType, ROUND($2));
 }
 
 styles_end: K_END K_STYLES
 { if (defrStylesEndCbk)
 CALLBACK(defrStylesEndCbk, defrStylesEndCbkType, 0); }
 
 styles_rules: /* empty */

 | styles_rules styles_rule
 ;
 
 styles_rule: '-' K_STYLE NUMBER
 {
 if (defrStylesCbk) defiStyles_setStyle( &(defrStyles),
 (int)$3) ;
 if (!defrGeomPtr) {
 defrGeomPtr = (defiGeometries*)defMalloc(sizeof(defiGeometries));
 defiGeometries_Init( defrGeomPtr ) ;
 } else /* Just reset the number of points
 */
defiGeometries_Reset( defrGeomPtr ) ;
 }
 firstPt nextPt otherPts ';'
 {
 if (defVersionNum >= 5.6) { /* only 5.6 and beyond will call the callback
 */
if (defrStylesCbk) {
 defiStyles_setPolygon( &(defrStyles),
 defrGeomPtr) ;
 CALLBACK(defrStylesCbk, defrStylesCbkType, &defrStyles);
 }
 }
 }
 
 %%
 

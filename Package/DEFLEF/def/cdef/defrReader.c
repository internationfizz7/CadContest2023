/*
 * This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 * Distribution,  Product Version 5.7, and is subject to the Cadence LEF/DEF
 * Open Source License Agreement.   Your  continued  use  of this file
 * constitutes your acceptance of the terms of the LEF/DEF Open Source
 * License and an agreement to abide by its  terms.   If you  don't  agree
 * with  this, you must remove this and any other files which are part of the
 * distribution and  destroy any  copies made.
 * 
 * For updates, support, or to become part of the LEF/DEF Community, check
 * www.openeda.org for details.
 */
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "defrReader.h"
#include "defiProp.h"
#include "defiPropType.h"
#include "defrCallBacks.h"
#include "defiDebug.h"
#include "defiMisc.h"
#include "defiUtil.h"
#include "lex.h"
#include "DFEF_stringhash.h"
extern int defyyparse(void);	/* do the parsing */

extern void lex_init(void);	/* start the lexer */

extern int names_case_sensitive;

extern int reader_case_sensitive;

/*
 * From defiDebug.cpp
 */ extern void defrNerr(int num);

extern void NWARN(int num);

extern char defDebug[100];

int     defLogFileAppend = 0;

int     defDisPropStrProcess = 0;

int     nDDMsgs = 0;

int    *disableDMsgs = 0;

static char defrErrMsg[100];

void 
defrNerr(int i)
{
  sprintf(defrErrMsg, "ERROR number %d\n", i);
  defiError(defrErrMsg);
  exit(2);
}

void 
defrNwarn(int i)
{
  sprintf(defrErrMsg, "WARNING number %d\n", i);
  defiError(defrErrMsg);
  exit(2);
}

/*
 * Global variables
 *//* keep track if the Design is done parsing *//* The following global
 * variables are for storing the propertydefination *//* types.  Only real &
 * integer need to store since the parser can *//* distinguish string and
 * quote string *//* NEW CALLBACK - If you had to create a global instance of
 * a class for the parser to add to, then define it here.  */

/*
 * List of call back routines
 *//* These are filled in by the user.  See the *//* "set" routines at the end
 * of the file *//* NEW CALLBACK - Here is the actual location where the
 * address of the callback routines is stored.   Place each callback that you
 * create here.  */

/*
 * Flags to control number of warnings to print out, max will be 999
 *//* Flags to control what happens *//* Function to initialize global
    variables. *//* This make sure the global variables are initialized */ defiUserData defrUserData = 0;

char   *defrFileName = 0;

FILE   *defrFile = 0;

defiSubnet *defrSubnet = 0;

defiPath *defrPath = 0;

defiProp defrProp;

defiSite defrSite;

defiComponent defrComponent;

defiNet defrNet;

defiPinCap defrPinCap;

defiSite defrCannotOccupy;

defiSite defrCanplace;

defiBox defrDieArea;

defiPin defrPin;

defiRow defrRow;

defiTrack defrTrack;

defiGcellGrid defrGcellGrid;

defiVia defrVia;

defiRegion defrRegion;

defiGroup defrGroup;

defiAssertion defrAssertion;

defiScanchain defrScanchain;

defiIOTiming defrIOTiming;

defiFPC defrFPC;

defiTimingDisable defrTimingDisable;

defiPartition defrPartition;

defiPinProp defrPinProp;

defiBlockage defrBlockage;

defiSlot defrSlot;

defiFill defrFill;

defiNonDefault defrNonDefault;

defiStyles defrStyles;

defiGeometries *defrGeomPtr = 0;

int     defrIsReset = 0;

int     doneDesign = 0;

defiPropType defrCompProp;

defiPropType defrCompPinProp;

defiPropType defrDesignProp;

defiPropType defrGroupProp;

defiPropType defrNDefProp;

defiPropType defrNetProp;

defiPropType defrPinDefProp;

defiPropType defrRegionProp;

defiPropType defrRowProp;

defiPropType defrSNetProp;

defrStringCbkFnType defrDesignCbk = 0;

defrStringCbkFnType defrTechnologyCbk = 0;

defrVoidCbkFnType defrDesignEndCbk = 0;

defrPropCbkFnType defrPropCbk = 0;

defrVoidCbkFnType defrPropDefEndCbk = 0;

defrVoidCbkFnType defrPropDefStartCbk = 0;

defrStringCbkFnType defrArrayNameCbk = 0;

defrStringCbkFnType defrFloorPlanNameCbk = 0;

defrDoubleCbkFnType defrUnitsCbk = 0;

defrStringCbkFnType defrDividerCbk = 0;

defrStringCbkFnType defrBusBitCbk = 0;

defrSiteCbkFnType defrSiteCbk = 0;

defrSiteCbkFnType defrCanplaceCbk = 0;

defrSiteCbkFnType defrCannotOccupyCbk = 0;

defrIntegerCbkFnType defrComponentStartCbk = 0;

defrVoidCbkFnType defrComponentEndCbk = 0;

defrComponentCbkFnType defrComponentCbk = 0;

defrIntegerCbkFnType defrNetStartCbk = 0;

defrVoidCbkFnType defrNetEndCbk = 0;

defrNetCbkFnType defrNetCbk = 0;

defrStringCbkFnType defrNetNameCbk = 0;

defrStringCbkFnType defrNetSubnetNameCbk = 0;

defrStringCbkFnType defrNetNonDefaultRuleCbk = 0;

defrNetCbkFnType defrNetPartialPathCbk = 0;

defrPathCbkFnType defrPathCbk = 0;

defrDoubleCbkFnType defrVersionCbk = 0;

defrStringCbkFnType defrVersionStrCbk = 0;

defrStringCbkFnType defrPinExtCbk = 0;

defrStringCbkFnType defrComponentExtCbk = 0;

defrStringCbkFnType defrViaExtCbk = 0;

defrStringCbkFnType defrNetConnectionExtCbk = 0;

defrStringCbkFnType defrNetExtCbk = 0;

defrStringCbkFnType defrGroupExtCbk = 0;

defrStringCbkFnType defrScanChainExtCbk = 0;

defrStringCbkFnType defrIoTimingsExtCbk = 0;

defrStringCbkFnType defrPartitionsExtCbk = 0;

defrStringCbkFnType defrHistoryCbk = 0;

defrBoxCbkFnType defrDieAreaCbk = 0;

defrPinCapCbkFnType defrPinCapCbk = 0;

defrPinCbkFnType defrPinCbk = 0;

defrIntegerCbkFnType defrStartPinsCbk = 0;

defrVoidCbkFnType defrPinEndCbk = 0;

defrIntegerCbkFnType defrDefaultCapCbk = 0;

defrRowCbkFnType defrRowCbk = 0;

defrTrackCbkFnType defrTrackCbk = 0;

defrGcellGridCbkFnType defrGcellGridCbk = 0;

defrIntegerCbkFnType defrViaStartCbk = 0;

defrVoidCbkFnType defrViaEndCbk = 0;

defrViaCbkFnType defrViaCbk = 0;

defrIntegerCbkFnType defrRegionStartCbk = 0;

defrVoidCbkFnType defrRegionEndCbk = 0;

defrRegionCbkFnType defrRegionCbk = 0;

defrIntegerCbkFnType defrSNetStartCbk = 0;

defrVoidCbkFnType defrSNetEndCbk = 0;

defrNetCbkFnType defrSNetCbk = 0;

defrNetCbkFnType defrSNetPartialPathCbk = 0;

defrNetCbkFnType defrSNetWireCbk = 0;

defrIntegerCbkFnType defrGroupsStartCbk = 0;

defrVoidCbkFnType defrGroupsEndCbk = 0;

defrStringCbkFnType defrGroupNameCbk = 0;

defrStringCbkFnType defrGroupMemberCbk = 0;

defrGroupCbkFnType defrGroupCbk = 0;

defrIntegerCbkFnType defrAssertionsStartCbk = 0;

defrVoidCbkFnType defrAssertionsEndCbk = 0;

defrAssertionCbkFnType defrAssertionCbk = 0;

defrIntegerCbkFnType defrConstraintsStartCbk = 0;

defrVoidCbkFnType defrConstraintsEndCbk = 0;

defrAssertionCbkFnType defrConstraintCbk = 0;

defrIntegerCbkFnType defrScanchainsStartCbk = 0;

defrVoidCbkFnType defrScanchainsEndCbk = 0;

defrScanchainCbkFnType defrScanchainCbk = 0;

defrIntegerCbkFnType defrIOTimingsStartCbk = 0;

defrVoidCbkFnType defrIOTimingsEndCbk = 0;

defrIOTimingCbkFnType defrIOTimingCbk = 0;

defrIntegerCbkFnType defrFPCStartCbk = 0;

defrVoidCbkFnType defrFPCEndCbk = 0;

defrFPCCbkFnType defrFPCCbk = 0;

defrIntegerCbkFnType defrTimingDisablesStartCbk = 0;

defrVoidCbkFnType defrTimingDisablesEndCbk = 0;

defrTimingDisableCbkFnType defrTimingDisableCbk = 0;

defrIntegerCbkFnType defrPartitionsStartCbk = 0;

defrVoidCbkFnType defrPartitionsEndCbk = 0;

defrPartitionCbkFnType defrPartitionCbk = 0;

defrIntegerCbkFnType defrPinPropStartCbk = 0;

defrVoidCbkFnType defrPinPropEndCbk = 0;

defrPinPropCbkFnType defrPinPropCbk = 0;

defrIntegerCbkFnType defrCaseSensitiveCbk = 0;

defrIntegerCbkFnType defrBlockageStartCbk = 0;

defrVoidCbkFnType defrBlockageEndCbk = 0;

defrBlockageCbkFnType defrBlockageCbk = 0;

defrIntegerCbkFnType defrSlotStartCbk = 0;

defrVoidCbkFnType defrSlotEndCbk = 0;

defrSlotCbkFnType defrSlotCbk = 0;

defrIntegerCbkFnType defrFillStartCbk = 0;

defrVoidCbkFnType defrFillEndCbk = 0;

defrFillCbkFnType defrFillCbk = 0;

defrIntegerCbkFnType defrNonDefaultStartCbk = 0;

defrVoidCbkFnType defrNonDefaultEndCbk = 0;

defrNonDefaultCbkFnType defrNonDefaultCbk = 0;

defrIntegerCbkFnType defrStylesStartCbk = 0;

defrVoidCbkFnType defrStylesEndCbk = 0;

defrStylesCbkFnType defrStylesCbk = 0;

defrStringCbkFnType defrExtensionCbk = 0;

int     defrAssertionWarnings = 999;

int     defrBlockageWarnings = 999;

int     defrCaseSensitiveWarnings = 999;

int     defrComponentWarnings = 999;

int     defrConstraintWarnings = 999;

int     defrDefaultCapWarnings = 999;

int     defrFillWarnings = 999;

int     defrGcellGridWarnings = 999;

int     defrIOTimingWarnings = 999;

int     defrNetWarnings = 999;

int     defrNonDefaultWarnings = 999;

int     defrPinExtWarnings = 999;

int     defrPinWarnings = 999;

int     defrRegionWarnings = 999;

int     defrRowWarnings = 999;

int     defrTrackWarnings = 999;

int     defrScanchainWarnings = 999;

int     defrSNetWarnings = 999;

int     defrStylesWarnings = 999;

int     defrUnitsWarnings = 999;

int     defrVersionWarnings = 999;

int     defrViaWarnings = 999;

int     defrAddPathToNet = 0;

int     defrNeedPathData = 0;

int     defrAllowComponentNets = 0;

int     defrRegisterUnused = 0;

int     defrUnusedCallbacks[CBMAX];

void 
defrInitGlobals()
{
  defrUserData = 0;
  defrIsReset = 0;
  return;
}

/*
 * Parser control by the user.
 *//* Reader initialization */ int 
defrInit()
{
  defrInitGlobals();
  defrReset();

#ifndef __cplusplus
  defiProp_Init(&(defrProp));
  defiSite_Init(&(defrSite));
  defiComponent_Init(&(defrComponent));
  defiNet_Init(&(defrNet));
  defiSite_Init(&(defrCannotOccupy));
  defiSite_Init(&(defrCanplace));
  defiRow_Init(&(defrRow));
  defiTrack_Init(&(defrTrack));
  defiGcellGrid_Init(&(defrGcellGrid));
  defiVia_Init(&(defrVia));
  defiPin_Init(&(defrPin));
  defiRegion_Init(&(defrRegion));
  defiGroup_Init(&(defrGroup));
  defiAssertion_Init(&(defrAssertion));
  defiScanchain_Init(&(defrScanchain));
  defiIOTiming_Init(&(defrIOTiming));
  defiFPC_Init(&(defrFPC));
  defiTimingDisable_Init(&(defrTimingDisable));
  defiPartition_Init(&(defrPartition));
  defiPinProp_Init(&(defrPinProp));
  defiBlockage_Init(&(defrBlockage));
  defiSlot_Init(&(defrSlot));
  defiFill_Init(&(defrFill));
  defiNonDefault_Init(&(defrNonDefault));
  defiStyles_Init(&(defrStyles));
  defiPropType_Init(&(defrCompProp));
  defiPropType_Init(&(defrCompPinProp));
  defiPropType_Init(&(defrDesignProp));
  defiPropType_Init(&(defrGroupProp));
  defiPropType_Init(&(defrNDefProp));
  defiPropType_Init(&(defrNetProp));
  defiPropType_Init(&(defrPinDefProp));
  defiPropType_Init(&(defrRegionProp));
  defiPropType_Init(&(defrRowProp));
  defiPropType_Init(&(defrSNetProp));
#endif

  return 0;
}

int 
defrReset()
{
  defrIsReset = 1;
  defrFileName = 0;
  defrFile = 0;
  defrSubnet = 0;
  defrPath = 0;
  doneDesign = 0;
  return 0;
}

int 
defrCountUnused(defrCallbackType_e e, void *v, defiUserData d)
{
  int     i;

  if (defiDebug(23))
    printf("Count %d, 0x%p, 0x%p\n", (int) e, v, d);
  i = (int) e;
  if (i <= 0 || i >= CBMAX) {
    return 1;
  }
  defrUnusedCallbacks[i] += 1;
  return 0;
}

void 
defrSetRegisterUnusedCallbacks()
{
  int     i;

  defrRegisterUnused = 1;
  for (i = 0;
       i < CBMAX;
       i++)
    defrUnusedCallbacks[i] = 0;
  defrSetUnusedCallbacks(defrCountUnused);
}

void 
defrSetUnusedCallbacks(defrVoidCbkFnType f)
{
  if (!defrDesignCbk)
    defrDesignCbk = (defrStringCbkFnType) f;
  if (!defrTechnologyCbk)
    defrTechnologyCbk = (defrStringCbkFnType) f;
  if (!defrDesignEndCbk)
    defrDesignEndCbk = (defrVoidCbkFnType) f;
  if (!defrPropCbk)
    defrPropCbk = (defrPropCbkFnType) f;
  if (!defrPropDefEndCbk)
    defrPropDefEndCbk = (defrVoidCbkFnType) f;
  if (!defrPropDefStartCbk)
    defrPropDefStartCbk = (defrVoidCbkFnType) f;
  if (!defrArrayNameCbk)
    defrArrayNameCbk = (defrStringCbkFnType) f;
  if (!defrFloorPlanNameCbk)
    defrFloorPlanNameCbk = (defrStringCbkFnType) f;
  if (!defrUnitsCbk)
    defrUnitsCbk = (defrDoubleCbkFnType) f;
  if (!defrDividerCbk)
    defrDividerCbk = (defrStringCbkFnType) f;
  if (!defrBusBitCbk)
    defrBusBitCbk = (defrStringCbkFnType) f;
  if (!defrSiteCbk)
    defrSiteCbk = (defrSiteCbkFnType) f;
  if (!defrCanplaceCbk)
    defrCanplaceCbk = (defrSiteCbkFnType) f;
  if (!defrCannotOccupyCbk)
    defrCannotOccupyCbk = (defrSiteCbkFnType) f;
  if (!defrComponentStartCbk)
    defrComponentStartCbk = (defrIntegerCbkFnType) f;
  if (!defrComponentEndCbk)
    defrComponentEndCbk = (defrVoidCbkFnType) f;
  if (!defrComponentCbk)
    defrComponentCbk = (defrComponentCbkFnType) f;
  if (!defrNetStartCbk)
    defrNetStartCbk = (defrIntegerCbkFnType) f;
  if (!defrNetEndCbk)
    defrNetEndCbk = (defrVoidCbkFnType) f;
  if (!defrNetCbk)
    defrNetCbk = (defrNetCbkFnType) f;
  if (!defrPathCbk)
    defrPathCbk = (defrPathCbkFnType) f;
  if ((!defrVersionCbk) && (!defrVersionStrCbk)) {
    defrVersionCbk = (defrDoubleCbkFnType) f;
    defrVersionStrCbk = (defrStringCbkFnType) f;
  }
  if (!defrPinExtCbk)
    defrPinExtCbk = (defrStringCbkFnType) f;
  if (!defrComponentExtCbk)
    defrComponentExtCbk = (defrStringCbkFnType) f;
  if (!defrViaExtCbk)
    defrViaExtCbk = (defrStringCbkFnType) f;
  if (!defrNetConnectionExtCbk)
    defrNetConnectionExtCbk = (defrStringCbkFnType) f;
  if (!defrNetExtCbk)
    defrNetExtCbk = (defrStringCbkFnType) f;
  if (!defrGroupExtCbk)
    defrGroupExtCbk = (defrStringCbkFnType) f;
  if (!defrScanChainExtCbk)
    defrScanChainExtCbk = (defrStringCbkFnType) f;
  if (!defrIoTimingsExtCbk)
    defrIoTimingsExtCbk = (defrStringCbkFnType) f;
  if (!defrPartitionsExtCbk)
    defrPartitionsExtCbk = (defrStringCbkFnType) f;
  if (!defrHistoryCbk)
    defrHistoryCbk = (defrStringCbkFnType) f;
  if (!defrDieAreaCbk)
    defrDieAreaCbk = (defrBoxCbkFnType) f;
  if (!defrPinCapCbk)
    defrPinCapCbk = (defrPinCapCbkFnType) f;
  if (!defrPinCbk)
    defrPinCbk = (defrPinCbkFnType) f;
  if (!defrStartPinsCbk)
    defrStartPinsCbk = (defrIntegerCbkFnType) f;
  if (!defrPinEndCbk)
    defrPinEndCbk = (defrVoidCbkFnType) f;
  if (!defrDefaultCapCbk)
    defrDefaultCapCbk = (defrIntegerCbkFnType) f;
  if (!defrRowCbk)
    defrRowCbk = (defrRowCbkFnType) f;
  if (!defrTrackCbk)
    defrTrackCbk = (defrTrackCbkFnType) f;
  if (!defrGcellGridCbk)
    defrGcellGridCbk = (defrGcellGridCbkFnType) f;
  if (!defrViaStartCbk)
    defrViaStartCbk = (defrIntegerCbkFnType) f;
  if (!defrViaEndCbk)
    defrViaEndCbk = (defrVoidCbkFnType) f;
  if (!defrViaCbk)
    defrViaCbk = (defrViaCbkFnType) f;
  if (!defrRegionStartCbk)
    defrRegionStartCbk = (defrIntegerCbkFnType) f;
  if (!defrRegionEndCbk)
    defrRegionEndCbk = (defrVoidCbkFnType) f;
  if (!defrRegionCbk)
    defrRegionCbk = (defrRegionCbkFnType) f;
  if (!defrSNetStartCbk)
    defrSNetStartCbk = (defrIntegerCbkFnType) f;
  if (!defrSNetEndCbk)
    defrSNetEndCbk = (defrVoidCbkFnType) f;
  if (!defrSNetCbk)
    defrSNetCbk = (defrNetCbkFnType) f;
  if (!defrGroupsStartCbk)
    defrGroupsStartCbk = (defrIntegerCbkFnType) f;
  if (!defrGroupsEndCbk)
    defrGroupsEndCbk = (defrVoidCbkFnType) f;
  if (!defrGroupNameCbk)
    defrGroupNameCbk = (defrStringCbkFnType) f;
  if (!defrGroupMemberCbk)
    defrGroupMemberCbk = (defrStringCbkFnType) f;
  if (!defrGroupCbk)
    defrGroupCbk = (defrGroupCbkFnType) f;
  if (!defrAssertionsStartCbk)
    defrAssertionsStartCbk = (defrIntegerCbkFnType) f;
  if (!defrAssertionsEndCbk)
    defrAssertionsEndCbk = (defrVoidCbkFnType) f;
  if (!defrAssertionCbk)
    defrAssertionCbk = (defrAssertionCbkFnType) f;
  if (!defrConstraintsStartCbk)
    defrConstraintsStartCbk = (defrIntegerCbkFnType) f;
  if (!defrConstraintsEndCbk)
    defrConstraintsEndCbk = (defrVoidCbkFnType) f;
  if (!defrConstraintCbk)
    defrConstraintCbk = (defrAssertionCbkFnType) f;
  if (!defrScanchainsStartCbk)
    defrScanchainsStartCbk = (defrIntegerCbkFnType) f;
  if (!defrScanchainsEndCbk)
    defrScanchainsEndCbk = (defrVoidCbkFnType) f;
  if (!defrScanchainCbk)
    defrScanchainCbk = (defrScanchainCbkFnType) f;
  if (!defrIOTimingsStartCbk)
    defrIOTimingsStartCbk = (defrIntegerCbkFnType) f;
  if (!defrIOTimingsEndCbk)
    defrIOTimingsEndCbk = (defrVoidCbkFnType) f;
  if (!defrIOTimingCbk)
    defrIOTimingCbk = (defrIOTimingCbkFnType) f;
  if (!defrFPCStartCbk)
    defrFPCStartCbk = (defrIntegerCbkFnType) f;
  if (!defrFPCEndCbk)
    defrFPCEndCbk = (defrVoidCbkFnType) f;
  if (!defrFPCCbk)
    defrFPCCbk = (defrFPCCbkFnType) f;
  if (!defrTimingDisablesStartCbk)
    defrTimingDisablesStartCbk = (defrIntegerCbkFnType) f;
  if (!defrTimingDisablesEndCbk)
    defrTimingDisablesEndCbk = (defrVoidCbkFnType) f;
  if (!defrTimingDisableCbk)
    defrTimingDisableCbk = (defrTimingDisableCbkFnType) f;
  if (!defrPartitionsStartCbk)
    defrPartitionsStartCbk = (defrIntegerCbkFnType) f;
  if (!defrPartitionsEndCbk)
    defrPartitionsEndCbk = (defrVoidCbkFnType) f;
  if (!defrPartitionCbk)
    defrPartitionCbk = (defrPartitionCbkFnType) f;
  if (!defrPinPropStartCbk)
    defrPinPropStartCbk = (defrIntegerCbkFnType) f;
  if (!defrPinPropEndCbk)
    defrPinPropEndCbk = (defrVoidCbkFnType) f;
  if (!defrPinPropCbk)
    defrPinPropCbk = (defrPinPropCbkFnType) f;
  if (!defrCaseSensitiveCbk)
    defrCaseSensitiveCbk = (defrIntegerCbkFnType) f;
  if (!defrBlockageStartCbk)
    defrBlockageStartCbk = (defrIntegerCbkFnType) f;
  if (!defrBlockageEndCbk)
    defrBlockageEndCbk = (defrVoidCbkFnType) f;
  if (!defrBlockageCbk)
    defrBlockageCbk = (defrBlockageCbkFnType) f;
  if (!defrSlotStartCbk)
    defrSlotStartCbk = (defrIntegerCbkFnType) f;
  if (!defrSlotEndCbk)
    defrSlotEndCbk = (defrVoidCbkFnType) f;
  if (!defrSlotCbk)
    defrSlotCbk = (defrSlotCbkFnType) f;
  if (!defrFillStartCbk)
    defrFillStartCbk = (defrIntegerCbkFnType) f;
  if (!defrFillEndCbk)
    defrFillEndCbk = (defrVoidCbkFnType) f;
  if (!defrFillCbk)
    defrFillCbk = (defrFillCbkFnType) f;
  if (!defrNonDefaultStartCbk)
    defrNonDefaultStartCbk = (defrIntegerCbkFnType) f;
  if (!defrNonDefaultEndCbk)
    defrNonDefaultEndCbk = (defrVoidCbkFnType) f;
  if (!defrNonDefaultCbk)
    defrNonDefaultCbk = (defrNonDefaultCbkFnType) f;
  if (!defrStylesStartCbk)
    defrStylesStartCbk = (defrIntegerCbkFnType) f;
  if (!defrStylesEndCbk)
    defrStylesEndCbk = (defrVoidCbkFnType) f;
  if (!defrStylesCbk)
    defrStylesCbk = (defrStylesCbkFnType) f;
  if (!defrExtensionCbk)
    defrExtensionCbk = (defrStringCbkFnType) f;
}

const char *
typeToString(defrCallbackType_e num)
{
  switch ((int) num) {
    case defrUnspecifiedCbkType:return "Unspecified";
  case defrDesignStartCbkType:
    return "Design Start";
  case defrTechNameCbkType:
    return "Tech Name";
  case defrPropCbkType:
    return "Property";
  case defrPropDefEndCbkType:
    return "Property Definitions Section End";
  case defrPropDefStartCbkType:
    return "Property Definitions Section Start";
  case defrFloorPlanNameCbkType:
    return "FloorPlanName";
  case defrArrayNameCbkType:
    return "Array Name";
  case defrUnitsCbkType:
    return "Units";
  case defrDividerCbkType:
    return "Divider";
  case defrBusBitCbkType:
    return "BusBit Character";
  case defrSiteCbkType:
    return "Site";
  case defrComponentStartCbkType:
    return "Components Section Start";
  case defrComponentCbkType:
    return "Component";
  case defrComponentEndCbkType:
    return "Components Section End";
  case defrNetStartCbkType:
    return "Nets Section Start";
  case defrNetCbkType:
    return "Net";
  case defrNetNameCbkType:
    return "Net Name";
  case defrNetNonDefaultRuleCbkType:
    return "Net Nondefaultrule";
  case defrNetSubnetNameCbkType:
    return "Net Subnet Name";
  case defrNetEndCbkType:
    return "Nets Section End";
  case defrPathCbkType:
    return "Path";
  case defrVersionCbkType:
    return "Version";
  case defrVersionStrCbkType:
    return "Version";
  case defrComponentExtCbkType:
    return "Component User Extention";
  case defrPinExtCbkType:
    return "Pin User Extension";
  case defrViaExtCbkType:
    return "Via User Extension";
  case defrNetConnectionExtCbkType:
    return "NetConnection User Extention";
  case defrNetExtCbkType:
    return "Net User Extension";
  case defrGroupExtCbkType:
    return "Group User Extension";
  case defrScanChainExtCbkType:
    return "ScanChain User Extension";
  case defrIoTimingsExtCbkType:
    return "IoTimings User Extension";
  case defrPartitionsExtCbkType:
    return "Partitions User Extension";
  case defrHistoryCbkType:
    return "History";
  case defrDieAreaCbkType:
    return "DieArea";
  case defrCanplaceCbkType:
    return "Canplace";
  case defrCannotOccupyCbkType:
    return "CannotOccupy";
  case defrPinCapCbkType:
    return "PinCap";
  case defrDefaultCapCbkType:
    return "DefaultCap";
  case defrStartPinsCbkType:
    return "Start Pins Section";
  case defrPinCbkType:
    return "Pin";
  case defrPinEndCbkType:
    return "End Pins Section";
  case defrRowCbkType:
    return "Row";
  case defrTrackCbkType:
    return "Track";
  case defrGcellGridCbkType:
    return "GcellGrid";
  case defrViaStartCbkType:
    return "Start Vias Section";
  case defrViaCbkType:
    return "Via";
  case defrViaEndCbkType:
    return "End Vias Section";
  case defrRegionStartCbkType:
    return "Region Section Start";
  case defrRegionCbkType:
    return "Region";
  case defrRegionEndCbkType:
    return "Region Section End";
  case defrSNetStartCbkType:
    return "Special Net Section Start";
  case defrSNetCbkType:
    return "Special Net";
  case defrSNetEndCbkType:
    return "Special Net Section End";
  case defrGroupsStartCbkType:
    return "Groups Section Start";
  case defrGroupNameCbkType:
    return "Group Name";
  case defrGroupMemberCbkType:
    return "Group Member";
  case defrGroupCbkType:
    return "Group";
  case defrGroupsEndCbkType:
    return "Groups Section End";
  case defrAssertionsStartCbkType:
    return "Assertions Section Start";
  case defrAssertionCbkType:
    return "Assertion";
  case defrAssertionsEndCbkType:
    return "Assertions Section End";
  case defrConstraintsStartCbkType:
    return "Constraints Section Start";
  case defrConstraintCbkType:
    return "Constraint";
  case defrConstraintsEndCbkType:
    return "Constraints Section End";
  case defrScanchainsStartCbkType:
    return "Scanchains Section Start";
  case defrScanchainCbkType:
    return "Scanchain";
  case defrScanchainsEndCbkType:
    return "Scanchains Section End";
  case defrIOTimingsStartCbkType:
    return "IOTimings Section Start";
  case defrIOTimingCbkType:
    return "IOTiming";
  case defrIOTimingsEndCbkType:
    return "IOTimings Section End";
  case defrFPCStartCbkType:
    return "Floor Plan Constraints Section Start";
  case defrFPCCbkType:
    return "Floor Plan Constraint";
  case defrFPCEndCbkType:
    return "Floor Plan Constraints Section End";
  case defrTimingDisablesStartCbkType:
    return "TimingDisables Section Start";
  case defrTimingDisableCbkType:
    return "TimingDisable";
  case defrTimingDisablesEndCbkType:
    return "TimingDisables Section End";
  case defrPartitionsStartCbkType:
    return "Partitions Section Start";
  case defrPartitionCbkType:
    return "Partition";
  case defrPartitionsEndCbkType:
    return "Partitions Section End";
  case defrPinPropStartCbkType:
    return "PinProp Section Start";
  case defrPinPropCbkType:
    return "PinProp";
  case defrPinPropEndCbkType:
    return "PinProp Section End";
  case defrCaseSensitiveCbkType:
    return "CaseSensitive";
  case defrBlockageStartCbkType:
    return "Blockage Section Start";
  case defrBlockageCbkType:
    return "Blockage";
  case defrBlockageEndCbkType:
    return "Blockage Section End";
  case defrSlotStartCbkType:
    return "Slots Section Start";
  case defrSlotCbkType:
    return "Slots";
  case defrSlotEndCbkType:
    return "Slots Section End";
  case defrFillStartCbkType:
    return "Fills Section Start";
  case defrFillCbkType:
    return "Fills";
  case defrFillEndCbkType:
    return "Fills Section End";
  case defrNonDefaultStartCbkType:
    return "NonDefaultRule Section Start";
  case defrNonDefaultCbkType:
    return "NonDefault";
  case defrNonDefaultEndCbkType:
    return "NonDefaultRule Section End";
  case defrStylesStartCbkType:
    return "Styles Section Start";
  case defrStylesCbkType:
    return "Styles";
  case defrStylesEndCbkType:
    return "Styles Section End";
  case defrExtensionCbkType:
    return "Extension";
  case defrDesignEndCbkType:
    return "DesignEnd";
  default:
    break;
  }
  return "BOGUS";
}

void 
defrUnsetCallbacks()
{
  defrArrayNameCbk = 0;
  defrAssertionCbk = 0;
  defrAssertionsEndCbk = 0;
  defrAssertionsStartCbk = 0;
  defrBlockageCbk = 0;
  defrBlockageEndCbk = 0;
  defrBlockageStartCbk = 0;
  defrBusBitCbk = 0;
  defrCannotOccupyCbk = 0;
  defrCanplaceCbk = 0;
  defrCaseSensitiveCbk = 0;
  defrComponentCbk = 0;
  defrComponentEndCbk = 0;
  defrComponentExtCbk = 0;
  defrComponentStartCbk = 0;
  defrConstraintCbk = 0;
  defrConstraintsEndCbk = 0;
  defrConstraintsStartCbk = 0;
  defrDefaultCapCbk = 0;
  defrDesignCbk = 0;
  defrDesignEndCbk = 0;
  defrDieAreaCbk = 0;
  defrDividerCbk = 0;
  defrExtensionCbk = 0;
  defrFloorPlanNameCbk = 0;
  defrGcellGridCbk = 0;
  defrGroupCbk = 0;
  defrGroupExtCbk = 0;
  defrGroupMemberCbk = 0;
  defrGroupNameCbk = 0;
  defrGroupsEndCbk = 0;
  defrGroupsStartCbk = 0;
  defrFillCbk = 0;
  defrFillEndCbk = 0;
  defrFillStartCbk = 0;
  defrFPCCbk = 0;
  defrFPCEndCbk = 0;
  defrFPCStartCbk = 0;
  defrHistoryCbk = 0;
  defrIOTimingCbk = 0;
  defrIOTimingsEndCbk = 0;
  defrIoTimingsExtCbk = 0;
  defrIOTimingsStartCbk = 0;
  defrNetCbk = 0;
  defrNetConnectionExtCbk = 0;
  defrNetEndCbk = 0;
  defrNetExtCbk = 0;
  defrNetNameCbk = 0;
  defrNetNonDefaultRuleCbk = 0;
  defrNetSubnetNameCbk = 0;
  defrNetPartialPathCbk = 0;
  defrNetStartCbk = 0;
  defrNonDefaultCbk = 0;
  defrNonDefaultEndCbk = 0;
  defrNonDefaultStartCbk = 0;
  defrPartitionCbk = 0;
  defrPartitionsEndCbk = 0;
  defrPartitionsExtCbk = 0;
  defrPartitionsStartCbk = 0;
  defrPathCbk = 0;
  defrPinCapCbk = 0;
  defrPinCbk = 0;
  defrPinEndCbk = 0;
  defrPinExtCbk = 0;
  defrPinPropCbk = 0;
  defrPinPropEndCbk = 0;
  defrPinPropStartCbk = 0;
  defrPropCbk = 0;
  defrPropDefEndCbk = 0;
  defrPropDefStartCbk = 0;
  defrRegionCbk = 0;
  defrRegionEndCbk = 0;
  defrRegionStartCbk = 0;
  defrRowCbk = 0;
  defrScanchainCbk = 0;
  defrScanchainsEndCbk = 0;
  defrScanChainExtCbk = 0;
  defrScanchainsStartCbk = 0;
  defrSiteCbk = 0;
  defrSlotCbk = 0;
  defrSlotEndCbk = 0;
  defrSlotStartCbk = 0;
  defrSNetCbk = 0;
  defrSNetEndCbk = 0;
  defrSNetPartialPathCbk = 0;
  defrSNetStartCbk = 0;
  defrSNetWireCbk = 0;
  defrStartPinsCbk = 0;
  defrStylesCbk = 0;
  defrStylesEndCbk = 0;
  defrStylesStartCbk = 0;
  defrTechnologyCbk = 0;
  defrTimingDisableCbk = 0;
  defrTimingDisablesEndCbk = 0;
  defrTimingDisablesStartCbk = 0;
  defrTrackCbk = 0;
  defrUnitsCbk = 0;
  defrVersionCbk = 0;
  defrVersionStrCbk = 0;
  defrViaCbk = 0;
  defrViaExtCbk = 0;
  defrViaEndCbk = 0;
  defrViaStartCbk = 0;
}

void 
defrUnsetArrayNameCbk()
{
  defrArrayNameCbk = 0;
}

void 
defrUnsetAssertionCbk()
{
  defrAssertionCbk = 0;
}

void 
defrUnsetAssertionsStartCbk()
{
  defrAssertionsStartCbk = 0;
}

void 
defrUnsetAssertionsEndCbk()
{
  defrAssertionsEndCbk = 0;
}

void 
defrUnsetBlockageCbk()
{
  defrBlockageCbk = 0;
}

void 
defrUnsetBlockageStartCbk()
{
  defrBlockageStartCbk = 0;
}

void 
defrUnsetBlockageEndCbk()
{
  defrBlockageEndCbk = 0;
}

void 
defrUnsetBusBitCbk()
{
  defrBusBitCbk = 0;
}

void 
defrUnsetCannotOccupyCbk()
{
  defrCannotOccupyCbk = 0;
}

void 
defrUnsetCanplaceCbk()
{
  defrCanplaceCbk = 0;
}

void 
defrUnsetCaseSensitiveCbk()
{
  defrCaseSensitiveCbk = 0;
}

void 
defrUnsetComponentCbk()
{
  defrComponentCbk = 0;
}

void 
defrUnsetComponentExtCbk()
{
  defrComponentExtCbk = 0;
}

void 
defrUnsetComponentStartCbk()
{
  defrComponentStartCbk = 0;
}

void 
defrUnsetComponentEndCbk()
{
  defrComponentEndCbk = 0;
}

void 
defrUnsetConstraintCbk()
{
  defrConstraintCbk = 0;
}

void 
defrUnsetConstraintsStartCbk()
{
  defrConstraintsStartCbk = 0;
}

void 
defrUnsetConstraintsEndCbk()
{
  defrConstraintsEndCbk = 0;
}

void 
defrUnsetDefaultCapCbk()
{
  defrDefaultCapCbk = 0;
}

void 
defrUnsetDesignCbk()
{
  defrDesignCbk = 0;
}

void 
defrUnsetDesignEndCbk()
{
  defrDesignEndCbk = 0;
}

void 
defrUnsetDieAreaCbk()
{
  defrDieAreaCbk = 0;
}

void 
defrUnsetDividerCbk()
{
  defrDividerCbk = 0;
}

void 
defrUnsetExtensionCbk()
{
  defrExtensionCbk = 0;
}

void 
defrUnsetFillCbk()
{
  defrFillCbk = 0;
}

void 
defrUnsetFillStartCbk()
{
  defrFillStartCbk = 0;
}

void 
defrUnsetFillEndCbk()
{
  defrFillEndCbk = 0;
}

void 
defrUnsetFPCCbk()
{
  defrFPCCbk = 0;
}

void 
defrUnsetFPCStartCbk()
{
  defrFPCStartCbk = 0;
}

void 
defrUnsetFPCEndCbk()
{
  defrFPCEndCbk = 0;
}

void 
defrUnsetFloorPlanNameCbk()
{
  defrFloorPlanNameCbk = 0;
}

void 
defrUnsetGcellGridCbk()
{
  defrGcellGridCbk = 0;
}

void 
defrUnsetGroupCbk()
{
  defrGroupCbk = 0;
}

void 
defrUnsetGroupExtCbk()
{
  defrGroupExtCbk = 0;
}

void 
defrUnsetGroupMemberCbk()
{
  defrGroupMemberCbk = 0;
}

void 
defrUnsetGroupNameCbk()
{
  defrGroupNameCbk = 0;
}

void 
defrUnsetGroupsStartCbk()
{
  defrGroupsStartCbk = 0;
}

void 
defrUnsetGroupsEndCbk()
{
  defrGroupsEndCbk = 0;
}

void 
defrUnsetHistoryCbk()
{
  defrHistoryCbk = 0;
}

void 
defrUnsetIOTimingCbk()
{
  defrIOTimingCbk = 0;
}

void 
defrUnsetIOTimingsStartCbk()
{
  defrIOTimingsStartCbk = 0;
}

void 
defrUnsetIOTimingsEndCbk()
{
  defrIOTimingsEndCbk = 0;
}

void 
defrUnsetIOTimingsExtCbk()
{
  defrIoTimingsExtCbk = 0;
}

void 
defrUnsetNetCbk()
{
  defrNetCbk = 0;
}

void 
defrUnsetNetNameCbk()
{
  defrNetNameCbk = 0;
}

void 
defrUnsetNetNonDefaultRuleCbk()
{
  defrNetNonDefaultRuleCbk = 0;
}

void 
defrUnsetNetConnectionExtCbk()
{
  defrNetConnectionExtCbk = 0;
}

void 
defrUnsetNetExtCbk()
{
  defrNetExtCbk = 0;
}

void 
defrUnsetNetPartialPathCbk()
{
  defrNetPartialPathCbk = 0;
}

void 
defrUnsetNetSubnetNameCbk()
{
  defrNetSubnetNameCbk = 0;
}

void 
defrUnsetNetStartCbk()
{
  defrNetStartCbk = 0;
}

void 
defrUnsetNetEndCbk()
{
  defrNetEndCbk = 0;
}

void 
defrUnsetNonDefaultCbk()
{
  defrNonDefaultCbk = 0;
}

void 
defrUnsetNonDefaultStartCbk()
{
  defrNonDefaultStartCbk = 0;
}

void 
defrUnsetNonDefaultEndCbk()
{
  defrNonDefaultEndCbk = 0;
}

void 
defrUnsetPartitionCbk()
{
  defrPartitionCbk = 0;
}

void 
defrUnsetPartitionsExtCbk()
{
  defrPartitionsExtCbk = 0;
}

void 
defrUnsetPartitionsStartCbk()
{
  defrPartitionsStartCbk = 0;
}

void 
defrUnsetPartitionsEndCbk()
{
  defrPartitionsEndCbk = 0;
}

void 
defrUnsetPathCbk()
{
  defrPathCbk = 0;
}

void 
defrUnsetPinCapCbk()
{
  defrPinCapCbk = 0;
}

void 
defrUnsetPinCbk()
{
  defrPinCbk = 0;
}

void 
defrUnsetPinEndCbk()
{
  defrPinEndCbk = 0;
}

void 
defrUnsetPinExtCbk()
{
  defrPinExtCbk = 0;
}

void 
defrUnsetPinPropCbk()
{
  defrPinPropCbk = 0;
}

void 
defrUnsetPinPropStartCbk()
{
  defrPinPropStartCbk = 0;
}

void 
defrUnsetPinPropEndCbk()
{
  defrPinPropEndCbk = 0;
}

void 
defrUnsetPropCbk()
{
  defrPropCbk = 0;
}

void 
defrUnsetPropDefEndCbk()
{
  defrPropDefEndCbk = 0;
}

void 
defrUnsetPropDefStartCbk()
{
  defrPropDefStartCbk = 0;
}

void 
defrUnsetRegionCbk()
{
  defrRegionCbk = 0;
}

void 
defrUnsetRegionStartCbk()
{
  defrRegionStartCbk = 0;
}

void 
defrUnsetRegionEndCbk()
{
  defrRegionEndCbk = 0;
}

void 
defrUnsetRowCbk()
{
  defrRowCbk = 0;
}

void 
defrUnsetScanChainExtCbk()
{
  defrScanChainExtCbk = 0;
}

void 
defrUnsetScanchainCbk()
{
  defrScanchainCbk = 0;
}

void 
defrUnsetScanchainsStartCbk()
{
  defrScanchainsStartCbk = 0;
}

void 
defrUnsetScanchainsEndCbk()
{
  defrScanchainsEndCbk = 0;
}

void 
defrUnsetSiteCbk()
{
  defrSiteCbk = 0;
}

void 
defrUnsetSlotCbk()
{
  defrSlotCbk = 0;
}

void 
defrUnsetSlotStartCbk()
{
  defrSlotStartCbk = 0;
}

void 
defrUnsetSlotEndCbk()
{
  defrSlotEndCbk = 0;
}

void 
defrUnsetSNetWireCbk()
{
  defrSNetWireCbk = 0;
}

void 
defrUnsetSNetCbk()
{
  defrSNetCbk = 0;
}

void 
defrUnsetSNetStartCbk()
{
  defrSNetStartCbk = 0;
}

void 
defrUnsetSNetEndCbk()
{
  defrSNetEndCbk = 0;
}

void 
defrUnsetSNetPartialPathCbk()
{
  defrSNetPartialPathCbk = 0;
}

void 
defrUnsetStartPinsCbk()
{
  defrStartPinsCbk = 0;
}

void 
defrUnsetStylesCbk()
{
  defrStylesCbk = 0;
}

void 
defrUnsetStylesStartCbk()
{
  defrStylesStartCbk = 0;
}

void 
defrUnsetStylesEndCbk()
{
  defrStylesEndCbk = 0;
}

void 
defrUnsetTechnologyCbk()
{
  defrTechnologyCbk = 0;
}

void 
defrUnsetTimingDisableCbk()
{
  defrTimingDisableCbk = 0;
}

void 
defrUnsetTimingDisablesStartCbk()
{
  defrTimingDisablesStartCbk = 0;
}

void 
defrUnsetTimingDisablesEndCbk()
{
  defrTimingDisablesEndCbk = 0;
}

void 
defrUnsetTrackCbk()
{
  defrTrackCbk = 0;
}

void 
defrUnsetUnitsCbk()
{
  defrUnitsCbk = 0;
}

void 
defrUnsetVersionCbk()
{
  defrVersionCbk = 0;
}

void 
defrUnsetVersionStrCbk()
{
  defrVersionStrCbk = 0;
}

void 
defrUnsetViaCbk()
{
  defrViaCbk = 0;
}

void 
defrUnsetViaExtCbk()
{
  defrViaExtCbk = 0;
}

void 
defrUnsetViaStartCbk()
{
  defrViaStartCbk = 0;
}

void 
defrUnsetViaEndCbk()
{
  defrViaEndCbk = 0;
}

void 
defrPrintUnusedCallbacks(FILE * log)
{
  int     i;

  int     first = 1;

  for (i = 0;
       i < CBMAX;
       i++) {
    if (defrUnusedCallbacks[i]) {
      if (first) {
	fprintf(log, "WARNING (DEFPARS-5001): DEF statements found in the def file that no callback was set:\n");
	first = 0;
      }
      fprintf(log, "%5d %s\n", defrUnusedCallbacks[i], typeToString((defrCallbackType_e) i));
    }
  }
}

int    *
defUnusedCallbackCount()
{
  return defrUnusedCallbacks;
}

int 
defrCatchAll(defrCallbackType_e typ, void *data, defiUserData ud)
{
  if (defDebug[3])
    printf("CatchAll(%d, 0x%p, 0x%p)\n", typ, data, ud);
  if ((int) typ >= 0 && (int) typ < CBMAX) {
    defrUnusedCallbacks[(int) typ] += 1;
  }
  else {
    defrUnusedCallbacks[0] += 1;
    return 1;
  }
  return 0;
}

const char *
defrFName()
{
  return defrFileName;
}

int 
defrReleaseNResetMemory()
{
  defiAssertion_Destroy(&(defrAssertion));
  defiBlockage_Destroy(&(defrBlockage));
  defiSite_Destroy(&(defrCannotOccupy));
  defiSite_Destroy(&(defrCanplace));
  defiComponent_Destroy(&(defrComponent));
  defiPropType_Destroy(&(defrCompPinProp));
  defiPropType_Destroy(&(defrCompProp));
  defiPropType_Destroy(&(defrDesignProp));
  defiFill_Destroy(&(defrFill));
  defiFPC_Destroy(&(defrFPC));
  defiGcellGrid_Destroy(&(defrGcellGrid));
  defiGroup_Destroy(&(defrGroup));
  defiPropType_Destroy(&(defrGroupProp));
  defiIOTiming_Destroy(&(defrIOTiming));
  defiPropType_Destroy(&(defrNDefProp));
  defiNet_Destroy(&(defrNet));
  defiPropType_Destroy(&(defrNetProp));
  defiNonDefault_Destroy(&(defrNonDefault));
  defiPartition_Destroy(&(defrPartition));
  defiPin_Destroy(&(defrPin));
  defiPropType_Destroy(&(defrPinDefProp));
  defiPinProp_Destroy(&(defrPinProp));
  defiProp_Destroy(&(defrProp));
  defiRegion_Destroy(&(defrRegion));
  defiPropType_Destroy(&(defrRegionProp));
  defiRow_Destroy(&(defrRow));
  defiPropType_Destroy(&(defrRowProp));
  defiScanchain_Destroy(&(defrScanchain));
  defiSite_Destroy(&(defrSite));
  defiSlot_Destroy(&(defrSlot));
  defiPropType_Destroy(&(defrSNetProp));
  defiStyles_Destroy(&(defrStyles));
  defiTimingDisable_Destroy(&(defrTimingDisable));
  defiTrack_Destroy(&(defrTrack));
  defiVia_Destroy(&(defrVia));
  defiAssertion_Init(&(defrAssertion));
  defiBlockage_Init(&(defrBlockage));
  defiSite_Init(&(defrCannotOccupy));
  defiSite_Init(&(defrCanplace));
  defiComponent_Init(&(defrComponent));
  defiPropType_Init(&(defrCompPinProp));
  defiPropType_Init(&(defrCompProp));
  defiPropType_Init(&(defrDesignProp));
  defiFill_Init(&(defrFill));
  defiFPC_Init(&(defrFPC));
  defiGcellGrid_Init(&(defrGcellGrid));
  defiGroup_Init(&(defrGroup));
  defiPropType_Init(&(defrGroupProp));
  defiIOTiming_Init(&(defrIOTiming));
  defiPropType_Init(&(defrNDefProp));
  defiNet_Init(&(defrNet));
  defiPropType_Init(&(defrNetProp));
  defiNonDefault_Init(&(defrNonDefault));
  defiPartition_Init(&(defrPartition));
  defiPin_Init(&(defrPin));
  defiPropType_Init(&(defrPinDefProp));
  defiPinProp_Init(&(defrPinProp));
  defiProp_Init(&(defrProp));
  defiRegion_Init(&(defrRegion));
  defiPropType_Init(&(defrRegionProp));
  defiRow_Init(&(defrRow));
  defiPropType_Init(&(defrRowProp));
  defiScanchain_Init(&(defrScanchain));
  defiSite_Init(&(defrSite));
  defiSlot_Init(&(defrSlot));
  defiPropType_Init(&(defrSNetProp));
  defiStyles_Init(&(defrStyles));
  defiTimingDisable_Init(&(defrTimingDisable));
  defiTrack_Init(&(defrTrack));
  defiVia_Init(&(defrVia));
  return 0;
}

int 
defrRead(FILE * f, const char *fName, defiUserData uData, int case_sensitive)
{
  int     status;

  if (defrIsReset == 0) {
    fprintf(stderr, "ERROR (DEFPARS-5000): defrRead called before defrInit\n");
    return -1;
  }
  defrFileName = (char *) fName;
  defrFile = f;
  defrUserData = uData;
  reader_case_sensitive = case_sensitive;
  defrNeedPathData = (((defrNetCbk || defrSNetCbk) && defrAddPathToNet) || defrPathCbk) ? 1 : 0;
  if (defrNeedPathData) {
    defrPath = (defiPath *) malloc(sizeof(defiPath));
    defiPath_Init(defrPath);
  }
  lex_init();
  status = defyyparse();
  defiProp_clear(&(defrProp));
  defiSite_clear(&(defrSite));
  defiComponent_clear(&(defrComponent));
  defiNet_clear(&(defrNet));
  defiSite_clear(&(defrCannotOccupy));
  defiSite_clear(&(defrCanplace));
  defiScanchain_clear(&(defrScanchain));
  defiIOTiming_clear(&(defrIOTiming));
  defiFPC_clear(&(defrFPC));
  defiTimingDisable_clear(&(defrTimingDisable));
  defiPartition_clear(&(defrPartition));
  if (defrNeedPathData) {
    if (defrPath) {
      defiPath_Destroy(defrPath);
      defFree((char *) (defrPath));
    }
  }
  lex_un_init();
  defrIsReset = 0;
  return status;
}

void 
defrSetUserData(defiUserData d)
{
  defrUserData = d;
}

defiUserData 
defrGetUserData()
{
  return defrUserData;
}

void 
defrSetDesignCbk(defrStringCbkFnType f)
{
  defrDesignCbk = f;
}

void 
defrSetTechnologyCbk(defrStringCbkFnType f)
{
  defrTechnologyCbk = f;
}

void 
defrSetDesignEndCbk(defrVoidCbkFnType f)
{
  defrDesignEndCbk = f;
}

void 
defrSetPropCbk(defrPropCbkFnType f)
{
  defrPropCbk = f;
}

void 
defrSetPropDefEndCbk(defrVoidCbkFnType f)
{
  defrPropDefEndCbk = f;
}

void 
defrSetPropDefStartCbk(defrVoidCbkFnType f)
{
  defrPropDefStartCbk = f;
}

void 
defrSetArrayNameCbk(defrStringCbkFnType f)
{
  defrArrayNameCbk = f;
}

void 
defrSetFloorPlanNameCbk(defrStringCbkFnType f)
{
  defrFloorPlanNameCbk = f;
}

void 
defrSetUnitsCbk(defrDoubleCbkFnType f)
{
  defrUnitsCbk = f;
}

void 
defrSetVersionCbk(defrDoubleCbkFnType f)
{
  defrVersionCbk = f;
}

void 
defrSetVersionStrCbk(defrStringCbkFnType f)
{
  defrVersionStrCbk = f;
}

void 
defrSetDividerCbk(defrStringCbkFnType f)
{
  defrDividerCbk = f;
}

void 
defrSetBusBitCbk(defrStringCbkFnType f)
{
  defrBusBitCbk = f;
}

void 
defrSetSiteCbk(defrSiteCbkFnType f)
{
  defrSiteCbk = f;
}

void 
defrSetCanplaceCbk(defrSiteCbkFnType f)
{
  defrCanplaceCbk = f;
}

void 
defrSetCannotOccupyCbk(defrSiteCbkFnType f)
{
  defrCannotOccupyCbk = f;
}

void 
defrSetComponentStartCbk(defrIntegerCbkFnType f)
{
  defrComponentStartCbk = f;
}

void 
defrSetComponentEndCbk(defrVoidCbkFnType f)
{
  defrComponentEndCbk = f;
}

void 
defrSetComponentCbk(defrComponentCbkFnType f)
{
  defrComponentCbk = f;
}

void 
defrSetNetStartCbk(defrIntegerCbkFnType f)
{
  defrNetStartCbk = f;
}

void 
defrSetNetEndCbk(defrVoidCbkFnType f)
{
  defrNetEndCbk = f;
}

void 
defrSetNetCbk(defrNetCbkFnType f)
{
  defrNetCbk = f;
}

void 
defrSetNetNameCbk(defrStringCbkFnType f)
{
  defrNetNameCbk = f;
}

void 
defrSetNetSubnetNameCbk(defrStringCbkFnType f)
{
  defrNetSubnetNameCbk = f;
}

void 
defrSetNetNonDefaultRuleCbk(defrStringCbkFnType f)
{
  defrNetNonDefaultRuleCbk = f;
}

void 
defrSetNetPartialPathCbk(defrNetCbkFnType f)
{
  defrNetPartialPathCbk = f;
}

void 
defrSetSNetStartCbk(defrIntegerCbkFnType f)
{
  defrSNetStartCbk = f;
}

void 
defrSetSNetEndCbk(defrVoidCbkFnType f)
{
  defrSNetEndCbk = f;
}

void 
defrSetSNetCbk(defrNetCbkFnType f)
{
  defrSNetCbk = f;
}

void 
defrSetSNetPartialPathCbk(defrNetCbkFnType f)
{
  defrSNetPartialPathCbk = f;
}

void 
defrSetSNetWireCbk(defrNetCbkFnType f)
{
  defrSNetWireCbk = f;
}

void 
defrSetPathCbk(defrPathCbkFnType f)
{
  defrPathCbk = f;
}

void 
defrSetAddPathToNet()
{
  defrAddPathToNet = 1;
}

void 
defrSetAllowComponentNets()
{
  defrAllowComponentNets = 1;
}

void 
defrSetComponentExtCbk(defrStringCbkFnType f)
{
  defrComponentExtCbk = f;
}

void 
defrSetPinExtCbk(defrStringCbkFnType f)
{
  defrPinExtCbk = f;
}

void 
defrSetViaExtCbk(defrStringCbkFnType f)
{
  defrViaExtCbk = f;
}

void 
defrSetNetConnectionExtCbk(defrStringCbkFnType f)
{
  defrNetConnectionExtCbk = f;
}

void 
defrSetNetExtCbk(defrStringCbkFnType f)
{
  defrNetExtCbk = f;
}

void 
defrSetGroupExtCbk(defrStringCbkFnType f)
{
  defrGroupExtCbk = f;
}

void 
defrSetScanChainExtCbk(defrStringCbkFnType f)
{
  defrScanChainExtCbk = f;
}

void 
defrSetIoTimingsExtCbk(defrStringCbkFnType f)
{
  defrIoTimingsExtCbk = f;
}

void 
defrSetPartitionsExtCbk(defrStringCbkFnType f)
{
  defrPartitionsExtCbk = f;
}

void 
defrSetHistoryCbk(defrStringCbkFnType f)
{
  defrHistoryCbk = f;
}

void 
defrSetDieAreaCbk(defrBoxCbkFnType f)
{
  defrDieAreaCbk = f;
}

void 
defrSetPinCapCbk(defrPinCapCbkFnType f)
{
  defrPinCapCbk = f;
}

void 
defrSetPinEndCbk(defrVoidCbkFnType f)
{
  defrPinEndCbk = f;
}

void 
defrSetStartPinsCbk(defrIntegerCbkFnType f)
{
  defrStartPinsCbk = f;
}

void 
defrSetDefaultCapCbk(defrIntegerCbkFnType f)
{
  defrDefaultCapCbk = f;
}

void 
defrSetPinCbk(defrPinCbkFnType f)
{
  defrPinCbk = f;
}

void 
defrSetRowCbk(defrRowCbkFnType f)
{
  defrRowCbk = f;
}

void 
defrSetTrackCbk(defrTrackCbkFnType f)
{
  defrTrackCbk = f;
}

void 
defrSetGcellGridCbk(defrGcellGridCbkFnType f)
{
  defrGcellGridCbk = f;
}

void 
defrSetViaStartCbk(defrIntegerCbkFnType f)
{
  defrViaStartCbk = f;
}

void 
defrSetViaEndCbk(defrVoidCbkFnType f)
{
  defrViaEndCbk = f;
}

void 
defrSetViaCbk(defrViaCbkFnType f)
{
  defrViaCbk = f;
}

void 
defrSetRegionStartCbk(defrIntegerCbkFnType f)
{
  defrRegionStartCbk = f;
}

void 
defrSetRegionEndCbk(defrVoidCbkFnType f)
{
  defrRegionEndCbk = f;
}

void 
defrSetRegionCbk(defrRegionCbkFnType f)
{
  defrRegionCbk = f;
}

void 
defrSetGroupsStartCbk(defrIntegerCbkFnType f)
{
  defrGroupsStartCbk = f;
}

void 
defrSetGroupsEndCbk(defrVoidCbkFnType f)
{
  defrGroupsEndCbk = f;
}

void 
defrSetGroupNameCbk(defrStringCbkFnType f)
{
  defrGroupNameCbk = f;
}

void 
defrSetGroupMemberCbk(defrStringCbkFnType f)
{
  defrGroupMemberCbk = f;
}

void 
defrSetGroupCbk(defrGroupCbkFnType f)
{
  defrGroupCbk = f;
}

void 
defrSetAssertionsStartCbk(defrIntegerCbkFnType f)
{
  defrAssertionsStartCbk = f;
}

void 
defrSetAssertionsEndCbk(defrVoidCbkFnType f)
{
  defrAssertionsEndCbk = f;
}

void 
defrSetAssertionCbk(defrAssertionCbkFnType f)
{
  defrAssertionCbk = f;
}

void 
defrSetConstraintsStartCbk(defrIntegerCbkFnType f)
{
  defrConstraintsStartCbk = f;
}

void 
defrSetConstraintsEndCbk(defrVoidCbkFnType f)
{
  defrConstraintsEndCbk = f;
}

void 
defrSetConstraintCbk(defrAssertionCbkFnType f)
{
  defrConstraintCbk = f;
}

void 
defrSetScanchainsStartCbk(defrIntegerCbkFnType f)
{
  defrScanchainsStartCbk = f;
}

void 
defrSetScanchainsEndCbk(defrVoidCbkFnType f)
{
  defrScanchainsEndCbk = f;
}

void 
defrSetScanchainCbk(defrScanchainCbkFnType f)
{
  defrScanchainCbk = f;
}

void 
defrSetIOTimingsStartCbk(defrIntegerCbkFnType f)
{
  defrIOTimingsStartCbk = f;
}

void 
defrSetIOTimingsEndCbk(defrVoidCbkFnType f)
{
  defrIOTimingsEndCbk = f;
}

void 
defrSetIOTimingCbk(defrIOTimingCbkFnType f)
{
  defrIOTimingCbk = f;
}

void 
defrSetFPCStartCbk(defrIntegerCbkFnType f)
{
  defrFPCStartCbk = f;
}

void 
defrSetFPCEndCbk(defrVoidCbkFnType f)
{
  defrFPCEndCbk = f;
}

void 
defrSetFPCCbk(defrFPCCbkFnType f)
{
  defrFPCCbk = f;
}

void 
defrSetTimingDisablesStartCbk(defrIntegerCbkFnType f)
{
  defrTimingDisablesStartCbk = f;
}

void 
defrSetTimingDisablesEndCbk(defrVoidCbkFnType f)
{
  defrTimingDisablesEndCbk = f;
}

void 
defrSetTimingDisableCbk(defrTimingDisableCbkFnType f)
{
  defrTimingDisableCbk = f;
}

void 
defrSetPartitionsStartCbk(defrIntegerCbkFnType f)
{
  defrPartitionsStartCbk = f;
}

void 
defrSetPartitionsEndCbk(defrVoidCbkFnType f)
{
  defrPartitionsEndCbk = f;
}

void 
defrSetPartitionCbk(defrPartitionCbkFnType f)
{
  defrPartitionCbk = f;
}

void 
defrSetPinPropStartCbk(defrIntegerCbkFnType f)
{
  defrPinPropStartCbk = f;
}

void 
defrSetPinPropEndCbk(defrVoidCbkFnType f)
{
  defrPinPropEndCbk = f;
}

void 
defrSetPinPropCbk(defrPinPropCbkFnType f)
{
  defrPinPropCbk = f;
}

void 
defrSetCaseSensitiveCbk(defrIntegerCbkFnType f)
{
  defrCaseSensitiveCbk = f;
}

void 
defrSetBlockageStartCbk(defrIntegerCbkFnType f)
{
  defrBlockageStartCbk = f;
}

void 
defrSetBlockageEndCbk(defrVoidCbkFnType f)
{
  defrBlockageEndCbk = f;
}

void 
defrSetBlockageCbk(defrBlockageCbkFnType f)
{
  defrBlockageCbk = f;
}

void 
defrSetSlotStartCbk(defrIntegerCbkFnType f)
{
  defrSlotStartCbk = f;
}

void 
defrSetSlotEndCbk(defrVoidCbkFnType f)
{
  defrSlotEndCbk = f;
}

void 
defrSetSlotCbk(defrSlotCbkFnType f)
{
  defrSlotCbk = f;
}

void 
defrSetFillStartCbk(defrIntegerCbkFnType f)
{
  defrFillStartCbk = f;
}

void 
defrSetFillEndCbk(defrVoidCbkFnType f)
{
  defrFillEndCbk = f;
}

void 
defrSetFillCbk(defrFillCbkFnType f)
{
  defrFillCbk = f;
}

void 
defrSetNonDefaultStartCbk(defrIntegerCbkFnType f)
{
  defrNonDefaultStartCbk = f;
}

void 
defrSetNonDefaultEndCbk(defrVoidCbkFnType f)
{
  defrNonDefaultEndCbk = f;
}

void 
defrSetNonDefaultCbk(defrNonDefaultCbkFnType f)
{
  defrNonDefaultCbk = f;
}

void 
defrSetStylesStartCbk(defrIntegerCbkFnType f)
{
  defrStylesStartCbk = f;
}

void 
defrSetStylesEndCbk(defrVoidCbkFnType f)
{
  defrStylesEndCbk = f;
}

void 
defrSetStylesCbk(defrStylesCbkFnType f)
{
  defrStylesCbk = f;
}

/* NEW CALLBACK - Put the set functions for the new callbacks here.  */
extern DEFI_LOG_FUNCTION defiErrorLogFunction;

void 
defrSetLogFunction(DEFI_LOG_FUNCTION f)
{
  defiErrorLogFunction = f;
}

extern DEFI_WARNING_LOG_FUNCTION defiWarningLogFunction;

void 
defrSetWarningLogFunction(DEFI_WARNING_LOG_FUNCTION f)
{
  defiWarningLogFunction = f;
}

DEFI_MALLOC_FUNCTION defiMallocFunction = 0;

void 
defrSetMallocFunction(DEFI_MALLOC_FUNCTION f)
{
  defiMallocFunction = f;
}

DEFI_REALLOC_FUNCTION defiReallocFunction = 0;

void 
defrSetReallocFunction(DEFI_REALLOC_FUNCTION f)
{
  defiReallocFunction = f;
}

DEFI_FREE_FUNCTION defiFreeFunction = 0;

void 
defrSetFreeFunction(DEFI_FREE_FUNCTION f)
{
  defiFreeFunction = f;
}

extern DEFI_LINE_NUMBER_FUNCTION defiLineNumberFunction;

void 
defrSetLineNumberFunction(DEFI_LINE_NUMBER_FUNCTION f)
{
  defiLineNumberFunction = f;
}

extern int defiDeltaNumberLines;

void 
defrSetDeltaNumberLines(int numLines)
{
  defiDeltaNumberLines = numLines;
}

void 
pathIsDone(int shield, int reset, int netOsnet, int *needCbk)
{
  if ((defrNetCbk || defrSNetCbk) && defrAddPathToNet) {
    if (defrSubnet) {
      defiSubnet_addWirePath(defrSubnet,
			     defrPath,
			     reset,
			     netOsnet,
			     needCbk);
    }
    else {
      if (shield)
	defiNet_addShieldPath(&(defrNet),
			      defrPath,
			      reset,
			      netOsnet,
			      needCbk);
      else
	defiNet_addWirePath(&(defrNet),
			    defrPath,
			    reset,
			    netOsnet,
			    needCbk);
    }
  }
  else if (defrPathCbk) {
    (*defrPathCbk) (defrPathCbkType, defrPath, defrUserData);
    defiPath_Destroy(defrPath);
    free((char *) defrPath);
  }
  defrPath = (defiPath *) malloc(sizeof(defiPath));
  defiPath_Init(defrPath);
}

/* from lexer  */
extern char commentChar;

void 
defrSetCommentChar(char c)
{
  commentChar = c;
}

void 
defrSetCaseSensitivity(int caseSense)
{
  names_case_sensitive = caseSense;
  reader_case_sensitive = caseSense;
}

/*
 * 4/4/2003
 */ void 
defrSetExtensionCbk(defrStringCbkFnType f)
{
  defrExtensionCbk = f;
}

/*
 * Set the maximum number of warnings
 */ void 
defrSetAssertionWarnings(int warn)
{
  defrAssertionWarnings = warn;
}

void 
defrSetBlockageWarnings(int warn)
{
  defrBlockageWarnings = warn;
}

void 
defrSetCaseSensitiveWarnings(int warn)
{
  defrCaseSensitiveWarnings = warn;
}

void 
defrSetComponentWarnings(int warn)
{
  defrComponentWarnings = warn;
}

void 
defrSetConstraintWarnings(int warn)
{
  defrConstraintWarnings = warn;
}

void 
defrSetDefaultCapWarnings(int warn)
{
  defrDefaultCapWarnings = warn;
}

void 
defrSetFillWarnings(int warn)
{
  defrFillWarnings = warn;
}

void 
defrSetGcellGridWarnings(int warn)
{
  defrGcellGridWarnings = warn;
}

void 
defrSetIOTimingWarnings(int warn)
{
  defrIOTimingWarnings = warn;
}

void 
defrSetNetWarnings(int warn)
{
  defrNetWarnings = warn;
}

void 
defrSetNonDefaultWarnings(int warn)
{
  defrNonDefaultWarnings = warn;
}

void 
defrSetPinExtWarnings(int warn)
{
  defrPinExtWarnings = warn;
}

void 
defrSetPinWarnings(int warn)
{
  defrPinWarnings = warn;
}

void 
defrSetRegionWarnings(int warn)
{
  defrRegionWarnings = warn;
}

void 
defrSetRowWarnings(int warn)
{
  defrRowWarnings = warn;
}

void 
defrSetScanchainWarnings(int warn)
{
  defrScanchainWarnings = warn;
}

void 
defrSetSNetWarnings(int warn)
{
  defrSNetWarnings = warn;
}

void 
defrSetStylesWarnings(int warn)
{
  defrStylesWarnings = warn;
}

void 
defrSetTrackWarnings(int warn)
{
  defrTrackWarnings = warn;
}

void 
defrSetUnitsWarnings(int warn)
{
  defrUnitsWarnings = warn;
}

void 
defrSetVersionWarnings(int warn)
{
  defrVersionWarnings = warn;
}

void 
defrSetViaWarnings(int warn)
{
  defrViaWarnings = warn;
}

extern void *def_alias_set;

void 
defrAddAlias(const char *key, const char *value, int marked)
{
  char   *k1;

  char   *v1;

  int     len = strlen(key) + 1;

  k1 = (char *) malloc(len);
  strcpy(k1, key);
  len = strlen(value) + 1 + 1;
  v1 = (char *) malloc(len);
  if (marked != 0)
    marked = 1;
  sprintf(v1, "%d%s", marked, value);
  if (def_alias_set == NULL)
    def_alias_set = STR_make_stringtab(TABLE_CALLER_ALLOCS, "&alias", TABLE_USE_CASE);
  STR_add_to_stringtab(def_alias_set, k1, v1);
}

void 
defrSetOpenLogFileAppend()
{
  defLogFileAppend = TRUE;
}

void 
defrUnsetOpenLogFileAppend()
{
  defLogFileAppend = FALSE;
}

/* gzip read function set by defzlib for gzip only  */
DEFI_READ_FUNCTION defiReadFunction = 0;

void 
defrSetReadFunction(DEFI_READ_FUNCTION f)
{
  defiReadFunction = f;
}

void 
defrUnsetReadFunction()
{
  defiReadFunction = 0;
}

void 
defrDisablePropStrProcess()
{
  defDisPropStrProcess = 1;
}

void
defrDisableParserMsgs (int nMsg, int* msgs)
{
  int i, j;
  int *tmp;

  if (nDDMsgs == 0) {  /* new list */
     nDDMsgs = nMsg;
     disableDMsgs = (int*)defMalloc(sizeof(int)*nMsg);
     for (i = 0; i < nMsg; i++)
        disableDMsgs[i] = msgs[i];
  } else {  /* add the list to the existing list */
     /* 1st check if the msgId is already on the list before adding it on */
     tmp = (int*)defMalloc(sizeof(int)*(nMsg + nDDMsgs));
     for (i = 0; i < nDDMsgs; i++)  /* copy the existing to the new list */
        tmp[i] = disableDMsgs[i];
     defFree((int*)(disableDMsgs));
     disableDMsgs = tmp;           /* set disableDMsgs to the new list */
     for (i = 0; i < nMsg; i++) {  /* merge the new list with the existing */
        for (j = 0; j < nDDMsgs; j++) {
           if (disableDMsgs[j] == msgs[i])
               break;              /* msgId already on the list */
        }
        if (j == nDDMsgs)           /* msgId not on the list, add it on */
           disableDMsgs[nDDMsgs++] = msgs[i];
     }
  }
  return;
}

void
defrEnableParserMsgs (int nMsg, int* msgs)
{
  int i, j;
  int *tmp;

  if (nDDMsgs == 0)
     return;                       /* list is empty, nothing to remove */

  for (i = 0; i < nMsg; i++) {     /* loop through the given list */
     for (j = 0; j < nDDMsgs; j++) {
        if (disableDMsgs[j] == msgs[i]) {
           disableDMsgs[j] = -1;    /* temp assign a -1 on that slot */
           break;
        }
     }
  }
  /* fill up the empty slot with the next non -1 msgId */
  for (i = 0; i < nDDMsgs; i++) {
     if (disableDMsgs[i] == -1) {
        j = i + 1;
        while (j < nDDMsgs) {
           if (disableDMsgs[j] != -1)
              disableDMsgs[i++] = disableDMsgs[j++];
        }
        break;     /* break out the for loop, the list should all moved */
     }
  }
  /* Count how many messageId left and change all -1 to 0 */
  for (j = 0; j < nDDMsgs; j++) {
     disableDMsgs[j] = 0;     /* set to 0 */
  }
  nDDMsgs = i;
  return;
}

void
defrEnableAllMsgs ()
{
  nDDMsgs = 0;
  defFree((int*)(disableDMsgs));
  return;
}

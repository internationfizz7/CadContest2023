#ifndef DEFLEF_PARSER_H_INCLUDED
#define DEFLEF_PARSER_H_INCLUDED

#include "DumpFile.h"
#include "structure.h"
#include "lefrReader.hpp"
#include "defrReader.hpp"


#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <limits>

void clear_para_deflef();
void ReadDefFile_unit( string filename );
void ReadLefFile(string filename);
void ReadDefFile( string filename);
void Load_pl(string filename);
//lef
int LEF_macroObsCB( lefrCallbackType_e c, lefiObstruction* obs, lefiUserData ud );
int LEF_LayerCB(lefrCallbackType_e c, lefiLayer* layer, lefiUserData ud);
int LEF_macroCB( lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud );
int LEF_pinCB(lefrCallbackType_e c, lefiPin* pin, lefiUserData ud);
bool Multi_lef(string filename);

int DEF_UnitCB(defrCallbackType_e typ, double d, defiUserData ud);
int DEF_diearea (defrCallbackType_e c, defiBox* diebox, defiUserData ud);
int DEF_compoentstartCB(defrCallbackType_e type,int numcompoent,defiUserData userData);
int DEF_componentCB (defrCallbackType_e type,defiComponent* compInfo,defiUserData userData);
int DEF_pinCB (defrCallbackType_e type,defiPin* pinInfo,defiUserData userData) ;
int DEF_netsCB(defrCallbackType_e c, defiNet* net, defiUserData ud);
int compoentstartCB(defrCallbackType_e type,int numcompoent,defiUserData userData);
int DEF_netstart(defrCallbackType_e type,int netnumber,defiUserData userData);

int Def_blockageCB ( defrCallbackType_e c ,defiBlockage* block ,defiUserData ud );
void Dummy_Blockage();
void Call_deflef_parser(string file_name);

/*******plus 107.09.17**********/
void datapath_parser(int argc, char* argv[]);
void Check_GP_cluster(int argc, char* argv[]);
void GP_cluster_parser(int argc, char* argv[]);
/*******************************/
/*******plus 108.07.14**********/
int DEF_trackCB(defrCallbackType_e type, defiTrack* trackInfo, defiUserData userData);
/*******************************/
/*******plus 110.03.04**********/
int DEF_gcellCB(defrCallbackType_e type, defiGcellGrid* gcellInfo, defiUserData userData);
void ReadTechFile(string filename);
/*******************************/
void Macro_gp_location(int argc, char* argv[]);

#endif // DEFLEF_PARSER_H_INCLUDED

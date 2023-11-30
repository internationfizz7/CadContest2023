#ifndef DUMPFILE_H_INCLUDED
#define DUMPFILE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include "structure.h"
#include "dataType.h"

using namespace std;

//for output info and netlist

extern int MacroNum;
extern int StdNum;
extern int PreNum;
void clear_para_dump();
void Output_LEF_info();
void Output_info();
void Output_netlist();
void Output_modDEF();
void Output_tcl(vector<Macro*> &MacroClusterSet, string Filename, unsigned int STDXC, unsigned int STDYC);
string MacroOrientation(Macro* macro_temp, int STDXC, int STDYC, int x, int y);

void WriteDefFile(vector<Macro*> &MacroClusterSet, string OutputName, unsigned int STDXC, unsigned int STDYC, string filename, bool SoftBlockage,NETLIST & nt);      ///modify 108.03.26
void WriteDefFilewithCell();

void OutputTCL_withcell(string Filename, unsigned int STDXC, unsigned int STDYC);


#endif // DUMPFILE_H_INCLUDED

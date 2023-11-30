
#ifndef SOLVECPLEX_H_INCLUDED
#define SOLVECPLEX_H_INCLUDED
#include "dataType.h"
#include <ilcplex/ilocplex.h>

int solveCPLEX3D(vector<int> nVertex3D,vector<vector <MODULE*> > module3,vector<vector < vector<int> > > matricesH3D,vector< vector < vector<int> > > matricesV3D,NETLIST nt);


#endif // SOLVECPLEX_H_INCLUDED

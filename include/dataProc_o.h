/*!
 * \file	structure.cpp
 * \brief	data structure Parser
 *
 * \author	NCKU SEDA LAB PYChiu
 * \version	3.1
 * \date	2016.02.05
 */
#ifndef _DATAPROC_H__o
#define _DATAPROC_H__o

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "structure_o.h"

#define AMP_PARA 1			///< constant to enlarge data (avoid rounding problem)
#define HARD_BLK '0'			///< hard module
#define SOFT_BLK '1'			///< soft module

using namespace std;

void ReadCadcontestfile_o( char *bench, NETLIST_o &nt, Lib_o &lib, DIE_o &die);

#endif

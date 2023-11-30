/*!
 * \file	structure.h
 * \brief	data structures (header)
 *
 * \author	NCKU SEDA LAB
 * \version	1.0
 * \date	2015.04.05
 */

#ifndef _DATATYPE_H__o
#define _DATATYPE_H__o

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#define FILELEN 100
#define MAX_INT 2147483647
#define MAX_FLT 3e+20
#define FIXED 0
#define MOVABLE -1

using namespace std;

class DIE_o;
class PIN_o;
class NET_o;
class Libcell_o;
class Lib_o;
class Block_o;
class MODULE_o;
class NETLIST_o;
class Solution_o;

class DIE_o
{
	friend ostream& operator<<(ostream &output, const DIE_o &die);
	public:
		int Width;
		int Height;
		long long DieSize;
		int TopUtil;
		int BotUtil;
		int TopRowLen;
		int BotRowLen;
		int TopRowH;
		int BotRowH;
		int TopRowCount;
		int BotRowCount;

		DIE_o();
		~DIE_o();
};

class PIN_o
{
	friend ostream& operator<<(ostream &output, const PIN_o &pin);
	public:
		char name[50];
		int  id;
		int  corr_id;
		int  offset_x;
		int  offset_y;

		PIN_o();
		~PIN_o();
};

class NET_o
{
	friend ostream& operator<<(ostream &output, const NET_o &net);
	public:
		char name[50];
		int  id;
		int  head;
		int  degree;		
		
		int nModonTop;
		int nModonBot;
		bool OnCutline;
		int num_mod;
		vector<int> connect_mod;

		int nTSV; // no need				///< number of TSVs

		NET_o();
		~NET_o();
};

class Libcell_o
{
	public:
		char name[50];
		int id;
		int w0; 				// TOP
		int h0;
		long long a0;
		int w1;					// BOTTOM
		int h1;
		long long a1;
		int p;
		bool is_macro;
		vector<PIN_o> lib_pin;	// first half for top, second half for bot
		map<string, int> pin_NameToID;

		Libcell_o();
		~Libcell_o();
};

class Lib_o  //4/26
{
	friend ostream& operator<<(ostream &output, const Lib_o &lib);
	public:
		vector<Libcell_o> lib_cell;
		map<string, int> lib_NameToID;
		string Techname[2];
		int num_tech;

		void CopySpec(int MODE);

		Lib_o();
		~Lib_o();
};

class Block_o
{
	public:
		int gain;
		bool lock;
		MODULE_o *mymod;
		Block_o *prebck;
		Block_o *nexbck;
		
		Block_o();
		~Block_o();
};

class MODULE_o
{
	friend ostream& operator<<(ostream &output, const MODULE_o &module);
	public:
		char name[50];
		int mod_w; //4/26
		int mod_h; //4/26
		int id;
		int cell_id;		
		int tier;
		int area;		
		bool is_macro;

		int num_net;
		int *array_net;

		int type;
		int angle;	// 0 -> 0, 1 -> 90, 2 -> 180, 3 -> 270

		int nLayer; ///f///      ///< number of layers the stacked module with; 1: not stack, 2~: stack (different with nt.nLayer)
		
		Block_o *mybck;
		
		MODULE_o& operator=(const MODULE_o &);	

		MODULE_o();
		~MODULE_o();
};

class NETLIST_o
{
	friend ostream& operator<<(ostream &output, const NETLIST_o &netlist);
	public:
		int num_mod;
		vector<MODULE_o> mods;
		map<string, int> mod_NameToID;

		int num_net;
		vector<NET_o> nets;

		int num_pin;
		vector<PIN_o> pins;
		
		int nTSV;				///< number of TSVs
		int Layer;

		int nHard;
		int nSoft;
		int nMacro;
		long long TopArea, BotArea;

		NETLIST_o();
		~NETLIST_o();
};

class Solution_o:public NETLIST_o{
	//class Solution{
	public:
		vector<MODULE_o> tmp_node;
	//    int nTSV;
		double cost;
		int index;
		Solution_o *next;
		Solution_o *pre;
	//    void Area();
	//    Solution& operator=(const Solution &);
		
		Solution_o();
		~Solution_o();
};

extern int format_o;
extern int metis_nparts_o;
extern int nOutput_o;

extern int max_mod_area_o;
// extern double max_WS;
// extern unsigned short *buffNet;

extern int maxpin_o;
extern int maxarea_o;

extern int Tsize_o;		///< TerminalSize 23/6/28
extern int Tspace_o;		///< TerminalSpacing 23/6/28
extern int Tcost_o;		///< TerminalCost 23/6/28

extern double r_o;		///< Partition ratio 23/7/3

extern long long AreaUpperlimit_o;		///< Area Upperlimit 23/8/29

#endif

/*!
 * \file	structure.cpp
 * \brief	data structure
 *
 * \author	NCKU SEDA LAB
 * \version	1.0
 * \date	2015.04.05
 */

#include <cstring>

using namespace std;

#include "structure_o.h"

DIE_o::DIE_o(): Width(0), Height(0), DieSize(0), TopUtil(100), BotUtil(100), TopRowLen(0), BotRowLen(0), TopRowH(0), BotRowH(0), TopRowCount(0), BotRowCount(0)
{ }
DIE_o::~DIE_o()
{ }

ostream& operator<<(ostream &output, const DIE_o &die)
{
	output << "***** Check Die data *****" << endl;
	output << "-Width of Die            :" << die.Width << endl;
	output << "-Height of Die           :" << die.Height << endl;
	output << "-Size of Die             :" << die.DieSize << endl;
	output << "-Util of TopDie          :" << die.TopUtil << endl;
	output << "-Util of BotDie          :" << die.BotUtil << endl;
	output << "-Row Length of TopDie    :" << die.TopRowLen << endl;
	output << "-Row Length of BotDie    :" << die.BotRowLen << endl;
	output << "-Row Height of TopDie    :" << die.TopRowH << endl;
	output << "-Row Height of BotDie    :" << die.BotRowH << endl;
	output << "-# of Row on TopDie      :" << die.TopRowCount << endl;
	output << "-# of Row on BotDie      :" << die.BotRowCount << endl;
	return output;
}

PIN_o::PIN_o(): id(-1), corr_id(-1), offset_x(0), offset_y(0)
{
	strcpy(name, "\0");
}
PIN_o::~PIN_o()
{ }

ostream& operator<<(ostream &output, const PIN_o &pin)
{
	output << "***** Check PIN data *****" << endl;
	output << "name                                 :" << pin.name << endl;	
	output << "ID                                   :" << pin.id << endl;
	output << "Belong to the module which index is  :" << pin.corr_id << endl;
	output << "offset_x                             :" << pin.offset_x << endl;
	output << "offset_y                             :" << pin.offset_y << endl;
	return output;
}

NET_o::NET_o(): id(-1), head(-1), degree(0), nModonTop(0), nModonBot(0), OnCutline(false), num_mod(0), nTSV(0)
{
	strcpy(name, "\0");
}
NET_o::~NET_o()
{ }

ostream& operator<<(ostream &output, const NET_o &net)
{
	output << "***** Check NET data *****" << endl;
	output << "name                  :" << net.name << endl;	
	output << "ID                    :" << net.id << endl;
	output << "Pin head              :" << net.head << endl;
	output << "degree                :" << net.degree << endl;
	output << "# of mod on top       :" << net.nModonTop << endl;
	output << "# of mod on bot       :" << net.nModonBot << endl;
	output << "On cut line?          :" << (net.OnCutline?"Y":"N") << endl;
	output << "number of connect mod :" << net.num_mod << endl;
	output << "connect module        :";
	/*for(auto cm : net.connect_mod){
		output << cm << " ";
	}*/
	output << endl;
	output << "number of TSV         :" << net.nTSV << endl;

	return output;
}

Libcell_o::Libcell_o(): id(-1), w0(0), h0(0), a0(0), w1(0), h1(0), a1(0), p(0), is_macro(false)
{ 
	strcpy(name, "\0");
}
Libcell_o::~Libcell_o()
{ }

Lib_o::Lib_o(): num_tech(0)
{ }
Lib_o::~Lib_o()
{ }

void Lib_o::CopySpec(int MODE)
{
	switch(MODE)
	{
		case 1:						// Copy Second Tech to First Tech
			for(unsigned int i = 0;i < lib_cell.size();i++){
				lib_cell[i].w1 = lib_cell[i].w0;
				lib_cell[i].h1 = lib_cell[i].h0;
				lib_cell[i].a1 = lib_cell[i].a0;
				for(int j = 0;j < lib_cell[i].p;j++){
					lib_cell[i].lib_pin[j+lib_cell[i].p].offset_x = lib_cell[i].lib_pin[j].offset_x;
					lib_cell[i].lib_pin[j+lib_cell[i].p].offset_y = lib_cell[i].lib_pin[j].offset_y;
				}
			}
			break;
		case 2:						// Copy First Tech to Second Tech
			for(unsigned int i = 0;i < lib_cell.size();i++){
				lib_cell[i].w0 = lib_cell[i].w1;
				lib_cell[i].h0 = lib_cell[i].h1;				
				lib_cell[i].a0 = lib_cell[i].a1;
				for(int j = 0;j < lib_cell[i].p;j++){
					lib_cell[i].lib_pin[j].offset_x = lib_cell[i].lib_pin[j+lib_cell[i].p].offset_x;
					lib_cell[i].lib_pin[j].offset_y = lib_cell[i].lib_pin[j+lib_cell[i].p].offset_y;
				}
			}
			break;
		case 3:						// Exchange Tech
			long long temp;
			for(unsigned int i = 0;i < lib_cell.size();i++){
				temp = lib_cell[i].w0;
				lib_cell[i].w0 = lib_cell[i].w1;
				lib_cell[i].w1 = temp;
				temp = lib_cell[i].h0;
				lib_cell[i].h0 = lib_cell[i].h1;
				lib_cell[i].h1 = temp;
				temp = lib_cell[i].a0;
				lib_cell[i].a0 = lib_cell[i].a1;
				lib_cell[i].a1 = temp;
				for(int j = 0;j < lib_cell[i].p;j++){
					temp = lib_cell[i].lib_pin[j].offset_x;
					lib_cell[i].lib_pin[j].offset_x = lib_cell[i].lib_pin[j+lib_cell[i].p].offset_x;
					lib_cell[i].lib_pin[j+lib_cell[i].p].offset_x = temp;
					temp = lib_cell[i].lib_pin[j].offset_y;
					lib_cell[i].lib_pin[j].offset_y = lib_cell[i].lib_pin[j+lib_cell[i].p].offset_y;
					lib_cell[i].lib_pin[j+lib_cell[i].p].offset_y = temp;
				}
			}
			break;
		default:
			cout << "Error  : MODE " << MODE << " doesnt's exist!" << endl;
			exit(EXIT_FAILURE);
			break;
	}	
}

ostream& operator<<(ostream &output, const Lib_o &lib)
{
	output << "***** Check Lib data *****" << endl;
	output << "-Number of Tech          :" << lib.num_tech << endl;
	output << "-Number of Libcell       :" << lib.lib_cell.size() << endl;
	/*for(auto l : lib.lib_cell){
		output << "***** Check Libcell data *****" << endl;
		output << "LibCell          :" << l.name << endl;
		output << "ID               :" << l.id << endl;
		output << "W/H on top       :" << l.w0 << "/" << l.h0 << endl;
		output << "W/H on bot       :" << l.w1 << "/" << l.h1 << endl;		
		output << "Area on top/bot  :" << l.a0 << "/" << l.a1 << endl;
		output << "# of pins        :" << l.p << endl;
		output << "Macro?           :" << (l.is_macro?"Y":"N") << endl;
		for(auto lp : l.lib_pin){
			output << lp << endl;
		}
		
		for(auto MAP : l.pin_NameToID){
			output << MAP.first << "->" << MAP.second << endl;
		}
	}*/
	
	output << "*************************************" << endl;
	output << "-NameToID                :" << endl;
	/*for(auto MAP : lib.lib_NameToID){
		output << MAP.first << "->" << MAP.second << endl;
	}*/
	output << "*************************************" << endl;
	output << "-Sequence of technology	: " << endl;
	/*for(auto name : lib.Techname){
		output << name << " ";
	}*/
	output << endl;
	return output;
}

Block_o::Block_o() : gain(0), lock(false), mymod(NULL), prebck(NULL), nexbck(NULL)
{ }
Block_o::~Block_o()
{ }

MODULE_o::MODULE_o(): mod_w(0), mod_h(0), id(-1), cell_id(-1), tier(-1), area(0), is_macro(false), num_net(0), array_net(NULL), type(0), angle(0), nLayer(1), mybck(NULL)
{
	strcpy(name, "\0");
}
MODULE_o::~MODULE_o()
{ }

ostream& operator<<(ostream &output, const MODULE_o &module)
{
	output << "***** Check MODULE data *****" << endl;
	output << "name             :" << module.name << endl;
	output << "W/H              :" << module.mod_w << "/" << module.mod_h << endl;
	output << "area             :" << module.area << endl;
	output << "ID               :" << module.id << endl;
	output << "library ID       :" << module.cell_id << endl;
	output << "layer            :" << module.tier << endl;
	output << "number of nets   :" << module.num_net << endl;
	output << "angle            :Degree " <<  module.angle*90 << endl;
	output << "Macro?           :" << (module.is_macro?"Y":"N") << endl;
	return output;
}

MODULE_o& MODULE_o::operator=(const MODULE_o &module)
{
	if(&module != this)
	{
		strcpy(name, module.name);
		mod_w = module.mod_w;
		mod_h = module.mod_h;
		cell_id = module.cell_id;
		id = module.id;
		tier = module.tier;
		area = module.area;
		num_net = module.num_net;
		array_net = module.array_net;
		nLayer = module.nLayer;
		type = module.type;
		is_macro = module.is_macro;
	}
	return *this;
}

NETLIST_o::NETLIST_o(): num_mod(0), num_net(0), num_pin(0), nTSV(0), Layer(2), nHard(0), nSoft(0), nMacro(0), TopArea(0), BotArea(0)
{ }
NETLIST_o::~NETLIST_o()
{ }

ostream& operator<<(ostream &output, const NETLIST_o &netlist)
{
	output << "***** Check NETLIST data *****" << endl;
	
	output << "-Number of modules           :" << netlist.num_mod << endl;
	/*for(auto m : netlist.mods){
		output << m;
	}*/
	/*
	output << "-Module to Net               :" << endl;	
	for(int i = 0;i < netlist.num_mod;i++){
		cout << netlist.mods[i].name << " : ";
        for(int j = 0;j < netlist.mods[i].num_net;j++){
            cout << netlist.nets[ netlist.mods[i].array_net[j] ].name << " ";
        }
        cout << endl;        
    }
	
	output << "-NameToID				:" << endl;
	for(auto MAP : netlist.mod_NameToID){
		output << MAP.first << "->" << MAP.second << endl;
	}
	*/
	output << "*************************************" << endl;
	output << "-Number of nets              :" << netlist.num_net << endl;
	/*for(auto n : netlist.nets){
		output << n;
	}*/
	
	output << "*************************************" << endl;
	output << "-Number of pins              :" << netlist.num_pin << endl;
	/*for(auto p : netlist.pins){
		//output << p;
	}*/
	
	output << "*************************************" << endl;
	output << "-Number of TSV               :" << netlist.nTSV << endl;	
	output << "-Number of layers            :" << netlist.Layer << endl;
	
	output << "-(soft/hard)                 :" << netlist.nSoft << "/" << netlist.nHard << endl;
	
	return output;
}

Solution_o::Solution_o(): cost(0) , index(0) , next(NULL) , pre(NULL)
{ }
Solution_o::~Solution_o()
{
    next = NULL;
    pre  = NULL;
    tmp_node.clear();
}

int format_o = 0;
int metis_nparts_o = 0;
int nOutput_o = 0;

int max_mod_area_o = 0;
// double max_WS = 0.0;
// unsigned short *buffNet;

int maxpin_o = 0;
int maxarea_o = 0;

int Tsize_o = 0;		///< TerminalSize 23/6/28
int Tspace_o = 0;		///< TerminalSpacing 23/6/28
int Tcost_o = 0;		///< TerminalCost 23/6/28

double r_o;		///< Partition ratio 23/7/3

long long AreaUpperlimit_o;		///< Area Upperlimit 23/8/29

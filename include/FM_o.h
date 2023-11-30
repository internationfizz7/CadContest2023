#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

#include "structure_o.h"

class Partitioning{ //main data structure
	public:    	
    	vector<Block_o> bcks, SetT, SetB;
		vector<int> macroindex;
		
		string outfile;
		int maxgain;
    	int LastSelTier;
		long long TOPbound, BOTbound;
		long long sizeT, sizeB;
    	int cutsize;
		int minsize;
		int step;
    	Block_o *AgainMax,*BgainMax;
		bool Legal;
		bool M_partition;
				
    	Partitioning(){
			outfile = "Out.out";
			maxgain = 0;
			LastSelTier = 0;
			TOPbound = 0;
			BOTbound = 0;
			sizeT = 0;
			sizeB = 0;
			cutsize = 0;
			minsize = 0;
			step = -1;
			AgainMax = NULL;
			BgainMax = NULL;
			Legal = false;
			M_partition = false;
		}
    	    	
		void changelastSel(int i = 1);
    	void construct(NETLIST_o &nt);		
		int selectorA(NETLIST_o &nt, Lib_o &lib);
		void schedule(NETLIST_o &nt);   	
		void schedule_cell(NETLIST_o &nt);
		void schedule_macro(NETLIST_o &nt);
		void add_bck(Block_o* movebck, Block_o* toset);
		void resetgainMax();
		void write(NETLIST_o &nt);
		void Move(NETLIST_o &nt, Lib_o &lib, int dotimes = -1);
		int selectorM(NETLIST_o &nt, Lib_o &lib);
		void update(NETLIST_o &nt, Block_o* bptr);
		void delete_bck(Block_o* movebck);
		void FM(NETLIST_o &nt, Lib_o &lib, DIE_o &die);
    	void show(NETLIST_o &nt, Lib_o &lib);
		void conclude(NETLIST_o &nt, Lib_o &lib);
};
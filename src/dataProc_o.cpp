/*!

 * \file	structure.cpp
 * \brief	data structure Parser
 *
 * \author	NCKU SEDA LAB PYChiu
 * \version	3.1
 * \date	2016.02.05
 */
#include <iostream>
#include <fstream>
#include <sstream>

#include "dataProc_o.h"

using namespace std;

void ReadCadcontestfile_o( char *bench, NETLIST_o &nt, Lib_o &lib, DIE_o &die)
{
	ifstream fin;
	fin.open( bench );
    
    bool readtech = false;
	stringstream sstream;
	string strtemp1, strtemp2;
    string booltemp;
    string usetech[2];
    int libsize;
    int inttemp;
    int corrindex1, corrindex2;
    
	while( getline(fin, strtemp1) )
	{
		sstream << strtemp1;
		sstream >> strtemp1;       

		if( strtemp1 == "" || strtemp1.substr(0, 1) == "#" ){
			sstream.clear();
			continue;
		}
        else if( strtemp1 == "NumTechnologies" ){                    // NumTechnologies <technologyCount> 
            sstream >> lib.num_tech;
		    sstream.clear();            
        }
        else if( strtemp1 == "Tech" ){                               // Tech <techName> <libCellCount>            
            if(!readtech){
                sstream >> lib.Techname[0] >> libsize;
                lib.lib_cell.resize(libsize);
                sstream.clear();
                for(int i = 0;i < libsize;i++){                     // LibCell <isMacro> <libCellName> <libCellSizeX> <libCellSizeY> <pinCount> 
                    getline(fin, strtemp1);
                    sstream << strtemp1;
                    sstream >> strtemp1;
                    if(strtemp1 != "LibCell"){
                        i--;
                        sstream.clear();
                        continue;
                    }
                    sstream >> strtemp1;
                    if(strtemp1 == "N") lib.lib_cell[i].is_macro = false;
                    else if(strtemp1 == "Y") lib.lib_cell[i].is_macro = true;
                    else exit(EXIT_FAILURE);
                    sstream >> lib.lib_cell[i].name >> lib.lib_cell[i].w0 >> lib.lib_cell[i].h0 >> lib.lib_cell[i].p;
                    sstream.clear();
                    lib.lib_cell[i].id = i;
                    lib.lib_NameToID[lib.lib_cell[i].name] = i;
                    lib.lib_cell[i].a0 = lib.lib_cell[i].w0;
                    lib.lib_cell[i].a0 *= lib.lib_cell[i].h0;
                    lib.lib_cell[i].lib_pin.resize(2*lib.lib_cell[i].p);
                    for(int j = 0;j < lib.lib_cell[i].p;j++){       // Pin <pinName> <pinLocationX> <pinLocationY> 
                        getline(fin, strtemp1);
                        sstream << strtemp1;
		                sstream >> strtemp1;
                        if(strtemp1 != "Pin"){
                            j--;
                            sstream.clear();
                            continue;
                        }
                        sstream >> lib.lib_cell[i].lib_pin[j].name >> lib.lib_cell[i].lib_pin[j].offset_x >> lib.lib_cell[i].lib_pin[j].offset_y;
                        sstream.clear();
                        lib.lib_cell[i].lib_pin[j].id = j;
                        lib.lib_cell[i].lib_pin[j+lib.lib_cell[i].p].id = j+lib.lib_cell[i].p;
                        strcpy(lib.lib_cell[i].lib_pin[j+lib.lib_cell[i].p].name, lib.lib_cell[i].lib_pin[j].name);
                        lib.lib_cell[i].pin_NameToID[lib.lib_cell[i].lib_pin[j].name] = j;
                    }
                }
                readtech = true;
            }
            else{
                sstream >> lib.Techname[1] >> libsize;
                sstream.clear();
                for(int i = 0;i < libsize;i++){
                    getline(fin, strtemp1);
                    sstream << strtemp1;
                    sstream >> strtemp1;
                    if(strtemp1 != "LibCell"){
                        i--;
                        sstream.clear();
                        continue;
                    }
                    sstream >> booltemp >> strtemp1;
                    corrindex1 = lib.lib_NameToID[strtemp1];                    
                    sstream >> lib.lib_cell[corrindex1].w1 >> lib.lib_cell[corrindex1].h1 >> inttemp;
                    sstream.clear();
                    lib.lib_cell[i].a1 = lib.lib_cell[i].w1;
                    lib.lib_cell[i].a1 *= lib.lib_cell[i].h1;
                    for(int j = 0;j < lib.lib_cell[corrindex1].p;j++){
                        getline(fin, strtemp1);
                        sstream << strtemp1;
		                sstream >> strtemp1;
                        if(strtemp1 != "Pin"){
                            j--;
                            sstream.clear();
                            continue;
                        }
                        sstream >> strtemp1;                        
                        corrindex2 = lib.lib_cell[corrindex1].pin_NameToID[strtemp1] + lib.lib_cell[corrindex1].p;                        
                        sstream >> lib.lib_cell[corrindex1].lib_pin[corrindex2].offset_x >> lib.lib_cell[corrindex1].lib_pin[corrindex2].offset_y;
                        sstream.clear();
                    }
                }
            }            
        }
        else if( strtemp1 == "DieSize" ){                            // DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY> 
            sstream >> inttemp >> inttemp >> die.Width >> die.Height;
            die.DieSize = die.Width;
            die.DieSize *= die.Height;
            sstream.clear();              
        }
        else if( strtemp1 == "TopDieMaxUtil" ){                      // TopDieMaxUtil <util> 
            sstream >> die.TopUtil;
			sstream.clear();
        }
        else if( strtemp1 == "BottomDieMaxUtil" ){                   // BottomDieMaxUtil <util> 
            sstream >> die.BotUtil;
			sstream.clear();
        }
        else if( strtemp1 == "TopDieRows" ){                         // TopDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount> 
            sstream >> inttemp >> inttemp >> die.TopRowLen >> die.TopRowH >> die.TopRowCount;
			sstream.clear();
        }
        else if( strtemp1 == "BottomDieRows" ){                      // BottomDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount> 
            sstream >> inttemp >> inttemp >> die.BotRowLen >> die.BotRowH >> die.BotRowCount;
			sstream.clear();
        }
        else if( strtemp1 == "TopDieTech" ){                         // TopDieTech <TechName> 
            if(lib.num_tech == 1){
                lib.CopySpec(1);
                sstream >> strtemp1;
            }
            else{
                sstream >> usetech[0];
            }
			sstream.clear();            
        }
        else if( strtemp1 == "BottomDieTech" ){                      // BottomDieTech <TechName> 
            if(lib.num_tech != 1){
                sstream >> usetech[1];
                if(usetech[0] != lib.Techname[0] && usetech[1] != lib.Techname[1]){
                    lib.CopySpec(3);
                }
                else if(usetech[0] == lib.Techname[0] && usetech[1] != lib.Techname[1]){
                    lib.CopySpec(1);
                }
                else if(usetech[0] != lib.Techname[0] && usetech[1] == lib.Techname[1]){
                    lib.CopySpec(2);
                }
            }
            else{
                sstream >> strtemp1;
            }
			sstream.clear();
        }
        else if( strtemp1 == "TerminalSize" ){                       // TerminalSize <sizeX> <sizeY> 
			sstream >> Tsize_o >> strtemp1;
			sstream.clear();
		}
		else if( strtemp1 == "TerminalSpacing" ){                    // TerminalSpacing <spacing> 
			sstream >> Tspace_o;
			sstream.clear();
		}
		else if( strtemp1 == "TerminalCost" ){                       // TerminalCost <val>
			sstream >> Tcost_o;
			sstream.clear();
		}
		else if( strtemp1 == "NumInstances" ){                       // NumInstances <instanceCount> 
			sstream >> nt.num_mod;
			nt.mods.resize(nt.num_mod);
			sstream.clear();
            
            nt.nHard = nt.num_mod;

            int M_count = 0;
            for(int i = 0;i < nt.num_mod;i++){
                getline(fin, strtemp1);
                sstream << strtemp1;
                sstream >> strtemp1;
                if(strtemp1 != "Inst"){                              // Inst <instName> <libCellName> 
                    i--;
                    sstream.clear();
                    continue;
                }
                sstream >> nt.mods[i].name >> strtemp1;
                nt.mods[i].id = i;
                nt.mods[i].cell_id = lib.lib_NameToID[strtemp1];
                nt.mods[i].is_macro = lib.lib_cell[nt.mods[i].cell_id].is_macro;
                if(nt.mods[i].is_macro){
                    M_count++;
                    nt.mods[i].tier = M_count%2;
                }
                else{
                    nt.mods[i].tier = (i < nt.num_mod/2? 0 : 1);                    
                }
                nt.mods[i].mod_w = (1-nt.mods[i].tier)*lib.lib_cell[nt.mods[i].cell_id].w0 + nt.mods[i].tier*lib.lib_cell[nt.mods[i].cell_id].w1;
                nt.mods[i].mod_h = (1-nt.mods[i].tier)*lib.lib_cell[nt.mods[i].cell_id].h0 + nt.mods[i].tier*lib.lib_cell[nt.mods[i].cell_id].h1;
                nt.mods[i].area = nt.mods[i].mod_w * nt.mods[i].mod_h;
                if(nt.mods[i].area > max_mod_area_o) max_mod_area_o = nt.mods[i].area;                
                if(nt.mods[i].tier){
                    nt.BotArea += nt.mods[i].area;
                }
                else{
                    nt.TopArea += nt.mods[i].area;
                }
                nt.mod_NameToID[nt.mods[i].name] = i;

                sstream.clear();
            }
		}        
		else if( strtemp1 == "NumNets" ){                            // NumNets <netCount> 
            PIN_o pintemp;

            sstream >> nt.num_net;
            nt.nets.resize(nt.num_net);
			sstream.clear();
            
            for(int i = 0;i < nt.num_net;i++){                
                getline(fin, strtemp1);
                sstream << strtemp1;
                sstream >> strtemp1;
                if(strtemp1 != "Net"){                               // Net <netName> <numPins> 
                    i--;
                    sstream.clear();
                    continue;
                }
                sstream >> nt.nets[i].name >> nt.nets[i].degree;
                sstream.clear();
                nt.nets[i].id = i;
                nt.nets[i].head = nt.num_pin;
                for(int j = 0;j < nt.nets[i].degree;j++){           // Pin <instName>/<libPinName> 
                    getline(fin, strtemp1);
                    sstream << strtemp1;
                    sstream >> strtemp1;
                    if(strtemp1 != "Pin"){
                        j--;
                        sstream.clear();
                        continue;
                    }
                    sstream >> strtemp1;
                    sstream.clear();
                    strcpy(pintemp.name, strtemp1.c_str());
                    strtemp2 = strtemp1.substr(strtemp1.find("/") + 1, strtemp1.length());
                    strtemp1 = strtemp1.substr(0, strtemp1.find("/"));
                    pintemp.corr_id = nt.mod_NameToID[strtemp1];
                    pintemp.id = nt.num_pin;
                    inttemp = nt.mods[pintemp.corr_id].cell_id;
                    
                    nt.pins.push_back(pintemp);
                    nt.mods[pintemp.corr_id].num_net++;
                    nt.num_pin++;
                }
            }
	    }        
        else{
			sstream.clear();
		}
    }
    
    /// allocate memory for nets which link to mod[index] (mod[index].array_net)
    for(int i = 0;i < nt.num_mod;i++){
        try{
            nt.mods[i].array_net = new int [nt.mods[i].num_net];
            //if(nt.mods[i].num_net>maxpin)
            //maxpin=nt.mods[i].num_net;            
        }
        catch( bad_alloc &bad ){
            cout << "Error  : Run out of memory on nt.mods[" << nt.mods[i].id << "].array_net" << endl;
            exit(EXIT_FAILURE);
        }
        nt.mods[i].num_net = 0;
    }
    
    /// construct the relationship between nets and modules
    for(int i = 0;i < nt.num_net;i++){
        for(int j = nt.nets[i].head;j < nt.nets[i].head + nt.nets[i].degree;j++){
            nt.mods[ nt.pins[j].corr_id ].array_net[ nt.mods[ nt.pins[j].corr_id ].num_net ] = nt.nets[i].id;
            nt.mods[ nt.pins[j].corr_id ].num_net++;
        }
    }

    /// record the modules connect to the net without repeating and wheather the net is on the cutline
	int onlinecount = 0;
	vector<int> corr_mod;
	vector<int>::iterator iter;
    for(int i = 0;i < nt.num_net;i++){
		nt.nets[i].OnCutline = false;		
		for(int j = 0;j < nt.nets[i].degree;j++){
			if(nt.mods[nt.pins[nt.nets[i].head].corr_id].tier != nt.mods[nt.pins[nt.nets[i].head+j].corr_id].tier){
				nt.nets[i].OnCutline = true;
			}

			iter = find(corr_mod.begin(), corr_mod.end(), nt.mods[nt.pins[nt.nets[i].head+j].corr_id].id);
			if(iter == corr_mod.end()){
				corr_mod.push_back(nt.mods[nt.pins[nt.nets[i].head+j].corr_id].id);
				if(nt.mods[nt.pins[nt.nets[i].head+j].corr_id].tier == 0){
					nt.nets[i].nModonTop++;
				}
				else if(nt.mods[nt.pins[nt.nets[i].head+j].corr_id].tier == 1){
					nt.nets[i].nModonBot++;
				}
			}
		}
		if(nt.nets[i].OnCutline) onlinecount++;
        nt.nets[i].connect_mod = corr_mod;
        nt.nets[i].num_mod = nt.nets[i].connect_mod.size();
		corr_mod.clear();
	}
    nt.nTSV = onlinecount;
}

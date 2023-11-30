#include <iostream>

#include "FM_o.h"

using namespace std;

void Partitioning::changelastSel(int Select){ // record last select tier 
	if(Select == 0) LastSelTier = 0;
	else if(Select == 1) LastSelTier = 1;
	else cout << "Unexpected select\n";

	return;
}

void Partitioning::construct(NETLIST_o &nt){ // construct the bucket list
	for (size_t i = 0; i < nt.mods.size(); i++) {
        if (nt.mods[i].num_net > maxgain) {
            maxgain = nt.mods[i].num_net;
        }
		if(nt.mods[i].is_macro){
			macroindex.push_back(nt.mods[i].id);
			nt.nMacro++;
		}
    }
	SetT.resize(2*maxgain + 1);
	SetB.resize(2*maxgain + 1);
	for(int i = 0;i < 2*maxgain + 1;++i){
		SetT[i].gain = -(i - maxgain);
		SetB[i].gain = -(i - maxgain);
	}
	
	bcks.resize(nt.num_mod);
	LastSelTier = 0;
	for(int i = 0;i < nt.num_mod;++i){
		nt.mods[i].mybck = &bcks[i];
		bcks[i].mymod = &nt.mods[i];
	}

	for(int i = 0;i < nt.num_net;i++){
		if(nt.nets[i].nModonTop > 1 && nt.nets[i].nModonBot > 1) continue;

		if(nt.nets[i].nModonTop == 1){
			for(int j = 0;j < nt.nets[i].num_mod;j++){
				if(nt.mods[nt.nets[i].connect_mod[j]].tier == 0){
					nt.mods[nt.nets[i].connect_mod[j]].mybck->gain++;
					break;
				}
			}
		}
		else if(nt.nets[i].nModonTop == 0){
			for(int j = 0;j < nt.nets[i].num_mod;j++){				
				nt.mods[nt.nets[i].connect_mod[j]].mybck->gain--;
			}
		}

		if(nt.nets[i].nModonBot == 1){
			for(int j = 0;j < nt.nets[i].num_mod;j++){
				if(nt.mods[nt.nets[i].connect_mod[j]].tier == 1){
					nt.mods[nt.nets[i].connect_mod[j]].mybck->gain++;
					break;
				}
			}
		}
		else if(nt.nets[i].nModonBot == 0){
			for(int j = 0;j < nt.nets[i].num_mod;j++){				
				nt.mods[nt.nets[i].connect_mod[j]].mybck->gain--;
			}
		}
	}

	for(int i = 0;i < nt.num_mod;i++){
		nt.mods[i].mybck->lock = true;
	}

	cutsize = nt.nTSV;
	minsize = nt.nTSV;
	
	return;
}

int Partitioning::selectorA(NETLIST_o &nt, Lib_o &lib){ //determine the tier of the module initially
	int choosetier = -1;
	int LibC_T, LibC_B;

	if(AgainMax && BgainMax){
		LibC_T = AgainMax->nexbck->mymod->cell_id;
		LibC_B = BgainMax->nexbck->mymod->cell_id;
		if(sizeT + lib.lib_cell[LibC_B].a0 > TOPbound) choosetier = 0;
		else if(sizeB + lib.lib_cell[LibC_T].a1 > BOTbound) choosetier = 1;
		else{			
			if(AgainMax->gain > BgainMax->gain) choosetier = 0;
			else if(AgainMax->gain < BgainMax->gain) choosetier = 1;
			else{
				if((lib.lib_cell[LibC_T].a0 - lib.lib_cell[LibC_T].a1 > lib.lib_cell[LibC_B].a1 - lib.lib_cell[LibC_B].a0) & 
				   (lib.lib_cell[LibC_T].a0 > lib.lib_cell[LibC_T].a1)
				) choosetier = 0;
				else if((lib.lib_cell[LibC_T].a0 - lib.lib_cell[LibC_T].a1 < lib.lib_cell[LibC_B].a1 - lib.lib_cell[LibC_B].a0) & 
				   		(lib.lib_cell[LibC_B].a1 > lib.lib_cell[LibC_B].a0)	
				) choosetier = 1;
				else{
					if(sizeT/TOPbound - sizeB/BOTbound > 0.1) choosetier = 0;
					else if(sizeB/BOTbound - sizeT/TOPbound > 0.1) choosetier = 1;
					else{
						if(LastSelTier == 0) choosetier = 1;
						else choosetier = 0;
					}
				}			
			}
		}
	}
	else if(!AgainMax && BgainMax) choosetier = 1;	
	else if(AgainMax && !BgainMax) choosetier = 0;
	else{
		return -1;
	}
	changelastSel(choosetier);

	return choosetier;
}

void Partitioning::schedule(NETLIST_o &nt){ // put all the locked mmodule onto the bucket list
    int setindex;
	
	for(int i = 0;i < nt.num_mod;++i){
		if(bcks[i].lock){
			bcks[i].lock = false;

			setindex = -bcks[i].gain + maxgain;
			
			if(bcks[i].mymod->tier == 0){
				add_bck(&bcks[i],&SetT[setindex]);
			}
			else if(bcks[i].mymod->tier == 1){
				add_bck(&bcks[i],&SetB[setindex]);
			}
			else{
				cout << "Area Wrong" << endl;
				break;
			}
		}
	}
	
	resetgainMax();
	
	return;
}

void Partitioning::schedule_cell(NETLIST_o &nt){ // put all the locked mmodule onto the bucket list
    int setindex;
	
	for(int i = 0;i < nt.num_mod;++i){
		if(bcks[i].lock && !nt.mods[i].is_macro){
			bcks[i].lock = false;

			setindex = -bcks[i].gain + maxgain;
			
			if(bcks[i].mymod->tier == 0){
				add_bck(&bcks[i],&SetT[setindex]);
			}
			else if(bcks[i].mymod->tier == 1){
				add_bck(&bcks[i],&SetB[setindex]);
			}
			else{
				cout << "Area Wrong" << endl;
				break;
			}
		}
	}
	
	resetgainMax();
	
	return;
}

void Partitioning::schedule_macro(NETLIST_o &nt){ // put all the locked mmodule onto the bucket list
    int setindex, m_index;
	
	for(int i = 0;i < nt.nMacro;++i){
		m_index = macroindex[i];
		if(bcks[m_index].lock && nt.mods[m_index].is_macro){
			bcks[m_index].lock = false;

			setindex = -bcks[m_index].gain + maxgain;
			
			if(bcks[m_index].mymod->tier == 0){
				add_bck(&bcks[m_index],&SetT[setindex]);
			}
			else if(bcks[i].mymod->tier == 1){
				add_bck(&bcks[m_index],&SetB[setindex]);
			}
			else{
				cout << "Area Wrong" << endl;
				break;
			}
		}
	}
	
	resetgainMax();

	M_partition = true;
	
	return;
}

void Partitioning::add_bck(Block_o* movebck, Block_o* toset){ // add the block onto corresponding place
	movebck->prebck = toset;
	movebck->nexbck = toset->nexbck;
	if(toset->nexbck) toset->nexbck->prebck = movebck;
	toset->nexbck = movebck;
	
	return;
}

void Partitioning::resetgainMax(){ // reset the max gain pointer
	AgainMax = NULL;
	BgainMax = NULL;
	for(int i = 0;i < 2*maxgain + 1;i++){
		if(!SetT[i].nexbck) continue;
		else if(!SetT[i].nexbck->lock){
			AgainMax = &SetT[i];
			break;
		}
	}
	for(int i = 0;i < 2*maxgain + 1;i++){
		if(!SetB[i].nexbck) continue;
		else if(!SetB[i].nexbck->lock){
			BgainMax = &SetB[i];
			break;
		}
	}
	
	return;
}

void Partitioning::write(NETLIST_o &nt){ // dump file	
	bool first = true;

	ofstream ofs(outfile.c_str());

	ofs << "Cut_size: " << cutsize << endl;
	ofs << "TOP_size: " << nt.TopArea << endl;
	ofs << "BOT_size: " << nt.BotArea << endl;
	ofs << "TOP:\n";	
	Block_o* B;
	for (size_t i = 0; i < bcks.size(); i++) {
        B = &bcks[i];
        if (B->mymod->tier == 0) {
            ofs << (first ? "" : " ") << B->mymod->name;
            if (first) first = false;
        }
    }
	ofs << ";\n";
	first = true;
	ofs << "BOT:\n";
	for (size_t i = 0; i < bcks.size(); i++) {
        B = &bcks[i];
        if(B->mymod->tier == 1){
			ofs << (first ? "" : " ") << B->mymod->name;
			if(first) first = false;
		}
    }
	ofs << ";\n";
	first = true;
	ofs << "Cut_set:\n";
	NET_o* N;
	for(size_t i=0;i<nt.nets.size();i++){
		N = &nt.nets[i];
		if(N->OnCutline){
			ofs << (first ? "" : " ") << N->name;
			if(first) first = false;
		}
	}
	ofs << ";";
	ofs.close();
	
	return;
}

void Partitioning::Move(NETLIST_o &nt, Lib_o &lib, int dotimes){ // move n blocks on the bucket list
	int Choose, docount, havedo = 0;
	Block_o* move = NULL;
	
	if(dotimes < 0 || dotimes > nt.num_mod) docount = nt.num_mod;
	else docount = dotimes;

	resetgainMax();
	
	LastSelTier = 0;
	while(havedo < docount){
		if(M_partition){
			Choose = selectorA(nt, lib);
		}
		else{
			Choose = selectorM(nt, lib);
		}
		
		if(Choose == 0){
			move = AgainMax->nexbck;
		}
		else if(Choose == 1){
			move = BgainMax->nexbck;
		}
		else{
			//cout << havedo << " times move passed, but over\n";
			break;
		}

		move->lock = true;
		update(nt, move);
		if(move->mymod->tier == 0) move->mymod->tier = 1;
		else move->mymod->tier = 0;
		move->gain = -(move->gain);
		
		if(Choose == 0){
			sizeT -= lib.lib_cell[move->mymod->cell_id].a0;
			sizeB += lib.lib_cell[move->mymod->cell_id].a1;
		}
		else if(Choose == 1){
			sizeT += lib.lib_cell[move->mymod->cell_id].a0;
			sizeB -= lib.lib_cell[move->mymod->cell_id].a1;
	    }
		
		delete_bck(move);
		
		resetgainMax();
		
		havedo++;
		
		if(Legal){
			if(cutsize < minsize && sizeT <= TOPbound && sizeB <= BOTbound && sizeT >= AreaUpperlimit_o && sizeB >= AreaUpperlimit_o){ //8/29 Will
				minsize = cutsize;
				step = havedo;
			}
		}
		else{
			if(sizeT <= TOPbound && sizeB <= BOTbound){
				minsize = cutsize;
				step = havedo;
				Legal = true;
				break;
			}
			else{ //8/14 Will
				if(cutsize < minsize){
					minsize = cutsize;
					step = havedo;
				}
			}
		}
	}
	
	return;
}

int Partitioning::selectorM(NETLIST_o &nt, Lib_o &lib){ // determine the block which should move next
	int choosetier = -1;
	int LibC_T, LibC_B;

	if(AgainMax && BgainMax){
		LibC_T = AgainMax->nexbck->mymod->cell_id;
		LibC_B = BgainMax->nexbck->mymod->cell_id;
		if(sizeT + lib.lib_cell[LibC_B].a0 > TOPbound) choosetier = 0;
		else if(sizeB + lib.lib_cell[LibC_T].a1 > BOTbound) choosetier = 1;
		else{			
			if(AgainMax->gain > BgainMax->gain) choosetier = 0;
			else if(AgainMax->gain < BgainMax->gain) choosetier = 1;
			else{
				if((lib.lib_cell[LibC_T].a0 - lib.lib_cell[LibC_T].a1 > lib.lib_cell[LibC_B].a1 - lib.lib_cell[LibC_B].a0) & 
				   (lib.lib_cell[LibC_T].a0 > lib.lib_cell[LibC_T].a1)
				) choosetier = 0;
				else if((lib.lib_cell[LibC_T].a0 - lib.lib_cell[LibC_T].a1 < lib.lib_cell[LibC_B].a1 - lib.lib_cell[LibC_B].a0) & 
				   		(lib.lib_cell[LibC_B].a1 > lib.lib_cell[LibC_B].a0)	
				) choosetier = 1;
				else{
					if(sizeT/TOPbound - sizeB/BOTbound > 0.1) choosetier = 0;
					else if(sizeB/BOTbound - sizeT/TOPbound > 0.1) choosetier = 1;
					else{
						if(LastSelTier == 0) choosetier = 1;
						else choosetier = 0;
					}
				}			
			}
		}
	}
	else if(!AgainMax && BgainMax) choosetier = 1;	
	else if(AgainMax && !BgainMax) choosetier = 0;
	else{
		return -1;
	}
	changelastSel(choosetier);

	return choosetier;
}

void Partitioning::update(NETLIST_o &nt, Block_o* bptr){ // after move, update related block & cutsize //error
    int nowtier = bptr->mymod->tier, toindex, cn;
	int side[2];
	vector<int> updated;
	vector<int>::iterator iter;
	
	Block_o* to = NULL;
	Block_o* target = NULL;

    for(int i = 0;i < bptr->mymod->num_net;i++){
		iter = find(updated.begin(), updated.end(), bptr->mymod->array_net[i]);
		if(iter != updated.end()) continue;
		updated.push_back(bptr->mymod->array_net[i]);

		cn = bptr->mymod->array_net[i];		
		fill_n(side,2,0);
		
		if(nowtier == 0){
			side[0] = nt.nets[cn].nModonTop;
			side[1] = nt.nets[cn].nModonBot;
		}
		else if(nowtier == 1){
			side[0] = nt.nets[cn].nModonBot;
			side[1] = nt.nets[cn].nModonTop;
		}
		else{
			cout << "No this tier\n";
		}
		
		if(side[0] <= 2 || side[1] <= 1){
			if(side[1] == 0){
				if(!nt.nets[cn].OnCutline){
					nt.nets[cn].OnCutline = true;
				}
				else{
					cout << "add cutsize invalid\n";
				}
				
				cutsize++;
				for(int j = 0;j < nt.nets[cn].num_mod;j++){
					target = nt.mods[nt.nets[cn].connect_mod[j]].mybck;
					target->gain++;
					
					if(!target->lock){
						delete_bck(target);
						toindex = -(target->gain) + maxgain;
						if(target->mymod->tier == 0) to = &SetT[toindex];				
						else if(target->mymod->tier == 1) to = &SetB[toindex];
						add_bck(target, to);
					}
				}
				bptr->gain--;
			}
			else if(side[1] == 1){
				for(int j = 0;j < nt.nets[cn].num_mod;j++){
					target = nt.mods[nt.nets[cn].connect_mod[j]].mybck;

					if(target->mymod->tier != nowtier){					
						target->gain--;
						if(!target->lock){
							delete_bck(target);
							toindex = -(target->gain) + maxgain;
							if(target->mymod->tier == 0) to = &SetT[toindex];				
							else if(target->mymod->tier == 1) to = &SetB[toindex];
							add_bck(target, to);
						}
					}
				}
			}
			if(side[0] == 1){
				if(nt.nets[cn].OnCutline){
					nt.nets[cn].OnCutline = false;
				}
				else{
					cout << "delete cutsize invalid\n";
				}

				cutsize--;
				for(int j = 0;j < nt.nets[cn].num_mod;j++){
					target = nt.mods[nt.nets[cn].connect_mod[j]].mybck;
					target->gain--;

					if(!target->lock){
						delete_bck(target);
						toindex = -(target->gain) + maxgain;
						if(target->mymod->tier == 0) to = &SetT[toindex];				
						else if(target->mymod->tier == 1) to = &SetB[toindex];
						add_bck(target, to);
					}
				}
				bptr->gain++;
			}
			else if(side[0] == 2){
				for(int j = 0;j < nt.nets[cn].num_mod;j++){
					target = nt.mods[nt.nets[cn].connect_mod[j]].mybck;

					if(target->mymod->tier == nowtier){
						target->gain++;
						if(!target->lock){
							delete_bck(target);
							toindex = -(target->gain) + maxgain;
							if(target->mymod->tier == 0) to = &SetT[toindex];				
							else if(target->mymod->tier == 1) to = &SetB[toindex];
							add_bck(target, to);
						}
					}
				}
				bptr->gain--;
			}
		}

		if(nowtier == 0){
			nt.nets[cn].nModonTop--;
			nt.nets[cn].nModonBot++;
		}
		else if(nowtier == 1){
			nt.nets[cn].nModonBot--;
			nt.nets[cn].nModonTop++;
		}		
	}
	
	return;
}

void Partitioning::delete_bck(Block_o* movebck){ // remove the block from the bucket list
	Block_o* pre = movebck->prebck;
	pre->nexbck = movebck->nexbck;
	if(movebck->nexbck)	movebck->nexbck->prebck = pre;
	movebck->prebck = NULL;
	movebck->nexbck = NULL;
	
	return;
}

void Partitioning::FM(NETLIST_o &nt, Lib_o &lib, DIE_o &die){ // FM algorithm
	int oldsize, inttemp;
	TOPbound = die.DieSize*die.TopUtil/100;
	BOTbound = die.DieSize*die.BotUtil/100;
	sizeT = nt.TopArea;
	sizeB = nt.BotArea;
	
	if(sizeT + sizeB < 0.8 * die.DieSize){
		r_o = (((sizeT+sizeB)/2)-max_mod_area_o)/die.DieSize;
	}
	else{
		r_o = 0.4;
	}
	if(r_o < 0.0){
		r_o = 0.0;
	}
	/*
	cout << "Util T " << (double)sizeT/die.DieSize << endl;
	cout << "Util B " << (double)sizeB/die.DieSize << endl;
	cout << "max_mod_area_o " << (double)max_mod_area_o/die.DieSize << endl;
	cout << "r_o : " << r_o/die.DieSize << endl << endl << endl << endl;
	*/
	AreaUpperlimit_o = r_o * die.DieSize; //8/29 Will
	if(sizeT <= TOPbound && sizeB <= BOTbound) Legal = true;
	else Legal = false;

	construct(nt);
	schedule(nt);
	//Move(nt, lib, nt.nMacro);

	while(!Legal){
		Move(nt, lib, nt.num_mod);
		
		if(Legal){
			nt.TopArea = sizeT;
			nt.BotArea = sizeB;
			
			write(nt);
			for(int i = 0;i < nt.num_mod;i++){
				if(!nt.mods[i].mybck->lock){
					delete_bck(nt.mods[i].mybck);
					nt.mods[i].mybck->lock = true;
				}				
			}
			schedule(nt);
		}
		
		else{ //8/14 Will
			for(int i = 0;i < nt.num_mod;i++){
				if(nt.mods[i].tier == 0) nt.mods[i].tier = 1;
				else if(nt.mods[i].tier == 1) nt.mods[i].tier = 0;
			}
			for(int i = 0;i < nt.num_net;i++){
				inttemp = nt.nets[i].nModonTop;
				nt.nets[i].nModonTop = nt.nets[i].nModonBot;
				nt.nets[i].nModonBot = inttemp;
			}
			sizeT = nt.TopArea;
			sizeB = nt.BotArea;
			minsize = cutsize;
			schedule(nt);
			Move(nt, lib, step);

			nt.TopArea = sizeT;
			nt.BotArea = sizeB;

			for(int i = 0;i < nt.num_mod;i++){
				if(!nt.mods[i].mybck->lock){
					delete_bck(nt.mods[i].mybck);
					nt.mods[i].mybck->lock = true;
				}				
			}
			schedule(nt);
		}
	}
	
	while(1){
		oldsize = minsize;
		Move(nt, lib, nt.num_mod);
		
		if(minsize < oldsize){
			for(int i = 0;i < nt.num_mod;i++){
				if(nt.mods[i].tier == 0) nt.mods[i].tier = 1;
				else if(nt.mods[i].tier == 1) nt.mods[i].tier = 0;
			}
			for(int i = 0;i < nt.num_net;i++){
				inttemp = nt.nets[i].nModonTop;
				nt.nets[i].nModonTop = nt.nets[i].nModonBot;
				nt.nets[i].nModonBot = inttemp;
			}
			sizeT = nt.TopArea;
			sizeB = nt.BotArea;
			minsize = cutsize;
			schedule(nt);
			Move(nt, lib, step);

			nt.TopArea = sizeT;
			nt.BotArea = sizeB;
			write(nt);

			for(int i = 0;i < nt.num_mod;i++){
				if(!nt.mods[i].mybck->lock){
					delete_bck(nt.mods[i].mybck);
					nt.mods[i].mybck->lock = true;
				}				
			}
			schedule(nt);
		}
		else break;
	}
	
	conclude(nt, lib);
	
	return;
}

void Partitioning::show(NETLIST_o &nt, Lib_o &lib){ //display the detailed data	
	/*
	int count = 0;
    Block* bptr = NULL;
	
	cout << "net Size : " << nt.num_net << endl;
	for(auto N : nt.nets){
		cout << "Name : " << N.name << endl;
		cout << "ID : " << N.id << endl;
		cout << "head : " << N.head << endl;
		cout << "degree : " << N.degree << endl;
		cout << "Connect cell int : ";		
		for(int j = N.head;j < N.head + N.degree;j++){
			cout << setw(4) << nt.pins[j].corr_id << " ";
		}
		cout << endl;
		cout << "Connect cell     : ";
		for(int j = N.head;j < N.head + N.degree;j++){
			cout << setw(4) << nt.mods[nt.pins[j].corr_id].name << " ";
		}
		cout << endl;
		cout << "On line? : " << N.OnCutline;
		cout << endl << endl;
	}
	
	cout << "module Size :" << nt.num_mod << endl;
	for(auto M : nt.mods){
		cout << "Name : " << M.name << endl;
		cout << "ID : " << M.id << endl;
		cout << "Area : " << M.area << endl;
		cout << "Tier : " << M.tier << endl;
		cout << "Connect net int : ";
		for(int i = 0;i < M.num_net;i++){
			cout << setw(4) << M.array_net[i] << " ";
		}
		cout << endl;
		cout << "Connect net     : ";
		for(int i = 0;i < M.num_net;i++){
			cout << setw(4) << nt.nets[M.array_net[i]].name << " ";
		}
		cout << endl << endl;
	}
	
	cout << "Maxgain : " << maxgain << endl;
	
	cout << "SetT :\n"; 
	for(auto t : SetT){
		cout << "Gain : " << setw(2) << t.gain << endl;
	}
	cout << endl;
	cout << "SetB :\n";
	for(auto b : SetB){
		cout << "Gain : " << setw(2) << b.gain << endl;
	}
	cout << endl;
	
	cout << "Gain BLock checker:\n";
	for(int i = 0;i < nt.num_mod;i++){
		cout << setw(4) << bcks[i].mymod->name << " : Gain " << setw(2) << bcks[i].gain << " Lock " << bcks[i].lock << " Tier " << bcks[i].mymod->tier << endl;
	}
	cout << endl;
	
	cout << "SetT:connect condition\n";
	for(auto t : SetT){
		if(!t.nexbck) continue;
		count = 0;
		bptr = &t;		
		while(bptr->nexbck){
			bptr = bptr->nexbck;
			//cout << " -> " << bptr->mymod->name;
			count++;
		}
		cout << setw(2) << t.gain << " -> " << count << endl;
	}
	cout << "SetB:connect condition\n";
	for(auto b : SetB){
		if(!b.nexbck) continue;
		count = 0;
		bptr = &b;
		while(bptr->nexbck){
			bptr = bptr->nexbck;
			//cout << " -> " << bptr->mymod->name;
			count++;
		}
		cout << setw(2) << b.gain << " -> " << count << endl;
	}
	cout << endl;

	cout << "Maxgain in SetT : " << (AgainMax?AgainMax->gain:-maxgain-1);
	cout << "\nMaxgain in SetB : " << (BgainMax?BgainMax->gain:-maxgain-1);
	cout << "\nTopArea : " << nt.TopArea << "\nBotArea : " << nt.BotArea;
	cout << "\nsizeT : " << sizeT << endl << "sizeB : " << sizeB << endl;		
	cout << "\nTOPbound : " << TOPbound << "\nBOTbound : " << BOTbound << endl;
	cout << "\nmax_mod_area : " << max_mod_area;
	cout << "\nCutsize : " << cutsize << "\nminsize : " << minsize << "\nstep : " << step << endl;
	*/
	return;
}

void Partitioning::conclude(NETLIST_o &nt, Lib_o &lib){
	int id1, id2, inttemp;
	string pin_name;
	
	for(int i = 0;i < nt.num_mod;i++){
		if(nt.mods[i].tier == 0) nt.mods[i].tier = 1;
		else if(nt.mods[i].tier == 1) nt.mods[i].tier = 0;
	}
	for(int i = 0;i < nt.num_net;i++){
		inttemp = nt.nets[i].nModonTop;
		nt.nets[i].nModonTop = nt.nets[i].nModonBot;
		nt.nets[i].nModonBot = inttemp;
	}
	sizeT = nt.TopArea;
	sizeB = nt.BotArea;

	for(int i = 0;i < nt.num_mod;i++){
		id1 = nt.mods[i].cell_id;
		nt.mods[i].mod_w = nt.mods[i].tier*lib.lib_cell[id1].w1 + (1-nt.mods[i].tier)*lib.lib_cell[id1].w0;
		nt.mods[i].mod_h = nt.mods[i].tier*lib.lib_cell[id1].h1 + (1-nt.mods[i].tier)*lib.lib_cell[id1].h0;
		nt.mods[i].area = nt.mods[i].mod_w*nt.mods[i].mod_h;
	}

	for(int i = 0;i < nt.num_pin;i++){
		pin_name = nt.pins[i].name;
		pin_name = pin_name.substr(pin_name.find("/") + 1, pin_name.length());
		id1 = nt.mods[nt.pins[i].corr_id].cell_id;
		id2 = lib.lib_cell[id1].pin_NameToID[pin_name];
		nt.pins[i].offset_x = lib.lib_cell[id1].lib_pin[id2+(nt.mods[nt.pins[i].corr_id].tier * lib.lib_cell[id1].p)].offset_x;
		nt.pins[i].offset_y = lib.lib_cell[id1].lib_pin[id2+(nt.mods[nt.pins[i].corr_id].tier * lib.lib_cell[id1].p)].offset_y;
	}

	nt.nTSV = cutsize;

	return;
}
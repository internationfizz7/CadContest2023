#include "Abacus_tsv.h"
using namespace std;

ABTSV::ABTSV(NETLIST &nt)
{
	readNT(nt);
	runAbacus();
	cout<<"overlap(TSV): "<<isOverlap()<<endl;
	writeNT(nt);
}
int ABTSV::readNT(NETLIST &nt)
{
	//-------------------------------row------------------------------------//
	int ex_width = nt.terminal_w + nt.terminal_space;//expanded width
	int ex_height = nt.terminal_h + nt.terminal_space;//expanded height
	
	row_high = ex_height;
	Xmin = ceil(nt.terminal_space/2);
	Ymin = ceil(nt.terminal_space/2);
	Xmax = nt.Xmax_top - ceil(nt.terminal_space/2);
	Ymax = nt.Ymax_top - ceil(nt.terminal_space/2);
	
	NumRows = (Ymax-Ymin)/ex_height;
	
	rows.resize(NumRows);
	for(int i =0; i<NumRows; i++)
	{
		rows[i].Coordinate = i*row_high;
		rows[i].Height = row_high;
		rows[i].NumSites = Xmax - Xmin;
		rows[i].SubrowOrigin = Xmin;
		rows[i].termail.push_back(rows[i].SubrowOrigin);
		rows[i].termail.push_back(rows[i].SubrowOrigin + rows[i].NumSites);
		rows[i].subrow.push_back({rows[i].SubrowOrigin, rows[i].NumSites});
	}
	cout<<"row constructed: "<<NumRows<<" "<<row_high<<endl;
	int num_tsv = 0;
	//--------------------------cell-----------------------------------//
	for(unsigned int i =0; i<nt.mods.size(); i++)
	{
		if(!nt.mods[i].flagTSV){
			continue;
		}
		num_tsv++;
		Cell tsv;
		tsv.name = nt.mods[i].modName;
		tsv.type = "moveable";
		tsv.index = num_tsv;
		tsv.index_nt =i;
		tsv.box = findBox(nt, *nt.mods[i].pNet);
		tsv.llx = (tsv.box.llx + tsv.box.urx)/2 - ex_width/2;
		tsv.lly = (tsv.box.lly + tsv.box.ury)/2 - ex_height/2;
		tsv.width = ex_width;
		tsv.height = ex_height;
		cells.push_back(tsv);
	}
	NumNodes = num_tsv;
	NumTerminals=0;
	cout<<"TSV read:"<<num_tsv<<endl;
	//--------------------------------------------------------------//
    return 0;
}
int ABTSV::writeNT(NETLIST &nt)
{
    for(int i =0; i<NumNodes; i++)
	{
		nt.mods[cells[i].index_nt].LeftX = nt.mods[cells[i].index_nt].GLeftX = cells[i].llx;
		nt.mods[cells[i].index_nt].LeftY = nt.mods[cells[i].index_nt].GLeftY = cells[i].lly;
		nt.mods[cells[i].index_nt].CenterX = nt.mods[cells[i].index_nt].GCenterX = cells[i].llx + 0.5*cells[i].width;
		nt.mods[cells[i].index_nt].CenterY = nt.mods[cells[i].index_nt].GCenterY = cells[i].lly + 0.5*cells[i].height;
	}
	return 0;
}
ABTSV::REGION ABTSV::findBox(NETLIST &nt, int net_index){
	int llx = Xmax;
	int lly = Ymax;
	int urx = Xmin;
	int ury = Ymin;
	if(nt.nets[net_index].nPin==0)
	{
		cout<<"error, tsv nets without connection";
		return {0,0,0,0};
	}
	for(int i=0; i<nt.nets[net_index].nPin; i++)
	{
		int j = i+nt.nets[net_index].head;
		MODULE* thismod = &nt.mods[nt.pins[j].index]; 
		if(thismod->flagTSV)
		{
			continue;
		} 
		if(thismod->CenterX < llx){
			llx = thismod->CenterX;
		}
		if(thismod->CenterY < lly){
			lly = thismod->CenterY;
		}
		if(thismod->CenterX > urx){
			urx = thismod->CenterX;
		}
		if(thismod->CenterY > ury){
			ury = thismod->CenterY;
		}
	}
	REGION r(llx,lly,urx,ury);
	return r; 
}
void ABTSV::addcell(Cluster &c, Cell n){
	c.cellorder.push_back(n.index_new);
	c.ec += 1; // e(i) = 1 
	c.qc = c.qc + (n.llx - c.wc);
	c.wc = c.wc + n.width;
}
void ABTSV::addcluster(Cluster &c1, Cluster c2)
{
	unsigned int s = c1.cellorder.size();
	c1.cellorder.resize(s + c2.cellorder.size());
	for (unsigned int i = 0; i < c2.cellorder.size(); ++i)
	{
		c1.cellorder[s + i] = c2.cellorder[i];
	}
	c1.ec = c1.ec + c2.ec;
	c1.qc = c1.qc + c2.qc - c2.ec*c1.wc;
	c1.wc = c1.wc + c2.wc;
}
void ABTSV::collapse(Subrow &r)
{
	// place cluster c
	r.C.back().x = r.C.back().qc / r.C.back().ec;
	// Limit position between x_min and x_max - wc(c)
	if (r.C.back().x < r.Xmin)
	{
		r.C.back().x = r.Xmin;
	}
	if (r.C.back().x > r.Xmax - r.C.back().wc)
	{
		r.C.back().x = r.Xmax - r.C.back().wc;
	}
	// overlap between c and its predecessor c' ?
	if (r.C.size() > 1)
	{
		if (r.C[r.C.size() - 2].x + r.C[r.C.size() - 2].wc >= r.C.back().x)
		{
			addcluster(r.C[r.C.size() - 2], r.C.back());
			r.C.pop_back();
			collapse(r);
		}
	}
}
void ABTSV::PlaceRow(int &row, Subrow &r, Cell &n, vector<Cell> &cell)
{
	// set new x position for cell[i]
	if (n.llx < r.Xmin)
	{
		n.llx = r.Xmin;
	}
	else if (n.llx + n.width > r.Xmax)
	{
		n.llx = r.Xmax - n.width;
	}
	Cluster C_last;
	if (!r.C.empty())
	{
		C_last = r.C.back();
	}
	if (r.C.empty() || C_last.x + C_last.wc <= n.llx) // cell i = 1 or Xc(C) + Wc(C) <= X'(i)
	{
		Cluster C_new = Cluster{ 0, 0, 0 };
		C_new.x = n.llx;
		addcell(C_new, n);
		r.C.push_back(C_new);
	}
	else // cell i !=1 && Xc(C) + Wc(C) > X'(C) nor first cell and overlap
	{
		addcell(r.C.back(), n);
		collapse(r);
	}
	int x = r.C.back().x;
	for (unsigned int j = 0; j < r.C.back().cellorder.size(); ++j)
	{
		cell[r.C.back().cellorder[j]].llx = x;
		cell[r.C.back().cellorder[j]].lly = row_high * (row) + this->Ymin;
		x = x + cell[r.C.back().cellorder[j]].width;
	}
}

bool ABTSV::CompareCellX(Cell C1, Cell C2) // Sort modules base on their dimension
{
	return (C1.llx < C2.llx);
}

int ABTSV::runAbacus(){
	//int shapecount = 0; //modified unused
	/*
		for (unsigned i = 0; i < nrn.size(); ++i)
		{
			for (unsigned int j = 0; j < nrn[i].s.size(); ++j)
			{
				row[nrn[i].s[j].lly / 9 - 2].termail.push_back(nrn[i].s[j].llx);
				row[nrn[i].s[j].lly / 9 - 2].termail.push_back(nrn[i].s[j].llx + nrn[i].s[j].swidth);
			}
		}
	*/
	/*
		//  ------------------------------------------------  split out subrow  ------------------------------------------------
		for (int i = 0; i < NumNodes; i++)
		{
			if ((cells[i].type == "fixed"))
			{
				for (int j = cells[i].lly / row_high; j < ceil((cells[i].lly + cells[i].height) / row_high); j++)//modified
				{
					if(j>NumRows-1)
						break;
					rows[j].termail.push_back(cells[i].llx);
					rows[j].termail.push_back(cells[i].llx + cells[i].width);
				}
			}
		}
		for (unsigned int i = 0; i < rows.size(); ++i)
		{
			sort(rows[i].termail.begin(), rows[i].termail.end());
		}
		for (unsigned int i = 0; i < rows.size(); ++i)
		{
			for (unsigned int j = 0; j < rows[i].termail.size(); j += 2)
			{
				if (rows[i].termail[j] != rows[i].termail[j + 1])
				{
					rows[i].subrow.push_back(Subrow{ rows[i].termail[j], rows[i].termail[j + 1] });
				}
			}
		}
		//------clean some dirty row---------
		for(int i = 0; i < NumNodes; i++)
		{
			if (cells[i].type == "fixed")
			{
				int llx_macro = cells[i].llx;
				int urx_macro = cells[i].llx + cells[i].width;
				//lower
				int bound_row = cells[i].lly / row_high;
				for(vector<Subrow>::iterator it=rows[bound_row].subrow.begin(); it<rows[bound_row].subrow.end(); it++)
				{
					Subrow sbr = *it;
					if((sbr.Xmin<=llx_macro && sbr.Xmin<=urx_macro)||(sbr.Xmax<=llx_macro && sbr.Xmax<=urx_macro))
					{
						rows[bound_row].subrow.erase(it);
						it--;
					}
				}
				//upper
				bound_row = ceil((cells[i].lly + cells[i].height)/row_high)-1;
				if(bound_row > NumRows-1)
						continue;
				for(vector<Subrow>::iterator it=rows[bound_row].subrow.begin(); it<rows[bound_row].subrow.end(); it++)
				{
					Subrow sbr = *it;
					if((sbr.Xmin>=llx_macro&&sbr.Xmin<=urx_macro)||(sbr.Xmax>=llx_macro&&sbr.Xmax<=urx_macro))
					{
						rows[bound_row].subrow.erase(it);
						it--;
					}	
				}
			}
		}
		//--------------------------------------
		cout<<"rows has been split\n";
	*/
	//  ------------------------------------------------ legalization main frame ------------------------------------------------
	
	double bestcost, cost;
	int bestrow, bestsubrow, original_x;
	vector<Cluster> original_Cluster;
	map<int, int> index; // to_be_comfirm
	sort(cells.begin(), cells.begin() + NumNodes, CompareCellX); // sort vector<Cell> cell accroding to x-position 
	for (unsigned int i = 0; i < cells.size(); ++i)
	{
		cells[i].index_new = i;
		index[cells[i].index] = i;
	}
	cout<<"TSV mods have been resorted\n";
	/*if (filename == "superblue1") // 4,4,10 -> wl : 330604068
	{
		row_min = 4;
		row_max = 4;
		window_percent = 10;
	}
	else if (filename == "superblue5") // 8,8,5 -> wl : 382317806 / 7,7,6 -> wl : 382824663
	{
		row_min = 7;
		row_max = 7;
		window_percent = 6;
	}
	else if (filename == "superblue19") // 5,5,35 -> wl : 163247568
	{
		row_min = 5;
		row_max = 5;
		window_percent = 35;
	}*/
	int fail =0;
	for (int i = 0; i <NumNodes; ++i) // for each cell i
	{
		
		int row_min = (cells[i].box.ury - cells[i].box.lly)/2 + 1; 
		int row_max = (cells[i].box.ury - cells[i].box.lly)/2 + 1;

		int row_index = (cells[i].lly - this->Ymin) / row_high;
		if (row_index > NumRows - 1)
		{
			row_index = NumRows - 1; // modified_ if out of the maximum row
		}
		original_x = cells[i].llx; // store original cell position information
		//cout << i << " " << cells[i].name << " " << original_x <<" r"<< row_index<<endl;
		while(true)
		{
			bestcost = INT_MAX; // c_best = infinity
			bestrow = -1; // test for bug
			bestsubrow = -1; // test for bug
			for (int j = row_index - row_min; j < row_index + row_max; ++j) // for the row r within the window : row[j] 
			{
				if ((j < 0) || (j > NumRows - 1))
				{
					continue;
				}
				for (unsigned int k = 0; k < rows[j].subrow.size(); ++k) // for the subrow row[j].subrow[k] 
				{
					// insert cell i into row r
					// Placerow r (trial)
					if (rows[j].subrow[k].w < cells[i].width) // i.e. row[j].subrow[k] have no space to place cell
					{
						continue;
					}
					//cout<<j<<"->";
					// if row[j].subrow[k] still have space to place cell
					original_Cluster = rows[j].subrow[k].C; // store original Cluster vector information
					//cell[i].lly = row[j].Coordinate; // set new y position for cell[i]
					PlaceRow(j, rows[j].subrow[k], cells[i], cells);
					cost = 0; // reset cost each time 
					// Determine cost c , cost = wire length( modified->distance)
					/*
						for (unsigned int m = 0; m < cells[i].CN.size(); ++m)
						{
							Xmax = 0;
							Ymax = 0;
							Xmin = 10000000;
							Ymin = 10000000;
							// calculate cost by HWPL
							for (int n = 0; n < cells[i].CN[m]->pin_num; ++n)
							{
								if (round(cell[cell[i].CN[m]->CC_new[n]].llx + cell[cell[i].CN[m]->CC_new[n]].width / 2 + cell[i].CN[m]->P[n].Xoff) > Xmax)
								{
									Xmax = round(cell[cell[i].CN[m]->CC_new[n]].llx + cell[cell[i].CN[m]->CC_new[n]].width / 2 + cell[i].CN[m]->P[n].Xoff);
								}
								if (round(cell[cell[i].CN[m]->CC_new[n]].llx + cell[cell[i].CN[m]->CC_new[n]].width / 2 + cell[i].CN[m]->P[n].Xoff) < Xmin)
								{
									Xmin = round(cell[cell[i].CN[m]->CC_new[n]].llx + cell[cell[i].CN[m]->CC_new[n]].width / 2 + cell[i].CN[m]->P[n].Xoff);
								}
								if (round(cell[cell[i].CN[m]->CC_new[n]].lly + cell[cell[i].CN[m]->CC_new[n]].height / 2 + cell[i].CN[m]->P[n].Yoff) > Ymax)
								{
									Ymax = round(cell[cell[i].CN[m]->CC_new[n]].lly + cell[cell[i].CN[m]->CC_new[n]].height / 2 + cell[i].CN[m]->P[n].Yoff);
								}
								if (round(cell[cell[i].CN[m]->CC_new[n]].lly + cell[cell[i].CN[m]->CC_new[n]].height / 2 + cell[i].CN[m]->P[n].Yoff) < Ymin)
								{
									Ymin = round(cell[cell[i].CN[m]->CC_new[n]].lly + cell[cell[i].CN[m]->CC_new[n]].height / 2 + cell[i].CN[m]->P[n].Yoff);
								}
							}
							cost = cost + (Xmax - Xmin) + (Ymax - Ymin);
							// calulate cost by distance:
							cost = abs(cells[i].llx - original_x) + abs(cells[i].lly - row_index * row_high);
							if (cost >= bestcost)
							{
								break;
							}
						}
					*/
					cost = abs(cells[i].llx - original_x) + abs(cells[i].lly - row_index * row_high);//manhattan distance
					if (cost < bestcost) // store the info of best cost and best row and subrow position 
					{
						bestcost = cost;
						bestrow = j;
						bestsubrow = k;
					}
					// remove cell i from row r i.e. 
					cells[i].llx = original_x;
					cells[i].lly = row_index * row_high + this->Ymin;
					rows[j].subrow[k].C = original_Cluster;
				}
			}
			//cout<<bestrow<<endl;
			if(bestrow==-1)
			{
				cout<<"row_min:"<<row_min<<endl;
				row_min*=10;
				row_max*=10;
			}
			else
			{
				PlaceRow(bestrow, rows[bestrow].subrow[bestsubrow], cells[i], cells);
				//cout<<" "<<cells[i].llx<<" "<<cells[i].lly<<endl;
				rows[bestrow].subrow[bestsubrow].w -= cells[i].width;
				break;
			}	
		}		
	}
	//cout<<"fail: "<<fail<<endl;
	cout<<"TSV legalization completed\n";
	return 0;
}

bool ABTSV::CompareCellYX(Cell C1, Cell C2) // Sort modules base on their dimension
{
	if (C1.lly != C2.lly)
		return (C1.lly < C2.lly);
	else
		return (C1.llx < C2.llx);
}

int ABTSV::isOverlap(){
	sort(cells.begin(), cells.begin() + NumNodes, CompareCellYX); // sort vector<Cell> cell accroding to x-position 
	int overlaps=0;
	for (int i = 0; i < NumNodes - 1; i++)
	{
		if (cells[i].lly == cells[i+1].lly)
		{
			if (cells[i].llx+cells[i].width > cells[i+1].llx)
				overlaps++;
		}
	}
	return overlaps;
}
#include "Abacus.h"
using namespace std;

Abacus::Abacus(){
}
Abacus::~Abacus(){
}
/*
Abacus::Abacus(NETLIST& tmpnt, int layer){
	NumNodes = tmpnt.LayernMod[layer];
	int n = 0;
	cells.resize(tmpnt.LayernMod[layer]);
	for (int i = 0; i < tmpnt.GetnMod(); i++)
	{
		if (tmpnt.mods[i].modL == layer)
		{
			cells[n].name = tmpnt.mods[i].modName;
			cells[n].width = tmpnt.mods[i].modW;
			cells[n].height = tmpnt.mods[i].modH;
			cells[n].llx = tmpnt.mods[i].LeftX;
			cells[n].lly = tmpnt.mods[i].LeftY;
			cells[n].index = n;
			cells[n].index_nt = i;
			if (tmpnt.mods[i].flagPre)
				cells[n].type = "terminal";//macro
			else if (tmpnt.mods[i].flagTSV)
				cells[n].type = "terminal_NI";//TSV
			else
				cells[n].type = "movable";//cell 
			n++;
		}
	}
}*/
Abacus::Abacus(string bench, bool top,NETLIST &nt){
	benchmark = bench;
	layer = top;
	readScale(nt);
	cout<<"row_scale_read\n";
	if (top)
		benchmark = benchmark + "_top_ml";
	else
		benchmark = benchmark + "_bot_ml";
	bench_txt=benchmark.substr(0,benchmark.size()-3);
	readFile();
	cout<<"cells_read\n";
	cout<<cells.size()<<"\n";
}
int Abacus::readScale(NETLIST nt){
	/*ifstream fin;
	string tempstr, str;
	fin.open(benchmark + ".txt", ios::in);
	if(!fin)
	{
		cerr << benchmark + ".txt"<<" cannot be found\n";
		exit(1);
	}
	string key;
	if (layer == 0)
		key = "BottomDieRows";
	else
		key = "TopDieRows";
	//-----------------------------------------
	while (fin>>tempstr)
	{
		if (tempstr == key)
		{
			fin >> Xmin >> Ymin >> Xmax >> row_high >> NumRows;
			Ymax = Ymin + row_high*NumRows;
		}
	}
	fin.close();*/
	//-----------------------------------------

    if(layer==1){
        Xmin=nt.Xmin_top;
        Ymin=nt.Ymin_top;
        Xmax=nt.Xmax_top;
        row_high=nt.row_high_top;
        NumRows=nt.NumRows_top;
        Ymax=nt.Ymax_top;

    }
    else{
        Xmin=nt.Xmin_bot;
        Ymin=nt.Ymin_bot;
        Xmax=nt.Xmax_bot;
        row_high=nt.row_high_bot;
        NumRows=nt.NumRows_bot;
        Ymax=nt.Ymax_bot;
    }


	rows.resize(NumRows);
	for (int i = 0; i < NumRows; i++)
	{
		rows[i].Coordinate = i*row_high;
		rows[i].Height = row_high;
		rows[i].NumSites = Xmax - Xmin;
		rows[i].SubrowOrigin = Xmin;
		rows[i].termail.push_back(rows[i].SubrowOrigin);
		rows[i].termail.push_back(rows[i].SubrowOrigin + rows[i].NumSites);
	}
	cout<<rows.size()<<" "<<row_high;
	return 0;
}
//inside constructor
int Abacus::readFile(){
	ifstream inFile;
	string tempstr, str;
	//----------------------------------------read nodes---------------------------------------------------//
	string temp2=bench_txt+".nodes";
	inFile.open(temp2.c_str(), ios::in); // read .,nodes : argv[1] = superblue.nodes
        if(!inFile)
        {
                cerr << bench_txt + ".txt"<<" cannot be found\n";
                exit(1);
        }
	for (int i = 0; i < 1; ++i)
	{
		getline(inFile, tempstr);
	}
	inFile >> str >> str >> NumNodes >> str >> str >> NumTerminals;
	cells.resize(NumNodes);
	bool flag = 0;//determine if terminal
	string name, name2;
	int w, h;
	for (int i = 0; i < NumNodes; ++i)
	{
		if (flag == 0)
		{
			inFile >> name >> w >> h >> name2;
			if ((name2 != "fixed") && (name2 != "terminal_NI"))
			{
				flag = 1;
				if ((w == 0) || (h == 0))
				{
					name = name2;
					continue;
				}
				cells[i].name = name;
				cells[i].width = w;
				cells[i].height = h;
				cells[i].index = i;
				cells[i].type = "moveable";
				mapcell[name] = i;
				name = name2;
			}
			else //if(( name2 == "terminal" ) || ( name2 == "terminal_NI" ))
			{
				flag = 0;
				if ((w == 0) || (h == 0))
				{
					continue;
				}
				cells[i].name = name;
				cells[i].width = w;
				cells[i].height = h;
				cells[i].index = i;
				cells[i].type = name2;
				mapcell[name] = i;
			}
		}
		else // flag == 1
		{
			inFile >> w >> h >> name2;
			if ((name2 != "fixed") && (name2 != "terminal_NI"))
			{
				flag = 1;
				if ((w == 0) || (h == 0))
				{
					name = name2;
					continue;
				}
				cells[i].name = name;
				cells[i].width = w;
				cells[i].height = h;
				cells[i].index = i;
				cells[i].type = "moveable";
				mapcell[name] = i;
				name = name2;
			}
			else // if( ( name2 == "terminal" ) || ( name2 == "terminal_NI" ) )
			{
				flag = 0;
				if ((w == 0) || (h == 0))
				{
					continue;
				}
				cells[i].name = name;
				cells[i].width = w;
				cells[i].height = h;
				cells[i].index = i;
				cells[i].type = name2;
				mapcell[name] = i;
			}
		}
	}
	inFile.close();
	//----------------------------------------read pl--------------------------------------
	string temp=benchmark + ".pl";
	inFile.open(temp.c_str(), ios::in); // read .pl : argv[4] = superblue.legal
	if(!inFile)
	{
		cerr<<benchmark+".pl"<<" cannot be found";
	}
	for (int i = 0; i < 1; ++i)
	{
		getline(inFile, tempstr);
	}
	for (int i = 0; i < NumNodes; ++i)
	{
		inFile >> str >> cells[i].llx >> cells[i].lly;
		inFile>> str >> str;
	}
	//cout<<cells[5000].name<<endl;
	inFile.close();
	//----------------------------------------------------------------------------------------
	return 0;
}
//inside constructor

void Abacus::addcell(Cluster &c, Cell n){
	c.cellorder.push_back(n.index_new);
	c.ec += 1; // e(i) = 1 
	c.qc = c.qc + (n.llx - c.wc);
	c.wc = c.wc + n.width;
}
void Abacus::addcluster(Cluster &c1, Cluster c2)
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
void Abacus::collapse(Subrow &r)
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
void Abacus::PlaceRow(int &row, Subrow &r, Cell &n, vector<Cell> &cell)
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
		cell[r.C.back().cellorder[j]].lly = row_high * (row);
		x = x + cell[r.C.back().cellorder[j]].width;
	}
}

bool Abacus::CompareCellX(Cell C1, Cell C2) // Sort modules base on their dimension
{
	return (C1.llx < C2.llx);
}

int Abacus::runAbacus(NETLIST &nt){
	//int shapecount = 0; //modified unused
	/*for (unsigned i = 0; i < nrn.size(); ++i)
	{
		for (unsigned int j = 0; j < nrn[i].s.size(); ++j)
		{
			row[nrn[i].s[j].lly / 9 - 2].termail.push_back(nrn[i].s[j].llx);
			row[nrn[i].s[j].lly / 9 - 2].termail.push_back(nrn[i].s[j].llx + nrn[i].s[j].swidth);
		}
	}*/
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
	//  ------------------------------------------------ legalization main frame ------------------------------------------------
	int row_index, row_min = 300, row_max = 300;
	double window_percent = 10;// to be confirm
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
	cout<<"cells have been resorted\n";
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
		//=========================
		row_min = max(int(NumRows/3), 300); 
		row_max = max(int(NumRows/3), 300);
		window_percent = 3;
		//=========================
		if (cells[i].type == "fixed" || cells[i].type == "terminal_NI")
		{
			continue;
		}
		bestcost = INT_MAX; // c_best = infinity
		bestrow = -1; // test for bug
		bestsubrow = -1; // test for bug
		row_index = cells[i].lly / row_high;
		if (row_index > NumRows - 1)
		{
			row_index = NumRows - 1; // modified_ if out of the maximum row
		}
		original_x = cells[i].llx; // store original cell position information
		//cout << i << " " << cells[i].name << " " << original_x <<" r"<< row_index<<endl;
		while(true)
		{
			for (int j = row_index - row_min; j < row_index + row_max; ++j) // for the row r within the window : row[j] 
			{
				if ((j < 0) || (j > NumRows - 1))
				{
					continue;
				}
				for (unsigned int k = 0; k < rows[j].subrow.size(); ++k) // for the subrow row[j].subrow[k] 
				{
					if (cells[i].llx + rows[j].NumSites / window_percent < rows[j].subrow[k].Xmin)//to be confirm
					{
						break;
					}
					if (cells[i].llx - rows[j].NumSites / window_percent > rows[j].subrow[k].Xmax)
					{
						continue;
					}
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
						}*/
					cost = abs(cells[i].llx - original_x) + abs(cells[i].lly - row_index * row_high);
					if (cost < bestcost) // store the info of best cost and best row and subrow position 
					{
						bestcost = cost;
						bestrow = j;
						bestsubrow = k;
					}
					// remove cell i from row r i.e. ÅÜ¦^­ìª¬
					cells[i].llx = original_x;
					cells[i].lly = row_index * row_high;
					rows[j].subrow[k].C = original_Cluster;
				}
			}
			//cout<<bestrow<<endl;
			if(bestrow==-1){
				fail++;
				row_min*= 1.5;
				row_max*= 1.5;
				window_percent = (window_percent-1.0)/1.5+1.0;
				continue;
			}
			else{
				PlaceRow(bestrow, rows[bestrow].subrow[bestsubrow], cells[i], cells);
				//cout<<" "<<cells[i].llx<<" "<<cells[i].lly<<endl;
				rows[bestrow].subrow[bestsubrow].w -= cells[i].width;
				break;
			}
		}
	}
	cout<<"fail: "<<fail<<endl;
	cout<<"legalization completed\n";
	writeFile(nt);
	return 0;
}
int Abacus::writeFile(NETLIST &nt)
{
	ofstream fout;
	string temp=benchmark+"_out.pl";
	fout.open(temp.c_str(), ios::out);
	fout << "UCLA pl 1.0\n" << "# Created  :  Jun 17 2023\n" << "# User     :  SEDA95416\n\n";
	for (int i = 0; i < NumNodes; i++)
	{
		if (cells[i].type == "moveable")
		{
			fout << cells[i].name << "\t" << cells[i].llx << "\t" << cells[i].lly << "\t:  N" << endl;
		}
		else if (cells[i].type == "fixed")
		{
			fout << cells[i].name << "\t" << cells[i].llx << "\t" << cells[i].lly << "\t:  N  /FIXED" << endl;
		}
		else //  cell[i].type == "terminal_NI"
		{
			fout << cells[i].name << "\t" << cells[i].llx << "\t" << cells[i].lly << "\t:  N  /FIXED_NI" << endl;
		}
        nt.mods[nt.GetModID(cells[i].name)].LeftX=nt.mods[nt.GetModID(cells[i].name)].GLeftX=cells[i].llx;
        nt.mods[nt.GetModID(cells[i].name)].LeftY=nt.mods[nt.GetModID(cells[i].name)].GLeftY=cells[i].lly;
        nt.mods[nt.GetModID(cells[i].name)].CenterX=nt.mods[nt.GetModID(cells[i].name)].GCenterX=cells[i].llx+0.5*nt.mods[nt.GetModID(cells[i].name)].modW;
        nt.mods[nt.GetModID(cells[i].name)].CenterY=nt.mods[nt.GetModID(cells[i].name)].GCenterY=cells[i].lly+0.5*nt.mods[nt.GetModID(cells[i].name)].modH;
	}
	fout.close();
	return 0;
} 
bool Abacus::CompareCellYX(Cell C1, Cell C2) // Sort modules base on their dimension
{
	if (C1.lly != C2.lly)
		return (C1.lly < C2.lly);
	else
		return (C1.llx < C2.llx);
}
int Abacus::isOverlap(){
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
int Abacus::isOverlap_Macro(){
	int ovlps = 0;
	for (int i = 0; i < NumNodes; i++)
	{
		if (cells[i].type != "fixed")
		{
			continue;
		}
		//cout<<cells[i].name<<", ";
		
		int llx_macro = cells[i].llx;
		int lly_macro = cells[i].lly;
		int urx_macro = cells[i].llx + cells[i].width;
		int ury_macro = cells[i].lly + cells[i].height;
		for (int j = 0; j < NumNodes ; j++)
		{	
			if (i==j)
			{
				continue;
			}
			int llx_cell = cells[j].llx;
			int lly_cell = cells[j].lly;
			int urx_cell = cells[j].llx + cells[j].width;
			int ury_cell = cells[j].lly + cells[j].height;
			bool ovlp_x = (llx_cell>llx_macro && llx_cell<urx_macro)||(urx_cell>llx_macro && urx_cell<urx_macro);
			bool ovlp_y = (lly_cell>lly_macro && lly_cell<ury_macro)||(ury_cell>lly_macro && ury_cell<ury_macro);
			if(ovlp_x && ovlp_y )
			{
				cout<<cells[i].name<<":("<<llx_macro<<", "<<lly_macro<<"), ("<<urx_macro<<", "<<ury_macro <<"); ";
				cout<<cells[j].name<<":("<<llx_cell<<", "<<lly_cell<<"), ("<<urx_cell<<", "<<ury_cell <<"); ";
				ovlps++;
				cout<<endl;
			}
		}
	}
	cout<<endl;
	return ovlps;
}
#include "structure.h"
#include "QP_ordering.h"

using namespace std;

NON_ZERO_ENTRY::NON_ZERO_ENTRY() : col(0), weight(0)
{ }

NON_ZERO_ENTRY::~NON_ZERO_ENTRY()
{ }

QP::QP()
{ }

QP::~QP()
{ }

NET_QP::NET_QP() : id(0), degree(0)
{ }

NET_QP::~NET_QP()
{ }

MODULE_QP::MODULE_QP()
{
	id = 0;
	x = 0; y = 0; llx = 0; lly = 0; urx = 0; ury = 0;
    nets.clear();
    is_fixed = false;
}

MODULE_QP::~MODULE_QP()
{ }

NETLIST_QP::NETLIST_QP()
{
    modules.clear();
    nets.clear();
}

NETLIST_QP::~NETLIST_QP()
{ }


typedef pair<int, int> PACKINGCOOR;

void UpdateNetlistQP(NETLIST_QP &nt, Legalization_INFO &LG_INFO){
    Macro* macro_temp;
    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        macro_temp = LG_INFO.Macro_Ordering[i];
        MODULE_QP*  module_temp = new MODULE_QP;
        module_temp->id = i;
		module_temp->macro_id = macro_temp->macro_id;
//		cout << "MACRO ID : " << macro_temp->macro_id << endl;
//		getchar();
		module_temp->macro = macro_temp;

        module_temp->x = (macro_temp->lg.llx + macro_temp->lg.urx)/2;
        module_temp->y = (macro_temp->lg.lly + macro_temp->lg.ury)/2;
		if(module_temp->x > BenchInfo.chip_W || module_temp->y > BenchInfo.chip_H){
			cout << "ERROR" << endl;
		}
        if(macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {
            module_temp->is_fixed = TRUE;      //// fix macro for QP
        }
        else
        {
            module_temp->is_fixed = FALSE;     //// rip up macros for QP
        }
        for(unsigned int j = 0; j < macro_temp->NetID.size(); j++){
            int net_id = macro_temp->NetID[j];
            net_list[net_id].module_set.insert(module_temp->id);
            module_temp->nets.insert(net_id);
        }
        nt.modules.push_back(module_temp);
    }
	int moduleID = (int)LG_INFO.Macro_Ordering.size();
    STD_Group* STD_temp;
    PACKINGCOOR packingcoor;
    map <PACKINGCOOR, MODULE_QP*> cellgroup;    // int packingX , int packing Y
    map <PACKINGCOOR, MODULE_QP*>::iterator itermap;
    for(unsigned int i = 0; i < STDGroupBy2.size(); i++){     // all cell groups are fixed
        STD_temp = STDGroupBy2[i];
        packingcoor = make_pair(STD_temp->packingX, STD_temp->packingY);
        itermap = cellgroup.find(packingcoor);
        if(itermap != cellgroup.end()){ // if module exists insert cell & net infor
            for(unsigned int j = 0; j < STD_temp->member_ID.size(); j++){
                int cell_id = STD_temp->member_ID[j];
                for(unsigned int k = 0; k < macro_list[cell_id]->NetID.size(); k++){
                    int net_id = macro_list[cell_id]->NetID[k];
                    net_list[net_id].module_set.insert((*itermap).second->id);
                    (*itermap).second->nets.insert(net_id);
                }
            }
        }else{  // if module not exist, creat new module infor
            MODULE_QP*  module_temp = new MODULE_QP;
            module_temp->id = moduleID++;
            module_temp->x = STD_temp->packingX;
            module_temp->y = STD_temp->packingY;
			if(module_temp->x > BenchInfo.chip_W || module_temp->y > BenchInfo.chip_H){
				cout << "ERROR" << endl;
			}
            module_temp->is_fixed = true;
            cellgroup.insert(make_pair(packingcoor,module_temp));
            for(unsigned int j = 0; j < STD_temp->member_ID.size(); j++){
                int cell_id = STD_temp->member_ID[j];
                for(unsigned int k = 0; k < macro_list[cell_id]->NetID.size(); k++){
                    int net_id = macro_list[cell_id]->NetID[k];
                    net_list[net_id].module_set.insert(module_temp->id);
                    module_temp->nets.insert(net_id);
                }
            }
            nt.modules.push_back(module_temp);
        }
    }
    //cout << "cell groups : " << cellgroup.size() << endl;
	//cout << "index begin ID : " << nt.modules.front()->id << endl;
	//cout << "index end ID : " << nt.modules.back()->id << endl;
    //cout << nt.modules.size() << endl;

	int count = 0;
	for(unsigned int i = 0; i < net_list.size(); i++){
		if(net_list[i].module_set.size() > 1 ){
			NET_QP* net_temp = new NET_QP;
			net_temp->id = count++;
			net_temp->degree = net_list[i].module_set.size();
			for(set<int>::iterator it = net_list[i].module_set.begin(); it != net_list[i].module_set.end(); it++){
				net_temp->modules.push_back(*it);
				nt.modules[(*it)]->QPnet.push_back(net_temp->id);
			}
			nt.nets.push_back(net_temp);
		}
	}

	//cout << "CHECK CHIP  coor : " << BenchInfo.chip_W << "  "  << BenchInfo.chip_H << endl;
	//cout << "CHECK coor  module : " << nt.modules[0]->x << "  "  << nt.modules[0]->y << endl;
	//cout << "All net num : " << net_list.size() << endl;
	//cout << "QP net number : " << count << endl;

}


void SolveQP(NETLIST_QP &nt, Legalization_INFO& LG_INFO){

    //cout << "*Solve Quadratic Programming" << endl;
	//cout << "ALL modules nem : " << nt.modules.size() << endl;
	//cout << "ALL nets nem : " << nt.nets.size() << endl;
	int num_module = 0, num_net = 0;
	map<unsigned int, unsigned int> module_map;
	map< unsigned int, unsigned int> net_map;


	for (unsigned int i = 0; i < nt.modules.size(); i++)
	{
		if (nt.modules[i]->is_fixed == false){
			//cout << "module coor : (" << nt.modules[i]->x << ", " << nt.modules[i]->y << ")" << endl;
			module_map[nt.modules[i]->id] = num_module;
			num_module++;
		}
	}

	bool all_fixed = true;
	for (unsigned int i = 0; i < nt.nets.size(); i++)
	{
		for(unsigned int j = 0; j < nt.nets[i]->modules.size(); j++){
			int module_id = nt.nets[i]->modules[j];
			if(nt.modules[module_id]->is_fixed == false){
				all_fixed = false;
			}
		}
		if(all_fixed == false){
			net_map[num_net] = i;
			num_net++;
		}
	}
	//cout << "num_net : " << num_net << endl;
	//cout << "num_module : " << num_module <<endl;

	for (int modeXY = 0; modeXY <= 1; modeXY++)
	{
		vector< QP > Q_matrix(num_module + num_net);
		vector< double > B_vector(num_net);

		for (unsigned int i = 0; i < B_vector.size(); i++)
			B_vector[i] = 0;

		CreateQmBv(nt, Q_matrix, B_vector, module_map, net_map, modeXY);
		QMatrix Qm;
		Vector Bv, Xv;

		Q_Constr(&Qm, "Qm", (num_module + num_net), True, Rowws, Normal, True);

		for (int i = 0; i < num_module + num_net; i++)
		{
			int num_entry = 0;
			for (unsigned int j = 0; j < Q_matrix[i].connect.size(); j++)
			{
				if ((int)Q_matrix[i].connect[j].col >= i)
					num_entry++;
			}
			Q_SetLen(&Qm, i + 1, num_entry);
			num_entry = 0;
			for (unsigned int j = 0; j < Q_matrix[i].connect.size(); j++)
			{
				if ((int)Q_matrix[i].connect[j].col >= i)
				{
					Q_SetEntry(&Qm, i + 1, num_entry, Q_matrix[i].connect[j].col + 1, Q_matrix[i].connect[j].weight);
					num_entry++;
				}
			}
		}

		V_Constr(&Xv, "Xv", (num_module + num_net), Normal, True);
		if (modeXY == 0)
			V_SetAllCmp(&Xv, (double)BenchInfo.chip_W / 2);
		else
			V_SetAllCmp(&Xv, (double)BenchInfo.chip_H / 2);
		V_Constr(&Bv, "Bv", (num_module + num_net), Normal, True);

		for (int i = 0; i < (num_module + num_net); i++)
		{
			if (i >= num_module)
				V_SetCmp(&Bv, i + 1, B_vector[i - num_module]);
			else
				V_SetCmp(&Bv, i + 1, 0.0);
		}
		SetRTCAccuracy(1e-10);
		if(LASResult() != LASOK)
		{
			WriteLASErrDescr(stdout);
			exit(1);
		}
		CGIter(&Qm, &Xv, &Bv, 1, SSORPrecond, 1.2);
		for (unsigned int i = 0; i < nt.modules.size(); i++)
		{
			if (module_map.find(nt.modules[i]->id) == module_map.end())
				continue;

			unsigned int id = nt.modules[i]->id;

			if (modeXY == 0)
			{
				nt.modules[i]->x = (int)(V_GetCmp(&Xv, id + 1));
				//nt.modules[i].llx = nt.modules[i].x - nt.modules[i].width / 2;
			}
			else
			{
				nt.modules[i]->y = (int)(V_GetCmp(&Xv, id + 1));
				//nt.modules[i].lly = nt.modules[i].y - nt.modules[i].height / 2;
			}
		}
	}


	/*for (unsigned int i = 0; i < nt.modules.size(); i++)
	{
		if (nt.modules[i]->is_fixed == false){
			cout << "module coor : (" << nt.modules[i]->x << ", " << nt.modules[i]->y << ")" << endl;
 		}
	}*/

}


void CreateQmBv(NETLIST_QP &nt, vector< QP > &Q_matrix, vector< double > &B_vector,map<unsigned int, unsigned int> &module_map, map<unsigned int, unsigned int> &net_map, int modeXY){
	int count = 0;
	for(unsigned int i = 0; i < net_map.size(); i++){
		unsigned int net_id = net_map[i];
		unsigned int star_node_id = module_map.size() + i;
		double weight = 1.0 / (double)nt.nets[net_id]->degree;

		NON_ZERO_ENTRY star_node;
		star_node.col = star_node_id;
		star_node.weight = 1;
		Q_matrix[star_node_id].connect.push_back(star_node);  // i, i
		count = 0;
		for(unsigned int j = 0; j < nt.nets[net_id]->modules.size(); j++){
			unsigned int id = nt.nets[net_id]->modules[j];
			if(nt.modules[id]->is_fixed == true){
				if (modeXY == 0)
					B_vector[i] += weight * nt.modules[id]->x;
				else
					B_vector[i] += weight * nt.modules[id]->y;
			}else{
				count++;
				bool is_added = false;
				unsigned int module_id = module_map[nt.modules[id]->id];

				for (unsigned int k = 0; k < Q_matrix[module_id].connect.size(); k++){
					if (module_id == Q_matrix[module_id].connect[k].col)
					{
						Q_matrix[module_id].connect[k].weight += weight;
						is_added = true;
						break;
					}
				}

				if (!is_added)
				{
					NON_ZERO_ENTRY module_node;
					module_node.col = module_id;
					module_node.weight = weight;
					Q_matrix[module_id].connect.push_back(module_node);
				}

				is_added = false;

				for (unsigned int k = 0; k < Q_matrix[module_id].connect.size(); k++)
				{
					if (star_node_id == Q_matrix[module_id].connect[k].col)
					{
						Q_matrix[module_id].connect[k].weight -= weight;
						is_added = true;
						break;
					}
				}
				if (!is_added)
				{
					star_node.weight = -weight;
					Q_matrix[module_id].connect.push_back(star_node);
				}
			}
		}
	}
}



void MacroRefineOrder(NETLIST_QP &nt, vector<Macro*> &MacroOrder){
	map<Macro*, double> score_map;
    ///2021.02
    float exp_x = 10;
    float exp_y = 10;
    float scale_down = (float)1 / (float)10;
    float exp1 = 10;
    int full_W = full_boundary.urx - full_boundary.llx;
    int full_H = full_boundary.ury - full_boundary.lly;
    exp1 = (float)10 * (log(0.5*(float)min(full_W, full_H))) / (log(0.5*(float)max(full_W, full_H)));

    if(full_W > full_H)
        exp_x = exp1;
    if(full_H > full_W)
        exp_y = exp1;


	MODULE_QP * module_temp;
	//module_temp	= nt.modules[0];
	int id; // = module_temp->macro_id;
	int macro_id; // = MovableMacro_ID[id];
	Macro* macro_temp; // = macro_list[macro_id];
    float center_dist; // = (float)( pow( pow(fabs(module_temp->x - BenchInfo.WhiteSpaceXCenter),exp_x) + pow(fabs(module_temp->y - BenchInfo.WhiteSpaceYCenter),exp_y) ,scale_down))/ (float)PARA;

    //float center_dist = (float)( pow( pow(fabs(macro_temp->Macro_Center.first - BenchInfo.WhiteSpaceXCenter),10)
                                        //+ pow(fabs(macro_temp->Macro_Center.second - BenchInfo.WhiteSpaceYCenter),10) ,0.1))/ (float)PARA;
    //////

	for(unsigned int i = 0; i < nt.modules.size(); i++){
		if(nt.modules[i]->is_fixed == false){
			module_temp	= nt.modules[i];
			id = module_temp->macro_id;
			macro_id = MovableMacro_ID[id];
			macro_temp = macro_list[macro_id];
			center_dist = (float)( pow( pow(fabs(module_temp->x - BenchInfo.WhiteSpaceXCenter),exp_x) + pow(fabs(module_temp->y - BenchInfo.WhiteSpaceYCenter),exp_y) ,scale_down))/ (float)PARA;
			//cout << "center_dist : " << center_dist << endl;
			score_map[module_temp->macro] = 1/center_dist;
		}
	}

	//cout << "SCORE TABLE" << endl;
	for(map<Macro*, double>::iterator it = score_map.begin(); it != score_map.end(); it++){
		//cout << "score : (" << (*it).first << ", " << (*it).second << ")" << endl;
		MacroOrder.push_back((*it).first);
	}

}

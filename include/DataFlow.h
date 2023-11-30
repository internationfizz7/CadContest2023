/// 108.05.20
#ifndef DATAFLOW_H_INCLUDED
#define DATAFLOW_H_INCLUDED

#include "structure.h"
#include "hier_group.h"
#include "Partition.h"
#include "plot.h"

class Macro_Group;
class Partition_Node;
class Partition_Net;
class Partition_Region;

class dataflow_group
{
public:
    int ID;
    string heir_name;
    int G_X;
    int G_Y;
    pair<int, int> Original_G;
    vector<int> SubGroup;
    vector< pair<int, int> > GCenter_SubGroup;
    vector< pair<int, int> > Relative_Position;
    vector<float> Area;
    float TotalArea;
    map<int, int> dataflow_net;
    //vector<int> dataflow_net;
    //set<int> dataflow_net;
    set<int> SubG_WL_net;
    set<int> WL_net;

    dataflow_group()
    {
        ID = G_X = G_Y = -1;
        heir_name = "";
        TotalArea = 0;
    }
};

class net_df_wl
{
public:
    int ID;
    vector<int> group;
};


void group_shifting(vector<Macro_Group> &MacroGroupBySC);
void Calculate_quality();

void Exchange_with_Dataflow(bool HoriCut, float &BEST_CUT, Partition_Region &region_temp, vector<Partition_Node> &P_Node, vector<Partition_Net> &P_Net);
void Determine_two_part(bool HoriCut, float &BEST_CUT, set<int> &group1, set<int> &group2, Partition_Region &region_temp, vector<Partition_Node> &P_Node);
void Exchange_G(Partition_Node &node_tmp_a, Partition_Node &node_tmp_b);
void Calculate_gain(set<int> &Group1, set<int> &Group2, Partition_Region &region_temp, vector<Partition_Node> &P_Node, vector<Partition_Net> &P_Net);
void Updata_cutline(float &BEST_CUT);
void Initialize_GroupCount(vector<Partition_Net> &P_Net);

#endif

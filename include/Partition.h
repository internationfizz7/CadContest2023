#ifndef PARTITION_H_INCLUDED
#define PARTITION_H_INCLUDED

#include <queue>
#include <fstream>
#include <algorithm>
#include <map>
#include "corner_stitching.h"
#include "CongestionMap.h"
#include "DensityMap.h"
#include "structure.h"
#include "plot.h"
#include "hier_group.h" /// ADD 2021.04


class dataflow_group;
class CornerNode;
class Macro_Group;  /// ADD 2021.04

class Partition_Node
{
public :
    int ID;
    float Macro_Area;
    float STD_Area;
    int max_w, max_h;               // max macro w/h
    pair<int, int> Center;
    vector<int> net_ID;
    Boundary PlaceBox;
    bool Group_node;                // the Node is a macro group or pre-placed macro

    float Gain;     ///plus 108.08.20
};

class Partition_Net
{
public :
    int ID;
    float net_weight;               // this weight is used for?
    int group1_count;
    int group2_count;
    vector<int> node_ID;            // partition_node ID
};

class Partition_Region
{
public:
    Boundary rectangle;
    int Level;
    vector<int> node_ID;
    float cap;
    float demand;

    /// ADD 2021.07
    pair<int, int> rip_up_point;
    //////
    vector<int> cellcluster_ID;
    float movable_area;
};

class Partition_INFO
{
public :
    vector<Partition_Node> P_Node;
    vector<Partition_Net> P_Net;
    Partition_Region P_Region;

};



void Recurrsive_Partition(Partition_INFO &info_tmp, vector<Partition_Region> &P_region, int node_num, vector<Macro_Group> &MacroGroupBySC); // MODIFY 2021.04
bool Sort_Coor(pair<int, int> a, pair<int, int> b);

int Cal_Displacement_Cost(vector<pair<int, int> > &Coor_ID, Partition_INFO &info_tmp, bool Hori_Cut, int cutline, int now_i);
void Cal_Net_Cut(int &net_cut, Partition_Node &node_tmp, vector<Partition_Net> &P_Net);
float Cal_Aspect_Ratio(Boundary &region_box, bool HoriCut, int cutline);

void BIG_Macro_Align_Boundary(bool HoriCut, Boundary &region_box, vector<pair<int, int> > &Coor_ID, Partition_INFO &info_tmp);

Boundary Blank_BBOX(Boundary &region_box, vector<CornerNode*> &AllCornerNode);
void plot_region(std::ofstream& foutt, Boundary &blank_box, int &color_count);

void Recurrsive_Partition_yu(Partition_INFO &info_tmp, vector<Partition_Region> &P_region, int node_num, vector<Macro_Group> &MacroGroupBySC);



///yu plus
void leagal_cluster_in_region(Partition_Region &region, map<float, AreaTable> Area_Line, bool Hori_Cut, vector<Partition_Node>& P_Node, bool firstside);
bool Sort_Cell_CoorX(STD_Group* a, STD_Group* b);
bool Sort_Cell_CoorY(STD_Group* a, STD_Group* b);

#endif // PARTITION_H_INCLUDED

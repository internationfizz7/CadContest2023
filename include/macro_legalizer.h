#ifndef MACRO_LEGALIZER_H_INCLUDED
#define MACRO_LEGALIZER_H_INCLUDED

#include "corner_stitching.h"
#include "DumpFile.h"
#include "hier_group.h"
#include "Partition.h"
#include "flat_approach.h"
#include "DensityMap.h"
#include "deflef_parser.h"  ///plus 108.04.22
#include "DataFlow.h"       ///plus 108.05.19
#include "CongestionMap.h"  ///plus 108.07.13
#include "cell_placement.h"
#include "refinement.h"
#include "bipartite_matching.h" /// ADD 2021.06
#include "dataType.h"

#include <queue>
#include <algorithm>
#include <math.h>
#include <climits>

class ID_TABLE                  // id_table[i] = macro_list[i]
{
    public:
        int _macro_list_ID;     // = macro_list[i]
        int _MovableMacro_ID;   // = MovableMacro_ID[i] // =MacroClusterSet[i]
        int _Group_ID;          // = MacroGroupBySC[i].member_id
        int _P_Node_ID;         // = P_Region[i]
        ID_TABLE()
        {
            _macro_list_ID = _MovableMacro_ID = _Group_ID = _P_Node_ID = -1;
        }
};

class ORDER_TYPE
{
   public:
   int type_id;
   int w, h;
   int pin_num;
   float total_macro_area;
   float total_std_area;
   float score;
   float SE_score;
   float cong_cost;

   string lef_type_name;
   Boundary type_boundary;
   pair<int, int> type_center;  // X and Y center
   vector<int> macro_id;        // MacroClusterSet ID

   ORDER_TYPE()
   {
        pin_num = total_macro_area = total_std_area = score = SE_score = 0;
   }
};

class ORDER_GROUP
{
    public:
    int group_id;
    Macro_Group* macro_group;       // pounter ti macro_group
    vector<ORDER_TYPE> lef_type;    // ORDER_TYPE

    float total_macro_area;
    float total_std_area;
    float max_dimension;
    float center_Euclid_Distance; /// from center;
    float score;
    float cong_cost;

    Boundary group_boundary;
    pair<int, int> group_center;

    int dataflow_score; ///plus 108.05.28
    vector<string> heirName;    ///plus 108.05.28

    ORDER_GROUP()
    {
        total_macro_area = total_std_area = max_dimension = center_Euclid_Distance = score= 0;
        dataflow_score = -1;    ///plus 108.05.28
    }
};

class ORDER_REGION
{
    public:
    int region_id;
    Partition_Region* p_region;
    vector<ORDER_GROUP> group;

    float center_Euclid_Distance; /// from center;
    float max_macro_Dimension; /// max macro width or height
    float score;

    float cong_cost;

    //int dataflow_score; ///plus 108.05.28
    multimap<string, int> Table_heir_groupID;   ///plus 108.05.28

    ORDER_REGION()
    {
        center_Euclid_Distance = max_macro_Dimension = score = cong_cost = 0;
    }

};

class Legalization_INFO
{
    public:
    vector<Macro*> MacroClusterSet;                 // the vector store all maveable macro by some new <Macro*> ([NOTE: the macro_id is ordering form 0 to macro.size()-1])
    vector<CornerNode*> AllCornerNode;              // for corner stitching

    vector<ORDER_REGION> region;

    vector<vector<Macro*> > Net_INFO;               // store the connected macros of each nets // for cal wirelength

    vector<COST_TERM> MacroClusterSet_Cost;

    //// for each iteration
    vector<vector<Boundary> > Macro_Solution;       // the macro lg solution in each iteration
    vector<USER_SPECIFIED_PARAMETER> PARAMETER_container;
    vector<COST_TERM> cost_container;
    vector<vector<MacrosSet*/*, cmp_id*/> > set_container; // ADD 2021.06

    vector<Macro*> Macro_Ordering;                  /// refinement place ordering of macros
    vector<Partition_Region> PRegion;               // extern vector<Partition_Region>  P_Region in IterativeMacroLegalizeFlow()
    vector<ID_TABLE> ID_Table;                      // extern vector<ID_TABLE> id_table in IterativeMacroLegalizeFlow()
    vector<Macro_Group> MacroGroups;                // extern vector<Macro_Group>  MacroGroupBySC; in IterativeMacroLegalizeFlow()

    bool legal_mode_0;


    Legalization_INFO()
    {
        Macro_Solution.reserve(1000);
        cost_container.reserve(1000);
        PARAMETER_container.reserve(1000);
        set_container.reserve(1000);
    }
};


struct cmp_pair
{
    bool operator()(pair<int, int> a, pair<int, int> b)
    {
        if(a.first == b.first)
        {
            return a.second > b.second;
        }
        return a.first < b.first;
    }
};

/*/// ADD 2021.05
class MacrosSet
{
public:
    int id; // record the index of MacrosSet
    map<string, Macro*> members;
    Boundary bbx;
    float total_area_ratio;     // the sum of each area ratio
    float total_area;           // the sum of area of each macro in the set
    float bbx_area;             // the bbx area
    float bbx_overlap_area;     // the pre-placed area in bbx
    float total_over_bbx;       // total_area / (bbx_area - bbx_overlap_area)

    float H_W_ratio;            // (bbx.ury - bbx.lly) / (bbx.urx - bbx.llx)

    bool aspect_ratio_increase; // true: H/W increase (long), false: H/W decrease (wide)
    bool aspect_ratio_decrease; // true: H/W decrease (wide), false: H/W increase (long), if both increase and decrease are true, the goal is total_over_bbx = 1.

    bool valid;     // for lazy update
    MacrosSet()
    {
        total_area_ratio = 0;     // the sum of each area ratio
        total_area = 0;           // the sum of area of each macro in the set
        bbx_area = 0;             // the bbx area
        total_over_bbx = 0;       // total_area / bbx_area
    }
};

extern vector<MacrosSet*> Macros_Set;
//////*/

void MacroLegal(NETLIST & nt);

void Legalization_Preprocessing(vector<Macro*> &MacroClusterSet, vector<ORDER_REGION> &region, vector<Macro_Group>  &MacroGroupBySC,
vector<Partition_Region> &P_Region, vector<ID_TABLE> &id_table);

void RemoveOverlapwithPreplaced(vector<CornerNode*> AllCornerNode);

void IterativeMacroLegalizeFlow(NETLIST & nt);

void ConstructMacroDeClusterSet(vector<Macro*> &MacroClusterSet);
void Update_NET_INFO(vector<Macro*> &MacroClusterSet);


bool BBoxFeasibility(Boundary BBox);

pair<int, int> Cal_WhiteSpaceCenter(vector<CornerNode*> AllCornerNode);

void Const_ID_TABLE(vector<ID_TABLE> &id_table, vector<Macro_Group> &MacroGroupBySC);

void Do_Partition(HIER_LEVEL* HIER_LEVEL_temp, vector<Macro_Group> &MacroGroupBySC, vector<ID_TABLE> &id_table, vector<Partition_Region> &P_Region);

void TraverseHierTreeConst_P_NET(HIER_LEVEL* HIER_LEVEL_temp, vector<Partition_Net> &P_Net, vector<ID_TABLE> &id_table);

void PLOTMG(vector<Macro_Group> &MacroGroupBySC, vector<ID_TABLE> &id_table);


bool cmp_region_score(ORDER_REGION a, ORDER_REGION b);
bool cmp_group_score(ORDER_GROUP a, ORDER_GROUP b);
bool cmp_type_score(ORDER_TYPE a, ORDER_TYPE b);
void Initial_Legalization(Legalization_INFO &LG_INFO);
void Select_Packing_Corner(Legalization_INFO &LG_INFO, vector<float> &occupy_ratio, Boundary &subregion, Macro* &macro_temp);
void Select_BestCoor(COST_INFO &cost_info, Macro* &macro_temp);
void clear_para_ml();
float CostEvaluation(COST_TERM &cost_term);
bool Expand_subregion(Boundary &subregion, float expand_ratio);

/// refinement
void Simulated_Evolution(vector<Macro*> &MacroClusterSet, vector<ORDER_REGION> &region);
void Simulated_Evolution_0819(Legalization_INFO &LG_INFO, int iter_count);
bool cmp_ordering(Macro* a, Macro* b);

/// legalization
void Packing_Boundary_Ordering_Based_Legalization(Legalization_INFO &LG_INFO, pair<int, int> WhiteCenter);
void Packing_Boundary_Legalization(Legalization_INFO &LG_INFO, pair<int, int> WhiteCenter);
void Leagalize_Illegal_Macro(Legalization_INFO &LG_INFO, queue<int> illegal_set);
void Evaluate_Placement(Legalization_INFO &LG_INFO);
void Choose_Best_Placement(Legalization_INFO &LG_INFO);
void Update_NET_INFO_IN_EVALUATION(Legalization_INFO &LG_INFO);
int Block_Edge_Caculation(set<pair<int, int>, cmp_pair> &Edge);
vector<float> Determine_PackDirection(vector<CornerNode*> AllCornerNode, Boundary region_boundary);
void Region_Sizing(vector<Macro*> &MacroClusterSet, vector<CornerNode*> &AllCornerNode, vector<ORDER_REGION> &region);
void Total_Type_Center_Calculate(ORDER_TYPE &type_tmp, vector<Macro*> &MacroClusterSet);
void ConsideCal_Type_Boundary_Calculate(ORDER_TYPE &type_tmp, vector<Macro*> &MacroClusterSet);
void Total_Group_Center_Calculate(ORDER_GROUP &group_tmp);

/*******plus 108.04.22*********/
float CostEvaluation_soft(COST_TERM &cost_term);
void Construct_dataflow_p_net(vector<Macro_Group> &MacroGroupBySC, vector<Partition_Net> &P_Net);
float Related_dataflow(string a, string b, float hier_score);
bool cmp_groupWdf_score(ORDER_GROUP a, ORDER_GROUP b);
/************************/

/// ADD 2021.03.22
void DivideTilesToGrids();
int gcd(int x, int y);
//////

/// ADD 2021.05
void SearchMovableTile(CornerNode* start, map<string, Macro*> &Surrounding_Macro);
//map<int, MacrosSet*> Find_Surrounding_Sets(vector<CornerNode*> &AllCornerNode, Macro* macro_temp, Boundary macro_lg);
void Update_Sets_Info(vector<CornerNode*> &AllCornerNode);
void Update_Each_Set(MacrosSet* set_tmp, vector<CornerNode*> &AllCornerNode);
void Re_Assign_Sets_ID();
void Revise_Sets();
void Check_Sets_Macros(Legalization_INFO &LG_INFO); // for debug
//////

/// ADD 2021.06
void Bipartite_Matching_Flow(Legalization_INFO &LG_INFO);
void PlotMacroClusterSet_BM(vector<Macro*> MacroClusterSet, string filename);
void PlotGroup(Legalization_INFO &LG_INFO, string filename);
void PlotSet(string filename);
//////

/// ADD 2021.07
void Calculate_Region_Rip_Up_Point(Legalization_INFO &LG_INFO, vector<CornerNode*> &AllCornerNode, vector<pair<int, int> > &PRegion_Rip_Up_Point, vector<Partition_Region> &P_Region);
void Simulated_Evolution_for_each_set(Legalization_INFO &LG_INFO, vector<MacrosSet*> &Priority_Queue, MacrosSet* set_tmp);
void Packing_Boundary_Ordering_Based_Legalization_for_each_set(Legalization_INFO &LG_INFO, pair<int, int> WhiteCenter, MacrosSet* set_tmp);
void Check_Tile(Legalization_INFO &LG_INFO);    // for debug, check all Tile in LG_INFO.AllCornerNode
//////


void SimplLegal(vector<Macro_Group> &MacroGroupBySC, Legalization_INFO &LG_INFO_tmp);
#endif // MACRO_LEGALIZER_H_INCLUDED

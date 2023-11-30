#ifndef CORNER_STITCHING_H_INCLUDED
#define CORNER_STITCHING_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <math.h>
#include <algorithm>
#include <fstream>


#include "structure.h"
#include "preprocessing.h"
#include "plot.h"
#include "CongestionMap.h" /// ADD 2021.03.17

enum CornerNode_type
{
    Blank,
    Fixed,
    Movable
};

enum PlaceCorner
{
    LeftBot,
    LeftTop,
    RightBot,
    RightTop
};
class CornerNode
{
    public:
        int CornerNode_id;
        int NodeType;
        Boundary rectangle;
        CornerNode *N, *S, *E, *W;
        bool TraverseFlag;
        // for update cornernode

        /// lef type for movable macro
        int lef_type_ID;
        /// ADD 2021.05, for movable macro, record the movable tile belongs to which macro
        Macro* macro;
        bool visited;

        CornerNode *Top, *Bottom;
        CornerNode(int type, int id, int llx, int lly, int urx, int ury)
        {
            NodeType = type;
            CornerNode_id = id;
            rectangle.llx = llx;
            rectangle.lly = lly;
            rectangle.urx = urx;
            rectangle.ury = ury;
            TraverseFlag = false;
            this->N = NULL;
            this->S = NULL;
            this->W = NULL;
            this->E = NULL;
            this->Top = NULL;
            this->Bottom = NULL;
            /// ADD 2021.05
            visited = false;
            //////
        }
        CornerNode()
        {
            CornerNode_id = -1;
            TraverseFlag = false;
            this->N = NULL;
            this->S = NULL;
            this->W = NULL;
            this->E = NULL;
            this->Top = NULL;
            this->Bottom = NULL;
            /// ADD 2021.05
            visited = false;
            //////
        }
};

class FeasiblePillar
{
    public:
        CornerNode* temp;
        pair<int, int> LRBound;
        vector<FeasiblePillar> Hori_interval;
        FeasiblePillar()
        {
            Hori_interval.reserve(10);
        }
};

class CylinderInterval
{
  public:
        pair<int, int> LR;
        pair<int, int> LRBound;
        int FinalCount;
        int Width, Height;


};

class COST_TERM
{
    public :
    float Displacement;
    float Center_Distance;
    float Group_Distance;
    float Type_Distance;
    float Packing_DeadSpace;
    float Overlap_BoundaryArea;
    float Thickness;
    float WireLength;
    float Congestion;   //   ADD 2021.03.16
    float Regularity;   //   ADD 2021.07

    /// CornerNode
    float Blank_Center_Dist;
    pair<int, int> MacroCoor;

    COST_TERM()
    {
        Displacement = Center_Distance = Group_Distance = Type_Distance = Packing_DeadSpace = Overlap_BoundaryArea = Thickness = WireLength = Blank_Center_Dist = Congestion = Regularity = 0;
    }

};
class COST_INFO // stored min and max value
{
    public :
    queue<COST_TERM> cost;
    pair<float, float> Displacement;
    pair<float, float> Center_Distance;
    pair<float, float> Group_Distance;
    pair<float, float> Type_Distance;
    pair<float, float> Packing_DeadSpace;
    pair<float, float> Overlap_BoundaryArea;
    pair<float, float> Thickness;
    pair<float, float> WireLength;
    pair<float, float> Congestion;   // ADD 2021.03.16
    pair<float, float> Regularity;   // ADD 2021.08

    COST_INFO()
    {
        Displacement = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Center_Distance = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Group_Distance = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Type_Distance = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Packing_DeadSpace = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Overlap_BoundaryArea = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Thickness = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Congestion = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
        Regularity = make_pair(numeric_limits<float>::max(), numeric_limits<float>::min());
    }


};
class PACKING_INFO
{
    public :
    Macro* Macro_temp;
    CornerNode* ptr;
    int Pack_Direction; ///0 : hori, 1 : vert, 2 : both
    Boundary region_boundary;
    Boundary group_boundary;
    Boundary type_boundary;

    vector<float> occupy_ratio; /// blocked length 0: top, 1: bot, 2: lef, 3: rig
    int WhiteSpaceCenterX;
    int WhiteSpaceCenterY;


};

/// ADD 2021.05
class MacrosSet
{
public:
    int id; // record the index of MacrosSet
    map<string, Macro*> members;
    Boundary bbx;
    float total_area;           // the sum of area of each macro in the set
    float bbx_area;             // the bbx area
    float bbx_overlap_area;     // the pre-placed area in bbx
    float total_over_bbx;       // total_area / (bbx_area - bbx_overlap_area)

    float H_W_ratio;            // (bbx.ury - bbx.lly) / (bbx.urx - bbx.llx)

    vector<float> occupy_ratio;   // this vector size is 4, 0: up, 1: down, 2: left, 3:right, 0 <= value <= 1
    bool aspect_ratio_increase; // true: H/W increase (long), false: H/W decrease (wide)
    bool aspect_ratio_decrease; // true: H/W decrease (wide), false: H/W increase (long), if both increase and decrease are true, the goal is total_over_bbx = 1.

    bool valid;                 // for lazy update, "not used now!"

    MacrosSet()
    {
        id = -1;
        total_area = 0;           // the sum of area of each macro in the set
        bbx_area = 0;             // the bbx area
        bbx_overlap_area = 0;     // the pre-placed area in bbx
        total_over_bbx = 0;       // total_area / bbx_area
        H_W_ratio = 0;
    }
};

extern vector<MacrosSet*> Macros_Set;
//////

void Coner_stitching_const();
void ConerNode_const(vector<CornerNode*> &ConerNode_temp);
void CornerNode_hori_connect(CornerNode* ptr, CornerNode* next);
void CornerNode_vert_connect(CornerNode* ptr, CornerNode* next);
void ConerNode_Vert_link_const(vector<CornerNode*> &ConerNode_temp);
void Cout_CornerNode_linkID(vector<CornerNode*> ConerNode_temp);
void ConerNode_merge(vector<CornerNode*> &ConerNode_temp);
CornerNode* Horizontal_search(pair<double, double> macro_coordinate, CornerNode* ptr);
CornerNode* Vertical_search(pair<double, double> macro_coordinate, CornerNode* ptr);


void ReturnLeftBotCoordinate(int &x, int &y, unsigned int PlaceCornerType, Macro* temp);

void Before_UpdateMacroInDatastruct(vector<CornerNode*> &AllCornerNode, Macro* Macro_temp, vector<CornerNode*> CornerNode_temp);
CornerNode* CornerNodePointSearch(pair<int, int> macro_coordinate, CornerNode* ptr);
void Top_Bot_Link(CornerNode* a, CornerNode* b);
void UpdateMacroInDatastruct(vector<CornerNode*> &AllCornerNode, Macro* Macro_temp, CornerNode* Head, CornerNode* Tail);


void UpdateLeftEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode);
void UpdateRightEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode);
void UpdateBottomEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode);
void UpdateTopEdge(CornerNode* &ptr, CornerNode* UpdateCornerNode);


vector<CornerNode*> NeighborFindingTopEdge(CornerNode* ptrCornerNode);
vector<CornerNode*> NeighborFindingBottomEdge(CornerNode* ptrCornerNode);
vector<CornerNode*> NeighborFindingLeftEdge(CornerNode* ptrCornerNode);
vector<CornerNode*> NeighborFindingRightEdge(CornerNode* ptrCornerNode);


void UpdateInfoInPreplacedInCornerStitching(vector<CornerNode*> temp, vector<CornerNode*> &temp2);
void DeleteCornerNode(vector<CornerNode*> &deletecornernode);
void UpdateFixedPreplacedInCornerStitching();


void Directed_HorizontalEnumeration(CornerNode* StartNode, Boundary &BBox, queue<CornerNode*> &CornerNodeSet);
void Directed_VerticalSearchingBBoxLeftEdge(Boundary &BBox, CornerNode* StartNode, queue<CornerNode*> &VerticalSearchingBBoxLeftEdgeNode);
void Directed_AreaEnumeration(Boundary &BBox, CornerNode* StartNode, queue<CornerNode*> &CornerNodeSet);


void PointPacking(vector<CornerNode*> &AllCornerNode, PACKING_INFO &pack_tmp, COST_INFO &cost_info);
void UpdateBoundingValue(COST_INFO &cost_info, COST_TERM &cost_term);
int Thickness_Cost(int macroCenterX, int macroCenterY, Boundary &region_boundary, vector<float> &occupy_ratio, int PackCorner);
int Cal_WireLength(Macro* macro_temp, pair<int, int> MacroCoor);

void Cout_PossibleCombination(vector<vector<CornerNode*> > PossibleCombination);
vector<CylinderInterval> DP_VertCommonInterval(vector<CornerNode*> CornerNode_temp, Macro* Macro_temp);
void Find_topBlank(vector<CornerNode*> &combination, Macro* Macro_temp, int Height, Boundary &PackingBoundary);
void Find_Right_topBlank(vector<CornerNode*> &combination, Macro* Macro_temp, int Height, Boundary &PackingBoundary);
void Possible_Combination_update(vector<vector<CornerNode*> > &PossibleCombination, CornerNode* start_node, Macro* macro_temp, Boundary &macro_boundary);
void plot_Directed_AreaEnumeration(queue<CornerNode*> nodeset, Boundary region);

/// ADD 2021.08, Set function
/// I use this function in corner_stitching.cpp, and can't include macro_legalizer.h correctly, so I declare MacrosSet and Find_Surrounding_Sets here,
/// other update and check function are declared in macro_legalizer.h
map<int, MacrosSet*> Find_Surrounding_Sets(vector<CornerNode*> &AllCornerNode, Macro* macro_temp, Boundary macro_lg);
//////

extern vector<CornerNode*> PreplacedInCornerStitching;
extern vector<int> FixedPreplacedInCornerStitching;
#endif // CONER_STITCHING_H_INCLUDED

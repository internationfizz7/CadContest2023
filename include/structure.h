#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

class MacrosSet; /// ADD 2021.05
class MODULE_ML;

////// MODIFY 2020.02.21 /////// ADD port info.
class Port
{
public:
    int llx_shift;   ///< offset llx of port (from (0,0) of MACRO)
    int lly_shift;   ///< offset lly of port (from (0,0) of MACRO)
    int urx_shift;   ///< offset urx of port (from (0,0) of MACRO)
    int ury_shift;   ///< offset ury of port (from (0,0) of MACRO)
    Port(){
        llx_shift = lly_shift = urx_shift = ury_shift = 0;
    };
    ~Port(){};
};

/////// MODIFY 2020.02.20 ////// ADD pin info.
class Pin
{
public:
    int pin_id;             ///< pin index
    string pin_name;        ///< "use XXX" in .lef MACRO
    string type;            ///< "direction XXX" in .lef MACRO
    string io;              ///< "direction XXX" in .lef MACRO
    int macroID;

    int x_shift;            ///< offset x-coor of ports' center (from (0,0) of MACRO)
    int y_shift;            ///< offset y-coor of ports' center (from (0,0) of MACRO)
    int x_shift_plus;       ///< plus PARA(=1000) to transfer to DEF unit
    int y_shift_plus;       ///< plus PARA(=1000) to transfer to DEF unit
    vector<Port*> Ports;    ///< ports in this pin
    Pin(){
        pin_name = "";
        pin_id  = x_shift = y_shift = 0;
    };

    ~Pin(){};
};
extern vector<Pin*> pin_list;

// [NOTE] Pins_list[] do not consider celll's pin
// If you want to record cells pin info.
// deflef_parser.cpp (p.830) remove the code "if(macro_temp->macro_type != STD_CELL)
// you can see the info in "output/INFO/STD_info.txt"

/////// MODIFY 2020.07.17//////
class OBS_RECT
{
public :
    int blklayer;                       // routing_layer[] index you can get the layer info. (ME1 ME2 ME3 ..)
    float llx, lly, urx, ury;           // obstacle (llx lly) (urx ury)
    OBS_RECT(){};
    ~OBS_RECT(){};
};

class LefMacroType                                      // for define a "macro type"
{
public :
    int macro_count;                                    // number of moudle are this type
    string macro_Name;                                  // macro type Name
    int type;                                           /// core, blobool Congestion_PARA;
    int macroW, macroH;                                 /// DEF unit
    float macroW_no_unit, macroH_no_unit;               /// LEF unit

    ////// MODIFY 2020.02.21 ////                       // delete "shiftXY" by using Pin
    //vector<pair<int, int> > shiftXY;
    //vector<pair<float, float> > shiftXY_no_unit;      // delect this structure // only record x/y shift
    vector<Pin*> Pins;                                  // pin info. // _is_unit (*PARA)
    /// MODIFT 2020.07.10 ///// POLYGON
    vector<pair<double,double> > polygon;
    /// MODIFT 2020.07.17 ///// OBS
    vector<OBS_RECT* > rect;
    LefMacroType(){
        macro_count = macroW = macroH = 0;
        Pins.clear();
        polygon.clear();
        rect.clear();
    };
};
                                            // only use "DEFLEFInfo" to construct <DEFLEF_INFO>;
class DEFLEF_INFO                           // just "LEF module info."
{
public :
    map<string, int> LefMacroName_ID;       // map macroType name to its LEF_Macros[] ID.
    vector<LefMacroType> LEF_Macros;        // store all MacroType(not pointer) in LEF_Macros // it is a library of macros from LEF
    DEFLEF_INFO(){
        LEF_Macros.clear();
        LefMacroName_ID.clear();
    }
};
extern DEFLEF_INFO DEFLEFInfo;

class Net_ML
{
public :
    int NetID;                      // for net_list[] index
    string net_Name;
    vector<int> macro_idSet;        // connected  movable / preplace / pins in macro_list[] id
    vector<int> cell_idSet;         // connected cells in "macro_list[]" id

    vector<int> group_idSet;        // the net connected std cluster "STDGroupBy[]" id
    vector<int> group2_idSet;       // the net connected std cluster "STDGroupBy2[]" id

    set<int> module_set;            // for QP_ordering

};
extern vector<Net_ML> net_list;        // store all Nets

enum Macro_type
{
    MOVABLE_MACRO,          // lef type == "BLOCK"
    PRE_PLACED,             // lef type == "COVER" or type == "PAD"
    STD_CELL,               // lef type == "CORE"
    PSEUDO_MODULE_ML,          // Nothing
    PORT                    // macro_type = PORT;
};

class Boundary
{
public :
    int llx,lly,urx,ury ;
    Boundary()
    {
        llx = lly = urx = ury = -1 ;
    }
};
void Boundary_Cout(Boundary a);
void Boundary_Assign(Boundary &a, int llx, int lly, int urx, int ury);
void Boundary_Assign_Limits(Boundary &a);
void Cal_BBOX(Boundary &src, Boundary &tar);
void clear_para_structure();

Boundary Overlap_Box(Boundary &region_box, Boundary &Blank_Space);

extern Boundary chip_boundary;              // update in parser
extern vector<Boundary> Placement_blockage; // update in parser

class Macro                                 //include  macros and cells
{
public :
    /// ADD 2021.05
    MacrosSet* set; // store this macro is belong to the set.
    //////
    ///2021.01
    Macro* OriginalMacro;                   //The macro in MacroClusterSet that cut_macro belongs to
    //////
    //////////// for parser ///////////////
    int macro_id ;                          // macro_list id ( =  MovableMacro_ID[ i ] ) see: macro_legalizer.cpp p.1272
    int macro_type;                         // 0: move able macro 1 :preplaced macro 2: cell
    string macro_name ;
    int w,h ;                               // [DON'T USED] record the width and height of LEF FILE (the orient is not orient)
    int cal_w_wo_shrink, cal_h_wo_shrink;   // [AFTER SHINK] macro shrink by "WidthShrinkPARA" and "HeightShrinkPARA" and add "2 * spacing"
    int cal_w,cal_h ;                       // [AFTER SHINK] macro shrink by "WidthShrinkPARA" and "HeightShrinkPARA" and add "2 * spacing"
    int real_w, real_h;                     // [REAL VALUE] with orient and without spacing value
    int w_before_expand, h_before_expand;   // W/H before macro expand size

    double area;                            // [note] : area = w * h /PARE /PARA !!!
    Boundary gp_ori ;                       // gp_ori is without modify the GP postition
    Boundary gp ;                           // gp coordinate : llx lly urx ury  // rcoordinate will plus(+) shifting_X/Y
    Boundary lg ;                           // standard cells is placing in "cell_placement" / macro is packing in "macro_leglizer"
    Boundary lg_no_bi_matching;             // store the result before bipartite matching. ADD 2021.04
    string orient ;
    int pin_num;                            // pin number
    vector<int> NetID;                      // note : all net are not with weght 1 (the bus of the net is devide to multi-net)
    //////// MODIFY 2020.02.21  //////////  // Add pin info.
    //vector<pair<int,int> > netsID_to_pinsID; // vector store net ID with its pin ID
    map<int,vector<int> > NetsID_to_pinsID;  // a net will be two or more pins in the same macro,


    int lef_type_ID;                        // it find MacroLEF type in DEFLEFInfo.LEF_Macros[ID]
    double score_for_place_order;           // = center_dist * macro_temp-> macro_area_ratio // but do not used?

    /////////// for legalzation stage ////////////
    int Spacing;                            // explane macro by spacing and record to "cal_w", "cal_h"
    int Spacing_before_expand;              // Spacing before macro expand size
    bool LegalFlag;                         // legal or not //use for cal. WL
    double WidthShrinkPARA;                 // = ShrinkPara define in main.cpp
    double HeightShrinkPARA;                // = ShrinkPara define in main.cpp
    pair<int, int> RefCoor;
    pair<int, int> Macro_Center;            // cal WL used
    vector<vector<Macro*> > NET_INFO;       // update in "macro_legailzer.cpp" stage
    float macro_area_ratio;                 // macro_area_ratio = macro_area / total_macro_area

    /////////// Using in legalization ///////////
    int region_id;                          // {for macros} ORDER_REGION ID
    int group_id;                           // {for cells} group STDGroupBy[] id  // {for macros} ORDER_GROUP ID
    int group2_id;                          // {for cells} group STDGroupBy2[] id
    int type_id;                            // {for macros} ORDER_TYPE ID

    float HierStdArea;                      // Update in hier_group.cpp func : Update_hier_area();
    bool ConsiderCal;                       // refinement stage set the macro do not need to rip-up
    float score;                            // for simulated evolution score
    /// ADD 2021.05
    bool Rip_up_by_range;   // false: rip-up,  true: don`t rip-up. the same as ConsiderCal
    //////

    Macro()
    {
        cal_w = cal_h = cal_w_wo_shrink = cal_h_wo_shrink = 0;
        score_for_place_order = -1 ;
        macro_type = -1;
        lef_type_ID = -1;
        HierStdArea = 0;
        group_id = group2_id = -1;
        LegalFlag = false;
    }
};
extern vector<Macro*> macro_list;           // update in parser & preprocessing(macro_type) // MACRO + PORT

class STD_Group                             //cell group
{
public :
    int ID;
    bool Dont_Care;                         // ??
    int GPCenter_X, GPCenter_Y;             // group center x,y at GP
    int GCenter_X, GCenter_Y;               // group center x,y for cell pl // all cells has its weight (= the area of the cell) //the position of cell groups is never moving
    int Avg_W, Avg_H;                       // = total area / cell number
    int Width, Height;                      // = sqrt( toatal area of cell group)
    float TotalArea;                        // note : Area is devide (PARE)^2
    int Total_W;
    // for nets
    vector<int> member_ID;                  // cell's ID is use in macro_list[]


    map<int, int> NetID_to_weight;          // <netID, weight>  // "net weight" is the number of cell is connect to the net in the group

    // for partition stage
    int P_regionID;                         // P_Region[] index
    int packingX, packingY;                 // packing in cell_placement()

    STD_Group()
    {
        Dont_Care = false;
        TotalArea = 0;
        GCenter_X = 0;
        GCenter_Y = 0;
        P_regionID = -1;
    }
};
extern vector<STD_Group*> STDGroupBy;       //deflef_parser.cpp is define
//extern vector<vector<int> > net_std;        //deflef_parser.cpp is define

class Bechmark_INFO
{
public:

    string output_tcl_name;

    float chip_area;                            // chip area
    float placeable_area;                       // can be place area in chip
    float movable_macro_area;                   // all movable mcro area
    float preplaced_macro_blkage_area;          // all pre-placed macro area
    float std_cell_area;                        // all cell's area
    float blank_area;
    float preplace_utilization, utilization, total_utilization, std_utilization;
    int min_macro_width, min_macro_height;
    int chip_W, chip_H;
    int max_std_h, min_std_h;                   // min_std_h = 2400
    int max_pin_number;
    int stdYCenter, stdXcenter;
    int WhiteSpaceXCenter, WhiteSpaceYCenter;
    vector<Boundary> FixedHorizontalCut;
    vector<Boundary> FixedVerticalCut;

    Bechmark_INFO()
    {
        output_tcl_name="";
        chip_area=0;
        placeable_area=0;
        movable_macro_area=0;
        preplaced_macro_blkage_area=0;
        std_cell_area=0;
        blank_area=0;
        preplace_utilization=0;
        utilization=0;
        total_utilization=0;
        std_utilization=0;
        min_macro_width=0;
        min_macro_height=0;
        chip_W=0;
        chip_H=0;
        max_std_h=0;
        min_std_h=0;
        max_pin_number=0;
        stdYCenter=0;
        stdXcenter=0;
        WhiteSpaceXCenter=0;
        WhiteSpaceYCenter=0;
        FixedHorizontalCut.clear();
        FixedVerticalCut.clear();
    }

};
extern Bechmark_INFO BenchInfo;         ///Updata in Preprocessing.cpp

class Node
{
public :
    unsigned int macro_id ;
    Boundary rectangle ;
    Node *next, *prev ;
    Node()
    {
        macro_id = -1 ;
        this -> next = NULL ;
        this -> prev = NULL ;
    }
};
void Delete_Node(Node* a );
void Insert_Node(Node* a, Node* b);     // a insert the location infront of b

struct set_sort_x
{
    bool operator()(Node a, Node b)
    {
        return a.rectangle.llx < b.rectangle.llx ;
    }
};

class Row
{
public :
    double index ;
    pair<double, double> left_right_bound ;
//    set<Node,set_sort_x> Node_info ;
    Node *head, *tail ;
    Row()
    {
        index = -1 ;
    }
};
void Row_Delete(map<double,Row*> &for_delete_row);

extern int PARA;            ///DEF TO LEF unit = 1000

class USER_SPECIFIED_PARAMETER
{
public:

    bool PARTITION;         ///set partition  // if(PARAMETER._SUB_REGION_AREA > 1) PARTITION = false; do not partition
    ///MACRO SPACING
    float _SPACING;

    ///MACRO ALIGN BOUNDARY
    int _ROWH;

    /// HIERACHICAL
    int _GROUP_HIER_LEVEL;
    float _GROUP_AREA;
    float _GROUP_MACRO_AREA;
    float _GROUP_STD_AREA;
    float _GROUP_TYPE_STD;

    ///FOR PARTITION
    float _PARTITION_AREA;
    float _PARTITION_DISPLACEMENT;
    float _PARTITION_NETCUT;
    float _PARTITION_ITER;            /// this reoder the macro group to make big macros move to chip boundary
    float _PARTITION_SUBREGION_AREA;
    /// ADD 2021.04
    float _PARTITION_MACRO_CUT;
    //////

    /// PACKING POINT COST GUIDE LEGALIZER to FIND BETTER SOLUTION
    float _PACK_DISPLACEMENT;
    float _PACK_CENTER_DIST;
    float _PACK_GROUP_DIST;
    float _PACK_TYPE_DIST;
    float _PACK_PACK_DEAD;
    float _PACK_OVERLAP;
    float _PACK_THICKNESS;
    float _PACK_WIRELENGTH;
    /// ADD 2021.05, 08
    float _PACK_CONGESTION;
    float _PACK_REGULARITY;
    //////

    /// CHOOSE BEST PLACEMENT
    float _CHOOSE_Blank_Center_Dist;
    float _CHOOSE_Displacement;
    float _CHOOSE_Center_Distance;
    float _CHOOSE_Group_Distance;
    float _CHOOSE_Type_Distance;
    float _CHOOSE_Thickness;
    float _CHOOSE_WireLength;
    /// ADD 2021.04, 08
    float _CHOOSE_Congestion;
    float _CHOOSE_Regularity;
    //////

    /****plus 108.05.19****/
    /// group shifting
    float _Shifting_displacement;
    float _Shifting_dataflow_wl;
    float _Shifting_wirelength;
    /*********************/

    USER_SPECIFIED_PARAMETER()
    {
        PARTITION = true;                       // _SUB_REGION_AREA < 1 then do partition
        _SPACING = 9 * PARA;

        // for Patition
        _ROWH = 2000;                            // if the macro W or H is bigger then std cell H (_ROWH) times then swap p_Node?
        _PARTITION_AREA = 0.6;                  // A weight for "Area_Cost * PARA" which is two region area different
        _PARTITION_DISPLACEMENT = 0.2;
        _PARTITION_NETCUT = 0.4;
        _PARTITION_SUBREGION_AREA = 0.2;        // if the the region is lower then the "placeable_area * PARA" then terminal the partition  // "_PARTITION_SUBREGION_AREA" is very critical para!!
        _PARTITION_ITER = 1;                    // if the region.level is bigger then (_ITER) then deal with the big macro
        /// ADD 2021.04
        _PARTITION_MACRO_CUT = 0.5;
        //////

        // for hier_group.cpp
        _GROUP_AREA = 0.3;                      // A weight for "BenchINfo.std_area * PARA" which is determine the area is over or not
        _GROUP_MACRO_AREA = 0.3;                // A weight for "BenchINfo.placeable_area * PARA"which is a MacroAreaConstraint for decuster macro group or not
        _GROUP_STD_AREA = 0.3;                  // A weight for "BenchINfo.std_cell_area * PARA" which is a STDAreaConstraint for decuster macro group or not
        _GROUP_TYPE_STD = 2;                    // A weight for "StdAraeaInGroup / PARA"
        _GROUP_HIER_LEVEL = 1;                  // the macros in the same node whicih is low level (_HIER_LEVEL) will be a macro group


        // for choose a best corner to packing
        _PACK_DISPLACEMENT = 0.1;
        _PACK_CENTER_DIST = 0.2;
        _PACK_GROUP_DIST = 0.05;
        _PACK_TYPE_DIST = 0.15;
        _PACK_PACK_DEAD = 0.1;
        _PACK_OVERLAP = 0.2;
        _PACK_THICKNESS = 0.2;
        _PACK_WIRELENGTH = 0.1;
        /// ADD 2021.05
        _PACK_CONGESTION = 0.01;
        _PACK_REGULARITY = 10;
        //////

        // for choose a best macro placement result
        _CHOOSE_Blank_Center_Dist = 0.05;
        _CHOOSE_Displacement = 0.05;
        _CHOOSE_Center_Distance = 0.06;
        _CHOOSE_Group_Distance = 0.03;
        _CHOOSE_Type_Distance = 0.1;
        _CHOOSE_Thickness = 0.2;
        _CHOOSE_WireLength = 0.01;

        /// ADD 2021.04
        // for refinement stage -> congestion
        _CHOOSE_Congestion = 0.01;
        _CHOOSE_Regularity = 1;
        //////
    }

};
extern USER_SPECIFIED_PARAMETER PARAMETER;

/***************plus 107.09.17******************/
class Track
{
public :
    int V_routing_track_num;        // # of tracks construct a grid
    int V_routing_track_ori_x;      // the first net passes through ( V_routing_track_ori_x , 0 )
    int V_routing_track_pitch;      // the spacing between tracks
    int V_routing_layer_num;        // # of routing layers of virtical direction
    //int V_routing_track_ori_y;

    int H_routing_track_num;        // # of tracks construct a grid
    int H_routing_track_ori_y;      // the first net passes through ( 0, H_routing_track_ori_y )
    int H_routing_track_pitch;      // the spacing between tracks
    int H_routing_layer_num;        // # of routing layers of horizontal direction
    //int H_routing_track_ori_x;

    set<string> routing_layer_str;

    Track()
    {
        V_routing_track_num = 0;
        V_routing_track_ori_x = 0;
        V_routing_track_pitch = 0;
        V_routing_layer_num = 0;

        H_routing_track_num = 0;
        H_routing_track_ori_y = 0;
        H_routing_track_pitch = 0;
        H_routing_layer_num = 0;
    }
};
extern Track track;

extern map<string, set<string> > DataFlow_outflow;
extern map<string, set<string> > DataFlow_inflow;
/***********************************************/

// MODIFY 2020.07.17////
class LAYER
{
public :
    string name;            // ME1 ME2 ME3 ME4 ME5...
    string direction;       // HORIZONTAL / VERTICAL
    float min_width;
    float min_spacing;

    LAYER(){};
    ~LAYER(){};
};
extern vector<LAYER*> routing_layer;

/*********MODIFY 2021.03.04*********/
class GCELLGRID
{
public:
    string direction;   // x: vertical, y: horizontal
    int num;            // # of columns / rows
    int step;           // the width / height of a grid.
    int step_const;
};
extern vector<GCELLGRID*> gcellgrid;    // the size of gcellgrid should be 2.
/**********************************/

/*********MODIFY 108.03.21*********/

//extern int shifting_X;      ///chip shift coor x  // = -(chip_boundary.llx) // and let (chip_boundary.llx = 0) and (chip_boundary.urx + shifting_X)
//extern int shifting_Y;      ///chip shift coor y  // = -(chip_boundary.lly)

extern string def_file;
extern bool dataflow_PARA;          // dataflow on/off oro not
extern bool Cluster_PARA;           // Cluster on/off oro not
extern bool Congestion_PARA;        // Congestion on/off oro not
extern bool Debug_PARA;             // write netlist on/off
extern int dataflow_heir_num;

/*****************************/


/////////////// EXTERN DATA STRUCTURE /////////////////
extern int shifting_X;                  // chip shift coor x  // = -(chip_boundary.llx) // and let (chip_boundary.llx = 0) and (chip_boundary.urx + shifting_X)
extern int shifting_Y;                  // chip shift coor y  // = -(chip_boundary.lly)
extern map<string,Macro*> Nodemap;      // update in parser

extern vector<int> MovableMacro_ID;     // store the Moveable macro id in macro_list
extern vector<int> PreplacedMacro_ID;   // store the Preplaced Macro id in macro_list
extern map<double,Row*> row_vector;

// extern unsigned int seed;
extern double ShrinkPara;               // shink macros it will bw\e 0< ShrinkPara <1


////////////////  FUNCTIONS  ///////////////////////
double Max_cal(double a, double b);
double Min_cal(double a, double b);
void Pair_max_min(float src, pair<float, float> &tar);
float Scaling_Cost(float range, float cost);

double Absolute(double variable);
string int2str(int i);

///2021.02
extern Boundary full_boundary;
/// ADD 2021.03.17
extern bool Refinement_Flag;
extern bool Build_Set;
extern int ITER;   // for refinement iteration

#endif // STRUCTURE_H_INCLUDED

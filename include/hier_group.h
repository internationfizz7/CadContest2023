#ifndef HIER_GROUP_H_INCLUDED
#define HIER_GROUP_H_INCLUDED

#include <limits>
#include <queue>
#include <set>


#include "structure.h"
#include "plot.h"
class Macro_Group
{
    public :
        int ID;
        bool Dont_Care;
        vector<int> member_ID;          // macro_list id set
        set<int> Net_ID;                // for all macro's connected nets store into a set<>
        queue<string> Hier_Name;
        vector<string> HierName;                                    ///plus 108.04.23
        int Avg_W, Avg_H;
        int GCenter_X, GCenter_Y;       // group center
        float StdAreaInGroup;           // STD area
        float MacroArea;                // macro area
        int lef_type_ID;

        /// ADD 2021.04
        float bound_w, bound_h;         // the w/h of the group bounding box
        //////
        Macro_Group()
        {
            Dont_Care = false;
        }
};

class HIER_NAME
{
public :
    int macro_id;
    vector<string> NameSet;   ///  A/B/C.... -> NameSet[0]=A; NameSet[1]=B; NameSet[2]=C...
};

class HIER_LEVEL // this is a hierarchy tree from root to leaf
{
public :

    int Level_num;                      // level is the deep of the tree from 0 to (leaf-1)
    string Hier_name;

    vector<int> SameLevelSTDSet;        // store the STDs in the tree node

    float StdAreaSameLevel;             // store the STD area in the tree node
    float StdAreaUnderLevel;            // store the STD area is under(below) the tree node


    vector<int> SameLevelMACROSet;      // store the Macros in the tree node
    vector<int> UnderLevelMacroSet;     // store the Macros is under(below) the tree node

    float MacroAreaUnderLevel;          // store the Macro area is under(below) the tree node

    int macro_type;
    map<string, HIER_LEVEL* > Name_HierGroup; // the children of the tree node
    Boundary HierarchicalBBox;
    HIER_LEVEL()
    {
        MacroAreaUnderLevel = StdAreaUnderLevel = StdAreaSameLevel = 0;
        HierarchicalBBox.llx = numeric_limits<int>::max();
        HierarchicalBBox.lly = numeric_limits<int>::max();
        HierarchicalBBox.urx = numeric_limits<int>::min();
        HierarchicalBBox.ury = numeric_limits<int>::min();
    }

};

struct SC_Node
{
    float score;
    int ID_1, ID_2;
    int MemberSize_1, MemberSize_2;
};

struct cmp_score
{
    bool operator()(SC_Node a, SC_Node b)
    {
        return a.score < b.score;
    }
};
void HierGroup(vector<Macro_Group> &MacroGroupBySC, HIER_LEVEL* &HIER_LEVEL_temp);
void HierarchicalNameAnalysis(HIER_LEVEL* &HIER_LEVEL_temp);
void RecurrsiveConstructHierarchy(HIER_LEVEL* &HIER_LEVEL_temp, HIER_NAME &name_temp);
void OutputHierarchicalBBox(HIER_LEVEL* curr_HIER_LEVEL_temp, vector<int> &STDID, vector<int> &MACROID, vector<pair<string, int> > &HIER_NAME_FOR_MATLAB);
void CalHierStdArea(HIER_LEVEL* HIER_LEVEL_temp, map<int, float> &ID_StdArea, float AvgArea);
void TraverseHierTreeConstGroup(HIER_LEVEL* HIER_LEVEL_temp, vector<vector<int> > &MacroGroup);
void Cout_MacroGroup(vector<vector<int> > &MacroGroup);
void GroupMacroBySC(map<int, float> &ID_StdArea, vector<vector<int> > &MacroGroup, vector<vector<Macro_Group> > &MacroGroupBySC);
void Cout_MacroGroupSC(vector<Macro_Group> &MacroGroupBySC);
void CalSC(vector<Macro_Group> &MG_temp);
float SC_Funct(Macro_Group a, Macro_Group b);
float Cal_Connection(set<int> &src1, set<int> &src2);
void Update_hier_area(map<int, float> &ID_StdArea);
void HIER_LEVEL_Delete(HIER_LEVEL* &hier_temp);
void clear_para_hier();
#endif // HIER_GROUP_H_INCLUDED

#ifndef REFINEMENT_H_INCLUDED
#define REFINEMENT_H_INCLUDED

#include "macro_legalizer.h"

class Legalization_INFO;
class CELLPLACEMENT;
//class CONGESTIONMAP;
void clear_para_refine();
class SA
{
protected:
    // store the max and min value for calculate cost
    class NORMALIZED
    {
    public:
        pair<float, float> SA_Blank_Center_Dist;
        pair<float, float> SA_Displacement;
        pair<float, float> SA_center_dist;
        pair<float, float> SA_Group_Distance;
        pair<float, float> SA_Type_Distance;
        pair<float, float> SA_Thickness;
        pair<float, float> SA_WireLength;
    };
    // store the perturb number to Undo
    int SA_Perturb_region;
    // sa iteration
    int SA_iter;

public:

    NORMALIZED normalized;

    void Normalized(SA &sa, Legalization_INFO &LG_INFO);

    void Simulated_annealing(SA &sa, Legalization_INFO &LG_INFO);
    float Calcost(SA &sa, Legalization_INFO &LG_INFO);
    void Perturbation(SA &sa, Legalization_INFO &LG_INFO);
    void StoredBestsol(SA &sa, Legalization_INFO &LG_INFO);
    void Undo(SA &sa, Legalization_INFO &LG_INFO);

};

///////////////////////////////////////////////////////////////////////////////

class REFINECONG
{
protected:
    class REF_INFO
    {
    public:
        vector<Partition_Region> PRegion;               // equal to P_Region
    };

public:
    void RandomSearch(REFINECONG &refine, Legalization_INFO &LG_INFO);
    static void* RandomPARA(void* void_LG_INFO);
    static void CalCongestion(Legalization_INFO &LG_INFO);
    void findBest(Legalization_INFO &LG_INFO);
};

/////////////////////////////////////////////////////////////////////////////////

class WSA
{
protected:
    class TreeNode
    {
    public:
        TreeNode *parent;

        TreeNode *leftchild, *rightchild; // partition to tree left
        Boundary area_ori, area_after;

        float cong_cost;
        bool vertial_cut;
        TreeNode()
        {
            parent = leftchild = rightchild = NULL;
            cong_cost = 1;
        }
    };
    TreeNode *treeroot; // whole chip is root
    int ver_cut, hor_cut;

public:
    void WSARefineFlow(WSA &wsa, Legalization_INFO &LG_INFO);
    void WhiteSpaceAllocationFlow(WSA &wsa, Legalization_INFO &LG_INFO, CONGESTIONMAP &congestion);
    void Devide_Node(TreeNode* this_node);
    void CalNodeCong(CONGESTIONMAP &cong, Legalization_INFO &LG_INFO, TreeNode* this_node);
    void NormalizeCong(TreeNode* this_node);
    void ShiftingAccordingCong(TreeNode* root);
    void MovingMacrosbyArea(TreeNode* root);
    void Print_Node(TreeNode* root);
};

////////////////////////////////////////////////////////////////////////////////////

class MACROEXPAND
{
protected:

public:
    void MacroRefineFlow(MACROEXPAND macroexpand, Legalization_INFO &LG_INFO, int iter, CONGESTIONMAP &congestion);
    void CalMacroTypeCong(CONGESTIONMAP &congestion, Legalization_INFO &LG_INFO);
    void AddMacroSpacingbyLength(Legalization_INFO &LG_INFO);
    void CalRegionWhiteSpace(CELLPLACEMENT *cellpl, Legalization_INFO &LG_INFO);
    void AddMacroSpacingbyArea(Legalization_INFO &LG_INFO);
    void MacroReOrdering(Legalization_INFO &LG_INFO);
    void LegalizationMacors(Legalization_INFO &LG_INFO);
    void RepackingMacros(Legalization_INFO &LG_INFO, CONGESTIONMAP &congestion);
};

#endif

#include "refinement.h"

#define SCALE 1000

#define NUM_THREADS 10000

Legalization_INFO LG_INFO_congestion;     // store every random purturb congestion result
int max_cong = 0;
void clear_para_refine(){
    max_cong=0;
}
void SA::Normalized(SA &sa, Legalization_INFO &LG_INFO)
{
    // sort the mix / min value from "Initial_Legalization()"
    sa.normalized.SA_Blank_Center_Dist = make_pair(FLT_MIN, FLT_MAX);
    sa.normalized.SA_Displacement      = make_pair(FLT_MIN, FLT_MAX);
    sa.normalized.SA_center_dist       = make_pair(FLT_MIN, FLT_MAX);
    sa.normalized.SA_Group_Distance    = make_pair(FLT_MIN, FLT_MAX);
    sa.normalized.SA_Type_Distance     = make_pair(FLT_MIN, FLT_MAX);
    sa.normalized.SA_Thickness         = make_pair(FLT_MIN, FLT_MAX);
    sa.normalized.SA_WireLength        = make_pair(FLT_MIN, FLT_MAX);


    for(int i = 0; i < (int)LG_INFO.cost_container.size(); i++)
    {
        COST_TERM &cost_temp = LG_INFO.cost_container[i];

        Pair_max_min(cost_temp.Blank_Center_Dist,   sa.normalized.SA_Blank_Center_Dist);
        Pair_max_min(cost_temp.Displacement,        sa.normalized.SA_Displacement);
        Pair_max_min(cost_temp.Center_Distance,     sa.normalized.SA_center_dist);
        Pair_max_min(cost_temp.Group_Distance,      sa.normalized.SA_Group_Distance);
        Pair_max_min(cost_temp.Type_Distance,       sa.normalized.SA_Type_Distance);
        Pair_max_min(cost_temp.Thickness,           sa.normalized.SA_Thickness);
        Pair_max_min(cost_temp.WireLength,          sa.normalized.SA_WireLength);
    }
}

void SA::Simulated_annealing(SA &sa, Legalization_INFO &LG_INFO)
{
    USER_SPECIFIED_PARAMETER PARAMETER_ori = PARAMETER, PARAMETER_prev;

    vector<ORDER_REGION> region_ori;


    for(unsigned int i = 0 ; i < LG_INFO.region.size() ; i++)
    {
        region_ori.push_back(LG_INFO.region[i]);

        int region_llx = LG_INFO.region[i].p_region-> rectangle.llx;
        int region_lly = LG_INFO.region[i].p_region-> rectangle.lly;
        int region_urx = LG_INFO.region[i].p_region-> rectangle.urx;
        int region_ury = LG_INFO.region[i].p_region-> rectangle.ury;
        Boundary &region_boundary = LG_INFO.region[i].p_region-> rectangle;


        region_boundary.llx -= (region_urx - region_llx) /2;
        region_boundary.lly -= (region_ury - region_lly) /2;
        region_boundary.urx += (region_urx - region_llx) /2;
        region_boundary.ury += (region_ury - region_lly) /2;

        if(region_boundary.llx < chip_boundary.llx) region_boundary.llx = chip_boundary.llx;
        if(region_boundary.lly < chip_boundary.lly) region_boundary.lly = chip_boundary.lly;
        if(region_boundary.urx > chip_boundary.urx) region_boundary.urx = chip_boundary.urx;
        if(region_boundary.ury > chip_boundary.ury) region_boundary.ury = chip_boundary.ury;
    }


    int  MT = 0, reject = 0, uphill = 0;
    float T = 1000, now_cost = INT_MAX, next_cost, delta_cost, best_cost = INT_MAX, random;
    sa.SA_iter = 0;

    // if the tempture is 1 or the reject times is hight in a tempture then stop
    while(T > 1 && reject / MT < 1.95)
    {
        reject = 0; MT = 0; uphill = 0;
        cout << "Tempture: " << T << endl;

        // MT is the perturb time in each tempture
        while(MT < 100 && uphill < 50)
        {
            PARAMETER_prev = PARAMETER;
            // perturb until delta cost != 0
            do
            {
                sa.Perturbation(sa, LG_INFO);

                next_cost = sa.Calcost(sa, LG_INFO);
                delta_cost = next_cost - now_cost;
            }while(delta_cost == 0);


            // used renadom  0-0.999 to determind the bad solution will be accept or not
            random = (rand() %1000) /(float)1000;

            //cout << "next cost " << next_cost << " iter " << sa.SA_iter  << " delta cost "<< delta_cost << " random " << random  <<" exp " << exp(-delta_cost * 10000 / T) << endl;


            // case 1 the result is accept, the solution will be record
                    // delta_cost * SCALE ( = 1000) the value bigger is more greddy
            if(delta_cost < 0 || random < exp(-delta_cost * SCALE / T)){

                cout << "accept cost " << next_cost << " iter " << sa.SA_iter << " delta cost "<< delta_cost << " random " << random  <<" exp " << exp(-delta_cost * SCALE / T) << endl;
                now_cost = next_cost;
                sa.SA_iter++;
                // case 1-1 uphill
				if(delta_cost >= 0) uphill++;
                // case 1-2 downhill, record the best cost and best solution
				else if(next_cost < best_cost ){
					sa.StoredBestsol(sa, LG_INFO);
					best_cost = next_cost;
                    cout << "### find best cost ### " << best_cost <<endl;
				}
			}
			// case 2 the result is reject
			else{
                PARAMETER = PARAMETER_prev;
				sa.Undo(sa, LG_INFO);
				reject++;
			}
			//sa.SA_iter++;
			MT ++;
        }
		T = T * 0.85;
    }

    PARAMETER = PARAMETER_ori;
    for(unsigned int i = 0 ; i < LG_INFO.region.size() ; i++)
    {
        LG_INFO.region[i] = region_ori[i];
    }
}

// modify form "Choose_Best_Placement()"
float SA::Calcost(SA &sa, Legalization_INFO &LG_INFO)
{
    /*
    float Blank_Center_Dist_range   = sa.normalized.SA_Blank_Center_Dist.first - sa.normalized.SA_Blank_Center_Dist.second;
    float Displacement_range        = sa.normalized.SA_Displacement.first      - sa.normalized.SA_Displacement.second;
    float center_dist_range         = sa.normalized.SA_center_dist.first       - sa.normalized.SA_center_dist.second;
    float Group_Distance_range      = sa.normalized.SA_Group_Distance.first    - sa.normalized.SA_Group_Distance.second;
    float Type_Distance_range       = sa.normalized.SA_Type_Distance.first     - sa.normalized.SA_Type_Distance.second;
    float Thickness_range           = sa.normalized.SA_Thickness.first         - sa.normalized.SA_Thickness.second;
    float WireLength_range          = sa.normalized.SA_WireLength.first        - sa.normalized.SA_WireLength.second;

    // the cost result is store at least of the vector from "Evaluate_Placement()"
    COST_TERM cost_temp = LG_INFO.cost_container.back();

    cost_temp.Blank_Center_Dist     = Scaling_Cost(Blank_Center_Dist_range,     cost_temp.Blank_Center_Dist     - sa.normalized.SA_Blank_Center_Dist.second );
    cost_temp.Displacement          = Scaling_Cost(Displacement_range,          cost_temp.Displacement          - sa.normalized.SA_Displacement.second      );
    cost_temp.Center_Distance       = Scaling_Cost(center_dist_range,           cost_temp.Center_Distance       - sa.normalized.SA_center_dist.second       );
    cost_temp.Group_Distance        = Scaling_Cost(Group_Distance_range,        cost_temp.Group_Distance        - sa.normalized.SA_Group_Distance.second    );
    cost_temp.Type_Distance         = Scaling_Cost(Type_Distance_range,         cost_temp.Type_Distance         - sa.normalized.SA_Type_Distance.second     );
    cost_temp.Thickness             = Scaling_Cost(Thickness_range,             cost_temp.Thickness             - sa.normalized.SA_Thickness.second         );
    cost_temp.WireLength            = Scaling_Cost(WireLength_range,            cost_temp.WireLength            - sa.normalized.SA_WireLength.second        );
    */

    float Blank_Center_Dist_avg   = (sa.normalized.SA_Blank_Center_Dist.first + sa.normalized.SA_Blank_Center_Dist.second)  /2;
    float Displacement_avg        = (sa.normalized.SA_Displacement.first      + sa.normalized.SA_Displacement.second)       /2;
    float center_dist_avg         = (sa.normalized.SA_center_dist.first       + sa.normalized.SA_center_dist.second)        /2;
    float Group_Distance_avg      = (sa.normalized.SA_Group_Distance.first    + sa.normalized.SA_Group_Distance.second)     /2;
    float Type_Distance_avg       = (sa.normalized.SA_Type_Distance.first     + sa.normalized.SA_Type_Distance.second)      /2;
    float Thickness_avg           = (sa.normalized.SA_Thickness.first         + sa.normalized.SA_Thickness.second)          /2;
    float WireLength_avg          = (sa.normalized.SA_WireLength.first        + sa.normalized.SA_WireLength.second)         /2;

    // the cost result is store at least of the vector from "Evaluate_Placement()"
    COST_TERM cost_temp = LG_INFO.cost_container.back();

    cost_temp.Blank_Center_Dist     = Scaling_Cost(Blank_Center_Dist_avg,     cost_temp.Blank_Center_Dist     );
    cost_temp.Displacement          = Scaling_Cost(Displacement_avg,          cost_temp.Displacement          );
    cost_temp.Center_Distance       = Scaling_Cost(center_dist_avg,           cost_temp.Center_Distance       );
    cost_temp.Group_Distance        = Scaling_Cost(Group_Distance_avg,        cost_temp.Group_Distance        );
    cost_temp.Type_Distance         = Scaling_Cost(Type_Distance_avg,         cost_temp.Type_Distance         );
    cost_temp.Thickness             = Scaling_Cost(Thickness_avg,             cost_temp.Thickness             );
    cost_temp.WireLength            = Scaling_Cost(WireLength_avg,            cost_temp.WireLength            );

    float cost = cost_temp.Blank_Center_Dist    * 1;//PARAMETER._CHOOSE_Blank_Center_Dist;
    cost +=  cost_temp.Displacement             * 1;//PARAMETER._CHOOSE_Displacement;
    cost +=  cost_temp.Center_Distance          * 1;//PARAMETER._CHOOSE_Center_Distance;
    cost +=  cost_temp.Group_Distance           * 1;//PARAMETER._CHOOSE_Group_Distance;
    cost +=  cost_temp.Type_Distance            * 1;//PARAMETER._CHOOSE_Type_Distance;
    cost +=  cost_temp.Thickness                * 1;//PARAMETER._CHOOSE_Thickness;
    cost +=  cost_temp.WireLength               * 1;//PARAMETER._CHOOSE_WireLength;

    //cout << "\t"<<cost_temp.Blank_Center_Dist << " "<< cost_temp.Displacement << " "<< cost_temp.Center_Distance << " "<< cost_temp.Group_Distance << " "
    //    << cost_temp.Type_Distance << " "<< cost_temp.Thickness << " "<< cost_temp.WireLength << endl;
    return cost;
}

// modify by "Initial_Legalization()"
void SA::Perturbation(SA &sa, Legalization_INFO &LG_INFO)
{
    /// step 1 initialize boundary
    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];

        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];
            Boundary_Assign_Limits(group_tmp.group_boundary);

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];
                Boundary_Assign_Limits(type_tmp.type_boundary);
            }
        }
    }

    // step 2 random to perturb one macro type of the a region

    int rendom_region = rand() % LG_INFO.region.size();

    ORDER_REGION &region_tmp = LG_INFO.region[rendom_region];

    for(int j = 0; j < (int)region_tmp.group.size(); j++)
    {
        ORDER_GROUP &group_tmp = region_tmp.group[j];

        for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
        {
            ORDER_TYPE &type_tmp = group_tmp.lef_type[k];
            for(int l = 0 ; l < (int)type_tmp.macro_id.size(); l++)
            {
                int macroid = type_tmp.macro_id[l];
                Macro* macro_temp = LG_INFO.MacroClusterSet[macroid];
                macro_temp->LegalFlag = false;
            }

        }
    }
    sa.SA_Perturb_region = rendom_region;

    // step 3 reandom to determine packing cost for rendom choose the paching corner
    // PARAMETER._PACK_  = 0.00-0.99s
    PARAMETER._PACK_WIRELENGTH      = rand()%100 / (float)100;
    PARAMETER._PACK_TYPE_DIST       = rand()%100 / (float)100;
    PARAMETER._PACK_CENTER_DIST     = rand()%100 / (float)100;
    PARAMETER._PACK_DISPLACEMENT    = rand()%100 / (float)100;
    PARAMETER._PACK_GROUP_DIST      = rand()%100 / (float)100;
    PARAMETER._PACK_OVERLAP         = rand()%100 / (float)100;
    PARAMETER._PACK_THICKNESS       = rand()%100 / (float)100;

    // step 4 update pre-placed macro to "AllCornerNode"
    UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, LG_INFO.AllCornerNode);

    // step 5 used Packing_Boundary_Ordering_Based_Legalization deal with the macros
    pair<int,int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);

    Packing_Boundary_Legalization(LG_INFO, WhiteCenter);

    // step 6 store the cost in the "LG_INFO.cost_container" at least
    Evaluate_Placement(LG_INFO);

    DeleteCornerNode(LG_INFO.AllCornerNode);

    if(Debug_PARA)
    {
        string filename = "./output/Refine/SA_";
        filename += int2str(sa.SA_iter);
        PlotMacroClusterSet(LG_INFO.MacroClusterSet, filename + ".m");
    }


}

void SA::StoredBestsol(SA &sa, Legalization_INFO &LG_INFO)
{
    for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        // LG_INFO.MacroClusterSet is store the Best solution for the output
        LG_INFO.MacroClusterSet[i]->lg = LG_INFO.Macro_Solution.back().at(i);
    }
}

void SA::Undo(SA &sa, Legalization_INFO &LG_INFO)
{
    ORDER_REGION &region_tmp = LG_INFO.region[sa.SA_Perturb_region];

    for(int j = 0; j < (int)region_tmp.group.size(); j++)
    {
        ORDER_GROUP &group_tmp = region_tmp.group[j];

        for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
        {
            ORDER_TYPE &type_tmp = group_tmp.lef_type[k];
            for(int l = 0 ; l < (int)type_tmp.macro_id.size(); l++)
            {
                int macroid = type_tmp.macro_id[l];
                Macro* macro_temp = LG_INFO.MacroClusterSet[macroid];
                macro_temp->LegalFlag = false;
            }

        }
    }

    // step 1 update pre-placed macro to "AllCornerNode"
    UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, LG_INFO.AllCornerNode);

    // step 2 used Packing_Boundary_Ordering_Based_Legalization deal with the macros
    pair<int,int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);

    Packing_Boundary_Legalization(LG_INFO, WhiteCenter);

    // step 3 store the cost in the "LG_INFO.cost_container" at least
    Evaluate_Placement(LG_INFO);

    DeleteCornerNode(LG_INFO.AllCornerNode);

    /// step 4 initialize boundary
    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];

        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];
            Boundary_Assign_Limits(group_tmp.group_boundary);

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];
                Boundary_Assign_Limits(type_tmp.type_boundary);
            }
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void REFINECONG::RandomSearch(REFINECONG &refine, Legalization_INFO &LG_INFO)
{
    // 1.record PARA PARAMETER
    USER_SPECIFIED_PARAMETER PARAMETER_tmp = PARAMETER;

    // 2.set macro's score_for_place_order (used for "Packing_Boundary_Ordering_Based_Legalization")
    for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = LG_INFO.MacroClusterSet[i];
        int center_dist = fabs(BenchInfo.WhiteSpaceXCenter - macro_temp->Macro_Center.first) + fabs(BenchInfo.WhiteSpaceYCenter - macro_temp->Macro_Center.second);

        macro_temp->score_for_place_order = center_dist * macro_temp->macro_area_ratio;
    }
    sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_place_order); // sort by macro area

    // 3.do cell cluster just one times


    // 3.rendom to get many result by modify PARA
    pthread_t threads[NUM_THREADS];
    int rc, t_num = 0;

    for(int i = 0 ; i < 1000 ;i++)
    {
        Legalization_INFO* LG_INFO_pointer = &LG_INFO;
        //rc = pthread_create(&threads[t_num], NULL, refine.RandomPARA, (void *)(LG_INFO_pointer));
        refine.RandomPARA(&LG_INFO);
        if(rc == 1){
                cout << "[Error] unable to create multi-thread " << rc << endl; exit(-1);
        }
        t_num++;

    }
        // thread join
    //for(int m = 0; m < t_num ; m++)
        //pthread_join(threads[m], NULL);

    // 4.find the best result
    refine.findBest(LG_INFO);

    // 5.RECOVER PARA
    PARAMETER = PARAMETER_tmp;

    // 6. output result
    PlotMacroClusterSet(LG_INFO.MacroClusterSet, "./output/DEF/Cong.m");
    //cout<<"[INFO] OUTPUT Congestion aware result in matlab & DEF File "<<endl;
    //Output_tcl(LG_INFO.MacroClusterSet, "./output/DEF/Cong.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);
    //WriteDefFile(LG_INFO.MacroClusterSet, "./output/DEF/Cong.def", BenchInfo.stdXcenter, BenchInfo.stdYCenter, def_file, false);
}

void* REFINECONG::RandomPARA(void* void_LG_INFO)
{
    Legalization_INFO LG_INFO_tmp = *(Legalization_INFO*)(void_LG_INFO);

    // PARAMETER._PACK_  = 0.00-0.99
    PARAMETER._PACK_WIRELENGTH      = rand()%100 / (float)100;
    PARAMETER._PACK_TYPE_DIST       = rand()%100 / (float)100;
    PARAMETER._PACK_CENTER_DIST     = rand()%100 / (float)100;
    PARAMETER._PACK_DISPLACEMENT    = rand()%100 / (float)100;
    PARAMETER._PACK_GROUP_DIST      = rand()%100 / (float)100;
    PARAMETER._PACK_OVERLAP         = rand()%100 / (float)100;
    PARAMETER._PACK_THICKNESS       = rand()%100 / (float)100;

    // 1. get a new purturb result to LG_INFO_tmp.MacroClusterSet
    UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, LG_INFO_tmp.AllCornerNode);

    pair<int,int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);

    if(rand() % 2 == 0 || !PARAMETER.PARTITION)
        Packing_Boundary_Ordering_Based_Legalization(LG_INFO_tmp, WhiteCenter); // according to Macro_Ordering to packing macro
    else
        Packing_Boundary_Legalization(LG_INFO_tmp, WhiteCenter);  // according to Macro region/group/type order to packing macro

    // cal cost after cell placement
    CalCongestion(LG_INFO_tmp);

    DeleteCornerNode(LG_INFO_tmp.AllCornerNode);

    // output macro placement result
    if(Debug_PARA)
    {
        string filename = "./output/Refine/Rand_";
        filename += int2str(LG_INFO_congestion.Macro_Solution.size());
        PlotMacroClusterSet(LG_INFO_tmp.MacroClusterSet, filename + ".m");
    }
    //pthread_exit(NULL);
}

void REFINECONG::CalCongestion(Legalization_INFO &LG_INFO_tmp)
{
    // 1. cell placement
    cout << "   Now Run CELL PLACEMENT STAGE"<< endl;
    CELLPLACEMENT cellpl;
    cellpl.CellPlacementFlow(&cellpl, LG_INFO_tmp, 0);

    //2. congestion Map
    CONGESTIONMAP congestion;

    COST_TERM cost_temp;
    cost_temp.Congestion = congestion.CongestionMapFlow(&congestion,false, 0);

    //cout << "[INFO] Total overflow: " << cost_temp.Congestion << endl;

    vector<Boundary> Macro_Solution;
    Macro_Solution.resize(LG_INFO_tmp.MacroClusterSet.size());

    vector<Macro*> &MacroCluster = LG_INFO_tmp.MacroClusterSet;
    for(int i = 0; i < (int)MacroCluster.size(); i++)
    {
        Macro* macro_temp = MacroCluster[i];
        Macro_Solution[i] = macro_temp-> lg; // record the solution of the macro
    }

    // Record Macro LG position to macro_list[] for each iteration
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro * this_macro = macro_list[macro_id];
        this_macro-> lg = LG_INFO_tmp.MacroClusterSet[i]-> lg;
    }

    LG_INFO_congestion.PARAMETER_container.push_back(PARAMETER);
    LG_INFO_congestion.cost_container.push_back(cost_temp);
    LG_INFO_congestion.Macro_Solution.push_back(Macro_Solution);
}

void REFINECONG::findBest(Legalization_INFO &LG_INFO)
{
    float best_cost = FLT_MAX;
    int best_ID = -1;

    //cout << LG_INFO_congestion.cost_container.size() <<endl;

    for(int i = 0; i < (int)LG_INFO_congestion.cost_container.size(); i++)
    {
        if(LG_INFO_congestion.cost_container[i].Congestion < best_cost)
        {
            best_cost = LG_INFO_congestion.cost_container[i].Congestion;
            best_ID = i;
        }
    }
    //cout << "[INFO] Best total overflow: "<< LG_INFO_congestion.cost_container[best_ID].Congestion << endl;

    // Record Macro LG position to MacroClusterSet
    for(int i = 0; i < (int)LG_INFO_congestion.MacroClusterSet.size(); i++)
    {
        // output to original "LG_INFO"
        LG_INFO.MacroClusterSet[i]-> lg = LG_INFO_congestion.Macro_Solution[best_ID][i];
    }


    LG_INFO.cost_container.clear();
    LG_INFO.Macro_Solution.clear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WSA::WSARefineFlow(WSA &wsa, Legalization_INFO &LG_INFO)
{
    // 1. cell placement
    cout << "   Now Run CELL PLACEMENT STAGE" << endl;
    CELLPLACEMENT cellpl;
    cellpl.CellPlacementFlow(&cellpl, LG_INFO, 0);

    // 2. congestion Map
    cout << "   Calaurate the congestion map after post-placemnt of cells" << endl;
    CONGESTIONMAP congestion;

    COST_TERM cost_temp;
    cost_temp.Congestion = congestion.CongestionMapFlow(&congestion, false, 0);

    //cout << "[INFO] Total overflow: " << cost_temp.Congestion << endl;

    // 3. Do the WSA to refine the GP by WSA (Cancel)
    cout << "   Do White Space Allocation" << endl;
    WhiteSpaceAllocationFlow(wsa, LG_INFO, congestion);
}



void WSA::WhiteSpaceAllocationFlow(WSA &wsa, Legalization_INFO &LG_INFO, CONGESTIONMAP &congestion)
{
    wsa.treeroot = new TreeNode;
    wsa.treeroot-> area_ori = chip_boundary;
    wsa.ver_cut = 0;
    wsa.hor_cut = 0;

    cout << "    Creat slicing tree for WSA" << endl;
    // 1. creat slicing tree from root
    Devide_Node(wsa.treeroot);

    // search ver_cut and hor_cut by DFS
    TreeNode* this_node = treeroot-> leftchild;
    while(this_node != NULL)
    {
        if(this_node-> vertial_cut) wsa.ver_cut++;
        else  wsa.hor_cut++;
        this_node = this_node-> leftchild;
    }
    //cout << "[INFO] Number of ver/hor cut of WSA: "<< wsa.ver_cut << "," << wsa.hor_cut << endl;

    // 2. calcurate all node's congestion
    CalNodeCong(congestion, LG_INFO, wsa.treeroot);

    // 3. normalize the congestion
    NormalizeCong(wsa.treeroot);

    // 4. change the area of each region
    ShiftingAccordingCong(wsa.treeroot);

    // 5. moving macro by area shifting
    MovingMacrosbyArea(wsa.treeroot);


    // IF u want to print all nodes coorinate in MATLAB
    if(Debug_PARA)
        Print_Node(wsa.treeroot);

    // IF u want to print macro loacation in MATLAB
    if(Debug_PARA)
        Plot_global(false, "./output/GP_graph/GP_refinebyWSA.m");

}

void WSA::Devide_Node(TreeNode* this_node) // Recurively do this
{
    float w = this_node-> area_ori.urx - this_node-> area_ori.llx;
    float h = this_node-> area_ori.ury - this_node-> area_ori.lly;
    //cout << "W " <<  w <<" H "<< h << endl;

    // Devide the region and creat new node
    this_node-> leftchild  = new TreeNode;
    this_node-> rightchild = new TreeNode;

    this_node-> leftchild -> parent = this_node;
    this_node-> rightchild-> parent = this_node;

    this_node-> leftchild -> area_ori = this_node-> area_ori;
    this_node-> rightchild-> area_ori = this_node-> area_ori;

    // ver cut
    if(w > h)
    {
        this_node-> leftchild -> vertial_cut = true;
        this_node-> rightchild-> vertial_cut = true;
        this_node-> leftchild -> area_ori.urx = (this_node-> area_ori.llx + this_node-> area_ori.urx)/2;
        this_node-> rightchild-> area_ori.llx = (this_node-> area_ori.llx + this_node-> area_ori.urx)/2;
        w/=2;
    }
    // hor cut
    else
    {
        this_node-> leftchild -> vertial_cut = false;
        this_node-> rightchild-> vertial_cut = false;
        this_node-> leftchild -> area_ori.lly = (this_node-> area_ori.lly + this_node-> area_ori.ury)/2;
        this_node-> rightchild-> area_ori.ury = (this_node-> area_ori.lly + this_node-> area_ori.ury)/2;
        h/=2;
    }
    // Dev END

    // recurive cut the region
    if(w > BenchInfo.min_macro_width || h > BenchInfo.min_macro_height)
        // a simpe devide for testing
        //if(w > 1000000 || h > 1000000)
    {
        Devide_Node(this_node-> leftchild);
        Devide_Node(this_node-> rightchild);
    }

}

void WSA::CalNodeCong(CONGESTIONMAP &congestion, Legalization_INFO &LG_INFO, TreeNode* this_node) // Preorder Traversal
{
    if(this_node != NULL)
    {
        // Calcurate this node congstion

        //cout << this_node-> area_ori.llx << " " << this_node-> area_ori.lly << " " << this_node-> area_ori.urx << " " << this_node-> area_ori.ury << endl;
        Boundary& this_bound = this_node-> area_ori;
        int start_H, start_V, end_H, end_V;

        start_H = (this_bound.lly - track.H_routing_track_ori_y ) / (track.H_routing_track_pitch * congestion.Hedge_track);
        start_V = (this_bound.llx - track.V_routing_track_ori_x ) / (track.V_routing_track_pitch * congestion.Vedge_track);
        end_H   = (this_bound.ury - track.H_routing_track_ori_y ) / (track.H_routing_track_pitch * congestion.Hedge_track);
        end_V   = (this_bound.urx - track.V_routing_track_ori_x ) / (track.V_routing_track_pitch * congestion.Vedge_track);

        // for all H_Edge (end_H need to reduce one)
        for (unsigned int k = start_H; k < end_H -1; k++)
        {
            for (unsigned int l = start_V; l < end_V; l++)
            {
                CONGESTIONMAP::EDGE *this_edge = congestion.H_Edge.at(k).at(l);
                if(this_edge-> demand > congestion.V_layer_num * congestion.Vedge_track * 1.25)
                    this_node-> cong_cost += (this_edge-> demand - congestion.V_layer_num * congestion.Vedge_track * 1.25);
            }

        }
        // for all V_Edge (end_V need to reduce one)
        for (unsigned int k = start_V; k < end_V -1; k++)
        {
            for (unsigned int l = start_H; l < end_H; l++)
            {
                CONGESTIONMAP::EDGE *this_edge = congestion.V_Edge.at(k).at(l);
                if(this_edge-> demand > congestion.H_layer_num * congestion.Hedge_track * 1.25)
                    this_node-> cong_cost += (this_edge-> demand - congestion.H_layer_num * congestion.Hedge_track * 1.25);
            }
        }
        //cout << this_node-> cong_cost << endl;

        // find the max cong to normalize cost (just consider the left node)
        if(this_node-> leftchild == NULL && this_node-> rightchild == NULL)
            if(this_node-> cong_cost > max_cong)
                max_cong = this_node-> cong_cost;

        // Cal END
        CalNodeCong(congestion, LG_INFO, this_node-> leftchild);
        CalNodeCong(congestion, LG_INFO, this_node-> rightchild);
    }
}

void WSA::NormalizeCong(TreeNode* this_node) // Preorder Traversal
{
    if(this_node != NULL)
    {
        // Normalize the cong

        // Normalize to 0-0.05
        this_node-> cong_cost *= (0.05 / (float)max_cong);
        // Normalize to 1-1.05
        this_node-> cong_cost += 1;

        // Normalize END
        NormalizeCong(this_node-> leftchild);
        NormalizeCong(this_node-> rightchild);
    }
}

void WSA::ShiftingAccordingCong(TreeNode* root) // BFS Traversal (Top-down)
{
    queue<TreeNode*> q;
    q.push(root);

    root-> area_after = root-> area_ori;

    while(q.empty() == false)
    {
        TreeNode* this_node = q.front();
        //cout << "Coor " << this_node-> area_ori.llx << " " << this_node-> area_ori.lly << " " << this_node-> area_ori.urx << " " << this_node-> area_ori.ury << endl;

        q.pop();
        if(this_node-> leftchild != NULL && this_node-> rightchild != NULL)
        {

            Boundary &left_bound_ori  = this_node-> leftchild-> area_ori;
            Boundary &right_bound_ori = this_node-> rightchild-> area_ori;
            Boundary &left_bound_after  = this_node-> leftchild-> area_after;
            Boundary &right_bound_after = this_node-> rightchild-> area_after;

            // update the value of parent area;
            left_bound_after = this_node-> area_after;
            right_bound_after = this_node-> area_after;

            // moving the cut lines

            float radio_area = (this_node-> leftchild-> cong_cost / (this_node-> leftchild-> cong_cost + this_node-> rightchild-> cong_cost) );
            //cout << "cost " << this_node-> leftchild-> cong_cost << " " << this_node-> rightchild-> cong_cost << endl;
            //cout << "radio " << radio_area << endl;

            if(this_node-> leftchild-> vertial_cut)
            {
                left_bound_after.urx = left_bound_after.llx + (left_bound_after.urx - left_bound_after.llx) * radio_area;

                right_bound_after.llx = left_bound_after.urx;
            }
            else
            {

                right_bound_after.ury = left_bound_after.lly + (left_bound_after.ury - left_bound_after.lly) * (1 - radio_area);
                left_bound_after.lly = right_bound_after.ury;
            }

            // Mov END
            q.push(this_node-> leftchild);
            q.push(this_node-> rightchild);
        }
    }
}

void WSA::MovingMacrosbyArea(TreeNode* root) // BFS Traversal (Top-down) twice
{
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro * this_macro = macro_list[macro_id];

        // 1. shrink the minimum searching tree
        TreeNode* minimumTree;
        queue<TreeNode*> q;
        q.push(root);
        while(q.empty() == false)
        {
            TreeNode* this_node = q.front();
            // Top-down search the minimum of the tree node is cover whole macro
            if(this_node-> area_ori.llx < this_macro-> lg.llx &&
               this_node-> area_ori.lly < this_macro-> lg.lly &&
               this_node-> area_ori.urx > this_macro-> lg.urx &&
               this_node-> area_ori.ury > this_macro-> lg.ury )
            {
                minimumTree = this_node;
            }
            // Seacch END
            q.pop();
            if(this_node-> leftchild != NULL && this_node-> rightchild != NULL)
            {
                q.push(this_node-> leftchild);
                q.push(this_node-> rightchild);
            }
        }
        //cout << minimumTree-> area_ori.llx << " " << minimumTree-> area_ori.lly << " " << minimumTree-> area_ori.urx << " " << minimumTree-> area_ori.ury << endl;
        //cout << "MACRO "<<this_macro-> lg.llx         << " " << this_macro-> lg.lly       << " " << this_macro-> lg.urx       << " " << this_macro-> lg.ury << endl;

        // 2. searching the cover node the find the grvity center and move macro
        double gravity_center_x = 0, gravity_center_y = 0, total_area = 0 ;

        q.push(minimumTree);
        while(q.empty() == false)
        {
            TreeNode* this_node = q.front();
            // Top-down search the leaf and if the node is overlap with the macro, add to the gravity and used to move the macro
            if(this_node-> leftchild == NULL && this_node-> rightchild == NULL)
                if( ! ( this_macro -> lg.urx < this_node-> area_ori.llx || this_macro -> lg.ury < this_node-> area_ori.lly  ||
                        this_node-> area_ori.urx < this_macro -> lg.llx || this_node-> area_ori.ury < this_macro -> lg.lly ) )
            {
                float node_area = (this_node-> area_after.urx - this_node-> area_after.llx) /(float)PARA * (this_node-> area_after.ury - this_node-> area_after.lly) /(float)PARA;

                //cout << "ORI " << this_node-> area_ori.llx << " " << this_node-> area_ori.lly << " " << this_node-> area_ori.urx << " " << this_node-> area_ori.ury << endl;
                //cout << "AFT " << this_node-> area_after.llx << " " << this_node-> area_after.lly << " " << this_node-> area_after.urx << " " << this_node-> area_after.ury << endl;
                //cout << "AREA " << node_area << endl;
                gravity_center_x += (this_node-> area_after.urx + this_node-> area_after.llx) /2 * node_area;
                gravity_center_y += (this_node-> area_after.ury + this_node-> area_after.lly) /2 * node_area;

                total_area += node_area;
            }

            // Move END

            q.pop();
            if(this_node-> leftchild != NULL && this_node-> rightchild != NULL)
            {
                q.push(this_node-> leftchild);
                q.push(this_node-> rightchild);
            }
        }
//cout << "TOT "<< total_area << endl;
        gravity_center_x /= total_area;
        gravity_center_y /= total_area;

//cout << "GRA " << gravity_center_x << " " << gravity_center_y << endl;

        // 3. moving macros
        this_macro-> gp_ori = this_macro-> gp;
        this_macro-> gp.llx = gravity_center_x - this_macro-> real_w /2;
        this_macro-> gp.lly = gravity_center_y - this_macro-> real_h /2;
        this_macro-> gp.urx = gravity_center_x + this_macro-> real_w /2;
        this_macro-> gp.ury = gravity_center_y + this_macro-> real_h /2;


    }

}

void WSA::Print_Node(TreeNode* root) // BFS Traversal (Top-down)
{
    queue<TreeNode*> q;
    q.push(root);

    int num = 1;
    vector<pair<string,Boundary> > plot_pair;
    vector<Boundary> plot_boundary;

    while(q.empty() == false)
    {
        TreeNode* this_node = q.front();

        // Top-down cout node info

        //cout << "Node "<<num << ": "<< this_node-> area_after.llx << " " << this_node-> area_after.lly << " " << this_node-> area_after.urx << " " << this_node-> area_after.ury << endl;
        num ++;
        if(this_node-> leftchild == NULL)
            plot_pair.push_back(make_pair( int2str(this_node-> cong_cost) ,this_node-> area_ori));
        else
             plot_pair.push_back(make_pair( "" ,this_node-> area_ori));

        plot_boundary.push_back(this_node-> area_after);
        // Cout END

        q.pop();
        if(this_node-> leftchild != NULL && this_node-> rightchild != NULL)
        {
            q.push(this_node-> leftchild);
            q.push(this_node-> rightchild);
        }
    }

    cout << "   Write WSA_region.m ..." << endl;
    Plot_rectangle_w_ID(plot_pair, "./output/GP_graph/WSA_region_ori.m" );
    Plot_rectangle(plot_boundary, "./output/GP_graph/WSA_region_shift.m" );

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////// for macro expand//////////////////////////////

#define OVERCONGESTION 10            // the expand condiction is overflow or not
// nor = 10
// PA5658A_TX = 0                   // the congestion is bad because GP
#define MAXEXPANDRADIO 0.25         // the maximun radio of the region of area/length

#define MINMACRORADIO 0.3           // the mini macro can be expand radio of region area/length without white-area/length constrain
#define MAXMACRORADIO 0.3           // the macro is bogger then the region * MAXMACROAREARADIO do not expand
// nor = 0.3
// PA5654 = 0.1
// PA5663 = 0.15

///////////////// for blk expand and repacking macros/////////////

#define OVERCONGESTIONBLK 100       // for the blockage is overflow or not
#define BLKEXPANDRADIO 0.5
//PA5658A_TX = 10up


void MACROEXPAND::MacroRefineFlow(MACROEXPAND macroexpand, Legalization_INFO &LG_INFO, int iter, CONGESTIONMAP &congestion)
{
    // 1. cell placement
    cout << "   Now Run CELL PLACEMENT STAGE" << endl;
    CELLPLACEMENT cellpl;
    cellpl.CellPlacementFlow(&cellpl, LG_INFO, iter);

    // 2. congestion Map
    cout << "   Calculate the congestion map after post-placement of cells" << endl;
    //CONGESTIONMAP congestion;

    //COST_TERM cost_temp;
    //cost_temp.Congestion = congestion.CongestionMapFlow(&congestion, false, iter);
    float total_overflow = congestion.CongestionMapFlow(&congestion, false, iter);

    //cout << "[INFO] Total overflow: " << total_overflow << endl;


    // upgrate the gp loactions and record the expand info. for the next expand method
    /*for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro* this_macro = LG_INFO.MacroClusterSet[i];

        // refine GP location
        //this_macro-> gp_ori = this_macro-> gp;
        //this_macro-> gp = this_macro-> lg;

        // record macro cal_w/cal_h
        this_macro-> w_before_expand = this_macro-> cal_w;
        this_macro-> h_before_expand = this_macro-> cal_h;
        this_macro-> Spacing_before_expand = this_macro-> Spacing;
    }*/

    //CalMacroTypeCong(congestion, LG_INFO);
/*
    // 3.[METHOD 1] expand by length
    cout << "  Now run expand macros" << endl;
    // 3-1.1 change the macro spacing calcurate by region length
    AddMacroSpacingbyLength(LG_INFO);

    // 3-1.2 refine macros by initial placment method
    LegalizationMacors(LG_INFO);

    // 3-1.3 output result
    PlotMacroClusterSet(LG_INFO.MacroClusterSet, "./output/TCL/Refine_1.m");
    cout<<"[INFO] OUTPUT Refinement result 1 in TCL & DEF File "<<endl;
    WriteDefFile(LG_INFO.MacroClusterSet, "./output/TCL/Refine_1.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file);

    // recover the macro W/H before re-run macro expand method 2
    for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro* this_macro = LG_INFO.MacroClusterSet[i];
        this_macro-> cal_h   = this_macro-> h_before_expand;
        this_macro-> cal_w   = this_macro-> w_before_expand;
        this_macro-> cal_h_wo_shrink = this_macro-> h_before_expand;
        this_macro-> cal_w_wo_shrink = this_macro-> w_before_expand;
        this_macro-> Spacing = this_macro-> Spacing_before_expand;
    }
*/

    // 3.[METHOD 2] expand by area
    //cout << "  Now run expand macros" << endl;
    // 3-2.0 calcurate the region white space without pre-placed macro
    //CalRegionWhiteSpace(&cellpl, LG_INFO);    //commented by m108yjhuang, due to this function is commented by m107jjchen.

    // 3-2.1. change the macro spacing calcurate by region area

    //AddMacroSpacingbyArea(LG_INFO);

    // 3-2.2 re-ordering the macros
    //MacroReOrdering(LG_INFO);

    // 3-2.2 refine macros by initial placment method
    //LegalizationMacors(LG_INFO);

    // 3-2.4 expand blockage and repacking covering macros
    //RepackingMacros(LG_INFO, congestion);

    // 3-2.5 output result

    //cout<<"[INFO] OUTPUT Refinement result in matlab & DEF File "<< endl;
    //PlotMacroClusterSet(LG_INFO.MacroClusterSet, "./output/Result/ans.m");

    //WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_W_blk.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, true);
    // PA5658A_TX and PA5658B do not add soft blockage
    //WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, false);

}

void MACROEXPAND::CalMacroTypeCong(CONGESTIONMAP &congestion, Legalization_INFO &LG_INFO)
{
    //cout << congestion.H_Edge.size() << " " << congestion.H_Edge[0].size() << endl;
    //cout << congestion.V_Edge.size() << " " << congestion.V_Edge[0].size() << endl;

    int start_H, start_V, end_H, end_V;

    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];

        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];

                // for all macros
                for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int macroID = type_tmp.macro_id[l];
                    Macro *macro_tmp = LG_INFO.MacroClusterSet[ macroID ];

                    //cout << macro_tmp-> macro_id << " " << macro_tmp-> lg.llx << " " << macro_tmp-> lg.lly << endl;

                    start_H = (macro_tmp-> lg.lly - track.H_routing_track_ori_y ) / (track.H_routing_track_pitch * congestion.Hedge_track);
                    start_V = (macro_tmp-> lg.llx - track.V_routing_track_ori_x ) / (track.V_routing_track_pitch * congestion.Vedge_track);
                    end_H   = (macro_tmp-> lg.ury - track.H_routing_track_ori_y ) / (track.H_routing_track_pitch * congestion.Hedge_track);
                    end_V   = (macro_tmp-> lg.urx - track.V_routing_track_ori_x ) / (track.V_routing_track_pitch * congestion.Vedge_track);

                    // for all H_Edge (end_H need to reduce one)
                    for (unsigned int k = start_H; k < end_H -1; k++)
                    {
                        for (unsigned int l = start_V; l < end_V; l++)
                        {
                            CONGESTIONMAP::EDGE *this_edge = congestion.H_Edge.at(k).at(l);
                            if(this_edge-> demand > congestion.V_layer_num * congestion.Vedge_track * 1.25)
                                type_tmp.cong_cost += (this_edge-> demand - congestion.V_layer_num * congestion.Vedge_track * 1.25);
                        }

                    }
                    // for all V_Edge (end_V need to reduce one)
                    for (unsigned int k = start_V; k < end_V -1; k++)
                    {
                        for (unsigned int l = start_H; l < end_H; l++)
                        {
                            CONGESTIONMAP::EDGE *this_edge = congestion.V_Edge.at(k).at(l);
                            if(this_edge-> demand > congestion.H_layer_num * congestion.Hedge_track * 1.25)
                                type_tmp.cong_cost += (this_edge-> demand - congestion.H_layer_num * congestion.Hedge_track * 1.25);
                        }
                    }

                }
                //cout << "type cong: "<< type_tmp.cong_cost << endl << endl;
            }
        }
    }
}

void MACROEXPAND::CalRegionWhiteSpace(CELLPLACEMENT *cellpl, Legalization_INFO &LG_INFO)
{
    /*
    // ref pair<int, int> Cal_WhiteSpaceCenter(vector<CornerNode*> AllCornerNode)

    for(int i = 0; i < (int)AllCornerNode.size(); i++)
    {
        CornerNode* corner_temp = AllCornerNode[i];
        if(corner_temp == NULL)
        {
            continue;
        }
        if(corner_temp->NodeType != Blank)
        {
            continue;
        }
        Boundary &blank_temp = corner_temp->rectangle;
        float blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
        TotalBlankSpaceArea += blank_NodeArea;
        BlankSpaceArea.push(blank_NodeArea);
        BlankRectangle.push(blank_temp);
    }*/
}

void MACROEXPAND::AddMacroSpacingbyLength(Legalization_INFO &LG_INFO)
{
    float length_region, macro_group_length, macro_type_length, macro_length, white_length, addspacing;
    int num_expand = 0;

    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];

        Boundary &region_boundary = region_tmp.p_region-> rectangle;
        length_region = region_boundary.urx - region_boundary.llx + region_boundary.ury - region_boundary.lly;

//cout << " Region length " << length_region << endl;

        macro_group_length = 0;
        for(int j = 0; j < (int)region_tmp.group.size(); j++)
            for(int k = 0; k < (int)region_tmp.group[j].lef_type.size(); k++)
                for(int l = 0; l < (int)region_tmp.group[j].lef_type[k].macro_id.size(); l++)
        {
            int macroID = region_tmp.group[j].lef_type[k].macro_id[l];
            Macro *macro_tmp = LG_INFO.MacroClusterSet[macroID];
            macro_group_length += macro_tmp-> cal_h + macro_tmp-> cal_w;
        }


        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];

                // if the overflow is bigger then OVERCONGESTION
                // uniform distribution the macro by spacing
                white_length = length_region - macro_group_length;

                Macro *macro_tmp = LG_INFO.MacroClusterSet[ type_tmp.macro_id[0] ];
                macro_length = macro_tmp-> cal_h + macro_tmp-> cal_w;
                macro_type_length = macro_length * type_tmp.macro_id.size();

                addspacing = white_length * ((float)macro_length / (float)macro_group_length);

                // maximun spacing constrain
                if(addspacing > length_region *MAXEXPANDRADIO ) addspacing = length_region *MAXEXPANDRADIO;

//cout  << " White space " << white_length << " macro length " << macro_length << " total length of macros " << macro_group_length << endl;

//if(type_tmp.cong_cost / (int)type_tmp.macro_id.size() > OVERCONGESTION && macro_length > length_region *MAXMACRORADIO)
    //for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
        //cout << "Macro " << type_tmp.macro_id[l] << " is cong, but too big" << endl;

                if(type_tmp.cong_cost / (int)type_tmp.macro_id.size() > OVERCONGESTION && macro_length < length_region *MAXMACRORADIO)
                {
                    // if white_length is > 0 or macro w/h is very small
                    if(addspacing > PARA || macro_length < length_region *MINMACRORADIO )
                    {
                        // if white_length is < 0, but macro w/h is very small
                        if(addspacing <= PARA && macro_length < length_region *MINMACRORADIO )
                        {

//for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
        //cout << type_tmp.macro_id[l] << " no spacing, but is small" << endl;

                            if(macro_tmp-> cal_h < macro_tmp-> cal_w)
                                addspacing = macro_tmp-> cal_h *MAXEXPANDRADIO;
                            else addspacing = macro_tmp-> cal_w *MAXEXPANDRADIO;
                        }

                        for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                        {
                            int macroID = type_tmp.macro_id[l];
                            Macro *macro_tmp = LG_INFO.MacroClusterSet[ macroID ];
//cout << "[INFO] expand macro: " << macro_tmp-> macro_id << " add space " << addspacing << endl;
                            num_expand++;
                            macro_tmp-> cal_h           += addspacing;
                            macro_tmp-> cal_w           += addspacing;
                            macro_tmp-> cal_w_wo_shrink += addspacing;
                            macro_tmp-> cal_h_wo_shrink += addspacing;
                            macro_tmp-> Spacing         += addspacing;
                        }
                    }
                }
            }
        }
    }
    //cout << "[INFO] Number of expand macro: " << num_expand <<endl;
}

void MACROEXPAND::AddMacroSpacingbyArea(Legalization_INFO &LG_INFO)
{
    float region_area, macro_group_area, macro_type_area, macro_area, white_area, addspacing;
    int num_expand = 0;

    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];

        Boundary &region_boundary = region_tmp.p_region-> rectangle;
        region_area = (region_boundary.urx - region_boundary.llx) /(float)PARA * (region_boundary.ury - region_boundary.lly) /(float)PARA;
        //cout << " Region area " << region_area << endl;

        // setting macro_group_area
        macro_group_area = 0;
        for(int j = 0; j < (int)region_tmp.group.size(); j++)
            for(int k = 0; k < (int)region_tmp.group[j].lef_type.size(); k++)
                for(int l = 0; l < (int)region_tmp.group[j].lef_type[k].macro_id.size(); l++)
        {
            int macroID = region_tmp.group[j].lef_type[k].macro_id[l];
            Macro *macro_tmp = LG_INFO.MacroClusterSet[macroID];
            macro_group_area += macro_tmp-> cal_h /PARA * macro_tmp-> cal_w /PARA;
        }

        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];


                // [NOTE] the white_area doesn't reduce the pre-placed macros !!!
                white_area = region_area - macro_group_area;

                Macro *macro_tmp = LG_INFO.MacroClusterSet[ type_tmp.macro_id[0] ];
                macro_area = macro_tmp-> cal_h /PARA * macro_tmp-> cal_w /PARA;
                macro_type_area = macro_area * type_tmp.macro_id.size();

                if(macro_tmp-> cal_h < macro_tmp-> cal_w)
                    addspacing = white_area *PARA * ((float)macro_area / (float)macro_group_area) / macro_tmp-> cal_w ;
                else
                    addspacing = white_area *PARA * ((float)macro_area / (float)macro_group_area) / macro_tmp-> cal_h ;

                // maximun spacing constrain
                if(addspacing > (region_boundary.urx - region_boundary.llx) *MAXEXPANDRADIO ) addspacing = (region_boundary.urx - region_boundary.llx) *MAXEXPANDRADIO;
                if(addspacing > (region_boundary.ury - region_boundary.lly) *MAXEXPANDRADIO ) addspacing = (region_boundary.ury - region_boundary.lly) *MAXEXPANDRADIO;
                //cout << " White space " << white_area << " macro area " << macro_area << " total area of macros " << macro_group_area << endl;

                if(Debug_PARA)
                    if(type_tmp.cong_cost / (int)type_tmp.macro_id.size() > OVERCONGESTION && macro_type_area > region_area *MAXMACRORADIO)
                        for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                            ////cout << "[INFO] macro " << type_tmp.macro_id[l] << " is cong, but too big" << endl;

                // if the overflow is bigger then OVERCONGESTION and the macro types are not too big
                if(type_tmp.cong_cost / (int)type_tmp.macro_id.size() > OVERCONGESTION && macro_type_area < region_area *MAXMACRORADIO)
                {
                    // if white_area is > 1000 or macro w/h is very small
                    if(addspacing > PARA || macro_area < region_area *MINMACRORADIO )
                    {
                        // if white_area is < 1000, but macro w/h is very small, expand it
                        if(macro_area < region_area *MINMACRORADIO)
                        {
                            if(Debug_PARA)
                                for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                                        ////cout << "[INFO] macro " << type_tmp.macro_id[l] << " is cong, and is small" << endl;

                            if(macro_tmp-> cal_h < macro_tmp-> cal_w)
                                addspacing = macro_tmp-> cal_h *MAXEXPANDRADIO;
                            else addspacing = macro_tmp-> cal_w *MAXEXPANDRADIO;
                        }

                        for(int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                        {
                            int macroID = type_tmp.macro_id[l];
                            Macro *macro_tmp = LG_INFO.MacroClusterSet[ macroID ];

                            if(Debug_PARA)
                                    //cout << "[INFO] expand macro: " << macro_tmp-> macro_id << " add space " << addspacing << endl;
                            num_expand++;
                            macro_tmp-> cal_h           += addspacing;
                            macro_tmp-> cal_w           += addspacing;
                            macro_tmp-> cal_w_wo_shrink += addspacing;
                            macro_tmp-> cal_h_wo_shrink += addspacing;
                            macro_tmp-> Spacing         += addspacing;
                        }

                    }
                }
            }
        }
    }
    //cout << "[INFO] Number of expand macro: " << num_expand <<endl;
}

void MACROEXPAND::MacroReOrdering(Legalization_INFO &LG_INFO)
{

    // case1 the next stage will use "Packing_Boundary_Ordering_Based_Legalization()" to packing amcros
    if(Debug_PARA){
        cout <<"[INFO] Before Macro re-ordering : ";
        for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
        {
            Macro* macro_temp = LG_INFO.Macro_Ordering[i];
            cout << macro_temp-> macro_id << " ";
        }cout << endl;
    }

    if(LG_INFO.legal_mode_0)
    {
        // input macro's score_for_place_order
        for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
        {
            Macro* macro_temp = macro_list[ MovableMacro_ID[i]];

            // for all std cells connected the macro
            float centerX= 0, centerY= 0, num_cell= 0;

            for(int j = 0; j < macro_temp-> NetID.size() ; j++)
                for(int k = 0 ; k< net_list[ macro_temp-> NetID[j] ].cell_idSet.size() ; k++)
            {
                Macro* this_Cell = macro_list[ net_list[ macro_temp-> NetID[j] ].cell_idSet[k] ];
                centerX += (this_Cell-> lg.urx + this_Cell-> lg.llx)/2 /PARA;
                centerY += (this_Cell-> lg.ury + this_Cell-> lg.lly)/2 /PARA;
                num_cell++;
            }
            //cout << i << " "<< num_cell <<endl;
            centerX /= num_cell;
            centerY /= num_cell;

            macro_temp-> score_for_place_order = pow( pow(fabs(BenchInfo.WhiteSpaceXCenter - centerX*PARA), 10) +
                                                      pow(fabs(BenchInfo.WhiteSpaceYCenter - centerY*PARA), 10) , 0.1);
        }
        sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_place_order);

        if(Debug_PARA){
            cout <<"[INFO] After Macro re-ordering : ";
            for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
            {
                Macro* macro_temp = LG_INFO.Macro_Ordering[i];
                cout <<  macro_temp-> macro_id << " ";
            }cout << endl;
        }
    }

    // case2 the next stage will use "Packing_Boundary_Legalization()" to packing amcros
    else
    {
        for(int i = 0; i < (int)LG_INFO.region.size(); i++)
        {
            ORDER_REGION &region_tmp = LG_INFO.region[i];
            Partition_Region &p_region = LG_INFO.PRegion[region_tmp.region_id];
            //region_tmp.p_region = &p_region;

            int chip_W = chip_boundary.urx - chip_boundary.llx;
            int chip_H = chip_boundary.ury - chip_boundary.lly;

            for(int j = 0; j < (int)region_tmp.group.size(); j++)
            {
                ORDER_GROUP &group = region_tmp.group[j];

                //group.macro_group = &MacroGroupBySC[group.group_id];

                group.score = group.max_dimension / (sqrt(pow(chip_W, 2) + pow(chip_H, 2)) / 2) + group.total_macro_area / BenchInfo.movable_macro_area + group.total_std_area / BenchInfo.std_cell_area;

                for(int k = 0; k < (int)group.lef_type.size(); k++)
                {
                    ORDER_TYPE &type_tmp = group.lef_type[k];

                    type_tmp.score += max(type_tmp.h, type_tmp.w) + type_tmp.pin_num / BenchInfo.max_pin_number + type_tmp.total_macro_area / BenchInfo.movable_macro_area + type_tmp.total_std_area / BenchInfo.std_cell_area;
                    sort(type_tmp.macro_id.begin(), type_tmp.macro_id.end());
                }
                sort(group.lef_type.begin(), group.lef_type.end(), cmp_type_score);
            }
            sort(region_tmp.group.begin(), region_tmp.group.end(), cmp_group_score);
        }
        //sort(LG_INFO.region.begin(), LG_INFO.region.end(), cmp_region_score);
    }
}


void MACROEXPAND::LegalizationMacors(Legalization_INFO &LG_INFO)
{

    UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, LG_INFO.AllCornerNode);

    pair<int,int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);

    // case1 using "Packing_Boundary_Ordering_Based_Legalization" to packing amcros
    if(LG_INFO.legal_mode_0)
        Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter);
    // case2 using "Packing_Boundary_Legalization" to packing amcros
    else
        Packing_Boundary_Legalization(LG_INFO, WhiteCenter);

    Evaluate_Placement(LG_INFO);
    DeleteCornerNode(LG_INFO.AllCornerNode);

    // initialize boundary

    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];

        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group_tmp = region_tmp.group[j];
            Boundary_Assign_Limits(group_tmp.group_boundary);

            for(int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group_tmp.lef_type[k];
                Boundary_Assign_Limits(type_tmp.type_boundary);
            }
        }
    }

    // initialize legal flag
    /*
    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        macro_temp->LegalFlag = false;

    }*/

    // update result to MacroClusterSet
    for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        LG_INFO.MacroClusterSet[i]->lg = LG_INFO.Macro_Solution.back().at(i);
    }

    // Record Macro LG position to macro_list[]
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro * this_macro = macro_list[macro_id];
        this_macro-> lg = LG_INFO.MacroClusterSet[i]-> lg;
    }

    // initial processing for the next perturb
    LG_INFO.PARAMETER_container.clear();
    LG_INFO.cost_container.clear();
    LG_INFO.Macro_Solution.clear();


}


void MACROEXPAND::RepackingMacros(Legalization_INFO &LG_INFO, CONGESTIONMAP &congestion)
{
    // 1. find all rip-up macros after blockage expand
    set<int>::iterator iter_set;
    set<int> ripup_macro;

    int start_H, start_V, end_H, end_V;
/*
    for(int i =0 ; i< PreplacedMacro_ID.size() ; i++)
    {
        Macro* this_blockage = macro_list[PreplacedMacro_ID[i]];
        // 1.1 cal blockages congestion
        start_H = (this_blockage-> lg.lly - track.H_routing_track_ori_y ) / (track.H_routing_track_pitch * congestion.Hedge_track);
        start_V = (this_blockage-> lg.llx - track.V_routing_track_ori_x ) / (track.V_routing_track_pitch * congestion.Vedge_track);
        end_H   = (this_blockage-> lg.ury - track.H_routing_track_ori_y ) / (track.H_routing_track_pitch * congestion.Hedge_track);
        end_V   = (this_blockage-> lg.urx - track.V_routing_track_ori_x ) / (track.V_routing_track_pitch * congestion.Vedge_track);

            float blk_overflow = 0;
            // 1.1.1 for all H_Edge (end_H need to reduce one)
            for (unsigned int k = start_H; k < end_H -1; k++)
            {
                for (unsigned int l = start_V; l < end_V; l++)
                {
                    CONGESTIONMAP::EDGE *this_edge = congestion.H_Edge.at(k).at(l);
                    if(this_edge-> demand > congestion.V_layer_num * congestion.Vedge_track * 1.25)
                        blk_overflow += (this_edge-> demand - congestion.V_layer_num * congestion.Vedge_track * 1.25);
                }

            }
            // 1.1.2 for all V_Edge (end_V need to reduce one)
            for (unsigned int k = start_V; k < end_V -1; k++)
            {
                for (unsigned int l = start_H; l < end_H; l++)
                {
                    CONGESTIONMAP::EDGE *this_edge = congestion.V_Edge.at(k).at(l);
                    if(this_edge-> demand > congestion.H_layer_num * congestion.Hedge_track * 1.25)
                        blk_overflow += (this_edge-> demand - congestion.H_layer_num * congestion.Hedge_track * 1.25);
                }
            }
        // 1.2 if the blockage tis congested expand it by the congested area
        Boundary Cong_blk_boundary;
        if(blk_overflow > OVERCONGESTIONBLK)
        {

            Cong_blk_boundary.llx = INT_MAX;
            Cong_blk_boundary.lly = INT_MAX;
            Cong_blk_boundary.urx = INT_MIN;
            Cong_blk_boundary.ury = INT_MIN;

            // 1.2.1 for all H_Edge (end_H need to reduce one)
            for (unsigned int k = start_H; k < end_H -1; k++)
            {
                for (unsigned int l = start_V; l < end_V; l++)
                {
                    CONGESTIONMAP::EDGE *this_edge = congestion.H_Edge.at(k).at(l);
                    if(this_edge-> demand > congestion.V_layer_num * congestion.Vedge_track * 1.25)
                    {
                        if(this_edge-> llx < Cong_blk_boundary.llx) Cong_blk_boundary.llx = this_edge-> llx;
                        if(this_edge-> urx > Cong_blk_boundary.urx) Cong_blk_boundary.urx = this_edge-> urx;
                        if(this_edge-> lly < Cong_blk_boundary.lly) Cong_blk_boundary.lly = this_edge-> lly;
                        if(this_edge-> ury > Cong_blk_boundary.ury) Cong_blk_boundary.ury = this_edge-> ury;
                    }
                }

            }
            // 1.2.2 for all V_Edge (end_V need to reduce one)
            for (unsigned int k = start_V; k < end_V -1; k++)
            {
                for (unsigned int l = start_H; l < end_H; l++)
                {
                    CONGESTIONMAP::EDGE *this_edge = congestion.V_Edge.at(k).at(l);
                    if(this_edge-> demand > congestion.H_layer_num * congestion.Hedge_track * 1.25)
                    {
                        if(this_edge-> llx < Cong_blk_boundary.llx) Cong_blk_boundary.llx = this_edge-> llx;
                        if(this_edge-> urx > Cong_blk_boundary.urx) Cong_blk_boundary.urx = this_edge-> urx;
                        if(this_edge-> lly < Cong_blk_boundary.lly) Cong_blk_boundary.lly = this_edge-> lly;
                        if(this_edge-> ury > Cong_blk_boundary.ury) Cong_blk_boundary.ury = this_edge-> ury;
                    }
                }
            }


            cout << "BLK region: " << Cong_blk_boundary.llx << " " <<  Cong_blk_boundary.lly << " " << Cong_blk_boundary.urx << " " << Cong_blk_boundary.ury << endl;

            // 1.3 expand blockage
            Cong_blk_boundary.llx -= (Cong_blk_boundary.urx - Cong_blk_boundary.llx) *BLKEXPANDRADIO;
            Cong_blk_boundary.lly -= (Cong_blk_boundary.ury - Cong_blk_boundary.lly) *BLKEXPANDRADIO;
            Cong_blk_boundary.urx += (Cong_blk_boundary.urx - Cong_blk_boundary.llx) *BLKEXPANDRADIO;
            Cong_blk_boundary.ury += (Cong_blk_boundary.ury - Cong_blk_boundary.lly) *BLKEXPANDRADIO;

            // 1.4 find cover macros by expanded blockage
            Boundary overlap;
            for(int j = 0; j< LG_INFO.MacroClusterSet.size() ; j++)
            {
                Macro* this_macro = LG_INFO.MacroClusterSet[j];

                overlap.llx = max(this_macro-> lg.llx, Cong_blk_boundary.llx);
                overlap.lly = max(this_macro-> lg.lly, Cong_blk_boundary.lly);
                overlap.urx = min(this_macro-> lg.urx, Cong_blk_boundary.urx);
                overlap.ury = min(this_macro-> lg.ury, Cong_blk_boundary.ury);
                if(overlap.llx < overlap.urx && overlap.lly < overlap.ury)  // is overlap
                {
                    ripup_macro.insert(j);
                }
            }
        }
        //else
            //cout << "This blockage is not cong" << endl;
    }


    //for PA5658A_TX rip-up macros
    ripup_macro.insert(61);ripup_macro.insert(62);ripup_macro.insert(72);ripup_macro.insert(73);
    ripup_macro.insert(74);ripup_macro.insert(75);ripup_macro.insert(82);ripup_macro.insert(91);ripup_macro.insert(93);
    ripup_macro.insert(88);ripup_macro.insert(89);ripup_macro.insert(90);ripup_macro.insert(92);ripup_macro.insert(99);
    ripup_macro.insert(110);ripup_macro.insert(111);ripup_macro.insert(112);ripup_macro.insert(113);
*/
    if(Debug_PARA)
        for(iter_set = ripup_macro.begin(); iter_set != ripup_macro.end(); iter_set++)
            //cout << "[INFO] Rip-up macro: " << *iter_set << endl;

    //cout << "[INFO] Number of Rip-up macros: " << ripup_macro.size() << endl;

    // 2. expand blockage and remove the cover macros
    for(iter_set = ripup_macro.begin(); iter_set != ripup_macro.end(); iter_set++)
    {
        Macro* this_macro = LG_INFO.MacroClusterSet[*iter_set];
        this_macro-> LegalFlag = false;
    }
    // used New PARA to packing macros
    PARAMETER._PACK_WIRELENGTH      = 0;
    PARAMETER._PACK_TYPE_DIST       = 1;
    PARAMETER._PACK_CENTER_DIST     = 0;
    PARAMETER._PACK_DISPLACEMENT    = 0;
    PARAMETER._PACK_GROUP_DIST      = 0;
    PARAMETER._PACK_OVERLAP         = 0;
    PARAMETER._PACK_THICKNESS       = 0;

    // 3. repacking the macros
    LegalizationMacors(LG_INFO);

}

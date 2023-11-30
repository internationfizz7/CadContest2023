#include "macro_legalizer.h"
#include "corner_stitching.h"
#include "QP_ordering.h"
static int stdYCenter, stdXCenter;

#define INITIALTIMES 1000   // initial iteration times = 1000
#define REFINETIMES 200      // refinement iteration times = 30

/// 2021.02
Boundary full_boundary;

/// ADD 2021.04
int Best_Placement_ID;
float Best_Placement_Cost;
bool Better = false;
//int ITER;   // for refinement iteration
//bool Build_Set = false;
//////

/// ADD 2021.08
vector<MacrosSet*> Priority_Queue;
//////

/// 2021.03
bool CornerNode_sort(CornerNode* a, CornerNode* b)
{
    if (a->rectangle.lly == b->rectangle.lly)
    {
        return a->rectangle.llx < b->rectangle.llx;
    }
    return a->rectangle.lly < b->rectangle.lly;
}

bool CornerNode_sort_llx(CornerNode* a, CornerNode* b)
{
    if (a->rectangle.llx == b->rectangle.llx)
    {
        return a->rectangle.lly < b->rectangle.lly;
    }
    return a->rectangle.llx < b->rectangle.llx;
}

bool CornerNode_sort_ury(CornerNode* a, CornerNode* b)
{
    if (a->rectangle.ury == b->rectangle.ury)
    {
        return a->rectangle.llx < b->rectangle.llx;
    }
    return a->rectangle.ury < b->rectangle.ury;
}

bool CornerNode_sort_urx(CornerNode* a, CornerNode* b)
{
    if (a->rectangle.urx == b->rectangle.urx)
    {
        return a->rectangle.lly < b->rectangle.lly;
    }
    return a->rectangle.urx < b->rectangle.urx;
}

bool Macro_sort_by_x(Macro* a, Macro* b)
{
    if (a->lg.llx == b->lg.llx)
    {
        return a->lg.lly < b->lg.lly;
    }
    return a->lg.llx < b->lg.llx;
}

bool Macro_sort_by_y(Macro* a, Macro* b)
{
    if (a->lg.lly == b->lg.lly)
    {
        return a->lg.llx < b->lg.llx;
    }
    return a->lg.lly < b->lg.lly;
}

bool Set_sort_by_ratio(MacrosSet* a, MacrosSet* b)
{
    return a->total_over_bbx < b->total_over_bbx;
}
//////

void MacroLegal()
{
    Coner_stitching_const(); // construct corner stitching (merge same weight tile)

    pair<int, int> w_Center = Cal_WhiteSpaceCenter(PreplacedInCornerStitching);

    BenchInfo.WhiteSpaceXCenter = w_Center.first;
    BenchInfo.WhiteSpaceYCenter = w_Center.second;

    // output will print the
    stdXCenter = BenchInfo.stdXcenter;
    stdYCenter = BenchInfo.stdYCenter;

    // setup the distance of all center cost to the std center of GP result
    BenchInfo.WhiteSpaceXCenter = BenchInfo.stdXcenter;
    BenchInfo.WhiteSpaceYCenter = BenchInfo.stdYCenter;

    /// ADD 2021.03.22
    /*DivideTilesToGrids();
    FixedPreplacedInCornerStitching.clear();
    UpdateFixedPreplacedInCornerStitching();*/
    //////

    IterativeMacroLegalizeFlow();
    //  Flat_Approach();

}

void IterativeMacroLegalizeFlow()
{
    Legalization_INFO LG_INFO;

    vector<Macro*>& MacroClusterSet = LG_INFO.MacroClusterSet;
    vector<CornerNode*>& AllCornerNode = LG_INFO.AllCornerNode;

    HIER_LEVEL* HIER_LEVEL_temp;            // store hier. tree info.
    vector<Macro_Group>  MacroGroupBySC;    // store macro group info.
    vector<ID_TABLE> id_table;
    vector<Partition_Region> P_Region;

    id_table.resize(macro_list.size());

    // Calcurate Congestion Map at GP position
    /*CONGESTIONMAP GP_congestion;
    if(Debug_PARA)
        GP_congestion.CongestionMapFlow(&GP_congestion,true);*/

        //          =======================================================================
        ///         ======================== stage 2 macro grouping  ======================
        //          =======================================================================

    cout << "\n========   Now Run MACROGROUPING STAGE\t========" << endl;
    HierGroup(MacroGroupBySC, HIER_LEVEL_temp);

    /// ID converter
    Const_ID_TABLE(id_table, MacroGroupBySC);

    // copy id_table to LG_INFO
    LG_INFO.ID_Table.resize(macro_list.size());
    for (int i = 0; i < id_table.size(); i++)
        LG_INFO.ID_Table[i] = id_table[i];

    // copy MacroGroupBySC to LG_INFO
    LG_INFO.MacroGroups.resize(MacroGroupBySC.size());
    for (int i = 0; i < MacroGroupBySC.size(); i++)
        LG_INFO.MacroGroups[i] = MacroGroupBySC[i];

    // print macro groups
    if (Debug_PARA)
        PLOTMG(MacroGroupBySC, id_table);

    /*************plus 108.05.17***********************/
    /// ADD group shifting according to dataflow // doesn't in partition ??
    if (dataflow_PARA == true)
    {
        // group_shifting(MacroGroupBySC);  // if it open will Segmentation fault
        if (Debug_PARA)
            Plot_subG(MacroGroupBySC, 2);      // Plot graph
    }
    /**************************************************/



    clock_t start_1, end_1;
    start_1 = clock();
    //          =======================================================================
    ///         =====================  stage 3 Recursive partition ====================
    //          =======================================================================

    cout << "\n========   Now Run PARTITION STAGE\t========" << endl;
    Do_Partition(HIER_LEVEL_temp, MacroGroupBySC, id_table, P_Region);

    HIER_LEVEL_Delete(HIER_LEVEL_temp);

    /// Construct movable macro's container MacroClusterSet different with the macro_list !
    ConstructMacroDeClusterSet(MacroClusterSet);
    Update_NET_INFO_IN_EVALUATION(LG_INFO);
    LG_INFO.Macro_Ordering = LG_INFO.MacroClusterSet;
    LG_INFO.MacroClusterSet_Cost.resize(MacroClusterSet.size());
    //    for(int i = 0; i < (int)MacroGroupBySC.size(); i++)
    //    {
    //        Macro_Group &mg_tmp = MacroGroupBySC[i];
    //        cout<<"ID : "<<i<<"\tstd ratio : "<<mg_tmp.StdAreaInGroup / BenchInfo.std_cell_area<<"\tmacro ratio : "<<mg_tmp.MacroArea / BenchInfo.movable_macro_area<<endl;
    //    }
    //    cout<<"\n\n";
    //    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    //    {
    //
    //        Macro* &macro_temp = MacroClusterSet[i];
    //        cout<<"ID : "<<i;
    //        cout<<"\tW : "<<macro_temp->cal_w_wo_shrink / BenchInfo.min_std_h;
    //        cout<<"\tH : "<<macro_temp->cal_h_wo_shrink / BenchInfo.min_std_h;
    //        cout<<"hier area : "<<macro_temp->HierStdArea<<endl;
    //    }
    //    cout<<"AREA end"<<endl;

    end_1 = clock();
    cout << "[INFO] Partition Runing Time: "<< (double)(end_1-start_1) /CLOCKS_PER_SEC << 's'<< endl;

    vector<ORDER_REGION>& order_region = LG_INFO.region;

    //          ========================================================================
    //          ========================  stage 3 Legailization ========================
    //          ========================================================================



    cout << "\n========   Now Run LEGALIZATION STAGE\t========" << endl;

    /// 1.Determine placed ordering of macros
    Legalization_Preprocessing(MacroClusterSet, order_region, MacroGroupBySC, P_Region, id_table);

    ///2021.02
    Boundary_Assign_Limits(full_boundary);
    for (int i = 0; i < LG_INFO.region.size(); i++)
    {
        ORDER_REGION& region_tmp = LG_INFO.region[i];
        Boundary& this_boundary = region_tmp.p_region->rectangle;
        Cal_BBOX(this_boundary, full_boundary);
    }

    cout << endl << "full boundary: " << endl;
    cout << full_boundary.llx << " " << full_boundary.lly << " " << full_boundary.urx << " " << full_boundary.ury << endl;
    //////
    vector<pair<int, int> > PRegion_Rip_Up_Point;
    //Calculate_Region_Rip_Up_Point(LG_INFO, AllCornerNode, PRegion_Rip_Up_Point, P_Region);
    cout << "[DEBUG] patition region num : " << P_Region.size() << endl;
    /*for(int i = 0; i < P_Region.size(); i++){
        cout << "[DEBUG] patition region " << i << "  node num : " << P_Region[i].node_ID.size() << endl;
        for(unsigned int j = 0; j < P_Region[i].node_ID.size(); j++){
            cout << "[DEBUG] node id : " << P_Region[i].node_ID[j] << endl;
        }
    }*/

    // 2.get perturb "PARA_PACK_" result
    Initial_Legalization(LG_INFO);

    // 3.find the best result
    Choose_Best_Placement(LG_INFO);

    /* /// CHECK FOR TYPE IN GROUPS
    for(int i = 0; i < (int)MacroGroupBySC.size(); i++)
    {
        cout<<"\tGroup :"<<i<<", size: "<<MacroGroupBySC[i].member_ID.size()<<endl;
        for(int j = 0; j < (int)MacroGroupBySC[i].member_ID.size(); j++)
        {
            int index = MacroGroupBySC[i].member_ID[j];
            cout<<macro_list[index]->macro_name<<", type: "<<macro_list[index]->lef_type_ID<<endl;
        }
        cout<<endl;
    }exit(1);*/

    cout << "[INFO] OUTPUT Macro Placement result in matlab & DEF File " << endl;
    //Output_tcl(LG_INFO.MacroClusterSet, "./output/TCL/Initial.tcl", stdXCenter, stdYCenter);
    PlotMacroClusterSet(MacroClusterSet, "./output/Result/ans.m");
    //PlotMacroClusterSet2(PRegion_Rip_Up_Point, MacroClusterSet, "./output/Result/ans.m");
    WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans.def", stdXCenter, stdYCenter, def_file, false); ///modify 108.03.26
    if (Congestion_PARA)
        WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_W_blk.def", stdXCenter, stdYCenter, def_file, true);

    end_1 = clock();
    cout << "[INFO] Legalization Runing Time: "<< (double)(end_1-start_1) /CLOCKS_PER_SEC << 's'<< endl;

    //          ========================================================================
    //          ========================  stage 3-1 Cell Legailization =================
    //          ========================================================================

    //cout << "\n========   Now Run CELL PLACEMENT STAGE\t========"<< endl;
    // MODIFY 2020.03.24 // Add cell placement to partition regions
    //CELLPLACEMENT cellpl;
    //if(Congestion_PARA == true)
    //    cellpl.CellPlacementFlow(&cellpl, P_Region);


    //          =======================================================================
    //          ======================== stage 4 Refinement ===========================
    //          =======================================================================

    if (Congestion_PARA)
        cout << "\n========   Now Run REFINEMENT STAGE\t========" << endl;

    // [Method 1] Using Simulate annealing to Refine placment result
    // ##SA has some thing wrong that result can't control
    // sa.Simulated_annealing(sa, LG_INFO);


    // [Method 2] Using initial placement method to find the the best cong. cost
    // ##the cong of cong. is not enought and the reusult is not well

    // REFINECONG refinement;
    // if(Congestion_PARA)
    //      refinement.RandomSearch(refinement, LG_INFO);

    // [Method 3] Using the WSA to refine macros
    // ## WSA is bad to moving the macro, and the moving by contour-aware method

    // WSA wsa;
    // if(Congestion_PARA)
    //      wsa.WSARefineFlow(wsa, LG_INFO);

    // [Method 4] macro spacing expand(O) to re-packe the macors
    //MACROEXPAND macroexpand;
    //if(Congestion_PARA)
    //    macroexpand.MacroRefineFlow(macroexpand, LG_INFO, 0);




    // refine by SIMULATED EVOLUTION ALGORITHM by You-Lung
    // this method is not well on big cases(ex PA5663), it doesn't packing organized
/*string filename = "./output/LG_graph/PlotMacroClusterSetSecondLG";
int iter = 9999;
            filename += int2str(iter);
            PlotMacroClusterSet(MacroClusterSet, filename + ".m");*/
            /*ofstream fout("check.m");
            fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

            //chip boundary
            fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
            fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
            fout << "fill(block_x, block_y, 'w');" << endl;
            for(int i = 0; i < (int)LG_INFO.region.size(); i++)
            {
                ORDER_REGION &region = LG_INFO.region[i];
                for(int j = 0; j < (int)region.group.size(); j++)
                {
                    ORDER_GROUP &group = region.group[j];
                    for(int k = 0; k < (int)group.lef_type.size(); k++)
                    {
                        ORDER_TYPE &type = group.lef_type[k];
                        for(int l = 0; l < (int)type.macro_id.size(); l++)
                        {
                            int index = type.macro_id[l];
                            Macro *macro = LG_INFO.MacroClusterSet[index];
                            cout<<macro->area<<endl;
                            fout<<"block_x=["<<macro->lg.llx<<" "<<macro->lg.llx<<" "<<macro->lg.urx<<" "<<macro->lg.urx<<" "<<macro->lg.llx<<" ];"<<endl;
                            fout<<"block_y=["<<macro->lg.lly<<" "<<macro->lg.ury<<" "<<macro->lg.ury<<" "<<macro->lg.lly<<" "<<macro->lg.lly<<" ];"<<endl;
                            fout << "fill(block_x, block_y,";
                            if(k%3 == 0)
                                fout<<" 'r');"<<endl;
                            else if(k%3 == 1)
                                fout<<" 'g');"<<endl;
                            else if(k%3 == 2)
                                fout<<" 'b');"<<endl;
                        }
                    }getchar();
                }
                //getchar();
            }
            fout.close();*/

    if (Congestion_PARA)
    {
        /// ADD 2021.03
        Refinement_Flag = true;     // for congestion, history cost
        int historical_cost = 0;
        Build_Set = true;
        Macros_Set.reserve(LG_INFO.MacroClusterSet.size());

        Priority_Queue.reserve(LG_INFO.MacroClusterSet.size());
        //////
        /// ADD 2021.03.22, in refinement stage cut tile into grid, 1/2, ...
        /*DivideTilesToGrids();
        FixedPreplacedInCornerStitching.clear();
        UpdateFixedPreplacedInCornerStitching();*/
        //////
        int iteration = 0;
        for (int iter = 0; iter < REFINETIMES; iter++)
        {
            //cout<<"PlotMacroClusterSet"<<endl;PlotMacroClusterSet(MacroClusterSet, "");getchar();
            /// ADD 2021.05
            ITER = iter;    // for CostEvaluation
            //////

            /// CongestionMap MODIFY 2021.03.18
            if (iter == 0 && (Refinement_Flag == true /*&& Better == true iter%20 == 0*/)) // cal. congestion in iter = 0/10/20
            {
                //historical_cost = 0;
                CG_INFO.H_Edge.clear();
                CG_INFO.V_Edge.clear();
                CG_INFO.ary_2pinnet.clear();

                //CONGESTIONMAP congestion;
                //float total_overflow = congestion.CongestionMapFlow(&CG_INFO, false, iter);
                //cout << "[INFO] Total overflow: " << total_overflow << endl;
                MACROEXPAND macroexpand;
                macroexpand.MacroRefineFlow(macroexpand, LG_INFO, iter, CG_INFO);
                //Better = false;
            }
            //////

            cout << "   Now running Refinement..(" << (iter + 1) << "/" << REFINETIMES << ")" << endl;

            pair<int, int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);

            UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode);
            // Simulated_Evolution(MacroClusterSet, order_region);
            //Simulated_Evolution_0819(LG_INFO, iter);

            MacrosSet* set_tmp;     // the target set of SE algorithm

            if (Build_Set && iter == 0)
            {
                if (Best_Placement_ID < INITIALTIMES / 2 || !PARAMETER.PARTITION)
                    Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter); // according "Macro_Ordering" to packing macro
                else
                    Packing_Boundary_Legalization(LG_INFO, WhiteCenter);

                Update_Sets_Info(AllCornerNode);    // calculate set information (i.e., bounding box Am/Ab, H/W tendency...)
                Check_Sets_Macros(LG_INFO);         // check each set and macro pointer
                // put sets into PQ
                for (int i = 0; i < (int)Macros_Set.size(); i++)
                {
                    MacrosSet* set_tmp = Macros_Set[i];
                    if (set_tmp->total_over_bbx <= 1.0)
                        Priority_Queue.push_back(set_tmp);
                }
                sort(Priority_Queue.begin(), Priority_Queue.end(), Set_sort_by_ratio);

                /// ADD 2021.07, calculate each partition rip-up point
                Calculate_Region_Rip_Up_Point(LG_INFO, AllCornerNode, PRegion_Rip_Up_Point, P_Region);
                //////
            }
            else if (Build_Set && iter > 0)
            {
                /// get PQ.front
                set_tmp = Priority_Queue.front();

                vector<MacrosSet*>::iterator it = Priority_Queue.begin();
                Priority_Queue.erase(it);

                /// apply SE for those macros in this set Si
                Simulated_Evolution_for_each_set(LG_INFO, Priority_Queue, set_tmp);

                if (!set_tmp->members.empty())
                    Update_Each_Set(set_tmp, AllCornerNode);
                //Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter); // according "Macro_Ordering" to packing macro
                Packing_Boundary_Ordering_Based_Legalization_for_each_set(LG_INFO, WhiteCenter, set_tmp); // according "Macro_Ordering" to packing macro
                /*if(Best_Placement_ID < 500)
                    Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter); // according "Macro_Ordering" to packing macro
                else
                    Packing_Boundary_Legalization(LG_INFO, WhiteCenter);*/

                    /// update all sets
                bool pointer_exist = false;
                for (int i = 0; i < (int)Macros_Set.size(); i++)
                {
                    MacrosSet* set = Macros_Set[i];

                    Update_Each_Set(set, AllCornerNode);
                    Check_Sets_Macros(LG_INFO);         // check each set and macro pointer

                    if (set_tmp == set)
                    {
                        pointer_exist = true;
                    }
                }
                /// update set Si and put it back PQ

                if (pointer_exist == true)
                {
                    /// put it back PQ
                    Priority_Queue.push_back(set_tmp);
                    sort(Priority_Queue.begin(), Priority_Queue.end(), Set_sort_by_ratio);
                }

                /// if create some new sets, put them into PQ
                for (int i = 0; i < (int)Macros_Set.size(); i++)
                {
                    MacrosSet* set_MS = Macros_Set[i];
                    bool new_set = true;
                    for (int j = 0; j < (int)Priority_Queue.size(); j++)
                    {
                        MacrosSet* set_PQ = Priority_Queue[j];
                        if (set_MS == set_PQ)
                        {
                            new_set = false;
                            break;
                        }
                    }
                    if (new_set == true)
                    {
                        Priority_Queue.push_back(set_MS);
                    }
                }

                /// remove set in PQ whose members is empty
                for (vector<MacrosSet*>::iterator i = Priority_Queue.begin(); i != Priority_Queue.end();)
                {
                    MacrosSet* tmp = (*i);
                    if (tmp->members.empty())
                    {
                        Priority_Queue.erase(i);
                    }
                    else
                        i++;
                }
                sort(Priority_Queue.begin(), Priority_Queue.end(), Set_sort_by_ratio);
            }

            Evaluate_Placement(LG_INFO);
            DeleteCornerNode(AllCornerNode);

            //print macro placement result
            if (Debug_PARA)
            {
                int out_iter = iteration++;
                string filename = "./output/LG_graph/PlotMacroClusterSetSecondLG";
                filename += int2str(out_iter);
                //PlotMacroClusterSet(MacroClusterSet, filename + ".m");
                PlotMacroClusterSet2(PRegion_Rip_Up_Point, MacroClusterSet, filename + ".m");

                string filename2 = "./output/Set_graph/Set_graph";
                filename2 += int2str(out_iter);
                PlotSet(filename2 + ".m");

                string filename3 = "./output/LG_Group_graph/LG_Group";
                filename3 += int2str(out_iter);
                PlotGroup(LG_INFO, filename3 + ".m");
            }

            /// ADD 2021.03.16 ///
            // update the historical cost
            if (Refinement_Flag == true)
            {
                //historical_cost++;
                for (int i = 0; i < (int)CG_INFO.H_Edge.size(); i++)
                {
                    for (int j = 0; j < (int)CG_INFO.H_Edge[i].size(); j++)
                    {
                        if (CG_INFO.H_Edge[i][j]->demand > CG_INFO.H_Edge[i][j]->capacity)
                        {
                            CG_INFO.H_Edge[i][j]->historical_cost++;
                        }
                    }
                }
                for (int i = 0; i < (int)CG_INFO.V_Edge.size(); i++)
                {
                    for (int j = 0; j < (int)CG_INFO.V_Edge[i].size(); j++)
                    {
                        if (CG_INFO.V_Edge[i][j]->demand > CG_INFO.V_Edge[i][j]->capacity)
                        {
                            CG_INFO.V_Edge[i][j]->historical_cost++;
                        }
                    }
                }
            }
            //////
            /// ADD 2021.03.22, in refinement stage cut tile into grid, 1/2, ...
            if (iter == 0)
            {
                /*DivideTilesToGrids();
                FixedPreplacedInCornerStitching.clear();
                UpdateFixedPreplacedInCornerStitching();*/
            }
            //////

            if (iter == 0)
            {
                /*LEMON_SolveNetworkFlow(LG_INFO);
                cout<<"[INFO] OUTPUT Bipartite Matching result in matlab & DEF File "<< endl;
                PlotMacroClusterSet_BM(LG_INFO.MacroClusterSet, "./output/Result/ans_net_degree_BM.m");

                WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_BM.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, false);
                if(Congestion_PARA)
                    WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_W_blk_BM.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, true);*/
            }

            // reset bool flag;
            for (int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
            {
                Macro* macro = MacroClusterSet[i];
                macro->ConsiderCal = true;
                macro->Rip_up_by_range = true;
            }
            //////
        }

        Choose_Best_Placement(LG_INFO);
        // re-place macro to corner stitching data structure, and update set information
        pair<int, int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);
        UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode);
        Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter);
        Update_Sets_Info(AllCornerNode);    // calculate set information (i.e., bounding box Am/Ab, H/W tendency...)
        PlotSet("");

        cout << "[INFO] OUTPUT Refinement result in matlab & DEF File " << endl;
        //PlotMacroClusterSet(LG_INFO.MacroClusterSet, "./output/Result/ans_REF.m");
        PlotMacroClusterSet2(PRegion_Rip_Up_Point, LG_INFO.MacroClusterSet, "./output/Result/ans_REF.m");

        //PlotSet("./output/Result/ans_REF_set.m");

        WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_REF.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, false);
        if (Congestion_PARA)
            WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_REF_W_blk.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, true);

        //PlotMacroClusterSet_BM(LG_INFO.MacroClusterSet, "./output/Result/ans_REF_net_degree.m");

        /// Bipartite matching flow ///
        //pair<int,int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);
        //UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode);
        //Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter);
        //Calculate_Region_Rip_Up_Point(LG_INFO, AllCornerNode, PRegion_Rip_Up_Point, P_Region);
        //for(int i = 0; i < (int)Macros_Set.size(); i++)
        //{
        //    Update_Each_Set(Macros_Set[i], AllCornerNode);
        //}
        //Check_Sets_Macros(LG_INFO);         // check each set and macro pointer
        PlotMacroClusterSet_BM(LG_INFO.MacroClusterSet, "./output/Result/ans_REF_net_degree_BM_before.m");
        LEMON_SolveNetworkFlow(LG_INFO);
        //        Bipartite_Matching_Flow(LG_INFO);
        cout << "[INFO] OUTPUT Bipartite Matching result in matlab & DEF File " << endl;
        PlotMacroClusterSet_BM(LG_INFO.MacroClusterSet, "./output/Result/ans_REF_net_degree_BM.m");
        PlotGroup(LG_INFO, "./output/Result/ans_REF_BM_group.m");

        WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_REF_BM.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, false);
        if (Congestion_PARA)
            WriteDefFile(LG_INFO.MacroClusterSet, "./output/Result/ans_REF_W_blk_BM.def", BenchInfo.stdYCenter, BenchInfo.stdXcenter, def_file, true);

        end_1 = clock();
        cout << "[INFO] Refinement Runing Time: "<< (double)(end_1-start_1) /CLOCKS_PER_SEC << 's'<< endl;
    }
}

/// ADD 2021.07
void Check_Tile(Legalization_INFO& LG_INFO) // for debug
{
    ofstream fout("Tile.m");
    fout << "figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n" << endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;
    for (int i = 0; i < (int)LG_INFO.AllCornerNode.size(); i++)
    {
        CornerNode* tmp = LG_INFO.AllCornerNode[i];
        if (tmp == NULL)
            continue;
        string color;
        double llx = tmp->rectangle.llx;
        double urx = tmp->rectangle.urx;
        double lly = tmp->rectangle.lly;
        double ury = tmp->rectangle.ury;
        if (tmp->NodeType == Blank)
            color = "y";
        else if (tmp->NodeType == Fixed)
            color = "c";
        else if (tmp->NodeType == Movable)
            color = "g";

        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'" << color << "','facealpha',0.5);" << endl;
    }
}

void Packing_Boundary_Ordering_Based_Legalization_for_each_set(Legalization_INFO& LG_INFO, pair<int, int> WhiteCenter, MacrosSet* set_tmp)
{
    queue<int> illegal_set;

    /// put the "no" rip-up macros to AllCornerNode
    for (int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        if (macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {
            // macro is placed, input to coner sitiching data structure
            int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
            CornerNode* starNode = LG_INFO.AllCornerNode[startID];

            ///2021.01
            vector<Macro*> Cut_macros;

            queue<CornerNode*> CornerNodeSetQ;
            vector<CornerNode*> CornerNodeSetV;
            Directed_AreaEnumeration(macro_temp->lg, starNode, CornerNodeSetQ);
            while (!CornerNodeSetQ.empty())
            {
                CornerNodeSetV.insert(CornerNodeSetV.begin(), CornerNodeSetQ.front()); //let y from low to high
                CornerNodeSetQ.pop();
            }
            for (int j = 0; j < CornerNodeSetV.size(); j++)
            {
                if (CornerNodeSetV[j]->rectangle.lly >= macro_temp->lg.ury)
                    continue;

                Macro* macro_tmp = new Macro;
                macro_tmp->lg.llx = max(macro_temp->lg.llx, CornerNodeSetV[j]->rectangle.llx);
                macro_tmp->lg.lly = max(macro_temp->lg.lly, CornerNodeSetV[j]->rectangle.lly);
                macro_tmp->lg.urx = min(macro_temp->lg.urx, CornerNodeSetV[j]->rectangle.urx);
                macro_tmp->lg.ury = min(macro_temp->lg.ury, CornerNodeSetV[j]->rectangle.ury);
                macro_tmp->lef_type_ID = macro_temp->lef_type_ID;
                macro_tmp->OriginalMacro = macro_temp;
                Cut_macros.push_back(macro_tmp);
            }
            //////

            ///2021.01
            Macro* cut_macro = macro_temp;
            for (int j = 0; j < Cut_macros.size(); j++)
            {
                cut_macro = Cut_macros[j];
                vector<CornerNode*> combination;
                combination.reserve(10);
                pair<int, int> macro_coor = make_pair(cut_macro->lg.llx, cut_macro->lg.lly);

                combination.push_back(CornerNodePointSearch(macro_coor, starNode));
                Find_topBlank(combination, cut_macro, cut_macro->lg.ury, cut_macro->lg);

                //Cal_BBOX(cut_macro->lg, type_tmp.type_boundary);
                //Cal_BBOX(cut_macro->lg, group_tmp.group_boundary);

                Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, cut_macro, combination);
            }
            //////
        }
    }

    /// update some new sets tendency after SE
    if (Build_Set && ITER > 0)
    {
        bool pointer_exist = false;
        for (int i = 0; i < (int)Macros_Set.size(); i++)
        {
            MacrosSet* set = Macros_Set[i];
            if (set_tmp == set)
            {
                pointer_exist = true;
                break;
            }
        }
        if (pointer_exist == true)
        {
            /// update set Si
            Update_Each_Set(set_tmp, LG_INFO.AllCornerNode);
            Check_Sets_Macros(LG_INFO);         // check each set and macro pointer
        }

        /// after SE, if create some new sets, update their tendency
        for (int i = 0; i < (int)Macros_Set.size(); i++)
        {
            MacrosSet* set_MS = Macros_Set[i];
            bool new_set = true;
            for (int j = 0; j < (int)Priority_Queue.size(); j++)
            {
                MacrosSet* set_PQ = Priority_Queue[j];
                if (set_MS == set_PQ)
                {
                    new_set = false;
                    break;
                }
            }
            if (new_set == true)
            {
                Update_Each_Set(set_MS, LG_INFO.AllCornerNode);
                Check_Sets_Macros(LG_INFO);         // check each set and macro pointer
            }
        }
    }
    //PlotSet("");getchar();
    NETLIST_QP nt;
    vector<Macro*> MacroOrder;
    UpdateNetlistQP(nt, LG_INFO);
    SolveQP(nt, LG_INFO);
    MacroRefineOrder(nt, MacroOrder);


    /// put the "rip-up" macros to AllCornerNode
    for (int i = 0; i < (int)MacroOrder.size(); i++)
    {
        Macro* macro_temp = MacroOrder[i];
        //cout << "Macro " << i << " ID : " << macro_temp->macro_id << endl;
        //getchar();
        if (macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {
            // do nothing
        }
        else
        {
            //            cout<<"refine : "<<macro_temp->macro_id<<endl;
            ORDER_REGION& region_tmp = LG_INFO.region[macro_temp->region_id];

            // Determine_PackDirection
            vector<float> occupy_ratio = Determine_PackDirection(LG_INFO.AllCornerNode, region_tmp.p_region->rectangle);

            // Select_Packing_Corner
            Select_Packing_Corner(LG_INFO, occupy_ratio, region_tmp.p_region->rectangle, macro_temp);


            if (macro_temp->LegalFlag == true)
            {
                // do nothing
            }
            else
            {
                illegal_set.push(macro_temp->macro_id);
            }
        }
    }
    //    getchar();

        // Leagalize_Illegal_Macro LG : illegal_set
    Leagalize_Illegal_Macro(LG_INFO, illegal_set);
}
void Simulated_Evolution_for_each_set(Legalization_INFO& LG_INFO, vector<MacrosSet*>& Priority_Queue, MacrosSet* set_tmp)
{
    vector<Macro*>& MacroClusterSet = LG_INFO.MacroClusterSet;
    vector<ORDER_REGION>& region = LG_INFO.region;
    /// Update Center
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            int group_CenterX = 0, group_CenterY = 0;
            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];
                float ratio = type_tmp.total_macro_area / group_tmp.total_macro_area;
                group_CenterX += type_tmp.type_center.first * ratio;
                group_CenterY += type_tmp.type_center.second * ratio;

            }
            group_tmp.group_center = make_pair(group_CenterX, group_CenterY);
        }
    }

    /// update cost
    vector<float> center_dist;
    vector<float> type_dist;
    vector<float> group_dist;

    center_dist.resize(set_tmp->members.size());
    type_dist.resize(set_tmp->members.size());
    group_dist.resize(set_tmp->members.size());

    pair<float, float> score_limits = make_pair(FLT_MIN, FLT_MAX);
    pair<float, float> max_min_center_dist = score_limits;
    pair<float, float> max_min_type_dist = score_limits;
    pair<float, float> max_min_group_dist = score_limits;
    pair<float, float> max_min_cong_cost = score_limits;


    /// calculate each term of the cost
    map<string, Macro*>::iterator it = set_tmp->members.begin();
    for (int i = 0; i < set_tmp->members.size(); i++)
    {
        Macro* macro_temp = it->second;

        // center_dist
        float& center_dist_temp = center_dist[i];
        int dist = (fabs(BenchInfo.WhiteSpaceXCenter - macro_temp->Macro_Center.first) + fabs(BenchInfo.WhiteSpaceYCenter - macro_temp->Macro_Center.second));
        dist *= macro_temp->macro_area_ratio;
        center_dist_temp = 1 / (float)max(dist, 1);
        max_min_center_dist.first = max(max_min_center_dist.first, center_dist_temp);
        max_min_center_dist.second = min(max_min_center_dist.second, center_dist_temp);

        pair<int, int>& group_center = region[macro_temp->region_id].group[macro_temp->group_id].group_center;
        pair<int, int>& type_center = region[macro_temp->region_id].group[macro_temp->group_id].lef_type[macro_temp->type_id].type_center;

        // type_dist
        float& type_dist_temp = type_dist[i];
        type_dist_temp = fabs(type_center.first - macro_temp->Macro_Center.first) + fabs(type_center.second - macro_temp->Macro_Center.second);
        max_min_type_dist.first = max(max_min_type_dist.first, type_dist_temp);
        max_min_type_dist.second = min(max_min_type_dist.second, type_dist_temp);

        // group_dist
        float& group_dist_temp = group_dist[i];
        group_dist_temp = fabs(group_center.first - macro_temp->Macro_Center.first) + fabs(group_center.second - macro_temp->Macro_Center.second);
        max_min_group_dist.first = max(max_min_group_dist.first, group_dist_temp);
        max_min_group_dist.second = min(max_min_group_dist.second, group_dist_temp);

        if (Refinement_Flag == true)
        {
            float cong_cost = 0;
            Boundary macro_boundary;
            Boundary_Assign(macro_boundary, macro_temp->lg.llx, macro_temp->lg.lly, macro_temp->lg.urx, macro_temp->lg.ury);
            cong_cost += CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boundary, true);
            cong_cost += CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boundary, true);
            max_min_cong_cost.first = max(max_min_cong_cost.first, cong_cost);
            max_min_cong_cost.second = min(max_min_cong_cost.second, cong_cost);
        }

        it++;
    }

    /// normalize each term of the cost
    float ceter_dist_range = max_min_center_dist.first - max_min_center_dist.second;
    float type_dist_range = max_min_type_dist.first - max_min_type_dist.second;
    float group_dist_range = max_min_group_dist.first - max_min_group_dist.second;
    float cong_cost_range = max_min_cong_cost.first - max_min_cong_cost.second;

    it = set_tmp->members.begin();
    for (int i = 0; i < set_tmp->members.size(); i++)
    {
        Macro* macro_temp = it->second;

        macro_temp->score = 0;

        float center_score;
        float type_score;
        float group_score;
        float cong_cost = 0;

        if (Refinement_Flag == true)
        {
            center_score = Scaling_Cost(ceter_dist_range, center_dist[i] - max_min_center_dist.second) * 0.3;
            type_score = Scaling_Cost(type_dist_range, type_dist[i] - max_min_center_dist.second) * 0.25;
            group_score = Scaling_Cost(group_dist_range, group_dist[i] - max_min_center_dist.second) * 0.25;

            Boundary macro_boundary;
            Boundary_Assign(macro_boundary, macro_temp->lg.llx, macro_temp->lg.lly, macro_temp->lg.urx, macro_temp->lg.ury);
            cong_cost += CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boundary, true);
            cong_cost += CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boundary, true);
            cong_cost = Scaling_Cost(cong_cost_range, cong_cost - max_min_cong_cost.second) * 0.2;
        }
        else
        {
            center_score = Scaling_Cost(ceter_dist_range, center_dist[i] - max_min_center_dist.second) * 0.4;
            type_score = Scaling_Cost(type_dist_range, type_dist[i] - max_min_center_dist.second) * 0.3;
            group_score = Scaling_Cost(group_dist_range, group_dist[i] - max_min_center_dist.second) * 0.3;
        }

        if (Refinement_Flag == true)
            macro_temp->score += center_score * 0.3 + type_score * 0.25 + group_score * 0.25 + cong_cost * 0.2;
        else
            macro_temp->score += center_score * 0.4 + type_score * 0.3 + group_score * 0.3;

        score_limits.first = max(score_limits.first, macro_temp->score);
        score_limits.second = min(score_limits.second, macro_temp->score);

        it++;
    }

    /// normalize score & determine ripped-up macros
    float range = score_limits.first - score_limits.second;
    int upper_limit = (int)set_tmp->members.size() * 0.5;
    set<Macro*> rip_up_count;

    for (map<string, Macro*>::iterator i = set_tmp->members.begin(); i != set_tmp->members.end(); i++)
    {
        Macro* macro_temp = i->second;

        if ((int)rip_up_count.size() > upper_limit)
            break;

        macro_temp->score = Scaling_Cost(range, macro_temp->score - score_limits.second);

        /// normalize score 0 - 1 to 0.1 - 0.9
        macro_temp->score = (macro_temp->score + 0.125) * 0.8;
        // random parameter r = 0.00 - 1.00
        float r = (float)(rand() % 101);
        r /= (float)100;

        // need to rip-up
        if (macro_temp->score > r)
        {
            macro_temp->ConsiderCal = false;
            rip_up_count.insert(macro_temp);

            /// ADD 2021.05, rip-up macros according to rectangle bounding box
            Boundary Macro_To_Chip_Center;
            ORDER_REGION& region_tmp = LG_INFO.region[macro_temp->region_id];

            Macro_To_Chip_Center.llx = min(macro_temp->lg.llx, region_tmp.p_region->rip_up_point.first);
            Macro_To_Chip_Center.lly = min(macro_temp->lg.lly, region_tmp.p_region->rip_up_point.second);
            Macro_To_Chip_Center.urx = max(macro_temp->lg.urx, region_tmp.p_region->rip_up_point.first);
            Macro_To_Chip_Center.ury = max(macro_temp->lg.ury, region_tmp.p_region->rip_up_point.second);

            for (map<string, Macro*>::iterator it = (macro_temp->set)->members.begin(); it != (macro_temp->set)->members.end(); it++)
            {
                Macro* macro = it->second;
                if (macro == macro_temp)
                    continue;
                if (macro->lg.urx <= Macro_To_Chip_Center.llx || macro->lg.llx >= Macro_To_Chip_Center.urx ||
                    macro->lg.ury <= Macro_To_Chip_Center.lly || macro->lg.lly >= Macro_To_Chip_Center.ury)
                {
                }
                else
                {
                    macro->Rip_up_by_range = false;
                    macro->ConsiderCal = false;
                    rip_up_count.insert(macro);
                }
            }

        }
        else
        {
            if (macro_temp->Rip_up_by_range == false)
            {

            }
            else
            {
                macro_temp->ConsiderCal = true;
            }
        }
    }

    /// for those should be ripped-up macros, remove from sets
    for (map<string, Macro*>::iterator i = set_tmp->members.begin(); i != set_tmp->members.end();)
    {
        Macro* macro = i->second;

        if (macro->set == NULL)
        {
            cout << "@@@, macro: " << macro->macro_name << endl;
            getchar();
            //continue;
        }
        if (macro->ConsiderCal == false)
        {
            // macro
            macro->set = NULL;
            // set
            set_tmp->members.erase(i++);
        }
        else
        {
            i++;
        }
    }

    /// if this set's members is empty, remove this set from Macros_Set and Priority_Queue
    for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); )
    {
        if ((*i)->members.empty())
        {
            for (vector<MacrosSet*>::iterator j = Priority_Queue.begin(); j != Priority_Queue.end(); )
            {
                if ((*i) == (*j))
                {
                    Priority_Queue.erase(j);
                    break;
                }
                else
                    j++;

            }
            Macros_Set.erase(i);
        }
        else
            i++;
    }

    /// after rip-up macros, one set might be separated to two or more sets.
    Revise_Sets();
    // re-assign id
    Re_Assign_Sets_ID();
    //////

    /// debug
    Check_Sets_Macros(LG_INFO);

    // update type boundary and group boundary
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            Boundary& group_boundary = group_tmp.group_boundary;
            Boundary_Assign_Limits(group_boundary);

            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];

                ConsideCal_Type_Boundary_Calculate(type_tmp, MacroClusterSet);
                Cal_BBOX(type_tmp.type_boundary, group_boundary);
            }
        }
    }

    // according the this order to re-packing macro one by one
    //sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_ordering);    /// MODIFY 2021.03.19, sort -> stable_sort
}

void Calculate_Region_Rip_Up_Point(Legalization_INFO& LG_INFO, vector<CornerNode*>& AllCornerNode, vector<pair<int, int> >& PRegion_Rip_Up_Point, vector<Partition_Region>& P_Region)
{
    //ofstream fout("enclose.m");
    vector<Macro*>& MacroClusterSet = LG_INFO.MacroClusterSet;
    vector<ORDER_REGION>& region = LG_INFO.region;
    PRegion_Rip_Up_Point.reserve(P_Region.size());

    if (region.size() == 1)  // means no partition, rip-up point is chip whitespace center
    {
        PRegion_Rip_Up_Point.push_back(make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter));
        return;
    }
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];
        Partition_Region* p_region = region_tmp.p_region;
        //cout<<p_region->rectangle.llx<<", "<<p_region->rectangle.lly<<"  "<<p_region->rectangle.urx<<", "<<p_region->rectangle.ury<<endl;

        /// find sets in this region
        //set<MacrosSet*> Sets_In_this_Region;
        Boundary enclose_all_sets;  // the bounding box can enclose all sets in this region
        Boundary_Assign_Limits(enclose_all_sets);
        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];

                for (int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int index = type_tmp.macro_id[l];
                    Macro* macro_tmp = MacroClusterSet[index];

                    //Sets_In_this_Region.insert(macro_tmp->set);
                    Cal_BBOX(macro_tmp->set->bbx, enclose_all_sets);
                }
            }
        }
        int color = 1;
        //plot_region(fout, enclose_all_sets, color);
        /// find blank tiles in this region
        int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
        CornerNode* startNode = AllCornerNode[startID];
        queue<CornerNode*> CornerNodeSet;

        Directed_AreaEnumeration(p_region->rectangle, startNode, CornerNodeSet);

        map<CornerNode*, float> Resize_CornerNodeSet; // second is the area of resized blank tile
        float total_area = 0;
        while (!CornerNodeSet.empty())
        {
            if (CornerNodeSet.front()->NodeType == Blank)
            {
                CornerNode* tmp = new CornerNode;
                tmp->rectangle = Overlap_Box(p_region->rectangle, CornerNodeSet.front()->rectangle);

                float area = ((float)(tmp->rectangle.urx - tmp->rectangle.llx) / PARA) * ((float)(tmp->rectangle.ury - tmp->rectangle.lly) / PARA);
                //printf("area: %f\n", area);
                if (area == 0)
                {
                    //cout<<tmp->rectangle.urx<<" - "<<tmp->rectangle.llx<<", "<<tmp->rectangle.ury<<" - "<<tmp->rectangle.lly<<endl;
                }
                else
                {
                    total_area += area;
                    Resize_CornerNodeSet.insert(make_pair(tmp, area));
                }
            }

            CornerNodeSet.pop();
        }

        /// according to resized blank tiles, calculate the whitespace center, and rip-up point is whitespace center
        float x, y; // whitespace center
        x = y = 0;

        for (map<CornerNode*, float>::iterator j = Resize_CornerNodeSet.begin(); j != Resize_CornerNodeSet.end(); j++)
        {
            CornerNode* tmp = j->first;
            float area_ratio = j->second / total_area;
            if (area_ratio <= 0 || area_ratio > 1)
            {
                cout << "[ERROR] Blank tile Area too small ! macro_legalizer.cpp Calculate_Region_Rip_Up_Point()...." << endl;
                cout << j->second << " / " << total_area << endl;
                exit(1);
            }

            x += (tmp->rectangle.urx + tmp->rectangle.llx) / 2 * area_ratio;
            y += (tmp->rectangle.ury + tmp->rectangle.lly) / 2 * area_ratio;
        }

        /// determine occupy_ratio of enclose_all_sets and adjust rip-up point
        vector<float> occupy_ratio = Determine_PackDirection(AllCornerNode, enclose_all_sets); // 0:up, 1:down, 2:left, 3:right; maximum value is 1.
        int count_num_ratio_greater_than_eta = 0;
        bool aspect_ratio_increase = false;
        bool aspect_ratio_decrease = false;
        float eta = 1;
        for (int j = 0; j < 4; j++)
        {
            if (occupy_ratio[j] >= eta)
                count_num_ratio_greater_than_eta += 1;
        }

        float max_ratio = numeric_limits<float>::min();
        int max_edge;
        int x_dist = 0;
        int y_dist = 0;

        switch (count_num_ratio_greater_than_eta)
        {
        case 0:
            for (int j = 0; j < 4; j++)
            {
                if (occupy_ratio[j] > max_ratio)
                {
                    max_ratio = occupy_ratio[j];
                    max_edge = j;
                }
            }
            if (max_edge == 0)    // up
            {
                if (y > enclose_all_sets.lly)    // move to lower
                    y = enclose_all_sets.lly;
            }
            else if (max_edge == 1)  // lower
            {
                if (y < enclose_all_sets.ury)    // move to up
                    y = enclose_all_sets.ury;
            }
            else if (max_edge == 2)  // left
            {
                if (x < enclose_all_sets.urx)    // move to right
                    x = enclose_all_sets.urx;
            }
            else if (max_edge == 3)   // right
            {
                if (x > enclose_all_sets.llx)    // move to left
                    x = enclose_all_sets.llx;
            }
            break;

        case 1:
            if (occupy_ratio[0] >= eta)          // up
            {
                if (y > enclose_all_sets.lly)    // move to lower
                    y = enclose_all_sets.lly;
            }
            else if (occupy_ratio[1] >= eta)     // lower
            {
                if (y < enclose_all_sets.ury)    // move to up
                    y = enclose_all_sets.ury;
            }
            else if (occupy_ratio[2] >= eta)     // left
            {
                if (x < enclose_all_sets.urx)    // move to right
                    x = enclose_all_sets.urx;
            }
            else if (occupy_ratio[3] >= eta)    // right
            {
                if (x > enclose_all_sets.llx)    // move to left
                    x = enclose_all_sets.llx;
            }
            break;

        case 2:
            if (occupy_ratio[0] >= eta && occupy_ratio[2] >= eta)    // upper left
            {
                if (x < enclose_all_sets.urx && y > enclose_all_sets.lly)    // whitespace center is in the set bounding box
                {
                    x_dist = enclose_all_sets.urx - x;
                    y_dist = y - enclose_all_sets.lly;

                    if (x_dist <= y_dist)
                        x = enclose_all_sets.urx;
                    else
                        y = enclose_all_sets.lly;
                }
                /*else
                {
                    if(x < enclose_all_sets.urx)    // move to right
                        x = enclose_all_sets.urx;
                    if(y > enclose_all_sets.lly)    // move to lower
                        y = enclose_all_sets.lly;
                }*/

            }
            else if (occupy_ratio[1] >= eta && occupy_ratio[2] >= eta)   // lower left
            {
                if (x < enclose_all_sets.urx && y < enclose_all_sets.ury)    // whitespace center is in the set bounding box
                {
                    x_dist = enclose_all_sets.urx - x;
                    y_dist = enclose_all_sets.ury - y;

                    if (x_dist <= y_dist)
                        x = enclose_all_sets.urx;
                    else
                        y = enclose_all_sets.ury;
                }
                /*else
                {
                    if(x < enclose_all_sets.urx)    // move to right
                        x = enclose_all_sets.urx;
                    if(y < enclose_all_sets.ury)    // move to upper
                        y = enclose_all_sets.ury;
                }*/

            }
            else if (occupy_ratio[0] >= eta && occupy_ratio[3] >= eta)   // upper right
            {
                if (x > enclose_all_sets.llx && y > enclose_all_sets.lly)    // whitespace center is in the set bounding box
                {
                    x_dist = x - enclose_all_sets.llx;
                    y_dist = y - enclose_all_sets.lly;

                    if (x_dist <= y_dist)
                        x = enclose_all_sets.llx;
                    else
                        y = enclose_all_sets.lly;
                }
                /*else
                {
                    if(x > enclose_all_sets.llx)    // move to left
                        x = enclose_all_sets.llx;
                    if(y > enclose_all_sets.lly)    // move to lower
                        y = enclose_all_sets.lly;
                }*/

            }
            else if (occupy_ratio[1] >= eta && occupy_ratio[3] >= eta)   // lower right
            {
                if (x > enclose_all_sets.llx && y < enclose_all_sets.ury)    // whitespace center is in the set bounding box
                {
                    x_dist = x - enclose_all_sets.llx;
                    y_dist = enclose_all_sets.ury - y;

                    if (x_dist <= y_dist)
                        x = enclose_all_sets.llx;
                    else
                        y = enclose_all_sets.ury;
                }
                /*else
                {
                    if(x > enclose_all_sets.llx)    // move to left
                        x = enclose_all_sets.llx;
                    if(y < enclose_all_sets.ury)    // move to upper
                        y = enclose_all_sets.ury;
                }*/

            }
            else
            {
                if ((occupy_ratio[0] >= eta && occupy_ratio[1] >= eta))
                {
                    // whitespace
                }
                else if ((occupy_ratio[2] >= eta && occupy_ratio[3] >= eta))
                {
                    // whitespace
                }
            }
            break;

        case 3:
            if (occupy_ratio[0] < eta)          // up
            {
                if (y < enclose_all_sets.ury)    // move to up
                    y = enclose_all_sets.ury;
            }
            else if (occupy_ratio[1] < eta)     // lower
            {
                if (y > enclose_all_sets.lly)    // move to lower
                    y = enclose_all_sets.lly;
            }
            else if (occupy_ratio[2] < eta)     // left
            {
                if (x > enclose_all_sets.llx)    // move to left
                    x = enclose_all_sets.llx;
            }
            else if (occupy_ratio[3] < eta)     // right
            {
                if (x < enclose_all_sets.urx)    // move to right
                    x = enclose_all_sets.urx;
            }
            break;

        default:
            cout << "line 1460, warning ! count_num_ratio_greater_than_eta = " << count_num_ratio_greater_than_eta << endl;
            exit(1);
            break;
        }

        /// assign rip-up point to p_region
        p_region->rip_up_point = make_pair(x, y);
        PRegion_Rip_Up_Point.push_back(make_pair(x, y));
    }
}
//////
/// ADD 2021.06
void PlotGroup(Legalization_INFO& LG_INFO, string filename)
{
    if (filename == "")
        filename = "Group.m";

    ofstream fout(filename.c_str());

    fout << "figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n" << endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for (int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION& region_tmp = LG_INFO.region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];

                for (int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int macro_id = type_tmp.macro_id[l];
                    string color;

                    if (j % 6 == 0) color = "r";
                    else if (j % 6 == 1) color = "g";
                    else if (j % 6 == 2) color = "b";
                    else if (j % 6 == 3) color = "c";
                    else if (j % 6 == 4) color = "m";
                    else if (j % 6 == 5) color = "y";

                    Macro* macro_temp = LG_INFO.MacroClusterSet[macro_id];

                    double llx = macro_temp->lg.llx;
                    double urx = macro_temp->lg.urx;
                    double lly = macro_temp->lg.lly;
                    double ury = macro_temp->lg.ury;
                    fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
                    fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;

                    fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << j << "-" << k << "','fontsize',7);" << endl << endl;
                    fout << "fill(block_x,block_y,'" << color << "','facealpha',0.5);" << endl;
                }
            }
        }
    }
}

void PlotSet(string filename)
{
    if (filename == "")
        filename = "Set.m";

    ofstream fout(filename.c_str());
    //ofstream fout("Set.m");

    fout << "figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n" << endl;
    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for (unsigned int i = 0; i < Placement_blockage.size(); i++)
    {
        double llx = Placement_blockage[i].llx;
        double urx = Placement_blockage[i].urx;
        double lly = Placement_blockage[i].lly;
        double ury = Placement_blockage[i].ury;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'c');" << endl;
    }

    int index = 0;
    for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        for (map<string, Macro*>::iterator j = (*i)->members.begin(); j != (*i)->members.end(); j++)
        {
            Macro* macro = j->second;
            fout << "block_x=[" << macro->lg.llx << " " << macro->lg.llx << " " << macro->lg.urx << " " << macro->lg.urx << " " << macro->lg.llx << " ];" << endl;
            fout << "block_y=[" << macro->lg.lly << " " << macro->lg.ury << " " << macro->lg.ury << " " << macro->lg.lly << " " << macro->lg.lly << " ];" << endl;

            if (index % 3 == 0)
                fout << "fill(block_x, block_y, 'r');" << endl;
            else if (index % 3 == 1)
                fout << "fill(block_x, block_y, 'g');" << endl;
            else if (index % 3 == 2)
                fout << "fill(block_x, block_y, 'b');" << endl;

            fout << "text(" << (macro->lg.llx + macro->lg.urx) / 2.0 << ", " << (macro->lg.lly + macro->lg.ury) / 2.0 << ", '" << (*i)->id << "','fontsize',8);" << endl << endl;
        }
        index++;
        int color = 0;
        plot_region(fout, (*i)->bbx, color);
    }
}

void PlotMacroClusterSet_BM(vector<Macro*> MacroClusterSet, string filename)
{
    if (filename == "")
        filename = "BM.m";

    ofstream fout(filename.c_str());

    //fout<<"figure('visible','off');\nclear;\nclf;\n";///show figure

    //fout<<"axis equal;\nhold on;\ngrid on;\n"<<endl;

    fout << "figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n" << endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for (int i = 0; i < (int)MacroClusterSet.size(); i++)
    {

        Macro* macro_temp = MacroClusterSet[i];
        string color;
        double llx = macro_temp->lg.llx;
        double urx = macro_temp->lg.urx;
        double lly = macro_temp->lg.lly;
        double ury = macro_temp->lg.ury;
        //        if(macro_temp->LegalFlag == true)
        //        {
        //            color = "r";
        //        }
        //        else
        //        {
        //            color = "k";
        //        }
        if (macro_temp->LegalFlag == false)
        {
            //            cout<<macro_temp->macro_id<<"false"<<endl;
            color = "y";
        }
        else
        {
            color = "g";
        }
        //        fout<<"line(["<< Refx <<" "<< llx <<"],["<< Refy <<" "<< lly <<"],'Color','r');"<<endl;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;

        // macro text print out 2019.12.23
                //if(MacroClusterSet.size() < 300)
        int net_degree = 0;
        set<string> connected;
        for (int j = 0; j < (int)macro_temp->NetID.size(); j++)
        {
            int net_index = macro_temp->NetID[j];
            //net_degree += net_list[net_index].macro_idSet.size() - 1 + net_list[net_index].cell_idSet.size();
            for (int m = 0; m < net_list[net_index].macro_idSet.size(); m++)
            {
                int macro_index = net_list[net_index].macro_idSet[m];
                Macro* tmp = macro_list[macro_index];
                connected.insert(tmp->macro_name);
            }
            for (int m = 0; m < net_list[net_index].cell_idSet.size(); m++)
            {
                int cell_index = net_list[net_index].cell_idSet[m];
                Macro* tmp = macro_list[cell_index];
                connected.insert(tmp->macro_name);
            }
        }
        net_degree = connected.size();

        fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << net_degree << "','fontsize',7);" << endl << endl;
        fout << "fill(block_x,block_y,'" << color << "','facealpha',0.5);" << endl;
    }
    //    getchar();
    for (unsigned int i = 0; i < Placement_blockage.size(); i++)
    {
        double llx = Placement_blockage[i].llx;
        double urx = Placement_blockage[i].urx;
        double lly = Placement_blockage[i].lly;
        double ury = Placement_blockage[i].ury;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'c');" << endl;
    }
    fout << "plot(" << BenchInfo.WhiteSpaceXCenter << "," << BenchInfo.WhiteSpaceYCenter << ",'r.','markersize',50);" << endl;
    fout << "plot(" << BenchInfo.stdXcenter << "," << BenchInfo.stdYCenter << ",'k.','markersize',25);" << endl;
}

void Bipartite_Matching_Flow(Legalization_INFO& LG_INFO)
{
    cout << "\n========   Now Run BIPARTITE MATCHING STAGE\t========" << endl;

    vector<ORDER_REGION>& region = LG_INFO.region;
    // 1. find the same type of macros in the set.
    vector<map<int, vector<Macro*> > > each_set_the_same_type_macros;    // map: first: lef type, second: those the same lef type macros
    each_set_the_same_type_macros.reserve(Macros_Set.size());
    for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        map<int, vector<Macro*> > the_same_type_macros;
        for (map<string, Macro*>::iterator j = (*i)->members.begin(); j != (*i)->members.end(); j++)
        {
            Macro* macro = j->second;
            int lef_id = macro->lef_type_ID;

            map<int, vector<Macro*> >::iterator it = the_same_type_macros.find(lef_id);
            if (it != the_same_type_macros.end())    // exist vector
            {
                it->second.push_back(macro);
            }
            else
            {
                vector<Macro*> tmp;
                tmp.push_back(macro);
                the_same_type_macros.insert(make_pair(lef_id, tmp));
            }
        }
        each_set_the_same_type_macros.push_back(the_same_type_macros);
    }

    // for the same type macros in each set, use bipartite matching
    for (int i = 0; i < (int)each_set_the_same_type_macros.size(); i++)
    {
        for (map<int, vector<Macro*> >::iterator j = each_set_the_same_type_macros[i].begin(); j != each_set_the_same_type_macros[i].end(); j++)
        {
            if (j->second.size() < 2)
                continue;

            // 2. calculate the cost of edges.
            vector<double> edge_cost;
            vector<double> type_dist;
            vector<double> group_dist;
            vector<double> net_degree_cost;
            vector<double> gp_dist;

            edge_cost.reserve((int)j->second.size() * (int)j->second.size());
            type_dist.reserve((int)j->second.size() * (int)j->second.size());
            group_dist.reserve((int)j->second.size() * (int)j->second.size());
            net_degree_cost.reserve((int)j->second.size() * (int)j->second.size());
            gp_dist.reserve((int)j->second.size() * (int)j->second.size());

            pair<double, double> score_limits = make_pair(FLT_MIN, FLT_MAX);
            pair<double, double> max_min_type_dist = score_limits;
            pair<double, double> max_min_group_dist = score_limits;
            pair<double, double> max_min_net_degree_cost = score_limits;
            pair<double, double> max_min_gp_dist = score_limits;
            //cout<<"------------------------------"<<endl;
            for (int k = 0; k < (int)j->second.size(); k++)
            {
                // calculate the net degree of target macro
                Macro* target = j->second[k];
                target->lg_no_bi_matching = target->lg;
                cout << k << " " << target->macro_name << endl;

                // net_degree
                int net_degree = 0;
                set<string> connected;
                cout << "NetID: " << target->NetID.size() << endl;
                for (int l = 0; l < (int)target->NetID.size(); l++)
                {
                    int net_index = target->NetID[l];
                    //net_degree += net_list[net_index].macro_idSet.size() - 1 + net_list[net_index].cell_idSet.size();
                    cout << "macro_idSet: " << net_list[net_index].macro_idSet.size() << ", cell_idSet: " << net_list[net_index].cell_idSet.size() << endl;

                    for (int m = 0; m < net_list[net_index].macro_idSet.size(); m++)
                    {
                        int macro_index = net_list[net_index].macro_idSet[m];
                        Macro* tmp = macro_list[macro_index];
                        connected.insert(tmp->macro_name);
                    }
                    for (int m = 0; m < net_list[net_index].cell_idSet.size(); m++)
                    {
                        int cell_index = net_list[net_index].cell_idSet[m];
                        Macro* tmp = macro_list[cell_index];
                        connected.insert(tmp->macro_name);
                    }
                }
                net_degree = connected.size();
                cout << "net_degree: " << net_degree << endl << endl;;
                double center_dist = (double)pow(pow(fabs(target->lg.llx - BenchInfo.WhiteSpaceXCenter) / (double)PARA, 2)
                    + pow(fabs(target->lg.lly - BenchInfo.WhiteSpaceYCenter) / (double)PARA, 2), 0.5);

                double target_degree_cost = center_dist * net_degree;


                for (int l = 0; l < (int)j->second.size(); l++)
                {
                    //double cost = 0;
                    if (k == l)
                    {
                        //edge_cost.push_back(cost);
                        //cout<<"  swap_to_cost: "<<0<<endl;
                        pair<int, int>& group_center = region[target->region_id].group[target->group_id].group_center;
                        pair<int, int>& type_center = region[target->region_id].group[target->group_id].lef_type[target->type_id].type_center;
                        // type_dist
                        double type_dist_temp;
                        type_dist_temp = fabs(type_center.first - target->Macro_Center.first) + fabs(type_center.second - target->Macro_Center.second);
                        max_min_type_dist.first = max(max_min_type_dist.first, type_dist_temp);
                        max_min_type_dist.second = min(max_min_type_dist.second, type_dist_temp);
                        type_dist.push_back(type_dist_temp);

                        // group_dist
                        double group_dist_temp;
                        group_dist_temp = fabs(group_center.first - target->Macro_Center.first) + fabs(group_center.second - target->Macro_Center.second);
                        max_min_group_dist.first = max(max_min_group_dist.first, group_dist_temp);
                        max_min_group_dist.second = min(max_min_group_dist.second, group_dist_temp);
                        group_dist.push_back(group_dist_temp);

                        // net_degree_cost
                        max_min_net_degree_cost.first = max(max_min_net_degree_cost.first, target_degree_cost);
                        max_min_net_degree_cost.second = min(max_min_net_degree_cost.second, target_degree_cost);
                        net_degree_cost.push_back(target_degree_cost);

                        // gp_dist
                        double gp_dist_temp;
                        gp_dist_temp = fabs(target->gp.llx - target->lg.llx) + fabs(target->gp.lly - target->lg.lly);
                        max_min_gp_dist.first = max(max_min_gp_dist.first, gp_dist_temp);
                        max_min_gp_dist.first = min(max_min_gp_dist.second, gp_dist_temp);
                        gp_dist.push_back(gp_dist_temp);
                        //continue;
                    }
                    else
                    {
                        Macro* destination = j->second[l];
                        //double swap_to_center_dist = (double)pow(pow(fabs(destination->lg.llx - BenchInfo.WhiteSpaceXCenter) / (double)PARA, 2)
                        //                                       + pow(fabs(destination->lg.lly - BenchInfo.WhiteSpaceYCenter) / (double)PARA, 2), 0.5 );
                        //double swap_to_displacement = (double)pow(pow(fabs(destination->lg.llx - target->gp.llx) / (double)PARA, 2)
                        //                                        + pow(fabs(destination->lg.lly - target->gp.lly) / (double)PARA, 2), 0.5 );
                        //double swap_to_degree_cost = (swap_to_center_dist * net_degree - target_degree_cost) + swap_to_displacement;
                        //edge_cost.push_back(swap_to_degree_cost);
                        //cout<<"  swap_to_cost: "<<swap_to_degree_cost<<endl;
                        pair<int, int>& group_center = region[destination->region_id].group[destination->group_id].group_center;
                        pair<int, int>& type_center = region[destination->region_id].group[destination->group_id].lef_type[destination->type_id].type_center;
                        // type_dist
                        double type_dist_temp;
                        type_dist_temp = fabs(type_center.first - destination->Macro_Center.first) + fabs(type_center.second - destination->Macro_Center.second);
                        max_min_type_dist.first = max(max_min_type_dist.first, type_dist_temp);
                        max_min_type_dist.second = min(max_min_type_dist.second, type_dist_temp);
                        type_dist.push_back(type_dist_temp);

                        // group_dist
                        double group_dist_temp;
                        group_dist_temp = fabs(group_center.first - destination->Macro_Center.first) + fabs(group_center.second - destination->Macro_Center.second);
                        max_min_group_dist.first = max(max_min_group_dist.first, group_dist_temp);
                        max_min_group_dist.second = min(max_min_group_dist.second, group_dist_temp);
                        group_dist.push_back(group_dist_temp);

                        // net_degree_cost
                        double swap_to_center_dist = (double)pow(pow(fabs(destination->lg.llx - BenchInfo.WhiteSpaceXCenter) / (double)PARA, 2)
                            + pow(fabs(destination->lg.lly - BenchInfo.WhiteSpaceYCenter) / (double)PARA, 2), 0.5);
                        double net_degree_cost_tmp = swap_to_center_dist * net_degree;
                        max_min_net_degree_cost.first = max(max_min_net_degree_cost.first, net_degree_cost_tmp);
                        max_min_net_degree_cost.second = min(max_min_net_degree_cost.second, net_degree_cost_tmp);
                        net_degree_cost.push_back(net_degree_cost_tmp);

                        // gp_dist
                        double gp_dist_temp;
                        gp_dist_temp = fabs(target->gp.llx - destination->lg.llx) + fabs(target->gp.lly - destination->lg.lly);
                        max_min_gp_dist.first = max(max_min_gp_dist.first, gp_dist_temp);
                        max_min_gp_dist.first = min(max_min_gp_dist.second, gp_dist_temp);
                        gp_dist.push_back(gp_dist_temp);
                    }
                }
                //getchar();
            }
            /// normalize each term of the cost
            double type_dist_range = max_min_type_dist.first - max_min_type_dist.second;
            double group_dist_range = max_min_group_dist.first - max_min_group_dist.second;
            double net_degree_cost_range = max_min_net_degree_cost.first - max_min_net_degree_cost.second;
            double gp_dist_range = max_min_gp_dist.first - max_min_gp_dist.second;
            //cout<<type_dist.size()<<" "<<group_dist.size()<<endl;getchar();
            for (int k = 0; k < (int)type_dist.size(); k++)
            {
                type_dist[k] = Scaling_Cost(type_dist_range, type_dist[k] - max_min_type_dist.second) * 0.5;
                group_dist[k] = Scaling_Cost(group_dist_range, group_dist[k] - max_min_group_dist.second) * 0.5;
                net_degree_cost[k] = Scaling_Cost(net_degree_cost_range, net_degree_cost[k] - max_min_net_degree_cost.second) * 0.01;
                gp_dist[k] = Scaling_Cost(gp_dist_range, gp_dist[k] - max_min_gp_dist.second) * 0.4;
                edge_cost.push_back(type_dist[k] + group_dist[k]);//+ net_degree_cost[k] + gp_dist[k]
            }

            // 3. set input, n = # of vertices, m = # of edges, n = 2*(# of the same type macros), m = (# of the same type macros)^2
            int n = 2 * (int)j->second.size();  // macro: 0~(int)j->second.size()-1, i and (int)j->second.size()+i are the same location.
            int m = (int)j->second.size() * (int)j->second.size();
            vector<pair<int, int> > edge;
            edge.reserve(m);
            int kk = 0;
            for (int k = 0; k < (int)j->second.size(); k++)
            {
                for (int l = 0; l < (int)j->second.size(); l++)
                {
                    cout << k << ", " << l + (int)j->second.size() << " " << edge_cost[kk++] << endl;
                    edge.push_back(make_pair(k, l + (int)j->second.size()));
                }
            }
            vector<pair<int, int> > result;
            result = Bipartite_Matching(n, m, edge, edge_cost);
            // 4. according to the result, swap macros
            for (int i = 0; i < (int)result.size(); i++)
            {
                //cout<<result[i].first<<" "<<result[i].second<<endl;
                int left_index = result[i].first;
                int right_index = result[i].second - (int)j->second.size();

                Macro* left_macro = j->second[left_index];
                Macro* right_macro = j->second[right_index];
                left_macro->lg = right_macro->lg_no_bi_matching;
            }
            //getchar();
        }
    }
}
//////

/// ADD 2021.05, 07
void Update_Each_Set(MacrosSet* set_tmp, vector<CornerNode*>& AllCornerNode)
{
    set_tmp->bbx.llx = numeric_limits<int>::max();
    set_tmp->bbx.lly = numeric_limits<int>::max();
    set_tmp->bbx.urx = numeric_limits<int>::min();
    set_tmp->bbx.ury = numeric_limits<int>::min();

    float total_area_tmp = 0;

    for (map<string, Macro*>::iterator j = set_tmp->members.begin(); j != set_tmp->members.end(); j++)
    {
        if (set_tmp->bbx.llx > j->second->lg.llx)     set_tmp->bbx.llx = j->second->lg.llx;
        if (set_tmp->bbx.lly > j->second->lg.lly)     set_tmp->bbx.lly = j->second->lg.lly;
        if (set_tmp->bbx.urx < j->second->lg.urx)     set_tmp->bbx.urx = j->second->lg.urx;
        if (set_tmp->bbx.ury < j->second->lg.ury)     set_tmp->bbx.ury = j->second->lg.ury;

        total_area_tmp += j->second->area;
    }

    // update set H/W
    set_tmp->H_W_ratio = (float)(set_tmp->bbx.ury - set_tmp->bbx.lly) / (set_tmp->bbx.urx - set_tmp->bbx.llx);

    // update total_area
    set_tmp->total_area = total_area_tmp;

    // update bbx_area
    set_tmp->bbx_area = (float)((set_tmp->bbx.urx - set_tmp->bbx.llx) / PARA) * ((set_tmp->bbx.ury - set_tmp->bbx.lly) / PARA);

    // calculate overlap area with pre-placed macro in bbx area
    float overlap_area = 0;
    int starID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];;
    CornerNode* starNode = AllCornerNode[starID];
    queue<CornerNode*> CornerNodeSet;
    Directed_AreaEnumeration(set_tmp->bbx, starNode, CornerNodeSet);

    while (!CornerNodeSet.empty())
    {
        CornerNode* corner_temp = CornerNodeSet.front();
        if (corner_temp->NodeType != Fixed)
        {
            CornerNodeSet.pop();
            continue;
        }

        Boundary overlap_box = Overlap_Box(set_tmp->bbx, corner_temp->rectangle);

        overlap_area += ((overlap_box.urx - overlap_box.llx) / PARA) * ((overlap_box.ury - overlap_box.lly) / PARA);

        CornerNodeSet.pop();
    }
    // update bbx_overlap_area
    set_tmp->bbx_overlap_area = overlap_area;

    // update total_over_bbx
    set_tmp->total_over_bbx = set_tmp->total_area / (set_tmp->bbx_area - set_tmp->bbx_overlap_area);
    //set_tmp->total_over_bbx = 1/(1 + exp( (-20)*( set_tmp->num_exe_se-30) ) ) + set_tmp->total_area / (set_tmp->bbx_area - set_tmp->bbx_overlap_area);

    // determine H/W tendency of the set
    vector<float> occupy_ratio = Determine_PackDirection(AllCornerNode, set_tmp->bbx); // 0:up, 1:down, 2:left, 3:right; maximum value is 1.
    set_tmp->occupy_ratio = occupy_ratio;

    int count_num_ratio_greater_than_eta = 0;
    float eta = 1;
    for (int j = 0; j < 4; j++)
    {
        if (occupy_ratio[j] >= eta)
            count_num_ratio_greater_than_eta += 1;
    }

    float max_ratio = numeric_limits<float>::min();
    int max_edge;

    switch (count_num_ratio_greater_than_eta)
    {
    case 0:
        for (int j = 0; j < 4; j++)
        {
            if (occupy_ratio[j] > max_ratio)
            {
                max_ratio = occupy_ratio[j];
                max_edge = j;
            }
        }
        if (max_edge == 0 || max_edge == 1)    // up or lower
        {
            set_tmp->aspect_ratio_increase = false;
            set_tmp->aspect_ratio_decrease = true;
        }
        else if (max_edge == 2 || max_edge == 3)   // left or right
        {
            set_tmp->aspect_ratio_increase = true;
            set_tmp->aspect_ratio_decrease = false;
        }
        break;

    case 1:
        if (occupy_ratio[0] >= eta || occupy_ratio[1] >= eta)    // up or lower
        {
            set_tmp->aspect_ratio_increase = false;
            set_tmp->aspect_ratio_decrease = true;
        }
        else if (occupy_ratio[2] >= eta || occupy_ratio[3] >= eta)    // left or right
        {
            set_tmp->aspect_ratio_increase = true;
            set_tmp->aspect_ratio_decrease = false;
        }
        break;

    case 2:
        if ((occupy_ratio[0] >= eta && occupy_ratio[2] >= eta) || (occupy_ratio[0] >= eta && occupy_ratio[3] >= eta) ||
            (occupy_ratio[1] >= eta && occupy_ratio[2] >= eta) || (occupy_ratio[1] >= eta && occupy_ratio[3] >= eta))
            // upper left // upper right // lower left // lower right
        {
            set_tmp->aspect_ratio_increase = true;
            set_tmp->aspect_ratio_decrease = true;
        }
        else
        {
            /*cout<<"In Update_Sets_Info case 2 wrong!"<<endl;
            cout<<"set id: "<<set_tmp->id<<endl;
            cout<<occupy_ratio[0]<<", "<<occupy_ratio[1]<<", "<<occupy_ratio[2]<<", "<<occupy_ratio[3]<<endl;
            PlotSet("");
            exit(1);*/
            if ((occupy_ratio[0] >= eta && occupy_ratio[1] >= eta))
            {
                set_tmp->aspect_ratio_increase = false;
                set_tmp->aspect_ratio_decrease = true;
            }
            else if ((occupy_ratio[2] >= eta && occupy_ratio[3] >= eta))
            {
                set_tmp->aspect_ratio_increase = true;
                set_tmp->aspect_ratio_decrease = false;
            }
        }
        break;

    case 3:
        if (occupy_ratio[0] <= eta || occupy_ratio[1] <= eta)    // up or lower
        {
            set_tmp->aspect_ratio_increase = true;
            set_tmp->aspect_ratio_decrease = true;
        }
        else if (occupy_ratio[2] <= eta || occupy_ratio[3] <= eta)    // left or right
        {
            set_tmp->aspect_ratio_increase = true;
            set_tmp->aspect_ratio_decrease = true;
        }
        break;

    default:
        cout << "warning ! count_num_ratio_greater_than_eta = " << count_num_ratio_greater_than_eta << endl;
        exit(1);
        break;
    }

    // set booling flag lazy update
    set_tmp->valid = true;
}
void Update_Sets_Info(vector<CornerNode*>& AllCornerNode)
{
    Re_Assign_Sets_ID();
    //vector<CornerNode*> AllCornerNode2;
    //UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode2);

    /*for(set<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        Update_Each_Set(*i, AllCornerNode);
    }*/
    for (int i = 0; i < (int)Macros_Set.size(); i++)
    {
        MacrosSet* set_tmp = Macros_Set[i];
        Update_Each_Set(set_tmp, AllCornerNode);
    }
    //DeleteCornerNode(AllCornerNode2);
    //getchar();
}
//////

void Update_NET_INFO(vector<Macro*>& MacroClusterSet)
{
    //    cout<<"Update Net Info"<<endl;
    map<int, int> macro_listIDvsMovableMacroID;
    for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        macro_listIDvsMovableMacroID.insert(make_pair(MovableMacro_ID[i], i));
    }

    for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        Macro* ori_macro_temp = macro_list[MovableMacro_ID[i]];
        macro_temp->NET_INFO.resize(ori_macro_temp->NetID.size());
        //        cout<<"Macro ID : "<<macro_temp->macro_id<<endl;
        for (int j = 0; j < (int)ori_macro_temp->NetID.size(); j++)
        {
            vector<Macro*>& NET_INFO_temp = macro_temp->NET_INFO[j];
            int net_id = ori_macro_temp->NetID[j];
            Net_QP& net_temp = net_list[net_id];
            NET_INFO_temp.resize(net_temp.macro_idSet.size());
            //            cout<<"NET "<<j<<" : "<<endl;
            for (int k = 0; k < (int)net_temp.macro_idSet.size(); k++)
            {
                Macro* connect_macro = macro_list[net_temp.macro_idSet[k]];
                NET_INFO_temp[k] = NULL;
                if (connect_macro->macro_type == MOVABLE_MACRO)
                {
                    map<int, int>::iterator iter = macro_listIDvsMovableMacroID.find(connect_macro->macro_id);
                    if (iter == macro_listIDvsMovableMacroID.end())
                    {
                        cout << "[ERROR] Can't find macro in update net info !!" << endl;
                        exit(1);
                    }
                    NET_INFO_temp[k] = MacroClusterSet[iter->second];

                }
                else if (connect_macro->macro_type == PRE_PLACED || connect_macro->macro_type == PORT)
                {


                    connect_macro->Macro_Center.first = (connect_macro->gp.llx + connect_macro->gp.urx) / 2;
                    connect_macro->Macro_Center.second = (connect_macro->gp.lly + connect_macro->gp.ury) / 2;
                    connect_macro->LegalFlag = true;
                    NET_INFO_temp[k] = connect_macro;
                }
                else
                {
                    cout << "[ERROR] please update net info." << endl;
                    exit(1);
                }
            }
        }
    }

}


void Legalization_Preprocessing(vector<Macro*>& MacroClusterSet, vector<ORDER_REGION>& region, vector<Macro_Group>& MacroGroupBySC,
    vector<Partition_Region>& P_Region, vector<ID_TABLE>& id_table)
{
    region.resize(P_Region.size());
    // step 1
    for (int i = 0; i < (int)P_Region.size(); i++)
    {
        ORDER_REGION& region_temp = region[i];
        region_temp.region_id = i;
        Partition_Region& p_region = P_Region[i];
        region_temp.group.resize(p_region.node_ID.size());
        float& max_macro_Dimension = region_temp.max_macro_Dimension;
        max_macro_Dimension = 0;

        for (int j = 0; j < (int)region_temp.group.size(); j++)
        {

            // 1-1. input region info
            int group_id = p_region.node_ID[j];
            Macro_Group& mg_tmp = MacroGroupBySC[group_id];

            ORDER_GROUP& group_temp = region_temp.group[j];
            group_temp.group_id = j;
            float& total_macro_area = group_temp.total_macro_area;
            float& total_std_area = group_temp.total_std_area;

            /**************plus 108.05.28*******************/
            /*
            int heir_size = mg_tmp.HierName.size();
            int num = min(dataflow_heir_num, heir_size);
            if(num < 3)
            {
                string heirName = "";
                for(int l = 0; l < num; l++)
                {
                    if(l == 0)
                    {
                        heirName = mg_tmp.HierName[l];
                        group_temp.heirName.push_back(heirName);
                        region_temp.Table_heir_groupID.insert(make_pair(heirName, j));
                    }
                    else
                    {
                        heirName =  heirName + "/" + mg_tmp.HierName[l];
                        group_temp.heirName.push_back(heirName);
                        region_temp.Table_heir_groupID.insert(make_pair(heirName, j));
                    }
                }
            }
            else
            {
                string heirName = "";
                for(int l = 0; l < 3; l++)
                {
                    if(l == 0)
                    {
                        heirName = mg_tmp.HierName[l];
                        group_temp.heirName.push_back(heirName);
                        region_temp.Table_heir_groupID.insert(make_pair(heirName, j));
                    }
                    else
                    {
                        heirName =  heirName + "/" + mg_tmp.HierName[l];
                        group_temp.heirName.push_back(heirName);
                        region_temp.Table_heir_groupID.insert(make_pair(heirName, j));
                    }
                }
            }
            */
            /**********************************************/


            // 1-2. find lef_type macro's id
            multimap<int, int> leftype_id;
            //            cout<<"in"<<endl;
            for (int k = 0; k < (int)mg_tmp.member_ID.size(); k++)
            {
                int in_macro_list_id = mg_tmp.member_ID[k];
                int in_macro_cluster_set_id = id_table[in_macro_list_id]._MovableMacro_ID;

                Macro* macro_temp = MacroClusterSet[in_macro_cluster_set_id];
                total_macro_area += macro_temp->area;
                total_std_area += macro_temp->HierStdArea;
                int max_dimension = max(macro_temp->cal_h_wo_shrink, macro_temp->cal_w_wo_shrink);
                if (max_dimension > group_temp.max_dimension)
                {
                    group_temp.max_dimension = max_dimension;
                }
                if (max_dimension > max_macro_Dimension)
                {
                    max_macro_Dimension = max_dimension;
                }
                leftype_id.insert(make_pair(macro_temp->lef_type_ID, macro_temp->macro_id));
            }

            //            cout<<"total_macro_area : "<<total_macro_area<<endl;
            //            cout<<"total_std_area : "<<total_std_area<<endl;
            //            cout<<"out"<<endl;
            //            cout<<"in"<<endl;
            int name_flag = -1;

            // 1-3. input  LG_INFO.region.group.lef_type.macro_id
            for (multimap<int, int>::iterator iter = leftype_id.begin(); iter != leftype_id.end(); iter++)
            {

                if (name_flag != iter->first)
                {
                    ORDER_TYPE type_tmp;
                    type_tmp.lef_type_name = iter->first;
                    type_tmp.type_id = (int)group_temp.lef_type.size();

                    Macro* macro_temp = MacroClusterSet[iter->second];
                    type_tmp.pin_num = (int)macro_temp->NetID.size();
                    type_tmp.h = macro_temp->cal_h_wo_shrink;
                    type_tmp.w = macro_temp->cal_w_wo_shrink;

                    name_flag = iter->first;
                    //                    cout<<iter->first<<"\t"<<iter->second<<endl;
                    //                    getchar();
                    for (; iter != leftype_id.end(); iter++)
                    {
                        Macro* macro_tmp = MacroClusterSet[iter->second];
                        type_tmp.total_macro_area += macro_tmp->area;
                        type_tmp.total_std_area += macro_tmp->HierStdArea;

                        if (name_flag != iter->first)
                        {
                            type_tmp.total_macro_area -= macro_tmp->area;
                            type_tmp.total_std_area -= macro_tmp->HierStdArea;
                            break;
                        }
                        type_tmp.macro_id.push_back(iter->second);
                        ///Update id
                    }
                    iter--;
                    //                    cout<<iter->first<<"\t"<<iter->second<<endl;
                    group_temp.lef_type.push_back(type_tmp);
                }
            }
            //            cout<<"out"<<endl;
        }
    }

    /// step 2 Calculate region score

    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];
        Partition_Region& p_region = P_Region[region_tmp.region_id];
        region_tmp.p_region = &p_region;
        int far_X = max(fabs(BenchInfo.WhiteSpaceXCenter - p_region.rectangle.llx), fabs(BenchInfo.WhiteSpaceXCenter - p_region.rectangle.urx));
        int far_Y = max(fabs(BenchInfo.WhiteSpaceYCenter - p_region.rectangle.lly), fabs(BenchInfo.WhiteSpaceYCenter - p_region.rectangle.ury));
        region_tmp.center_Euclid_Distance = sqrt(pow(far_X, 2) + pow(far_Y, 2));

        region_tmp.score = region_tmp.center_Euclid_Distance / region_tmp.max_macro_Dimension + region_tmp.max_macro_Dimension * 2;

        int chip_W = chip_boundary.urx - chip_boundary.llx;
        int chip_H = chip_boundary.ury - chip_boundary.lly;

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group = region_tmp.group[j];

            group.macro_group = &MacroGroupBySC[group.group_id];

            group.score = group.max_dimension / (sqrt(pow(chip_W, 2) + pow(chip_H, 2)) / 2) + group.total_macro_area / BenchInfo.movable_macro_area + group.total_std_area / BenchInfo.std_cell_area;

            for (int k = 0; k < (int)group.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group.lef_type[k];

                type_tmp.score += max(type_tmp.h, type_tmp.w) + type_tmp.pin_num / BenchInfo.max_pin_number + type_tmp.total_macro_area / BenchInfo.movable_macro_area + type_tmp.total_std_area / BenchInfo.std_cell_area;
                sort(type_tmp.macro_id.begin(), type_tmp.macro_id.end());

            }
            sort(group.lef_type.begin(), group.lef_type.end(), cmp_type_score);

        }

        sort(region_tmp.group.begin(), region_tmp.group.end(), cmp_group_score);
    }
    sort(region.begin(), region.end(), cmp_region_score);

    // check ordering
    /*
    for(int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION &region_tmp = LG_INFO.region[i];
        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP &group = region_tmp.group[j];
            for(int k = 0; k < (int)group.lef_type.size(); k++)
            {
                ORDER_TYPE &type_tmp = group.lef_type[k];
                cout << "TYPE TOTAL AREA " << type_tmp.score << endl;
                for(int l = 0; l < type_tmp.macro_id.size() ;l++)
                {
                    int macro_id = type_tmp.macro_id[l];
                    Macro *this_macro = LG_INFO.MacroClusterSet[macro_id];
                    cout << macro_id << " " << this_macro-> area << endl;
                }
            }
            cout << endl;
        }
        cout << "----" << endl;
    }*/

    /*************plus 108.05.28*****************/
    /*
    for(int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION &region_tmp = region[i];
        int df_score = 0;
        for(int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            if(j == 0)
            {
                region_tmp.group[j].dataflow_score = df_score;
                for(int k = region_tmp.group[j].heirName.size()-1; k >= 0; k--)
                {
                    string name = region_tmp.group[j].heirName[k];
                    map<string, set<string> >::iterator iter;
                    iter = DataFlow_outflow.find(name);
                }
                if(j!= (int)region_tmp.group.size() )
                {
                    for(int k = j; k < (int)region_tmp.group.size(); k++)
                    {
                        //region_tmp.group[k];

                    }
                }
            }
            else
            {

            }
            sort(region_tmp.group.begin(), region_tmp.group.end(), cmp_groupWdf_score);
        }

    }

    */
    /*******************************************/


    // step 3 update all LG_INFO info
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];
        region_tmp.region_id = i;
        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group = region_tmp.group[j];
            group.group_id = j;
            Boundary_Assign_Limits(group.group_boundary);
            for (int k = 0; k < (int)group.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group.lef_type[k];
                type_tmp.type_id = k;
                Boundary_Assign_Limits(type_tmp.type_boundary);
                ///update ID
                for (int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    Macro*& macro_temp = MacroClusterSet[type_tmp.macro_id[l]];
                    macro_temp->region_id = i;
                    macro_temp->group_id = j;
                    macro_temp->type_id = k;
                }
            }
        }
    }

}

bool cmp_region_score(ORDER_REGION a, ORDER_REGION b)
{
    return a.score > b.score;
}

bool cmp_group_score(ORDER_GROUP a, ORDER_GROUP b)
{
    return a.score > b.score;
}

bool cmp_type_score(ORDER_TYPE a, ORDER_TYPE b)
{
    return a.score > b.score;
}

/**************108.06.06***************/
bool cmp_groupWdf_score(ORDER_GROUP a, ORDER_GROUP b)
{
    if (a.dataflow_score == b.dataflow_score)
    {
        return a.score > b.score;
    }
    else
        return a.dataflow_score < b.dataflow_score;
}
/**************************************/

void Initial_Legalization(Legalization_INFO& LG_INFO)
{

    ///set PARAMETER
    USER_SPECIFIED_PARAMETER PARAMETER_tmp = PARAMETER;

    pair<int, int> WhiteCenter = make_pair(BenchInfo.WhiteSpaceXCenter, BenchInfo.WhiteSpaceYCenter);

    // input macro's score_for_place_order
    for (int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = LG_INFO.MacroClusterSet[i];
        int center_dist = fabs(BenchInfo.WhiteSpaceXCenter - macro_temp->Macro_Center.first) + fabs(BenchInfo.WhiteSpaceYCenter - macro_temp->Macro_Center.second);

        macro_temp->score_for_place_order = center_dist * macro_temp->macro_area_ratio;
    }
    sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_place_order); // sort by macro area

    // check macro ordering by area
    /*
    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        cout << macro_temp->area <<endl;
    }
    */

    ///// open the regio boundry to packing macro flatten //// 2020.07.14
/*
    ofstream fout("./output/subregion_graph/Region_modify.m");
    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    Boundary blank_box = Blank_BBOX(chip_boundary, PreplacedInCornerStitching);
    //cout <<[INFO] << "Blank region (llx lly urx ury): "<< blank_box.llx <<" " << blank_box.lly <<" " << blank_box.urx <<" " << blank_box.ury << endl;

    for(int i = 0 ; i < LG_INFO.region.size() ; i++)
    {
        Boundary &region_boundary = LG_INFO.region[i].p_region-> rectangle;

        int delta_W = region_boundary.urx - region_boundary.llx;
        int delta_H = region_boundary.ury - region_boundary.lly;

        // left region
        if(region_boundary.llx == blank_box.llx && region_boundary.lly != blank_box.lly && region_boundary.ury != blank_box.ury)
        {
            //region_boundary.urx -= delta_W * 0.25;
            region_boundary.ury += delta_H * 0.125;
            region_boundary.lly -= delta_H * 0.125;
            cout << "left region is open"<<endl;
        }
        // lower region
        else if(region_boundary.lly == blank_box.lly && region_boundary.llx != blank_box.llx && region_boundary.urx != blank_box.urx)
        {
            region_boundary.llx -= delta_W * 0.125;
            region_boundary.urx += delta_W * 0.125;
            //region_boundary.ury -= delta_H * 0.25;
            cout << "lower region is open"<<endl;

        }
        // upper region
        else if(region_boundary.ury == blank_box.ury && region_boundary.llx != blank_box.llx && region_boundary.urx != blank_box.urx)
        {
            region_boundary.llx -= delta_W * 0.125;
            region_boundary.urx += delta_W * 0.125;
            //region_boundary.lly += delta_H * 0.25;
            cout << "upper region is open"<<endl;
        }
        // reght region
        else if(region_boundary.urx == blank_box.urx && region_boundary.lly != blank_box.lly && region_boundary.ury != blank_box.ury)
        {
            //region_boundary.llx += delta_W * 0.25;
            region_boundary.ury += delta_H * 0.125;
            region_boundary.lly -= delta_H * 0.125;
            cout << "right region is open"<<endl;
        }

        if(region_boundary.llx < blank_box.llx) region_boundary.llx = blank_box.llx;
        if(region_boundary.lly < blank_box.lly) region_boundary.lly = blank_box.lly;
        if(region_boundary.urx > blank_box.urx) region_boundary.urx = blank_box.urx;
        if(region_boundary.ury > blank_box.ury) region_boundary.ury = blank_box.ury;

        // output matlab
        fout << "block_x=[" << region_boundary.llx << " " << region_boundary.llx << " " << region_boundary.urx
             << " " << region_boundary.urx << " " << region_boundary.llx << " ];" << endl;
        fout << "block_y=[" << region_boundary.lly << " " << region_boundary.ury << " " << region_boundary.ury
             << " " << region_boundary.lly << " " << region_boundary.lly << " ];" << endl;
        fout << "fill(block_x,block_y,'"<<"c');" << endl;
    }
    fout.close() ;
*/
    cerr << "    Now run Legalization ";
    for (int iter = 0; iter < INITIALTIMES; iter++)
    {
        //cerr<<"iter: "<<iter<<endl;
        if (iter % (INITIALTIMES / 10) == 0)
            cerr << (iter / (INITIALTIMES / 10) + 1) * 10 << "%" << " ";
        //        if(rand()%2 == 1)
        //        Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter);
        //        else
        //        cout<<"\t%%ITER : "<<iter<<"%%"<<endl;

        UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, LG_INFO.AllCornerNode);
        //if(rand() % 2 == 0 || !PARAMETER_tmp.PARTITION)
        if (iter < INITIALTIMES / 2 || !PARAMETER_tmp.PARTITION)
            Packing_Boundary_Ordering_Based_Legalization(LG_INFO, WhiteCenter); // according to Macro_Ordering to packing macro
        else
            Packing_Boundary_Legalization(LG_INFO, WhiteCenter);                // according to Macro region/group/type order to packing macro


        Evaluate_Placement(LG_INFO);
        /*ofstream fout("debug.m") ;
        fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

        //chip boundary
        fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
        fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
        fout << "fill(block_x, block_y, 'w');" << endl;
        for(int i = 0; i < (int)LG_INFO.AllCornerNode.size(); i++)
        {
            if(LG_INFO.AllCornerNode[i] == NULL)
                continue;
            fout<<"block_x=["<<LG_INFO.AllCornerNode[i]->rectangle.llx<<" "<<LG_INFO.AllCornerNode[i]->rectangle.llx<<" "<<LG_INFO.AllCornerNode[i]->rectangle.urx<<" "<<LG_INFO.AllCornerNode[i]->rectangle.urx<<" "<<LG_INFO.AllCornerNode[i]->rectangle.llx<<" ];"<<endl;
            fout<<"block_y=["<<LG_INFO.AllCornerNode[i]->rectangle.lly<<" "<<LG_INFO.AllCornerNode[i]->rectangle.ury<<" "<<LG_INFO.AllCornerNode[i]->rectangle.ury<<" "<<LG_INFO.AllCornerNode[i]->rectangle.lly<<" "<<LG_INFO.AllCornerNode[i]->rectangle.lly<<" ];"<<endl;
            fout << "text(" << (LG_INFO.AllCornerNode[i]->rectangle.llx + LG_INFO.AllCornerNode[i]->rectangle.urx) / 2.0 << ", " << (LG_INFO.AllCornerNode[i]->rectangle.lly + LG_INFO.AllCornerNode[i]->rectangle.ury) / 2.0 << ", '" << LG_INFO.AllCornerNode[i]->CornerNode_id << "');" << endl << endl;
            if(LG_INFO.AllCornerNode[i]->NodeType == Fixed)
            {
                fout<<"fill(block_x, block_y, 'r', 'facealpha', 0.5)"<<endl;
            }
            else if(LG_INFO.AllCornerNode[i]->NodeType == Blank)
            {
                fout<<"fill(block_x, block_y, 'c', 'facealpha', 0.5)"<<endl;
            }
            else if(LG_INFO.AllCornerNode[i]->NodeType == Movable)
            {
                fout<<"fill(block_x, block_y, 'b', 'facealpha', 0.5)"<<endl;
            }
        }*/
        DeleteCornerNode(LG_INFO.AllCornerNode);

        // PARAMETER._PACK_  = 0.00-0.99
        PARAMETER._PACK_WIRELENGTH = 0.1;//0; //rand()%100 / (float)100;
        PARAMETER._PACK_TYPE_DIST = rand() % 100 / (float)100;
        PARAMETER._PACK_CENTER_DIST = rand() % 100 / (float)100;
        PARAMETER._PACK_DISPLACEMENT = rand() % 100 / (float)100;
        PARAMETER._PACK_GROUP_DIST = rand() % 100 / (float)100;
        PARAMETER._PACK_OVERLAP = rand() % 100 / (float)100;
        PARAMETER._PACK_THICKNESS = rand() % 100 / (float)100;


        if (Debug_PARA)
        {
            string filename = "./output/Initial/Initial";
            filename += int2str(iter);
            PlotMacroClusterSet(LG_INFO.MacroClusterSet, filename + ".m");
        }


        /// initialize boundary
        for (int i = 0; i < (int)LG_INFO.region.size(); i++)
        {
            ORDER_REGION& region_tmp = LG_INFO.region[i];

            for (int j = 0; j < (int)region_tmp.group.size(); j++)
            {
                ORDER_GROUP& group_tmp = region_tmp.group[j];
                Boundary_Assign_Limits(group_tmp.group_boundary);

                for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
                {
                    ORDER_TYPE& type_tmp = group_tmp.lef_type[k];
                    Boundary_Assign_Limits(type_tmp.type_boundary);
                }
            }
        }

        /// initialize legal flag
        for (int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
        {
            Macro* macro_temp = LG_INFO.Macro_Ordering[i];

            macro_temp->LegalFlag = false;

        }
    }
    cout << endl;
    /// RECOVER PARA
    PARAMETER = PARAMETER_tmp;
}

void Select_Packing_Corner(Legalization_INFO& LG_INFO, vector<float>& occupy_ratio, Boundary& subregion, Macro*& macro_temp)
{
    //cout<<macro_temp->macro_id<<", subregion: "<<subregion.llx<<", "<<subregion.lly<<"   "<<subregion.urx<<", "<<subregion.ury<<endl;
    int starID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];;
    CornerNode* starNode = LG_INFO.AllCornerNode[starID];

    queue<CornerNode*> CornerNodeSet;
    Directed_AreaEnumeration(subregion, starNode, CornerNodeSet);
    ///for debug
    plot_Directed_AreaEnumeration(CornerNodeSet, subregion);
    vector<ORDER_REGION>& region = LG_INFO.region;

    PACKING_INFO pack_tmp;
    pack_tmp.Macro_temp = macro_temp;
    pack_tmp.region_boundary = subregion;
    pack_tmp.WhiteSpaceCenterX = BenchInfo.WhiteSpaceXCenter;
    pack_tmp.WhiteSpaceCenterY = BenchInfo.WhiteSpaceYCenter;
    pack_tmp.occupy_ratio = occupy_ratio;

    ORDER_GROUP& group_tmp = region[macro_temp->region_id].group[macro_temp->group_id];
    ORDER_TYPE& type_tmp = region[macro_temp->region_id].group[macro_temp->group_id].lef_type[macro_temp->type_id];
    pack_tmp.group_boundary = group_tmp.group_boundary;
    pack_tmp.type_boundary = type_tmp.type_boundary;

    COST_INFO cost_info; // the cost_info is for choose which corner to packing the macro

    while (!CornerNodeSet.empty())
    {
        CornerNode* corner_temp = CornerNodeSet.front();
        if (corner_temp->NodeType == Blank)
        {

            pack_tmp.ptr = corner_temp;

            PointPacking(LG_INFO.AllCornerNode, pack_tmp, cost_info);
        }

        CornerNodeSet.pop();
    }

    // select best coor
    Select_BestCoor(cost_info, macro_temp);

    if (macro_temp->LegalFlag == true)
    {
        /*vector<CornerNode*> combination;
        combination.reserve(10);
        pair<int, int> macro_coor = make_pair(macro_temp->lg.llx, macro_temp->lg.lly);

        LG_INFO.MacroClusterSet_Cost[macro_temp->macro_id] = cost_info.cost.front();
        cost_info.cost.pop();

        combination.push_back(CornerNodePointSearch(macro_coor, starNode));
        Find_topBlank(combination, macro_temp, macro_temp->lg.ury, macro_temp->lg);
        Cal_BBOX(macro_temp->lg, type_tmp.type_boundary);
        Cal_BBOX(macro_temp->lg, group_tmp.group_boundary);

        Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, macro_temp, combination);*/ /// COMMENT 2021.03.19

        ///2021.01
        vector<Macro*> Cut_macros;


        queue<CornerNode*> CornerNodeSetQ;
        vector<CornerNode*> CornerNodeSetV;
        Directed_AreaEnumeration(macro_temp->lg, starNode, CornerNodeSetQ);
        while (!CornerNodeSetQ.empty())
        {
            /*if(CornerNodeSetQ.front()->NodeType == Fixed)
            {
                cout<<"there is a fixed tile in CornerNodeSetQ"<<endl;
                cout<<"macro id: "<<macro_temp->macro_id<<endl;
                cout<<"macro->lg: "<<macro_temp->lg.llx<<", "<<macro_temp->lg.lly<<endl;
                getchar();
            }*/
            CornerNodeSetV.insert(CornerNodeSetV.begin(), CornerNodeSetQ.front()); //let y from low to high
            CornerNodeSetQ.pop();
        }
        for (int i = 0; i < CornerNodeSetV.size(); i++)
        {
            if (CornerNodeSetV[i]->rectangle.lly >= macro_temp->lg.ury)
                continue;

            Macro* macro_tmp = new Macro;
            macro_tmp->lg.llx = max(macro_temp->lg.llx, CornerNodeSetV[i]->rectangle.llx);
            macro_tmp->lg.lly = max(macro_temp->lg.lly, CornerNodeSetV[i]->rectangle.lly);
            macro_tmp->lg.urx = min(macro_temp->lg.urx, CornerNodeSetV[i]->rectangle.urx);
            macro_tmp->lg.ury = min(macro_temp->lg.ury, CornerNodeSetV[i]->rectangle.ury);
            macro_tmp->lef_type_ID = macro_temp->lef_type_ID;
            macro_tmp->OriginalMacro = macro_temp;
            Cut_macros.push_back(macro_tmp);
        }
        //////

        ///2021.01
        LG_INFO.MacroClusterSet_Cost[macro_temp->macro_id] = cost_info.cost.front();
        cost_info.cost.pop();

        Macro* cut_macro = macro_temp;
        for (int i = 0; i < Cut_macros.size(); i++)
        {
            cut_macro = Cut_macros[i];
            vector<CornerNode*> combination;
            combination.reserve(10);
            pair<int, int> macro_coor = make_pair(cut_macro->lg.llx, cut_macro->lg.lly);

            combination.push_back(CornerNodePointSearch(macro_coor, starNode));
            Find_topBlank(combination, cut_macro, cut_macro->lg.ury, cut_macro->lg);

            Cal_BBOX(cut_macro->lg, type_tmp.type_boundary);
            Cal_BBOX(cut_macro->lg, group_tmp.group_boundary);

            Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, cut_macro, combination);
        }
        //////

        /// ADD 2021.05, construct set
        if (Build_Set == true)
        {
            // 1. check there are any placed macro (movable) surrounding macro_temp.
            map<int, MacrosSet*> set_id;
            set_id = Find_Surrounding_Sets(LG_INFO.AllCornerNode, macro_temp, macro_temp->lg);

            // 2. if no, create a new set, and put macro_temp into this set.
            // 3. if yes, check the set of those surrounding macros, and put macro_temp into the set.
            // 4. if 3. more than one set, union those sets.

            if (set_id.size() == 0)  // 2.
            {
                // set
                MacrosSet* ms = new MacrosSet;
                ms->valid = false;
                ms->total_over_bbx = 0;
                ms->id = Macros_Set.size();
                ms->members.insert(make_pair(macro_temp->macro_name, macro_temp));

                Macros_Set.push_back(ms);

                // macro
                macro_temp->set = ms;
            }
            else if (set_id.size() >= 1) // 3. 4.
            {
                if (set_id.size() == 1)  // 3.
                {
                    // set
                    MacrosSet* target = set_id.begin()->second;
                    target->valid = false;
                    target->total_over_bbx = 0;
                    pair<map<string, Macro*>::iterator, bool> check;
                    check = target->members.insert(make_pair(macro_temp->macro_name, macro_temp));
                    if (check.second == false)
                    {
                        cout << "gg, line 1296, // 3" << endl;
                        cout << "macro: " << macro_temp->macro_name << endl;
                        exit(1);
                    }

                    // macro
                    macro_temp->set = target;
                }
                else    // 4.
                {
                    MacrosSet* target = set_id.begin()->second;
                    target->valid = false;
                    target->total_over_bbx = 0;
                    pair<map<string, Macro*>::iterator, bool> check;
                    check = target->members.insert(make_pair(macro_temp->macro_name, macro_temp));
                    if (check.second == false)
                    {
                        cout << "gg, line 1305, // 4" << endl;
                        cout << "macro: " << macro_temp->macro_name << endl;
                        exit(1);
                    }

                    // macro
                    macro_temp->set = target;

                    for (map<int, MacrosSet*>::iterator i = ++set_id.begin(); i != set_id.end(); i++)
                    {
                        // put elements to target
                        target->members.insert((i->second)->members.begin(), (i->second)->members.end());

                        // update the iterator in macro data structure
                        for (map<string, Macro*>::iterator j = (i->second)->members.begin(); j != (i->second)->members.end(); j++)
                        {
                            Macro* macro = j->second;
                            macro->set = target;
                        }

                        // erase this iterator
                        for (vector<MacrosSet*>::iterator j = Macros_Set.begin(); j != Macros_Set.end(); j++)
                        {
                            if (*j == i->second)
                            {
                                (*j)->members.clear();
                                Macros_Set.erase(j);
                                break;
                            }
                        }

                    }
                    // re-assign id
                    Re_Assign_Sets_ID();
                }
            }
        }
    }
}

void Select_BestCoor(COST_INFO& cost_info, Macro*& macro_temp)
{
    queue<COST_TERM>& cost = cost_info.cost;
    pair<int, int> BestCoor;
    float Displacement_Range = cost_info.Displacement.second - cost_info.Displacement.first;
    float Center_Distance_Range = cost_info.Center_Distance.second - cost_info.Center_Distance.first;
    float Group_Distance_Range = cost_info.Group_Distance.second - cost_info.Group_Distance.first;
    float Type_Distance_Range = cost_info.Type_Distance.second - cost_info.Type_Distance.first;
    float Packing_DeadSpace_Range = cost_info.Packing_DeadSpace.second - cost_info.Packing_DeadSpace.first;
    float Overlap_BoundaryArea_Range = cost_info.Overlap_BoundaryArea.second - cost_info.Overlap_BoundaryArea.first;
    float Thickness_Range = cost_info.Thickness.second - cost_info.Thickness.first;
    float WireLength_Range = cost_info.WireLength.second - cost_info.WireLength.first;  ///modify 108.04.20
    float Congestion_Range = 0; /// ADD 2021.03.19
    if (Refinement_Flag == true) /// ADD 2021.03.19
        Congestion_Range = cost_info.Congestion.second - cost_info.Congestion.first;  /// ADD 2021.03.19
    float Regularity_Range = 0;
    if (Build_Set == true && ITER > 0)
        Regularity_Range = cost_info.Regularity.second - cost_info.Regularity.first;    /// ADD 2021.08

    float total_cost = FLT_MAX;
    COST_TERM Best_Cost;
    while (!cost.empty())
    {
        COST_TERM& cost_term = cost.front();
        COST_TERM for_calculate = cost_term;

        for_calculate.Displacement = Scaling_Cost(Displacement_Range, cost_term.Displacement - cost_info.Displacement.first);
        for_calculate.Center_Distance = 1 - Scaling_Cost(Center_Distance_Range, cost_term.Center_Distance - cost_info.Center_Distance.first);
        for_calculate.Group_Distance = Scaling_Cost(Group_Distance_Range, cost_term.Group_Distance - cost_info.Group_Distance.first);
        for_calculate.Type_Distance = Scaling_Cost(Type_Distance_Range, cost_term.Type_Distance - cost_info.Type_Distance.first);
        for_calculate.Packing_DeadSpace = Scaling_Cost(Packing_DeadSpace_Range, cost_term.Packing_DeadSpace - cost_info.Packing_DeadSpace.first);
        for_calculate.Overlap_BoundaryArea = Scaling_Cost(Overlap_BoundaryArea_Range, cost_term.Overlap_BoundaryArea - cost_info.Overlap_BoundaryArea.first);
        for_calculate.Thickness = Scaling_Cost(Thickness_Range, cost_term.Thickness - cost_info.Thickness.first);
        for_calculate.WireLength = Scaling_Cost(WireLength_Range, cost_term.WireLength - cost_info.WireLength.first);

        if (Refinement_Flag == true)  /// ADD 2021.03.19
            for_calculate.Congestion = Scaling_Cost(Congestion_Range, cost_term.Congestion - cost_info.Congestion.first);  /// ADD 2021.03.19
        if (Build_Set == true && ITER > 0)
            for_calculate.Regularity = Scaling_Cost(Regularity_Range, cost_term.Regularity - cost_info.Regularity.first);   /// ADD 2021.08

        // cose evaluation
        float new_cost = CostEvaluation(for_calculate);

        if (new_cost < total_cost)
        {
            total_cost = new_cost;
            BestCoor = cost_term.MacroCoor;
            Best_Cost = cost_term;
        }

        cost.pop();
    }
    cost.push(Best_Cost);
    /// Update macro final coor
    if (total_cost < FLT_MAX)
    {
        Boundary_Assign(macro_temp->lg, BestCoor.first, BestCoor.second, BestCoor.first + macro_temp->cal_w_wo_shrink, BestCoor.second + macro_temp->cal_h_wo_shrink);
        macro_temp->Macro_Center = make_pair((macro_temp->lg.llx + macro_temp->lg.urx) / 2, (macro_temp->lg.lly + macro_temp->lg.ury) / 2);
        macro_temp->LegalFlag = true;
    }
    else
    {
        macro_temp->LegalFlag = false;
    }
}



float CostEvaluation(COST_TERM& cost_term) // this cost for paching corner
{
    /// ADD 2021.05
    if (Refinement_Flag == true)
    {
        //float y = 2.9*( (exp(pow(-16,exp(-4)))) - (exp(pow(-16,exp(-4*(10-ITER))))) );
        //PARAMETER._PACK_CENTER_DIST = 2.9 * ( (exp(-pow(16, exp(-1)))) - (exp(-pow(16, exp(-1*(10-ITER))))) ) + 0.9;
        //PARAMETER._PACK_CONGESTION  = 2.7182 * ( exp(-pow(16, exp(-1*(10-ITER) ) ) ) );
        //PARAMETER._PACK_GROUP_DIST  = 2.9 * ( (exp(-pow(16, exp(-1)))) - (exp(-pow(16, exp(-1*(10-ITER))))) ) + 0.9;
    }
    //////
    float cost = 0;
    if (PARAMETER._PACK_CENTER_DIST != 0)
        cost += cost_term.Center_Distance * PARAMETER._PACK_CENTER_DIST;

    if (PARAMETER._PACK_DISPLACEMENT != 0)
        cost += cost_term.Displacement * PARAMETER._PACK_DISPLACEMENT;

    if (PARAMETER._PACK_GROUP_DIST != 0)
        cost += cost_term.Group_Distance * PARAMETER._PACK_GROUP_DIST;

    if (PARAMETER._PACK_OVERLAP != 0)
        cost += cost_term.Overlap_BoundaryArea * PARAMETER._PACK_OVERLAP;

    if (PARAMETER._PACK_PACK_DEAD != 0)
        cost += cost_term.Packing_DeadSpace * PARAMETER._PACK_PACK_DEAD;

    if (PARAMETER._PACK_TYPE_DIST != 0)
        cost += cost_term.Type_Distance * PARAMETER._PACK_TYPE_DIST;

    //    cout<<cost_term.Type_Distance * PARAMETER._PACK_TYPE_DIST<<"\t"<<cost_term.Group_Distance * PARAMETER._PACK_GROUP_DIST<<endl;
    if (PARAMETER._PACK_THICKNESS != 0)
        cost += cost_term.Thickness * PARAMETER._PACK_THICKNESS;

    if (PARAMETER._PACK_WIRELENGTH != 0)
        cost += cost_term.WireLength * PARAMETER._PACK_WIRELENGTH;
    /// ADD 2021.05, 08
    if (PARAMETER._PACK_CONGESTION != 0 && Refinement_Flag == true && ITER > 0)
        cost += cost_term.Congestion * PARAMETER._PACK_CONGESTION;
    if (PARAMETER._PACK_REGULARITY != 0 && Build_Set == true && ITER > 0)
        cost += cost_term.Regularity * PARAMETER._PACK_REGULARITY;
    //////

    return cost;
}

/**********************108.04.22****************************/
float CostEvaluation_soft(COST_TERM& cost_term)
{
    float cost = 0;

    if (PARAMETER._PACK_DISPLACEMENT != 0)
        cost += cost_term.Displacement * PARAMETER._PACK_DISPLACEMENT;

    if (PARAMETER._PACK_OVERLAP != 0)
        cost += cost_term.Overlap_BoundaryArea * PARAMETER._PACK_OVERLAP;

    if (PARAMETER._PACK_WIRELENGTH != 0)
        cost += cost_term.WireLength * PARAMETER._PACK_WIRELENGTH;

    return cost;
}
/***********************************************************/


void Simulated_Evolution_0819(Legalization_INFO& LG_INFO, int iter_count)
{
    /// ADD 2021.05, set the legal flag true, perturb the best placement in initial
    /*for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        macro_temp->LegalFlag = true;
    }*/
    //////
    vector<Macro*>& MacroClusterSet = LG_INFO.MacroClusterSet;
    vector<ORDER_REGION>& region = LG_INFO.region;
    /// Update Center
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            int group_CenterX = 0, group_CenterY = 0;
            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];
                float ratio = type_tmp.total_macro_area / group_tmp.total_macro_area;
                group_CenterX += type_tmp.type_center.first * ratio;
                group_CenterY += type_tmp.type_center.second * ratio;

            }

            group_tmp.group_center = make_pair(group_CenterX, group_CenterY);

        }
    }
    /// update cost
    vector<float> center_dist;
    vector<float> type_dist;
    vector<float> group_dist;

    center_dist.resize(MacroClusterSet.size());
    type_dist.resize(MacroClusterSet.size());
    group_dist.resize(MacroClusterSet.size());

    pair<float, float> score_limits = make_pair(FLT_MIN, FLT_MAX);
    pair<float, float> max_min_center_dist = score_limits;
    pair<float, float> max_min_type_dist = score_limits;
    pair<float, float> max_min_group_dist = score_limits;
    /// ADD 2021.04
    pair<float, float> max_min_cong_cost = score_limits;
    //////


    /// calculate each term of the cost
    for (int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];

        if (macro_temp->LegalFlag != true)
        {
            continue;
        }

        // center_dist
        float& center_dist_temp = center_dist[i];
        int dist = (fabs(BenchInfo.WhiteSpaceXCenter - macro_temp->Macro_Center.first) + fabs(BenchInfo.WhiteSpaceYCenter - macro_temp->Macro_Center.second));
        dist *= macro_temp->macro_area_ratio;
        center_dist_temp = 1 / (float)max(dist, 1);
        max_min_center_dist.first = max(max_min_center_dist.first, center_dist_temp);
        max_min_center_dist.second = min(max_min_center_dist.second, center_dist_temp);

        pair<int, int>& group_center = region[macro_temp->region_id].group[macro_temp->group_id].group_center;
        pair<int, int>& type_center = region[macro_temp->region_id].group[macro_temp->group_id].lef_type[macro_temp->type_id].type_center;

        // type_dist
        float& type_dist_temp = type_dist[i];
        type_dist_temp = fabs(type_center.first - macro_temp->Macro_Center.first) + fabs(type_center.second - macro_temp->Macro_Center.second);
        max_min_type_dist.first = max(max_min_type_dist.first, type_dist_temp);
        max_min_type_dist.second = min(max_min_type_dist.second, type_dist_temp);

        // group_dist
        float& group_dist_temp = group_dist[i];
        group_dist_temp = fabs(group_center.first - macro_temp->Macro_Center.first) + fabs(group_center.second - macro_temp->Macro_Center.second);
        max_min_group_dist.first = max(max_min_group_dist.first, group_dist_temp);
        max_min_group_dist.second = min(max_min_group_dist.second, group_dist_temp);

        /// ADD 2021.04
        if (Refinement_Flag == true)
        {
            float cong_cost = 0;
            Boundary macro_boundary;
            Boundary_Assign(macro_boundary, macro_temp->lg.llx, macro_temp->lg.lly, macro_temp->lg.urx, macro_temp->lg.ury);
            cong_cost += CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boundary, true);
            cong_cost += CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boundary, true);
            max_min_cong_cost.first = max(max_min_cong_cost.first, cong_cost);
            max_min_cong_cost.second = min(max_min_cong_cost.second, cong_cost);
        }

        //////

//        cout<<center_dist_temp<<"\t"<<type_dist_temp<<"\t"<<group_dist_temp<<endl;
    }


    /// normalize each term of the cost
    float ceter_dist_range = max_min_center_dist.first - max_min_center_dist.second;
    float type_dist_range = max_min_type_dist.first - max_min_type_dist.second;
    float group_dist_range = max_min_group_dist.first - max_min_group_dist.second;
    /// ADD 2021.04
    float cong_cost_range = max_min_cong_cost.first - max_min_cong_cost.second;
    float average_cost = 0;
    //////
    for (int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        if (macro_temp->LegalFlag != true)
        {
            macro_temp->score = macro_temp->area;
            macro_temp->ConsiderCal = false;
            continue;
        }

        macro_temp->score = 0;

        /// center dist, MODIFY 2021.04
        float center_score;
        float type_score;
        float group_score;
        float cong_cost = 0;

        if (Refinement_Flag == true)
        {
            center_score = Scaling_Cost(ceter_dist_range, center_dist[i] - max_min_center_dist.second) * 0.4;
            type_score = Scaling_Cost(type_dist_range, type_dist[i] - max_min_center_dist.second) * 0.2;
            group_score = Scaling_Cost(group_dist_range, group_dist[i] - max_min_center_dist.second) * 0.2;

            Boundary macro_boundary;
            Boundary_Assign(macro_boundary, macro_temp->lg.llx, macro_temp->lg.lly, macro_temp->lg.urx, macro_temp->lg.ury);
            cong_cost += CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boundary, true);
            cong_cost += CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boundary, true);
            cong_cost = Scaling_Cost(cong_cost_range, cong_cost - max_min_cong_cost.second) * 0.2;
        }
        else
        {
            center_score = Scaling_Cost(ceter_dist_range, center_dist[i] - max_min_center_dist.second) * 0.4;
            type_score = Scaling_Cost(type_dist_range, type_dist[i] - max_min_center_dist.second) * 0.3;
            group_score = Scaling_Cost(group_dist_range, group_dist[i] - max_min_center_dist.second) * 0.3;
        }
        //////


        //macro_temp->score += center_score * 0.4 + type_score * 0.3 + group_score * 0.3;   /// COMMENT 2021.04
        /// ADD 2021.04
        if (Refinement_Flag == true)
            macro_temp->score += center_score * 0.4 + type_score * 0.3 + group_score * 0.2 + cong_cost * 0.1;
        else
            macro_temp->score += center_score * 0.4 + type_score * 0.3 + group_score * 0.3;
        //////
//
//        if(center_score != 0)
//        {
//            cout<<0.1 / (center_score * log(iter_count + 2))<<endl;
//            macro_temp->score += 0.1 / (center_score * log(iter_count + 2));
//        }
        score_limits.first = max(score_limits.first, macro_temp->score);
        score_limits.second = min(score_limits.second, macro_temp->score);
        //        cout<<"center : "<<center_score<<"\ttype : "<<type_score<<"\tgroup : "<<group_score<<endl;
        //        cout<<"macro_temp->score : "<<macro_temp->score<<endl;
    }

    /// ADD 2021.05, put the placed macro into the vector AllCornerNode2
    /*vector<CornerNode*> AllCornerNode2;
    UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode2);

    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
        CornerNode* starNode = AllCornerNode2[startID];

        vector<Macro*> Cut_macros;

        queue<CornerNode*> CornerNodeSetQ;
        vector<CornerNode*> CornerNodeSetV;
        Directed_AreaEnumeration(macro_temp->lg, starNode, CornerNodeSetQ);
        while(!CornerNodeSetQ.empty())
        {
            CornerNodeSetV.insert(CornerNodeSetV.begin(), CornerNodeSetQ.front()); //let y from low to high
            CornerNodeSetQ.pop();
        }
        for(int j = 0; j < CornerNodeSetV.size(); j++)
        {
            if(CornerNodeSetV[j]->rectangle.lly >= macro_temp->lg.ury)
                continue;

            Macro* macro_tmp = new Macro;
            macro_tmp->lg.llx = max(macro_temp->lg.llx, CornerNodeSetV[j]->rectangle.llx);
            macro_tmp->lg.lly = max(macro_temp->lg.lly, CornerNodeSetV[j]->rectangle.lly);
            macro_tmp->lg.urx = min(macro_temp->lg.urx, CornerNodeSetV[j]->rectangle.urx);
            macro_tmp->lg.ury = min(macro_temp->lg.ury, CornerNodeSetV[j]->rectangle.ury);
            macro_tmp->lef_type_ID = macro_temp->lef_type_ID;
            macro_tmp->OriginalMacro = macro_temp;
            Cut_macros.push_back(macro_tmp);
        }

        Macro* cut_macro = macro_temp;
        for(int j = 0; j < Cut_macros.size(); j++)
        {
            cut_macro = Cut_macros[j];
            vector<CornerNode*> combination;
            combination.reserve(10);
            pair<int, int> macro_coor = make_pair(cut_macro->lg.llx, cut_macro->lg.lly);

            combination.push_back(CornerNodePointSearch(macro_coor, starNode));
            Find_topBlank(combination, cut_macro, cut_macro->lg.ury, cut_macro->lg);

            Before_UpdateMacroInDatastruct(AllCornerNode2, cut_macro, combination);
        }
    }*/
    //////
    /*ofstream fout("AllCornerNode2.m");
    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;
    for(int i = 0; i < (int)AllCornerNode2.size(); i++)
    {
        if(AllCornerNode2[i] == NULL)
            continue;

        fout<<"block_x=["<<AllCornerNode2[i]->rectangle.llx<<" "<<AllCornerNode2[i]->rectangle.llx<<" "<<AllCornerNode2[i]->rectangle.urx<<" "<<AllCornerNode2[i]->rectangle.urx<<" "<<AllCornerNode2[i]->rectangle.llx<<" ];"<<endl;
        fout<<"block_y=["<<AllCornerNode2[i]->rectangle.lly<<" "<<AllCornerNode2[i]->rectangle.ury<<" "<<AllCornerNode2[i]->rectangle.ury<<" "<<AllCornerNode2[i]->rectangle.lly<<" "<<AllCornerNode2[i]->rectangle.lly<<" ];"<<endl;
        fout << "text(" << (AllCornerNode2[i]->rectangle.llx + AllCornerNode2[i]->rectangle.urx) / 2.0 << ", " << (AllCornerNode2[i]->rectangle.lly + AllCornerNode2[i]->rectangle.ury) / 2.0 << ", '" << AllCornerNode2[i]->CornerNode_id << "');" << endl << endl;
        if(AllCornerNode2[i]->NodeType == Fixed)
        {
            fout<<"fill(block_x, block_y, 'r', 'facealpha', 0.5)"<<endl;
        }
        else if(AllCornerNode2[i]->NodeType == Blank)
        {
            fout<<"fill(block_x, block_y, 'c', 'facealpha', 0.5)"<<endl;
        }
        else if(AllCornerNode2[i]->NodeType == Movable)
        {
            fout<<"fill(block_x, block_y, 'b', 'facealpha', 0.5)"<<endl;
        }
    }exit(1);*/
    /// normalize score & determine ripped-up macros
    float range = score_limits.first - score_limits.second;
    int upper_limit = (int)MacroClusterSet.size() * 0.4;
    set<Macro*> rip_up_count;

    //ofstream out("Check_Scores.m");
    //out<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;
    //chip boundary
    //out << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    //out << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    //out << "fill(block_x, block_y, 'w');" << endl;

    for (int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        if (macro_temp->LegalFlag != true)
        {
            macro_temp->ConsiderCal = false;
            /// ADD 2021.05
            macro_temp->Rip_up_by_range = false;
            //////
            continue;
        }

        //out << "block_x=[" << macro_temp->lg.llx << " " << macro_temp->lg.llx << " " << macro_temp->lg.urx << " " << macro_temp->lg.urx << " " << macro_temp->lg.llx << " ];" << endl;
        //out << "block_y=[" << macro_temp->lg.lly << " " << macro_temp->lg.ury << " " << macro_temp->lg.ury << " " << macro_temp->lg.lly << " " << macro_temp->lg.lly << " ];" << endl;
        /// ADD 2021.05
        //cout<<"rip_up_count: "<<(int)rip_up_count.size()<<endl;
        if ((int)rip_up_count.size() > upper_limit)
            break;
        //////

        macro_temp->score = Scaling_Cost(range, macro_temp->score - score_limits.second);

        /// normalize score 0 - 1 to 0.1 - 0.9
        macro_temp->score = (macro_temp->score + 0.125) * 0.8;
        // random parameter r = 0.00 - 1.00
        float r = (float)(rand() % 101);
        r /= (float)100;

        // need to rip-up
        if (macro_temp->score > r)
        {
            macro_temp->ConsiderCal = false;
            rip_up_count.insert(macro_temp);

            /// ADD 2021.05, rip-up macros according to rectangle bounding box
            Boundary Macro_To_Chip_Center;
            ORDER_REGION& region_tmp = LG_INFO.region[macro_temp->region_id];

            Macro_To_Chip_Center.llx = min(macro_temp->lg.llx, region_tmp.p_region->rip_up_point.first);
            Macro_To_Chip_Center.lly = min(macro_temp->lg.lly, region_tmp.p_region->rip_up_point.second);
            Macro_To_Chip_Center.urx = max(macro_temp->lg.urx, region_tmp.p_region->rip_up_point.first);
            Macro_To_Chip_Center.ury = max(macro_temp->lg.ury, region_tmp.p_region->rip_up_point.second);

            //ofstream fout1("rip_up.m");
            //fout1<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;
            //chip boundary
            //fout1 << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
            //fout1 << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
            //fout1 << "fill(block_x, block_y, 'w');" << endl;

            //int color = 0;
            //plot_region(fout1, Macro_To_Chip_Center, color);
            /*Directed_AreaEnumeration(Macro_To_Chip_Center, starNode, CornerNodeSet);
            while(!CornerNodeSet.empty())
            {
                if(CornerNodeSet.front()->NodeType == Movable)
                {
                    Surrounding_Macro.insert( make_pair(CornerNodeSet.front()->macro->OriginalMacro->macro_name, CornerNodeSet.front()->macro->OriginalMacro) );
                }
                CornerNodeSet.pop();
            }*/
            //fout1 << "block_x=[" << macro_temp->lg.llx << " " << macro_temp->lg.llx << " " << macro_temp->lg.urx << " " << macro_temp->lg.urx << " " << macro_temp->lg.llx << " ];" << endl;
            //fout1 << "block_y=[" << macro_temp->lg.lly << " " << macro_temp->lg.ury << " " << macro_temp->lg.ury << " " << macro_temp->lg.lly << " " << macro_temp->lg.lly << " ];" << endl;
            //fout1 << "fill(block_x, block_y, 'b');" << endl;
            for (map<string, Macro*>::iterator it = (macro_temp->set)->members.begin(); it != (macro_temp->set)->members.end(); it++)
            {
                Macro* macro = it->second;
                if (macro == macro_temp)
                    continue;
                if (macro->lg.urx <= Macro_To_Chip_Center.llx || macro->lg.llx >= Macro_To_Chip_Center.urx ||
                    macro->lg.ury <= Macro_To_Chip_Center.lly || macro->lg.lly >= Macro_To_Chip_Center.ury)
                {
                }
                else
                {
                    macro->Rip_up_by_range = false;
                    macro->ConsiderCal = false;
                    rip_up_count.insert(macro);

                    //fout1 << "block_x=[" << macro->lg.llx << " " << macro->lg.llx << " " << macro->lg.urx << " " << macro->lg.urx << " " << macro->lg.llx << " ];" << endl;
                    //fout1 << "block_y=[" << macro->lg.lly << " " << macro->lg.ury << " " << macro->lg.ury << " " << macro->lg.lly << " " << macro->lg.lly << " ];" << endl;
                    //fout1 << "fill(block_x, block_y, 'y');" << endl;
                }
            }

            //cout<<"rip"<<endl;
            //getchar();
            //////
            //out << "fill(block_x, block_y, 'r');" << endl;
            //out << "text(" << (macro_temp->lg.llx + macro_temp->lg.urx) / 2.0 << ", " << (macro_temp->lg.lly + macro_temp->lg.ury) / 2.0 << ", '" << macro_temp->score << "','fontsize',8);" << endl << endl;
        }
        else
        {
            if (macro_temp->Rip_up_by_range == false)
            {
                //out << "fill(block_x, block_y, 'r');" << endl;
                //out << "text(" << (macro_temp->lg.llx + macro_temp->lg.urx) / 2.0 << ", " << (macro_temp->lg.lly + macro_temp->lg.ury) / 2.0 << ", '" << macro_temp->score << "','fontsize',8);" << endl << endl;
            }  // has been ripped up, avoid to setting ConsiderCal from false to true
            else
            {
                macro_temp->ConsiderCal = true;
                //out << "fill(block_x, block_y, 'g');" << endl;
                //out << "text(" << (macro_temp->lg.llx + macro_temp->lg.urx) / 2.0 << ", " << (macro_temp->lg.lly + macro_temp->lg.ury) / 2.0 << ", '" << macro_temp->score << "','fontsize',8);" << endl << endl;
            }
        }
        //////
        /// ADD 2021.05, set all tiles false
        /*for(int j = 0; j < (int)AllCornerNode2.size(); j++)
        {
            if(AllCornerNode2[j] != NULL)
                AllCornerNode2[j]->visited = false;
        }*/
        //////
        //macro_temp->score *= 1 + macro_temp-> macro_area_ratio;
        //cout<<"*"<<macro_temp->score<<endl;
    }
    //cout<<"check_scores"<<endl;
    //getchar();
    //cout<<"rip up num: "<<(int)rip_up_count.size()<<endl;

    /// ADD 2021.05, for those should be ripped-up macros, remove from sets
    for (int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro = MacroClusterSet[i];
        if (macro->LegalFlag != true)
        {
            //macro->ConsiderCal = false;
            continue;
        }

        if (macro->set == NULL)
        {
            cout << "@@, macro: " << macro->macro_name << endl;
            getchar();
            //continue;
        }
        if (macro->ConsiderCal == false)
        {
            // set
            map<string, Macro*>::iterator it = (macro->set)->members.find(macro->macro_name);
            if (it != (macro->set)->members.end())
            {
                (macro->set)->members.erase(it);
            }
            else
            {
                cout << "can`t find this macro in members" << endl;
                cout << "macro: " << macro->macro_name << endl;
                getchar();
            }
            // macro
            macro->set = NULL;
        }
        else
        {
        }
    }

    for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); )
    {
        if ((*i)->members.empty())
        {
            Macros_Set.erase(i);
        }
        else
            i++;
    }

    // re-assign id
    Re_Assign_Sets_ID();

    /*ofstream out("Remained_macros.m");
    out<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;
    //chip boundary
    out << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    out << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    out << "fill(block_x, block_y, 'w');" << endl;
    for(int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro *macro = LG_INFO.MacroClusterSet[i];
        if(macro->LegalFlag != true)
        {
            continue;
        }
        if(macro->ConsiderCal == true)
        {
            out << "block_x=[" << macro->lg.llx << " " << macro->lg.llx << " " << macro->lg.urx << " " << macro->lg.urx << " " << macro->lg.llx << " ];" << endl;
            out << "block_y=[" << macro->lg.lly << " " << macro->lg.ury << " " << macro->lg.ury << " " << macro->lg.lly << " " << macro->lg.lly << " ];" << endl;

            out << "fill(block_x, block_y, 'y');" << endl;
        }
    }*/

    /*ofstream fout("Before_Set.m");
    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;
    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    int x = 0;
    for(set<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        for(map<string, Macro*>::iterator j = (*i)->members.begin(); j != (*i)->members.end(); j++)
        {
            Macro *macro = j->second;
            fout << "block_x=[" << macro->lg.llx << " " << macro->lg.llx << " " << macro->lg.urx << " " << macro->lg.urx << " " << macro->lg.llx << " ];" << endl;
            fout << "block_y=[" << macro->lg.lly << " " << macro->lg.ury << " " << macro->lg.ury << " " << macro->lg.lly << " " << macro->lg.lly << " ];" << endl;

            if(x %3 == 0)
                fout << "fill(block_x, block_y, 'r');" << endl;
            else if(x %3 == 1)
                fout << "fill(block_x, block_y, 'g');" << endl;
            else if(x %3 == 2)
                fout << "fill(block_x, block_y, 'b');" << endl;

            fout << "text(" << (macro->lg.llx + macro->lg.urx) / 2.0 << ", " << (macro->lg.lly + macro->lg.ury) / 2.0 << ", '" << x << "','fontsize',8);" << endl << endl;
        }
        x++;
    }*/
    //cout<<"Macros_Set size: "<<Macros_Set.size()<<endl;
    //cout<<"before"<<endl;
    //getchar();
    //////
    /// after rip-up macros, one set might be separated to two or more sets.
    Revise_Sets();
    // re-assign id
    Re_Assign_Sets_ID();
    //////

    /// debug
    if (ITER != 0)
    {
        Check_Sets_Macros(LG_INFO);
    }

    /// debug
    /*ofstream ffout("After_Set.m");

    ffout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;
    //chip boundary
    ffout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    ffout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    ffout << "fill(block_x, block_y, 'w');" << endl;

    int xx = 0;
    for(set<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        for(map<string, Macro*>::iterator j = (*i)->members.begin(); j != (*i)->members.end(); j++)
        {
            Macro *macro = j->second;
            ffout << "block_x=[" << macro->lg.llx << " " << macro->lg.llx << " " << macro->lg.urx << " " << macro->lg.urx << " " << macro->lg.llx << " ];" << endl;
            ffout << "block_y=[" << macro->lg.lly << " " << macro->lg.ury << " " << macro->lg.ury << " " << macro->lg.lly << " " << macro->lg.lly << " ];" << endl;

            if(xx %3 == 0)
                ffout << "fill(block_x, block_y, 'r');" << endl;
            else if(xx %3 == 1)
                ffout << "fill(block_x, block_y, 'g');" << endl;
            else if(xx %3 == 2)
                ffout << "fill(block_x, block_y, 'b');" << endl;

            ffout << "text(" << (macro->lg.llx + macro->lg.urx) / 2.0 << ", " << (macro->lg.lly + macro->lg.ury) / 2.0 << ", '" << xx << "','fontsize',8);" << endl << endl;
        }
        xx++;
    }*/
    //cout<<"after"<<endl;
    //getchar();
    //////
    /*/// debug
    int rip_num = 0;
    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro *macro = LG_INFO.Macro_Ordering[i];
        if(macro->ConsiderCal == false)
            rip_num++;
    }
    int leg_num = 0;
    for(set<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        leg_num += (*i)->members.size();
    }
    if(rip_num + leg_num != LG_INFO.Macro_Ordering.size())
    {
        cout<<"-----------------------------------------------------------------------------"<<endl;
        cout<<"rip_num: "<<rip_num<<endl;
        cout<<"leg_num: "<<leg_num<<endl;
        cout<<"correct num: "<<LG_INFO.Macro_Ordering.size()<<endl;
        cout<<"Macros_Set size: "<<Macros_Set.size()<<endl;
        int total = 0;
        for(set<MacrosSet*>::iterator it = Macros_Set.begin(); it != Macros_Set.end(); it++)
        {
            cout<<"id: "<<(*it)->id<<endl;
            cout<<"member size: "<<(*it)->members.size()<<endl;
            total += (*it)->members.size();
            for(map<string, Macro*>::iterator iter = (*it)->members.begin(); iter != (*it)->members.end(); iter++)
                cout<<"\t"<<iter->first<<endl;
        }
        exit(1);
    }
    //////*/

    // update type boundary and group boundary
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            Boundary& group_boundary = group_tmp.group_boundary;
            Boundary_Assign_Limits(group_boundary);

            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];

                ConsideCal_Type_Boundary_Calculate(type_tmp, MacroClusterSet);
                Cal_BBOX(type_tmp.type_boundary, group_boundary);
            }
        }
    }

    /// ADD 2021.05
    //DeleteCornerNode(AllCornerNode2);
    //////

    // according the this order to re-packing macro one by one
    stable_sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_ordering);    /// MODIFY 2021.03.19, sort -> stable_sort
//    getchar();
    //cout<<"Finish SE"<<endl;
}

/// ADD 2021.05
void Revise_Sets()
{
    for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
    {
        Boundary bbx = (*i)->bbx;
        vector<Macro*> Macro_In_Set;
        Macro_In_Set.reserve((int)(*i)->members.size());
        for (map<string, Macro*>::iterator j = (*i)->members.begin(); j != (*i)->members.end(); j++)
        {
            Macro_In_Set.push_back(j->second);
        }

        vector<vector<Macro*> > collected_x; collected_x.reserve((int)(*i)->members.size());
        vector<vector<Macro*> > collected_y; collected_y.reserve((int)(*i)->members.size());
        vector<Macro*> tmp;                  tmp.reserve((int)(*i)->members.size());
        // x
        sort(Macro_In_Set.begin(), Macro_In_Set.end(), Macro_sort_by_x);
        for (int j = 0; j < (int)Macro_In_Set.size(); j++)
        {
            Macro* macro = Macro_In_Set[j];
            int range_llx, range_urx;
            if (j == 0)
            {
                range_llx = macro->lg.llx;
                range_urx = macro->lg.urx;
                tmp.push_back(macro);
            }
            else
            {
                if (range_llx <= macro->lg.llx && macro->lg.llx <= range_urx)    // continuous
                {
                    tmp.push_back(macro);
                    // update range
                    range_llx = min(range_llx, macro->lg.llx);
                    range_urx = max(range_urx, macro->lg.urx);
                }
                else    // separated
                {
                    // new range
                    range_llx = macro->lg.llx;
                    range_urx = macro->lg.urx;
                    // put tmp into collected x
                    collected_x.push_back(tmp);
                    tmp.clear();
                    tmp.push_back(macro);
                }
            }
        }
        if (!tmp.empty())
        {
            collected_x.push_back(tmp);
            tmp.clear();
        }

        if (collected_x.size() > 1)  // don`t need to check y
        {
            for (int j = 0; j < (int)collected_x.size() - 1; j++)
            {
                MacrosSet* set_tmp = new MacrosSet;
                set_tmp->id = (int)Macros_Set.size();

                Macros_Set.push_back(set_tmp);
                for (int k = 0; k < (int)collected_x[j].size(); k++)
                {
                    Macro* macro_tmp = collected_x[j][k];
                    // remove from the original set
                        // set
                    (macro_tmp->set)->members.erase(macro_tmp->macro_name);
                    // macro
                    macro_tmp->set = NULL;
                    // new set
                        // set
                    set_tmp->members.insert(make_pair(macro_tmp->macro_name, macro_tmp));
                    // macro
                    macro_tmp->set = set_tmp;
                }
            }
        }
        else    // need to check y
        {
            // y
            sort(Macro_In_Set.begin(), Macro_In_Set.end(), Macro_sort_by_y);
            for (int i = 0; i < (int)Macro_In_Set.size(); i++)
            {
                Macro* macro = Macro_In_Set[i];
            }

            for (int j = 0; j < (int)Macro_In_Set.size(); j++)
            {
                Macro* macro = Macro_In_Set[j];

                int range_lly, range_ury;
                if (j == 0)
                {
                    range_lly = macro->lg.lly;
                    range_ury = macro->lg.ury;
                    tmp.push_back(macro);
                }
                else
                {
                    if (range_lly <= macro->lg.lly && macro->lg.lly <= range_ury)    // continuous
                    {
                        tmp.push_back(macro);
                        // update range
                        range_lly = min(range_lly, macro->lg.lly);
                        range_ury = max(range_ury, macro->lg.ury);
                    }
                    else    // separated
                    {
                        // new range
                        range_lly = macro->lg.lly;
                        range_ury = macro->lg.ury;
                        // put tmp into collected y
                        collected_y.push_back(tmp);
                        tmp.clear();
                        tmp.push_back(macro);
                    }
                }
            }
            if (!tmp.empty())
            {
                collected_y.push_back(tmp);
                tmp.clear();
            }


            if (collected_y.size() > 1)
            {

                for (int j = 0; j < (int)collected_y.size() - 1; j++)
                {
                    MacrosSet* set_tmp = new MacrosSet;
                    set_tmp->id = (int)Macros_Set.size();

                    Macros_Set.push_back(set_tmp);
                    for (int k = 0; k < (int)collected_y[j].size(); k++)
                    {
                        Macro* macro_tmp = collected_y[j][k];

                        // remove from the original set
                            // set
                        (macro_tmp->set)->members.erase(macro_tmp->macro_name);
                        // macro
                        macro_tmp->set = NULL;
                        // new set
                            // set
                        set_tmp->members.insert(make_pair(macro_tmp->macro_name, macro_tmp));
                        // macro
                        macro_tmp->set = set_tmp;
                    }
                }
            }
        }
    }
    for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); )
    {
        if ((*i)->members.empty())
        {
            Macros_Set.erase(i);
        }
        else
            i++;
    }
}

void Re_Assign_Sets_ID()
{
    int index = 0;
    for (int i = 0; i < (int)Macros_Set.size(); i++)
    {
        MacrosSet* set_tmp = Macros_Set[i];
        if (set_tmp != NULL)
        {
            set_tmp->id = index;
            index++;
        }
    }
}

void Check_Sets_Macros(Legalization_INFO& LG_INFO)
{
    for (int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro* macro = LG_INFO.MacroClusterSet[i];
        if (macro->ConsiderCal == false)
            continue;
        if (macro->set == NULL)
        {
            cout << "set pointer is wrong" << endl;
            cout << macro->macro_name << endl;
            getchar();
            exit(1);
        }
        map<string, Macro*>::iterator it = (macro->set)->members.find(macro->macro_name);
        if (it == (macro->set)->members.end())
        {
            cout << "in members can`t find macro: " << macro->macro_name << endl;
            getchar();
        }
    }

    for (int i = 0; i < (int)Macros_Set.size(); i++)
    {
        MacrosSet* set_tmp = Macros_Set[i];
        for (map<string, Macro*>::iterator j = set_tmp->members.begin(); j != set_tmp->members.end(); j++)
        {
            bool yes = false;
            for (int k = 0; k < (int)LG_INFO.MacroClusterSet.size(); k++)
            {
                Macro* macro = LG_INFO.MacroClusterSet[k];
                if (j->first == macro->macro_name && j->second == macro)
                {
                    yes = true;
                    break;
                }
            }
            if (yes == false)
            {
                cout << "map in set is wrong, macro: " << j->first << endl;
                getchar();
            }
        }
    }
}

void SearchMovableTile(CornerNode* start, map<string, Macro*>& Surrounding_Macro)
{
    CornerNode* target = start;
    //cout<<"target tile: "<<target->rectangle.llx<<", "<<target->rectangle.lly<<endl;
    // E
    while (target->E->NodeType == Movable && target->E->visited == false)
    {
        Surrounding_Macro.insert(make_pair(target->E->macro->OriginalMacro->macro_name, target->E->macro->OriginalMacro));
        target->E->visited = true;
        //cout<<target->E->rectangle.llx<<", "<<target->E->rectangle.lly<<endl;
        target = target->E;
        SearchMovableTile(target, Surrounding_Macro);
    }
    // W
    while (target->W->NodeType == Movable && target->W->visited == false)
    {
        Surrounding_Macro.insert(make_pair(target->W->macro->OriginalMacro->macro_name, target->W->macro->OriginalMacro));
        target->W->visited = true;
        //cout<<target->W->rectangle.llx<<", "<<target->W->rectangle.lly<<endl;
        target = target->W;
        SearchMovableTile(target, Surrounding_Macro);
    }
    // S
    while (target->S->NodeType == Movable && target->S->visited == false)
    {
        Surrounding_Macro.insert(make_pair(target->S->macro->OriginalMacro->macro_name, target->S->macro->OriginalMacro));
        target->S->visited = true;
        //cout<<target->S->rectangle.llx<<", "<<target->S->rectangle.lly<<endl;
        target = target->S;
        SearchMovableTile(target, Surrounding_Macro);
    }
    // N
    while (target->N->NodeType == Movable && target->N->visited == false)
    {
        Surrounding_Macro.insert(make_pair(target->N->macro->OriginalMacro->macro_name, target->N->macro->OriginalMacro));
        target->N->visited = true;
        //cout<<target->N->rectangle.llx<<", "<<target->N->rectangle.lly<<endl;
        target = target->N;
        SearchMovableTile(target, Surrounding_Macro);
    }
}
//////

bool cmp_ordering(Macro* a, Macro* b)
{
    return a->score > b->score;
}

void Simulated_Evolution(vector<Macro*>& MacroClusterSet, vector<ORDER_REGION>& region)
{

    /// Update Center
    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];

            int group_CenterX = 0, group_CenterY = 0;
            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];
                float ratio = type_tmp.total_macro_area / group_tmp.total_macro_area;


                group_CenterX += type_tmp.type_center.first * ratio;
                group_CenterY += type_tmp.type_center.second * ratio;

            }

            group_tmp.group_center = make_pair(group_CenterX, group_CenterY);

        }
    }

    for (int i = 0; i < (int)region.size(); i++)
    {
        ORDER_REGION& region_tmp = region[i];

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];
            float max_score = FLT_MIN;
            float min_score = FLT_MAX;
            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];


                for (int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int macro_id = type_tmp.macro_id[l];
                    Macro* macro_temp = MacroClusterSet[macro_id];
                    if (macro_temp->LegalFlag != true)
                    {
                        continue;
                    }

                    pair<int, int>& group_center = group_tmp.group_center;
                    pair<int, int>& type_center = type_tmp.type_center;

                    int CenterX = (macro_temp->lg.urx + macro_temp->lg.llx) / 2;
                    int CenterY = (macro_temp->lg.ury + macro_temp->lg.lly) / 2;
                    macro_temp->score = 0;
                    macro_temp->score += sqrt(pow((CenterX - group_center.first) / (float)PARA, 2) + pow((CenterY - group_center.second) / (float)PARA, 2));
                    macro_temp->score += sqrt(pow((CenterX - type_center.first) / (float)PARA, 2) + pow((CenterY - type_center.second) / (float)PARA, 2));
                    max_score = max(macro_temp->score, max_score);
                    min_score = min(macro_temp->score, min_score);
                    //                    cout<<"SCORE : "<<macro_temp->score<<endl;

                }

            }
            float range = max_score - min_score;


            Boundary& group_boundary = group_tmp.group_boundary;
            Boundary_Assign_Limits(group_boundary);

            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];


                for (int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int macro_id = type_tmp.macro_id[l];
                    Macro* macro_temp = MacroClusterSet[macro_id];
                    if (macro_temp->LegalFlag != true)
                    {
                        macro_temp->score = 1;
                        macro_temp->ConsiderCal = false;
                        continue;
                    }

                    macro_temp->score = Scaling_Cost(range, macro_temp->score - min_score);
                    /// normalize score to 0.1-0.9
                    macro_temp->score = (macro_temp->score + 0.125) * 0.8;

                    float r = (float)(rand() % 101);
                    r /= (float)100;
                    if (macro_temp->score > r && type_tmp.macro_id.size() > 1)
                    {

                        macro_temp->ConsiderCal = false;
                    }
                    else
                    {

                        macro_temp->ConsiderCal = true;
                    }

                }

                ConsideCal_Type_Boundary_Calculate(type_tmp, MacroClusterSet);
                Cal_BBOX(type_tmp.type_boundary, group_boundary);

            }
        }
    }

    //getchar();
}



void ConsideCal_Type_Boundary_Calculate(ORDER_TYPE& type_tmp, vector<Macro*>& MacroClusterSet)
{
    vector<int>& macro_ID = type_tmp.macro_id;

    Boundary& type_boundary = type_tmp.type_boundary;
    Boundary_Assign_Limits(type_boundary);

    for (int i = 0; i < (int)macro_ID.size(); i++)
    {
        int macro_id = macro_ID[i];
        Macro* macro_temp = MacroClusterSet[macro_id];
        if (macro_temp->ConsiderCal)
        {
            Cal_BBOX(macro_temp->lg, type_boundary);

        }
    }

}

void Total_Group_Center_Calculate(ORDER_GROUP& group_tmp)
{
    int CenterX = 0, CenterY = 0;
    for (int i = 0; i < (int)group_tmp.lef_type.size(); i++)
    {
        ORDER_TYPE& type_tmp = group_tmp.lef_type[i];
        float ratio = type_tmp.total_macro_area / group_tmp.total_macro_area;
        CenterX += type_tmp.type_center.first * ratio;
        CenterY += type_tmp.type_center.second * ratio;
    }

    group_tmp.group_center = make_pair(CenterX, CenterY);
}

void ConstructMacroDeClusterSet(vector<Macro*>& MacroClusterSet)
{
    MacroClusterSet.resize(MovableMacro_ID.size());
    for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_assign_id = MovableMacro_ID[i];
        Macro* macro_assign = macro_list[macro_assign_id];
        Macro* macro_temp = new Macro;

        macro_temp->Spacing = macro_assign->Spacing;
        macro_temp->macro_id = i;
        macro_temp->cal_h = macro_assign->cal_h;
        macro_temp->cal_h_wo_shrink = macro_assign->cal_h_wo_shrink;
        macro_temp->cal_w = macro_assign->cal_w;
        macro_temp->cal_w_wo_shrink = macro_assign->cal_w_wo_shrink;
        macro_temp->area = macro_assign->area;
        macro_temp->gp = macro_assign->gp;
        macro_temp->lg = macro_assign->gp;
        macro_temp->h = macro_assign->h;
        macro_temp->w = macro_assign->w;
        macro_temp->HeightShrinkPARA = ShrinkPara;
        macro_temp->WidthShrinkPARA = ShrinkPara;
        macro_temp->macro_name = macro_assign->macro_name;
        macro_temp->macro_type = macro_assign->macro_type;
        macro_temp->lef_type_ID = macro_assign->lef_type_ID;
        macro_temp->HierStdArea = macro_assign->HierStdArea;
        macro_temp->RefCoor = make_pair(macro_assign->gp.llx, macro_assign->gp.lly);
        macro_temp->Macro_Center = make_pair((macro_assign->gp.llx + macro_assign->gp.urx) / 2, (macro_assign->gp.lly + macro_assign->gp.ury) / 2);
        macro_temp->macro_area_ratio = macro_temp->area / BenchInfo.movable_macro_area;
        macro_temp->NetID = macro_assign->NetID;
        MacroClusterSet[i] = macro_temp;

    }

    Update_NET_INFO(MacroClusterSet);
    //PlotMacroClusterSet(MacroClusterSet, "./output/GP_graph/GP_macro.m");

}


bool cmp_area(pair<float, int> a, pair<float, int> b)
{
    if (a.first == b.first)
    {
        return a.second < b.second;
    }
    return a.first > b.first;

}

pair<int, int> Cal_WhiteSpaceCenter(vector<CornerNode*> AllCornerNode)
{
    float BlankSpaceCenterX, BlankSpaceCenterY;
    BlankSpaceCenterX = BlankSpaceCenterY = 0;

    queue<float> BlankSpaceArea;
    queue<Boundary> BlankRectangle;
    float TotalBlankSpaceArea = 0;
    for (int i = 0; i < (int)AllCornerNode.size(); i++)
    {
        CornerNode* corner_temp = AllCornerNode[i];
        if (corner_temp == NULL)
        {
            continue;
        }
        if (corner_temp->NodeType != Blank)
        {
            continue;
        }
        Boundary& blank_temp = corner_temp->rectangle;
        float blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
        TotalBlankSpaceArea += blank_NodeArea;
        BlankSpaceArea.push(blank_NodeArea);
        BlankRectangle.push(blank_temp);
    }

    while (!BlankSpaceArea.empty())
    {
        float massWeight = BlankSpaceArea.front() / TotalBlankSpaceArea;
        Boundary& blank_temp = BlankRectangle.front();
        BlankSpaceCenterX += massWeight * (blank_temp.urx + blank_temp.llx) / 2;
        BlankSpaceCenterY += massWeight * (blank_temp.ury + blank_temp.lly) / 2;

        BlankSpaceArea.pop();
        BlankRectangle.pop();
    }

    return make_pair(BlankSpaceCenterX, BlankSpaceCenterY);

}

void Packing_Boundary_Legalization(Legalization_INFO& LG_INFO, pair<int, int> WhiteCenter)
{
    /// region-based legalization


    /// update fixed macros
    for (int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = LG_INFO.MacroClusterSet[i];
        if (macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {
            //input to coner sitiching data structure
            int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
            CornerNode* starNode = LG_INFO.AllCornerNode[startID];
            /*vector<CornerNode*> combination;
            combination.reserve(10);
            pair<int, int> macro_coor = make_pair(macro_temp->lg.llx, macro_temp->lg.lly);

            combination.push_back(CornerNodePointSearch(macro_coor, starNode));
            Find_topBlank(combination, macro_temp, macro_temp->lg.ury, macro_temp->lg);
            Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, macro_temp, combination);*/ /// COMMENT 2021.03.19

            ///2021.01
            vector<Macro*> Cut_macros;

            queue<CornerNode*> CornerNodeSetQ;
            vector<CornerNode*> CornerNodeSetV;
            Directed_AreaEnumeration(macro_temp->lg, starNode, CornerNodeSetQ);
            while (!CornerNodeSetQ.empty())
            {
                CornerNodeSetV.insert(CornerNodeSetV.begin(), CornerNodeSetQ.front()); //let y from low to high
                CornerNodeSetQ.pop();
            }
            for (int j = 0; j < CornerNodeSetV.size(); j++)
            {
                if (CornerNodeSetV[j]->rectangle.lly >= macro_temp->lg.ury)
                    continue;

                Macro* macro_tmp = new Macro;
                macro_tmp->lg.llx = max(macro_temp->lg.llx, CornerNodeSetV[j]->rectangle.llx);
                macro_tmp->lg.lly = max(macro_temp->lg.lly, CornerNodeSetV[j]->rectangle.lly);
                macro_tmp->lg.urx = min(macro_temp->lg.urx, CornerNodeSetV[j]->rectangle.urx);
                macro_tmp->lg.ury = min(macro_temp->lg.ury, CornerNodeSetV[j]->rectangle.ury);
                macro_tmp->lef_type_ID = macro_temp->lef_type_ID;
                macro_tmp->OriginalMacro = macro_temp;
                Cut_macros.push_back(macro_tmp);
            }
            //////

            ///2021.01
            Macro* cut_macro = macro_temp;
            for (int j = 0; j < Cut_macros.size(); j++)
            {
                cut_macro = Cut_macros[j];
                vector<CornerNode*> combination;
                combination.reserve(10);
                pair<int, int> macro_coor = make_pair(cut_macro->lg.llx, cut_macro->lg.lly);

                combination.push_back(CornerNodePointSearch(macro_coor, starNode));
                Find_topBlank(combination, cut_macro, cut_macro->lg.ury, cut_macro->lg);

                //Cal_BBOX(cut_macro->lg, type_tmp.type_boundary);
                //Cal_BBOX(cut_macro->lg, group_tmp.group_boundary);

                Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, cut_macro, combination);
            }
            //////
        }
    }

    queue<int> illegal_set;
    for (int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION& region_tmp = LG_INFO.region[i];

        // Determine_PackDirection
        vector<float> occupy_ratio = Determine_PackDirection(LG_INFO.AllCornerNode, region_tmp.p_region->rectangle);

        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];


            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];


                for (int l = 0; l < (int)type_tmp.macro_id.size(); l++)
                {
                    int macro_id = type_tmp.macro_id[l];

                    Macro* macro_temp = LG_INFO.MacroClusterSet[macro_id];


                    if (macro_temp->ConsiderCal && macro_temp->LegalFlag)
                    {
                        continue;
                    }

                    // Select_Packing_Corner
                    Select_Packing_Corner(LG_INFO, occupy_ratio, region_tmp.p_region->rectangle, macro_temp);


                    if (macro_temp->LegalFlag == true)
                    {

                    }
                    else
                    {
                        illegal_set.push(macro_id);
                    }

                }

            }
        }
    }
    Leagalize_Illegal_Macro(LG_INFO, illegal_set);

}

void Packing_Boundary_Ordering_Based_Legalization(Legalization_INFO& LG_INFO, pair<int, int> WhiteCenter)
{

    queue<int> illegal_set;

    /// put the "no" rip-up macros to AllCornerNode
    for (int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        if (macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {
            // macro is placed, input to coner sitiching data structure
            int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
            CornerNode* starNode = LG_INFO.AllCornerNode[startID];
            /*vector<CornerNode*> combination;
            combination.reserve(10);
            pair<int, int> macro_coor = make_pair(macro_temp->lg.llx, macro_temp->lg.lly);

            combination.push_back(CornerNodePointSearch(macro_coor, starNode));
            Find_topBlank(combination, macro_temp, macro_temp->lg.ury, macro_temp->lg);
            Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, macro_temp, combination);*/ /// COMMENT 2021.03.19

            ///2021.01
            vector<Macro*> Cut_macros;

            queue<CornerNode*> CornerNodeSetQ;
            vector<CornerNode*> CornerNodeSetV;
            Directed_AreaEnumeration(macro_temp->lg, starNode, CornerNodeSetQ);
            while (!CornerNodeSetQ.empty())
            {
                CornerNodeSetV.insert(CornerNodeSetV.begin(), CornerNodeSetQ.front()); //let y from low to high
                CornerNodeSetQ.pop();
            }
            for (int j = 0; j < CornerNodeSetV.size(); j++)
            {
                if (CornerNodeSetV[j]->rectangle.lly >= macro_temp->lg.ury)
                    continue;

                Macro* macro_tmp = new Macro;
                macro_tmp->lg.llx = max(macro_temp->lg.llx, CornerNodeSetV[j]->rectangle.llx);
                macro_tmp->lg.lly = max(macro_temp->lg.lly, CornerNodeSetV[j]->rectangle.lly);
                macro_tmp->lg.urx = min(macro_temp->lg.urx, CornerNodeSetV[j]->rectangle.urx);
                macro_tmp->lg.ury = min(macro_temp->lg.ury, CornerNodeSetV[j]->rectangle.ury);
                macro_tmp->lef_type_ID = macro_temp->lef_type_ID;
                macro_tmp->OriginalMacro = macro_temp;
                Cut_macros.push_back(macro_tmp);
            }
            //////

            ///2021.01
            Macro* cut_macro = macro_temp;
            for (int j = 0; j < Cut_macros.size(); j++)
            {
                cut_macro = Cut_macros[j];
                vector<CornerNode*> combination;
                combination.reserve(10);
                pair<int, int> macro_coor = make_pair(cut_macro->lg.llx, cut_macro->lg.lly);

                combination.push_back(CornerNodePointSearch(macro_coor, starNode));
                Find_topBlank(combination, cut_macro, cut_macro->lg.ury, cut_macro->lg);

                //Cal_BBOX(cut_macro->lg, type_tmp.type_boundary);
                //Cal_BBOX(cut_macro->lg, group_tmp.group_boundary);

                Before_UpdateMacroInDatastruct(LG_INFO.AllCornerNode, cut_macro, combination);
            }
            //////
        }

    }
    /// put the "rip-up" macros to AllCornerNode
    for (int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        if (macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {
            // do nothing
        }
        else
        {
            //            cout<<"refine : "<<macro_temp->macro_id<<endl;
            ORDER_REGION& region_tmp = LG_INFO.region[macro_temp->region_id];

            // Determine_PackDirection
            vector<float> occupy_ratio = Determine_PackDirection(LG_INFO.AllCornerNode, region_tmp.p_region->rectangle);

            // Select_Packing_Corner
            Select_Packing_Corner(LG_INFO, occupy_ratio, region_tmp.p_region->rectangle, macro_temp);


            if (macro_temp->LegalFlag == true)
            {
                // do nothing
            }
            else
            {
                illegal_set.push(macro_temp->macro_id);
            }
        }
    }
    //    getchar();

        // Leagalize_Illegal_Macro LG : illegal_set
    Leagalize_Illegal_Macro(LG_INFO, illegal_set);
}

void Leagalize_Illegal_Macro(Legalization_INFO& LG_INFO, queue<int> illegal_set)
{
    int illegal_number = 0;
    while (!illegal_set.empty())
    {
        int macro_id = illegal_set.front();
        Macro* macro_temp = LG_INFO.MacroClusterSet[macro_id];

        bool escape_legal = false;
        Boundary subregion = LG_INFO.region[macro_temp->region_id].p_region->rectangle;

        float expand_ratio = 0.1;
        while (!macro_temp->LegalFlag && !escape_legal)
        {
            if (!Expand_subregion(subregion, expand_ratio))
            {

                break;
            }
            // Determine_PackDirection
            vector<float> occupy_ratio = Determine_PackDirection(LG_INFO.AllCornerNode, subregion);

            // Select_Packing_Corner
            Select_Packing_Corner(LG_INFO, occupy_ratio, subregion, macro_temp);

        }
        if (macro_temp->LegalFlag == false)
            illegal_number++;
        illegal_set.pop();

    }
    if (illegal_number != 0)
    {
        cout << "[Warrning] Illegal number : " << illegal_number << endl;
    }

}

bool Expand_subregion(Boundary& subregion, float expand_ratio)
{
    int subW = subregion.urx - subregion.llx;
    int subH = subregion.ury - subregion.lly;

    bool expand_W = false;
    bool expand_H = false;
    if (!(chip_boundary.lly < subregion.lly || subregion.ury < chip_boundary.ury))
    {
        expand_H = true;
    }

    if (!(chip_boundary.llx < subregion.llx || subregion.urx < chip_boundary.urx))
    {
        expand_W = true;
    }

    if (!expand_H && (subW > subH || expand_W))
    {
        /// expand vertically
        int expanding_H = (int)(subH * expand_ratio) / 2;
        subregion.lly = subregion.lly - expanding_H;
        subregion.ury = subregion.ury + expanding_H;
        subregion.lly = max(chip_boundary.lly, subregion.lly);
        subregion.ury = min(chip_boundary.ury, subregion.ury);
    }
    else if (!expand_W)
    {
        /// expand horizontally
        int expanding_W = (int)(subW * expand_ratio) / 2;
        subregion.llx = subregion.llx - expanding_W;
        subregion.urx = subregion.urx + expanding_W;
        subregion.llx = max(chip_boundary.llx, subregion.llx);
        subregion.urx = min(chip_boundary.urx, subregion.urx);
    }

    /// stop expanding
    if (expand_W && expand_H)
        return false;

    return true;
}



void Const_ID_TABLE(vector<ID_TABLE>& id_table, vector<Macro_Group>& MacroGroupBySC)
{
    for (int i = 0; i < (int)id_table.size(); i++)
    {
        ID_TABLE& table = id_table[i];
        table._macro_list_ID = i;
    }

    for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        ID_TABLE& table = id_table[macro_id];
        table._MovableMacro_ID = i;
    }


    for (int i = 0; i < (int)MacroGroupBySC.size(); i++)
    {

        Macro_Group& mg_tmp = MacroGroupBySC[i];
        for (int k = 0; k < (int)mg_tmp.member_ID.size(); k++)
        {
            int macro_id = mg_tmp.member_ID[k];
            ID_TABLE& table = id_table[macro_id];
            table._Group_ID = mg_tmp.ID;

        }
    }

}

void Do_Partition(HIER_LEVEL* HIER_LEVEL_temp, vector<Macro_Group>& MacroGroupBySC, vector<ID_TABLE>& id_table, vector<Partition_Region>& P_Region)
{
    Partition_INFO info_tmp;
    vector<Partition_Node>& P_Node = info_tmp.P_Node;
    vector<Partition_Net>& P_Net = info_tmp.P_Net;

    int P_node_size = (int)MacroGroupBySC.size() + PreplacedMacro_ID.size();

    P_Node.resize(P_node_size);
    int count = 0;

    // 1. input the info_tmp.P_Node info
    /// 1.1 update group node
    cout << "   Now run update Group Node" << endl;
    for (int i = 0; i < (int)MacroGroupBySC.size(); i++)
    {
        int node_id = i + count;
        Partition_Node& node_tmp = P_Node[node_id];
        node_tmp.ID = node_id;

        Macro_Group& mg_tmp = MacroGroupBySC[i];

        node_tmp.Macro_Area = mg_tmp.MacroArea;
        node_tmp.STD_Area = mg_tmp.StdAreaInGroup;
        node_tmp.Center = make_pair(mg_tmp.GCenter_X, mg_tmp.GCenter_Y);
        node_tmp.max_w = 0;
        node_tmp.max_h = 0;
        node_tmp.Group_node = true;
        for (int j = 0; j < (int)mg_tmp.member_ID.size(); j++)
        {
            int macro_id = mg_tmp.member_ID[j];
            ID_TABLE& table_tmp = id_table[macro_id];
            table_tmp._P_Node_ID = node_id;
            Macro* macro_temp = macro_list[macro_id];
            node_tmp.max_w = max(macro_temp->cal_w_wo_shrink, node_tmp.max_w);
            node_tmp.max_h = max(macro_temp->cal_h_wo_shrink, node_tmp.max_h);
        }
    }

    count += (int)MacroGroupBySC.size();


    /// 1.2 update preplace
    cout << "   Now run update Preplace" << endl;
    for (int i = 0; i < (int)PreplacedMacro_ID.size(); i++)
    {
        int macro_id = PreplacedMacro_ID[i];
        Macro* macro_tmp = macro_list[macro_id];

        int node_id = i + count;
        Partition_Node& node_tmp = P_Node[node_id];
        node_tmp.ID = node_id;
        node_tmp.Macro_Area = macro_tmp->area;
        node_tmp.max_w = macro_tmp->cal_w_wo_shrink;
        node_tmp.max_h = macro_tmp->cal_h_wo_shrink;
        node_tmp.Group_node = false;
        int GCenterX = (macro_tmp->gp.llx + macro_tmp->gp.urx) / 2;
        int GCenterY = (macro_tmp->gp.lly + macro_tmp->gp.ury) / 2;
        node_tmp.Center = make_pair(GCenterX, GCenterY);

        ID_TABLE& table_tmp = id_table[macro_id];
        table_tmp._P_Node_ID = node_id;
    }

    P_Net.reserve(P_node_size * 10);

    // 2. input the info_tmp.P_Net info (weight, .node_ID)
    ///update fix pin net & net between group
    cout << "   Now run update fix pin net & net between group " << endl;
    set<int> Total_Group_Net_ID;
    for (int i = 0; i < (int)MacroGroupBySC.size(); i++)
    {
        Macro_Group& mg_tmp = MacroGroupBySC[i];
        set<int>& NET_ID = mg_tmp.Net_ID;
        for (set<int>::iterator iter = NET_ID.begin(); iter != NET_ID.end(); iter++)
        {
            Total_Group_Net_ID.insert((*iter));
        }

    }

    for (set<int>::iterator iter = Total_Group_Net_ID.begin(); iter != Total_Group_Net_ID.end(); iter++)
    {
        int net_id = (*iter);
        Net_QP& net = net_list[net_id];
        set<int> node_id;
        for (int i = 0; i < (int)net.macro_idSet.size(); i++)
        {
            int macro_id = net.macro_idSet[i];
            Macro* macro_temp = macro_list[macro_id];
            if (macro_temp->macro_type != STD_CELL)
            {
                ID_TABLE& table_tmp = id_table[macro_id];

                node_id.insert(table_tmp._P_Node_ID);
            }
        }
        // remove the redundant net
        if (node_id.size() > 1)
        {
            Partition_Net net_tmp;
            net_tmp.ID = (int)P_Net.size();
            net_tmp.node_ID.resize(node_id.size());
            net_tmp.net_weight *= 15;
            int i = 0;

            for (set<int>::iterator iteri = node_id.begin(); iteri != node_id.end(); iteri++)
            {
                net_tmp.node_ID[i] = (*iteri);
                i++;
            }
            P_Net.push_back(net_tmp);
        }

    }


    /// update group net
    // according the hier tree to const net
    TraverseHierTreeConst_P_NET(HIER_LEVEL_temp, P_Net, id_table);

    /*************plus 108.04.22****************/
    if (dataflow_PARA != false)
    {
        Construct_dataflow_p_net(MacroGroupBySC, P_Net);
    }
    /******************************************/

    //cout<<"OUTPUT NET"<<endl;

    // 3. update the info_tmp.P_Node[].net_ID
    for (int i = 0; i < (int)P_Net.size(); i++)
    {
        Partition_Net& net = P_Net[i];
        net.net_weight *= 10;
        //        cout<<"NET ID : "<<net.ID<<endl;
        for (int j = 0; j < (int)net.node_ID.size(); j++)
        {
            //            cout<<net.node_ID[j]<<" ";
            int node_id = net.node_ID[j];
            Partition_Node& node_tmp = P_Node[node_id];
            node_tmp.net_ID.push_back(i);
        }
        //        cout<<endl;
    }


    cout << "   Now run Recurrsive_Partition " << endl;
    Recurrsive_Partition_yu(info_tmp, P_Region, (int)MacroGroupBySC.size(), MacroGroupBySC);


    //    getchar();
}

void TraverseHierTreeConst_P_NET(HIER_LEVEL* HIER_LEVEL_temp, vector<Partition_Net>& P_Net, vector<ID_TABLE>& id_table)
{
    vector<int>& macro_ID = HIER_LEVEL_temp->UnderLevelMacroSet;

    Partition_Net net_tmp;
    net_tmp.ID = (int)P_Net.size();
    net_tmp.net_weight = HIER_LEVEL_temp->Level_num + 1;

    set<int> Group_ID;
    for (int i = 0; i < (int)macro_ID.size(); i++)
    {
        int macro_id = macro_ID[i];
        ID_TABLE& table_tmp = id_table[macro_id];
        if (table_tmp._P_Node_ID == -1)
        {
            cout << "[ERROR] FUNCT - TraverseHierTreeConst_P_NET in macro_legalizer.cpp" << endl;
            for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
            {
                cout << macro_list[MovableMacro_ID[i]]->macro_name << endl;
            }
        }
        Group_ID.insert(table_tmp._P_Node_ID);
    }
    if (!(Group_ID.size() > 1))
    {
        return;
    }
    net_tmp.node_ID.resize(Group_ID.size());
    int i = 0;
    for (set<int>::iterator iter = Group_ID.begin(); iter != Group_ID.end(); iter++)
    {
        net_tmp.node_ID[i] = (*iter);
        i++;
    }

    P_Net.push_back(net_tmp);


    map<string, HIER_LEVEL*>& Name_HierGroup = HIER_LEVEL_temp->Name_HierGroup;
    for (map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
    {
        TraverseHierTreeConst_P_NET(iter->second, P_Net, id_table);
    }
}


/********************************************plus 108.04.22*******************************************/
void Construct_dataflow_p_net(vector<Macro_Group>& MacroGroupBySC, vector<Partition_Net>& P_Net)
{
    for (int i = 0; i < (int)MacroGroupBySC.size() - 1; i++)
    {
        for (int j = i + 1; j < (int)MacroGroupBySC.size(); j++)
        {
            Macro_Group& mg_tmpi = MacroGroupBySC[i];
            Macro_Group& mg_tmpj = MacroGroupBySC[j];
            Partition_Net net_tmp;
            net_tmp.ID = (int)P_Net.size();

            float Hier_Score = 0;
            string Hier_Namei = "";
            string Hier_Namej = "";

            unsigned int k = 0, l = 0;
            while (k < mg_tmpi.HierName.size())
            {
                while (l < mg_tmpj.HierName.size())
                {
                    if (mg_tmpi.HierName[k] == mg_tmpj.HierName[l])
                    {
                        if (Hier_Score == 0)
                        {
                            Hier_Namei += mg_tmpi.HierName[k];
                            Hier_Namej += mg_tmpj.HierName[l];
                        }
                        else
                        {
                            Hier_Namei = Hier_Namei + "/" + mg_tmpi.HierName[k];
                            Hier_Namej = Hier_Namej + "/" + mg_tmpj.HierName[l];
                        }

                        Hier_Score++;
                        k++;
                        l++;
                        // net_tmp.net_weight = Related_dataflow(Hier_Namei, Hier_Namej, Hier_Score);
                    }
                    else
                    {
                        if (Hier_Score == 0)
                        {
                            Hier_Namei += mg_tmpi.HierName[k];
                            Hier_Namej += mg_tmpj.HierName[l];
                        }
                        else
                        {
                            Hier_Namei = Hier_Namei + "/" + mg_tmpi.HierName[k];
                            Hier_Namej = Hier_Namej + "/" + mg_tmpj.HierName[l];
                        }
                        net_tmp.net_weight = Related_dataflow(Hier_Namei, Hier_Namej, Hier_Score);
                        //cout<<"a: "<<Hier_Namei<<"  "<<"b: "<<Hier_Namej<< "  Hier_Score: "<<Hier_Score
                        //<<"  net_weight: "<<net_tmp.net_weight<<endl;

                        /*int ID = mg_tmpi.member_ID[0];
                        Macro * temp = macro_list[ID];
                        cout<< temp->macro_name<<endl;
                        ID = mg_tmpj.member_ID[0];
                        temp = macro_list[ID];
                        cout<< temp->macro_name<<endl;
                        getchar();*/
                        break;
                    }

                }
                break;
            }


            net_tmp.node_ID.resize(2);
            net_tmp.node_ID[0] = mg_tmpi.ID;
            net_tmp.node_ID[1] = mg_tmpj.ID;
            if (net_tmp.net_weight != 0)
            {
                P_Net.push_back(net_tmp);
            }

        }
    }
}

float Related_dataflow(string a, string b, float hier_score)
{
    map<string, set<string> >::iterator iter_a;
    map<string, set<string> >::iterator iter_b;
    set<string>::iterator iters;
    iter_a = DataFlow_outflow.find(a);
    iter_b = DataFlow_outflow.find(b);
    float score = 0;

    /// direct relationship
    if (iter_a != DataFlow_outflow.end())
    {
        iters = iter_a->second.find(b);
        if (iters != iter_a->second.end())
        {
            //if(hier_score != 0)
            return score = (hier_score)+(hier_score + 1) * (1 / (float)iter_a->second.size());
            //return score = (1/iter_a->second.size()) *10;
        }
    }
    if (iter_b != DataFlow_outflow.end())
    {
        iters = iter_b->second.find(a);
        if (iters != iter_b->second.end())
        {
            //if(hier_score != 0)
            return score = (hier_score)+(hier_score + 1) * (1 / (float)iter_b->second.size());
            //return score = (1/iter_b->second.size());
        }
    }

    /// indirect relationship
    if (iter_a != DataFlow_outflow.end() && iter_b != DataFlow_outflow.end())
    {
        map<string, set<string> >::iterator iterin_a;
        map<string, set<string> >::iterator iterin_b;
        set<string>::iterator iterSET_a;
        set<string>::iterator iterSET_b;
        float in_score = 0;
        float out_score = 0;
        /*******common inflow***********/
        iterin_a = DataFlow_inflow.find(a);
        iterin_b = DataFlow_inflow.find(b);
        if (iterin_a != DataFlow_inflow.end() && iterin_b != DataFlow_inflow.end())
        {
            for (iterSET_a = iterin_a->second.begin(); iterSET_a != iterin_a->second.end(); iterSET_a++)
            {
                for (iterSET_b = iterin_b->second.begin(); iterSET_b != iterin_b->second.end(); iterSET_b++)
                {
                    if (*iterSET_a == *iterSET_b)
                    {
                        map<string, set<string> >::iterator iterin;
                        iterin = DataFlow_outflow.find(*iterSET_a);
                        int Size = iterin->second.size();
                        in_score += 1 / (float)Size;
                    }
                }
            }
        }
        /*******common outflow*********/
        iterin_a = DataFlow_outflow.find(a);
        iterin_b = DataFlow_outflow.find(b);
        if (iterin_a != DataFlow_outflow.end() && iterin_b != DataFlow_outflow.end())
        {
            for (iterSET_a = iterin_a->second.begin(); iterSET_a != iterin_a->second.end(); iterSET_a++)
            {
                for (iterSET_b = iterin_b->second.begin(); iterSET_b != iterin_b->second.end(); iterSET_b++)
                {
                    if (*iterSET_a == *iterSET_b)
                    {
                        map<string, set<string> >::iterator iterin;
                        iterin = DataFlow_inflow.find(*iterSET_a);

                        int Size = iterin->second.size();
                        out_score += 1 / (float)Size;
                    }
                }
            }
        }
        score = (hier_score)+(hier_score + 1) * in_score * out_score;
        return score;
    }
    else
    {
        return score = hier_score;
    }


    /*if(iter_a != DataFlow_outflow.end() && iter_b != DataFlow_outflow.end())
    {
        map<string, set<string> >::iterator iterin_a;
        map<string, set<string> >::iterator iterin_b;
        set<string>::iterator iterSET_a;
        set<string>::iterator iterSET_b;
        ///common inflow
        iterin_a = DataFlow_inflow.find(a);
        iterin_b = DataFlow_inflow.find(b);
        if(iterin_a != DataFlow_inflow.end() && iterin_b != DataFlow_inflow.end())
        {
            //cout<<"**situation 2-1**"<<endl;
            for(iterSET_a = iterin_a->second.begin(); iterSET_a != iterin_a->second.end(); iterSET_a++)
            {
                for(iterSET_b = iterin_b->second.begin(); iterSET_b != iterin_b->second.end(); iterSET_b++)
                {
                    if( *iterSET_a == *iterSET_b)
                    {
                        map<string, set<string> >::iterator iterin;
                        iterin = DataFlow_outflow.find(*iterSET_a);

                        int Size = iterin->second.size();
                        if(score != 0)
                            score *= (hier_score+1) * (2/(float)Size);
                        else
                            score += (hier_score+1) * (2/(float)Size);
                    }
                }
            }
        }
        ///common outflow
        iterin_a = DataFlow_outflow.find(a);
        iterin_b = DataFlow_outflow.find(b);
        if(iterin_a != DataFlow_outflow.end() && iterin_b != DataFlow_outflow.end())
        {
            //cout<<"**situation 2-2**"<<endl;
            for(iterSET_a = iterin_a->second.begin(); iterSET_a != iterin_a->second.end(); iterSET_a++)
            {
                for(iterSET_b = iterin_b->second.begin(); iterSET_b != iterin_b->second.end(); iterSET_b++)
                {
                    if( *iterSET_a == *iterSET_b)
                    {
                        map<string, set<string> >::iterator iterin;
                        iterin = DataFlow_inflow.find(*iterSET_a);

                        int Size = iterin->second.size();
                        if(score != 0)
                            score *= (hier_score+1) * (2/(float)Size);
                        else
                            score += (hier_score+1) * (2/(float)Size);

                    }
                }
            }
        }


        //return score*0.05 ;
        return score*10;
    }
    else
    {
        //if(hier_score != 0)
        //cout<<"**situation 1**"<<endl;
        return score = (hier_score+1) - 1;             ///> situation 1
        //return score;
    }*/



    return score;

}
/***************************************************************************************************/

void PLOTMG(vector<Macro_Group>& MacroGroupBySC, vector<ID_TABLE>& id_table)
{
    cout << "   Write ./output/Group/Group.m" << endl;
    for (int i = 0; i < (int)MacroGroupBySC.size(); i++)
    {
        Macro_Group& mg_tmp = MacroGroupBySC[i];
        if (mg_tmp.Dont_Care == false)
        {
            float total_area = 0;
            float CenterX = 0;
            float CenterY = 0;
            Boundary GroupBBox;
            GroupBBox.llx = INT_MAX;
            GroupBBox.lly = INT_MAX;
            GroupBBox.urx = INT_MIN;
            GroupBBox.ury = INT_MIN;
            for (unsigned int k = 0; k < mg_tmp.member_ID.size(); k++)
            {
                int ID = mg_tmp.member_ID[k];
                total_area += macro_list[ID]->area;
                Cal_BBOX(macro_list[ID]->gp, GroupBBox);
            }
            for (unsigned int k = 0; k < mg_tmp.member_ID.size(); k++)
            {
                int ID = mg_tmp.member_ID[k];
                Macro* macro_temp = macro_list[ID];
                float mass_Weight = macro_temp->area / total_area;
                CenterX += mass_Weight * (macro_temp->gp.llx + macro_temp->gp.urx) / 2;
                CenterY += mass_Weight * (macro_temp->gp.lly + macro_temp->gp.ury) / 2;

            }
            vector<PLOT_BLOCK> plot_block;
            vector<PLOT_LINE> plot_line;
            PLOT_BLOCK blk_tmp;

            blk_tmp.color = 'w';
            blk_tmp.TEXT = false;
            blk_tmp.plotrectangle = chip_boundary;
            plot_block.push_back(blk_tmp);
            for (unsigned int k = 0; k < mg_tmp.member_ID.size(); k++)
            {
                int ID = mg_tmp.member_ID[k];
                Macro* macro_temp = macro_list[ID];

                blk_tmp.color = 'r';
                blk_tmp.int_TEXT = id_table[ID]._MovableMacro_ID;
                blk_tmp.TEXT = true;
                blk_tmp.int_stringTEXT = true;
                blk_tmp.plotrectangle = macro_temp->gp;
                plot_block.push_back(blk_tmp);

            }

            PLOT_LINE line_tmp;
            line_tmp.color = 'k';
            Boundary_Assign(line_tmp.plotline, (int)CenterX, GroupBBox.lly, (int)CenterX, GroupBBox.ury);
            plot_line.push_back(line_tmp);
            Boundary_Assign(line_tmp.plotline, GroupBBox.llx, (int)CenterY, GroupBBox.urx, (int)CenterY);
            plot_line.push_back(line_tmp);
            string i_name = int2str(i);
            string file_name = "./output/Group/Group";
            file_name += i_name;
            file_name += ".m";

            // output matlab "group[i].m"
            PlotMatlab(plot_block, plot_line, true, true, file_name);
            string member_name = file_name + i_name + ".txt";
            /*
            ofstream fout(member_name.c_str());
            for(unsigned int k = 0; k < mg_tmp.member_ID.size(); k++)
            {
                int ID = mg_tmp.member_ID[k];
                Macro* macro_temp = macro_list[ID];
                fout<< macro_temp->macro_name <<"\t"<< macro_temp->macro_id <<endl;

            }
            fout.close();
            */
        }

    }
}

vector<float> Determine_PackDirection(vector<CornerNode*> AllCornerNode, Boundary region_boundary)
{
    /// detect the preplaced macros block the boudaries of the subregion
    Boundary Expand_Region;
    Expand_Region.llx = max(chip_boundary.llx, region_boundary.llx - 10);
    Expand_Region.lly = max(chip_boundary.lly, region_boundary.lly - 10);
    Expand_Region.urx = min(chip_boundary.urx, region_boundary.urx + 10);
    Expand_Region.ury = min(chip_boundary.ury, region_boundary.ury + 10);
    int Vert_length = region_boundary.ury - region_boundary.lly;
    int Hori_length = region_boundary.urx - region_boundary.llx;

    int start_ID = rand() % FixedPreplacedInCornerStitching.size();
    start_ID = FixedPreplacedInCornerStitching[start_ID];
    queue<CornerNode*> CornerNodeSet;

    // Directed_AreaEnumeration
    Directed_AreaEnumeration(Expand_Region, AllCornerNode[start_ID], CornerNodeSet);


    //    plot_Directed_AreaEnumeration(CornerNodeSet, region_boundary);getchar();
    set<pair<int, int>, cmp_pair> Top_Edge;
    set<pair<int, int>, cmp_pair> Bot_Edge;
    set<pair<int, int>, cmp_pair> Lef_Edge;
    set<pair<int, int>, cmp_pair> Rig_Edge;

    // for all CornerNodeSet
    while (!CornerNodeSet.empty())
    {
        CornerNode* corner_temp = CornerNodeSet.front();
        if (corner_temp->NodeType != Fixed)
        {
            CornerNodeSet.pop();
            continue;
        }
        //        cout<<"ID : "<<corner_temp->CornerNode_id<<endl;
        int llx = max(corner_temp->rectangle.llx, region_boundary.llx);
        int urx = min(corner_temp->rectangle.urx, region_boundary.urx);
        int lly = max(corner_temp->rectangle.lly, region_boundary.lly);
        int ury = min(corner_temp->rectangle.ury, region_boundary.ury);

        if (!(corner_temp->rectangle.ury < region_boundary.ury))
        {
            Top_Edge.insert(make_pair(llx, urx));
        }

        if (!(corner_temp->rectangle.lly > region_boundary.lly))
        {
            Bot_Edge.insert(make_pair(llx, urx));
        }

        if (!(corner_temp->rectangle.llx > region_boundary.llx))
        {
            Lef_Edge.insert(make_pair(lly, ury));
        }

        if (!(corner_temp->rectangle.urx < region_boundary.urx))
        {

            Rig_Edge.insert(make_pair(lly, ury));
        }
        CornerNodeSet.pop();
    }
    //    cout<<"TOP"<<endl;
    int top_Blocked_length = Block_Edge_Caculation(Top_Edge);
    //    cout<<"BOT"<<endl;
    int bot_Blocked_length = Block_Edge_Caculation(Bot_Edge);
    //    cout<<"LEF"<<endl;
    int lef_Blocked_length = Block_Edge_Caculation(Lef_Edge);
    //    cout<<"RIG"<<endl;
    int rig_Blocked_length = Block_Edge_Caculation(Rig_Edge);
    //    cout<<"Vert_length : "<<Vert_length<<"\tHori_length : "<<Hori_length<<endl;
    vector<float> occupy_ratio;
    occupy_ratio.resize(4);
    occupy_ratio[0] = top_Blocked_length / (float)Hori_length; /// top boundary
    occupy_ratio[1] = bot_Blocked_length / (float)Hori_length; /// bot boundary
    occupy_ratio[2] = lef_Blocked_length / (float)Vert_length; /// lef boundary
    occupy_ratio[3] = rig_Blocked_length / (float)Vert_length; /// rig boundary

    if (region_boundary.ury == chip_boundary.ury)
    {
        occupy_ratio[0] = 1;
    }
    if (region_boundary.lly == chip_boundary.lly)
    {
        occupy_ratio[1] = 1;
    }
    if (region_boundary.llx == chip_boundary.llx)
    {
        occupy_ratio[2] = 1;
    }
    if (region_boundary.urx == chip_boundary.urx)
    {
        occupy_ratio[3] = 1;
    }


    return occupy_ratio;
}

int Block_Edge_Caculation(set<pair<int, int>, cmp_pair>& Edge)
{
    if (Edge.size() == 0)
    {
        return 0;
    }

    int blocked_length = 0;
    pair<int, int> mark;
    set<pair<int, int>, cmp_pair>::iterator iter = Edge.begin();
    mark = (*iter);

    for (; iter != Edge.end(); iter++)
    {
        //        cout<<"first : "<<(*iter).first<<"\tsecond : "<<(*iter).second<<endl;
        if (!((*iter).first > mark.second))
        {
            mark.second = max((*iter).second, mark.second);
        }
        else
        {
            //            cout<<"cal : "<<blocked_length<<endl;
            blocked_length += mark.second - mark.first;
            mark = (*iter);
        }
    }
    blocked_length += mark.second - mark.first;
    //    cout<<"BLOCKED LENGTH : "<<blocked_length<<endl;

    return blocked_length;

}


void Evaluate_Placement(Legalization_INFO& LG_INFO)
{
    COST_TERM cost_temp;
    vector<Boundary> Macro_Solution;
    Macro_Solution.resize(LG_INFO.MacroClusterSet.size());
    vector<Macro*>& MacroCluster = LG_INFO.MacroClusterSet;
    vector<CornerNode*>& AllCornerNode = LG_INFO.AllCornerNode;

    // cost_temp.Displacement
    // cost_temp.Center_Distance
    // cost_temp.Thickness
    for (int i = 0; i < (int)MacroCluster.size(); i++)
    {
        Macro* macro_temp = MacroCluster[i];

        if (macro_temp->LegalFlag != true)
            return;

        cost_temp.Displacement += pow(pow(fabs(macro_temp->lg.llx - macro_temp->gp.llx) / (float)PARA, 2) +
            pow(fabs(macro_temp->lg.lly - macro_temp->gp.lly) / (float)PARA, 2), 0.5) * macro_temp->macro_area_ratio;

        ///2021.02
        float exp_x = 10;
        float exp_y = 10;
        float scale_down = (float)1 / (float)10;

        float exp1 = 10;
        int full_W = full_boundary.urx - full_boundary.llx;
        int full_H = full_boundary.ury - full_boundary.lly;
        exp1 = (float)10 * (log(0.5 * (float)min(full_W, full_H))) / (log(0.5 * (float)max(full_W, full_H)));

        if (full_W > full_H)
            exp_x = exp1;
        if (full_H > full_W)
            exp_y = exp1;

        float center_dist = (float)(pow(pow(fabs(macro_temp->Macro_Center.first - BenchInfo.WhiteSpaceXCenter), exp_x)
            + pow(fabs(macro_temp->Macro_Center.second - BenchInfo.WhiteSpaceYCenter), exp_y), scale_down)) / (float)PARA;

        //float center_dist = (float)( pow( pow(fabs(macro_temp->Macro_Center.first - BenchInfo.WhiteSpaceXCenter),10)
                                        //+ pow(fabs(macro_temp->Macro_Center.second - BenchInfo.WhiteSpaceYCenter),10) ,0.1))/ (float)PARA;
        //////

        center_dist *= macro_temp->macro_area_ratio;
        cost_temp.Center_Distance += 1 / center_dist;
        cost_temp.Thickness += LG_INFO.MacroClusterSet_Cost[i].Thickness * macro_temp->macro_area_ratio;

        /// ADD 2021.04, calculate congestion cost
        if (Refinement_Flag == true)
        {
            float cong_cost = 0;
            Boundary macro_boundary;
            Boundary_Assign(macro_boundary, macro_temp->lg.llx, macro_temp->lg.lly, macro_temp->lg.urx, macro_temp->lg.ury);
            cong_cost = CONGESTIONMAP::Enumerate_H_Edge(&CG_INFO, macro_boundary, false);
            cost_temp.Congestion += cong_cost;
            cong_cost = CONGESTIONMAP::Enumerate_V_Edge(&CG_INFO, macro_boundary, false);
            cost_temp.Congestion += cong_cost;
        }
        //////

        Macro_Solution[i] = macro_temp->lg; // record the solution of the macro
    }

    // cost_temp.Blank_Center_Dist (chk OK)
    for (int i = 0; i < (int)AllCornerNode.size(); i++)
    {
        CornerNode* corner_temp = AllCornerNode[i];
        if (corner_temp == NULL)
            continue;

        if (corner_temp->NodeType == Blank)
        {
            int W = corner_temp->rectangle.urx - corner_temp->rectangle.llx;
            int H = corner_temp->rectangle.ury - corner_temp->rectangle.lly;

            int CenterX = (corner_temp->rectangle.urx + corner_temp->rectangle.llx) / 2;
            int CenterY = (corner_temp->rectangle.ury + corner_temp->rectangle.lly) / 2;

            float area_ratio = W / (float)PARA * H / (float)PARA / BenchInfo.blank_area;

            float Hori_Dist = fabs(BenchInfo.WhiteSpaceXCenter - CenterX) / (float)PARA;
            float Vert_Dist = fabs(BenchInfo.WhiteSpaceYCenter - CenterY) / (float)PARA;

            cost_temp.Blank_Center_Dist += (Hori_Dist * Hori_Dist + Vert_Dist * Vert_Dist) * area_ratio;
        }
    }
    // cost_temp.Type_Distance
    // cost_temp.Group_Distance
    for (int i = 0; i < (int)LG_INFO.region.size(); i++)
    {
        ORDER_REGION& region_tmp = LG_INFO.region[i];
        for (int j = 0; j < (int)region_tmp.group.size(); j++)
        {
            ORDER_GROUP& group_tmp = region_tmp.group[j];
            for (int k = 0; k < (int)group_tmp.lef_type.size(); k++)
            {
                ORDER_TYPE& type_tmp = group_tmp.lef_type[k];
                Boundary& type_boundary = type_tmp.type_boundary;
                float type_box_area = (type_boundary.urx - type_boundary.llx) / (float)PARA * (type_boundary.ury - type_boundary.lly) / (float)PARA;
                cost_temp.Type_Distance += type_box_area / type_tmp.total_macro_area;
            }

            Boundary& group_boundary = group_tmp.group_boundary;
            float group_box_area = (group_boundary.urx - group_boundary.llx) / (float)PARA * (group_boundary.ury - group_boundary.lly) / (float)PARA;
            cost_temp.Group_Distance += group_box_area / group_tmp.total_macro_area;
            //            Boundary_Cout(group_boundary);
        }
    }
    // cost_temp.WireLength
    for (int i = 0; i < (int)LG_INFO.Net_INFO.size(); i++)
    {
        Boundary BBox;
        Boundary_Assign_Limits(BBox);
        for (int j = 0; j < (int)LG_INFO.Net_INFO[i].size(); j++)
        {
            pair<int, int> macroCoor = LG_INFO.Net_INFO[i][j]->Macro_Center;
            if (BBox.urx < macroCoor.first)
            {
                BBox.urx = macroCoor.first;
            }
            if (BBox.ury < macroCoor.second)
            {
                BBox.ury = macroCoor.second;
            }
            if (BBox.llx > macroCoor.first)
            {
                BBox.llx = macroCoor.first;
            }
            if (BBox.lly > macroCoor.second)
            {
                BBox.lly = macroCoor.second;
            }
        }
        cost_temp.WireLength += (BBox.urx - BBox.llx) + (BBox.ury - BBox.lly);
    }
    //    cout<<"WL : "<<cost_temp.WireLength<<endl;
    //    cout<<cost_temp.Blank_Center_Dist<<"\t"<<cost_temp.Center_Distance<<"\t"<<cost_temp.Displacement<<"\t"<<cost_temp.Type_Distance<<"\t"<<cost_temp.Group_Distance<<endl;getchar();
        // cost.temp.Congestion

        /// ADD 2021.06
    if (Build_Set == true)
    {
        vector<MacrosSet*/*, cmp_id*/> set_container_tmp;
        for (vector<MacrosSet*>::iterator i = Macros_Set.begin(); i != Macros_Set.end(); i++)
        {
            cost_temp.Regularity += 1 / (*i)->total_over_bbx;


            MacrosSet* set_tmp = new MacrosSet;
            set_tmp->id = (*i)->id;
            set_tmp->members = (*i)->members;
            set_tmp->bbx = (*i)->bbx;
            set_tmp->total_area = (*i)->total_area;
            set_tmp->bbx_area = (*i)->bbx_area;
            set_tmp->bbx_overlap_area = (*i)->bbx_overlap_area;
            set_tmp->total_over_bbx = (*i)->total_over_bbx;
            set_tmp->H_W_ratio = (*i)->H_W_ratio;
            set_tmp->occupy_ratio = (*i)->occupy_ratio;
            Update_Each_Set(set_tmp, AllCornerNode);
            set_container_tmp.push_back(set_tmp);
        }
        LG_INFO.set_container.push_back(set_container_tmp);
    }
    //////
    LG_INFO.PARAMETER_container.push_back(PARAMETER);
    LG_INFO.cost_container.push_back(cost_temp);
    LG_INFO.Macro_Solution.push_back(Macro_Solution);
}


void Choose_Best_Placement(Legalization_INFO& LG_INFO)
{
    pair<float, float> Blank_Center_Dist_max_min = make_pair(FLT_MIN, FLT_MAX);
    pair<float, float> Displacement_max_min = Blank_Center_Dist_max_min;
    pair<float, float> center_dist_max_min = Blank_Center_Dist_max_min;
    pair<float, float> Group_Distance_max_min = Blank_Center_Dist_max_min;
    pair<float, float> Type_Distance_max_min = Blank_Center_Dist_max_min;
    pair<float, float> Thickness_max_min = Blank_Center_Dist_max_min;
    pair<float, float> WireLength_max_min = Blank_Center_Dist_max_min;
    /// 2021.04 & 07
    pair<float, float> Congestion_max_min = Blank_Center_Dist_max_min;
    pair<float, float> Regularity_max_min = Blank_Center_Dist_max_min;
    //////

    for (int i = 0; i < (int)LG_INFO.cost_container.size(); i++)
    {
        COST_TERM& cost_temp = LG_INFO.cost_container[i];

        Pair_max_min(cost_temp.Blank_Center_Dist, Blank_Center_Dist_max_min);
        Pair_max_min(cost_temp.Displacement, Displacement_max_min);
        Pair_max_min(cost_temp.Center_Distance, center_dist_max_min);
        Pair_max_min(cost_temp.Group_Distance, Group_Distance_max_min);
        Pair_max_min(cost_temp.Type_Distance, Type_Distance_max_min);
        Pair_max_min(cost_temp.Thickness, Thickness_max_min);
        Pair_max_min(cost_temp.WireLength, WireLength_max_min);
        /// 2021.04
        if (Refinement_Flag == true)
            Pair_max_min(cost_temp.Congestion, Congestion_max_min);
        //////
        /// ADD 2021.07
        if (Build_Set == true)
        {
            Pair_max_min(cost_temp.Regularity, Regularity_max_min);
        }
        //////
    }


    float Blank_Center_Dist_range = Blank_Center_Dist_max_min.first - Blank_Center_Dist_max_min.second;
    float Displacement_range = Displacement_max_min.first - Displacement_max_min.second;
    float center_dist_range = center_dist_max_min.first - center_dist_max_min.second;
    float Group_Distance_range = Group_Distance_max_min.first - Group_Distance_max_min.second;
    float Type_Distance_range = Type_Distance_max_min.first - Type_Distance_max_min.second;
    float Thickness_range = Thickness_max_min.first - Thickness_max_min.second;
    float WireLength_range = WireLength_max_min.first - WireLength_max_min.second;
    /// 2021.04
    float Congestion_range = Congestion_max_min.first - Congestion_max_min.second;
    //////
    /// ADD 2021.07
    float Regularity_range = Regularity_max_min.first - Regularity_max_min.second;
    //////

    float Best_Cost = FLT_MAX;
    int Best_ID = -1;

    for (int i = 0; i < (int)LG_INFO.cost_container.size(); i++)
    {
        if (Refinement_Flag == true && i == 0)
            continue;
        /*if(Congestion_PARA == true && Build_Set == true && i == 0)
            continue;*/
        COST_TERM& cost_temp = LG_INFO.cost_container[i];


        cost_temp.Blank_Center_Dist = Scaling_Cost(Blank_Center_Dist_range, cost_temp.Blank_Center_Dist - Blank_Center_Dist_max_min.second);
        cost_temp.Displacement = Scaling_Cost(Displacement_range, cost_temp.Displacement - Displacement_max_min.second);
        cost_temp.Center_Distance = Scaling_Cost(center_dist_range, cost_temp.Center_Distance - center_dist_max_min.second);
        cost_temp.Group_Distance = Scaling_Cost(Group_Distance_range, cost_temp.Group_Distance - Group_Distance_max_min.second);
        cost_temp.Type_Distance = Scaling_Cost(Type_Distance_range, cost_temp.Type_Distance - Type_Distance_max_min.second);
        cost_temp.Thickness = Scaling_Cost(Thickness_range, cost_temp.Thickness - Thickness_max_min.second);
        cost_temp.WireLength = Scaling_Cost(WireLength_range, cost_temp.WireLength - WireLength_max_min.second);
        /// 2021.04
        if (Refinement_Flag == true)
            cost_temp.Congestion = Scaling_Cost(Congestion_range, cost_temp.Congestion - Congestion_max_min.second);
        //////
        /// ADD 2021.07
        cost_temp.Regularity = Scaling_Cost(Regularity_range, cost_temp.Regularity - Regularity_max_min.second);
        //////

        float cost = cost_temp.Blank_Center_Dist * PARAMETER._CHOOSE_Blank_Center_Dist;
        cost += cost_temp.Displacement * PARAMETER._CHOOSE_Displacement;
        cost += cost_temp.Center_Distance * PARAMETER._CHOOSE_Center_Distance;
        cost += cost_temp.Group_Distance * PARAMETER._CHOOSE_Group_Distance;
        cost += cost_temp.Type_Distance * PARAMETER._CHOOSE_Type_Distance;
        cost += cost_temp.Thickness * PARAMETER._CHOOSE_Thickness;
        cost += cost_temp.WireLength * PARAMETER._CHOOSE_WireLength;
        /// 2021.04
        if (Refinement_Flag == true) {
            cost += cost_temp.Congestion * PARAMETER._CHOOSE_Congestion;
            /*cout<<"No."<<i<<" placement result"<<endl;
            cout<<"cost_temp.Blank_Center_Dist: "<<cost_temp.Blank_Center_Dist *  PARAMETER._CHOOSE_Blank_Center_Dist<<endl;
            cout<<"cost_temp.Displacement: "<<cost_temp.Displacement *           PARAMETER._CHOOSE_Displacement<<endl;
            cout<<"cost_temp.Center_Distance: "<<cost_temp.Center_Distance *        PARAMETER._CHOOSE_Center_Distance<<endl;
            cout<<"cost_temp.Group_Distance: "<<cost_temp.Group_Distance *          PARAMETER._CHOOSE_Group_Distance<<endl;
            cout<<"cost_temp.Type_Distance: "<<cost_temp.Type_Distance *           PARAMETER._CHOOSE_Type_Distance<<endl;
            cout<<"cost_temp.Thickness: "<<cost_temp.Thickness *               PARAMETER._CHOOSE_Thickness<<endl;
            cout<<"cost_temp.WireLength: "<<cost_temp.WireLength *              PARAMETER._CHOOSE_WireLength<<endl;
            //cout<<"cost_temp.Congestion: "<<cost_temp.Congestion *          PARAMETER._CHOOSE_Congestion<<endl;
            cout<<"total cost: "<<cost<<endl;
            getchar();*/
        }
        //////
        if (Build_Set == true)
        {
            cost += cost_temp.Regularity * PARAMETER._CHOOSE_Regularity;
        }
        if (cost < Best_Cost)
        {
            Best_Cost = cost;
            Best_ID = i;
            /// ADD 2021.05
            Best_Placement_Cost = Best_Cost;
            //////
        }
    }

    for (int i = 0; i < (int)LG_INFO.MacroClusterSet.size(); i++)
    {
        LG_INFO.MacroClusterSet[i]->lg = LG_INFO.Macro_Solution[Best_ID][i];
    }

    // Record Macro LG position to macro_list[]
    for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro* this_macro = macro_list[macro_id];
        this_macro->lg = LG_INFO.MacroClusterSet[i]->lg;
    }

    PARAMETER = LG_INFO.PARAMETER_container[Best_ID];



    if (Best_ID < INITIALTIMES / 2)
    {
        LG_INFO.legal_mode_0 = true;
        cout << "[INFO] Packing order is according to macro area " << endl;
    }
    else
    {
        LG_INFO.legal_mode_0 = false;
        cout << "[INFO] Packing order is according to region/groups/types order" << endl;
    }
    if (Debug_PARA)
    {
        cout << "[INFO] Best ID is " << Best_ID << endl;
        cout << "[INFO] PACK_WIRELENGTH " << PARAMETER._PACK_WIRELENGTH << endl;
        cout << "[INFO] PACK_TYPE_DIST " << PARAMETER._PACK_TYPE_DIST << endl;
        cout << "[INFO] PACK_CENTER_DIST " << PARAMETER._PACK_CENTER_DIST << endl;
        cout << "[INFO] PACK_DISPLACEMENT " << PARAMETER._PACK_DISPLACEMENT << endl;
        cout << "[INFO] GROUP_DIST " << PARAMETER._PACK_GROUP_DIST << endl;
        cout << "[INFO] PACK_OVERLAP " << PARAMETER._PACK_OVERLAP << endl;
        cout << "[INFO] PACK_THICKNESS " << PARAMETER._PACK_THICKNESS << endl;
    }

    /// ADD 2021.04
    Best_Placement_ID = Best_ID;
    //////

    /// ADD 2021.06, select the set of best placement
    if (Build_Set == true)
    {
        Macros_Set.clear();
        cout << "set_container size: " << LG_INFO.set_container.size() << endl;
        for (vector<MacrosSet*>::iterator i = LG_INFO.set_container[Best_ID].begin(); i != LG_INFO.set_container[Best_ID].end(); i++)
        {
            Macros_Set.push_back(*i);
        }
        cout << "set size: " << Macros_Set.size() << endl;

        for (int i = 0; i < (int)Macros_Set.size(); i++)
        {
            MacrosSet* set = Macros_Set[i];
            for (map<string, Macro*>::iterator j = set->members.begin(); j != set->members.end(); j++)
            {
                Macro* macro = j->second;
                macro->set = set;
            }
        }
    }
    //////

    // initial processing for the next perturb////////
    LG_INFO.PARAMETER_container.clear();
    LG_INFO.cost_container.clear();
    LG_INFO.Macro_Solution.clear();
    /////////////////////////////////////////////////
}

void Update_NET_INFO_IN_EVALUATION(Legalization_INFO& LG_INFO)
{
    map<int, int> macro_listIDvsMovableMacroID;
    for (int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        macro_listIDvsMovableMacroID.insert(make_pair(MovableMacro_ID[i], i));
    }

    vector<vector<Macro*> >& NET_INFO = LG_INFO.Net_INFO;
    NET_INFO.resize(net_list.size());

    for (int i = 0; i < (int)NET_INFO.size(); i++)
    {
        Net_QP& net_tmp = net_list[i];
        vector<Macro*>& NET = NET_INFO[i];
        NET.resize(net_tmp.macro_idSet.size());

        for (int j = 0; j < (int)net_tmp.macro_idSet.size(); j++)
        {
            int macro_id = net_tmp.macro_idSet[j];
            Macro* connect_macro = macro_list[macro_id];

            if (connect_macro->macro_type == MOVABLE_MACRO)
            {
                map<int, int>::iterator iter = macro_listIDvsMovableMacroID.find(connect_macro->macro_id);
                if (iter == macro_listIDvsMovableMacroID.end())
                {
                    cout << "[ERROR] Can't find macro in update net info !!" << endl;
                    exit(1);
                }
                NET[j] = LG_INFO.MacroClusterSet[iter->second];
            }
            else if (connect_macro->macro_type == PRE_PLACED || connect_macro->macro_type == PORT)
            {


                connect_macro->Macro_Center.first = (connect_macro->gp.llx + connect_macro->gp.urx) / 2;
                connect_macro->Macro_Center.second = (connect_macro->gp.lly + connect_macro->gp.ury) / 2;
                connect_macro->LegalFlag = true;
                NET[j] = connect_macro;
            }
            else
            {
                cout << "[ERROR] update net info" << endl;
                exit(1);
            }
        }
    }
}

/// ADD 2021.03.22
int gcd(int x, int y)    // find Greatest common factor
{
    if (x != 0 && y != 0)
        return gcd((x >= y) ? x % y : x, (x < y) ? y % x : y);
    else
        return(x != 0) ? x : y;
}



void DivideTilesToGrids()
{
    /// 1. Construct grids for chip boundary
    int num_width, num_height;
    int GreatestCommonFactor = gcd(chip_boundary.urx, chip_boundary.ury);
    int grid_width = (chip_boundary.urx - chip_boundary.llx) / 2;//320000;
    int grid_height = (chip_boundary.ury - chip_boundary.lly) / 2;//300000;
    num_width = chip_boundary.urx / grid_width;
    num_height = chip_boundary.ury / grid_height;
    vector<vector<CornerNode*> > AllChipGrids;
    AllChipGrids.resize(num_height);
    for (int i = 0; i < (int)AllChipGrids.size(); i++)
    {
        int lly = chip_boundary.lly + i * grid_height;
        int ury = chip_boundary.lly + (i + 1) * grid_height;
        AllChipGrids[i].resize(num_width);
        for (int j = 0; j < (int)AllChipGrids[i].size(); j++)
        {
            int llx = chip_boundary.llx + j * grid_width;
            int urx = chip_boundary.llx + (j + 1) * grid_width;
            CornerNode* tmp = new CornerNode(Blank, -1, llx, lly, urx, ury);
            AllChipGrids[i][j] = tmp;
        }
    }
    /// 2. Replace blank tiles by grids
    //  2.1 find the blank tiles which are overlap with grids
    vector<CornerNode*> BlankGrids;
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        if (PreplacedInCornerStitching[i]->NodeType == Blank)
        {
            int row_from, row_to, col_from, col_to;

            row_from = (PreplacedInCornerStitching[i]->rectangle.lly / grid_height == 0) ? 0 : PreplacedInCornerStitching[i]->rectangle.lly / grid_height;
            row_to = (PreplacedInCornerStitching[i]->rectangle.ury / grid_height == 0) ? 0 : PreplacedInCornerStitching[i]->rectangle.ury / grid_height;
            row_from = (row_from > num_height) ? num_height - 1 : row_from;
            row_to = (row_to > num_height) ? num_height - 1 : row_to;

            col_from = (PreplacedInCornerStitching[i]->rectangle.llx / grid_width == 0) ? 0 : PreplacedInCornerStitching[i]->rectangle.llx / grid_width;
            col_to = (PreplacedInCornerStitching[i]->rectangle.urx / grid_width == 0) ? 0 : PreplacedInCornerStitching[i]->rectangle.urx / grid_width;
            col_from = (col_from > num_width) ? num_width - 1 : col_from;
            col_to = (col_to > num_width) ? num_width - 1 : col_to;

            for (int j = row_from; j <= row_to; j++)
            {
                for (int k = col_from; k <= col_to; k++)
                {
                    CornerNode* grid = new CornerNode(AllChipGrids[j][k]->NodeType, -1, AllChipGrids[j][k]->rectangle.llx, AllChipGrids[j][k]->rectangle.lly, AllChipGrids[j][k]->rectangle.urx, AllChipGrids[j][k]->rectangle.ury);
                    if (grid->rectangle.lly < PreplacedInCornerStitching[i]->rectangle.lly &&
                        grid->rectangle.ury > PreplacedInCornerStitching[i]->rectangle.lly)    // bottom
                    {
                        grid->rectangle.lly = PreplacedInCornerStitching[i]->rectangle.lly;
                    }
                    if (grid->rectangle.lly < PreplacedInCornerStitching[i]->rectangle.ury &&
                        grid->rectangle.ury > PreplacedInCornerStitching[i]->rectangle.ury)    // upper
                    {
                        grid->rectangle.ury = PreplacedInCornerStitching[i]->rectangle.ury;
                    }
                    if (grid->rectangle.llx < PreplacedInCornerStitching[i]->rectangle.llx &&
                        grid->rectangle.urx > PreplacedInCornerStitching[i]->rectangle.llx)    // left
                    {
                        grid->rectangle.llx = PreplacedInCornerStitching[i]->rectangle.llx;
                    }
                    if (grid->rectangle.llx < PreplacedInCornerStitching[i]->rectangle.urx &&
                        grid->rectangle.urx > PreplacedInCornerStitching[i]->rectangle.urx)    // right
                    {
                        grid->rectangle.urx = PreplacedInCornerStitching[i]->rectangle.urx;
                    }
                    BlankGrids.push_back(grid);

                    // debug
                    /*if(AllChipGrids[j][k]->rectangle.urx <= PreplacedInCornerStitching[i]->rectangle.llx ||
                       AllChipGrids[j][k]->rectangle.llx >= PreplacedInCornerStitching[i]->rectangle.urx)
                    {
                        cerr<<"grid: "<<AllChipGrids[j][k]->rectangle.llx<<", "<<AllChipGrids[j][k]->rectangle.lly<<"   "<<AllChipGrids[j][k]->rectangle.urx<<", "<<AllChipGrids[j][k]->rectangle.ury<<endl;
                        cerr<<"blank: "<<PreplacedInCornerStitching[i]->rectangle.llx<<", "<<PreplacedInCornerStitching[i]->rectangle.lly<<"   "<<PreplacedInCornerStitching[i]->rectangle.urx<<", "<<PreplacedInCornerStitching[i]->rectangle.ury<<endl;
                        cerr<<"x, Wrong!!!!!"<<endl;exit(1);
                    }
                    if(AllChipGrids[j][k]->rectangle.ury <= PreplacedInCornerStitching[i]->rectangle.lly ||
                       AllChipGrids[j][k]->rectangle.lly >= PreplacedInCornerStitching[i]->rectangle.ury)
                    {
                        cerr<<"grid: "<<AllChipGrids[j][k]->rectangle.llx<<", "<<AllChipGrids[j][k]->rectangle.lly<<"   "<<AllChipGrids[j][k]->rectangle.urx<<", "<<AllChipGrids[j][k]->rectangle.ury<<endl;
                        cerr<<"blank: "<<PreplacedInCornerStitching[i]->rectangle.llx<<", "<<PreplacedInCornerStitching[i]->rectangle.lly<<"   "<<PreplacedInCornerStitching[i]->rectangle.urx<<", "<<PreplacedInCornerStitching[i]->rectangle.ury<<endl;
                        cerr<<"y, Wrong!!!!!"<<endl;exit(1);
                    }*/
                }
            }
            //getchar();
        }
    }
    /*ofstream fout("gird_blank.m") ;
    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;
    for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        if(PreplacedInCornerStitching[i]->NodeType == Fixed)
        {
            fout<<"block_x=["<<PreplacedInCornerStitching[i]->rectangle.llx<<" "<<PreplacedInCornerStitching[i]->rectangle.llx<<" "<<PreplacedInCornerStitching[i]->rectangle.urx<<" "<<PreplacedInCornerStitching[i]->rectangle.urx<<" "<<PreplacedInCornerStitching[i]->rectangle.llx<<" ];"<<endl;
            fout<<"block_y=["<<PreplacedInCornerStitching[i]->rectangle.lly<<" "<<PreplacedInCornerStitching[i]->rectangle.ury<<" "<<PreplacedInCornerStitching[i]->rectangle.ury<<" "<<PreplacedInCornerStitching[i]->rectangle.lly<<" "<<PreplacedInCornerStitching[i]->rectangle.lly<<" ];"<<endl;
            fout << "text(" << (PreplacedInCornerStitching[i]->rectangle.llx + PreplacedInCornerStitching[i]->rectangle.urx) / 2.0 << ", " << (PreplacedInCornerStitching[i]->rectangle.lly + PreplacedInCornerStitching[i]->rectangle.ury) / 2.0 << ", '" << i << "');" << endl << endl;
            fout<<"fill(block_x, block_y, 'r', 'facealpha', 0.5)"<<endl;
        }
    }
    for(int i = 0; i < (int)BlankGrids.size(); i++)
    {
        CornerNode* grid = BlankGrids[i];
        fout<<"block_x=["<<grid->rectangle.llx<<" "<<grid->rectangle.llx<<" "<<grid->rectangle.urx<<" "<<grid->rectangle.urx<<" "<<grid->rectangle.llx<<" ];"<<endl;
        fout<<"block_y=["<<grid->rectangle.lly<<" "<<grid->rectangle.ury<<" "<<grid->rectangle.ury<<" "<<grid->rectangle.lly<<" "<<grid->rectangle.lly<<" ];"<<endl;
        fout << "text(" << (grid->rectangle.llx + grid->rectangle.urx) / 2.0 << ", " << (grid->rectangle.lly + grid->rectangle.ury) / 2.0 << ", '" << i << "');" << endl << endl;
        fout<<"fill(block_x, block_y, 'c', 'facealpha', 0.5)"<<endl;
    }*/
    // 2.2 store grids in vector PreplacedInCornerStitching
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        if (PreplacedInCornerStitching[i]->NodeType == Fixed)
        {
            BlankGrids.push_back(PreplacedInCornerStitching[i]);
        }
    }
    sort(BlankGrids.begin(), BlankGrids.end(), CornerNode_sort);
    PreplacedInCornerStitching.clear();
    swap(BlankGrids, PreplacedInCornerStitching);
    DeleteCornerNode(BlankGrids);
    //sort(PreplacedInCornerStitching.begin(), PreplacedInCornerStitching.end(), CornerNode_sort);
    /*vector<pair<int, Boundary> > for_plot;
    for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        for_plot.push_back(make_pair(PreplacedInCornerStitching[i]->NodeType, PreplacedInCornerStitching[i]->rectangle));
    }
    Plot_rectangle_w_color(for_plot, "./output/corner_stitching_graph/PreplacedInCornerStitching0322.m");*/
    /// 3. Re-Link tiles
    set<int> set_lly;
    set<int> set_ury;
    set<int> set_llx;
    set<int> set_urx;
    vector<vector<CornerNode*> > row_lly;       // lly are the same, for W
    vector<vector<CornerNode*> > row_ury;       // ury are the same, for E
    vector<vector<CornerNode*> > col_llx;       // llx are the same, for S
    vector<vector<CornerNode*> > col_urx;       // urx are the same, for N
    map<CornerNode*, pair<int, int> > fixed_y;  // record NodeType == Fixed, pair means the y direction range.
    map<CornerNode*, pair<int, int> > fixed_x;  // record NodeType == Fixed, pair means the x direction range.

    // horizontal
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        set_lly.insert(PreplacedInCornerStitching[i]->rectangle.lly);
        set_ury.insert(PreplacedInCornerStitching[i]->rectangle.ury);
        set_llx.insert(PreplacedInCornerStitching[i]->rectangle.llx);
        set_urx.insert(PreplacedInCornerStitching[i]->rectangle.urx);
        if (PreplacedInCornerStitching[i]->NodeType == Fixed)
        {
            pair<int, int> x_range, y_range;
            x_range = make_pair(PreplacedInCornerStitching[i]->rectangle.llx, PreplacedInCornerStitching[i]->rectangle.urx);
            y_range = make_pair(PreplacedInCornerStitching[i]->rectangle.lly, PreplacedInCornerStitching[i]->rectangle.ury);
            pair<CornerNode*, pair<int, int> > tmp;
            tmp = make_pair(PreplacedInCornerStitching[i], x_range);
            fixed_x.insert(tmp);
            tmp = make_pair(PreplacedInCornerStitching[i], y_range);
            fixed_y.insert(tmp);
        }
    }

    for (set<int>::iterator iter = set_lly.begin(); iter != set_lly.end(); iter++)
    {
        vector<CornerNode*> row_lly_tmp;
        for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
        {
            if (PreplacedInCornerStitching[i]->rectangle.lly == (*iter))
            {
                row_lly_tmp.push_back(PreplacedInCornerStitching[i]);
            }
        }
        row_lly.push_back(row_lly_tmp);
    }
    for (set<int>::iterator iter = set_ury.begin(); iter != set_ury.end(); iter++)
    {
        vector<CornerNode*> row_ury_tmp;
        for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
        {
            if (PreplacedInCornerStitching[i]->rectangle.ury == (*iter))
            {
                row_ury_tmp.push_back(PreplacedInCornerStitching[i]);
            }
        }
        sort(row_ury_tmp.begin(), row_ury_tmp.end(), CornerNode_sort_ury);
        row_ury.push_back(row_ury_tmp);
    }
    /*for(int i = 0; i < (int)row_lly.size(); i++)
    {
        for(int j = 0; j < (int)row_lly[i].size(); j++)
        {
            cout<<row_lly[i][j]->rectangle.llx<<", "<<row_lly[i][j]->rectangle.lly<<"  "<<row_lly[i][j]->rectangle.urx<<", "<<row_lly[i][j]->rectangle.ury<<endl;
        }
        getchar();
    }
    for(int i = 0; i < (int)row_ury.size(); i++)
    {
        for(int j = 0; j < (int)row_ury[i].size(); j++)
        {
            cout<<row_ury[i][j]->rectangle.llx<<", "<<row_ury[i][j]->rectangle.lly<<"  "<<row_ury[i][j]->rectangle.urx<<", "<<row_ury[i][j]->rectangle.ury<<endl;
        }
        getchar();
    }exit(1);*/
    for (int i = 0; i < (int)row_lly.size(); i++)    // W
    {
        for (int j = (int)row_lly[i].size() - 1; j >= 0; j--)
        {
            if (j != 0)
            {
                row_lly[i][j]->W = row_lly[i][j - 1];
            }
        }
    }
    for (int i = 0; i < (int)row_ury.size(); i++)    // E
    {
        for (int j = 0; j < (int)row_ury[i].size(); j++)
        {
            if (j != (int)row_ury[i].size() - 1)
            {
                row_ury[i][j]->E = row_ury[i][j + 1];
            }
        }
    }
    for (map<CornerNode*, pair<int, int> >::iterator iter = fixed_y.begin(); iter != fixed_y.end(); iter++)
    {
        for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
        {
            if (PreplacedInCornerStitching[i]->NodeType == Blank)
            {
                if (PreplacedInCornerStitching[i]->rectangle.lly >= iter->second.first && PreplacedInCornerStitching[i]->rectangle.ury <= iter->second.second &&
                    PreplacedInCornerStitching[i]->rectangle.llx == iter->first->rectangle.urx)    // W
                {
                    PreplacedInCornerStitching[i]->W = iter->first;
                }
                else if (PreplacedInCornerStitching[i]->rectangle.lly >= iter->second.first && PreplacedInCornerStitching[i]->rectangle.ury <= iter->second.second &&
                    PreplacedInCornerStitching[i]->rectangle.urx == iter->first->rectangle.llx)   // E
                {
                    PreplacedInCornerStitching[i]->E = iter->first;
                }
            }
        }
    }
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* tmp = PreplacedInCornerStitching[i];
        tmp->CornerNode_id = i;
    }
    ofstream fout("gird_blank.m");
    fout << "figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n" << endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        fout << "block_x=[" << PreplacedInCornerStitching[i]->rectangle.llx << " " << PreplacedInCornerStitching[i]->rectangle.llx << " " << PreplacedInCornerStitching[i]->rectangle.urx << " " << PreplacedInCornerStitching[i]->rectangle.urx << " " << PreplacedInCornerStitching[i]->rectangle.llx << " ];" << endl;
        fout << "block_y=[" << PreplacedInCornerStitching[i]->rectangle.lly << " " << PreplacedInCornerStitching[i]->rectangle.ury << " " << PreplacedInCornerStitching[i]->rectangle.ury << " " << PreplacedInCornerStitching[i]->rectangle.lly << " " << PreplacedInCornerStitching[i]->rectangle.lly << " ];" << endl;
        fout << "text(" << (PreplacedInCornerStitching[i]->rectangle.llx + PreplacedInCornerStitching[i]->rectangle.urx) / 2.0 << ", " << (PreplacedInCornerStitching[i]->rectangle.lly + PreplacedInCornerStitching[i]->rectangle.ury) / 2.0 << ", '" << PreplacedInCornerStitching[i]->CornerNode_id << "');" << endl << endl;
        if (PreplacedInCornerStitching[i]->NodeType == Fixed)
        {
            fout << "fill(block_x, block_y, 'r', 'facealpha', 0.5)" << endl;
        }
        else if (PreplacedInCornerStitching[i]->NodeType == Blank)
        {
            fout << "fill(block_x, block_y, 'c', 'facealpha', 0.5)" << endl;
        }
        else if (PreplacedInCornerStitching[i]->NodeType == Movable)
        {
            fout << "fill(block_x, block_y, 'b', 'facealpha', 0.5)" << endl;
        }
    }
    // vertical
    ConerNode_Vert_link_const(PreplacedInCornerStitching);

    // debug
    /*for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* tmp = PreplacedInCornerStitching[i];
        //cout<<"Start: "<<tmp->rectangle.llx<<", "<<tmp->rectangle.lly<<"   "<<tmp->rectangle.urx<<", "<<tmp->rectangle.ury<<endl;
        cout<<"Start: "<<tmp->CornerNode_id<<endl;
        while(tmp->S != NULL)
        {
            tmp = tmp->S;
            //cout<<tmp->rectangle.llx<<", "<<tmp->rectangle.lly<<"   "<<tmp->rectangle.urx<<", "<<tmp->rectangle.ury<<endl;
            cout<<tmp->CornerNode_id<<endl;
        }
        //cout<<endl;
        getchar();
    }*/
    /*for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        if(PreplacedInCornerStitching[i]->E == NULL && PreplacedInCornerStitching[i]->W == NULL &&
           PreplacedInCornerStitching[i]->S == NULL && PreplacedInCornerStitching[i]->N == NULL)
        {
            cerr<<"Wrong!!!!!!!!!!"<<endl;
            cerr<<"NodeType: "<<PreplacedInCornerStitching[i]->NodeType<<endl;
            cerr<<PreplacedInCornerStitching[i]->rectangle.llx<<", "<<PreplacedInCornerStitching[i]->rectangle.lly<<"   "<<
            PreplacedInCornerStitching[i]->rectangle.urx<<", "<<PreplacedInCornerStitching[i]->rectangle.ury<<endl;
            exit(1);
        }
    }*/
    //exit(1);
}
//////

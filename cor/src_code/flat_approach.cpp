#include "flat_approach.h"


void Flat_Approach()
{
    FLAT_Legalization_INFO LG_INFO;
    vector<Flat_Partition_Region> Final_region;
    vector<Macro*> &Macro_set = LG_INFO.MacroClusterSet;
    vector<CornerNode*> &AllCornerNode = LG_INFO.AllCornerNode;
    Flat_Partition(Final_region);
    Update_Macro_Info(Final_region, Macro_set);
    LG_INFO.Macro_Ordering = LG_INFO.MacroClusterSet;


    UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode);

    sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_place_order);

    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];

        Flat_Legal(Final_region[macro_temp->region_id].rectangle, macro_temp, AllCornerNode);

        Boundary subregion = Final_region[macro_temp->region_id].rectangle;
        if(macro_temp->LegalFlag == false)
        {
            bool escape_legal = false;
            float expand_ratio = 0.05;
            while(!macro_temp->LegalFlag && !escape_legal)
            {
                if(!Expand_subregion(subregion, expand_ratio))
                {
                    break;
                }
//                Boundary_Cout(subregion);
                Flat_Legal(subregion, macro_temp, AllCornerNode);



            }
//            getchar();
        }
    }
    DeleteCornerNode(AllCornerNode);
    for(int iter = 0; iter < 30; iter++)
    {
        UpdateInfoInPreplacedInCornerStitching(PreplacedInCornerStitching, AllCornerNode);
        Flat_Simulated_Evolution(LG_INFO, iter);
        Refinement_Legalization(LG_INFO, Final_region);
        DeleteCornerNode(AllCornerNode);
        string filename = "./output/LG_graph/Flat_LG";
        filename += int2str(iter);
        PlotMacroClusterSet(Macro_set, filename + ".m");
    }

    PlotMacroClusterSet(Macro_set, "./output/LG_graph/Flat_legal.m");
    cout<<"[INFO] OUTPUT TCL & DEF File(Flat_Hope)"<<endl;
    Output_tcl(Macro_set, "./output/TCL/Flat_Hope.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);
}



void Flat_Partition(vector<Flat_Partition_Region> &Final_region)
{

    vector<vector<Bin> > Bin_Array;


    Final_region.reserve(20);

    double Bin_Length = 0;

    /// bin map for finding white space
    ConstructBin(Bin_Array, Bin_Length);
    for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if(corner_temp->NodeType == Fixed)
        {
            AddInfoToBinArray(corner_temp->rectangle, false, Bin_Array, Bin_Length);
        }
    }
    PlotDensityMap("DensityMap.x", Bin_Array);


    queue<Flat_Partition_Region> region;

    Flat_Partition_Region chip;
    chip.macro_info.reserve(macro_list.size());
    chip.rectangle = chip_boundary;
    chip.cap = BenchInfo.chip_area;
    chip.demand = 0;
    for(int i = 0; i < (int)macro_list.size(); i++)
    {

        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == STD_CELL || macro_temp->macro_type == MOVABLE_MACRO)
        {
            chip.macro_info.push_back(macro_temp);
            chip.demand += macro_temp->area;
            macro_temp->RefCoor.first = (macro_temp->gp.urx+macro_temp->gp.llx)/2;
            macro_temp->RefCoor.second = (macro_temp->gp.ury+macro_temp->gp.lly)/2;

        }
    }

    region.push(chip);

    vector<PLOT_BLOCK> plot_block;
    vector<PLOT_LINE> plot_line;
    for(int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if(corner_temp->NodeType == Fixed)
        {
            PLOT_BLOCK blk_tmp;
            blk_tmp.TEXT = false;
            blk_tmp.color = 'c';
            blk_tmp.plotrectangle = corner_temp->rectangle;
            plot_block.push_back(blk_tmp);
        }
    }

    cout << "   Write flat_Region.m" << endl;
    PlotMatlab(plot_block, plot_line, true, false, "./output/subregion_graph/flat_Region.m");
    ofstream foutt("./output/subregion_graph/flat_Region.m", ios::app);
    int color_count = 0;
    while(!region.empty())
    {
        bool Hori_cut = false;
        Flat_Partition_Region &region_temp = region.front();
        //region_temp.rectangle = Blank_BBOX(region_temp.rectangle, PreplacedInCornerStitching);
        region_temp.cap = (region_temp.rectangle.urx - region_temp.rectangle.llx)/(float)PARA * (region_temp.rectangle.ury - region_temp.rectangle.lly)/(float)PARA;
        int region_W = region_temp.rectangle.urx - region_temp.rectangle.llx;
        int region_H = region_temp.rectangle.ury - region_temp.rectangle.lly;
        if(region_H > region_W)
        {
            Hori_cut = true;
            sort(region_temp.macro_info.begin(), region_temp.macro_info.end(), cmp_y);

        }
        else
        {

            sort(region_temp.macro_info.begin(), region_temp.macro_info.end(), cmp_x);

        }

        float area_bound = region_temp.demand / 2;
        float cmp_area = 0;
        float best_diff_area = FLT_MAX;
        int best_id = -1;
        float best_cmp_area = 0;

        bool stop_condition = false;
        plot_region(foutt, region_temp.rectangle, color_count);
        color_count++;
        if(region_temp.cap < BenchInfo.placeable_area * PARAMETER._PARTITION_SUBREGION_AREA )
        {
            Boundary blank_box = region_temp.rectangle;
            foutt << "TEXT = text(" << (blank_box.llx + blank_box.urx) / 2.0 << ", " << (blank_box.lly + blank_box.ury) / 2.0 << ", '" <<Final_region.size()  << "');" << endl;
            Flat_Partition_Region region_final;
            region_final.rectangle = region_temp.rectangle;
            for(int i = 0; i < (int)region_temp.macro_info.size(); i++)
            {
                Macro* macro_temp = region_temp.macro_info[i];
                if(macro_temp->macro_type == MOVABLE_MACRO)
                {
                    region_final.macro_info.push_back(macro_temp);
                }
            }
            Final_region.push_back(region_final);
            region.pop();
            continue;
        }

        for(int i = 0; i < (int)region_temp.macro_info.size(); i++)
        {
            Macro* macro_temp = region_temp.macro_info[i];
            cmp_area += macro_temp->area;

            float diff_area = fabs(area_bound - cmp_area);
            if(diff_area < best_diff_area)
            {
                best_id = i;
                best_diff_area = diff_area;
                stop_condition = false;
                best_cmp_area = cmp_area;

            }
            else
            {
                if(stop_condition)
                {
                    break;
                }

                stop_condition = true;
            }

        }


        map<float, AreaTable> Area_Line;
        Area_Table(region_temp.rectangle, Hori_cut, Bin_Array, Bin_Length, Area_Line);

        ///for calculate preplaced area
        map<float, AreaTable>::iterator iter_end = Area_Line.end();
        iter_end--;


        float preplace_area = iter_end->second.PreplaceArea;
        float region_temp_utilization = region_temp.demand / (region_temp.cap - preplace_area);
        /// area table info
//        cout<<"AREA TABLE"<<endl;
//        for(map<float, AreaTable>::iterator iter = Area_Line.begin(); iter != Area_Line.end(); iter++)
//        {
//            cout<<iter->first<<"\tcut: "<<iter->second.cutline<<"\tblank : "<<iter->second.TotalBlank<<endl;
//        }

        map<float, AreaTable>::iterator iter = Area_Line.lower_bound(best_cmp_area / region_temp_utilization);
//        cout<<"best_cmp_area : "<<best_cmp_area<<endl;
//        cout<<"Actual area "<<best_cmp_area / region_temp_utilization<<endl;
//        cout<<"Blank "<<iter_end->second.TotalBlank<<endl;
//        cout<<"region_temp_utilization "<<region_temp_utilization<<endl;
        if(iter == Area_Line.end())
        {

            Boundary blank_box = region_temp.rectangle;
            foutt << "TEXT = text(" << (blank_box.llx + blank_box.urx) / 2.0 << ", " << (blank_box.lly + blank_box.ury) / 2.0 << ", '" <<Final_region.size()  << "');" << endl;
            Flat_Partition_Region region_final;
            region_final.rectangle = region_temp.rectangle;
            for(int i = 0; i < (int)region_temp.macro_info.size(); i++)
            {
                Macro* macro_temp = region_temp.macro_info[i];
                if(macro_temp->macro_type == MOVABLE_MACRO)
                {
                    region_final.macro_info.push_back(macro_temp);
                }
            }
            Final_region.push_back(region_final);
            region.pop();
            continue;
        }

        for(int i = 0; i < 2; i++)
        {
            Flat_Partition_Region region_tmp;
            region_tmp.rectangle = region_temp.rectangle;
            if(i == 0)
            {
                if(Hori_cut)
                    region_tmp.rectangle.ury = iter->second.cutline;
                else
                    region_tmp.rectangle.urx = iter->second.cutline;

                Boundary &for_area = region_tmp.rectangle;
                region_tmp.cap = (for_area.urx - for_area.llx) / (float)PARA * (for_area.ury - for_area.lly) / (float)PARA;
                region_tmp.demand = best_cmp_area;
                region_tmp.macro_info.assign(region_temp.macro_info.begin(), region_temp.macro_info.begin()+best_id);

                region_tmp.preplace_area = iter->second.PreplaceArea;

            }
            else
            {
                if(Hori_cut)
                    region_tmp.rectangle.lly = iter->second.cutline;
                else
                    region_tmp.rectangle.llx = iter->second.cutline;

                Boundary &for_area = region_tmp.rectangle;
                region_tmp.cap = (for_area.urx - for_area.llx) / (float)PARA * (for_area.ury - for_area.lly) / (float)PARA;
                region_tmp.demand = region_temp.demand - best_cmp_area;
                region_tmp.macro_info.assign(region_temp.macro_info.end()-best_id, region_temp.macro_info.end());


                region_tmp.preplace_area = iter_end->second.PreplaceArea-iter->second.PreplaceArea;
            }

            if(region_tmp.cap < 1)
            {
                cerr <<"[WARNING] region_tmp.cap < 1" <<endl;
                cerr << "   Key enter to continue...";
                getchar();
            }
            region.push(region_tmp);
        }
        region.pop();
    }
}

bool cmp_x(Macro* a, Macro* b)
{
    return a->RefCoor.first < b->RefCoor.first;
}

bool cmp_y(Macro* a, Macro* b)
{
    return a->RefCoor.second < b->RefCoor.second;
}

void Update_Macro_Info(vector<Flat_Partition_Region> &Final_region, vector<Macro*> &Macro_Set)
{
    ///update region_id in macro_list
    for(int i = 0; i < (int)Final_region.size(); i++)
    {
        vector<Macro*> &macro_info = Final_region[i].macro_info;

        for(int j = 0; j < (int)macro_info.size(); j++)
        {
            Macro* &macro_temp = macro_info[j];
            macro_temp->region_id = i;
        }
    }



    Macro_Set.resize(MovableMacro_ID.size());
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
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
        macro_temp->region_id = macro_assign->region_id;
        macro_temp->Macro_Center = make_pair((macro_assign->gp.llx + macro_assign->gp.urx)/2, (macro_assign->gp.lly + macro_assign->gp.ury)/2);
        macro_temp->macro_area_ratio = macro_temp->area / BenchInfo.movable_macro_area;
        Macro_Set[i] = macro_temp;




    }
    Update_NET_INFO(Macro_Set);
    PlotMacroClusterSet(Macro_Set, "flat.m");
}

void Flat_Legal(Boundary &subregion, Macro* macro_temp, vector<CornerNode*> &AllCornerNode)
{

    int starID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];;
    CornerNode* starNode = AllCornerNode[starID];

    queue<CornerNode*> CornerNodeSet;
    Directed_AreaEnumeration(subregion, starNode, CornerNodeSet);
    ///for debug
//  plot_Directed_AreaEnumeration(CornerNodeSet, subregion);


    PACKING_INFO pack_tmp;
    pack_tmp.Macro_temp = macro_temp;
    pack_tmp.region_boundary = subregion;
    pack_tmp.WhiteSpaceCenterX = BenchInfo.WhiteSpaceXCenter;
    pack_tmp.WhiteSpaceCenterY = BenchInfo.WhiteSpaceYCenter;
//    cout<<"BenchInfo.WhiteSpaceYCenter"<<BenchInfo.WhiteSpaceYCenter<<"\t"<<"BenchInfo.WhiteSpaceXCenter"<<BenchInfo.WhiteSpaceXCenter<<endl;getchar();
    pack_tmp.occupy_ratio = Determine_PackDirection(AllCornerNode, subregion);


    COST_INFO cost_info;

    while(!CornerNodeSet.empty())
    {
        CornerNode* corner_temp = CornerNodeSet.front();
        if(corner_temp->NodeType == Blank)
        {

            pack_tmp.ptr = corner_temp;

            PointPacking(AllCornerNode, pack_tmp, cost_info);


        }

        CornerNodeSet.pop();
    }

    Select_BestCoor(cost_info, macro_temp);
    if(macro_temp->LegalFlag == true)
    {
        vector<CornerNode*> combination;
        combination.reserve(10);
        pair<int, int> macro_coor = make_pair(macro_temp->lg.llx, macro_temp->lg.lly);

        combination.push_back(CornerNodePointSearch(macro_coor, starNode));
        Find_topBlank(combination, macro_temp, macro_temp->lg.ury, macro_temp->lg);

        cout<<"Legal ID ; "<<macro_temp->macro_id<<endl;
        Before_UpdateMacroInDatastruct(AllCornerNode, macro_temp, combination);
    }

}

bool cmp_place_order(Macro* a, Macro* b)
{
    return a->area > b->area;
}



void Flat_Simulated_Evolution(FLAT_Legalization_INFO &LG_INFO, int iter_count)
{

    vector<Macro*> &MacroClusterSet = LG_INFO.MacroClusterSet;


    /// update cost
    vector<float> center_dist;


    center_dist.resize(MacroClusterSet.size());

    pair<float, float> score_limits = make_pair(FLT_MIN, FLT_MAX);
    pair<float, float> max_min_center_dist = score_limits;



    /// calculate each term of the cost
    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];

        if(macro_temp->LegalFlag != true)
        {
            continue;
        }

        float &center_dist_temp = center_dist[i];
        int dist = (fabs(BenchInfo.WhiteSpaceXCenter - macro_temp->Macro_Center.first) + fabs(BenchInfo.WhiteSpaceYCenter - macro_temp->Macro_Center.second));
        dist *= macro_temp->macro_area_ratio;
        center_dist_temp = 1 / (float)max(dist, 1);
        max_min_center_dist.first = max(max_min_center_dist.first, center_dist_temp);
        max_min_center_dist.second = min(max_min_center_dist.second, center_dist_temp);



    }


    /// normalize each term of the cost
    float ceter_dist_range = max_min_center_dist.first - max_min_center_dist.second;

    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        if(macro_temp->LegalFlag != true)
        {
            macro_temp->score = macro_temp->area;
            macro_temp->ConsiderCal = false;
            continue;
        }

        macro_temp->score = 0;

        /// ceter dist
        float center_score = Scaling_Cost(ceter_dist_range, center_dist[i] - max_min_center_dist.second) * 0.4;



        macro_temp->score += center_score;
//
//        if(center_score != 0)
//        {
//            cout<<0.1 / (center_score * log(iter_count + 2))<<endl;
//            macro_temp->score += 0.1 / (center_score * log(iter_count + 2));
//        }
        score_limits.first = max(score_limits.first, macro_temp->score);
        score_limits.second = min(score_limits.second, macro_temp->score);


    }


    /// normalize score & determine ripped-up macros
    float range = score_limits.first - score_limits.second;
    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        if(macro_temp->LegalFlag != true)
        {
            macro_temp->ConsiderCal = false;
            continue;
        }

        macro_temp->score = Scaling_Cost(range, macro_temp->score - score_limits.second);


        /// normalize score to 0.1-0.9
        macro_temp->score = (macro_temp->score + 0.125) * 0.8;

        float r = (float)(rand() % 101);
        r /= (float)100;
        if(macro_temp->score > r)
        {

            macro_temp->ConsiderCal = false;
        }
        else
        {

            macro_temp->ConsiderCal = true;
        }

        macro_temp->score *= macro_temp->macro_area_ratio;
    }




    sort(LG_INFO.Macro_Ordering.begin(), LG_INFO.Macro_Ordering.end(), cmp_ordering);

//    getchar();
}


void Refinement_Legalization(FLAT_Legalization_INFO &LG_INFO, vector<Flat_Partition_Region> &Final_region)
{
    vector<CornerNode*> &AllCornerNode = LG_INFO.AllCornerNode;
    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        if(macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {

            int startID = FixedPreplacedInCornerStitching[rand() % FixedPreplacedInCornerStitching.size()];
            CornerNode* starNode = AllCornerNode[startID];
            vector<CornerNode*> combination;
            combination.reserve(10);
            pair<int, int> macro_coor = make_pair(macro_temp->lg.llx, macro_temp->lg.lly);

            combination.push_back(CornerNodePointSearch(macro_coor, starNode));
            Find_topBlank(combination, macro_temp, macro_temp->lg.ury, macro_temp->lg);
            Before_UpdateMacroInDatastruct(AllCornerNode, macro_temp, combination);
        }
    }

    for(int i = 0; i < (int)LG_INFO.Macro_Ordering.size(); i++)
    {
        Macro* macro_temp = LG_INFO.Macro_Ordering[i];
        if(macro_temp->ConsiderCal && macro_temp->LegalFlag)
        {

        }
        else
        {

            Flat_Legal(Final_region[macro_temp->region_id].rectangle, macro_temp, AllCornerNode);
//            cout<<"REFINE ID : "<<macro_temp->macro_id<<endl;
            Boundary subregion = Final_region[macro_temp->region_id].rectangle;
            if(macro_temp->LegalFlag == false)
            {
                bool escape_legal = false;
                float expand_ratio = 0.05;
                while(!macro_temp->LegalFlag && !escape_legal)
                {
                    if(!Expand_subregion(subregion, expand_ratio))
                    {
                        break;
                    }
//                Boundary_Cout(subregion);
                    Flat_Legal(subregion, macro_temp, AllCornerNode);



                }
//            getchar();
            }
        }
    }
}

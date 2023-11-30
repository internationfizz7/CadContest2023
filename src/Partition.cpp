#include "Partition.h"
#include "cell_placement.h"
#include "CongestionMap.h"


void Recurrsive_Partition(Partition_INFO& info_tmp, vector<Partition_Region>& P_region, int node_num, vector<Macro_Group>& MacroGroupBySC)
{

    P_region.reserve(20);
    ///Const Bin Map
    vector<Partition_Node>& P_Node = info_tmp.P_Node;
    vector<Partition_Net>& P_Net = info_tmp.P_Net;
    vector<vector<Bin> > Bin_Array;



    double Bin_Length = 0;

    /// bin map for finding white space
    ConstructBin(Bin_Array, Bin_Length);
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if (corner_temp->NodeType == Fixed)
        {
            AddInfoToBinArray(corner_temp->rectangle, false, Bin_Array, Bin_Length);
        }
    }

    PlotDensityMap("./output/Density_map/DensityMap.x", Bin_Array);

    /// partition region
    queue<Partition_Region> region;

    /// update chip for cut
    Partition_Region chip;
    chip.rectangle = chip_boundary;
    chip.Level = 0;
    float Total_fixed = 0;
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if (corner_temp->NodeType == Fixed)
        {
            Total_fixed += (corner_temp->rectangle.urx - corner_temp->rectangle.llx) / (float)PARA * (corner_temp->rectangle.ury - corner_temp->rectangle.lly) / (float)PARA;
        }
    }
    chip.cap = (chip_boundary.urx - chip_boundary.llx) / (float)PARA * (chip_boundary.ury - chip_boundary.lly) / (float)PARA;
    chip.demand = Total_fixed;
    chip.node_ID.resize(node_num); // node_num is MacroGroupBySC.size()

    for (int i = 0; i < node_num; i++)
    {
        chip.node_ID[i] = i;
    }
    region.push(chip);


    vector<PLOT_BLOCK> plot_block;
    vector<PLOT_LINE> plot_line;
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if (corner_temp->NodeType == Fixed)
        {
            PLOT_BLOCK blk_tmp;
            blk_tmp.TEXT = false;
            blk_tmp.color = 'c';
            blk_tmp.plotrectangle = corner_temp->rectangle;
            plot_block.push_back(blk_tmp);
        }
    }
    PlotMatlab(plot_block, plot_line, true, false, "./output/subregion_graph/Region.m");

    int color_count = 0;
    ofstream foutt("./output/subregion_graph/Region.m", ios::app);

    /// start partition
    while (!region.empty())
    {
        info_tmp.P_Region = region.front();
        //        cout<<"LEVEL : "<<info_tmp.P_Region.Level<<endl;
        Partition_Region& region_temp = region.front();
        Boundary& region_box = region_temp.rectangle;
        bool HoriCut = true;
        Boundary blank_box = Blank_BBOX(region_box, PreplacedInCornerStitching);

        /// determine cut direction
        if (blank_box.urx - blank_box.llx > blank_box.ury - blank_box.lly)
        {
            //            cout<<"VERT CUT"<<endl;
            HoriCut = false;
        }
        else
        {
            //            cout<<"HORI CUT"<<endl;
        }

        float total_blank_space = (region_temp.cap - region_temp.demand);
        float blank_cap = (blank_box.urx - blank_box.llx) / (float)PARA * (blank_box.ury - blank_box.lly) / (float)PARA;
        plot_region(foutt, blank_box, color_count);

        /// terminal condition
        if (total_blank_space < BenchInfo.placeable_area * PARAMETER._PARTITION_SUBREGION_AREA || region_temp.node_ID.size() < 2)
        {

            foutt << "TEXT = text(" << (blank_box.llx + blank_box.urx) / 2.0 << ", " << (blank_box.lly + blank_box.ury) / 2.0 << ", '" << color_count << "');" << endl;
            //            cout<<"\n\nPreplace utilization : "<<region_temp.demand / region_temp.cap<<endl;
            //            cout<<"&&&&\n";
            //            Boundary_Cout(region_temp.rectangle);
            //            cout<<"&&&&\n";
            //            getchar();
            region_temp.rectangle = blank_box;
            P_region.push_back(region_temp);
            region.pop();
            continue;
        }



        ///sort for cut
//        cout<<"sort for cut"<<endl;
        vector<pair<int, int> > Coor_ID;
        Coor_ID.resize(region_temp.node_ID.size());
        if (HoriCut)
        {
            for (int i = 0; i < (int)region_temp.node_ID.size(); i++)
            {
                int ID = region_temp.node_ID[i];
                int Coor_Y = P_Node[ID].Center.second;
                Coor_ID[i] = make_pair(Coor_Y, ID);
            }
        }
        else
        {
            for (int i = 0; i < (int)region_temp.node_ID.size(); i++)
            {
                int ID = region_temp.node_ID[i];
                int Coor_X = P_Node[ID].Center.first;
                Coor_ID[i] = make_pair(Coor_X, ID);
            }
        }


        sort(Coor_ID.begin(), Coor_ID.end(), Sort_Coor);
        /// BIG macro align boundary
        if (region_temp.Level > PARAMETER._PARTITION_ITER)
        {
            BIG_Macro_Align_Boundary(HoriCut, blank_box, Coor_ID, info_tmp);
        }
        ///Calculate cut line cost
        float Total_Demand = 0; ///lef\bot

        for (int i = 0; i < (int)P_Net.size(); i++)
        {
            Partition_Net& net_tmp = P_Net[i];
            net_tmp.group1_count = 0;
            net_tmp.group2_count = 0;
        }


        for (int i = 0; i < (int)Coor_ID.size(); i++)
        {
            int ID = Coor_ID[i].second;
            Partition_Node& node_tmp = P_Node[ID];
            //            cout<<ID<<" macro : "<<node_tmp.Macro_Area<<"\tstd : "<<node_tmp.STD_Area<<endl;
            Total_Demand += (node_tmp.Macro_Area + node_tmp.STD_Area);
            for (int j = 0; j < (int)node_tmp.net_ID.size(); j++)
            {
                int net_id = node_tmp.net_ID[j];
                P_Net[net_id].group2_count++;

            }
        }

        int net_cut = 0;

        float group1_area = 0;

        float BEST_COST = numeric_limits<float>::max();
        float BEST_CUT, BEST_ID, BEST_ACTUAL_GROUP1_AREA;

        /// Const Blank area table
//        cout<<"Const Blank area table"<<endl;
        map<float, AreaTable> Area_Line;
        Area_Table(blank_box, HoriCut, Bin_Array, Bin_Length, Area_Line);

        /// area table info
//        cout<<"AREA TABLE"<<endl;
//        for(map<float, AreaTable>::iterator iter = Area_Line.begin(); iter != Area_Line.end(); iter++)
//        {
//            cout<<iter->first<<"\tcut: "<<iter->second.cutline<<"\tblank : "<<iter->second.TotalBlank<<endl;
//        }

        float total_macro_area = 0;
        float total_std_area = 0;
        for (int i = 0; i < (int)Coor_ID.size(); i++)
        {
            int ID = Coor_ID[i].second;
            Partition_Node& node_tmp = P_Node[ID];
            total_macro_area += node_tmp.Macro_Area;
            total_std_area += node_tmp.STD_Area;
        }

        /// determine cutline
        vector<float> Area_Cost, Displacement_Cost, Net_Cut_Cost, actual_group1_area_set;
        Area_Cost.resize(Coor_ID.size() - 1);
        Displacement_Cost.resize(Area_Cost.size());
        Net_Cut_Cost.resize(Area_Cost.size());
        actual_group1_area_set.resize(Area_Cost.size());
        /// ADD 2021.04, find the cut line pass through the min macro group
        //vector<float> Macro_Cut_Cost;
        //Macro_Cut_Cost.resize(Area_Cost.size());
        //////

        pair<float, float>  Area_Cost_max_min = make_pair(numeric_limits<float>::min(), numeric_limits<float>::max());
        pair<float, float> Displacement_Cost_max_min = Area_Cost_max_min;
        pair<float, float> Net_Cut_Cost_max_min = Area_Cost_max_min;
        /// ADD 2021.04
        //pair<float, float> Macro_Cut_Cost_max_min = Area_Cost_max_min;
        //////


        float group1_macro_area = 0;
        for (int i = 0; i < (int)Coor_ID.size() - 1; i++)
        {
            int ID = Coor_ID[i].second;
            Partition_Node& node_tmp = P_Node[ID];
            if (node_tmp.Group_node == false)
                continue;
            /// net cut
            Cal_Net_Cut(net_cut, node_tmp, P_Net);

            group1_macro_area += node_tmp.Macro_Area;
            group1_area += (node_tmp.Macro_Area + node_tmp.STD_Area);
            float actual_group1_area = (total_blank_space - total_macro_area - total_std_area) * group1_area / (total_macro_area + total_std_area) + group1_area;
            float actual_group2_area = total_blank_space - actual_group1_area;
            //            cout<<"region : "<<region_temp.cap<<"\t"<<region_temp.demand<<endl;
            //            cout<<"total_blank_space : "<<total_blank_space<<endl;
            //            cout<<"blank_pre_demand : "<<blank_pre_demand<<endl;
            //            cout<<"blank_cap : "<<blank_cap<<endl;
            //            cout<<"utilization : "<<utilization<<endl;
            //            cout<<"group1_area : "<<group1_area<<endl;
            //            cout<<"actual_group1_area : "<<actual_group1_area<<endl;
                        /// find associated cut line location
            map<float, AreaTable>::iterator iter = Area_Line.upper_bound(actual_group1_area);

            if (iter == Area_Line.end())
            {
                cout << "actual_group1_area_orig: " << actual_group1_area << endl;
                float group1_area_orig_std = group1_area - group1_macro_area;
                int multiple = 9;
                while (multiple != -1)
                {
                    float para = multiple / 10;
                    group1_area = group1_area_orig_std * para + group1_macro_area;
                    actual_group1_area = (total_blank_space - total_macro_area - total_std_area) * group1_area / (total_macro_area + total_std_area) + group1_area;
                    actual_group2_area = total_blank_space - actual_group1_area;

                    //cout << "[INFO] multiple: " << multiple << endl;
                    //cout << "[INFO] actual_group1_area " << (float)multiple / 10 << ": " << actual_group1_area << endl;
                    iter = Area_Line.upper_bound(actual_group1_area);

                    if (iter != Area_Line.end())
                    {
                        break;
                    }
                    multiple--;
                }

                if (multiple == -1)
                {
                    cout << actual_group1_area << endl;
                    cout << "[ERROR] Violate Area Constraint in partition stage." << endl;
                    exit(1);
                }
            }

            /*if(iter == Area_Line.end())
            {
                cout<<actual_group1_area<<endl;
                cout<<"ERROR : Violate Area Constraint !!!"<<endl;
                exit(1);

            }*/

            int cutline = iter->second.cutline;

            actual_group1_area = iter->first;
            actual_group2_area = total_blank_space - actual_group1_area;



            Area_Cost[i] = fabs(actual_group1_area - actual_group2_area);
            Displacement_Cost[i] = Cal_Displacement_Cost(Coor_ID, info_tmp, HoriCut, cutline, i);
            Net_Cut_Cost[i] = net_cut;
            actual_group1_area_set[i] = actual_group1_area;
            /// ADD 2021.04
            //float macro_cut_cost = 0;
            /*for(int j = 0; j < (int)macro_list.size(); j++)
            {
                Macro *macro_temp = macro_list[j];
                float xk = (float)( macro_temp->gp.llx + macro_temp->gp.urx ) / 2;  // center of macro
                float yk = (float)( macro_temp->gp.lly + macro_temp->gp.ury ) / 2;  // center of macro
                if(macro_temp->macro_type == MOVABLE_MACRO)
                {
                    if(HoriCut)
                    {
                        if(yk - macro_temp->real_h / 2 <= Coor_ID[i].first && Coor_ID[i].first <= yk)
                        {
                            macro_cut_cost += Coor_ID[i].first - yk + macro_temp->real_h;
                        }
                        else if(yk <= Coor_ID[i].first && Coor_ID[i].first <= yk + macro_temp->real_h / 2)
                        {
                            macro_cut_cost += yk + macro_temp->real_h / 2 - Coor_ID[i].first;
                        }
                    }
                    else
                    {
                        if(xk <= Coor_ID[i].first && Coor_ID[i].first <= xk + macro_temp->real_w / 2)
                        {
                            macro_cut_cost += xk + macro_temp->real_w / 2 - Coor_ID[i].first;
                        }
                        else if(xk - macro_temp->real_w / 2 <= Coor_ID[i].first && Coor_ID[i].first <= xk)
                        {
                            macro_cut_cost += Coor_ID[i].first - xk + macro_temp->real_w / 2;
                        }
                    }
                }
            }*/
            /*for(int j = 0; j < (int)MacroGroupBySC.size(); j++)
            {
                int xk = MacroGroupBySC[j].GCenter_X;
                int yk = MacroGroupBySC[j].GCenter_Y;
                int w  = MacroGroupBySC[j].bound_w;
                int h  = MacroGroupBySC[j].bound_h;
                if(HoriCut)
                {
                    if(yk - h / 2 <= Coor_ID[i].first && Coor_ID[i].first <= yk)
                    {
                        macro_cut_cost += Coor_ID[i].first - yk + h;
                    }
                    else if(yk <= Coor_ID[i].first && Coor_ID[i].first <= yk + h / 2)
                    {
                        macro_cut_cost += yk + h / 2 - Coor_ID[i].first;
                    }
                }
                else
                {
                    if(xk <= Coor_ID[i].first && Coor_ID[i].first <= xk + w / 2)
                    {
                        macro_cut_cost += xk + w / 2 - Coor_ID[i].first;
                    }
                    else if(xk - w / 2 <= Coor_ID[i].first && Coor_ID[i].first <= xk)
                    {
                        macro_cut_cost += Coor_ID[i].first - xk + w / 2;
                    }
                }
            }
            Macro_Cut_Cost[i] = macro_cut_cost;
            //////*/

            Pair_max_min(Area_Cost[i], Area_Cost_max_min);
            Pair_max_min(Displacement_Cost[i], Displacement_Cost_max_min);
            Pair_max_min(Net_Cut_Cost[i], Net_Cut_Cost_max_min);
            /// ADD 2021.04
            //Pair_max_min(Macro_Cut_Cost[i], Macro_Cut_Cost_max_min);
            //////

        }

        float Area_Cost_range = Area_Cost_max_min.first - Area_Cost_max_min.second;
        float Displacement_Cost_range = Displacement_Cost_max_min.first - Displacement_Cost_max_min.second;
        float Net_Cut_Cost_range = Net_Cut_Cost_max_min.first - Net_Cut_Cost_max_min.second;
        /// ADD 2021.04
        //float Macro_Cut_Cost_range = Macro_Cut_Cost_max_min.first - Macro_Cut_Cost_max_min.second;
        //////

        for (int i = 0; i < (int)Area_Cost.size(); i++)
        {
            Area_Cost[i] = Scaling_Cost(Area_Cost_range, Area_Cost[i]);
            Displacement_Cost[i] = Scaling_Cost(Displacement_Cost_range, Displacement_Cost[i]);
            Net_Cut_Cost[i] = Scaling_Cost(Net_Cut_Cost_range, Net_Cut_Cost[i]);
            /// ADD 2021.04
            //Macro_Cut_Cost[i] = Scaling_Cost(Macro_Cut_Cost_range, Macro_Cut_Cost[i]);
            //////

            float cost = Area_Cost[i] * PARAMETER._PARTITION_AREA + Displacement_Cost[i] * PARAMETER._PARTITION_DISPLACEMENT + Net_Cut_Cost[i] * PARAMETER._PARTITION_NETCUT /*+ Macro_Cut_Cost[i] * PARAMETER._PARTITION_MACRO_CUT*/;
            if (cost < BEST_COST)
            {
                BEST_COST = cost;
                BEST_ID = i;

            }
        }


        map<float, AreaTable>::iterator Table_End = Area_Line.end();
        Table_End--;
        BEST_ACTUAL_GROUP1_AREA = actual_group1_area_set[BEST_ID];
        map<float, AreaTable>::iterator iter = Area_Line.lower_bound(BEST_ACTUAL_GROUP1_AREA);
        BEST_CUT = iter->second.cutline;
        if (Table_End->second.cutline == BEST_CUT)
        {

            P_region.push_back(region_temp);
            for (map<float, AreaTable>::iterator iter = Area_Line.begin(); iter != Area_Line.end(); iter++)
            {
                cout << iter->first << "\tcut: " << iter->second.cutline << "\tblank : " << iter->second.TotalBlank << endl;
            }
            cout << actual_group1_area_set[BEST_ID] << " actual_group1_area_set " << total_blank_space - actual_group1_area_set[BEST_ID] << endl;
            region.pop();
            continue; getchar();
        }
        //        exit(1);
        //        BEST_ACTUAL_GROUP1_AREA =
        //        cout<<"BEST_COST : "<<BEST_COST<<"\tBEST_ACTUAL_GROUP1_AREA : "<<BEST_ACTUAL_GROUP1_AREA<<endl;
        //        cout<<"total_blank_space : "<<total_blank_space<<endl;
        ////        getchar();
        //        cout<<"UPDATE REGION"<<endl;


                /// update subregion


        for (int i = 0; i < 2; i++)
        {
            Partition_Region new_region;
            new_region.rectangle = blank_box;
            new_region.Level = region_temp.Level + 1;
            Boundary& new_rectangle = new_region.rectangle;
            if (i == 0) ///lef\bot
            {
                if (HoriCut)
                {
                    new_rectangle.ury = BEST_CUT;
                }
                else
                {
                    new_rectangle.urx = BEST_CUT;
                }
                new_region.cap = (new_rectangle.urx - new_rectangle.llx) / (float)PARA * (new_rectangle.ury - new_rectangle.lly) / (float)PARA;




                //                new_region.demand = max((float)0, new_region.cap - BEST_ACTUAL_GROUP1_AREA);
                new_region.demand = iter->second.PreplaceArea;

                new_region.node_ID.resize(BEST_ID + 1);

                for (int j = 0; j < BEST_ID + 1; j++)
                {
                    new_region.node_ID[j] = Coor_ID[j].second;

                }

            }
            else ///rig\top
            {
                if (HoriCut)
                {
                    new_region.rectangle.lly = BEST_CUT;
                }
                else
                {
                    new_region.rectangle.llx = BEST_CUT;
                }
                new_region.cap = (new_rectangle.urx - new_rectangle.llx) / (float)PARA * (new_rectangle.ury - new_rectangle.lly) / (float)PARA;




                //                new_region.demand = max((float)0, new_region.cap - (total_blank_space - BEST_ACTUAL_GROUP1_AREA));
                new_region.demand = Table_End->second.PreplaceArea - iter->second.PreplaceArea;

                new_region.node_ID.resize(Coor_ID.size() - (BEST_ID + 1));

                for (int j = BEST_ID + 1; j < (int)Coor_ID.size(); j++)
                {
                    new_region.node_ID.at(j - BEST_ID - 1) = Coor_ID.at(j).second;

                }

            }



            region.push(new_region);

        }



        region.pop();
    }


}

bool Sort_Coor(pair<int, int> a, pair<int, int> b)
{
    if (a.first == b.first)
    {
        return a.second < b.second;
    }

    return a.first < b.first;
}

int Cal_Displacement_Cost(vector<pair<int, int> >& Coor_ID, Partition_INFO& info_tmp, bool Hori_Cut, int cutline, int now_i)
{
    int displacement = 0;
    if (Hori_Cut)
    {
        for (int i = now_i; i < (int)Coor_ID.size(); i++)
        {
            Partition_Node& node_tmp = info_tmp.P_Node[Coor_ID[i].second];
            if (node_tmp.Center.second > cutline)
            {
                break;
            }

            displacement += cutline - node_tmp.Center.second;

        }
        for (int i = now_i; i >= 0; i--)
        {
            Partition_Node& node_tmp = info_tmp.P_Node[Coor_ID[i].second];
            if (node_tmp.Center.second < cutline)
            {
                break;
            }

            displacement += node_tmp.Center.second - cutline;

        }
    }
    else
    {
        for (int i = now_i; i < (int)Coor_ID.size(); i++)
        {
            Partition_Node& node_tmp = info_tmp.P_Node[Coor_ID[i].second];
            if (node_tmp.Center.first > cutline)
            {
                break;
            }

            displacement += cutline - node_tmp.Center.first;

        }
        for (int i = now_i; i >= 0; i--)
        {
            Partition_Node& node_tmp = info_tmp.P_Node[Coor_ID[i].second];
            if (node_tmp.Center.first < cutline)
            {
                break;
            }

            displacement += node_tmp.Center.first - cutline;

        }
    }

    return displacement;
}

void Cal_Net_Cut(int& net_cut, Partition_Node& node_tmp, vector<Partition_Net>& P_Net)
{
    for (int j = 0; j < (int)node_tmp.net_ID.size(); j++)
    {
        int net_id = node_tmp.net_ID[j];
        Partition_Net& net_tmp = P_Net[net_id];
        if (net_tmp.group1_count == 0 && net_tmp.group2_count > 1)
        {
            net_cut += net_tmp.net_weight;
        }
        else if (net_tmp.group1_count != 0 && net_tmp.group2_count == 1)
        {
            net_cut -= net_tmp.net_weight;
        }
        net_tmp.group1_count++;
        net_tmp.group2_count--;
    }
}

float Cal_Aspect_Ratio(Boundary& region_box, bool HoriCut, int cutline)
{
    float aspect_ratio;
    if (HoriCut)
    {
        int box_W, box_H1, box_H2;
        float aspect_1, aspect_2;
        box_W = region_box.urx - region_box.llx;
        box_H1 = cutline - region_box.lly;
        box_H2 = region_box.ury - cutline;
        aspect_1 = max(box_W / box_H1, box_H1 / box_W);
        aspect_2 = max(box_W / box_H2, box_H2 / box_W);
        aspect_ratio = max(aspect_1, aspect_2);
    }
    else
    {
        int box_W1, box_W2, box_H;
        float aspect_1, aspect_2;
        box_W1 = cutline - region_box.llx;
        box_W2 = region_box.urx - cutline;
        box_H = region_box.ury - region_box.lly;
        aspect_1 = max(box_W1 / box_H, box_H / box_W1);
        aspect_2 = max(box_W2 / box_H, box_H / box_W2);
        aspect_ratio = max(aspect_1, aspect_2);
    }
    return aspect_ratio;

}

void BIG_Macro_Align_Boundary(bool HoriCut, Boundary& region_box, vector<pair<int, int> >& Coor_ID, Partition_INFO& info_tmp)
{
    vector<Partition_Node>& P_Node = info_tmp.P_Node;
    bool increase = false;
    if (HoriCut)
    {
        int top_move = fabs(region_box.ury - BenchInfo.WhiteSpaceYCenter);
        int bot_move = fabs(region_box.lly - BenchInfo.WhiteSpaceYCenter);
        if (bot_move > top_move)
        {
            increase = true;
        }
    }
    else
    {
        int rig_move = fabs(region_box.urx - BenchInfo.WhiteSpaceXCenter);
        int lef_move = fabs(region_box.llx - BenchInfo.WhiteSpaceXCenter);
        if (lef_move > rig_move)
        {
            increase = true;
        }
    }
    //    cout<<"Begin"<<endl;
    for (int i = 0; i < (int)Coor_ID.size(); i++)
    {
        int ID = Coor_ID[i].second;
        Partition_Node& node_tmp = P_Node[ID];
        int w_times = node_tmp.max_w / BenchInfo.min_std_h;
        int h_times = node_tmp.max_h / BenchInfo.min_std_h;
        int max_times = max(w_times, h_times);
        //        if(max_times > PARAMETER._ROWH)
        //        {
        //            cout<<"&&";
        //        }
        //        cout<<"ID : "<<ID<<endl;

    }
    if (increase)
    {
        for (int i = 0; i < (int)Coor_ID.size(); i++)
        {
            int ID = Coor_ID[i].second;
            Partition_Node& node_tmp = P_Node[ID];
            int w_times = node_tmp.max_w / BenchInfo.min_std_h;
            int h_times = node_tmp.max_h / BenchInfo.min_std_h;
            int max_times = max(w_times, h_times);
            if (max_times > PARAMETER._ROWH)
            {

                for (int j = i - 1; j >= 0; j--)
                {

                    int swap_ID = Coor_ID[j].second;
                    Partition_Node& swap_node_tmp = P_Node[swap_ID];
                    int swap_w_times = swap_node_tmp.max_w / BenchInfo.min_std_h;
                    int swap_h_times = swap_node_tmp.max_h / BenchInfo.min_std_h;
                    int swap_max_times = max(swap_w_times, swap_h_times);
                    if (max_times > swap_max_times)
                    {
                        swap(Coor_ID[j], Coor_ID[j + 1]);
                    }
                    else
                    {
                        break;
                    }
                }

            }
        }
    }
    else
    {
        for (int i = Coor_ID.size() - 1; i >= 0; i--)
        {
            int ID = Coor_ID[i].second;
            Partition_Node& node_tmp = P_Node[ID];
            int w_times = node_tmp.max_w / BenchInfo.min_std_h;
            int h_times = node_tmp.max_h / BenchInfo.min_std_h;
            int max_times = max(w_times, h_times);
            if (max_times > PARAMETER._ROWH)
            {

                for (int j = i + 1; j < (int)Coor_ID.size(); j++)
                {

                    int swap_ID = Coor_ID[j].second;
                    Partition_Node& swap_node_tmp = P_Node[swap_ID];
                    int swap_w_times = swap_node_tmp.max_w / BenchInfo.min_std_h;
                    int swap_h_times = swap_node_tmp.max_h / BenchInfo.min_std_h;
                    int swap_max_times = max(swap_w_times, swap_h_times);
                    if (max_times > swap_max_times)
                    {
                        swap(Coor_ID[j], Coor_ID[j - 1]);
                    }
                    else
                    {
                        break;
                    }
                }
            }

        }
    }
    //    cout<<"INCREASE : "<<increase<<endl;
    //    for(int i = 0; i < (int)Coor_ID.size(); i++)
    //    {
    //        int ID = Coor_ID[i].second;
    //        Partition_Node &node_tmp = P_Node[ID];
    //        int w_times = node_tmp.max_w / BenchInfo.min_std_h;
    //        int h_times = node_tmp.max_h / BenchInfo.min_std_h;
    //        int max_times = max(w_times, h_times);
    //        cout<<"max times : "<<max_times<<endl;
    //        if(max_times > PARAMETER._ROWH)
    //        {
    //            cout<<"&&";
    //        }
    //        cout<<"ID : "<<ID<<endl;
    //
    //    }
    //    getchar();
}

Boundary Blank_BBOX(Boundary& region_box, vector<CornerNode*>& AllCornerNode)
{
    Boundary blank_box;
    blank_box.llx = numeric_limits<int>::max();
    blank_box.lly = numeric_limits<int>::max();
    blank_box.urx = numeric_limits<int>::min();
    blank_box.ury = numeric_limits<int>::min();

    for (int i = 0; i < (int)AllCornerNode.size(); i++)
    {
        CornerNode* corner_tmp = AllCornerNode[i];
        if (corner_tmp == NULL)
            continue;

        if (corner_tmp->NodeType != Blank)
            continue;

        Boundary overlap_box = Overlap_Box(region_box, corner_tmp->rectangle);


        if (!(overlap_box.urx - overlap_box.llx > 0) || !(overlap_box.ury - overlap_box.lly > 0))
            continue;

        Cal_BBOX(overlap_box, blank_box);
    }
    return blank_box;
}

void plot_region(std::ofstream& foutt, Boundary& blank_box, int& color_count)
{
    string color;
    if (color_count % 2 == 0)
    {
        color = 'k';
    }
    else
    {
        color = 'r';
    }
    color_count++;
    Boundary& RegionBoundary = blank_box;
    int llx = RegionBoundary.llx;
    int lly = RegionBoundary.lly;
    int urx = RegionBoundary.llx;
    int ury = RegionBoundary.ury;
    foutt << "LINE = line([" << llx << " " << urx << "],[" << lly << " " << ury << "],'Color','" << color << "');" << endl;
    foutt << "set(LINE, 'linewidth', 5)" << endl;
    llx = RegionBoundary.urx;
    lly = RegionBoundary.lly;
    urx = RegionBoundary.urx;
    ury = RegionBoundary.ury;

    foutt << "LINE = line([" << llx << " " << urx << "],[" << lly << " " << ury << "],'Color','" << color << "');" << endl;
    foutt << "set(LINE, 'linewidth', 5)" << endl;

    llx = RegionBoundary.llx;
    lly = RegionBoundary.lly;
    urx = RegionBoundary.urx;
    ury = RegionBoundary.lly;

    foutt << "LINE = line([" << llx << " " << urx << "],[" << lly << " " << ury << "],'Color','" << color << "');" << endl;
    foutt << "set(LINE, 'linewidth', 5)" << endl;

    llx = RegionBoundary.llx;
    lly = RegionBoundary.ury;
    urx = RegionBoundary.urx;
    ury = RegionBoundary.ury;

    foutt << "LINE = line([" << llx << " " << urx << "],[" << lly << " " << ury << "],'Color','" << color << "');" << endl;
    foutt << "set(LINE, 'linewidth', 5)" << endl;
}








//----------------------YU plus 2022/03/07

void Recurrsive_Partition_yu(Partition_INFO& info_tmp, vector<Partition_Region>& P_region, int node_num, vector<Macro_Group>& MacroGroupBySC) {
    Legalization_INFO legal;
    CELLPLACEMENT cellpl;
    cellpl.CellPlacementFlow(&cellpl, legal, 0);
    //---------//
    STD_Group* group_temp;
    int llx, lly, urx, ury;
    cout << "   Write CellCluster_Placement.m ..." << endl;
    string file = "./output/Cell_graph/cellclusters.m";
    ofstream fout_cell(file.c_str(), fstream::out);
    fout_cell << "figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n" << endl;
    // chip boundary
    fout_cell << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout_cell << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout_cell << "fill(block_x, block_y, 'w');" << endl;

    // for all STD group
    for (unsigned int i = 0; i < STDGroupBy2.size(); i++) {
        group_temp = STDGroupBy2[i];
        llx = group_temp-> packingX - (group_temp -> Width ) /2;
        lly = group_temp-> packingY - (group_temp -> Height) /2;
        urx = group_temp-> packingX + (group_temp -> Width ) /2;
        ury = group_temp-> packingY + (group_temp -> Height) /2;
        fout_cell << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout_cell << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout_cell << "fill(block_x, block_y, 'g');" << endl;
        // set cell lg coordinate for congestion map
        for(unsigned int j = 0; j < group_temp->member_ID.size(); j++){
            Macro* macro_temp = macro_list[group_temp->member_ID[j]];
            macro_temp->lg.llx = group_temp->packingX;
            macro_temp->lg.lly = group_temp->packingY;
            macro_temp->lg.urx = llx + macro_temp->real_w/2;
            macro_temp->lg.ury = lly + macro_temp->real_h/2;
        }
    }
    fout_cell.close();
    if(Debug_PARA)
        OutputTCL_withcell("./output/Cell_graph/cell_test.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);
    //cout << "******STOP*****" << endl;
    //getchar();

    //---------//
    P_region.reserve(20);
    ///Const Bin Map
    vector<Partition_Node>& P_Node = info_tmp.P_Node;
    vector<Partition_Net>& P_Net = info_tmp.P_Net;
    vector< vector<Bin> > Bin_Array;
    double Bin_Length = 0;
    /// bin map for finding white space
    ConstructBin(Bin_Array, Bin_Length);
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++){
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if (corner_temp->NodeType == Fixed)
            AddInfoToBinArray(corner_temp->rectangle, false, Bin_Array, Bin_Length);
    }
    PlotDensityMap("./output/Density_map/DensityMap.x", Bin_Array);

    /// partition region
    queue<Partition_Region> region;
    /// update chip for cut
    Partition_Region chip;
    chip.rectangle = chip_boundary;
    chip.Level = 0;
    float Total_fixed = 0;
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if (corner_temp->NodeType == Fixed)
        {
            Total_fixed += (corner_temp->rectangle.urx - corner_temp->rectangle.llx) / (float)PARA * (corner_temp->rectangle.ury - corner_temp->rectangle.lly) / (float)PARA;
        }
    }
    chip.cap = (chip_boundary.urx - chip_boundary.llx) / (float)PARA * (chip_boundary.ury - chip_boundary.lly) / (float)PARA;
    chip.demand = Total_fixed;
    chip.node_ID.resize(node_num); // node_num is MacroGroupBySC.size()
    for (int i = 0; i < node_num; i++)
    {
        chip.node_ID[i] = i;
    }
    chip.cellcluster_ID.resize((int)STDGroupBy2.size());
    for (int i = 0; i < (int)STDGroupBy2.size(); i++)
    {
        chip.cellcluster_ID[i] = i;
    }
    region.push(chip);

    vector<PLOT_BLOCK> plot_block;
    vector<PLOT_LINE> plot_line;
    for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
    {
        CornerNode* corner_temp = PreplacedInCornerStitching[i];
        if (corner_temp->NodeType == Fixed)
        {
            PLOT_BLOCK blk_tmp;
            blk_tmp.TEXT = false;
            blk_tmp.color = 'c';
            blk_tmp.plotrectangle = corner_temp->rectangle;
            plot_block.push_back(blk_tmp);
        }
    }
    PlotMatlab(plot_block, plot_line, true, false, "./output/subregion_graph/Region.m");
    int color_count = 0;
    ofstream foutt("./output/subregion_graph/Region.m", ios::app);

    while (!region.empty())
    {
        info_tmp.P_Region = region.front();
        Partition_Region& region_temp = region.front();
        Boundary& region_box = region_temp.rectangle;
        bool HoriCut = true;
        Boundary blank_box = Blank_BBOX(region_box, PreplacedInCornerStitching);

        /// determine cut direction
        if (blank_box.urx - blank_box.llx > blank_box.ury - blank_box.lly)
        {
            HoriCut = false;
        }
        float total_blank_space = (region_temp.cap - region_temp.demand);
        float blank_cap = (blank_box.urx - blank_box.llx) / (float)PARA * (blank_box.ury - blank_box.lly) / (float)PARA;
        plot_region(foutt, blank_box, color_count);

        /// terminal condition
        if (total_blank_space < BenchInfo.placeable_area * PARAMETER._PARTITION_SUBREGION_AREA || region_temp.node_ID.size() < 2)
        {
            foutt << "TEXT = text(" << (blank_box.llx + blank_box.urx) / 2.0 << ", " << (blank_box.lly + blank_box.ury) / 2.0 << ", '" << color_count << "');" << endl;
            region_temp.rectangle = blank_box;
            P_region.push_back(region_temp);
            region.pop();
            continue;
        }

       CONGESTIONMAP congestion;
        congestion.H_Edge.clear();
        congestion.V_Edge.clear();
        congestion.ary_2pinnet.clear();
        float total_overflow_LG = congestion.CongestionMapFlow(&congestion, false, 0);
        float congestion_V = 0.0, congesiton_H = 0.0;
        congestion_V = Cal_V_congestion(&congestion, chip_boundary);
        congesiton_H = Cal_H_congestion(&congestion, chip_boundary);

        ///sort macro for cut
        vector<pair<int, int> > Coor_ID;
        float movable_macro_area = 0.0;
        Coor_ID.resize(region_temp.node_ID.size());
        if (HoriCut){
            for (int i = 0; i < (int)region_temp.node_ID.size(); i++){
                int ID = region_temp.node_ID[i];
                movable_macro_area += P_Node[ID].Macro_Area;
                int Coor_Y = P_Node[ID].Center.second;
                Coor_ID[i] = make_pair(Coor_Y, ID);
            }
        }
        else{
            for (int i = 0; i < (int)region_temp.node_ID.size(); i++){
                int ID = region_temp.node_ID[i];
                movable_macro_area += P_Node[ID].Macro_Area;
                int Coor_X = P_Node[ID].Center.first;
                Coor_ID[i] = make_pair(Coor_X, ID);
            }
        }
        sort(Coor_ID.begin(), Coor_ID.end(), Sort_Coor);
        /// BIG macro align boundary
        if (region_temp.Level > PARAMETER._PARTITION_ITER)
        {
            BIG_Macro_Align_Boundary(HoriCut, blank_box, Coor_ID, info_tmp);
        }

        
        //calculate total cell area
        float movable_cell_area = 0.0;
        for(int i = 0; i < (int) region_temp.cellcluster_ID.size(); i++){
            movable_cell_area += STDGroupBy2[region_temp.cellcluster_ID[i]]->TotalArea;
        }
        //cal whitespace center
        float BlankSpaceCenterX = 0.0 , BlankSpaceCenterY = 0.0;
        float MovableCenterX = 0.0, MovableCenterY = 0.0;
        queue<float> BlankSpaceArea;
        queue<Boundary> BlankRectangle;
        float TotalBlankSpaceArea = 0;
        for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++){
            CornerNode* corner_temp = PreplacedInCornerStitching[i];
            if (corner_temp == NULL || corner_temp->NodeType != Blank)
                continue;
            Boundary blank_temp = Overlap_Box(corner_temp->rectangle, region_box);
            float blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
            if(blank_NodeArea > 0){
                TotalBlankSpaceArea += blank_NodeArea;
                BlankSpaceArea.push(blank_NodeArea);
                BlankRectangle.push(blank_temp);
            }
        }
        while (!BlankSpaceArea.empty()){
            float massWeight = BlankSpaceArea.front() / TotalBlankSpaceArea;
            Boundary& blank_temp = BlankRectangle.front();
            BlankSpaceCenterX += massWeight * (blank_temp.urx + blank_temp.llx) / 2;
            BlankSpaceCenterY += massWeight * (blank_temp.ury + blank_temp.lly) / 2;
            BlankSpaceArea.pop();
            BlankRectangle.pop();
        }

        ///cal gravity center
        float tatal_area = 0.0;
        tatal_area = movable_macro_area + movable_cell_area;
        for(int i = 0; i < (int)region_temp.node_ID.size(); i++){
            int ID = region_temp.node_ID[i];
            float massWeight = P_Node[ID].Macro_Area / tatal_area;
            MovableCenterX += massWeight * P_Node[ID].Center.first;
            MovableCenterY += massWeight * P_Node[ID].Center.second;
        }
        for(int i = 0; i < (int)region_temp.cellcluster_ID.size(); i++){
            int ID = region_temp.cellcluster_ID[i];
            float massWeight = STDGroupBy2[ID]->TotalArea / tatal_area;
            MovableCenterX += massWeight * STDGroupBy2[ID]->packingX;
            MovableCenterY += massWeight * STDGroupBy2[ID]->packingY;
        }

        ///////////////////////////////////////////
        if(HoriCut == true){
            Boundary bottom;
            bottom.llx = blank_box.llx;
            bottom.lly = blank_box.lly;
            bottom.urx = blank_box.urx;
            bottom.ury = (int)BlankSpaceCenterY;
            Boundary top;
            top.llx = blank_box.llx;
            top.lly = (int)BlankSpaceCenterY;
            top.urx = blank_box.urx;
            top.ury = blank_box.ury;

            Boundary blank_temp, fixed_temp;
            float bot_blank_Area = 0.0, bot_fixed_Area = 0.0, top_blank_Area = 0.0, top_fixed_Area = 0;
            float blank_NodeArea, fixed_NodeArea;
            for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++){
                CornerNode* corner_temp = PreplacedInCornerStitching[i];
                /////bottom region blank and fix area
                if (corner_temp != NULL && corner_temp->NodeType == Blank){
                    blank_temp = Overlap_Box(bottom, corner_temp->rectangle);
                    blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
                    if(blank_NodeArea > 0)
                        bot_blank_Area += blank_NodeArea;
                }
                if (corner_temp != NULL && corner_temp->NodeType == Fixed){
                    fixed_temp = Overlap_Box(bottom, corner_temp->rectangle);
                    fixed_NodeArea = ((fixed_temp.urx - fixed_temp.llx) / (double)PARA) * ((fixed_temp.ury - fixed_temp.lly) / (double)PARA);
                    if(fixed_NodeArea > 0)
                        bot_fixed_Area += fixed_NodeArea;
                }
                /////top region blank and fix area
                if (corner_temp != NULL && corner_temp->NodeType == Blank){
                    blank_temp = Overlap_Box(top, corner_temp->rectangle);
                    blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
                    if(blank_NodeArea > 0)
                        top_blank_Area += blank_NodeArea;
                }
                if (corner_temp != NULL && corner_temp->NodeType == Fixed){
                    fixed_temp = Overlap_Box(top, corner_temp->rectangle);
                    fixed_NodeArea = ((fixed_temp.urx - fixed_temp.llx) / (double)PARA) * ((fixed_temp.ury - fixed_temp.lly) / (double)PARA);
                    if(fixed_NodeArea > 0)
                        top_fixed_Area += fixed_NodeArea;
                }
            }

            Partition_Region new_region_bottom;
            new_region_bottom.rectangle = bottom;
            new_region_bottom.Level = region_temp.Level + 1;
            new_region_bottom.cap = (bottom.urx - bottom.llx) / (float)PARA * (bottom.ury - bottom.lly) / (float)PARA;
            new_region_bottom.demand = bot_fixed_Area;
            new_region_bottom.node_ID.clear();
            Partition_Region new_region_top;
            new_region_top.rectangle = top;
            new_region_top.Level = region_temp.Level + 1;
            new_region_top.cap = (top.urx - top.llx) / (float)PARA * (top.ury - top.lly) / (float)PARA;
            new_region_top.demand = top_fixed_Area;
            new_region_top.node_ID.clear();

            for(int i = 0; i < (int)Coor_ID.size(); i++){
                if(Coor_ID[i].first <= MovableCenterY && bot_blank_Area > P_Node[Coor_ID[i].second].Macro_Area){
                    new_region_bottom.node_ID.push_back(Coor_ID[i].second);
                    bot_blank_Area -= P_Node[Coor_ID[i].second].Macro_Area;
                    continue;
                }else if(Coor_ID[i].first > MovableCenterY && top_blank_Area > P_Node[Coor_ID[i].second].Macro_Area){
                    new_region_top.node_ID.push_back(Coor_ID[i].second);
                    top_blank_Area -= P_Node[Coor_ID[i].second].Macro_Area;
                    continue;
                }else{
                    cout << "Macro ERRORY" << endl;
                    cout << Coor_ID[i].first << " "<<P_Node[Coor_ID[i].second].Macro_Area << endl;
                }
            }
            ////////////make sure at least one node in one region/////////////
            if(new_region_top.node_ID.size() == 0 && top_blank_Area > P_Node[new_region_bottom.node_ID.back()].Macro_Area){
                new_region_top.node_ID.push_back(new_region_bottom.node_ID.back());
                top_blank_Area -= P_Node[new_region_bottom.node_ID.back()].Macro_Area;
                bot_blank_Area += P_Node[new_region_bottom.node_ID.back()].Macro_Area;
                new_region_bottom.node_ID.pop_back();
            }
            if(new_region_bottom.node_ID.size() == 0 && bot_blank_Area > P_Node[new_region_top.node_ID.front()].Macro_Area){
                new_region_bottom.node_ID.push_back(new_region_top.node_ID.front());
                bot_blank_Area -= P_Node[new_region_top.node_ID.front()].Macro_Area;
                top_blank_Area += P_Node[new_region_top.node_ID.front()].Macro_Area;
                new_region_top.node_ID.erase(new_region_top.node_ID.begin());
            }

            //control utilization
            float top_threshold = (new_region_top.cap - new_region_top.demand) * 0.1;
            float bot_threshold = (new_region_bottom.cap - new_region_bottom.demand) * 0.1;
            for(int i = 0; i < (int)region_temp.cellcluster_ID.size(); i++){
                int ID = region_temp.cellcluster_ID[i];
                if(STDGroupBy2[ID]->packingY <= MovableCenterY){
                    if(bot_blank_Area > bot_threshold){
                        new_region_bottom.cellcluster_ID.push_back(ID);
                        bot_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else if(bot_blank_Area < top_blank_Area && top_blank_Area > STDGroupBy2[ID]->TotalArea){
                        new_region_top.cellcluster_ID.push_back(ID);
                        top_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else{
                        new_region_bottom.cellcluster_ID.push_back(ID);
                        bot_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }
                }
                if(STDGroupBy2[ID]->packingY > MovableCenterY){
                    if(top_blank_Area > top_threshold){
                        new_region_top.cellcluster_ID.push_back(ID);
                        top_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else if(top_blank_Area < bot_blank_Area && bot_blank_Area > STDGroupBy2[ID]->TotalArea){
                        new_region_bottom.cellcluster_ID.push_back(ID);
                        bot_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else{
                        new_region_top.cellcluster_ID.push_back(ID);
                        top_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }
                }
                if(top_blank_Area < 0 || bot_blank_Area < 0){
                    cout << "[DEBUG]  blank area error!!" << endl;
                    cout << "[DEBUG]  top_blank_Area : " << top_blank_Area << endl;
                    cout << "[DEBUG]  bot_blank_Area : " << bot_blank_Area << endl;
                    getchar();
                }
            }

            float top_congestion_V = Cal_V_congestion(&congestion, top);
            float bot_congesiton_V = Cal_V_congestion(&congestion, bottom);
            float top_congestion_H = Cal_H_congestion(&congestion, top);
            float bot_congesiton_H = Cal_H_congestion(&congestion, bottom);
            float top_congestion = 1.2 * top_congestion_V + 0.8 * top_congestion_H;
            float bot_congestion = 1.2 * bot_congesiton_V + 0.8 * bot_congesiton_H;
            int top_movable_area = new_region_top.cap - new_region_top.demand - top_blank_Area;
            int bot_movable_area = new_region_bottom.cap - new_region_bottom.demand - bot_blank_Area;

            float base = (top_congestion > bot_congestion)? top_congestion : bot_congestion;
            float variation = fabs(top_congestion - bot_congestion);
            int cutline_adjust = 15*(exp(-pow(50,exp(-5)))-exp(-pow(50,exp(-5*(1-variation/base*2)))));

            map<float, AreaTable> Area_Line;
            Area_Table(blank_box, HoriCut, Bin_Array, Bin_Length, Area_Line);
            map<float, AreaTable>::iterator iter = Area_Line.upper_bound((float)(new_region_bottom.cap - new_region_bottom.demand));
            for(int i = 0; i < cutline_adjust; i++){
                if(top_congestion > bot_congestion){
                    iter--;
                }else{
                    iter++;
                }
            }
            float BEST_CUT = iter->second.cutline;
            float bot_preplace = iter->second.PreplaceArea;

            map<float, AreaTable>::iterator Table_End = Area_Line.end();
            Table_End--;
            top.lly = (int)BEST_CUT;
            new_region_top.rectangle = top;
            new_region_top.cap = (top.urx - top.llx) / (float)PARA * (top.ury - top.lly) / (float)PARA;
            new_region_top.demand = Table_End->second.PreplaceArea - bot_preplace;
            new_region_top.movable_area = top_movable_area;
            // cout << "[DEBUG] top   Boundary " << top.llx << " " << top.lly  << "   " << top.urx << " " << top.ury << endl;
            // cout << "[DEBUG] new_region_top.cap " << (int)new_region_top.cap << endl;
            // cout << "[DEBUG] new_region_top.demand " << (int)new_region_top.demand << endl;

            bottom.ury = (int)BEST_CUT;
            new_region_bottom.rectangle = bottom;
            new_region_bottom.cap = (bottom.urx - bottom.llx) / (float)PARA * (bottom.ury - bottom.lly) / (float)PARA;
            new_region_bottom.demand = bot_preplace;
            new_region_bottom.movable_area = bot_movable_area;
            // cout << "[DEBUG] bottom   Boundary " << bottom.llx << " " << bottom.lly  << "   " << bottom.urx << " " << bottom.ury << endl;
            // cout << "[DEBUG] new_region_bottom.cap " << (int)new_region_bottom.cap << endl;
            // cout << "[DEBUG] new_region_bottom.demand " << (int)new_region_bottom.demand << endl;
            //getchar();
            // leagal_cluster_in_region(new_region_bottom, Area_Line, HoriCut, P_Node, true);
            // leagal_cluster_in_region(new_region_top, Area_Line, HoriCut, P_Node, false);
            region.push(new_region_top);
            region.push(new_region_bottom);

        }else{
            Boundary right;
            right.llx = (int)BlankSpaceCenterX;
            right.lly = blank_box.lly;
            right.urx = blank_box.urx;
            right.ury = blank_box.ury;
            // cout << "[DEBUG]  right   Boundary " << right.llx << " " << right.lly  << "   " << right.urx << " " << right.ury << endl;
            Boundary left;
            left.llx = blank_box.llx;
            left.lly = blank_box.lly;
            left.urx = (int)BlankSpaceCenterX;
            left.ury = blank_box.ury;
            // cout << "[DEBUG]  left    Boundary " << left.llx << " " << left.lly  << "   " << left.urx << " " << left.ury << endl;

            float right_blank_Area = 0, right_fixed_Area = 0, left_blank_Area = 0, left_fixed_Area = 0;
            for (int i = 0; i < (int)PreplacedInCornerStitching.size(); i++)
            {
                CornerNode* corner_temp = PreplacedInCornerStitching[i];
                if (corner_temp != NULL && corner_temp->NodeType == Blank){
                    Boundary blank_temp = Overlap_Box(right, corner_temp->rectangle);
                    float blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
                    if(blank_NodeArea > 0){
                        right_blank_Area += blank_NodeArea;
                    }
                }
                if (corner_temp != NULL && corner_temp->NodeType == Fixed){
                    Boundary fixed_temp = Overlap_Box(right, corner_temp->rectangle);
                    float fixed_NodeArea = ((fixed_temp.urx - fixed_temp.llx) / (double)PARA) * ((fixed_temp.ury - fixed_temp.lly) / (double)PARA);
                    if(fixed_NodeArea > 0){
                        right_fixed_Area += fixed_NodeArea;
                    }
                }
                if (corner_temp != NULL && corner_temp->NodeType == Blank){
                    Boundary blank_temp = Overlap_Box(left, corner_temp->rectangle);
                    float blank_NodeArea = ((blank_temp.urx - blank_temp.llx) / (double)PARA) * ((blank_temp.ury - blank_temp.lly) / (double)PARA);
                    if(blank_NodeArea > 0){
                        left_blank_Area += blank_NodeArea;
                    }
                }
                if (corner_temp != NULL && corner_temp->NodeType == Fixed){
                    Boundary fixed_temp = Overlap_Box(left, corner_temp->rectangle);
                    float fixed_NodeArea = ((fixed_temp.urx - fixed_temp.llx) / (double)PARA) * ((fixed_temp.ury - fixed_temp.lly) / (double)PARA);
                    if(fixed_NodeArea > 0){
                        left_fixed_Area += fixed_NodeArea;
                    }
                }
            }

            Partition_Region new_region_right;
            new_region_right.rectangle = right;
            new_region_right.Level = region_temp.Level + 1;
            new_region_right.cap = (right.urx - right.llx) / (float)PARA * (right.ury - right.lly) / (float)PARA;
            new_region_right.demand = right_fixed_Area;
            new_region_right.node_ID.clear();
            Partition_Region new_region_left;
            new_region_left.rectangle = left;
            new_region_left.Level = region_temp.Level + 1;
            new_region_left.cap = (left.urx - left.llx) / (float)PARA * (left.ury - left.lly) / (float)PARA;
            new_region_left.demand = left_fixed_Area;
            new_region_left.node_ID.clear();

            //cout << Coor_ID.size() << endl;
            for(int i = 0; i < (int)Coor_ID.size(); i++){
                if(Coor_ID[i].first <= MovableCenterX && left_blank_Area > P_Node[Coor_ID[i].second].Macro_Area){
                     cout << "Left COOR_ID : " << Coor_ID[i].second << "   " << Coor_ID[i].first << "    AREA : " << P_Node[Coor_ID[i].second].Macro_Area << endl;
                    new_region_left.node_ID.push_back(Coor_ID[i].second);
                    left_blank_Area -= P_Node[Coor_ID[i].second].Macro_Area;
                    continue;
                }else if(Coor_ID[i].first > MovableCenterX && right_blank_Area > P_Node[Coor_ID[i].second].Macro_Area){
                     cout << "Right COOR_ID : " << Coor_ID[i].second << "   " << Coor_ID[i].first << "AREA : " << P_Node[Coor_ID[i].second].Macro_Area << endl;
                    new_region_right.node_ID.push_back(Coor_ID[i].second);
                    right_blank_Area -= P_Node[Coor_ID[i].second].Macro_Area;
                    continue;
                }else{
                    cout << "Macro ERRORX " << endl;
                    cout << Coor_ID[i].first << " " << P_Node[Coor_ID[i].second].Macro_Area<<" " << left_blank_Area<<" "<< right_blank_Area << endl;
                }
            }
            ////////////make sure at least one node in one region/////////////
            if(new_region_right.node_ID.size() == 0 && right_blank_Area > P_Node[new_region_left.node_ID.back()].Macro_Area){
                new_region_right.node_ID.push_back(new_region_left.node_ID.back());
                right_blank_Area -= P_Node[new_region_left.node_ID.back()].Macro_Area;
                left_blank_Area += P_Node[new_region_left.node_ID.back()].Macro_Area;
                new_region_left.node_ID.pop_back();
            }
            if(new_region_left.node_ID.size() == 0 && left_blank_Area > P_Node[new_region_right.node_ID.front()].Macro_Area){
                new_region_left.node_ID.push_back(new_region_right.node_ID.front());
                left_blank_Area -= P_Node[new_region_right.node_ID.front()].Macro_Area;
                right_blank_Area += P_Node[new_region_right.node_ID.front()].Macro_Area;
                new_region_right.node_ID.erase(new_region_right.node_ID.begin());
            }

            float left_threshold = (new_region_left.cap - new_region_left.demand) * 0.1;
            float right_threshold = (new_region_right.cap - new_region_right.demand) * 0.1;

            for(int i = 0; i < (int)region_temp.cellcluster_ID.size(); i++){
                int ID = region_temp.cellcluster_ID[i];
                if(STDGroupBy2[ID]->packingX <= MovableCenterX){
                    if(left_blank_Area > left_threshold){
                        new_region_left.cellcluster_ID.push_back(ID);
                        left_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else if(left_blank_Area < right_blank_Area && right_blank_Area > STDGroupBy2[ID]->TotalArea){
                        new_region_right.cellcluster_ID.push_back(ID);
                        right_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else{
                        new_region_left.cellcluster_ID.push_back(ID);
                        left_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }
                }
                if(STDGroupBy2[ID]->packingX > MovableCenterX){
                    if(right_blank_Area > right_threshold){
                        new_region_right.cellcluster_ID.push_back(ID);
                        right_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else if(right_blank_Area < left_blank_Area && left_blank_Area > STDGroupBy2[ID]->TotalArea){
                        new_region_left.cellcluster_ID.push_back(ID);
                        left_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }else{
                        new_region_right.cellcluster_ID.push_back(ID);
                        right_blank_Area -= STDGroupBy2[ID]->TotalArea;
                        continue;
                    }
                }
                if(right_blank_Area < 0 || left_blank_Area < 0){
                    cout << "[DEBUG]  blank area error!!" << endl;
                    getchar();
                }
            }

            int right_movable_area = (int)(new_region_right.cap - new_region_right.demand - right_blank_Area);
            int left_movable_area = (int)(new_region_left.cap - new_region_left.demand - left_blank_Area);
            float left_congestion_V = Cal_V_congestion(&congestion, left);
            float right_congesiton_V = Cal_V_congestion(&congestion, right);
            float left_congestion_H = Cal_H_congestion(&congestion, left);
            float right_congesiton_H = Cal_H_congestion(&congestion, right);

            float left_congestion = 0.8 * left_congestion_V + 1.2 * left_congestion_H;
            float right_congestion = 0.8 * right_congesiton_V + 1.2 * right_congesiton_H;


            float base = (left_congestion > right_congestion)? left_congestion : right_congestion;
            float variation = fabs(left_congestion - right_congestion);
            int cutline_adjust = 15*(exp(-pow(50,exp(-5)))-exp(-pow(50,exp(-5*(1-variation/base*2)))));


            map<float, AreaTable> Area_Line;
            Area_Table(blank_box, HoriCut, Bin_Array, Bin_Length, Area_Line);
            map<float, AreaTable>::iterator iter = Area_Line.upper_bound((float)(new_region_left.cap - new_region_left.demand));
            for(int i = 0; i < cutline_adjust; i++){
                if(left_congestion > right_congestion){
                    iter++;
                }else{
                    iter--;
                }
            }
            float BEST_CUT = iter->second.cutline;
            float left_preplace = iter->second.PreplaceArea;


            map<float, AreaTable>::iterator Table_End = Area_Line.end();
            Table_End--;
            // cout << "[DEBUG] BEST CUTLINE : " << (int)BEST_CUT << endl;
            right.llx = (int)BEST_CUT;
            new_region_right.rectangle = right;
            new_region_right.cap = (right.urx - right.llx) / (float)PARA * (right.ury - right.lly) / (float)PARA;
            new_region_right.demand = Table_End->second.PreplaceArea - left_preplace;
            new_region_right.movable_area = right_movable_area;
            /*cout << "[DEBUG] right   Boundary " << right.llx << " " << right.lly  << "   " << right.urx << " " << right.ury << endl;
            cout << "[DEBUG] new_region_right.cap " << (int)new_region_right.cap << endl;
            cout << "[DEBUG] new_region_right.demand " << (int)new_region_right.demand << endl;*/

            left.urx = (int)BEST_CUT;
            new_region_left.rectangle = left;
            new_region_left.cap = (left.urx - left.llx) / (float)PARA * (left.ury - left.lly) / (float)PARA;
            new_region_left.demand = left_preplace;
            new_region_left.movable_area = left_movable_area;
            /*cout << "[DEBUG] left   Boundary " << left.llx << " " << left.lly  << "   " << left.urx << " " << left.ury << endl;
            cout << "[DEBUG] new_region_left.cap " << (int)new_region_left.cap << endl;
            cout << "[DEBUG] new_region_left.demand " << (int)new_region_left.demand << endl;*/
            // leagal_cluster_in_region(new_region_right, Area_Line, HoriCut, P_Node, false);
            // leagal_cluster_in_region(new_region_left, Area_Line, HoriCut, P_Node, true);
            region.push(new_region_right);
            region.push(new_region_left);

        }
        region.pop();
    }
}

void leagal_cluster_in_region(Partition_Region &region, map<float, AreaTable> Area_Line, bool Hori_Cut, vector<Partition_Node>& P_Node, bool firstside){
    float macro_area = 0.0;
    for(unsigned int i = 0; i < region.node_ID.size(); i++){
        macro_area += P_Node[region.node_ID[i]].Macro_Area;
    }
    float utilization =  region.movable_area/(region.cap - region.demand);
    /*cout << "[DEBUG] region.cap : " <<  region.cap << endl;
    cout << "[DEBUG] region.demand : " <<  region.demand << endl;
    cout << "[DEBUG] blank_area : " << (int)(region.cap - region.demand) << endl;
    cout << "[DEBUG] region.movable_area : " << (int)region.movable_area<< endl;
    cout << "[DEBUG] macro_area : " << (int)macro_area << endl;
    cout << "[DEBUG] utilization : " << utilization << endl;*/
    if(Hori_Cut)
    {
        if(firstside){     ///first side is bottom region
            vector<STD_Group*> STDGroup_tmp;
            STDGroup_tmp.resize(region.cellcluster_ID.size());
            for(unsigned int i = 0; i < region.cellcluster_ID.size(); i++){
                STDGroup_tmp[i] = STDGroupBy2[region.cellcluster_ID[i]];
            }
            sort(STDGroup_tmp.begin(),STDGroup_tmp.end(), Sort_Cell_CoorY);

            map<float, AreaTable>::iterator iter = Area_Line.begin();
            while((iter->first < macro_area) && iter->second.cutline < region.rectangle.ury){
                iter++;
            }

            float allcellArea = 0.0;
            for(unsigned int i = 0; i < STDGroup_tmp.size(); i++){
                while(((iter->first - macro_area)*0.95 - STDGroup_tmp[i]->TotalArea) < 0){
                    iter++;
                    if(iter->second.cutline > region.rectangle.ury){
                        cout << "error" << endl;
                        getchar();
                    }
                }
                allcellArea += STDGroup_tmp[i]->TotalArea;
                STDGroup_tmp[i]->packingY = iter->second.cutline;
                if(STDGroup_tmp[i]->packingY > region.rectangle.ury)
                    cout << "shitQQQQQQQQQ" << endl;

                for(unsigned int j = 0; j < STDGroup_tmp[i]->member_ID.size(); j++){
                    Macro* macro_temp = macro_list[STDGroup_tmp[i]->member_ID[j]];
                    macro_temp->lg.llx = STDGroup_tmp[i]->packingX;
                    macro_temp->lg.lly = STDGroup_tmp[i]->packingY;
                    macro_temp->lg.urx = macro_temp->lg.llx + macro_temp->real_w/2;
                    macro_temp->lg.ury = macro_temp->lg.lly + macro_temp->real_h/2;
                }
            }
            // cout << "[DEBUG] cell number : " << STDGroup_tmp.size() << endl;
        }else{    ///first side is top region
            vector<STD_Group*> STDGroup_tmp;
            STDGroup_tmp.resize(region.cellcluster_ID.size());
            for(unsigned int i = 0; i < region.cellcluster_ID.size(); i++){
                STDGroup_tmp[i] = STDGroupBy2[region.cellcluster_ID[i]];
            }
            sort(STDGroup_tmp.begin(),STDGroup_tmp.end(), Sort_Cell_CoorY);

            map<float, AreaTable>::reverse_iterator iter  = Area_Line.rbegin()++;
            while(((Area_Line.rbegin()->first - iter->first) < macro_area) && iter->second.cutline > region.rectangle.lly){
                iter++;
            }
            // cout << "[DEBUG] Blankarea : " << (Area_Line.rbegin()->first - iter->first) << endl;
            float allcellArea = 0.0;
            for(unsigned int i = STDGroup_tmp.size()-1; i > 0 ; i--){
                while(((Area_Line.rbegin()->first - iter->first - macro_area) * 0.95 ) - allcellArea - STDGroup_tmp[i]->TotalArea < 0){
                    iter++;
                    if(iter->second.cutline < region.rectangle.lly){
                        cout << "error" << endl;
                        getchar();
                    }
                }
                allcellArea += STDGroup_tmp[i]->TotalArea;
                STDGroup_tmp[i]->packingY = iter->second.cutline;
                if(STDGroup_tmp[i]->packingY < region.rectangle.lly)
                    cout << "shitQQQQQQQQQ" << endl;

                for(unsigned int j = 0; j < STDGroup_tmp[i]->member_ID.size(); j++){
                    Macro* macro_temp = macro_list[STDGroup_tmp[i]->member_ID[j]];
                    macro_temp->lg.llx = STDGroup_tmp[i]->packingX;
                    macro_temp->lg.lly = STDGroup_tmp[i]->packingY;
                    macro_temp->lg.urx = macro_temp->lg.llx + macro_temp->real_w/2;
                    macro_temp->lg.ury = macro_temp->lg.lly + macro_temp->real_h/2;
                }
            }
            // cout << "[DEBUG] cell number : " << STDGroup_tmp.size() << endl;
        }
    }
    else
    {
        if(firstside){     ///first side is left region
            vector<STD_Group*> STDGroup_tmp;
            STDGroup_tmp.resize(region.cellcluster_ID.size());
            for(unsigned int i = 0; i < region.cellcluster_ID.size(); i++){
                STDGroup_tmp[i] = STDGroupBy2[region.cellcluster_ID[i]];
            }
            sort(STDGroup_tmp.begin(),STDGroup_tmp.end(), Sort_Cell_CoorX);
            map<float, AreaTable>::iterator iter = Area_Line.begin();
            while((iter->first < macro_area) && iter->second.cutline < region.rectangle.urx){
                iter++;
            }
            float allcellArea = 0.0;
            for(unsigned int i = 0; i < STDGroup_tmp.size(); i++){
                while(((iter->first - macro_area)*0.95 - STDGroup_tmp[i]->TotalArea) < 0){
                    iter++;
                    if(iter->second.cutline > region.rectangle.urx){
                        cout << "error" << endl;
                        getchar();
                    }
                }
                allcellArea += STDGroup_tmp[i]->TotalArea;
                STDGroup_tmp[i]->packingX = iter->second.cutline;
                if(STDGroup_tmp[i]->packingX > region.rectangle.urx)
                    cout << "shitQQQQQQQQQ" << endl;

                for(unsigned int j = 0; j < STDGroup_tmp[i]->member_ID.size(); j++){
                    Macro* macro_temp = macro_list[STDGroup_tmp[i]->member_ID[j]];
                    macro_temp->lg.llx = STDGroup_tmp[i]->packingX;
                    macro_temp->lg.lly = STDGroup_tmp[i]->packingY;
                    macro_temp->lg.urx = macro_temp->lg.llx + macro_temp->real_w/2;
                    macro_temp->lg.ury = macro_temp->lg.lly + macro_temp->real_h/2;
                }

            }
            // cout << "[DEBUG] cell number : " << STDGroup_tmp.size() << endl;
        }else{    ///first side is right region
            vector<STD_Group*> STDGroup_tmp;
            STDGroup_tmp.resize(region.cellcluster_ID.size());
            for(unsigned int i = 0; i < region.cellcluster_ID.size(); i++){
                STDGroup_tmp[i] = STDGroupBy2[region.cellcluster_ID[i]];
            }
            sort(STDGroup_tmp.begin(),STDGroup_tmp.end(), Sort_Cell_CoorX);
            map<float, AreaTable>::reverse_iterator iter = Area_Line.rbegin()++;
            while(((Area_Line.rbegin()->first - iter->first) < macro_area) && iter->second.cutline > region.rectangle.llx){
                iter++;
            }
            // cout << "[DEBUG] Blankarea : " << (Area_Line.rbegin()->first - iter->first) << endl;
            float allcellArea = 0.0;
            for(unsigned int i = STDGroup_tmp.size()-1; i > 0 ; i--){
                while(((Area_Line.rbegin()->first - iter->first - macro_area) * 0.95) - allcellArea - STDGroup_tmp[i]->TotalArea < 0){
                    iter++;
                    if(iter->second.cutline < region.rectangle.llx){
                        cout << "error" << endl;
                        getchar();
                    }
                }
                allcellArea += STDGroup_tmp[i]->TotalArea;
                STDGroup_tmp[i]->packingX = iter->second.cutline;
                if(STDGroup_tmp[i]->packingX < region.rectangle.llx)
                    cout << "shitQQQQQQQQQ" << endl;

                for(unsigned int j = 0; j < STDGroup_tmp[i]->member_ID.size(); j++){
                    Macro* macro_temp = macro_list[STDGroup_tmp[i]->member_ID[j]];
                    macro_temp->lg.llx = STDGroup_tmp[i]->packingX;
                    macro_temp->lg.lly = STDGroup_tmp[i]->packingY;
                    macro_temp->lg.urx = macro_temp->lg.llx + macro_temp->real_w/2;
                    macro_temp->lg.ury = macro_temp->lg.lly + macro_temp->real_h/2;
                }
            }
            // cout << "[DEBUG] cell number : " << STDGroup_tmp.size() << endl;
        }
    }
    return;
}

bool Sort_Cell_CoorX(STD_Group* a, STD_Group* b)
{
    if(a->packingX == b->packingX)
    {
        return a->packingY < b->packingY;
    }

    return a->packingX < b->packingX;
}

bool Sort_Cell_CoorY(STD_Group* a, STD_Group* b)
{
    if(a->packingY == b->packingY)
    {
        return a->packingX < b->packingX;
    }

    return a->packingY < b->packingY;
}

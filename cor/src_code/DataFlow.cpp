/// 108.05.20
#include "DataFlow.h"

void group_shifting(vector<Macro_Group> &MacroGroupBySC)
{
    if(Debug_PARA){
        /// Reassign the macro group
        Plot_subG(MacroGroupBySC, 2);       //plot original group
    }
    vector<net_df_wl> Net_WL;
    Net_WL.resize(net_list.size());

    vector<dataflow_group> DataFlow_Group;
    map<string, int> Table_heir_groupID;
    int Group_id = 0;
    for(unsigned int i = 0; i < MacroGroupBySC.size(); i++)
    {
        Macro_Group &mg_tmp = MacroGroupBySC[i];
        string hier_name = mg_tmp.HierName[0] + "/" + mg_tmp.HierName[1];
        string hier_name2 = mg_tmp.HierName[0] + "/" + mg_tmp.HierName[1] + "/" + mg_tmp.HierName[2];
        cout<<"G"<<i<<"  DH: "<<hier_name<<"   "<< hier_name2 <<endl;


        map<string, int>::iterator iter;
        iter = Table_heir_groupID.find(hier_name);
        if(iter != Table_heir_groupID.end())
        {
            int GroupID = iter->second;
            dataflow_group &dg_tmp = DataFlow_Group[GroupID];
            dg_tmp.SubGroup.push_back(i);
            int GCenter_SG_X = mg_tmp.GCenter_X;
            int GCenter_SG_Y = mg_tmp.GCenter_Y;
            dg_tmp.GCenter_SubGroup.push_back(make_pair(GCenter_SG_X, GCenter_SG_Y));
            dg_tmp.Relative_Position.push_back(make_pair(0, 0));

            float total_area = mg_tmp.MacroArea + mg_tmp.StdAreaInGroup;
            dg_tmp.Area.push_back(total_area);
            dg_tmp.TotalArea += total_area;

            float Total_GX = 0, Total_GY = 0;
            for(unsigned int j = 0; j < dg_tmp.GCenter_SubGroup.size(); j++)
            {
                Total_GX += dg_tmp.GCenter_SubGroup[j].first * dg_tmp.Area[j];
                Total_GY += dg_tmp.GCenter_SubGroup[j].second * dg_tmp.Area[j];
            }
            int G_X = (int)(Total_GX/(float)dg_tmp.TotalArea);
            int G_Y = (int)(Total_GY/(float)dg_tmp.TotalArea);
            dg_tmp.G_X = G_X;
            dg_tmp.G_Y = G_Y;
            dg_tmp.Original_G = make_pair(G_X, G_Y);

            for(unsigned int k = 0; k < dg_tmp.Relative_Position.size(); k++)
            {
                dg_tmp.Relative_Position[k].first = dg_tmp.GCenter_SubGroup[k].first - dg_tmp.G_X;
                dg_tmp.Relative_Position[k].second = dg_tmp.GCenter_SubGroup[k].second - dg_tmp.G_Y;
            }

            set<int>::iterator iter_net;
            for(iter_net = mg_tmp.Net_ID.begin(); iter_net != mg_tmp.Net_ID.end(); iter_net++)
            {
                dg_tmp.SubG_WL_net.insert( *iter_net );
                Net_WL[*iter_net].group.push_back(mg_tmp.ID);
            }
        }
        else
        {
            dataflow_group temp;
            temp.ID = Group_id;
            temp.heir_name = hier_name;
            DataFlow_Group.push_back(temp);

            dataflow_group &dg_tmp = DataFlow_Group[Group_id];
            dg_tmp.SubGroup.push_back(i);
            int GCenter_SG_X = mg_tmp.GCenter_X;
            int GCenter_SG_Y = mg_tmp.GCenter_Y;
            dg_tmp.GCenter_SubGroup.push_back(make_pair(GCenter_SG_X, GCenter_SG_Y));
            dg_tmp.G_X = GCenter_SG_X;
            dg_tmp.G_Y = GCenter_SG_Y;
            dg_tmp.Original_G = make_pair(GCenter_SG_X, GCenter_SG_Y);
            dg_tmp.Relative_Position.push_back(make_pair(0, 0));

            float total_area = mg_tmp.MacroArea + mg_tmp.StdAreaInGroup;
            dg_tmp.Area.push_back(total_area);
            dg_tmp.TotalArea += total_area;

            set<int>::iterator iter_net;
            for(iter_net = mg_tmp.Net_ID.begin(); iter_net != mg_tmp.Net_ID.end(); iter_net++)
            {
                dg_tmp.SubG_WL_net.insert( *iter_net );
            }

            Table_heir_groupID[hier_name] = Group_id;

            Group_id++;
        }

    }
    if(Debug_PARA){
        Plot_Group(DataFlow_Group);     //plot combination Group
    }
    /**************/
    int num1 = -1, num2 = -1;
    map<string, int>::iterator iter1;
    string name1 = "u_dptcon_dtop/u_dprx_dtop";
    string name2 = "u_dptcon_dtop/u_hx51_top";
    iter1 = Table_heir_groupID.find(name1);
    if(iter1 != Table_heir_groupID.end())
    {
        num1 = iter1->second;
    }
    else
        cerr << "ERROR num1" << endl;

    map<string, int>::iterator iter2;
    iter2 = Table_heir_groupID.find(name2);
    if(iter2 != Table_heir_groupID.end())
    {
        num2 = iter2->second;
    }
    else
        cerr << "ERROR num2" << endl;


    dataflow_group &dg_tmp1 = DataFlow_Group[num1];
    dataflow_group &dg_tmp2 = DataFlow_Group[num2];
    pair<int, int> G_XY1;
    pair<int, int> G_XY2;
    G_XY1 = make_pair(dg_tmp1.G_X, dg_tmp1.G_Y);
    G_XY2 = make_pair(dg_tmp2.G_X, dg_tmp2.G_Y);
    dg_tmp2.G_X= G_XY1.first;
    dg_tmp2.G_Y = G_XY1.second;
    dg_tmp1.G_X = G_XY2.first;
    dg_tmp1.G_Y = G_XY2.second;

    for(unsigned int i = 0; i < dg_tmp1.SubGroup.size(); i++)
    {
        int Group_num = dg_tmp1.SubGroup[i];
        Macro_Group &mg_tmp = MacroGroupBySC[Group_num];
        mg_tmp.GCenter_X = dg_tmp1.G_X + dg_tmp1.Relative_Position[i].first;
        mg_tmp.GCenter_Y = dg_tmp1.G_Y + dg_tmp1.Relative_Position[i].second;
        dg_tmp1.GCenter_SubGroup[i].first = mg_tmp.GCenter_X;
        dg_tmp1.GCenter_SubGroup[i].second = mg_tmp.GCenter_Y;
    }

    for(unsigned int i = 0; i < dg_tmp2.SubGroup.size(); i++)
    {
        int Group_num = dg_tmp2.SubGroup[i];
        Macro_Group &mg_tmp = MacroGroupBySC[Group_num];
        mg_tmp.GCenter_X = dg_tmp2.G_X + dg_tmp2.Relative_Position[i].first;
        mg_tmp.GCenter_Y = dg_tmp2.G_Y + dg_tmp2.Relative_Position[i].second;
        dg_tmp2.GCenter_SubGroup[i].first = mg_tmp.GCenter_X;
        dg_tmp2.GCenter_SubGroup[i].second = mg_tmp.GCenter_Y;
    }

    /*************/


    /// create edge
    vector<net_df_wl> Net_DF;
    int Net_num = 0;
    //vector<net_df_wl> Net_WL;
    //int NetWL_num = 0;
    //Net_WL.resize(net_list.size());

    for(unsigned int i = 0; i < DataFlow_Group.size(); i++)
    {
        /// dataflow Net
        dataflow_group &dg_tmp = DataFlow_Group[i];
        string heir_name = dg_tmp.heir_name;
        map<string, set<string> >::iterator iter;
        iter = DataFlow_outflow.find(heir_name);
        if(iter != DataFlow_outflow.end())
        {
            set<string>::iterator its;
            for(its = iter->second.begin(); its != iter->second.end(); its++)
            {
                string name = *its;
                map<string, int>::iterator iterT;
                iterT = Table_heir_groupID.find(name);
                if(iterT != Table_heir_groupID.end())
                {
                    int group1_ID = dg_tmp.ID;
                    int group2_ID = iterT->second;
                    dataflow_group &dg_tmp2 = DataFlow_Group[group2_ID];
                    dg_tmp.dataflow_net[Net_num] = group2_ID;
                    dg_tmp2.dataflow_net[Net_num] = group1_ID;

                    net_df_wl temp;
                    temp.ID = Net_num;
                    temp.group.push_back(group1_ID);
                    temp.group.push_back(group2_ID);
                    Net_DF.push_back(temp);

                    Net_num++;
                }
                else
                    cerr<<"Do not create net"<<endl;
            }
        }
        else
        {
            cerr<<"The DH_name(" << heir_name <<") is not in dataflow"<<endl;
        }


        /// WL Net


    }


//PreplacedMacro_ID


    /****************output Check******************/
    ofstream fout_DFG("DataFlowGroup.txt");
    for(unsigned int i = 0; i < DataFlow_Group.size(); i++)
    {
        dataflow_group &dg_tmp = DataFlow_Group[i];
        fout_DFG<<"Group "<< dg_tmp.ID<<"   DH: "<< dg_tmp.heir_name<<endl;
        fout_DFG<<"Subgroup: "<< endl;
        for(unsigned int j = 0; j < dg_tmp.SubGroup.size(); j++)
        {
            int MacroGroupID = dg_tmp.SubGroup[j];
            Macro_Group &tmp = MacroGroupBySC[MacroGroupID];
            string DH_name = tmp.HierName[0] + "/" + tmp.HierName[1];
            fout_DFG << DH_name << endl;
        }
        for(unsigned int k = 0; k < dg_tmp.GCenter_SubGroup.size(); k++)
        {
            int G_X = dg_tmp.GCenter_SubGroup[k].first;
            int G_Y = dg_tmp.GCenter_SubGroup[k].second;
            fout_DFG << "(" << G_X << ", " << G_Y << ")" << endl;
        }
    }

    map<string, int>::iterator iter;
    for(iter = Table_heir_groupID.begin(); iter != Table_heir_groupID.end(); iter++)
    {
        cout<<"DH: "<<iter->first<<"  ID: "<<iter->second<<endl;
    }
    //getchar();

    ofstream fout_NetDF("Net_DF.txt");
    for(unsigned int i = 0; i < Net_DF.size(); i++)
    {
        fout_NetDF<<"ID: "<<Net_DF[i].ID<<endl;
        fout_NetDF<<" MeM: ";
        for(unsigned int j = 0; j < Net_DF[i].group.size(); j++)
        {
            fout_NetDF<<Net_DF[i].group[j]<<"  ";
        }
        fout_NetDF<<endl;
    }
    /*******************************************/


    //getchar();
}


//map<string, set<string> >::iterator iter_a;
//iter_a = DataFlow_outflow.find(a);

void Calculate_quality()
{
    float displacement = 0;
    float dataflow_wl = 0;
    float wirelength = 0;
    float quality = 0;
    quality = displacement * PARAMETER._Shifting_displacement + wirelength * PARAMETER._Shifting_wirelength + dataflow_wl * PARAMETER._Shifting_dataflow_wl;
}

//void updata

/****************plus 108.08.12**************************/
void Exchange_with_Dataflow(bool HoriCut, float &BEST_CUT, Partition_Region &region_temp, vector<Partition_Node> &P_Node, vector<Partition_Net> &P_Net)
{
    /// Determine g1 g2
    set<int> Group1;
    set<int> Group2;
    Determine_two_part(HoriCut, BEST_CUT, Group1, Group2, region_temp, P_Node);
    /// Calculate gain
    Initialize_GroupCount(P_Net);
    Calculate_gain(Group1, Group2, region_temp, P_Node, P_Net);

    /// Change GCenter
    Partition_Node node_tmp_a;
    Partition_Node node_tmp_b;
    Exchange_G(node_tmp_a, node_tmp_b);

    /// re-determine cutline location
    Updata_cutline(BEST_CUT);

    //vector<Macro_Group> MG_temp


}

void Determine_two_part(bool HoriCut, float &BEST_CUT, set<int> &group1, set<int> &group2, Partition_Region &region_temp, vector<Partition_Node> &P_Node)
{
    if(HoriCut)
    {
        for(int i = 0; i < (int)region_temp.node_ID.size(); i++)
        {
            int ID = region_temp.node_ID[i];
            int Coor_Y = P_Node[ID].Center.second;
            if(Coor_Y <= BEST_CUT)
            {
                group1.insert(ID);
            }
            else
            {
                group2.insert(ID);
            }
            //Coor_ID[i] = make_pair(Coor_Y, ID);
        }
    }
    else
    {
        for(int i = 0; i < (int)region_temp.node_ID.size(); i++)
        {
            int ID = region_temp.node_ID[i];
            int Coor_X = P_Node[ID].Center.first;
            if(Coor_X <= BEST_CUT)
            {
                group1.insert(ID);
            }
            else
            {
                group2.insert(ID);
            }
            //Coor_ID[i] = make_pair(Coor_X, ID);
        }
    }
}

void Initialize_GroupCount(vector<Partition_Net> &P_Net)
{
    for(int i = 0; i < (int)P_Net.size(); i++)
    {
        Partition_Net &net_tmp = P_Net[i];
        net_tmp.group1_count = 0;
        net_tmp.group2_count = 0;
    }
}

void Calculate_gain(set<int> &Group1, set<int> &Group2, Partition_Region &region_temp, vector<Partition_Node> &P_Node, vector<Partition_Net> &P_Net)
{
    /// Calculate group count
    for(int i = 0; i < (int)region_temp.node_ID.size(); i++)
    {
        int NodeID = region_temp.node_ID[i];

        set<int>::iterator iterS = Group1.find(NodeID);
        bool group = true;
        if(iterS == Group1.end())
            group = false;

                    for(unsigned int j = 0; j < P_Node[NodeID].net_ID.size(); j++)
            {
                int NetID = P_Node[NodeID].net_ID[j];
                Partition_Net &net_tmp = P_Net[NetID];
                if(group)
                    net_tmp.group1_count++;
                else
                    net_tmp.group2_count++;
            }
    }

    /// Calculate gain
    for(int i = 0; i < (int)region_temp.node_ID.size(); i++)
    {
        int NodeID = region_temp.node_ID[i];

        set<int>::iterator iterS = Group1.find(NodeID);
        bool group = true;
        if(iterS == Group1.end())
            group = false;
/*
      float Gain = 0;
      int Group1_num = 0;
      int Group2_num = 0;

        for(int j = 0; j < P_Node[NodeID].net_ID.size(); j++)
        {
            int NetID = P_Node[NodeID].net_ID[j];
            Partition_Net &net_tmp = P_Net[NetID];
            if(group)
            {
                Group1_num = net_tmp.group1_count - 1;
                Group2_num = net_tmp.group2_count + 1;

                if(Group2_num == 1)
                {

                }
                if()
                {

                }



            }
            else
            {
                Group1_num = net_tmp.group1_count + 1;
                Group2_num = net_tmp.group2_count - 1;
            }
        }
 */
    }

}

void Exchange_G(Partition_Node &node_tmp_a, Partition_Node &node_tmp_b)
{
    int GX_a = node_tmp_a.Center.first;
    int GY_a = node_tmp_a.Center.second;
    int GX_b = node_tmp_b.Center.first;
    int GY_b = node_tmp_b.Center.second;

    node_tmp_a.Center.first = GX_b;
    node_tmp_a.Center.second = GY_b;
    node_tmp_b.Center.first = GX_a;
    node_tmp_b.Center.second = GY_a;
}

void Updata_cutline(float &BEST_CUT)
{


}






/*******************************************************/

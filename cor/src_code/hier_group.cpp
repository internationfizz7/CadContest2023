#include "hier_group.h"

/// 2021.04
bool same_type_macro_grouping = true;
//////

float MacroGroupAreaConstraint = 0.15;

void HierGroup(vector<Macro_Group> &MacroGroupBySC, HIER_LEVEL* &HIER_LEVEL_temp)
{
    HIER_LEVEL_temp = NULL;

    /// spilit hierarchy names of each macro and each standard cell
    HierarchicalNameAnalysis(HIER_LEVEL_temp);


    vector<vector<int> > MacroGroup;
    map<int, float> ID_StdArea;
    vector<vector<Macro_Group> > MacroGroupBySC_temp;

    /// assign area of related standard cells to macros
    CalHierStdArea(HIER_LEVEL_temp, ID_StdArea, 0);

    /// update related standard cell area of each macro
    Update_hier_area(ID_StdArea);

    /// determine each grouping set for macro grouping
    TraverseHierTreeConstGroup(HIER_LEVEL_temp, MacroGroup);

    /// 2021.04
    /*if(same_type_macro_grouping)
    {
        MacroGroup.clear();
        set<int> types;
        for(int i = 0; i < macro_list.size(); i ++)
            if(macro_list[i]->macro_type == MOVABLE_MACRO)
                types.insert(macro_list[i]->lef_type_ID);
        vector<int> same_type_macros;
        for(set<int>::iterator iter = types.begin(); iter != types.end(); iter ++)
        {
            for(int i = 0; i < macro_list.size(); i ++)
                if(macro_list[i]->macro_type == MOVABLE_MACRO && macro_list[i]->lef_type_ID == *iter)
                    same_type_macros.push_back(macro_list[i]->macro_id);
            MacroGroup.push_back(same_type_macros);
            same_type_macros.clear();
        }
    }*/
    //////

    // print macro group info.
    Cout_MacroGroup(MacroGroup);

    /// for each grouping set, group macros
    GroupMacroBySC(ID_StdArea, MacroGroup, MacroGroupBySC_temp);

    int Group_ID = 0;

    /// update final macro group
    for(int i = 0; i < (int)MacroGroupBySC_temp.size(); i++)
    {
        for(int j = 0; j < (int)MacroGroupBySC_temp[i].size(); j++)
        {
            Macro_Group &mg_tmp = MacroGroupBySC_temp[i][j];
            if(mg_tmp.Dont_Care == false)
            {
                mg_tmp.ID = Group_ID;
                MacroGroupBySC.push_back(mg_tmp);
            }
            Group_ID++;
        }
    }

    /// ADD 2021.04
    for(int i = 0; i < (int)MacroGroupBySC.size(); i++)
    {
        int most_left_x = numeric_limits<int>::max();
        int most_right_x = numeric_limits<int>::min();
        int most_bottom_y = numeric_limits<int>::max();
        int most_top_y = numeric_limits<int>::min();
        for(int j = 0; j < (int)MacroGroupBySC[i].member_ID.size(); j++)
        {
            int macro_id = MacroGroupBySC[i].member_ID[j];
            most_left_x = min(most_left_x, macro_list[macro_id]->gp.llx);
            most_right_x = max(most_right_x, macro_list[macro_id]->gp.urx);
            most_bottom_y = min(most_bottom_y, macro_list[macro_id]->gp.lly);
            most_top_y = max(most_top_y, macro_list[macro_id]->gp.ury);
        }
        MacroGroupBySC[i].bound_w = most_right_x - most_left_x;
        MacroGroupBySC[i].bound_h = most_top_y - most_bottom_y;
    }
    //////

        // print macro group info.
    Cout_MacroGroupSC(MacroGroupBySC);
}

void HierarchicalNameAnalysis(HIER_LEVEL* &HIER_LEVEL_temp)
{
    vector<HIER_NAME> HIER_NAME_temp;
    HIER_NAME_temp.resize(macro_list.size());

    /// split hierarchy name
    for(int i = 0; i < (int)macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        vector<string> &NameSet = HIER_NAME_temp[i].NameSet;
        NameSet.reserve(20);
        HIER_NAME_temp[i].macro_id = i;
        string name_temp = "";
        for(int j = 0; j < (int)macro_temp->macro_name.size(); j++)
        {
            if(macro_temp->macro_name[j] != '/')
            {
                name_temp += macro_temp->macro_name[j];
            }
            else
            {
                NameSet.push_back(name_temp);
                name_temp = "";
            }
        }
        NameSet.push_back(name_temp);
    }


    HIER_LEVEL_temp = new HIER_LEVEL;
    HIER_LEVEL_temp->Level_num = 0;

    /// construct hierarchy tree according to hierarchy name of "macros"
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
//        cout<<macro_id<<" : "<<macro_list[macro_id]->macro_name<<endl;
        RecurrsiveConstructHierarchy(HIER_LEVEL_temp, HIER_NAME_temp[macro_id]);

    }

//    for(map<string, HIER_LEVEL*>::iterator iter = HIER_LEVEL_temp->Name_HierGroup.begin(); iter != HIER_LEVEL_temp->Name_HierGroup.end(); iter++)
//    {
//        cout<<"Hier Name        : "<<iter->first<<endl;
//        cout<<"level num        : "<<iter->second->Level_num<<endl;
//    }
//    exit(1);
//    getchar();

    /// add "standard cell" into hierarchy tree
    for(int i = 0; i < (int)HIER_NAME_temp.size(); i++)
    {
        if(macro_list[i]->macro_type == STD_CELL)
            RecurrsiveConstructHierarchy(HIER_LEVEL_temp, HIER_NAME_temp[i]);

    }
//    exit(1);
//    system("rm -rf Hiername.o");
//    vector<int> STDID;
//    vector<int> MACROID;
//    vector<pair<string, int> > HIER_NAME_FOR_MATLAB;
//    OutputHierarchicalBBox(HIER_LEVEL_temp, STDID, MACROID, HIER_NAME_FOR_MATLAB);


//    getchar();

}

void RecurrsiveConstructHierarchy(HIER_LEVEL* &HIER_LEVEL_temp, HIER_NAME &name_temp)
{
    int Level_num = HIER_LEVEL_temp->Level_num;
    int SetSize = (int)name_temp.NameSet.size() - Level_num;
//    cout<<"Level_num : "<<Level_num<<endl;
//    cout<<name_temp.NameSet[Level_num]<<endl;
    Macro* macro_temp = macro_list[name_temp.macro_id];
    if(macro_temp->macro_type == STD_CELL)
        HIER_LEVEL_temp->StdAreaUnderLevel += macro_temp->area;
    else
    {
        HIER_LEVEL_temp->MacroAreaUnderLevel += macro_temp->area;
        HIER_LEVEL_temp->UnderLevelMacroSet.push_back(name_temp.macro_id);
    }

    Cal_BBOX(macro_temp->gp, HIER_LEVEL_temp->HierarchicalBBox);
    if(SetSize == 1) /// the last hierarchy name
    {

        if(macro_temp->macro_type == MOVABLE_MACRO)
        {
            HIER_LEVEL_temp->SameLevelMACROSet.push_back(name_temp.macro_id);
        }
        else if(macro_temp->macro_type == STD_CELL)
        {
            HIER_LEVEL_temp->SameLevelSTDSet.push_back(name_temp.macro_id);
            HIER_LEVEL_temp->StdAreaSameLevel += macro_temp->area;
        }
//        cout<<"Correct !!"<<endl;
    }
    else
    {
        map<string, HIER_LEVEL*>::iterator iter = HIER_LEVEL_temp->Name_HierGroup.find(name_temp.NameSet[Level_num]);

        if(iter != HIER_LEVEL_temp->Name_HierGroup.end())
        {
//            cout<<"In !"<<endl;
            RecurrsiveConstructHierarchy(iter->second, name_temp);
        }
        else if(macro_list[name_temp.macro_id]->macro_type == MOVABLE_MACRO)
        {
            /// only macros will change the topology of hierarchy tree
            HIER_LEVEL* temp = new HIER_LEVEL;
            temp->Hier_name = name_temp.NameSet[Level_num];
            temp->Level_num = Level_num + 1;
            HIER_LEVEL_temp->Name_HierGroup.insert(make_pair(name_temp.NameSet[Level_num], temp));
            RecurrsiveConstructHierarchy(temp, name_temp);
        }
        else
        {
            /// add std in hierarchy node
            HIER_LEVEL_temp->SameLevelSTDSet.push_back(name_temp.macro_id);
            HIER_LEVEL_temp->StdAreaSameLevel += macro_temp->area;
        }
    }
}


void OutputHierarchicalBBox(HIER_LEVEL* curr_HIER_LEVEL_temp, vector<int> &STDID, vector<int> &MACROID, vector<pair<string, int> > &HIER_NAME_FOR_MATLAB)
{
    map<string, HIER_LEVEL*> &Name_HierGroup = curr_HIER_LEVEL_temp->Name_HierGroup;
    if(!((int)HIER_NAME_FOR_MATLAB.size() > curr_HIER_LEVEL_temp->Level_num))
    {
        if(HIER_NAME_FOR_MATLAB.size() == 0)
        {
            HIER_NAME_FOR_MATLAB.push_back(make_pair("A", 0));
        }
        else
        {
            string a;
            a.push_back((int)HIER_NAME_FOR_MATLAB.back().first[0]+1);
            HIER_NAME_FOR_MATLAB.push_back(make_pair(a, 0));
        }
    }
    else
    {
        HIER_NAME_FOR_MATLAB[curr_HIER_LEVEL_temp->Level_num].second++;
    }
    cout<<HIER_NAME_FOR_MATLAB[curr_HIER_LEVEL_temp->Level_num].first<<HIER_NAME_FOR_MATLAB[curr_HIER_LEVEL_temp->Level_num].second<<endl;
    for(map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
    {
        vector<int> STDID_temp;
        vector<int> MACROID_temp;

        OutputHierarchicalBBox(iter->second, STDID_temp, MACROID_temp, HIER_NAME_FOR_MATLAB);
        for(int i = 0; i < (int)STDID_temp.size(); i++)
        {
            STDID.push_back(STDID_temp[i]);
        }
        for(int i = 0; i < (int)MACROID_temp.size(); i++)
        {
            MACROID.push_back(MACROID_temp[i]);
        }
    }
    vector<PLOT_BLOCK> plot_block;
    vector<PLOT_LINE> plot_line;
    PLOT_BLOCK block_tmp;
    block_tmp.color = "w";
    block_tmp.TEXT = false;
    block_tmp.plotrectangle = chip_boundary;
    plot_block.push_back(block_tmp);
//    cout<<curr_HIER_LEVEL_temp->SameLevelMACROSet.size()<<endl;
//    cout<<curr_HIER_LEVEL_temp->SameLevelSTDSet.size()<<endl;
    for(int j = 0; j < (int)MACROID.size(); j++)
    {
        int ID = MACROID[j];
        Macro* temp = macro_list[ID];
        PLOT_BLOCK block_tmp;
        block_tmp.color = "b";
        block_tmp.TEXT = false;
        block_tmp.plotrectangle = temp->gp;
        plot_block.push_back(block_tmp);

    }

    for(int j = 0; j < (int)STDID.size(); j++)
    {
        int ID = STDID[j];
        Macro* temp = macro_list[ID];
        PLOT_BLOCK block_tmp;
        block_tmp.color = "r";
        block_tmp.TEXT = false;
        block_tmp.plotrectangle = temp->gp;
        plot_block.push_back(block_tmp);
    }
    for(int j = 0; j < (int)curr_HIER_LEVEL_temp->SameLevelMACROSet.size(); j++)
    {
        int ID = curr_HIER_LEVEL_temp->SameLevelMACROSet[j];
        MACROID.push_back(ID);
        Macro* temp = macro_list[ID];
        PLOT_BLOCK block_tmp;
        block_tmp.color = "b";
        block_tmp.TEXT = false;
        block_tmp.plotrectangle = temp->gp;
        plot_block.push_back(block_tmp);

    }

    for(int j = 0; j < (int)curr_HIER_LEVEL_temp->SameLevelSTDSet.size(); j++)
    {
        int ID = curr_HIER_LEVEL_temp->SameLevelSTDSet[j];
        STDID.push_back(ID);
        Macro* temp = macro_list[ID];
        PLOT_BLOCK block_tmp;
        block_tmp.color = "r";
        block_tmp.TEXT = false;
        block_tmp.plotrectangle = temp->gp;
        plot_block.push_back(block_tmp);



    }

    Boundary &BBox = curr_HIER_LEVEL_temp->HierarchicalBBox;
    PLOT_LINE line_temp;
    line_temp.color = 'k';
    line_temp.plotline.llx = BBox.llx;
    line_temp.plotline.lly = BBox.lly;

    line_temp.plotline.urx = BBox.llx;
    line_temp.plotline.ury = BBox.ury;
    plot_line.push_back(line_temp);


    line_temp.plotline.urx = BBox.urx;
    line_temp.plotline.ury = BBox.lly;
    plot_line.push_back(line_temp);


    line_temp.plotline.urx = BBox.urx;
    line_temp.plotline.ury = BBox.ury;

    line_temp.plotline.llx = BBox.urx;
    line_temp.plotline.lly = BBox.lly;
    plot_line.push_back(line_temp);


    line_temp.plotline.llx = BBox.llx;
    line_temp.plotline.lly = BBox.ury;
    plot_line.push_back(line_temp);
//    cout<<plot_block.size()<<endl;
//    cout<<plot_line.size()<<endl;
    string matlab_name;
    for(int i = 0; i < curr_HIER_LEVEL_temp->Level_num + 1; i++)
    {
        matlab_name += HIER_NAME_FOR_MATLAB[i].first;
        matlab_name += int2str(HIER_NAME_FOR_MATLAB[i].second);
        if(curr_HIER_LEVEL_temp->Level_num != i)
        {
            matlab_name += "_";
        }
    }
    if(Debug_PARA){
        cout << "   Write ./output/Group/plot"<< matlab_name <<".m" << endl;
        PlotMatlab(plot_block, plot_line, false, true, "./output/Group/plot"+matlab_name+".m");
    }
}

/// from root to leaf to assign std area
void CalHierStdArea(HIER_LEVEL* HIER_LEVEL_temp, map<int, float> &ID_StdArea, float AvgArea)
{
    if(HIER_LEVEL_temp->UnderLevelMacroSet.size() == 0)
    {
        cout<<"UnderLevelMacroSet == 0"<<endl;
        exit(1);
    }
//    cout<<"HIER_LEVEL_temp->UnderLevelMacroSet.size : "<<HIER_LEVEL_temp->UnderLevelMacroSet.size()<<endl;
//    cout<<"HIER_LEVEL_temp->StdAreaSameLevel  : "<<HIER_LEVEL_temp->StdAreaSameLevel<<endl;
//    cout<<"HIER_LEVEL_temp->StdAreaUnderLevel : "<<HIER_LEVEL_temp->StdAreaUnderLevel<<endl;
    AvgArea += HIER_LEVEL_temp->StdAreaSameLevel / (float)(HIER_LEVEL_temp->UnderLevelMacroSet.size() + HIER_LEVEL_temp->SameLevelMACROSet.size());
    map<string, HIER_LEVEL*> &Name_HierGroup = HIER_LEVEL_temp->Name_HierGroup;
    if(Name_HierGroup.size() == 0)
    {
        AvgArea += (HIER_LEVEL_temp->StdAreaUnderLevel - HIER_LEVEL_temp->StdAreaSameLevel) / (float)(HIER_LEVEL_temp->SameLevelMACROSet.size());
        for(unsigned int i = 0; i < HIER_LEVEL_temp->SameLevelMACROSet.size(); i++)
        {
            int &ID = HIER_LEVEL_temp->SameLevelMACROSet[i];
            ID_StdArea.insert(make_pair(ID, AvgArea));
//            cout<<"ID : "<<ID<<endl;
//            cout<<"Name : "<<macro_list[ID]->macro_name<<endl;
        }
//        cout<<"AvgArea : "<<AvgArea<<endl<<endl;
//        getchar();
    }
    else
    {
        for(unsigned int i = 0; i < HIER_LEVEL_temp->SameLevelMACROSet.size(); i++)
        {
            int &ID = HIER_LEVEL_temp->SameLevelMACROSet[i];
            ID_StdArea.insert(make_pair(ID, AvgArea));
//            cout<<"same:"<<endl;
//            cout<<"ID : "<<ID<<endl;
//            cout<<"Name : "<<macro_list[ID]->macro_name<<endl;
        }
        map<string, HIER_LEVEL*> &Name_HierGroup = HIER_LEVEL_temp->Name_HierGroup;
        for(map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
        {
            CalHierStdArea(iter->second, ID_StdArea, AvgArea);
        }
    }
    return;
}


void TraverseHierTreeConstGroup(HIER_LEVEL* HIER_LEVEL_temp, vector<vector<int> > &MacroGroup)
{
//    float StdAreaConstrint = GroupAreaConstraint / (float)(MovableMacro_ID.size()) * GroupAreaConstraint;
//    float MacroAreaConstrint = BenchInfo.movable_macro_area * MacroGroupAreaConstraint;
    map<string, HIER_LEVEL*> &Name_HierGroup = HIER_LEVEL_temp->Name_HierGroup;
//    cout<<HIER_LEVEL_temp->Level_num<<"\t"<<HIER_LEVEL_temp->Hier_name<<endl;


    // the macros in the same node which is low hevel (<= _HIER_LEVEL) will be a macro group
    if(!(HIER_LEVEL_temp->Level_num > PARAMETER._GROUP_HIER_LEVEL) && HIER_LEVEL_temp->SameLevelMACROSet.size() != 0)
    {
//        cout<<"IN"<<endl;
        MacroGroup.push_back(HIER_LEVEL_temp->SameLevelMACROSet);
    }

    // the macro is not in the low level or the node is the leaf node
    if((HIER_LEVEL_temp->Level_num > PARAMETER._GROUP_HIER_LEVEL) || Name_HierGroup.size() == 0)
    {
        // the level is _HIER_LEVEL +1 or will be push back directly
        if(HIER_LEVEL_temp->Level_num == PARAMETER._GROUP_HIER_LEVEL + 1)
        {
            bool area_constraint = false;

            // cal the area_constraint
            for(map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
            {
                HIER_LEVEL* hier_tmp = iter->second;
                if(hier_tmp->StdAreaUnderLevel > BenchInfo.std_cell_area * PARAMETER._GROUP_AREA)
                {
                    area_constraint = true;
                    break;
                }
            }
            // if the area is over the constrain, Macro Group will pushback the children node to a macro group
            if(area_constraint)
            {
                for(map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
                {
                    TraverseHierTreeConstGroup(iter->second, MacroGroup);
                }

                return;
            }
            // if the area is enought
            else
            {

                if(HIER_LEVEL_temp->UnderLevelMacroSet.size() > 0 && MacroGroup.size() > 0)
                {
                    int macro_id = MacroGroup.back().back();
                    int next_macro_id = HIER_LEVEL_temp->UnderLevelMacroSet.back();

                    int lef_type_ID = macro_list[macro_id]->lef_type_ID;
                    int next_lef_type_ID = macro_list[next_macro_id]->lef_type_ID;

                    if(lef_type_ID == next_lef_type_ID) // what? the least macro in the MacroGropu and the least mocro below the tree node is same type ?
                    {
                        for(int i = 0; i < (int)HIER_LEVEL_temp->UnderLevelMacroSet.size(); i++)
                        {
                            MacroGroup.back().push_back(HIER_LEVEL_temp->UnderLevelMacroSet[i]); // all mocros below the tree node pushback to this group!
                        }
                        return;
                    }
                    // if the type are different the macro group will push back directly
                }


            }
        }
        MacroGroup.push_back(HIER_LEVEL_temp->UnderLevelMacroSet);

    }
    // the macro is on the low Level node and isn't the tree left need to track the tree to other macro
    else
    {

        for(map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
        {
            TraverseHierTreeConstGroup(iter->second, MacroGroup);
        }
    }
}

void Cout_MacroGroup(vector<vector<int> > &MacroGroup)
{
    cout<<"[INFO] Number of Movable macro: "<< MovableMacro_ID.size() <<endl;
    cout<<"[INFO] Number of Macro groups in hierarchy: "<< MacroGroup.size() <<endl;


    cout << "   Write macro_group.txt ..." << endl;
    ofstream fout("./output/Group/macro_group.txt", fstream::out);

    for(unsigned int i = 0 ; i < MacroGroup.size(); i++)
    {
        fout<<"  group "<< (i+1) <<" size : "<< MacroGroup[i].size()<<endl;
        for(unsigned int j = 0; j < MacroGroup[i].size(); j++)
        {
            int macro_id = MacroGroup[i][j];
            fout<< "macro id: "<< macro_id << " " << macro_list[macro_id]-> macro_name << endl;
        }
        fout<<"\n";
    }
    fout.close();

}

void Cout_MacroGroupSC(vector<Macro_Group> &MacroGroupBySC)
{
    cout<<"[INFO] Number of Macro group by SC: "<< MacroGroupBySC.size() <<endl;


    cout << "   Write macro_group.txt ..." << endl;
    ofstream fout("./output/Group/macro_groupSC.txt", fstream::out);

    for(unsigned int i = 0 ; i < MacroGroupBySC.size(); i++)
    {
        fout<<"  group "<< (i+1) <<" size : "<< MacroGroupBySC[i].member_ID.size()<<endl;
        for(unsigned int j = 0; j < MacroGroupBySC[i].member_ID.size(); j++)
        {
            int macro_id = MacroGroupBySC[i].member_ID[j];
            fout<< "macro id: "<<  macro_id << " " << macro_list[macro_id]-> macro_name << endl;
        }
        fout<<"\n";
    }
    fout.close();
}

void GroupMacroBySC(map<int, float> &ID_StdArea, vector<vector<int> > &MacroGroup, vector<vector<Macro_Group> > &MacroGroupBySC)
{
    MacroGroupBySC.resize(MacroGroup.size());
    for(unsigned int i = 0; i < MacroGroup.size(); i++)
    {
        /// In the begining, we see each as a macro group
        vector<Macro_Group> &MG_temp = MacroGroupBySC[i];
        MG_temp.resize(MacroGroup[i].size());
        for(unsigned int j = 0; j < MacroGroup[i].size(); j++)
        {
            int macro_id = MacroGroup[i][j];
            Macro_Group &mg_tmp = MG_temp[j];
            Macro* macro_temp = macro_list[macro_id];

            mg_tmp.ID = j;
            mg_tmp.member_ID.reserve(10);
            mg_tmp.member_ID.push_back(macro_id);
            mg_tmp.Avg_H = macro_temp->cal_h_wo_shrink;
            mg_tmp.Avg_W = macro_temp->cal_w_wo_shrink;
            mg_tmp.GCenter_X = (macro_temp->gp.llx + macro_temp->gp.urx) / 2;
            mg_tmp.GCenter_Y = (macro_temp->gp.lly + macro_temp->gp.ury) / 2;
            mg_tmp.StdAreaInGroup = ID_StdArea[macro_id];
            mg_tmp.MacroArea = macro_temp->area;
            mg_tmp.lef_type_ID = macro_temp->lef_type_ID;
//            cout<<"TYPE : "<<macro_temp->lef_type_ID<<endl;
//                cout<<"macro_temp->NetID.size() : "<<macro_temp->NetID.size()<<endl;
            for(unsigned int k = 0; k < macro_temp->NetID.size(); k++)
            {
                mg_tmp.Net_ID.insert(macro_temp->NetID[k]);
            }

            string name_temp = "";
            for(unsigned int k = 0; k < macro_temp->macro_name.size(); k++)
            {
                if(macro_temp->macro_name[k] != '/')
                {
                    name_temp += macro_temp->macro_name[k];
                }
                else
                {
                    mg_tmp.Hier_Name.push(name_temp);
                    mg_tmp.HierName.push_back(name_temp);   ///plus 108.04.22
                    name_temp = "";
                }
            }
            mg_tmp.Hier_Name.push(name_temp);
            //mg_tmp.HierName.push_back(name_temp);   ///plus 108.04.22
        }
        if(MacroGroup[i].size() > 1)
        {
            CalSC(MG_temp);
        }
    }
//    exit(1);
}

void CalSC(vector<Macro_Group> &MG_temp)
{
    priority_queue<SC_Node, vector<SC_Node>, cmp_score> SC_container;
    float StdAreaConstraint = BenchInfo.std_cell_area * PARAMETER._GROUP_STD_AREA;
    float MacroAreaConstraint = BenchInfo.placeable_area * PARAMETER._GROUP_MACRO_AREA;

    /// initialize the priority queue and determine pairs of macros which satisfy the area constraint
    for(unsigned int i = 0; i < MG_temp.size(); i++)
    {
        for(unsigned int j = i + 1; j < MG_temp.size(); j++)
        {
            if(MG_temp[i].lef_type_ID == MG_temp[j].lef_type_ID)
            {   /// macros who are with the same type can tolerate more stand cell area
                if(MG_temp[i].StdAreaInGroup + MG_temp[j].StdAreaInGroup * 1 / PARAMETER._GROUP_TYPE_STD > StdAreaConstraint || MG_temp[i].MacroArea + MG_temp[j].MacroArea > MacroAreaConstraint)
                {
                    continue;
                }
            }
            else
            {
                if(MG_temp[i].StdAreaInGroup + MG_temp[j].StdAreaInGroup > StdAreaConstraint || MG_temp[i].MacroArea + MG_temp[j].MacroArea > MacroAreaConstraint)
                {
                    continue;
                }
            }
            SC_Node sc_tmp;
            sc_tmp.ID_1 = i;
            sc_tmp.ID_2 = j;
            sc_tmp.MemberSize_1 = 1;
            sc_tmp.MemberSize_2 = 1;
            sc_tmp.score = SC_Funct(MG_temp[i], MG_temp[j]);
            SC_container.push(sc_tmp);
        }
    }

    while(!SC_container.empty())
    {
        const SC_Node &sc_node_tmp = SC_container.top();

        int ID_1 = sc_node_tmp.ID_1;
        int ID_2 = sc_node_tmp.ID_2;
        Macro_Group &mg_temp1 = MG_temp[ID_1];
        Macro_Group &mg_temp2 = MG_temp[ID_2];
//        cout<<sc_node_tmp.score<<endl;
        /// mg_temp1.Dont_Care == true means that it has been grouped and its macros have been others' group member
        /// sc_node_tmp.MemberSize_1 == mg_temp1.member_ID.size() means that the information of this pair is outdated
        if(mg_temp1.Dont_Care == false && mg_temp2.Dont_Care == false &&
                (unsigned int)sc_node_tmp.MemberSize_1 == mg_temp1.member_ID.size() && (unsigned int)sc_node_tmp.MemberSize_2 == mg_temp2.member_ID.size()) /// COMMENT 2021.04
        /*///2021.04
        bool cluster_all = false;
        bool cluster_same_size = false;
        if(mg_temp1.Dont_Care == false && mg_temp2.Dont_Care == false &&
                (unsigned int)sc_node_tmp.MemberSize_1 == mg_temp1.member_ID.size() && (unsigned int)sc_node_tmp.MemberSize_2 == mg_temp2.member_ID.size())
            cluster_same_size = true;
        if(mg_temp1.Dont_Care == false && mg_temp2.Dont_Care == false && same_type_macro_grouping) //cluster all macros in same MacroGroup
            cluster_all = true;
        if(cluster_same_size || cluster_all)
        //////*/
        {
            float total_member = (float)(sc_node_tmp.MemberSize_1 + sc_node_tmp.MemberSize_2);
            float member1_ratio = sc_node_tmp.MemberSize_1 / total_member;
            float member2_ratio = sc_node_tmp.MemberSize_2 / total_member;


            mg_temp1.Avg_H = mg_temp1.Avg_H * member1_ratio + mg_temp2.Avg_H * member2_ratio;
            mg_temp1.Avg_W = mg_temp1.Avg_W * member1_ratio + mg_temp2.Avg_W * member2_ratio;
            mg_temp1.GCenter_X = mg_temp1.GCenter_X * member1_ratio + mg_temp2.GCenter_X * member2_ratio;
            mg_temp1.GCenter_Y = mg_temp1.GCenter_Y * member1_ratio + mg_temp2.GCenter_Y * member2_ratio;
            mg_temp1.StdAreaInGroup += mg_temp2.StdAreaInGroup;
            mg_temp1.MacroArea += mg_temp2.MacroArea;
            if(mg_temp1.lef_type_ID != mg_temp2.lef_type_ID)
            {
                mg_temp1.lef_type_ID = -1;
            }
//            cout<<"member push"<<endl;
            for(int i = 0; i < (int)mg_temp2.member_ID.size(); i++)
            {
                mg_temp1.member_ID.push_back(mg_temp2.member_ID[i]);
            }

//            cout<<"net push"<<endl;
            for(set<int>::iterator iter = mg_temp2.Net_ID.begin(); iter != mg_temp2.Net_ID.end(); iter++)
            {
                mg_temp1.Net_ID.insert((*iter));
            }

//            cout<<"sc node const push"<<endl;

            /// add new pairs of macros
            for(int i = 0; i < (int)MG_temp.size(); i++)
            {
                float ConmbineArea = MG_temp[i].StdAreaInGroup + MG_temp[ID_1].StdAreaInGroup;
                if(MG_temp[ID_1].lef_type_ID != -1 && MG_temp[i].lef_type_ID == MG_temp[ID_1].lef_type_ID)
                {
                    ConmbineArea *= PARAMETER._GROUP_TYPE_STD;
                }
                if(ConmbineArea > StdAreaConstraint || MG_temp[i].MacroArea + MG_temp[ID_1].MacroArea > MacroAreaConstraint)
                {
                    continue;
                }
                if(MG_temp[i].Dont_Care == false && i != ID_1)
                {
                    SC_Node sc_tmp;
                    sc_tmp.ID_1 = i;
                    sc_tmp.ID_2 = ID_1;
                    sc_tmp.MemberSize_1 =  MG_temp[i].member_ID.size();
                    sc_tmp.MemberSize_2 =  MG_temp[ID_1].member_ID.size();
                    sc_tmp.score = SC_Funct(MG_temp[i], MG_temp[ID_1]);
                    SC_container.push(sc_tmp);
                }
            }
            mg_temp2.Dont_Care = true;
        }
        SC_container.pop();
    }
}

float SC_Funct(Macro_Group a, Macro_Group b)
{
    float score;
    float gamma, delta, epsilon, kappa, sigma;
    float Diff_Distance, Hier_Score, Connection, Diff_Area;
    ///User Specified
    gamma = 0.2;
    delta = 0.3;
    epsilon = 10;
    kappa = 0.1;
    sigma = 1;


    Diff_Distance = fabs(a.member_ID.back() - b.member_ID.back());

    Hier_Score = 0;
    while(!a.Hier_Name.empty() && !b.Hier_Name.empty())
    {
        if(a.Hier_Name.front() == b.Hier_Name.front())
        {
            Hier_Score++;
        }
        else
        {
            break;
        }
        a.Hier_Name.pop();
        b.Hier_Name.pop();
    }
    if(!(a.lef_type_ID == -1 && b.lef_type_ID == -1))
    {
        if(a.lef_type_ID == b.lef_type_ID)
        {
            Hier_Score *= 5;
        }
    }
    if(a.Net_ID.size() < b.Net_ID.size())
    {
        Connection = Cal_Connection(a.Net_ID, b.Net_ID);
    }
    else
    {
        Connection = Cal_Connection(b.Net_ID, a.Net_ID);
    }

    Diff_Area = fabs(a.Avg_H * a.Avg_W - b.Avg_H * b.Avg_W);


    score = gamma * min((BenchInfo.chip_H + BenchInfo.chip_W) / Diff_Distance, (float)40) + delta * Hier_Score * 10 + epsilon * Connection * 30+ kappa * min(1 / Diff_Area * 3, (float)3);
    score += (sigma * (15 * exp(min(a.MacroArea, b.MacroArea) / max(a.MacroArea, b.MacroArea))));
//    cout<<"Connection : "<<Connection<<endl;
//    cout<<"score : "<<score<<"\n";
//    cout<<"DIff area : "<<sigma * (25 * exp(min(a.MacroArea, b.MacroArea) / max(a.MacroArea, b.MacroArea)))<<endl;
//    cout<<"a.MacroArea : "<<a.MacroArea<<"\tb.MacroArea : "<<b.MacroArea<<"\n";
//    getchar();
    return score;
}

float Cal_Connection(set<int> &src1, set<int> &src2)
{
    float Connection = 0;

    for(set<int>::iterator iter1 = src1.begin(); iter1 != src1.end(); iter1++)
    {
        set<int>::iterator iter2 = src2.find((*iter1));
        if(iter2 != src2.end())
        {
            Connection += 1;
        }
    }

    return Connection;
}

void Update_hier_area(map<int, float> &ID_StdArea)
{
    for(map<int, float>::iterator iter = ID_StdArea.begin(); iter != ID_StdArea.end(); iter++)
    {
        int ID = iter->first;
        macro_list[ID]->HierStdArea = iter->second;
    }
}

void HIER_LEVEL_Delete(HIER_LEVEL* &hier_temp)
{
    map<string, HIER_LEVEL*> &Name_HierGroup = hier_temp->Name_HierGroup;
    for(map<string, HIER_LEVEL*>::iterator iter = Name_HierGroup.begin(); iter != Name_HierGroup.end(); iter++)
    {
        HIER_LEVEL_Delete(iter->second);
    }

    delete hier_temp;
}

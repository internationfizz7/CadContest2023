#include "DumpFile.h"

int MacroNum = 0;
int StdNum = 0;
int PreNum = 0;
int PortNum= 0;
int lefmacrosPin = 0;
int PinNum = 0;


void clear_para_dump(){
    PortNum= 0;
    lefmacrosPin = 0;
    PinNum = 0;
}

void Output_LEF_info()
{
    cout << "   Write LEFMacros_info.txt..." << endl;
    ofstream fout_LEF("./output/INFO/LEFMacros_info.txt");

    // "macro_count" and "pin ID" is not define yet, it would be 0 before read "DEF file".
    fout_LEF<<"Num of LEF Macros :"<< DEFLEFInfo.LEF_Macros.size()      << endl;
    for(unsigned int i = 0; i< DEFLEFInfo.LEF_Macros.size(); i++)
    {
        LefMacroType &lefmacro_tmp = DEFLEFInfo.LEF_Macros[i];
        fout_LEF    << "\nName: "           << lefmacro_tmp.macro_Name  << "\tID: "             << i                    << endl;
        fout_LEF    << "\tPins number: "    << lefmacro_tmp.Pins.size() << endl;
        lefmacrosPin += lefmacro_tmp.Pins.size();
        for(unsigned int j =0 ; j < lefmacro_tmp.Pins.size() ; j++)
        {
            Pin * pin_tmp = lefmacro_tmp.Pins[j];
            fout_LEF << "\tPinName: "       << pin_tmp-> pin_name         << "\t\tshiftX: "      << pin_tmp-> x_shift
                     << "\tshiftY: "        << pin_tmp-> y_shift          << endl;
        }
        fout_LEF    << "\tRect obstacle number: "    << lefmacro_tmp.rect.size() << endl;
        for(unsigned int j =0 ; j < lefmacro_tmp.rect.size() ; j++)
        {
            OBS_RECT* obs_tmp = lefmacro_tmp.rect[j];
            fout_LEF << "\tLayer: "         << obs_tmp-> blklayer         << "\tllx: " << obs_tmp-> llx
                     << "\tlly: "           << obs_tmp-> lly              << "\turx: " << obs_tmp-> urx
                     << "\tury: "           << obs_tmp-> ury              << endl;
        }
    }

    fout_LEF << "Total Pin num in LEF:\t" << lefmacrosPin    << endl;
    fout_LEF.close();
}

void Output_info()
{
    // 1.output macro_list[] info
    cout << "   Write STD_info.txt / MACRO_info.txt..." << endl;
    ofstream fout_std("./output/INFO/STD_info.txt");
    ofstream fout_m("./output/INFO/MACRO_info.txt");
    ofstream fout_pre("./output/INFO/PRE_info.txt");
    ofstream fout_port("./output/INFO/PORT_info.txt");

    for(unsigned int i = 0; i<macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == STD_CELL)
        {
            fout_std    <<"Name: "          << macro_temp-> macro_name   << "  ID: "  << macro_temp-> macro_id        << endl;
            fout_std    <<"\tX: "           << macro_temp-> gp.llx       << "  Y: "   << macro_temp-> gp.lly          << endl;
            fout_std    <<"\tW * H: "       << macro_temp-> cal_w        << " * "     << macro_temp-> cal_h<<" = "    << macro_temp->area  <<endl;
            fout_std    <<"\tLEF type: "    << macro_temp-> lef_type_ID  << endl;
            fout_std    <<"\tPins num: "    << macro_temp-> pin_num      << endl;
            fout_std    <<"\tNets num: "    << macro_temp-> NetID.size() << endl;
            fout_std    <<"\tGroup id: "    << macro_temp-> group_id     << endl;
            for (unsigned int j =0; j< macro_temp -> NetID.size() ; j++)
            {
                fout_std << "\tNetID: " << macro_temp-> NetID[j] << endl;
            }

            map<int,vector<int> >::iterator itermap;
            for(itermap = macro_temp-> NetsID_to_pinsID.begin() ; itermap!= macro_temp-> NetsID_to_pinsID.end(); itermap++)
            {
                int netid = itermap->first;
                vector<int> pins = itermap->second;
                fout_std  << "\tNetID: "  << netid;
                for(unsigned int k = 0 ; k < pins.size() ; k++)
                {
                    int pinid = pins[k];
                    fout_std  << "\tPinID: "      << pinid    << "\tshiftX: "         << pin_list[pinid]-> x_shift_plus
                            << "\tshiftY: "     << pin_list[pinid]-> y_shift_plus;
                }
                fout_std <<endl;
            }
            fout_std <<endl;
            StdNum++;
        }
        else if(macro_temp->macro_type == MOVABLE_MACRO)
        {
            fout_m    <<"Name: "            << macro_temp-> macro_name   << "  ID: "  << macro_temp-> macro_id          << endl;
            fout_m    <<"\tX: "             << macro_temp-> gp.llx       << "  Y: "   << macro_temp-> gp.lly            << endl;
            fout_m    <<"\tW * H: "         << macro_temp-> cal_w        << " * "     << macro_temp-> cal_h <<" = "     << macro_temp->area << endl;
            fout_m    <<"\tLEF type: "      << macro_temp-> lef_type_ID  << endl;
            fout_m    <<"\tNets num: "      << macro_temp-> NetID.size() << endl;
            for(unsigned int j=0; j< macro_temp -> NetID.size(); j++)
            {
                fout_m << "\tNetID: " << macro_temp-> NetID[j]           << endl;
            }
            fout_m    <<"\tPins num with ports: "      << macro_temp-> pin_num << endl;
            fout_m    <<"\tPins num without ports: "   << macro_temp-> NetsID_to_pinsID.size() << endl;

            map<int,vector<int> >::iterator itermap;
            for(itermap = macro_temp-> NetsID_to_pinsID.begin() ; itermap!= macro_temp-> NetsID_to_pinsID.end(); itermap++)
            {
                int netid = itermap->first;
                vector<int> pins = itermap->second;
                fout_m  << "\tNetID: "  << netid;
                for(unsigned int k = 0 ; k < pins.size() ; k++)
                {
                    int pinid = pins[k];
                    fout_m  << "\tPinID: "      << pinid    << "\tshiftX: "         << pin_list[pinid]-> x_shift_plus
                            << "\tshiftY: "     << pin_list[pinid]-> y_shift_plus;
                }
                fout_m <<endl;
            }
            fout_m <<endl;
            MacroNum++;
        }
        else if(macro_temp->macro_type == PRE_PLACED)
        {
            fout_pre    <<"Name: "              << macro_temp->macro_name       << "  ID: "     << macro_temp->macro_id     << endl;
            fout_pre    <<"\tX: "               << macro_temp->gp.llx           << "  Y: "      << macro_temp->gp.lly       << endl;
            fout_pre    <<"\tW: "               << macro_temp->cal_w            << "\tH :"      << macro_temp->cal_h        << "\tTerminal" << endl;
            fout_pre    <<"\tLEF type: "        << macro_temp-> lef_type_ID     << endl;
            fout_pre    <<"\tNets num: "        << macro_temp-> NetID.size()    << endl;
            for(unsigned int j=0; j< macro_temp -> NetID.size(); j++)
            {
                fout_pre << "\tNetID: " << macro_temp-> NetID[j]                << endl;
            }

            fout_pre    <<"\tPins num with ports: "      << macro_temp-> pin_num << endl;
            fout_pre    <<"\tPins num without ports: "   << macro_temp-> NetsID_to_pinsID.size() << endl;

            map<int,vector<int> >::iterator itermap;
            for(itermap = macro_temp-> NetsID_to_pinsID.begin() ; itermap!= macro_temp-> NetsID_to_pinsID.end(); itermap++)
            {
                int netid = itermap->first;
                vector<int> pins = itermap->second;
                fout_pre  << "\tNetID: "  << netid;
                for(unsigned int k = 0 ; k < pins.size() ; k++)
                {
                    int pinid = pins[k];
                    fout_pre  << "\tPinID: "      << pinid    << "\tshiftX: "       << pin_list[pinid]-> x_shift_plus
                            << "\tshiftY: "     << pin_list[pinid]-> y_shift_plus;
                }
                fout_pre <<endl;
            }
            fout_pre <<endl;
            PreNum++;
        }
        else if(macro_temp->macro_type == PORT)
        {
            fout_port    <<"Name: "          << macro_temp->macro_name   << "  ID: "     << macro_temp->macro_id    << endl;
            fout_port    <<"\tX: "           << macro_temp->gp.llx       << "  Y: "      << macro_temp->gp.lly      << endl;
            fout_port    <<"\tPins num: "       << macro_temp-> pin_num      << endl;
            fout_port    <<"\tNets num: "       << macro_temp-> NetID.size() << endl;
            for(unsigned int j=0; j< macro_temp -> NetID.size(); j++)
            {
                fout_port << "\tNetID: " << macro_temp-> NetID[j] << endl;
            }
            fout_port <<endl;
            PortNum++;
        }
    }

    fout_std<<"STD NUM: "   <<StdNum   <<endl;
    fout_m  <<"MACRO NUM: " <<MacroNum <<endl;
    fout_pre<<"PRE NUM: "   <<PreNum   <<endl;
    fout_port<<"PORT NUM: " <<PortNum  <<endl;

    fout_std.close();
    fout_m.close();
    fout_pre.close();
    fout_port.close();

    // 2.Output pin_list[] info.
    cout << "   Write PIN_LIST_info..." << endl;
    ofstream fout_pin("./output/INFO/PIN_LIST_info.txt");

    fout_pin <<"NumPins : "<< pin_list.size() << endl;

    for(unsigned int i = 0; i< pin_list.size(); i++)
    {
        Pin* pin_temp = pin_list[i];
        fout_pin << "\nName: "      << pin_temp-> pin_name      << " ID: "          << pin_temp-> pin_id        << endl;
        fout_pin << "\tBelong to LEF Macro ID: "                << pin_temp-> macroID                           << endl;
        fout_pin << "\tshiftX: "    << pin_temp-> x_shift_plus  << "\tshiftY: "     << pin_temp-> y_shift_plus  << endl;
    }

    fout_pin.close();

    // 3.Output net_list info.
    cout << "   Write NET_LIST_info.txt..." << endl;
    ofstream fout_net("./output/INFO/NET_LIST_info.txt");

    fout_net << "NumNets : "<< net_list.size() << endl << endl;

    for(unsigned int i = 0; i<net_list.size(); i++)
    {
        Net_ML net_temp = net_list[i];
        fout_net <<"NetDegree : "   << net_temp.macro_idSet.size() + net_temp.cell_idSet.size()
                 << "\tnet ID:\t"   << net_temp.NetID   <<"\tNet Name : "    << net_temp.net_Name << endl;
        fout_net <<"  -Non std ID connections: "        << net_temp.macro_idSet.size()  << endl;
        for(unsigned int j =0; j<net_temp.macro_idSet.size(); j++)
        {
            int MacroID = net_temp.macro_idSet[j];
            Macro* macro_temp = macro_list[MacroID];
            string str = "NULL";
            if(macro_temp-> macro_type == 0) str = "MOVABLE";
            if(macro_temp-> macro_type == 1) str = "PREPLACED";
            if(macro_temp-> macro_type == 4) str = "PORT";
            fout_net << "\tName: \t"     << macro_temp-> macro_name    <<"\t\tType:\t"  << str      << endl;
        }
        fout_net  <<"  -std ID connections: "     << net_temp.cell_idSet.size()         << endl;
        for(unsigned int j =0; j<net_temp.cell_idSet.size(); j++)
        {
            int stdID = net_temp.cell_idSet[j];
            Macro* std_temp = macro_list[stdID];
            fout_net <<'\t'  << std_temp-> macro_name   << endl;
        }
        fout_net  <<"  -std group ID: "     << net_temp.group_idSet.size()    << endl;
        for(unsigned int j =0; j<net_temp.group_idSet.size(); j++)
        {
            int GroupID = net_temp.group_idSet[j];
            STD_Group* group_temp = STDGroupBy[GroupID];
            fout_net <<"\tgroup "  << group_temp-> ID    << endl;
        }
        fout_net<<endl;
    }
    fout_net.close();

    // 4.Output std group info.
    cout << "   Write STD_GROUP_info.txt..." << endl;
    ofstream fout_std_group("./output/INFO/STD_GROUP_info.txt");

    fout_std_group << "Number of groups : "<< STDGroupBy.size() << endl;
    for(unsigned int i = 0; i < STDGroupBy.size() ; i++)
    {
        fout_std_group << "\nSTD_group ID: "<< STDGroupBy[i]-> ID           << "\tCenter X: "   << STDGroupBy[i]-> GCenter_X
                       << "\tCenter Y: "    << STDGroupBy[i]-> GCenter_Y    << "\tAvg W: "      << STDGroupBy[i]-> Avg_W
                       << "\tAvg H: "       << STDGroupBy[i]-> Avg_H        << "\tsqrt W: "     << STDGroupBy[i]-> Width
                       << "\tsqrt H: "      << STDGroupBy[i]-> Height       << "\tToatlArea: "  << STDGroupBy[i]-> TotalArea << endl;
        fout_std_group << "\tNumber of cells: " << STDGroupBy[i]-> member_ID.size()  << endl;
        for(unsigned int j = 0; j < STDGroupBy[i]-> member_ID.size() ; j++)
        {
            int stdid = STDGroupBy[i]-> member_ID[j];
            fout_std_group << "\tCell ID: "<< stdid << "\tNumber of connected Net weight: "    << macro_list[stdid]-> NetID.size()   << endl;
        }
        map<int, int>::iterator itermap;
        map<int, int> &netsmap = STDGroupBy[i]-> NetID_to_weight;
        fout_std_group << "\tNumber of connected nets: " << netsmap.size()  << endl;
        for(itermap = netsmap.begin() ; itermap != netsmap.end() ; itermap++ )
        {
            fout_std_group << "\tNet ID: "<< itermap-> first << "\tNet weight: "    << itermap-> second   << endl;
        }
    }
    fout_std_group.close();

}
void Output_netlist()
{
    // 108.03.20////
    // -------------node-----------
    cout << "   Write output.nodes..." << endl;
    ofstream fout_node("./output/netlist/output.nodes");
    fout_node<<"2019.03.20 YA-CHU Yang"<<endl<<endl<<endl;
    fout_node<<"NumNodes : "<< MacroNum+PreNum <<endl;
    fout_node<<"NumTerminals : "<< PreNum <<endl;

    for(unsigned int i = 0; i<macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == MOVABLE_MACRO)
        {
            fout_node<< macro_temp->macro_name;
            fout_node<<'\t'<< macro_temp->cal_w << '\t' <<macro_temp->cal_h << endl;
        }
    }
    for(unsigned int i = 0; i<Placement_blockage.size(); i++)
    {
        Boundary tmp = Placement_blockage[i];
        fout_node<< "Placement_blockage_" <<i;
        fout_node<<'\t'<< tmp.urx - tmp.llx << '\t' << tmp.ury - tmp.lly << '\t'<<"terminal"<< endl;
    }
    for(unsigned int i = 0; i<macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == PRE_PLACED)
        {
            fout_node<< macro_temp->macro_name;
            fout_node<<'\t'<< macro_temp->cal_w << '\t' <<macro_temp->cal_h << '\t'<<"terminal"<<endl;
        }
    }

    // ----------net----------
    cout << "   Write output.nets..." << endl;
    ofstream fout_net("./output/netlist/output.nets");

    PinNum = 0;
    for(unsigned int i = 0; i< net_list.size(); i++)
    {
        PinNum += net_list[i].macro_idSet.size();
        PinNum += net_list[i].cell_idSet.size();
    }
    fout_net<<"2019.03.20 YA-CHU Yang" <<endl <<endl <<endl;
    fout_net<<"NumNets : "<< net_list.size()  <<endl;
    fout_net<<"NumPins : "<< PinNum << "(plus all macro_idSet and cell_idset)"   <<endl;

    for(unsigned int i = 0; i<net_list.size(); i++)
    {
        Net_ML net_temp = net_list[i];
        fout_net<<"NetDegree : "<< net_temp.macro_idSet.size() + net_temp.cell_idSet.size() <<'\t'<< "n"<< net_temp.NetID << endl;
        for(unsigned int j =0; j<net_temp.macro_idSet.size(); j++)
        {
            int MacroID = net_temp.macro_idSet[j];
            Macro* macro_temp = macro_list[MacroID];
            fout_net<<'\t'<<macro_temp->macro_name<<'\t'<<"I"<<'\t'<<":"<<'\t'<<"0"<<'\t'<<"0"<<endl;
        }
        for(unsigned int j =0; j<net_temp.group_idSet.size(); j++)
        {
            int MacroID = net_temp.group_idSet[j];
            Macro* macro_temp = macro_list[MacroID];
            fout_net<<'\t'<<macro_temp->macro_name<<endl;
        }
        fout_net<<endl;
    }

    //-----------config------------
    cout << "   Write output.config..." << endl;
    ofstream fout_config("./output/netlist/output.config");
    fout_config << "boundary_x  " << chip_boundary.llx <<endl;
    fout_config << "boundary_y  " << chip_boundary.lly <<endl;
    fout_config << "boundary_w  " << chip_boundary.urx - chip_boundary.llx <<endl;
    fout_config << "boundary_h  " << chip_boundary.ury - chip_boundary.lly <<endl;
    fout_config << "shift_x  "  << chip_boundary.llx <<endl;
    fout_config << "shift_y  "  << chip_boundary.lly <<endl;
    fout_config << "core_w  "   << chip_boundary.urx - chip_boundary.llx <<endl;
    fout_config << "core_h  "   << chip_boundary.ury - chip_boundary.lly <<endl;

    fout_config << endl;
    fout_config << "Macro Num :\t"    << MacroNum     <<endl;
    fout_config << "Preplace Num :\t" << PreNum       <<endl;
    fout_config << "STD Num :\t"      << StdNum       <<endl;
    fout_config << "PORT NUM :\t"     << PortNum      <<endl;
    fout_config << endl;
    fout_config << "NumNets : "     << net_list.size()  <<endl;
    fout_config << "NumPins : "     << PinNum           <<endl;
    fout_config << endl;
    //fout_config << "LEF Macro's pins num (Pin type with PORT ): "     << lefmacrosPin     << endl;
    //fout_config << "pin_list[] pins num  (Pin type without PORT): "   << pin_list.size()  << endl;


    //--------------pl---------------
    cout << "   Write output.pl..." << endl;
    ofstream fout_pl("./output/netlist/output.pl");
    fout_pl << "UCLA pl 1.0" <<endl<<endl;
    for(unsigned int i = 0; i<macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == MOVABLE_MACRO)
        {
            fout_pl<< macro_temp->macro_name;
            fout_pl<<'\t'<< macro_temp->gp.llx << '\t' << macro_temp->gp.lly << " : " << macro_temp->orient << endl;
        }
    }
    for(unsigned int i = 0; i<Placement_blockage.size(); i++)
    {
        Boundary tmp = Placement_blockage[i];
        fout_pl<< "Placement_blockage_" <<i;
        fout_pl<<'\t'<< tmp.llx << '\t' << tmp.lly << " : N " << "  /FIXED" << endl;
    }

    for(unsigned int i = 0; i<macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        if(macro_temp->macro_type == PRE_PLACED)
        {
            fout_pl<< macro_temp->macro_name;
            fout_pl<<'\t'<< macro_temp->gp.llx << '\t' << macro_temp->gp.lly << " : " << macro_temp->orient << "  /FIXED" << endl;
        }
    }

    fout_node.close();
    fout_net.close();
    fout_config.close();
    fout_pl.close();

}


void Output_modDEF() //for what?
{
    //-------------- output ModifyDEF.def-----------------
    // change compoments to macro_list[] amount
    cout << "   Write ModifyDEF.def..." << endl;
    ifstream fin(def_file.c_str());     // need to run deflef_parser()
    string filename = "./output/netlist/ModifyDEF.def";
    ofstream fout_def(filename.c_str());
    string temp, line;
    while(fin>>temp)
    {
        if(temp == "COMPONENTS")
        {
            getline(fin, line);
            fout_def << temp <<" "<< line<<endl;
            for(unsigned int i = 0; i<macro_list.size(); i++)
            {
                Macro* macro_temp = macro_list[i];
                LefMacroType &lef_tmp = DEFLEFInfo.LEF_Macros[macro_temp ->lef_type_ID];

                if(macro_temp->macro_type == MOVABLE_MACRO || macro_temp->macro_type == STD_CELL )
                {
                    fout_def<< "- "<< macro_temp->macro_name << " " << lef_tmp.macro_Name << " + ";
                    fout_def << "PLACED ( " << macro_temp->gp.llx <<" "<<macro_temp->gp.lly
                             << " ) " << macro_temp->orient << " ;"<<endl;
                }
                else if (macro_temp->macro_type == PRE_PLACED)
                {
                    fout_def<< "- "<< macro_temp->macro_name << " " << lef_tmp.macro_Name << " + ";
                    fout_def << "FIXED ( " << macro_temp->gp.llx <<" "<<macro_temp->gp.lly
                             << " ) " << macro_temp->orient << " ;"<<endl;
                }
                else if (macro_temp->macro_type == PORT)
                {
                    fout_def<< "- "<< macro_temp->macro_name << " " << lef_tmp.macro_Name << " + ";
                    fout_def << "FIXED ( " << macro_temp->gp.llx <<" "<<macro_temp->gp.lly
                             << " ) " << macro_temp->orient << " ;"<<endl;
                }
                else
                    cout<<"(Isn't a MOVABLE_MACRO STD_CELL and fixed ) name: "<<macro_temp->macro_name<<endl;
            }
            fout_def<<"END COMPONENTS"<<endl;
            while(getline(fin, line))
            {
                if(line == "END COMPONENTS")
                {
                    break;
                }
            }
        }
        else
        {
            getline(fin, line);
            fout_def << temp <<" "<<line<<endl;
        }
    }
    fin.close();
    fout_def.close();

}

void Output_tcl(vector<Macro*> &MacroClusterSet, string Filename, unsigned int STDXC, unsigned int STDYC)
{
    cout << "   Write " << Filename <<"..." << endl;
    ofstream fout(Filename.c_str());
    /// set_undoable_attribute [get_cells pa5645_dig/od_main_i/od_table_pool/od_lut6/S3_G/uBisted_RAM1R_m3d_84X40/RAM1R_m3d_84X40_u0/SRAM_i0] orientation {S}
    double space = PARAMETER._SPACING / (double)(2*PARA);
    for(unsigned int i = 0; i < MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];

        double x, y;
        x = (double)(macro_temp->lg.llx - shifting_X + macro_temp->Spacing);    ///modify 108.03.21
        y = (double)(macro_temp->lg.lly - shifting_Y + macro_temp->Spacing);    ///modify 108.03.21


        string macroName = macro_temp->macro_name;
        string Orientation = MacroOrientation(macro_temp, STDXC, STDYC,  x, y);
        x /= (double) PARA;
        y /= (double) PARA;
        Boundary placement_blockage;
        Boundary_Assign(placement_blockage, x - space, y - space, x + macro_temp->w / (double)PARA+  space, y + macro_temp->h / (double)PARA +  space);
        fout<<"set_undoable_attribute [get_cells "<<macroName<<"] orientation {"<<Orientation<<"}"<<endl;
        fout<<"move_objects -x "<<x<<" -y "<<y<<" "<<macroName<<endl;
        fout<<"set_object_fixed_edit "<<macroName<<" 1"<<endl;
        fout<<"create_placement_blockage -coordinate {{"<<placement_blockage.llx<<" "<<placement_blockage.lly<<"} {";
        fout<<placement_blockage.urx<<" "<<placement_blockage.ury<<"}} -name YL_placement_blockage_"<<i<<" -type hard"<<endl;

    }

    fout.close();
}

string MacroOrientation(Macro* macro_temp, int STDXC, int STDYC, int x, int y)
{

    LefMacroType &lef_temp = DEFLEFInfo.LEF_Macros[macro_temp->lef_type_ID];

    unsigned int N_WL = 0;
    unsigned int FN_WL = 0;
    unsigned int S_WL = 0;
    unsigned int FS_WL = 0;
    for(int i = 0; i < (int)lef_temp.Pins.size(); i++)
    {
        //// MODIFY 2020.02.21 ////
        //int shift_x = lef_temp.shiftXY[i].first;
        //int shift_y = lef_temp.shiftXY[i].second;
        int shift_x = lef_temp.Pins[i]-> x_shift;
        int shift_y = lef_temp.Pins[i]-> y_shift;

        N_WL += (fabs(STDXC - (x+shift_x)) + fabs(STDYC - (y+shift_y)));
        FN_WL += (fabs(STDXC - (x+lef_temp.macroW-shift_x)) + fabs(STDYC - (y+shift_y)));
        S_WL += (fabs(STDXC - (x+lef_temp.macroW-shift_x)) + fabs(STDYC - (y+lef_temp.macroH-shift_y)));
        FS_WL += (fabs(STDXC - (x+shift_x)) + fabs(STDYC - (y+lef_temp.macroH-shift_y)));
    }
    string Orientation = "N";
    unsigned int minWL = N_WL;
    if(minWL > FN_WL)
    {
        minWL = FN_WL;
        Orientation = "FN";
    }

    if(minWL > S_WL)
    {
        minWL = S_WL;
        Orientation = "S";
    }

    if(minWL > FS_WL)
    {
        minWL = FS_WL;
        Orientation = "FS";
    }

    return Orientation;
}



/**********************modify 108.03.26************************/
void WriteDefFile(vector<Macro*> &MacroClusterSet, string OutputName, unsigned int STDXC, unsigned int STDYC, string filename, bool SoftBlockage,NETLIST & nt)
{
    
    cout << "   Write " << OutputName <<"..." << endl;
    ifstream fin (filename.c_str());
    ofstream fout (OutputName.c_str());

    if (!fin)
    {
        cout << "[ERROR] can't open file " << filename << endl;
        return;
    }
    if (!fout)
    {
        cout << "[ERROR] can't open file " << OutputName << endl;
        return;
    }

    string line, temp;

    while (fin>>temp)
    {
        if(temp != "DIEAREA")
        {
            getline(fin, line);
            fout << temp + line << endl;
        }
        else
        {
            getline(fin, line);
            fout << temp + line << endl;
            break;
        }
    }
    /***component***/
    fout << "COMPONENTS " << MacroClusterSet.size() <<" ;"<<endl;
    for(unsigned int i = 0; i < MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];

        int x, y;
        x = (double)(macro_temp->lg.llx - shifting_X + macro_temp->Spacing);
        y = (double)(macro_temp->lg.lly - shifting_Y + macro_temp->Spacing);

        string macroName = macro_temp->macro_name;

        
        int LefTypeID = macro_temp->lef_type_ID;
        string macroType = DEFLEFInfo.LEF_Macros[LefTypeID].macro_Name;
        string Orientation = MacroOrientation(macro_temp, STDXC, STDYC,  x, y);
        //x /= (double) PARA;
        //y /= (double) PARA;

        fout << " - "<< macroName << " " << macroType <<
             " + FIXED ( " << x << " " << y <<" ) " << Orientation << " ;"<<endl;
        nt.mods[nt.GetModID(macroName)].LeftX=nt.mods[nt.GetModID(macroName)].GLeftX=x;
        nt.mods[nt.GetModID(macroName)].LeftY=nt.mods[nt.GetModID(macroName)].GLeftY=y;
        nt.mods[nt.GetModID(macroName)].CenterX=nt.mods[nt.GetModID(macroName)].GCenterX=x+0.5*nt.mods[nt.GetModID(macroName)].modW;
        nt.mods[nt.GetModID(macroName)].CenterY=nt.mods[nt.GetModID(macroName)].GCenterY=y+0.5*nt.mods[nt.GetModID(macroName)].modH;

        //cout<<nt.GetModID(macroName)<<" "<<nt.mods[nt.GetModID(macroName)].LeftY<<" "<<nt.mods[nt.GetModID(macroName)].LeftY<<"\n";
    }
    //fout << "END COMPONENTS"<<endl;
    /******blockage******/
    //fout<< "BLOCKAGES "<< MacroClusterSet.size() << " ;" << endl;
    //double space = PARAMETER._SPACING / (double)(2*PARA);
    double space = PARAMETER._SPACING / (double)(2);
    for(unsigned int i = 0; i < MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];

        int x, y;
        x = (double)(macro_temp->lg.llx - shifting_X + macro_temp->Spacing);
        y = (double)(macro_temp->lg.lly - shifting_Y + macro_temp->Spacing);

        string Orientation = MacroOrientation(macro_temp, STDXC, STDYC,  x, y);
        //x /= (double) PARA;
        //y /= (double) PARA;


        if(SoftBlockage)
        {
            double shifting = - space + macro_temp->Spacing/2;
            fout<<" - PLACEMENT + SOFT"<<endl;
            fout<< "  RECT ( "<< macro_temp->lg.llx + shifting << " " << macro_temp->lg.lly + shifting <<
                      " ) ( " << macro_temp->lg.urx + shifting << " " << macro_temp->lg.ury + shifting << " ) ;"<<endl;
        }
        else{
            Boundary placement_blockage;
            //Boundary_Assign(placement_blockage, x - space, y - space, x + macro_temp->w / (double)PARA+  space, y + macro_temp->h / (double)PARA +  space);
            Boundary_Assign(placement_blockage, x - space, y - space, x + macro_temp->w + space, y + macro_temp->h + space);
            fout<<" - PLACEMENT"<<endl;
            fout<< "  RECT ( "<< placement_blockage.llx << " " << placement_blockage.lly <<
                      " ) ( " << placement_blockage.urx << " " << placement_blockage.ury << " ) ;"<<endl;

        }
    }
    fout << "END BLOCKAGES" <<endl<<endl;

    fout << "END DESIGN" <<endl;
}
/**************************************************************/

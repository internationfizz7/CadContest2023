#include "plot.h"

void Plot_global(bool STD_plot, string fileName)
{
    cout << "   Write "<< fileName << endl;
    if(fileName == "") fileName = "./output/GP_graph/GP.m";
    ofstream fout(fileName.c_str());


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;
    int moveable_count = 0;
    for(unsigned int i = 0 ; i < macro_list.size() ; i++)
    {
        if(macro_list[i]->macro_type == PSEUDO_MODULE)
            continue;
        int llx = macro_list[i] -> gp.llx ;
        int urx = macro_list[i] -> gp.urx ;
        int lly = macro_list[i] -> gp.lly ;
        int ury = macro_list[i] -> gp.ury ;


        string macro_name = macro_list[i] -> macro_name;
        string name_temp = "";
        int countingSlash = 0;
        for(int j = 0; j < (int)macro_name.size(); j++)
        {
            if(macro_name[j] == '/')
            {
                countingSlash++;
                if(countingSlash == 2)
                {
                    break;
                }
            }
            if(macro_name[j] == '_')
            {
                name_temp += ":";
            }
            else
            {
                name_temp += macro_name[j];
            }
        }
        macro_name = name_temp;
        unsigned int macro_type = macro_list[i] -> macro_type ;
        string color ;


        if(macro_type == MOVABLE_MACRO)
        {
            color = "r" ;

        }
        else if(macro_type == PRE_PLACED)
        {
//            continue;
            color = "b" ;
        }
        else
        {
            color = "g" ;

        }
        if(macro_type == MOVABLE_MACRO || macro_type == PRE_PLACED )
        {
            fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
            fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
            fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << moveable_count << "');" << endl << endl;
            if(macro_type == MOVABLE_MACRO) moveable_count ++;
            fout << "fill(block_x,block_y,'"<< color <<"','facealpha',0.5);" << endl;
        }
    }

    for(unsigned int i = 0 ; i < Placement_blockage.size() ; i++)
    {
        double llx = Placement_blockage[i].llx ;
        double urx = Placement_blockage[i].urx ;
        double lly = Placement_blockage[i].lly ;
        double ury = Placement_blockage[i].ury ;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
//        fout << "fill(block_x,block_y,'c');" << endl;
        fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << i << "');" << endl << endl;
        fout << "fill(block_x,block_y,'c','facealpha',0.5);" << endl;
    }

    fout.close() ;
}

void Plot_legal(bool STD_plot, string fileName)
{
    cout << "   Write LG.m ..." << endl;
    if(fileName == "") fileName = "./output/LG_graph/LG.m" ;
    ofstream fout(fileName.c_str()) ;


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;
    for(unsigned int i = 0 ; i < macro_list.size() ; i++)
    {
//        if(macro_list[i]->macro_type == PSEUDO_MODULE)
//            continue;
        double llx = macro_list[i] -> lg.llx ;
        double urx = llx + macro_list[i] -> cal_w_wo_shrink ;
        double lly = macro_list[i] -> lg.lly ;
        double ury = lly + macro_list[i] -> cal_h_wo_shrink ;
        unsigned int macro_type = macro_list[i] -> macro_type ;
        string color ;


        if(macro_type == MOVABLE_MACRO)
        {
            color = "r" ;
        }
        else if(macro_type == PRE_PLACED)
        {
//            continue;
            color = "b" ;
        }
        else if(macro_type == PSEUDO_MODULE)
        {
            color = "c" ;
        }
        if(macro_type != STD_CELL )
        {
            if(macro_type == MOVABLE_MACRO)
                fout<<"line(["<< BenchInfo.stdXcenter <<" "<< llx <<"],["<< BenchInfo.stdYCenter <<" "<< lly <<"],'Color','r');"<<endl;
            fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
            fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
            //fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << i << "');" << endl << endl;
            fout << "fill(block_x,block_y,'"<< color <<"','facealpha',0.5);" << endl;
        }
    }

    for(unsigned int i = 0 ; i < Placement_blockage.size() ; i++)
    {
        double llx = Placement_blockage[i].llx ;
        double urx = Placement_blockage[i].urx ;
        double lly = Placement_blockage[i].lly ;
        double ury = Placement_blockage[i].ury ;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'c');" << endl;
    }

    fout.close() ;
}

void Plot_rectangle(vector<Boundary> plot_boundary, string fileName)
{
    if(fileName == "")
    {
        cout << "   Write Plot_rectangle.m ..." << endl;
        fileName = "./output/GP_graph/Plot_rectangle.m";
    }
    ofstream fout(fileName.c_str()) ;


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(unsigned int i = 0 ; i < plot_boundary.size() ; i++)
    {
        double llx = plot_boundary[i].llx ;
        double urx = plot_boundary[i].urx ;
        double lly = plot_boundary[i].lly ;
        double ury = plot_boundary[i].ury ;

        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << i << "');" << endl << endl;
        fout << "fill(block_x,block_y,'c','facealpha',0.5);" << endl;

    }

    fout.close() ;
}

void Plot_rectangle_w_ID(vector<pair<string,Boundary> > plot_boundary, string fileName)
{
    if(fileName == "")
    {
        cout << "   Write Plot_rectangle.m ..." << endl;
        fileName = "./output/GP_graph/Plot_rectangle.m";
    }
    ofstream fout(fileName.c_str()) ;


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(unsigned int i = 0 ; i < plot_boundary.size() ; i++)
    {
        double llx = plot_boundary[i].second.llx ;
        double urx = plot_boundary[i].second.urx ;
        double lly = plot_boundary[i].second.lly ;
        double ury = plot_boundary[i].second.ury ;

        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << plot_boundary[i].first << "');" << endl << endl;
        fout << "fill(block_x,block_y,'c','facealpha',0.5);" << endl;

    }

    fout.close() ;
}

void Plot_rectangle_w_color(vector<pair<int,Boundary> > plot_boundary, string fileName)
{
    cout << "   Write Plot_rectangle.m ..." << endl;
    if(fileName == "")
    {
        fileName = "./output/GP_graph/Plot_rectangle.m";
    }
    ofstream fout(fileName.c_str()) ;


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(unsigned int i = 0 ; i < plot_boundary.size() ; i++)
    {
        double llx = plot_boundary[i].second.llx ;
        double urx = plot_boundary[i].second.urx ;
        double lly = plot_boundary[i].second.lly ;
        double ury = plot_boundary[i].second.ury ;
        string color;
        if(plot_boundary[i].first < 0)
        {
            continue;
        }
        if(plot_boundary[i].first == 1)
        {
            color = 'b';
        }
        else if(plot_boundary[i].first == 2)
        {
            color = 'g';
        }
        else if(plot_boundary[i].first == 3)
        {
            color = 'c';
        }
        else if(plot_boundary[i].first == 4)
        {
            color = 'r';
        }
        else
        {
            color = 'k';
        }
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << i << "');" << endl << endl;
        fout << "fill(block_x,block_y,'"<<color<<"','facealpha',0.5);" << endl;

    }

    fout.close() ;
}


void PlotMacroClusterSet(vector<Macro*> MacroClusterSet, string fileName)
{
    if(fileName == "")
        fileName = "./output/LG_graph/PlotMacroClusterSetLG.m" ;

    ofstream fout(fileName.c_str());

    //fout<<"figure('visible','off');\nclear;\nclf;\n";///show figure

    //fout<<"axis equal;\nhold on;\ngrid on;\n"<<endl;

	fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {

        Macro* macro_temp = MacroClusterSet[i];
        string color;
        double llx = macro_temp->lg.llx ;
        double urx = macro_temp->lg.urx ;
        double lly = macro_temp->lg.lly ;
        double ury = macro_temp->lg.ury ;
//        if(macro_temp->LegalFlag == true)
//        {
//            color = "r";
//        }
//        else
//        {
//            color = "k";
//        }
        if(macro_temp->LegalFlag == false)
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
            fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << i/*macro_temp->macro_id*/ << "','fontsize',5);" << endl << endl;
        fout << "fill(block_x,block_y,'"<<color<<"','facealpha',0.5);" << endl;
    }
//    getchar();
    for(unsigned int i = 0 ; i < Placement_blockage.size() ; i++)
    {
        double llx = Placement_blockage[i].llx ;
        double urx = Placement_blockage[i].urx ;
        double lly = Placement_blockage[i].lly ;
        double ury = Placement_blockage[i].ury ;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'c');" << endl;
    }
    fout<<"plot("<<BenchInfo.WhiteSpaceXCenter<<","<<BenchInfo.WhiteSpaceYCenter<<",'r.','markersize',50);"<<endl;
    fout<<"plot("<<BenchInfo.stdXcenter<<","<<BenchInfo.stdYCenter<<",'k.','markersize',25);"<<endl;

    string fileName_temp = "";
    for(int i = (int)fileName.size() - 3; i >= 0; i--)
    {
        if(fileName[i] == '/')
        {
            break;
        }
        else
        {
            fileName_temp += fileName[i];
        }
    }

    reverse(fileName_temp.begin(), fileName_temp.end());
    //fout<<"saveas(gcf,'"<<fileName_temp<<"','jpg');"<<endl;
    fout.close() ;
}

/// ADD 2021.07
void PlotMacroClusterSet2(vector<pair<int, int> > PRegion_Center_Point, vector<Macro*> MacroClusterSet, string fileName)
{
    if(fileName == "")
        fileName = "./output/LG_graph/PlotMacroClusterSetLG.m" ;

    ofstream fout(fileName.c_str());

    //fout<<"figure('visible','off');\nclear;\nclf;\n";///show figure

    //fout<<"axis equal;\nhold on;\ngrid on;\n"<<endl;

	fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {

        Macro* macro_temp = MacroClusterSet[i];
        string color;
        double llx = macro_temp->lg.llx ;
        double urx = macro_temp->lg.urx ;
        double lly = macro_temp->lg.lly ;
        double ury = macro_temp->lg.ury ;
//        if(macro_temp->LegalFlag == true)
//        {
//            color = "r";
//        }
//        else
//        {
//            color = "k";
//        }
        if(macro_temp->LegalFlag == false)
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
            fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << i/*macro_temp->macro_id*/ << "','fontsize',5);" << endl << endl;
        fout << "fill(block_x,block_y,'"<<color<<"','facealpha',0.5);" << endl;
    }
//    getchar();
    for(unsigned int i = 0 ; i < Placement_blockage.size() ; i++)
    {
        double llx = Placement_blockage[i].llx ;
        double urx = Placement_blockage[i].urx ;
        double lly = Placement_blockage[i].lly ;
        double ury = Placement_blockage[i].ury ;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'c');" << endl;
    }
    fout<<"plot("<<BenchInfo.WhiteSpaceXCenter<<","<<BenchInfo.WhiteSpaceYCenter<<",'r.','markersize',50);"<<endl;
    fout<<"plot("<<BenchInfo.stdXcenter<<","<<BenchInfo.stdYCenter<<",'k.','markersize',25);"<<endl;

    /// ADD 2021.07
    for(int i = 0; i < (int)PRegion_Center_Point.size(); i++)
    {
        fout<<"plot("<<PRegion_Center_Point[i].first<<","<<PRegion_Center_Point[i].second<<",'b.','markersize',20);"<<endl;
    }
    //////
    string fileName_temp = "";
    for(int i = (int)fileName.size() - 3; i >= 0; i--)
    {
        if(fileName[i] == '/')
        {
            break;
        }
        else
        {
            fileName_temp += fileName[i];
        }
    }

    reverse(fileName_temp.begin(), fileName_temp.end());
    //fout<<"saveas(gcf,'"<<fileName_temp<<"','jpg');"<<endl;
    fout.close() ;
}
//////

void PlotGlobalMacroClusterSet(vector<Macro*> MacroClusterSet, string fileName)
{
    cout << "   Write PlotMacroClusterSetLG.m..." << endl;
    if(fileName == "")
        fileName = "./output/LG_graph/PlotMacroClusterSetLG.m" ;
    ofstream fout(fileName.c_str()) ;


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(int i = 0; i < (int)MacroClusterSet.size(); i++)
    {
        Macro* macro_temp = MacroClusterSet[i];
        string color;
        int llx = macro_temp->gp.llx ;
        int urx = macro_temp->gp.urx ;
        int lly = macro_temp->gp.lly ;
        int ury = macro_temp->gp.ury ;
        if(macro_temp->LegalFlag == false)
        {
            color = "r";
        }
        else
        {
            color = "g";
        }
//        if(macro_temp->MoveDirection == false)
//        {
//            cout<<macro_temp->macro_id<<"false"<<endl;
//            color = "y";
//        }
//        else
//        {
//            color = "g";
//        }

        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;

        fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << macro_temp->macro_id << "');" << endl << endl;
        fout << "fill(block_x,block_y,'"<<color<<"','facealpha',0.5);" << endl;
    }
//    getchar();
    for(unsigned int i = 0 ; i < Placement_blockage.size() ; i++)
    {
        double llx = Placement_blockage[i].llx ;
        double urx = Placement_blockage[i].urx ;
        double lly = Placement_blockage[i].lly ;
        double ury = Placement_blockage[i].ury ;
        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout << "fill(block_x,block_y,'c');" << endl;
    }

    fout.close() ;
}

void PlotMatlab(vector<PLOT_BLOCK> plot_block, vector<PLOT_LINE> plot_line, bool Figure_visible, bool output_jpg, string fileName)
{
    if(fileName == "")
    {
        cout << "   Write PlotMacroClusterSetLG.m..." << endl;
        fileName = "./output/LG_graph/PlotMacroClusterSetLG.m" ;
    }
    ofstream fout(fileName.c_str()) ;

    if(Figure_visible == true)
    {
        fout<<"figure;"<<endl;
    }
    else
    {
        fout<<"figure('visible','off');"<<endl;///show figure
    }
    fout<<"clear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;


    for(int i = 0; i < (int)plot_block.size(); i++)
    {
        PLOT_BLOCK &block_tmp = plot_block[i];
        int llx = block_tmp.plotrectangle.llx;
        int lly = block_tmp.plotrectangle.lly;
        int urx = block_tmp.plotrectangle.urx;
        int ury = block_tmp.plotrectangle.ury;


        fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;

		//output name 2019.12.20
		//block_tmp.TEXT allways is false  ~~ no text output

		fout << "fill(block_x,block_y,'"<<block_tmp.color<<"');" << endl;

        if(block_tmp.TEXT == true)
        {
            if(block_tmp.int_stringTEXT == true)
            {
                fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << block_tmp.int_TEXT << "');" << endl << endl;
            }
            else
            {
                fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << block_tmp.string_TEXT << "');" << endl << endl;
            }
        }
    }

    for(int i = 0; i < (int)plot_line.size(); i++)
    {
        PLOT_LINE &line_tmp = plot_line[i];
        int llx = line_tmp.plotline.llx;
        int lly = line_tmp.plotline.lly;
        int urx = line_tmp.plotline.urx;
        int ury = line_tmp.plotline.ury;

        fout<<"LINE = line(["<< llx <<" "<< urx <<"],["<< lly <<" "<< ury <<"],'Color','"<<line_tmp.color<<"');"<<endl;
        fout<<"set(LINE, 'linewidth', 3)"<<endl;
    }
    if(output_jpg == true)
    {
        string fileName_temp = "";
        for(int i = (int)fileName.size() - 3; i >= 0; i--)
        {
            if(fileName[i] == '/')
            {
                break;
            }
            else
            {
                fileName_temp += fileName[i];
            }
        }
        reverse(fileName_temp.begin(), fileName_temp.end());
        fout<<"saveas(gcf,'"<<fileName_temp<<"','jpg');"<<endl;
    }
    fout.close();

}

void Plot_Row(map<double,Row*> &Row_y_index, string filename)
{
    vector<Boundary> for_plot ;
    for(map<double,Row*>::iterator iter = Row_y_index.begin() ; iter != Row_y_index.end() ; iter++)
    {
        Node* head = iter -> second -> head ;
        Node* tail = iter -> second -> tail ;
        Node* ptr = head -> next ;
        if(ptr != tail)
        {
            while(ptr != tail)
            {
                Boundary tmp ;
                map<double,Row*>::iterator iter_next = iter ;
                iter_next ++ ;
                tmp.llx = ptr -> rectangle.llx ;
                tmp.urx = ptr -> rectangle.urx ;
                tmp.lly = iter -> first ;
                tmp.ury = iter_next -> first ;
                for_plot.push_back(tmp);
                ptr = ptr -> next ;
            }
        }
    }
    Plot_rectangle(for_plot, filename) ;
}


void Plot_subG(vector<Macro_Group> &MacroGroupBySC, int DH_num)
{
    cout << "   Write SubG.m..." << endl;
    string filename = "./output/Group/SubG.m";
    ofstream fout(filename.c_str()) ;


    fout <<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(unsigned int i = 0; i < MacroGroupBySC.size(); i++)
    {
        Macro_Group &mg_tmp = MacroGroupBySC[i];
        string hier_name = "";
        for(int j = 0; j < DH_num; j++)
        {
            if(j == 0)
                hier_name += mg_tmp.HierName[j];
            else
                hier_name = hier_name + "/" + mg_tmp.HierName[j];
        }

        int G_X = mg_tmp.GCenter_X;
        int G_Y = mg_tmp.GCenter_Y;

        fout<<"plot(" <<G_X<< ", " <<G_Y<< ",'r.','markersize',50);"<<endl;
        fout<<"text(" <<G_X<< ", " <<G_Y<< ",'G"<< i << "_" << hier_name <<"');"<<endl;
    }
}

void Plot_Group(vector<dataflow_group> &DataFlow_Group)
{
    cout << "   Write Group.m..." << endl;
    string filename = "./output/Group/Group.m" ;
    ofstream fout(filename.c_str()) ;


    fout<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    //chip boundary
    fout << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout << "fill(block_x, block_y, 'w');" << endl;

    for(unsigned int i = 0; i < DataFlow_Group.size(); i++)
    {
        dataflow_group &dfg_tmp = DataFlow_Group[i];

        int G_X = dfg_tmp.G_X;
        int G_Y = dfg_tmp.G_Y;

        fout<<"plot(" <<G_X<< ", " <<G_Y<< ",'r.','markersize',50);"<<endl;
        fout<<"text(" <<G_X<< ", " <<G_Y<< ",'G"<< i << "_" << dfg_tmp.heir_name <<"');"<<endl;
    }

}


// plot for "cell_placement.cpp"
void OutputTCL_withcell(string Filename, unsigned int STDXC, unsigned int STDYC)
{
    cout << "   Write " << Filename <<"..." << endl;
    ofstream fout(Filename.c_str());
    /// set_undoable_attribute [get_cells pa5645_dig/od_main_i/od_table_pool/od_lut6/S3_G/uBisted_RAM1R_m3d_84X40/RAM1R_m3d_84X40_u0/SRAM_i0] orientation {S}
    double space = PARAMETER._SPACING / (double)(2*PARA);
    for(unsigned int i = 0; i < macro_list.size(); i++)
    {
        Macro* macro_temp = macro_list[i];
        // for cells
        if(macro_temp-> macro_type == STD_CELL || macro_temp-> macro_type == MOVABLE_MACRO)
        {
            fout<< "set_cell_location -coordinates {"<< (float)macro_temp->lg.llx /PARA << " " << (float)macro_temp->lg.lly /PARA << "}" << " " << macro_temp-> macro_name <<endl;
        }
    }
    fout.close();
}

#ifndef PLOT_H_INCLUDED
#define PLOT_H_INCLUDED

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <climits>
using namespace std;
#include "structure.h"
#include "hier_group.h"     ///plus 108.05.20
#include "DataFlow.h"     ///plus 108.05.20
//#include "DensityMap.h"

/******plus 108.05.20********/
class Macro_Group;
class dataflow_group;
/***************************/

class PLOT_BLOCK
{
    public :
        Boundary plotrectangle;
        int int_TEXT;
        string string_TEXT;
        bool int_stringTEXT; /// true : int_TEXT   false : string_TEXT
        bool TEXT;/// true : output TEXT
        string color; /// k, c, b, y, g, w, r
};
class PLOT_LINE
{
    public :
        Boundary plotline;
        string color;
};
void Plot_global(bool STD_plot, string fileName) ;
void Plot_legal(bool STD_plot, string fileName) ;
void Plot_rectangle(vector<Boundary> plot_boundary, string fileName) ;
void Plot_rectangle_w_ID(vector<pair<string,Boundary> > plot_boundary, string fileName);
void Plot_rectangle_w_color(vector<pair<int,Boundary> > plot_boundary, string fileName);
void PlotMacroClusterSet(vector<Macro*> MacroClusterSet, string filename);
void PlotGlobalMacroClusterSet(vector<Macro*> MacroClusterSet, string fileName);
void PlotMatlab(vector<PLOT_BLOCK> plot_block, vector<PLOT_LINE> plot_line, bool Figure_visible, bool output_jpg, string fileName);
void Plot_Row(map<double,Row*> &Row_y_index, string filename);

/*****plus 108.05.20*****/
void Plot_subG(vector<Macro_Group> &MacroGroupBySC, int DH_num);
void Plot_Group(vector<dataflow_group> &DataFlow_Group);
/***********************/

/// ADD 2021.07
void PlotMacroClusterSet2(vector<pair<int, int> > PRegion_Center_Point, vector<Macro*> MacroClusterSet, string filename);
//////

#endif // PLOT_H_INCLUDED

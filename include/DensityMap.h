#ifndef DENSITYMAP_H_INCLUDED
#define DENSITYMAP_H_INCLUDED

#include "structure.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <limits>


using namespace std;
class Bin
{
    public:
        float cap;
        float demand;
        Boundary rectangle;
        Bin()
        {
            cap = demand = 0;
        }

};

class AreaTable
{
    public :
        float PreplaceArea;
        float TotalBlank;
        int cutline;
        AreaTable()
        {
            PreplaceArea = TotalBlank = cutline = 0;
        }
};
void ConstructBin(vector<vector<Bin> > &BinArray, double &AvgMacroLength);
void AddPreplaced(vector<vector<Bin> > &BinArray, double &AvgMacroLength);
void AddInfoToBinArray(Boundary &rectangle, bool Inflat, vector<vector<Bin> > &BinArray, double &AvgMacroLength);
int Calculate_CutLine(Boundary &region_box, bool Hori_Cut, vector<vector<Bin> > &BinArray, double &AvgMacroLength, float &AreaConstraint);
void Area_Table(Boundary &region_box, bool Hori_Cut, vector<vector<Bin> > &BinArray, double &AvgMacroLength, map<float, AreaTable> &Area_Line);
void PlotDensityMap(string filename, vector<vector<Bin> > &BinArray);
#endif // DENSITYMAP_H_INCLUDED

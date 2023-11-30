#ifndef FLAT_APPROACH_H_INCLUDED
#define FLAT_APPROACH_H_INCLUDED
#include "structure.h"
#include "corner_stitching.h"
#include "DensityMap.h"
#include "macro_legalizer.h"
#include "DumpFile.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <vector>
#include <cfloat>

class Flat_Partition_Region
{
    public:
        Boundary rectangle;
        float demand;
        float cap;
        float preplace_area;
        vector<Macro*> macro_info;
        Flat_Partition_Region()
        {
            demand = cap = preplace_area = 0;
        }
};

class FLAT_Legalization_INFO
{
    public:
    vector<Macro*> MacroClusterSet;
    vector<CornerNode*> AllCornerNode;

    /// refinement place ordering of macros
    vector<Macro*> Macro_Ordering;

};

void Flat_Approach();
void Flat_Partition(vector<Flat_Partition_Region> &Final_region);
void Update_Macro_Info(vector<Flat_Partition_Region> &Final_region, vector<Macro*> &Macro_Set);
void Flat_Legal(Boundary &subregion, Macro* macro_temp, vector<CornerNode*> &AllCornerNode);
void Flat_Simulated_Evolution(FLAT_Legalization_INFO &LG_INFO, int iter_count);
void Refinement_Legalization(FLAT_Legalization_INFO &LG_INFO, vector<Flat_Partition_Region> &Final_region);
bool cmp_x(Macro* a, Macro* b);
bool cmp_y(Macro* a, Macro* b);
bool cmp_place_order(Macro* a, Macro* b);

#endif // FLAT_APPROACH_H_INCLUDED

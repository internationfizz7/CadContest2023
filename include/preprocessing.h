#ifndef PREPROCESSING_H_INCLUDED
#define PREPROCESSING_H_INCLUDED

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <limits>
#include <fstream>
using namespace std;

#include "structure.h"
#include "plot.h"

void Call_preprocessing();
void Macro_outofbouundary_shifting();
void Check_overlap(bool GP_or_LG);
void Find_min_macro_height_width();
map<double,Row*> Block_merge(vector<Boundary> rectangle, Boundary rectangle_boundary);
void Calculate_placeable_area(float &preplaced_macro_blkage_area);
void Update_fixed_blkage();
void Bechmark_analysis();
void StdCenterCal();
void Update_fixed_blkage_Vert();
void MacroInflation();
void MacroInflation_new();
void Determine_Macro();
void clean_para_prepro();
#endif // PREPROCESSING_H_INCLUDED

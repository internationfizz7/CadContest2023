#include "DensityMap.h"

void ConstructBin(vector<vector<Bin> > &BinArray, double &AvgMacroLength)
{
    int nBincol, nBinrow;
    unsigned int totalMacroArea = 0;

    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        totalMacroArea += macro_list[macro_id]->w * macro_list[macro_id]->h;
    }
    AvgMacroLength = floor(sqrt(totalMacroArea / (double)MovableMacro_ID.size()));
//    AvgMacroLength *= 30;
//    cout<<"AvgMacroLength : "<<AvgMacroLength<<endl;
//    cout<<"chip_boundary.urx : "<<chip_boundary.urx<<endl;
    nBinrow = (int)ceil((chip_boundary.urx - chip_boundary.llx) / AvgMacroLength);
    nBincol = (int)ceil((chip_boundary.ury - chip_boundary.lly) / AvgMacroLength);
    BinArray.resize(nBinrow);
    for(int i = 0; i < nBinrow; i++)
    {
        BinArray[i].resize(nBincol);
    }

    /// update cap
    float BinArea = AvgMacroLength / (float)PARA * AvgMacroLength / (float)PARA;
    for(int i = 0; i < (int)BinArray.size(); i++)
    {
        int lx = i * AvgMacroLength;
        int rx = lx + AvgMacroLength;
        for(int j = 0; j < (int)BinArray[i].size(); j++)
        {
            int by = j * AvgMacroLength;
            int ty = by + AvgMacroLength;

            BinArray[i][j].rectangle.llx = lx;
            BinArray[i][j].rectangle.urx = rx;
            BinArray[i][j].rectangle.lly = by;
            BinArray[i][j].rectangle.ury = ty;
            BinArray[i][j].cap = BinArea;
        }
    }

    /// tuning right boundary
    BinArea = (chip_boundary.urx - (nBinrow - 1) * AvgMacroLength) / (float)PARA * AvgMacroLength / (float)PARA;
    for(int i = 0; i < nBincol; i++)
    {
        BinArray.back()[i].rectangle.urx = chip_boundary.urx;
        BinArray.back()[i].cap = BinArea;
    }

    /// tunong top boundary
    BinArea = (chip_boundary.ury - (nBincol - 1) * AvgMacroLength) / (float)PARA * AvgMacroLength / (float)PARA;
    for(int i = 0; i < nBinrow; i++)
    {
        BinArray[i].back().rectangle.ury = chip_boundary.ury;
        BinArray[i].back().cap = BinArea;
    }

    /// tuning right top Bin
    BinArea = (chip_boundary.urx - (nBinrow - 1) * AvgMacroLength) / (float)PARA * (chip_boundary.ury - (nBincol - 1) * AvgMacroLength) / (float)PARA;
    BinArray.back().back().cap = BinArea;

}



void AddPreplaced(vector<vector<Bin> > &BinArray, double &AvgMacroLength)
{
    for(int i = 0; i < (int)PreplacedMacro_ID.size(); i++)
    {
        int macro_id = PreplacedMacro_ID[i];

        Macro* macro_temp = macro_list[macro_id];
        if(macro_temp->macro_type != PRE_PLACED)
            continue;
        AddInfoToBinArray(macro_temp->gp, true, BinArray, AvgMacroLength);

    }
}



void AddInfoToBinArray(Boundary &rectangle, bool Inflat, vector<vector<Bin> > &BinArray, double &AvgMacroLength)
{
    int rectangleW = rectangle.urx - rectangle.llx;
    int rectangleH = rectangle.ury - rectangle.lly;
    int amplication_w = 0;
    int amplication_h = 0;
    if(Inflat)
    {
        double amplication = 0.3;
        amplication_w = rectangleW * amplication;
        amplication_h = rectangleH * amplication;
    }
    int lx = max(rectangle.llx - amplication_w, chip_boundary.llx);
    int rx = min(rectangle.urx + amplication_w, chip_boundary.urx);
    int ly = max(rectangle.lly - amplication_h, chip_boundary.lly);
    int ry = min(rectangle.ury + amplication_h, chip_boundary.ury);
    int rowL = lx / AvgMacroLength;
    int rowR = ceil((double)rx / AvgMacroLength);
    int colB = ly / AvgMacroLength;
    int colT = ceil((double)ry / AvgMacroLength);

    rowL = max(0, rowL);
    rowR = min((int)BinArray.size(), rowR);
    colB = max(0, colB);
    colT = min((int)BinArray[0].size(), colT);
    vector<int> occupyW;
    vector<int> occupyH;
    occupyW.resize(rowR - rowL);
    occupyH.resize(colT - colB);
    for(int j = 0; j < (int)occupyW.size(); j++)
    {
        occupyW[j] = min(rx, BinArray[rowL + j][0].rectangle.urx) - max(lx, BinArray[rowL + j][0].rectangle.llx);
    }
    for(int j = 0; j < (int)occupyH.size(); j++)
    {
        occupyH[j] = min(ry, BinArray[0][colB + j].rectangle.ury) - max(ly, BinArray[0][colB + j].rectangle.lly);
    }
    for(int j = rowL; j < rowR; j++)
    {
        for(int k = colB; k < colT; k++)
        {
            BinArray[j][k].demand += occupyW[j - rowL] / (float)PARA * occupyH[k - colB] / (float)PARA;
        }
    }
    occupyW.clear();
    occupyH.clear();
}

int Calculate_CutLine(Boundary &region_box, bool Hori_Cut, vector<vector<Bin> > &BinArray, double &AvgMacroLength, float &AreaConstraint)
{

    int lx = region_box.llx;
    int rx = region_box.urx;
    int ly = region_box.lly;
    int ry = region_box.ury;
    int rowL = lx / AvgMacroLength;
    int rowR = ceil((double)rx / AvgMacroLength);
    int colB = ly / AvgMacroLength;
    int colT = ceil((double)ry / AvgMacroLength);

    rowL = max(0, rowL);
    rowR = min((int)BinArray.size(), rowR);
    colB = max(0, colB);
    colT = min((int)BinArray[0].size(), colT);

//    cout<<"AvgMacroLength : "<<AvgMacroLength * BinArray.size()<<endl;
//    cout<<"rowL : "<<rowL<<" rowR : "<<rowR<<endl;
//    cout<<"colB : "<<colB<<" colT : "<<colT<<endl;
//    cout<<"row size : "<<BinArray.size()<<" col size : "<<BinArray[0].size()<<endl;

    float deviation = numeric_limits<float>::max();
    float BlankArea = 0;
    int cutline = 0;

    int iStart, iEnd, jStart, jEnd;
    if(Hori_Cut)
    {
//        cout<<"HORI CUT"<<endl;
        iStart = colB;
        iEnd = colT;
        jStart = rowL;
        jEnd = rowR;
    }
    else
    {
//        cout<<"VERT CUT"<<endl;
        iStart = rowL;
        iEnd = rowR;
        jStart = colB;
        jEnd = colT;
    }

    if(Hori_Cut)
    {
        for(int i = iStart; i < iEnd; i++)
        {

            float iArea = 0;
            for(int j = jStart; j < jEnd; j++)
            {
                Bin &bin_temp = BinArray[j][i];
                iArea += bin_temp.cap - bin_temp.demand;
            }
            float new_deviation = abs((int)(BlankArea + iArea - AreaConstraint));

            if(!(new_deviation > deviation))
            {
                deviation = new_deviation;
                BlankArea += iArea;
                cutline = i;
            }
            else
            {
                cutline = (cutline+ 1) * AvgMacroLength;
                AreaConstraint = BlankArea;
                break;
            }
        }
    }
    else
    {
        for(int i = iStart; i < iEnd; i++)
        {

            float iArea = 0;
            for(int j = jStart; j < jEnd; j++)
            {
                Bin &bin_temp = BinArray[i][j];
                iArea += bin_temp.cap - bin_temp.demand;
            }
            float new_deviation = abs((int)(BlankArea + iArea - AreaConstraint));

            if(!(new_deviation > deviation))
            {
                deviation = new_deviation;
                BlankArea += iArea;
                cutline = i;
            }
            else
            {
                cutline = (cutline+ 1) * AvgMacroLength;
                AreaConstraint = BlankArea;
                break;
            }
        }
    }
    return cutline;

}


void Area_Table(Boundary &region_box, bool Hori_Cut, vector<vector<Bin> > &BinArray, double &AvgMacroLength, map<float, AreaTable> &Area_Line)
{

    int lx = region_box.llx;
    int rx = region_box.urx;
    int ly = region_box.lly;
    int ry = region_box.ury;
    int rowL = lx / AvgMacroLength;
    int rowR = ceil((double)rx / AvgMacroLength);
    int colB = ly / AvgMacroLength;
    int colT = ceil((double)ry / AvgMacroLength);

    rowL = max(0, rowL);
    rowR = min((int)BinArray.size(), rowR);
    colB = max(0, colB);
    colT = min((int)BinArray[0].size(), colT);

//    cout<<"AvgMacroLength : "<<AvgMacroLength * BinArray.size()<<endl;
//    cout<<"utilization : "<<utilization<<endl;
//    cout<<"rowL : "<<rowL<<" rowR : "<<rowR<<endl;
//    cout<<"colB : "<<colB<<" colT : "<<colT<<endl;
//    cout<<"row size : "<<BinArray.size()<<" col size : "<<BinArray[0].size()<<endl;

//    if(utilization < 0.7)
//    {
//        utilization = 0.7;
//    }

    int iStart, iEnd, jStart, jEnd;
    if(Hori_Cut)
    {
//        cout<<"HORI CUT"<<endl;
        iStart = colB;
        iEnd = colT;
        jStart = rowL;
        jEnd = rowR;
    }
    else
    {
//        cout<<"VERT CUT"<<endl;
        iStart = rowL;
        iEnd = rowR;
        jStart = colB;
        jEnd = colT;
    }


    AreaTable area_tmp;

    float &BlankArea = area_tmp.TotalBlank;
    float &PreplacedArea = area_tmp.PreplaceArea;
    int &cutline = area_tmp.cutline;


    if(Hori_Cut)
    {
        for(int i = iStart; i < iEnd; i++)
        {

            float iArea = 0;
            float i_Pre_Area =0;
            for(int j = jStart; j < jEnd; j++)
            {
                Bin &bin_temp = BinArray[j][i];
                iArea += bin_temp.cap - bin_temp.demand;
                i_Pre_Area += bin_temp.demand;
            }
/**********************109.07**************************/
            PreplacedArea += i_Pre_Area; //PreplacedArea must be accumulated even if iArea(blank) is zero in this row
            BlankArea += iArea;
            if(iArea < 1) //iArea(blank) is zero, BlankArea didn't change
            {
                if(i==iEnd-1) //if this is the last row of the region, even if iArea of this row is zero, it can't be continued since Area_Line need to record all preplaced information
                {
                    BlankArea += 1; //Cause the iArea of this row is zero, the first key of the Area_Line equals to this BlankArea has already exists, so plus 1 to make them different
                }
                else
                    continue;
            }
            //if(iArea < 1)
                //continue;
/******************************************************/

            cutline = (i + 1) * AvgMacroLength;
            AreaTable table = area_tmp;
            Area_Line.insert(make_pair(BlankArea, table));
        }
    }
    else
    {
        for(int i = iStart; i < iEnd; i++)
        {

            float iArea = 0;
            float i_Pre_Area =0;
            for(int j = jStart; j < jEnd; j++)
            {
                Bin &bin_temp = BinArray[i][j];
                iArea += bin_temp.cap - bin_temp.demand;
                i_Pre_Area += bin_temp.demand;
            }
            PreplacedArea += i_Pre_Area;
            BlankArea += iArea;
/*********************109.07***********************/
            if(iArea < 1)
            {
                if(i==iEnd-1)
                {
                    BlankArea += 1;
                }
                else
                    continue;
            }
            //if(iArea < 1)
                //continue;
/**************************************************/

            cutline = (i + 1) * AvgMacroLength;
            AreaTable table = area_tmp;
            Area_Line.insert(make_pair(BlankArea, table));
        }
    }

    return;
}


///plot DensityMap
void PlotDensityMap(string filename, vector<vector<Bin> > &BinArray)
{
    ofstream fout(filename.c_str());
    fout<<"COLOR black"<<endl;
    //chip boundary
    fout << "SRF " << chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury <<endl;

    for(int i = 0; i < (int)BinArray.size(); i++)
    {
        for(int j = 0; j < (int)BinArray[i].size(); j++)
        {
            int llx = BinArray[i][j].rectangle.llx;
            int urx = BinArray[i][j].rectangle.urx;
            int lly = BinArray[i][j].rectangle.lly;
            int ury = BinArray[i][j].rectangle.ury;;
            float utilization = BinArray[i][j].demand / BinArray[i][j].cap;
            string color;
            if(utilization > 0.6)
            {
                color = "red";
            }
            else
            {
                color = "blue";
            }

            fout<<"COLOR "<<color<<endl;
            fout << "SRF " << llx << " -" << lly << " " << urx << " -" << ury << endl;
        }
    }
    fout.close();
}


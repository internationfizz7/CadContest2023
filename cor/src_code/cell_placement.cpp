
#include "cell_placement.h"

#define CLUSTER_SIZE 1000                       // cluster 1000 stdandard cells for one cluster
#define BIG_STD 4                               // "big_STDGroup" store the cluster which area is bigger than a quarter of the maximum cluster
#define NUM_THREADS 10000                       // define maximun multi-thread can be used
#define SWAP(x,y) {int t; t = x; x = y; y = t;} // SWAP() is used for quicksort() function
vector<STD_Group*> STDGroupBy2;                 // new cell clusters

void CELLPLACEMENT::CellPlacementFlow(CELLPLACEMENT *cellpl, Legalization_INFO &LG_INFO_tmp, int iter)
{
/*** Step 1-4 deal with the cell clusters in region ***/

    // 1.Cluster cells according GP coordinate
    CELLPLACEMENT::CellClustering(cellpl);

    // 2.Allocate cell cluster to a partition region
    CELLPLACEMENT::PartitionCellCluster(cellpl, LG_INFO_tmp.PRegion);

    // 3.Spare cell cluster by QP
    //CELLPLACEMENT::SpareCluster(cellpl);

    // 4.Legail cell cluster
    CELLPLACEMENT::LegailCellCluster(cellpl);

    // Plot cell cluster graph to matlab
    if(Debug_PARA)
        CELLPLACEMENT::PlotCellCluster(cellpl, iter);

/*** Step 5-6 deal with the cells in cluster ***/

    // 5.QP spread the cell cluster in the bin
    //CELLPLACEMENT::SpareCell(cellpl);

    // Plot QP result to ICC
    if(Debug_PARA)
        OutputTCL_withcell("./output/Cell_graph/Cell_QP.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);

    // 6.Packing cells to the bin
    CELLPLACEMENT::PackingCell(cellpl);

    // Plot cell placement result to ICC
    if(Debug_PARA)
        OutputTCL_withcell("./output/Cell_graph/Cell_Legal.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);


}
// [NOTE] the small_stdGroup is ignore in spaare cell cluster and to legal cell

void CELLPLACEMENT::CellClustering(CELLPLACEMENT *cellpl)
{
    cout << "   Now run Cluster Cell Groups" << endl;
    int chipW = BenchInfo.chip_W;
    int chipH = BenchInfo.chip_H;

    // "Radio" is the expand area from cell area to all chip area, it also the radio to expand of grid number!
    float Radio = BenchInfo.chip_area / BenchInfo.std_cell_area;
    // cell placement area need to spilt to the number of "macro_list.size() / CLUSTER_SIZE"
    int ngrid_obj = macro_list.size() / CLUSTER_SIZE * Radio;
    //ngrid_obj = 7000 will br faster // for PA5663

    // step 1 cut chip area to  approximate the grid number of the "ngrid_obj"
    int nRow = 1, nCol = 1;
    while(nRow * nCol < ngrid_obj)
    {
        // cut chip vertical or horizontal let the grid W is similar than H
        if( fabs( chipH/(nRow+1) - chipW/nCol ) < fabs( chipH/nRow - chipW/(nCol+1)) )
            nRow++;
        else
            nCol++;
    }

    // step 2 setup "grid[][]" which can store the cell by the cell's GP location
    vector< vector<BIN*> > grid;
    // [NOTE] grid area must bigger then the placement region to store cells in "STDGroupBy2"
    int gridW = ceil((float)chipW / nCol);
    int gridH = ceil((float)chipH / nRow);
    grid.resize(nRow);
    for(unsigned int i = 0; i < grid.size() ; i++)
    {
        grid[i].resize(nCol);
        for(unsigned int j = 0; j < grid[i].size(); j++)
        {
            grid[i][j]           = new BIN();
            grid[i][j]-> llx     = gridW *  j;
            grid[i][j]-> urx     = gridW * (j +1);
            grid[i][j]-> lly     = gridH *  i;
            grid[i][j]-> ury     = gridH * (i +1);
            grid[i][j]-> centX   = gridW * (float)(j +0.5);
            grid[i][j]-> centY   = gridH * (float)(i +0.5);
        }
    }
    /*
    // for all grid position
    for(unsigned int i = 0; i < grid.size() ; i++)
    {
        for(unsigned int j = 0; j < grid[i].size(); j++)
            cout << grid[i][j]-> llx << " " << grid[i][j]-> lly << endl;
    }*/


    // setp 3 allocate all cells into "grid[][]"
    for(unsigned int i = 0; i < macro_list.size() ; i++)
    {
        Macro* cell_tmp = macro_list[i];
        // skip the cell if it is dummy cell (without any net)
        if(cell_tmp-> macro_type == STD_CELL && cell_tmp-> NetID.size() != 0)
        {
            int row     = cell_tmp-> gp.lly / gridH;
            int column  = cell_tmp-> gp.llx / gridW;
            // modify the cell position is out of chip
            if(row      > nRow-1) row       = nRow -1;
            if(column   > nCol-1) column    = nCol -1;
            if(row      <  0)     row       = 0;
            if(column   <  0)     column    = 0;

            grid[row][column]-> packing_std.push_back(i);
        }
    }
    /*
    // amount of cells in the grids
    for(unsigned int i = 0; i < grid.size() ; i++)
    {
        for(unsigned int j = 0; j < grid[i].size(); j++)
            cout << grid[i][j]-> packing_std.size() << endl;
    }*/

    STDGroupBy2.resize(0);
    // step 3 input all cluster to "STDGroupBy2" from "grid[][]"
    for(unsigned int i = 0; i< nRow * nCol ; i++)
    {
        // map the group index to grid's row and cloumn
        int row = i / nCol;
        int column = i % nCol;
        BIN* thisGrid = grid[row][column];

        // skip the grid if the grid without any cell
        if(thisGrid-> packing_std.size() == 0)
            continue;

        STD_Group* STDgroup_tmp= new STD_Group();
        int ClusterID = STDGroupBy2.size();
        float total_w = 0, total_h = 0, total_area = 0;

        for(unsigned int j = 0; j < thisGrid-> packing_std.size() ; j++)
        {
            int cellID = thisGrid-> packing_std[j];
            Macro* thisCell = macro_list[cellID];
            // Update "group2_id" info.
            thisCell-> group2_id = ClusterID;

            total_w     += thisCell-> real_w;
            total_h     += thisCell-> real_h;
            total_area  += thisCell-> area;
            STDgroup_tmp-> member_ID.push_back(cellID);

            // input "NetID_to_weight" info.
            for(unsigned int k = 0; k < thisCell-> NetID.size() ; k++)
            {
                int netID = thisCell-> NetID[k];
                map<int, int>::iterator itermap;
                itermap = STDgroup_tmp-> NetID_to_weight.find(netID);
                // this net is exist in the cluster or not
                if(itermap == STDgroup_tmp-> NetID_to_weight.end())
                {
                    STDgroup_tmp-> NetID_to_weight[netID] = 1;
                    net_list[netID].group2_idSet.push_back(i);
                }
                else
                    STDgroup_tmp-> NetID_to_weight[netID] += 1;
            }
        }
        STDgroup_tmp-> ID           = ClusterID;
        STDgroup_tmp-> Total_W      = total_w;
        STDgroup_tmp-> Avg_W        = total_w / thisGrid-> packing_std.size();
        STDgroup_tmp-> Avg_H        = total_h / thisGrid-> packing_std.size();
        STDgroup_tmp-> TotalArea    = total_area;
        STDgroup_tmp-> GPCenter_X    = thisGrid-> centX;
        STDgroup_tmp-> GPCenter_Y    = thisGrid-> centY;
        STDgroup_tmp-> GCenter_X    = thisGrid-> centX;
        STDgroup_tmp-> GCenter_Y    = thisGrid-> centY;
        STDgroup_tmp-> Width        = sqrt(total_area* PARA* PARA);
        STDgroup_tmp-> Height       = sqrt(total_area* PARA* PARA);

        STDGroupBy2.push_back(STDgroup_tmp);
    }

    cout << "[INFO] Number of the cell cluster in clustering stage: " << STDGroupBy2.size() << endl;
}

void CELLPLACEMENT::PartitionCellCluster(CELLPLACEMENT *cellpl, vector<Partition_Region> &P_Region_real)
{

    // "P_Region" is a set of placement region after partition stage. you can see in output/subregion_graph/Region.m

    /*
    cout << "\n[Debug] Num of pattition region: " << P_Region.size() << endl;

    for(unsigned int i = 0 ; i< P_Region.size(); i++)
    {
        cout << "[Debug] "  << P_Region.at(i).rectangle.llx << " " << P_Region.at(i).rectangle.lly << " "
                            << P_Region.at(i).rectangle.urx << " " << P_Region.at(i).rectangle.ury << endl;
    }
    */

    // * Now we do not devide the region, thus the region is only one !!!
    Boundary blank_box = Blank_BBOX(chip_boundary, PreplacedInCornerStitching);
    vector<Partition_Region> P_Region;
    P_Region.resize(1);
    P_Region.at(0).rectangle.llx = blank_box.llx;
    P_Region.at(0).rectangle.lly = blank_box.lly;
    P_Region.at(0).rectangle.urx = blank_box.urx;
    P_Region.at(0).rectangle.ury = blank_box.ury;


    // Copy "P_Region" to "Regions"
    REGION *region_tmp;
    cellpl-> Regions.resize(P_Region.size());
    for(unsigned int i = 0 ; i< P_Region.size(); i++)
    {
        region_tmp = new REGION();
        region_tmp-> P_RegionID = i;
        region_tmp-> llx = P_Region[i].rectangle.llx;
        region_tmp-> lly = P_Region[i].rectangle.lly;
        region_tmp-> urx = P_Region[i].rectangle.urx;
        region_tmp-> ury = P_Region[i].rectangle.ury;
        cellpl-> Regions[i] = region_tmp;
    }

    // find the maximum STD group store in "max_StdArea"
    float max_StdArea = 0;
    for(unsigned int i = 0; i < STDGroupBy2.size(); i++)
    {
        if(STDGroupBy2[i]-> TotalArea > max_StdArea)
            max_StdArea = STDGroupBy2[i]-> TotalArea;
    }


    // Allocate all groups to a partition region accroding their center position
    cout << "   Now run Allocate Cell Groups to the region" << endl;
    int P_centerX, P_centerY;
    int distance = 0;
    for(unsigned int i = 0; i < STDGroupBy2.size(); i++)
    {
        STDGroupBy2[i]-> P_regionID = -1; // initial value is -1
        for(unsigned int j = 0 ; j< P_Region.size(); j++)
        {
            // if their center position is inside the partition region then store in the P_regionID
            if(STDGroupBy2[i]-> GCenter_X >= P_Region[j].rectangle.llx &&
               STDGroupBy2[i]-> GCenter_X <= P_Region[j].rectangle.urx &&
               STDGroupBy2[i]-> GCenter_Y >= P_Region[j].rectangle.lly &&
               STDGroupBy2[i]-> GCenter_Y <= P_Region[j].rectangle.ury)
               {
                   //cout << "   assign P_regionID by INSIDE pos," << endl;
                   STDGroupBy2[i]-> P_regionID = j;
                   // "BIG_STD" is define bigger then (max cluster /BIG_STD)
                   if(STDGroupBy2[i]-> TotalArea > max_StdArea / BIG_STD)
                        cellpl-> Regions[j]-> big_STDGroupID.push_back(i);
                   else
                        cellpl-> Regions[j]-> small_STDGroupID.push_back(i);
                   break;
               }
        }

        int min_Distance = INT_MAX;
        int ReginID_tmp = -1;
        // if cell group postiton doesn't in P_Regin
        if(STDGroupBy2[i]-> P_regionID == -1)
        {
            for(unsigned int j = 0 ; j< P_Region.size(); j++)
            {
                P_centerX = (P_Region[j].rectangle.urx + P_Region[j].rectangle.llx) /2;
                P_centerY = (P_Region[j].rectangle.ury + P_Region[j].rectangle.lly) /2;
                distance = fabs(STDGroupBy2[i]-> GCenter_X - P_centerX ) +
                           fabs(STDGroupBy2[i]-> GCenter_Y - P_centerY );
                // allocate to the P_Region with minimum distance accroding HPWL
                if(distance < min_Distance)
                {
                    min_Distance = distance;
                    ReginID_tmp = j;
                }
            }
            //cout << "   assign P_regionID by MINIMUM dis." << endl;
            STDGroupBy2[i]-> P_regionID = ReginID_tmp;

            // classify the cluster is "big_STDGroup" or not
            if(STDGroupBy2[i]-> TotalArea > max_StdArea / BIG_STD)
                cellpl-> Regions[ReginID_tmp]-> big_STDGroupID.push_back(i);
            else
                cellpl-> Regions[ReginID_tmp]-> small_STDGroupID.push_back(i);
        }
    }

    /*
    float damand[P_Region.size()];
    int Pregion_ID;

    // calcurate damand for P_Region

    for(unsigned int i = 0 ; i< STDGroupBy2.size(); i++)
    {
        Pregion_ID = STDGroupBy2[i]-> P_regionID;
        damand[Pregion_ID] += STDGroupBy2[i]-> TotalArea;
    }

    for(unsigned int i = 0 ; i< P_Region.size();i++)
    {
        cout << "[Debug] P_Region id: " << i << "\tcapacity: "  << P_Region[i].cap
             << "\tdamand: "   << utilization[i]   << endl;
    }
    */
}

void CELLPLACEMENT::SpareCluster(CELLPLACEMENT *cellpl)
{
    pthread_t threads[NUM_THREADS];
    int rc, t_num = 0;

    cout << "   Now run QP to Spread Clusters " << endl;
    // run QP for each bin in the region with cell group
    for(unsigned int i = 0; i< cellpl-> Regions.size(); i++)
    {
        REGION *this_region = cellpl-> Regions[i];
        if(this_region-> big_STDGroupID.size() > 0 /*|| this_region-> small_STDGroupID.size() > 0*/){
            // multi-thread to run QP // [NOTE] debug do not used
            rc = pthread_create(&threads[t_num], NULL, QPSpreadCluster, (void *)this_region);
            //QPSpreadCluster( (void *)this_region );
            if(rc == 1){
                cout << "[Error] unable to create multi-thread " << rc << endl; exit(-1);
            }
            t_num++;
        }
    }
    // thread join
    for(int m = 0; m < t_num ; m++)
        pthread_join(threads[m], NULL);
}


// [NOTE] this CODE is written by SZU (GP.cpp/.h) and modify to our data structure
void* CELLPLACEMENT::QPSpreadCluster(void* vregion)
{
    CELLPLACEMENT::REGION* region = (CELLPLACEMENT::REGION*)vregion;

    // step 1 "mapNet" is remove same nets between the packing STD Groups of the region
    int countNet = 0;
    // [NOTE] mapnet is input the inside nets of the region and its weight is connected cells in the region
    map<int, int> mapNet;

    map<int,bool> HasNetorNot; // store the cell cluster and if its nets has outside nets or not.

    // for all cell cluster of the region
    for( unsigned int i = 0; i < region-> big_STDGroupID.size(); i++ ) // NOTE: big std groups in the region
    {
        int std_id = region-> big_STDGroupID[i];
        STD_Group* std_group = STDGroupBy2[std_id];

        int HasNets = false; // the cell cluster has outside nets
        map<int, int>::iterator itermap;
        // for all net in the cluster
        for(itermap = std_group-> NetID_to_weight.begin(); itermap != std_group-> NetID_to_weight.end(); itermap++)
        {
            int netid = itermap-> first;
            int nCell = itermap-> second; // connected cell in cluster
            int netdegree = net_list[netid].macro_idSet.size() + net_list[netid].cell_idSet.size();
            if(nCell == netdegree)
                continue;
            HasNets = true;
            map<int, int>::iterator itermap2;
            itermap2 = mapNet.find(netid);
            // case 1 the net is not exist in "mapNet"
            if(itermap2 == mapNet.end())
                mapNet[netid] = nCell; // "net weight" is the number of cell is connect to the net in this cluster
            // case 2 the net is exist in "mapNet"
            else
                //[NOTE] the net connected cells in these groups  are independent
                mapNet[netid] = itermap2-> second + nCell; // mapNet's net weight is the number of cell is connect to the net in this BIN
        }
        HasNetorNot[std_id] = HasNets;
    }
/*
    for( unsigned int i = 0; i < region-> small_STDGroupID.size(); i++ ) // NOTE: small std groups in the region
    {
        int std_id = region-> small_STDGroupID[i];
        STD_Group* std_group = STDGroupBy2[std_id];

        int HasNets = false; // the cell cluster has outside nets
        map<int, int>::iterator itermap;
        // for all net in the cluster
        for(itermap = std_group-> NetID_to_weight.begin(); itermap != std_group-> NetID_to_weight.end(); itermap++)
        {
            int netid = itermap-> first;
            int nCell = itermap-> second; // connected cell in cluster
            int netdegree = net_list[netid].macro_idSet.size() + net_list[netid].cell_idSet.size();
            if(nCell == netdegree)
                continue;
            HasNets = true;
            map<int, int>::iterator itermap2;
            itermap2 = mapNet.find(netid);
            // case 1 the net is not exist in "mapNet"
            if(itermap2 == mapNet.end())
                mapNet[netid] = nCell; // "net weight" is the number of cell is connect to the net in this cluster
            // case 2 the net is exist in "mapNet"
            else
                //[NOTE] the net connected cells in these groups  are independent
                mapNet[netid] = itermap2-> second + nCell; // mapNet's net weight is the number of cell is connect to the net in this BIN
        }
        HasNetorNot[std_id] = HasNets;
    }
*/
    countNet = mapNet.size();

    // step 2 "mapMod" is store all cluster in the region and map to a index
    int countMod = 0;
    map<int, int> mapMod;
    // for all cluster of the region
    for( unsigned int i = 0; i < region-> big_STDGroupID.size(); i++ ) // NOTE: big std group in the region
    {
        int std_id = region-> big_STDGroupID[i];
        if(HasNetorNot[std_id] == false)
            continue;
        mapMod[std_id] = countMod; // "countMod" is equal to "modID" map a cell id to a index which can assign value to matrix's column in below
        countMod++;
    }
/*
    for( unsigned int i = 0; i < region-> small_STDGroupID.size(); i++ ) // NOTE: small std group in the region
    {
        int std_id = region-> small_STDGroupID[i];
        if(HasNetorNot[std_id] == false)
             { cout << "DDDDDD"<<endl; continue;}
        mapMod[std_id] = countMod; // "countMod" is equal to "modID" map a cell id to a index which can assign value to matrix's column in below
        countMod++;
    }
*/
    int sizeQ = countMod + countNet;

    cout << "number of cluster: "   << countMod << endl;
    cout << "number of nets: "      << countNet << endl;

    // step 3 olve X direction and Y direction
    for(int modeXY = 0; modeXY <= 1; modeXY++ )
    {

        vector<CELLPLACEMENT::QP> matrixQ;
        matrixQ.resize(sizeQ);

        float *vectorB = new float [countNet];
        for( int i = 0; i < countNet; i++ )
        {
            vectorB[i] = 0;
        }

        CreateQmBv2( matrixQ, vectorB, region, modeXY, mapNet, mapMod ); // function : CreateQmBv2() is in below..

        /* LASPack solve QP*/

        QMatrix Qm;
        Vector Bv, Xv;

        // step 3-1 construct Qm
        Q_Constr( &Qm, "Qm", sizeQ, True, Rowws, Normal, True );
        for( int i = 0; i < sizeQ; i++ )
        {
            int entryNum = 0;
            for( unsigned int j = 0; j < matrixQ[i].connect.size(); j++ )
            {
                if( matrixQ[i].connect[j].col >= i ) entryNum++;
            }
            Q_SetLen( &Qm, i+1, entryNum );

            entryNum = 0;
            for( unsigned int j = 0; j < matrixQ[i].connect.size(); j++ )
            {
                if( matrixQ[i].connect[j].col >= i )
                {
                    Q_SetEntry( &Qm, i+1, entryNum, matrixQ[i].connect[j].col+1, matrixQ[i].connect[j].weight );
                    entryNum++;
                }
            }
        }
        // step 3-2 construct Xv
        V_Constr( &Xv, "Xv", sizeQ, Normal, True );

        if(modeXY == 0)         ///< set initial value for x
            V_SetAllCmp( &Xv, 0.5 * ((float)region-> llx + (float)region-> urx) / PARA );
        else                    ///< set initial value for y
            V_SetAllCmp( &Xv, 0.5 * ((float)region-> lly + (float)region-> ury) / PARA );

        // step 3-3 construct Bv
        V_Constr( &Bv, "Bv", sizeQ, Normal, True );

        for( int i = 0; i < sizeQ; i++ )
        {
            if( i >= countMod )
                V_SetCmp( &Bv, i+1, vectorB[i-countMod] );
            else
                V_SetCmp( &Bv, i+1, 0.0 );
        }

        // step 3-4 solve Xv using "laspack"
        SetRTCAccuracy(1e-9);

        CGIter( &Qm , &Xv , &Bv , 100 , SSORPrecond , 1.0 );

        // step 3-5 output value
        for( unsigned int i = 0; i < region-> big_STDGroupID.size(); i++ ) // NOTE: big std groups in the region
        {
            int std_id = region-> big_STDGroupID[i];
            STD_Group* std_group = STDGroupBy2[std_id];

            int modID = mapMod[std_id];
            if( modeXY == 0 )
                std_group-> GCenter_X = (int)(V_GetCmp(&Xv, modID + 1) * PARA);
            else
                std_group-> GCenter_Y = (int)(V_GetCmp(&Xv, modID + 1) * PARA);
        }
/*
        for( unsigned int i = 0; i < region-> small_STDGroupID.size(); i++ ) // NOTE: small std group in the region
        {
            int std_id = region-> small_STDGroupID[i];
            STD_Group* std_group = STDGroupBy2[std_id];

            int modID = mapMod[std_id];
            if( modeXY == 0 )
                std_group-> GCenter_X = (int)(V_GetCmp(&Xv, modID + 1) * PARA);
            else
                std_group-> GCenter_Y = (int)(V_GetCmp(&Xv, modID + 1) * PARA);
        }
*/
        Q_Destr( &Qm );
        V_Destr( &Bv );
        V_Destr( &Xv );

        // LASPack method end

        // step 3-6 clear Qm Bv Xv
        for( unsigned int i = 0; i < matrixQ.size(); i++ )
        {
            matrixQ[i].connect.clear();
        }
        matrixQ.clear();

        vector<QP> (matrixQ).swap(matrixQ);

        delete [] vectorB;
    }

    pthread_exit(NULL);
}

// construct Qm/ Bv vector for QP
void CELLPLACEMENT::CreateQmBv2( vector<CELLPLACEMENT::QP> &matrixQ, float *vectorB,CELLPLACEMENT::REGION* region, int modeXY, map<int, int> &mapNet, map<int, int> &mapMod )
{
    int numNet = 0; // number of the net index in mapNet

    map<int, int>::iterator itermap;
    // for all net in the region
    for(itermap = mapNet.begin(); itermap != mapNet.end(); itermap++)
    {
        int netID = itermap-> first;
        int npin = net_list[netID].macro_idSet.size() + net_list[netID].cell_idSet.size();  // number of the pin of the net is connected

        // creat starNode and push back into Qm
        int nMod = (int)mapMod.size();
        int starNodeID = nMod + numNet;

        float pinWeight = (float)1 / npin;

        CELLPLACEMENT::WEIGHT starNode;    // start net module will creat a pseude node "starNode"
        starNode.col = starNodeID;
        starNode.weight = pinWeight * (float)npin; // starNode.weight = 1

        // starNode would add into Q matrix from (nMod) to (nMod + nNet)
        matrixQ[starNodeID].connect.push_back(starNode);

        // set the value of vector Bv and Qm

        // 1. for all pin of the net in "macro_idSet"
        for(unsigned int i = 0; i < net_list[netID].macro_idSet.size(); i++)
        {
            int macro_id = net_list[netID].macro_idSet[i];
            Macro* macro_temp = macro_list[macro_id];

            // find the pin of the macro is connect to the net
            int shiftx = 0, shifty = 0;
            map<int, vector<int> >::iterator itermap2;
            itermap2 = macro_temp-> NetsID_to_pinsID.find(netID);

            if(itermap2 == macro_temp-> NetsID_to_pinsID.end())
            {
                //cout << "[WARNING] Can not find pin of the net "<< netID <<" in macro "<< macro_id << endl;
                shiftx = macro_temp-> real_w /2;
                shifty = macro_temp-> real_h /2;
            }
            else
            {
                // Calcurate the average pin position
                int numpin = itermap2-> second.size();
                for(int j=0; j < numpin; j++)
                {
                    int pin_id = itermap2-> second.at(j);
                    shiftx += pin_list[pin_id]-> x_shift_plus;
                    shifty += pin_list[pin_id]-> y_shift_plus;
                }
                if(numpin != 0)
                {
                    shiftx /= numpin;
                    shifty /= numpin;
                }
                else
                {
                    shiftx + macro_temp-> real_w /2;
                    shifty + macro_temp-> real_h /2;
                }
            }

            // All the macro's pin is connect to "fixed pin" : create "vectorB" by the X/Y position
            if(modeXY == 0) // for x
            {
                int pin_x = macro_temp-> lg.llx + shiftx;
                vectorB[numNet] += pinWeight * (float)pin_x / PARA;
            }
            else // for y
            {
                int pin_y = macro_temp-> lg.lly + shifty;
                vectorB[numNet] += pinWeight * (float)pin_y / PARA;
            }

        }
        // 2. for all pin of the net in "cell_idSet"
        for(unsigned int i = 0; i < net_list[netID].cell_idSet.size(); i++)
        {
            int cell_id = net_list[netID].cell_idSet[i];
            Macro* cell_temp = macro_list[cell_id];

            // find the cell is in the region's cluster or not
            bool inside = false;
            int group_id = -1;

            for(unsigned int j =0; j < region-> big_STDGroupID.size() ; j++)
            {
                int std_group_id = region-> big_STDGroupID[j];

                STD_Group* std_group = STDGroupBy2[std_group_id];
                for(unsigned int k =0; k < std_group-> member_ID.size(); k++)
                {
                    if(std_group-> member_ID[k] == cell_id)
                    {
                        inside = true;
                        group_id = std_group_id;
                    }
                }
            }/*
            for(unsigned int j =0; j < region-> small_STDGroupID.size() ; j++)
            {
                int std_group_id = region-> small_STDGroupID[j];

                STD_Group* std_group = STDGroupBy2[std_group_id];
                for(unsigned int k =0; k < std_group-> member_ID.size(); k++)
                {
                    if(std_group-> member_ID[k] == cell_id)
                    {
                        inside = true;
                        group_id = std_group_id;
                    }
                }
            }*/

            // case 1 if the cell is not inside, then the pin is connect to "fixed pin" : create "vectorB" by the X/Y position
            if(inside == false)
            {
                int std_group_id = cell_temp-> group2_id;
                if(modeXY == 0) // for x
                {
                    // outside cell postiton is fixed at the bin center
                    int pin_x = STDGroupBy2[std_group_id]-> GPCenter_X - cell_temp-> real_w /2;
                    vectorB[numNet] += pinWeight * (float)pin_x / PARA;
                }
                else // for y
                {
                    int pin_y = STDGroupBy2[std_group_id]-> GPCenter_Y - cell_temp-> real_h /2;
                    vectorB[numNet] += pinWeight * (float)pin_y / PARA;
                }
            }
            else // case 2 otherwise the pin is connect to "non-fixed pin" : create "matrixQ"
            {

                int modID = mapMod[group_id];       // modID is the Qm index from 0 to nMod (the number of group)

                CELLPLACEMENT::WEIGHT modNode;
                modNode.col = modID;
                modNode.weight = pinWeight;

                // 1. increase pinWeight to the matrixQ[modID]
                    // deal with diagonal
                bool flagHadAdd = false;
                for( unsigned int k = 0; k < matrixQ[modID].connect.size(); k++ )
                {
                    if(modID == matrixQ[modID].connect[k].col)
                    {
                        matrixQ[modID].connect[k].weight += pinWeight;
                        flagHadAdd = true;
                        break;
                    }
                }
                if(!flagHadAdd)
                {
                    matrixQ[modID].connect.push_back(modNode);
                }

                // 2. descrease pinWeight if the matrixQ[modID]
                    // deal with diagonal
                flagHadAdd = false;
                for( unsigned int k = 0; k < matrixQ[modID].connect.size(); k++ )
                {
                    if( starNodeID == matrixQ[modID].connect[k].col )
                    {
                        matrixQ[modID].connect[k].weight -= pinWeight;
                        flagHadAdd = true;
                        break;
                    }
                }
                if( !flagHadAdd )
                {
                    starNode.weight = -pinWeight;
                    matrixQ[modID].connect.push_back(starNode);
                }
                    // deal with dutton of the matrix
                modNode.weight = -pinWeight;
                matrixQ[starNodeID].connect.push_back(modNode);
            }
        }
        numNet ++;
    }
}

void CELLPLACEMENT::LegailCellCluster(CELLPLACEMENT *cellpl)
{
    cout << "   Now run Cell Cluster Legalization " << endl;

    int CornerNode_ID;
    // "FixedPreplacedInCornerStitching" is store "PreplacedInCornerStitching" ID for blockage
    /*
    cout << "\n[DEBUG] Number of FixedPreplaced: " << FixedPreplacedInCornerStitching.size() << endl;

    for(int i = 0; i < (int)FixedPreplacedInCornerStitching.size(); i++)
    {
        CornerNode_ID = FixedPreplacedInCornerStitching[i];
        CornerNode* corner_temp = PreplacedInCornerStitching[CornerNode_ID];
        cout << corner_temp-> rectangle.llx << " " << corner_temp-> rectangle.lly << " "
             << corner_temp-> rectangle.urx << " " << corner_temp-> rectangle.ury << endl;
    }
    */

    // step 1 find preplaced area overlap area between region and preplaced
    Boundary overlap;
    for(int i = 0; i < (int)FixedPreplacedInCornerStitching.size(); i++)
    {
        CornerNode_ID = FixedPreplacedInCornerStitching[i];
        CornerNode* this_corner = PreplacedInCornerStitching[CornerNode_ID];

        for(unsigned int j = 0 ; j< cellpl-> Regions.size(); j++)
        {
            REGION *this_region = cellpl-> Regions[j];

            overlap.llx = max(this_corner-> rectangle.llx, this_region-> llx);
            overlap.lly = max(this_corner-> rectangle.lly, this_region-> lly);
            overlap.urx = min(this_corner-> rectangle.urx, this_region-> urx);
            overlap.ury = min(this_corner-> rectangle.ury, this_region-> ury);
            if(overlap.llx < overlap.urx && overlap.lly < overlap.ury)  // is overlap
            {
                this_region-> preplace.push_back(overlap);
                this_region-> preplacedArea += (float)(overlap.urx - overlap.llx) * (float)(overlap.ury - overlap.lly);
                //cout << "[DEBUG] Preplaced "<< i << " Region: " << j << " llx: " << overlap.llx << " lly: " << overlap.lly
                //     << " urx: " << overlap.urx << " ury: " << overlap.ury<< endl;
            }
        }
    }
    // step 2 find macro is placed in which region
    for(int i = 0; i < (int)MovableMacro_ID.size(); i++)
    {
        int macro_id = MovableMacro_ID[i];
        Macro* macro_temp = macro_list[macro_id];
        bool flag = false;
        for(unsigned int j = 0; j <  cellpl-> Regions.size(); j++)
        {
            REGION *this_region = cellpl-> Regions[j];
            // if the macro is overlap with which region (this function is find the non-overlap and inverse)
            if(!( macro_temp -> lg.urx <= this_region-> llx    || macro_temp -> lg.ury <= this_region-> lly  ||
                  this_region-> urx    <= macro_temp -> lg.llx || this_region-> ury    <= macro_temp -> lg.lly ) )
            {
                this_region-> macroID.push_back(macro_id);
                flag = true;
            }
        }
        //if(flag == false) cout << "[WARNING] The macro " << macro_id << " is out of region" << endl;
    }

    // legal all cell cluster in the region
    for(unsigned int i = 0 ; i< cellpl-> Regions.size(); i++)
    {
        // step 1 determine "Bin_amount"
        REGION *this_region = cellpl-> Regions[i];

        int Bin_amount = 1;
        int Cell_amount = this_region-> big_STDGroupID.size();

        if(Cell_amount == 0)
            continue;

        int region_W = this_region-> urx - this_region-> llx;
        int region_H = this_region-> ury - this_region-> lly;

        int hor_amount = 1, ver_amount = 1;
        int bin_W1, bin_H1, bin_W2 , bin_H2;
        int bin_W, bin_H;

        // "Binamount" which reduce preplaced must bigger then "Cellamount"
        while(Bin_amount < Cell_amount)
        {
            bin_W1 = ceil(region_W / (hor_amount + 1));
            bin_H1 = ceil(region_H /  ver_amount);
            bin_W2 = ceil(region_W /  hor_amount);
            bin_H2 = ceil(region_H / (ver_amount + 1));

            // if the bin W is similar than bin H it would be better
            if((int)fabs(bin_W1 - bin_H1) < (int)fabs(bin_W2 - bin_H2))
                hor_amount++;
            else
                ver_amount++;

            // "Bin_amount" need to reduce the area overlap with preplaced macro
            Bin_amount = hor_amount * ver_amount;

            bin_W = ceil((float)region_W / hor_amount);
            bin_H = ceil((float)region_H / ver_amount);

            this_region-> w = (int)bin_W;
            this_region-> h = (int)bin_H;

            // construct BINs
            this_region-> Bins.resize(hor_amount);
            for(unsigned int j = 0; j < this_region-> Bins.size() ; j++)
            {
                this_region-> Bins[j].resize(ver_amount);
                for(unsigned int k = 0; k < this_region-> Bins[j].size() ; k++)
                {
                    this_region-> Bins[j][k] = new BIN(); // !
                    BIN *this_bin = this_region-> Bins[j][k];
                    this_bin-> llx = this_region-> llx +  j    * bin_W;
                    this_bin-> urx = this_region-> llx + (j+1) * bin_W;
                    this_bin-> lly = this_region-> lly +  k    * bin_H;
                    this_bin-> ury = this_region-> lly + (k+1) * bin_H;
                    this_bin-> centX = (this_bin-> urx + this_bin-> llx) /2;
                    this_bin-> centY = (this_bin-> ury + this_bin-> lly) /2;
                    //cout << "[DEBUG] bin (x,y): " << this_bin-> centX << " " << this_bin-> centY << endl;
                }
            }
            //cout << "[DEBUG] "<< "Region: "<< i << " hor amount: " << hor_amount << " ver amount: " << ver_amount << endl;

            // if "Bin_amount" is enought for "Cellamount" then reduce the preplaced overlap area
            if(Bin_amount >= Cell_amount)
            {
                // for all preplaced in this region set "isPreplaced"
                for(unsigned int j = 0 ; j < this_region-> preplace.size(); j++)
                {
                    Boundary &this_preplaced = this_region-> preplace[j];

                    int begin_coverX = floor((float)(this_preplaced.llx - this_region-> llx ) / bin_W);
                    int begin_coverY = floor((float)(this_preplaced.lly - this_region-> lly ) / bin_H);
                    int end_coverX   = ceil ((float)(this_preplaced.urx - this_region-> llx ) / bin_W);
                    int end_coverY   = ceil ((float)(this_preplaced.ury - this_region-> lly ) / bin_H);

                    for(int k = begin_coverX ; k < end_coverX ; k++ )
                    {
                        for(int l = begin_coverY ; l < end_coverY ; l++)
                        {
                            // Out of this region's bins index
                            if(k > hor_amount-1 || l > ver_amount-1 || k < 0 || l < 0)
                            {
                                //cout << "[WARNING] Preplace overlap region is out of this region's bins index: "<< k << " " << l << endl;
                                continue;
                            }
                            // "Bin_amount" do not re-reduce by different preplace
                            if(this_region-> Bins[k][l]-> isPreplaced == false)
                            {
                                this_region-> Bins[k][l]-> isPreplaced = true;
                                Bin_amount--;
                            }
                        }
                    }
                }

                // Remove overlap between macro and cell
                for(unsigned int j = 0; j < this_region-> macroID.size(); j++)
                {
                    int macro_id = this_region-> macroID[j];
                    Macro* macro_temp = macro_list[macro_id];

                    int begin_coverX = floor((float)(macro_temp->lg.llx - this_region-> llx ) / bin_W);
                    int begin_coverY = floor((float)(macro_temp->lg.lly - this_region-> lly ) / bin_H);
                    int end_coverX   = ceil ((float)(macro_temp->lg.urx - this_region-> llx ) / bin_W);
                    int end_coverY   = ceil ((float)(macro_temp->lg.ury - this_region-> lly ) / bin_H);

                    for(int k = begin_coverX ; k < end_coverX ; k++ )
                    {
                        for(int l = begin_coverY ; l < end_coverY ; l++)
                        {
                            // Out of this region's bins index
                            if(k > hor_amount-1 || l > ver_amount-1 || k < 0 || l < 0)
                            {
                                //cout << "[WARNING] Macro " << macro_id  << " overlap region is out of this region's bins index: "<< k << " " << l << endl;
                                continue;
                            }
                            // "Bin_amount" do not re-reduce by preplace
                            if(this_region-> Bins[k][l]-> isPreplaced == false)
                            {
                                this_region-> Bins[k][l]-> isPreplaced = true;
                                Bin_amount--;
                            }
                        }
                    }

                }

            }
        }
        this_region-> binsize = this_region-> w * this_region-> h;


        /*
        // Ouput max displacement between bin's Gcenter and the cell GP position
        cout << "\n[DEBUG] Region " << i << " bin weight: " <<  ceil((float)region_W / hor_amount) << endl;

        for(unsigned int j = 0; j < this_region-> big_STDGroupID.size() ; j++)
        {
            int max_dis = 0;
            int groupID = this_region-> big_STDGroupID[j];
            for(unsigned int k = 0; k < STDGroupBy2[groupID]-> member_ID.size(); k++)
            {
                Macro* cell_tmp =  macro_list[ STDGroupBy2[groupID]-> member_ID[k] ];
                int deltaX = fabs(cell_tmp-> gp.llx - STDGroupBy2[groupID]-> GCenter_X );
                int deltaY = fabs(cell_tmp-> gp.lly - STDGroupBy2[groupID]-> GCenter_Y );
                int dis = deltaX + deltaY;
                if(dis > max_dis) max_dis = dis;
            }
            cout << "Group " << group_id <<" max displacement: " << max_dis / ceil((float)region_W / hor_amount)  <<" times"<< endl;
        }*/
        cout << "   Now solve NWF START" << endl;

        SolveNetworkFlow(this_region);
        // step 5 allocate small cell groups to cloest bin

        for(unsigned int j = 0 ; j < this_region-> small_STDGroupID.size() ; j++)
        {
            int groupid = this_region-> small_STDGroupID[j];

            // std group is in the region
            // NOTE: do not consider pre-placed it can be omit
            if(STDGroupBy2[groupid]-> GCenter_X >= this_region-> llx &&
               STDGroupBy2[groupid]-> GCenter_X <  this_region-> urx &&
               STDGroupBy2[groupid]-> GCenter_Y >= this_region-> lly &&
               STDGroupBy2[groupid]-> GCenter_Y <  this_region-> ury )
            {
                int bin_X = (STDGroupBy2[groupid]-> GCenter_X - this_region-> llx) / this_region-> w;
                int bin_Y = (STDGroupBy2[groupid]-> GCenter_Y - this_region-> lly) / this_region-> h;

                STDGroupBy2[groupid]-> packingX = this_region-> Bins[bin_X][bin_Y]-> centX;
                STDGroupBy2[groupid]-> packingY = this_region-> Bins[bin_X][bin_Y]-> centY;
                //this_region-> Bins[bin_X][bin_Y]-> packing_std.push_back(groupid); // do not deal with the cell later beacuse its connected net is NULL
            }

            // std group is out of region
            else
            {
                int binX = -1, binY = -1;
                int binX_max = this_region-> Bins.size() -1;
                int binY_max = this_region-> Bins[0].size() -1;
                if(STDGroupBy2[groupid]-> GCenter_X < this_region-> llx){

                    if(STDGroupBy2[groupid]-> GCenter_Y < this_region-> lly){
                        // case 1 out of llx lly
                        binX = 0;
                        binY = 0;
                    }
                    else if(STDGroupBy2[groupid]-> GCenter_Y >=  this_region-> ury){
                        // case 2 out of llx ury
                        binX = 0;
                        binY = binY_max;
                    }
                    else{
                        // case 3 out of llx
                        binX = 0;
                        binY = (STDGroupBy2[groupid]-> GCenter_Y - this_region-> lly) / this_region-> h;
                    }
                }
                else if(STDGroupBy2[groupid]-> GCenter_X >=  this_region-> urx){
                    if(STDGroupBy2[groupid]-> GCenter_Y < this_region-> lly){
                        // case 4 out of urx lly
                        binX = binX_max;
                        binY = 0;
                    }
                    else if(STDGroupBy2[groupid]-> GCenter_Y >=  this_region-> ury){
                        // case 5 out of urx ury
                        binX = binX_max;
                        binY = binY_max;
                    }
                    else{
                        // case 6 out of urx
                        binX = binX_max;
                        binY = (STDGroupBy2[groupid]-> GCenter_Y - this_region-> lly) / this_region-> h;
                    }
                }
                else if(STDGroupBy2[groupid]-> GCenter_Y < this_region-> lly){
                    // case 7 out of lly
                    binX = (STDGroupBy2[groupid]-> GCenter_X - this_region-> llx) / this_region-> w;
                    binY = 0;
                }
                else if(STDGroupBy2[groupid]-> GCenter_Y >=  this_region-> ury){
                    // case 8 out of ury
                    binX = (STDGroupBy2[groupid]-> GCenter_X - this_region-> llx) / this_region-> w;
                    binY = binY_max;
                }

                STDGroupBy2[groupid]-> packingX = this_region-> Bins[binX][binY]-> centX;
                STDGroupBy2[groupid]-> packingY = this_region-> Bins[binX][binY]-> centY;
                //this_region-> Bins[binX][binY]-> packing_std.push_back(groupid); // do not deal with the cell later beacuse its connected net is NULL
            }
        }
    }
}


void CELLPLACEMENT::SolveNetworkFlow(CELLPLACEMENT::REGION *this_region)
{
    // for example :
    // ss = 4
	// ts = 4
	// The graph picture will be:
    //
    //  source side        target side
    //        -- s1 ----- t1 --
    //       /                 \
    //      /--- s2 ----- t2 ---\
    //   src                     snk
    //      \--- s3 ----- t3 ---/
    //       \                 /
    //        -- s4 ----- t4 --
    //                |
    //                |
    //          you created arcs

	// first create graph g to save data, ex nodes,arcs
	lemon::SmartDigraph g;
	// create map of cost,capacity,supply
	lemon::SmartDigraph::ArcMap<int> cost(g);
	lemon::SmartDigraph::ArcMap<int> lcap(g);
	lemon::SmartDigraph::ArcMap<int> ucap(g);
	lemon::SmartDigraph::NodeMap<int> supl(g);


	// first need to store empty packing bins to "EmptyBins" for all bins
    vector<CELLPLACEMENT::BIN*> EmptyBins;
    for(unsigned int j = 0; j < this_region-> Bins.size() ; j++)
    {
        for(unsigned int k = 0; k < this_region-> Bins[j].size() ; k++)
        {
            CELLPLACEMENT::BIN *this_bin = this_region-> Bins[j][k];
            if(!this_bin-> isPreplaced)
                EmptyBins.push_back(this_bin);
        }
    }

    // set you want to create the count of nodes
    int ss = this_region-> big_STDGroupID.size();
    int ts = EmptyBins.size();
    cout << "[INFO] Number of cell clusters: "<< ss << endl;
    cout << "[INFO] Number of bins: "<< ts << endl;

	// create node & arc container
	// s : source side nodes
	// t : target side nodes
	lemon::SmartDigraph::Node src,snk;
	vector<lemon::SmartDigraph::Node> s,t;
	vector<lemon::SmartDigraph::Arc> arcs;

	// then start action to create nodes & arcs
	Create_Node_Arc(g,src,snk,s,t,arcs,ss,ts);

	// set capacity & cost & supply
	Set_Cap_Cost(src,snk,s,t,arcs,cost,lcap,ucap,supl,this_region,EmptyBins);

	// use network simplex container ns
	// first let cost,cap,supply into ns, then run Check_NS
    lemon::NetworkSimplex<lemon::SmartDigraph, int> ns(g);
    ns.costMap(cost).lowerMap(lcap).upperMap(ucap).supplyMap(supl);
    Check_NS(g, cost, ns, arcs, s.size(), t.size(),this_region,EmptyBins);


	// Writer(g,ns,cost,lcap,ucap,supl,src,snk); // [DEBUG] can used this function // output the data of all graph,nodes,arcs


}

void CELLPLACEMENT::Create_Node_Arc(lemon::SmartDigraph &g,
                     lemon::SmartDigraph::Node &src,
					 lemon::SmartDigraph::Node &snk,
                     vector<lemon::SmartDigraph::Node> &s,
                     vector<lemon::SmartDigraph::Node> &t,
                     vector<lemon::SmartDigraph::Arc> &arcs,
					 int ss, int ts)
{
	// first push src & snk into g
	src = g.addNode();
	snk = g.addNode();
    // create nodes of source side & target side
    // note : whether it is source side or target side;
    //        the first item must be "source node"(src) or "sink node"(snk).
    for(unsigned int i=0 ; i<ss ; ++i)
		s.push_back(g.addNode());
	for(unsigned int i=0 ; i<ts ; ++i)
		t.push_back(g.addNode());

    // create arcs between nodes;
    // we first set every nodes of source side link to every nodes of target side
	for(unsigned int i=0 ; i<s.size() ; ++i)
	{
		for(unsigned int j=0 ; j<t.size() ; ++j)
			arcs.push_back(g.addArc(s[i],t[j]));
	}
	// then create arcs to link src or snk
	for(int i=0 ; i<ss ; ++i)
		arcs.push_back(g.addArc(src,s[i]));
	for(int i=0 ; i<ts ; ++i)
		arcs.push_back(g.addArc(t[i],snk));
	// so the arcs have two part : "between s side & t side" + "link to src or snk"

}

void CELLPLACEMENT::Set_Cap_Cost(lemon::SmartDigraph::Node &src,
                  lemon::SmartDigraph::Node &snk,
                  vector<lemon::SmartDigraph::Node> &s,
                  vector<lemon::SmartDigraph::Node> &t,
                  vector<lemon::SmartDigraph::Arc> &arcs,
                  lemon::SmartDigraph::ArcMap<int> &cost,
                  lemon::SmartDigraph::ArcMap<int> &lcap,
                  lemon::SmartDigraph::ArcMap<int> &ucap,
                  lemon::SmartDigraph::NodeMap<int> &supl,
                  CELLPLACEMENT::REGION *this_region,
                  vector<CELLPLACEMENT::BIN*> &EmptyBins)
{

    for(unsigned int i=0 ; i<arcs.size() ; ++i)
    {
		lcap[arcs[i]] = 0;
		ucap[arcs[i]] = 1;
        if(i < ( s.size() * t.size() ) )
            cost[arcs[i]] = -1;
        else
            cost[arcs[i]] = 0;
    }

    for(unsigned int i=0 ; i<s.size() ; ++i)
        supl[s[i]] = 0;
    for(unsigned int i=0 ; i<t.size() ; ++i)
        supl[t[i]] = 0;

    supl[src] = s.size();
    supl[snk] = 0 - t.size();

	// set you want to change the cost of the arcs
	// you can change,add or delete the cost to look how the result is

    for(unsigned int i=0 ; i<s.size() ; i++)
    {
        for(unsigned int j=0 ; j<t.size() ; j++)
        {
            int groupid = this_region-> big_STDGroupID[i];
            int deltaX = fabs(STDGroupBy2[groupid]-> GCenter_X/PARA - EmptyBins[j]-> centX/PARA);
            int deltaY = fabs(STDGroupBy2[groupid]-> GCenter_Y/PARA - EmptyBins[j]-> centY/PARA);
            int distance = sqrt(deltaX * deltaX + deltaY * deltaY);

            //cout << "STDG cent: " << STDGroupBy2[groupid]-> GCenter_X/PARA << "," << STDGroupBy2[groupid]-> GCenter_Y/PARA << endl;
            //cout << "Bin cent: " << EmptyBins[j]-> centX/PARA << "," << EmptyBins[j]-> centY/PARA << endl;
            //cout << " i:" << i << " j:" << j << " "<< distance  << endl;
            cost[arcs[ t.size() * i + j ]] = distance * distance;
        }
    }
}

void CELLPLACEMENT::Check_NS(lemon::SmartDigraph &g,
              lemon::SmartDigraph::ArcMap<int> &cost,
              lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
              vector<lemon::SmartDigraph::Arc> &arcs,
			  int ss, int ts,CELLPLACEMENT::REGION *this_region,
			  vector<CELLPLACEMENT::BIN*> &EmptyBins)
{
	//cout << "========== Reseult ==========" << endl;

	// set found to check this case can be feasible
    lemon::NetworkSimplex<lemon::SmartDigraph,int>::ProblemType found = ns.run();

	// if found is 1, it is feasible
	// then show the total cost and which arcs between src side & tar side is selected;
    switch(found)
    {
        case 0:
            cout << "[ERROR] This case is INFEASIBLE" << endl;
            cout << "[ERROR] Can't solve" << endl;
            break;
        case 1:
            //cout << "[INFO] This case is OPTIMAL." << endl;
            //cout << "[INFO] Total Cost : " << ns.totalCost() << endl;

            // update cell cluster postiton packingX /Y and add to bin's packing_std
            for(int i=0 ; i< ss * ts ; ++i)
            {
                if(ns.flow(arcs[i]))
				{
				    int group_id = g.id(g.source(arcs[i])) -1 -1;
				    int clusterID = this_region-> big_STDGroupID[group_id];
				    int binID = g.id(g.target(arcs[i])) -ss -1 -1;
				    STDGroupBy2[clusterID]-> packingX = EmptyBins[binID]-> centX;
                    STDGroupBy2[clusterID]-> packingY = EmptyBins[binID]-> centY;
                    EmptyBins[binID]-> packing_std.push_back(clusterID);
				}
            }

            break;
        case 2:
            cout << "[ERROR] This case is UNBOUNDED" << endl;
            cout << "[ERROR] There is a directed cycle having negative total cost and infinite upper bound." << endl;
            break;
    }
}

void CELLPLACEMENT::Writer(lemon::SmartDigraph &g,
            lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
            lemon::SmartDigraph::ArcMap<int> &cost,
            lemon::SmartDigraph::ArcMap<int> &lcap,
            lemon::SmartDigraph::ArcMap<int> &ucap,
            lemon::SmartDigraph::NodeMap<int> &supl,
			lemon::SmartDigraph::Node src,
			lemon::SmartDigraph::Node snk)
{
	cout << "========== Graph ==========" << endl;

	lemon::SmartDigraph::ArcMap<int> flow(g);
	ns.flowMap(flow);

	digraphWriter(g).                  // write g to the standard output
			node("source", src).       // write s to 'source'
	        node("target", snk).       // write t to 'target'
	        arcMap("cost", cost).      // write 'cost' for arcs
	        arcMap("flow", flow).      // write 'flow' for arcs
	        arcMap("lcap", lcap).      // write 'capacity' for arcs
	        arcMap("ucap", ucap).
			nodeMap("supply",supl).    // write 'supply' for nodes
	        run();
}

void CELLPLACEMENT::PlotCellCluster(CELLPLACEMENT *cellpl, int iter)
{
    STD_Group *group_temp;
    int llx, lly, urx, ury;

    // 1. output CellCluster.txt
    cout << "   Write CellCluster.txt..." << endl;
    ofstream fout_cluster("./output/Cell_graph/CellCluster.txt");

    fout_cluster << "Number of groups : "<< STDGroupBy2.size() << endl;
    for(unsigned int i = 0; i < STDGroupBy2.size() ; i++)
    {
        fout_cluster << "\nSTD_group ID: "  << STDGroupBy2[i]-> ID           << "\tCenter X: "   << STDGroupBy2[i]-> GCenter_X
                       << "\tCenter Y: "    << STDGroupBy2[i]-> GCenter_Y    << "\tAvg W: "      << STDGroupBy2[i]-> Avg_W
                       << "\tAvg H: "       << STDGroupBy2[i]-> Avg_H        << "\tsqrt W: "     << STDGroupBy2[i]-> Width
                       << "\tsqrt H: "      << STDGroupBy2[i]-> Height       << "\tToatalArea: " << STDGroupBy2[i]-> TotalArea << endl;
        fout_cluster << "\tNumber of cells: " << STDGroupBy2[i]-> member_ID.size()  << endl;
        for(unsigned int j =0; j < STDGroupBy2[i]-> member_ID.size() ; j++)
        {
            int stdid = STDGroupBy2[i]-> member_ID[j];
            fout_cluster << "\tCell ID: "<< stdid << "\tNumber of connected Net weight: "    << macro_list[stdid]-> NetID.size()   << endl;
        }
        map<int, int>::iterator itermap;
        map<int, int> &netsmap = STDGroupBy2[i]-> NetID_to_weight;
        fout_cluster << "\tNumber of connected nets: " << netsmap.size()  << endl;
        for(itermap = netsmap.begin() ; itermap != netsmap.end() ; itermap++ )
        {
            fout_cluster << "\tNet ID: "<< itermap-> first << "\tNet weight: "    << itermap-> second   << endl;
        }
    }
    fout_cluster.close();

    // 2. Output CellCluster_GP.m
    cout << "   Write CellCluster_GP"<<iter<<".m ..." << endl;
    ofstream foutGP("./output/Cell_graph/CellCluster_GP.m", fstream::out);

    foutGP<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    // chip boundary
    foutGP << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    foutGP << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    foutGP << "fill(block_x, block_y, 'w');" << endl;

    // all STD group
    for(unsigned int i = 0; i < STDGroupBy2.size(); i++)
    {
        group_temp = STDGroupBy2[i];
        llx = group_temp-> GPCenter_X - (group_temp -> Width ) /2;
        lly = group_temp-> GPCenter_Y - (group_temp -> Height) /2;
        urx = group_temp-> GPCenter_X + (group_temp -> Width ) /2;
        ury = group_temp-> GPCenter_Y + (group_temp -> Height) /2;

        foutGP << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        foutGP << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        foutGP << "fill(block_x, block_y, 'g');" << endl;
        // print the cluster id on MATLAB
        //foutGP << "text(" << llx << "," << group_temp-> GPCenter_Y << ",'" << i << "');" << endl;
    }

    foutGP.close();

    // 3. Output CellCluster_GP.m
    cout << "   Write CellCluster_QPspare"<<iter<<".m ..." << endl;
    string file = "./output/Cell_graph/CellCluster_QPspare";
    file += int2str(iter);
    file += ".m";
    ofstream foutQP(file.c_str(), fstream::out);

    foutQP <<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    // chip boundary
    foutQP << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    foutQP << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    foutQP << "fill(block_x, block_y, 'w');" << endl;

    // all STD group
    for(unsigned int i = 0; i < STDGroupBy2.size(); i++)
    {
        group_temp = STDGroupBy2[i];
        llx = group_temp-> GCenter_X - (group_temp -> Width ) /2;
        lly = group_temp-> GCenter_Y - (group_temp -> Height) /2;
        urx = group_temp-> GCenter_X + (group_temp -> Width ) /2;
        ury = group_temp-> GCenter_Y + (group_temp -> Height) /2;

        foutQP << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        foutQP << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        foutQP << "fill(block_x, block_y, 'g');" << endl;
        // print the cluster id on MATLAB
        //foutQP << "text(" << llx << "," << group_temp-> GCenter_Y << ",'" << i << "');" << endl;
    }

    foutQP.close();

    // 4. Output CellCluster_toPRegion.txt
    cout << "   Write CellCluster_toPRegion.txt..." << endl;
    ofstream fout_std_group("./output/Cell_graph/CellCluster_toPRegion.txt");
    for(unsigned int i = 0; i < STDGroupBy2.size() ; i++)
    {
        fout_std_group << "STD_Group id: " << i <<"\tP_Region id: "<< STDGroupBy2[i]-> P_regionID << endl;
    }
    fout_std_group.close();



    // 4. Output CellCluster_Placement.m
    cout << "   Write CellCluster_Placement.m ..." << endl;
    file = "./output/Cell_graph/CellCluster_Placement";
    file += int2str(iter);
    file += ".m";
    ofstream fout_cell(file.c_str(), fstream::out);

    fout_cell<<"figure;\nclear;\nclf;\naxis equal;\nhold on;\ngrid on;\n"<<endl;

    // chip boundary
    fout_cell << "block_x=[" << chip_boundary.llx << " " << chip_boundary.llx << " " << chip_boundary.urx << " " << chip_boundary.urx << " " << chip_boundary.llx << " ];" << endl;
    fout_cell << "block_y=[" << chip_boundary.lly << " " << chip_boundary.ury << " " << chip_boundary.ury << " " << chip_boundary.lly << " " << chip_boundary.lly << " ];" << endl;
    fout_cell << "fill(block_x, block_y, 'w');" << endl;

    // for all STD group
    for(unsigned int i = 0; i < STDGroupBy2.size(); i++)
    {
        group_temp = STDGroupBy2[i];
        llx = group_temp-> packingX - (group_temp -> Width ) /2;
        lly = group_temp-> packingY - (group_temp -> Height) /2;
        urx = group_temp-> packingX + (group_temp -> Width ) /2;
        ury = group_temp-> packingY + (group_temp -> Height) /2;

        fout_cell << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
        fout_cell << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
        fout_cell << "fill(block_x, block_y, 'g');" << endl;
        // print the cluster id on MATLAB
        //fout_cell << "text(" << llx << "," << group_temp-> packingY << ",'" << i << "');" << endl;
    }

    fout_cell.close();
}


// NOTE: only spread the "big_STDgroup" in the bin
void CELLPLACEMENT::SpareCell(CELLPLACEMENT *cellpl)
{
    pthread_t threads[NUM_THREADS];
    int rc, t_num = 0;

    cout << "   Now run QP to Spread Cells " << endl;
    // run QP for each bin in the region with cell group
    for(unsigned int i = 0; i< cellpl-> Regions.size(); i++)
    {
        REGION *this_region = cellpl-> Regions[i];
        for(unsigned int j = 0; j< this_region-> Bins.size(); j++)
        {
            for(unsigned int k = 0 ; k< this_region-> Bins[j].size(); k++)
            {
                if(this_region-> Bins[j][k]-> packing_std.size() > 0){
                    // multi-thread to run QP
                    // [NOTE] debug do not used
                    rc = pthread_create(&threads[t_num], NULL, QPSpreadCell, (void *)this_region-> Bins[j][k]);
                    // QPSpreadCell( (void *)this_region-> Bins[j][k] );
                    if(rc == 1){
                            cout << "[Error] unable to create multi-thread " << rc << endl; exit(-1);
                    }
                    t_num++;
                }

            }
        }
    }
    // thread join
    for(int m = 0; m < t_num ; m++)
        pthread_join(threads[m], NULL);
}


// [NOTE] this CODE is written by SZU (GP.cpp/.h) and modify to our data structure
void* CELLPLACEMENT::QPSpreadCell(void* vbin)
{
    CELLPLACEMENT::BIN* bin = (CELLPLACEMENT::BIN*)vbin;
    //double start_time, end_time, QP_ST, QP_ALLT;
    //start_time = clock();

    // step 1 "mapNet" is remove same nets between the packing STD Groups of the bin
    int countNet = 0;
    // [NOTE] mapnet is input the inside nets of the bin and its weight is connected cells in the bin
    map<int, int> mapNet;
    // for all cell cluster of the bin
    for( unsigned int i = 0; i < bin-> packing_std.size(); i++ ) // NOTE: just one std group in the bin now
    {
        int std_id = bin-> packing_std[i];
        STD_Group* std_group = STDGroupBy2[std_id];

        map<int, int>::iterator itermap;
        // for all net in the cluster
        for(itermap = std_group-> NetID_to_weight.begin(); itermap != std_group-> NetID_to_weight.end(); itermap++)
        {
            //if(itermap-> second == 0)
            //{
            //    cout <<"[WARNING] net "<< itermap-> first << "doesn't have any pin, has been remove." << endl;
            //    continue;
            //}
            map<int, int>::iterator itermap2;
            itermap2 = mapNet.find(itermap-> first);
            // case 1 the net is not exist in "mapNet"
            if(itermap2 == mapNet.end())
                mapNet[itermap-> first] = itermap-> second; // "net weight" is the number of pin is connect to the net in cluster
            // case 2 the net is exist in "mapNet"
            else
                 //[NOTE] the net connected cells in these groups  are independent
                mapNet[itermap-> first] = itermap-> second + itermap2-> second; // mapNet's net weight is the number of pin is connect to the net in BIN
        }
    }
    countNet = mapNet.size();

    // step 2 "mapMod" is store all cell in the bin (big cluster) and map to a index
    int countMod = 0;
    map<int, int> mapMod;
    // for all cell cluster of the bin
    for( unsigned int i = 0; i < bin-> packing_std.size(); i++ ) // NOTE: just one std group in the bin now
    {
        int std_id = bin-> packing_std[i];
        STD_Group* std_group = STDGroupBy2[std_id];
        // for all cell in the cluster
        for(unsigned int j = 0 ; j< std_group-> member_ID.size(); j++)
        {
            int cell_id = std_group-> member_ID[j];
            if(macro_list[cell_id]-> NetID.size() == 0)
                cout <<"[ERROR] Cell "<< cell_id << " doesn't have any net, placese remove this cell in the cluster!" << endl;
            else{
                mapMod[cell_id] = countMod; // "countMod" is equal to "modID" map a cell id to a index which can assign value to matrix's column in below
                countMod++;
            }
        }
    }

    int sizeQ = countMod + countNet;

    // step 3 solve X direction and Y direction
    for(int modeXY = 0; modeXY <= 1; modeXY++ )
    {

        vector<CELLPLACEMENT::QP> matrixQ;
        matrixQ.resize(sizeQ);

        float *vectorB = new float [countNet];
        for( int i = 0; i < countNet; i++ )
        {
            vectorB[i] = 0;
        }

        CreateQmBv( matrixQ, vectorB, bin, modeXY, mapNet, mapMod ); // function : CreateQmBv() is in below..

        QMatrix Qm;
        Vector Bv, Xv;

        // step 3-1 construct Qm
        Q_Constr( &Qm, "Qm", sizeQ, True, Rowws, Normal, True );
        for( int i = 0; i < sizeQ; i++ )
        {
            int entryNum = 0;
            for( unsigned int j = 0; j < matrixQ[i].connect.size(); j++ )
            {
                if( matrixQ[i].connect[j].col >= i ) entryNum++;
            }
            Q_SetLen( &Qm, i+1, entryNum );

            entryNum = 0;
            for( unsigned int j = 0; j < matrixQ[i].connect.size(); j++ )
            {
                if( matrixQ[i].connect[j].col >= i )
                {
                    Q_SetEntry( &Qm, i+1, entryNum, matrixQ[i].connect[j].col+1, matrixQ[i].connect[j].weight );
                    entryNum++;
                }
            }
        }
        // step 3-2 construct Xv
        V_Constr( &Xv, "Xv", sizeQ, Normal, True );

        if(modeXY == 0)         ///< set initial value for x
            V_SetAllCmp( &Xv, (float)bin-> centX / PARA );
        else                    ///< set initial value for y
            V_SetAllCmp( &Xv, (float)bin-> centY / PARA );

        // step 3-3 construct Bv
        V_Constr( &Bv, "Bv", sizeQ, Normal, True );

        for( int i = 0; i < sizeQ; i++ )
        {
            if( i >= countMod )
                V_SetCmp( &Bv, i+1, vectorB[i-countMod] );
            else
                V_SetCmp( &Bv, i+1, 0.0 );
        }

        // step 3-4 solve Xv using "laspack"
        SetRTCAccuracy(1e-9);

        //QP_ST = clock();

        CGIter( &Qm , &Xv , &Bv , 100 , SSORPrecond , 1.0 );

        //QP_ALLT = clock() - QP_ST;

        // step 3-5 output value
        for( unsigned int i = 0; i < bin-> packing_std.size(); i++ ) // NOTE: just one std group in the bin now
        {
            int std_id = bin-> packing_std[i];
            STD_Group* std_group = STDGroupBy2[std_id];

            for(unsigned int j = 0 ; j< std_group-> member_ID.size(); j++)
            {
                int cell_id = std_group-> member_ID[j];
                int modID = mapMod[cell_id];
                if( modeXY == 0 )
                {
                    macro_list[cell_id]-> lg.llx = (int)(V_GetCmp(&Xv, modID + 1) * PARA - macro_list[cell_id]-> real_w / 2);
                    macro_list[cell_id]-> lg.urx = (int)(V_GetCmp(&Xv, modID + 1) * PARA + macro_list[cell_id]-> real_w / 2);
                }
                else
                {
                    macro_list[cell_id]-> lg.lly = (int)(V_GetCmp(&Xv, modID + 1) * PARA - macro_list[cell_id]-> real_h / 2);
                    macro_list[cell_id]-> lg.ury = (int)(V_GetCmp(&Xv, modID + 1) * PARA + macro_list[cell_id]-> real_h / 2);
                }
            }
        }
        // step 3-6 clear Qm Bv Xv
        for( unsigned int i = 0; i < matrixQ.size(); i++ )
        {
            matrixQ[i].connect.clear();
        }
        matrixQ.clear();

        vector<QP> (matrixQ).swap(matrixQ);

        delete [] vectorB;

        Q_Destr( &Qm );
        V_Destr( &Bv );
        V_Destr( &Xv );

    }
    pthread_exit(NULL);
    //end_time = clock();
    //cout << "Time comsume in Laspack: " << QP_ALLT / (end_time - start_time)  * 100.0 << "% "<<endl;
}

// construct Qm/ Bv vector for QP
void CELLPLACEMENT::CreateQmBv( vector<CELLPLACEMENT::QP> &matrixQ, float *vectorB,CELLPLACEMENT::BIN* bin, int modeXY, map<int, int> &mapNet, map<int, int> &mapMod )
{
    int numNet = 0; // number of the net index in mapNet
    map<int, int>::iterator itermap;
    // for all net in the bin's cell
    for(itermap = mapNet.begin(); itermap != mapNet.end(); itermap++)
    {
        int netID = itermap-> first;
        int npin = net_list[netID].macro_idSet.size() + net_list[netID].cell_idSet.size();  // number of the pin of the net is connected

        // creat starNode and push back into Qm
        int nMod = (int)mapMod.size();
        int starNodeID = nMod + numNet;

        float pinWeight = (float)1 / npin;

        CELLPLACEMENT::WEIGHT starNode;    // start net module will creat a pseude node "starNode"
        starNode.col = starNodeID;
        starNode.weight = pinWeight * (float)npin; // starNode.weight = 1

        // starNode would add into Q matrix from (nMod) to (nMod + nNet)
        matrixQ[starNodeID].connect.push_back(starNode);

        // set the value of vector Bv and Qm

        // 1. for all pin of the net in "macro_idSet"
        for(unsigned int i = 0; i < net_list[netID].macro_idSet.size(); i++)
        {
            int macro_id = net_list[netID].macro_idSet[i];
            Macro* macro_temp = macro_list[macro_id];

            // find the pin of the macro is connect to the net
            int shiftx = 0, shifty = 0;
            map<int, vector<int> >::iterator itermap2;
            itermap2 = macro_temp-> NetsID_to_pinsID.find(netID);

            if(itermap2 == macro_temp-> NetsID_to_pinsID.end())
            {
                //cout << "[WARNING] Can not find pin of the net "<< netID <<" in macro "<< macro_id << endl;
                shiftx = macro_temp-> real_w /2;
                shifty = macro_temp-> real_h /2;
            }
            else
            {
                // Calcurate the average pin position
                int numpin = itermap2-> second.size();
                for(int j=0; j < numpin; j++)
                {
                    int pin_id = itermap2-> second.at(j);
                    shiftx += pin_list[pin_id]-> x_shift_plus;
                    shifty += pin_list[pin_id]-> y_shift_plus;
                }
                if(numpin != 0)
                {
                    shiftx /= numpin;
                    shifty /= numpin;
                }
                else
                {
                    shiftx + macro_temp-> real_w /2;
                    shifty + macro_temp-> real_h /2;
                }
            }

            // All the macro's pin is connect to "fixed pin" : create "vectorB" by the X/Y position
            if(modeXY == 0) // for x
            {
                int pin_x = macro_temp-> lg.llx + shiftx;
                vectorB[numNet] += pinWeight * (float)pin_x / PARA;
            }
            else // for y
            {
                int pin_y = macro_temp-> lg.lly + shifty;
                vectorB[numNet] += pinWeight * (float)pin_y / PARA;
            }
        }
        // 2. for all pin of the net in "cell_idSet"
        for(unsigned int i = 0; i < net_list[netID].cell_idSet.size(); i++)
        {
            int cell_id = net_list[netID].cell_idSet[i];
            Macro* cell_temp = macro_list[cell_id];

            // skip the cell without any net which doesn't exist in "mapMod"
            //if(cell_temp-> NetID.size() == 0) {
            //    cout<< "ZZZ" <<endl;
            //    continue;
            //}

            // find the cell is in the bin's cluster or not
            bool inside = false;
            for(unsigned int j =0; j < bin-> packing_std.size() ; j++) // NOTE: just one std group in the bin now
            {
                int std_group_id = bin-> packing_std[j];
                STD_Group* std_group = STDGroupBy2[std_group_id];
                for(unsigned int k =0; k < std_group-> member_ID.size(); k++)
                {
                    if(std_group-> member_ID[k] == cell_id)
                        inside = true;
                }
            }

            // case 1 if the cell is not inside, then the pin is connect to "fixed pin" : create "vectorB" by the X/Y position
            if(inside == false)
            {
                int std_group_id = cell_temp-> group2_id;
                if(modeXY == 0) // for x
                {
                    // outside cell postiton is fixed at the bin center
                    int pin_x = STDGroupBy2[std_group_id]-> packingX - cell_temp-> real_w /2;
                    vectorB[numNet] += pinWeight * (float)pin_x / PARA;
                }
                else // for y
                {
                    int pin_y = STDGroupBy2[std_group_id]-> packingY - cell_temp-> real_h /2;
                    vectorB[numNet] += pinWeight * (float)pin_y / PARA;
                }
            }
            else // case 2 otherwise the pin is connect to "non-fixed pin" : create "matrixQ"
            {
                int modID = mapMod[cell_id]; // mod ID is the Qm index from 0 to nMod (the number of cell)
                CELLPLACEMENT::WEIGHT modNode;
                modNode.col = modID;
                modNode.weight = pinWeight;

                // 1. increase pinWeight to the matrixQ[modID]
                    // deal with diagonal
                bool flagHadAdd = false;
                for( unsigned int k = 0; k < matrixQ[modID].connect.size(); k++ )
                {
                    if(modID == matrixQ[modID].connect[k].col)
                    {
                        matrixQ[modID].connect[k].weight += pinWeight;
                        flagHadAdd = true;
                        break;
                    }
                }
                if(!flagHadAdd)
                {
                    matrixQ[modID].connect.push_back(modNode);
                }


                // 2. descrease pinWeight if the matrixQ[modID]
                    // deal with diagonal
                flagHadAdd = false;
                for( unsigned int k = 0; k < matrixQ[modID].connect.size(); k++ )
                {
                    if( starNodeID == matrixQ[modID].connect[k].col )
                    {
                        matrixQ[modID].connect[k].weight -= pinWeight;
                        flagHadAdd = true;
                        break;
                    }
                }
                if( !flagHadAdd )
                {
                    starNode.weight = -pinWeight;
                    matrixQ[modID].connect.push_back(starNode);
                }
                    // deal with dutton of the matrix
                modNode.weight = -pinWeight;
                matrixQ[starNodeID].connect.push_back(modNode);
            }
        }
        numNet ++;
    }
}
void CELLPLACEMENT::PackingCell(CELLPLACEMENT *cellpl)
{
    pthread_t threads[NUM_THREADS];
    int rc, t_num = 0;

    cout << "   Now run Legal Cells of each bin" << endl;
    // Legal cells to the bin
    for(unsigned int i = 0; i< cellpl-> Regions.size(); i++)
    {
        REGION *this_region = cellpl-> Regions[i];
        for(unsigned int j = 0; j< this_region-> Bins.size(); j++)
        {
            for(unsigned int k = 0 ; k< this_region-> Bins[j].size(); k++)
            {
                if(this_region-> Bins[j][k]-> packing_std.size() > 0){
                    // multi-thread to run tetris
                    // [NOTE] debug do not used
                    rc = pthread_create(&threads[t_num], NULL, OrderlyPackingCell, (void *)this_region-> Bins[j][k]);
                    //OrderlyPackingCell( (void *)this_region-> Bins[j][k]);
                    if(rc == 1){
                            cout << "[Error] unable to create multi-thread " << rc << endl; exit(-1);
                    }
                    t_num++;
                }
            }
        }
    }
    // thread join
    for(int m = 0; m < t_num ; m++)
        pthread_join(threads[m], NULL);
}

void CELLPLACEMENT::quickSort(vector<int> &number, int left, int right , bool sortX)
{
    if( left < right) {
        int i = left - 1;
        for(int j = left; j < right; j++) {
            if((macro_list[ number[j] ]-> lg.llx <= macro_list[ number[right] ]-> lg.urx && sortX == true ) ||
               (macro_list[ number[j] ]-> lg.lly <= macro_list[ number[right] ]-> lg.ury && sortX == false)) {
                i++;
                SWAP(number[i], number[j]);
            }
        }
        SWAP(number[i+1], number[right]);
        int q = i + 1;
        quickSort(number, left, q-1, sortX);
        quickSort(number, q+1, right, sortX);
    }
}


void* CELLPLACEMENT::OrderlyPackingCell(void* vbin)
{
    CELLPLACEMENT::BIN* bin = (CELLPLACEMENT::BIN*)vbin;

    int std_id = bin-> packing_std[0];
    STD_Group* std_group = STDGroupBy2[std_id];

    int bin_h = bin-> ury - bin-> lly;
    int bin_w = bin-> urx - bin-> llx;

    // step 1 Calculate "zoom" to expand cells in the row
    int rowH = BenchInfo.min_std_h;
    // [NOTE] the bin area could be small can't placed the least row, so we get the placeable row of the bin
    int NumRow = bin_h / rowH;
    // [NOTE] double-row hight cells are seen as one-row hight( total weight of all cells is "CellW") therefore the bin will not full
    float zoom = (float)(NumRow * bin_w) / std_group-> Total_W;

    // step 2 sort cells by Y coordinate
    quickSort(std_group-> member_ID, 0 ,std_group-> member_ID.size()-1, false);

    // step 3 push in the queue to store non-legal cell
    queue<int> Cell_queue;

    for(unsigned int i = 0; i< std_group-> member_ID.size() ; i++)
    {
        int cell_id = std_group-> member_ID[i];
        Cell_queue.push(cell_id);
    }

    // step 7 creat "Packing_CellID" to store packing cell to the row

    int overflowW = 0; // the over packing weight of the cells in the row

    // for all row in the bin place the cells in "Cell_queue"
    for(unsigned int i = 0; i< NumRow ; i++)
    {
        // step 5 get the lower cell to packing to the row from button
        vector<int> Packing_CellID;
        int RowUsage = 0 + overflowW;

        if(Cell_queue.empty())
            cout << "[WARNING] The row is empty" << endl;

        // if the row is not full of cells then push more cell in "Packing_CellID"
        while(RowUsage <= bin_w && !Cell_queue.empty())
        {
            Macro* cell_tmp = macro_list[ Cell_queue.front() ];
            RowUsage += cell_tmp-> real_w * zoom ;
            Packing_CellID.push_back(cell_tmp-> macro_id);
            Cell_queue.pop();
            if(!Cell_queue.empty())
                cell_tmp = macro_list[ Cell_queue.front() ];
        }
        overflowW = RowUsage - bin_w;

        // sort the paching cell by X coordinate in the same row's cell
        quickSort(Packing_CellID, 0 ,Packing_CellID.size()-1, true);

        // step 6 packing cell from left to right
        int packing_X = bin-> llx + overflowW;
        for(unsigned int j = 0; j< Packing_CellID.size() ; j++)
        {
            Macro* cell_tmp = macro_list[ Packing_CellID[j] ];
            cell_tmp-> lg.llx = packing_X;
            cell_tmp-> lg.urx = packing_X + cell_tmp-> real_w;
            cell_tmp-> lg.lly = bin-> lly + rowH * i;
            cell_tmp-> lg.ury = bin-> lly + rowH * i + cell_tmp-> real_h;
            // expand cells in the row
            packing_X += cell_tmp-> real_w * zoom;
        }

    }
    if(!Cell_queue.empty())
        cout << "[ERROR] Has "<< Cell_queue.size() <<" cell doesn't packing to the bin " << endl;

    pthread_exit(NULL);
}

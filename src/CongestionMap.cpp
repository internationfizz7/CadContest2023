#include "CongestionMap.h"

#define MAXDEGREE 3000 // if the net degree > 3000 then skip

// for congestion penalty
#define PENALTY 0.05
#define SLOPE 0.5

/// 2021.03.17
CONGESTIONMAP CG_INFO;
int step = 0;


void clear_para_congmap(){
    step = 0;
}

// [NOTE] this CODE is written by I-R Chen(CongestionMap.cpp/.h)
float CONGESTIONMAP::CongestionMapFlow(CONGESTIONMAP *cong, bool sloveGP, int iter)
{
    cout << "   Now run Congestion Estimation" << endl;
   // Build GR Edges
    //cout << "1"<<endl;
    CONGESTIONMAP::GRBinInfo(cong);
   // cout << "1" << endl;
    // Convert All Nets to 2_Pin Nets
    CONGESTIONMAP::Flute2PinNet(cong, sloveGP);
    //cout << "2" << endl;
    // Plot obstacles of the macro in each layer
    if(Debug_PARA)
        CONGESTIONMAP::PlotMacroObs(cong, sloveGP);
   // cout << "3" << endl;
    // Insert Obstacle (as Demand) into Bins
    CONGESTIONMAP::AddObsDemand(cong, sloveGP, iter);
    //cout << "4" << endl;
    //CONGESTIONMAP::ForDebugPlotDPX(cong);

    // Insert Demand into Bins
    CONGESTIONMAP::AddGRBINDemand(cong);
    //cout << "5" << endl;
    // Display in matlab
    if(Debug_PARA)
        CONGESTIONMAP::Displaymatlab(cong, sloveGP, iter);
    //cout << "6" << endl;
    // Calcurate congestion map cost
    return CONGESTIONMAP::Calcost(cong);
}

void CONGESTIONMAP::ForDebugPlotDPX(CONGESTIONMAP *cong)
{
    ofstream fout("./output/CongestionMap/V_CGMap.dpx");
    fout<< "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;
    for(int i = 0; i < (int)cong->V_Edge.size(); i++)
    {
        for(int j = 0; j < (int)cong->V_Edge[i].size(); j++)
        {
            if(cong->V_Edge[i][j]->demand == cong->V_Edge[i][j]->capacity)
            {
                int x1 = cong->V_Edge[i][j]->llx;
                int x2 = cong->V_Edge[i][j]->urx;
                int y1 = cong->V_Edge[i][j]->lly;
                int y2 = cong->V_Edge[i][j]->ury;

                //cout<<x1<<"\t"<<y1<<" to "<<y2<<endl;getchar();
                fout<<"COLOR "<<"red"<<endl;
                fout<<"SR "<<x1<<" -"<<y1<<" "<<x2<<" -"<<y2<<endl;
            }
            //cout<<"("<<cong->V_Edge[i][j]->demand<<", "<<cong->V_Edge[i][j]->capacity<<") ";
        }
    }
    fout.close();
    //cout<<endl;
    ofstream ffout("./output/CongestionMap/H_CGMap.dpx");
    ffout<< "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;
    for(int i = 0; i < (int)cong->H_Edge.size(); i++)
    {
        for(int j = 0; j < (int)cong->H_Edge[i].size(); j++)
        {
            if(cong->H_Edge[i][j]->demand == cong->H_Edge[i][j]->capacity)
            {
                int x1 = cong->H_Edge[i][j]->llx;
                int x2 = cong->H_Edge[i][j]->urx;
                int y1 = cong->H_Edge[i][j]->lly;
                int y2 = cong->H_Edge[i][j]->ury;

                //cout<<y1<<"\t"<<x1<<" to "<<x2<<endl;getchar();
                ffout<<"COLOR "<<"blue"<<endl;
                ffout<<"SR "<<x1<<" -"<<y1<<" "<<x2<<" -"<<y2<<endl;
            }
            //cout<<"("<<cong->H_Edge[i][j]->demand<<", "<<cong->H_Edge[i][j]->capacity<<") ";
        }
    }

    ffout.close();
}

void CONGESTIONMAP::CalculateCapacity(CONGESTIONMAP *cong)
{
    for(int i = 0; i < (int)routing_layer.size(); i++)
    {
        if(routing_layer[i]->direction == "VERTICAL")
        {
            for(int j = 0; j < (int)cong->H_Edge.size(); j++)
            {
                for(int k = 0; k < (int)cong->H_Edge[j].size(); k++)
                {
                    cong->H_Edge[j][k]->capacity += floor((float)gcellgrid[0]->step / ( PARA * (routing_layer[i]->min_width + routing_layer[i]->min_spacing) ) );
                }
            }
        }
        else if(routing_layer[i]->direction == "HORIZONTAL")
        {
            for(int j = 0; j < (int)cong->V_Edge.size(); j++)
            {
                for(int k = 0; k < (int)cong->V_Edge[j].size(); k++)
                {
                        cong->V_Edge[j][k]->capacity += floor((float)gcellgrid[1]->step / ( PARA * (routing_layer[i]->min_width + routing_layer[i]->min_spacing) ) );
                }
            }
        }
    }
}

void CONGESTIONMAP::GRBinInfo(CONGESTIONMAP *cong)
{
    cout << "01" << endl;
    cong->layer_num = routing_layer.size();
    cout << "11"<<endl;
    /// MODIFY 2021.03.04 ///
    for(int i = 0; i < (int)gcellgrid.size(); i++)
    {
        if( gcellgrid[i]->direction == "X" )    // column
        {
            //cong->col = gcellgrid[i]->num;    // let grids in congestion map are the same as ICC congestion map

            gcellgrid[i]->step = gcellgrid[i]->step_const * 10;   // let grids in congestion map are greater than ICC congestion map
            gcellgrid[i]->num = (chip_boundary.urx - chip_boundary.llx) / gcellgrid[i]->step + 1;
            cong->col = gcellgrid[i]->num;
        }
        else if( gcellgrid[i]->direction == "Y" )   // row
        {
            //cong->row = gcellgrid[i]->num;    // let grids in congestion map are the same as ICC congestion map

            gcellgrid[i]->step = gcellgrid[i]->step_const * 10;   // let grids in congestion map are greater than ICC congestion map
            gcellgrid[i]->num = (chip_boundary.ury - chip_boundary.lly) / gcellgrid[i]->step + 1;
            cong->row = gcellgrid[i]->num;
        }
    }

    /// MODIFY 2021.03.04 ///
    cong->H_Edge.resize(cong->row + 1);
    cong->V_Edge.resize(cong->col + 1);
    for(int i = 0; i < (int)cong->H_Edge.size(); i++)
        cong->H_Edge[i].resize(cong->col);
    for(int i = 0; i < (int)cong->V_Edge.size(); i++)
        cong->V_Edge[i].resize(cong->row);

    /// Create Edge ///
    if( (chip_boundary.urx-chip_boundary.llx) % gcellgrid[0]->step != 0 )
    {
        // construct columns H_Edge
        for(int i = 0; i < (int)cong->H_Edge.size(); i++)
        {
            // construct first column H_Edge
            cong->H_Edge[i][0] = new EDGE();
            cong->H_Edge[i][0]->llx = (int)chip_boundary.llx;
            cong->H_Edge[i][0]->urx = (int)( (chip_boundary.urx-chip_boundary.llx) -
                                            ((chip_boundary.urx-chip_boundary.llx) / gcellgrid[0]->step * gcellgrid[0]->step) );

            // construct other columns H_Edge
            for(int j = 1; j < (int)cong->H_Edge[i].size(); j++)
            {
                cong->H_Edge[i][j] = new EDGE();
                cong->H_Edge[i][j]->llx = cong->H_Edge[i][j-1]->urx;
                cong->H_Edge[i][j]->urx = cong->H_Edge[i][j]->llx + gcellgrid[0]->step;
            }
            // y coordinate doesn`t assign value yet.
        }
    }
    else
    {
        // construct columns H_Edge
        for(int i = 0; i < (int)cong->H_Edge.size(); i++)
        {
            // construct first column H_Edge
            cong->H_Edge[i][0] = new EDGE();
            cong->H_Edge[i][0]->llx = (int)chip_boundary.llx;
            cong->H_Edge[i][0]->urx = cong->H_Edge[i][0]->llx + gcellgrid[0]->step;

            // construct other columns H_Edge
            for(int j = 1; j < (int)cong->H_Edge[i].size(); j++)
            {
                cong->H_Edge[i][j] = new EDGE();
                cong->H_Edge[i][j]->llx = cong->H_Edge[i][j-1]->urx;
                cong->H_Edge[i][j]->urx = cong->H_Edge[i][j]->llx + gcellgrid[0]->step;
            }
            // y coordinate doesn`t assign value yet.
        }
    }

    if( (chip_boundary.ury-chip_boundary.lly) % gcellgrid[1]->step != 0 )
    {
        // construct row V_Edge
        for(int i = 0; i < (int)cong->V_Edge.size(); i++)
        {
            // construct first row V_Edge
            cong->V_Edge[i][0] = new EDGE();
            cong->V_Edge[i][0]->lly = (int)chip_boundary.lly;
            cong->V_Edge[i][0]->ury = (int)( (chip_boundary.ury-chip_boundary.lly) -
                                            ((chip_boundary.ury-chip_boundary.lly) / gcellgrid[1]->step * gcellgrid[1]->step) );

            // construct other rows V_Edge
            for(int j = 1; j < (int)cong->V_Edge[i].size(); j++)
            {
                cong->V_Edge[i][j] = new EDGE();
                cong->V_Edge[i][j]->lly = cong->V_Edge[i][j-1]->ury;
                cong->V_Edge[i][j]->ury = cong->V_Edge[i][j]->lly + gcellgrid[1]->step;
            }
            // x coordinate doesn`t assign value yet.
        }
    }
    else
    {
        // construct row V_Edge
        for(int i = 0; i < (int)cong->V_Edge.size(); i++)
        {
            // construct first row V_Edge
            cong->V_Edge[i][0] = new EDGE();
            cong->V_Edge[i][0]->lly = (int)chip_boundary.lly;
            cong->V_Edge[i][0]->ury = cong->V_Edge[i][0]->lly + gcellgrid[1]->step;

            // construct other rows V_Edge
            for(int j = 1; j < (int)cong->V_Edge[i].size(); j++)
            {
                cong->V_Edge[i][j] = new EDGE();
                cong->V_Edge[i][j]->lly = cong->V_Edge[i][j-1]->ury;
                cong->V_Edge[i][j]->ury = cong->V_Edge[i][j]->lly + gcellgrid[1]->step;
            }
            // x coordinate doesn`t assign value yet.
        }
    }

    // construct all H_Edge y coordinates
    for(int i = 0; i < (int)cong->H_Edge.size(); i++)
    {
        for(int j = 0; j < (int)cong->H_Edge[i].size(); j++)
        {
            if(i == 0)
            {
                cong->H_Edge[i][j]->lly = (int)chip_boundary.lly;
                cong->H_Edge[i][j]->ury = cong->H_Edge[i][j]->lly;
            }
            else if(i == 1)
            {
                cong->H_Edge[i][j]->lly = cong->H_Edge[i-1][j]->lly + (cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly);
                cong->H_Edge[i][j]->ury = cong->H_Edge[i][j]->lly;
            }
            else
            {
                cong->H_Edge[i][j]->lly = cong->H_Edge[i-1][j]->lly + gcellgrid[1]->step;
                cong->H_Edge[i][j]->ury = cong->H_Edge[i][j]->lly;
            }
        }
    }
    // construct all V_Edge x coordinates
    for(int i = 0; i < (int)cong->V_Edge.size(); i++)
    {
        for(int j = 0; j < (int)cong->V_Edge[i].size(); j++)
        {
            if(i == 0)
            {
                cong->V_Edge[i][j]->llx = (int)chip_boundary.llx;
                cong->V_Edge[i][j]->urx = cong->V_Edge[i][j]->llx;
            }
            else if(i == 1)
            {
                cong->V_Edge[i][j]->llx = cong->V_Edge[i-1][j]->llx + (cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx);
                cong->V_Edge[i][j]->urx = cong->V_Edge[i][j]->llx;
            }
            else
            {
                cong->V_Edge[i][j]->llx = cong->V_Edge[i-1][j]->llx + gcellgrid[0]->step;
                cong->V_Edge[i][j]->urx = cong->V_Edge[i][j]->llx;
            }
        }
    }
    /// calculate capacity of each edge
    cong->CalculateCapacity(cong);
}

bool CompareNetLength(CONGESTIONMAP::TWO_PIN_NET *A, CONGESTIONMAP::TWO_PIN_NET *B)
{
	return (A->wire_length < B->wire_length);
};

void CONGESTIONMAP::Flute2PinNet(CONGESTIONMAP *cong_info, bool sloveGP)
{
    //cout<<"   Convert All Nets to 2_Pin Nets ..."<<endl;
    TWO_PIN_NET *tmp_2pin_net;
    readLUT();

    DTYPE *X, *Y;
    Tree* fluteTree;
    fluteTree = (Tree*)calloc(net_list.size(), sizeof(Tree));       // dynamic allocate memory calloc(AMOUNT,SIZE)

    int stdID;
    int macroID;
    Macro *Macro_temp;
    int skipnet = 0;

    for(unsigned int i = 0; i < net_list.size(); i++)
    {
        unsigned int netdegree = net_list[i].cell_idSet.size() + net_list[i].macro_idSet.size();

        // [NOTE] if the net degree is very big skip to speed up
        if(netdegree > MAXDEGREE || netdegree == 0){
            skipnet++;
            continue;
        }

        // MODIFY 2020.02.26 /// change group_idSet to cell_idSet
        X = new DTYPE[netdegree]; // X pointer to a new DTYPE[]
        Y = new DTYPE[netdegree];

        unsigned int pin_num = 0;

        // MODIFY 2020.02.26 /// change group_idSet to cell_idSet
        for (unsigned int j = 0; j<net_list[i].cell_idSet.size(); j++)
        {

            stdID = net_list[i].cell_idSet[j];
            Macro_temp = macro_list[stdID];

            if(sloveGP == true) // using GP position
            {
                X[pin_num] = Macro_temp-> gp.llx + Macro_temp->w /2;   // do not consider pin info.
                Y[pin_num] = Macro_temp-> gp.lly + Macro_temp->h /2;   // do not consider pin info.
            }
            else // using cell at LG position which is moving by cell_placement.cpp
            {
                X[pin_num] = Macro_temp-> lg.llx + Macro_temp->w /2;   // do not consider pin info.
                Y[pin_num] = Macro_temp-> lg.lly + Macro_temp->h /2;   // do not consider pin info.
            }

            pin_num++;
        }
        for (unsigned int j = 0; j<net_list[i].macro_idSet.size(); j++)
        {

            macroID = net_list[i].macro_idSet[j];
            Macro_temp = macro_list[macroID];


            /// MODIFY 2020.03.27 /// handle the module's pins
            int pinid;
            int shiftX = 0, shiftY = 0;
            unsigned int pinnum = Macro_temp-> NetsID_to_pinsID[i].size();
            for(unsigned int k = 0;k< pinnum ; k++) // for multi pins
            {
                pinid = Macro_temp-> NetsID_to_pinsID[i].at(k);
                shiftX += pin_list[pinid]-> x_shift_plus;
                shiftY += pin_list[pinid]-> y_shift_plus;
            }
            if(pinnum != 0)
            {
                shiftX /= pinnum;
                shiftY /= pinnum;
            }
            else
            {
                shiftX += Macro_temp-> real_w /2;
                shiftY += Macro_temp-> real_h /2;
            }

            X[pin_num] = Macro_temp-> lg.llx + shiftX; // get average pin position
            Y[pin_num] = Macro_temp-> lg.lly + shiftY; // get average pin position


            pin_num++;
        }

        if (pin_num != netdegree )
        {
            cerr << "[Error] Pin num is  error when input the net connection pins" << endl;
            exit(1);
        }

        // [IMPORTANCE] the cell is at the same position it can resolve flute() segmentation fault
        bool isSame = true;
        for(unsigned int j = 0; j< pin_num-1; j++)
        {
            if(X[j] != X[j+1] || Y[j] != Y[j+1]){
                isSame = false;
                break;
            }
        }
        if (isSame == true){
            skipnet++;
            continue;
        }

        // [DEBUG] output for flute() all pin position
        /*
        cout << " deegree of net: " << pin_num << endl;
        for(int j=0; j< netdegree ; j++)
        {
            cout << X[j] << " " << Y[j] << endl;
        }*/

        fluteTree[i] = flute(pin_num, X, Y, 3);

        //cout << flute_wl(pin_num, X, Y, 3) << endl;

        // printtree();

        // rocord all two pin's position of net (it may be non-vertical/horizontal net)
        for (int j = 0; j < 2 * fluteTree[i].deg - 2; j++)
        {
            int x1 = fluteTree[i].branch[j].x;
            int y1 = fluteTree[i].branch[j].y;
            int x2 = fluteTree[i].branch[fluteTree[i].branch[j].n].x;
            int y2 = fluteTree[i].branch[fluteTree[i].branch[j].n].y;

            // ignore the net with the same pins
            if (!(x1 == x2 && y1 == y2))
            {
                // MODIFY from 2020.03.20// delete to random pin postiion
                tmp_2pin_net = new TWO_PIN_NET();
                tmp_2pin_net->x1 = x1;
                tmp_2pin_net->y1 = y1;
                tmp_2pin_net->x2 = x2;
                tmp_2pin_net->y2 = y2;
                tmp_2pin_net->wire_length = (int)(fabs(x1 - x2) + fabs(y1 - y2));
                cong_info->ary_2pinnet.push_back(tmp_2pin_net);
                //cout << " (x1,y1) = " << tmp_2pin_net->x1 <<"," << tmp_2pin_net->y1
                //     << " (x2,y2) = " << tmp_2pin_net->x2 <<"," << tmp_2pin_net->y2<< endl;
            }
        }
        delete X;
        delete Y;
    }
    //cout << "[INFO] Number of skip net: " << skipnet << endl;
    sort(cong_info->ary_2pinnet.begin(), cong_info->ary_2pinnet.end(), CompareNetLength);
}

void CONGESTIONMAP::PlotMacroObs(CONGESTIONMAP *cong, bool sloveGP)
{
    int obs_llx, obs_lly, obs_urx, obs_ury;
    Boundary macro_bound;

    // plot obstracle of four layer
    ofstream fout_0("./output/CongestionMap/MacroObstracle_ME1.dpx");
    ofstream fout_1("./output/CongestionMap/MacroObstracle_ME2.dpx");
    ofstream fout_2("./output/CongestionMap/MacroObstracle_ME3.dpx");
    ofstream fout_3("./output/CongestionMap/MacroObstracle_ME4.dpx");

    //chip boundary
    fout_0 << "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;
    //chip boundary
    fout_1 << "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;
    //chip boundary
    fout_2 << "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;
    //chip boundary
    fout_3 << "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;


    // 3.Macro has the routing obstacle needs to add to demand
    for(unsigned int i = 0 ; i < macro_list.size() ; i++)
    {
        Macro *this_macro = macro_list[i];
        // just for all movable macro and perplaced amcro
        if(this_macro-> macro_type != MOVABLE_MACRO && this_macro-> macro_type != PRE_PLACED)
            continue;

        int lef_id = this_macro-> lef_type_ID;
        LefMacroType &this_macrolef = DEFLEFInfo.LEF_Macros[lef_id];

        //for each macro obstacle which LEF_Macros can get the info.
        for(unsigned int j = 0; j < this_macrolef.rect.size() ;j++)
        {
            OBS_RECT *this_obs = this_macrolef.rect[j];

            if(sloveGP)
                macro_bound = this_macro-> gp;

            else
                macro_bound = this_macro-> lg;

            // According the orient to define the "obs"
            if(this_macro-> orient == "E")
            {
                obs_llx = macro_bound.llx + this_obs-> lly * PARA;
                obs_lly = macro_bound.ury - this_obs-> urx * PARA;
                obs_urx = macro_bound.llx + this_obs-> ury * PARA;
                obs_ury = macro_bound.ury - this_obs-> llx * PARA;

            }
            else if(this_macro-> orient == "W")
            {
                obs_llx = macro_bound.urx - this_obs-> ury * PARA;
                obs_lly = macro_bound.lly + this_obs-> llx * PARA;
                obs_urx = macro_bound.urx - this_obs-> lly * PARA;
                obs_ury = macro_bound.lly + this_obs-> urx * PARA;
            }
            else if(this_macro-> orient == "S")
            {
                obs_llx = macro_bound.urx - this_obs-> urx * PARA;
                obs_lly = macro_bound.ury - this_obs-> ury * PARA;
                obs_urx = macro_bound.urx - this_obs-> llx * PARA;
                obs_ury = macro_bound.ury - this_obs-> lly * PARA;
            }
            else if(this_macro-> orient == "N")
            {
                obs_llx = macro_bound.llx + this_obs-> llx * PARA;
                obs_lly = macro_bound.lly + this_obs-> lly * PARA;
                obs_urx = macro_bound.llx + this_obs-> urx * PARA;
                obs_ury = macro_bound.lly + this_obs-> ury * PARA;
            }
            else if(this_macro-> orient == "FE")
            {
                obs_llx = ((macro_bound.llx + macro_bound.urx)/2 + this_macro-> h/2) - this_obs-> ury * PARA;
                obs_lly = ((macro_bound.lly + macro_bound.ury)/2 + this_macro-> w/2) - this_obs-> urx * PARA;
                obs_urx = ((macro_bound.llx + macro_bound.urx)/2 + this_macro-> h/2) - this_obs-> lly * PARA;
                obs_ury = ((macro_bound.lly + macro_bound.ury)/2 + this_macro-> w/2) - this_obs-> llx * PARA;
            }
            else if(this_macro-> orient == "FW")
            {
                obs_llx = ((macro_bound.llx + macro_bound.urx)/2 - this_macro-> h/2) + this_obs-> lly * PARA;
                obs_lly = ((macro_bound.lly + macro_bound.ury)/2 - this_macro-> w/2) + this_obs-> llx * PARA;
                obs_urx = ((macro_bound.llx + macro_bound.urx)/2 - this_macro-> h/2) + this_obs-> ury * PARA;
                obs_ury = ((macro_bound.lly + macro_bound.ury)/2 - this_macro-> w/2) + this_obs-> urx * PARA;
            }
            else if(this_macro-> orient == "FS")
            {
                obs_llx = macro_bound.llx + this_obs-> llx * PARA;
                obs_lly = macro_bound.ury - this_obs-> ury * PARA;
                obs_urx = macro_bound.llx + this_obs-> urx * PARA;
                obs_ury = macro_bound.ury - this_obs-> lly * PARA;
            }
            else if(this_macro-> orient == "FN")
            {
                obs_llx = macro_bound.urx - this_obs-> urx * PARA;
                obs_lly = macro_bound.lly + this_obs-> lly * PARA;
                obs_urx = macro_bound.urx - this_obs-> llx * PARA;
                obs_ury = macro_bound.lly + this_obs-> ury * PARA;
            }

            if(obs_llx < chip_boundary.llx)     obs_llx = chip_boundary.llx;
            if(obs_lly < chip_boundary.lly)     obs_lly = chip_boundary.lly;
            if(obs_urx > chip_boundary.urx)     obs_urx = chip_boundary.urx;
            if(obs_ury > chip_boundary.ury)     obs_ury = chip_boundary.ury;

            int layer_id = this_obs-> blklayer;

            // if the layer isn't exist in routing_layer[] then skip
            if(layer_id == -1 ) continue;

            if(layer_id == 0)
            {
                fout_0 <<"COLOR " << "green" << endl;
                fout_0 <<"SRF "   << obs_llx <<" -"<< obs_lly <<" "<< obs_urx <<" -"<< obs_ury <<endl;
            }
            if(layer_id == 1)
            {
                fout_1 <<"COLOR " << "green" << endl;
                fout_1 <<"SRF "   << obs_llx <<" -"<< obs_lly <<" "<< obs_urx <<" -"<< obs_ury <<endl;
            }
            if(layer_id == 2)
            {
                fout_2 <<"COLOR " << "green" << endl;
                fout_2 <<"SRF "   << obs_llx <<" -"<< obs_lly <<" "<< obs_urx <<" -"<< obs_ury <<endl;
            }
            if(layer_id == 3)
            {
                fout_3 <<"COLOR " << "green" << endl;
                fout_3 <<"SRF "   << obs_llx <<" -"<< obs_lly <<" "<< obs_urx <<" -"<< obs_ury <<endl;
            }
        }
    }
    fout_0.close();
    fout_1.close();
    fout_2.close();
    fout_3.close();
}

void CONGESTIONMAP::AddObsDemand(CONGESTIONMAP *cong, bool sloveGP, int iter)
{
    int obs_llx, obs_lly, obs_urx, obs_ury;
    int h_pitch, v_pitch;
    Boundary macro_bound;
    
    /*start_x = track.V_routing_track_ori_x;
    start_y = track.H_routing_track_ori_y;
    end_x = track.V_routing_track_ori_x + ( track.V_routing_track_num - 1 ) * (int)track.V_routing_track_pitch; // this is not the real end GR dege
    end_y = track.H_routing_track_ori_y + ( track.H_routing_track_num - 1 ) * (int)track.H_routing_track_pitch;*/

    /// MODIFY 2021.03.09 ///

    // initial "LayerHasRect[]" vector size to the number of layer
    for(unsigned int i = 0 ; i < cong-> H_Edge.size() ; i++)
    {
        for(unsigned int j = 0 ; j < cong-> H_Edge[i].size(); j++)
        {
            cong-> H_Edge[i][j]-> LayerHasRect.resize(cong-> layer_num);
            // initial LayerHasRect = false
            for(int k = 0 ; k < cong-> layer_num ; k++)
                cong-> H_Edge[i][j]-> LayerHasRect[k] = false;
        }
    }

    for(unsigned int i = 0 ; i <  cong-> V_Edge.size() ; i++)
    {
        for(unsigned int j = 0 ; j < cong-> V_Edge[i].size() ; j++)
        {
            cong-> V_Edge[i][j]-> LayerHasRect.resize(cong-> layer_num);
            // initial LayerHasRect = false
            for(int k = 0 ; k < cong-> layer_num ; k++)
                cong-> V_Edge[i][j]-> LayerHasRect[k] = false;
        }
    }
    
    string file = "./output/CongestionMap/macro";
    file += int2str(iter);
    file += ".dpx";
    ofstream fout(file.c_str());
    fout << "COLOR black" << endl << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;

    // 3.Macro has the routing obstacle needs to add to demand
    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int V_edge_spacing = cong->V_Edge[0][1]->ury - cong->V_Edge[0][1]->lly;
    int H_edge_spacing = cong->H_Edge[0][1]->urx - cong->H_Edge[0][1]->llx;

    for(unsigned int i = 0 ; i < macro_list.size() ; i++)
    {
        Macro *this_macro = macro_list[i];
        // just for all movable macro and perplaced amcro
        if(this_macro-> macro_type != MOVABLE_MACRO && this_macro-> macro_type != PRE_PLACED)
            continue;

        int lef_id = this_macro-> lef_type_ID;
        LefMacroType &this_macrolef = DEFLEFInfo.LEF_Macros[lef_id];

        //if(this_macrolef.rect.size() !=0)
            //cout << "this_macrolef "<< this_macro-> macro_name << " "<< this_macro-> macro_type <<endl;

        //for each macro obstacle which LEF_Macros can get the info.
        for(unsigned int j = 0; j < this_macrolef.rect.size() ;j++)
        {
            OBS_RECT *this_obs = this_macrolef.rect[j];

            if(sloveGP)
                macro_bound = this_macro-> gp;

            else
                macro_bound = this_macro-> lg;

            // According the orient to define the "obs"
            if(this_macro-> orient == "E")
            {
                obs_llx = macro_bound.llx + this_obs-> lly * PARA;
                obs_lly = macro_bound.ury - this_obs-> urx * PARA;
                obs_urx = macro_bound.llx + this_obs-> ury * PARA;
                obs_ury = macro_bound.ury - this_obs-> llx * PARA;

            }
            else if(this_macro-> orient == "W")
            {
                obs_llx = macro_bound.urx - this_obs-> ury * PARA;
                obs_lly = macro_bound.lly + this_obs-> llx * PARA;
                obs_urx = macro_bound.urx - this_obs-> lly * PARA;
                obs_ury = macro_bound.lly + this_obs-> urx * PARA;
            }
            else if(this_macro-> orient == "S")
            {
                obs_llx = macro_bound.urx - this_obs-> urx * PARA;
                obs_lly = macro_bound.ury - this_obs-> ury * PARA;
                obs_urx = macro_bound.urx - this_obs-> llx * PARA;
                obs_ury = macro_bound.ury - this_obs-> lly * PARA;
            }
            else if(this_macro-> orient == "N")
            {
                obs_llx = macro_bound.llx + this_obs-> llx * PARA;
                obs_lly = macro_bound.lly + this_obs-> lly * PARA;
                obs_urx = macro_bound.llx + this_obs-> urx * PARA;
                obs_ury = macro_bound.lly + this_obs-> ury * PARA;
            }
            else if(this_macro-> orient == "FE")
            {
                obs_llx = ((macro_bound.llx + macro_bound.urx)/2 + this_macro-> h/2) - this_obs-> ury * PARA;
                obs_lly = ((macro_bound.lly + macro_bound.ury)/2 + this_macro-> w/2) - this_obs-> urx * PARA;
                obs_urx = ((macro_bound.llx + macro_bound.urx)/2 + this_macro-> h/2) - this_obs-> lly * PARA;
                obs_ury = ((macro_bound.lly + macro_bound.ury)/2 + this_macro-> w/2) - this_obs-> llx * PARA;
            }
            else if(this_macro-> orient == "FW")
            {
                obs_llx = ((macro_bound.llx + macro_bound.urx)/2 - this_macro-> h/2) + this_obs-> lly * PARA;
                obs_lly = ((macro_bound.lly + macro_bound.ury)/2 - this_macro-> w/2) + this_obs-> llx * PARA;
                obs_urx = ((macro_bound.llx + macro_bound.urx)/2 - this_macro-> h/2) + this_obs-> ury * PARA;
                obs_ury = ((macro_bound.lly + macro_bound.ury)/2 - this_macro-> w/2) + this_obs-> urx * PARA;
            }
            else if(this_macro-> orient == "FS")
            {
                obs_llx = macro_bound.llx + this_obs-> llx * PARA;
                obs_lly = macro_bound.ury - this_obs-> ury * PARA;
                obs_urx = macro_bound.llx + this_obs-> urx * PARA;
                obs_ury = macro_bound.ury - this_obs-> lly * PARA;
            }
            else if(this_macro-> orient == "FN")
            {
                obs_llx = macro_bound.urx - this_obs-> urx * PARA;
                obs_lly = macro_bound.lly + this_obs-> lly * PARA;
                obs_urx = macro_bound.urx - this_obs-> llx * PARA;
                obs_ury = macro_bound.lly + this_obs-> ury * PARA;
            }

            if(obs_llx < chip_boundary.llx)     obs_llx = chip_boundary.llx;
            if(obs_lly < chip_boundary.lly)     obs_lly = chip_boundary.lly;
            if(obs_urx > chip_boundary.urx)     obs_urx = chip_boundary.urx;
            if(obs_ury > chip_boundary.ury)     obs_ury = chip_boundary.ury;

            int layer_id = this_obs-> blklayer;
            //cout<<"layer_id: "<<layer_id<<endl;
            // if the layer isn't exist in routing_layer[] then skip or the layer is non METAL LAYER (layer_id == routing_layer.size() -1)
            if(layer_id == -1 || layer_id >= routing_layer.size() -1)
                continue;

            if(this_macro-> macro_type == MOVABLE_MACRO)
            {
                fout<<"COLOR "<<"blue"<<endl;
                fout<<"SRF "<<obs_llx<<" -"<<obs_lly<<" "<<obs_urx<<" -"<<obs_ury<<endl;
            }

            LAYER* this_layer = routing_layer[layer_id];
            // case 1 HORIZONTAL layer
            if(this_layer-> direction == "HORIZONTAL")
            {
                v_pitch = (this_layer->min_width + this_layer->min_spacing) * PARA;

                int col_r, col_l, row_from, row_to;

                col_r = ((obs_llx - (H_bias)) < 0) ? 1 : (obs_llx - (H_bias)) / V_edge_spacing + 2;
                col_l = ((obs_urx - (H_bias)) < 0) ? 0 : (obs_urx - (H_bias)) / V_edge_spacing + 1;

                row_from = ((obs_lly - (V_bias)) < 0) ? 0 : (obs_lly - (V_bias)) / V_edge_spacing + 1;
                row_to = ((obs_ury - (V_bias)) < 0) ? 0 : (obs_ury - (V_bias)) / V_edge_spacing + 1;

                col_l = (col_l >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_l;
                col_r = (col_r >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_r;
                row_from = (row_from >= (int)cong->V_Edge[0].size()) ? cong->V_Edge[0].size() - 1 : row_from;
                row_to = (row_to >= (int)cong->V_Edge[0].size()) ? cong->V_Edge[0].size() - 1 : row_to;

                for(int k = col_l ; k <= col_r ; k++)
                {
                    for(int l = row_from ; l <= row_to ; l++)
                    {
                        if(cong->V_Edge[k][l]->LayerHasRect[layer_id] == false)
                        {
                            if( cong->V_Edge[k][l]->llx >= obs_llx && cong->V_Edge[k][l]->llx <= obs_urx )
                            {
                                if(cong->V_Edge[k][l]->lly < obs_lly && cong->V_Edge[k][l]->ury > obs_lly)
                                {
                                    cong->V_Edge[k][l]->demand += floor((float)(cong->V_Edge[k][l]->ury - obs_lly) / v_pitch);
                                }
                                else if(cong->V_Edge[k][l]->lly >= obs_lly && cong->V_Edge[k][l]->ury <= obs_ury)
                                {
                                    cong->V_Edge[k][l]->demand += floor((float)(cong->V_Edge[k][l]->ury - cong->V_Edge[k][l]->lly) / v_pitch);
                                }
                                else if(cong->V_Edge[k][l]->lly <= obs_ury && cong->V_Edge[k][l]->ury > obs_ury)
                                {
                                    cong->V_Edge[k][l]->demand += floor((float)(obs_ury - cong->V_Edge[k][l]->lly) / v_pitch);
                                }
                                else if(cong->V_Edge[k][l]->lly < obs_lly && cong->V_Edge[k][l]->ury > obs_ury)
                                {
                                    cong->V_Edge[k][l]->demand += floor((float)(obs_ury - obs_lly) / v_pitch);
                                }
                                cong->V_Edge[k][l]->LayerHasRect[layer_id] = true;
                            }
                        }
                    }
                }
            }
            // case 2 VERTICAL layer
            else
            {
                h_pitch = (this_layer->min_width + this_layer->min_spacing) * PARA;

                int row_l, row_u, col_from, col_to;

                row_l = ((obs_lly - V_bias) < 0) ? 1 : (obs_lly - V_bias) / H_edge_spacing + 2;
                row_u = ((obs_ury - V_bias) < 0) ? 0 : (obs_ury - V_bias) / H_edge_spacing + 1;

                col_from = ((obs_llx - H_bias) < 0) ? 0 : (obs_llx - H_bias) / H_edge_spacing + 1;
                col_to = ((obs_urx - H_bias) < 0) ? 0 : (obs_urx - H_bias) / H_edge_spacing +1;

                row_l = (row_l >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_l;
                row_u = (row_u >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_u;
                col_from = (col_from >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_from;
                col_to = (col_to >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_to;

                for(int k = row_l; k <= row_u; k++)
                {
                    for(int l = col_from; l <= col_to; l++)
                    {
                        if(cong->H_Edge[k][l]->LayerHasRect[layer_id] == false)
                        {
                            if(cong->H_Edge[k][l]->lly >= obs_lly && cong->H_Edge[k][l]->lly <= obs_ury)
                            {
                                if(cong->H_Edge[k][l]->llx < obs_llx  && cong->H_Edge[k][l]->urx > obs_llx) // -|- |
                                {
                                    cong->H_Edge[k][l]->demand += floor((float)(cong->H_Edge[k][l]->urx - obs_llx) / h_pitch);
                                }
                                else if(cong->H_Edge[k][l]->llx >= obs_llx && cong->H_Edge[k][l]->urx <= obs_urx)   // |--|
                                {
                                    cong->H_Edge[k][l]->demand += floor((float)(cong->H_Edge[k][l]->urx - cong->H_Edge[k][l]->llx) / h_pitch);
                                }
                                else if(cong->H_Edge[k][l]->llx <= obs_urx && cong->H_Edge[k][l]->urx > obs_urx)    // | -|-
                                {
                                    cong->H_Edge[k][l]->demand += floor((float)(obs_urx - cong->H_Edge[k][l]->llx) / h_pitch);
                                }
                                else if(cong->H_Edge[k][l]->llx < obs_llx && cong->H_Edge[k][l]->urx > obs_urx)     // -|---|-
                                {
                                    cong->H_Edge[k][l]->demand += floor((float)(obs_urx - obs_llx) / h_pitch);
                                }
                                cong->H_Edge[k][l]->LayerHasRect[layer_id] = true;
                            }
                        }
                    }
                }
            }
        }
    }
    fout.close();
    // test for double
    /*for(int i = 0; i < (int)cong->H_Edge.size(); i++)
    {
        for(int j = 0; j < (int)cong->H_Edge[i].size(); j++)
        {
            if(cong->H_Edge[i][j]->demand > 0)
            {
                cout<<"("<<cong->H_Edge[i][j]->demand<<", "<<cong->H_Edge[i][j]->capacity<<") ";
                cout<<"H_Edge: "<<cong->H_Edge[i][j]->lly<<", "<<cong->H_Edge[i][j]->llx<<" to "<<cong->H_Edge[i][j]->urx<<endl;
                //getchar();
            }
        }
        //cout<<endl;
    }
    //getchar();
    for(int i = 0; i < (int)cong->V_Edge.size(); i++)
    {
        for(int j = 0; j < (int)cong->V_Edge[i].size(); j++)
        {
            if(cong->V_Edge[i][j]->demand > 0)
            {
                cout<<"("<<cong->V_Edge[i][j]->demand<<", "<<cong->V_Edge[i][j]->capacity<<") ";
                cout<<"V_Edge: "<<cong->V_Edge[i][j]->llx<<", "<<cong->V_Edge[i][j]->lly<<" to "<<cong->V_Edge[i][j]->ury<<endl;
                //getchar();
            }
        }
    }*/
}

void CONGESTIONMAP::InitializeCongestionMap(CONGESTIONMAP *cong)
{
    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int V_edge_spacing = cong->V_Edge[0][1]->ury - cong->V_Edge[0][1]->lly;
    int H_edge_spacing = cong->H_Edge[0][1]->urx - cong->H_Edge[0][1]->llx;

    for(int i = 0; i < (int)cong->ary_2pinnet.size(); i++)
    {
        int lx = min(cong->ary_2pinnet[i]->x1, cong->ary_2pinnet[i]->x2);
		int ly = min(cong->ary_2pinnet[i]->y1, cong->ary_2pinnet[i]->y2);
		int rx = max(cong->ary_2pinnet[i]->x1, cong->ary_2pinnet[i]->x2);
		int uy = max(cong->ary_2pinnet[i]->y1, cong->ary_2pinnet[i]->y2);

		// V_Edge
		int col_l, col_r, row_from, row_to;

		col_l = ((lx - H_bias) < 0) ? 1 : (lx - H_bias) / V_edge_spacing + 2;
		col_r = ((rx - H_bias) < 0) ? 0 : (rx - H_bias) / V_edge_spacing + 1;

		row_from = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / V_edge_spacing + 1;
		row_to = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / V_edge_spacing + 1;

		col_l = (col_l >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_l;
		col_r = (col_r >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_r;
		row_from = (row_from >= (int)cong->V_Edge[0].size()) ? (int)cong->V_Edge[0].size() - 1 : row_from;
		row_to = (row_to >= (int)cong->V_Edge[0].size()) ? (int)cong->V_Edge[0].size() - 1 : row_to;

		for(int j = row_from; j <= row_to; j++)
        {
            cong->V_Edge[col_l][j]->demand += 0.5;
            cong->V_Edge[col_r][j]->demand += 0.5;
        }

		// H_Edge
		int row_l, row_u, col_from, col_to;

		row_l = ((ly - V_bias) < 0) ? 1 : (ly - V_bias) / H_edge_spacing + 2;
		row_u = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / H_edge_spacing + 1;

		col_from = ((lx - H_bias) < 0) ? 0 : (lx - H_bias) / H_edge_spacing + 1;
		col_to = ((rx - H_bias) < 0) ? 0 : (rx - H_bias) / H_edge_spacing +1;

		row_l = (row_l >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_l;
		row_u = (row_u >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_u;
		col_from = (col_from >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_from;
		col_to = (col_to >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_to;

		for(int j = col_from; j <= col_to; j++)
        {
            cong->H_Edge[row_l][j]->demand += 0.5;
            cong->H_Edge[row_u][j]->demand += 0.5;
        }
    }
}

void CONGESTIONMAP::AddGRBINDemand(CONGESTIONMAP *cong)
{
    cout << " *Calculate routing demand" << endl;
    CONGESTIONMAP::InitializeCongestionMap(cong);

    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int V_edge_spacing = cong->V_Edge[0][1]->ury - cong->V_Edge[0][1]->lly;
    int H_edge_spacing = cong->H_Edge[0][1]->urx - cong->H_Edge[0][1]->llx;

    for(int i = 0; i < (int)cong->ary_2pinnet.size(); i++)
    {
        int lx = cong->ary_2pinnet[i]->x1;
		int ly = cong->ary_2pinnet[i]->y1;
		int rx = cong->ary_2pinnet[i]->x2;
		int uy = cong->ary_2pinnet[i]->y2;

		int topology = 0; // 0 : Left bottom / Right top, 1 : Left top / Right bottom
		int col_l, col_r, row_l, row_u;
		int row_from, row_to, col_from, col_to;

		if (lx == rx && ly == uy) continue;
		else
		{
			if (lx < rx)
			{
				if (ly < uy)
				{
					topology = 0;
					row_l = ((ly - V_bias) < 0) ? 1 : (ly - V_bias) / H_edge_spacing + 2;
                    row_u = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / H_edge_spacing + 1;
					row_from = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / V_edge_spacing + 1;
					row_to = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / V_edge_spacing + 1;
				}
				else
				{
					topology = 1;
					row_l = ((uy - V_bias) < 0) ? 1 : (uy - V_bias) / H_edge_spacing + 2;
                    row_u = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / H_edge_spacing + 1;
					row_from = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / V_edge_spacing + 1;
					row_to = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / V_edge_spacing + 1;
				}

				col_l = ((lx - H_bias) < 0) ? 1 : (lx - H_bias) / V_edge_spacing + 2;
                col_r = ((rx - H_bias) < 0) ? 0 : (rx - H_bias) / V_edge_spacing + 1;
                col_from = ((lx - H_bias) < 0) ? 0 : (lx - H_bias) / H_edge_spacing + 1;
                col_to = ((rx - H_bias) < 0) ? 0 : (rx - H_bias) / H_edge_spacing +1;
			}
			else
			{
				if (ly < uy)
				{
					topology = 1;
					row_l = ((ly - V_bias) < 0) ? 1 : (ly - V_bias) / H_edge_spacing + 2;
					row_u = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / H_edge_spacing + 1;
					row_from = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / V_edge_spacing + 1;
					row_to = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / V_edge_spacing + 1;
				}
				else
				{
					topology = 0;
					row_l = ((uy - V_bias) < 0) ? 1 : (uy - V_bias) / H_edge_spacing + 2;
					row_u = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / H_edge_spacing + 1;
					row_from = ((uy - V_bias) < 0) ? 0 : (uy - V_bias) / V_edge_spacing + 1;
					row_to = ((ly - V_bias) < 0) ? 0 : (ly - V_bias) / V_edge_spacing + 1;
				}

				col_l = ((rx - H_bias) < 0) ? 1 : (rx - H_bias) / V_edge_spacing + 2;
                col_r = ((lx - H_bias) < 0) ? 0 : (lx - H_bias) / V_edge_spacing + 1;
                col_from = ((rx - H_bias) < 0) ? 0 : (rx - H_bias) / H_edge_spacing + 1;
                col_to = ((lx - H_bias) < 0) ? 0 : (lx - H_bias) / H_edge_spacing +1;
			}
		}

		col_l = (col_l >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_l;
		col_r = (col_r >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_r;
		row_from = (row_from >= (int)cong->V_Edge[0].size()) ? (int)cong->V_Edge[0].size() - 1 : row_from;
		row_to = (row_to >= (int)cong->V_Edge[0].size()) ? (int)cong->V_Edge[0].size() - 1 : row_to;

		row_l = (row_l >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_l;
		row_u = (row_u >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_u;
		col_from = (col_from >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_from;
		col_to = (col_to >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_to;

		double upper_L_cost = 0, lower_L_cost = 0;
		int alpha = 10, beta = 2;

		if(topology == 0)
        {
            for (int j = row_from; j <= row_to; j++)
			{
				upper_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->V_Edge[col_l][j]->demand - cong->V_Edge[col_l][j]->capacity))));
				lower_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->V_Edge[col_r][j]->demand - cong->V_Edge[col_r][j]->capacity))));
			}
			for (int j = col_from; j <= col_to; j++)
			{
				upper_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->H_Edge[row_u][j]->demand - cong->H_Edge[row_u][j]->capacity))));
				lower_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->H_Edge[row_l][j]->demand - cong->H_Edge[row_l][j]->capacity))));
			}

			if (upper_L_cost < lower_L_cost)
			{
				for (int j = row_from; j <= row_to; j++)
				{
					cong->V_Edge[col_l][j]->demand += 0.5;
					cong->V_Edge[col_r][j]->demand -= 0.5;
				}
				for (int j = col_from; j <= col_to; j++)
				{
					cong->H_Edge[row_u][j]->demand += 0.5;
					cong->H_Edge[row_l][j]->demand -= 0.5;
				}
			}
			else
			{
				for (int j = row_from; j <= row_to; j++)
				{
					cong->V_Edge[col_r][j]->demand += 0.5;
					cong->V_Edge[col_l][j]->demand -= 0.5;
				}
				for (int j = col_from; j <= col_to; j++)
				{
					cong->H_Edge[row_l][j]->demand += 0.5;
					cong->H_Edge[row_u][j]->demand -= 0.5;
				}
			}
        }
        else
        {
            for (int j = row_from; j <= row_to; j++)
			{
				upper_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->V_Edge[col_r][j]->demand - cong->V_Edge[col_r][j]->capacity))));
				lower_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->V_Edge[col_l][j]->demand - cong->V_Edge[col_l][j]->capacity))));
			}
			for (int j = col_from; j <= col_to; j++)
			{
				upper_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->H_Edge[row_u][j]->demand - cong->H_Edge[row_u][j]->capacity))));
				lower_L_cost += 1 + (alpha / (1 + exp(-beta * (cong->H_Edge[row_l][j]->demand - cong->H_Edge[row_l][j]->capacity))));
			}

			if (upper_L_cost < lower_L_cost)
			{
				for (int j = row_from; j <= row_to; j++)
				{
					cong->V_Edge[col_r][j]->demand += 0.5;
					cong->V_Edge[col_l][j]->demand -= 0.5;
				}
				for (int j = col_from; j <= col_to; j++)
				{
					cong->H_Edge[row_u][j]->demand += 0.5;
					cong->H_Edge[row_l][j]->demand -= 0.5;
				}
			}
			else
			{
				for (int j = row_from; j <= row_to; j++)
				{
					cong->V_Edge[col_l][j]->demand += 0.5;
					cong->V_Edge[col_r][j]->demand -= 0.5;
				}
				for (int j = col_from; j <= col_to; j++)
				{
					cong->H_Edge[row_l][j]->demand += 0.5;
					cong->H_Edge[row_u][j]->demand -= 0.5;
				}
			}
        }
    }
}


void CONGESTIONMAP::Displaymatlab(CONGESTIONMAP *cong, bool SolveGP, int iter)
{
    cout << "   Output CongestionMap.m..." << endl;

    string outputfile;
    string outputfile2;
    if(SolveGP == true )
    {
        outputfile = "./output/CongestionMap/CongestionMap_GP_H_edge.dpx";
        outputfile2 = "./output/CongestionMap/CongestionMap_GP_V_edge.dpx";
    }
    else
    {
        outputfile = "./output/CongestionMap/CongestionMap_RF_H_edge";
        outputfile += int2str(iter);
        outputfile += ".dpx";

        outputfile2 = "./output/CongestionMap/CongestionMap_RF_V_edge";
        outputfile2 += int2str(iter);
        outputfile2 += ".dpx";
    }
    fstream fout(outputfile.c_str(), fstream::out);
    fout << "COLOR black" << endl;
    fout << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;

    // for all H_Edge
    for (unsigned int i = 0; i < cong->H_Edge.size(); i++)
    {
        for (unsigned int j = 0; j < cong->H_Edge.at(i).size(); j++)
        {

            int x1 = cong->H_Edge.at(i).at(j)-> llx;
            int x2 = cong->H_Edge.at(i).at(j)-> urx;
            int y1 = cong->H_Edge.at(i).at(j)-> lly;
            int y2 = cong->H_Edge.at(i).at(j)-> ury;
            int dm = cong->H_Edge.at(i).at(j)-> demand;

            if(dm > cong->H_Edge[i][j]->capacity  *2)
            {
                fout <<"COLOR " << "red"    <<endl;
                fout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            else if(dm > cong->H_Edge[i][j]->capacity  *1.75)
            {
                fout <<"COLOR " << "yellow" <<endl;
                fout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            else if(dm > cong->H_Edge[i][j]->capacity  *1.25)
            {
                fout <<"COLOR " << "green"  <<endl;
                fout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            else if(dm > cong->H_Edge[i][j]->capacity  *1)
            {
                fout <<"COLOR " << "blue"  <<endl;
                fout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            //else if(dm > cong->H_Edge[i][j]->capacity  *0.5)     fout <<"COLOR " << "blue"   <<endl;
            //else if(dm > cong->H_Edge[i][j]->capacity  *0.25)    fout <<"COLOR " << "black"  <<endl;

            /*if(dm > cong->H_Edge[i][j]->capacity  *0.25)
                fout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;*/

        }
    }
    fout.close();

    fstream ffout(outputfile2.c_str(), fstream::out);
    ffout << "COLOR black" << endl;
    ffout << "SR "<< chip_boundary.llx << " -" << chip_boundary.lly << " " << chip_boundary.urx << " -" << chip_boundary.ury << endl;
    // for all V_Edge
    for (unsigned int i = 0; i < cong->V_Edge.size(); i++)
    {
        for (unsigned int j = 0; j < cong->V_Edge.at(i).size(); j++)
        {

            int x1 = cong->V_Edge.at(i).at(j)-> llx;
            int x2 = cong->V_Edge.at(i).at(j)-> urx;
            int y1 = cong->V_Edge.at(i).at(j)-> lly;
            int y2 = cong->V_Edge.at(i).at(j)-> ury;
            int dm = cong->V_Edge.at(i).at(j)-> demand;

            if(dm > cong->V_Edge[i][j]->capacity  *2)
            {
                ffout <<"COLOR " << "red" <<endl;
                ffout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            else if(dm > cong->V_Edge[i][j]->capacity  *1.75)
            {
                ffout <<"COLOR " << "yellow" <<endl;
                ffout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            else if(dm > cong->V_Edge[i][j]->capacity  *1.25)
            {
                ffout <<"COLOR " << "green" <<endl;
                ffout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            else if(dm > cong->V_Edge[i][j]->capacity  *1)
            {
                ffout <<"COLOR " << "blue"  <<endl;
                ffout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;
            }
            //else if(dm > cong->V_Edge[i][j]->capacity  *0.5)     ffout <<"COLOR " << "blue" <<endl;
            //else if(dm > cong->V_Edge[i][j]->capacity  *0.25)    ffout <<"COLOR " << "black" <<endl;

            /*if(dm > cong->V_Edge[i][j]->capacity  *0.25)
                ffout <<"SR " << x1 <<" -"<< y1 <<" "<< x2 <<" -"<< y2 <<endl;*/

        }
    }
    ffout.close();

}

float CONGESTIONMAP::Calcost(CONGESTIONMAP *cong)
{
    cout << "   Calcurate congestionMap cost.." << endl;

    float cost = 0;
    // for all H_Edge
    for (unsigned int i = 0; i < cong->H_Edge.size(); i++)
    {
        for (unsigned int j = 0; j < cong->H_Edge.at(i).size(); j++)
        {
            cong->H_Edge[i][j]->overflow = (cong->H_Edge[i][j]->demand - cong->H_Edge[i][j]->capacity < 0) ? 0 : cong->H_Edge[i][j]->demand - cong->H_Edge[i][j]->capacity;

            cong->H_Edge[i][j]->penalty = 1 + PENALTY / (1 + exp((0-SLOPE) * cong->H_Edge[i][j]->overflow));

            /*if(cong->H_Edge[i][j]->demand > cong->H_Edge[i][j]->capacity * 0.75)
            {
                cost += (cong->H_Edge[i][j]->demand - cong->H_Edge[i][j]->capacity * 0.75);
            }*/
            if(cong->H_Edge[i][j]->overflow > 0)
            {
                cost += cong->H_Edge[i][j]->overflow;
            }
        }
    }
    for (unsigned int i = 0; i < cong->V_Edge.size(); i++)
    {
        for (unsigned int j = 0; j < cong->V_Edge.at(i).size(); j++)
        {
            int dm = cong->V_Edge.at(i).at(j)-> demand;
            cong->V_Edge[i][j]->overflow = (cong->V_Edge[i][j]->demand - cong->V_Edge[i][j]->capacity < 0) ? 0 : cong->V_Edge[i][j]->demand - cong->V_Edge[i][j]->capacity;

            cong->V_Edge[i][j]->penalty = 1 + PENALTY / (1 + exp((0-SLOPE) * cong->V_Edge[i][j]->overflow));

            /*if(dm > cong->V_Edge[i][j]->capacity * 0.75)
            {
                cost += (dm - cong->V_Edge[i][j]->capacity * 0.75);
            }*/
            if(cong->V_Edge[i][j]->overflow > 0)
            {
                cost += cong->V_Edge[i][j]->overflow;
            }
        }
    }
    return cost;
}

float CONGESTIONMAP::Enumerate_H_Edge(CONGESTIONMAP *cong, Boundary b, bool historical)
{
    float Congestion_Cost = 0;

    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int H_edge_spacing = cong->H_Edge[0][1]->urx - cong->H_Edge[0][1]->llx;

    int row_l, row_u, col_from, col_to;

    row_l = ((b.lly - V_bias) < 0) ? 1 : (b.lly - V_bias) / H_edge_spacing + 2;
    row_u = ((b.ury - V_bias) < 0) ? 0 : (b.ury - V_bias) / H_edge_spacing + 1;

    col_from = ((b.llx - H_bias) < 0) ? 0 : (b.llx - H_bias) / H_edge_spacing + 1;
    col_to = ((b.urx - H_bias) < 0) ? 0 : (b.urx - H_bias) / H_edge_spacing +1;

    row_l = (row_l >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_l;
    row_u = (row_u >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_u;
    col_from = (col_from >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_from;
    col_to = (col_to >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_to;

    for(int k = row_l; k <= row_u; k++)
    {
        for(int l = col_from; l <= col_to; l++)
        {
            if(cong->H_Edge[k][l]->llx < b.llx && cong->H_Edge[k][l]->urx > b.llx)
            {
                float Covered_Area = cong->H_Edge[k][l]->urx - b.llx;
                float Area = cong->H_Edge[k][l]->urx - cong->H_Edge[k][l]->llx;
                if(historical == true)
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->H_Edge[k][l]->penalty * ( 1 + cong->H_Edge[k][l]->historical_cost);
                else
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->H_Edge[k][l]->penalty * ( 1 + 0);
            }
            else if(cong->H_Edge[k][l]->llx >= b.llx && cong->H_Edge[k][l]->urx <= b.urx)
            {
                // Covered_Area = Area, thus = 1
                if(historical == true)
                    Congestion_Cost += (float)( 1 ) * cong->H_Edge[k][l]->penalty * ( 1 + cong->H_Edge[k][l]->historical_cost);
                else
                    Congestion_Cost += (float)( 1 ) * cong->H_Edge[k][l]->penalty * ( 1 + 0);
            }
            else if(cong->H_Edge[k][l]->llx < b.urx && cong->H_Edge[k][l]->urx > b.urx)
            {
                float Covered_Area = b.urx - cong->H_Edge[k][l]->llx;
                float Area = cong->H_Edge[k][l]->urx - cong->H_Edge[k][l]->llx;
                if(historical == true)
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->H_Edge[k][l]->penalty * ( 1 + cong->H_Edge[k][l]->historical_cost);
                else
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->H_Edge[k][l]->penalty * ( 1 + 0);
            }
        }
    }

    return Congestion_Cost;
}

float CONGESTIONMAP::Enumerate_V_Edge(CONGESTIONMAP *cong, Boundary b, bool historical)
{
    float Congestion_Cost = 0;

    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int V_edge_spacing = cong->V_Edge[0][1]->ury - cong->V_Edge[0][1]->lly;

    int col_r, col_l, row_from, row_to;

    col_r = ((b.llx - (H_bias)) < 0) ? 1 : (b.llx - (H_bias)) / V_edge_spacing + 2;
    col_l = ((b.urx - (H_bias)) < 0) ? 0 : (b.urx - (H_bias)) / V_edge_spacing + 1;

    row_from = ((b.lly - (V_bias)) < 0) ? 0 : (b.lly - (V_bias)) / V_edge_spacing + 1;
    row_to = ((b.ury - (V_bias)) < 0) ? 0 : (b.ury - (V_bias)) / V_edge_spacing + 1;

    col_l = (col_l >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_l;
    col_r = (col_r >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_r;
    row_from = (row_from >= (int)cong->V_Edge[0].size()) ? cong->V_Edge[0].size() - 1 : row_from;
    row_to = (row_to >= (int)cong->V_Edge[0].size()) ? cong->V_Edge[0].size() - 1 : row_to;

    for(int k = col_l ; k <= col_r ; k++)
    {
        for(int l = row_from ; l <= row_to ; l++)
        {
            if(cong->V_Edge[k][l]->lly < b.lly && cong->V_Edge[k][l]->ury > b.lly)
            {
                float Covered_Area = cong->V_Edge[k][l]->ury - b.ury;
                float Area = cong->V_Edge[k][l]->ury - cong->V_Edge[k][l]->lly;
                if(historical == true)
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->V_Edge[k][l]->penalty * (1 + cong->V_Edge[k][l]->historical_cost);
                else
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->V_Edge[k][l]->penalty * (1 + 0);
            }
            else if(cong->V_Edge[k][l]->lly >= b.lly && cong->V_Edge[k][l]->ury <= b.ury)
            {
                // Covered_Area = Area, thus = 1
                if(historical == true)
                    Congestion_Cost += (float)( 1 ) * cong->V_Edge[k][l]->penalty * (1 + cong->V_Edge[k][l]->historical_cost);
                else
                    Congestion_Cost += (float)( 1 ) * cong->V_Edge[k][l]->penalty * (1 + 0);
            }
            else if(cong->V_Edge[k][l]->lly < b.ury && cong->V_Edge[k][l]->ury > b.ury)
            {
                float Covered_Area = b.ury - cong->V_Edge[k][l]->lly;
                float Area = cong->V_Edge[k][l]->ury - cong->V_Edge[k][l]->lly;
                if(historical == true)
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->V_Edge[k][l]->penalty * (1 + cong->V_Edge[k][l]->historical_cost);
                else
                    Congestion_Cost += (float)(Covered_Area / Area) * cong->V_Edge[k][l]->penalty * (1 + 0);
            }
        }
    }

    return Congestion_Cost;
}




float Cal_H_congestion(CONGESTIONMAP *cong, Boundary b){
    float Congestion_Cost = 0;

    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int H_edge_spacing = cong->H_Edge[0][1]->urx - cong->H_Edge[0][1]->llx;

    int row_l, row_u, col_from, col_to;

    row_l = ((b.lly - V_bias) < 0) ? 1 : (b.lly - V_bias) / H_edge_spacing + 1;
    row_u = ((b.ury - V_bias) < 0) ? 0 : (b.ury - V_bias) / H_edge_spacing + 1;

    col_from = ((b.llx - H_bias) < 0) ? 0 : (b.llx - H_bias) / H_edge_spacing + 1;
    col_to = ((b.urx - H_bias) < 0) ? 0 : (b.urx - H_bias) / H_edge_spacing + 1;

    row_l = (row_l >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_l;
    row_u = (row_u >= (int)cong->H_Edge.size()) ? cong->H_Edge.size()-1 : row_u;
    col_from = (col_from >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_from;
    col_to = (col_to >= (int)cong->H_Edge[0].size()) ? (int)cong->H_Edge[0].size()-1 : col_to;
    // cout << H_edge_spacing << endl;
    // cout << row_l << " " << row_u << " " << col_from << " " << col_to << endl;
    for(int k = row_l; k <= row_u; k++)
    {
        for(int l = col_from; l <= col_to; l++)
        {
            if(cong->H_Edge[k][l]->llx < b.llx && cong->H_Edge[k][l]->urx > b.llx)
            {
                float Covered_Area = cong->H_Edge[k][l]->urx - b.llx;
                float Area = cong->H_Edge[k][l]->urx - cong->H_Edge[k][l]->llx;
                Congestion_Cost += (float)(Covered_Area / Area) * cong->H_Edge[k][l]->overflow;
            }
            else if(cong->H_Edge[k][l]->llx >= b.llx && cong->H_Edge[k][l]->urx <= b.urx)
            {
                Congestion_Cost += cong->H_Edge[k][l]->overflow;
            }
            else if(cong->H_Edge[k][l]->llx < b.urx && cong->H_Edge[k][l]->urx > b.urx)
            {
                float Covered_Area = b.urx - cong->H_Edge[k][l]->llx;
                float Area = cong->H_Edge[k][l]->urx - cong->H_Edge[k][l]->llx;
                Congestion_Cost += (float)(Covered_Area / Area) * cong->H_Edge[k][l]->overflow;
            }
        }
    }
    return Congestion_Cost;
}




float Cal_V_congestion(CONGESTIONMAP *cong, Boundary b){
    float Congestion_Cost = 0;

    int V_bias = cong->V_Edge[0][0]->ury - cong->V_Edge[0][0]->lly;    // the first edge might not be the same as step; Vertical bias
    int H_bias = cong->H_Edge[0][0]->urx - cong->H_Edge[0][0]->llx;    // the first edge might not be the same as step; Horizontal bias
    int V_edge_spacing = cong->V_Edge[0][1]->ury - cong->V_Edge[0][1]->lly;

    int col_r, col_l, row_from, row_to;

    col_l = ((b.llx - (H_bias)) < 0) ? 1 : (b.llx - (H_bias)) / V_edge_spacing + 1;
    col_r = ((b.urx - (H_bias)) < 0) ? 0 : (b.urx - (H_bias)) / V_edge_spacing + 1;
    row_from = ((b.lly - (V_bias)) < 0) ? 0 : (b.lly - (V_bias)) / V_edge_spacing + 1;
    row_to = ((b.ury - (V_bias)) < 0) ? 0 : (b.ury - (V_bias)) / V_edge_spacing + 1;

    col_l = (col_l >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_l;
    col_r = (col_r >= (int)cong->V_Edge.size()) ? (int)cong->V_Edge.size()-1 : col_r;
    row_from = (row_from >= (int)cong->V_Edge[0].size()) ? cong->V_Edge[0].size() - 1 : row_from;
    row_to = (row_to >= (int)cong->V_Edge[0].size()) ? cong->V_Edge[0].size() - 1 : row_to;
    // cout << V_edge_spacing << endl;
    // cout << col_l << " " << col_r << " " << row_from << " " << row_to << endl;
    for(int k = col_l ; k <= col_r ; k++)
    {
        for(int l = row_from ; l <= row_to ; l++)
        {
            if(cong->V_Edge[k][l]->lly < b.lly && cong->V_Edge[k][l]->ury > b.lly)
            {
                float Covered_Area = cong->V_Edge[k][l]->ury - b.lly;
                float Area = cong->V_Edge[k][l]->ury - cong->V_Edge[k][l]->lly;
                Congestion_Cost += (float)(Covered_Area / Area) * cong->V_Edge[k][l]->overflow;
            }
            else if(cong->V_Edge[k][l]->lly >= b.lly && cong->V_Edge[k][l]->ury <= b.ury)
            {
                Congestion_Cost += cong->V_Edge[k][l]->overflow;
            }
            else if(cong->V_Edge[k][l]->lly < b.ury && cong->V_Edge[k][l]->ury > b.ury)
            {
                float Covered_Area = b.ury - cong->V_Edge[k][l]->lly;
                float Area = cong->V_Edge[k][l]->ury - cong->V_Edge[k][l]->lly;
                Congestion_Cost += (float)(Covered_Area / Area) * cong->V_Edge[k][l]->overflow;
            }
            //Congestion_Cost += cong->V_Edge[k][l]->overflow;
        }
    }

    return Congestion_Cost;
}

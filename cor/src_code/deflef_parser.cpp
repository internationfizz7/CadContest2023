#include "deflef_parser.h"

/** container***************************************/
DEFLEF_INFO DEFLEFInfo;                             // just LEF module info. include "LEF_Macros" and "LefMacroName_ID"

Boundary chip_boundary;
vector<Boundary> Placement_blockage;

vector<Net_ML> net_list;                               // include vector<int> macro_idset and std_idset
vector<Macro*> macro_list;                          // pointer to macro

map<string, int> macroName_ID;                      // map macroName to macroID

/*******plus 107.10.16*********/
map<string, set<string> > DataFlow_outflow;
map<string, set<string> > DataFlow_inflow;

vector<STD_Group*> STDGroupBy;                      // STD cells group pointer
vector<vector<int> > Net_Std_Tmp;                   // this data structure isn't necessary just a temp. for nets with connected cells
Track track;

/*****************************/

//// MODIFY 2020.02.21////
//static vector<pair<float, float> > shift_xy;      // pin shifting x/y (but it can't map to macro's pin ?)
vector<Pin*> pin_list;                              // pointer to pin
vector<Pin*> PINs_tmp;                              // store PIN* from "LEF_pinCB" to "LEF_macroCB"


/// MODIFY 2020.07.10 /////
vector<pair<double,double> > polygon_tmp;           // the polygon is a tmp for the lefrSetMacroCbk() reading the macro
vector<OBS_RECT* > rect_tmp;                    // the rect is a tmp for the lefrSetMacroCbk() reading the macro

/// MODIFY 2020.07.17 /////
vector<LAYER*> routing_layer;

/// MODIFY 2021.03.04 ///
vector<GCELLGRID*> gcellgrid;

/** start program***************************************/


void ReadLefFile(string filename)
{
    char* userData = NULL;
    //cout<<"READ LEF : "<< filename<<endl;
    lefrInit();
    // routing layer for each metal layer is define in LEF
    lefrSetLayerCbk( LEF_LayerCB );

    lefrSetMacroCbk( LEF_macroCB );
    // [NOTE] while LEF_macroCB is input a macro, it will read "polygon" first in LEF_macroObsCB
    lefrSetObstructionCbk( LEF_macroObsCB );
    // [NOTE] while LEF_macroCB is input a macro, it will read "pin" first in LEF_pinCB
    lefrSetPinCbk( LEF_pinCB );



//    lefrReset();

    // Assume the file is LIST file, is has the .list file then read LIST or read a LEF
    ifstream fin;
    string lefList = filename;
    lefList += ".list";
    fin.open(lefList.c_str());

    // if IS NOT, just a LEF file
    if(!fin)
    {
        cout << "[INFO] The LEF File is only one" << endl;
        string file = filename;
        char lefList[100] = "\0";
        file =  filename;
        file += ".lef";

        //cout << "READ LEF File : " << file << endl;
        lefrReset();
        FILE* f;
        if( (f = fopen( file.c_str(), "r" ) ) == 0 )
        {
            cerr << "[WARNING] Can't open LEF file " << file << endl;
        }

        int res = lefrRead( f, file.c_str(), (void*)userData );

        if( res )
            cerr << "[WARNING] Reader returns bad status in file " << file << endl;
        (void)lefrReleaseNResetMemory();
        fclose( f );
    }
    // LIST file to read multi LEF
    else
    {
        cout << "[INFO] Read LEF File by a list" << endl;
        //cout<<"LEF list name : "<< filename<<endl;
        while(fin)
        {
            string lef_name = "";
            fin >> lef_name;
            string file = lef_name;
            lefrReset();
            FILE* f;
            //// MODIFY 2020.03.25 //// shik file == ""
            if(file != "")
            {
                if( (f = fopen( file.c_str(), "r" ) ) == 0 )
                {
                    cerr << "[WARNING] Can't open this LEF file :" << file << endl;
                    cerr << "   Key enter to continue...";
                    getchar();
                    continue;
                }

                int res = lefrRead( f, file.c_str(), (void*)userData );
                if( res )
                    cout << "[WARNING] Reader returns bad status in file " << file << endl;
                (void)lefrReleaseNResetMemory();
                fclose( f );
            }

        }
        fin.close();

    }
    // Output_LEF_info(); // outpupt LEF macro Type info. before read DEF file
}

bool Multi_lef(string filename)
{
    string attachment_name = "";
    bool attach_flag = false;
    for(int i = 0; i < (int)filename.size(); i++)
    {
        if(filename[i] == '.' && i != 0 && i !=1 ) //2020.02.03// skip the first and second '.',to judge the file is leflist or not // bad method
		{
            attach_flag = true;
        }
        if(attach_flag && filename[i] != '.')
        {
            attachment_name += filename[i];
        }
    }
    //cout<<"attachment_name : "<<attachment_name<<endl;

    if(attachment_name == "lef")
    {
        return false;    // only lef
    }
    return true;       // list file to read multi lef


}

int LEF_macroObsCB( lefrCallbackType_e c, lefiObstruction* obs, lefiUserData ud )
{
    lefiGeometries* geometry;
    lefiGeomPolygon* polygon;
    lefiGeomRect* geomrect;
    int numItems;
    pair<double,double> pair_tmp;
    polygon_tmp.clear();
    rect_tmp.clear();
    string layer_tmp = "";

    geometry = obs->geometries();
    numItems = geometry-> lefiGeometries::numItems();

    rect_tmp.reserve(numItems);
    for (int i = 0; i < numItems; i++)
    {
        if(geometry-> itemType(i) == lefiGeomPolygonE)
        {
            polygon = geometry-> getPolygon(i);
            //cout <<" POLYGON ";
            polygon_tmp.reserve(polygon-> numPoints);
            for (int j = 0; j < polygon-> numPoints; j++)
            {
                //cout << " (" << polygon->x[j] << ","<< polygon->y[j] << ") ";
                pair_tmp.first = polygon-> x[j];
                pair_tmp.second = polygon-> y[j];
                polygon_tmp.push_back( pair_tmp);
            }
        }
        if(geometry-> lefiGeometries::itemType(i) == lefiGeomLayerE)
			layer_tmp = geometry->lefiGeometries::getLayer(i);

        if(geometry-> lefiGeometries::itemType(i) == lefiGeomRectE)
        {
            OBS_RECT *new_rect = new OBS_RECT();
            new_rect-> blklayer = -1;
            /*
            for (unsigned int j = 0; j < routing_layer.size(); j++)
            {
                if(layer_tmp == routing_layer[j]->name)
                    new_rect-> blklayer = j;
            }*/

            // [NOTE] the layer info donen't exist in each cases, therefore directly define the layer name
            if(layer_tmp == "ME1" || layer_tmp == "M1") new_rect-> blklayer = 0;
            if(layer_tmp == "ME2" || layer_tmp == "M2") new_rect-> blklayer = 1;
            if(layer_tmp == "ME3" || layer_tmp == "M3") new_rect-> blklayer = 2;
            if(layer_tmp == "ME4" || layer_tmp == "M4") new_rect-> blklayer = 3;
            if(layer_tmp == "ME5" || layer_tmp == "M5") new_rect-> blklayer = 4;
            if(layer_tmp == "ME6" || layer_tmp == "M6") new_rect-> blklayer = 5;
            if(layer_tmp == "ME7" || layer_tmp == "M7") new_rect-> blklayer = 6;
            if(layer_tmp == "ME8" || layer_tmp == "M8") new_rect-> blklayer = 7;

            new_rect-> llx = geometry-> getRect(i)-> xl;
            new_rect-> urx = geometry-> getRect(i)-> xh;
            new_rect-> lly = geometry-> getRect(i)-> yl;
            new_rect-> ury = geometry-> getRect(i)-> yh;
            rect_tmp.push_back(new_rect);
        }

    }
    return 0;
}

int LEF_LayerCB(lefrCallbackType_e c, lefiLayer* layer, lefiUserData ud)
{
	if (strcmp(layer->lefiLayer::type(), "ROUTING") == 0)
	{
		LAYER *new_routing_layer = new LAYER();
		new_routing_layer-> name        = layer->lefiLayer::name();
		new_routing_layer-> direction   = layer-> lefiLayer::direction();
		new_routing_layer-> min_width   = layer-> lefiLayer::minwidth();
		new_routing_layer-> min_spacing = layer-> lefiLayer::pitch() - layer-> lefiLayer::minwidth();
		routing_layer.push_back(new_routing_layer);
	}

	return 0;
}

int LEF_macroCB( lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud )
{
    LefMacroType lefmacro_tmp;                                    // for define a macro type

    DEFLEFInfo.LefMacroName_ID[ macro->lefiMacro::name() ] = (int)DEFLEFInfo.LEF_Macros.size();

    lefmacro_tmp.macro_Name             = macro->lefiMacro::name();
    lefmacro_tmp.macroW_no_unit         = macro->lefiMacro::sizeX();        ///modify 108.03.23
    lefmacro_tmp.macroH_no_unit         = macro->lefiMacro::sizeY();        ///modify 108.03.23
    string type= macro->lefiMacro::macroClass();

    if (type == "CORE")         lefmacro_tmp.type = STD_CELL;
    else if (type == "BLOCK")   lefmacro_tmp.type = MOVABLE_MACRO;
    else if (type == "COVER")   lefmacro_tmp.type = PRE_PLACED;
    else if (type == "PAD")     lefmacro_tmp.type = PRE_PLACED;

  //// MODIFY 2020.02.21 ////                                   // delect this structure // this is fault
    //lefmacro_tmp.shiftXY_no_unit = shift_xy;                  // shiftXY_no_unit store pin info.(shift_xy)
    lefmacro_tmp.Pins.clear();
    lefmacro_tmp.Pins.reserve(PINs_tmp.size());
    for(unsigned int i= 0; i< PINs_tmp.size(); i++)
    {
        lefmacro_tmp.Pins.push_back(PINs_tmp[i]);               // input to lefmacro.Pins[]
    }
    PINs_tmp.clear();
/*
    ///// TEST for lefmacro_tmp.Pins ///// ok
    for(unsigned int i = 0 ; i < lefmacro_tmp.Pins.size(); i++)
    {
        cout<< lefmacro_tmp.Pins[i]-> pin_id    << "  "
            << lefmacro_tmp.Pins[i]-> pin_name  <<"  "
            << lefmacro_tmp.Pins[i]-> x_shift   <<"  "
            << lefmacro_tmp.Pins[i]-> y_shift   <<endl;
    }
    //getchar();
*/
    /*
    ///// TEST for shift_xy /////
    for(int i = 0 ; i < shift_xy.size(); i++)
    {
        cout<< shift_xy[i].first<< "  "<< shift_xy[i].second<<endl;
    }
    getchar();
    */
    //shift_xy.clear();

    //cout<<"in lef macro"<<endl;

    /// MODIFT 2020.07.10 /////  add POLYGON
    lefmacro_tmp.polygon.clear();
    lefmacro_tmp.polygon.reserve(polygon_tmp.size());
    for(unsigned int i = 0; i< polygon_tmp.size() ; i++)
    {
        lefmacro_tmp.polygon.push_back(polygon_tmp[i]);
    }
    polygon_tmp.clear();

    // check polygon value
    /*
    for(unsigned int i =0; i <lefmacro_tmp.polygon.size();i++)
    {
        cout << lefmacro_tmp.polygon[i].first <<"," << lefmacro_tmp.polygon[i].second <<endl;
    }*/

     /// MODIFT 2020.07.17 /////  add routing Layer
    lefmacro_tmp.rect.clear();
    lefmacro_tmp.rect.reserve(rect_tmp.size());
    for(unsigned int i = 0; i< rect_tmp.size() ; i++)
    {
        lefmacro_tmp.rect.push_back(rect_tmp[i]);
    }
    rect_tmp.clear();

    // check rect value
    /*
    for(unsigned int i =0; i <lefmacro_tmp.rect.size();i++)
    {
        cout << lefmacro_tmp.rect[i]->blklayer << endl;
        cout << lefmacro_tmp.rect[i]->llx <<"," << lefmacro_tmp.rect[i]->lly <<","
             << lefmacro_tmp.rect[i]->urx <<"," << lefmacro_tmp.rect[i]->ury<<endl;
    }*/

    DEFLEFInfo.LEF_Macros.push_back(lefmacro_tmp);
//    cout<<DEFLEFInfo.LEF_Macros.back().macro_Name<<endl;
//    cout<<"DEFLEFInfo.LEF_Macros.back().macroW : "<<DEFLEFInfo.LEF_Macros.back().macroW<<"\tDEFLEFInfo.LEF_Macros.back().macroH"<<DEFLEFInfo.LEF_Macros.back().macroH<<endl;
//    cout<<"end lef macro"<<endl ;
    return 0;
}

int LEF_pinCB(lefrCallbackType_e c, lefiPin* pin, lefiUserData ud) // pdf p 287
{
    int numPorts;
    lefiGeometries* geometry;

//    cout<<"in lef pin"<<endl;
//    string pin_name;
//    pin_name=pin->lefiPin::name();

//    cout<<"pin name " <<pin_name<<endl;


//    if(pin->lefiPin::hasDirection())
//    {
//        string direction = pin->lefiPin::direction();
//        if( direction == "INPUT")
//            temp_pin.is_output=0;
//        else
//            temp_pin.is_output=1;
////        cout<<"*"<<pin->lefiPin::direction()<<"* "<<temp_pin.is_output<<endl;
//    }

    Pin* pin_tmp        = new Pin;
    pin_tmp-> pin_name  = pin->lefiPin::name();
    pin_tmp-> type      = pin->lefiPin::use();
    pin_tmp-> io        = pin->lefiPin::direction();

    numPorts= pin->lefiPin::numPorts();
//  DEFLEFInfo.LEF_Macros.push_back(lefmacro_tmp);
//  cout<<shift_xy.size()<<endl;getchar();
//  cout<<"numPorts : "<<numPorts<<endl;
    for(int i=0; i< numPorts; i++)                                  // for all ports in one pin
    {
        geometry= pin->lefiPin::port(i);                            // pdf p293

        /// MODIFY 2020.02.21 ////
        //pair<int, int> shift_xy_temp;                             //for cal.center of gravity
        //int &shiftx_tmp= shift_xy_temp.first;
        //int &shifty_tmp= shift_xy_temp.second;

        ///// MODIFY  2020.02.21 ////  delete prtGeometry(geometry,shift_xy_temp);

        int numItems = geometry->lefiGeometries::numItems();
        lefiGeomRect* rect;

        int llx_shift = INT_MAX, urx_shift = INT_MIN,               // temp for find port's llx/y, urx/y shift
            lly_shift = INT_MAX, ury_shift = INT_MIN;
        for(int j=0; j< numItems; j++)                              // a port has many rect !
        {
            // p258 still have a lot of information if you want to use you can call these functions.
            switch(geometry->lefiGeometries::itemType(j))
            {
            case 8:
                rect = geometry->lefiGeometries::getRect(j);

            //// MODIFY 2020.02.21 ////                             //cal.rect shify X/Y
                //shiftx_tmp += (int)(rect -> xl + rect -> xh) /2;  //modify 108.03.23
                //shifty_tmp += (int)(rect -> yl + rect -> yh) /2;  //modify 108.03.23
                if(rect->xl < llx_shift) llx_shift = rect->xl;
                if(rect->xh > urx_shift) urx_shift = rect->xh;
                if(rect->yl < lly_shift) lly_shift = rect->yl;
                if(rect->yh > ury_shift) ury_shift = rect->yh;

                break;
            default:
                break;
            }
        }
   //// MODIFY 2020.02.21 ////                                      // port info.
        //shiftx_tmp = (int)(shiftx_tmp / (double)numItems);        //for cal.center of gravity
        //shifty_tmp = (int)(shifty_tmp / (double)numItems);        //for cal.center of gravity
        //cout<<"pin x : "<<shiftx_tmp<<"\ty : "<<shifty_tmp<<endl;

        Port* port_tmp = new Port;
        port_tmp-> llx_shift = llx_shift;
        port_tmp-> lly_shift = lly_shift;
        port_tmp-> urx_shift = urx_shift;
        port_tmp-> ury_shift = ury_shift;

        pin_tmp-> Ports.push_back(port_tmp);
    }

    //shift_xy.push_back(shift_xy_temp);                            // only push a pin shiftX/Y
//// MODIFY 2020.02.21 //////                                       // pin info.
    int llx_shift = INT_MAX, urx_shift = INT_MIN,                   // temp for find pin's llx/y, urx/y shift
        lly_shift = INT_MAX, ury_shift = INT_MIN;
    for(unsigned int i=0; i< pin_tmp->Ports.size(); i++)                     // a port has many rect !
    {
        if(pin_tmp-> Ports[i]->llx_shift < llx_shift) llx_shift = pin_tmp-> Ports[i]-> llx_shift;
        if(pin_tmp-> Ports[i]->urx_shift > urx_shift) urx_shift = pin_tmp-> Ports[i]-> urx_shift;
        if(pin_tmp-> Ports[i]->lly_shift < lly_shift) lly_shift = pin_tmp-> Ports[i]-> lly_shift;
        if(pin_tmp-> Ports[i]->ury_shift > ury_shift) ury_shift = pin_tmp-> Ports[i]-> ury_shift;
    }
    pin_tmp-> x_shift = (llx_shift + urx_shift) / 2 ;
    pin_tmp-> y_shift = (lly_shift + ury_shift) / 2 ;
    PINs_tmp.push_back(pin_tmp);

    //temp_pin.shiftx_tmp=pin_x;
    //temp_pin.shifty_tmp=pin_y;
    /*
    cout<< "vector size "<< pin_shift.size()<<"  "<< temp_pin.shift_xy.size()<<endl;
    for(i=0; i< temp_pin.shift_xy.size(); i++)
    {
        cout<<" final to push"<<endl;
        cout<< temp_pin.shift_xy[i].first<< "  "<<temp_pin.shift_xy[i].second<<endl;
    }*/
    return 0;
}

static set<string> orient_insert;

void ReadDefFile( string filename )
{
    char* userData = NULL;

    //cout<<"DEF : "<< filename <<endl;
    orient_insert.insert("N") ;
    orient_insert.insert("S") ;
    orient_insert.insert("FN") ;
    orient_insert.insert("FS") ;

    defrInit();
    /** Initial Call Routine */

    defrSetUnitsCbk(DEF_UnitCB);            ///modify 108.03.23
    defrSetDieAreaCbk (DEF_diearea);//2

    defrSetComponentStartCbk(DEF_compoentstartCB); //7
    defrSetComponentCbk( DEF_componentCB); //8
    defrSetPinCbk(DEF_pinCB); //9

    defrSetNetStartCbk(DEF_netstart);
    defrSetNetCbk( DEF_netsCB ); //10

    defrSetBlockageCbk ( Def_blockageCB );

    //read ourting track
    defrSetTrackCbk (DEF_trackCB); ///plus 108.07.14

    //read routing gcellgrid
    defrSetGcellGridCbk(DEF_gcellCB);   ///modify 2021.03.04
//getchar();
    defrReset();

    FILE* f;
    if( (f=fopen(filename.c_str(), "r")) == 0 )
    {
        cout << "[ERROR] Can't open <DEF> file from command path " << filename << endl;
        exit(2);
    }
//    cout<<"res"<<endl;getchar();
    int res = defrRead( f, filename.c_str(), (void*)userData, 1 );

    if( res )
        cout << "Reader returns bad status in file " << filename << endl;
//    cout<<"mem"<<res<<endl;getchar();
    (void) defrReleaseNResetMemory();
//    cout<<"def end"<<endl;

    /*for(int i = 0; i < Net_Std_Tmp.size(); i++)
    {
        cout<<"Net "<<i<<endl;
        for(int j = 0; j < Net_Std_Tmp[i].size(); j++)
        {
            cout << Net_Std_Tmp[i][j]<<endl;
        }
        getchar();
    }*/
}

int DEF_UnitCB(defrCallbackType_e typ, double unit, defiUserData ud)
{
    PARA = (int)unit; // PARA = 1000 (LEF * PARA = DEF unit)
    return 0;
}

int DEF_diearea (defrCallbackType_e c, defiBox* diebox, defiUserData ud) //ok
{
//    cout<<"in diearea"<<endl;
    int llx = numeric_limits<int>::max(), urx = numeric_limits<int>::min(), lly = numeric_limits<int>::max(), ury = numeric_limits<int>::min();
    struct defiPoints points = diebox->getPoint();
    for(int i=0; i<points.numPoints; i++)
    {
        if(points.x[i] < llx)
            llx = points.x[i];
        if(points.x[i] > urx)
            urx = points.x[i];

        if(points.y[i] < lly)
            lly = points.y[i];
        if(points.y[i] > ury)
            ury = points.y[i];
    }

    chip_boundary.llx = llx;
    chip_boundary.lly = lly;
    chip_boundary.urx = urx;
    chip_boundary.ury = ury;
    // cout<<chip_boundary.llx<<"\t"<<chip_boundary.lly<<"\t"<<chip_boundary.urx<<"\t"<<chip_boundary.ury<<endl;

    /*******modify 108.03.21******/
    if(Congestion_PARA == true)
    {
        if(chip_boundary.llx < 0 || chip_boundary.lly < 0)
        {
            shifting_X = 0 - chip_boundary.llx;
            shifting_Y = 0 - chip_boundary.lly;
            chip_boundary.llx = chip_boundary.llx + shifting_X;  // =0
            chip_boundary.lly = chip_boundary.lly + shifting_Y;  // =0
            chip_boundary.urx = chip_boundary.urx + shifting_X;  // =-llx
            chip_boundary.ury = chip_boundary.ury + shifting_Y;  // =-lly
        }
    }
    else
    {
        //cerr<<"[WARNING] Do not consider congestion "<< endl;
        //cerr << "   Key enter to continue...";
        //getchar();
    }
    /*************************/


    return 0;
}

int DEF_compoentstartCB(defrCallbackType_e type,int numcompoent,defiUserData userData) //ok
{
//    cout<<"DEF_compoentstartCB"<<endl;
//    getchar();
    macro_list.resize(numcompoent);

    //cout<<macro_list[0]->w<<endl;
    //cout<<"[INFO] Number of Components: " <<numcompoent<<endl;
    //getchar();
    return 0;

}

int cell_count=0;

int DEF_componentCB (defrCallbackType_e type,defiComponent* compInfo,defiUserData userData) // compoent p64 ok
{
//    cout<<"in DEF_componetCB"<<endl;
    macroName_ID[compInfo->id()]=cell_count;

    Macro* macro_temp = new Macro;
//    cout<<"cell compoent count"<<cell_count<<endl;

//    printf("%s %s ", compInfo->id(), compInfo->name());
//    cout<<"print"<<endl;
//    if (compInfo->hasNets())
//    {
//        for (i = 0; i < compInfo->numNets(); i++)
//            printf("%s ", compInfo->net(i));
//        printf("\n");
//    }
//    cout<<"print net"<<endl;
    int orient = 0;
    orient = compInfo->placementOrient();
    macro_temp->gp.llx = compInfo->placementX();
    macro_temp->gp.lly = compInfo->placementY();
    macro_temp->macro_id = cell_count;
//    cout<<"END"<<endl;
    if (compInfo->isFixed())
    {

//        printf(" FIXED %d %d %d\n", compInfo->placementX(),compInfo->placementY(),compInfo->placementOrient());

        macro_temp->macro_type = PRE_PLACED;
        macro_temp->gp.llx = compInfo->placementX() + shifting_X;       ///modify 108.03.21
        macro_temp->gp.lly = compInfo->placementY() + shifting_Y;       ///modify 108.03.21
    }
    else if (compInfo->isCover())
    {
//        printf(" COVER %d %d %d\n", compInfo->placementX(),compInfo->placementY(),compInfo->placementOrient());
        macro_temp->macro_type = PRE_PLACED;
        macro_temp->gp.llx = compInfo->placementX() + shifting_X;       ///modify 108.03.21
        macro_temp->gp.lly = compInfo->placementY() + shifting_Y;       ///modify 108.03.21
    }
    else if (compInfo->isPlaced())
    {
//        printf(" PLACED %d %d %d\n", compInfo->placementX(),compInfo->placementY(),compInfo->placementOrient());
        macro_temp->macro_type = STD_CELL;
    }
    else
    {
        macro_temp->macro_type = STD_CELL;
    }
//    cout<<"END"<<endl;

    switch(orient)
    {
    case 0 :
        macro_temp -> orient = 'N' ;
        break;
    case 1 :
        macro_temp -> orient = 'W' ;
        break;
    case 2 :
        macro_temp -> orient = 'S' ;
        break;
    case 3 :
        macro_temp -> orient = 'E' ;
        break;
    case 4 :
        macro_temp -> orient = "FN" ;
        break;
    case 5 :
        macro_temp -> orient = "FW" ;
        break;
    case 6 :
        macro_temp -> orient = "FS" ;
        break;
    case 7 :
        macro_temp -> orient = "FE" ;
        break;
    }
//    cout<<"END"<<endl;

    map<string,int >::iterator it1;
    map<string,int > &Lefname_ID = DEFLEFInfo.LefMacroName_ID;
    it1 = Lefname_ID.find(compInfo->name());

    if(it1!=Lefname_ID.end())
    {
        LefMacroType &lefmacro_tmp = DEFLEFInfo.LEF_Macros[ it1->second ];

        /*******************modify 108.03.23**********************/

        lefmacro_tmp.macroW = (int)(lefmacro_tmp.macroW_no_unit * PARA);
        lefmacro_tmp.macroH = (int)(lefmacro_tmp.macroH_no_unit * PARA);
        /*
        //// MODIFY 2020.02.21 ////
        lefmacro_tmp.shiftXY.resize(lefmacro_tmp.shiftXY_no_unit.size());               // plus PARA to DEF unit
        for(int i = 0; i<(int)lefmacro_tmp.shiftXY_no_unit.size(); i++)
        {
            lefmacro_tmp.shiftXY[i].first = (int)(lefmacro_tmp.shiftXY_no_unit[i].first * PARA);
            lefmacro_tmp.shiftXY[i].second = (int)(lefmacro_tmp.shiftXY_no_unit[i].second * PARA);
        }
        */

        for(unsigned int i = 0; i< lefmacro_tmp.Pins.size(); i++)                       // plus PARA to DEF unit
        {
            lefmacro_tmp.Pins[i]-> x_shift_plus = lefmacro_tmp.Pins[i]-> x_shift * PARA;
            lefmacro_tmp.Pins[i]-> y_shift_plus = lefmacro_tmp.Pins[i]-> y_shift * PARA;
        }
        /**********************************************/

        macro_temp->w = lefmacro_tmp.macroW;
        macro_temp->h = lefmacro_tmp.macroH;
        macro_temp->area = (float)(macro_temp->w / (float)PARA  * macro_temp->h / (float)PARA); // area is down size of PARP

        macro_temp->lef_type_ID = it1->second;
        macro_temp->macro_name =compInfo->id();

        //// MODIFY 2020.02.21 ////
        //macro_temp->pin_num = (int)lefmacro_tmp.shiftXY.size();
        macro_temp-> pin_num = lefmacro_tmp.Pins.size();

        lefmacro_tmp.macro_count++;

        if(macro_temp->macro_type == STD_CELL)
        {
            if(lefmacro_tmp.type == MOVABLE_MACRO)
            {
                macro_temp->macro_type = MOVABLE_MACRO;
//                cout<<macro_temp->macro_name<<"\t"<<macro_temp->h<<endl;
            }
        }

        // MODIFY for PA5663
        /*
        if( macro_temp-> w > 1500000 || macro_temp-> h > 1500000 )
            cout << "[MODIFY] IO PAD NAME: " << compInfo->id();

        if( macro_temp-> macro_name == "u_combo_txphy_wrap/U0_COMBOTXPHY" || macro_temp-> macro_name == "u_combo_txphy_wrap/U0_IP0898_07A_COMBOTXPHY")
        {
            macro_temp->h = 560000;
            cout << "\tmodify h = 560" << endl;
        }
        if( macro_temp-> macro_name == "u_combo_txphy_wrap/U1_COMBOTXPHY" || macro_temp-> macro_name == "u_combo_txphy_wrap/U1_IP0898_07A_COMBOTXPHY")
        {
            macro_temp->h = 560000;
            cout << "\tmodify h = 560" << endl;
        }
        if( macro_temp-> macro_name == "dram_phy_wrap_u0/PHY_dram_u0/dramphy_u0")
        {
            macro_temp->h = 690000;
            macro_temp-> gp.llx = 4975000;
            cout << "\tmodify h = 690, llx = 4975" << endl;
        }*/

        set<string>::iterator iter ;
        iter = orient_insert.find(macro_temp->orient) ;             // "orient_insert" is include these string : N S FN FS
        if(iter != orient_insert.end())
        {
            macro_temp -> cal_w           = macro_temp -> w ;
            macro_temp -> cal_h           = macro_temp -> h ;
            macro_temp -> cal_w_wo_shrink = macro_temp -> w ;
            macro_temp -> cal_h_wo_shrink = macro_temp -> h ;
            macro_temp -> real_w          = macro_temp -> w ;
            macro_temp -> real_h          = macro_temp -> h ;
        }
        else                                                        // the orient is not N S FN FS, swap w and h value!
        {
            macro_temp -> cal_w           = macro_temp -> h ;
            macro_temp -> cal_h           = macro_temp -> w ;
            macro_temp -> cal_w_wo_shrink = macro_temp -> h ;
            macro_temp -> cal_h_wo_shrink = macro_temp -> w ;
            macro_temp -> real_w          = macro_temp -> h ;
            macro_temp -> real_h          = macro_temp -> w ;
        }

        macro_temp->gp.urx = macro_temp->gp.llx + macro_temp->cal_w_wo_shrink;
        macro_temp->gp.ury = macro_temp->gp.lly + macro_temp->cal_h_wo_shrink;

        ////MODIFY 2020.02.21 //// insert to map :IncName_to_MacroType
        //LefMacroType *lefM_tmp = &(lefmacro_tmp);
        //IncName_to_MacroType[macro_temp-> macro_name] = lefM_tmp;       // input map incName to MacroType
    }
    else
    {
        cout<<"[ERROR] Can't find component type : "<< compInfo->name() << " in LEF while reading DEF"<<endl;
        exit(1);
    }

    macro_list[cell_count] = macro_temp;

    cell_count++;
    return 0;
}


//int cell_count1=0;
//
int DEF_pinCB (defrCallbackType_e type,defiPin* pinInfo,defiUserData userData) // P77 ok
{
    // Do not input unfixed pin info.           // these ports are not placed, so the coor. (x,y) are = 0
    if (pinInfo->hasPlacement())
    {

        if(!(pinInfo->isPlaced() || pinInfo->isFixed()))
            return 0;

        int macro_id = (int)macro_list.size();
        macroName_ID[pinInfo->pinName()] = macro_id;

        Macro* macro_temp           = new Macro;
        macro_temp  ->macro_type    = PORT;
        macro_temp  ->macro_name    = pinInfo-> pinName();
        macro_temp  ->macro_id      = macro_id;
        macro_temp  ->gp.llx        = macro_temp->gp.urx = pinInfo-> defiPin::placementX(); // = 0 ?
        macro_temp  ->gp.lly        = macro_temp->gp.ury = pinInfo-> defiPin::placementY(); // = 0 ?

        macro_list.push_back(macro_temp);
    }
    return 0;
}

int Def_blockageCB ( defrCallbackType_e c,defiBlockage* block,defiUserData ud )
{
    if (block->defiBlockage::hasPlacement())
    {
        Placement_blockage.reserve(block->defiBlockage::numRectangles() + 4);
        for (int i = 0; i < block->defiBlockage::numRectangles(); i++)
        {
            //cout<<"xl "<<block->defiBlockage::xl(i)<<" yl "<<block->defiBlockage::yl(i)<<" xh "<<block->defiBlockage::xh(i)<<" yh "<<block->defiBlockage::yh(i)<<endl;
            Boundary tmp;
            tmp.llx = max(block->defiBlockage::xl(i) + shifting_X, chip_boundary.llx);      ///modify 108.03.21
            tmp.lly = max(block->defiBlockage::yl(i) + shifting_Y, chip_boundary.lly);      ///modify 108.03.21
            tmp.urx = min(block->defiBlockage::xh(i) + shifting_X, chip_boundary.urx);      ///modify 108.03.21
            tmp.ury = min(block->defiBlockage::yh(i) + shifting_Y, chip_boundary.ury);      ///modify 108.03.21

            Placement_blockage.push_back(tmp);
        }

        Dummy_Blockage();
    }

    return 0;
}

void Dummy_Blockage()
{
    Boundary Pseudo_Blkage;
    Boundary_Assign(Pseudo_Blkage, chip_boundary.llx, chip_boundary.lly, chip_boundary.llx + 1, chip_boundary.ury);
    Placement_blockage.push_back(Pseudo_Blkage);
    Boundary_Assign(Pseudo_Blkage, chip_boundary.urx - 1, chip_boundary.lly, chip_boundary.urx, chip_boundary.ury);
    Placement_blockage.push_back(Pseudo_Blkage);
    Boundary_Assign(Pseudo_Blkage, chip_boundary.llx, chip_boundary.lly, chip_boundary.urx, chip_boundary.lly + 1);
    Placement_blockage.push_back(Pseudo_Blkage);
    Boundary_Assign(Pseudo_Blkage, chip_boundary.llx, chip_boundary.ury - 1, chip_boundary.urx, chip_boundary.ury);
    Placement_blockage.push_back(Pseudo_Blkage);
}

int DEF_netstart(defrCallbackType_e type,int netnumber,defiUserData userData)//ok
{
    //cout<<"[INFO] Number of Nets: "<< netnumber<<endl;
    net_list.reserve(netnumber);
    Net_Std_Tmp.reserve(netnumber);    ///plus 108.04.26
    return 0;
}

int DEF_netsCB(defrCallbackType_e c, defiNet* net, defiUserData ud)//ok
{
    //p 273
    // cout<<"in net"<<endl;

    ////////////  MODIFY from 2020.02.21 //////////
    Net_ML net_tmp;
    net_tmp.NetID = (int)net_list.size();                                           // assign net_list[] index
    net_tmp.net_Name = net-> defiNet::name();

    set<int> macro_idSet_tmp;                                                       // store net info. by a <set> can selete multi pin at same macro
    //set<int> cell_idSet_tmp;
    vector<int> cell_idSet_tmp;

    //bool include_movable = false;

    //transfer instance name which connect the net to "macro_list" id
    pin_list.reserve(pin_list.size()+net->defiNet::numConnections());

    for( int i = 0; i < (int)net->defiNet::numConnections(); i++ )                  // for all cconnection macro and its the pin
    {
        string instance_name_tmp = net->defiNet::instance(i);
        string pin_name_tmp = net->defiNet::pin(i);
        if(instance_name_tmp == "PIN") {                                            // "PAD" is store its pin Name in macro_list[]
                instance_name_tmp = pin_name_tmp;
                //cout << pin_name_tmp <<endl;
        }

        map<string, int>::iterator iter = macroName_ID.find(instance_name_tmp);

        if(iter != macroName_ID.end())                                              // find out the macro/cell/pin
        {

            int macro_id = iter->second;
                                                   // which instance is connected
            Macro* macro_temp = macro_list[macro_id];

            if(macro_temp->macro_type != STD_CELL )                                 // for MOVABLE_MACRO / PRE_PLACED / PORT
            {
                macro_idSet_tmp.insert(macro_id);                                   // input macro_idSet_tmp
                //if(macro_temp->macro_type == MOVABLE_MACRO) include_movable = true; // define include_movable

        /////////////////// MODIFY 2020.02.21 ///////////////////////////           // add macro's "NetsID_to_pinsID" info.

                if(macro_temp->macro_type != PORT )                                 // **PORT do not have "lef_type_ID"
                {
                    int lef_type_id = macro_temp-> lef_type_ID;

                    if(lef_type_id != -1)
                    {
                        LefMacroType* lefmacro_tmp;
                        lefmacro_tmp = & ( DEFLEFInfo.LEF_Macros[lef_type_id] );    // USE "LEF_Macros" map to find its LEF module name
                        Pin* pin_tmp = new Pin;
                        pin_tmp = NULL;

                        for(unsigned int j = 0; j < lefmacro_tmp->Pins.size(); j++)
                        {
                            if(pin_name_tmp == lefmacro_tmp-> Pins[j]-> pin_name)
                            {
                                pin_tmp = lefmacro_tmp-> Pins[j];                   // assign pin
                                break;
                            }
                        }
                        if(pin_tmp != NULL)
                        {
                            pin_tmp-> pin_id = pin_list.size();                     // pin_id is pin_list[] index
                            pin_tmp-> macroID = lef_type_id;
                            pin_list.push_back(pin_tmp);
                            //cout << pin_tmp -> pin_id<<"\t" << pin_tmp-> x_shift
                            //     << "\t"<<pin_tmp ->y_shift << endl;
                            //getchar();
                            // MODIFY 2020.03.27 //
                            //pair<int,int> pair_tmp;
                            //pair_tmp.first = net_tmp.NetID;
                            //pair_tmp.second = pin_tmp-> pin_id;
                            //macro_temp-> netsID_to_pinsID.push_back(pair_tmp);      // assign a pair of <netID to pinID> in the macro
                            map<int,vector<int> >::iterator itermap;
                            itermap = macro_temp-> NetsID_to_pinsID.find(net_tmp.NetID);
                            if(itermap != macro_temp-> NetsID_to_pinsID.end())
                            {
                                itermap-> second.push_back(pin_tmp-> pin_id);
                            }
                            else
                            {
                                vector<int> pins_tmp;
                                pins_tmp.push_back(pin_tmp-> pin_id);
                                macro_temp-> NetsID_to_pinsID[net_tmp.NetID] = pins_tmp;
                            }

                        }
                        else
                        {
                            cerr << "[WARNING] Can't find LEF macro 's pin: \t" << pin_name_tmp;
                            cerr << "\tMacro Type Name: \t"         << lefmacro_tmp-> macro_Name    << endl;
                            cerr << "   Key enter to continue...";

                            getchar();
                        }
                    }
                    else
                    {

                        cerr << "[WARNING] Can't find this inistence name's Type \t"<< instance_name_tmp <<endl;
                        cerr << "   Key enter to continue...";
                        getchar();
                    }
                }

        /////////////////////   END MODIDY   //////////////////
            }
            else if(macro_temp->macro_type == STD_CELL )                                // macro_type == STD_CELL
            {
                //if(Cluster_PARA == true)
                //{
                    cell_idSet_tmp.insert(cell_idSet_tmp.begin(),macro_id);           // do not consider cell's pin
                //}
            }

        }
        else
        {
            string instance_tmp = net->defiNet::instance(i);
            if(instance_tmp != "PIN")                                                   // if there are some non-fix port do not consider
            {
                cerr << "[WARNING] None Fixed Port\t" << instance_tmp <<"\t"<< net->defiNet::pin(i)<<endl;
                cerr << "   Key enter to continue...";
                getchar();
            }

        }
    }

    /////////////////////////////////// After assign macro_idSet_tmp and cell_idSet_tmp  ///////////////////////////////////////////////////


    net_tmp.macro_idSet.reserve( net->defiNet::numConnections() );                      // for speed up
    //net_tmp.group_idSet.reserve( net->defiNet::numConnections() );                      // "group_idSet" can't modify, while using vector type and Net_Std_Tmp[][]


    ////////////  MODIFY from 2020.02.17 //////////
    //if(include_movable && ( macro_idSet_tmp.size() + cell_idSet_tmp.size() ) > 1)      // only consider the net is connect to moveable macro add info.

    if(  macro_idSet_tmp.size() + cell_idSet_tmp.size() >=2)
    {
        net_tmp.macro_idSet.resize(macro_idSet_tmp.size());                             // macro_idSet has these number of macros
        //net_tmp.group_idSet.resize(cell_idSet_tmp.size());                               // "group_idSet"  doesn't store cells id

        ////////////  MODIFY from 2020.02.17 ////////// rewrite for loop end condiction about "net_list[]" "macro_idset" and "std_idset"
/*
        int index_tmp = 0;
        set<int>::iterator iter;
        for(iter = macro_idSet_tmp.begin(); iter != macro_idSet_tmp.end() ; iter++)     // for all connected macros
        {
            int macro_id = *iter; // macro_idSet_tmp's value

            net_tmp.macro_idSet[index_tmp++] = macro_id;

            //// TEST macro_idSet ///////////// always only one macro ?
            cout << macro_idSet_tmp.size() << endl;
            cout << macro_id << endl;
            cout << index_tmp << endl;
            cout << net_tmp.macro_idSet[1] <<endl;
            if (macro_idSet_tmp.size() >1)getchar();

            macro_list[macro_id]->NetID.push_back(net_tmp.NetID);
        }

        if(Cluster_PARA == true)
        {
            index_tmp = 0;
            for(iter = cell_idSet_tmp.begin(); iter != cell_idSet_tmp.end() ; iter++)     // for all connected cells
            {
                int macro_id = *iter; // cell_idSet_tmp's value

                net_tmp.group_idSet[index_tmp++] = macro_id;

                macro_list[macro_id]->NetID.push_back(net_tmp.NetID);
            }


            //Net_Std_Tmp.push_back(cell_idSet_tmp); // "net_std" not to used
        }
*/
        // NOT TO UESD//


        set<int>::iterator iter = macro_idSet_tmp.begin();
        for(int i = 0; i < (int)macro_idSet_tmp.size(); i++)
        {
            int macro_id = (*iter); // <SET>'s value

            net_tmp.macro_idSet[i] = macro_id;

            macro_list[macro_id]-> NetID.push_back(net_tmp.NetID);
            iter++;
        }

        //if(Cluster_PARA == true)
        //{
            //net_tmp.group_idSet.resize(cell_idSet_tmp.size());                       // can't change "group_idSet" result will change

            vector<int>::iterator iter_std = cell_idSet_tmp.begin();
            for(int i = 0; i < (int)cell_idSet_tmp.size(); i++)
            {
                int macro_id = (*iter_std);
                net_tmp.cell_idSet.push_back(macro_id);

                //[NOTE] net_ID's group_idSet is input data at deflef_parser.cpp p.1699

                ////// MODIFY  from 2020.02.18/////// input "macro_list[] -> NetID" of cells
                macro_list[macro_id]-> NetID.push_back(net_tmp.NetID);               // open
                iter_std++;
            }

            /*
            for(int i =0;i< cell_idSet_tmp.size(); i++)
            {
                cout<<cell_idSet_tmp[i]<<" ";

            }
            cout <<endl;
            getchar();
            */

            Net_Std_Tmp.push_back(cell_idSet_tmp);   ///108.04.26  // "Net_Std_Tmp[][]" must to used
            //[NOTE] be used to net_ID's group_idSet is input data at deflef_parser.cpp p.1699
        //}
        net_list.push_back(net_tmp);

        ///// TEST for NetID and macro_idSet and cell_idSet_tmp /////
        /*
        cout << "NetID: " << net_tmp.NetID  << endl;
        cout << "Macro num: " << macro_idSet_tmp.size() << endl;
        cout << "STD num: " << cell_idSet_tmp.size() << endl;
        cout << "push in macro_idSet: "<< net_list[net_tmp.NetID].macro_idSet.size() << endl;
        cout << "push in cell_idSet_tmp: " << cell_idSet_tmp.size() << endl;
        getchar();
        */
    }


    return 0;
}

/*****************plus 108.07.13*********************/

// read routing track
int DEF_trackCB(defrCallbackType_e type, defiTrack* trackInfo, defiUserData userData)
{
    //Track *track;
    //cout << "in track" << endl;

    if (strcmp(trackInfo->macro(), "X") == 0)
    {
        //track = new TRACK();
        //track->vertical = 1;
        if(track.V_routing_track_ori_x==0 && track.V_routing_track_num==0 && track.V_routing_track_pitch==0)
        {
            track.V_routing_track_ori_x = (int)trackInfo->x();
            track.V_routing_track_num = (int)trackInfo->xNum();
            track.V_routing_track_pitch = trackInfo->xStep();
        }

        /*for (unsigned int i = 0; i<par_nt->metal_layer.size(); i++)
        {
            if (par_nt->metal_layer.at(i)->layer_name == trackInfo->layer(0))
                track->metal_layer = par_nt->metal_layer.at(i);
        }
        if (track->metal_layer == par_nt->top_layer2)
            par_nt->V_routing_track = track;*/
        //track->name = "V_TRACK_" + track->metal_layer->layer_name;
        //cout << "XXX " << trackInfo->macro() << " , " << track.ori_x << " , " << track.track_num << " , " << track.track_pitch;
    }
    else
    {
        //track = new TRACK();
        //track->vertical = 0;
        if(track.H_routing_track_ori_y==0 && track.H_routing_track_num==0 && track.H_routing_track_pitch==0)
        {
            track.H_routing_track_ori_y = (int)trackInfo->x();
            track.H_routing_track_num = (int)trackInfo->xNum();
            track.H_routing_track_pitch = trackInfo->xStep();
        }
        /*for (unsigned int i = 0; i< par_nt->metal_layer.size(); i++)
        {
            if (par_nt->metal_layer.at(i)->layer_name == trackInfo->layer(0))
                track->metal_layer = par_nt->metal_layer.at(i);
        }
        if (track->metal_layer == par_nt->top_layer1)
            par_nt->H_routing_track = track;*/
        //track->name = "H_TRACK_" + track->metal_layer->layer_name;
        //cout << "YYY " << trackInfo->macro() << " , " << track.ori_x << " , " << track.track_num << " , " << track.track_pitch;
    }
    for (int i = 0; i < trackInfo->numLayers(); i++)
    {
        string Track_name = trackInfo->layer(i);
        string tracktemp = "";
        for(unsigned int j = 0; j < Track_name.size()-1; j++)
        {
            tracktemp += Track_name[j];
        }
        if(tracktemp == "ME" || tracktemp == "metal" || tracktemp == "M")
        {
            track.routing_layer_str.insert(Track_name);
        }
    }
    //par_nt->track.push_back(track);
    return 0;
}

/***************************************************/

/*****************modify 2021.03.04*********************/ //m108yjhuang
bool sort_by_direction(GCELLGRID *a, GCELLGRID *b)
{
    return a->direction < b->direction;
}

int DEF_gcellCB(defrCallbackType_e type, defiGcellGrid* gcellInfo, defiUserData userData)
{
    //cout<<"GCELLGRID "<<gcellInfo->macro()<<" "<<gcellInfo->x()<<" "<<gcellInfo->xNum()<<" "<<gcellInfo->xStep()<<endl;
    if (type != defrGcellGridCbkType)
    {
        printf("Type is not defrGcellGridCbkType, terminateparsing.\n");
        return 1;
    }
    else
    {
        if(gcellgrid.size() == 0)
        {
            GCELLGRID* gcell_temp = new GCELLGRID;
            gcell_temp->direction = gcellInfo->macro();
            gcell_temp->num = gcellInfo->xNum();
            gcell_temp->step_const = gcellInfo->xStep();

            gcellgrid.push_back(gcell_temp);
        }
        else
        {
            bool the_same_info = false;
            for(int i = 0; i < (int)gcellgrid.size(); i++)
            {
                if( gcellgrid[i]->direction == gcellInfo->macro() &&
                    gcellgrid[i]->num == gcellInfo->xNum() &&
                    gcellgrid[i]->step_const == gcellInfo->xStep() )
                {
                    the_same_info = true;
                    break;
                }
            }
            if(the_same_info == false)
            {
                GCELLGRID* gcell_temp = new GCELLGRID;
                gcell_temp->direction = gcellInfo->macro();
                gcell_temp->num = gcellInfo->xNum();
                gcell_temp->step_const = gcellInfo->xStep();

                gcellgrid.push_back(gcell_temp);
            }
        }

    }
    sort(gcellgrid.begin(), gcellgrid.end(), sort_by_direction);
    //printf("GCELLGRID %s %d DO %d STEP %g\n", gcellInfo->macro(),gcellInfo->x(), gcellInfo->xNum(), gcellInfo->xStep());

    return 0;
}
/****************************************************/
// pl file is Gobal placement rsult output file
void Load_pl(string filename)
{
    ifstream fin (filename.c_str());
    string trash ;

    if(!fin)
    {
        cout<<"[ERROR] Can't open <pl> file from command path"<<filename<<endl;
        exit(1);
    }

    getline(fin,trash);
    getline(fin,trash);
    int id_count = 0 ;
    set<string>::iterator iter ;
    while(fin)
    {
        string macro_name ;
        string fixed = "";
        getline(fin,trash);
        istringstream ffin(trash);
        if(trash == "")
        {
            continue ;
        }
        double x, y ;
        ffin >> macro_name >> x >> y >> trash; // input macro name and x/y postion

        map<string, int>::iterator it = macroName_ID.find(macro_name);
        if(it == macroName_ID.end())
        {
            cerr<<"[WARNING] Can't not find module "<< macro_name <<" while read pl file. "<<endl;
            //cerr << "   Key enter to continue...";
            //getchar();
            continue;
        }
        id_count = it->second;
//        cout<<"ID : "<<id_count<<endl;

        Macro* macro_temp = macro_list.at(id_count);
        ffin >> macro_list.at(id_count)-> orient >>fixed;               // input orient (E W S N )

        if(macro_temp->macro_type == PRE_PLACED)
        {
            continue;                                                   //do not change position
        }

        iter = orient_insert.find(macro_list.at(id_count)->orient) ;    // orient_insert is include these string : N S FN FS
        //x = (int)x;
        //y = (int)y;

        if(iter != orient_insert.end())                                 // the orient is not N S FN FS
        {
            macro_temp -> cal_w             = macro_temp -> w ;
            macro_temp -> cal_h             = macro_temp -> h ;
            macro_temp -> cal_w_wo_shrink   = macro_temp -> w ;
            macro_temp -> cal_h_wo_shrink   = macro_temp -> h ;
            macro_temp -> real_w            = macro_temp -> w ;
            macro_temp -> real_h            = macro_temp -> h ;
        }
        else                                                            // the orient is N S FN FS  // swap w and h value ?
        {
            macro_temp -> cal_w             = macro_temp -> h ;
            macro_temp -> cal_h             = macro_temp -> w ;
            macro_temp -> cal_w_wo_shrink   = macro_temp -> h ;
            macro_temp -> cal_h_wo_shrink   = macro_temp -> w ;
            macro_temp -> real_w            = macro_temp -> h ;
            macro_temp -> real_h            = macro_temp -> w ;
        }


        macro_temp->gp.llx = (int)x + shifting_X;                            ///modify 108.03.21
        macro_temp->gp.lly = (int)y + shifting_Y;                            ///modify 108.03.21
        macro_temp->gp.urx = (int)x + shifting_X + macro_temp -> cal_w ;     ///modify 108.03.21
        macro_temp->gp.ury = (int)y + shifting_X + macro_temp -> cal_h ;     ///modify 108.03.21

        macro_temp->lg = macro_temp->gp;
//        if(macro_temp->macro_type == MOVABLE_MACRO)
//        {
//            Boundary_Cout(macro_list[id_count]->gp);
//            cout<<"^^^^^^^^^^^^^^^^^^^"<<endl;
//        }

    }
//    for(int i = 0; i < (int)macro_list.size(); i++)
//    {
//        Macro* macro_temp = macro_list[i];
//        if(macro_temp->macro_type == MOVABLE_MACRO)
//        {
//            if(!(macro_list[id_count]->gp.llx > 0 && macro_list[id_count]->gp.lly > 0))
//            {
//                cout<<">>>>>>>>>>>>>><<<<<<<<<<<<<<<<"<<endl;
//            }
//        }
//    }
    fin.close();
}



//////////////////////////  MAIN FUNCTION ////////////////////////////

void Call_deflef_parser(int argc, char* argv[])
{
    string lef_name = "", def_name = "", loadpl_name = "", output_name = "", tf_name;

    Congestion_PARA = false;     ///plus 108.07.26

    for(int i = 0; i < argc; i++)
    {
        string operation = argv[i];
        if(operation == "-lefdef")
        {
            i++;
            // the lef def file is without Filename Extension
            lef_name = argv[i];
            def_name = argv[i];
            def_name += ".def";
            /// ADD 2021.03.12 ///
            tf_name = argv[i];
            tf_name += ".tf";
        }
        else if (operation == "-loadpl")
        {
            i++;
            loadpl_name = argv[i];
        }
        else if(operation == "-o")
        {
            i++;
            output_name = argv[i];
        }
        else if(operation == "-Refine")  ///plus 108.07.26
        {
            i++;
            Congestion_PARA = atoi(argv[i]);
        }
        else if(operation == "-debug")
        {
            i++;
            Debug_PARA = atoi(argv[i]);
        }
    }

    // setting "Cluster_PARA" = true or fault
    //Check_GP_cluster(argc, argv);       ///plus 108.04.26

    cout<<"   Now Read Lef file: " << lef_name << endl;
    ReadLefFile(lef_name);

    if (routing_layer.empty())
		ReadTechFile(tf_name);

    if(Debug_PARA)
    {
        cout << "[INFO] Routing layer num: "<< routing_layer.size() << endl;
        //if(routing_layer.size() == 0)
           //cout << "[WARNING] The layer information is missing, do not consider the obstracle of the macro" << endl;

        for(unsigned int i=0 ; i< routing_layer.size() ; i++)
        {
            cout << "[INFO] layer name: " << routing_layer[i]-> name << endl;
            cout << "[INFO] layer direction: " << routing_layer[i]-> direction << endl;
            cout << "[INFO] layer min width: " << routing_layer[i]-> min_width << endl;
            cout << "[INFO] layer min spacing: " << routing_layer[i]-> min_spacing << endl;
        }
    }

    cout<<"   Now Read Def file: " << def_name << endl;
    ReadDefFile(def_name);

    if(loadpl_name != "")
    {
        cout<<"   Now Read PL file: " << loadpl_name <<endl;
        //cout<< "PL FILE pATH :"<<loadpl_name <<endl;
        Load_pl(loadpl_name);
    }
    else{
        cout << "[WARNING] Do not input pl file." <<endl;
        cout << "   Key enter to continue...";
        getchar();
    }




    cout<<"[INFO] Number of macro_list: "  << macro_list.size() << endl;
    cout<<"[INFO] Number of net_list: "    << net_list.size() << endl;

    //cout<<"----------   macro data at  "macro_list"    ---------------"<<endl;
    //cout<<"----------   row in the     "row_vector"    ---------------"<<endl;
    //cout<<"----------   net in the     "net_list"      ---------------"<<endl;
    //cout<<"----------   pin in the     "pin_data"      ---------------"<<endl;
    //cout<<"----------   region in the  "region_map"    ---------------"<<endl;


    def_file = def_name;          ///modify 108.03.26

    /*********plus 108.07.14**********/


    //Output routing track info
    track.V_routing_layer_num = (track.routing_layer_str.size() +1 ) /2;
    track.H_routing_layer_num =  track.routing_layer_str.size() - track.V_routing_layer_num;

    if(Debug_PARA)
    {
        cout<<"[INFO] Routing_layer_num: "      <<track.V_routing_layer_num + track.H_routing_layer_num <<endl;
        cout<<"[INFO] V/H_routing_layer_num: "  <<track.V_routing_layer_num<<"  "<<track.H_routing_layer_num<<endl;
        cout<<"[INFO] V/H_routing_track_num: "  <<track.V_routing_track_num<<"  "<<track.H_routing_track_num<<endl;
        cout<<"[INFO] V/H_routing_track_ori_x: "<<track.V_routing_track_ori_x<<"  "<<track.H_routing_track_ori_y<<endl;
        cout<<"[INFO] V/H_routing_track_pitch: "<<track.V_routing_track_pitch<<"  "<<track.H_routing_track_pitch<<endl;

    }


    if(track.V_routing_layer_num == 0 && track.H_routing_layer_num == 0)
    {
        cerr<<"[WARNING] No track info."<<endl;
        if(Congestion_PARA == true)
        {
            cerr<<"[ERROR] Please check track information in def file "<< endl;
            exit(1);
        }
    }

    /*if(routing_layer.size() == 0)
    {
        int lay_nmu = track.V_routing_layer_num + track.H_routing_layer_num;
        cout << "[WARNING] The layer information is missing, define the layer: ME1 to ME" << lay_nmu << endl;

        if(lay_nmu >=2)
        {
            LAYER *new_routing_layer1 = new LAYER();
            new_routing_layer1-> name        = "ME1";
            new_routing_layer1-> direction   = "HORIZONTAL";
            routing_layer.push_back(new_routing_layer1);
            LAYER *new_routing_layer2 = new LAYER();
            new_routing_layer2-> name        = "ME2";
            new_routing_layer2-> direction   = "VERTICAL";
            routing_layer.push_back(new_routing_layer2);
        }

        if(lay_nmu >=4)
        {
            LAYER *new_routing_layer3 = new LAYER();
            new_routing_layer3-> name        = "ME3";
            new_routing_layer3-> direction   = "HORIZONTAL";
            routing_layer.push_back(new_routing_layer3);
            LAYER *new_routing_layer4 = new LAYER();
            new_routing_layer4-> name        = "ME4";
            new_routing_layer4-> direction   = "VERTICAL";
            routing_layer.push_back(new_routing_layer4);
        }

        if(lay_nmu >=6)
        {
            LAYER *new_routing_layer5 = new LAYER();
            new_routing_layer5-> name        = "ME5";
            new_routing_layer5-> direction   = "HORIZONTAL";
            routing_layer.push_back(new_routing_layer5);
            LAYER *new_routing_layer6 = new LAYER();
            new_routing_layer6-> name        = "ME6";
            new_routing_layer6-> direction   = "VERTICAL";
            routing_layer.push_back(new_routing_layer6);
        }

        if(lay_nmu >=8)
        {
            LAYER *new_routing_layer7 = new LAYER();
            new_routing_layer7-> name        = "ME7";
            new_routing_layer7-> direction   = "HORIZONTAL";
            routing_layer.push_back(new_routing_layer7);
            LAYER *new_routing_layer8 = new LAYER();
            new_routing_layer8-> name        = "ME8";
            new_routing_layer8-> direction   = "VERTICAL";
            routing_layer.push_back(new_routing_layer8);
        }


    }*/
    /*
    if(Cluster_PARA == true)
    {
        // read cluster file
        GP_cluster_parser(argc, argv);
    }*/




   ////////////  TEST  macro_type /////////////
    /*
    int a=0,b=0,c=0,d=0,e=0,f=0;
    cout << "macro_list.size :"<<(int)macro_list.size() <<endl;
    for(int i = 0; i < (int)macro_list.size(); i++)
    {
        Macro* macro = macro_list[i];
        if(macro->macro_type == MOVABLE_MACRO) a++;
        else if(macro->macro_type == PRE_PLACED) b++;
        else if(macro->macro_type == STD_CELL) c++;
        else if(macro->macro_type == PSEUDO_MODULE) d++;
        else if(macro->macro_type == PORT) e++;
        else f++;

    }
    cout << "NUM of MACRO TYPE : " << a << " " << b << " " << c << " " << d << " " << e << " " << "ELSE : "<<f << " " <<endl;
    getchar();
    */

}


/*******plus 107.09.17*********/

void datapath_parser(int argc, char* argv[])
{
    string datapathFile;
    string temp;
    for(int i = 0; i<argc; i++)
    {
        temp = argv[i];
        if(temp == "-datapath")
        {
            datapathFile = argv[i+1];
        }
    }
    if(datapathFile.empty())
    {
        dataflow_PARA = false;
        cerr <<"[WARNING] No Datapath file "<<endl;
        cerr << "   Key enter to continue...";
        getchar();
    }
    else
    {
        cout<<"   Now Read Datapath Parser "<< datapathFile <<endl;

        dataflow_PARA = true;
        ifstream fin(datapathFile.c_str());
        if(!fin)
        {
            cout<<"[ERROR] Can't open <datapath> file "<<datapathFile<<endl;
            exit(1);
        }

        string trash ;
        string temp, flow ;
        while(fin>>temp)
        {
            //cout<<temp<<endl;
            if(temp == "############")
                fin>>trash>>trash;
            else
            {
                fin>>trash>>flow;
                DataFlow_outflow[temp].insert(flow);
                DataFlow_inflow[flow].insert(temp);
            }
        }

        /// count max heir
        int dataflow_heir = 1;
        for(unsigned int j = 0; j < temp.size(); j++)
        {
            if(temp[j] == '/')
            {
                dataflow_heir++;
            }
        }
        dataflow_heir_num = dataflow_heir;
        cout<<"[INFO] Dataflow number:  "<<dataflow_heir_num<<endl;
        //getchar();
/*
        map<string, set<string> >::iterator iter;
        set<string>::iterator iters;
        for(iter = DataFlow_outflow.begin(); iter !=DataFlow_outflow.end(); iter++)
        {
            cout<<iter->first<<endl;
            for(iters = iter->second.begin(); iters != iter->second.end(); iters++)
            {
                cout<<"     ->"<<*iters<<endl;
            }
        }

        cout<<"/-----------------inflow-----------------------/"<<endl;
        map<string, set<string> >::iterator iter_in;
        set<string>::iterator iters_in;
        for(iter_in = DataFlow_inflow.begin(); iter_in !=DataFlow_inflow.end(); iter_in++)
        {
            cout<<iter_in->first<<endl;
            for(iters_in = iter_in->second.begin(); iters_in != iter_in->second.end(); iters_in++)
            {
                cout<<"     ->"<<*iters_in<<endl;
            }
        }
*/

    }
}
/******************************/



void Macro_gp_location(int argc, char* argv[])
{
    string def_file;
    for(int i = 0; i< argc; i++)
    {
        string temp;
        temp = argv[i];
        if(temp == "-NewDEF")
        {
            def_file = argv[i+1];
        }
    }
    if(!def_file.empty())
    {
        cout<<"   Now Read New Macro DEF file "<< def_file <<endl;

        ifstream fin(def_file.c_str());
        if(!fin)
        {
            cout<<"[ERROR] Can't open New Macro DEF file from command path"<< def_file <<endl;
            exit(1);
        }

        string trash, temp;

        int num = 0;
        int x,y;
        string macro_name;

        while(fin >> temp)
        {
            if(temp == "COMPONENTS")
            {
                fin >> num >> trash;
                break;
            }
        }

        for(int i=0 ; i< num ; i++)
        {
            fin >> trash >> macro_name >> trash >> trash >> trash >> trash >> x >> y ;

            map<string, int>::iterator it = macroName_ID.find(macro_name);
            if(it == macroName_ID.end())
            {
                cerr<<"[WARNING] Can't not find module "<< macro_name <<" while read New Macro DEF file. "<<endl;
                continue;
            }
            Macro* macro_temp = macro_list.at(it->second);
            if(macro_temp-> macro_type == MOVABLE_MACRO)
            {
                macro_temp-> gp.llx = x + shifting_X;
                macro_temp-> gp.lly = y + shifting_Y;
                macro_temp-> gp.urx = x + shifting_X + macro_temp-> cal_w ;
                macro_temp-> gp.ury = y + shifting_Y + macro_temp-> cal_h ;
                fin >> trash >> trash >> trash;
            }
            else
            {
                while(fin >> trash)
                    if(trash == ";")
                        break;
            }
        }
    }

}



/*******plus 108.04.26*********/

// Befoer DEF LFE file neeed to know "CLUSERT_PARE" is FALSE or TRUE
void Check_GP_cluster(int argc, char* argv[])
{
    string GP_ClusterFile;
    for(int i = 0; i<argc; i++)
    {
        string temp;
        temp = argv[i];
        if(temp == "-GP_cluster")
        {
            GP_ClusterFile = argv[i+1];
        }
    }
    if(GP_ClusterFile.empty())
    {
        Cluster_PARA = false;
        //cerr << "[WARNING] Do Not Consider <STD cells>. "<<endl;
        //cerr << "   Key enter to continue...";
        //getchar();
    }
    else
        Cluster_PARA = true;

}

// read clusert file
void GP_cluster_parser(int argc, char* argv[])
{
    string GP_ClusterFile;
    for(int i = 0; i<argc; i++)
    {
        string temp;
        temp = argv[i];
        if(temp == "-GP_cluster")
        {
            GP_ClusterFile = argv[i+1];
        }
    }
    if(GP_ClusterFile.empty())
    {
        cerr <<"[WARNING] No GP_Cluster File loading path"<<endl;
        // stop the check this warning
        cerr << "   Key enter to continue...";
        getchar();
    }
    else
    {
        cout<<"   Now Read Cluster file "<< GP_ClusterFile <<endl;
        ifstream fin(GP_ClusterFile.c_str());
        if(!fin)
        {
            cout<<"[ERROR] Can't open GP_Cluster from command path"<< GP_ClusterFile <<endl;
            exit(1);
        }
        string trash, temp;
        getline(fin,trash);

        int error_count = 0;

        while(fin >> temp)
        {
            if(temp == "STD_cluster_number:" || temp == "cluster_number:")
            {
                int STD_Cluster_num;
                fin >> STD_Cluster_num;
                cout<< "[INFO] STD_Cluster_num: "<< STD_Cluster_num << endl;
                STDGroupBy.resize(STD_Cluster_num);
                for(int i = 0; i < STD_Cluster_num; i++)
                {
                    int STD_num = 0;
                    fin >> temp >> STD_num;
                    //cout<<"STD_num: "<<STD_num<<endl;
                    STD_Group* STD_temp = new STD_Group;    // "STD_temp" is a cell group temp (it will be store in STDgroupby[])
                    // STD_ID
                    STD_temp -> ID = i;
                    // member_ID
                    STD_temp -> member_ID.resize(STD_num);
                    int sum_Width = 0, sum_Height = 0;
                    float sum_GCenter_X = 0, sum_GCenter_Y = 0;

                    for(int j = 0; j < STD_num; j++)
                    {
                        fin >> temp;                        //"temp" is a macro or a cell
                        map<string, int>::iterator iter;    // "iter" is a map of macro's Name to it's ID
                        iter = macroName_ID.find(temp);

                        if(iter != macroName_ID.end() )     // if it's exist in macroName_ID and it is cell ( definitely not macro )
                        {
                            int ID = iter -> second;        //"ID" is a index in macro_list[]

                            Macro* macro_temp = macro_list[ID];
                            macro_temp -> group_id = i;     // group_id is ?

                            STD_temp    -> member_ID[j]  = ID;
                            STD_temp    -> TotalArea    += macro_temp -> area;
                            sum_Width                   += macro_temp -> w;
                            sum_Height                  += macro_temp -> h;

                            //cout<<"Area: "<<macro_temp->area<<endl;
                            sum_GCenter_X += macro_temp-> area * (macro_temp->gp.llx + macro_temp -> w/2);
                            sum_GCenter_Y += macro_temp-> area * (macro_temp->gp.lly + macro_temp -> h/2);
                            //cout<<"gp.llx: "<<macro_temp->gp.llx<<endl;
                            //cout<<"gp.lly: "<<macro_temp->gp.lly<<endl;
                            //cout<<"sum_GCenter_X: "<<sum_GCenter_X<<endl;
                            //cout<<"sum_GCenter_Y: "<<sum_GCenter_Y<<endl;

                        }
                        else{
                            if(error_count < 100)
                                cout <<"[WARNING] can't find module: " << temp <<" in reading cluster stage." <<endl;
                            else if (error_count == 100)
                                cout <<"[WARNING] can't find module...(> one hundre of cell can't find)"<<endl;
                            error_count++;
                            //getchar();
                        }
                    }
                    STD_temp -> Avg_W       = sum_Width / STD_num;
                    STD_temp -> Avg_H       = sum_Height / STD_num;
                    STD_temp -> GCenter_X   = (int)(sum_GCenter_X / (float)(STD_temp->TotalArea));
                    STD_temp -> GCenter_Y   = (int)(sum_GCenter_Y / (float)(STD_temp->TotalArea));
                    STD_temp -> GPCenter_X  = STD_temp -> GCenter_X;
                    STD_temp -> GPCenter_Y  = STD_temp -> GCenter_Y;
                    STD_temp -> packingX    = STD_temp -> GCenter_X;
                    STD_temp -> packingY    = STD_temp -> GCenter_Y;

                    STD_temp -> Width       = STD_temp -> Height    = (int)sqrt(STD_temp->TotalArea*PARA*PARA); // cell group weight = heitht = sqrt(area)

                    /*
                    cout << "STDGroup[" <<i<<"] :"<<endl;
                    cout<<"avgWidth: "<<Width<<endl;
                    cout<<"avgHeight: "<<Height<<endl;
                    cout<<"TotalArea: "<<STD_temp->TotalArea<<endl;
                    cout<<"Width: "<<STD_temp->sum_Width<<endl;
                    cout<<"Height: "<<STD_temp->sum_Height<<endl;
                    cout<<"sum_GCenter_X: "<<STD_temp->GCenter_X<<endl;
                    cout<<"sum_GCenter_Y: "<<STD_temp->GCenter_Y<<endl;

                    getchar();
                    */
                    STDGroupBy[i] = STD_temp;

                }
            }
        }
        if(error_count >= 100)
            cout <<"[WARNING] "<< error_count << " of cell can't find, please check the PL FILE." << endl;

        /////////   TEST for cell cluster    /////////////
        /*
        //int count_big_group = 0; // for testing

        for(int i = 0; i < STDGroupBy.size(); i++)
        {
            cout<<"STD_Group_ID: "<<STDGroupBy[i]->ID<<endl;
            cout<<"member: "<<endl;
            for(int j = 0; j < STDGroupBy[i]->member_ID.size(); j++)
            {
                cout<<macro_list[STDGroupBy[i]->member_ID[j]]->macro_name <<" "<<endl;
            }
            cout<<"AVG_W: "<<STDGroupBy[i]->Avg_W<<"   AVG_H: "<<STDGroupBy[i]->Avg_H<<endl;
            cout<<"TotalArea: "<<STDGroupBy[i]->TotalArea<<endl;
            cout<<"GCenter_X: "<<STDGroupBy[i]->GCenter_X<<"   GCenter_Y: "<<STDGroupBy[i]->GCenter_Y<<endl;

            //getchar();

            //if(STDGroupBy[i]->TotalArea > 2000) count_big_group++;

        }
        //cout << "NUM of group : "<<STDGroupBy.size() << "  BID GROUP NUM:" << count_big_group <<endl;
        getchar();
        */
    }


    ///////// MODIFY  from 2020.02.17 /////////// delete Net_Std_Tmp data struture
    // Cal. NetID_to_weight and add net_list[].group_idSet info.
    /*
    for(unsigned int i = 0; i < net_list.size() ; i++)

    {
        for(unsigned int j =0 ; j < net_list[i].group_idSet.size() ; j++)
        {
            int Net_id_tmp = i;
            int std_id_tmp  = net_list[i].group_idSet[j];

            Macro* macro_temp = macro_list[std_id_tmp];
            int group_id_tmp  = macro_temp->group_id;

            STD_Group* std_temp = STDGroupBy[group_id_tmp];                 // the cell is in this STD_Group


            map<int, int>::iterator iter;
            iter = std_temp -> NetID_to_weight.find(Net_id_tmp);
            if(iter != std_temp->NetID_to_weight.end())                     // if net exist, weight++
            {
                iter->second++;
            }
            else                                                            // if net does not exist, weight = 1
            {
                std_temp->NetID_to_weight[Net_id_tmp] = 1;
            }

        }
    }
    */
    // NOT TO UESD//
    int error_num = 0;

    // update "group_idSet" info

    for(unsigned int i = 0; i < Net_Std_Tmp.size(); i++)
    {
        //cout <<"NET STD["<<i<<"] SIZE : "<< Net_Std_Tmp[i].size() << endl;
        //getchar();

        for(unsigned int j = 0; j < Net_Std_Tmp[i].size(); j++)
        {
            int Net_ID_tmp = i;
            int std_ID_tmp  = Net_Std_Tmp[i][j];

            Macro* std_temp = macro_list[std_ID_tmp];                       	// "macro_tmp" is a cell in net i
            int groupID_tmp  = std_temp-> group_id;

            if(groupID_tmp >= STDGroupBy.size() || groupID_tmp < 0 )
            {
                if(error_num < 100)
                    cout << "[WARNING] STD group id: " << groupID_tmp << " is not exist" << endl;
                else if(error_num == 100)
                    cout << "[WARNING] STD group id is not exist... (> one hundre of cell can't find)" << endl;
                error_num++;
                continue;
            }


            STD_Group* group_temp = STDGroupBy[groupID_tmp];                  	// cell is in this STD_Group


            map<int, int>::iterator iter;
            iter = group_temp -> NetID_to_weight.find(Net_ID_tmp);
            if(iter != group_temp-> NetID_to_weight.end())                    	// if net exist, weight++
            {
                iter-> second++;                                            	// weight of NetID_to_weight[] +1

            }
            else                                                            	// if net does not exist, weight = 1
            {
                group_temp-> NetID_to_weight[Net_ID_tmp] = 1;                 	// map< NET_ID, weight=1 >
                //int group_idSet_ID = net_list[Net_ID_tmp].group_idSet.size();
                net_list[Net_ID_tmp].group_idSet.insert(net_list[Net_ID_tmp].group_idSet.begin(), groupID_tmp );
                                                                                // the net connected std "groups" id
            }
            //cout<<"Net: "<<i<<endl;
            //cout<<"std_ID_tmp : "<<std_ID_tmp <<endl;
            //cout<<"groupID_tmp : "<<groupID_tmp <<endl;
            //getchar();

            ///// MODIFY 2020.02.26 ///////
            //net_list[Net_ID_tmp].cell_idSet.push_back(std_ID_tmp);
        }
    }


    if(error_num >= 100)
        cout << "[WARNING] "<< error_num <<" STD group id is not exist, plaease check PL FILE" << endl;
    //////// TEST for  net_list[].group_idSet[] ///////
    /*
    for(unsigned int i = 0; i < net_list.size(); i++)
    {
        cout<<"Net "<<i<<endl;
        for(unsigned int j = 0; j < net_list[i].group_idSet.size(); j++)
        {
            cout<< net_list[i].group_idSet[j] <<endl;
        }
        getchar();
    }
    */

    //////// TEST for  STDGroupBy[] ///////
    /*
    for(int i = 0; i < STDGroupBy.size(); i++)
    {
        cout<<"STD_Group_ID: "<<STDGroupBy[i]->ID<<endl;
        cout<<"Net: "<<endl;
        map<int, int> ::iterator iter;
        for(iter = STDGroupBy[i]->NetID_to_weight.begin(); iter != STDGroupBy[i]->NetID_to_weight.end(); iter++)
        {
            cout<< iter->first <<" weight: "<< iter->second <<endl;
        }

        getchar();
    }
    */
}

/**************** ADD 2021.03.12 ****************/
void ReadTechFile(string filename)
{
	cout << " *Parse TECH File" << endl;

	ifstream in_file((filename).c_str(), ios::in);

	if (!in_file)
	{
		cerr << "[ERROR] Can't open file " << (filename) << endl;
		exit(EXIT_FAILURE);
	}

	string word;

	while (in_file >> word)
	{
		if (word == "Layer")
		{
			in_file >> word;
			word.erase(word.end() - 1);
			word.erase(word.begin());
			string layer_name = word;

			while (in_file >> word)
			{
				if (word == "maskName")
				{
					in_file >> word >> word;

					if (word != "\"\"")
					{
						word.erase(word.end() - 1);
						word.erase(word.begin());

						if (word[0] == 'm' && word[1] == 'e' && word[2] == 't' && word[3] == 'a' && word[4] == 'l')
						{
							LAYER *temp = new LAYER();
							temp->name = layer_name;
							word.erase(word.begin(), word.begin() + 5);
							int layer = atoi(word.c_str());

							if (layer % 2 == 0)
								temp->direction = "VERTICAL";
							else
								temp->direction = "HORIZONTAL";

							while (in_file >> word)
							{
								if (word == "minWidth")
								{
									in_file >> word >> word;
									temp->min_width = atof(word.c_str());
								}
								else if (word == "minSpacing")
								{
									in_file >> word >> word;
									temp->min_spacing = atof(word.c_str());
								}
								else if (word == "}")
								{
									routing_layer.push_back(temp);
									break;
								}
							}

							if (word == "}")
								break;
						}
					}
					else
						break;
				}
				else if (word == "}")
					break;
			}
		}
	}

    cout << "[INFO] routing_layer size: " << routing_layer.size() << endl;

	in_file.close();
}

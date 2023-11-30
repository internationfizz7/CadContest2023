#include <iostream>

using namespace std;
#include "structure.h"
#include "DataFlow.h"
#include "DensityMap.h"
#include "flat_approach.h"
#include "deflef_parser.h"
#include "preprocessing.h"
#include "plot.h"
#include "hier_group.h"
#include "macro_legalizer.h"
#include "cell_placement.h"     // ADD 2021.03.17
#include "corner_stitching.h"   // ADD 2021.03.17
#include "CongestionMap.h"      // ADD 2021.03.17
#include "DumpFile.h"           // ADD 2021.03.17
#include "Partition.h"          // ADD 2021.03.17
#include "refinement.h"         // ADD 2021.03.17
#include "bipartite_matching.h" // ADD 2021.06

unsigned int seed = 18;
double ShrinkPara = 1; // 0< ShrinkPara <1

// mod1: PA5663 PARA & SUBREGION_AREA = 0.2
void Parameter_mod1()
{

    PARAMETER._SPACING = 0* PARA;
    PARAMETER._ROWH = 2000;
    PARAMETER._PARTITION_AREA = 0.6;
    PARAMETER._PARTITION_DISPLACEMENT = 0.2;
    PARAMETER._PARTITION_NETCUT = 0.4;

    PARAMETER._GROUP_AREA = 0.3;
    PARAMETER._GROUP_MACRO_AREA = 0.2;
    PARAMETER._GROUP_STD_AREA = 0.1;
    PARAMETER._GROUP_TYPE_STD = 2;
    PARAMETER._GROUP_HIER_LEVEL = 1;

    PARAMETER._PARTITION_SUBREGION_AREA = 0.3;

    PARAMETER._CHOOSE_Blank_Center_Dist = 0.08;
    PARAMETER._CHOOSE_Displacement = 0.05;
    PARAMETER._CHOOSE_Center_Distance = 0.2;
    PARAMETER._CHOOSE_Group_Distance = 0.2;
    PARAMETER._CHOOSE_Type_Distance = 0.3;
    PARAMETER._CHOOSE_Thickness = 0.2;
    PARAMETER._CHOOSE_WireLength = 0.01;

    //for case7
    /*
    PARAMETER._SPACING =9 * PARA;
    PARAMETER._ROWH = 2000;
    PARAMETER._PARTITION_AREA = 0.6;
    PARAMETER._PARTITION_DISPLACEMENT = 0.2;
    PARAMETER._PARTITION_NETCUT = 0.4;

    PARAMETER._GROUP_AREA = 0.2;
    PARAMETER._GROUP_MACRO_AREA = 0.3;
    PARAMETER._GROUP_STD_AREA = 0.3;
    PARAMETER._GROUP_TYPE_STD = 2;
    PARAMETER._GROUP_HIER_LEVEL = 1;

    PARAMETER._PARTITION_SUBREGION_AREA = 0.3;

    PARAMETER._CHOOSE_Blank_Center_Dist = 0.03;
    PARAMETER._CHOOSE_Displacement = 0.05;
    PARAMETER._CHOOSE_Center_Distance = 0.1;
    PARAMETER._CHOOSE_Group_Distance = 0.08;
    PARAMETER._CHOOSE_Type_Distance = 0.15;
    PARAMETER._CHOOSE_Thickness = 0.2;
    PARAMETER._CHOOSE_WireLength = 0.01;*/
	/*PARAMETER._SPACING = 16 * PARA;
    PARAMETER._ROWH = 2000;
    PARAMETER._PARTITION_AREA = 0.6;
    PARAMETER._PARTITION_DISPLACEMENT = 0.2;
    PARAMETER._PARTITION_NETCUT = 0.4;

    PARAMETER._GROUP_AREA = 0.2;
    PARAMETER._GROUP_MACRO_AREA = 0.1;
    PARAMETER._GROUP_STD_AREA = 0.2;
    PARAMETER._GROUP_TYPE_STD = 2;
    PARAMETER._GROUP_HIER_LEVEL = 1;

    PARAMETER._PARTITION_SUBREGION_AREA = 0.2;

    PARAMETER._CHOOSE_Blank_Center_Dist = 0.2;
    PARAMETER._CHOOSE_Displacement = 0.01;
    PARAMETER._CHOOSE_Center_Distance = 0.2;
    PARAMETER._CHOOSE_Group_Distance = 0.3;
    PARAMETER._CHOOSE_Type_Distance = 0.4;
    PARAMETER._CHOOSE_Thickness = 0.1;
    PARAMETER._CHOOSE_WireLength = 0.01;*/
}

// mod2: PA8658B PARA & SUBREGION_AREA = 0.5
void Parameter_mod2()
{
    /*PARAMETER._SPACING = 9 * PARA;
    PARAMETER._ROWH = 2000;
    PARAMETER._PARTITION_AREA = 0.6;
    PARAMETER._PARTITION_DISPLACEMENT = 0.2;
    PARAMETER._PARTITION_NETCUT = 0.4;

    PARAMETER._GROUP_AREA = 0.2;
    PARAMETER._GROUP_MACRO_AREA = 0.3;
    PARAMETER._GROUP_STD_AREA = 0.2;
    PARAMETER._GROUP_TYPE_STD = 1;
    PARAMETER._GROUP_HIER_LEVEL = 1;

    PARAMETER._PARTITION_SUBREGION_AREA = 0.5;

    PARAMETER._CHOOSE_Blank_Center_Dist = 0.05;
    PARAMETER._CHOOSE_Displacement = 0.08;
    PARAMETER._CHOOSE_Center_Distance = 0.1;
    PARAMETER._CHOOSE_Group_Distance = 0.05;
    PARAMETER._CHOOSE_Type_Distance = 0.1;
    PARAMETER._CHOOSE_Thickness = 0.1;
    PARAMETER._CHOOSE_WireLength = 0.01;*/
        PARAMETER._SPACING = 0 * PARA;
        PARAMETER._ROWH = 2000;
        PARAMETER._PARTITION_AREA = 0.6;
        PARAMETER._PARTITION_DISPLACEMENT = 0.2;
        PARAMETER._PARTITION_NETCUT = 0.4;

        PARAMETER._GROUP_AREA = 0.2;
        PARAMETER._GROUP_MACRO_AREA = 0.03;
        PARAMETER._GROUP_STD_AREA = 0.2;
        PARAMETER._GROUP_TYPE_STD = 1;
        PARAMETER._GROUP_HIER_LEVEL = 1;

        PARAMETER._PARTITION_SUBREGION_AREA = 0.5;

        PARAMETER._CHOOSE_Blank_Center_Dist = 0.05;
        PARAMETER._CHOOSE_Displacement = 0.01;
        PARAMETER._CHOOSE_Center_Distance = 0.3;
        PARAMETER._CHOOSE_Group_Distance = 0.01;
        PARAMETER._CHOOSE_Type_Distance = 0.15;
        PARAMETER._CHOOSE_Thickness = 0.2;
        PARAMETER._CHOOSE_WireLength = 0.01;
}

// mod3: PA8536 PARA & SUBREGION_AREA = 1.2
void Parameter_mod3()
{
    PARAMETER._SPACING = 0 * PARA;
    PARAMETER._ROWH = 2000;
    PARAMETER._PARTITION_AREA = 0.6;
    PARAMETER._PARTITION_DISPLACEMENT = 0.2;
    PARAMETER._PARTITION_NETCUT = 0.4;

    PARAMETER._GROUP_AREA = 0.3;
    PARAMETER._GROUP_MACRO_AREA = 0.5;
    PARAMETER._GROUP_STD_AREA = 0.3;
    PARAMETER._GROUP_TYPE_STD = 2;
    PARAMETER._GROUP_HIER_LEVEL = 2;

    PARAMETER._PARTITION_SUBREGION_AREA = 1.2;

    PARAMETER._CHOOSE_Blank_Center_Dist = 0.5;
    PARAMETER._CHOOSE_Displacement = 0.05;
    PARAMETER._CHOOSE_Center_Distance = 0.3;
    PARAMETER._CHOOSE_Group_Distance = 0.1;
    PARAMETER._CHOOSE_Type_Distance = 0.3;
    PARAMETER._CHOOSE_Thickness = 0.3;
    PARAMETER._CHOOSE_WireLength = 0.1;
}

void Switch_Case(int argc, char* argv[])
{
    string Himax_case;
    for(int i = 0; i<argc; i++)
    {
        string temp;
        temp = argv[i];
        if(temp == "-mod")
        {
            Himax_case = argv[i+1];
        }
    }
    int case_number = 0;
    stringstream ss;
    ss << Himax_case;
    ss >> case_number;
    switch(case_number)
    {
    case 1 :
        Parameter_mod1();
        break;
    case 2 :
        Parameter_mod2();
        break;
    case 3 :
        Parameter_mod3();
        break;

    default :
        Parameter_mod3();
        cout << "[INFO] Using Defult Parameter: mod3" << endl;
        break;
    }
}

void INPUT_PARA(int argc, char* argv[])
{
    string filename;
    bool para_flag = false;
    for(int i = 0; i < argc; i++)
    {
        filename = argv[i];
        if(filename == "-para")
        {
            filename = argv[i+1];
            para_flag = true;
            break;
        }
    }
    if(!para_flag)
        return;

    //cout<<"%%%%INPUT PARA FILE "<<filename<<"%%%%"<<endl;
    cout<< "   Now Read PARA File.." << endl;
    ifstream fin(filename.c_str());
    if(!fin)
    {
        cout <<"[WARNING] DO NOT SET PARAMETER! "<<endl;
        cout << "   Key enter to continue...";
        getchar();
    }
    string trash;
    getline(fin, trash);
    getline(fin, trash);
    getline(fin, trash);

    bool default_flag = false;
    while(fin)
    {
        int PARA_name;
        fin>>PARA_name;
        switch (PARA_name)
        {
        case 1:

            fin>>trash>>trash>>default_flag;

            if(default_flag == true)
            {
                cout<<"[INFO] Using Default Paremeter"<<endl;
                goto ENDFILE;
            }
            break;
        case 2:
            fin>>trash>>trash>>PARAMETER._PARTITION_SUBREGION_AREA;
            break;
        case 3:
            fin>>trash>>trash>>PARAMETER._CHOOSE_Blank_Center_Dist;
            break;
        case 4:
            fin>>trash>>trash>>PARAMETER._CHOOSE_Displacement;
            break;
        case 5:
            fin>>trash>>trash>>PARAMETER._CHOOSE_Center_Distance;
            break;
        case 6:
            fin>>trash>>trash>>PARAMETER._CHOOSE_Group_Distance;

            break;
        case 7:
            fin>>trash>>trash>>PARAMETER._CHOOSE_Type_Distance;
            break;
        case 8:
            fin>>trash>>trash>>PARAMETER._CHOOSE_Thickness;
            break;
        case 9:
            fin>>trash>>trash>>PARAMETER._CHOOSE_WireLength;
            break;
        default:
            //return;
            break;
        }
    }
    ENDFILE:
    if(Debug_PARA)
    {
        cout << "[INFO] SPACING: "                     << PARAMETER._SPACING                   << endl;

        cout << "[INFO] ROWH: "                        << PARAMETER._ROWH                      << endl;
        cout << "[INFO] AREA: "                        << PARAMETER._PARTITION_AREA                      << endl;
        cout << "[INFO] DISPLACEMENT: "                << PARAMETER._PARTITION_DISPLACEMENT              << endl;
        cout << "[INFO] NETCUT: "                      << PARAMETER._PARTITION_NETCUT                    << endl;
        cout << "[INFO] SUB_REGION_AREA: "             << PARAMETER._PARTITION_SUBREGION_AREA           << endl;
        cout << "[INFO] _ITER: "                        << PARAMETER._PARTITION_ITER                     << endl;

        cout << "[INFO] GROUP_AREA: "                  << PARAMETER._GROUP_AREA                << endl;
        cout << "[INFO] GROUP_MACRO_AREA: "            << PARAMETER._GROUP_MACRO_AREA          << endl;
        cout << "[INFO] GROUP_STD_AREA: "              << PARAMETER._GROUP_STD_AREA            << endl;
        cout << "[INFO] GROUP_TYPE_STD: "              << PARAMETER._GROUP_TYPE_STD            << endl;
        cout << "[INFO] HIER_LEVEL: "                  << PARAMETER._GROUP_HIER_LEVEL                << endl;

        cout << "[INFO] CHOOSE_Blank_Center_Dist: "   << PARAMETER._CHOOSE_Blank_Center_Dist  << endl;
        cout << "[INFO] CHOOSE_Displacement: "        << PARAMETER._CHOOSE_Displacement       << endl;
        cout << "[INFO] CHOOSE_Center_Distance: "     << PARAMETER._CHOOSE_Center_Distance    << endl;
        cout << "[INFO] CHOOSE_Group_Distance: "      << PARAMETER._CHOOSE_Group_Distance     << endl;
        cout << "[INFO] CHOOSE_Type_Distance: "       << PARAMETER._CHOOSE_Type_Distance      << endl;
        cout << "[INFO] CHOOSE_Thickness: "           << PARAMETER._CHOOSE_Thickness          << endl;
        cout << "[INFO] CHOOSE_WireLength: "          << PARAMETER._CHOOSE_WireLength         << endl;
    }

    if(PARAMETER._PARTITION_SUBREGION_AREA > 1)
    {
        PARAMETER.PARTITION = false;
    }
    fin.close();
}


int main(int argc, char* argv[])
{
    clock_t start_time, end_time;
    start_time = clock();
    srand(18);
    //srand(time(NULL));
    /*********modify 108.03.21*********/
    shifting_X = 0;
    shifting_Y = 0;
    PARA = 0;
    /*****************************/

    cout << "\n========   Now Run PARSER STAGE\t========" << endl;
    // 1.read def lef file and read cell cluster file parser
    Call_deflef_parser(argc, argv);
    // 2.choose case
    Switch_Case(argc, argv);
    // 3.input choose case parameter
    INPUT_PARA(argc, argv);

    // 4.datapath parser
    //datapath_parser(argc, argv);

    // 4. using new macro location
    Macro_gp_location(argc, argv);

    cout<<"[Paser is finished] "<<endl;

    // 5.output info for checker
    if(Debug_PARA)
    {
        // LEF info
        Output_LEF_info();
        // DEF info
        Output_info();
        // output nodes nets pl
        Output_netlist();
        // Output_modDEF();     // unknow
    }

    ///// starting macro placement five stage//////

    // Stage 1 preprocessing
    Call_preprocessing();

    // Plot matlab (bool STD_plot = false)
    Plot_global(false, "./output/GP_graph/GP_macro.m");
    if(Debug_PARA)
        OutputTCL_withcell("./output/GP_graph/GP.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);


    // Stage 2-5 macro grouping / recurve partition / macro legailzation / macro refinement
    MacroLegal();

    end_time = clock();
    double run_time = (double)(end_time-start_time) /CLOCKS_PER_SEC;
    cout << "[INFO] Macro Placement Runing Time: "<< run_time << 's'<< endl;

    cout << "[Macro Placement END]" << endl;

    return 0 ;
}

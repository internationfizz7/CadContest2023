/*----------------------------------------*
 * file name : main.cpp
 * abstract  : user interface & function interface
 *
 * current version : 2018-08 (3-D,2-D)
 * author          : NCKU SEDA LAB
 * data finished   :
 *----------------------------------------*/
#include "tool.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include "dataType.h"
#include "IO.h"
#include "gp.h"
#include "Cluster.h"
#include "WirelengthGP.h"
//#include "ThermalGlobal.h"
//#include "Legalization.h"
//#include "EP.h"
//#include "TemperatureAnalyze.h"
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
#include "Abacus.h"
#include "dataProc_o.h"
#include "structure_o.h"
#include "FM_o.h"
#include "Abacus_tsv.h"
using namespace std;

unsigned int seed = 18;
double ShrinkPara = 1; // 0< ShrinkPara <1

void InputPara(int argc, char* argv[],NETLIST & tmpnt);
void ChooseParser(NETLIST & nt,Lib_vector & lib_vector,NETLIST_o& nt_o);
void MultiLevel(NETLIST &tmpnt);
void InitialFloorplan(NETLIST &tmpnt);
void GlobalFP(NETLIST & tmpnt);
void LegalFP(NETLIST &tmpnt);
void AssignTSV(NETLIST & tmpnt);
void OutputFP(NETLIST & tmpnt);
void clean_parameter();
void CalculateHPWL_ML(NETLIST & tmpnt);
void CalculateHPWL_CL(NETLIST & tmpnt);
// mod3: PA8536 PARA & SUBREGION_AREA = 1.2
void Parameter_mod3()
{
	PARAMETER._SPACING = 0.1 * PARA;
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
	PARAMETER._CHOOSE_Displacement = 3;
	PARAMETER._CHOOSE_Center_Distance = 0.3;
	PARAMETER._CHOOSE_Group_Distance = 0.1;
	PARAMETER._CHOOSE_Type_Distance = 0.3;
	PARAMETER._CHOOSE_Thickness = 0.3;
	PARAMETER._CHOOSE_WireLength = 0.1;
}
void Switch_Case()
{

	int case_number = 3;

	switch(case_number)
	{
		
		case 3 :
			Parameter_mod3();
			break;

		default :
			Parameter_mod3();
			//cout << "[INFO] Using Defult Parameter: mod3" << endl;
			break;
	}
}
void INPUT_PARA()
{
	/*PARAMETER._PARTITION_SUBREGION_AREA=0.1;
	PARAMETER._CHOOSE_Blank_Center_Dist=0.05;
	PARAMETER._CHOOSE_Displacement=0.05;
	PARAMETER._CHOOSE_Center_Distance=0.06;
	PARAMETER._CHOOSE_Group_Distance=0.03;
	PARAMETER._CHOOSE_Type_Distance=0.06;
	PARAMETER._CHOOSE_Thickness=0.2;
	PARAMETER._CHOOSE_WireLength=0.1;*/
	/*string filename="./input/PARAMETER/SET_PARA";
	bool para_flag = true;

	if(!para_flag)
		return;

	//cout<<"%%%%INPUT PARA FILE "<<filename<<"%%%%"<<endl;
	//cout<< "   Now Read PARA File.." << endl;
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
					//cout<<"[INFO] Using Default Paremeter"<<endl;
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
	fin.close();*/
	/*cout << "[INFO] SPACING: "                     << PARAMETER._SPACING                   << endl;

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
		cout << "[INFO] CHOOSE_WireLength: "          << PARAMETER._CHOOSE_WireLength         << endl;*/
		PARAMETER.PARTITION = false;
}
int main(int argc, char **argv)
{
	
	//int result_dir=mkdir("temp",0777);
	
	
	clock_t start_overall,end_overall;
	start_overall=clock();
	Lib_vector lib_vector;
	NETLIST nt;						///< 3-D netlist
	vector<NETLIST> sub_nt;			///< sub-netlist
	PLOT ploter;
	//cout<<"1\n";					///< Plot Fcn.
	ploter.SetColoring(false);		///< Seting ploter color
	ploter.SetTexting(false);       ///< Seting ploter text
	//cout<<"2\n";
	InputPara(argc,argv,nt);		///< Input Setting
	//cout<<"3\n";
	//ChooseParser(nt,lib_vector);				///< Choose Parser
	cout<<"===================PARSER======================\n";
	char bench[FILELEN] = "\0";		///< benchmark name (including directory path)
	char str_format[4] = "3";		///< benchmark format. ("3": CAD)
	char str_nparts[4] = "2";		///< number of desired partitions (equal to number of tiers)
	char str_addName[4] = "-1";
	char str_output[4] = "1";      ///< output the best N result of all SA solution
	string OutputFile  = "Out.txt";

	bool is_appointed[4] =			///< flags for checking parameters has been appointed
	{
		false,	///< [0] benchmark format
		false,	///< [1] benchmark name
		false,	///< [2] help
		false	///< [3] additional name
	};

	argv++;
	argc--;

	int num_command = argc;
	is_appointed[0] = true;
	is_appointed[1] = true;
	strcpy(bench,InputOption.benName.c_str());

	string aaaaaaa=".txt";
	strcat(bench,aaaaaaa.c_str());
	
	

	if( num_command == 0 || !is_appointed[0] || !is_appointed[1] || is_appointed[2] )
	{
		cout << "                                                                                  " << endl;
		cout << "                          Layer Assignment - Version 1.0                          " << endl;
		cout << "                                 by NCKU SEDA LAB                                 " << endl;
		cout << "                                                                                  " << endl;
		cout << "       basic usage: ./LA -format <benchmark_format> -bench <benchmark_name>       " << endl;
		cout << "                                                                                  " << endl;
		cout << " arguments:                                                                       " << endl;
		cout << "   -format     <benchmark_format>  format of benchmark (\"3\": CAD)               " << endl;
		cout << "   -bench      <benchmark_name>    name of benchmark including directory path     " << endl;
		cout << "   -nparts     <number>            number of partitions                           " << endl;		
		cout << "   -addName    <name>              additional name of dump file                   " << endl;
        cout << "   -output     <number>            output the best N result of all SA solution    " << endl;
        cout << "   -OutputFile <number>            name of the output file                        " << endl;
		cout << "   -help                           help message and exit                          " << endl;
		cout << "                                                                                  " << endl;
		exit(EXIT_FAILURE);
	}

	format_o = atoi( str_format );
	metis_nparts_o = atoi( str_nparts );
	nOutput_o = atoi( str_output );
	
	NETLIST_o nt_o;
    Lib_o lib_o;
    DIE_o die_o;
    clock_t start_t1 ,end_t1;

    
	start_t1 = clock();
	switch( format_o )
	{		
        case 3:
            cout << "[Info] Read the cadcontest file..." << endl;
            ReadCadcontestfile_o(bench, nt_o, lib_o, die_o);
            cout << "[Info] Read file succeed" << endl;
            break;
		default:
			cout << "Error  : Format " << format_o << " doesnt's exist!" << endl;
			exit(EXIT_FAILURE);
			break;
	}
	end_t1 = clock();
	cout << "Parser takes : " << (double)(end_t1 - start_t1) / CLOCKS_PER_SEC <<"s"<<endl;

	
	cout<<"=====================PA========================\n";
	start_t1 = clock();
	cout << "[Info] Partition the netlist..." << endl;
	Partitioning partition;
	partition.FM(nt_o, lib_o, die_o);
	
	cout << "[Info] Partition netlist succeed" << endl;//난 재미있지
	
	
	
	/*
    cout << lib;
	
	cout << die;

	cout << "***** Check Terminal data *****" << endl;
	cout << "-Size          :" << Tsize << endl;
	cout << "-Spacing       :" << Tspace << endl;
	cout << "-Cost          :" << Tcost << endl;
	
	cout << nt << endl;
	*/
	cout << "Total area on top die :" << nt_o.TopArea << "\nThe size of die :" << die_o.DieSize << endl;
	cout << "The utilization ratio for top die is " << double(nt_o.TopArea)/die_o.DieSize << endl;
	if(die_o.TopUtil >= 100*double(nt_o.TopArea)/die_o.DieSize){
		cout << "Meet the utilization ratio\n\n";
	}
	else{
		cout << "Exceed the utilization ratio\n\n";
	}
	cout << "Total area on bot die :" << nt_o.BotArea << "\nThe size of die :" << die_o.DieSize << endl;
	cout << "The utilization ratio for bot die is " << double(nt_o.BotArea)/die_o.DieSize << endl;
	if(die_o.BotUtil >= 100*double(nt_o.BotArea)/die_o.DieSize){
		cout << "Meet the utilization ratio\n\n";
	}
	else{
		cout << "Exceed the utilization ratio\n\n";
	}
	
	
	//cout <<"Cutsize : "<< partition.cutsize << " " << partition.minsize << endl;
	if(InputOption.test_mode==50){
		InputOption.ThreeDimIC 			= false;		///< Set 3D or 2D
	}
	ChooseParser(nt,lib_vector,nt_o);				///< Choose Parser
	cout<<"realcase: "<<InputOption.realcase<<endl;

	end_t1 = clock();
	cout<<"Partition takes: "<<(double)(end_t1-start_t1)/CLOCKS_PER_SEC<<"s\n";
	if(InputOption.realcase!=1){
	cout<<"=====================QP========================\n";
	start_t1 = clock();
	int mod_num = nt.GetnMod();
	int mod_num_temp=mod_num;
	while(true){
		
		if(mod_num_temp<=20000){
			break;
		}
		InputOption.TargetClusterNum++ ;
		mod_num_temp/=1.5;
	}


	//註解調GP從這
	ploter.dump_deflef(nt, InputOption.benName);
	InitialFloorplan(nt);			///< SetingFloorplan Spec.
	// show info
	cout << "*************** Info ****************" << endl;
	cout << "-Program mode              : " << InputOption.mode << endl;
	cout << "-Benchmark name            : " << InputOption.benName << endl;
	cout << "*************************************" << endl;
	cout << nt;
	InputOption.QP_key=true;
	GlobalFP(nt);					///< Global Floorplanning
	ploter.dump_pl(nt, InputOption.benName,mod_num,0); //0 gp 1 ml
	string dpx_qp=InputOption.benName_sec+"_qp";
	//ploter.plot_step( nt, dpx_qp );


	//註解掉GP到這
	
	//LegalFP(nt);					///< Legalize Floorplan


	//if (nt.GetnLayer() > 1)
	  //{

	  //AssignTSV(nt);				///< Assign TSV
	  //}
	  
	//ploter.PlotResultGL_dy(nt,InputOption.OutputPath + "Lp",1);

	//OutputFP(nt);					///< Output Floorplan File

	end_t1 = clock();
	cout<<"QP takes: "<<(double)(end_t1-start_t1)/CLOCKS_PER_SEC<<"s\n";
	cout << "\n\n[QP END]" << endl;
	//if(InputOption.benName!="case02"){
	   cout<<"\n=====================ML========================\n";
	   Debug_PARA=false;
	   clock_t start_time_ml, end_time_ml;
	   start_time_ml = clock();
	   srand(18);

	   shifting_X = 0;
	   shifting_Y = 0;
	   PARA = 0;


	   //cout << "\n========   Now Run PARSER STAGE\t========" << endl;
	// 1.read def lef file and read cell cluster file parser
	string to_read=/*"temp/"+*/InputOption.benName_sec+"_top";
	Call_deflef_parser(to_read);
	// 2.choose case
	Switch_Case();
	// 3.input choose case parameter
	INPUT_PARA();

	// 4.datapath parser
	//datapath_parser(argc, argv);

	// 4. using new macro location
	//6/17
	//Macro_gp_location(argc, argv);

	//cout<<"[Parser is finished] "<<endl;

	// 5.output info for checker
	/*if(Debug_PARA)
	{
	// LEF info
	Output_LEF_info();
	// DEF info
	Output_info();
	// output nodes nets pl
	Output_netlist();
	// Output_modDEF();     // unknow
	}*/

	///// starting macro placement five stage//////

	// Stage 1 preprocessing
	Call_preprocessing();

	// Plot matlab (bool STD_plot = false)
	Plot_global(false, "./output/GP_graph/GP_macro.m");
	if(Debug_PARA)
	OutputTCL_withcell("./output/GP_graph/GP.tcl", BenchInfo.stdXcenter, BenchInfo.stdYCenter);


	// Stage 2-5 macro grouping / recurve partition / macro legailzation / macro refinement
	MacroLegal(nt);
	//  PreplacedInCornerStitching.clear(); /// For every iteration a all new clean fixed blocks & blanks /// If u want to get the info please use func : UpdateInfoInPreplacedInCornerStitching
	// FixedPreplacedInCornerStitching.clear(); /// record the ID numbers of fixed occupied tiles


	clean_parameter();
	if(InputOption.realcase==2){
		for(int i=0;i<nt.nMod;i++){
			if(nt.mods[i].modName[0]=='C'&&nt.mods[i].modName[1]=='2'&&nt.mods[i].modName[2]=='7'&&nt.mods[i].modName[3]=='9'&&nt.mods[i].modName[4]=='1'){
				cout<<"suc changed\n";
				nt.mods[i].LeftX=1652;
				nt.mods[i].GLeftX=1652;
				nt.mods[i].CenterX=nt.mods[i].LeftX+0.5*nt.mods[i].modW;
				nt.mods[i].GCenterX=nt.mods[i].GLeftX+0.5*nt.mods[i].modW;
			}
		}
	}
	else{
	to_read=InputOption.benName_sec+"_bot";
	Call_deflef_parser(to_read);
	// 2.choose case
	Switch_Case();
	// 3.input choose case parameter
	INPUT_PARA();

	// 4.datapath parser
	//datapath_parser(argc, argv);

	// 4. using new macro location
	//6/17
	//Macro_gp_location(argc, argv);

	//cout<<"[Parser is finished] "<<endl;

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
	MacroLegal(nt);
	}

	end_time_ml = clock();
	double run_time_ml = (double)(end_time_ml-start_time_ml) /CLOCKS_PER_SEC;
	cout << "ML takes: "<< run_time_ml << 's'<< endl;

	cout << "[Macro Placement END]" << endl;
	string dpx_ml=InputOption.benName_sec+"_ml";
	//ploter.plot_step( nt, dpx_ml );
	//ploter.PlotResultGL_dy(nt,InputOption.OutputPath + "Gpafterml",0);
	cout<<"\n=====================GP========================\n";
	InputOption.QP_key=false;
	GlobalFP(nt);					///< Global Floorplanning
	string dpx_gp=InputOption.benName_sec+"_gp";
	//ploter.plot_step( nt, dpx_gp );
	ploter.dump_pl(nt, InputOption.benName,mod_num,1);
	ploter.dump_nodes(nt, InputOption.benName,mod_num);
	CalculateHPWL_ML(nt);
	cout<<"HPWL after ML and GP: "<<nt.totalWL_ML<<endl;
	cout << "[Global Placement END]" << endl;
	//}
	cout<<"\n=====================CL========================\n";
	clock_t start_cl,end_cl;
	start_cl=clock();
	//string bench = "case02";
	Abacus Ab_bot(/*"temp/"+*/InputOption.benName_sec, 0,nt);
	Ab_bot.runAbacus(nt);
	cout <<"overlap: "<< Ab_bot.isOverlap() << endl;
	cout <<"overlap(M): "<< Ab_bot.isOverlap_Macro() << endl;
	Abacus Ab_top(/*"temp/"+*/InputOption.benName_sec, 1,nt);
	Ab_top.runAbacus(nt);
	cout <<"overlap: "<< Ab_top.isOverlap() << endl;
	cout <<"overlap(M): "<< Ab_top.isOverlap_Macro() << endl;
	end_cl = clock();
	double run_time = (double)(end_cl-start_cl) /CLOCKS_PER_SEC;
	cout << "CL takes: "<< run_time << 's'<< endl;
	cout << "\n\n[Cell Legalization END]" << endl;

	cout<<"\n====================TSV========================\n";
	start_cl=clock();
	ABTSV abtsv(nt);
	end_cl = clock();
	run_time = (double)(end_cl-start_cl) /CLOCKS_PER_SEC;
	cout << "TSV legalize takes: "<< run_time << 's'<< endl;
	cout << "\n\n[TSV Legalization END]" << endl;
	}
	IO::CAD2023 cd;
	if(InputOption.realcase==1){
		cd.outFile_01(nt.output_name, nt);
	}
	else{
    	cd.outFile(nt.output_name, nt);	
	}
	//ploter.PlotResultGL_dy(nt,InputOption.OutputPath + "Gpaftercl",0);
	string dpx_final=InputOption.benName_sec+"_final";
	//ploter.plot_step( nt, dpx_final );
	end_overall=clock();
	CalculateHPWL_CL(nt);
	cout<<"HPWL after CL: "<<nt.totalWL_CL<<endl;
	cout<<"\n\nThis program takes "<<(double)(end_overall-start_overall)/CLOCKS_PER_SEC<<"s in total\n";
	
	cout <<BOLD(Yellow (  " [ NCKU Floorplanner END ] "))<<endl ;
	return 0;
}

void clean_parameter(){
	pin_list.clear();
	DEFLEFInfo=DEFLEF_INFO();
	net_list.clear();
	chip_boundary=Boundary();
	Placement_blockage.clear();
	macro_list.clear();
	STDGroupBy.clear();
	PARAMETER=USER_SPECIFIED_PARAMETER();
	track=Track();
	DataFlow_outflow.clear();
	DataFlow_inflow.clear();
	routing_layer.clear();
	gcellgrid.clear();
	//Nodemap.clear();
	MovableMacro_ID.clear();
	PreplacedMacro_ID.clear();
	row_vector.clear();
	full_boundary=Boundary();
	MacroNum=0;
	StdNum=0;
	PreNum=0;
	dataflow_heir_num=0;
	ITER=0;
	Macros_Set.clear();
	PreplacedInCornerStitching.clear();
	FixedPreplacedInCornerStitching.clear();
	CG_INFO=CONGESTIONMAP();
	STDGroupBy2.clear();
	BenchInfo=Bechmark_INFO();
	clear_para_structure();
	clear_para_refine();
	clear_para_hier();
	clear_para_ml();
	clear_para_dump();
	clear_para_congmap();
	clear_para_deflef();
	clean_para_prepro();
}

void InputPara(int argc, char* argv[] , NETLIST & tmpnt)
{

	// Set Default Value

	InputOption.mode				= 3 ;			///< Mode to Choose Parser Type
	InputOption.overflow_bound		= 0.05;			///< GP Terminal Condition
	InputOption.bin_para			= 1 ;			///< Bin Number Para
	//InputOption.bin_para			= 5 ;			///< Bin Number Para
	//InputOption.bin_para			= 3 ;			///< Bin Number Para
	InputOption.shConst				= 10000;		///< EP Tree
	InputOption.test_mode			=	0;          ///< Default

	InputOption.TargetClusterNum	= 1 ;			///< Clustering Terminal Condition
	InputOption.ClusterArea			= 10;			///< Cluster Area Constraint
	InputOption.InitialGradient		= 1 ;			///< Initial Gradient for Density
	InputOption.DeclusterGradient	= 1 ;			///< Decluster Gradient

	InputOption.EnlargeBlockRatio   = 1.0;          ///< Enlarge the blocks
	InputOption.EnlargePowerRatio   = 1.0;          ///< Enlarge the power

	InputOption.benName				= "";			///< Read in Bench Name
	InputOption.CorbBench			= "";			///< Output HotSpot Sovler Ben Name
	InputOption.OutputPath			= "./out/";		///< Output Files Path
	InputOption.ITRIPath			= "";			///< ITRI Solver Files

	InputOption.EPorILP				= false;		///< Choose Legalizer EP or ILP
	InputOption.ThreeDimIC 			= true;		///< Set 3D or 2D
	InputOption.ThreeDimICMulti		= true;		///< Set 3D flat or milti-level
	InputOption.ThermalOpt			= true;		///< Set Thermal Optimize
	/*InputOption.ThreeDimIC 			= false;		///< Set 3D or 2D
	InputOption.ThreeDimICMulti		= false;		///< Set 3D flat or milti-level
	InputOption.ThermalOpt			= true;		///< Set Thermal Optimize*/
	InputOption.ThermalTsv          = false;        ///< Set Thermal TSV optimization
	InputOption.CompCorblivar       = false;        ///< Set to compare with corblivar
	InputOption.QP_key				= true;
	double maxWS 					= 0.15 ;		///< Floorplan White space
	double aR						= 1; 			///< Aspect Ratio of Floorplan
	tmpnt.SetmaxWS(maxWS);
	tmpnt.SetaR(aR);


	// Checking Flag
	vector <bool> InputFlag;

	InputFlag.resize(2);
	InputFlag[0] = InputFlag[1] = false;


	for(int i = 1 ; i < argc ; i++)
	{
		string temp = argv[i];
		if(i==1){
			InputOption.benName = argv[i];

			InputOption.benName=InputOption.benName.substr(0,InputOption.benName.size()-4);
			//cout<<InputOption.benName;
			InputOption.benName_sec=InputOption.benName;
			InputOption.benName_sec+="_";
			InputOption.benName_sec+=to_string(InputOption.test_mode);
		}
		else if(i==2){
			tmpnt.output_name=argv[i];
		}
		/*else if(i==3){
			string t="0";
			t=argv[i];
			InputOption.test_mode=stoi(t);
			if(InputOption.test_mode==12){
				InputOption.bin_para			= 3 ;
			}
			else if(InputOption.test_mode==13){
				InputOption.bin_para			= 1 ;
			}
			else if(InputOption.test_mode==21){
				InputOption.bin_para			= 20 ;
			}
			InputOption.benName_sec=InputOption.benName;
			InputOption.benName_sec+="_";
			InputOption.benName_sec+=to_string(InputOption.test_mode);
			tmpnt.output_name+="_";
			tmpnt.output_name+=to_string(InputOption.test_mode);
		}*/
		
		/*if(temp == "-mode")
		{
			InputOption.mode = atoi( argv[i+1] );
			InputFlag[0] = true;
		}
		else if(temp == "-benName")
		{
			InputOption.benName = argv[i+1];
			InputFlag[1] = true;
		}
		else if(temp == "-maxWS")
		{
			tmpnt.SetmaxWS ( atof(argv[i+1]) / 100.0  );
		}
		else if(temp == "-aR")
		{
			tmpnt.SetaR ( atof( argv[i+1] ) );
		}
		else if(temp == "-binPara")
		{
			InputOption.bin_para = atoi (argv[i+1]);
		}
		else if (temp ==  "-g")
		{
			InputOption.overflow_bound = atof( argv[i+1] );
		}
		else if (temp == "-ClusterNum")
		{
			InputOption.TargetClusterNum = atof(argv[i+1]);
		}
		else if (temp  == "-CorbBench")
		{
			InputOption.CorbBench = argv[i+1];
		}
		else if (temp == "-OutputPath")
		{
			tmpnt.SetoutputPath(argv[i+1]) ;
		}
		else if (temp == "-ClusterArea")
		{
			InputOption.ClusterArea = atoi( argv[i+1] );
		}
		else if (temp == "-InitialGradient")
		{
			InputOption.InitialGradient = atof(argv[i+1]);
		}
		else if (temp == "-DeclusterGradient")
		{
			InputOption.DeclusterGradient = atof(argv[i+1]);

		}
		else if (temp == "-EnlargeBlock")
		{
			InputOption.EnlargeBlockRatio = atof(argv[i+1]);
		}
		else if (temp == "-EnlargePower")
		{
			InputOption.EnlargePowerRatio = atof(argv[i+1]);
		}
		else if (temp == "-3DIC")
		{
			int Flag = atoi(argv[i+1]);
			if(Flag == 1)
				InputOption.ThreeDimIC = true;
		}
		else if (temp == "-3DICMULTI")
		{
			int Flag = atoi(argv[i+1]);
			if(Flag == 1)
				InputOption.ThreeDimICMulti = true;
		}
		else if (temp == "-3DALIGN")
		{
			int Flag = atoi(argv[i+1]);
			if(Flag == 1)
				InputOption.ThreeDimAlign = true;
		}
		else if (temp == "-ThermalTSV")
		{
			int Flag = atoi(argv[i+1]);
			if(Flag == 1)
				InputOption.ThermalTsv = true;
		}
		else if (temp == "-CompCorb")
		{
			int Flag = atoi(argv[i+1]);
			if(Flag == 1)
				InputOption.CompCorblivar = true;
		}
		else if (temp =="-Thermal")
		{
			int Flag = atoi(argv[i+1]);
			if(Flag == 1)
				InputOption.ThermalOpt = true;
		}
		else if (temp == "-shConst") // EP tree
		{

			InputOption.shConst = atoi(argv[i+1]);
		}
		else if (temp == "-LG")
		{
			string Name = argv[i+1];
			if( Name == "EP" )
			{
				InputOption.EPorILP = true;
			}
			else
				InputOption.EPorILP  = false;
		}
		else if (temp== "-ITRI")
		{
			InputOption.ITRIPath = argv[i+1];
		}
		else
		{
			if( (i)%2 == 1)
			{
				cout << "warning: unable to find command: " <<temp <<endl;
				exit(EXIT_FAILURE);
			}
		}*/
	}

	string t="0";
	InputOption.test_mode=201;
		
	InputOption.benName_sec=InputOption.benName;
	InputOption.benName_sec+="_";
	InputOption.benName_sec+=to_string(InputOption.test_mode);
	/*tmpnt.output_name+="_";
	tmpnt.output_name+=to_string(InputOption.test_mode);*/
	/*if( InputFlag[0] == false || InputFlag[1] == false )
	{
		if( InputFlag[0] == false )
		{
			cout << BOLD(Red(" Error : Input the Benchmark Type [-mode] "))<<endl;
		}
		else if (InputFlag[1] == false)
		{
			cout<< BOLD(Red(" Error : Input the Benchmark Name [-benName]"))<<endl;
		}
		exit(EXIT_FAILURE);

	}*/

	//cout<<"TEST MODE : "<<InputOption.test_mode<<endl;
	cout<<"*****START PROGRAM*****\n";
	//cout<< InputOption <<endl;

}
void calarea(NETLIST &tmpnt)
{
	if(InputOption.QP_key==true){
		long long int total=0;
		long long int top=0;
		long long int bot=0;
		for(int i=0;i<tmpnt.GetnMod();i++){
			if(tmpnt.mods[i].modL==0){
				top+=tmpnt.mods[i].modH*tmpnt.mods[i].modW;
			}
			else{
				bot+=tmpnt.mods[i].modH*tmpnt.mods[i].modW;
			}
			total+=tmpnt.mods[i].modH*tmpnt.mods[i].modW;
		}
		tmpnt.totalModArea=total;
		tmpnt.topModArea=top;
		tmpnt.botModArea=bot;
	}
	else{
		long long int total=0;
		long long int top=0;
		long long int bot=0;
		for(int i=0;i<tmpnt.GetnMod();i++){
			if(tmpnt.mods[i].is_m==false){
				if(tmpnt.mods[i].modL==0){
					top+=tmpnt.mods[i].modH*tmpnt.mods[i].modW;
				}
				else{
					bot+=tmpnt.mods[i].modH*tmpnt.mods[i].modW;
				}
				total+=tmpnt.mods[i].modH*tmpnt.mods[i].modW;
			
			}
		}
		tmpnt.totalModArea=total;
		tmpnt.topModArea=top;
		tmpnt.botModArea=bot;
	
	}
}
void MultiLevel(NETLIST &tmpnt )
{
	cout<< BOLD(Yellow("\\ === Thermal-aware Multi-level GD === //"))<<endl;
	//Container to store each level netlist
	vector < NETLIST > ClusteredNET;
	NETLIST temp=tmpnt;

	ClusteredNET.clear();
	ClusteredNET.push_back(temp);

	/// BEST CHOICE Clustering (coarsening step)
	CLUSTER BC_Clustering;
	BC_Clustering.SetBool( false ); // True for Thermal, false for Orignal Score fcn. 5/20 turn for false


	// 6/5註解掉
	/*if (tmpnt.GetnLayer() == 1)
	  BC_Clustering.Clustermain(temp,ClusteredNET);
	  else*/
	BC_Clustering.Clustermain3D(temp,ClusteredNET);

	// Build Table
	// create exponential table
	WLGP WLDrivenGP;
	WLDrivenGP.SetFlagPlot(false);
	WLDrivenGP.SetFpPlotPath("./out/MAP/ThermalMap/");
	WLDrivenGP.CreateExpTable(); // ExpTable is Global Para, just Create Once.
	WLDrivenGP.SetTotalLevel( (int) ClusteredNET.size() );
	/// Apply Multi-level (refinement step)
	for (int i = 0; i < ClusteredNET.size(); i++) {
		cout << "i=" << i << " : mods=" << ClusteredNET[i].GetnMod()<<"\n";
	}
	if( ClusteredNET.size() > 1 )
	{


		int Level = ClusteredNET.size()-1;
		
		
		
		
			
		NETLIST InitialNet = ClusteredNET[Level];


		WLDrivenGP.SetCurrentLevel(Level);
			/*for(int jj=0;jj<ClusteredNET.size();jj++){
				NETLIST INet=ClusteredNET[jj];
				cout<<"\n"<<jj<<"\n";
				for(int ii=0;ii<INet.mods.size();ii++){
								if(INet.mods[ii].modW==0){
									cout<<"mods"<<ii<<" W=0\n";
								}
								if(INet.mods[ii].modH==0){
									cout<<"mods"<<ii<<" H=0\n";
								}
							}
			}*/
		calarea(InitialNet);
		WLDrivenGP.GlobalStage(InitialNet );

		BC_Clustering.DeCluster(InitialNet, Level-1, ClusteredNET);
		//InputOption.QP_key=false;
		//cout<<"Level: "<<Level<<"\n";
		
		if(InputOption.QP_key==true){
			for(int i = Level-1 ; i>= 0 ; i--)
			{
				clock_t start_iter,end_iter;
				start_iter=clock();
				NETLIST CurrentNet = ClusteredNET[i];

				double TargetRatio = 0.03;
				WLDrivenGP.SetCurrentLevel(i);
				//6/5把if else判斷是註解掉 改成if>=0
				
				

				// Decluster and Update cell positions from i to i-1
				if(i!= 0)
					BC_Clustering.DeCluster( CurrentNet, i-1 , ClusteredNET);
				if(i==0)
				{
					tmpnt = CurrentNet;
				}
				end_iter=clock();
				//cout<<"\nThis cluster's GP takes: "<<(double)(end_iter-start_iter)/CLOCKS_PER_SEC<<"s\n";
			}
		}
		else{
			for(int i = Level-1 ; i>= 0 ; i--)
			{
				clock_t start_iter,end_iter;
				start_iter=clock();
				NETLIST CurrentNet = ClusteredNET[i];

				double TargetRatio = 0.03;
				/*
				Thermal_overflow_bound = TargetRatio * LinearCurve(i);
				double ThermalOvf = CalThermalOVerflow(CurrentNet);
				double AreaOvf = 100 * (float)overflow_area_1/(float)tmpnt.totalModArea ;
				CalculateHPWLDrivenGP(CurrentNet);
				cout<< BOLD(Yellow(" ------------- Current State -------------"))<<endl;
				cout<< " - Level : " << i << endl;
				cout<< " - ThermalOvf: "<< ThermalOvf * 100 << " %"<<endl;
				cout<< " - AreaOvf: "<< AreaOvf <<endl;
				cout<< " - Wirelength : "<< CurrentNet.totalWLDrivenGP <<endl;
				cout<<setw(10) << ThermalOvf * 100 <<" %" ;
				cout<<setw(10) << AreaOvf << "%" <<endl;
				*/

				//if(ThermalOvf > Thermal_overflow_bound)

				//6/5把if else判斷是註解掉 改成if>=0
				if(i>0)
				{
					WLDrivenGP.SetCurrentLevel(i);
					
					calarea(CurrentNet);
					WLDrivenGP.GlobalStage(CurrentNet);
				}
				else if(i==0){
					WLDrivenGP.SetCurrentLevel(i);
					cout<<"last stage: start monitor end time.\n";
					calarea(CurrentNet);
					WLDrivenGP.GlobalStage_last_stage(CurrentNet);
				}
				//else
				//{
				//5/18註解調
				/*THERMALGP ThermalAwareGP;
				ThermalAwareGP.SetCurrentLevel(i);
				ThermalAwareGP.SetTotalLevel((int) ClusteredNET.size() );


				ThermalAwareGP.SetFlagPlot(true); // If turn on this option, please set plots path
				// The default path is set in .h
				ThermalAwareGP.SetFpPlotPath("./out/MAP/ThermalMap/");
				ThermalAwareGP.SetBinMapPath("./out/MAP/PotentialMap/");
				ThermalAwareGP.ThermalGlobal(CurrentNet);*/
				//}

				// Decluster and Update cell positions from i to i-1
				if(i!= 0)
					BC_Clustering.DeCluster( CurrentNet, i-1 , ClusteredNET);
				if(i==0)
				{
					tmpnt = CurrentNet;
				}
				end_iter=clock();
				//cout<<"\nThis cluster's GP takes: "<<(double)(end_iter-start_iter)/CLOCKS_PER_SEC<<"s\n";
			}
		}
	}
	else /// Without Multi-level & Optimize Thermal
	{
		cout << BOLD(Red( "[Warning] : Using the Multi-Level, but the level count = 0" ))<<endl;
		cout << BOLD(Red( "[Warning] : The Target Cluster Number Should be Set" ))<<endl;
		WLDrivenGP.SetCurrentLevel(0);
		WLDrivenGP.GlobalStage(tmpnt);

	}


}
void ChooseParser(NETLIST & nt,Lib_vector& lib_vector,NETLIST_o & nt_o)
{

	/// parser
	switch(InputOption.mode)
	{
		// GSRC
		case 1:
			IO::GSRC::ReadBlockFile (InputOption.benName, nt);
			//IO::GSRC::ReadPlFile (InputOption.benName, nt);
			// Check 3DIC
			if( InputOption.ThreeDimIC  )
				IO::GSRC::ReadLayerFile (InputOption.benName, nt);
			else
			{
				nt.SetnLayer(1);
				nt.InitailLayer();
			}
			// Check Thermal
			if(InputOption.ThermalOpt){
				//IO::POWER::ReadPowerFile_ITRI(InputOption.benName, nt);

			}
			// Note Net File is needed the last
			IO::GSRC::ReadNetFile (InputOption.benName, nt);

			if( InputOption.ThreeDimAlign )
				IO::ThreeDim::ReadAlignFile(InputOption.benName, nt);

			break;
			// IBM-HB+
		case 2:
			IO::IBM::ReadBlockFile (InputOption.benName, nt);
			IO::IBM::ReadPlFile (InputOption.benName, nt);
			// Check 3DIC
			if(InputOption.ThreeDimIC)
				IO::GSRC::ReadLayerFile (InputOption.benName, nt);
			else
			{
				nt.SetnLayer(1);
				nt.InitailLayer();
			}
			// Check Thermal
			if(InputOption.ThermalOpt)
				IO::POWER::ReadPowerFile_ITRI(InputOption.benName, nt);

			if(InputOption.ThreeDimIC)
			{
				IO::IBM::ReadNetFile3D (InputOption.benName, nt);
				//IO::ThreeDim::ReadAlignFile(InputOption.benName, nt);
			}
			else
				IO::IBM::ReadNetFile (InputOption.benName, nt);

			if( InputOption.ThreeDimAlign )
				IO::ThreeDim::ReadAlignFile(InputOption.benName, nt);
			break;

		case 3:
			//IO::GSRC::ReadLayerFile
			IO::CAD::ConvertBlock(InputOption.benName, nt,lib_vector);
			IO::GSRC::ReadBlockFile (InputOption.benName, nt);
			if( InputOption.ThreeDimIC  ){
				/*IO::GSRC::ConstantLayer(InputOption.benName);
				IO::GSRC::ReadLayerFile (InputOption.benName, nt);*/

				//IO::GSRC::ConvertLayer(NETLIST_o nt_o,NETLIST & nt);
				IO::GSRC::ConvertLayer(nt_o, nt);
				
				

				//test layer
				int topn=0;
				int botn=0;
				/*for(int i=0;i<nt.GetnMod();i++){
					cout<<i<<": ";
					if(nt.mods[i].modL==1){
						cout<<"1"<<nt.mods[i].modW<<" "<<nt.mods[i].modH<<"\n";

						botn++;
					}
					else if(nt.mods[i].modL==0){
						
						cout<<"0"<<nt.mods[i].modW<<" "<<nt.mods[i].modH<<"\n";
						topn++;
					}
					else{
						cout<<"9999999999\n";
					}
				}
				cout<<topn<<" "<<botn<<"\n\n";*/
			}
			else
			{
				nt.SetnLayer(1);
				nt.InitailLayer();
			}
			//IO::CAD::ConvertNet
			IO::GSRC::ReadNetFile (InputOption.benName, nt);

			if( InputOption.ThreeDimAlign )
				IO::ThreeDim::ReadAlignFile(InputOption.benName, nt);

			if(nt.topn==5 && nt.botn==3){
				InputOption.realcase=1;
			}
			if(nt.topn==12282 && nt.botn==1625){
				InputOption.realcase=2;
			}

			break;
		default:
			cout << "error  : mode " << InputOption.mode << " doesn't exist!" << endl;
			exit(EXIT_FAILURE);
			break;
	}
}

void InitialFloorplan(NETLIST &tmpnt)
{

	///set random seed
	bool random_assign_pad=true;   ///<true for 3D false for 2D
	unsigned int randSeed = time(NULL);
	randSeed=56 ;
	srand( randSeed );

	// set fixed-outline region

	// 6/8註解掉
	//tmpnt.SetFixedOutline();




	// allocate locations of pads; 0: scale to boundary, 1: radomly assign

	//5/7註解掉
	tmpnt.NETLIST::AllocatePad( random_assign_pad );
	//PLOT::PlotPad( tmpnt,"./out/plotlayout", random_assign_pad);//0: scale to boundary, 1: radomly assign


	tmpnt.SetoutputPath(InputOption.OutputPath);

	// Close Voltage Driven Mode
	tmpnt.SetnVI(0);

}
void GlobalFP ( NETLIST & tmpnt)
{

	if(InputOption.ThermalOpt) // Thermal-Aware Multi-Level Gp(2018 ICCAD)
	{

		if(InputOption.ThreeDimIC == true && InputOption.ThreeDimICMulti == false)
		{

			WLGP WLGlobalFP;
			WLGlobalFP.SetTotalLevel(2);
			WLGlobalFP.SetCurrentLevel(1);
			WLGlobalFP.CreateExpTable();
			WLGlobalFP.GlobalStage( tmpnt  );

			/*THERMALGP ThermalAwareGP;
			  ThermalAwareGP.SetCurrentLevel(0);
			  ThermalAwareGP.SetTotalLevel(2 );
			  ThermalAwareGP.SetSuperBig(5);
			  ThermalAwareGP.Setrho(1);
			  ThermalAwareGP.SetTFM(true);
			  ThermalAwareGP.SetFlagPlot(true);
			  ThermalAwareGP.SetFpPlotPath("./out/MAP/ThermalMap/");
			  ThermalAwareGP.SetBinMapPath("./out/MAP/PotentialMap/");
			  ThermalAwareGP.ThermalGlobal(tmpnt);*/
		}
		else if(InputOption.ThreeDimIC == false && InputOption.ThreeDimICMulti == false){
			WLGP WLGlobalFP;
			WLGlobalFP.SetTotalLevel(2);
			WLGlobalFP.SetCurrentLevel(1);
			WLGlobalFP.CreateExpTable();
			WLGlobalFP.GlobalStage( tmpnt  );
		}
		else // The Clustering cannot handle TSV
			MultiLevel( tmpnt );
	}
	else // Wirelength Driven & Without Multi-level Framework
	{
		// Note if you have the layer files, it will imply nLayer > 1,
		// which will turn on the 3D mode.
		/*WLGP WLGlobalFP;
		  cout<<"11\n";
		  WLGlobalFP.SetTotalLevel(1);
		  cout<<"12\n";
		  WLGlobalFP.SetCurrentLevel(0);
		  cout<<"13\n";
		  WLGlobalFP.CreateExpTable();
		  cout<<"14\n";
		  WLGlobalFP.SetFlagPlot(true);
		  cout<<"15\n";
		  WLGlobalFP.SetFpPlotPath("./out/MAP/ThermalMap/");
		  cout<<"16\n";
		//cout<<"hi";
		WLGlobalFP.GlobalStage( tmpnt  );
		cout<<"16\n";*/
		if(InputOption.ThreeDimICMulti == true){
			MultiLevel(tmpnt);
		}
		else{
			WLGP WLGlobalFP;
			WLGlobalFP.SetTotalLevel(2);
			WLGlobalFP.SetCurrentLevel(1);
			WLGlobalFP.CreateExpTable();
			WLGlobalFP.GlobalStage( tmpnt  );
		}
	}
}
/*void LegalFP(NETLIST & tmpnt)
{
	if(InputOption.EPorILP == false)
	{
		// ILP based Legalization
		ILPLG ILP_Legalizer;
		int legal_OorX=ILP_Legalizer.LegalizationC(tmpnt);
	}
	else
	{
		// EP based Leglization
		EPLG  epLeglizer;
		epLeglizer.EPlegal(tmpnt);
	}
}*/
/*void AssignTSV(NETLIST & tmpnt)
{
	//analyze temperature
	//the functions are writen in the TemperatureAnalyze.h and.cpp
	

	// MCMF based TSV Assignment
	// EP based Solver include the TSV Assignment, so don't need to Assign TSV
	if(tmpnt.GetnLayer()>1 && InputOption.EPorILP == false)
	{
		cout<<"* assign TSVs...\n";
		LGTSV TSVLegalizer;
		TSVLegalizer.legalTSV(tmpnt);
	}

	ILPLG Legalizer;
	cout<< BOLD (Green ( " - nt.totalWL="));
	cout << Legalizer.calculate_HPWL(tmpnt)<<endl;
}*/
void OutputFP(NETLIST & tmpnt)
{

	// ------ Dump Files ------ //
	cout << "** Dump legal Distribution Result *_legal*" << endl;
	DUMP::Academic::DumpFiles(tmpnt, "./out/NCKU_FP", "_Academic");
	string HotSpotOutput = tmpnt.GetoutputPath() + "NCKU_FP";


	// ------ Dump HotSpot Files ------ //
	if(InputOption.CorbBench != "")
	{
		DUMP::Corblivar HotSpotDumper;
		HotSpotDumper.SetPowerAmp(10);
		HotSpotDumper.DumpFiles(tmpnt,HotSpotOutput,"_HotSpot", InputOption.CorbBench);
	}


	// ----- Dump ITRI Solver Files -----//

	if(InputOption.ITRIPath != "")
	{

		DUMP::ITRI::DumpFiles(tmpnt, InputOption.ITRIPath);
	}



}
void CalculateHPWL_ML(NETLIST & tmpnt)
{
    int maxX = 0;
    int minX = tmpnt.ChipWidth;
    int maxY = 0;
    int minY = tmpnt.ChipHeight;
    int centerX;
    int centerY;

	tmpnt.totalWL_ML = 0.0;

    for(int i = 0; i < tmpnt.nNet; i++)
    {
        if(tmpnt.nets[i].flagAlign) ///pseudo net
            continue;

        maxX = 0;
        minX = tmpnt.ChipWidth;
        maxY = 0;
        minY = tmpnt.ChipHeight;

        for(int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head+tmpnt.nets[i].nPin; j++)
        {
            if(tmpnt.pins[j].index < tmpnt.nMod)
            {
                centerX = tmpnt.mods[tmpnt.pins[j].index].GCenterX;
                centerY = tmpnt.mods[tmpnt.pins[j].index].GCenterY;
            }
            
            else
            {
                centerX = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].x;
                centerY = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].y;
               
            }

            maxX = (centerX > maxX) ? centerX : maxX;
            minX = (centerX < minX) ? centerX : minX;
            maxY = (centerY > maxY) ? centerY : maxY;
            minY = (centerY < minY) ? centerY : minY;
        }
        tmpnt.totalWL_ML = tmpnt.totalWL_ML + (double)(((maxX - minX) + (maxY - minY))/AMP_PARA);
    }

}
void CalculateHPWL_CL(NETLIST & tmpnt)
{
    int maxX = 0;
    int minX = tmpnt.ChipWidth;
    int maxY = 0;
    int minY = tmpnt.ChipHeight;
    int centerX;
    int centerY;

	tmpnt.totalWL_CL = 0.0;

    for(int i = 0; i < tmpnt.nNet; i++)
    {
        if(tmpnt.nets[i].flagAlign) ///pseudo net
            continue;

        maxX = 0;
        minX = tmpnt.ChipWidth;
        maxY = 0;
        minY = tmpnt.ChipHeight;

        for(int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head+tmpnt.nets[i].nPin; j++)
        {
            if(tmpnt.pins[j].index < tmpnt.nMod)
            {
                centerX = tmpnt.mods[tmpnt.pins[j].index].GCenterX;
                centerY = tmpnt.mods[tmpnt.pins[j].index].GCenterY;
            }
            
            else
            {
                centerX = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].x;
                centerY = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].y;
               
            }

            maxX = (centerX > maxX) ? centerX : maxX;
            minX = (centerX < minX) ? centerX : minX;
            maxY = (centerY > maxY) ? centerY : maxY;
            minY = (centerY < minY) ? centerY : minY;
        }
        tmpnt.totalWL_CL = tmpnt.totalWL_CL + (double)(((maxX - minX) + (maxY - minY))/AMP_PARA);
    }

}

/*!
 * \file 	IO.h
 * \brief 	File In & Out (header)
 *
 * \author 	 SEDA Lab
 * \version	2.0 (2-D)
 * \date	2018.07/
 */

#ifndef _IO_H_
#define _IO_H_

#include "dataType.h"
#include "structure_o.h"
#include "dataType.h"
class IO
{

	public:
		// Declare Static to directly Call Fcn.
		/* Parser Type */
		class CAD2023
		{
			public:
				void outFile(string benName, NETLIST &tmpnt); 
				void outFile_01(string benName, NETLIST &tmpnt); 
		};
		class IBM
		{
			public:
				static void ReadBlockFile (string benName,NETLIST &nt ) ;
				static void ReadNetFile (string benName,NETLIST &tmpnt) ;
				static void ReadNetFile3D (string benName,NETLIST &tmpnt) ;
				static void ReadPlFile (string benName, NETLIST &tmpnt) ;
				static void ReadLayerFile( string layerFile, NETLIST& tmpnt );
		};
		class GSRC
		{

			public:
				static void ReadBlockFile (string benName,NETLIST &nt ) ;
				static void ReadNetFile (string benName,NETLIST &tmpnt) ;
				static void ReadPlFile (string benName, NETLIST &tmpnt) ;
				static void ReadLayerFile( string layerFile, NETLIST& tmpnt );
				static void ConvertLayer(NETLIST_o nt_o,NETLIST & nt);
		};
		class CAD
		{
			public:
				static void ConvertBlock(string benName, NETLIST& nt, Lib_vector& lib_vector) ;
		};
		class POWER
		{
			public:
				static void ReadPowerFile_ITRI(string benName, NETLIST &tmpnt) ;
		};
		class ThreeDim
		{
            public:
                //Read alignment constraint and add new net.
                //this function must be used after net parser.
                static void ReadAlignFile (string benName, NETLIST &tmpnt) ;
		};
		class Industry
		{

		};

};

class DUMP
{
	public:
		class Academic
		{
			public:
				static void DumpFiles(NETLIST& tmpnt, string file, string subfile);
				static void DumpLayer( NETLIST& tmpnt, char* file, char* subfile );
				static void DumpNets( NETLIST& tmpnt, char* file, char* subfile );
				static void DumpBlocks( NETLIST& tmpnt, char* file, char* subfile );
				static void DumpPl( NETLIST& tmpnt, char* file, char* subfile );
		};
		class Corblivar
		{
			private:
				double SiResistivity;
				double SiSpecificHeat;
				double BondResistivity;
				double BondSpecificHeat;
				double CuResistivity;
				double CuSpecificHeat;
				double BEOLResistivity;
				double BEOLSpecificHeat;
				double PowerAmp;
				double scale;
				int HotspotLayer;
				int NowNtLayer;
				int GridResolution;//must be the power of 2
				double **GridTSVRatioArr;
				double **GridTSVAreaArr;

			public:
				// Set
				inline void SetR(double value){
					SiResistivity = value;
				};
				inline void SetSP(double value){
					SiSpecificHeat = value;
				}
				inline void SetPowerAmp(double value){
					PowerAmp = value;
				}

				// Output Fcn.
				 void DumpFiles(NETLIST& tmpnt, string file , string subfile , string benName);
				 void DumpPowerHotSpot( NETLIST& tmpnt, string file , string subfile  );
				 void DumpPowerFile_EVA( NETLIST& tmpnt, string file , string subfile  ) ; // This File is used for Corblivar
				 void DumpLayoutFile(NETLIST& tmpnt, string file , string subfile , string benName);
				 void DumpFpInFo(NETLIST& tmpnt, string file , string subfile , string benName); // Last one is Corblivar benName
				 void DumpPlHotSpot( NETLIST& tmpnt, string file , string subfile  );
				 void DumpPassive(NETLIST& tmpnt, string file , string subfile) ;

				 //new output Fcn
				 void DumpLcfHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpFlpHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpBeolHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpActiveHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpPassiveHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpBondHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpPtraceHotspot(NETLIST& tmpnt, string file , string subfile, string benName);

				 //PASSIVE and BOND cannot convergence. Rewrite the Fcn. in grid gormat
				 void DumpActiveGridHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpPassiveGridHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void DumpBondGridHotspot(NETLIST& tmpnt, string file , string subfile, string benName);
				 void CalculateGridTSVRatio(NETLIST& tmpnt);




				 Corblivar(){
                    SiResistivity = 0.00851064;
                    SiSpecificHeat = 1.631e+06;
                    BondResistivity = 5.0;
                    BondSpecificHeat = 2.29854e+06;
                    CuResistivity = 2.5316e-03;
                    CuSpecificHeat = 3.546401e+06;
                    BEOLResistivity = 	0.4444;
                    BEOLSpecificHeat = 1.20815e+06;
                    scale = 1000000;
                    PowerAmp= 1;
                    HotspotLayer=0;
                    NowNtLayer=0;
                    GridResolution = 64;

                    ///initial array
                    GridTSVRatioArr = new double*[this->GridResolution];
                    for(int i=0; i<this->GridResolution; i++)
                        GridTSVRatioArr[i] = new double[this->GridResolution];
                    GridTSVAreaArr = new double*[this->GridResolution];
                    for(int i=0; i<this->GridResolution; i++)
                        GridTSVAreaArr[i] = new double[this->GridResolution];
				 }

		};
		class ITRI
		{
			public:
				static void DumpFiles(NETLIST &tmpnt, string path );
		};

};
class PLOT
{

	private :
		bool Coloring; // Coloring accodring to module Group (Power)
		bool Texting ; // Writing the Moudle Name
	public:
		static void PlotPad( NETLIST& tmpnt, string file, int allocate_mode );
		static void PlotMap ( vector <vector <double> > &Map, string MapName, int binW, int binH );
		void dump_pl(NETLIST nt, string blockFile,int mod_num,int gp_ml);
		void dump_nodes(NETLIST nt, string blockFile,int mod_num);
		void dump_deflef(NETLIST nt, string blockFile);
		void plot_step( NETLIST &tmpnt, string filename );
		void PlotResultGL_dy( NETLIST& tmpnt, string file ,bool GorL);
		inline void SetColoring(bool Flag){
			Coloring = Flag;
		};
		inline void SetTexting(bool Flag){
			Texting = Flag;
		};
		void Reset(){
			Coloring = Texting = false;
		};
		PLOT(){
			Coloring = Texting = false;
		};
};
class UTILITY
{
	public:
		static string Int_2_String(int value){
			string Output;
			stringstream ss;
			ss << value;
			ss >> Output;
			return Output;
		};
		static int String_2_Int(string Name){
			int value;
			stringstream ss;
			ss << Name ;
			ss >> value ;
			return value;
		};
		static pair<double,double> GaussAnalysis(vector <double> data){
			double mean=0;
			for(unsigned int i=0; i < data.size() ; i++)
			{

				mean += data[i] ;
			}
			mean = (double) mean /data.size()  ;
			double standard = 0;
			double sigma =0;
			double dev=0;
			for(int i=0 ; i<data.size()  ; i++)
			{
				standard = data[i] - mean;
				standard *= standard;
				sigma += standard;
			}
			sigma = (double) sigma /data.size() ;

			dev = sqrt(sigma);
			pair <double,double > temp;
			temp.first = mean;
			temp.second = dev;
			return temp;
		};
		static vector <double> Normalize(vector <double > data){

			double MaxNum = -1 ;
			double MinNum = 999999 ;

			for(int i=0; i< data.size(); i++)
			{
				if(MaxNum< data[i])
					MaxNum = data[i];
				if( MinNum > data[i])
					MinNum = data[i];
			}
			vector <double > Normal ;
			Normal.resize( data.size() );
			for(int i=0; i< data.size(); i++)
			{
				Normal[i] = (double) ( data[i]- MinNum )  / (MaxNum - MinNum) ;
			}

			return Normal;
		}
		static const string CurrentTime()
		{
			char buf[80];
			time_t now = time(0);
			struct tm tstruct = *localtime(&now);

			strftime( buf, sizeof(buf), "%c", &tstruct );

			return buf;
		}

};



#endif


/*!
 * \file 	dataType.h
 * \brief 	data structures (header)
 *
 * \author 	SEDA NCKU LAB
 * \version	2.0 (3-D) by Tai-Ting
 * \date	2018.08.
 */

#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>
#include <map>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <set>
#include <queue>
#include <sstream>
#include <math.h>

// laspack
extern "C"
{
#include <errhandl.h>
#include <vector.h>
#include <qmatrix.h>
#include <operats.h>
#include <version.h>
#include <copyrght.h>
#include <precond.h>
#include <itersolv.h>
#include <rtc.h>
#include <elcmp.h>
#include <float.h>
#include <limits.h>
}

using namespace std;

//5/26 1000->1
#define AMP_PARA 1			///< constant to enlarge data (avoid rounding problem)
#define FILELEN 100				///< file length
#define HARD_BLK '0'			///< hard module
#define SOFT_BLK '1'			///< soft module
#define MAX_BASE 10				///< maximum number of modules in the subcircuit
#define MAX_CURVE_SIZE 100000	///< maximum size of temporary curve
#define MAX_WL 1e+15			///< maximum wirelength
#define MAX_INT 2147483647		///< maximum integer
#define MAX_FLT 3e+20			///< maximum float
#define TSV_PITCH 3				///< TSV pitch 3
#define TSV_SIZE 3  			///< size of a TSV 3
#define TSV_AREA (TSV_SIZE * TSV_SIZE)				///< area of a TSV
#define COM_MIN(x, y) (((x) < (y)) ? (x) : (y))			///< min comparison
#define COM_MAX(x, y) (((x) > (y)) ? (x) : (y))			///< max comparison
#define EXP(x) ExpTable[ (int)(1000*x) + 700000 ]	///< exponential conversion
#define ENLARGE_RATIO 1
#define HOT_MOD_ENLARGE 1.4

/* Color Setting */
#define RST  "\x1B[0m"
#define KRED  "\x1B[91m"
#define KGRN  "\x1B[92m"
#define KYEL  "\x1B[93m"
#define KBLU  "\x1B[94m"
#define KMAG  "\x1B[95m"
#define KCYN  "\x1B[96m"
#define KWHT  "\x1B[97m"

#define Red(x) KRED x RST
#define Green(x) KGRN x RST
#define Yellow(x) KYEL x RST
#define Blue(x) KBLU x RST
#define Magenta(x) KMAG x RST
#define Cyan(x) KCYN x RST
#define White(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

/*------------------------------*
 *
 * Class
 *
 *------------------------------*/
//class IO;
class MODULE;
class NET;
class PIN;
class PAD;
class IO;
class CLUSTER;
class POINT;
class CURVE;
class AlIGN_PAIR;
//class Lib;
//class Libcell;

struct str_coor
{
    int x;
    int y;
};
class NETLIST
{

	friend ostream& operator<<(ostream &, const NETLIST &);
	protected:

		

		int FenceArea;          ///< area of fence region

		
		

		float maxWS;			///< given maximum whitespace
		float aR;				///< given aspect ratio (= ChipWidth/ChipHeight)

		map<string, int> mod_NameToID;	///< map for modName to idMod
		map<string, int> pad_NameToID;	///< map for padName to idPad (w/o adding nMod)
        vector< vector< vector<double> > > simTemperature;   ///<temperature map from ThermalGlobal

		int nVI;///f///         ///< number of voltage islands
		string outputPath;

	public:
		int topn;
		int botn;
		int nHard;				///< number of hard modules
		int nSoft;				///< number of soft modules
		int nMod;				///< number of modules
		
		vector<int> LayernMod;  ///< number of modules at each layers
		int nNet;				///< number of nets
		
		int nPin;				///< number of pins
		
		int nPad;				///< number of pads
		
		int nAlign;              ///< number of align pairs
		vector<AlIGN_PAIR> aligns;   ///<align pairs

		int nTSV;				///< number of TSVs
		int nLayer;				///< number of layers

		int ChipWidth;			///< width of the chip
		int ChipHeight;			///< height of the chip
		float ChipArea;			///< area of the chip
		double totalWL;			///< total wirelength
		double totalWL_GP;		///< total wirelength for global distribution
		double totalWL_ML;		///< total wirelength for global distribution
		double totalWL_CL;		///< total wirelength for global distribution
		long long int totalModArea;		///< total area of all modules
		long long int topModArea;
		long long int botModArea;
		string output_name;
		double row_high_top;
		double NumRows_top;
		double Xmin_top;
		double Ymin_top;
		double Xmax_top;
		double Ymax_top;

		double row_high_bot;
		double NumRows_bot;
		double Xmin_bot;
		double Ymin_bot;
		double Xmax_bot;
		double Ymax_bot;

		double toputil;
		double botutil;

		string toptech;
		string bottech;

		double terminal_w;
		double terminal_h;
		double terminal_space;
		double terminal_cost;

		vector<MODULE> mods;	///< modules
		vector<NET> nets;		///< nets
		vector<PIN> pins;		///< pins
		vector<PAD> pads;		///< pads
		/* Utility */
		friend class IO;			///< class to Parse Files
		friend class PLOT;			///< class to Plot Maps
		friend class DUMP;			///< class to Dump Files
		friend class TEMPANALYZE;
		friend class Abacus;

		/* Clustering */
		friend class CLUSTER;		///< class to do Best-Choice Clustering
		/* Global Placement */
		friend class GP;			///< Basic class for Analytical Placement
		friend class WLGP;			///< Wirelength Driven Placement			(NTUplace3)
		friend class THERMALGP;		///< Thermal-Aware Placement				(m105 Tai-Ting Chen)
		/* Legalization */
		friend class ILPLG;			///< ILP based Legalization 				(m102 Chih-Yao Hu)
		friend class EPLG;			///< EP based Legalization 					(m100 Kai-Chung Chan)
		/* TSV Assignment */
		friend class LGTSV; 		///< MCMF based TSV assignment 				(m102 Pei-Shan Wu)


		friend class AlIGN_PAIR;
		friend class IRSOLVER;

 		void SetFixedOutline(int w,int h);
		void AllocatePad(bool random_assign_pad );


		/* Friend Fcn. */
		friend int solveCPLEX3D(vector<int> nVertex3D,vector<vector <MODULE*> > module3,vector<vector < vector<int> > > matricesH3D,vector< vector < vector<int> > > matricesV3D,NETLIST nt);

		void InitailLayer();
		// Value Fcn.
		inline void SetnLayer(int value){
			nLayer  = value ;
		};
		inline void SetoutputPath(string Name){
			outputPath = Name;
		}

		inline void SetmaxWS(float value){
			maxWS  = value ;
		};

		inline void SetaR(float value){
			aR  = value ;
		};
		inline void SetnVI(int value){
			nVI = value ;
		}

		// Get Value
		inline int GetnLayer() const {
			return nLayer;
		};
		inline int Getw() const {
			return ChipWidth;
		};
		inline int Geth() const {
			return ChipHeight;
		};
		inline int GetnMod()const{
			return nMod;
		};
		inline int GetLayernMod(int layer) const{    ///get module number at each layer
            return LayernMod[layer];
		};
		inline int GetnNet()const{
			return nNet;
		};
		inline int GetnPin()const{
			return nPin;
		};
		inline int GetnPad()const{
			return nPad;
		};
		inline float GetmaxWS()const{
			return maxWS;
		};
		inline float GetaR()const{
			return aR;
		};
		inline string GetoutputPath()const{
			return outputPath ;
		};
		inline int GetModID(string modName){
			map <string , int >:: iterator it1 ;

			it1 = mod_NameToID.find( modName );
			if(it1 != mod_NameToID.end())
			{
				if(it1->second < nMod)
					return it1->second;
				else
				{
					cerr << "Error : Exceed mod Index"<<endl;
					exit(1);
				}
			}
			else
			{
				cerr << "Error: Can't find modname : "<< modName<<endl;
				exit(1);
			}
		};

		NETLIST();
		NETLIST& operator=(const NETLIST &);
		~NETLIST();
};
/*   EP Class   */
class POINT
{
	friend ostream& operator<<(ostream &, const POINT &);
	public:
	unsigned int x;	///< point x
	unsigned int y;	///< point y

	/// 1) store the point index in left shapeCurve which generate point i \n
	/// 2) record the buffCurve index in the merge process \n
	int traceL;

	/// 1) store the point index in the right shapeCurve which generate point i \n
	/// 2) record the point index of the buffCurve index recorded in traceL in the \n
	///    merge process \n
	int traceR;

	/// record the direction information; 0: horizontally, 1: vertically
	char traceD;

	POINT();
	POINT& operator=(const POINT &);
	~POINT();
};

///\brief class CURVE \n
/// shape curve (leftCurve & rightCurve) \n
/// 1) for the shapeCurveN/subCurve/buffCurve which has been generated by adding, \n
///    e.g. leftCurve points to left shapeCurveN, rightCurve points to right shapeCurveN \n
/// 2) for the shapeCurveN/subCurve which has been generated by merging, \n
///    e.g. leftCurve is equal to the rightCurve, which is pointing to the shapeCurveN \n
///         itself (See MergeCurveB function) \n
/// 3) for the shapeCurveN which has only one vertex, leftCurve = rightCurve = NULL \n
/// 4) for the shapeCurveB, leftCurve = rightCurve = NULL \n
class CURVE
{
	friend ostream& operator<<(ostream &, const POINT &);
	public:
	CURVE *leftCurve;	///< left shape curve
	CURVE *rightCurve;	///< right shape curve

	/// 1) for shapeCurveB, it records the module index \n
	/// 2) for shapeCurveN, it records the tree node index \n
	/// 3) for buffCurve, it records the buffCurve index \n
	int id;

	int nPoint;			///< number of points in the shape curve
	POINT *points;		///< points in the shape curve

	char flagHalf;		///< flag to determine whether the lower-half curve

	unsigned int traceIndex;	///< the index of the point which will be traced back

	CURVE();
	CURVE& operator=(const CURVE &);
	~CURVE();
};
class PSEUDO_MODULE
{
	//friend ostream& operator<<(ostream &, const PSEUDO_MODULE &);
	public:
		/// the corresponding index of module in the new netlist \n
		/// NOTE: "vector<MODULE> mods" in the new sub-netlist is composed of \n
		///       modules & pseudo modules \n
		int idMod;

		int idClu;			///< the corresponding index of cluster
		int idSet;			///< the corresponding index of cutset

		int idPMod;			///< pseudo module id
		vector<int> idTSV;	///< TSVs in the pseudo module

		int lbX;			///< left-bottom x
		int lbY;			///< left-bottom y
		int rtX;			///< right-top x
		int rtY;			///< right-top y

		int centroidX;		///< centroid x of TSVs
		int centroidY;		///< centroid y of TSVs

		PSEUDO_MODULE(): idMod(0), idClu(0), idSet(0), idPMod(0), lbX(MAX_INT), lbY(MAX_INT), rtX(0), rtY(0), centroidX(0), centroidY(0)
	{ }

		~PSEUDO_MODULE()
		{ }
};


class MODULE
{
	//friend ostream& operator<<(ostream &, const MODULE &);

	public:
		char modName[100];		///< module name
		int idMod;				///< module id
		char type;				///< 0: hard module, 1: soft module

		int modW;				///< module width
		int modH;				///< module height

		int modW_bot;
		int modH_bot;
		int modArea_bot;
		bool is_m;
        int ShrinkW;              ///< module width with shrink
        int ShrinkH;              ///< module height with shrink

		int modL;				///< module layer
		int modArea;			///< module area
		float minAR;			///< minimum aspect ratio (modW/modH)
		float maxAR;			///< maximum aspect ratio (modW/modH)
		float minAR_bot;			///< minimum aspect ratio (modW/modH)
		float maxAR_bot;

		unsigned int nNet;	///< number of nets the module connected with
		unsigned int *pNet;	///< nets array the module connected with

        ///f///global width & height for leglization
		int GmodW;				///< module width for global distribution
		int GmodH;				///< module height for global distribution

		double GLeftX;				///< left-bottom x-coor for global distribution
		double GLeftY;				///< left-bottom y-coor for global distribution
		double GCenterX;			///< center x-coor for global distribution
		double GCenterY;			///< center y-coor for global distribution

		double LeftX;				///< left-bottom x-coor for legalization
		double LeftY;				///< left-bottom y-coor for legalization
		double CenterX;			///< center x-coor for legalization
		double CenterY;			///< center y-coor for legalization

		bool flagTSV;			///< flag to determine whether the TSV; true: TSV, false: module
		bool flagPre;			///< flag to determine whether the preplaced modules; true: preplaced, false: movable
		bool pseudoTSV;        	///y///150519///< pseudoTSV
		bool flagAlign;         ///< flag to determine the module which should be aligned
		bool flagCluster;

		PSEUDO_MODULE *pseudoMod;

		CURVE shapeCurveB;		///< shape curve of the module

		/// function for creating the shape curve of the module
		void CreateCurveB(int totalModArea, int shConst);

		int nLayer;///f///      ///< number of layers the stacked module with; 1: not stack, 2~: stack (different with nt.nLayer)

        int powerMode;///PSWu///
        bool folding;  ///y///teacher say need folding module ///useless term

        double Power;           ///20170221 thermal
        double Pdensity;        ///20170221 thermal

        int TemperatureGroup;   ///20170831 For classify each power density to specfied group , hot = 0 , warm =1 , cold =2

        int ModuleCount;        ///20180419 For the Number of Modules in the Cluster
        vector < int > InClusterMod ; ///20171002 The " last " level Cell in this Cluster


		MODULE();
		MODULE& operator=(const MODULE &);
		~MODULE();
};
class Libcell  //4/26
{
public:
	char name[50];
	int id;
	int w;
	int h;
	int p;
	bool is_m;
	Libcell();
	~Libcell();
};
class Lib  //4/26
{
public:
	string name;
	vector<Libcell> lib_cell;
	map<string, int> lib_NameToID;
	Lib();
	~Lib();
};
class Lib_vector
{

public:
	vector<Lib> lib;
	Lib_vector();
	~Lib_vector();
};
class NET
{
//	friend ostream& operator<<(ostream &, const NET &);
	public:
		char netName[100];		///< net name
		int idNet;				///< net id
		int head;				///< the started index of pins connected by the net
		unsigned int nPin;	///< number of pins connected by the net
		int netL;				///< net layer
		int nTSV;				///< number of TSVs connected by the net

		char flag;				///< if a module need to be moved in the swapping process, its related nets should be marked 1; otherwise, it is 0.

        bool pseudo;///f///     ///< flag for voltage islands; true: seudo net
        bool flagAlign;         ///< flag for the pseudo net to align the modules

		NET();
		NET& operator=(const NET &);
		~NET();
};

class PIN
{
//	friend ostream& operator<<(ostream &, const PIN &);
	public:
		char pinName[100];	///< pin name
		int idPin;			///< pin id
		int index;			///< if the index < number of modules (nMod), it corresponds to mods[index]; otherwise, it corresponds to pads[index - nMod].
		int offsetX;		///< pin offset x
		int offsetY;		///< pin offset y
		int pinL;			///< pin layer
		bool flagTSV;		///< used to sorting pins

		PIN();
		PIN& operator=(const PIN &);
		~PIN();
};

class PAD
{
//	friend ostream& operator<<(ostream &, const PAD &);
	public:
		char padName[100];	///< pad name
		int idPad;			///< pad id
		int x;				///< pad x-coor
		int y;				///< pad y-coor
		int padL;			///< pad layer

		PAD();
		PAD& operator=(const PAD &);
		~PAD();
};

class AlIGN_PAIR
{
    public:
        int lowModId;
        int highModId;

        int lowLayer;
        int highLayer;

        int lowCplexIndex;
        int highCplexIndex;

        char type; ///0:complete 1:center

        AlIGN_PAIR();
		~AlIGN_PAIR();
};

class OPTION
{
	friend ostream& operator<<(ostream &, const OPTION &);
	public:
	int mode;
	int bin_para;
	int shConst;
	int test_mode;
	int realcase; //1 case01,2 case02_hidden
	float overflow_bound;

	double TargetClusterNum;
	double InitialGradient;         ///< In gp. InitialGradient & main
	double DeclusterGradient;       ///< In gp .InitialGradient & main
	double ClusterArea;
	double EnlargeBlockRatio;       ///< In IO
	double EnlargePowerRatio;       ///< In IO

	string benName;
	string benName_sec;
	string CorbBench;
	string ITRIPath;

	bool ThreeDimIC;
	bool ThreeDimICMulti;          ///< 3D multilevel mode
	bool ThreeDimAlign;            ///< 3D Alignment
	bool ThermalOpt;
	bool ThermalTsv;                ///< Thermal TSV to optimize temperature in TSV Assignment
	bool EPorILP ;
	bool CompCorblivar;             ///< Run to compare corblivar
	bool QP_key;
	//bool one_stage;
	string OutputPath;

	OPTION(){

	};
};

class IRBIN ///be used in IRSOLVER.CPP
{
    friend ostream& operator<<(ostream &, const IRBIN &);
    public:

        int width;
        int height;
        int simLayer;
        vector<int> LayerTSV;
        vector<double> LayerPower;
        vector<double> LayerTemperatureWT;
        vector<double> LayerTemperatureWOT;
        double result;
        double resultWT;
        double temperatureVariation;
        double powerVariation;
        bool simFlag; ///1 for has to sim, 0 for not
        IRBIN();
        ~IRBIN();
};
/*------------------------------*
 *
 * External Variables
 *
 *------------------------------*/
extern OPTION InputOption;

//---- swap info ----//
/// array used to store the index of nets which needs to be updated \n
/// in terms of length in the swapping process \n
extern unsigned int *buffNet;
/// Expotential Table Used in LSE model(GP)
extern double ExpTable[1400001];			///< exponential table


#endif


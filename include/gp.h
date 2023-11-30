/*!
 * \file 	gp.h
 * \brief 	global distribution Basic Item [(header)]
 *
 * \author 	Tai-Ting
 * \version	2.0 (3-D)
 * \date	2018.07/
 */

#ifndef _GP_H_
#define _GP_H_


#include "dataType.h"
#include "Cluster.h"
#include "IO.h"


class GP 
{
	private:
		bool DEBUG_FLAG;
	protected:
		int CurrentLevel;					///< Index of current level
		int TotalLevel;						///< Total Level Number of Multilevel
		// CG Parameters

		int numIter;						///< number of iterations for loop in CG
		int numCG;							///< number of CG which has been called
		
		double **GX_0;						///< x-axis gradient direction g_k-1
		double **GX_1;						///< x-axis gradient direction g_k
		double **GY_0;						///< y-axis gradient direction g_k-1
		double **GY_1;						///< y-axis gradient direction g_k
		double **DX_0;						///< x-axis conjugate direction d_k-1
		double **DX_1;						///< x-axis conjugate direction d_k
		double **DY_0;						///< y-axis conjugate direction d_k-1
		double **DY_1;						///< y-axis conjugate direction d_k
		double **WG_X;						///< x-axis wirelength gradient
		double **WG_Y;						///< y-axis wirelength gradient
		double **DG_X;						///< x-axis density gradient
		double **DG_Y;						///< y-axis density gradient
		double **pure_DX;
		double **pure_DY;
		//top objective
		double Fk_0_top;						///< objective function f_k-1
		double Fk_1_top;						///< objective function f_k
		//bot objective
		double Fk_0_bot;						///< objective function f_k-1
		double Fk_1_bot;						///< objective function f_k
		double Gamma;						///< gamma in LSE wirelength model
		double *Cv;							///< normalization factor
		double **Xp;							///< postive x-term in LSE wirelength model
		double **Xn;							///< negtive x-term in LSE wirelength model
		double **Yp;							///< postive y-term in LSE wirelength model
		double **Yn;							///< negtive y-term in LSE wirelength model
		double lambda_top;						///< lambda in objective function(top)
		double lambda_bot;						///< lambda in objective function(top)
		double lambdaW;						///< Wirelength Weight
		double **alpha_x;						///< step size
		double **alpha_y;						///< step size
		
		double overflow_area_top_0;				///< sum of max(Db(x,y)-Mb) in each bin (former)
		double overflow_area_top_1;				///< sum of max(Db(x,y)-Mb) in each bin (current)

		double overflow_area_bot_0;				///< sum of max(Db(x,y)-Mb) in each bin (former)
		double overflow_area_bot_1;				///< sum of max(Db(x,y)-Mb) in each bin (current)
		
		// Bin Parameter

		int nBinRow;						///< number of row for bins
		int nBinCol;						///< number of col for bins
		double binW;							///< bin width
		double binH;							///< bin height


		class BIN
		{
			public:
				int idBin;					///< bin id
				double x;						///< left-bottom x-coor
				double y;						///< left-bottom y-coor
				double w;						///< width
				double h;						///< height
				int nMod;					///< number of modules in the bin
				int nPre;					///< number of preplaced modules in the bin
				double OccupyValue;			///< area occupied by modules in the bin
				double EnableValue;			///< allowable area in the bin
				double EscapeDist  ; 		///< The minimum distance from preplaced bin to free bin
				pair <int,int> FreeBin ;	///< The minimum distance free bin

				vector<int> pres;			///< preplaced modules in the bin
				bool flagHasPre;			///< flag to determine whether the bin has preplaced modules
				bool AddPotential; 			///<flag to say the bin is already add base potential
				bool FlagCold;      		///<flag to say the bin is cold


				BIN(){
					idBin = x = y = w = h = nMod = nPre = OccupyValue = EnableValue = EscapeDist = 0;
					flagHasPre = FlagCold = false ;
				};
				BIN& operator=(const BIN &bin){
					if(&bin != this)
					{
						idBin = bin.idBin;
						x = bin.x;
						y = bin.y;
						w = bin.w;
						h = bin.h;
						nMod = bin.nMod;
						nPre = bin.nPre;
						OccupyValue = bin.OccupyValue;
						EnableValue = bin.EnableValue;
						flagHasPre = bin.flagHasPre;
						EscapeDist = bin.EscapeDist;
						FreeBin = bin.FreeBin;
						FlagCold = bin.FlagCold;
						pres.reserve(nPre);
						for(int i = 0; i < nPre; i++)
						{
							pres[i] = bin.pres[i];
						}
					}
					return *this;
				};
				~BIN(){
				};
				
			// Bin Map

		};

		typedef vector < vector<BIN> > BIN2D;
		typedef map<int, vector<vector<BIN> > > BIN3D;
		BIN3D bin3D;  						///< Bin Map Structure 
		BIN3D potential3D;
		BIN3D smooth_potential3D;
		BIN3D level_smoothing_potential3D;
		/// \brief Class QP (for quadratic programming)
		/// rows for matrix
		class QP
		{

			public:
				class WEIGHT
				{
					public:
						float weight;
						int col;

						WEIGHT(){
							weight = col = 0;
						};
						~WEIGHT(){

						};
				};

				vector<WEIGHT> connect;

				QP() {};
				~QP() {};
		};

	public:
		inline void SetCurrentLevel(int value){
				this->CurrentLevel = value;
		};
		inline void SetTotalLevel(int value){
				this->TotalLevel= value;
		};
		void CreateExpTable();
		void CalculateHPWL_GP(NETLIST & tmpnt);
		void SetModToCenter(NETLIST & tmpnt);
		void RandomPosition(NETLIST & tmpnt);
		void CreateQmBv( vector<QP>& matrixQ, float* vectorB, NETLIST& tmpnt, int modeXY, map<int, int>& mapNet, map<string, int>& mapMod );
		void SolveQP( NETLIST& tmpnt );
		GP(){
		};
		~GP(){

		};

};

#endif



/*!
 * \file 	ThermalGlobal.h
 * \brief 	Thermal-Aware Global Placement [(header)(WL+Area+Thermal)]
 * 
 * \author 	Tai-Ting Chen
 * \version	2.0 (2D/3D)
 * \date	2018.07.
 */

/**
 * ===================================================================
 *	Input: 
 * 		- NETLIST nt:			Given NETLIST Info (Need Level index) 
 *		- Option:				There are some parameters you can adjust
 			- DefineSuperBigPara	///<  Define the Modules needed to be fixed
			- TFM					///<  Turn On/Off Thermal Force Modulation
			- rho					///<  Define the basis Allowable Ratio 
			- PLOT					///<  Plot the Fp in each iteration
 *  Output:
 *		- NETLIST nt: 			Contain New location of modules	
 * ===================================================================
 */
#ifndef _THERMALGLOBAL_H_
#define _THERMALGLOBAL_H_

#include "dataType.h"
#include "gp.h"
#include "IO.h"
#include <fftw3.h>

class THERMALGP : public GP
{

	private:
		bool FlagPlot;						///< Bool to Plot Fp Layout
		clock_t start_t, end_t;				///< GP Time
		bool TFM;							///< Thermal Force Modulation On or Off;
		bool Change;						///< Flag to switch Area mode
		
		int CountFactor;					///< Counter
		
		int Max_EscpaeDist ;				///< The maximum Distance in EscapeMap
		
		double DefineSuperBigPara ;			///< The Parameter to Set the Fixed Module (Area)

		double EnhanceRatio;				///< The enhance ratio of rho
		bool StopEnhance;					///< Flag to Stop enhance Allowable Ratio /rho
		double rho;							///< The initial value of allowable temperature(rho)
		string FpPlotPath ;					///< The Path to Output Layout
		string BinMapPath ;					///< The Path to Output Bin Value Map(e.g. Thermal map)
	
	protected:
		double *TG_X;						///< x-axis thermal density gradient 2017
		double *TG_Y;                       ///< y-axis thermal density gradient 2017

		double lambdaT;						 ///< lambdaT in thermal objective function

		double **mask_table;                 ///< thermal mask acceleration 2017
		double **mask_table_2;               ///< thermal mask acceleration 2017
		double **correction_table;           ///< blurring method correction 2017
		int ThermalRow;						 ///< number of row for thermal bins
		int ThermalCol;						 ///< number of col for thermal bins
		int binW_T;							 ///< thermal bin width
		int binH_T;							 ///< thermal bin height
		
		double thermal_difference_0;         ///< thermal difference former 2017
		double thermal_difference_1;         ///< thermal difference current 2017
		double Thermal_overflow_bound; 		 ///< upper bound of overflow ratio

		set <int> SuperBigID ;				 ///< The SuperBig Modules (Need Fixed) 
		map <int, vector < vector <double> > > FirstEscapeBin; 			///< The Free Bin (Non-Covered Bin) 
		
		vector <vector <double> > GaussianMask;
		vector <vector <double> > ThermalMask;

	public:
		void ThermalGlobal(NETLIST & tmpnt);				///< Global Fcn.
		
		// Adjust Module
		void ClassifyMacro(NETLIST & tmpnt);				///< Classify Module into Hot, Warm, Cold. 
		void ShrinkMod(NETLIST &tmpnt);						///< Shrink Module with ShrinkRatio
		void InflatMod(NETLIST &tmpnt);						///< Inflate Moudle to OG Size
		// Bin Construction
		vector<vector <pair <int,int> > >IdentifyFreeBin(NETLIST&tmpnt, double AllowableT);	///< Find the Cold Bin
		void CutBin(NETLIST & tmpnt );						///< Bulid Bin Map

		// Gradient
		void InitializeGradient( NETLIST& tmpnt);			///< Gradient Fcn.
		double CalWireGradient( NETLIST& tmpnt );
		double CalDensityGradient( NETLIST& tmpnt );	
		double CalThermalGradient( NETLIST& tmpnt );
		void MirrorMethod(vector < vector <double> > &Matrix );
		void SetTargetT(int CountInner, double max_power, vector<vector < pair<int, int > > > &FreeBin ,int LayerIndex  );


		void ConjugateGradient( NETLIST& tmpnt );
		// Evaluate Overflow
		void CalOverflowArea( NETLIST& tmpnt );
		// Table Fcn.
		void BuildGaussianMask();							/// Build Gaussain Map
		void BuildThermalMask();
		void CreateThermalTable( NETLIST& tmpnt );

		void PlotThermalMap( vector <vector <double> >& NewMb, NETLIST & tmpnt);
		void PlotBinMap(NETLIST & tmpnt, string Filename, string Type);
		static bool SortByPower(pair<int,double> a, pair <int,double> b){
			return a.second > b.second;
		}

		inline void SetSuperBig(double value){
			DefineSuperBigPara = value;
		};
		inline void Setrho(double value){
			rho = value;
		};
		inline void SetTFM(bool OnOff)
		{
			TFM = OnOff;
		};
		inline void SetFlagPlot(bool Onoff){
			FlagPlot = Onoff;
		};
		inline void SetFpPlotPath (string Name){
			FpPlotPath = Name;
		};
		inline void SetBinMapPath (string Name){
			BinMapPath = Name;
		};
		THERMALGP()
		{
			// Default Value
			this->rho 					= 1 ;
			this->DefineSuperBigPara	= 5;
			this->TFM					= true;
			this->FlagPlot				= false;
			this->FpPlotPath			= "./out/MAP/ThermalMap/";
			this->BinMapPath			= "./out/MAP/PotentialMap/";
		};
		~THERMALGP()
		{};

};
class FFTW
{

	public:

		static void Convolution(vector <vector <double> > & Map, vector <vector <double> > & Mask, vector <vector <double> > &Results);

		static void compute_fft_fftw3(double * src, int height, int width, double * dst)
		{
			// Create the required objects for FFTW
			fftw_complex *in_src, *out_src;
			fftw_plan p_forw;

			in_src = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height * width);
			out_src = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height * width);
			// Create the plan for performing the forward FFT
			p_forw = fftw_plan_dft_2d(height, width, in_src, out_src, FFTW_FORWARD, FFTW_ESTIMATE);

			// Fill in the real part of the matrix with the image
			for(int i = 0 ; i < height ; ++i)
			{
				for(int j = 0 ; j < width ; ++j)
				{
					in_src[i * width + j][0] = src[i*width + j];
					in_src[i * width + j][1] = 0.0;
				}
			}

			// Compute the forward fft
			fftw_execute(p_forw);

			// Now we just need to copy the right part of out_src into dst
			for(int i  = 0 ; i < height ; ++i)
			{
				for(int j = 0 ; j < width ; ++j)
				{
					dst[i*2*width + 2*j]= out_src[i * width + j][0];
					dst[i*2*width + 2*j + 1]= out_src[i * width + j][1];
				}
			}

			fftw_destroy_plan(p_forw);
			fftw_free(in_src);
			fftw_free(out_src);
		};
		static void compute_ifft_fftw3(double * src, int height, int width, double * dst)
		{
			// Create the required objects for FFTW
			fftw_complex *in_src, *out_src;
			fftw_plan p_back;

			in_src = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height * width);
			out_src = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * height * width);
			// Create the plan for performing the forward FFT
			p_back = fftw_plan_dft_2d(height, width, in_src, out_src, FFTW_BACKWARD, FFTW_ESTIMATE);

			// Fill in the real part of the matrix with the image

			for(int i = 0 ; i < height ; ++i)
			{
				for(int j = 0 ; j < width ; ++j)
				{
					in_src[i * width + j][0] = src[i*2*width + 2*j];
					in_src[i * width + j][1] = src[i*2*width+ 2*j+1];
				}
			}

			// Compute the forward fft
			fftw_execute(p_back);

			// Now we just need to copy the right part of out_src into dst
			for(int i  = 0 ; i < height ; ++i)
			{
				for(int j = 0 ; j < width ; ++j)
				{
					dst[i*2*width + 2*j]= out_src[i * width + j][0];
					dst[i*2*width + 2*j + 1]= out_src[i * width + j][1];
				}
			}

			fftw_destroy_plan(p_back);
			fftw_free(in_src);
			fftw_free(out_src);
		};

		static void product(double* image, double* kernel,int h,int w,double* output)
		{
			int i,j;
			for(i=0; i< h; ++ i)
			{
				for(j=0; j<w ; ++j)
				{
					int real= i*2*w+ 2*j;
					int cmplex= i*2*w+2*j +1 ;
					output[real ]=  image[real] * kernel[real] - image[cmplex] * kernel[cmplex];
					output[cmplex] = image [real] * kernel[cmplex] + image[cmplex]* kernel[real];
				}

			}

		};

		static double compute(double a,double b)
		{
			double value;

			value=sqrt(a*a+b*b);
			return value;
		};

};

#endif


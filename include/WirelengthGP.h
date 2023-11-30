/*!
 * \file 	gp.h
 * \brief 	Wirelength-Driven Global Placement [(header)(WL+Area)]
 *
 * \author 	Tai-Ting
 * \version	2.0 (3-D)
 * \date	2018.07/
 */

#ifndef _WIRELENGTHGP_H_
#define _WIRELENGTHGP_H_



#include "dataType.h"
#include "IO.h"
#include "gp.h"


class WLGP : public GP
{
	private:
		clock_t start_t, end_t;
		bool FlagPlot;
		string FpPlotPath ;
	public:
		void GlobalStage(NETLIST &tmpnt);
		void GlobalStage_last_stage(NETLIST & tmpnt);
		void plot_med( NETLIST tmpnt, string filename );
		void CutBin(NETLIST & tmpnt );
		void InitializeGradient( NETLIST& tmpnt,bool & start_monitor_nan);
		double* CalWireGradient( NETLIST& tmpnt );
		void check_every_gradient(NETLIST tmpnt);
		double* CalDensityGradient( NETLIST& tmpnt ,bool & start_monitor_nan);
		void CalOverflowArea( NETLIST& tmpnt );
		void plot_preplaced_info(string filename,NETLIST& tmpnt);
		void ConjugateGradient( NETLIST& tmpnt, bool &star_bad_sol,bool & start_monitor_nan,int countt );
		void ConjugateGradient_last_stage( NETLIST& tmpnt, bool &star_bad_sol,bool & start_monitor_nan,int countt ,double start_time,bool& halt);
		void calpuredensgradient(NETLIST& tmpnt);
		inline void SetFlagPlot(bool value){
			FlagPlot = value;
		}
		inline void SetFpPlotPath(string Name){
			FpPlotPath = Name;
		}

		WLGP(){
			FlagPlot	= false;
			FpPlotPath	= "./out/MAP/ThermalMap/";
		};
		~WLGP(){

		};
};

#endif


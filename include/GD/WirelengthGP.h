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
		void CutBin(NETLIST & tmpnt );
		void InitializeGradient( NETLIST& tmpnt);
		double CalWireGradient( NETLIST& tmpnt );
		double CalDensityGradient( NETLIST& tmpnt );
		void CalOverflowArea( NETLIST& tmpnt );
		void ConjugateGradient( NETLIST& tmpnt );
		
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


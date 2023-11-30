
/*!
 * \file 	Legalization.h
 * \brief 	ILP based Leglizer (header)
 *
 * \author 	m102 Jw H
 * \version	2.0 (3-D)
 * \date	2018.07/
 */

#ifndef _LEGALIZATION_H_
#define _LEGALIZATION_H_

#include "dataType.h"
#include "IO.h"
#include "DelaunayTriangulation.h"
#include "solveCPLEX.h"
#include "legalTSV.h"
class ILPLG
{



	public:
		
		int	LegalizationC(NETLIST & nt);

		double calculate_HPWL( NETLIST& tmpnt );
		double calculate_movement(NETLIST& nt);
		int CheckAllRule(NETLIST &nt);
		bool checkPreplace(NETLIST &nt);
		bool checkOutline(NETLIST &nt);
		bool checkArea(NETLIST &nt);
		bool checkOverlap(NETLIST &nt);
		bool checkRatio(NETLIST &nt);
		string realtime();
};


#endif


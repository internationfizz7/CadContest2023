#ifndef _QP_ORDERING_
#define _QP_ORDERING_

#include "structure.h"
#include "macro_legalizer.h"

 /* laspack */
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
}

class NON_ZERO_ENTRY
{
public:
	unsigned int col;
	double weight;

	NON_ZERO_ENTRY();
	~NON_ZERO_ENTRY();
};

class QP
{
public:
	vector< NON_ZERO_ENTRY > connect;

	QP();
	~QP();
};

class NET_QP
{
public:
	unsigned int id;
	int degree;
	vector<unsigned int> modules;
	NET_QP();
	~NET_QP();
};

class MODULE_QP
{
public:
	bool is_fixed;
	unsigned int id;
	int macro_id;   //MovableMacro_ID
	int x, y;
	int llx, lly, urx, ury;
	set< unsigned int > nets;    //index of net_list
	vector< unsigned int > QPnet;   //index for QP NET
	Macro* macro;
	MODULE_QP();
	~MODULE_QP();
};

// class NETLIST
class NETLIST_QP
{
public:
	vector< MODULE_QP* > modules;
	vector< NET_QP* > nets;

	NETLIST_QP();
	~NETLIST_QP();
};
void UpdateNetlistQP(NETLIST_QP &nt, Legalization_INFO& LG_INFO);
void SolveQP(NETLIST_QP &nt, Legalization_INFO& LG_INFO);
void CreateQmBv(NETLIST_QP &nt, vector< QP > &Q_matrix, vector< double > &B_vector,map<unsigned int, unsigned int> &module_map, map<unsigned int, unsigned int> &net_map, int modeXY);
void MacroRefineOrder(NETLIST_QP &nt, vector<Macro*> &MacroOrder);

#endif

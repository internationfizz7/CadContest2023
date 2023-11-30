#ifndef CELL_PLACEMENT_H_INCLUDED
#define CELL_PLACEMENT_H_INCLUDED

#include "deflef_parser.h"
#include "macro_legalizer.h"
#include <math.h>
#include <pthread.h>
/* networkflow*/
#include "lemon/smart_graph.h"
#include "lemon/network_simplex.h"
#include "lemon/lgf_writer.h"
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

class Legalization_INFO;

class CELLPLACEMENT
{
protected:
    // Class BIN (for placement cell cluster
    class BIN
	{
    public:
        int llx, lly, urx, ury;     // bin's llx lly ux ury
        int centX, centY;           // placed cell cluster at center
        vector<int> packing_std;    // std Group ID (used for "STDGroupBy[]")
        bool isPreplaced;           // BIN has preplaced do not place cell for  "LegailCellCluster()"

        BIN()
        {
            isPreplaced = false;
            llx = lly = urx = ury = 0;
            centX = centY = 0;
        };
	};

    // Class REGION (for placement cell cluster
	class REGION
    {
    public:
        int P_RegionID;                             // Regions[i] = P_Region[i]
        int llx, lly, urx, ury;                     // region's llx lly ux ury
        vector<int> big_STDGroupID;                 // bigger STD Gropups ID is legail to region by sorting
        vector<int> small_STDGroupID;               // smaller STD Gropups ID is legail to near region directly

        float preplacedArea;
        vector<Boundary> preplace;                  // preplaced in the region
        vector<int> macroID;                        // macros is overlap by the region //[NOTE] the macro amount is bigger then the benchmark macro
        int binsize;
        int w, h;                                    // bin's weight and height
        vector<vector<CELLPLACEMENT::BIN*> > Bins;    // Bins in the region

        REGION()
        {
            preplacedArea = 0;
            binsize = 0;
            P_RegionID = -1;
            llx = lly = urx = ury = 0;
        };
    };
	vector<CELLPLACEMENT::REGION*> Regions;         // Region is devide by rescive partiton

    // Class WEIGHT (for quadratic programming QP)  // elements for matrix
    class WEIGHT
    {
    public:
        float weight;
        int col;
        WEIGHT(){};
        ~WEIGHT(){};
    };
    // Class QP (for quadratic programming)        // rows for matrix
    class QP
    {
    public:
        vector<WEIGHT> connect;
        QP() {};
        ~QP() {};
    };


public:
    friend class MACROEXPAND;

    static void CellPlacementFlow(CELLPLACEMENT *cellpl, Legalization_INFO &LG_INFO_tmp, int iter);

    // Cluster cells
    static void CellClustering(CELLPLACEMENT *cellpl);

    // Allocate cell cluster to a bin
	static void PartitionCellCluster(CELLPLACEMENT *cellpl, vector<Partition_Region> &P_Region_real);

	// QP spare cell cluster inthe region
	static void SpareCluster(CELLPLACEMENT *cellpl);
	static void* QPSpreadCluster(void* vregion);
    static void CreateQmBv2( vector<CELLPLACEMENT::QP> &matrixQ, float *vectorB,CELLPLACEMENT::REGION* region, int modeXY, map<int, int> &mapNet, map<int, int> &mapMod );

    // Legal cell cluster to the bin
    static void LegailCellCluster(CELLPLACEMENT *cellpl);

    // for solve the Networkflow
    static void SolveNetworkFlow(CELLPLACEMENT::REGION *this_region);
    static void Create_Node_Arc(lemon::SmartDigraph &g,
                         lemon::SmartDigraph::Node &src,
                         lemon::SmartDigraph::Node &snk,
                         vector<lemon::SmartDigraph::Node> &s,
                         vector<lemon::SmartDigraph::Node> &t,
                         vector<lemon::SmartDigraph::Arc> &arcs,
                         int ss, int ts);
    static void Set_Cap_Cost(lemon::SmartDigraph::Node &src,
                      lemon::SmartDigraph::Node &snk,
                      vector<lemon::SmartDigraph::Node> &s,
                      vector<lemon::SmartDigraph::Node> &t,
                      vector<lemon::SmartDigraph::Arc> &arcs,
                      lemon::SmartDigraph::ArcMap<int> &cost,
                      lemon::SmartDigraph::ArcMap<int> &lcap,
                      lemon::SmartDigraph::ArcMap<int> &ucap,
                      lemon::SmartDigraph::NodeMap<int> &supl,
                      CELLPLACEMENT::REGION *this_region,
                      vector<CELLPLACEMENT::BIN*> &EmptyBins);
    static void Check_NS(lemon::SmartDigraph &g,
                  lemon::SmartDigraph::ArcMap<int> &cost,
                  lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
                  vector<lemon::SmartDigraph::Arc> &arcs,
                  int ss, int ts,
                  CELLPLACEMENT::REGION *this_region,
                  vector<CELLPLACEMENT::BIN*> &EmptyBins);
    static void Writer(lemon::SmartDigraph &g,
                lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
                lemon::SmartDigraph::ArcMap<int> &cost,
                lemon::SmartDigraph::ArcMap<int> &lcap,
                lemon::SmartDigraph::ArcMap<int> &ucap,
                lemon::SmartDigraph::NodeMap<int> &supl,
                lemon::SmartDigraph::Node src,
                lemon::SmartDigraph::Node snk);
    static void PlotCellCluster(CELLPLACEMENT *cellpl, int iter);


    // QP spare the cells of the bin's cluster
    static void SpareCell(CELLPLACEMENT *cellpl);
    static void* QPSpreadCell(void* vbin);
    static void CreateQmBv( vector<CELLPLACEMENT::QP> &matrixQ, float *vectorB, CELLPLACEMENT::BIN* bin, int modeXY, map<int, int> &mapNet, map<int, int> &mapMod );

    // Packing cells in the bin
    static void PackingCell(CELLPLACEMENT *cellpl);
    static void* OrderlyPackingCell(void* vbin);
    static void quickSort(vector<int> &, int , int, bool);

};

extern vector<STD_Group*> STDGroupBy2;

#endif

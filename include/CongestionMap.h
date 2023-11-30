#ifndef CONGESTOINMAP_H_INCLUDED
#define CONGESTOINMAP_H_INCLUDED

#include "deflef_parser.h"
#include "corner_stitching.h"
//#include "macro_legalizer.h"

extern "C"
{
    #include "flute.h"
}


class CONGESTIONMAP
{
public:
	class EDGE
	{
	public:

		int llx, lly, urx, ury;
		double overflow;
		double demand, capacity;    // must set double, or the calculate will be wrong
		float penalty;
		int historical_cost;
		vector<bool> LayerHasRect;   // LayerHasRect[x] = true is layer x has obstacle
		EDGE()
		{
		    llx = lly = urx = ury = 0;
			capacity = demand = 0;
			penalty = 0;
			historical_cost = 0;
		}
		//vector<pair<int, double> > cells_Npass;
	};
    class TWO_PIN_NET
    {
    public:
        int x1, y1;
        int x2, y2;
        int wire_length;

        //pair<int, int> two_cells;

        TWO_PIN_NET()
        {
            x1=-1;
            y1=-1;
            x2=-1;
            y2=-1;
            wire_length=0;
        };
    };
    int row, col;                                       // number of row/col
	vector<vector<CONGESTIONMAP::EDGE *> > V_Edge;      // global routing vertical edge
	vector<vector<CONGESTIONMAP::EDGE *> > H_Edge;      // global routing horizontal edge
    vector<CONGESTIONMAP::TWO_PIN_NET *> ary_2pinnet;   // for flute()

    int layer_num;                                      // layer number
    int Vedge_track, Hedge_track, H_layer_num, V_layer_num;

	CONGESTIONMAP(){
		row=0;
		col=0;
		V_Edge.clear();
		H_Edge.clear();
		ary_2pinnet.clear();
		layer_num=0;
		Vedge_track=0;
		Hedge_track=0;
		H_layer_num=0;
		V_layer_num=0;
		EDGE();
		TWO_PIN_NET();
	};
	~CONGESTIONMAP(){};

	

public:
	//friend class POWERPLANTRACK;
	friend class REFINEMENT;
	friend class WSA;
	friend class MACROEXPAND;

    static float CongestionMapFlow( CONGESTIONMAP *cong, bool sloveGP, int iter);
    static void GRBinInfo(CONGESTIONMAP *cong);
    static void PlotMacroObs(CONGESTIONMAP *cong, bool SolveGP);
    static void AddObsDemand(CONGESTIONMAP *cong, bool SolveGP, int iter);

    static void Flute2PinNet(CONGESTIONMAP *cong_info, bool sloveGP);
    static void AddGRBINDemand(CONGESTIONMAP *cong);
	static void Displaymatlab(CONGESTIONMAP *cong, bool SolveGP, int iter);
	static float Calcost(CONGESTIONMAP *cong);

	/// ADD 2021.03.05 ///
	static void ForDebugPlotDPX(CONGESTIONMAP *cong);
	static void InitializeCongestionMap(CONGESTIONMAP *cong);
	void CalculateCapacity(CONGESTIONMAP *cong);
	static float Enumerate_H_Edge(CONGESTIONMAP *cong, Boundary b, bool historical);    // return congestion cost
	static float Enumerate_V_Edge(CONGESTIONMAP *cong, Boundary b, bool historical);    // return congestion cost
};

float Cal_H_congestion(CONGESTIONMAP *cong, Boundary b);
float Cal_V_congestion(CONGESTIONMAP *cong, Boundary b);
void clear_para_congmap();
extern CONGESTIONMAP CG_INFO; /// ADD 2021.03.17
#endif


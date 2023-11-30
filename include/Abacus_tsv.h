#include "dataType.h"

using namespace std;

class ABTSV
{
public:
    ABTSV(){};
    ~ABTSV(){};
    ABTSV(NETLIST &);

    int readNT(NETLIST &);
	int writeNT(NETLIST &);
    
    
	int runAbacus();
    struct REGION
    {
        int llx;
        int lly;
        int urx;
        int ury;
        REGION(int a=0,int b=0, int c=0, int d=0)
        {
            llx =a;
            lly =b;
            urx =c;
            ury =d;
        }
    };
    class Cell
    {
    public:
        string name = "";
        string type = "";
        double llx;
        double lly;
        double width;
        double height;
        int index;
		int index_nt;
        int index_new;
        REGION box;//boundry box
        //vector<Net *> CN; // CN stands for connect net
        Cell() {}
        ~Cell() {}
        //friend class Net;
        // friend class Shape;
    };

	class Cluster
    {
    public:
        vector<int> cellorder;
        int x;
        double ec;
        double qc;
        double wc;
        Cluster() : ec(0), qc(0), wc(0) {}
        Cluster(int E, int Q, int W) : ec(E), qc(Q), wc(W) {}
        ~Cluster() {}
    };

	class Subrow
    {
    public:
        int Xmin;
        int Xmax;
        int w;
        vector<Cluster> C;
        Subrow() {}
        Subrow(int x1, int x2) : Xmin(x1), Xmax(x2), w(x2 - x1) {}
        ~Subrow() {}
        // friend class Cell;
    };
    class Row
    {
    public:
        vector<int> termail; //the x-position macro stand in this row
        vector<Subrow> subrow;
        int Coordinate;
        int Height;
        int Sitespacing;
        int SubrowOrigin;
        int NumSites;
        // int index ;
        // friend class Subrow;
        Row() {}
        ~Row() {}
    };
    static bool CompareCellX(Cell C1, Cell C2);
	static bool CompareCellYX(Cell C1, Cell C2);
	int isOverlap();
    REGION findBox(NETLIST &, int);
	void addcell(Cluster &c, Cell n);
	void addcluster(Cluster &c1, Cluster c2);
	void collapse(Subrow &r);
	void PlaceRow(int &row, Subrow &r, Cell &n, vector<Cell> &cell);
private:
    string benchmark;
    string bench_txt;
    vector<Cell> cells;
    vector<Cell> macros;
    vector<Row> rows;
	map<string, int> mapcell;
	bool layer;
	int NumNodes, NumTerminals;
	int NumRows;
	double Xmax, Ymax, Xmin, Ymin;
	int row_high;

};
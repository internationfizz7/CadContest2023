#ifndef BIPARTITE_MATCHING_H_INCLUDE
#define BIPARTITE_MATCHING_H_INCLUDE
/// Globals.h ///

#define EPSILON 0.000001
#define INFINITO 1000000000.0

/// ======================================================================///
#include <iostream>
#include <list>
#include <vector>
#include <math.h>
#include <fstream>
#include "structure.h"
#include "macro_legalizer.h"

using namespace std;

class Graph;

/// main ///
pair< Graph, vector<double> > CreateRandomGraph();
Graph ReadGraph(string filename);
pair< Graph, vector<double> > ReadWeightedGraph(int n, int m, vector<pair<int, int> > edge, vector<double> edge_cost);
vector<pair<int, int> > MinimumCostPerfectMatching(int n, int m, vector<pair<int, int> > edge, vector<double> edge_cost);
void MaximumMatchingExample(string filename);   // I don't use this function. (by YJ Huang)

/// Graph.h ///
class Graph
{
public:
	//n is the number of vertices
	//edges is a list of pairs representing the edges (default = empty list)
	Graph(int n, const list< pair<int, int> > & edges = list< pair<int, int> >());

	//Default constructor creates an empty graph
	Graph(): n(0), m(0) {};

	//Returns the number of vertices
	int GetNumVertices() const { return n; };
	//Returns the number of edges
	int GetNumEdges() const { return m; };

	//Given the edge's index, returns its endpoints as a pair
	pair<int, int> GetEdge(int e) const;
	//Given the endpoints, returns the index
	int GetEdgeIndex(int u, int v) const;

	//Adds a new vertex to the graph
	void AddVertex();
	//Adds a new edge to the graph
	void AddEdge(int u, int v);

	//Returns the adjacency list of a vertex
	const list<int> & AdjList(int v) const;

	//Returns the graph's adjacency matrix
	const vector< vector<bool> > & AdjMat() const;
private:
	//Number of vertices
	int n;
	//Number of edges
	int m;

	//Adjacency matrix
	vector< vector<bool> > adjMat;

	//Adjacency lists
	vector< list<int> > adjList;

	//Array of edges
	vector< pair<int, int> > edges;

	//Indices of the edges
	vector< vector<int> > edgeIndex;
};
/// ======================================================================///

/// BinaryHeap.h ///
/*
This is a binary heap for pairs of the type (double key, int satellite)
It is assumed that satellites are unique integers
This is the case with graph algorithms, in which satellites are vertex or edge indices
 */
class BinaryHeap
{
public:
	BinaryHeap(): satellite(1), size(0) {};

	//Inserts (key k, satellite s) in the heap
	void Insert(double k, int s);
	//Deletes the element with minimum key and returns its satellite information
	int DeleteMin();
	//Changes the key of the element with satellite s
	void ChangeKey(double k, int s);
	//Removes the element with satellite s
	void Remove(int s);
	//Returns the number of elements in the heap
	int Size();
	//Resets the structure
	void Clear();

private:
	vector<double> key;//Given the satellite, this is its key
	vector<int> pos;//Given the satellite, this is its position in the heap
	vector<int> satellite;//This is the heap!

	//Number of elements in the heap
	int size;
};
/// ======================================================================///

/// Matching.h ///
#define EVEN 2
#define ODD 1
#define UNLABELED 0

class Matching
{
public:
	//Parametric constructor receives a graph instance
	Matching(const Graph & G);

	//Solves the minimum cost perfect matching problem
	//Receives the a vector whose position i has the cost of the edge with index i
	//If the graph doest not have a perfect matching, a const char * exception will be raised
	//Returns a pair
	//the first element of the pair is a list of the indices of the edges in the matching
	//the second is the cost of the matching
	pair< list<int>, double > SolveMinimumCostPerfectMatching(const vector<double> & cost);

	//Solves the maximum cardinality matching problem
	//Returns a list with the indices of the edges in the matching
	list<int> SolveMaximumMatching();

private:
	//Grows an alternating forest
	void Grow();
	//Expands a blossom u
	//If expandBlocked is true, the blossom will be expanded even if it is blocked
	void Expand(int u, bool expandBlocked);
	//Augments the matching using the path from u to v in the alternating forest
	void Augment(int u, int v);
	//Resets the alternating forest
	void Reset();
	//Creates a blossom where the tip is the first common vertex in the paths from u and v in the hungarian forest
	int Blossom(int u, int v);
	void UpdateDualCosts();
	//Resets all data structures
	void Clear();
	void DestroyBlossom(int t);
	//Uses an heuristic algorithm to find the maximum matching of the graph
	void Heuristic();
	//Modifies the costs of the graph so the all edges have positive costs
	void PositiveCosts();
	list<int> RetrieveMatching();

	int GetFreeBlossomIndex();
	void AddFreeBlossomIndex(int i);
	void ClearBlossomIndices();

	//An edge might be blocked due to the dual costs
	bool IsEdgeBlocked(int u, int v);
	bool IsEdgeBlocked(int e);
	//Returns true if u and v are adjacent in G and not blocked
	bool IsAdjacent(int u, int v);

	const Graph & G;

	list<int> free;//List of free blossom indices

	vector<int> outer;//outer[v] gives the index of the outermost blossom that contains v, outer[v] = v if v is not contained in any blossom
	vector< list<int> > deep;//deep[v] is a list of all the original vertices contained inside v, deep[v] = v if v is an original vertex
	vector< list<int> > shallow;//shallow[v] is a list of the vertices immediately contained inside v, shallow[v] is empty is the default
	vector<int> tip;//tip[v] is the tip of blossom v
	vector<bool> active;//true if a blossom is being used

	vector<int> type;//Even, odd, neither (2, 1, 0)
	vector<int> forest;//forest[v] gives the father of v in the alternating forest
	vector<int> root;//root[v] gives the root of v in the alternating forest

	vector<bool> blocked;//A blossom can be blocked due to dual costs, this means that it behaves as if it were an original vertex and cannot be expanded
	vector<double> dual;//dual multipliers associated to the blossoms, if dual[v] > 0, the blossom is blocked and full
	vector<double> slack;//slack associated to each edge, if slack[e] > 0, the edge cannot be used
	vector<int> mate;//mate[v] gives the mate of v

	int m, n;

	bool perfect;

	list<int> forestList;
	vector<int> visited;
};
/// ======================================================================///
bool GREATER(double A, double B);
bool LESS(double A, double B);
bool EQUAL(double A, double B);
bool GREATER_EQUAL(double A, double B);
bool LESS_EQUAL(double A, double B);
vector<pair<int, int> > Bipartite_Matching(int n, int m, vector<pair<int, int> > edge, vector<double> edge_cost);  // n: # of vertices, m: # of edges

/// ========================= above from github source code to solving bipartite matching ========================= ///

/// ========================= below using LEMON NetworkFlow to solving bipartite matching ========================= ///
/* networkflow*/
#include "lemon/smart_graph.h"
#include "lemon/network_simplex.h"
#include "lemon/lgf_writer.h"

// for solve the Networkflow
void LEMON_SolveNetworkFlow(Legalization_INFO &LG_INFO);
void LEMON_Create_Node_Arc(lemon::SmartDigraph &g,
                           lemon::SmartDigraph::Node &src,
                           lemon::SmartDigraph::Node &snk,
                           vector<lemon::SmartDigraph::Node> &s,
                           vector<lemon::SmartDigraph::Node> &t,
                           vector<lemon::SmartDigraph::Arc> &arcs,
                           int ss, int ts);

void LEMON_Set_Cap_Cost(lemon::SmartDigraph::Node &src,
                        lemon::SmartDigraph::Node &snk,
                        vector<lemon::SmartDigraph::Node> &s,
                        vector<lemon::SmartDigraph::Node> &t,
                        vector<lemon::SmartDigraph::Arc> &arcs,
                        lemon::SmartDigraph::ArcMap<int> &cost,
                        lemon::SmartDigraph::ArcMap<int> &lcap,
                        lemon::SmartDigraph::ArcMap<int> &ucap,
                        lemon::SmartDigraph::NodeMap<int> &supl,
                        vector<int> &macro_id,
                        vector<Macro*> &MacroClusterSet);

void LEMON_Check_NS(lemon::SmartDigraph &g,
                    lemon::SmartDigraph::ArcMap<int> &cost,
                    lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
                    vector<lemon::SmartDigraph::Arc> &arcs,
                    int ss, int ts,
                    vector<int> &macro_id,
                    vector<Macro*> &MacroClusterSet);

void LEMON_Writer(lemon::SmartDigraph &g,
                  lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
                  lemon::SmartDigraph::ArcMap<int> &cost,
                  lemon::SmartDigraph::ArcMap<int> &lcap,
                  lemon::SmartDigraph::ArcMap<int> &ucap,
                  lemon::SmartDigraph::NodeMap<int> &supl,
                  lemon::SmartDigraph::Node src,
                  lemon::SmartDigraph::Node snk);

#endif

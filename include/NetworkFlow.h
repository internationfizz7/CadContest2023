#ifndef NETWORKFLOW_H_INCLUDED
#define NETWORKFLOW_H_INCLUDED

#include "lemon/smart_graph.h"
#include "lemon/network_simplex.h"
#include "lemon/lgf_writer.h"

using namespace std;
//using namespace lemon;

/* Create the data in graph

   @param[in] g : the base total data in graph, add nodes,arcs...
   @param[in] src : source node
   @param[in] snk : sink node
   @param[in] s : the container of node in source side
   @param[in] t : the container of node in target side
   @param[in] arcs : the container od arcs
   @param[in] ss : set count of source side nodes
   @param[in] ts : set count of target side nodes
*/
void Create_Node_Arc(lemon::SmartDigraph &g,
                     lemon::SmartDigraph::Node &src,
                     lemon::SmartDigraph::Node &snk,
                     vector<lemon::SmartDigraph::Node> &s,
                     vector<lemon::SmartDigraph::Node> &t,
                     vector<lemon::SmartDigraph::Arc> &arcs,
					 int ss, int ts);

/* Set the data in graph

   @param[in] src,snk,s,t,arcs : like above, the data in graph
   @param[in] cost : save the cost of arcs
   @param[in] lcap,ucap : save the capacity of arcs
   @param[in] supl : save the supply of nodes
*/
void Set_Cap_Cost(lemon::SmartDigraph::Node &src,
                  lemon::SmartDigraph::Node &snk,
                  vector<lemon::SmartDigraph::Node> &s,
                  vector<lemon::SmartDigraph::Node> &t,
                  vector<lemon::SmartDigraph::Arc> &arcs,
                  lemon::SmartDigraph::ArcMap<int> &cost,
                  lemon::SmartDigraph::ArcMap<int> &lcap,
                  lemon::SmartDigraph::ArcMap<int> &ucap,
                  lemon::SmartDigraph::NodeMap<int> &supl);

/* Check the data you set is feasible & show

   @param[in] g,cost,arcs : the data of graph
   @param[in] ns : the container of calculated data using network simplex algoirthm
*/
void Check_NS(lemon::SmartDigraph &g,
              lemon::SmartDigraph::ArcMap<int> &cost,
              lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
              vector<lemon::SmartDigraph::Arc> &arcs,
			  int ss, int ts);

/* Show the all data of graph

*/
void Writer(lemon::SmartDigraph &g,
            lemon::NetworkSimplex<lemon::SmartDigraph, int> &ns,
            lemon::SmartDigraph::ArcMap<int> &cost,
            lemon::SmartDigraph::ArcMap<int> &lcap,
            lemon::SmartDigraph::ArcMap<int> &ucap,
            lemon::SmartDigraph::NodeMap<int> &supl,
			lemon::SmartDigraph::Node src,
			lemon::SmartDigraph::Node snk);
	

void SolveNetworkFlow();

#endif

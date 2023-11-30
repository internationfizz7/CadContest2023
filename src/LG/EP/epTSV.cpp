
#include "EP.h"

#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>
using namespace lemon;



void EPLG::TSV::FindAdditionalSlots( NETLIST &pm_tmpnt, vector<NODE_SLOT> &sub_nodeSLOT )
{
	int num_x = (int)floor( pm_tmpnt.ChipWidth/(float)(TSV_SIZE * AMP_PARA) );
	int num_y = (int)floor( pm_tmpnt.ChipHeight/(float)(TSV_SIZE * AMP_PARA) );

	vector< vector<bool> > is_accupied( num_x, vector<bool>(num_y, false) );

	for( int i = 0; i < pm_tmpnt.nMod; i++ )
	{
		int src_x = (int)floor( pm_tmpnt.mods[i].LeftX/(float)(TSV_SIZE * AMP_PARA) );
		int src_y = (int)floor( pm_tmpnt.mods[i].LeftY/(float)(TSV_SIZE * AMP_PARA) );
		int trg_x = (int)ceil( (pm_tmpnt.mods[i].LeftX + pm_tmpnt.mods[i].modW)/(float)(TSV_SIZE * AMP_PARA) );
		int trg_y = (int)ceil( (pm_tmpnt.mods[i].LeftY + pm_tmpnt.mods[i].modH)/(float)(TSV_SIZE * AMP_PARA) );

		if( src_x < 0 ) src_x = 0;
		if( src_y < 0 ) src_y = 0;
		if( trg_x > num_x ) trg_x = num_x;
		if( trg_y > num_y ) trg_y = num_y;

		for( int j = src_x; j < trg_x; j++ )
		{
			for( int k = src_y; k < trg_y; k++ )
			{
				is_accupied[j][k] = true;
			}
		}
	}

	int initial_x = (int)(TSV_SIZE * AMP_PARA * 0.5);
	int initial_y = (int)(TSV_SIZE * AMP_PARA * 0.5);

	vector< vector<bool> > is_visited( num_x, vector<bool>(num_y, false) );

	int move_x = (int)floor(0.5 * num_x);
	int move_y = (int)floor(0.5 * num_y);

	char direct = 'D';

	while( 1 )
	{
		if( !is_accupied[move_x][move_y] && !is_visited[move_x][move_y] )
		{
			NODE_SLOT slot;
			slot.id = (int)sub_nodeSLOT.size();
			slot.x = initial_x + (move_x * TSV_SIZE * AMP_PARA);
			slot.y = initial_y + (move_y * TSV_SIZE * AMP_PARA);
			sub_nodeSLOT.push_back( slot );
		}

		is_visited[move_x][move_y] = true;

		if( direct == 'R' )
		{
			if( (move_x + 1) >= num_x ) break;

			if( is_visited[move_x+1][move_y] )
			{
				move_y++;
				direct = 'R';
			}
			else
			{
				move_x++;
				direct = 'D';
			}
		}
		else if( direct == 'D' )
		{
			if( (move_y - 1) < 0 ) break;

			if( is_visited[move_x][move_y-1] )
			{
				move_x++;
				direct = 'D';
			}
			else
			{
				move_y--;
				direct = 'L';
			}
		}
		else if( direct == 'L' )
		{
			if( (move_x - 1) < 0 ) break;

			if( is_visited[move_x-1][move_y] )
			{
				move_y--;
				direct = 'L';
			}
			else
			{
				move_x--;
				direct = 'U';
			}
		}
		else if( direct == 'U' )
		{
			if( (move_y + 1) >= num_y ) break;

			if( is_visited[move_x][move_y+1] )
			{
				move_x--;
				direct = 'U';
			}
			else
			{
				move_y++;
				direct = 'R';
			}
		}
		else
		{
			cout << "error  : unrecognized direction: " << direct << endl;
			exit(EXIT_FAILURE);
		}

		if( move_x < 0 || move_y < 0 || move_x >= num_x || move_y >= num_x ) break;
	}

	for( int i = 0; i < num_x; i++ )
	{
		is_accupied[i].clear();
		vector<bool> (is_accupied[i]).swap(is_accupied[i]);

		is_visited[i].clear();
		vector<bool> (is_visited[i]).swap(is_visited[i]);
	}

	is_accupied.clear();
	vector< vector<bool> > (is_accupied).swap(is_accupied);
	vector< vector<bool> > (is_visited).swap(is_visited);
}

bool EPLG::TSV::FindNetBoundingBox( int corr_id, int net_id, NBBOX &nbbox, NETLIST &tmpnt )
{
	int countTSV = 0;

	for( int j = tmpnt.nets[net_id].head; j < tmpnt.nets[net_id].head + tmpnt.nets[net_id].nPin; j++ )
	{
		if( (tmpnt.pins[j].flagTSV) && 
			(tmpnt.pins[j].index == corr_id || tmpnt.pins[j].pinL > tmpnt.mods[corr_id].modL) )
		{
			countTSV++;
			continue;
		}

		if( tmpnt.pins[j].index < tmpnt.nMod )
		{
			int mod_id = tmpnt.pins[j].index;
			if( tmpnt.mods[mod_id].CenterX < nbbox.llx ) nbbox.llx = tmpnt.mods[mod_id].CenterX;
			if( tmpnt.mods[mod_id].CenterY < nbbox.lly ) nbbox.lly = tmpnt.mods[mod_id].CenterY;
			if( tmpnt.mods[mod_id].CenterX > nbbox.urx ) nbbox.urx = tmpnt.mods[mod_id].CenterX;
			if( tmpnt.mods[mod_id].CenterY > nbbox.ury ) nbbox.ury = tmpnt.mods[mod_id].CenterY;
		}
		else
		{
			int pad_id = tmpnt.pins[j].index - tmpnt.nMod;
			if( tmpnt.pads[pad_id].x < nbbox.llx ) nbbox.llx = tmpnt.pads[pad_id].x;
			if( tmpnt.pads[pad_id].y < nbbox.lly ) nbbox.lly = tmpnt.pads[pad_id].y;
			if( tmpnt.pads[pad_id].x > nbbox.urx ) nbbox.urx = tmpnt.pads[pad_id].x;
			if( tmpnt.pads[pad_id].y > nbbox.ury ) nbbox.ury = tmpnt.pads[pad_id].y;
		}
	}

	if( tmpnt.nets[net_id].nPin == countTSV ) return false;

	return true;
}

void EPLG::TSV:: UpdateNodeTSV( vector<NODE_TSV> &sub_nodeTSV, NETLIST &sub_tmpnt, NETLIST &tmpnt )
{
	for( unsigned int i = 0; i < sub_nodeTSV.size(); i++ )
	{
		// update corr_id
		char name[100] = "\0";
		strcpy( name, sub_tmpnt.mods[ sub_nodeTSV[i].corr_id ].modName );

		if( tmpnt.mod_NameToID.find( name ) != tmpnt.mod_NameToID.end() )
		{
			sub_nodeTSV[i].corr_id = tmpnt.mod_NameToID[ name ];
		}
		else
		{
			cout << "error: unable to find " << name << endl;
			exit(EXIT_FAILURE);
		}

		sub_nodeTSV[i].has_nbbox_lower = FindNetBoundingBox( sub_nodeTSV[i].corr_id, tmpnt.mods[ sub_nodeTSV[i].corr_id ].pNet[0], sub_nodeTSV[i].netbbox_lower, tmpnt );
		sub_nodeTSV[i].has_nbbox_upper = FindNetBoundingBox( sub_nodeTSV[i].corr_id, tmpnt.mods[ sub_nodeTSV[i].corr_id ].pNet[1], sub_nodeTSV[i].netbbox_upper, tmpnt );
	}
}

void EPLG::TSV:: ComputeArcCost( int &cost, NBBOX &nbbox, NODE_SLOT &slot )
{
	if( slot.x < nbbox.llx )
	{
		cost += (nbbox.llx - slot.x);		
	}
	else if( slot.x > nbbox.urx )
	{
		cost += (slot.x - nbbox.urx);
	}

	if( slot.y < nbbox.lly )
	{
		cost += (nbbox.lly - slot.y);
	}
	else if( slot.y > nbbox.ury )
	{
		cost += (slot.y - nbbox.ury);
	}	
}

void EPLG::TSV:: SolveMinCostFlow( vector<NODE_TSV> &sub_nodeTSV, vector<NODE_SLOT> &sub_nodeSLOT, NETLIST &sub_tmpnt, NETLIST &tmpnt )
{
	int m = (int)sub_nodeTSV.size();	///< number of TSVs
	int n = (int)sub_nodeSLOT.size();	///< number of slots

	SmartDigraph g;

	g.reserveNode( m+n+2 );
	g.reserveArc( m+n+m*n );

	SmartDigraph::NodeMap<int> supply(g);
	SmartDigraph::ArcMap<int> lcap(g);
	SmartDigraph::ArcMap<int> ucap(g);
	SmartDigraph::ArcMap<int> cost(g);

	// construct node: TSVs & slots
	for( int i = 0; i < m+n; i++ )
	{
		SmartDigraph::Node node = g.addNode();
		supply[node] = 0;
	}

	// construct node: source
	SmartDigraph::Node s = g.addNode();
	supply[s] = m;
	
	// construct node: target
	SmartDigraph::Node t = g.addNode();
	supply[t] = -m;

	// construct arc: source to TSVs
	for( int i = 0; i < m; i++ )
	{
		SmartDigraph::Arc arc = g.addArc( s, g.nodeFromId(i) );
		lcap[arc] = 1;
		ucap[arc] = 1;
		cost[arc] = 0;
	}

	// construct arc: TSVs to target
	for( int i = m; i < m+n; i++ )
	{
		SmartDigraph::Arc arc = g.addArc( g.nodeFromId(i), t );
		lcap[arc] = 0;
		ucap[arc] = 1;
		cost[arc] = 0;
	}

	int bound_dis = (int)( (tmpnt.ChipWidth + tmpnt.ChipHeight)/(float)3 );
	// construct arc: TSVs to slots
	for( int i = 0; i < m; i++ )
	{
		for( unsigned int j = 0; j < sub_nodeTSV[i].corr_slots.size(); j++ )
		{
			int slot_id = sub_nodeTSV[i].corr_slots[j];
			int node_id = slot_id + m;

			SmartDigraph::Arc arc = g.addArc( g.nodeFromId(i), g.nodeFromId(node_id) );

			lcap[arc] = 0;
			ucap[arc] = 1;

			int cost_arc = 0;
			if( sub_nodeTSV[i].has_nbbox_lower ) ComputeArcCost( cost_arc, sub_nodeTSV[i].netbbox_lower, sub_nodeSLOT[slot_id] );
			if( sub_nodeTSV[i].has_nbbox_upper ) ComputeArcCost( cost_arc, sub_nodeTSV[i].netbbox_upper, sub_nodeSLOT[slot_id] );

			if( cost_arc % AMP_PARA >= 0.5 * AMP_PARA )
			{
				cost[arc] = (int)ceil( cost_arc/(float)AMP_PARA );
			}
			else
			{
				cost[arc] = (int)floor( cost_arc/(float)AMP_PARA );
			}
		}

		for( int j = m; j < m+n; j++ )
		{
			int global_dis = abs( tmpnt.mods[ sub_nodeTSV[i].corr_id ].GCenterX - sub_nodeSLOT[j-m].x ) + 
				             abs( tmpnt.mods[ sub_nodeTSV[i].corr_id ].GCenterY - sub_nodeSLOT[j-m].y );

			if( findArc( g, g.nodeFromId(i), g.nodeFromId(j) ) != INVALID || global_dis > bound_dis )
			{
				continue;
			}

			SmartDigraph::Arc arc = g.addArc( g.nodeFromId(i), g.nodeFromId(j) );

			lcap[arc] = 0;
			ucap[arc] = 1;

			int cost_arc = 0;
			if( sub_nodeTSV[i].has_nbbox_lower ) ComputeArcCost( cost_arc, sub_nodeTSV[i].netbbox_lower, sub_nodeSLOT[j-m] );
			if( sub_nodeTSV[i].has_nbbox_upper ) ComputeArcCost( cost_arc, sub_nodeTSV[i].netbbox_upper, sub_nodeSLOT[j-m] );

			if( cost_arc % AMP_PARA >= 0.5 * AMP_PARA )
			{
				cost[arc] = (int)ceil( cost_arc/(float)AMP_PARA );
			}
			else
			{
				cost[arc] = (int)floor( cost_arc/(float)AMP_PARA );
			}
		}
	}
	 
	NetworkSimplex<SmartDigraph, int>::ProblemType found;
	NetworkSimplex<SmartDigraph, int> network_simplex(g);
	network_simplex.costMap< SmartDigraph::ArcMap<int> >(cost);
	network_simplex.lowerMap< SmartDigraph::ArcMap<int> >(lcap);
	network_simplex.upperMap< SmartDigraph::ArcMap<int> >(ucap);
	network_simplex.supplyMap<SmartDigraph::NodeMap<int> >(supply);

	found = network_simplex.run();

	if( found )
	{
		for( SmartDigraph::ArcIt a(g); a != INVALID; ++a )
		{
			if( network_simplex.flow(a) <= 0 || g.source(a) == s || g.target(a) == t ) continue;

			int s_id = g.id( g.source(a) );
			int t_id = g.id( g.target(a) ) - m;

			tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].CenterX = sub_nodeSLOT[t_id].x;
			tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].CenterY = sub_nodeSLOT[t_id].y;
			tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].LeftX = (int)(sub_nodeSLOT[t_id].x - (0.5 * TSV_SIZE * AMP_PARA));
			tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].LeftY = (int)(sub_nodeSLOT[t_id].y - (0.5 * TSV_SIZE * AMP_PARA));

			int sub_corr_id = sub_tmpnt.mod_NameToID[ tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].modName ];
			sub_tmpnt.mods[sub_corr_id].CenterX = sub_nodeSLOT[t_id].x;
			sub_tmpnt.mods[sub_corr_id].CenterY = sub_nodeSLOT[t_id].y;
			sub_tmpnt.mods[sub_corr_id].LeftX = tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].LeftX;
			sub_tmpnt.mods[sub_corr_id].LeftY = tmpnt.mods[ sub_nodeTSV[s_id].corr_id ].LeftY;
		}

		cout << "-Found feasible flow for layer[" << sub_tmpnt.nLayer << "]" << endl;
	}
	else
	{
		cout << "-Fail to found feasible flow for layer[" << sub_tmpnt.nLayer << "]" << endl;
	}

}

void EPLG::TSV::AssignTSV( vector< vector<NODE_TSV> > &node_TSV, vector< vector<NODE_SLOT> > &node_SLOT, SUBNETLIST &sub_tmpnt, NETLIST &tmpnt )
{
	start_t_TA = clock();

	for( int i = 0; i < tmpnt.nLayer-1; i++ )
	{
		UpdateNodeTSV( node_TSV[i], sub_tmpnt[i], tmpnt );	
		SolveMinCostFlow( node_TSV[i], node_SLOT[i], sub_tmpnt[i], tmpnt );
	}

	end_t_TA = clock();

//	time_TA = (end_t_TA - start_t_TA)/(double)CLOCKS_PER_SEC;
}

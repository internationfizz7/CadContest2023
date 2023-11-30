/*!
 * \file 	Cluster.h
 * \brief 	Best choice Cluster (header)
 *
 * \author 	Tai Ting
 * \version	2.0 (2-D, Object-oriented)
 * \date	2018.07
 */
/**
 * ===================================================================
 *	Input:
 * 		- Initail NetList(nt):	Given NETLIST Info
 *		- ClusteredNET:			To Store Clustered NETLIST Info
 *		- TargetNum: 			Terminal Condition
 *		- ClusterArea: 			Hard Constraint for Area
 *  Output:
 *		- ClusteredNET: 		Contain NETLIST Info in each level
 * ===================================================================
 */
//0816 HYH

#ifndef _CLUSTER_H_
#define _CLUSTER_H_


#include "dataType.h"



class CLUSTER
{
	private:
		bool AreaOrThermal ; /// bool to switch Score Fcn. (ture for thermal)

	protected:

		class ClusterNet
		{
			public:
				//first node
				int ClusterCount;
				int NetIndex;
				bool flagAlign;

				//other node
				int ClusterIndex;


				ClusterNet* next;
				ClusterNet()
				{
					ClusterCount = 0 ;
					ClusterIndex = -1;
					flagAlign = false;
					this->next=NULL;
				}


		};
		class ClusterMod
		{
			public:
				//first Node
				vector <int> Mods; // Module in this cluster
				int NetCount;
				double Area;
				double power;
				double AvgPower;
				int CountMod; // The Total Number of Modules in Cluster
				int ClusterIndex ;
				int layer;
				int nTSV;
				bool InCluster; 
				bool marked;
				bool IsTSV;
				bool IsMacro;// 0810 HYH
				bool IsEverClustered; //0810 HYH

				//other Node
				int NetIndex;


				ClusterMod* next;

				ClusterMod()
				{
					power = NetCount = Area = layer = 0;
					CountMod =1;
					NetIndex = -1;
					marked = InCluster = IsTSV = false;
					this->next= NULL;
				}

		};
		class TUPLE
		{
			public:
				int u; 							///object
				int v; 							///neighbor
				set <int> Net;
				double score ;
				TUPLE()
				{
					score = 0;
					u=v=-1;
				}

		};
		struct cmp
		{
			bool operator() (TUPLE a , TUPLE b)
			{
				return a.score < b.score;
			}

		};


		ClusterNet **NetArray;						/// NET Array
		ClusterMod **ModArray;						/// Mod Array
		int ClusterCount ;							/// ClusterNumber in asscoiated level
		double AvgArea;								/// Average area in asscoiated level
		double AvgPower;							/// Average power in asscoiated level
		double PowerDif;							/// Maximum power difference in associated level
		int ScaledTerm;								/// Balance Power and Area term
		vector <map < int,TUPLE>  > ScoreTable;     /// Score Table (obj to neighbor)
		priority_queue< TUPLE,vector<TUPLE>,cmp> PQ_Score;	/// Proity queue for sorting

		int nTSV;

		int score_fcn_call_time;
		double updata_netlist_runtime;

		int clusterTSVnum;

	public:
		int Clustermain(NETLIST &OGtmpnt, vector <NETLIST> &ClusteredNET);
		int Clustermain3D(NETLIST &OGtmpnt, vector <NETLIST> &ClusteredNET);
		int ThermalCluster( NETLIST &tmpnt, vector <NETLIST> &ClusteredNET);
		//int ThermalCluster3D( NETLIST& tmpnt, NETLIST& LayerNET, int layer);
		vector<int> ThermalCluster3D( NETLIST& tmpnt, vector <NETLIST> &ClusteredNET);
		void CreateNetlist(NETLIST& tmpnt);
		void InitializationPQ(NETLIST & tmpnt);
		double InitialScore(int u_id, int v_id, int ClusterCount, int NetIndex);
		double ScoreFormula(int ClusterCount,ClusterMod* &Obj,ClusterMod* &Nei );
		void Score_fcn(int ObjectID);
		void UpdataNetlist(NETLIST &tmpnt, int ObjectId, int NeighborId );
		bool AreaBound(double AvgArea, double AreaSum);
		bool TSVNumBound(int ObjectId,vector<double> &LayerPowerArray, NETLIST &tmpnt );
		int CaculateResult(NETLIST &tmpnt);
		void Mark_Neighbor(int ObjectID,int ClosestID);
		void TransferOutput(NETLIST &tmpnt, vector< NETLIST > &ClusteredNET);
		//void TransferOutput3D(NETLIST &tmpnt, NETLIST &LayerNET);
		void TransferOutput3D(NETLIST &tmpnt, vector< NETLIST > &ClusteredNET);
		void DestroyArray(NETLIST &tmpnt);
		int Clustering(NETLIST &tmpnt);
		//int Clustering3D(NETLIST &tmpnt, int layer);
		vector<int> Clustering3D(NETLIST &tmpnt);
		void CheckerNetList ( NETLIST &tmpnt, NETLIST& Newtmpnt,int ClusterNum);
		void DeCluster (NETLIST &tmpnt,int level,vector<NETLIST>& ClusteredNET);
		void ShowNetList(NETLIST &tmpnt);
		void SetBool(bool value){
			AreaOrThermal = value;
		}
		void outClusterMod(NETLIST &tmpnt, int level);
		CLUSTER(){
			AreaOrThermal = false;
			clusterTSVnum = 0;
		};
};




#endif

/*!
 * \file 	Cluster.cpp
 * \brief 	Thermal-Aware Best choice Cluster
 *
 * \author 	Tai Ting
 * \version	1.0 (2-D)
 * \date	2017.09.26
 */

/*
	cluster.cpp 0816 HYH 
	# default cluster_divisor_num = 1.8
	# clustermod included TSV are seen as TSV but NOT counted in nTSV.
	# without clustering macro
	# macro fix_size
	# mods with pNet availible 
*/
#include "Cluster.h"
#include "tool.h"
double CLUSTER::ScoreFormula(int ClusterCount,ClusterMod* &Obj,ClusterMod* &Nei )
{

	double objArea = Obj->Area;
	double Neiarea = Nei->Area;
	double ObjPower = Obj->power;
	double NeiPower = Nei->power;
	double ObjPd = (double) ObjPower / objArea;
	double NeiPd = (double) NeiPower / Neiarea ;
	double ObjAvgP = (double) Obj->power / Obj->CountMod  ;
	double NeiAvgP = (double) Nei->power / Nei->CountMod  ;


	double weight = (double) 1/ (ClusterCount-1);
	double AreaSum = (objArea + Neiarea);
	double PowerSum = (ObjPower + NeiPower) * pow( 10.0, this->ScaledTerm );

	double ExpectedSize = (double) AvgArea * 5 ;
	double k = (double)  AreaSum / ExpectedSize ;

	/**
	 *\@ThermalAware : Score
	 */

	// old Score
	// double PowerTerm =abs(  (this->PowerDif ) - abs(ObjPower-NeiPower)  )    * pow( 10, Scaled ) ;
	// double score = (double) weight / pow( PowerTerm, 0.3 ) ;

	// ====== Thermal Formula 1 (Global Themral even) ====== //
	// double PowerTerm = (double) 1 / (ObjPower + NeiPower) ;
	// double score = (double) weight * PowerTerm  / AreaSum ;

	//===== Thermal Formula 2 (Local Thermal eveb) =====//

	double PowerTerm =  abs( ObjPower - NeiPower )  ;
	double score;

	if(this->AreaOrThermal== true) // Area + NET + Thermal
	{
		score = (double) weight * PowerTerm / AreaSum ;
	}
	else // Area + NET
	{

	   score = (double) weight / ( AreaSum) ; // orignal best choice score fcn.
	}

	return score;
}

bool CLUSTER::TSVNumBound(int ObjectId,vector<double> &LayerPowerArray, NETLIST &tmpnt )
{
    if(this->ModArray[ObjectId]->nTSV >
    (tmpnt.nTSV*(this->ModArray[ObjectId]->power/LayerPowerArray[this->ModArray[ObjectId]->layer]))+1)
    {
        //cout << "TSV bounding" << endl;
        //cout << this->ModArray[ObjectId]->nTSV << " " << tmpnt.nTSV*(this->ModArray[ObjectId]->power/LayerPowerArray[this->ModArray[ObjectId]->layer]) << endl;
        //cout << this->ModArray[ObjectId]->power << endl;
        return true;
    }
    else
        return false;
}

bool CLUSTER::AreaBound(double AvgArea, double AreaSum)
{
	// true for jump
	// flase for cluster


	/*  // Hard constraints
		cout<<"ClusterArea :" <<ClusterArea<<endl;
		cout<<"Target Num : "<<InputOption.TargetClusterNum<<endl;
		cout<<"Gredient "<<Initial_Gradient_Else<<endl;
		getchar();*/

	if( AreaSum >  InputOption.ClusterArea * AvgArea )
		return true;
	else
		return false;

	//Soft Constraints
	//srand(time(NULL));// randseed is fixed in main
	/*
	   if(AreaSum > 2 * AvgArea)
	   {
	   double k = 2.0;
	   double Exp = (double) AvgArea / AreaSum ;
	   Exp =  pow( Exp , k);
	   double Probability = pow( 2, Exp ) -1 ;
	   double random_p = rand()%100;
	   random_p = random_p * 0.01;
	// cout<<"random "<<random_p<<endl;
	// cout<<"Probability : "<<Probability<<endl;
	if(random_p <= Probability)
	return false;
	else
	return true;


	}
	 */
}

int CLUSTER::CaculateResult(NETLIST &tmpnt)
{
	int CountNet = 0;
	int CountCluster = 0;
	for(int i=0 ; i< tmpnt.nNet ; i++ )
	{
		int ClusterCount =   this->NetArray[i]->ClusterCount;
		ClusterNet* NetNode = this->NetArray[i];
		NetNode=NetNode->next;
		int PadCount = 0;
		//cout<<"-----Net "<<i<<"----------"<<endl;
		if(ClusterCount != 1)
			CountNet++;
		/*
		   for(int j=0 ; j<ClusterCount; j++)
		   {
		   int ClusterIndex = NetNode->ClusterIndex;

		// cout<<"CLuster "<<ClusterIndex<<endl;
		NetNode=NetNode->next;
		}*/
		// getchar();
	}
	vector <double > area_record;

	int totalArea = 0;
	double MaxPower = -1;
	double MinPower = 999999;
	double MaxAvgP = -1;
	for(int i=0 ; i< tmpnt.nMod ; i++ )
	{
		if(!this->ModArray[i]->InCluster)
		{
			if(this->ModArray[i]->power > MaxPower )
				MaxPower= this->ModArray[i]->power;
			if(this->ModArray[i]->power < MinPower)
				MinPower = this->ModArray[i]->power;

			if(this->ModArray[i]->AvgPower > MaxAvgP)
				MaxAvgP = this->ModArray[i]->AvgPower ;

			totalArea += this->ModArray[i]->Area;
			area_record.push_back( this->ModArray[i]->Area  );
			CountCluster++;
		}

	}

	this->AvgArea  =  (double) totalArea / CountCluster;
	this->PowerDif = MaxPower-MinPower;

	//  cout<<" ---- Results ----- "<<endl;
	//   cout<<" - MaxAvgP "<< MaxAvgP <<endli;
	//   cout<<" - MaxDiff "<< tmpclu.PowerDif<<endl;
	/*  cout<<"og area "<<tmpnt.totalModArea<<" clustered area  "<<totalArea<<endl;
		double Area_mean = (double) tmpnt.totalModArea / tmpnt.nMod;
		sort(area_record.begin(),area_record.end());

		pair<double,double> temp=   GDistribution(area_record);

		for(int i=0; i<area_record.size(); i++ )
		{
		cout<<"Area "<<area_record[i]<<endl;

		}*/
	/*
	   cout<<" ----------Original Circuit---------------"<<endl;
	   cout<<" Net : "<< tmpnt.nNet<<endl;
	   cout<<" Cluster : Total "<<tmpnt.nPad + tmpnt.nMod<<endl;
	   cout<<" Area : "<< tmpnt.totalModArea <<" Avg "<< (double) tmpnt.totalModArea / CountCluster <<endl;
	   cout<<"\t Pad "<<tmpnt.nPad<<endl;
	   cout<<"\t Mod "<<tmpnt.nMod<<endl;
	   cout<<" =========Clutered Circuit================="<<endl;
	   cout<<" Net : "<< CountNet<<endl;
	   cout<<" Area : "<< totalArea <<" Avg "<< tmpclu.AvgArea <<endl;
	   cout<<" Cluster : Total "<<tmpnt.nPad + CountCluster<<endl;
	   cout<<"\t Pad "<<tmpnt.nPad<<endl;
	   cout<<"\t Mod "<<CountCluster<<endl;

	 */
	// getchar();
	return CountCluster;
}
void CLUSTER::Mark_Neighbor(int ObjectID,int ClosestID)
{
	map<int,TUPLE >::iterator it1;
	//this->ModArray[ObjectID]->marked = true;
	for(it1 = this->ScoreTable[ObjectID].begin(); it1!= this->ScoreTable[ObjectID].end(); it1++)
	{
		int NeighborId =  it1->first;
		this->ModArray[NeighborId]->marked = true ;
	}
	for(it1 = this->ScoreTable[ClosestID].begin(); it1!= this->ScoreTable[ClosestID].end(); it1++)
	{
		int NeighborId =it1->first;
		this->ModArray[NeighborId]->marked = true ;
	}

}

void CLUSTER::UpdataNetlist(NETLIST &tmpnt, int ObjectId, int NeighborId )
{

	clock_t srt,end;
	srt=clock();
	// adjust Netarray
	ClusterMod* ModNode_Object =   this->ModArray[ObjectId] ;
	ClusterMod* ModNode_Neighbor = this->ModArray[NeighborId];

	int NetCount_ob= ModNode_Object->NetCount;
	int NetCount_Ne= ModNode_Neighbor->NetCount;

	if(ModNode_Object->next!= NULL)
		ModNode_Object=ModNode_Object->next;
	else
	{
		cout<<"obj mod empty "<<endl;
		getchar();
	}
	if(ModNode_Neighbor->next!= NULL)
		ModNode_Neighbor=ModNode_Neighbor->next;
	else
	{
		cout<<"The modarray empty "<<endl;
		getchar();
	}
	// cout<<"Update netarray.."<<endl;

	set <int> CommonNetA;
	set <int> CommonNetB;
	set <int> CommonNetAB;




	for(int i=0; i< NetCount_ob ; i++ )
	{
		int NetIndex = ModNode_Object->NetIndex;
		CommonNetA.insert(NetIndex);
		ModNode_Object=ModNode_Object->next;
	}

	// cout<<"Neighbor "<<endl;

	for(int i=0; i<NetCount_Ne; i++)
	{
		int NetIndex = ModNode_Neighbor->NetIndex;
		set<int>::iterator it1;
		it1= CommonNetA.find(NetIndex);
		if(it1!= CommonNetA.end()) // common
		{
			CommonNetAB.insert(NetIndex);
		}
		else
		{
			CommonNetB.insert(NetIndex);
		}


		ModNode_Neighbor=ModNode_Neighbor->next;
	}


	set<int>::iterator it1;

	for(it1= CommonNetA.begin(); it1!= CommonNetA.end() ;)
	{
		set<int>::iterator it2;

		it2= CommonNetAB.find( (*it1)   );
		if(it2!= CommonNetAB.end())
		{
			int index = (*it2);
			CommonNetA.erase( it1++ );
		}
		else
			++it1;
	}

	//Don't do anything for NetObject

	/*
	   for(it1 = CommonNetA.begin() ; it1 != CommonNetA.end() ; it1++)
	   {

	   }
	 */
	// Rename the mod name Neighbor to Object
	//cout<<"Rename "<<endl;

	for(it1 = CommonNetB.begin() ; it1 != CommonNetB.end() ; it1++)
	{
		ClusterNet* n1 = this->NetArray[ (*it1) ];

		n1=n1->next;
		bool Flag = false;
		while(n1!=NULL)
		{
			if(n1->ClusterIndex == NeighborId )
			{
				n1->ClusterIndex = ObjectId;
				Flag = true;
				break;
			}

			n1=n1->next;
		}
		if(!Flag)
		{
			cout<<"error : Can' find Cluster 388"<<endl;
			getchar();
		}

	}
	// Delete the neighbor node in common ab
	//cout<<"Delete commab"<<endl;

	for(it1 = CommonNetAB.begin() ; it1 != CommonNetAB.end() ; it1++)
	{
		ClusterNet* n1 = this->NetArray[ (*it1) ];

		n1->ClusterCount = n1->ClusterCount -1 ;

		ClusterNet* prev = n1;

		n1=n1->next;
		bool Flag = false;

		while(n1!=NULL)
		{

			if(n1->ClusterIndex == NeighborId )
			{
				ClusterNet* Next ;
				if(n1->next != NULL)
					Next= n1->next;
				else
					Next = NULL;
				prev->next = Next;

				delete n1 ;
				Flag = true;

				break;
			}

			prev = n1 ;
			n1=n1->next;
		}
		if(!Flag)
		{
			cout<<"error : Can' find Cluster 418"<<endl;
			getchar();
		}


	}
	//check
	ModNode_Object= this->ModArray[ObjectId];
	ModNode_Neighbor = this->ModArray[NeighborId];

	//Update CellArray
	//cout<<"Update Cellarray.."<<endl;

	//_____________________________________________________________
	if(ModNode_Neighbor->IsTSV||ModNode_Object->IsTSV)
	{
		ModNode_Object->IsTSV = true;
	}
    ModNode_Object->IsEverClustered = true;
	ModNode_Object->nTSV += ModNode_Neighbor->nTSV;
	//<----------------------------------------------------------TSV

	ModNode_Neighbor->InCluster= true ;

	ModNode_Object->Area = ModNode_Object->Area + ModNode_Neighbor->Area;

	ModNode_Object->power = ModNode_Object->power + ModNode_Neighbor->power;
	ModNode_Object->CountMod = ModNode_Object->CountMod + ModNode_Neighbor->CountMod;

	ModNode_Object->AvgPower = (double) ( ModNode_Object->power  ) / ( ModNode_Object->CountMod  ) ;


	for(int i=0; i<ModNode_Neighbor->Mods.size(); i ++)
	{
		int ModID = ModNode_Neighbor->Mods[i];

		ModNode_Object->Mods.push_back(  ModID );
	}

	ModNode_Object->NetCount = CommonNetA.size() + CommonNetB.size() + CommonNetAB.size() ;


	//Update object connect net


	// Add the negihbor net
	ClusterMod* trace = ModNode_Object;
	while(trace->next != NULL)
		trace=trace->next;
	for(it1 = CommonNetB.begin() ; it1!= CommonNetB.end() ; it1++)
	{
		ClusterMod* AddNode = new ClusterMod;
		AddNode->NetIndex = (*it1);
		AddNode->next = NULL;

		trace->next=AddNode;


		trace=trace->next;
	}
	// Delete commonAB  and this net need to be two pin
	int Delete_count = 0;

	for(it1 = CommonNetAB.begin() ; it1!= CommonNetAB.end(); it1++)
	{
		int NetIndex = (*it1);
		if(  this->NetArray[NetIndex]->ClusterCount == 1  )
		{

			ClusterMod*  n1 =  ModNode_Object ;
			ModNode_Object->NetCount = ModNode_Object->NetCount - 1 ;
			ClusterMod* prev =n1;
			n1=n1->next;

			bool Jump = false;
			while(n1!=NULL)
			{
				if(n1->NetIndex == NetIndex)
				{
					ClusterMod* NEXT ;


					if(n1->next == NULL)
						NEXT = NULL;
					else
						NEXT = n1->next;

					prev->next = NEXT;

					// cout<<"Delete two pin Net : "<<n1->NetIndex<<endl;
					Delete_count++;

					delete n1;
					Jump = true;
					if(Jump)
					{
						break;
					}
				}
				prev= n1;
				n1=n1->next;
			}

		}

	}
	//Checker
	ClusterMod* Check = ModNode_Object;
	Check=Check->next;
	int Check_netCount = 0;
	while(Check!=NULL)
	{
		Check_netCount++;
		Check=Check->next;
	}
	int TrueCount = CommonNetA.size()+CommonNetAB.size() + CommonNetB.size() - Delete_count;

	if(Check_netCount != TrueCount || Check_netCount != ModNode_Object->NetCount)
	{
		//5/31
		cout <<"UpdateList: netcount error"<<endl;
		//exit(1);
	}

	/*
	   if(NeighborId == 14)
	   {
	   cout<<"Common A : "<<CommonNetA.size()<<endl;
	   for(it1 = CommonNetA.begin() ; it1!= CommonNetA.end(); it1++)
	   {
	   cout<<(*it1)<<"  ";

	   }
	   cout<<endl;
	   cout<<"Common B : "<<CommonNetB.size()<< endl;
	   for(it1 = CommonNetB.begin() ; it1!= CommonNetB.end(); it1++)
	   {
	   cout<< (*it1)<<"  ";

	   }
	   cout<<endl;
	   cout<<"Common AB "<<CommonNetAB.size()<<endl;

	   for(it1 = CommonNetAB.begin() ; it1!= CommonNetAB.end(); it1++)
	   {
	   cout<< (*it1)<<"  ";
	   }
	   cout<<endl;
	//mod obj
	ClusterMod* run = ModNode_Object;
	cout<<"After Mod table : count "<<run->NetCount<<endl;
	run= run->next;

	int Count= 0;
	while(run!= NULL)
	{
	Count++;
	cout<<"net "<< run->NetIndex<<endl;
	run=run->next;
	}
	cout<<"Check size "<<Count<<endl;
	getchar();
	}

	 */

	//Update Score Table
	//Delete the closest Cluster and update the connection in every Neighbor


	// cout<<"Start UPdate Scoretable.. "<<endl;


	map<int,TUPLE >::iterator it2;
	set <int > ScoreObj;
	set <int > ScoreNei;
	set <int > ScoreCommon;


	for(it2 = this->ScoreTable[ObjectId].begin() ; it2 != this->ScoreTable[ObjectId].end() ; it2++ )
	{
		int ModId = it2->first;
		ScoreObj.insert(ModId);
	}

	for(it2 = this->ScoreTable[NeighborId].begin() ; it2!= this->ScoreTable[NeighborId].end(); it2++)
	{
		int ModId = it2->first;
		set<int >::iterator iter;
		iter = ScoreObj.find(ModId);
		if(iter != ScoreObj.end() )
		{
			ScoreCommon.insert( ModId );
		}
		else
		{
			ScoreNei.insert( ModId );
		}
	}
	for(it1 = ScoreObj.begin() ; it1 !=ScoreObj.end() ; )
	{
		int ModId = (*it1);
		set<int >::iterator iter;
		iter = ScoreCommon.find(ModId);
		if(iter!= ScoreCommon.end())
		{
			ScoreObj.erase(it1++);
		}
		else
			++it1;
	}
	// Show CommonScore Table && Orignal ScoreTable
	/*
	   cout<<"--------------show object table ------------"<<endl;
	   for(it2 = ScoreTable[ObjectId].begin() ; it2 != ScoreTable[ObjectId].end() ; it2++ )
	   {
	   int ModId = it2->first;
	   cout<<"MOD id : "<<ModId<<endl;;
	   set<int >::iterator iter;
	   cout<<"Connect Net : ";
	   for(iter = it2->second.Net.begin() ; iter!= it2->second.Net.end(); iter++)
	   {
	   cout<<(*iter) <<"  ";
	   }
	   cout<<endl;

	   }
	   cout<<"-------------neighbor---------------"<<endl;
	   for(it2 = ScoreTable[NeighborId].begin() ; it2 != ScoreTable[NeighborId].end() ; it2++ )
	   {
	   int ModId = it2->first;
	   cout<<"MOD id : "<<ModId<<endl;;
	   set<int >::iterator iter;
	   cout<<"Connect Net : ";
	   for(iter = it2->second.Net.begin() ; iter!= it2->second.Net.end(); iter++)
	   {
	   cout<<(*iter) <<"  ";
	   }
	   cout<<endl;
	// ScoreObj.insert(ModId);
	}

	cout<<"===============Score common Table ==============="<<endl;
	cout<<"Obj: "<<endl;
	for(it1 = ScoreObj.begin() ; it1 !=ScoreObj.end() ; it1++)
	cout<<*it1<<"  ";
	cout<<endl;
	cout<<"Nieghbor : "<<endl;
	for(it1 = ScoreNei.begin() ; it1 !=ScoreNei.end() ; it1++)
	cout<<*it1<<"  ";
	cout<<endl;
	cout<<"Common : "<<endl;
	for(it1 = ScoreCommon.begin() ; it1 !=ScoreCommon.end() ; it1++)
	cout<<*it1<<"  ";
	cout<<endl;


	getchar();
	 */

	for(it2 = this->ScoreTable[ObjectId].begin() ; it2 != this->ScoreTable[ObjectId].end() ; it2++ )
	{
		int ModId = it2->first;
		set<int >::iterator iter;

		iter = ScoreCommon.find( ModId );
		if(iter != ScoreCommon.end()) // is common node
		{
			map<int,TUPLE >::iterator it3;
			it3 =  this->ScoreTable[NeighborId].find(ModId);

			if(it3!=this->ScoreTable[NeighborId].end()) // find the Neibor and combine net
			{

				set<int >::iterator inner;
				for(inner = it3->second.Net.begin() ; inner!= it3->second.Net.end() ; inner++) // insert common node net to obj
				{
					it2->second.Net.insert( (*inner) );
				}
			}
			else
			{
				cout<<"error : can't find Score common"<<endl;
				getchar();
			}
		}


	}
	//Delete Closest Node in obj

	int n= this->ScoreTable[ObjectId].erase(NeighborId);

	//cout<<"Delete neighbor "<<NeighborId<<" ? "<< n<<endl;
	if(n != 1)
	{
		cerr << "Can't find closest : "<<NeighborId<<endl;
		exit(1);
	}
	//insert the neighbor's net to obj exclude objid
	n= ScoreNei.erase(ObjectId);

	if(n != 1)
	{
		cerr << "Can't find obj in closest "<< endl;
		cerr << "obj "<<ObjectId << " Nei "<< NeighborId<<endl;
		exit(1);
	}


	for(it1 = ScoreNei.begin(); it1!= ScoreNei.end(); it1++)
	{
		int ModIndex = (*it1);

		map<int,TUPLE >::iterator it3;
		TUPLE temp;
		temp.u= ObjectId;
		temp.v = ModIndex;


		it3 = this->ScoreTable[ NeighborId ].find(ModIndex);

		if(it3!= this->ScoreTable[NeighborId].end())
		{
			temp.Net = it3->second.Net;
		}
		else
		{
			cout<<"Can't find mod "<<ModIndex<<" in Scoretable : "<<NeighborId<<endl;
			getchar();
		}

		this->ScoreTable[ObjectId].insert((pair<int, TUPLE>( ModIndex, temp)) ) ;
		//cout<<"Insert "<< ModIndex<<endl;
	}

	/**Adjust all the neighbor's scoretable
	 *Three rule for three type:
	 /1. common node
	 /2. only in obj
	 /3. only in closest
	 **/

	for(it2 = this->ScoreTable[ObjectId].begin() ; it2!= this->ScoreTable[ObjectId].end(); it2++ )
	{
		int Modid = it2->first;
		set<int >::iterator it3;
		it3 = ScoreCommon.find( Modid );
		if(it3!= ScoreCommon.end())  // is the common node
		{
			map<int,TUPLE >::iterator Inner;
			Inner = this->ScoreTable[Modid].find(ObjectId);
			//1.Update ObjectId Net and Score
			//2.Delete cloesest ID
			Inner->second.Net = it2->second.Net;
			Inner->second.score = it2->second.score;
			this->ScoreTable[Modid].erase( NeighborId );
		}
		else
		{
			it3= ScoreNei.find(Modid);
			if(it3 != ScoreNei.end()) // is only in neighbor
			{
				//Rename the Neighbor ID to obj ID
				//update score
				TUPLE temp;
				temp.Net = it2->second.Net;
				temp.score = it2->second.score;
				this->ScoreTable[Modid].insert((pair<int, TUPLE>( ObjectId, temp)) ) ;
				this->ScoreTable[Modid].erase(NeighborId);
			}
			else
			{
				it3 = ScoreObj.find(Modid);
				//Just update score
				if(it3 != ScoreObj.end())// is only in obj
				{
					map<int,TUPLE >::iterator Inner;
					Inner = this->ScoreTable[Modid].find(ObjectId);
					Inner->second.score = it2->second.score;
					Inner->second.Net = it2->second.Net;

				}
				else
				{
					cerr << "Can't find mod "<< Modid <<endl ;
					exit(1);

				}
			}


		}


	}

	   /*cout<<"==============After update ================="<<endl;
	   cout<<"--------------show object table ------------"<<endl;
	   for(it2 = ScoreTable[ObjectId].begin() ; it2 != ScoreTable[ObjectId].end() ; it2++ )
	   {
	   int ModId = it2->first;
	   cout<<"MOD id : "<<ModId<<endl;;
	   set<int >::iterator iter;
	   cout<<"Score : "<<it2->second.score<<endl;
	   cout<<"Connect Net : ";
	   for(iter = it2->second.Net.begin() ; iter!= it2->second.Net.end(); iter++)
	   {
	   cout<<(*iter) <<"  ";
	   }
	   cout<<endl;

	   }
	   cout<<"-------------neighbor---------------"<<endl;
	   for(it2 = ScoreTable[ObjectId].begin() ; it2 != ScoreTable[ObjectId].end() ; it2++ )
	   {
	   int ModId = it2->first;
	   cout<<"Neighbors id : "<<ModId<<endl;
	   map<int,TUPLE >::iterator Inner;
	   for(Inner= ScoreTable[ModId].begin(); Inner!= ScoreTable[ModId].end(); Inner++)
	   {
	   cout<<"Negibor connect Mod : "<< Inner->first<<endl;
	   cout<<"Score : "<<Inner->second.score<<endl;
	   cout<<"Net: "<<endl;
	   set<int >::iterator iter;

	   for(iter = it2->second.Net.begin() ; iter!= it2->second.Net.end(); iter++)
	   {
	   cout<<(*iter) <<"  ";
	   }
	   cout<<endl;
	   }
	   }

	   getchar();*/
	end = clock();

	updata_netlist_runtime += (end-srt);
}

void CLUSTER::Score_fcn(int ObjectID)
{

	this->score_fcn_call_time++;

	ClusterMod* NodeU = this->ModArray[ObjectID];

	if(NodeU->InCluster)
	{
		cerr<<"This cell is in the Cluster"<<endl;
		exit(1);
	}
	//Caculate
	//cout<<"OBjectId : "<<ObjectID<<endl;
	map<int,TUPLE >::iterator it1 ;
	for(it1 = this->ScoreTable[ObjectID].begin(); it1!= this->ScoreTable[ObjectID].end(); it1++)
	{
		int Neighbor = it1->first;
		// cout<<"NEighbor : "<<it1->first<<endl;
		// cout<<"Before Score "<<it1->second.score<<endl;
		//initialize
		it1->second.score = 0;
		set<int >::iterator it2 ;
		for(it2 = it1->second.Net.begin() ; it2!= it1->second.Net.end() ; it2++)
		{
			int NetIndex = (*it2);
			int ClusterCount = this->NetArray[NetIndex]->ClusterCount;

			double weight = (double) 1 / ( ClusterCount - 1 );
			double AreaObj = this->ModArray[ObjectID]->Area;
			double AreaNei = this->ModArray[Neighbor]->Area;

			double score = ScoreFormula(ClusterCount, this->ModArray[ObjectID],this->ModArray[Neighbor]);


			it1->second.score += score ;

		}
		// copy score and net to all neighbor cell, this not the closest, (cloest is deleted)

		map<int,TUPLE >::iterator iter ;
		iter = this->ScoreTable[Neighbor].find(ObjectID);
		if(iter!= this->ScoreTable[Neighbor].end())
		{
			iter->second.Net = it1->second.Net;
			iter->second.score = it1->second.score;
		}
		else
		{
			cout<<"Can't find obj in nei"<<endl;
			getchar();
		}

	}


	//Find Closest

	double max_score = -1;
	int closest_obj = -1;

	for(it1 = this->ScoreTable[ObjectID].begin(); it1!= this->ScoreTable[ObjectID].end(); it1++)
	{
		int ModID = it1->first;


		if(this->ModArray[ModID]->InCluster)
		{
			it1->second.score = -1;
			cerr<<"The cell is in cluster: "<<ModID<<endl;
			exit(1);
		}
		else
		{
			if(it1->second.score > max_score)
			{

				max_score = it1->second.score;
				closest_obj = ModID;
			}

		}
		// cout<<"==============================================="<<endl;
		// cout<<"OBJ Power "<<tmpclu.ModArray[ObjectID]->power * pow( 10 , 6.0)<<endl;
		// cout<<"Nei Power "<<tmpclu.ModArray[ModID]->power * pow( 10 , 6.0)<<endl;
		// cout<<"Score : "<< it1->second.score<<endl;

	}
	if(closest_obj != -1)
	{
		TUPLE temp;
		temp.u=ObjectID;
		temp.v=closest_obj;
		temp.score = max_score;
		// cout<<"Re-insert obj "<<temp.u<<" Nei "<<temp.v<<" score "<< temp.score<<endl;
		this->PQ_Score.push( temp );
		/*
		   cout<<"---------------------------"<<endl<<endl;
		   cout<< "# Score "<<temp.score<<endl;
		   cout<< "OBJ: "<< temp.u<< "NEI :  "<< temp.v<<endl;
		   cout<< "Max Power Dif : "<< tmpclu.PowerDif <<endl;
		   cout<< "Obj P: "<< tmpclu.ModArray[temp.u]->power << " Nei : "<< tmpclu.ModArray[temp.v]->power<<endl;
		   cout<< "diff : "<< abs(tmpclu.ModArray[temp.u]->power - tmpclu.ModArray[temp.v]->power   ) <<endl;
		   cout<< "PowerTerm "<< abs(  (tmpclu.PowerDif ) - abs( tmpclu.ModArray[temp.u]->power - tmpclu.ModArray[temp.v]->power  )  )    * pow( 10, 6.0 ) <<endl;
		   cout<<"--Area "<<endl;
		   cout<< "OBJ Area "<< tmpclu.ModArray[temp.u]->Area << "NEI : "<< tmpclu.ModArray[temp.v]->Area<<endl;
		 */
	}
	else
	{
		//7/24 jack 註解調，此判斷是不影響輸出
		/*cout<<"Doesn't push back to q"<<endl;
		cout <<"ObjectID:" <<ObjectID << endl;*/

	}



}

int CLUSTER::Clustering(NETLIST &tmpnt)
{

	int OrignalModNum = tmpnt.nMod;

	int ClusterNum = tmpnt.nMod;

	int TargetNum = ClusterNum / 1.5;

	// ===== 0305 Test Minize PowerDif =====//
	double PowerDif = this->PowerDif ;
	double TargetDif = PowerDif / 1.5;
	//===================//

	double score = 1;

	int IterCount = 0;

	double TotalPower = 0;

	for(int i=0; i<tmpnt.nMod; i++)
	{
		TotalPower+=tmpnt.mods[i].Power ;
	}

	double PowerMean = TotalPower / tmpnt.nMod;
	double TargetMean = PowerMean * 0.5 ;
	TUPLE Max_score ;
	Max_score.score = -1 ;
	Max_score.u = 0;
	Max_score.v = 0;
	int NetCount = 0;

	while(  TargetNum <= ClusterNum && !(this->PQ_Score.empty()) && score > 0)
		//   while(  TargetDif <= PowerDif && !PQ_Score.empty() && score > 0)
	{



		// ShowScore(PQ_Score,IterCount,tmpclu);
		TUPLE BestTuple = this->PQ_Score.top();
		this->PQ_Score.pop();
		int ObjectId = BestTuple.u;
		int NeighborId = BestTuple.v;
		/*
		   cout<<"===========Cluster info================="<<endl;
		   cout<<" - Object "<<ObjectId<<" Power "<<ModArray[ObjectId]->power <<"  Area "<< ModArray[ObjectId]->Area<<endl;
		   cout<<" - neighbor "<<NeighborId<<" Power "<<ModArray[NeighborId]->power<<" Area "<<ModArray[NeighborId]->Area<<endl;
		   cout<<" - score "<<BestTuple.score<<" Iter "<<IterCount<<endl;
		   cout<<"========================================"<<endl;*/
		score = BestTuple.score;
		// filter the same pair , ex. 1 2 , 2 1
		if( this->ModArray[ObjectId]->InCluster == true ) //already cluster
		{
			continue;
		}

		// Hard Constraint

		double AvgArea = (double)  tmpnt.totalModArea / ClusterNum ;
		double AreaSum = this->ModArray[ObjectId]->Area + this->ModArray[NeighborId]->Area;

		if(  AreaBound(AvgArea,AreaSum)  )
			continue;
		//      if(  PowerBound(tmpclu.AvgPower,  ModArray[ObjectId]->power + ModArray[NeighborId]->power))
		//         continue;

		/*
		   cout<<" =========== Cluster info ================= "<<endl;
		   cout<<" - Object "<<ObjectId<<" Power "<<ModArray[ObjectId]->power <<"  Area "<< ModArray[ObjectId]->Area<<endl;
		   cout<<" - neighbor "<<NeighborId<<" Power "<<ModArray[NeighborId]->power<<" Area "<<ModArray[NeighborId]->Area<<endl;
		   cout<<" - PD : "<< (double)ModArray[ObjectId]->power /  ModArray[ObjectId]->Area<< "  "<<(double)ModArray[NeighborId]->power / ModArray[NeighborId]->Area<<endl;
		   cout<<" - AvgP : "<< ModArray[ObjectId]->AvgPower <<" AvgP2 "<<ModArray[NeighborId]->AvgPower<<endl;
		   cout<<" - Count : "<< ModArray[ObjectId]->CountMod << "  "<< ModArray[NeighborId]->CountMod <<endl;
		   cout<<" - Power Term :"<< abs( ModArray[ObjectId]->AvgPower - ModArray[NeighborId]->AvgPower ) ;
		   cout<<" - Area Term : "<< AreaSum<<endl;
		   cout<<" - score "<<BestTuple.score<<" Iter "<<IterCount<<endl;
		   cout<<" ========================================== "<<endl;*/
		// getchar();


		if( this->ModArray[ObjectId]->marked==true ) // Re-caculate
		{
			//cout<<" - Unlock cell and Re-caculate"<<endl;
			Score_fcn(ObjectId);   // find closest object with score
			this->ModArray[ObjectId]->marked = false;
		}
		else
		{
			if(  this->ModArray[NeighborId]->InCluster==true )
			{
				cerr<<"Error : Already Cluster :  "<< NeighborId<<endl ;
				exit(1);
			}

			//  cout<<"Update Net:"<<endl<<endl;
			UpdataNetlist( tmpnt, ObjectId, NeighborId ); //Update Cellarray netarray scoretable but not queue
			//  cout<<"Re-caculate Score "<<endl<<endl;
			Score_fcn(ObjectId);   // find closest object with score and insert to PQ
			Mark_Neighbor(ObjectId,NeighborId );

		}
		// cout<<"-------------Cluster finish----------------"<<endl;

		ClusterNum = CaculateResult(tmpnt);



		this->AvgArea = (double ) tmpnt.totalModArea / ClusterNum ;
		this->AvgPower = (double) TotalPower / ClusterNum;


		PowerMean = this->AvgPower;
		PowerDif = this->PowerDif;
		//double Dev = ReturnDev(tmpnt,tmpclu);
		// cout<<" - Global AvgP "<< tmpclu.AvgPower <<" Dev "<< Dev <<endl;

		IterCount++;
		//   getchar();

	}

	if(this->PQ_Score.empty())
	{
		cout<<" - PQ is Empty "<<endl;
	}

	// ShowScore(PQ_Score,IterCount,tmpclu);

	this->ClusterCount = CaculateResult(tmpnt);

	return ClusterNum;
	//   return PowerDif;

}

vector<int> CLUSTER::Clustering3D(NETLIST &tmpnt)
{

	int OrignalModNum = tmpnt.nMod;

	int ClusterNum = tmpnt.nMod;

	double cluster_divisor = 1.5;
	//cluster_divisor = double(InputOption.cluster_mode/100.0);
	
	int TargetNum = ClusterNum / cluster_divisor;

	// ===== 0305 Test Minize PowerDif =====//
	double PowerDif = this->PowerDif ;
	double TargetDif = PowerDif / 2/*1.5*/;
	//===================//

	double score = 1;

	int IterCount = 0;

	double TotalPower = 0;
	vector<double> LayerPowerArray;
	LayerPowerArray.resize(tmpnt.nLayer);

	for(int i=0; i<tmpnt.nMod; i++)
	{
		TotalPower+=tmpnt.mods[i].Power ;
		LayerPowerArray[tmpnt.mods[i].modL]+=tmpnt.mods[i].Power;
	}

	double PowerMean = TotalPower / tmpnt.nMod;
	double TargetMean = PowerMean * 0.5 ;
	TUPLE Max_score ;
	Max_score.score = -1 ;
	Max_score.u = 0;
	Max_score.v = 0;
	int NetCount = 0;

	clock_t srt,end;
	int sc_fn=0;
	srt = clock();
	while(  TargetNum <= ClusterNum && !(this->PQ_Score.empty()) && score > 0)
		//   while(  TargetDif <= PowerDif && !PQ_Score.empty() && score > 0)
	{
		// ShowScore(PQ_Score,IterCount,tmpclu);
		TUPLE BestTuple = this->PQ_Score.top();
		this->PQ_Score.pop();
		int ObjectId = BestTuple.u;
		int NeighborId = BestTuple.v;

		/*if( TSVNumBound(ObjectId,LayerPowerArray, tmpnt ) && this->ModArray[NeighborId]->nTSV)
		{
		   cout<<"===========Cluster info================="<<endl;
		   cout<<" - Object "<<ObjectId<<" Power "<<ModArray[ObjectId]->power <<"  Area "<< ModArray[ObjectId]->Area<< " Layer "<<ModArray[ObjectId]->layer <<endl;
		   cout<<" - neighbor "<<NeighborId<<" Power "<<ModArray[NeighborId]->power<<" Area "<<ModArray[NeighborId]->Area<< " Layer "<<ModArray[ObjectId]->layer <<endl;
		   cout<<" - score "<<BestTuple.score<<" Iter "<<IterCount<<endl;
		   cout<<"========================================"<<endl;
        }*/
		score = BestTuple.score;
		// filter the same pair , ex. 1 2 , 2 1
		if( this->ModArray[ObjectId]->InCluster == true ) //already cluster
		{
			continue;
		}

		if(this->ModArray[ObjectId]->IsMacro||this->ModArray[NeighborId]->IsMacro)
		{
			continue; //0810 HYH clustering without macro
		}

		double AvgArea = (double)  tmpnt.totalModArea / ClusterNum ;
		double AreaSum = this->ModArray[ObjectId]->Area + this->ModArray[NeighborId]->Area;

		if(  AreaBound(AvgArea,AreaSum)  )
			continue;
        if( TSVNumBound(ObjectId,LayerPowerArray, tmpnt ) && this->ModArray[NeighborId]->nTSV)
            continue;
		//      if(  PowerBound(tmpclu.AvgPower,  ModArray[ObjectId]->power + ModArray[NeighborId]->power))
		//         continue;


		  /* cout<<" =========== Cluster info ================= "<<endl;
		   cout<<" - Object "<<ObjectId<<" Power "<<ModArray[ObjectId]->power <<"  Area "<< ModArray[ObjectId]->Area<<endl;
		   cout<<" - neighbor "<<NeighborId<<" Power "<<ModArray[NeighborId]->power<<" Area "<<ModArray[NeighborId]->Area<<endl;
		   cout<<" - PD : "<< (double)ModArray[ObjectId]->power /  ModArray[ObjectId]->Area<< "  "<<(double)ModArray[NeighborId]->power / ModArray[NeighborId]->Area<<endl;
		   cout<<" - AvgP : "<< ModArray[ObjectId]->AvgPower <<" AvgP2 "<<ModArray[NeighborId]->AvgPower<<endl;
		   cout<<" - Count : "<< ModArray[ObjectId]->CountMod << "  "<< ModArray[NeighborId]->CountMod <<endl;
		   cout<<" - Power Term :"<< abs( ModArray[ObjectId]->AvgPower - ModArray[NeighborId]->AvgPower ) ;
		   cout<<" - Area Term : "<< AreaSum<<endl;
		   cout<<" - TSV   : " << ModArray[ObjectId]->IsTSV << " " << ModArray[NeighborId]->IsTSV <<endl;
		   cout<<" - Layer : " << ModArray[ObjectId]->layer << " " << ModArray[NeighborId]->layer <<endl;
		   cout<<" - score "<<BestTuple.score<<" Iter "<<IterCount<<endl;
		   cout<<" ========================================== "<<endl;*/
		//getchar();

		
		if( this->ModArray[ObjectId]->marked==true ) // Re-caculate
		{

			//cout<<" - Unlock cell and Re-caculate"<<endl;
			Score_fcn(ObjectId);   // find closest object with score
			this->ModArray[ObjectId]->marked = false;
		}
		else
		{
			if(  this->ModArray[NeighborId]->InCluster==true )
			{
				cerr<<"Error : Already Cluster :  "<< NeighborId<<endl ;
				exit(1);
			}

			//  cout<<"Update Net:"<<endl<<endl;
			UpdataNetlist( tmpnt, ObjectId, NeighborId ); //Update Cellarray netarray scoretable but not queue
			//  cout<<"Re-caculate Score "<<endl<<endl;
			Score_fcn(ObjectId);   // find closest object with score and insert to PQ
			Mark_Neighbor(ObjectId,NeighborId );
			ClusterNum--;

		}
		// cout<<"-------------Cluster finish----------------"<<endl;

		//ClusterNum = CaculateResult(tmpnt);
		


		this->AvgArea = (double ) tmpnt.totalModArea / ClusterNum ;
		this->AvgPower = (double) TotalPower / ClusterNum;


		PowerMean = this->AvgPower;
		PowerDif = this->PowerDif;
		//double Dev = ReturnDev(tmpnt,tmpclu);
		// cout<<" - Global AvgP "<< tmpclu.AvgPower <<" Dev "<< Dev <<endl;

		IterCount++;
		//   getchar();
		if(IterCount%1000==999)
		{
			end = clock();
			//cout<<"runtime(average): "<<float(end-srt)/(1000.0*CLOCKS_PER_SEC)<<"s( sc_fcn:\n";
			//cout<<score_fcn_call_time - sc_fn<<endl;
			//sc_fn = score_fcn_call_time;
			//cout<<"--update netlist: "<<updata_netlist_runtime/(1000.0*CLOCKS_PER_SEC)<<endl;
			updata_netlist_runtime = 0;
			srt = clock();
		}
	}

	if(this->PQ_Score.empty())
	{
		cout<<" - PQ is Empty "<<endl;
	}

	// ShowScore(PQ_Score,IterCount,tmpclu);

	this->ClusterCount = CaculateResult(tmpnt);

    vector<int> ClusterNumArray;
    ClusterNumArray.resize(tmpnt.nLayer);

    for(int i=0; i<tmpnt.nMod; i++)
	{
        if(!ModArray[i]->InCluster)
        {
            ClusterNumArray[ModArray[i]->layer]++ ;
        }
	}
	//cout<<ClusterNum<<"/"<<tmpnt.nMod<<"(/"<<cluster_divisor<<")"<<endl;
	return ClusterNumArray;
	//   return PowerDif;

}

/*int CLUSTER::Clustering3D(NETLIST &tmpnt, int layer)
{

	int OrignalModNum = tmpnt.nMod;

	int ClusterNum = tmpnt.LayernMod[layer];

	int TargetNum = ClusterNum / 1.5;

	// ===== 0305 Test Minize PowerDif =====//
	double PowerDif = this->PowerDif ;
	double TargetDif = PowerDif / 1.5 ;
	//===================//

	double score = 1;

	int IterCount = 0;

	double TotalPower = 0;

	for(int i=0; i<tmpnt.nMod; i++)
	{
		TotalPower+=tmpnt.mods[i].Power ;
	}

	double PowerMean = TotalPower / tmpnt.nMod;
	double TargetMean = PowerMean * 0.5 ;
	TUPLE Max_score ;
	Max_score.score = -1 ;
	Max_score.u = 0;
	Max_score.v = 0;
	int NetCount = 0;

	while(  TargetNum <= ClusterNum && !(this->PQ_Score.empty()) && score > 0)
		//   while(  TargetDif <= PowerDif && !PQ_Score.empty() && score > 0)
	{



		// ShowScore(PQ_Score,IterCount,tmpclu);
		TUPLE BestTuple = this->PQ_Score.top();
		this->PQ_Score.pop();
		int ObjectId = BestTuple.u;
		int NeighborId = BestTuple.v;

		//   cout<<"===========Cluster info================="<<endl;
		//   cout<<" - Object "<<ObjectId<<" Power "<<ModArray[ObjectId]->power <<"  Area "<< ModArray[ObjectId]->Area<<endl;
		//   cout<<" - neighbor "<<NeighborId<<" Power "<<ModArray[NeighborId]->power<<" Area "<<ModArray[NeighborId]->Area<<endl;
		//   cout<<" - score "<<BestTuple.score<<" Iter "<<IterCount<<endl;
		//   cout<<"========================================"<<endl;
		score = BestTuple.score;
		// filter the same pair , ex. 1 2 , 2 1
		if( this->ModArray[ObjectId]->InCluster == true ) //already cluster
		{
			continue;
		}

		// Hard Constraint

		double AvgArea = (double)  tmpnt.totalModArea / ClusterNum ;
		double AreaSum = this->ModArray[ObjectId]->Area + this->ModArray[NeighborId]->Area;

		if(  AreaBound(AvgArea,AreaSum)  )
			continue;
		//      if(  PowerBound(tmpclu.AvgPower,  ModArray[ObjectId]->power + ModArray[NeighborId]->power))
		//         continue;


		  //cout<<" =========== Cluster info ================= "<<endl;
		  // cout<<" - Object "<<ObjectId<<" Power "<<ModArray[ObjectId]->power <<"  Area "<< ModArray[ObjectId]->Area<<endl;
		  // cout<<" - neighbor "<<NeighborId<<" Power "<<ModArray[NeighborId]->power<<" Area "<<ModArray[NeighborId]->Area<<endl;
		  // cout<<" - PD : "<< (double)ModArray[ObjectId]->power /  ModArray[ObjectId]->Area<< "  "<<(double)ModArray[NeighborId]->power / ModArray[NeighborId]->Area<<endl;
		  // cout<<" - AvgP : "<< ModArray[ObjectId]->AvgPower <<" AvgP2 "<<ModArray[NeighborId]->AvgPower<<endl;
		  // cout<<" - Count : "<< ModArray[ObjectId]->CountMod << "  "<< ModArray[NeighborId]->CountMod <<endl;
		  // cout<<" - Power Term :"<< abs( ModArray[ObjectId]->AvgPower - ModArray[NeighborId]->AvgPower ) ;
		  // cout<<" - Area Term : "<< AreaSum<<endl;
		  // cout<<" - TSV   : " << ModArray[ObjectId]->IsTSV << " " << ModArray[NeighborId]->IsTSV <<endl;
		  // cout<<" - Layer : " << ModArray[ObjectId]->layer << " " << ModArray[NeighborId]->layer <<endl;
		   //cout<<" - score "<<BestTuple.score<<" Iter "<<IterCount<<endl;
		  // cout<<" ========================================== "<<endl;
		//getchar();


		if( this->ModArray[ObjectId]->marked==true ) // Re-caculate
		{
			//cout<<" - Unlock cell and Re-caculate"<<endl;
			Score_fcn(ObjectId);   // find closest object with score
			this->ModArray[ObjectId]->marked = false;
		}
		else
		{
			if(  this->ModArray[NeighborId]->InCluster==true )
			{
				cerr<<"Error : Already Cluster :  "<< NeighborId<<endl ;
				exit(1);
			}

			//  cout<<"Update Net:"<<endl<<endl;
			UpdataNetlist( tmpnt, ObjectId, NeighborId ); //Update Cellarray netarray scoretable but not queue
			//  cout<<"Re-caculate Score "<<endl<<endl;
			Score_fcn(ObjectId);   // find closest object with score and insert to PQ
			Mark_Neighbor(ObjectId,NeighborId );

		}
		// cout<<"-------------Cluster finish----------------"<<endl;

		ClusterNum = CaculateResult(tmpnt);



		this->AvgArea = (double ) tmpnt.totalModArea / ClusterNum ;
		this->AvgPower = (double) TotalPower / ClusterNum;


		PowerMean = this->AvgPower;
		PowerDif = this->PowerDif;
		//double Dev = ReturnDev(tmpnt,tmpclu);
		// cout<<" - Global AvgP "<< tmpclu.AvgPower <<" Dev "<< Dev <<endl;

		IterCount++;
		//   getchar();

	}

	if(this->PQ_Score.empty())
	{
		cout<<" - PQ is Empty "<<endl;
	}

	// ShowScore(PQ_Score,IterCount,tmpclu);

	this->ClusterCount = CaculateResult(tmpnt);

	return ClusterNum;
	//   return PowerDif;

}*/

double CLUSTER::InitialScore(int u_id, int v_id, int ClusterCount, int NetIndex)
{

	ClusterMod* NodeU = this->ModArray[u_id];
	ClusterMod* NodeV = this->ModArray[v_id];

	double score;

	score = ScoreFormula(ClusterCount,NodeU,NodeV);
    /*if(u_id == 990)
    {
        cout << score << endl;
    }*/
	int Mod_index =u_id;
	int NeighborIndex= v_id;

	map<int,TUPLE >::iterator it1;
	it1=this->ScoreTable[Mod_index].find( NeighborIndex );
	if(it1!= this->ScoreTable[Mod_index].end()) // This Cluster is already in the Score
	{

		set<int >::iterator it2;

		it2 = it1->second.Net.find(NetIndex);
		if(it2!=it1->second.Net.end())
		{
			cout<<"error: The net is already in Net(NetIndex) "<<NetIndex <<" connect(v_id) "<<NeighborIndex<<" (u_id) "<<Mod_index<<endl;
			getchar();

		}
		else
			it1->second.Net.insert( NetIndex );

		it1->second.score = it1->second.score + score;
	}
	else
	{
		TUPLE temp;
		temp.Net.insert(NetIndex);
		temp.score = score;
		this->ScoreTable[Mod_index].insert(pair<int, TUPLE>( NeighborIndex, temp));
	}


	return 0;
}
void CLUSTER::InitializationPQ(NETLIST & tmpnt)
{

	//Initail area
	double TotalArea = 0;
	double TotalPower = 0;
	double MaxPower = -1;
	double MinPower = 999999;
	for(int i=0; i< tmpnt.nMod; i++)
	{
		TotalPower += tmpnt.mods[i].Power;
		TotalArea += tmpnt.mods[i].modArea;

		if(tmpnt.mods[i].Power > MaxPower)
			MaxPower = tmpnt.mods[i].Power;
		if(tmpnt.mods[i].Power < MinPower)
			MinPower = tmpnt.mods[i].Power;


	}
	this->AvgArea = (double)  TotalArea / tmpnt.nMod ;
	this->AvgPower = (double) TotalPower / tmpnt.nMod;
	this->PowerDif =  MaxPower - MinPower ;


	// Build and Initial Score Table

	for(int i=0; i< tmpnt.nMod; i++)
	{
		ClusterMod* NodeU= this->ModArray[i] ;
		int NetCount = NodeU->NetCount;


		for(int j=0; j< NetCount ; j++ ) //ModList
		{
			//Jump first node
			if(j==0 )
				NodeU=NodeU->next;



			int ModCount = this->NetArray[NodeU->NetIndex]->ClusterCount; //NetDegree

			int NetIndex = NodeU->NetIndex;

			ClusterNet* NetNode = this->NetArray[NodeU->NetIndex];

			for(int z=0; z<ModCount ; z++ ) //NetList
			{
				if( z==0)
					NetNode=NetNode->next;

				// Jump itself and pad ,so ScoreTable doesn't has Pad and itself

				/*if(i == 990)
                {
				   cout<<"Mod: "<<i<<endl;
				   cout<<"Net: "<< NodeU->NetIndex<<endl;
				   cout<<"neighbor "<< NetNode->ClusterIndex<<endl;
				   cout<<"Layer: " <<  ModArray[i]->layer << " " <<ModArray[NetNode->ClusterIndex]->layer << endl;
				   cout<<"Before Score "<<endl;
				   getchar();
                }*/
				if(NetNode->ClusterIndex == i || NetNode->ClusterIndex >= tmpnt.nMod ||
                    ModArray[NetNode->ClusterIndex]->layer != ModArray[i]->layer)
				{
					NetNode=NetNode->next;
					continue;
				}


				ClusterMod* NodeV = ModArray[NetNode->ClusterIndex];



				InitialScore( i, NodeV->ClusterIndex, ModCount, NetIndex );


				NetNode=NetNode->next;
			}
			//  cout<<"================================"<<endl<<endl;

			NodeU=NodeU->next;
		}

	}
	/*
	   ofstream fout("Scoretable.txt");
	   if(!fout)
	   cout<<"error writing "<<endl;

	   for(int i=0; i<tmpnt.nMod ; i++ )
	   {
	   fout<<"Cell "<<tmpnt.mods[i].modName<<endl;
	   map<int , TUPLE>::iterator iter;

	   for(iter= this->ScoreTable[i].begin() ; iter != this->ScoreTable[i].end(); iter ++ )
	   {
	   int ModId = iter->first ;
	   cout<< "ModID : "<< ModId <<endl;
	   if(ModId < tmpnt.nMod)
	   {
	   fout<< tmpnt.mods[ModId].modName <<" Score "<<iter->second.score  <<endl;

	   }
	   else
	   {
	   fout<< tmpnt.pads[ModId-tmpnt.nMod].padName <<" Score "<< iter->second.score<<endl;


	   }


	   }
	   fout<<"========================================"<<endl;


	   }
	 */


	// Put the Closest mod to queue

	for(int i=0; i<tmpnt.nMod; i ++)
	{
		int ObjectID = i;
		double MaxScore = -1;
		int MaxNeighbor = -1;

		map<int, TUPLE>::iterator iter;

		for(iter= this->ScoreTable[i].begin() ; iter != this->ScoreTable[i].end(); iter ++ )
		{
			int NeighborID = iter->first;
			double NeighborScore = iter->second.score;
			/*if(ObjectID == 4)
            {
               cout<<"Mod: "<<ObjectID<<endl;
               cout<<"NeighborID "<< NeighborID<<endl;
               cout<<"NeighborScore "<< NeighborScore<<endl;
            }*/

			if(NeighborID < tmpnt.nMod)
			{
				if(MaxScore < NeighborScore)
				{
					MaxNeighbor = NeighborID;
					MaxScore = NeighborScore;
				}
			}
			else
			{
				cerr<< " Error: Pad in the Score "<<endl;
				exit(1);
			}

		}
		TUPLE temp;
		temp.u = ObjectID;
		temp.v = MaxNeighbor;
		temp.score = MaxScore;
        /*if(ObjectID == 4)
        {
            cout << "here" << endl;
            cout<<"Mod: "<<temp.u<<endl;
            cout<<"NeighborID "<< temp.v<<endl;
            cout<<"NeighborScore "<< temp.score<<endl;
            getchar();
        }*/

        if(MaxScore== -1)
            continue;

		this->PQ_Score.push(temp);

		//test
		if(MaxScore== -1)
		{
			cerr<<"Score error! "<<endl;
			cout << tmpnt.mods[ObjectID].flagTSV << " " << tmpnt.mods[MaxNeighbor].flagTSV << endl;
			cout << tmpnt.mods[ObjectID].modL << " " << tmpnt.mods[MaxNeighbor].modL << endl;
			cout << ObjectID << " " << MaxNeighbor << endl;
			//exit(1);
		}
	}
	/*
		ofstream fout("TUPLE.txt");
		fout<<"Queue: size "<<PQ_Score.size()<<endl;
		while(!PQ_Score.empty())
		{

		TUPLE temp;
		temp = PQ_Score.top();
		fout<< "object "<< temp.u<<" Neighbor "<<temp.v<<" score "<< temp.score<<endl;
		PQ_Score.pop();

		}
		fout.close();

		getchar();
	 */



}

void CLUSTER::CreateNetlist(NETLIST& tmpnt)
{
	//  cout<<"-----------Net info----------------"<<endl;
	//  cout<<"nNet : "<<tmpnt.nNet<<endl;
	//  cout<<"nMod : "<<tmpnt.nMod<<endl;
	//  cout<<"nPad : "<<tmpnt.nPad<<endl;

	this->NetArray = new ClusterNet*[tmpnt.GetnNet()];
	this->ModArray = new ClusterMod*[tmpnt.GetnMod()+ tmpnt.GetnPad()];
	this->ClusterCount = tmpnt.GetnMod();//initialize

	this->nTSV = tmpnt.nTSV;

	for(int i=0; i<tmpnt.GetnNet() ; i++)
	{
		ClusterNet* n1= new ClusterNet;
		this->NetArray [i]= n1;
	}
	for(int i=0; i<tmpnt.nMod; i++)
	{
		ClusterMod* n1= new ClusterMod;
		n1->Area = tmpnt.mods.at(i).modArea;
		n1->layer = tmpnt.mods.at(i).modL;
		n1->CountMod =tmpnt.mods.at(i).ModuleCount;
		n1->layer = tmpnt.mods.at(i).modL;
		n1->IsTSV = tmpnt.mods.at(i).flagTSV;
		n1->Mods.push_back(tmpnt.mods[i].idMod);
		n1->ClusterIndex = tmpnt.mods[i].idMod;
		n1->IsMacro = tmpnt.mods.at(i).is_m;
		n1->IsEverClustered = tmpnt.mods.at(i).flagCluster; //0816
		if(n1->IsTSV)
			n1->nTSV = 1;

		this->ModArray[i]= n1;
		
	}

	int PinNet = 0;
	double min_power = 9999;

	for(int i =0 ; i <tmpnt.GetnMod(); i++)//find minimum power
	{
		if(min_power > tmpnt.mods[i].Power && tmpnt.mods[i].Power != 0)
			min_power = tmpnt.mods[i].Power ;
	}
	int ScaledTerm = 0;

	while( (int)min_power/1 == 0 )
	{
		min_power *= 10 ;
		ScaledTerm ++ ;
	}
	//cout<<" Net Number : "<< tmpnt.nNet;

	for(int i=0 ; i< tmpnt.nNet ; i++ )
	{

		//cout<<"Net "<<i<<endl;
		if(tmpnt.nets[i].flagAlign)
		{
            this->NetArray[i]->flagAlign = true;
        }
		if(tmpnt.nets[i].nPin>100)
		{
			continue;
		}
		for(int j= tmpnt.nets[i].head; j< tmpnt.nets[i].head+ tmpnt.nets[i].nPin ; j++)
		{
			int ModID = tmpnt.pins[j].index ;
			/*
			   if(ModID > tmpnt.nMod )
			   {
			   cout<<"This is Pin : "<< ModID<< endl;
			// getchar();
			}

			 */
			//net
			ClusterNet* n1= new ClusterNet;
			n1->ClusterIndex= ModID;
			n1->next=NULL;

			ClusterNet* n2 = this->NetArray[i];
			n2->NetIndex = i;
			n2->ClusterCount++;

			while(n2->next != NULL)
				n2=n2->next;
			n2->next = n1 ;


			//mod
			ClusterMod* n3 =  new ClusterMod;
			n3->NetIndex = i;


			ClusterMod* n4 = this->ModArray[ModID];
			if(n4->Mods.size()==0)
				n4->Mods.push_back(ModID);

			n4->NetCount++;

			if(ModID >= tmpnt.nMod) //Pin
			{
				n4->Area= 0 ;
				n4->power = 0 ;

			}
			else
			{
				//  cout<<"Mod Num : "<<tmpnt.nMod <<"  ModSize : "<<tmpnt.mods.size()<<endl;
				//   cout<<"- Mod ID "<< ModID <<endl;
				n4->Area = tmpnt.mods.at(ModID).modArea;
				n4->power = tmpnt.mods.at(ModID).Power;
				n4->CountMod =tmpnt.mods.at(ModID).ModuleCount;
				n4->AvgPower = (double) tmpnt.mods.at(ModID).Power / tmpnt.mods.at(ModID).ModuleCount ;
				n4->layer = tmpnt.mods.at(ModID).modL;
				n4->IsTSV = tmpnt.mods.at(ModID).flagTSV;
				if(n4->IsTSV)
                    n4->nTSV = 1;

				// n4->AvgPower =(double) tmpnt.mods.at(ModID).Power / tmpnt.mods.at(ModID).InClusterMod.size() ;

			}




			n4->ClusterIndex = ModID;

			while(n4->next != NULL)
				n4=n4->next;
			n4->next = n3;
		}

	}


	// cout<< " - Sclaed term "<< ScaledTerm<<endl;
	this -> ScaledTerm = ScaledTerm ;




	/*

	   ofstream fout("ClusetNet.txt");
	   for(int i=0; i <tmpnt.nNet; i++ )
	   {
	   fout<<"Net "<<i<<" : ModCount "<<NetArray[i]->ClusterCount<<endl;
	   ClusterNet* n1= NetArray[i];
	   if(n1->next!=NULL)
	   n1=n1->next;


	   while(n1!=NULL)
	   {


	   if(n1->ClusterIndex < tmpnt.nMod)
	   fout<< tmpnt.mods[n1->ClusterIndex].modName<<" ";
	   else
	   fout<< tmpnt.pads[n1->ClusterIndex-tmpnt.nMod].padName<<" ";
	   n1=n1->next;


	   }


	   fout<<endl;

	   }
	   fout.close();
	   fout.open("ClusterMod.txt");
	   for(int i= 0; i< tmpnt.nMod + tmpnt.nPad; i++)
	   {
	   ClusterMod* n1= ModArray[i];
	   if(i<tmpnt.nMod)
	   {
	   fout<<"Mod "<< tmpnt.mods[n1->ClusterIndex].modName <<": Netcount  "<< n1->NetCount<<endl;
	   fout<<"Area "<< n1->Area<<" Mod num in Cluster "<< n1->Mods.size()<<endl;
	   fout<<"Net: "<<endl;
	   if(n1->next != NULL)
	   n1=n1->next;
	   while(n1!=NULL)
	   {
	   fout<<n1->NetIndex<<"  ";
	   n1=n1->next;
	   }
	   fout<<endl;


	   }
	   else
	   {

	   fout<<"Pad "<< tmpnt.pads[n1->ClusterIndex - tmpnt.nMod].padName <<": Netcount  "<< n1->NetCount<<endl;
	   fout<<"Area "<< n1->Area<<" Mod num in Cluster "<< n1->Mods.size()<<endl;
	   fout<<"Net: "<<endl;
	   if(n1->next != NULL)
	   n1=n1->next;
	   while(n1!=NULL)
	   {
	   fout<<n1->NetIndex<<"  ";
	   n1=n1->next;
	   }
	   fout<<endl;


	   }

	   }
	 */

	//cout<<"END NetList "<<endl;

}

void CLUSTER::TransferOutput(NETLIST& tmpnt , vector <NETLIST > & ClusteredNET)
{

	//New Netlist
	// cout<<"Start Transfer Output.."<<endl;
	NETLIST temp;
	temp.aR = tmpnt.aR;
	temp.ChipArea = tmpnt.ChipArea;
	temp.ChipHeight = tmpnt.ChipHeight;
	temp.ChipWidth = tmpnt.ChipWidth;

	temp.FenceArea = tmpnt.FenceArea;
	temp.maxWS = tmpnt.maxWS;
	temp.nVI = tmpnt.nVI;
	temp.nLayer = tmpnt.nLayer;
	//temp.nTSV = tmpnt.nTSV;
	temp.totalModArea = tmpnt.totalModArea;
	temp.totalWL =tmpnt.totalWL;
	temp.totalWL_GP =  tmpnt.totalWL_GP;
	//adjust
	temp.nHard = 0; // cluster is see as soft
	temp.nSoft = this->ClusterCount - tmpnt.nPad;

	temp.nMod = this->ClusterCount;
	temp.nPad = tmpnt.nPad;



	temp.pads.resize( tmpnt.nPad );  //pad number will not change

	//Mods & Pad info (block file)
	map <int,int> OGIndex_To_NewIndex;

	// cout<<"Mod "<<endl;
	ClusterMod** ModArray = this->ModArray;

	int ModCount = 0;
	int PadCount = 0;
	int TotalCount = 0;
	int TSVCount = 0;
	cout<<"Incluster:--------------\n";
	for(int i=0; i<tmpnt.nMod + tmpnt.nPad ; i++)
	{
		ClusterMod* ModNode = ModArray[i];
		if(!ModNode->InCluster) // This not be cluster
		{
			if(i< tmpnt.nMod)
			{
				MODULE modTemp;
				modTemp = tmpnt.mods[i];
				modTemp.type= SOFT_BLK;

				modTemp.modArea = ModNode->Area;
				modTemp.idMod = ModCount;
				modTemp.modH = modTemp.modW  =(int)(sqrt(ModNode->Area) ); //Because we see the Cluster as Soft Item
				
				modTemp.Pdensity = (double) modTemp.Power / modTemp.modArea;
				modTemp.InClusterMod = ModNode->Mods; // the mod id is the last level cell id not this level
				modTemp.ModuleCount = ModNode->CountMod;
				modTemp.is_m = ModNode->IsMacro; // 0810 HYH
				modTemp.flagCluster = ModNode->IsEverClustered;//0816
				if(ModNode->nTSV>0)
				{
                    modTemp.flagTSV == true;
                    TSVCount++;
                }
				temp.mod_NameToID [ modTemp.modName ]= ModCount;
				temp.mods.push_back( modTemp );

				OGIndex_To_NewIndex[ i ] = ModCount;

				ModCount++;
			}
			else //pad
			{
				temp.pad_NameToID[ tmpnt.pads[i - tmpnt.nMod].padName  ] = PadCount;

				OGIndex_To_NewIndex[ i ] = TotalCount;

				PadCount++;
			}
			TotalCount++;
		}
		else{
			MODULE modTemp = tmpnt.mods[i];
			cout<<modTemp.modName<<", ";
		}
	}
	cout<<"\n------------------------------\n";
	temp.nTSV = TSVCount;
	//cout << TSVCount << endl;

	//Set pl
	for(int i = 0; i<temp.nPad; i++)
	{
		temp.pads[i].x = tmpnt.pads[i].x;
		temp.pads[i].y = tmpnt.pads[i].y;
		temp.pads[i].padL = 0; // 2D
		temp.pads[i].idPad = i + temp.nMod;



	}



	//Caculate after the cluster remaining net
	ClusterNet** NetArray = this->NetArray;
	int NetCount = 0;
	for(int i=0 ; i < tmpnt.nNet ; i++)
	{
		int NetDegree = NetArray[i]->ClusterCount;
		if(NetDegree > 1 )
			NetCount++;
	}

	temp.nNet = NetCount;
	temp.nets.resize( NetCount );


	//  cout<<"Net "<<endl;

	NetCount=0;
	int TotalpinCount = 0;
	//Net info
	for(int i=0 ; i < tmpnt.nNet ; i++)
	{
		int NetDegree =NetArray[i]->ClusterCount;
		if(  NetDegree > 1 ) //exist
		{
			int PinCount =0;

			temp.nets[ NetCount].idNet = NetCount;
			temp.nets[ NetCount].head = TotalpinCount;
			temp.nets[ NetCount].nTSV = 0;

			ClusterNet* NetNode = NetArray[i];

			if(NetNode->next != NULL)
				NetNode=NetNode->next;
			else
			{
				cout<<"ERROR: Netdegree wrong "<<endl;
				getchar();

			}
			while(NetNode!=NULL)
			{
				map<int,int >::iterator it1 ;
				int ClusterIndex = NetNode->ClusterIndex;
				it1 = OGIndex_To_NewIndex.find( ClusterIndex );
				if(it1!= OGIndex_To_NewIndex.end())
				{
					ClusterIndex = it1->second;
				}
				else
				{
					cerr<<"Can't find index transfer .."<<endl;
					exit(1);
				}

				PIN pinTemp ;
				pinTemp.idPin = TotalpinCount;
				if(ClusterIndex < temp.nMod) // mod
				{
					pinTemp.pinL = temp.mods[ ClusterIndex ].modL;
					pinTemp.index = ClusterIndex;

					temp.mods[ ClusterIndex  ].nNet++;
				}
				else  //pad
				{
					pinTemp.pinL = temp.pads[ ClusterIndex ].padL;
					pinTemp.index = ClusterIndex;

				}

				// cout<<"Cluster index "<<ClusterIndex<<endl;

				PinCount++;
				TotalpinCount++;
				temp.pins.push_back(pinTemp);
				NetNode=NetNode->next;
			}

			temp.nets[NetCount].nPin = PinCount;
			NetCount++;
		}
	}

	temp.nPin= TotalpinCount;



	cout<<temp<<endl;
	
	ClusteredNET.push_back(temp);
}

void CLUSTER::TransferOutput3D(NETLIST& tmpnt , vector <NETLIST > & ClusteredNET)
{

	//New Netlist
	// cout<<"Start Transfer Output.."<<endl;
	NETLIST temp;
	temp.aR = tmpnt.aR;
	temp.ChipArea = tmpnt.ChipArea;
	temp.ChipHeight = tmpnt.ChipHeight;
	temp.ChipWidth = tmpnt.ChipWidth;

	temp.FenceArea = tmpnt.FenceArea;
	temp.maxWS = tmpnt.maxWS;
	temp.nVI = tmpnt.nVI;
	temp.nLayer = tmpnt.nLayer;
	//temp.nTSV = tmpnt.nTSV;
	temp.totalModArea = tmpnt.totalModArea;
	temp.totalWL =tmpnt.totalWL;
	temp.totalWL_GP =  tmpnt.totalWL_GP;
	//adjust
	temp.nHard = 0; // cluster is see as soft
	temp.nSoft = this->ClusterCount;

	temp.nMod = this->ClusterCount;
	temp.nPad = tmpnt.nPad;



	temp.pads.resize( tmpnt.nPad );  //pad number will not change

	//Mods & Pad info (block file)
	map <int,int> OGIndex_To_NewIndex;

	// cout<<"Mod "<<endl;
	ClusterMod** ModArray = this->ModArray;

	int ModCount = 0;
	int PadCount = 0;
	int TsvCount = 0;
	int TotalCount = 0;
	//cout<<"--------------------"<<ClusteredNET.size()-1<<"----------------------\n";
	for(int i=0; i<tmpnt.nMod ; i++)
	{
		ClusterMod* ModNode = ModArray[i];
		if(!ModNode->InCluster) // This not be cluster
		{
			if(i< tmpnt.nMod)
			{
				
				MODULE modTemp;
				modTemp = tmpnt.mods[i];
				modTemp.type= SOFT_BLK;
				//cout<<modTemp.modName;			
				//cout<<"("<<ModNode->IsTSV<<")\n";
				modTemp.modArea = ModNode->Area;
				modTemp.idMod = ModCount;
				if(!ModNode->IsMacro)//0810 HYH
				{
					modTemp.modH = modTemp.modW  =(int)(sqrt(ModNode->Area) * AMP_PARA); //Because we see the Cluster as Soft Item
				}
				if(modTemp.modH==0||modTemp.modW==0){
					cout<<"(out)i="<<i<<" area"<<modTemp.modArea<<"/"<<ModNode->Area<<" ";
					if(modTemp.modH==0){
						cout<<"modH=0 ";
					}
					if(modTemp.modW==0){
						cout<<"modW=0 ";
					}
					cout<<"\n";
				}
				modTemp.Power = ModNode->power;
				modTemp.Pdensity = (double) modTemp.Power / modTemp.modArea;
				modTemp.InClusterMod = ModNode->Mods; // the mod id is the last level cell id not this level
				modTemp.ModuleCount = ModNode->CountMod;
				modTemp.is_m = ModNode->IsMacro;//0810 HYH
				modTemp.flagCluster = ModNode->IsEverClustered; // 0816 HYH
				if(ModNode->IsTSV)
				{
                    modTemp.flagTSV = true;
					
                }
				else
				{
					modTemp.flagTSV = false;
				}
				if(!ModNode->IsEverClustered && ModNode->IsTSV)
                    TsvCount++;
				if(modTemp.modArea==0)
					cout<<"error: "<<modTemp.modName<<"with zerp area\n";
				modTemp.Power = ModNode->power;
				modTemp.nNet = 0;
				modTemp.pNet = new unsigned int[ModNode->NetCount];
				temp.mod_NameToID [ modTemp.modName ]= ModCount;
				
				temp.mods.push_back( modTemp );

				OGIndex_To_NewIndex[ i ] = ModCount;

				ModCount++;

			}
			else //pad
			{
				temp.pad_NameToID[ tmpnt.pads[i - tmpnt.nMod].padName  ] = PadCount;

				OGIndex_To_NewIndex[ i ] = TotalCount;

				PadCount++;
			}
			TotalCount++;
			
		}
		
	}
	//cout<<"\n----------------------------------------------\n";
    temp.nTSV = TsvCount;
	//cout <<"TSV:"<< TsvCount << endl;
	//-----------------------------------------------//

	//Set pl
	for(int i = 0; i<temp.nPad; i++)
	{
		temp.pads[i].x = tmpnt.pads[i].x;
		temp.pads[i].y = tmpnt.pads[i].y;
		temp.pads[i].padL = 0; // 2D
		temp.pads[i].idPad = i + temp.nMod;

	}



	//Caculate after the cluster remaining net
	ClusterNet** NetArray = this->NetArray;
	int NetCount = 0;
	for(int i=0 ; i < tmpnt.nNet ; i++)
	{
		int NetDegree = NetArray[i]->ClusterCount;
		if(NetDegree > 1 )
			NetCount++;
	}

	temp.nNet = NetCount;
	temp.nets.resize( NetCount );


	//  cout<<"Net "<<endl;

	NetCount=0;
	int TotalpinCount = 0;
	//Net info
	for(int i=0 ; i < tmpnt.nNet ; i++)
	{
		int NetDegree =NetArray[i]->ClusterCount;

		if( NetArray[i]->flagAlign )
		{
            temp.nets[NetCount].flagAlign = true;
		}

		if(  NetDegree > 1 ) //exist
		{
			int PinCount =0;
			bool flagTSV = false;
			int countTSV = 0;

			temp.nets[ NetCount].idNet = NetCount;
			temp.nets[ NetCount].head = TotalpinCount;


			ClusterNet* NetNode = NetArray[i];

			if(NetNode->next != NULL)
				NetNode=NetNode->next;
			else
			{
				cout<<"ERROR: Netdegree wrong "<<endl;
				getchar();

			}
			while(NetNode!=NULL)
			{
				map<int,int >::iterator it1 ;
				int ClusterIndex = NetNode->ClusterIndex;
				it1 = OGIndex_To_NewIndex.find( ClusterIndex );
				if(it1!= OGIndex_To_NewIndex.end())
				{
					ClusterIndex = it1->second;
				}
				else
				{
					cerr<<"Can't find index transfer .."<<endl;
					exit(1);
				}

				PIN pinTemp ;
				pinTemp.idPin = TotalpinCount;
				if(ClusterIndex < temp.nMod) // mod
				{
					pinTemp.pinL = temp.mods[ ClusterIndex ].modL;
					pinTemp.index = ClusterIndex;
					temp.mods[ClusterIndex].pNet[temp.mods[ClusterIndex].nNet] = NetCount;
					temp.mods[ClusterIndex].nNet++;
					//cout << ClusterIndex << " " << temp.mods[ ClusterIndex  ].modL << endl;

				}
				else  //pad
				{
					pinTemp.pinL = temp.pads[ ClusterIndex ].padL;
					pinTemp.index = ClusterIndex;

				}

                if(temp.mods[ ClusterIndex ].flagTSV == true)
                {
                    flagTSV=true;
                    countTSV++;
                }
				// cout<<"Cluster index "<<ClusterIndex<<endl;

				PinCount++;
				TotalpinCount++;
				temp.pins.push_back(pinTemp);
				NetNode=NetNode->next;
			}
			//cout<<"TSVCOUNT: " << countTSV<< endl;
			//getchar();
			if(flagTSV)
                temp.nets[ NetCount].nTSV = 1;
            else
                temp.nets[ NetCount].nTSV = 0;
			temp.nets[NetCount].nPin = PinCount;
			NetCount++;
		}
	}

	outClusterMod(temp, ClusteredNET.size());

	temp.nPin= TotalpinCount;
    //cout <<"here" << endl;
    //getchar();
	//cout<<temp<<endl;
	ClusteredNET.push_back(temp);
	//LayerNET = temp;
	//getchar();

}

void CLUSTER::DestroyArray(NETLIST& tmpnt )
{
	ClusterMod** ModArray =this->ModArray;
	ClusterNet** NetArray = this->NetArray;
	for(int i =0; i< tmpnt.nMod+tmpnt.nPad ; i++)
	{
		ClusterMod* trace = ModArray[i];

		while( trace->next!=NULL )
		{
			ClusterMod* NEXT ;
			if(trace->next == NULL)
				break;
			else
				NEXT = trace->next;
			delete trace;

			trace = NEXT;
		}
		delete trace;
	}
	for(int i=0; i<tmpnt.nNet; i++)
	{
		ClusterNet* trace = NetArray[i];
		while(trace->next != NULL)
		{
			ClusterNet* NEXT ;
			if(trace->next != NULL)
				break;
			else
				NEXT = trace->next;

			delete trace;
			trace= NEXT;
		}
		delete trace;
	}


	this->ClusterCount = 0;
	this->AvgArea  = 0 ;
	this->PowerDif = 0 ;
	this->ScaledTerm = 0;
	this->ScoreTable.clear();
	while( !(this->PQ_Score.empty()) )
		this->PQ_Score.pop();
	//cout<<" - Free Table "<<endl;
}


void CLUSTER::CheckerNetList ( NETLIST& tmpnt, NETLIST& Newtmpnt,int ClusterNum)
{
	//Orignal NetList
	ClusterNet**  NetArray = new ClusterNet*[tmpnt.nNet];
	ClusterMod**  ModArray = new ClusterMod*[tmpnt.nMod+ tmpnt.nPad];

	for(int i=0; i<tmpnt.nNet ; i++)
	{
		ClusterNet* n1= new ClusterNet;
		NetArray [i]= n1;
	}
	for(int i=0; i<tmpnt.nMod+tmpnt.nPad ; i++)
	{
		ClusterMod* n1= new ClusterMod;
		
		ModArray[i]= n1 ;
	}

	int PinNet = 0;
	for(int i=0 ; i< tmpnt.nNet ; i++ )
	{

		//cout<<"Net "<<i<<endl;
		for(int j= tmpnt.nets[i].head; j< tmpnt.nets[i].head+tmpnt.nets[i].nPin ; j++)
		{

			int ModID = tmpnt.pins[j].index ;
			//net
			ClusterNet* n1= new ClusterNet;
			n1->ClusterIndex= ModID;
			n1->next=NULL;

			ClusterNet* n2 = NetArray[i];
			n2->NetIndex = i;
			n2->ClusterCount++;

			while(n2->next != NULL)
				n2=n2->next;
			n2->next = n1 ;


			//mod
			ClusterMod* n3 =  new ClusterMod;
			n3->NetIndex = i;


			ClusterMod* n4 = ModArray[ModID];
			if(n4->Mods.size()==0)
				n4->Mods.push_back(ModID);
			n4->NetCount++;
			n4->Area=tmpnt.mods[ModID].modArea;
			n4->power = tmpnt.mods[ModID].Power;
			n4->ClusterIndex = ModID;

			while(n4->next != NULL)
				n4=n4->next;
			n4->next = n3;

		}

	}
	// New NetList
	/*
	   cout<<"Create end"<<endl;
	   ofstream fout("./ThermalMap/NewNetList.txt");
	   for(int i=0;i<Newtmpnt.nMod+Newtmpnt.nPad;i++)
	   {
	   fout<<"Mod :"<<Newtmpnt.mods[i].modName<<endl;
	   ClusterMod* n1 = TestArray[i];
	   fout<<"Area : "<<n1->Area<<" ModDegree "<<n1->NetCount<<endl;
	   fout<<"Clusted Mod"<<endl;
	   for(int j=0;j<n1->Mods.size();j++)
	   {
	   fout<<n1->Mods[j]<<"   ";
	   }
	   fout<<endl;
	   int NetCount = 0;
	   fout<<"Connent Net : "<<endl;
	   while(n1!=NULL)
	   {
	   n1->NetIndex;
	   fout<<  n1->NetIndex<<"  ";
	   NetCount++;
	   n1=n1->next;
	   }
	   fout<<endl;
	   if(NetCount-1 != TestArray[i]->NetCount)
	   {
	   cout<<"Warning : the netcount has problem.."<<endl;
	   }
	   }
	   fout<<"================Netlist======================="<<endl;
	   for(int i=0;i<Newtmpnt.nNet;i++)
	   {
	   fout<<"Net "<<i<<endl;
	   ClusterNet* n1 = TestNetArray[i];
	   fout<<"NetDegree : "<<n1->ClusterCount<<endl;
	   int NetCount = 0;
	   while(n1!=NULL)
	   {
	   NetCount++;
	   fout<<n1->ClusterIndex <<"  ";
	   n1=n1->next;
	   }
	   fout<<endl;
	   if(NetCount-1 != TestNetArray[i]->ClusterCount)
	   {
	   cout<<"Waringin : the Netarray has problem "<<endl;
	   }

	   }
	   fout.close();
	   fout.open("./ThermalMap/OldNetlist.txt");

	   for(int i=0;i<tmpnt.nMod+tmpnt.nPad;i++)
	   {
	   fout<<"Mod :"<<tmpnt.mods[i].modName<<endl;
	   ClusterMod* n1 = ModArray[i];
	   fout<<"Area : "<<n1->Area<<" ModDegree "<<n1->NetCount<<endl;
	   fout<<"Clusted Mod"<<endl;
	   for(int j=0;j<n1->Mods.size();j++)
	   {
	   fout<<n1->Mods[j]<<"   ";
	   }
	   fout<<endl;
	   int NetCount = 0;
	   fout<<"Connent Net : "<<endl;
	   while(n1!=NULL)
	   {
	   n1->NetIndex;
	   fout<<  n1->NetIndex<<"  ";
	   NetCount++;
	   n1=n1->next;
	}
	fout<<endl;
	if(NetCount-1 != ModArray[i]->NetCount)
	{
		cout<<"Warning : the netcount has problem.."<<endl;
	}
	}
	fout<<"================Netlist======================="<<endl;
	for(int i=0;i<tmpnt.nNet;i++)
	{
		fout<<"Net "<<i<<endl;
		ClusterNet* n1 = NetArray[i];
		fout<<"NetDegree : "<<n1->ClusterCount<<endl;
		int NetCount = 0;
		while(n1!=NULL)
		{
			NetCount++;
			fout<<n1->ClusterIndex <<"  ";
			n1=n1->next;
		}
		fout<<endl;
		if(NetCount-1 != NetArray[i]->ClusterCount)
		{
			cout<<"Waringin : the Netarray has problem "<<endl;
		}

	}
	*/
	ClusterMod** TestArray = this->ModArray;
	ClusterNet** TestNetArray = this->NetArray;


	vector < map<int, TUPLE>  > Checker;
	Checker.resize(tmpnt.nMod);
	int ClusterCount = 0;

	for(int i=0; i<tmpnt.nMod; i++)
	{
		if(!TestArray[i]->InCluster)
		{
			ClusterMod* node = TestArray[i];
			node=node->next;

			while(node!=NULL)
			{
				int NetId = node->NetIndex;
				int NetDegree = TestNetArray[NetId]->ClusterCount;
				ClusterNet* NetNode = TestNetArray[NetId];



				if(NetDegree > 1)
				{
					NetNode=NetNode->next;
					while(NetNode!=NULL)
					{
						int CLusterIndex = NetNode->ClusterIndex ;

						ClusterMod* trace = TestArray[CLusterIndex];

						if(!trace->InCluster && CLusterIndex!= i )
						{
							map <int, TUPLE>::iterator it1;

							it1= Checker[i].find( CLusterIndex );
							if(it1!= Checker[i].end())
							{
								set<int >::iterator it2;

								it2= it1->second.Net.find(  NetId );
								if(it2!= it1->second.Net.end())
								{
									cerr << "Error : This pair is already exist "<<endl;
									exit(1);
								}
								else
								{
									it1->second.Net.insert( NetId );

								}

							}
							else
							{
								TUPLE temp;
								temp.Net.insert(NetId);
								Checker[i].insert(pair <int, TUPLE> ( CLusterIndex, temp  ));
							}

						}

						NetNode=NetNode->next;
					}

				}
				node=node->next;
			}

			ClusterCount++;
		}

	}

	/*      ofstream fout1("./ThermalMap/ScoreCheck.txt");
			for(int i =0; i<tmpnt.nMod; i++)
			{
			map <int , TUPLE>::iterator it1;
			fout1<<"Mod : "<<tmpnt.mods[i].modName<<endl;
			fout1<<"Neighbor "<<endl;
			for(it1 = Checker[i].begin() ;  it1!= Checker[i].end(); it1++)
			{
			fout1<<"\tIndex "<<it1->first<<endl;
			fout1<<"\tConnect Net "<<endl;
			set<int >::iterator it2;
			fout1<<"\t\t";
			for(it2= it1->second.Net.begin(); it2!= it1->second.Net.end(); it2++)
			{
			fout1<<(*it2)<<"  ";
			}
			fout1<<endl;

			}

			}
	*/

	/*
	ofstream fout2("./ThermalMap/ClusterCell.txt");

	for(int i =0 ; i<tmpnt.nMod; i++)
	{
	if(!TestArray[i]->InCluster)
	{
	fout2<<"Mod "<<i<<endl;
	fout2<<"\tCell ";
	for(int j=0; j<TestArray[i]->Mods.size(); j++)
	{
	fout2<<TestArray[i]->Mods[j]<<"  ";

	}
	fout2<<endl;
	}
	}
	*/
	/*
	ofstream fout3("./ThermalMap/Netlist_OG.txt");

	for(int i =0 ;i < tmpnt.nMod + tmpnt.nPad ; i++)
	{
	ClusterMod* modNode = tmpclu.ModArray[i];
	if(!modNode->InCluster)
	{
	fout3<<"Mod "<<i<<endl;
	modNode=modNode->next;
	while(modNode!=NULL)
	{
	fout3<<modNode->NetIndex<<"  ";
	modNode=modNode->next;
	}
	fout3<<endl;

	}

	}
	for(int i=0;i<tmpnt.nNet;i++)
	{
	ClusterNet* NetNode = tmpclu.NetArray[i];
	NetNode=NetNode->next;
	fout3<<"Net : "<<i<<endl;
	while(NetNode!=NULL)
	{
	fout3<<NetNode->ClusterIndex<<" ";
	NetNode = NetNode->next;
	}
	fout3<<endl;


	}
	*/


	//Start Check
	cout<<"Start Check Netlist..."<<endl;
	for(int i =0; i<tmpnt.nMod ; i++)
	{
		map <int, TUPLE>::iterator it1;
		for(it1 = Checker[i].begin(); it1!=Checker[i].end(); it1++ )
		{
			int ClusterIndex = it1->first;
			set<int >::iterator it2;
			int NetCount = it1->second.Net.size();
			int CheckNetCount =0;

			vector <int > Group_1_Mods = TestArray[i]->Mods;
			vector <int > Group_2_Mods = TestArray[ClusterIndex]->Mods;


			cout<<"Group 1 : "<<i<<endl;
			cout<<"Group 2 : "<<ClusterIndex<<endl;

			set <int > Cluster_2_mod;
			//Group 2 set
			for(int j=0; j< Group_2_Mods.size(); j++)
			{
				Cluster_2_mod.insert( Group_2_Mods[j]  );

			}
			//Group 1 set
			set <int > OneCountNet;  //just one count for one net

			for(int j=0; j<Group_1_Mods.size(); j++)
			{
				int ModIndex = Group_1_Mods[j];
				cout<<"Check: mod "<<ModIndex<<" in Cluster "<<i <<endl;

				//check last level net
				ClusterMod* n1 = ModArray[ModIndex];
				n1=n1->next;



				while(n1!=NULL)
				{
					set<int >::iterator it3;

					ClusterNet* n2 = NetArray[n1->NetIndex];

					n2=n2->next;

					while(n2!=NULL)
					{
						it3 = Cluster_2_mod.find( n2->ClusterIndex );
						if(it3!=Cluster_2_mod.end()) //Yes this in the group 2
						{
							cout<<"Net "<<n1->NetIndex<<" Conntect "<< ModIndex <<" and "<< n2->ClusterIndex<<endl;
							cout<<"Yes: "<<n2->ClusterIndex<<" is in the Group 2"<<endl;

							set<int >::iterator it4;
							it4 = it1->second.Net.find( n1->NetIndex  );
							if(it4!=it1->second.Net.end())
							{

							}
							else
							{
								cout<<"Net "<< n1->NetIndex<<endl;
								cout<<"Can't find "<<n1->NetIndex<<" between "<< ModIndex <<" "<< n2->ClusterIndex<<endl;
								getchar();

							}

							set <int > ::iterator it5;
							it5= OneCountNet.find(n1->NetIndex);
							if(it5 != OneCountNet.end() )
							{


							}
							else
							{
								CheckNetCount++;
								cout<<"NetCount  +  1"<<endl<<endl;
								OneCountNet.insert(n1->NetIndex);
							}

						}
						else
						{


						}

						n2=n2->next;
					}


					n1=n1->next;
				}


			}

			if(NetCount!=CheckNetCount)
			{
				cout<<"NetCount error.."<<endl;
				cout<<"Check "<<CheckNetCount<<"  net count "<<NetCount<<endl;
				getchar();

			}
			cout<<"-------------------------"<<endl;
		}


	}



	cout<<"[ Checker pass !! ]"<<endl;


}
int CLUSTER::ThermalCluster( NETLIST& tmpnt, vector <NETLIST> &ClusteredNET)
{

	cout<< "Cluster .. "<<endl;


	//cout <<" - Create Netlist .."<<endl;
	CLUSTER::CreateNetlist(tmpnt);
	this->ScoreTable.resize(this->ClusterCount);

	// cout<< " - PQ "<<endl;
	InitializationPQ(tmpnt);

	int ClusterNum = CLUSTER::Clustering(tmpnt );

	CLUSTER::TransferOutput( tmpnt , ClusteredNET);

	//CLUSTER::CheckerNetList(ClusteredNET[0], ClusteredNET[ ClusteredNET.size()-1 ] ,ClusterNum);

	//ShowNetList(tmpnt,tmpclu);  //show the new netlist table

	CLUSTER::DestroyArray(tmpnt);

	return ClusterNum;

}

vector<int> CLUSTER::ThermalCluster3D( NETLIST& tmpnt, vector <NETLIST> &ClusteredNET)
{

	cout<< "Cluster .. "<<endl;

	this->score_fcn_call_time = 0;

	//cout <<" - Create Netlist .."<<endl;
	CLUSTER::CreateNetlist(tmpnt);
	this->ScoreTable.resize(this->ClusterCount);
    //cout << tmpnt.GetLayernMod(layer) << " " << this->ClusterCount << endl;
    //getchar();
    //cout<< " - PQ "<<endl;
	InitializationPQ(tmpnt);
	//cout<<"score_fcn call_time(after PQ): "<<this->score_fcn_call_time<<endl;
	vector<int> ClusterNumArray = CLUSTER::Clustering3D(tmpnt);
	//cout<<"score_fcn call_time(after clustering): "<<this->score_fcn_call_time<<endl;
	CLUSTER::TransferOutput3D( tmpnt , ClusteredNET);
	//CLUSTER::CheckerNetList(ClusteredNET[0], ClusteredNET[ ClusteredNET.size()-1 ] ,ClusterNum);

	//ShowNetList(tmpnt,tmpclu);  //show the new netlist table

	CLUSTER::DestroyArray(tmpnt);

	return ClusterNumArray;

}

int CLUSTER::Clustermain(NETLIST &OGtmpnt, vector <NETLIST> & ClusteredNET)
{

	cout<<BOLD(Cyan("\\ ====== Clustering ====== //"))<< endl;


	int ClusterNum = OGtmpnt.GetnMod();
	int TargetNum = ClusterNum / InputOption.TargetClusterNum ;

	int Updatenum = ClusterNum ;
	vector <int > Record ;
	Record.push_back(OGtmpnt.GetnMod() );
	cout<<" ---- Cluster Num ----"<<endl;

	while( TargetNum < Updatenum )
	{
		int index = ClusteredNET.size() -1 ;
		NETLIST tmpnt = ClusteredNET[index];

		Updatenum = CLUSTER::ThermalCluster(tmpnt, ClusteredNET);


		Record.push_back(Updatenum);
		int RecordIndex = (int) Record.size() -1 ;


		if( Record[RecordIndex] == Record[RecordIndex-1] )
		{
			cerr << "[Warning] : The cluster area constraint is too strict"<<endl;
			cerr << "[Warning] : Try to increase the area term"<<endl;
			exit(1);
		}
		cout<<setw(5 )<< Updatenum <<endl;
	}



	cout<<" - Final Cluster num : "<<Updatenum<<endl;
	cout<<" - Total Cluster Level "<< ClusteredNET.size()<<endl;
	cout<<" [ END CLUSTER ]"<<endl;
	// getchar();
	return 0;
}
int CLUSTER::Clustermain3D(NETLIST &OGtmpnt, vector <NETLIST> & ClusteredNET)
{
	clock_t aa = clock();

	cout<<BOLD(Cyan("\\\\ ====== Clustering 3D ====== //"))<< endl;

    vector < vector< int > > Record ;
    vector< int > TargetNumArray;
    vector< int > UpdateNumArray;
    bool endFlag = false;
	

    TargetNumArray.resize( OGtmpnt.nLayer );
    UpdateNumArray.resize( OGtmpnt.nLayer );
    Record.resize( 1 );

    for(int i=0; i< OGtmpnt.nLayer; i++)
    {
        Record[0].push_back( OGtmpnt.GetLayernMod(i) );

        int ClusterNum = OGtmpnt.GetLayernMod(i);
        TargetNumArray[i] = ClusterNum / InputOption.TargetClusterNum;//20000;
        UpdateNumArray[i] = ClusterNum;
        cout << "Target number in layer " << i << " : " <<  TargetNumArray[i] << endl;
    }
	
    cout<<" ---- Cluster Num ----"<<endl;
    while( !endFlag )
    {
		
        int index = ClusteredNET.size() -1 ;
        NETLIST tmpnt = ClusteredNET[index];
		cout<<"TSV initail numbers: "<< tmpnt.nTSV << endl;
        //clustering iteration for each layer
        //cout <<OGtmpnt.nLayer << endl;

        //cout << "here" << endl;
        NETLIST Ltmpnt;
		clock_t a =clock();
        UpdateNumArray = CLUSTER::ThermalCluster3D(tmpnt, ClusteredNET);
        Record.push_back(UpdateNumArray);
        int RecordIndex = (int) Record.size() -1 ;

        for(int i=0; i< OGtmpnt.nLayer; i++)
        {
            if( Record[RecordIndex][i] == Record[RecordIndex-1][i] && Record[RecordIndex][i]>TargetNumArray[i])
            {
                cerr << "[Warning] : The cluster area constraint of layer " << i << " is too strict"<<endl;
                cerr << "[Warning] : Target number is " << TargetNumArray[i] << endl;
                cerr << "[Warning] : Try to increase the area term"<<endl;
                exit(1);
            }
            //cout<<" - Current layer : "<<OGtmpnt.LayernMod[i]<< endl;
            //cout<<" - Update Number : "<<UpdateNumArray[i] <<endl;
        }
		clock_t b =clock();
		cout<< " runtime: "<<float(b-a)/CLOCKS_PER_SEC<<endl;

        ///ending condition
        int endcount = 0;
        for(int i=0; i< OGtmpnt.nLayer; i++)
        {
            if(TargetNumArray[i] >= UpdateNumArray[i])
                endcount++;
        }
        if(endcount == OGtmpnt.nLayer)
            endFlag = true;

    }

    cout<<" - Final Cluster num : \n";
    for(int i=0; i< OGtmpnt.nLayer; i++)
        cout <<setw(10 )<< UpdateNumArray[i] <<endl;

    cout<<" - Total Cluster Level "<< ClusteredNET.size()<<endl;
	clock_t bb =clock();
	cout<< "RunTime: "<<float(bb-aa)/CLOCKS_PER_SEC<<endl;
    cout<<" [ END CLUSTER ]"<<endl;


	return 0;
}

/*int CLUSTER::Clustermain3D(NETLIST &OGtmpnt, vector <NETLIST> & ClusteredNET)
{

	cout<<BOLD(Cyan("\\ ====== Clustering 3D ====== //"))<< endl;

    vector < vector< int > > Record ;
    vector< int > TargetNumArray;
    vector< int > UpdateNumArray;
    bool endFlag = false;

    TargetNumArray.resize( OGtmpnt.nLayer );
    UpdateNumArray.resize( OGtmpnt.nLayer );
    Record.resize( OGtmpnt.nLayer );

    for(int i=0; i< OGtmpnt.nLayer; i++)
    {
        Record[i].push_back( OGtmpnt.GetLayernMod(i) );

        int ClusterNum = OGtmpnt.GetLayernMod(i);
        TargetNumArray[i] = ClusterNum / InputOption.TargetClusterNum ;
        UpdateNumArray[i] = ClusterNum;
    }

    cout<<" ---- Cluster Num ----"<<endl;
    while( !endFlag )
    {
        int index = ClusteredNET.size() -1 ;
        NETLIST tmpnt = ClusteredNET[index];

        //clustering iteration for each layer
        cout <<OGtmpnt.nLayer << endl;
        for(int i=0; i< OGtmpnt.nLayer; i++)
        {
            //cout << "here" << endl;
            NETLIST Ltmpnt;

            UpdateNumArray[i] = CLUSTER::ThermalCluster3D(tmpnt, Ltmpnt, i);
            Record[i].push_back(UpdateNumArray[i]);
            int RecordIndex = (int) Record[i].size() -1 ;

            cout << Record[i][RecordIndex] << " " << Record[i][RecordIndex-1] << endl;
            if( Record[i][RecordIndex] == Record[i][RecordIndex-1] && Record[i][RecordIndex]>TargetNumArray[i])
            {
                cerr << "[Warning] : The cluster area constraint of layer " << i << " is too strict"<<endl;
                cerr << "[Warning] : Try to increase the area term"<<endl;
                exit(1);
            }

            cout<<" - Current layer : "<<OGtmpnt.LayernMod[i]<< endl;
            cout<<" - Update Number : "<<UpdateNumArray[i] <<endl;

            tmpnt = Ltmpnt;

        }
        ClusteredNET.push_back(tmpnt);

        ///ending condition
        int endcount = 0;
        for(int i=0; i< OGtmpnt.nLayer; i++)
        {
            if(TargetNumArray[i] >= UpdateNumArray[i])
                endcount++;
        }
        if(endcount == OGtmpnt.nLayer)
            endFlag = true;

    }

    cout<<" - Final Cluster num : ";
    for(int i=0; i< OGtmpnt.nLayer; i++)
        cout <<setw(10 )<< UpdateNumArray[i] <<endl;

    cout<<" - Total Cluster Level "<< ClusteredNET.size()<<endl;
    cout<<" [ END CLUSTER ]"<<endl;

	return 0;
}
*/
void CLUSTER::DeCluster (NETLIST &tmpnt,int level,vector<NETLIST>& ClusteredNET )
{
	/*
	//set coordinate
	for(int i = 0 ; i<  ClusteredNET[level].nMod + ClusteredNET[level].nPad; i++)
	{
	if(i <ClusteredNET[level].nMod )
	{
	if( ClusteredNET[level].mods[i].GCenterX ==0 &&   ClusteredNET[level].mods[i].GCenterY == 0 )
	{
	cout<<" Mod : "<<i <<" Unset Coordinate "<<endl;
	}

	}
	else
	{
	if( ClusteredNET[level].pads.at(i - ClusteredNET[level].nMod ).x ==0 &&   ClusteredNET[level].pads[i - ClusteredNET[level].nMod ].y == 0 )
	{
	cout<<" Pad : "<<i <<" Unset Coordinate "<<endl;
	}

	}


	}
	 */

	for(int i = 0 ; i< tmpnt.nMod ; i++ )
	{
		int Size = tmpnt.mods[i].InClusterMod.size();
		vector <int > Mods = tmpnt.mods[i].InClusterMod;
		for(int j=0; j<Size; j++)
		{
			int ModIndex = Mods[j];

			if(ModIndex >= ClusteredNET[level].nMod)
			{
				cerr<<"Exceed the mod array "<<endl;
				exit(1);
			}
			string s = ClusteredNET[level].mods[ModIndex].modName;

			ClusteredNET[level].mods[ModIndex].GCenterX = tmpnt.mods[i].GCenterX;
			ClusteredNET[level].mods[ModIndex].GCenterY = tmpnt.mods[i].GCenterY;
			ClusteredNET[level].mods[ModIndex].GLeftX = ClusteredNET[level].mods[ModIndex].GCenterX - 0.5 * ClusteredNET[level].mods[ModIndex].modW;
			ClusteredNET[level].mods[ModIndex].GLeftY = ClusteredNET[level].mods[ModIndex].GCenterY - 0.5 * ClusteredNET[level].mods[ModIndex].modH;
			ClusteredNET[level].mods[ModIndex].LeftX = ClusteredNET[level].mods[ModIndex].GLeftX;
			ClusteredNET[level].mods[ModIndex].LeftY = ClusteredNET[level].mods[ModIndex].GLeftY;
			ClusteredNET[level].mods[ModIndex].CenterX = ClusteredNET[level].mods[ModIndex].GCenterX;
			ClusteredNET[level].mods[ModIndex].CenterY = ClusteredNET[level].mods[ModIndex].GCenterY;
		}
	}

	/*
	   for(int i=0; i< ClusteredNET[level].nMod ; i++)
	   {
	   cout<<"mod : " << ClusteredNET[level].mods[i].modName <<" x "<< ClusteredNET[level].mods[i].GCenterX<<" y "<< ClusteredNET[level].mods[i].GCenterY <<endl;

	   }
	 */

	// Check whether Any Modules do not have coordinate
	for(int i = 0 ; i<ClusteredNET[level].nMod ; i++)
	{
		if(i < ClusteredNET[level].nMod )
		{
			if( ClusteredNET[level].mods[i].GCenterX ==0 &&   ClusteredNET[level].mods[i].GCenterY == 0 )
			{
				//cerr << " [DeCluster Error ]: " << ClusteredNET[level].mods[i].modName << "  Unset Coordinate " << endl;
				//exit(1);
				cout << " [DeCluster Error ]: " << ClusteredNET[level].mods[i].modName << "  Unset Coordinate " << endl;
				
			}

		}

	}

	cout<<"[ End Decluster ] "<<endl;



}

void CLUSTER::ShowNetList(NETLIST &tmpnt)
{
	ClusterMod** NodeArray = this->ModArray;
	ClusterNet** NetArray = this->NetArray;

	vector <int > Remain_cluster ;
	int ClusterIndex ;
	int TotalCluster = 0;



	for(int i =0; i< tmpnt.nMod + tmpnt.nPad ; i++)
	{
		ClusterMod* Node = NodeArray[i];
		if(!Node->InCluster)
		{
			Remain_cluster.push_back( i );
			if(i<tmpnt.nMod)
				ClusterIndex = i;
			TotalCluster++;
		}

	}
	ofstream fout("./ThermalMap/NetList.txt");
	fout<<"===============Netlist==============="<<endl;
	fout<<"pad : "<<tmpnt.nPad<<endl;
	fout<<"Mod : "<<TotalCluster<<endl;
	fout<<"Remaing Cluster "<<endl;
	for(int i=0; i< Remain_cluster.size(); i++)
	{
		int index =    Remain_cluster[i];
		if(index<tmpnt.nMod)
			fout<<" Mod "<< index<<endl;
		else
			fout<< index << "  ";

	}
	fout<<endl;
	fout<<"====================================="<<endl;
	for(int i=0; i<tmpnt.nNet ; i++)
	{
		ClusterNet* NetNode = NetArray[i];
		if(NetNode->ClusterCount > 1)
		{
			fout<<"Net : "<< i<<endl;
			NetNode=NetNode->next;
			while(NetNode!= NULL)
			{

				fout<<NetNode->ClusterIndex <<"  ";
				NetNode=NetNode->next;
			}
			fout<<endl;
		}

	}


}
void CLUSTER::outClusterMod(NETLIST &tmpnt, int level){
	ofstream fout;
	string fu="console/level"+to_string(level)+".txt";
	fout.open(fu.c_str());
	int macro_count = 0;
	int TSVcount =0;
	for(int i=0; i<tmpnt.mods.size(); i++)
	{	
		fout<<tmpnt.mods[i].modName;
		fout<<"(";
		if(tmpnt.mods[i].is_m)
		{
			fout<<"M";
			macro_count++;
		}
		else if(tmpnt.mods[i].flagTSV)
		{
			fout<<"T";
			TSVcount++;
		}
		else
		{
			fout<<"C";
		}
		if(tmpnt.mods[i].flagCluster)
			fout<<" c)";
		else
			fout<<" u)";
		fout<<" :"<< tmpnt.mods[i].InClusterMod.size()<<endl;
	}
	fout<<"macro:"<<macro_count<<"\nTSV:"<< TSVcount <<"\nnMods:"<<tmpnt.mods.size();
	fout<<"\n NETLIST: nTSV: "<< tmpnt.nTSV;
	fout.close();
}
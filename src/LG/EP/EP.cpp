#include "EP.h"

void EPLG:: EPlegal(NETLIST & tmpnt)
{
	// create sub-netlist for each layer
	CreateSubNetlist( tmpnt ); 

	// tempCurve declarations
	try
	{
		tempCurve[0].points = new POINT[MAX_CURVE_SIZE];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on tempCurve[0].points" << endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		tempCurve[1].points = new POINT[MAX_CURVE_SIZE];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on tempCurve[1].points" << endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		tempCurve[2].points = new POINT[MAX_CURVE_SIZE];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on tempCurve[2].points" << endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		tempCurve[3].points = new POINT[MAX_CURVE_SIZE];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on tempCurve[3].points" << endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		tempCurve[4].points = new POINT[MAX_CURVE_SIZE];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on tempCurve[4].points" << endl;
		exit(EXIT_FAILURE);
	}
	/// TSVs & SLOTs exist in (nt.nLayer-1) layers
	int numLayer_TSV = tmpnt.GetnLayer() - 1;

	/// node_TSV for network-flow
	vector< vector<NODE_TSV> > node_TSV( numLayer_TSV );

	/// NODE_SLOT for network-flow
	vector< vector<NODE_SLOT> > node_SLOT( numLayer_TSV );

	flagFinalResult = true ;
	
	string plotfile = InputOption.OutputPath + "EP";
	char* file = new char [plotfile.length()+1];
	strcpy(file, plotfile.c_str());

	cout << "******** Start Legalization *********" << endl;
	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		bool flagSuccess = false;

		if( i < numLayer_TSV )
		{
			node_TSV[i].resize( sub_nt[i].nTSV );

			flagSuccess = Legalization( sub_nt[i], InputOption.shConst, file , &node_TSV[i], &node_SLOT[i] );
			
		}
		else
		{
			flagSuccess = Legalization( sub_nt[i], InputOption.shConst, file, NULL, NULL );
		}

		if( !flagSuccess )
		{
			flagFinalResult = false;
			break;
		}

		// update netlist
		for( int j = 0; j < sub_nt[i].nMod; j++ )
		{
			int modID = tmpnt.mod_NameToID[ sub_nt[i].mods[j].modName ];
			tmpnt.mods[modID].CenterX = sub_nt[i].mods[j].CenterX;
			tmpnt.mods[modID].CenterY = sub_nt[i].mods[j].CenterY;
			tmpnt.mods[modID].LeftX = sub_nt[i].mods[j].LeftX;
			tmpnt.mods[modID].LeftY = sub_nt[i].mods[j].LeftY;
			tmpnt.mods[modID].modW = sub_nt[i].mods[j].modW;
			tmpnt.mods[modID].modH = sub_nt[i].mods[j].modH;

			// consider TSVs in layer k as pads in layer k+1
			if( sub_nt[i].mods[j].flagTSV && i < (numLayer-1) )
			{
				int padID = sub_nt[i+1].pad_NameToID[ sub_nt[i].mods[j].modName ];
				sub_nt[i+1].pads[padID].x = sub_nt[i].mods[j].CenterX;
				sub_nt[i+1].pads[padID].y = sub_nt[i].mods[j].CenterY;
			}
		}
	}
	
	cout << "*************** Done ****************" << endl;
	if( flagFinalResult )
	{
		CalculateHPWL_FINAL( tmpnt );
		
		wirelength_LG = tmpnt.totalWL;

		EPLG::TSV TSVinsert;
		TSVinsert.AssignTSV( node_TSV, node_SLOT, sub_nt, tmpnt );

		for( int i = 0; i < tmpnt.nLayer; i++ )
		{
			FindViolate( sub_nt[i] );
		}

		CalculateHPWL_FINAL( tmpnt );

		wirelength_TA = tmpnt.totalWL;

//		PlotResultL_3D( nt, plotFile );

		cout << "*Final Wirelength: " << tmpnt.totalWL << endl;

		cout << "*************************************" << endl;
		cout << "**                     ***         **" << endl;
		cout << "**                    ***          **" << endl;
		cout << "**                   ***           **" << endl;
		cout << "**           ***    ***            **" << endl;
		cout << "**            ***  ***             **" << endl;
		cout << "**             ******              **" << endl;
		cout << "**              ****               **" << endl;
		cout << "*************************************" << endl;
	}
	else
	{
		cout << Red(" EP Lealization Failed ")<<endl;
	}

	delete [] file;

}


bool EPLG::Legalization( NETLIST &sub_tmpnt, int shConst, char *plotFile, vector<NODE_TSV> *sub_nodeTSV, vector<NODE_SLOT> *sub_nodeSLOT )
{
	this->start_t_LG = clock();

	cout << "# Layer[" << sub_tmpnt.nLayer << "]" << endl;

	try
	{
		buffNet = new unsigned short [sub_tmpnt.nNet];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on buffNet" << endl;
		exit(EXIT_FAILURE);
	}

	numTree = 0;
	try
	{
		RGST = new TREE[3000];
	}
	catch( bad_alloc &bad )
	{
		cout << "error  : run out of memory on RGST" << endl;
		exit(EXIT_FAILURE);
	}

	vector<PSEUDO_MODULE> resultPM;
	NETLIST *pm_tmpnt = NULL;
	if( sub_tmpnt.nTSV == 0 )
	{
		if( flagModSoft )
			numBase = 8;
		else
			numBase = 10;
	}
	else if( flagModSoft )
	{
		numBase = 9;
	}
	else
	{
		numBase = 10;
		pm_tmpnt = new NETLIST;
	}

	//Group
	
	Grouping( resultPM, (*pm_tmpnt), sub_tmpnt, plotFile ); // Group.cpp


	if( pm_tmpnt == NULL )
	{
		pm_tmpnt = &sub_tmpnt;
	}

	SetModsInChip( (*pm_tmpnt) );

	CreateModCurve( (*pm_tmpnt), shConst );

	ConstructRGST( (*pm_tmpnt), RGST );

	try
	{
		pointerRGST = new TREE* [numTree];
	}
	catch(bad_alloc &bad)
	{
		cout << "error  : run out of memory on pointerRGST" << endl;
		exit(EXIT_FAILURE);
	}


	ReorderRGST( RGST, pointerRGST );

	bool flagValid = FindValidPoint( (*pm_tmpnt), RGST, pointerRGST, numTree );

	//int maxWidth = 0;
	//int maxHeight = 0;

	//for( int i = 0; i < (*pm_tmpnt).nMod; i++ )
	//{
	//	if( (*pm_tmpnt).mods[i].LeftX + (*pm_tmpnt).mods[i].modW > maxWidth )
	//		maxWidth = (*pm_tmpnt).mods[i].LeftX + (*pm_tmpnt).mods[i].modW;
	//	if( (*pm_tmpnt).mods[i].LeftY + (*pm_tmpnt).mods[i].modH > maxHeight )
	//		maxHeight = (*pm_tmpnt).mods[i].LeftY + (*pm_tmpnt).mods[i].modH;
	//}

	//int shiftX = ((*pm_tmpnt).ChipWidth - maxWidth)/2;
	//int shiftY = ((*pm_tmpnt).ChipHeight - maxHeight)/2;

	//if( shiftX < 0 ) shiftX = 0;
	//if( shiftY < 0 ) shiftY = 0;

	//for( int i = 0; i < (*pm_tmpnt).nMod; i++ )
	//{
	//	(*pm_tmpnt).mods[i].LeftX += shiftX;
	//	(*pm_tmpnt).mods[i].LeftY += shiftY;
	//	(*pm_tmpnt).mods[i].CenterX += shiftX;
	//	(*pm_tmpnt).mods[i].CenterY += shiftY;
	//}

	//PlotResultL( (*pm_tmpnt), plotFile );
	
	int count_TSV = 0;
	
	for( int i = 0; i < (*pm_tmpnt).nMod; i++ )
	{
		if( (*pm_tmpnt).mods[i].pseudoMod == NULL )
		{
			int modID = sub_tmpnt.mod_NameToID[ (*pm_tmpnt).mods[i].modName ];
			sub_tmpnt.mods[modID].CenterX = (*pm_tmpnt).mods[i].CenterX;
			sub_tmpnt.mods[modID].CenterY = (*pm_tmpnt).mods[i].CenterY;
			sub_tmpnt.mods[modID].LeftX = (*pm_tmpnt).mods[i].LeftX;
			sub_tmpnt.mods[modID].LeftY = (*pm_tmpnt).mods[i].LeftY;
			sub_tmpnt.mods[modID].modW = (*pm_tmpnt).mods[i].modW;
			sub_tmpnt.mods[modID].modH = (*pm_tmpnt).mods[i].modH;

			if( (*pm_tmpnt).mods[i].flagTSV )
			{
				NODE_SLOT slot;
				slot.id = (int)(*sub_nodeSLOT).size();
				slot.x = (*pm_tmpnt).mods[i].CenterX;
				slot.y = (*pm_tmpnt).mods[i].CenterY;
				(*sub_nodeSLOT).push_back( slot );

				(*sub_nodeTSV)[count_TSV].corr_id = modID;
				(*sub_nodeTSV)[count_TSV].corr_slots.push_back( slot.id );

				count_TSV++;
			}

			//if( (*pm_tmpnt).mods[i].flagTSV )
			//{
			//	SUPPLY temp;
			//	temp.x = (*pm_tmpnt).mods[i].CenterX;
			//	temp.y = (*pm_tmpnt).mods[i].CenterY;
			//	supplies[ (*pm_tmpnt).nLayer-1 ].push_back( temp );
			//}
		}
		else
		{
			int num_x = (int)( (*pm_tmpnt).mods[i].modW/(float)(TSV_SIZE * AMP_PARA) );
			int num_y = (int)( (*pm_tmpnt).mods[i].modH/(float)(TSV_SIZE * AMP_PARA) );

			int initial_x = (*pm_tmpnt).mods[i].LeftX + (TSV_SIZE * AMP_PARA / 2);
			int initial_y = (*pm_tmpnt).mods[i].LeftY + (TSV_SIZE * AMP_PARA / 2);
			
			vector<int> corr_slots( num_x * num_y, -1 );

			int count = 0;

			for( int j = 0; j < num_x; j++ )
			{
				for( int k = 0; k < num_y; k++ )
				{
					NODE_SLOT slot;
					slot.id = (int)(*sub_nodeSLOT).size();
					slot.x = initial_x + (j * TSV_SIZE * AMP_PARA);
					slot.y = initial_y + (k * TSV_SIZE * AMP_PARA);
					(*sub_nodeSLOT).push_back( slot );

					corr_slots[count] = slot.id;
					count++;
				}
			}

			PSEUDO_MODULE* pointPM = (*pm_tmpnt).mods[i].pseudoMod;
			for( int j = 0; j < (int)(*pointPM).idTSV.size(); j++ )
			{
				int modID = pointPM->idTSV[j];
				sub_tmpnt.mods[modID].CenterX = (*pm_tmpnt).mods[i].CenterX;
				sub_tmpnt.mods[modID].CenterY = (*pm_tmpnt).mods[i].CenterY;
				sub_tmpnt.mods[modID].LeftX = (*pm_tmpnt).mods[i].CenterX - (int)(TSV_SIZE * AMP_PARA * 0.5);
				sub_tmpnt.mods[modID].LeftY = (*pm_tmpnt).mods[i].CenterY - (int)(TSV_SIZE * AMP_PARA * 0.5);

				(*sub_nodeTSV)[count_TSV].corr_id = modID;
				(*sub_nodeTSV)[count_TSV].corr_slots = corr_slots;

				count_TSV++;
			}

			corr_slots.clear();
			vector<int> (corr_slots).swap(corr_slots);

			//int numW = (int)( (*pm_tmpnt).mods[i].modW/(float)(TSV_SIZE * AMP_PARA) );
			//int numH = (int)( (*pm_tmpnt).mods[i].modH/(float)(TSV_SIZE * AMP_PARA) );

			//int supplyX = (*pm_tmpnt).mods[i].LeftX + (TSV_SIZE * AMP_PARA / 2);
			//int supplyY = (*pm_tmpnt).mods[i].LeftY + (TSV_SIZE * AMP_PARA / 2);

			//for( int j = 0; j < numW; j++ )
			//{
			//	for( int k = 0; k < numH; k++ )
			//	{
			//		SUPPLY temp;
			//		temp.x = supplyX + (j * TSV_SIZE * AMP_PARA);
			//		temp.y = supplyY + (k * TSV_SIZE * AMP_PARA);
			//		supplies[ (*pm_tmpnt).nLayer-1 ].push_back( temp );
			//	}
			//}
		}
	}

	if( sub_tmpnt.nTSV != 0 )
	{
		EPLG::TSV TSVInsert;
		TSVInsert.FindAdditionalSlots( (*pm_tmpnt), (*sub_nodeSLOT) );
	}

	
	//if( sub_nodeTSV != NULL && sub_nodeSLOT != NULL )
	//{
	//	cout << count_TSV << endl;
	//	cout << sub_nodeTSV->size() << endl;
	//	cout << sub_nodeSLOT->size() << endl;

	//	for( int i = 0; i < (int)(*sub_nodeSLOT).size(); i++)
	//	{
	//		cout << (*sub_nodeSLOT)[i] << endl;

	//		getchar();
	//	}

	//	//for( int i = 0; i < (int)(*sub_nodeTSV).size(); i++ )
	//	//{
	//	//	cout << (*sub_nodeTSV)[i] << endl;

	//	//	getchar();
	//	//}
	//	getchar();
	//}

	//if( sub_tmpnt.nTSV != 0 )
	//{
	//	DetectWhitespace( (*pm_tmpnt) );
	//}

	delete [] RGST;
	delete [] subTree;
	delete [] buffNet;
	delete [] subCurve;
	delete [] buffCurve;
	delete [] pointerRGST;

	resultPM.clear();
	vector<PSEUDO_MODULE> (resultPM).swap(resultPM);

	end_t_LG = clock();

	time_LG += (end_t_LG - start_t_LG)/(double)CLOCKS_PER_SEC;

	return flagValid;
	
}
void EPLG::CreateSubNetlist( NETLIST &tmpnt )
{
	cout << "******** Create Sub-netlists ********" << endl;
	this->sub_nt.resize( tmpnt.nLayer );

	// copy general info to each sub-netlist
	for( int l = 0; l < tmpnt.nLayer; l++ )
	{
		this->sub_nt[l].ChipWidth  = tmpnt.ChipWidth;
		this->sub_nt[l].ChipHeight = tmpnt.ChipHeight;
		this->sub_nt[l].ChipArea   = tmpnt.ChipArea;
		this->sub_nt[l].maxWS      = tmpnt.maxWS;
		this->sub_nt[l].aR         = tmpnt.aR;
		this->sub_nt[l].nLayer     = l + 1;
	}

	// calculate nMod of each sub-netlist
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		this->sub_nt[ tmpnt.mods[i].modL ].nMod++;
	}

	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		int sub_modL  = tmpnt.mods[i].modL;
		int sub_modID = this->sub_nt[ sub_modL ].mods.size();

		// copy modules in layer k to sub-netlist[k] & update info of mods in sub-netlist[k]
		this->sub_nt[sub_modL].mods.push_back( tmpnt.mods[i] );
		this->sub_nt[sub_modL].mods.back().idMod = sub_modID;
		this->sub_nt[sub_modL].mods.back().nNet = 0;
		this->sub_nt[sub_modL].mod_NameToID[ tmpnt.mods[i].modName ] = sub_modID;
		this->sub_nt[sub_modL].totalModArea += tmpnt.mods[i].modArea;

		// calculate nSoft & nHard of each sub-netlist
		if( tmpnt.mods[i].type == HARD_BLK )
		{
			this->sub_nt[sub_modL].nHard++;
		}
		else
		{
			this->sub_nt[sub_modL].nSoft++;
		}

		// regard TSVs in layer k as pads in layer k+1
		if( tmpnt.mods[i].flagTSV == true )
		{
			PAD padTemp;
			strcpy( padTemp.padName, tmpnt.mods[i].modName );
			padTemp.idPad = this->sub_nt[ sub_modL + 1 ].pads.size();
			padTemp.x = tmpnt.mods[i].GCenterX;
			padTemp.y = tmpnt.mods[i].GCenterY;
			this->sub_nt[ sub_modL + 1 ].pads.push_back( padTemp );
			this->sub_nt[ sub_modL + 1 ].pads.back().idPad += this->sub_nt[ sub_modL + 1 ].nMod;
			this->sub_nt[ sub_modL + 1 ].pad_NameToID[ padTemp.padName ] = padTemp.idPad;
			this->sub_nt[ sub_modL + 1 ].nPad++;

			this->sub_nt[sub_modL].nTSV++;
		}
	}

	// pads in sub-netlist[0] are the same as pads in original netlist
	this->sub_nt[0].pads = tmpnt.pads;
	this->sub_nt[0].nPad = tmpnt.nPad;
	this->sub_nt[0].pad_NameToID = tmpnt.pad_NameToID;

	// update idPad in sub-netlist[0]
	for( int i = 0; i < this->sub_nt[0].nPad; i++ )
	{
		this->sub_nt[0].pads[i].idPad = i + this->sub_nt[0].nMod;
	}


	for( int i = 0; i < tmpnt.nNet; i++ )
	{
		int sub_netL  = tmpnt.nets[i].netL;
		int sub_netID = this->sub_nt[ sub_netL ].nets.size();

		// copy nets in layer k to sub-netlist[k] & update info of nets in sub-netlist[k]
		this->sub_nt[sub_netL].nets.push_back( tmpnt.nets[i] );
		this->sub_nt[sub_netL].nets.back().idNet = sub_netID;
		this->sub_nt[sub_netL].nets.back().head  = this->sub_nt[sub_netL].pins.size();

		map<string, int> *mod_NameToID = &this->sub_nt[sub_netL].mod_NameToID;
		map<string, int> *pad_NameToID = &this->sub_nt[sub_netL].pad_NameToID;

		// copy pins in layer k to sub-netlist[k] & update info pins in sub-netlist[k]
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			int sub_pinID = this->sub_nt[sub_netL].pins.size();
			this->sub_nt[sub_netL].pins.push_back( tmpnt.pins[j] );
			this->sub_nt[sub_netL].pins.back().idPin = sub_pinID;

			if( (*mod_NameToID).find( tmpnt.pins[j].pinName ) != (*mod_NameToID).end() )
			{
				int sub_modID = (*mod_NameToID)[ tmpnt.pins[j].pinName ];
				this->sub_nt[sub_netL].pins.back().index = sub_modID;
				this->sub_nt[sub_netL].mods[sub_modID].nNet++;
			}
			else if( (*pad_NameToID).find( tmpnt.pins[j].pinName ) != (*pad_NameToID).end() )
			{
				int sub_padID = (*pad_NameToID)[ tmpnt.pins[j].pinName ];
				this->sub_nt[sub_netL].pins.back().index = sub_padID + this->sub_nt[sub_netL].nMod;
			}
			else
			{
				cout << "error  : unrecognized pin:" << this->sub_nt[ sub_netL ].pins.back().pinName << " in sub_nt[" << sub_netL << "]" << endl;
				exit(EXIT_FAILURE);
			}
		}
	}

	// allocate memory for nets which link to mod[index] (mod[index].pNet) in each sub-netlist
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		int sub_modL  = tmpnt.mods[i].modL;
		int sub_modID = this->sub_nt[sub_modL].mod_NameToID[ tmpnt.mods[i].modName ];
		try
		{
			this->sub_nt[sub_modL].mods[sub_modID].pNet = new unsigned short [ this->sub_nt[sub_modL].mods[sub_modID].nNet ];
		}
		catch( bad_alloc &bad )
		{
			cout << "error  : run out of memory on sub_nt[" << sub_modL << "].mods[" << sub_modID << "].pNet" << endl;
			exit(EXIT_FAILURE);
		}
		this->sub_nt[sub_modL].mods[sub_modID].nNet = 0;
	}

	// construct the relationship between nets and modules in each sub-netlist
	vector<NET>::iterator *net_it = new vector<NET>::iterator [tmpnt.nLayer];
	for( int l = 0; l < tmpnt.nLayer; l++ )
	{
		net_it[l] = this->sub_nt[l].nets.begin();
	}

	for( int i = 0; i < tmpnt.nNet; i++ )
	{
		int sub_netL = tmpnt.nets[i].netL;
		for( int j = net_it[sub_netL]->head; j < net_it[sub_netL]->head + net_it[sub_netL]->nPin; j++ )
		{
			if( this->sub_nt[sub_netL].mod_NameToID.find( this->sub_nt[sub_netL].pins[j].pinName ) != this->sub_nt[sub_netL].mod_NameToID.end() )
			{
				int sub_modID = this->sub_nt[sub_netL].mod_NameToID[ this->sub_nt[sub_netL].pins[j].pinName ];
				this->sub_nt[sub_netL].mods[sub_modID].pNet[ this->sub_nt[sub_netL].mods[sub_modID].nNet ] = net_it[sub_netL]->idNet;
				this->sub_nt[sub_netL].mods[sub_modID].nNet++;
			}
		}
		++net_it[sub_netL];
	}

	for( int l = 0; l < tmpnt.nLayer; l++ )
	{
		this->sub_nt[l].nNet = (int)this->sub_nt[l].nets.size();
		this->sub_nt[l].nPin = (int)this->sub_nt[l].pins.size();
	}

	delete [] net_it;
	//cout << "*************** Done ****************" << endl;
}
void EPLG::SetModsInChip( NETLIST &tmpnt )
{
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		if( tmpnt.mods[i].GLeftX < 0 )
		{
			tmpnt.mods[i].GLeftX = 0;
			tmpnt.mods[i].GCenterX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW/2;
		}
		if( tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW > tmpnt.ChipWidth )
		{
			tmpnt.mods[i].GLeftX = tmpnt.ChipWidth - tmpnt.mods[i].modW;
			tmpnt.mods[i].GCenterX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW/2;
		}

		if( tmpnt.mods[i].GLeftY < 0 )
		{
			tmpnt.mods[i].GLeftY = 0;
			tmpnt.mods[i].GCenterY = tmpnt.mods[i].modH/2;
		}
		if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
		{
			tmpnt.mods[i].GLeftY = tmpnt.ChipHeight-tmpnt.mods[i].modH;
			tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
		}

		tmpnt.mods[i].CenterX = tmpnt.mods[i].GCenterX;
		tmpnt.mods[i].CenterY = tmpnt.mods[i].GCenterY;
		tmpnt.mods[i].LeftX = tmpnt.mods[i].GLeftX;
		tmpnt.mods[i].LeftY = tmpnt.mods[i].GLeftY;
	}
}



void EPLG::CreateModCurve( NETLIST &tmpnt, int shConst )
{
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		if( tmpnt.mods[i].pseudoMod != NULL )
		{
			int numTSV = (int)tmpnt.mods[i].pseudoMod->idTSV.size();
			int num = (int)sqrt( numTSV );

			tmpnt.mods[i].shapeCurveB.id = tmpnt.mods[i].idMod;
			tmpnt.mods[i].shapeCurveB.leftCurve = NULL;
			tmpnt.mods[i].shapeCurveB.rightCurve = NULL;
			tmpnt.mods[i].shapeCurveB.flagHalf = '1';
			tmpnt.mods[i].shapeCurveB.nPoint = num;

			try
			{
				tmpnt.mods[i].shapeCurveB.points = new POINT [num];
			}
			catch(bad_alloc &bad)
			{
				cout << "error  : run out of memory on CreateCurveB" << endl;
				exit(EXIT_FAILURE);
			}

			for( int j = 0; j < num; j++ )
			{
				tmpnt.mods[i].shapeCurveB.points[j].x = (int)ceil( numTSV/(float)(j+1) ) * TSV_SIZE * AMP_PARA;
				tmpnt.mods[i].shapeCurveB.points[j].y = (j+1) * TSV_SIZE * AMP_PARA;
			}
		}
		else
		{
			tmpnt.mods[i].CreateCurveB( tmpnt.totalModArea, shConst );
		}

	}
}
void EPLG::ConstructRGST( NETLIST &tmpnt, TREE *Tree )
{
	// Fcn. in the eptree.cpp
	cout << " *Construct Curves for RGST" << endl;
	for( int i = 0; i < MAX_BASE; i++ )
	{
		numPart[i] = 0;
	}

	for( int i = 0; i < numTree; i++ )
	{
		if( Tree[i].flagLeaf != '1' )
			continue;

		switch( Tree[i].nVertex )
		{
			case 1:
				numPart[0]++;
				break;
			case 2:
				numPart[1]++;
				break;
			case 3:
				numPart[2]++;
				break;
			case 4:
				numPart[3]++;
				break;
			case 5:
				numPart[4]++;
				break;
			case 6:
				numPart[5]++;
				break;
			case 7:
				numPart[6]++;
				break;
			case 8:
				numPart[7]++;
				break;
			case 9:
				numPart[8]++;
				break;
			case 10:
				numPart[9]++;
				break;
			default:
				cout << "error  : RGST[" << i << "].nVertex: " << Tree[i].nVertex << " over MAX_BASE" << endl;
				exit(EXIT_FAILURE);
				break;
		}
	}

	//for( int i = 0; i < 10; i++ )
	//{
	//	if( numPart[i] == 0 )
	//		continue;
	//	cout << " -leaf nVertex[" << i+1 << "]: " << numPart[i] << endl;
	//}

	// Initialize shape curve of leaf nodes of RGST
	// subCurve for the curve of two modules 2^n, n > 2
	int nSubC_PerM[MAX_BASE] = { 0, 0, 8, 16, 32, 64, 128, 256, 512, 1024};

	// Different slicing tree structures
	// buffCurve all modules curve result
	int nBuffC_PerM[MAX_BASE] = { 0, 0, 3, 19, 80, 286, 945, 2997, 9294, 28456};

	// subCurve
	numSCurve = 0;      // number of subCurve
	offsetSCurve = 0;   // offset of subCurve
	// buffCurve
	numBCurve = 0;      // number of buffCurve
	indexBCurve = 0;    // index of buffCurve
	// subTree
	numSTree = 0;       // number of subTree
	indexSTree = 0;     // index of subTree

	for( int i = 2; i < MAX_BASE; i++)
	{
		numSCurve = numSCurve + nSubC_PerM[i] * numPart[i];
		numBCurve = numBCurve + nBuffC_PerM[i] * numPart[i];
		numSTree  = numSTree + i * numPart[i];
	}

	try
	{
		subCurve = new CURVE[numSCurve];
	}
	catch(bad_alloc &bad)
	{
		cout << "error  : run out of memory on subCurve" << endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		buffCurve = new CURVE[numBCurve];
	}
	catch(bad_alloc &bad)
	{
		cout << "error  : run out of mem on buffCurve" << endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		subTree = new SubTREE[numSTree];
	}
	catch(bad_alloc &bad)
	{
		cout << "error  : run out of mem on subTree" << endl;
		exit(EXIT_FAILURE);
	}

	for( int i = 0; i < numTree; i++ )
	{
		Tree[i].shapeCurveN.id = i;

		if( Tree[i].flagLeaf != '1' )
			continue;

		switch( Tree[i].nVertex )
		{
			case 1:
				CreateCurveN1(tmpnt, &Tree[i]);
				break;
			case 2:
				CreateCurveN2(tmpnt, &Tree[i]);
				break;
			case 3:
				CreateCurveN3(tmpnt, &Tree[i]);
				break;
			case 4:
				CreateCurveN4(tmpnt, &Tree[i]);
				break;
			case 5:
				CreateCurveN5(tmpnt, &Tree[i]);
				break;
			case 6:
				CreateCurveN6(tmpnt, &Tree[i]);
				break;
			case 7:
				CreateCurveN7(tmpnt, &Tree[i]);
				break;
			case 8:
				CreateCurveN8(tmpnt, &Tree[i]);
				break;
			case 9:
				CreateCurveN9(tmpnt, &Tree[i]);
				break;
			case 10:
				CreateCurveN10(tmpnt, &Tree[i]);
				break;
			default:
				cout << "error  : RGST[" << i << "].nVertex: " << Tree[i].nVertex << " over MAX_BASE" << endl;
				exit(EXIT_FAILURE);
				break;
		}
		Tree[i].shapeCurveN.flagHalf = '1';
	}

	for( int i = numTree-1; i >= 0; i-- )
	{
		if( Tree[i].flagLeaf == '1' )
			continue;

		if( Tree[i].leftTree == NULL )
		{
			cout << "error  : left tree node empty: " << i << " " << Tree[i].nVertex << endl;
		}
		if( Tree[i].rightTree == NULL )
		{
			cout << "error  : right tree node empty: " << i << " " << Tree[i].nVertex << endl;
		}
		if( Tree[i].leftTree == NULL || Tree[i].rightTree == NULL )
		{
			exit(EXIT_FAILURE);
		}

		MergeTreeCurveN( &Tree[i] );
	}
}
void EPLG::ReorderRGST( TREE *Tree, TREE **pointerTree )
{
	pointerTree[0] = &Tree[0];
	pointerTree[0]->level = 0;

	int currentID = 1;
	int level = 1;

	deque<int> tempQ;
	tempQ.push_back(0);

	while( !tempQ.empty() )
	{
		int pointerID = tempQ.front();
		int originalID = pointerTree[pointerID]->id;
		tempQ.pop_front();

		if( Tree[originalID].leftTree != NULL )
		{
			int leftID = Tree[originalID].leftTree->id;
			pointerTree[currentID] = &Tree[leftID];
			pointerTree[currentID]->level = level;
			tempQ.push_back( currentID );
			currentID++;
		}
		if( Tree[originalID].rightTree != NULL )
		{
			int rightID = Tree[originalID].rightTree->id;
			pointerTree[currentID] = &Tree[rightID];
			pointerTree[currentID]->level = level;
			tempQ.push_back( currentID );
			currentID++;
		}
		level++;
	}
}
///< Trace Back Fcn.

bool EPLG::FindValidPoint( NETLIST &tmpnt, TREE *Tree, TREE **TreeP, unsigned short &numTree )
{
	cout << " *Tracing Points" << endl;

	vector<int> validPoints;	// record index of shapeCurveN
	vector<int> candidates; 	// record candidate index of shapeCurveN
	int bestIndex = -1;
	double bestWL = MAX_WL;
	bool flagSuc = false;

	int leftBound;
	int rightBound;
	int bottomBound;
	int topBound;

	cout << " -total number of points: " << TreeP[0]->shapeCurveN.nPoint << endl;

	for( int i = 0; i < TreeP[0]->shapeCurveN.nPoint; i++ )
	{
		if( (long int)TreeP[0]->shapeCurveN.points[i].x <= (long int)tmpnt.ChipWidth &&
		        (long int)TreeP[0]->shapeCurveN.points[i].y <= (long int)tmpnt.ChipHeight )
		{
			validPoints.push_back(i);
		}
		if( TreeP[0]->shapeCurveN.points[i].y/(float)TreeP[0]->shapeCurveN.points[i].x > 0.95 &&
		        TreeP[0]->shapeCurveN.points[i].y/(float)TreeP[0]->shapeCurveN.points[i].x < 1.05 )
		{
			candidates.push_back(i);
		}
	}

	if( validPoints.size() == 0 )	// no valid points
	{
		cout << " -total number of candidates: " << candidates.size() << endl;

		vector<MODULE*> modArray;
		modArray.resize( tmpnt.nMod );
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			modArray[i] = &tmpnt.mods[i];
		}

		int minIndex = -1;
		float min_diffAR = MAX_FLT;

		for( unsigned int i = 0; i < candidates.size(); i++ )
		{
			TraceBack( candidates[i], tmpnt, Tree, TreeP, numTree );
			OptimizeWL( tmpnt, Tree, TreeP, numTree );

			int packedBoundX = 0;
			int packedBoundY = 0;

			if( TreeP[0]->shapeCurveN.points[ candidates[i] ].x >=
			        TreeP[0]->shapeCurveN.points[ candidates[i] ].y )
			{
				 PackToLeftSub( modArray, 0, packedBoundX );
				 PackToBottomSub( modArray, 0, packedBoundY );
			}
			else
			{
				 PackToBottomSub( modArray, 0, packedBoundY );
				 PackToLeftSub( modArray, 0, packedBoundX );
			}

			if( packedBoundX > tmpnt.ChipWidth || packedBoundY > tmpnt.ChipHeight )
			{
				if( packedBoundX > tmpnt.ChipWidth && packedBoundY > tmpnt.ChipHeight )
				{
					 PackToCenter( leftBound, rightBound, bottomBound, topBound, packedBoundX/2, packedBoundY/2, tmpnt);
				}
				else if( packedBoundX > tmpnt.ChipWidth )
				{
					 PackToCenter( leftBound, rightBound, bottomBound, topBound, packedBoundX/2, tmpnt.ChipHeight/2, tmpnt);
				}
				else if( packedBoundY > tmpnt.ChipHeight )
				{
					 PackToCenter( leftBound, rightBound, bottomBound, topBound, tmpnt.ChipWidth/2, packedBoundY/2, tmpnt);
				}

				packedBoundX = rightBound - leftBound;
				packedBoundY = topBound - bottomBound;

				if( packedBoundX <= tmpnt.ChipWidth && packedBoundY <= tmpnt.ChipHeight )
				{
					for( int j = 0; j < tmpnt.nMod; j++ )
					{
						tmpnt.mods[j].LeftX -= leftBound;
						tmpnt.mods[j].LeftY -= bottomBound;
						tmpnt.mods[j].CenterX -= leftBound;
						tmpnt.mods[j].CenterX -= bottomBound;
					}
				}
			}

			if( packedBoundX <= tmpnt.ChipWidth && packedBoundY <= tmpnt.ChipHeight )
			{
				CalculateHPWL_FINAL( tmpnt );

				if( tmpnt.totalWL < bestWL )
				{
					for( int j = 0; j < tmpnt.nMod; j++ )
					{
						tempCurve[1].points[j].x = tmpnt.mods[j].LeftX;
						tempCurve[1].points[j].y = tmpnt.mods[j].LeftY;

						tempCurve[2].points[j].x = tmpnt.mods[j].modW;
						tempCurve[2].points[j].y = tmpnt.mods[j].modH;
					}

					bestWL = tmpnt.totalWL;
					bestIndex = candidates[i];
				}
			}
			else
			{
				float temp_diffAR = fabs( TreeP[0]->shapeCurveN.points[i].y/(float)TreeP[0]->shapeCurveN.points[i].x - 1.0 );
				if( temp_diffAR < min_diffAR )
				{
					min_diffAR = temp_diffAR;
					minIndex = candidates[i];
				}
			}

				//stringstream sstemp;
				//char name[50] = "\0";
				//sstemp << "show_" << i << endl;
				//sstemp >> name;
				//PlotResultL( tmpnt, name );
				//sstemp.str( string() );
				//sstemp.clear();

				//getchar();
			
		}

		if( bestIndex != -1 )
		{
			cout << " -choose point[" << bestIndex << "] as final solution for layer[" << tmpnt.nLayer << "]" << endl;
			for(int i=0; i<tmpnt.nMod; i++)
			{
				tmpnt.mods[i].LeftX = tempCurve[1].points[i].x;
				tmpnt.mods[i].LeftY = tempCurve[1].points[i].y;
				tmpnt.mods[i].modW  = tempCurve[2].points[i].x;
				tmpnt.mods[i].modH  = tempCurve[2].points[i].y;
				tmpnt.mods[i].CenterX = tmpnt.mods[i].LeftX + (int)(tmpnt.mods[i].modW/2);
				tmpnt.mods[i].CenterY = tmpnt.mods[i].LeftY + (int)(tmpnt.mods[i].modH/2);
			}

			 PackToCenter( leftBound, rightBound, bottomBound, topBound, tmpnt.ChipWidth/2, tmpnt.ChipHeight/2, tmpnt);

			flagSuc = true;

		}
		else
		{
			TraceBack( minIndex, tmpnt, Tree, TreeP, numTree );
			OptimizeWL( tmpnt, Tree, TreeP, numTree );
		}

		//int minIndex = 0;
		//float min_diffAR = MAX_FLT;
		//for( unsigned int i = 0; i < candidates.size(); i++ )
		//{
		//	float temp_diffAR = fabs(TreeP[0]->shapeCurveN.points[i].y/(float)TreeP[0]->shapeCurveN.points[i].x - 1.0);
		//	if( temp_diffAR < min_diffAR )
		//	{
		//		min_diffAR = temp_diffAR;
		//		minIndex = candidates[i];
		//	}
		//}

		//TraceBack( minIndex, tmpnt, Tree, TreeP, numTree );
		//OptimizeWL( tmpnt, Tree, TreeP, numTree );

	}
	else // have valid points
	{
		cout << " -total number of valid points: " << validPoints.size() << endl;

		for( int i = 0; i < (int)validPoints.size(); i++ )
		{
			TraceBack( validPoints[i], tmpnt, Tree, TreeP, numTree );
			OptimizeWL( tmpnt, Tree, TreeP, numTree );
			if( 1 )
			{
				CalculateHPWL_FINAL( tmpnt );
			}
			else
			{
				tmpnt.totalWL = COM_MAX(((tmpnt.ChipWidth-TreeP[0]->shapeCurveN.points[validPoints[i]].x)/(double)AMP_PARA) , ((tmpnt.ChipHeight-TreeP[0]->shapeCurveN.points[validPoints[i]].y)/(double)AMP_PARA));
			}

			if( tmpnt.totalWL < bestWL )
			{
				for(int j=0; j<tmpnt.nMod; j++)
				{
					// for the temp coordiante
					tempCurve[1].points[j].x = tmpnt.mods[j].LeftX;
					tempCurve[1].points[j].y = tmpnt.mods[j].LeftY;
					// for the temp dimension
					tempCurve[2].points[j].x = tmpnt.mods[j].modW;
					tempCurve[2].points[j].y = tmpnt.mods[j].modH;
				}
				bestWL = tmpnt.totalWL;
				bestIndex = validPoints[i];
			}
		}
		cout << " -choose point[" << bestIndex << "] as final solution for layer[" << tmpnt.nLayer << "]" << endl;
		TraceBack( bestIndex, tmpnt, Tree, TreeP, numTree );
		OptimizeWL( tmpnt, Tree, TreeP, numTree );
		for(int i=0; i<tmpnt.nMod; i++)
		{
			tmpnt.mods[i].LeftX = tempCurve[1].points[i].x;
			tmpnt.mods[i].LeftY = tempCurve[1].points[i].y;
			tmpnt.mods[i].modW  = tempCurve[2].points[i].x;
			tmpnt.mods[i].modH  = tempCurve[2].points[i].y;
			tmpnt.mods[i].CenterX = tmpnt.mods[i].LeftX + (int)(tmpnt.mods[i].modW/2);
			tmpnt.mods[i].CenterY = tmpnt.mods[i].LeftY + (int)(tmpnt.mods[i].modH/2);
		}

		 PackToCenter( leftBound, rightBound, bottomBound, topBound, tmpnt.ChipWidth/2, tmpnt.ChipHeight/2, tmpnt);

		flagSuc = true;
	}
	return flagSuc;
}

void EPLG::TraceBack( int tIndex, NETLIST &tmpnt, TREE* Tree, TREE **TreeP, unsigned short &numTree )
{
	indexSTree = 0;	// index of subTree

	// initialize the traced point in the root
	TreeP[0]->shapeCurveN.traceIndex = tIndex;
	TreeP[0]->flagSwap = 0;

	NCKUTracePoint(Tree, &(TreeP[0]->shapeCurveN));

	for(int i=1; i<numTree; i++)
	{
		TreeP[i]->flagSwap = 0;

		// for two cases:
		// 1) the tree node containing more than baseNum vertices
		// 2) the tree node containing EXACTLY two vertices
		if( (TreeP[i]->flagLeaf=='0') || (TreeP[i]->nVertex==2) )
		{
			NCKUTracePoint(TreeP[i], &(TreeP[i]->shapeCurveN));
		}
		// for the tree node containing less than
		// or equal to baseNum vertices except the two vertices case.
		else if((TreeP[i]->flagLeaf=='1') && (TreeP[i]->nVertex!=1) )
		{
			TraceBase(&(TreeP[i]->shapeCurveN));
		}
		// for the tree node containing EXACTLY one vertex
		else
		{
			tmpnt.mods[TreeP[i]->vArray[0]].shapeCurveB.traceIndex = TreeP[i]->shapeCurveN.traceIndex;
		}
	}

	return;
}

void EPLG::NCKUTracePoint(TREE* t, CURVE *c)
{
	if( t->flagLeaf == '1' )
	{
		TracePoint( c );
	}
	else
	{
		c->rightCurve->traceIndex = c->points[c->traceIndex].traceR;
		c->leftCurve->traceIndex = c->points[c->traceIndex].traceL;
	}
}

void EPLG::TracePoint(CURVE *c)
{
	int n;            // total number of points on the shape curve
	int newIndex;     // new index of point
	int nL;           // total number of points on the left child curve
	int nR;           // total number of points on the right child curve
	char flagUpHalf;  // to mark if the point is above W=H line

	// if the point position is above W=H line
	if (c->traceIndex >= c->nPoint)
	{
		if (c->points[c->nPoint-1].x != c->points[c->nPoint-1].y)
		{
			n = 2 * c->nPoint;
		}
		else // if there is one point on the W=H line
		{
			n = 2 * c->nPoint - 1;
		}
		newIndex = n - 1 - c->traceIndex;
		flagUpHalf = 1;
	}
	else
	{
		newIndex = c->traceIndex;
		flagUpHalf = 0;
	}

	if (c->leftCurve->points[c->leftCurve->nPoint-1].x != c->leftCurve->points[c->leftCurve->nPoint-1].y)
	{
		nL = 2 * c->leftCurve->nPoint;
	}
	else
	{
		nL = 2 * c->leftCurve->nPoint - 1;
	}

	if (c->rightCurve->points[c->rightCurve->nPoint-1].x != c->rightCurve->points[c->rightCurve->nPoint-1].y)
	{
		nR = 2 * c->rightCurve->nPoint;
	}
	else
	{
		nR = 2 * c->rightCurve->nPoint - 1;
	}

	if (flagUpHalf == 1)
	{
		c->leftCurve->traceIndex = nL - 1 - c->points[newIndex].traceL;
		c->rightCurve->traceIndex = nR - 1 - c->points[newIndex].traceR;
	}
	else
	{
		c->leftCurve->traceIndex = c->points[newIndex].traceL;
		c->rightCurve->traceIndex = c->points[newIndex].traceR;
	}
}

void EPLG::TraceBase(CURVE *c) // trace the points in EP
{
	int i;
	int j;
	int buffIndex;	// the buffCurve index that has been traced
	int nTmpCurve;
	CURVE *tmpCurve[2*MAX_BASE-1];	// pointers using to store all of the curves in the basic trees, easy to make a loop.
	int sTreeMark[2*MAX_BASE-1];	// record the subTree index which is coresponding to the curve

	int head;      // head index of tmpCurve array
	int tail;      // tail index of tmpCurve array
	int sThead;    // head index of subTree

	TraceMergedCurve(c, &buffIndex);

	tmpCurve[0] = &buffCurve[buffIndex];
	head = 0;
	nTmpCurve = 1;
	tail = head + nTmpCurve;
	subTree[indexSTree].id = indexSTree;
	subTree[indexSTree].flagSwap = 0;
	subTree[indexSTree].parentTree = NULL;
	subTree[indexSTree].nodeCurve = c;
	subTree[indexSTree].selfCurve = &buffCurve[buffIndex];
	subTree[indexSTree].nVertex = 0;
	sTreeMark[0] = indexSTree;
	sThead = indexSTree;
	indexSTree++;


	// Go through the selected buffCurve to trace, head and tail would store the tmpCurve index that need to be traced in each loop
	do
	{
		nTmpCurve = 0;
		for (i=head; i<tail; i++)
		{
			TracePoint(tmpCurve[i]);
			// for the left Curve
			// for the case that this curve is generated by merging
			if ((tmpCurve[i]->leftCurve->leftCurve==tmpCurve[i]->leftCurve->rightCurve) && (tmpCurve[i]->leftCurve->leftCurve==tmpCurve[i]->leftCurve))
			{
				// for buffCurve
				TraceMergedCurve(tmpCurve[i]->leftCurve, &buffIndex);
				tmpCurve[tail+nTmpCurve] = &buffCurve[buffIndex];

				subTree[indexSTree].id = indexSTree;
				subTree[indexSTree].flagSwap = 0;
				subTree[sTreeMark[i]].leftTree = &subTree[indexSTree];
				subTree[sTreeMark[i]].leftCurve = &buffCurve[buffIndex];
				subTree[indexSTree].parentTree = &subTree[sTreeMark[i]];
				subTree[indexSTree].selfCurve = &buffCurve[buffIndex];
				subTree[indexSTree].nodeCurve = NULL;
				subTree[indexSTree].nVertex = 0;
				sTreeMark[tail+nTmpCurve] = indexSTree;

				indexSTree++;
				nTmpCurve++;
			}
			// for the case that this curve is generated by adding two base shapeCurveB
			else if (tmpCurve[i]->leftCurve->leftCurve != tmpCurve[i]->leftCurve->rightCurve)
			{
				// for subCurve
				tmpCurve[tail+nTmpCurve] = tmpCurve[i]->leftCurve;

				subTree[indexSTree].id = indexSTree;
				subTree[indexSTree].flagSwap = 0;
				subTree[sTreeMark[i]].leftTree = &subTree[indexSTree];
				subTree[sTreeMark[i]].leftCurve = tmpCurve[i]->leftCurve;
				subTree[indexSTree].parentTree = &subTree[sTreeMark[i]];
				subTree[indexSTree].selfCurve = tmpCurve[i]->leftCurve;
				subTree[indexSTree].nodeCurve = NULL;
				subTree[indexSTree].nVertex = 0;
				sTreeMark[tail+nTmpCurve] = indexSTree;

				indexSTree++;
				nTmpCurve++;
			}
			else
			{
				// for shapeCurveB
				subTree[sTreeMark[i]].leftTree = NULL;
				subTree[sTreeMark[i]].leftCurve = tmpCurve[i]->leftCurve;
				subTree[sTreeMark[i]].vArray[subTree[sTreeMark[i]].nVertex] = tmpCurve[i]->leftCurve->id;

				subTree[sTreeMark[i]].nVertex++;
			}

			// for the right Curve
			// for the case that this curve is generated by merging
			if ((tmpCurve[i]->rightCurve->leftCurve==tmpCurve[i]->rightCurve->rightCurve) && (tmpCurve[i]->rightCurve->leftCurve==tmpCurve[i]->rightCurve))
			{
				// for buffCurve
				TraceMergedCurve(tmpCurve[i]->rightCurve, &buffIndex);

				tmpCurve[tail+nTmpCurve] = &buffCurve[buffIndex];

				subTree[indexSTree].id = indexSTree;
				subTree[indexSTree].flagSwap = 0;
				subTree[sTreeMark[i]].rightTree = &subTree[indexSTree];
				subTree[sTreeMark[i]].rightCurve = &buffCurve[buffIndex];
				subTree[indexSTree].parentTree = &subTree[sTreeMark[i]];
				subTree[indexSTree].selfCurve = &buffCurve[buffIndex];
				subTree[indexSTree].nodeCurve = NULL;
				subTree[indexSTree].nVertex = 0;
				sTreeMark[tail+nTmpCurve] = indexSTree;

				indexSTree++;
				nTmpCurve++;
			}
			// for the case that this curve is generated by adding two basic shapeCurveB
			else if (tmpCurve[i]->rightCurve->leftCurve != tmpCurve[i]->rightCurve->rightCurve)
			{
				// for subCurve
				tmpCurve[tail+nTmpCurve] = tmpCurve[i]->rightCurve;

				subTree[indexSTree].id = indexSTree;
				subTree[indexSTree].flagSwap = 0;
				subTree[sTreeMark[i]].rightTree = &subTree[indexSTree];
				subTree[sTreeMark[i]].rightCurve = tmpCurve[i]->rightCurve;
				subTree[indexSTree].parentTree = &subTree[sTreeMark[i]];
				subTree[indexSTree].selfCurve = tmpCurve[i]->rightCurve;
				subTree[indexSTree].nodeCurve = NULL;
				subTree[indexSTree].nVertex = 0;
				sTreeMark[tail+nTmpCurve] = indexSTree;

				indexSTree++;
				nTmpCurve++;
			}
			else
			{
				// for shapeCurveB
				subTree[sTreeMark[i]].rightTree = NULL;
				subTree[sTreeMark[i]].rightCurve = tmpCurve[i]->rightCurve;
				subTree[sTreeMark[i]].vArray[subTree[sTreeMark[i]].nVertex] = tmpCurve[i]->rightCurve->id;

				subTree[sTreeMark[i]].nVertex++;
			}
		}

		head = tail;
		tail = head + nTmpCurve;
	}
	while( nTmpCurve != 0 );

	for (i = indexSTree-1; i > sThead; i--)
	{
		for (j=0; j<subTree[i].nVertex; j++)
		{
			subTree[i].parentTree->vArray[subTree[i].parentTree->nVertex] = subTree[i].vArray[j];
			subTree[i].parentTree->nVertex++;
		}
	}
}

void EPLG::TraceMergedCurve(CURVE *mergedCurve, int *tracedBCurveID)
{
	int newIndex; // the point index that stored the trace information
	int n;

	// if the point is above W=H line
	if (mergedCurve->traceIndex >= mergedCurve->nPoint)
	{
		if (mergedCurve->points[mergedCurve->nPoint-1].x != mergedCurve->points[mergedCurve->nPoint-1].y)
		{
			n = 2 * mergedCurve->nPoint;
		}
		else
		{
			n = 2 * mergedCurve->nPoint - 1;
		}
		newIndex = n - 1 - mergedCurve->traceIndex;
		*tracedBCurveID = mergedCurve->points[newIndex].traceL;

		if (buffCurve[*tracedBCurveID].points[buffCurve[*tracedBCurveID].nPoint-1].x != buffCurve[*tracedBCurveID].points[buffCurve[*tracedBCurveID].nPoint-1].y)
		{
			n = 2 * buffCurve[*tracedBCurveID].nPoint;
		}
		else
		{
			n = 2 * buffCurve[*tracedBCurveID].nPoint - 1;
		}
		buffCurve[*tracedBCurveID].traceIndex = n - 1 - mergedCurve->points[newIndex].traceR;
	}
	else
	{
		newIndex = mergedCurve->traceIndex;
		*tracedBCurveID = mergedCurve->points[newIndex].traceL;
		buffCurve[*tracedBCurveID].traceIndex = mergedCurve->points[newIndex].traceR;
	}
	return;
}
void EPLG::CalculateHPWL_FINAL( NETLIST &tmpnt )
{
	int maxX = 0;
	int minX = tmpnt.ChipWidth;
	int maxY = 0;
	int minY = tmpnt.ChipHeight;
	int centerX;
	int centerY;

	tmpnt.totalWL = 0.0;

	for (int i=0; i<tmpnt.nMod; i++)
	{
		tmpnt.mods[i].CenterX = tmpnt.mods[i].LeftX + tmpnt.mods[i].modW/2;
		tmpnt.mods[i].CenterY = tmpnt.mods[i].LeftY + tmpnt.mods[i].modH/2;
	}

	for(int i=0; i<tmpnt.nNet; i++)
	{
		maxX = 0;
		minX = tmpnt.ChipWidth;
		maxY = 0;
		minY = tmpnt.ChipHeight;
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head+tmpnt.nets[i].nPin; j++)
		{
			if(tmpnt.pins[j].index < tmpnt.nMod)
			{
				centerX = tmpnt.mods[tmpnt.pins[j].index].CenterX;
				centerY = tmpnt.mods[tmpnt.pins[j].index].CenterY;
			}
			else
			{
				centerX = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].x;
				centerY = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].y;
			}

			maxX = (centerX > maxX) ? centerX : maxX;
			minX = (centerX < minX) ? centerX : minX;
			maxY = (centerY > maxY) ? centerY : maxY;
			minY = (centerY < minY) ? centerY : minY;
		}
		tmpnt.totalWL = tmpnt.totalWL + (double)(((maxX - minX) + (maxY - minY))/AMP_PARA);
	}
}

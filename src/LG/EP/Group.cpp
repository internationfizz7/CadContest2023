#include "EP.h"

void EPLG::Grouping( vector<PSEUDO_MODULE> &resultPM, NETLIST &pm_tmpnt, NETLIST &ori_tmpnt, char *plotFile )
{
	cout << " *Clustering" << endl;

	deque<GROUP> tempC;
	vector<GROUP> resultC;

	tempC.resize(1);

	InitialCluster( tempC[0], ori_tmpnt );

	BipartitionC( tempC, resultC, ori_tmpnt );

	cout << " -number of clusters: " << resultC.size() << endl;

	PlotResultC( resultC, ori_tmpnt, plotFile );

	
	if( ori_tmpnt.nTSV == 0 || flagModSoft == true )
	{
		UpdateVArray( ori_tmpnt, resultC );

		resultC.clear();
		vector<GROUP> (resultC).swap(resultC);
	}
	else
	{
		deque<PSEUDO_MODULE> tempPM;
		vector<CUTSET> tempCS;

		InitialPseudoMod( tempPM, tempCS, resultC, ori_tmpnt );

		BipartitionPM( tempPM, resultPM, tempCS, ori_tmpnt );

		RefineSinglePMod( resultPM, ori_tmpnt );

		PlotResultPM( resultC, resultPM, ori_tmpnt, plotFile );

		CreatePModNetlist( pm_tmpnt, ori_tmpnt, resultPM );

		UpdateVArray( pm_tmpnt, ori_tmpnt, resultPM, resultC );

		for( int i = 0; i < (int)tempCS.size(); i++ )
		{
			tempCS[i].cutV.clear();
			tempCS[i].cutH.clear();
		}

		tempCS.clear();
		resultC.clear();

		vector<CUTSET> (tempCS).swap(tempCS);
		vector<GROUP> (resultC).swap(resultC);
	}
}

void EPLG::InitialCluster( GROUP &firstC, NETLIST &tmpnt )
{
	// first cluster
	firstC.idClu = 0;
	firstC.idMod.push_back( tmpnt.mods[0].idMod );
	firstC.lbX = tmpnt.mods[0].GLeftX;
	firstC.lbY = tmpnt.mods[0].GLeftY;
	firstC.rtX = tmpnt.mods[0].GLeftX + tmpnt.mods[0].modW;
	firstC.rtY = tmpnt.mods[0].GLeftY + tmpnt.mods[0].modH;
	firstC.modArea += tmpnt.mods[0].modArea;

	if( tmpnt.mods[0].flagTSV )
	{
		firstC.nTSV++;
	}
	else
	{
		firstC.nMod++;

		firstC.maxModW = tmpnt.mods[0].modW;
		firstC.maxModH = tmpnt.mods[0].modH;
		firstC.minModL = COM_MIN( tmpnt.mods[0].modW, tmpnt.mods[0].modH );
		firstC.avgModL += (tmpnt.mods[0].modW + tmpnt.mods[0].modH)/2;
	}

	for( int i = 1; i < tmpnt.nMod; i++ )
	{
		firstC.idMod.push_back( tmpnt.mods[i].idMod );
		if( tmpnt.mods[i].GLeftX < firstC.lbX )
		{
			firstC.lbX = tmpnt.mods[i].GLeftX;
		}
		if( tmpnt.mods[i].GLeftY < firstC.lbY )
		{
			firstC.lbY = tmpnt.mods[i].GLeftY;
		}
		if( tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW > firstC.rtX )
		{
			firstC.rtX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW;
		}
		if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > firstC.rtY )
		{
			firstC.rtY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH;
		}
		firstC.modArea += tmpnt.mods[i].modArea;

		if( tmpnt.mods[i].modW > firstC.maxModW )
		{
			firstC.maxModW = tmpnt.mods[i].modW;
		}
		if( tmpnt.mods[i].modH > firstC.maxModH )
		{
			firstC.maxModH = tmpnt.mods[i].modH;
		}

		if( tmpnt.mods[i].flagTSV )
		{
			firstC.nTSV++;
		}
		else
		{
			firstC.nMod++;
			int minModL = COM_MIN( tmpnt.mods[i].modW, tmpnt.mods[i].modH );
			if( minModL < firstC.minModL )
			{
				firstC.minModL = minModL;
			}
			firstC.avgModL += (tmpnt.mods[i].modW + tmpnt.mods[i].modH)/2;
		}
	}

	if( firstC.nMod != 0 )
	{
		firstC.avgModL = (int)(firstC.avgModL/(float)firstC.nMod);
	}

	firstC.centerX = (firstC.lbX + firstC.rtX)/2;
	firstC.centerY = (firstC.lbY + firstC.rtY)/2;
	firstC.boxArea = (int)( ((firstC.rtX - firstC.lbX)/(double)AMP_PARA) * ((firstC.rtY - firstC.lbY)/(double)AMP_PARA) );

	// root
	numTree = 0;
	RGST[numTree].id = numTree;
	RGST[numTree].lbX = firstC.lbX;
	RGST[numTree].lbY = firstC.lbY;
	RGST[numTree].rtX = firstC.rtX;
	RGST[numTree].rtY = firstC.rtY;
	RGST[numTree].treeCoordX = firstC.centerX;
	RGST[numTree].treeCoordY = firstC.centerY;
	numTree++;
}
void EPLG::BipartitionC( deque<GROUP> &tempC, vector<GROUP> &resultC, NETLIST &tmpnt )
{
	int currentW = tempC.front().rtX - tempC.front().lbX;
	int currentH = tempC.front().rtY - tempC.front().lbY;

	float currentAR = (float)currentW/(float)currentH;
	float ratioW = (float)tempC.front().maxModW/(float)currentW;
	float ratioH = (float)tempC.front().maxModH/(float)currentH;

	GROUP lchildC;	///< left (or bottom) cluster
	GROUP rchildC;	///< right (or top) cluster

	char VHcut = '\0';	///< cut direction

	if( currentAR >= 0.8 && currentAR <= 1.25 )
	{
		if( ratioW >= 1 && ratioH < 1 )
		{
			VHcut = SelectCutH( tempC.front(), lchildC, rchildC, tmpnt );
		}
		else if( ratioW < 1 && ratioH >= 1 )
		{
			VHcut = SelectCutV( tempC.front(), lchildC, rchildC, tmpnt );
		}
		else
		{
			VHcut = SelectCutHV( tempC.front(), lchildC, rchildC, tmpnt );
		}
	}
	else if( currentAR > 1.25 )		///< width > height
	{
		if( ratioW >= 1 )
		{
			VHcut = SelectCutH( tempC.front(), lchildC, rchildC, tmpnt );
		}
		else
		{
			VHcut = SelectCutV( tempC.front(), lchildC, rchildC, tmpnt );
		}
	}
	else if( currentAR < 0.8 )		///< height > width
	{
		if( ratioH >= 1 )
		{
			VHcut = SelectCutV( tempC.front(), lchildC, rchildC, tmpnt );
		}
		else
		{
			VHcut = SelectCutH( tempC.front(), lchildC, rchildC, tmpnt );
		}
	}

	//cout << VHcut << " ";
	//if( VHcut == 'V' )
	//	cout << lchildC.rtX << endl;
	//else
	//	cout << lchildC.rtY << endl;
	UpdateClusterChild( lchildC, rchildC, tmpnt );

	ConstructTree( tempC.front(), lchildC, rchildC, VHcut, tmpnt );

	if( tmpnt.nTSV == 0 || flagModSoft == true )
	{
		if( (int)lchildC.idMod.size() <= numBase && (int)lchildC.idMod.size() > 0 )
		{
			RGST[ lchildC.idClu ].flagLeaf = '1';
			resultC.push_back( lchildC );
		}
		else
		{
			tempC.push_back( lchildC );
		}

		if( (int)rchildC.idMod.size() <= numBase && (int)rchildC.idMod.size() > 0 )
		{
			RGST[ rchildC.idClu ].flagLeaf = '1';
			resultC.push_back( rchildC );
		}
		else
		{
			tempC.push_back( rchildC );
		}
	}
	else
	{
		//int nLeftTSV = (int)floor( lchildC.avgModL/(float)(TSV_SIZE * AMP_PARA) );
		//int nLeftPM = (int)ceil( lchildC.nTSV/(lchildC.avgModL/(float)(TSV_SIZE * AMP_PARA)) );

		int maxLengthL = COM_MAX( lchildC.maxModW, lchildC.maxModH );
		int nLeftPM = (int)ceil( lchildC.nTSV/(maxLengthL/(float)(TSV_SIZE * AMP_PARA)) );

		int nLeftMod = nLeftPM + lchildC.nMod;

		if( nLeftMod <= numBase && nLeftMod > 0 )
		{
			RGST[ lchildC.idClu ].flagLeaf = '1';

			resultC.push_back( lchildC );
		}
		else
		{
			tempC.push_back( lchildC );
		}

		//int nRightTSV = (int)floor( rchildC.avgModL/(float)(TSV_SIZE * AMP_PARA) );
		//int nRightPM = (int)ceil( rchildC.nTSV/(rchildC.avgModL/(float)(TSV_SIZE * AMP_PARA)) );

		int maxLengthR = COM_MAX( rchildC.maxModW, rchildC.maxModH );
		int nRightPM = (int)ceil( rchildC.nTSV/(maxLengthR/(float)(TSV_SIZE * AMP_PARA)) );

		int nRightMod = nRightPM + rchildC.nMod;

		if( nRightMod <= numBase && nRightMod > 0 )
		{
			RGST[ rchildC.idClu ].flagLeaf = '1';

			resultC.push_back( rchildC );
		}
		else
		{
			tempC.push_back( rchildC );
		}
	}

	tempC.pop_front();

	if( tempC.empty() )
	{
		return;
	}
	else
	{
		BipartitionC( tempC, resultC, tmpnt );
	}
}


char EPLG::SelectCutV( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt )
{
	int currentW = currentC.rtX - currentC.lbX;
	
	int min_rtX = tmpnt.mods[ currentC.idMod[0] ].GLeftX + tmpnt.mods[ currentC.idMod[0] ].modW;
	int max_lbX = tmpnt.mods[ currentC.idMod[0] ].GLeftX;
	for( unsigned int i = 1; i < currentC.idMod.size(); i++ )
	{
		if( min_rtX > tmpnt.mods[ currentC.idMod[i] ].GLeftX + tmpnt.mods[ currentC.idMod[i] ].modW )
		{
			min_rtX = tmpnt.mods[ currentC.idMod[i] ].GLeftX + tmpnt.mods[ currentC.idMod[i] ].modW;
		}
		if( max_lbX < tmpnt.mods[ currentC.idMod[i] ].GLeftX )
		{
			max_lbX = tmpnt.mods[ currentC.idMod[i] ].GLeftX;
		}
	}

	int tileW = (max_lbX - min_rtX)/200;

	int midX = min_rtX + 51 * tileW;
	int weightX = 0;

	for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
	{
		int width = 0;
		int height = (int)(tmpnt.mods[ currentC.idMod[j] ].modH/(double)AMP_PARA);
		if( tmpnt.mods[ currentC.idMod[j] ].GCenterX < midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW > midX )
		{
			width = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - midX )/(double)AMP_PARA);
			weightX += width * height;
			//weightX += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - midX );
		}
		else if( tmpnt.mods[ currentC.idMod[j] ].GCenterX >= midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX < midX )
		{
			width = (int)(abs( midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX )/(double)AMP_PARA);
			weightX += width * height;
			//weightX += abs( midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX );
		}
	}

	weightX = (int)(weightX * pow((1.0 + abs(100-51)/(float)100), para_pow));

	for( int i = 52; i < 151; i++ )
	{
		int tmp_midX = min_rtX + i * tileW;
		int tmp_weightX = 0;

		for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
		{
			int width = 0;
			int height = (int)(tmpnt.mods[ currentC.idMod[j] ].modH/(double)AMP_PARA);
			if( tmpnt.mods[ currentC.idMod[j] ].GCenterX < tmp_midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW > tmp_midX )
			{
				width = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - tmp_midX )/(double)AMP_PARA);
				tmp_weightX += width * height;
				//tmp_weightX += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - tmp_midX );
			}
			else if( tmpnt.mods[ currentC.idMod[j] ].GCenterX >= tmp_midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX < tmp_midX )
			{
				width = (int)(abs( tmp_midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX )/(double)AMP_PARA);
				tmp_weightX += width * height;
				//tmp_weightX += abs( tmp_midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX );
			}
		}

		tmp_weightX = (int)(tmp_weightX * pow((1.0 + abs(100-i)/(float)100), para_pow));

		if( tmp_weightX < weightX )
		{
			weightX = tmp_weightX;
			midX = tmp_midX;
		}
	}

	double areaLeft = 0;
	double areaRight = 0;
	double expandPara = 0;
	for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
	{
		if( tmpnt.mods[ currentC.idMod[j] ].type == HARD_BLK )
			expandPara = 1.09;
		else
			expandPara = 1.01;

		if( tmpnt.mods[ currentC.idMod[j] ].GCenterX < midX )
		{
			areaLeft += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
			lchildC.idMod.push_back( currentC.idMod[j] );
			lchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
		}
		else
		{
			areaRight += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
			rchildC.idMod.push_back( currentC.idMod[j] );
			rchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
		}
	}

	if( lchildC.idMod.size() == 0 || rchildC.idMod.size() == 0 )
	{
		midX = currentC.centerX;
		areaLeft = 0;
		areaRight = 0;
		expandPara = 0;

		lchildC.idMod.clear();
		lchildC.modArea = 0;
		rchildC.idMod.clear();
		rchildC.modArea = 0;

		for( unsigned int i = 0; i < currentC.idMod.size(); i++ )
		{
			if( tmpnt.mods[ currentC.idMod[i] ].type == HARD_BLK )
				expandPara = 1.09;
			else
				expandPara = 1.01;

			if( i < currentC.idMod.size()/2 )
			{
				areaLeft += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
				lchildC.idMod.push_back( currentC.idMod[i] );
				lchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
			}
			else
			{
				areaRight += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
				rchildC.idMod.push_back( currentC.idMod[i] );
				rchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
			}
		}
	}

	midX = currentC.lbX + (int)( (currentW/(double)AMP_PARA) * (areaLeft/(areaLeft + areaRight)) * AMP_PARA );
	//midX = currentC.lbX + (int)( (currentW/(double)AMP_PARA) * (lchildC.modArea/(lchildC.modArea + rchildC.modArea)) * AMP_PARA );

	bool flagNotFit = false;
	for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
	{
		if( tmpnt.mods[ lchildC.idMod[i] ].modW > (midX - currentC.lbX) )
		{
			flagNotFit = true;
			midX = currentC.lbX + tmpnt.mods[ lchildC.idMod[i] ].modW;
		}
	}
	for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
	{
		if( tmpnt.mods[ rchildC.idMod[i] ].modW > (currentC.rtX - midX) )
		{
			flagNotFit = true;
			midX = currentC.rtX - tmpnt.mods[ rchildC.idMod[i] ].modW;
		}
	}

	lchildC.lbX = currentC.lbX;
	lchildC.lbY = currentC.lbY;
	lchildC.rtX = midX;
	lchildC.rtY = currentC.rtY;

	if( lchildC.lbX > lchildC.rtX ) swap( lchildC.lbX, lchildC.rtX );
	if( lchildC.lbY > lchildC.rtY ) swap( lchildC.lbY, lchildC.rtY );

	lchildC.centerX = (lchildC.lbX + lchildC.rtX)/2;
	lchildC.centerY = (lchildC.lbY + lchildC.rtY)/2;
	lchildC.boxArea = (int)( ((lchildC.rtX - lchildC.lbX)/(float)AMP_PARA) * ((lchildC.rtY - lchildC.lbY)/(float)AMP_PARA) );

	rchildC.lbX = midX;
	rchildC.lbY = currentC.lbY;
	rchildC.rtX = currentC.rtX;
	rchildC.rtY = currentC.rtY;

	if( rchildC.lbX > rchildC.rtX ) swap( rchildC.lbX, rchildC.rtX );
	if( rchildC.lbY > rchildC.rtY ) swap( rchildC.lbY, rchildC.rtY );

	rchildC.centerX = (rchildC.lbX + rchildC.rtX)/2;
	rchildC.centerY = (rchildC.lbY + rchildC.rtY)/2;
	rchildC.boxArea = (int)( ((rchildC.rtX - rchildC.lbX)/(float)AMP_PARA) * ((rchildC.rtY - rchildC.lbY)/(float)AMP_PARA) );

	if( flagNotFit )
	{
		AdjustC( currentC, lchildC, rchildC, 'V', tmpnt );
	}

	return 'V';
}

char EPLG::SelectCutH( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt )
{
	int currentH = currentC.rtY - currentC.lbY;
	
	int min_rtY = tmpnt.mods[ currentC.idMod[0] ].GLeftY + tmpnt.mods[ currentC.idMod[0] ].modH;
	int max_lbY = tmpnt.mods[ currentC.idMod[0] ].GLeftY;

	for( unsigned int i = 1; i < currentC.idMod.size(); i++ )
	{
		if( min_rtY > tmpnt.mods[ currentC.idMod[i] ].GLeftY + tmpnt.mods[ currentC.idMod[i] ].modH )
		{
			min_rtY = tmpnt.mods[ currentC.idMod[i] ].GLeftY + tmpnt.mods[ currentC.idMod[i] ].modH;
		}
		if( max_lbY < tmpnt.mods[ currentC.idMod[i] ].GLeftY )
		{
			max_lbY = tmpnt.mods[ currentC.idMod[i] ].GLeftY;
		}
	}

	int tileH = (max_lbY - min_rtY)/200;
	int midY = min_rtY + 51 * tileH;
	int weightY = 0;

	for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
	{
		int width = (int)(tmpnt.mods[ currentC.idMod[j] ].modW/(double)AMP_PARA);
		int height = 0;
		if( tmpnt.mods[ currentC.idMod[j] ].GCenterY < midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH > midY )
		{
			height = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - midY )/(double)AMP_PARA);
			weightY += width * height;
			//weightY += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - midY );
		}
		else if( tmpnt.mods[ currentC.idMod[j] ].GCenterY >= midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY < midY )
		{
			height = (int)(abs( midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY )/(double)AMP_PARA);
			weightY += width * height;
			//weightY += abs( midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY );
		}
	}

	weightY = (int)(weightY * pow((1.0 + abs(100-51)/(float)100), para_pow));

	for( int i = 52; i < 151; i++ )
	{
		int tmp_midY = min_rtY + i * tileH;
		int tmp_weightY = 0;

		for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
		{
			int width = (int)(tmpnt.mods[ currentC.idMod[j] ].modW/(double)AMP_PARA);
			int height = 0;
			if( tmpnt.mods[ currentC.idMod[j] ].GCenterY < tmp_midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH > tmp_midY )
			{
				height = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - tmp_midY )/(double)AMP_PARA);
				tmp_weightY += width * height;
				//tmp_weightY += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - tmp_midY );
			}
			else if( tmpnt.mods[ currentC.idMod[j] ].GCenterY >= tmp_midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY < tmp_midY )
			{
				height = (int)(abs( tmp_midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY )/(double)AMP_PARA);
				tmp_weightY += width * height;
				//tmp_weightY += abs( tmp_midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY );
			}
		}

		tmp_weightY = (int)(tmp_weightY * pow((1.0 + abs(100-i)/(float)100), para_pow));

		if( tmp_weightY < weightY )
		{
			weightY = tmp_weightY;
			midY = tmp_midY;
		}
	}

	double areaTop = 0;
	double areaBot = 0;
	double expandPara = 0;
	for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
	{
		if( tmpnt.mods[ currentC.idMod[j] ].type == HARD_BLK )
			expandPara = 1.09;
		else
			expandPara = 1.01;

		if( tmpnt.mods[ currentC.idMod[j] ].GCenterY < midY )
		{
			areaBot += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
			lchildC.idMod.push_back( currentC.idMod[j] );
			lchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
		}
		else
		{
			areaTop += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
			rchildC.idMod.push_back( currentC.idMod[j] );
			rchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
		}
	}

	if( lchildC.idMod.size() == 0 || rchildC.idMod.size() == 0 )
	{
		midY = currentC.centerY;
		areaTop = 0;
		areaBot = 0;
		expandPara = 0;

		lchildC.idMod.clear();
		lchildC.modArea = 0;
		rchildC.idMod.clear();
		rchildC.modArea = 0;

		for( unsigned int i = 0; i < currentC.idMod.size(); i++ )
		{
			if( tmpnt.mods[ currentC.idMod[i] ].type == HARD_BLK )
				expandPara = 1.09;
			else
				expandPara = 1.01;

			if( i < currentC.idMod.size()/2 )
			{
				areaBot += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
				lchildC.idMod.push_back( currentC.idMod[i] );
				lchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
			}
			else
			{
				areaTop += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
				rchildC.idMod.push_back( currentC.idMod[i] );
				rchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
			}
		}
	}

	midY = currentC.lbY + (int)( (currentH/(double)AMP_PARA) * (areaBot/(areaBot + areaTop)) * AMP_PARA );
	//midY = currentC.lbY + (int)( (currentH/(double)AMP_PARA) * (lchildC.modArea/(double)(lchildC.modArea + rchildC.modArea)) * AMP_PARA);

	bool flagNotFit = false;
	for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
	{
		if( tmpnt.mods[ lchildC.idMod[i] ].modH > (midY - currentC.lbY) )
		{
			flagNotFit = true;
			midY = currentC.lbY + tmpnt.mods[ lchildC.idMod[i] ].modH;
		}
	}
	for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
	{
		if( tmpnt.mods[ rchildC.idMod[i] ].modH > (currentC.rtY - midY) )
		{
			flagNotFit = true;
			midY = currentC.rtY - tmpnt.mods[ rchildC.idMod[i] ].modH;
		}
	}

	lchildC.lbX = currentC.lbX;
	lchildC.lbY = currentC.lbY;
	lchildC.rtX = currentC.rtX;
	lchildC.rtY = midY;

	if( lchildC.lbX > lchildC.rtX ) swap( lchildC.lbX, lchildC.rtX );
	if( lchildC.lbY > lchildC.rtY ) swap( lchildC.lbY, lchildC.rtY );

	lchildC.centerX = (lchildC.lbX + lchildC.rtX)/2;
	lchildC.centerY = (lchildC.lbY + lchildC.rtY)/2;
	lchildC.boxArea = (int)( ((lchildC.rtX - lchildC.lbX)/(float)AMP_PARA) * ((lchildC.rtY - lchildC.lbY)/(float)AMP_PARA) );

	rchildC.lbX = currentC.lbX;
	rchildC.lbY = midY;
	rchildC.rtX = currentC.rtX;
	rchildC.rtY = currentC.rtY;

	if( rchildC.lbX > rchildC.rtX ) swap( rchildC.lbX, rchildC.rtX );
	if( rchildC.lbY > rchildC.rtY ) swap( rchildC.lbY, rchildC.rtY );

	rchildC.centerX = (rchildC.lbX + rchildC.rtX)/2;
	rchildC.centerY = (rchildC.lbY + rchildC.rtY)/2;
	rchildC.boxArea = (int)( ((rchildC.rtX - rchildC.lbX)/(float)AMP_PARA) * ((rchildC.rtY - rchildC.lbY)/(float)AMP_PARA) );

	if( flagNotFit )
	{
		AdjustC( currentC, lchildC, rchildC, 'H', tmpnt );
	}

	return 'H';
}

char EPLG::SelectCutHV( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt )
{
	int currentW = currentC.rtX - currentC.lbX;
	int currentH = currentC.rtY - currentC.lbY;

	int min_rtX = tmpnt.mods[ currentC.idMod[0] ].GLeftX + tmpnt.mods[ currentC.idMod[0] ].modW;
	int max_lbX = tmpnt.mods[ currentC.idMod[0] ].GLeftX;

	int min_rtY = tmpnt.mods[ currentC.idMod[0] ].GLeftY + tmpnt.mods[ currentC.idMod[0] ].modH;
	int max_lbY = tmpnt.mods[ currentC.idMod[0] ].GLeftY;

	for( unsigned int i = 1; i < currentC.idMod.size(); i++ )
	{
		if( min_rtX > tmpnt.mods[ currentC.idMod[i] ].GLeftX + tmpnt.mods[ currentC.idMod[i] ].modW )
			min_rtX = tmpnt.mods[ currentC.idMod[i] ].GLeftX + tmpnt.mods[ currentC.idMod[i] ].modW;
		if( max_lbX < tmpnt.mods[ currentC.idMod[i] ].GLeftX )
			max_lbX = tmpnt.mods[ currentC.idMod[i] ].GLeftX;

		if( min_rtY > tmpnt.mods[ currentC.idMod[i] ].GLeftY + tmpnt.mods[ currentC.idMod[i] ].modH )
			min_rtY = tmpnt.mods[ currentC.idMod[i] ].GLeftY + tmpnt.mods[ currentC.idMod[i] ].modH;
		if( max_lbY < tmpnt.mods[ currentC.idMod[i] ].GLeftY )
			max_lbY = tmpnt.mods[ currentC.idMod[i] ].GLeftY;
	}

	//int centerX = (int)((currentC.rtX + currentC.lbX) * 0.5);
	//int centerY = (int)((currentC.rtY + currentC.lbY) * 0.5);
	
	int tileW = (max_lbX - min_rtX)/200;
	int tileH = (max_lbY - min_rtY)/200;

	int midX = min_rtX + 51 * tileW;
	int midY = min_rtY + 51 * tileH;

	int weightX = 0;
	int weightY = 0;
	
	for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
	{
		int widthX = 0;
		int heightX = (int)(tmpnt.mods[ currentC.idMod[j] ].modH/(double)AMP_PARA);
		if( tmpnt.mods[ currentC.idMod[j] ].GCenterX < midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW > midX )
		{
			widthX = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - midX )/(double)AMP_PARA);
			weightX += widthX * heightX;
			//weightX += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - midX );
		}
		else if( tmpnt.mods[ currentC.idMod[j] ].GCenterX >= midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX < midX )
		{
			widthX = (int)(abs( midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX )/(double)AMP_PARA);
			weightX += widthX * heightX;
			//weightX += abs( midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX );
		}

		int widthY = (int)(tmpnt.mods[ currentC.idMod[j] ].modW/(double)AMP_PARA);
		int heightY = 0;
		if( tmpnt.mods[ currentC.idMod[j] ].GCenterY < midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH > midY )
		{
			heightY = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - midY )/(double)AMP_PARA);
			weightY += widthY * heightY;
			//weightY += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - midY );
		}
		else if( tmpnt.mods[ currentC.idMod[j] ].GCenterY >= midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY < midY )
		{
			heightY = (int)(abs( midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY )/(double)AMP_PARA);
			weightY += widthY * heightY;
			//weightY += abs( midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY );
		}
	}
	
	weightX = (int)(weightX * pow((1.0 + abs(100-51)/(float)100), para_pow));
	weightY = (int)(weightY * pow((1.0 + abs(100-51)/(float)100), para_pow));

	for( int i = 52; i < 151; i++ )
	{
		int tmp_midX = min_rtX + i * tileW;
		int tmp_weightX = 0;

		int tmp_midY = min_rtY + i * tileH;
		int tmp_weightY = 0;

		for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
		{
			int widthX = 0;
			int heightX = (int)(tmpnt.mods[ currentC.idMod[j] ].modH/(double)AMP_PARA);
			if( tmpnt.mods[ currentC.idMod[j] ].GCenterX < tmp_midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW > tmp_midX )
			{
				widthX = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - tmp_midX )/(double)AMP_PARA);
				tmp_weightX += widthX * heightX;
				//weightX += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftX + tmpnt.mods[ currentC.idMod[j] ].modW - tmp_midX );
			}
			else if( tmpnt.mods[ currentC.idMod[j] ].GCenterX >= tmp_midX && tmpnt.mods[ currentC.idMod[j] ].GLeftX < tmp_midX )
			{
				widthX = (int)(abs( tmp_midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX )/(double)AMP_PARA);
				tmp_weightX += widthX * heightX;
				//weightX += abs( tmp_midX - tmpnt.mods[ currentC.idMod[j] ].GLeftX );
			}

			int widthY = (int)(tmpnt.mods[ currentC.idMod[j] ].modW/(double)AMP_PARA);
			int heightY = 0;
			if( tmpnt.mods[ currentC.idMod[j] ].GCenterY < tmp_midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH > tmp_midY )
			{
				heightY = (int)(abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - tmp_midY )/(double)AMP_PARA);
				tmp_weightY += widthY * heightY;
				//weightY += abs( tmpnt.mods[ currentC.idMod[j] ].GLeftY + tmpnt.mods[ currentC.idMod[j] ].modH - tmp_midY );
			}
			else if( tmpnt.mods[ currentC.idMod[j] ].GCenterY >= tmp_midY && tmpnt.mods[ currentC.idMod[j] ].GLeftY < tmp_midY )
			{
				heightY = (int)(abs( tmp_midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY )/(double)AMP_PARA);
				tmp_weightY += widthY * heightY;
				//weightY += abs( tmp_midY - tmpnt.mods[ currentC.idMod[j] ].GLeftY );
			}
		}

		tmp_weightX = (int)(tmp_weightX * pow((1.0 + abs(100-i)/(float)100), para_pow));
		tmp_weightY = (int)(tmp_weightY * pow((1.0 + abs(100-i)/(float)100), para_pow));

		if( tmp_weightX < weightX )
		{
			weightX = tmp_weightX;
			midX = tmp_midX;
		}
		if( tmp_weightY < weightY )
		{
			weightY = tmp_weightY;
			midY = tmp_midY;
		}
	}

	if( weightY < weightX )
	{
		double areaTop = 0;
		double areaBot = 0;
		double expandPara = 0;

		for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
		{
			if( tmpnt.mods[ currentC.idMod[j] ].type == HARD_BLK )
				expandPara = 1.09;
			else
				expandPara = 1.01;

			if( tmpnt.mods[ currentC.idMod[j] ].GCenterY < midY )
			{
				areaBot += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
				lchildC.idMod.push_back( currentC.idMod[j] );
				lchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
			}
			else
			{
				areaTop += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
				rchildC.idMod.push_back( currentC.idMod[j] );
				rchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
			}
		}
		if( lchildC.idMod.size() == 0 || rchildC.idMod.size() == 0 )
		{
			midY = currentC.centerY;
			areaTop = 0;
			areaBot = 0;
			expandPara = 0;

			lchildC.idMod.clear();
			lchildC.modArea = 0;
			rchildC.idMod.clear();
			rchildC.modArea = 0;

			for( unsigned int i = 0; i < currentC.idMod.size(); i++ )
			{
				if( tmpnt.mods[ currentC.idMod[i] ].type == HARD_BLK )
					expandPara = 1.09;
				else
					expandPara = 1.01;

				if( i < currentC.idMod.size()/2 )
				{
					areaBot += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
					lchildC.idMod.push_back( currentC.idMod[i] );
					lchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
				}
				else
				{
					areaTop += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
					rchildC.idMod.push_back( currentC.idMod[i] );
					rchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
				}
			}
		}

		midY = currentC.lbY + (int)( (currentH/(double)AMP_PARA) * (areaBot/(areaBot+ areaTop)) * AMP_PARA );
		//midY = currentC.lbY + (int)( (currentH/(double)AMP_PARA) * (lchildC.modArea/(double)(lchildC.modArea + rchildC.modArea)) * AMP_PARA);

		bool flagNotFit = false;
		for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
		{
			if( tmpnt.mods[ lchildC.idMod[i] ].modH > (midY - currentC.lbY) )
			{
				flagNotFit = true;
				midY = tmpnt.mods[ lchildC.idMod[i] ].modH + currentC.lbY;
			}
		}
		for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
		{
			if( tmpnt.mods[ rchildC.idMod[i] ].modH > (currentC.rtY - midY) )
			{
				flagNotFit = true;
				midY = currentC.rtY - tmpnt.mods[ rchildC.idMod[i] ].modH;
			}
		}

		lchildC.lbX = currentC.lbX;
		lchildC.lbY = currentC.lbY;
		lchildC.rtX = currentC.rtX;
		lchildC.rtY = midY;

		if( lchildC.lbX > lchildC.rtX ) swap( lchildC.lbX, lchildC.rtX );
		if( lchildC.lbY > lchildC.rtY ) swap( lchildC.lbY, lchildC.rtY );

		lchildC.centerX = (lchildC.lbX + lchildC.rtX)/2;
		lchildC.centerY = (lchildC.lbY + lchildC.rtY)/2;
		lchildC.boxArea = (int)( ((lchildC.rtX - lchildC.lbX)/(float)AMP_PARA) * ((lchildC.rtY - lchildC.lbY)/(float)AMP_PARA) );

		rchildC.lbX = currentC.lbX;
		rchildC.lbY = midY;
		rchildC.rtX = currentC.rtX;
		rchildC.rtY = currentC.rtY;

		if( rchildC.lbX > rchildC.rtX ) swap( rchildC.lbX, rchildC.rtX );
		if( rchildC.lbY > rchildC.rtY ) swap( rchildC.lbY, rchildC.rtY );

		rchildC.centerX = (rchildC.rtX + rchildC.lbX)/2;
		rchildC.centerY = (rchildC.rtY + rchildC.lbY)/2;
		rchildC.boxArea = (int)( ((rchildC.rtX - rchildC.lbX)/(float)AMP_PARA) * ((rchildC.rtY - rchildC.lbY)/(float)AMP_PARA) );

		if( flagNotFit )
		{
			AdjustC( currentC, lchildC, rchildC, 'H', tmpnt );
		}

		return 'H';
	}
	else
	{
		double areaLeft = 0;
		double areaRight = 0;
		double expandPara = 0;
		for( unsigned int j = 0; j < currentC.idMod.size(); j++ )
		{
			if( tmpnt.mods[ currentC.idMod[j] ].type == HARD_BLK )
				expandPara = 1.09;
			else
				expandPara = 1.01;

			if( tmpnt.mods[ currentC.idMod[j] ].GCenterX < midX )
			{
				areaLeft += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
				lchildC.idMod.push_back( currentC.idMod[j] );
				lchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
			}
			else
			{
				areaRight += expandPara * (double)tmpnt.mods[ currentC.idMod[j] ].modArea;
				rchildC.idMod.push_back( currentC.idMod[j] );
				rchildC.modArea += tmpnt.mods[ currentC.idMod[j] ].modArea;
			}
		}

		if(lchildC.idMod.size() == 0 || rchildC.idMod.size() == 0 )
		{
			midX = currentC.centerX;
			areaLeft = 0;
			areaRight = 0;
			expandPara = 0;

			lchildC.idMod.clear();
			lchildC.modArea = 0;
			rchildC.idMod.clear();
			rchildC.modArea = 0;

			for( unsigned int i = 0; i < currentC.idMod.size(); i++ )
			{
				if( tmpnt.mods[ currentC.idMod[i] ].type == HARD_BLK )
					expandPara = 1.09;
				else
					expandPara = 1.01;

				if( i < currentC.idMod.size()/2 )
				{
					areaLeft += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
					lchildC.idMod.push_back( currentC.idMod[i] );
					lchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
				}
				else
				{
					areaRight += expandPara * (double)tmpnt.mods[ currentC.idMod[i] ].modArea;
					rchildC.idMod.push_back( currentC.idMod[i] );
					rchildC.modArea += tmpnt.mods[ currentC.idMod[i] ].modArea;
				}
			}
		}

		midX = currentC.lbX + (int)( (currentW/(double)AMP_PARA) * (areaLeft/(areaLeft + areaRight)) * AMP_PARA );
		//midX = currentC.lbX + (int)( (currentW/(double)AMP_PARA) * (lchildC.modArea/(double)(lchildC.modArea + rchildC.modArea)) * AMP_PARA);


		bool flagNotFit = false;
		for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
		{
			if( tmpnt.mods[ lchildC.idMod[i] ].modW > (midX - currentC.lbX) )
			{
				flagNotFit = true;
				midX = currentC.lbX + tmpnt.mods[ lchildC.idMod[i] ].modW;
			}
		}
		for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
		{
			if( tmpnt.mods[ rchildC.idMod[i] ].modW > (currentC.rtX - midX) )
			{
				flagNotFit = true;
				midX = currentC.rtX - tmpnt.mods[ rchildC.idMod[i] ].modW;
			}
		}

		lchildC.lbX = currentC.lbX;
		lchildC.lbY = currentC.lbY;
		lchildC.rtX = midX;
		lchildC.rtY = currentC.rtY;

		if( lchildC.lbX > lchildC.rtX ) swap( lchildC.lbX, lchildC.rtX );
		if( lchildC.lbY > lchildC.rtY ) swap( lchildC.lbY, lchildC.rtY );

		lchildC.centerX = (lchildC.lbX + lchildC.rtX)/2;
		lchildC.centerY = (lchildC.lbY + lchildC.rtY)/2;
		lchildC.boxArea = (int)( ((lchildC.rtX - lchildC.lbX)/(float)AMP_PARA) * ((lchildC.rtY - lchildC.lbY)/(float)AMP_PARA) );

		rchildC.lbX = midX;
		rchildC.lbY = currentC.lbY;
		rchildC.rtX = currentC.rtX;
		rchildC.rtY = currentC.rtY;

		if( rchildC.lbX > rchildC.rtX ) swap( rchildC.lbX, rchildC.rtX );
		if( rchildC.lbY > rchildC.rtY ) swap( rchildC.lbY, rchildC.rtY );

		rchildC.centerX = (rchildC.lbX + rchildC.rtX)/2;
		rchildC.centerY = (rchildC.lbY + rchildC.rtY)/2;
		rchildC.boxArea = (int)( ((rchildC.rtX - rchildC.lbX)/(float)AMP_PARA) * ((rchildC.rtY - rchildC.lbY)/(float)AMP_PARA) );

		if( flagNotFit )
		{
			AdjustC( currentC, lchildC, rchildC, 'V', tmpnt );
		}

		return 'V';
	}
}

void EPLG::AdjustC( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, char VHcut, NETLIST &tmpnt )
{
	if( VHcut == 'V' )
	{
		if( lchildC.modArea/(double)lchildC.boxArea > rchildC.modArea/(double)rchildC.boxArea )
		{
			for( unsigned int i = 0; i < lchildC.idMod.size()-1; i++ )
			{
				for( unsigned int j = i+1; j < lchildC.idMod.size(); j++ )
				{
					if( (lchildC.rtX - tmpnt.mods[ lchildC.idMod[i] ].GCenterX) <
					        (lchildC.rtX - tmpnt.mods[ lchildC.idMod[j] ].GCenterX) )
					{
						swap( lchildC.idMod[i], lchildC.idMod[j] );
					}
				}
			}
			int num = (int)lchildC.idMod.size() - 1;
			for( int i = num; i > 0; i-- )
			{
				int modID = lchildC.idMod[i];
				if( (rchildC.modArea + tmpnt.mods[modID].modArea)/(double)rchildC.boxArea >
				        (lchildC.modArea - tmpnt.mods[modID].modArea)/(double)lchildC.boxArea )
				{
					break;
				}
				rchildC.idMod.push_back( modID );
				rchildC.modArea += tmpnt.mods[modID].modArea;
				lchildC.idMod.erase( lchildC.idMod.begin() + i );
				lchildC.modArea -= tmpnt.mods[modID].modArea;
			}
		}
		else if( rchildC.modArea/(double)rchildC.boxArea > lchildC.modArea/(double)lchildC.boxArea )
		{
			for( unsigned int i = 0; i < rchildC.idMod.size()-1; i++ )
			{
				for( unsigned int j = i+1; j < rchildC.idMod.size(); j++ )
				{
					if( (tmpnt.mods[ rchildC.idMod[i] ].GCenterX - rchildC.lbX ) <
					        (tmpnt.mods[ rchildC.idMod[j] ].GCenterX - rchildC.lbX) )
					{
						swap( rchildC.idMod[i], rchildC.idMod[j] );
					}
				}
			}
			int num = (int)rchildC.idMod.size() - 1;
			for( int i = num; i > 0; i-- )
			{
				int modID = rchildC.idMod[i];
				if( (lchildC.modArea + tmpnt.mods[modID].modArea)/(double)lchildC.boxArea >
				        (rchildC.modArea - tmpnt.mods[modID].modArea)/(double)rchildC.boxArea )
				{
					break;
				}
				lchildC.idMod.push_back( modID );
				lchildC.modArea += tmpnt.mods[modID].modArea;
				rchildC.idMod.erase( rchildC.idMod.begin() + i );
				rchildC.modArea -= tmpnt.mods[modID].modArea;
			}
		}
	}
	else if( VHcut == 'H' )
	{
		if( lchildC.modArea/(double)lchildC.boxArea > rchildC.modArea/(double)rchildC.boxArea )
		{
			for( unsigned int i = 0; i < lchildC.idMod.size()-1; i++ )
			{
				for( unsigned int j = i+1; j < lchildC.idMod.size(); j++ )
				{
					if( (lchildC.rtY - tmpnt.mods[ lchildC.idMod[i] ].GCenterY) <
					        (lchildC.rtY - tmpnt.mods[ lchildC.idMod[j] ].GCenterY) )
					{
						swap( lchildC.idMod[i], lchildC.idMod[j] );
					}
				}
			}
			int num = lchildC.idMod.size() - 1;
			for( int i = num; i > 0; i-- )
			{
				int modID = lchildC.idMod[i];
				if( (rchildC.modArea + tmpnt.mods[modID].modArea)/(double)rchildC.boxArea >
				        (lchildC.modArea - tmpnt.mods[modID].modArea)/(double)lchildC.boxArea )
				{
					break;
				}
				rchildC.idMod.push_back( modID );
				rchildC.modArea += tmpnt.mods[modID].modArea;
				lchildC.idMod.erase( lchildC.idMod.begin() + i );
				lchildC.modArea -= tmpnt.mods[modID].modArea;
			}
		}
		else if( rchildC.modArea/(double)rchildC.boxArea > lchildC.modArea/(double)lchildC.boxArea )
		{
			for( unsigned int i = 0; i < rchildC.idMod.size()-1; i++ )
			{
				for( unsigned int j = i+1; j < rchildC.idMod.size(); j++ )
				{
					if( (tmpnt.mods[ rchildC.idMod[i] ].GCenterY - rchildC.lbY) <
					        (tmpnt.mods[ rchildC.idMod[j] ].GCenterY - rchildC.lbY) )
					{
						swap( rchildC.idMod[i], rchildC.idMod[j] );
					}
				}
			}
			int num = rchildC.idMod.size() - 1;
			for( int i = num; i > 0; i-- )
			{
				int modID = rchildC.idMod[i];
				if( (lchildC.modArea + tmpnt.mods[modID].modArea)/(double)lchildC.boxArea >
				        (rchildC.modArea - tmpnt.mods[modID].modArea)/(double)rchildC.boxArea )
				{
					break;
				}
				lchildC.idMod.push_back( modID );
				lchildC.modArea += tmpnt.mods[modID].modArea;
				rchildC.idMod.erase( rchildC.idMod.begin() + i );
				rchildC.modArea -= tmpnt.mods[modID].modArea;
			}
		}
	}
}

void EPLG::ConstructTree( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, char VHcut, NETLIST &tmpnt )
{
	if( lchildC.lbX > lchildC.rtX || lchildC.lbY > lchildC.rtY )
	{
		cout << "error  : wrong left child boundary "
		     << "(" << lchildC.lbX << ", " << lchildC.lbY << ")" << "-"
		     << "(" << lchildC.rtX << ", " << lchildC.rtY << ")" << endl;
	}
	if( rchildC.lbX > rchildC.rtX || rchildC.lbY > rchildC.rtY )
	{
		cout << "error  : wrong left child boundary "
		     << "(" << rchildC.lbX << ", " << rchildC.lbY << ")" << "-"
		     << "(" << rchildC.rtX << ", " << rchildC.rtY << ")" << endl;
	}
	if( lchildC.lbX > lchildC.rtX || lchildC.lbY > lchildC.rtY || rchildC.lbX > rchildC.rtX || rchildC.lbY > rchildC.rtY )
	{
		exit(EXIT_FAILURE);
	}

	RGST[currentC.idClu].VHcut = VHcut;

	if( VHcut == 'V' )
	{
		for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
		{
			int modID = lchildC.idMod[i];
			if( tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW > lchildC.rtX )
			{
				tmpnt.mods[modID].GLeftX = lchildC.rtX - tmpnt.mods[modID].modW;
				tmpnt.mods[modID].GCenterX = tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW/2;
			}
		}
		for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
		{
			int modID = rchildC.idMod[i];
			if( tmpnt.mods[modID].GLeftX < rchildC.lbX )
			{
				tmpnt.mods[modID].GLeftX = rchildC.lbX;
				tmpnt.mods[modID].GCenterX = tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW/2;
			}
		}
	}
	else if( VHcut == 'H' )
	{
		for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
		{
			int modID = lchildC.idMod[i];
			if( tmpnt.mods[modID].GLeftY + tmpnt.mods[modID].modH > lchildC.rtY )
			{
				tmpnt.mods[modID].GLeftY = lchildC.rtY - tmpnt.mods[modID].modH;
				tmpnt.mods[modID].GCenterY = tmpnt.mods[modID].GLeftY + tmpnt.mods[modID].modH/2;
			}
		}
		for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
		{
			int modID = rchildC.idMod[i];
			if( tmpnt.mods[modID].GLeftY < rchildC.lbY )
			{
				tmpnt.mods[modID].GLeftY = rchildC.lbY;
				tmpnt.mods[modID].GCenterY = tmpnt.mods[modID].GLeftY+ tmpnt.mods[modID].modH/2;
			}
		}
	}

	if( (int)lchildC.idMod.size() > numBase )
	{
		bool flagNotFit = false;
		for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
		{
			int modID = lchildC.idMod[i];
			if( tmpnt.mods[modID].GLeftX < lchildC.lbX || tmpnt.mods[modID].GLeftY < lchildC.lbY )
			{
				flagNotFit = true;
			}
		}
		if( !flagNotFit )
		{
			RedistributeC( lchildC, tmpnt );
		}
	}

	if( (int)rchildC.idMod.size() > numBase )
	{
		bool flagNotFit = false;
		for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
		{
			int modID = rchildC.idMod[i];
			if( tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW > rchildC.rtX || tmpnt.mods[modID].GLeftY + tmpnt.mods[modID].modH > rchildC.rtY )
			{
				flagNotFit = true;
			}
		}
		if( !flagNotFit )
		{
			RedistributeC( rchildC, tmpnt );
		}
	}

	// left tree
	RGST[numTree].id = numTree;
	RGST[numTree].lbX = lchildC.lbX;
	RGST[numTree].lbY = lchildC.lbY;
	RGST[numTree].rtX = lchildC.rtX;
	RGST[numTree].rtY = lchildC.rtY;
	RGST[numTree].treeCoordX = (RGST[numTree].lbX + RGST[numTree].rtX)/2;
	RGST[numTree].treeCoordY = (RGST[numTree].lbY + RGST[numTree].rtY)/2;
	RGST[numTree].parentTree = &RGST[currentC.idClu];
	RGST[currentC.idClu].leftTree = &RGST[numTree];
	lchildC.idClu = numTree;
	numTree++;

	// right tree
	RGST[numTree].id = numTree;
	RGST[numTree].lbX = rchildC.lbX;
	RGST[numTree].lbY = rchildC.lbY;
	RGST[numTree].rtX = rchildC.rtX;
	RGST[numTree].rtY = rchildC.rtY;
	RGST[numTree].treeCoordX = (RGST[numTree].lbX + RGST[numTree].rtX)/2;
	RGST[numTree].treeCoordY = (RGST[numTree].lbY + RGST[numTree].rtY)/2;
	RGST[numTree].parentTree = &RGST[currentC.idClu];
	RGST[currentC.idClu].rightTree = &RGST[numTree];
	rchildC.idClu = numTree;
	numTree++;
}

void EPLG::RedistributeC( GROUP &c, NETLIST &tmpnt )
{
	DG_X = new double [c.idMod.size()];
	DG_Y = new double [c.idMod.size()];
	GX_0 = new double [c.idMod.size()];
	GX_1 = new double [c.idMod.size()];
	GY_0 = new double [c.idMod.size()];
	GY_1 = new double [c.idMod.size()];
	DX_0 = new double [c.idMod.size()];
	DX_1 = new double [c.idMod.size()];
	DY_0 = new double [c.idMod.size()];
	DY_1 = new double [c.idMod.size()];
	alpha = new double [c.idMod.size()];
	Cv = new double [c.idMod.size()];

	CutBinC( c );
	InitializeGradientC( c, tmpnt );
	SolveCoordinateC( c, tmpnt );

	bin2D.clear();
	BIN2D (bin2D).swap(bin2D);

	delete [] DG_X;
	delete [] DG_Y;
	delete [] GX_0;
	delete [] GX_1;
	delete [] GY_0;
	delete [] GY_1;
	delete [] DX_0;
	delete [] DX_1;
	delete [] DY_0;
	delete [] DY_1;
	delete [] alpha;
	delete [] Cv;
}

void EPLG::CutBinC( GROUP &c )
{
	int w = c.rtX - c.lbX;
	int h = c.rtY - c.lbY;

	nBinRow = nBinCol = (int)sqrt( (float)c.idMod.size() ) * 5;

	bin2D.resize(nBinRow);
	for(int i = 0; i < nBinRow; i++)
	{
		bin2D[i].resize( nBinCol );
	}

	binW = (int)( (w/((float)AMP_PARA)) / ((float)nBinCol) * AMP_PARA );
	binH = (int)( (h/((float)AMP_PARA)) / ((float)nBinRow) * AMP_PARA );

	for( int i = 0; i < nBinRow; i++ )
	{
		for( int j = 0; j < nBinCol; j++ )
		{
			bin2D[i][j].w = binW;
			bin2D[i][j].h = binH;
			bin2D[i][j].x = c.lbX + j*binW;
			bin2D[i][j].y = c.lbY + i*binH;
			bin2D[i][j].EnableValue = binW/(float)AMP_PARA * binH/(float)AMP_PARA;
		}
	}
}
void EPLG::UpdateClusterChild( GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt )
{
	for( unsigned int i = 0; i < lchildC.idMod.size(); i++ )
	{
		if( tmpnt.mods[ lchildC.idMod[i] ].flagTSV )
			lchildC.nTSV++;
		else
			lchildC.nMod++;

		if( lchildC.maxModW < tmpnt.mods[ lchildC.idMod[i] ].modW )
			lchildC.maxModW = tmpnt.mods[ lchildC.idMod[i] ].modW;
		if( lchildC.maxModH < tmpnt.mods[ lchildC.idMod[i] ].modH )
			lchildC.maxModH = tmpnt.mods[ lchildC.idMod[i] ].modH;

		if( !tmpnt.mods[ lchildC.idMod[i] ].flagTSV )
			lchildC.avgModL += ( (tmpnt.mods[ lchildC.idMod[i] ].modW + tmpnt.mods[ lchildC.idMod[i] ].modH)/2 );
	}
	if( lchildC.nMod != 0 )
		lchildC.avgModL = (int)(lchildC.avgModL/(float)lchildC.nMod);

	for( unsigned int i = 0; i < rchildC.idMod.size(); i++ )
	{
		if( tmpnt.mods[ rchildC.idMod[i] ].flagTSV )
			rchildC.nTSV++;
		else
			rchildC.nMod++;

		if( rchildC.maxModW < tmpnt.mods[ rchildC.idMod[i] ].modW )
			rchildC.maxModW = tmpnt.mods[ rchildC.idMod[i] ].modW;
		if( rchildC.maxModH < tmpnt.mods[ rchildC.idMod[i] ].modH )
			rchildC.maxModH = tmpnt.mods[ rchildC.idMod[i] ].modH;

		if( !tmpnt.mods[ rchildC.idMod[i] ].flagTSV )
			rchildC.avgModL += ( (tmpnt.mods[ rchildC.idMod[i] ].modW + tmpnt.mods[ rchildC.idMod[i] ].modH)/2 );
	}
	if( rchildC.nMod != 0 )
		rchildC.avgModL = (int)(rchildC.avgModL/(float)rchildC.nMod);

}
void EPLG::InitializeGradientC( GROUP &c, NETLIST &tmpnt )
{
	double BDterm = CalDensityGradientC( c, tmpnt );

	Fk_1 = BDterm;

	for( unsigned int i = 0; i < c.idMod.size(); i++ )
	{
		GX_1[i] = DG_X[i];
		GY_1[i] = DG_Y[i];
	}

	for( unsigned int i = 0; i < c.idMod.size(); i++ )
	{
		GX_0[i] = GX_1[i];
		GY_0[i] = GY_1[i];
		DX_0[i] = 0;
		DY_0[i] = 0;
	}
}
void EPLG::SolveCoordinateC( GROUP &c, NETLIST &tmpnt )
{
	int count = 0;

	int *originX = new int [c.idMod.size()];
	int *originY = new int [c.idMod.size()];

	while(1)
	{
		if(count > 0)
		{
			overflow_area_0 = overflow_area_1;
		}

		double beta;
		double beta_upper = 0;
		double beta_lower = 0;
		for( unsigned int i = 0; i < c.idMod.size(); i++ )
		{
			beta_upper += ( (GX_1[i]*(GX_1[i]-GX_0[i])) + (GY_1[i]*(GY_1[i]-GY_0[i])) );
			beta_lower += ( fabs(GX_0[i]) + fabs(GY_0[i]) );
		}
		beta_lower *= beta_lower;
		beta = beta_upper/beta_lower;
		for( unsigned int i = 0; i < c.idMod.size(); i++ )
		{
			DX_1[i] = -GX_1[i] + beta*DX_0[i];
			DY_1[i] = -GY_1[i] + beta*DY_0[i];
		}

		double alpha_lower = 0;
		double s = 0.2;
		for( unsigned int i = 0; i < c.idMod.size(); i++ )
		{
			alpha_lower = sqrt( DX_1[i]*DX_1[i] + DY_1[i]*DY_1[i] );
			alpha[i] = (s * binW/(float)AMP_PARA)/alpha_lower;
		}

		for( unsigned int l = 0; l < c.idMod.size(); l++ )
		{
			int i = c.idMod[l];

			originX[l] = tmpnt.mods[i].GCenterX;
			originY[l] = tmpnt.mods[i].GCenterY;

			tmpnt.mods[i].GCenterX += (int)(alpha[l] * DX_1[l] * AMP_PARA);
			tmpnt.mods[i].GLeftX += (int)(alpha[l] * DX_1[l] * AMP_PARA);
			if( tmpnt.mods[i].GLeftX < c.lbX )
			{
				tmpnt.mods[i].GLeftX = c.lbX;
				tmpnt.mods[i].GCenterX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW/2;
			}
			if( tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW > c.rtX )
			{
				tmpnt.mods[i].GLeftX = c.rtX - tmpnt.mods[i].modW;
				tmpnt.mods[i].GCenterX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW/2;
			}

			tmpnt.mods[i].GCenterY += (int)(alpha[l] * DY_1[l] * AMP_PARA);
			tmpnt.mods[i].GLeftY += (int)(alpha[l] * DY_1[l] * AMP_PARA);
			if( tmpnt.mods[i].GLeftY < c.lbY )
			{
				tmpnt.mods[i].GLeftY = c.lbY;
				tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
			}
			if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > c.rtY )
			{
				tmpnt.mods[i].GLeftY = c.rtY - tmpnt.mods[i].modH;
				tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
			}
		}

		for( unsigned int i = 0; i < c.idMod.size(); i++ )
		{
			GX_0[i] = GX_1[i];
			GY_0[i] = GY_1[i];
			DX_0[i] = DX_1[i];
			DY_0[i] = DY_1[i];
		}

		Fk_0 = Fk_1;
		bool flag = false;
		double BDterm = CalDensityGradientC( c, tmpnt );
		Fk_1 = BDterm;
		if( count > 0 )
		{
			if( overflow_area_1/(double)c.modArea < 0 && (overflow_area_1 - overflow_area_0) >= 0 )
			{
				flag = true;
			}
		}
		if( flag )
			break;

		count++;

		if( Fk_1 > Fk_0 || count >= 100 )
		{
			for( unsigned int l = 0; l < c.idMod.size(); l++ )
			{
				int i = c.idMod[l];
				tmpnt.mods[i].GCenterX = originX[l];
				tmpnt.mods[i].GLeftX = originX[l] - tmpnt.mods[i].modW/2;
				tmpnt.mods[i].GCenterY = originY[l];
				tmpnt.mods[i].GLeftY = originY[l] - tmpnt.mods[i].modH/2;
			}
			numIter += count;
			break;
		}

		for( unsigned int i = 0; i < c.idMod.size(); i++ )
		{
			GX_1[i] = DG_X[i];
			GY_1[i] = DG_Y[i];
		}
	}

	delete [] originX;
	delete [] originY;
}
double EPLG::CalDensityGradientC( GROUP &c, NETLIST &tmpnt )
{
	for( int i = 0; i < nBinRow; i++ )
	{
		for( int j = 0; j < nBinCol; j++ )
		{
			bin2D[i][j].OccupyValue = 0;
		}
	}

	double totalBinDensity = 0;

	for( unsigned int l = 0; l < c.idMod.size(); l++ )
	{
		int i = c.idMod[l];

		int colL = (tmpnt.mods[i].GLeftX - c.lbX)/binW - 2;
		int rowB = (tmpnt.mods[i].GLeftY - c.lbY)/binH - 2;
		int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW - c.lbX)/binW + 2;
		int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH - c.lbY)/binH + 2;

		if( colL < 0 ) colL = 0;
		if( rowB < 0 ) rowB = 0;
		if( colR >= nBinCol ) colR = nBinCol - 1;
		if( rowT >= nBinRow ) rowT = nBinRow - 1;

		double Db = 0;

		double *P_X = new double[ colR - colL + 1 ];
		double *P_Y = new double[ rowT - rowB + 1 ];
		double wb = binW/(float)AMP_PARA;
		double wv = tmpnt.mods[i].modW/(float)AMP_PARA;
		double hb = binH/(float)AMP_PARA;
		double hv = tmpnt.mods[i].modH/(float)AMP_PARA;
		double ax = 4/((wv+2*wb) * (wv+4*wb));
		double bx = 2/(wb * (wb*4+wv));
		double ay = 4/((hv+2*hb) * (hv+4*hb));
		double by = 2/(hb * (hb*4+hv));
		double range1_w = wv/2 + wb;
		double range2_w = wv/2 + 2*wb;
		double range1_h = hv/2 + hb;
		double range2_h = hv/2 + 2*hb;

		for( int k = 0; k <= (rowT-rowB); k++ )
		{
			double disY = (tmpnt.mods[i].GCenterY - bin2D[k+rowB][colL].y - bin2D[k+rowB][colL].h/2) / (float)AMP_PARA;
			if( disY < 0 )
			{
				disY = -disY;
			}

			if( disY <= range1_h && disY >= 0 )
			{
				P_Y[k] = 1 - ay * disY * disY;
			}
			else if( disY <= range2_h && disY > range1_h )
			{
				P_Y[k] = by * (disY-range2_h) * (disY-range2_h);
			}
			else
			{
				P_Y[k] = 0;
			}
		}
		for( int j = 0; j <= (colR-colL); j++ )
		{
			double disX = (tmpnt.mods[i].GCenterX - bin2D[rowB][j+colL].x - bin2D[rowB][j+colL].w/2) / (float)AMP_PARA;
			if( disX < 0 )
			{
				disX = -disX;
			}


			if( disX <= range1_w && disX >= 0 )
			{
				P_X[j] = 1 - ax * disX * disX;
			}
			else if( disX <= range2_w && disX > range1_w )
			{
				P_X[j] = bx * (disX-range2_w) * (disX-range2_w);
			}
			else
			{
				P_X[j] = 0;
			}
		}

		for( int j = 0; j <= (colR-colL); j++ )
		{
			for( int k = 0; k <= (rowT-rowB); k++ )
			{
				Db += (P_X[j] * P_Y[k]);
			}
		}

		Cv[l] = tmpnt.mods[i].modArea/Db;

		for( int j = colL; j <= colR; j++ )
		{
			for( int k = rowB; k <= rowT; k++ )
			{
				bin2D[k][j].OccupyValue += P_X[j-colL] * P_Y[k-rowB] * Cv[l];
			}
		}

		delete [] P_X;
		delete [] P_Y;
	}

	overflow_area_1 = 0;
	for( int i = 0; i < nBinRow; i++ )
	{
		for( int j = 0; j < nBinCol; j++ )
		{
			overflow_area_1 += COM_MAX( (bin2D[i][j].OccupyValue - bin2D[i][j].EnableValue), 0 );
			totalBinDensity += (bin2D[i][j].OccupyValue - bin2D[i][j].EnableValue) *  (bin2D[i][j].OccupyValue - bin2D[i][j].EnableValue);
		}
	}

	for( unsigned int i = 0; i < c.idMod.size(); i++ )
	{
		DG_X[i] = 0;
		DG_Y[i] = 0;
	}

	for( unsigned int l = 0; l < c.idMod.size(); l++ )
	{
		int i = c.idMod[l];

		int colL = (tmpnt.mods[i].GLeftX - c.lbX)/binW - 2;
		int rowB = (tmpnt.mods[i].GLeftY - c.lbY)/binH - 2;
		int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW - c.lbX)/binW + 2;
		int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH - c.lbY)/binH + 2;

		if( colL < 0 ) colL = 0;
		if( rowB < 0 ) rowB = 0;
		if( colR >= nBinCol ) colR = nBinCol - 1;
		if( rowT >= nBinRow ) rowT = nBinRow - 1;

		double *P_X = new double[ colR - colL + 1 ];
		double *P_Y = new double[ rowT - rowB + 1 ];
		double *dP_X = new double[ colR - colL + 1 ];
		double *dP_Y = new double[ rowT - rowB + 1 ];
		double hb = binH/(float)AMP_PARA;
		double wv = tmpnt.mods[i].modW/(float)AMP_PARA;
		double wb = binW/(float)AMP_PARA;
		double hv = tmpnt.mods[i].modH/(float)AMP_PARA;
		double ax = 4/((wv+2*wb) * (wv+4*wb));
		double bx = 2/(wb * (wb*4+wv));
		double ay = 4/((hv+2*hb) * (hv+4*hb));
		double by = 2/(hb * (hb*4+hv));
		double range1_w = wv/2 + wb ;
		double range2_w = wv/2 + wb*2;
		double range1_h = hv/2 + hb;
		double range2_h = hv/2 + hb*2;

		for( int j = rowB; j <= rowT; j++ )
		{
			double disY;
			if( bin2D[j][colL].y + bin2D[j][colL].h/2 >= tmpnt.mods[i].GCenterY )
			{
				disY = (bin2D[j][colL].y + bin2D[j][colL].h/2 - tmpnt.mods[i].GCenterY);
				disY /= (float)AMP_PARA;
				if( disY <= range1_h && disY >= 0 )
				{
					dP_Y[j-rowB] = 2 * ay * disY;
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					dP_Y[j-rowB] = -2 * by * (disY-range2_h);
				}
				else
				{
					dP_Y[j-rowB] = 0;
				}
			}
			else
			{
				disY = (tmpnt.mods[i].GCenterY - bin2D[j][colL].y - bin2D[j][colL].h/2);
				disY /= (float)AMP_PARA;
				if( disY <= range1_h && disY >= 0 )
				{
					dP_Y[j-rowB] = -2 * ay * disY;
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					dP_Y[j-rowB] = 2 * by * (disY-range2_h);
				}
				else
				{
					dP_Y[j-rowB] = 0;
				}
			}

			if( disY <= range1_h && disY >= 0 )
			{
				P_Y[j-rowB] = 1 - ay * disY * disY;
			}
			else if( disY <= range2_h && disY > range1_h )
			{
				P_Y[j-rowB] = by * (disY-range2_h) * (disY-range2_h);
			}
			else
			{
				P_Y[j-rowB] = 0;
			}
		}

		for( int k = colL; k <= colR; k++ )
		{
			double disX;
			if( bin2D[rowB][k].x + bin2D[rowB][k].w/2 >= tmpnt.mods[i].GCenterX )
			{
				disX = (bin2D[rowB][k].x + bin2D[rowB][k].w/2 - tmpnt.mods[i].GCenterX);
				disX /= (float)AMP_PARA;
				if( disX <= range1_w && disX >= 0 )
				{
					dP_X[k-colL] = 2 * ax * disX;
				}
				else if( disX <= range2_w && disX > range1_w )
				{
					dP_X[k-colL] = -2 * bx * (disX-range2_w);
				}
				else
				{
					dP_X[k-colL] = 0;
				}
			}
			else
			{
				disX = (tmpnt.mods[i].GCenterX - bin2D[rowB][k].x - bin2D[rowB][k].w/2);
				disX /= (float)AMP_PARA;
				if( disX <= range1_w && disX >= 0 )
				{
					dP_X[k-colL] = -2 * ax * disX;
				}
				else if( disX <= range2_w && disX > range1_w )
				{
					dP_X[k-colL] = 2 * bx * (disX-range2_w);
				}
				else
				{
					dP_X[k-colL] = 0;
				}
			}

			if( disX <= range1_w && disX >= 0 )
			{
				P_X[k-colL] = 1 - ax * disX * disX;
			}
			else if( disX <= range2_w && disX > range1_w )
			{
				P_X[k-colL] = bx * (disX-range2_w) * (disX-range2_w);
			}
			else
			{
				P_X[k-colL] = 0;
			}
		}

		for( int j = rowB; j <= rowT; j++ )
		{
			for( int k = colL; k <= colR; k++ )
			{
				DG_X[l] += dP_X[k-colL] * P_Y[j-rowB] * Cv[l] * (bin2D[j][k].OccupyValue - bin2D[j][k].EnableValue) * 2;
				DG_Y[l] += P_X[k-colL] * dP_Y[j-rowB] * Cv[l] * (bin2D[j][k].OccupyValue - bin2D[j][k].EnableValue) * 2;
			}
		}

		delete [] P_X;
		delete [] P_Y;
		delete [] dP_X;
		delete [] dP_Y;
	}
	return totalBinDensity;
}
void EPLG::PlotResultC( vector<GROUP> &resultC, NETLIST &tmpnt, char *file )
{
	char fileName[50] = "\0";

	char subName[10] = "\0";
	sprintf( subName, "%d", tmpnt.nLayer );

	strcpy( fileName, file );
	strcat( fileName, "C_" );
	strcat( fileName, subName );
	strcat( fileName, ".m" );

	ofstream fout;
	fout.open( fileName );
	if( !fout.is_open() )
	{
		cout << "error  : unable to open " << fileName << "for plotting the result of clustering" << endl;
		exit(EXIT_FAILURE);
	}

	fout.precision(3);
	fout.setf( ios::fixed, ios::floatfield );

	float llx;
	float lly;
	float urx;
	float ury;

	fout << "figure;" << endl;
	fout << "clear;" << endl;
	fout << "clf;" << endl;
	fout << "axis equal;" << endl;

	int ChipWidth = 0;
	int ChipHeight = 0;
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		ChipWidth = COM_MAX( tmpnt.mods[i].LeftX + tmpnt.mods[i].modW, ChipWidth );
		ChipHeight = COM_MAX( tmpnt.mods[i].LeftY + tmpnt.mods[i].modH, ChipHeight );
	}
	ChipWidth = COM_MAX( tmpnt.ChipWidth, ChipWidth );
	ChipHeight = COM_MAX( tmpnt.ChipHeight, ChipHeight );

	llx = -ChipWidth/(20)/(float)AMP_PARA;
	lly = -ChipHeight/(20)/(float)AMP_PARA;
	urx = ChipWidth/((float)AMP_PARA) + ChipWidth/(20)/(float)AMP_PARA;
	ury = ChipHeight/((float)AMP_PARA) + ChipHeight/(20)/(float)AMP_PARA;

	fout << "axis([ " << llx << " " << urx << " " << lly << " " << ury << "]);" << endl;
	fout << "hold on;" << endl;
	fout << "grid on;" << endl;
	fout << "title('" << fileName << "');" << endl;

	llx = 0;
	lly = 0;
	urx = tmpnt.ChipWidth/((float)AMP_PARA);
	ury = tmpnt.ChipHeight/((float)AMP_PARA);

	// plot fixed-outline
	fout << "% fixed-outline" << endl;
	fout << "line(" << "[ " << llx << " " << llx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
	fout << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << lly << " " << lly << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
	fout << "line(" << "[ " << urx << " " << urx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2'" << ");" << endl;
	fout << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << ury << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;

	char c = 'g';
	for( unsigned int i = 0; i < resultC.size(); i++ )
	{

		for( unsigned int j = 0; j < resultC[i].idMod.size(); j++ )
		{
			int modID = resultC[i].idMod[j];

			llx = tmpnt.mods[modID].GLeftX/(float)AMP_PARA;
			lly = tmpnt.mods[modID].GLeftY/(float)AMP_PARA;
			urx = tmpnt.mods[modID].GLeftX/(float)AMP_PARA + tmpnt.mods[modID].modW/(float)AMP_PARA;
			ury = tmpnt.mods[modID].GLeftY/(float)AMP_PARA + tmpnt.mods[modID].modH/(float)AMP_PARA;

			fout << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
			     << "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], ";
			if( tmpnt.mods[modID].flagTSV == false )
				fout << "[220/255 220/255 220/255]);" << " % module" << endl;
			else
				fout << "[217/255 144/255 88/255]);" << " % TSV" << endl;

			fout << "plot(" << "[ " << llx << " " << urx << " " << urx << " " << llx << " " << llx << " ], "
			     << "[ " << lly << " " << lly << " " << ury << " " << ury << " " << lly << " ], "
			     << "'Color', [0/255 0/255 128/255]);" << endl;
			//if( tmpnt.mods[modID].flagTSV == false )
			//	fout << "text(" << tmpnt.mods[modID].GCenterX/(float)AMP_PARA << ", " << tmpnt.mods[modID].GCenterY/(float)AMP_PARA << ", " << "'" << tmpnt.mods[modID].modName << "'" << ")" << endl;


		}

		fout << "% cluster outline" << endl;
		//fout << "text(" << resultC[i].centerX/(float)AMP_PARA << ", " << resultC[i].centerY/(float)AMP_PARA << ", " << "'" << resultC[i].idClu << "'" << ")" << endl;
		fout << "line([ " << resultC[i].lbX/((float)AMP_PARA) << " " << resultC[i].rtX/((float)AMP_PARA) << " ],[ " << resultC[i].lbY/((float)AMP_PARA) << " " << resultC[i].lbY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 4);" << endl;
		fout << "line([ " << resultC[i].rtX/((float)AMP_PARA) << " " << resultC[i].rtX/((float)AMP_PARA) << " ],[ " << resultC[i].lbY/((float)AMP_PARA) << " " << resultC[i].rtY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 4);" << endl;
		fout << "line([ " << resultC[i].rtX/((float)AMP_PARA) << " " << resultC[i].lbX/((float)AMP_PARA) << " ],[ " << resultC[i].rtY/((float)AMP_PARA) << " " << resultC[i].rtY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 4);" << endl;
		fout << "line([ " << resultC[i].lbX/((float)AMP_PARA) << " " << resultC[i].lbX/((float)AMP_PARA) << " ],[ " << resultC[i].rtY/((float)AMP_PARA) << " " << resultC[i].lbY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 4);" << endl;
	}

	//fout << "print( gcf, '-r700', '-dpng', 'Fig/" << numLayer << "tier_" << sub_benName << "_C" << tmpnt.nLayer << "' );" << endl;


	fout.close();
}
void EPLG::InitialPseudoMod( deque<PSEUDO_MODULE> &tempPM, vector<CUTSET> &tempCS, vector<GROUP> &resultC, NETLIST &tmpnt )
{
	int countPM = 0;
	for( unsigned int i = 0; i < resultC.size(); i++ )
	{
		if( (resultC[i].nMod + resultC[i].nTSV) <= numBase )
			continue;


		PSEUDO_MODULE firstPM;
		firstPM.idClu = resultC[i].idClu;
		firstPM.idSet = countPM;
		firstPM.idPMod = countPM;

		CUTSET firstCS;
		firstCS.idSet = countPM;
		firstCS.boundPM = numBase - resultC[i].nMod - 1;
		firstCS.boundTSV = (int)ceil( resultC[i].nTSV/(float)resultC[i].nMod );

		for( int j = 0; j < (int)resultC[i].idMod.size(); j++ )
		{
			int modID = resultC[i].idMod[j];

			if( !tmpnt.mods[modID].flagTSV )
			{
				firstCS.cutV.push_back( tmpnt.mods[modID].GLeftX );
				firstCS.cutV.push_back( tmpnt.mods[modID].GCenterX );
				firstCS.cutV.push_back( tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW );

				firstCS.cutH.push_back( tmpnt.mods[modID].GLeftY );
				firstCS.cutH.push_back( tmpnt.mods[modID].GCenterY );
				firstCS.cutH.push_back( tmpnt.mods[modID].GLeftY + tmpnt.mods[modID].modH );
			}
			else
			{
				if( firstPM.lbX > tmpnt.mods[modID].GLeftX )
				{
					firstPM.lbX = tmpnt.mods[modID].GLeftX;
				}
				if( firstPM.lbY > tmpnt.mods[modID].GLeftY )
				{
					firstPM.lbY = tmpnt.mods[modID].GLeftY;
				}
				if( firstPM.rtX < tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW )
				{
					firstPM.rtX = tmpnt.mods[modID].GLeftX + tmpnt.mods[modID].modW;
				}
				if( firstPM.rtY < tmpnt.mods[modID].GLeftY + tmpnt.mods[modID].modH )
				{
					firstPM.rtY = tmpnt.mods[modID].GLeftY + tmpnt.mods[modID].modH;
				}
				firstPM.centroidX += tmpnt.mods[modID].GCenterX;
				firstPM.centroidY += tmpnt.mods[modID].GCenterY;
				firstPM.idTSV.push_back( modID );
			}
		}
		firstPM.centroidX = (int)(firstPM.centroidX/(float)firstPM.idTSV.size());
		firstPM.centroidY = (int)(firstPM.centroidY/(float)firstPM.idTSV.size());

		tempPM.push_back( firstPM );
		tempCS.push_back( firstCS );
		countPM++;
	}
}
void EPLG::UpdateVArray( NETLIST &ori_tmpnt, vector<GROUP> &resultC )
{
	for( int i = 0; i < (int)resultC.size(); i++ )
	{
		RGST[ resultC[i].idClu ].vArray = resultC[i].idMod;
		RGST[ resultC[i].idClu ].nVertex = (int)resultC[i].idMod.size();

		TraceRGST( &RGST[resultC[i].idClu], RGST[resultC[i].idClu].parentTree );
	}
}
void EPLG::UpdateVArray( NETLIST &pm_tmpnt, NETLIST &ori_tmpnt, vector<PSEUDO_MODULE> &resultPM, vector<GROUP> &resultC )
{
	for( int i = 0; i < (int)resultC.size(); i++ )
	{
		bool noPM = false;

		if( (resultC[i].nMod + resultC[i].nTSV) <= numBase )
		{
			noPM = true;
		}

		for( int j = 0; j < (int)resultC[i].idMod.size(); j++ )
		{
			if( ori_tmpnt.mods[ resultC[i].idMod[j] ].flagTSV && !noPM )
			{
				continue;
			}
			string modName = ori_tmpnt.mods[ resultC[i].idMod[j] ].modName;
			if( pm_tmpnt.mod_NameToID.find(modName) != pm_tmpnt.mod_NameToID.end() )
			{
				int modID = pm_tmpnt.mod_NameToID[ modName ];
				RGST[ resultC[i].idClu ].vArray.push_back( modID );
				RGST[ resultC[i].idClu ].nVertex++;
			}
		}
	}

	for( int i = 0; i < (int)resultPM.size(); i++ )
	{
		RGST[ resultPM[i].idClu ].vArray.push_back( resultPM[i].idMod );
		RGST[ resultPM[i].idClu ].nVertex++;
	}

	for( int i = 0; i < (int)resultC.size(); i++ )
	{
		TraceRGST( &RGST[resultC[i].idClu], RGST[resultC[i].idClu].parentTree );
	}
}
void EPLG::TraceRGST( TREE *leaf, TREE *trace )
{
	if( trace == NULL )
		return;

	trace->vArray.insert( trace->vArray.end(), leaf->vArray.begin(), leaf->vArray.end() );
	trace->nVertex += (int)leaf->vArray.size();

	TraceRGST( leaf, trace->parentTree );
}
void EPLG::BipartitionPM( deque<PSEUDO_MODULE> &tempPM, vector<PSEUDO_MODULE> &resultPM, vector<CUTSET> &tempCS, NETLIST &tmpnt )
{
	if( tempCS[ tempPM.front().idSet ].boundPM == 0 )
	{
		resultPM.push_back( tempPM.front() );

		tempPM.pop_front();
		if( tempPM.empty() )
			return;
	}

	int setID = tempPM.front().idSet;

	int currentW = tempPM.front().rtX - tempPM.front().lbX;
	int currentH = tempPM.front().rtY - tempPM.front().lbY;

	float currentAR = currentW/(float)currentH;

	PSEUDO_MODULE lchildPM;
	PSEUDO_MODULE rchildPM;

	bool flagHasCut = false;

	float minRatioW = (tempPM.front().centroidX - tempPM.front().lbX)/(float)currentW;
	float minRatioH = (tempPM.front().centroidY - tempPM.front().lbY)/(float)currentH;

	if( minRatioW > 0.5 )
		minRatioW = 1 - minRatioW;
	if( minRatioH > 0.5 )
		minRatioH = 1 - minRatioH;

	char VHcut = '\0';

	if( currentAR >= 0.625 && currentAR <= 1.6 )
	{
		if( minRatioW >= 0.333 && minRatioH >= 0.333 )
		{
			flagHasCut = SelectCutHV_PM( tempPM.front(), lchildPM, rchildPM, tempCS[setID], tmpnt, VHcut );
		}
		else if( minRatioW < minRatioH )
		{
			flagHasCut = SelectCutV_PM( tempPM.front(), lchildPM, rchildPM, tempCS[setID], tmpnt, VHcut );
		}
		else
		{
			flagHasCut = SelectCutH_PM( tempPM.front(), lchildPM, rchildPM, tempCS[setID], tmpnt, VHcut );
		}
	}
	else if( currentAR > 1.6 )
	{
		flagHasCut = SelectCutV_PM( tempPM.front(), lchildPM, rchildPM, tempCS[setID], tmpnt, VHcut );
	}
	else if( currentAR < 0.625 )
	{
		flagHasCut = SelectCutH_PM( tempPM.front(), lchildPM, rchildPM, tempCS[setID], tmpnt, VHcut );
	}

	if( flagHasCut )
	{
		tempCS[setID].boundPM--;

		if( tempCS[setID].boundPM == 0 )
		{
			resultPM.push_back( lchildPM );
			resultPM.back().idPMod = (int)resultPM.size()-1;
			resultPM.push_back( rchildPM );
			resultPM.back().idPMod = (int)resultPM.size()-1;
		}
		else
		{
			bool pushbackL = false;
			bool pushbackR = false;

			float areaLeft = (float)(lchildPM.idTSV.size() * TSV_AREA);
			float areaRight = (float)(rchildPM.idTSV.size() * TSV_AREA);
			float boxLeft = ((lchildPM.rtX - lchildPM.lbX)/(float)AMP_PARA) *
			                ((lchildPM.rtY - lchildPM.lbY)/(float)AMP_PARA);
			float boxRight = ((rchildPM.rtX - rchildPM.lbX)/(float)AMP_PARA) *
			                 ((rchildPM.rtY - rchildPM.lbY)/(float)AMP_PARA);

			if( areaLeft > 0.25 * boxLeft ||
			        ( (int)lchildPM.idTSV.size() < tempCS[setID].boundTSV && areaLeft > 0.05 * boxLeft ) )
				pushbackL = true;
			if( areaRight > 0.25 * boxRight ||
			        ( (int)rchildPM.idTSV.size() < tempCS[setID].boundTSV && areaRight > 0.05 * boxRight ) )
				pushbackR = true;

			if( !pushbackL && !pushbackR )
			{
				int	HP_Left = (lchildPM.rtX - lchildPM.lbX) + (lchildPM.rtY - lchildPM.lbY);
				int HP_Right = (rchildPM.rtX - rchildPM.lbX) + (rchildPM.rtY - rchildPM.lbY);

				if( HP_Left > HP_Right )
				{
					tempPM.push_back( lchildPM );
					tempPM.push_back( rchildPM );
				}
				else
				{
					tempPM.push_back( rchildPM );
					tempPM.push_back( lchildPM );
				}
			}
			else
			{
				if( pushbackL )
				{
					resultPM.push_back( lchildPM );
					resultPM.back().idPMod = (int)resultPM.size()-1;
				}
				else
				{
					tempPM.push_back( lchildPM );
				}

				if( pushbackR )
				{
					resultPM.push_back( rchildPM );
					resultPM.back().idPMod = (int)resultPM.size()-1;
				}
				else
				{
					tempPM.push_back( rchildPM );
				}
			}
		}
	}
	else
	{
		resultPM.push_back( tempPM.front() );
		resultPM.back().idPMod = (int)resultPM.size()-1;
	}

	tempPM.pop_front();

	if( tempPM.empty() )
		return;

	while( tempCS[ tempPM.front().idSet ].boundPM == 0 )
	{
		resultPM.push_back( tempPM.front() );
		resultPM.back().idPMod = (int)resultPM.size()-1;
		tempPM.pop_front();

		if( tempPM.empty() )
			return;
	}

	BipartitionPM( tempPM, resultPM, tempCS, tmpnt );
}
bool EPLG::SelectCutV_PM( PSEUDO_MODULE &currentPM, PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, CUTSET &cutset, NETLIST &tmpnt, char &VHcut )
{
	VHcut = 'V';

	int cutlineV = 0;
	float weightV = MAX_FLT;

	int *lengthV = new int[currentPM.idTSV.size()];

	bool flagHasCutV = false;

	for( int i = 0; i < (int)cutset.cutV.size(); i++ )
	{
		if( (cutset.cutV[i] - currentPM.lbX) < (TSV_SIZE * AMP_PARA * 0.5) ||
		        (currentPM.rtX - cutset.cutV[i]) < (TSV_SIZE * AMP_PARA * 0.5) ||
		        (cutset.cutV[i] > currentPM.rtX) ||
		        (cutset.cutV[i] < currentPM.lbX) )
		{
			continue;
		}

		flagHasCutV = true;

		int tmp_cutlineV = cutset.cutV[i];
		float tmp_weightV = CalCutWeightV_PM( lengthV, tmp_cutlineV, currentPM, tmpnt );

		if( tmp_weightV < weightV )
		{
			cutlineV = tmp_cutlineV;
			weightV = tmp_weightV;
		}
	}

	//cout << " V: " << cutlineV << "/" << weightV << endl;

	delete [] lengthV;

	if( !flagHasCutV)
		return false;

	lchildPM.idClu = currentPM.idClu;
	lchildPM.idSet = currentPM.idSet;

	rchildPM.idClu = currentPM.idClu;
	rchildPM.idSet = currentPM.idSet;

	for( int i = 0; i < (int)currentPM.idTSV.size(); i++ )
	{
		if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterX < cutlineV )
		{
			lchildPM.idTSV.push_back( currentPM.idTSV[i] );
		}
		else
		{
			rchildPM.idTSV.push_back( currentPM.idTSV[i] );
		}
	}

	UpdatePModChild( lchildPM, rchildPM, tmpnt );

	float diffLR = (rchildPM.lbX - lchildPM.rtX)/(float)AMP_PARA;

	if( (lchildPM.idTSV.size() == 1 || rchildPM.idTSV.size() == 1) && diffLR < 1.5 * TSV_SIZE )
		return false;

	return true;
}

bool EPLG::SelectCutH_PM( PSEUDO_MODULE &currentPM, PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, CUTSET &cutset, NETLIST &tmpnt, char &VHcut )
{
	VHcut = 'H';

	int cutlineH = 0;
	float weightH = MAX_FLT;

	int *lengthH = new int[currentPM.idTSV.size()];

	bool flagHasCutH = false;

	for( int i = 0; i < (int)cutset.cutH.size(); i++ )
	{
		if( (cutset.cutH[i] - currentPM.lbY) < (TSV_SIZE * AMP_PARA * 0.5) ||
		    (currentPM.rtY - cutset.cutH[i]) < (TSV_SIZE * AMP_PARA * 0.5) ||
		    (cutset.cutH[i] > currentPM.rtY) ||
		    (cutset.cutH[i] < currentPM.lbY) )
		{
			continue;
		}

		flagHasCutH = true;

		int tmp_cutlineH = cutset.cutH[i];
		float tmp_weightH = CalCutWeightH_PM( lengthH, tmp_cutlineH, currentPM, tmpnt );

		if( tmp_weightH < weightH )
		{
			cutlineH = tmp_cutlineH;
			weightH = tmp_weightH;
		}
	}

	//cout << " H: " << cutlineH << "/" << weightH << endl;

	delete [] lengthH;

	if( !flagHasCutH )
		return false;

	lchildPM.idClu = currentPM.idClu;
	lchildPM.idSet = currentPM.idSet;

	rchildPM.idClu = currentPM.idClu;
	rchildPM.idSet = currentPM.idSet;

	for( int i = 0; i < (int)currentPM.idTSV.size(); i++ )
	{
		if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterY < cutlineH )
		{
			lchildPM.idTSV.push_back( currentPM.idTSV[i] );
		}
		else
		{
			rchildPM.idTSV.push_back( currentPM.idTSV[i] );
		}
	}

	UpdatePModChild( lchildPM, rchildPM, tmpnt );

	float diffLR = (rchildPM.lbY - lchildPM.rtY)/(float)AMP_PARA;

	if( (lchildPM.idTSV.size() == 1 || rchildPM.idTSV.size() == 1) && diffLR < 1.5 * TSV_SIZE )
		return false;

	return true;
}

bool EPLG::SelectCutHV_PM( PSEUDO_MODULE &currentPM, PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, CUTSET &cutset, NETLIST &tmpnt, char &VHcut )
{
	int cutlineV = 0;
	int cutlineH = 0;

	float weightV = MAX_FLT;
	float weightH = MAX_FLT;

	int *length = new int[currentPM.idTSV.size()];

	bool flagHasCutV = false;
	for( int i = 0; i < (int)cutset.cutV.size(); i++ )
	{
		if( (cutset.cutV[i] - currentPM.lbX) < (TSV_SIZE * AMP_PARA * 0.5) ||
		        (currentPM.rtX - cutset.cutV[i]) < (TSV_SIZE * AMP_PARA * 0.5) ||
		        (cutset.cutV[i] > currentPM.rtX) ||
		        (cutset.cutV[i] < currentPM.lbX) )
		{
			continue;
		}

		flagHasCutV = true;

		int tmp_cutlineV = cutset.cutV[i];
		float tmp_weightV = CalCutWeightV_PM( length, tmp_cutlineV, currentPM, tmpnt );

		if( tmp_weightV < weightV )
		{
			cutlineV = tmp_cutlineV;
			weightV = tmp_weightV;
		}
	}

	bool flagHasCutH = false;
	for( int i = 0; i < (int)cutset.cutH.size(); i++ )
	{
		if( (cutset.cutH[i] - currentPM.lbY) < (TSV_SIZE * AMP_PARA * 0.5) ||
		        (currentPM.rtY - cutset.cutH[i]) < (TSV_SIZE * AMP_PARA * 0.5) ||
		        (cutset.cutH[i] > currentPM.rtY) ||
		        (cutset.cutH[i] < currentPM.lbY) )
		{
			continue;
		}

		flagHasCutH = true;

		int tmp_cutlineH = cutset.cutH[i];
		float tmp_weightH = CalCutWeightH_PM( length, tmp_cutlineH, currentPM, tmpnt );

		if( tmp_weightH < weightH )
		{
			cutlineH = tmp_cutlineH;
			weightH = tmp_weightH;
		}
	}

	//cout << " V: " << cutlineV << "/" << weightV << endl;
	//cout << " H: " << cutlineH << "/" << weightH << endl;

	delete [] length;

	if( !flagHasCutV && !flagHasCutH )
		return false;

	lchildPM.idClu = currentPM.idClu;
	lchildPM.idSet = currentPM.idSet;

	rchildPM.idClu = currentPM.idClu;
	rchildPM.idSet = currentPM.idSet;

	if( weightH < weightV )
	{
		VHcut = 'H';
		for( int i = 0; i < (int)currentPM.idTSV.size(); i++ )
		{
			if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterY < cutlineH )
			{
				lchildPM.idTSV.push_back( currentPM.idTSV[i] );
			}
			else
			{
				rchildPM.idTSV.push_back( currentPM.idTSV[i] );
			}
		}

		UpdatePModChild( lchildPM, rchildPM, tmpnt );
	}
	else
	{
		VHcut = 'V';
		for( int i = 0; i < (int)currentPM.idTSV.size(); i++ )
		{
			if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterX < cutlineV )
			{
				lchildPM.idTSV.push_back( currentPM.idTSV[i] );
			}
			else
			{
				rchildPM.idTSV.push_back( currentPM.idTSV[i] );
			}
		}

		UpdatePModChild( lchildPM, rchildPM, tmpnt );
	}

	float diffLR = 0.0;
	if( VHcut == 'H' )
		diffLR = (rchildPM.lbY - lchildPM.rtY)/(float)AMP_PARA;
	else
		diffLR = (rchildPM.lbX - lchildPM.rtX)/(float)AMP_PARA;

	if( (lchildPM.idTSV.size() == 1 || rchildPM.idTSV.size() == 1) && diffLR < 1.5 * TSV_SIZE )
		return false;

	return true;
}

void EPLG::CreatePModNetlist( NETLIST &pm_tmpnt, NETLIST &ori_tmpnt, vector<PSEUDO_MODULE> &resultPM )
{
	map<int, int> mapTSV;
	for( int i = 0; i < (int)resultPM.size(); i++ )
	{
		for( int j = 0; j < (int)resultPM[i].idTSV.size(); j++ )
		{
			mapTSV[ resultPM[i].idTSV[j] ] = -1;
		}
	}

	pm_tmpnt.ChipWidth  = ori_tmpnt.ChipWidth;
	pm_tmpnt.ChipHeight = ori_tmpnt.ChipHeight;
	pm_tmpnt.ChipArea   = ori_tmpnt.ChipArea;
	pm_tmpnt.aR         = ori_tmpnt.aR;
	pm_tmpnt.maxWS      = ori_tmpnt.maxWS;
	pm_tmpnt.nLayer     = ori_tmpnt.nLayer;

	for( int i = 0; i < (int)ori_tmpnt.mods.size(); i++ )
	{
		if( ori_tmpnt.mods[i].flagTSV && mapTSV.find(ori_tmpnt.mods[i].idMod) != mapTSV.end() )
			continue;

		pm_tmpnt.mods.push_back( ori_tmpnt.mods[i] );
		pm_tmpnt.mods.back().idMod = pm_tmpnt.nMod;
		pm_tmpnt.mods.back().nNet = 0;
		pm_tmpnt.mod_NameToID[ ori_tmpnt.mods[i].modName ] = pm_tmpnt.nMod;
		pm_tmpnt.totalModArea += ori_tmpnt.mods[i].modArea;

		if( ori_tmpnt.mods[i].type == HARD_BLK )
			pm_tmpnt.nHard++;
		else
			pm_tmpnt.nSoft++;

		if( ori_tmpnt.mods[i].flagTSV )
			pm_tmpnt.nTSV++;

		pm_tmpnt.nMod++;
	}

	for( int i = 0; i < (int)resultPM.size(); i++ )
	{
		MODULE pmod;

		char subName[500] = "\0";
		sprintf( subName, "%d", i );
		strcpy( pmod.modName, "PM" );
		strcat( pmod.modName, subName );
		pmod.idMod = pm_tmpnt.nMod;
		pmod.type = SOFT_BLK;
		pmod.modW = pmod.modH = (int)(sqrt((float)resultPM[i].idTSV.size() * (float)TSV_SIZE * (float)TSV_SIZE) * (float)AMP_PARA);
		pmod.modL = pm_tmpnt.nLayer;
		pmod.modArea = resultPM[i].idTSV.size() * TSV_SIZE * TSV_SIZE;
		pmod.minAR = 1.0/(float)resultPM[i].idTSV.size();
		pmod.maxAR = (float)resultPM[i].idTSV.size();
		pmod.CenterX = pmod.GCenterX = resultPM[i].centroidX;
		pmod.CenterY = pmod.GCenterY = resultPM[i].centroidY;
		pmod.LeftX = pmod.GLeftX = resultPM[i].centroidX - pmod.modW/2;
		pmod.LeftY = pmod.GLeftY = resultPM[i].centroidY - pmod.modH/2;
		pmod.flagTSV = true;
		pmod.pseudoMod = &resultPM[i];
		resultPM[i].idMod = pmod.idMod;

		pm_tmpnt.mod_NameToID[ pmod.modName ] = pm_tmpnt.nMod;
		pm_tmpnt.totalModArea += pmod.modArea;
		pm_tmpnt.nSoft++;
		pm_tmpnt.nTSV++;

		pm_tmpnt.mods.push_back( pmod );
		pm_tmpnt.nMod++;

		for( int j = 0; j < (int)resultPM[i].idTSV.size(); j++ )
		{
			mapTSV[ resultPM[i].idTSV[j] ] = pmod.idMod;
		}
	}

	pm_tmpnt.nPad = ori_tmpnt.nPad;
	pm_tmpnt.pads = ori_tmpnt.pads;
	pm_tmpnt.pad_NameToID = ori_tmpnt.pad_NameToID;
	for( int i = 0; i < pm_tmpnt.nPad; i++ )
	{
		pm_tmpnt.pads[i].idPad = pm_tmpnt.nMod + pm_tmpnt.pad_NameToID[ pm_tmpnt.pads[i].padName ];
	}

	pm_tmpnt.nNet = ori_tmpnt.nNet;
	pm_tmpnt.nets = ori_tmpnt.nets;
	pm_tmpnt.nPin = ori_tmpnt.nPin;
	pm_tmpnt.pins = ori_tmpnt.pins;

	for( int i = 0; i < pm_tmpnt.nPin; i++ )
	{
		if( pm_tmpnt.mod_NameToID.find( pm_tmpnt.pins[i].pinName ) != pm_tmpnt.mod_NameToID.end() )
		{
			int modID = pm_tmpnt.mod_NameToID[ pm_tmpnt.pins[i].pinName ];
			pm_tmpnt.pins[i].index = modID;
			pm_tmpnt.mods[modID].nNet++;
		}
		else if( pm_tmpnt.pad_NameToID.find( pm_tmpnt.pins[i].pinName ) != pm_tmpnt.pad_NameToID.end() )
		{
			int padID = pm_tmpnt.pad_NameToID[ pm_tmpnt.pins[i].pinName ];
			pm_tmpnt.pins[i].index = padID + pm_tmpnt.nMod;
		}
		else if( mapTSV.find( pm_tmpnt.pins[i].index ) != mapTSV.end() )
		{
			int modID = mapTSV[ pm_tmpnt.pins[i].index ];
			strcpy( pm_tmpnt.pins[i].pinName, pm_tmpnt.mods[modID].modName );
			pm_tmpnt.pins[i].index = modID;
			pm_tmpnt.mods[modID].nNet++;
		}
		else
		{
			cout << "error  : unrecognized pin:" << pm_tmpnt.pins[i].pinName << endl;
			exit(EXIT_FAILURE);
		}
	}

	for( int i = 0; i < pm_tmpnt.nMod; i++ )
	{
		int pm_modID = pm_tmpnt.mod_NameToID[ pm_tmpnt.mods[i].modName ];
		try
		{
			pm_tmpnt.mods[pm_modID].pNet = new unsigned short [ pm_tmpnt.mods[pm_modID].nNet ];
		}
		catch( bad_alloc &bad )
		{
			cout << "error  : run out of memory on new_nt.mods[" << pm_modID << "].pNet" << endl;
			exit(EXIT_FAILURE);
		}
		pm_tmpnt.mods[pm_modID].nNet = 0;
	}

	for( int i = 0; i < pm_tmpnt.nNet; i++ )
	{
		for( int j = pm_tmpnt.nets[i].head; j < pm_tmpnt.nets[i].head + pm_tmpnt.nets[i].nPin; j++ )
		{
			if( pm_tmpnt.mod_NameToID.find( pm_tmpnt.pins[j].pinName ) != pm_tmpnt.mod_NameToID.end() )
			{
				int pm_modID = pm_tmpnt.mod_NameToID[ pm_tmpnt.pins[j].pinName ];
				pm_tmpnt.mods[pm_modID].pNet[ pm_tmpnt.mods[pm_modID].nNet ] = pm_tmpnt.nets[i].idNet;
				pm_tmpnt.mods[pm_modID].nNet++;
			}
		}
	}

	mapTSV.clear();
}
float EPLG::CalCutWeightV_PM( int *lengthV, int &cutlineV, PSEUDO_MODULE &currentPM, NETLIST &tmpnt )
{
	float valueSD_L = 0.0;
	float valueSD_R = 0.0;
	float valueAvgL = 0.0;
	float valueAvgR = 0.0;
	int numL = 0;
	int numR = 0;

	int numTSV = (int)currentPM.idTSV.size();

	for( int i = 0; i < numTSV; i++ )
	{
		lengthV[i] = abs( cutlineV - tmpnt.mods[ currentPM.idTSV[i] ].GCenterX );

		if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterX < cutlineV )
		{
			valueAvgL += (lengthV[i]/(float)AMP_PARA);
			numL++;
		}
		else
		{
			valueAvgR += (lengthV[i]/(float)AMP_PARA);
			numR++;
		}
	}
	valueAvgL /= (float)numL;
	valueAvgR /= (float)numR;

	for( int i = 0; i < numTSV; i++ )
	{
		if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterX < cutlineV )
		{
			valueSD_L += ((lengthV[i]/(float)AMP_PARA - valueAvgL) * (lengthV[i]/(float)AMP_PARA - valueAvgL));
		}
		else
		{
			valueSD_R += ((lengthV[i]/(float)AMP_PARA - valueAvgR) * (lengthV[i]/(float)AMP_PARA - valueAvgR));
		}
	}

	valueSD_L = sqrt( valueSD_L/(float)numL);
	valueSD_R = sqrt( valueSD_R/(float)numR);

	return (valueSD_L + valueSD_R);
}

float EPLG::CalCutWeightH_PM( int *lengthH, int &cutlineH, PSEUDO_MODULE &currentPM, NETLIST &tmpnt )
{
	float valueSD_T = 0.0;
	float valueSD_B = 0.0;
	float valueAvgT = 0.0;
	float valueAvgB = 0.0;
	int numT = 0;
	int numB = 0;

	int numTSV = (int)currentPM.idTSV.size();

	for( int i = 0; i < numTSV; i++ )
	{
		lengthH[i] = abs( cutlineH - tmpnt.mods[ currentPM.idTSV[i] ].GCenterY );

		if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterY < cutlineH )
		{
			valueAvgB += (lengthH[i]/(float)AMP_PARA);
			numB++;
		}
		else
		{
			valueAvgT += (lengthH[i]/(float)AMP_PARA);
			numT++;
		}
	}
	valueAvgB /= (float)numB;
	valueAvgT /= (float)numT;

	for( int i = 0; i < numTSV; i++ )
	{
		if( tmpnt.mods[ currentPM.idTSV[i] ].GCenterY < cutlineH )
		{
			valueSD_B += ((lengthH[i]/(float)AMP_PARA - valueAvgB) * (lengthH[i]/(float)AMP_PARA - valueAvgB));
		}
		else
		{
			valueSD_T += ((lengthH[i]/(float)AMP_PARA - valueAvgT) * (lengthH[i]/(float)AMP_PARA - valueAvgT));
		}
	}
	valueSD_B = sqrt(valueSD_B/(float)numB);
	valueSD_T = sqrt(valueSD_T/(float)numT);

	return (valueSD_T + valueSD_B);
}

void EPLG::UpdatePModChild( PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, NETLIST &tmpnt )
{
	for( int i = 0; i < (int)lchildPM.idTSV.size(); i++ )
	{
		if( lchildPM.lbX > tmpnt.mods[ lchildPM.idTSV[i] ].GLeftX )
			lchildPM.lbX = tmpnt.mods[ lchildPM.idTSV[i] ].GLeftX;
		if( lchildPM.lbY > tmpnt.mods[ lchildPM.idTSV[i] ].GLeftY )
			lchildPM.lbY = tmpnt.mods[ lchildPM.idTSV[i] ].GLeftY;
		if( lchildPM.rtX < tmpnt.mods[ lchildPM.idTSV[i] ].GLeftX + tmpnt.mods[ lchildPM.idTSV[i] ].modW )
			lchildPM.rtX = tmpnt.mods[ lchildPM.idTSV[i] ].GLeftX + tmpnt.mods[ lchildPM.idTSV[i] ].modW;
		if( lchildPM.rtY < tmpnt.mods[ lchildPM.idTSV[i] ].GLeftY + tmpnt.mods[ lchildPM.idTSV[i] ].modH )
			lchildPM.rtY = tmpnt.mods[ lchildPM.idTSV[i] ].GLeftY + tmpnt.mods[ lchildPM.idTSV[i] ].modH;
		lchildPM.centroidX += tmpnt.mods[ lchildPM.idTSV[i] ].GCenterX;
		lchildPM.centroidY += tmpnt.mods[ lchildPM.idTSV[i] ].GCenterY;
	}

	for( int i = 0; i < (int)rchildPM.idTSV.size(); i++ )
	{
		if( rchildPM.lbX > tmpnt.mods[ rchildPM.idTSV[i] ].GLeftX )
			rchildPM.lbX = tmpnt.mods[ rchildPM.idTSV[i] ].GLeftX;
		if( rchildPM.lbY > tmpnt.mods[ rchildPM.idTSV[i] ].GLeftY )
			rchildPM.lbY = tmpnt.mods[ rchildPM.idTSV[i] ].GLeftY;
		if( rchildPM.rtX < tmpnt.mods[ rchildPM.idTSV[i] ].GLeftX + tmpnt.mods[ rchildPM.idTSV[i] ].modW )
			rchildPM.rtX = tmpnt.mods[ rchildPM.idTSV[i] ].GLeftX + tmpnt.mods[ rchildPM.idTSV[i] ].modW;
		if( rchildPM.rtY < tmpnt.mods[ rchildPM.idTSV[i] ].GLeftY + tmpnt.mods[ rchildPM.idTSV[i] ].modH )
			rchildPM.rtY = tmpnt.mods[ rchildPM.idTSV[i] ].GLeftY + tmpnt.mods[ rchildPM.idTSV[i] ].modH;
		rchildPM.centroidX += tmpnt.mods[ rchildPM.idTSV[i] ].GCenterX;
		rchildPM.centroidY += tmpnt.mods[ rchildPM.idTSV[i] ].GCenterY;
	}

	lchildPM.centroidX = (int)(lchildPM.centroidX/(float)lchildPM.idTSV.size());
	lchildPM.centroidY = (int)(lchildPM.centroidY/(float)lchildPM.idTSV.size());
	rchildPM.centroidX = (int)(rchildPM.centroidX/(float)rchildPM.idTSV.size());
	rchildPM.centroidY = (int)(rchildPM.centroidY/(float)rchildPM.idTSV.size());
}
void EPLG::RefineSinglePMod( vector<PSEUDO_MODULE> &resultPM )
{
	vector<PSEUDO_MODULE> singlePM;

	vector<PSEUDO_MODULE>::iterator pmod_it;
	for( pmod_it = resultPM.begin(); pmod_it != resultPM.end(); )
	{
		if( pmod_it->idTSV.size() == 1 )
		{
			singlePM.push_back( (*pmod_it) );
			resultPM.erase( pmod_it );
		}
		else
		{
			++pmod_it;
		}
	}

	vector<PSEUDO_MODULE>::iterator single_it;
	vector<PSEUDO_MODULE>::iterator merge_it;
	for( single_it = singlePM.begin(); single_it != singlePM.end(); ++single_it )
	{
		int minDistance = MAX_INT;

		for( pmod_it = resultPM.begin(); pmod_it != resultPM.end(); ++pmod_it )
		{
			int curDistance = abs( single_it->centroidX - pmod_it->centroidX ) +
			                  abs( single_it->centroidY - pmod_it->centroidY );
			if( curDistance > minDistance )
				continue;

			minDistance = curDistance;
			merge_it = pmod_it;
		}

		if( single_it->lbX < merge_it->lbX ) merge_it->lbX = single_it->lbX;
		if( single_it->rtX > merge_it->rtX ) merge_it->rtX = single_it->rtX;
		if( single_it->lbY < merge_it->lbY ) merge_it->lbY = single_it->lbY;
		if( single_it->rtY > merge_it->rtY ) merge_it->rtY = single_it->rtY;

		merge_it->centroidX = (int)((merge_it->centroidX * merge_it->idTSV.size() + single_it->centroidX)/(float)(merge_it->idTSV.size() + 1));
		merge_it->centroidY = (int)((merge_it->centroidY * merge_it->idTSV.size() + single_it->centroidY)/(float)(merge_it->idTSV.size() + 1));

		merge_it->idTSV.push_back( single_it->idTSV[0] );
	}

	singlePM.clear();
	vector<PSEUDO_MODULE> (singlePM).swap(singlePM);
}

void EPLG::RefineSinglePMod( vector<PSEUDO_MODULE> &resultPM, NETLIST &tmpnt )
{
	vector<int> refineTSV;

	vector<PSEUDO_MODULE>::iterator pmod_it;
	for( pmod_it = resultPM.begin(); pmod_it != resultPM.end(); )
	{
		if( pmod_it->idTSV.size() <= 2 )
		{
			refineTSV.insert( refineTSV.end(), pmod_it->idTSV.begin(), pmod_it->idTSV.end() );
			resultPM.erase( pmod_it );
		}
		else
		{
			++pmod_it;
		}
	}

	vector<PSEUDO_MODULE>::iterator merge_it;
	for( int i = 0; i < (int)refineTSV.size(); i++ )
	{
		int minDistance = MAX_INT;
		for( pmod_it = resultPM.begin(); pmod_it != resultPM.end(); ++pmod_it )
		{
			int curDistance = abs( tmpnt.mods[ refineTSV[i] ].GCenterX - pmod_it->centroidX ) +
			                  abs( tmpnt.mods[ refineTSV[i] ].GCenterY - pmod_it->centroidY );

			if( curDistance > minDistance )
				continue;

			minDistance = curDistance;
			merge_it = pmod_it;
		}

		if( tmpnt.mods[ refineTSV[i] ].GLeftX < merge_it->lbX )
			merge_it->lbX = tmpnt.mods[ refineTSV[i] ].GLeftX;
		if( tmpnt.mods[ refineTSV[i] ].GLeftY < merge_it->lbY )
			merge_it->lbY = tmpnt.mods[ refineTSV[i] ].GLeftY;
		if( tmpnt.mods[ refineTSV[i] ].GLeftX + tmpnt.mods[ refineTSV[i] ].modW > merge_it->rtX )
			merge_it->rtX = tmpnt.mods[ refineTSV[i] ].GLeftX + tmpnt.mods[ refineTSV[i] ].modW;
		if( tmpnt.mods[ refineTSV[i] ].GLeftY + tmpnt.mods[ refineTSV[i] ].modH > merge_it->rtY )
			merge_it->rtY = tmpnt.mods[ refineTSV[i] ].GLeftY + tmpnt.mods[ refineTSV[i] ].modH;

		merge_it->centroidX = (int)((merge_it->centroidX * merge_it->idTSV.size() + tmpnt.mods[ refineTSV[i] ].GCenterX)/(float)(merge_it->idTSV.size() + 1));
		merge_it->centroidY = (int)((merge_it->centroidY * merge_it->idTSV.size() + tmpnt.mods[ refineTSV[i] ].GCenterY)/(float)(merge_it->idTSV.size() + 1));

		merge_it->idTSV.push_back( refineTSV[i] );
	}

	refineTSV.clear();
	vector<int> (refineTSV).swap(refineTSV);
}
void EPLG::PlotResultPM( vector<GROUP> &resultC, vector<PSEUDO_MODULE> &resultPM, NETLIST &tmpnt, char *file )
{
	char fileName[50] = "\0";

	char subName[50] = "\0";
	sprintf( subName, "%d", tmpnt.nLayer );

	strcpy( fileName, file );
	strcat( fileName, "PM_" );
	strcat( fileName, subName );
	strcat( fileName, ".m" );

	ofstream fout;
	fout.open( fileName );
	if( !fout.is_open() )
	{
		cout << "error  : unable to open " << fileName << "for plotting the result of clustering & TSV groups" << endl;
		exit(EXIT_FAILURE);
	}

	fout.precision(3);
	fout.setf( ios::fixed, ios::floatfield );

	float llx;
	float lly;
	float urx;
	float ury;

	fout << "figure;" << endl;
	fout << "clear;" << endl;
	fout << "clf;" << endl;
	fout << "axis equal;" << endl;

	int ChipWidth = 0;
	int ChipHeight = 0;
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		ChipWidth = COM_MAX( tmpnt.mods[i].LeftX + tmpnt.mods[i].modW, ChipWidth );
		ChipHeight = COM_MAX( tmpnt.mods[i].LeftY + tmpnt.mods[i].modH, ChipHeight );
	}
	ChipWidth = COM_MAX( tmpnt.ChipWidth, ChipWidth );
	ChipHeight = COM_MAX( tmpnt.ChipHeight, ChipHeight );

	llx = -ChipWidth/(20)/(float)AMP_PARA;
	lly = -ChipHeight/(20)/(float)AMP_PARA;
	urx = ChipWidth/((float)AMP_PARA) + ChipWidth/(20)/(float)AMP_PARA;
	ury = ChipHeight/((float)AMP_PARA) + ChipHeight/(20)/(float)AMP_PARA;

	fout << "axis([ " << llx << " " << urx << " " << lly << " " << ury << "]);" << endl;
	fout << "hold on;" << endl;
	fout << "grid on;" << endl;
	fout << "title('" << fileName << "');" << endl;

	llx = 0;
	lly = 0;
	urx = tmpnt.ChipWidth/((float)AMP_PARA);
	ury = tmpnt.ChipHeight/((float)AMP_PARA);

	// plot fixed-outline
	fout << "% fixed-outline" << endl;
	fout << "line(" << "[ " << llx << " " << llx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
	fout << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << lly << " " << lly << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
	fout << "line(" << "[ " << urx << " " << urx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2'" << ");" << endl;
	fout << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << ury << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;

	char c = 'g';
	for( unsigned int i = 0; i < resultC.size(); i++ )
	{
		for( unsigned int j = 0; j < resultC[i].idMod.size(); j++ )
		{
			int modID = resultC[i].idMod[j];

			llx = tmpnt.mods[modID].GLeftX/(float)AMP_PARA;
			lly = tmpnt.mods[modID].GLeftY/(float)AMP_PARA;
			urx = tmpnt.mods[modID].GLeftX/(float)AMP_PARA + tmpnt.mods[modID].modW/(float)AMP_PARA;
			ury = tmpnt.mods[modID].GLeftY/(float)AMP_PARA + tmpnt.mods[modID].modH/(float)AMP_PARA;

			fout << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
			     << "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], ";
			if( tmpnt.mods[modID].flagTSV == false )
				fout << "[220/255 220/255 220/255], 'FaceAlpha', 0.5);" << " % module" << endl;
			else
				fout << "[217/255 144/255 88/255], 'FaceAlpha', 0.5);" << " % TSV" << endl;

			fout << "plot(" << "[ " << llx << " " << urx << " " << urx << " " << llx << " " << llx << " ], "
			     << "[ " << lly << " " << lly << " " << ury << " " << ury << " " << lly << " ], "
			     << "'Color', [0/255 0/255 128/255]);" << endl;
		}

		fout << "% cluster outline" << endl;
		fout << "text(" << resultC[i].centerX/(float)AMP_PARA << ", " << resultC[i].centerY/(float)AMP_PARA << ", " << "'" << resultC[i].idClu << "'" << ")" << endl;
		fout << "line([ " << resultC[i].lbX/((float)AMP_PARA) << " " << resultC[i].rtX/((float)AMP_PARA) << " ],[ " << resultC[i].lbY/((float)AMP_PARA) << " " << resultC[i].lbY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
		fout << "line([ " << resultC[i].rtX/((float)AMP_PARA) << " " << resultC[i].rtX/((float)AMP_PARA) << " ],[ " << resultC[i].lbY/((float)AMP_PARA) << " " << resultC[i].rtY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
		fout << "line([ " << resultC[i].rtX/((float)AMP_PARA) << " " << resultC[i].lbX/((float)AMP_PARA) << " ],[ " << resultC[i].rtY/((float)AMP_PARA) << " " << resultC[i].rtY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
		fout << "line([ " << resultC[i].lbX/((float)AMP_PARA) << " " << resultC[i].lbX/((float)AMP_PARA) << " ],[ " << resultC[i].rtY/((float)AMP_PARA) << " " << resultC[i].lbY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
	}

	c = 'm';
	for( unsigned int i = 0; i < resultPM.size(); i++ )
	{
		//fout << "text(" << resultPM[i].centerX/(float)AMP_PARA << ", " << resultPM[i].centerY/(float)AMP_PARA << ", " << "'" << resultPM[i].idClu << "'" << ")" << endl;
		fout << "line([ " << resultPM[i].lbX/((float)AMP_PARA) << " " << resultPM[i].rtX/((float)AMP_PARA) << " ],[ " << resultPM[i].lbY/((float)AMP_PARA) << " " << resultPM[i].lbY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
		fout << "line([ " << resultPM[i].rtX/((float)AMP_PARA) << " " << resultPM[i].rtX/((float)AMP_PARA) << " ],[ " << resultPM[i].lbY/((float)AMP_PARA) << " " << resultPM[i].rtY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
		fout << "line([ " << resultPM[i].rtX/((float)AMP_PARA) << " " << resultPM[i].lbX/((float)AMP_PARA) << " ],[ " << resultPM[i].rtY/((float)AMP_PARA) << " " << resultPM[i].rtY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;
		fout << "line([ " << resultPM[i].lbX/((float)AMP_PARA) << " " << resultPM[i].lbX/((float)AMP_PARA) << " ],[ " << resultPM[i].rtY/((float)AMP_PARA) << " " << resultPM[i].lbY/((float)AMP_PARA) << " ], 'Color', '" << c << "', 'LineWidth', 2);" << endl;

	}
	//fout << "print( gcf, '-r700', '-dpng', 'Fig/" << numLayer << "tier_" << sub_benName << "_TG" << tmpnt.nLayer << "' );" << endl;

	fout.close();

}

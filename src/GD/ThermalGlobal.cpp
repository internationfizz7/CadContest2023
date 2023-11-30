
#include "ThermalGlobal.h"

/*!
 * \file 	ThermalGlobal.cpp
 * \brief 	Thermal-Aware Global Placement [(header)(WL+Area+Thermal)]
 *
 * \author 	Tai-Ting Chen
 * \version	2.0 (2D/3D)
 * \date	2018.07.
 */

void THERMALGP:: ThermalGlobal(NETLIST& tmpnt)
{

	///f///
	this->CalculateHPWL_GP(tmpnt);
	cout<<BOLD(Red("\\ ====== Thermal Mode ====== //"))<< endl;
	cout<< BOLD( Green(" - TFM On/Off\t:")) << this->TFM <<endl;
	cout<< BOLD( Green(" - Fixed Module\t:")) << this->DefineSuperBigPara <<endl;
	cout<< BOLD( Green(" - Initial rho\t:")) << this->rho <<endl;
	cout<< BOLD( Green(" - PLOT\t\t?")) << this->FlagPlot <<endl;
	if(this->FlagPlot == true)
	{
		cout<< BOLD( Green(" - FP Path\t:")) << this->FpPlotPath <<endl;
		cout<< BOLD( Green(" - Bin Path\t:")) << this->BinMapPath <<endl;
	}

	this->start_t = clock();


	this->Xp = new double [tmpnt.nMod + tmpnt.nPad];	///< postive x-term in LSE wirelength model
	this->Xn = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive x-term in LSE wirelength model
	this->Yp = new double [tmpnt.nMod + tmpnt.nPad];	///< postive y-term in LSE wirelength model
	this->Yn = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive y-term in LSE wirelength model

	this->GX_0 = new double [tmpnt.nMod];				///< x-axis gradient directions g_k-1
	this->GX_1 = new double [tmpnt.nMod];				///< x-axis gradient directions g_k
	this->GY_0 = new double [tmpnt.nMod];				///< y-axis gradient directions g_k-1
	this->GY_1 = new double [tmpnt.nMod];				///< y-axis gradient directions g_k
	this->WG_X = new double [tmpnt.nMod];				///< x-axis wirelength gradient
	this->WG_Y = new double [tmpnt.nMod];				///< y-axis wirelength gradient
	this->DG_X = new double [tmpnt.nMod];				///< x-axis density gradient
	this->DG_Y = new double [tmpnt.nMod];				///< y-axis density gradient
	this->DX_1 = new double [tmpnt.nMod];				///< x-axis conjuagte directions d_k
	this->DX_0 = new double [tmpnt.nMod];				///< x-axis conjugate directions d_k-1
	this->DY_1 = new double [tmpnt.nMod];				///< y-axis conjugate directions d_k
	this->DY_0 = new double [tmpnt.nMod];				///< y-axis conjugate directions d_k-1
	this->Cv = new double [tmpnt.nMod];					///< normalization factor
	this->alpha = new double [tmpnt.nMod];				///< step size


	this->TG_X = new double [tmpnt.nMod];				///< x-axis thermal density gradient
	this->TG_Y = new double [tmpnt.nMod];				///< y-axis thermal density gradient

	//============== Global Para ================//
	this->CountFactor 		= 0;
	this->StopEnhance 		= false;
	this->Change 			= false;
	this->Max_EscpaeDist 	= 0 ;
	this->EnhanceRatio 		= 0.1 ;
	//===========================================//

	int LevelCount = this->TotalLevel;
	/// set gamma (log-sum-exp wirelength model)
	if( tmpnt.ChipWidth >= tmpnt.ChipHeight )
	{
		this->Gamma = (float)tmpnt.ChipWidth/1000/700;
	}
	else
	{
		this->Gamma = (float)tmpnt.ChipHeight/1000/700;
	}

	/// set modules & TSVs to the center of chip

/*
	if(CurrentLevel == LevelCount-1)
		this->SetModToCenter( tmpnt );
*/



	this->ClassifyMacro(tmpnt);

	this->CutBin( tmpnt );

	this->ShrinkMod ( tmpnt );

	this->BuildGaussianMask();

	this->BuildThermalMask();


	//this->CreateThermalTable(tmpnt); 

/*

	/// solve quadratic programming when the level is top
	if(this->CurrentLevel == this->TotalLevel -1 )
	{
		cout<<" - Solving QP.. "<<endl;
		this->SolveQP(tmpnt);
		char* cstr_init= "./ThermalMap/QP";
		PLOT Ploter;
		Ploter.PlotResultGL_dy( tmpnt, cstr_init, 0 );
	}
*/


	/// calculate each term in LSE wirelength model for pads
	for( int i = tmpnt.nMod; i < tmpnt.nMod + tmpnt.nPad; i++ )
	{
		double tempX = tmpnt.pads[i-tmpnt.nMod].x/(double)AMP_PARA;
		double tempY = tmpnt.pads[i-tmpnt.nMod].y/(double)AMP_PARA;
		this->Xp[i] = EXP( (tempX/Gamma)  );
		this->Xn[i] = EXP( (-tempX/Gamma) );
		this->Yp[i] = EXP( (tempY/Gamma)  );
		this->Yn[i] = EXP( (-tempY/Gamma) );

	}


	cout << " - Solve Ojective Function by Conjugate Gradient Method" << endl;

	/// initialize gradient
	this->InitializeGradient( tmpnt);
	this->numIter = 0;
	this->numCG   = 0;


	double OverlapL = 2.0 ;
	double ThermalL = 2.0;


	cout<<Green(" ------------------------- Overflow Info ----------------------------")<<endl;
	cout<<Green("    iter    Wirelength    AreaOvf    ThermalOvf    ThermalImprove    ")<<endl;
	cout<<Green(" --------------------------------------------------------------------")<<endl;


	int it_time = this->TotalLevel  - 1 - this->CurrentLevel;
	int cmode = 0 ;
	int bad_solve =0 ;
	set <double> Leave ;
	int CountForJump = 0;
	double RecordWLTerm = tmpnt.totalWL_GP;



	/// solve min W(x,y) + lambda * Sum((Db-Mb)*(Db-Mb)) + lambdaTH * Sum( (Tg-Cg) )^2
	for( int count = 0; count < 200; count++ )
	{
		double WLterm = 0;
		double BDterm = 0;
		double TMterm = 0;

		if( count > 0 )
		{
			this->overflow_area_0 = this->overflow_area_1;
			this->thermal_difference_0 = this->thermal_difference_1;
		}

		this->numCG++;

		this->ConjugateGradient( tmpnt );

		// Evaluate new overflow ratio
		this->CalOverflowArea( tmpnt );

		this->CalThermalGradient(tmpnt);


		// Check Leave 
		if( count > 0 )
		{
			// Print Performance
			cout<<setw(7) << count ;
			cout<<setw(15)<<RecordWLTerm;
			cout<<setw(12)<<100 * (float)overflow_area_1/(float)tmpnt.totalModArea<<" %";
			cout<<setw(12)<< thermal_difference_1 * 100 <<" %";
			cout<<setw(12)<< ( thermal_difference_0- thermal_difference_1) *100 << " %";
			cout<<endl;


			if( thermal_difference_1 - thermal_difference_0 >= 0  )
			{
				bad_solve++;

				if(bad_solve >= 3 ) // Can tolerate N times bad sol.
				{
					this->Change = true;
				}
			}
			else
				bad_solve = 0;

		}


		// PLOT 
		if(this->FlagPlot==true)
		{
			std::string label= UTILITY::Int_2_String( it_time );
			string path= this->FpPlotPath;
			std::string inner_label = UTILITY::Int_2_String( count );
			string dynamic_out = path+"ThermalGradient"+"_"+label+"_"+inner_label;
			
			PLOT Ploter;
			Ploter.SetColoring(true);	
			Ploter.PlotResultGL_dy( tmpnt, dynamic_out, 0 );
		}


		// Change to Remove Overlap mode
		if(cmode == 0 && this->Change == true )
		{


			cmode = 1 ;
			ThermalL = 1 ;
			OverlapL = 5 ;

			cout << " ----------------- Remove Overlap ----------------" << endl;

			cout << " - WL Weight : "<< 1 <<endl;
			cout << " - Area Weight : "<< OverlapL <<endl;
			cout << " - Thermal Weight : "<< ThermalL <<endl;
			cout << " -------------------------------------------------" << endl;
			cout << " - conut "<<count<<endl;
			cout << " - Inflat Area "<<endl;
			cout << " -------------------------------------------------"<<endl;

			this->InflatMod(tmpnt);
		}

		// Break Condition2 : (1) The Thermal Optimzation is finished (2) The overlap is small 

		if( count > 0 &&  (float)overflow_area_1/(float)tmpnt.totalModArea <= InputOption.overflow_bound && overflow_area_1 - overflow_area_0 >= 0 && cmode==1)
		{
			cout << " --------  CG break trigger -------- " << endl;
			break;
		}

		CountFactor++ ;
		StopEnhance = false ;

		WLterm = this->CalWireGradient( tmpnt );
		BDterm = this->CalDensityGradient( tmpnt );
		TMterm = this->CalThermalGradient( tmpnt  );

		RecordWLTerm = WLterm ;

		// Ensure the area cost is larger than thermal

		bool Start_Jump = false ;
		if(lambda * BDterm > lambdaT * TMterm   )
			Start_Jump = true ;
		else
			Start_Jump = false ;


		// Break Condition3 : (1) The area cost > thermal cost (2) Can't not improve in 3 iteration.
		if(cmode == 1 && Start_Jump && overflow_area_1-overflow_area_0 >= 0 )
		{
			//cout<< " - Start Jump "<<endl;
			set<double >::iterator it2;
			it2=Leave.find(overflow_area_1);
			if(it2 != Leave.end())
			{
				CountForJump ++ ;
			}
			else
			{
				Leave.insert( overflow_area_1 );
			}

			if(CountForJump >= 3)
			{
				CountForJump = 0;

				break;
			}

		}


		this->lambda *= OverlapL ;
		this->lambdaT *= ThermalL ;


		Fk_1 = this->lambdaW*WLterm + this->lambda * BDterm + this->lambdaT * TMterm ; /// Next Iter Cost Fcn.
		//  cout<<" ---- Update New Weight Cost : "<< Fk_1<<endl;
		//  cout<<" - WL : "<< lambdaW*WLterm <<endl;
		//  cout<<" - Density : "<< lambda * BDterm <<endl;
		//  cout<<" - Thermal : "<<lambdaT * TMterm <<endl<<endl;


		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			this->GX_1[i] = this->lambdaW*this->WG_X[i] + this->lambda * this->DG_X[i] +  this->lambdaT * this->TG_X[i] ;
			this->GY_1[i] = this->lambdaW*this->WG_Y[i] + this->lambda * this->DG_Y[i] +  this->lambdaT * this->TG_Y[i] ;

			this->GX_0[i] = this->GX_1[i];
			this->GY_0[i] = this->GY_1[i];
			this->DX_0[i] = 0;
			this->DY_0[i] = 0;
		}
	}

	this->CalculateHPWL_GP( tmpnt );


	// Check the stuked Modules and Pull out
	if(this->CurrentLevel == 0  )
	{
		cout<<" - Pull Stucked Module "<<endl;
		for(int i=0; i<tmpnt.nMod ; i++ )
		{

			if(tmpnt.mods[i].flagTSV== true) ///< TSV do need to pull
			{
				continue;
			}

			int colL = tmpnt.mods[i].GLeftX/binW ;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/binH ;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/binW ;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/binH ;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= nBinCol ) colR = nBinCol - 1;
			if( rowT >= nBinRow ) rowT = nBinRow - 1;

			set<int>::iterator iter;
			iter = this->SuperBigID.find( i );

			for (int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL ++)
			{
				for( int j = rowB; j <= rowT; j++ )
				{
					for(int k = colL ; k <= colR; k++)
					{

						// Note FirstEscapeBin record the fixed modules bin
						// The value of covered bins will large than zero
						// Therefore, the modules are needed to pull when two condition meet
						// (1) FirstEscpaeBin > 0 (2) modules are not fixed module
						if( FirstEscapeBin[modL][j][k] > 3 && iter==SuperBigID.end() )
						{

							tmpnt.mods[i].GCenterX = this->binW * bin3D[modL][j][k].FreeBin.second ;
							tmpnt.mods[i].GCenterY = this->binH * bin3D[modL][j][k].FreeBin.first ;

							tmpnt.mods[i].GLeftX = tmpnt.mods[i].GCenterX - tmpnt.mods[i].modW/2;
							tmpnt.mods[i].GLeftY = tmpnt.mods[i].GCenterY - tmpnt.mods[i].modH/2;

						}

					}
				}
			}


		}

		string dynamic_out = this->BinMapPath +"ThermalGradient_END";

		PLOT FinalPloter;
		FinalPloter.SetColoring(true);
		FinalPloter.PlotResultGL_dy( tmpnt, dynamic_out, 0 );

	}

	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		tmpnt.mods[i].CenterX = tmpnt.mods[i].GCenterX;
		tmpnt.mods[i].CenterY = tmpnt.mods[i].GCenterY;
		tmpnt.mods[i].LeftX = tmpnt.mods[i].GLeftX;
		tmpnt.mods[i].LeftY = tmpnt.mods[i].GLeftY;
	}

	delete [] this->Xp;
	delete [] this->Xn;
	delete [] this->Yp;
	delete [] this->Yn;

	delete [] this->TG_X;
	delete [] this->TG_Y;
	delete [] this->GX_0;
	delete [] this->GX_1;
	delete [] this->GY_0;
	delete [] this->GY_1;
	delete [] this->WG_X;
	delete [] this->WG_Y;
	delete [] this->DG_X;
	delete [] this->DG_Y;
	delete [] this->DX_1;
	delete [] this->DX_0;
	delete [] this->DY_1;
	delete [] this->DY_0;
	delete [] this->Cv;
	delete [] this->alpha;

	this->end_t = clock();

	double gp_t = (this->end_t - this->start_t)/(double)CLOCKS_PER_SEC;


	///f///0623 avoid memory error
	this->bin3D.clear();

	cout << "*************** Done ****************" << endl;
	cout << "-CPU Time           : " << gp_t << endl;
	cout << "-HPWL               : " << tmpnt.totalWL_GP << endl;
	cout << "-Overflow Ratio     : " << overflow_area_1/tmpnt.totalModArea << endl;
	cout << "-Number of CG       : " << numCG << endl;
	cout << "-Number of Iteration: " << numIter << endl;
	cout << "*************************************" << endl;

}

void THERMALGP::ClassifyMacro(NETLIST& tmpnt )
{
	/**
	 * \Input : factor of dev . amp
	 * \Output :Temperature flag of module
	 * \brief : Define the group of each module.
	 * \brief : Note that you can try different criteria to classify. ex. Pd or Power.
	 */




	// Store Data e.q. (1) Power (2) PD (3) Area

	vector <double> data;
	vector <double> PdData;
	vector <double> AreaData;

	data.resize(tmpnt.nMod - tmpnt.nTSV);
	PdData.resize(tmpnt.nMod- tmpnt.nTSV);
	AreaData.resize(tmpnt.nMod - tmpnt.nTSV);

	double Max_power = -1 ;
	double Min_Power = 1000;

	for(int i=0; i< tmpnt.nMod ; i++)
	{
		if( tmpnt.mods[i].flagTSV == false)
		{

			data[i] = tmpnt.mods[i].Power;
			PdData[i] = (double) tmpnt.mods[i].Power /tmpnt.mods[i].modArea;
			AreaData[i] = tmpnt.mods[i].modArea;

			if(tmpnt.mods[i].Power > Max_power )
				Max_power = tmpnt.mods[i].Power;
			if(tmpnt.mods[i].Power < Min_Power)
				Min_Power = tmpnt.mods[i].Power ;
		}

	}
	// Build Gaussian Analysis
	// First : Avg , Second : Dev

	pair <double,double > mean_dev;
	pair <double,double > PdMeanDev;
	pair <double,double > AreaMeanDev;

	mean_dev = UTILITY::GaussAnalysis(data);
	PdMeanDev = UTILITY::GaussAnalysis(PdData);
	AreaMeanDev = UTILITY::GaussAnalysis(AreaData);

	vector <int > cold ;
	vector <int > warm;
	vector <int > hot ;



	// ======= Top K ======= //
	
	vector < pair <int,double> > PowerData ;
	PowerData.resize(tmpnt.nMod -tmpnt.nTSV );
	for(int i = 0 ; i <tmpnt.nMod- tmpnt.nTSV ; i++)
	{
		pair <int, double> temp ;
		temp.first = i ;
		temp.second = data[i];
		PowerData[i] = temp;
	}
	sort(PowerData.begin(),PowerData.end(),SortByPower);	


	double TopK =  0.05 ; ///< Hot Define  Top x %
	double Warm =  0.2 ; ///< Warm Define Top x ~ y %
	int PickNum =  (tmpnt.nMod - tmpnt.nTSV) * TopK ;
	int WarmNum =  (tmpnt.nMod - tmpnt.nTSV) * Warm ;


	for(int i = 0 ; i < tmpnt.nMod - tmpnt.nTSV ; i++)
	{
		int ModuleID = PowerData[i].first ;
		if(tmpnt.mods[ModuleID].flagTSV== true)
		{
			tmpnt.mods[ModuleID].TemperatureGroup = 2;
			cout << "[ERROR] : TSV should located after the Modules in modArray "<<endl;
			cout << "[modArray] : <------Modules------> <------TSVs----->"<<endl;
			continue;
		}
		if( i < PickNum)
		{
			hot.push_back(ModuleID);
			tmpnt.mods[ModuleID].TemperatureGroup = 0; 
		}
		else if ( i >= PickNum && i < WarmNum)
		{
			warm.push_back(ModuleID);
			tmpnt.mods[ModuleID].TemperatureGroup = 1;
		}
		else
		{
			cold.push_back(ModuleID);
			tmpnt.mods[ModuleID].TemperatureGroup = 2;
		}
	}


	// ======= Gaussain distribution ======== //
/*
	double amp = 2 ; /// Gaussin STD.dev amplifier

	vector <int > HighPdIndex ;
	for(int i =0; i<tmpnt.nMod; i++)
	{
		double Pd = tmpnt.mods[i].Pdensity;
		if(Pd >= 2 * PdMeanDev.second + PdMeanDev.first)
			HighPdIndex.push_back(i);
	}



	double Max = -1 ;
	double Min = 99999;
	double Avg= 0;
	// Use Gaussian Analysis to Classify Module Group
	for(int i=0 ; i< tmpnt.nMod; i++)
	{
		double Pd = tmpnt.mods[i].Power;

		double area = tmpnt.mods[i].modW * tmpnt.mods[i].modH;
		if(Pd  <= mean_dev.first- 0.1 * amp * mean_dev.second   )
		{
			cold.push_back(i);
			tmpnt.mods[i].TemperatureGroup = 2 ;
		}
		else if (Pd >= mean_dev.first- 0.1 * amp * mean_dev.second   && Pd <= mean_dev.first+ amp*mean_dev.second)
		{
			warm.push_back(i);
			tmpnt.mods[i].TemperatureGroup = 1;
		}
		else
		{
			tmpnt.mods[i].TemperatureGroup = 0 ;
			hot.push_back(i);

		}
		if(Max < tmpnt.mods[i].Power)
			Max = tmpnt.mods[i].Power;
		if(Min > tmpnt.mods[i].Power)
			Min = tmpnt.mods[i].Power;
		Avg+=tmpnt.mods[i].Power;


	}
*/
	// SuperBig

	this->SuperBigID.clear();



	/// Define SuperBig Modulea
	for(int i =0; i< tmpnt.nMod ; i++ )
	{
		if( tmpnt.mods[i].modArea > this->DefineSuperBigPara *  AreaMeanDev.first  )
		{
			this->SuperBigID.insert( i );
			tmpnt.mods[i].TemperatureGroup = 2 ; //Define in cold group

		}
	}

	cout<<" ------- Power Info ------- "<<endl;
	cout<<" - MaxP		: "<< Max_power <<endl;
	cout<<" - MinP		: "<< Min_Power <<endl;
	cout<<" - mean		: "<< mean_dev.first<<endl;
	cout<<" - dev		: "<< mean_dev.second<<endl;
	cout<<" - cold		: "<<cold.size();
	cout<<" - warm		: "<<warm.size();
	cout<<" - hot		: "<<hot.size()<<endl;
	cout<<" - SuperBig	: "<<SuperBigID.size()<<endl;
	cout<<" --------------------------- "<<endl;


}

void THERMALGP::InitializeGradient( NETLIST& tmpnt)
{
	double WLterm = this->CalWireGradient( tmpnt );		///< wirelength term in the objective function
	double BDterm = this->CalDensityGradient( tmpnt );	///< density term in the objective function
	double THterm = this->CalThermalGradient(tmpnt);// this->CalThermalGradient( tmpnt );  ///< Thermal term in the obj fcn.
	double dk = 0;
	double wk = 0;
	double tk = 0;

	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		dk += sqrt( this->DG_X[i]*this->DG_X[i] + this->DG_Y[i]*this->DG_Y[i] );
		wk += sqrt( this->WG_X[i]*this->WG_X[i] + this->WG_Y[i]*this->WG_Y[i] );
		tk += sqrt( this->TG_X[i]*this->TG_X[i] + this->TG_Y[i]*this->TG_Y[i] );
	}


	this->lambdaW	= 1;
	this->lambda	= (wk/dk);
	this->lambdaT	= (wk/tk);

	this->Fk_1 = WLterm + this->lambda * BDterm + this->lambdaT * THterm;

	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		this->GX_1[i] = this->WG_X[i] + lambda * this->DG_X[i] + lambdaT * this->TG_X[i];
		this->GY_1[i] = this->WG_Y[i] + lambda * this->DG_Y[i] + lambdaT * this->TG_Y[i];

		this->GX_0[i] = this->GX_1[i];
		this->GY_0[i] = this->GY_1[i];
		this->DX_0[i] = 0;
		this->DY_0[i] = 0;
	}

}

double THERMALGP::CalWireGradient( NETLIST& tmpnt )
{
	double LSE_WL = 0;

	/// initial wirelength gradient
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		this->WG_X[i] = 0.0;
		this->WG_Y[i] = 0.0;
	}

	/// calculate each term in LSE wirelength model for modules
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		double tempX = tmpnt.mods[i].GCenterX/(double)AMP_PARA;
		double tempY = tmpnt.mods[i].GCenterY/(double)AMP_PARA;

		this->Xp[i] = EXP( (tempX/Gamma)  );
		this->Xn[i] = EXP( (-tempX/Gamma) );
		this->Yp[i] = EXP( (tempY/Gamma)  );
		this->Yn[i] = EXP( (-tempY/Gamma) );
	}


	for( int i = 0; i < tmpnt.nNet; i++ )
	{


		double XpSum = 0;		///< Sum(EXP(X))
		double XnSum = 0;		///< Sum(EXP(-X))
		double YpSum = 0;		///< Sum(EXP(Y))
		double YnSum = 0;		///< Sum(EXP(-Y))

		double XpSumInv = 0;	///< 1/Sum(EXP(X))
		double XnSumInv = 0;	///< 1/Sum(EXP(-X))
		double YpSumInv = 0;	///< 1/Sum(EXP(Y))
		double YnSumInv = 0;	///< 1/Sum(EXP(-Y))

		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{

			int modID = tmpnt.pins[j].index;
			XpSum += this->Xp[modID];
			XnSum += this->Xn[modID];
			YpSum += this->Yp[modID];
			YnSum += Yn[modID];
		}
		LSE_WL += ( log(XpSum) + log(XnSum) + log(YpSum) + log(YnSum) );

		XpSumInv = 1/XpSum;
		XnSumInv = 1/XnSum;
		YpSumInv = 1/YpSum;
		YnSumInv = 1/YnSum;

		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			int modID = tmpnt.pins[j].index;
			if( modID >= tmpnt.nMod )
				continue;

			this->WG_X[modID] += XpSumInv * this->Xp[modID];
			this->WG_X[modID] -= XnSumInv * this->Xn[modID];
			this->WG_Y[modID] += YpSumInv * this->Yp[modID];
			this->WG_Y[modID] -= YnSumInv * this->Yn[modID];

		}


	}



	LSE_WL *= this->Gamma;
	return LSE_WL;
}

double THERMALGP::CalDensityGradient( NETLIST& tmpnt )
{
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		this->DG_X[i] = 0;
		this->DG_Y[i] = 0;
	}

	BIN3D::iterator bin3D_it;
	for( bin3D_it = this->bin3D.begin(); bin3D_it != this->bin3D.end(); bin3D_it++ )
	{
		for( int i = 0; i < this->nBinRow; i++ )
		{
			for( int j = 0; j < this->nBinCol; j++ )
			{
				bin3D_it->second[i][j].OccupyValue = 0;
				bin3D_it->second[i][j].EnableValue = 0;
			}
		}
	}

	double WbHb = ((float)binW/(float)AMP_PARA) * ((float)binH/(float)AMP_PARA);
	double totalBinDensity = 0;

	/// calculate OccupyValue in each bin & Cv
	for( int i = 0; i < tmpnt.nMod; i++ )
	{

		///f///0925 stacked module has multiple layer
		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )
		{
			if( modL >= tmpnt.nLayer )
			{
				cerr << "error  : stack's layer out of range" << endl;
				exit(EXIT_FAILURE);
			}


			int colL = tmpnt.mods[i].GLeftX/binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].ShrinkW)/binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].ShrinkH)/binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= this->nBinCol ) colR = this->nBinCol - 1;
			if( rowT >= this->nBinRow ) rowT = this->nBinRow - 1;

			double Db = 0;
			double *P_X = new double [colR - colL + 1];		///< potential x
			double *P_Y = new double [rowT - rowB + 1];		///< potential y

			double wb = (double)binW/(double)AMP_PARA;					///< width of bin
			double wv = (double)tmpnt.mods[i].ShrinkW/(double)AMP_PARA;	///< width of module
			double hb = (double)binH/(double)AMP_PARA;					///< height of bin
			double hv = (double)tmpnt.mods[i].ShrinkH/(double)AMP_PARA;	///< height of module

			double ax = 4/((wv+2*wb)*(wv+4*wb));
			double bx = 2/((wb)*(wv+4*wb));
			double ay = 4/((hv+2*hb)*(hv+4*hb));
			double by = 2/((hb)*(hv+4*hb));

			double range1_w = wv/2 + wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range2_w = wv/2 + 2*wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range1_h = hv/2 + hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)
			double range2_h = hv/2 + 2*hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)



			for( int j = rowB; j <= rowT; j++ )
			{
				double disY = (double)(this->bin3D[modL][j][colL].y + this->bin3D[modL][j][colL].h/2 - tmpnt.mods[i].GCenterY)/(double)AMP_PARA;
				if( disY < 0 )
				{
					disY = -disY;
				}
				if( disY <= range1_h && disY >= 0 )
				{
					P_Y[j-rowB] = 1 - ay * pow( disY, 2.0 );
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					P_Y[j-rowB] = by * pow( (disY-range2_h), 2.0 );
				}
				else
				{
					P_Y[j-rowB] = 0;
				}
			}


			for( int k = colL; k <= colR; k++ )
			{
				double disX = (double)(this->bin3D[modL][rowB][k].x + this->bin3D[modL][rowB][k].w/2 - tmpnt.mods[i].GCenterX)/(double)AMP_PARA;
				if( disX < 0 )
				{
					disX = -disX;
				}

				if( disX <= range1_w && disX >= 0 )
				{
					P_X[k-colL] = 1 - ax * pow( disX, 2.0 );
				}
				else if( disX <= range2_w && disX > range1_w )
				{
					P_X[k-colL] = bx * pow( (disX-range2_w), 2.0 );
				}
				else
				{
					P_X[k-colL] = 0;
				}
			}

			for( int j = 0; j <= (rowT-rowB); j++ )
			{
				for( int k = 0; k <= (colR-colL); k++ )
				{
					Db += (P_X[k] * P_Y[j]);
				}
			}

			if(Db == 0)
				this->Cv[i] = 0;
			else
				this->Cv[i] = (double) (hv *wv )/Db;

			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{
					this->bin3D[modL][j][k].OccupyValue += this->Cv[i] * P_X[k-colL] * P_Y[j-rowB];
				}
			}

			delete [] P_X;
			delete [] P_Y;


		}///f///0925


	}



	/// calculate density gradient
	for( int i = 0; i < tmpnt.nMod; i++ )
	{

		///f///0925 stacked module has multiple layer
		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )
		{
			if( modL >= tmpnt.nLayer )
			{
				cerr << "error  : stack's layer out of range" << endl;
				exit(EXIT_FAILURE);
			}


			int colL = tmpnt.mods[i].GLeftX/binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].ShrinkW)/binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].ShrinkH)/binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= this->nBinCol ) colR = this->nBinCol - 1;
			if( rowT >= this->nBinRow ) rowT = this->nBinRow - 1;

			double *P_X = new double [colR - colL + 1];		///< potential x
			double *P_Y = new double [rowT - rowB + 1];		///< potential y
			double *dP_X = new double [colR - colL + 1];	///< differential potential x
			double *dP_Y = new double [rowT - rowB + 1];	///< differential potential y

			double wb = (double)binW/(double)AMP_PARA;					///< width of bin
			double wv = (double)tmpnt.mods[i].ShrinkW/(double)AMP_PARA;	///< width of module
			double hb = (double)binH/(double)AMP_PARA;					///< height of bin
			double hv = (double)tmpnt.mods[i].ShrinkH/(double)AMP_PARA;	///< height of module

			double ax = 4/((wv+2*wb)*(wv+4*wb));
			double bx = 2/((wb)*(wv+4*wb));
			double ay = 4/((hv+2*hb)*(hv+4*hb));
			double by = 2/((hb)*(hv+4*hb));

			double range1_w = wv/2 + wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range2_w = wv/2 + 2*wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range1_h = hv/2 + hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)
			double range2_h = hv/2 + 2*hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)


			for( int j = rowB; j <= rowT; j++ )
			{
				double disY = (double)(this->bin3D[modL][j][colL].y + this->bin3D[modL][j][colL].h/2 - tmpnt.mods[i].GCenterY)/(double)AMP_PARA;
				int sign;
				if( disY < 0 )
				{
					disY = -disY;
					sign = 1;
				}
				else
				{
					sign = -1;
				}

				if( disY <= range1_h && disY >= 0 )
				{
					P_Y[j-rowB] = 1 - ay * pow( disY, 2.0 );
					dP_Y[j-rowB] = -2 * sign * ay * disY;
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					P_Y[j-rowB] = by * pow( (disY-range2_h), 2.0 );
					dP_Y[j-rowB] = 2 * sign * by * (disY-range2_h);
				}
				else
				{
					P_Y[j-rowB] = 0;
					dP_Y[j-rowB] = 0;
				}
			}

			for( int k = colL; k <= colR; k++ )
			{
				double disX = (double)(this->bin3D[modL][rowB][k].x + this->bin3D[modL][rowB][k].w/2 - tmpnt.mods[i].GCenterX)/(double)AMP_PARA;
				int sign;
				if( disX < 0 )
				{
					disX = -disX;
					sign = 1;
				}
				else
				{
					sign = -1;
				}

				if( disX <= range1_w && disX >= 0 )
				{
					P_X[k-colL] = 1 - ax * pow( disX, 2.0 );
					dP_X[k-colL] = -2 * sign * ax * disX;
				}
				else if( disX <= range2_w && disX > range1_w )
				{
					P_X[k-colL] = bx * pow( (disX-range2_w), 2.0 );
					dP_X[k-colL] = 2 * sign * bx * (disX-range2_w);
				}
				else
				{
					P_X[k-colL] = 0;
					dP_X[k-colL] = 0;
				}
			}

			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{
					if(this->TFM== true)
					{

					if(this->Change == true)
						this->bin3D[modL][j][k].EnableValue = WbHb - 0.3 * this->FirstEscapeBin[modL][j][k] * WbHb;
					else
						bin3D[modL][j][k].EnableValue = WbHb - 0.03 * this->FirstEscapeBin[modL][j][k] * WbHb;
					}
					else
					{

						bin3D[modL][j][k].EnableValue = WbHb ;
					}

					this->DG_X[i] += dP_X[k-colL] * P_Y[j-rowB] * this->Cv[i] * (this->bin3D[modL][j][k].OccupyValue - this->bin3D[modL][j][k].EnableValue) * 2;
					this->DG_Y[i] += P_X[k-colL] * dP_Y[j-rowB] * this->Cv[i] * (this->bin3D[modL][j][k].OccupyValue - this->bin3D[modL][j][k].EnableValue) * 2;
				}
			}

			delete [] P_X;
			delete [] P_Y;
			delete [] dP_X;
			delete [] dP_Y;

		}///f///0925

	}
	// Plot Utilization Map
/*
	string label = UTILITY::Int_2_String(CountFactor);
	string OutFile =this->BinMapPath +"EnableArea"+label;

	this->PlotBinMap(tmpnt,OutFile,"Density");
*/
	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		for( int j = 0; j < this->nBinRow; j++ )
		{
			for( int k = 0; k < this->nBinCol; k++ )
			{
				totalBinDensity += pow( (this->bin3D[i][j][k].OccupyValue - this->bin3D[i][j][k].EnableValue), 2.0 );
			}
		}
	}

	return totalBinDensity;
}


double THERMALGP::CalThermalGradient( NETLIST& tmpnt )
{
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		this->TG_X[i] = 0;
		this->TG_Y[i] = 0;
	}

	BIN3D::iterator bin3D_it;
	//initilaize the bin power
	for( bin3D_it = this->bin3D.begin(); bin3D_it != this->bin3D.end(); bin3D_it++ )
	{
		for( int i = 0; i < this->nBinRow; i++ )
		{
			for( int j = 0; j < this->nBinCol; j++ )
			{
				bin3D_it->second[i][j].OccupyValue = 0 ;
				bin3D_it->second[i][j].EnableValue = 0 ;
				bin3D_it->second[i][j].AddPotential = false ;

			}
		}
	}


	double TemperatureCost 	= 0;
	double avr_thermal		= 0;
	double max_power 		= 0, min_power = 100000000;
	double PowerSum 		= 0;

	///<  Bell-Shaped : calculate occupyPower in each bin & Cv
	for( int i = 0; i < tmpnt.nMod; i++ )
	{

		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )
		{
			if( modL >= tmpnt.nLayer )
			{
				cerr << "error  : stack's layer out of range" << endl;
				exit(EXIT_FAILURE);
			}


			int colL = tmpnt.mods[i].GLeftX/this->binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/this->binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/this->binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/this->binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= this->nBinCol ) colR = this->nBinCol - 1;
			if( rowT >= this->nBinRow ) rowT = this->nBinRow - 1;

			double Db = 0; //Cv* Px* Py
			double *P_X = new double [colR - colL + 1];		///< potential x
			double *P_Y = new double [rowT - rowB + 1];		///< potential y

			double wb = (double)this->binW/(double)AMP_PARA;					///< width of bin
			double wv = (double)tmpnt.mods[i].modW/(double)AMP_PARA;	///< width of module
			double hb = (double)this->binH/(double)AMP_PARA;					///< height of bin
			double hv = (double)tmpnt.mods[i].modH/(double)AMP_PARA;	///< height of module

			double ax = 4/((wv+2*wb)*(wv+4*wb));
			double bx = 2/((wb)*(wv+4*wb));
			double ay = 4/((hv+2*hb)*(hv+4*hb));
			double by = 2/((hb)*(hv+4*hb));

			double range1_w = wv/2 + wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range2_w = wv/2 + 2*wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range1_h = hv/2 + hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)
			double range2_h = hv/2 + 2*hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)



			for( int j = rowB; j <= rowT; j++ )
			{
				double disY = (double)(this->bin3D[modL][j][colL].y + this->bin3D[modL][j][colL].h/2 - tmpnt.mods[i].GCenterY)/(double)AMP_PARA;
				if( disY < 0 )
				{
					disY = -disY;
				}
				if( disY <= range1_h && disY >= 0 )
				{
					P_Y[j-rowB] = 1 - ay * pow( disY, 2.0 );
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					P_Y[j-rowB] = by * pow( (disY-range2_h), 2.0 );
				}
				else
				{
					P_Y[j-rowB] = 0;
				}
			}


			for( int k = colL; k <= colR; k++ )
			{
				double disX = (double)(this->bin3D[modL][rowB][k].x + this->bin3D[modL][rowB][k].w/2 - tmpnt.mods[i].GCenterX)/(double)AMP_PARA;
				if( disX < 0 )
				{
					disX = -disX;
				}

				if( disX <= range1_w && disX >= 0 )
				{
					P_X[k-colL] = 1 - ax * pow( disX, 2.0 );
				}
				else if( disX <= range2_w && disX > range1_w )
				{
					P_X[k-colL] = bx * pow( (disX-range2_w), 2.0 );
				}
				else
				{
					P_X[k-colL] = 0;
				}
			}

			for( int j = 0; j <= (rowT-rowB); j++ )
			{
				for( int k = 0; k <= (colR-colL); k++ )
				{
					Db += (P_X[k] * P_Y[j]);
				}
			}

			Cv[i] = tmpnt.mods[i].modArea/Db;


			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{
					this->bin3D[modL][j][k].OccupyValue += tmpnt.mods[i].Pdensity * Cv[i] * P_X[k-colL] * P_Y[j-rowB] ;  //yan-fu
					PowerSum+= this->bin3D[modL][j][k].OccupyValue ;
				}
			}

			delete [] P_X;
			delete [] P_Y;


		}


	}



	///< PowerMap -> ThermalMap

	for(int z=0; z<tmpnt.nLayer; z++)
	{
		// Initital Power Matrix 
		vector < vector <double> > PowerMap;
		PowerMap.resize( 3 * this->nBinRow );
		for(int j = 0 ; j < 3 * this->nBinRow; j++ )
		{
			PowerMap[j].resize(this->nBinCol * 3);
		}
		for(int j=0; j<this->nBinRow; j++)
		{
			for(int k=0; k<this->nBinCol; k++)
			{
				//lbin[j][k] = this->bin3D[z][j][k].OccupyValue * 0.001 ;////test
				PowerMap[j+this->nBinRow][k+this->nBinCol] = this->bin3D[z][j][k].OccupyValue  ;////test
				this->bin3D[z][j][k].OccupyValue = 0 ;
			}
		}
		// PowerMap (With Mirrow)
		//PLOT::PlotMap(PowerMap,this->BinMapPath + "Powermap.m",this->binW,this->binH);
		
		// Mirror
		this->MirrorMethod(PowerMap);	

		///  PowerMap (With Mirror Method) [Convloution]  ThermalMask  
		vector <vector <double> > Results ; 		/// Store Result
		FFTW::Convolution(PowerMap,ThermalMask, Results); /// (PotentailMap, Mask , Output)


		// Extract Results
		for(int j=this->nBinRow; j<this->nBinRow*2; j++)
		{
			for(int k=this->nBinCol; k<this->nBinCol*2; k++)
			{
				this->bin3D[z][j-this->nBinRow][k-this->nBinCol].OccupyValue = Results[j][k];
			}
		}


	}

	// ThermalMap Done
	double ThermalSum=0;

	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		for( int j = 0; j < this->nBinRow; j++ )
		{
			for( int k = 0; k < this->nBinCol; k++ )
			{


				if( this->bin3D[i][j][k].OccupyValue > max_power )
				{
					max_power = this->bin3D[i][j][k].OccupyValue ;
				}
				if( this->bin3D[i][j][k].OccupyValue < min_power )
				{
					min_power = this->bin3D[i][j][k].OccupyValue ;
				}
				ThermalSum += this->bin3D[i][j][k].OccupyValue ;

			}
		}
	}

	avr_thermal = (double) ThermalSum/( this->nBinRow*this->nBinCol*tmpnt.nLayer );

	///< The Tg is Done, and we need to evaluate new Cg

	static int CountInner = 0;

	if(StopEnhance == false)
		CountInner = 0 ;

	vector <vector <double> > NewMb ;
	if(this->TFM == true)
	{

		for (int z = 0 ; z < tmpnt.nLayer ; z++)
		{

			///< Allowable Temperature Ratio : rho
			// This will affect the Thermal Force Modulation Scope(TFM) 
			if ( StopEnhance==false  && CountFactor != 0 )
			{
				if( this->rho < 2 )
					this->rho = this->rho + this->EnhanceRatio ;

			}


			vector <vector < pair<int, int > > >FreeBin = IdentifyFreeBin(tmpnt,  avr_thermal * this->rho  ); /// Define the ColdBin Set

			///< Set Target Temperature


			///< Set the Target T in bin3D.EnableValue
			this->SetTargetT(CountInner,max_power,FreeBin, z); 


			///< Initialize New Target Temperature Cg
			vector <vector <double> > MbValue;
			MbValue.resize(this->nBinRow);
			for(int i=0 ; i <this->nBinRow ; i++)
			{
				MbValue[i].resize(this->nBinCol);
			}
			for(int i = 0 ; i < this->nBinRow ; i++)
				for(int j=0 ; j< this->nBinCol ; j++)
					MbValue[i][j]=0;

			///< Extract EnableValue Cg
			for(int i = 0 ; i < this->nBinRow ; i ++)
			{
				for(int j=0  ; j< this->nBinCol ; j++)
				{
					MbValue[i][j]= this->bin3D[z][i][j].EnableValue;

				}
			}


			// Note that original eq. is - > (Tg-enable)^2 , which enable is ( AvgT )
			// For conveniencely, I rewrite as (Tg + enable)^2, which enable is ( NewMb - AvgT) rather than AvgT-NewMb (original)
			// NewMb is -> ( log fcn according to dist to freeBin and Convolution with GaussianMask)

			FFTW::Convolution( MbValue, GaussianMask, NewMb) ;

			for(int i = 0 ; i < this->nBinRow ; i ++)
			{
				for(int j=0  ; j< this->nBinCol ; j++)
				{
					this->bin3D[z][i][j].EnableValue= NewMb[i][j] - avr_thermal  ;
					// this->bin3D[0][i][j].EnableValue= -1 * avr_thermal  ; // Original
				}
			}
		}
	}
	else
	{
		for(int z = 0 ; z< tmpnt.nLayer ; z++)
			for(int i = 0 ; i< this->nBinRow ; i++)
				for(int j = 0 ; j < this->nBinCol ; j++)
					bin3D[z][i][j].EnableValue = - avr_thermal;
	}


	double oF = 0 ;

	for(int z = 0 ; z< tmpnt.nLayer ; z++)
		for(int i = 0 ; i< this->nBinRow ; i++)
			for(int j = 0 ; j < this->nBinCol ; j++)
				oF += max( this->bin3D[z][i][j].OccupyValue + this->bin3D[z][i][j].EnableValue, 0.0 ) ;


	this->thermal_difference_1 = (double) (oF) / ThermalSum ; ///thermal test percent 20170320


	double average_temp = avr_thermal;


	///< PLOT Thermal MAP

	//this->PlotThermalMap( NewMb, tmpnt);
	

	///< Calculate Thermal gradient
	for( int i = 0; i < tmpnt.nMod; i++ )
	{

		///f///0925 stacked module has multiple layer
		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )
		{
			if( modL >= tmpnt.nLayer )
			{
				cerr << "error  : stack's layer out of range" << endl;
				exit(EXIT_FAILURE);
			}

			int colL = tmpnt.mods[i].GLeftX/this->binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/this->binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/this->binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/this->binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= this->nBinCol ) colR = this->nBinCol - 1;
			if( rowT >= this->nBinRow ) rowT = this->nBinRow - 1;

			double *P_X = new double [colR - colL + 1];		///< potential x
			double *P_Y = new double [rowT - rowB + 1];		///< potential y
			double *dP_X = new double [colR - colL + 1];	///< differential potential x
			double *dP_Y = new double [rowT - rowB + 1];	///< differential potential y

			double wb = (double)this->binW/(double)AMP_PARA;					///< width of bin
			double wv = (double)tmpnt.mods[i].modW/(double)AMP_PARA;	///< width of module
			double hb = (double)this->binH/(double)AMP_PARA;					///< height of bin
			double hv = (double)tmpnt.mods[i].modH/(double)AMP_PARA;	///< height of module

			double ax = 4/((wv+2*wb)*(wv+4*wb));
			double bx = 2/((wb)*(wv+4*wb));
			double ay = 4/((hv+2*hb)*(hv+4*hb));
			double by = 2/((hb)*(hv+4*hb));

			double range1_w = wv/2 + 1*wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range2_w = wv/2 + 2*wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range1_h = hv/2 + 1*hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)
			double range2_h = hv/2 + 2*hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)



			for( int j = rowB; j <= rowT; j++ )
			{
				double disY = (double)(this->bin3D[modL][j][colL].y + this->bin3D[modL][j][colL].h/2 - tmpnt.mods[i].GCenterY)/(double)AMP_PARA;
				int sign;
				if( disY < 0 )
				{
					disY = -disY;
					sign = 1;
				}
				else
				{
					sign = -1;

				}

				if( disY <= range1_h && disY >= 0 )
				{
					P_Y[j-rowB] = 1 - ay * pow( disY, 2.0 );
					dP_Y[j-rowB] = -2 * sign * ay * disY;
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					P_Y[j-rowB] = by * pow( (disY-range2_h), 2.0 );
					dP_Y[j-rowB] = 2 * sign * by * (disY-range2_h);
				}
				else
				{
					P_Y[j-rowB] = 0;
					dP_Y[j-rowB] = 0;
				}
			}

			for( int k = colL; k <= colR; k++ )
			{
				double disX = (double)(this->bin3D[modL][rowB][k].x + this->bin3D[modL][rowB][k].w/2 - tmpnt.mods[i].GCenterX)/(double)AMP_PARA;
				int sign;// identify the macro at bin left or right
				if( disX < 0 ) // at left of bin
				{
					disX = -disX;
					sign = 1 ;

				}
				else
				{
					sign = -1 ;
				}

				if( disX <= range1_w && disX >= 0 )
				{
					P_X[k-colL] = 1 - ax * pow( disX, 2.0 );
					dP_X[k-colL] = -2 * sign * ax * disX;

				}
				else if( disX <= range2_w && disX > range1_w )
				{
					P_X[k-colL] = bx * pow( (disX-range2_w), 2.0 );
					dP_X[k-colL] = 2 * sign * bx * (disX-range2_w);

				}
				else
				{
					P_X[k-colL] = 0;
					dP_X[k-colL] = 0;
				}

			}
			/*
			   int MaskCenterX = (int) (this->nBinRow * 3 -1) /2 ;
			   int MaskCenterY = (int) (this->nBinCol * 3 -1) /2 ;

			   int ModCenterX = ( tmpnt.mods[i].GCenterX / this->binW ) ;
			   int ModCenterY = ( tmpnt.mods[i].GCenterY / this->binH ) ;*/
			// int DistFromCenter = sqrt( pow( ModCenterX,2.0 ) + pow(ModCenterY, 2.0 )  ) ;


			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{
					/*

					   int MaskDistX = abs(j - ModCenterX) ;
					   int MaskDistY = abs(k - ModCenterY) ;
					   double MaskValue = ThermalMask[MaskCenterX+MaskDistX][MaskCenterY+MaskDistY];*/
					double Cg = this->bin3D[modL][j][k].EnableValue;


					// TG_X[i] += dP_X[k-colL] * P_Y[j-rowB] * Cv[i] * tmpnt.mods[i].Pdensity * max ( ( this->bin3D[modL][j][k].OccupyValue + Cg ) * 2, 0.0)    ;  //Tai-Ting
					TG_X[i] += dP_X[k-colL] * P_Y[j-rowB] * Cv[i] * tmpnt.mods[i].Pdensity * ( this->bin3D[modL][j][k].OccupyValue + Cg ) * 2   ;  //Tai-Ting
					//  TG_Y[i] += P_X[k-colL] * dP_Y[j-rowB] * Cv[i] * tmpnt.mods[i].Pdensity * max ( ( this->bin3D[modL][j][k].OccupyValue + Cg ) * 2, 0.0)    ;  //Tai-Ting
					TG_Y[i] += P_X[k-colL] * dP_Y[j-rowB] * Cv[i] * tmpnt.mods[i].Pdensity * ( this->bin3D[modL][j][k].OccupyValue + Cg ) * 2    ;  //Tai-Ting


				}
			}

			delete [] P_X;
			delete [] P_Y;
			delete [] dP_X;
			delete [] dP_Y;

		}
		//  getchar();
	}



	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		for( int j = 0; j < this->nBinRow; j++ )
		{
			for( int k = 0; k < this->nBinCol; k++ )
			{
				double Cg = this->bin3D[i][j][k].EnableValue ;

				TemperatureCost += pow( (this->bin3D[i][j][k].OccupyValue + Cg), 2.0 );

			}
		}
	}

	this->StopEnhance = true;
	CountInner++ ;

	return TemperatureCost  ;
}
void THERMALGP::MirrorMethod( vector < vector<double> > &Matrix)
{
	///< Power Blurring : Mirrorw Method
	///< Extend Map 1*1 - > Map 3*3
	for(int j=0; j<this->nBinRow; j++)
	{
		for(int k=0; k<this->nBinCol; k++)
		{
			Matrix[this->nBinRow-1-j][k+this->nBinCol] = 1 * Matrix[j+this->nBinRow][k+this->nBinCol];
			Matrix[this->nBinRow*3-1-j][k+this->nBinCol] = 1 * Matrix[j+this->nBinRow][k+this->nBinCol];
		}
	}
	for(int j=0; j<this->nBinRow*3; j++)
	{
		for(int k=0; k<this->nBinCol; k++)
		{
			Matrix[j][this->nBinRow-1-k] = 1 * Matrix[j][k+this->nBinCol];
			Matrix[j][this->nBinRow*3-1-k] = 1 * Matrix[j][k+this->nBinCol];
		}
	}
}
void THERMALGP::SetTargetT(int CountInner, double max_power, vector<vector < pair<int, int > > >&FreeBin ,int LayerIndex  )
{

	double Target_Thermal = max_power ;
	int Distance = this->nBinRow / 2;
	double LogBase = pow (Distance, (double) 1/ Target_Thermal  );
	double LogLower = log10 (LogBase);


		for( int j = 0 ; j < this->nBinRow; j++ )
		{
			for( int k = 0; k < this->nBinCol; k++ )
			{

				if(this->bin3D[LayerIndex][j][k].AddPotential == false )
				{

					//enhance Potential to let cell leave

					double dist;
					if(CountFactor == 0 && CountInner==0)
						dist = this->nBinCol + this->nBinRow ;
					else
						dist = this->bin3D[LayerIndex][j][k].EscapeDist ;

					double Distlast = dist;

					//UPdate Dist

					if( this->bin3D[LayerIndex][j][k].FlagCold == false ) // Cold bin don't need to caculate
					{
						for(int t = 0 ;(int) t < FreeBin[LayerIndex].size() ; t++)
						{
							double temp;
							temp = abs( FreeBin[LayerIndex][t].first - j ) + abs( FreeBin[LayerIndex][t].second- k) ;
							if(temp < dist )
							{
								dist = temp ;
								this->bin3D[LayerIndex][j][k].FreeBin = FreeBin[LayerIndex][t];
								this->bin3D[LayerIndex][j][k].EscapeDist = dist ;
							}
						}
						dist = this->bin3D[LayerIndex][j][k].EscapeDist ; /// update new dist


					}
					else
					{
						dist = 0 ;
						pair <int, int  > temp ;
						temp.first = j ;
						temp.second = k ;
						this->bin3D[LayerIndex][j][k].EscapeDist = dist ;
						this->bin3D[LayerIndex][j][k].FreeBin =temp ;

					}


					if( dist < 1  )
						dist = 1 ;



					double DistTerm = (double)  log10( dist ) / LogLower ;

					double LinearSlope = (double) Target_Thermal / Distance ;

					double DistTermLinear = LinearSlope * dist  ;

					// cout<< "Dist : "<< dist <<" Enable : "<< DistTerm <<endl;

					this->bin3D[LayerIndex][j][k].EnableValue =   DistTerm  ; /// new Enable Cg'

				}

				this->bin3D[LayerIndex][j][k].AddPotential = true ;

			}

		}
}

void THERMALGP::ConjugateGradient( NETLIST& tmpnt )
{
	int count = 0;

	int *originX = new int [tmpnt.nMod];
	int *originY = new int [tmpnt.nMod];

	do
	{
		count++;

		// calculate beta
		double beta = 0;
		double beta_upper = 0;	///< numerator of beta
		double beta_lower = 0;	///< denominator of beta
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			beta_upper += ( this->GX_1[i] * (this->GX_1[i]-this->GX_0[i]) ) + ( this->GY_1[i] * (this->GY_1[i]-this->GY_0[i]) );
			beta_lower += ( fabs(this->GX_0[i]) + fabs(this->GY_0[i]) );
		}
		beta_lower *= beta_lower;
		beta = beta_upper/beta_lower;

		// calculate conjuagte directions d_k
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			this->DX_1[i] = (-this->GX_1[i] + beta * this->DX_0[i]);
			this->DY_1[i] = (-this->GY_1[i] + beta * this->DY_0[i]);
		}

		// calculate alpha
		double alpha_lower = 0;	///< denominator of beta
		double s = 0.2;			///< user-specified scaling factor
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			alpha_lower = sqrt( this->DX_1[i]*this->DX_1[i] + this->DY_1[i]*this->DY_1[i] );
			this->alpha[i]    = (s * binW/(double)AMP_PARA)/alpha_lower;
		}

		// update the solution
		for( int i = 0; i < tmpnt.nMod; i++ )
		{

			set<int>::iterator iter;
			iter = SuperBigID.find( i );
			if(iter!= SuperBigID.end() && this->TFM == true) /// Fix SuperBig Module
			{
				originX[i] = tmpnt.mods[i].GCenterX;
				originY[i] = tmpnt.mods[i].GCenterY;
				tmpnt.mods[i].GCenterX += 0;
				tmpnt.mods[i].GLeftX += 0;
				tmpnt.mods[i].GCenterY += 0;
				tmpnt.mods[i].GLeftY += 0;

			}
			else
			{

				originX[i] = tmpnt.mods[i].GCenterX;
				originY[i] = tmpnt.mods[i].GCenterY;

				tmpnt.mods[i].GCenterX += (int)(this->alpha[i] * this->DX_1[i] * AMP_PARA);
				tmpnt.mods[i].GLeftX += (int)(this->alpha[i] * this->DX_1[i] * AMP_PARA);

				// Fixed-outline
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

				tmpnt.mods[i].GCenterY += (int)(this->alpha[i] * this->DY_1[i] * AMP_PARA);
				tmpnt.mods[i].GLeftY += (int)(this->alpha[i] * this->DY_1[i] * AMP_PARA);

				if( tmpnt.mods[i].GLeftY < 0 )
				{
					tmpnt.mods[i].GLeftY = 0;
					tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
				}
				if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
				{
					tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
					tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
				}
				// Fence-Region
				if( tmpnt.mods[i].TemperatureGroup == 0 )
				{
					if( tmpnt.mods[i].GLeftX <= 2*binW )
					{
						tmpnt.mods[i].GLeftX = 2*binW ;
						tmpnt.mods[i].GCenterX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW/2 ;
					}
					if( tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW >= tmpnt.ChipWidth - 2 * binW )
					{
						tmpnt.mods[i].GLeftX = tmpnt.ChipWidth - tmpnt.mods[i].modW - 2 *binW;
						tmpnt.mods[i].GCenterX = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW/2 ;
					}
					// y direction

					if( tmpnt.mods[i].GLeftY <= 2*binH )
					{
						tmpnt.mods[i].GLeftY = 2*binH;
						tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
					}
					if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH >= tmpnt.ChipHeight -2 * binH )
					{
						tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH - 2 * binH ;
						tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
					}

				}
			}
		}

		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			this->GX_0[i] = this->GX_1[i];
			this->GY_0[i] = this->GY_1[i];
			this->DX_0[i] = this->DX_1[i];
			this->DY_0[i] = this->DY_1[i];
		}

		Fk_0 = Fk_1;

		double WLterm = this->CalWireGradient( tmpnt );
		double BDterm = this->CalDensityGradient( tmpnt );
		double THterm = this->CalThermalGradient( tmpnt );

		Fk_1 = WLterm + this->lambda * BDterm + this->lambdaT * THterm;

		if( Fk_1 < Fk_0 || count <= 5000 )
		{
			for( int i = 0; i < tmpnt.nMod; i++ )
			{


				this->GX_1[i] = this->WG_X[i] + lambda * this->DG_X[i] + this->lambdaT * TG_X[i];
				this->GY_1[i] = this->WG_Y[i] + lambda * this->DG_Y[i] + this->lambdaT * TG_Y[i];



			}
		}

	}
	while( Fk_1 < Fk_0 && count <= 5000 );

	if( Fk_1 > Fk_0 )	// recover the solution to former iteration
	{
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			tmpnt.mods[i].GCenterX = originX[i];
			tmpnt.mods[i].GCenterY = originY[i];
			tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
			tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
		}
	}
	this->numIter += count;

	delete [] originX;
	delete [] originY;
}

void THERMALGP::CutBin( NETLIST& tmpnt)
{
	cout << " ----- Cut Bin Info -----" << endl;

	double tsvArea = pow( TSV_PITCH, 2.0 );

	double AvgModLength = sqrt((double)(tmpnt.totalModArea - tmpnt.nTSV * tsvArea )/(double)(tmpnt.nMod - tmpnt.nTSV));
	int numBin = (int)((double)tmpnt.ChipHeight/(double)AMP_PARA/AvgModLength);

	THERMALGP::BIN2D tempBin;

	this->nBinRow = this->nBinCol = numBin * InputOption.bin_para;


	tempBin.resize( this->nBinRow );
	for( int i = 0; i < this->nBinRow; i++ )
	{
		tempBin[i].resize( this->nBinCol );
	}


	this->binW = (int)((tmpnt.ChipWidth/((float)AMP_PARA)) / ((float)this->nBinCol) * AMP_PARA);
	this->binH = (int)((tmpnt.ChipHeight/((float)AMP_PARA)) / ((float)this->nBinRow) * AMP_PARA);

	cout << " - nBinRow : " << this->nBinRow << endl;
	cout << " - nBinCol : " << this->nBinCol << endl;
	cout << " - binW   : " << this->binW << endl;
	cout << " - binH   : " << this->binH << endl;

	/// Initial
	for( int i = 0; i < this->nBinRow; i++ )
	{
		for( int j = 0; j < this->nBinCol; j++ )
		{
			pair <int, int > temp ;
			temp.first 					= i;
			temp.second					= j ;
			tempBin[i][j].w				= this->binW;
			tempBin[i][j].h				= this->binH;
			tempBin[i][j].x				= j * this->binW;
			tempBin[i][j].y				= i * this->binH;
			tempBin[i][j].EnableValue	= 0;
			tempBin[i][j].OccupyValue	= 0;
			tempBin[i][j].FreeBin 		= temp;      /// The nearest Free Bin b' for the current Bin b
			tempBin[i][j].EscapeDist 	= 0;         /// The Distance between b' and b
			tempBin[i][j].flagHasPre 	= false ;    /// Prefixed Flag
			tempBin[i][j].FlagCold		= true ;
			tempBin[i][j].AddPotential 	= false ;    /// Flat to avoid double Add Potential in one bin.
		}
	}


	// Assign the Bin Map Value
	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		this->bin3D[i] = tempBin;
	}

	tempBin.clear();
	BIN2D (tempBin).swap(tempBin);

	// Initilize EscapeMap

	// Check Average Power (Bin)
	double PowerSum=0;
	for(int i=0; i<tmpnt.nMod; i++)
	{
		PowerSum+= tmpnt.mods[i].Power ;
	}
	double avgPower = (double) PowerSum / (this->nBinCol * this->nBinCol);

	// Setting those bin which are covered by fixed module

	set<int>::iterator iter;
	double FreeBin = ((float)this->binW/(float)AMP_PARA) * ((float)this->binH/(float)AMP_PARA) ;


	for(  iter = SuperBigID.begin(); iter!= SuperBigID.end() ; iter++ )
	{
		int i = (*iter) ;

		for(int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL ++)
		{

			int colL = tmpnt.mods[i].GLeftX/this->binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/this->binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/this->binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/this->binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= this->nBinCol ) colR = this->nBinCol - 1;
			if( rowT >= this->nBinRow ) rowT = this->nBinRow - 1;


			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{
					pair <int,int >  temp ;
					temp.first = -1 ;
					temp.second = -1 ;
					bin3D[modL][j][k].FreeBin = temp;
					bin3D[modL][j][k].flagHasPre=true;
				}
			}
		}


	}


	// Set inital map to build the Escpaemap(Just Covered Bin has Value)

	//this->PlotBinMap (tmpnt, this->BinMapPath+"Fixed","Fixed");

	/// BUILD Free Bin Set
	vector <vector <pair <int,int > > >EscapeBin = IdentifyFreeBin(tmpnt, 10 ); 


	//this->PlotBinMap (tmpnt, this->BinMappath + "Cold","Cold");
	for(  iter = SuperBigID.begin(); iter!= SuperBigID.end() ; iter++ )
	{
		int i = (*iter) ;

		for (int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++)
		{

			int colL = tmpnt.mods[i].GLeftX/this->binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/this->binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/this->binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/this->binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= this->nBinCol ) colR = this->nBinCol - 1;
			if( rowT >= this->nBinRow ) rowT = this->nBinRow - 1;


			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{

					int DIST = this->nBinCol+ this->nBinRow;
					pair <int,int > Bin ;
					for(int z = 0 ; z <(int) EscapeBin[modL].size() ; z++)
					{
						int dist = abs(j-EscapeBin.at(modL).at(z).first ) + abs(k - EscapeBin.at(modL).at(z).second);
						if(dist < DIST)
						{
							Bin = EscapeBin[modL][z];
							DIST = dist;
						}
					}
					this->bin3D[modL][j][k].EscapeDist = DIST ;  ///< The distance to Free Bin
					this->bin3D[modL][j][k].FreeBin = Bin ;      ///< The Closest Free Bin
					this->bin3D[modL][j][k].flagHasPre = true ;

					if( DIST > this->Max_EscpaeDist)
						this->Max_EscpaeDist = DIST;  /// Find the MaximumDistance

				}
			}

		}


	}
/*
	// Set the value of the non-Covered Bin
	for(int z = 0 ; z < tmpnt.nLayer ; z++)
	{

		for (int i = 0 ; i< this->nBinRow ; i++)
		{
			for(int j = 0 ; j< this->nBinCol ; j++)
			{
				if(this->bin3D[z][i][j].flagHasPre == true)
					continue;
				else
				{
					// Initilize
					int DIST = 0;
					pair <int,int > Bin ;
					Bin.first = i ;
					Bin.second = j ;

					this->bin3D[z][i][j].EscapeDist = DIST ;
					this->bin3D[z][i][j].FreeBin = Bin ;

				}

			}
		}
	}
*/
	// This EscapeBin is initial status
	// Only use the condition of Fixed module.
	// which is used to push away the module overlaped with fixed Module 

	for (int z = 0 ;z <tmpnt.nLayer ; z++)
	{
		vector < vector <double> > Escapetemp;
			Escapetemp.resize(nBinRow);
		for(int i =0 ; i< this->nBinRow ; i++)
			Escapetemp[i].resize(this->nBinCol);
		for(int i = 0 ; i < this->nBinRow ; i++)
		{
			for(int j=0 ; j <this->nBinCol ; j++)
			{
				Escapetemp[i][j] = this->bin3D[z][i][j].EscapeDist;
			}
		}

		this->FirstEscapeBin[z] = Escapetemp;

	}

	//================Escape dist plot=================//

	//PLOT::PlotMap( this->FirstEscapeBin[0] , "./PotentialMap/Escape.m" , this->binW, this->binH);
	this->PlotBinMap (tmpnt, this->BinMapPath+ "FirstEscape","Escape");


}

vector<vector <pair <int,int > > >THERMALGP::IdentifyFreeBin(NETLIST & tmpnt, double AllowableT )
{

	// This Fcn. is used to Search Cold Bin
	// According to two condition:
	// - Covered by Fixed Module ?
	// - The Temperature is less than AllowableT ?
	// Then Output the vector with the Cold Bin.

	vector < vector <pair<int,int > > > OutputVec ;
	OutputVec.resize(tmpnt.nLayer);
	static vector<set <int> >  AlreadySet;

	static bool OneShotUnLock  = false;
	
	if(OneShotUnLock == false)
		AlreadySet.resize(tmpnt.nLayer);
	

	for(int z=  0 ; z < tmpnt.nLayer ; z++)
	{
		for(int i = 0 ; i< this->nBinRow ; i++)
		{
			for(int j= 0 ; j< this->nBinCol ; j++)
			{
				if(this->bin3D[z][i][j].flagHasPre) /// Condition 1
				{
					bin3D[z][i][j].FlagCold = false;
					continue;
				}

				double Temperature = this->bin3D[z][i][j].OccupyValue - AllowableT ;

				if(Temperature < 0  ) /// Condition 2 
				{
					int Label  = i * this->nBinRow + j ;
					set<int >::iterator it1 = AlreadySet[z].find(Label) ;
					this->bin3D[z][i][j].FlagCold = true;

					if(it1 == AlreadySet[z].end() )
					{
						pair <int,int> temp ;
						temp.first = i ;
						temp.second = j;

						OutputVec[z].push_back(temp);
						AlreadySet[z].insert(Label);
					}
				}
				else
				{
					this->bin3D[z][i][j].FlagCold = false;
				}
			}
		}
	}


	if(OneShotUnLock == false) /// Clean the First Espcape
	{
		for(int i = 0 ; i < tmpnt.nLayer ; i++)
			AlreadySet[i].clear();
		AlreadySet.clear();
		OneShotUnLock= true;

	}
	return OutputVec ;

}

void THERMALGP::CreateThermalTable( NETLIST& tmpnt )
{
	//cout << "******* Create Thermal Table ********" << endl;
	ThermalCol = 50 ;   ///user specify number
	ThermalCol = 40 ;
	ThermalRow = ThermalCol * tmpnt.aR ;
	binW_T = (int)((tmpnt.ChipWidth/((float)AMP_PARA)) / ((float)ThermalCol) * AMP_PARA);
	binH_T = (int)((tmpnt.ChipHeight/((float)AMP_PARA)) / ((float)ThermalRow) * AMP_PARA);
	// cout<<"ThermalCol: "<<ThermalCol<<" binW_T: "<<binW_T<<endl;
	// cout<<"ThermalRow: "<<ThermalRow<<" binH_T: "<<binH_T<<endl;
	/// create thermal mask after cutbin
	mask_table = new double*[2*nBinRow];
	for(int i=0; i<2*nBinRow; i++)
	{
		mask_table[i] = new double[2*nBinCol];
	}

	correction_table = new double*[nBinRow];
	for(int i=0; i<nBinRow; i++)
	{
		correction_table[i] = new double[nBinCol];
	}
	mask_table_2 = new double*[2*ThermalRow];
	for(int i=0; i<2*ThermalRow; i++)
	{
		mask_table_2[i] = new double[2*ThermalCol];
	}
	/////////////////////////
	double ar = tmpnt.aR ;
	ar = sqrt(ar);
	double t_ratio_x = (double) (40*ar)/nBinRow;
	double t_ratio_y = (double) (40/ar)/nBinCol;


	for(int i=0; i<2*nBinRow; i++)
	{
		for(int j=0; j<2*nBinCol; j++)
		{

			mask_table[i][j] = 0 ;
			if(i==0&&j==0) continue;

			double dt = (i*t_ratio_x)*(i*t_ratio_x) + (j*t_ratio_y)*(j*t_ratio_y) ;
			double s_num = sqrt( dt ) ;
			double tt = -100 * log ( s_num ) + 300 ;

			if( tt > 0 )
			{
				mask_table[i][j] = tt;
			}

		}
	}
	mask_table[0][0] = 400 ;///zero point

	////////////////////////////////
	double cx = nBinRow/2 ;
	double cy = nBinCol/2 ;
	double xtimes = (double) (100*ar)/nBinRow ;
	double ytimes = (double) (100/ar)/nBinCol ;
	for(int i=0; i<nBinRow; i++)
	{
		for(int j=0; j<nBinCol; j++)
		{
			double dd = (i-cx)*(i-cx)*xtimes*xtimes + (j-cy)*(j-cy)*ytimes*ytimes ;
			correction_table[i][j] = (100 - dd * 0.0003 + 0.0203) * 0.01 ;
		}
	}
	//cout << correction_table[0][0] <<endl;

	/////////////////////////////////
	double t_ratio = 0;
	if( tmpnt.aR >= 1 )
	{
		t_ratio = (double) (40)/ThermalCol;
	}

	else
	{
		t_ratio = (double) (40)/ThermalRow;
	}

	for(int i=0; i<2*ThermalRow; i++)
	{
		for(int j=0; j<2*ThermalCol; j++)
		{

			mask_table_2[i][j] = 0 ;
			if(i==0&&j==0) continue;

			double dt = (i*t_ratio)*(i*t_ratio) + (j*t_ratio)*(j*t_ratio) ;
			double s_num = sqrt( dt ) ;
			double tt = -100 * log ( s_num ) + 300 ;

			if( tt > 0 )
			{
				mask_table_2[i][j] = tt;
			}

		}
	}
	mask_table_2[0][0] = 400 ;///zero point

	////////////////////////////////
	/*
	   double **sbin;
	   sbin = new double*[nBinRow];

	   for(int i=0; i<nBinRow; i++)
	   {
	   sbin[i] = new double[nBinCol];
	   }

	   for(int i=0; i<nBinRow; i++)
	   {
	   for(int j=0; j<nBinCol; j++)
	   {
	   sbin[i][j] = 0;
	   }
	   }
	   xtimes = (double) 40/nBinRow ;
	   ytimes = (double) 40/nBinCol ;
	   for(int i=0; i<nBinRow; i++)
	   {
	   for(int j=0; j<nBinCol; j++)
	   {
	   if(i==cx &&j==cy) continue;
	   double dd = (i-cx)*(i-cx)*xtimes*xtimes + (j-cy)*(j-cy)*ytimes*ytimes ;
	   double s_num = sqrt( dd ) ;
	   double tt = -100 * log ( s_num ) + 300 ;
	   if( tt > 0 )
	   {
	   sbin[i][j] = tt;
	   }
	   }
	   }
	   int rr=nBinRow/2;
	   int cc=nBinCol/2;
	   sbin[rr][cc] = 400 ;

	   for(int i=0; i<nBinRow; i++)
	   {
	   delete [] sbin[i];
	   }

	   delete [] sbin;
	 */

}
void THERMALGP::ShrinkMod(NETLIST &tmpnt)
{
	double SumMod = 0;
	double SumLength = 0;
	double HotSumArea=0;
	double WarmArea = 0;
	for(int i=0; i<tmpnt.nMod; i++)
	{
		double area = (double) tmpnt.mods[i].modW / AMP_PARA;
		SumLength += area;
		SumMod += area * area;

		if(tmpnt.mods[i].TemperatureGroup == 0)
			HotSumArea+= area * area;
		else if (tmpnt.mods[i].TemperatureGroup == 1)
			WarmArea += area * area;

	}
	double AvgLength = SumLength / tmpnt.nMod;
	double ShrinkRatio;
	double ShrinkRatioWarm ;


	// Define Ratio
	ShrinkRatio = (double) (SumMod - HotSumArea ) / ( SumMod*1.5 ) ;
	cout<<" - ShrinkRatio : "<< ShrinkRatio <<endl;


	ShrinkRatioWarm = 1 ;


	// Shrink

	for(int i=0; i<tmpnt.nMod; i++)
	{
		tmpnt.mods[i].ShrinkW = tmpnt.mods[i].modW;
		tmpnt.mods[i].ShrinkH = tmpnt.mods[i].modH;


		if(tmpnt.mods[i].TemperatureGroup == 0) // Hot Group
		{
			tmpnt.mods[i].ShrinkW = tmpnt.mods[i].modW  * ShrinkRatio;
			tmpnt.mods[i].ShrinkH = tmpnt.mods[i].modH  * ShrinkRatio;
			tmpnt.mods[i].GLeftX = tmpnt.mods[i].GCenterX - tmpnt.mods[i].ShrinkW /2;
			tmpnt.mods[i].GLeftY = tmpnt.mods[i].GCenterY - tmpnt.mods[i].ShrinkH /2;

		}
		else if (tmpnt.mods[i].TemperatureGroup == 1)
		{
			tmpnt.mods[i].ShrinkW = tmpnt.mods[i].modW * ShrinkRatioWarm;
			tmpnt.mods[i].ShrinkH = tmpnt.mods[i].modH * ShrinkRatioWarm;
			tmpnt.mods[i].GLeftX = tmpnt.mods[i].GCenterX - tmpnt.mods[i].ShrinkW /2;
			tmpnt.mods[i].GLeftY = tmpnt.mods[i].GCenterY - tmpnt.mods[i].ShrinkH /2;


		}
	}

}
void THERMALGP::InflatMod(NETLIST &tmpnt)
{
	for(int i=0; i<tmpnt.nMod; i++)
	{

		tmpnt.mods[i].ShrinkW = tmpnt.mods[i].modW;
		tmpnt.mods[i].ShrinkH = tmpnt.mods[i].modH;
		tmpnt.mods[i].GLeftX = tmpnt.mods[i].GCenterX - tmpnt.mods[i].modW/2;
		tmpnt.mods[i].GLeftY = tmpnt.mods[i].GCenterY - tmpnt.mods[i].modH/2;

	}


}

void THERMALGP::CalOverflowArea( NETLIST& tmpnt )
{
	BIN3D::iterator bin3D_it;
	for( bin3D_it = bin3D.begin(); bin3D_it != bin3D.end(); bin3D_it++ )
	{
		for( int i = 0; i < nBinRow; i++ )
		{
			for( int j = 0; j < nBinCol; j++ )
			{
				bin3D_it->second[i][j].OccupyValue = 0;
				bin3D_it->second[i][j].EnableValue = 0;
			}
		}
	}

	/// calculate OccupyValue in each bin & Cv
	for( int i = 0; i < tmpnt.nMod; i++ )
	{


		///f///0925 stacked module has multiple layer
		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )
		{
			if( modL >= tmpnt.nLayer )
			{
				cerr << "error  : stack's layer out of range" << endl;
				exit(EXIT_FAILURE);
			}


			int colL = tmpnt.mods[i].GLeftX/binW - 2;	///< occupy bin column left
			int rowB = tmpnt.mods[i].GLeftY/binH - 2;	///< occupy bin row bottom
			int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/binW + 2;	///< occupy bin column right
			int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/binH + 2;	///< occupy bin row top

			if( colL < 0 ) colL = 0;
			if( rowB < 0 ) rowB = 0;
			if( colR >= nBinCol ) colR = nBinCol - 1;
			if( rowT >= nBinRow ) rowT = nBinRow - 1;

			double Db = 0;
			double *P_X = new double [colR - colL + 1];		///< potential x
			double *P_Y = new double [rowT - rowB + 1];		///< potential y

			double wb = (double)binW/(double)AMP_PARA;					///< width of bin
			double wv = (double)tmpnt.mods[i].modW/(double)AMP_PARA;	///< width of module
			double hb = (double)binH/(double)AMP_PARA;					///< height of bin
			double hv = (double)tmpnt.mods[i].modH/(double)AMP_PARA;	///< height of module

			double ax = 4/((wv+2*wb)*(wv+4*wb));
			double bx = 2/((wb)*(wv+4*wb));
			double ay = 4/((hv+2*hb)*(hv+4*hb));
			double by = 2/((hb)*(hv+4*hb));

			double range1_w = wv/2 + wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range2_w = wv/2 + 2*wb;	///< range (wv/2 + wb)~(wv/2 + 2wb)
			double range1_h = hv/2 + hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)
			double range2_h = hv/2 + 2*hb;	///< range (hv/2 + hb)~(hv/2 + 2hb)


			for( int j = rowB; j <= rowT; j++ )
			{
				double disY = (double)(bin3D[modL][j][colL].y + bin3D[modL][j][colL].h/2 - tmpnt.mods[i].GCenterY)/(double)AMP_PARA;
				if( disY < 0 )
				{
					disY = -disY;
				}
				if( disY <= range1_h && disY >= 0 )
				{
					P_Y[j-rowB] = 1 - ay * pow( disY, 2.0 );
				}
				else if( disY <= range2_h && disY > range1_h )
				{
					P_Y[j-rowB] = by * pow( (disY-range2_h), 2.0 );
				}
				else
				{
					P_Y[j-rowB] = 0;
				}
			}

			for( int k = colL; k <= colR; k++ )
			{
				double disX = (double)(bin3D[modL][rowB][k].x + bin3D[modL][rowB][k].w/2 - tmpnt.mods[i].GCenterX)/(double)AMP_PARA;
				if( disX < 0 )
				{
					disX = -disX;
				}

				if( disX <= range1_w && disX >= 0 )
				{
					P_X[k-colL] = 1 - ax * pow( disX, 2.0 );
				}
				else if( disX <= range2_w && disX > range1_w )
				{
					P_X[k-colL] = bx * pow( (disX-range2_w), 2.0 );
				}
				else
				{
					P_X[k-colL] = 0;
				}
			}

			for( int j = 0; j <= (rowT-rowB); j++ )
			{
				for( int k = 0; k <= (colR-colL); k++ )
				{
					Db += (P_X[k] * P_Y[j]);
				}
			}

			this->Cv[i] = tmpnt.mods[i].modArea/Db;

			for( int j = rowB; j <= rowT; j++ )
			{
				for( int k = colL; k <= colR; k++ )
				{
					bin3D[modL][j][k].OccupyValue += this->Cv[i] * P_X[k-colL] * P_Y[j-rowB];
				}
			}

			delete [] P_X;
			delete [] P_Y;


		}///f///0925


	}

	double WbHb = ((float)binW/(float)AMP_PARA) * ((float)binH/(float)AMP_PARA);
	overflow_area_1 = 0;
	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		for( int j = 0; j < nBinRow; j++ )
		{
			for( int k = 0; k < nBinCol; k++ )
			{
				/*
				if( this-> TFM )
				{

					if(this->Change == true)
						this->bin3D[i][j][k].EnableValue = WbHb - 0.3 * this->FirstEscapeBin[j][k] * WbHb;
					else
						bin3D[i][j][k].EnableValue = WbHb - 0.03 * this->FirstEscapeBin[j][k] * WbHb;
				}
				else
				{

						bin3D[i][j][k].EnableValue = WbHb ;
				}
*/
				bin3D[i][j][k].EnableValue = WbHb ;
				overflow_area_1 += COM_MAX( bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue, 0 );
			}
		}
	}
}
void THERMALGP::BuildGaussianMask()
{
	cout <<" - Build GaussianMask "<<endl;

	this->GaussianMask.resize(this->nBinRow);
	for(int i=0 ; i < this->nBinRow ; i++)
	{
		this->GaussianMask[i].resize(this->nBinCol);
	}
	int CenterX =(int) (this->nBinRow -1 ) /2 ;
	int CenterY =(int) (this->nBinCol -1 ) /2 ;
	double MaskSum = 0;
	for(int i=0 ; i< this->nBinRow ; i++)
	{
		for(int j=0; j <this->nBinCol ; j++)
		{
			double Dist = (i - CenterX ) * ( i - CenterX ) + (j - CenterY) * (j-CenterY);

			double Sigma = this->nBinRow * 0.02 ;
			double SigmaQu = pow(Sigma,2.0);
			double Upper = (double) -1 *  (Dist) / (2* SigmaQu);
			double Value = (double) exp(Upper) /(2* 3.14 *SigmaQu );

			this->GaussianMask[i][j] = Value;
			MaskSum += Value;

		}

	}
	//Normalize to make summation equal to 1

	for(int i=0 ; i< this->nBinRow ; i++)
	{
		for(int j=0; j <this->nBinCol ; j++)
		{
			this->GaussianMask[i][j] = (double) this->GaussianMask[i][j] / MaskSum ;
		}

	}


	PLOT::PlotMap(this->GaussianMask, this->BinMapPath + "GaussianMask.m",this->binW, this->binH);






}
void THERMALGP::BuildThermalMask()
{
	// make mask
	cout <<" - Build Thermal Mask : image size 3 * nBinRow, 3 * nBinCol "<<endl;
	double RowSize = 3* nBinRow ;
	double ColSize = 3* nBinCol ;

	this->ThermalMask.resize(RowSize);
	for(int i =0 ; i <RowSize ; i++)
		this->ThermalMask[i].resize(ColSize);

	double xtimes = (double) 40/nBinRow ;
	double ytimes = (double) 40/nBinCol ;
	double cx = (double) (RowSize-1) /2 ;
	double cy = (double) (ColSize-1) /2 ;
	//cout<<"Row: "<<nBinRow<<" cx: "<<cx<<endl;

	for(int j=0; j< RowSize ; j++)
	{
		for(int k=0; k< ColSize ; k++)
		{

			double dd = (j-cx)*(j-cx)*xtimes*xtimes + (k-cy)*(k-cy)*ytimes*ytimes ;
			double s_num = sqrt( dd ) * 4.0 ; // Corblivar - parameter
			// double s_num = sqrt( dd ) * 1.0; // ITRI - parameter
			double tt = -100 * log ( s_num ) + 300 ;
			/*
			   if(j >= cx-2 && j <=cx+2 && k >= cy-2 && k <= cy +2   )
			   cout << j <<"  "<<k <<" Value : "<< tt <<endl;*/


			if( tt > 0 )
			{
				this->ThermalMask[j][k] = tt;
			}

			if(j==cx &&k == cy)
			{
				this->ThermalMask[j][k] = 400;
				//  cout  <<"Cx : "<<cx<<" Cy "<< cy <<endl;
				//    getchar();
				//cout<<"center hit"<<endl;
			}
		}
	}

	// Normalize Mask to let the summation equal to 1

	double MaskTSum = 0;
	for(int i =0 ; i< nBinRow *3 ; i++)
	{
		for(int j = 0 ; j< nBinCol * 3; j++)
		{
			MaskTSum += this->ThermalMask[i][j];
		}
	}

	for(int i = 0 ; i< nBinRow *3 ; i++)
	{
		for(int j = 0 ; j< nBinCol * 3; j++)
		{
			this->ThermalMask[i][j] = (double) this->ThermalMask[i][j] / MaskTSum;

		}
	}

	PLOT::PlotMap(this->ThermalMask, this->BinMapPath +"ThermalMask.m", this->binW, this->binH);





}

void THERMALGP::PlotBinMap(NETLIST & tmpnt, string Filename, string Type)
{
	
	char *file = new char[Filename.length() + 1];

	strcpy(file, Filename.c_str());


    char** fileName;
    fileName = new char* [tmpnt.nLayer];
    for( int i = 0; i < tmpnt.nLayer; i++ )
    {
        fileName[i] = new char [50];

        char subName[10] = "\0";
        sprintf( subName, "_%d", i );

        strcpy( fileName[i], file );
        strcat( fileName[i], subName );
        strcat( fileName[i], ".m" );
    }
	ofstream *fout = new ofstream [tmpnt.nLayer];



	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		fout[i].open( fileName[i] );


		if( !fout[i].is_open() )
		{
			cout << "error  : unable to open " << fileName[i] << "for plotting the result of global distribution" << endl;
			exit(EXIT_FAILURE);
		}

		fout[i] << "axis equal;" << endl;
		fout[i] << "x = linspace( " << this->bin3D[i] [0][0].x << ", " << this->binW*this->nBinCol << ", " << this->nBinCol << " );" << endl;
		fout[i] << "y = linspace( " << this->bin3D[i] [0][0].y << ", " << this->binH*this->nBinRow << ", " << this->nBinRow << " );" << endl;
		fout[i] << "[xx,yy] = meshgrid(x, y); " << endl;
		fout[i] << "zz = zeros (" << this->nBinRow << ", " << this->nBinCol << ", 'double');" << endl << endl;



		for( int j = 0; j < this->nBinRow; j++ )
		{
			for( int k = 0; k < this->nBinCol; k++ )
			{

				double ratio = 0;
				if(Type == "Escape")
					ratio= this->bin3D[i][j][k].EscapeDist; 
				else if( Type== "Occupy")
					ratio = this->bin3D[i][j][k].OccupyValue;
				else if (Type == "Density")
					ratio = this->bin3D[i][j][k].OccupyValue - this->bin3D[i][j][k].EnableValue;
				else if(Type=="Total")
					ratio = this->bin3D[i][j][k].OccupyValue + this->bin3D[i][j][k].EnableValue;
				else if(Type == "Fixed")
				{
					if( this->bin3D[i][j][k].flagHasPre== true )
						ratio = 1;
					else
						ratio = 0;
				}
				else if (Type== "Cold")
				{
					if(this->bin3D[i][j][k].FlagCold == false)
						ratio = 1;
					else 
						ratio = 0;
				}
				else 
				{
					cerr << " The Type Option doesn't exist"<<endl;
				}
				fout[i] << "zz (" << j+1 << ", " << k+1 << ") = " << ratio  << ";" << endl << endl;
			}
		}

		fout[i]<<"surf(xx, yy, zz);"<<endl;
	}


	delete [] fout;




	///f///0924/*
	char* fileName_all;

	fileName_all = new char [50];

	char subName_all[10] = "\0";
	sprintf( subName_all, "all" );


	strcpy( fileName_all, file );
	strcat( fileName_all, subName_all );
	strcat( fileName_all, ".m" );


	ofstream gout;
	gout.open( fileName_all );

	ifstream gin;
	string strtemp;

	gout << "SS = get(0,'ScreenSize');" <<endl;
	gout << "figure('Position',[ SS(1)+SS(3)/5 SS(2)+SS(4)/5 SS(3)*3/5 SS(4)*3/5 ]);" <<endl;
	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		gin.open( fileName[i] );
		gout << "subplot (1,"<< tmpnt.nLayer <<"," << i+1 << ");" << endl;
		getline( gin, strtemp );
		getline( gin, strtemp );
		getline( gin, strtemp );
		while( getline( gin, strtemp ) )
		{
			gout << strtemp << endl;
		}
		gin.close();

	}
	gout.close();
	///f///0924*/




	for( int i = 0; i < tmpnt.nLayer; i++ )
	{
		delete [] fileName[i];
	}
	delete [] fileName;
	delete [] file;
}


void THERMALGP::PlotThermalMap( vector <vector <double> >& NewMb, NETLIST &tmpnt)
{

	cout<<"- Plot Thermal Maps "<<endl;


	// Escape Map
	string EscapeLabel = UTILITY::Int_2_String(CountFactor);
	string EscapeFile =this->BinMapPath +"Escape" + EscapeLabel ;

	this->PlotBinMap(tmpnt,EscapeFile,"Escape");

	// Thermal Term Map

	string label = UTILITY::Int_2_String( CountFactor );
	string OutFile = this->BinMapPath +"fftw"+label;

	this->PlotBinMap(tmpnt,OutFile,"Total");
	// Temperature MAP
	OutFile = this->BinMapPath +"Occupy"+label;
	this->PlotBinMap(tmpnt,OutFile,"Occupy");
	
	// Gradient Map (need Matlab)


/*
	string CgLabel = UTILITY::Int_2_String( CountFactor );
	string CgFile = this->BinMapPath +"TargetT" + CgLabel + ".txt";
	ofstream fout_Cg(CgFile.c_str());


	for(int i = 0 ; i< this->nBinRow ; i++)
	{
		for(int j =0; j < this->nBinCol ; j++)
		{
			double ratio = NewMb[i][j];

			//   cout<< ratio << " ";
			fout_Cg  <<ratio   << " " ;
		}
		fout_Cg<<endl;
		//  cout<<endl;
	}

	fout_Cg.close();*/
}



void FFTW::Convolution(vector <vector <double> > & Map, vector <vector <double> > & Mask, vector <vector <double> > &Results)
{
	//cout<<"- Convolution..."<<endl;
	//===== Extend Map =====//

	int RowSize = (int) Map.size();
	int ColSize = (int) Map[RowSize-1].size();

	int ImageRow = 1 * RowSize;
	int ImageCol = 1 * ColSize;

	vector <vector <double> > ExtendMap ;
	vector <vector <double> > ExtendMask ;

	ExtendMap.resize(ImageRow);
	ExtendMask.resize(ImageCol);

	// Initialize Map & Mask
	for(int i =0 ; i < ImageRow ; i++)
	{
		ExtendMap[i].resize(ImageCol);
		ExtendMask[i].resize(ImageCol);

	}
	for(int i =0 ; i <ImageRow; i++)
	{
		for(int j=0 ; j< ImageCol ; j++)
		{
			ExtendMap[i][j] = 0;
			ExtendMask[i][j] = 0;
		}

	}
	// Give the Value of gird which are located at middle

	int Middle_RowBegin = 0 ;
	// int Middle_RowBegin = RowSize ;
	int Middle_ColBegin = 0 ;
	// int Middle_ColBegin = ColSize ;
	int Middle_RowEnd = 1 * RowSize ;
	int Middle_ColEnd = 1 * ColSize ;
	for(int i= Middle_RowBegin ; i< Middle_RowEnd ; i++)
	{
		for(int j=Middle_ColBegin; j< Middle_ColEnd ; j++)
		{
			ExtendMap[i][j] = Map[ i-Middle_RowBegin ][j-Middle_ColBegin];
			ExtendMask[i][j] =   Mask[ i-Middle_RowBegin ][j-Middle_ColBegin];

		}
	}


	int w_src = ImageRow;
	int h_src = ImageCol;
	// Shift Matrix
	vector <vector <double> > MapTemp  = ExtendMap ;

	for (int j=0 ; j<  w_src; j++)
	{
		for (int k= 0; k<  h_src; k++ )
		{
			int sum_length=h_src;
			int shift= (h_src-1)/2;
			int ww=(j+shift)%(sum_length);
			int hh=(k+shift)%(sum_length);
			ExtendMap[j][k]=  MapTemp[ww][hh];

		}
	}


	Results.resize(RowSize);
	for(int i =0 ; i< RowSize ; i++)
	{
		Results[i].resize(ColSize);
	}


	// FFTW Packge


	double * src = new double[h_src*w_src];
	double * kernel = new double[h_src*w_src];
	for(int j=0; j< w_src; j++)
	{
		for(int k=0; k< h_src; k++)
		{
			src[j*w_src+k]=ExtendMap[j][k];
		}

	}
	for(int j=0; j< w_src; j++)
	{
		for(int k=0; k<h_src; k++)
		{
			kernel[j*w_src+k]=ExtendMask[j][k];
		}
	}

	//  printf("Image size : %i x %i \n", h_src, w_src);
	double * dst_fft_fftw3 = new double[h_src*2*w_src];

	double * dst_fft_fftw4 = new double[h_src*2*w_src];
	// real->complex
	FFTW::compute_fft_fftw3(src,h_src, w_src, dst_fft_fftw3);
	FFTW::compute_fft_fftw3(kernel,h_src, w_src, dst_fft_fftw4);
	double * output=new double [h_src*2*w_src];
	//Yn=Fn*Sn
	FFTW::product(dst_fft_fftw3,dst_fft_fftw4,h_src,w_src,output);
	double * result = new double[h_src*2*w_src];
	//complex->real
	FFTW::compute_ifft_fftw3(output,h_src,w_src,result);

	// output->result
	for(int i= Middle_RowBegin; i<Middle_RowEnd; i++)
	{
		for(int j= Middle_ColBegin; j<Middle_ColEnd; j++)
		{
			double ratio= FFTW::compute(result[i*2*w_src + 2*j],result[i*2*w_src + 2*j + 1]);
			double data=(double) ratio / (h_src*w_src);

			Results[i-Middle_RowBegin][j-Middle_ColBegin] = data;
		}
	}
/*
	// Map Plot
	PLOT::PlotMap(ExtendMask,this->BinMapPath +"mask.m",1000,1000);
	PLOT:: PlotMap(ExtendMap,this->BinMapPath +"Map.m",1000,1000);
	PLOT::PlotMap(Results,this->BinMapPath +"Result.m",1000,1000);
	// getchar();
*/	 


	delete[] src;
	delete[] kernel;

	delete[] dst_fft_fftw3;
	delete[] dst_fft_fftw4;
	delete[] output;
	delete[] result;




}

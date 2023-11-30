#include "WirelengthGP.h"

void WLGP::GlobalStage(NETLIST & tmpnt)
{

    ///f///
	this->CalculateHPWL_GP(tmpnt);
    cout<<BOLD(Cyan("\\ ====== Wirelength Mode ====== //"))<< endl;


    start_t = clock();


    this->Xp = new double [tmpnt.nMod + tmpnt.nPad];	///< postive x-term in LSE wirelength model
    this->Xn = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive x-term in LSE wirelength model
    this->Yp = new double [tmpnt.nMod + tmpnt.nPad];	///< postive y-term in LSE wirelength model
    this->Yn = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive y-term in LSE wirelength model

    this->GX_0 = new double [tmpnt.nMod];		///< x-axis gradient directions g_k-1
    this->GX_1 = new double [tmpnt.nMod];		///< x-axis gradient directions g_k
    this->GY_0 = new double [tmpnt.nMod];		///< y-axis gradient directions g_k-1
    this->GY_1 = new double [tmpnt.nMod];		///< y-axis gradient directions g_k
    this->WG_X = new double [tmpnt.nMod];		///< x-axis wirelength gradient
    this->WG_Y = new double [tmpnt.nMod];		///< y-axis wirelength gradient
    this->DG_X = new double [tmpnt.nMod];		///< x-axis density gradient
    this->DG_Y = new double [tmpnt.nMod];		///< y-axis density gradient
    this->DX_1 = new double [tmpnt.nMod];		///< x-axis conjuagte directions d_k
    this->DX_0 = new double [tmpnt.nMod];		///< x-axis conjugate directions d_k-1
    this->DY_1 = new double [tmpnt.nMod];		///< y-axis conjugate directions d_k
    this->DY_0 = new double [tmpnt.nMod];		///< y-axis conjugate directions d_k-1
    this->Cv = new double [tmpnt.nMod];		///< normalization factor
    this->alpha = new double [tmpnt.nMod];	///< step size



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


    if(CurrentLevel == LevelCount-1)
        this->SetModToCenter( tmpnt );





	this->CutBin( tmpnt );


    /// solve quadratic programming
    if(this->CurrentLevel == this->TotalLevel -1 )
	{
		this->SolveQP(tmpnt);
    }


    this->numIter = 0;
    this->numCG   = 0;
    int it_time= this->TotalLevel -1 - this->CurrentLevel;
    if( this->CurrentLevel == LevelCount-1 && this->FlagPlot == true)
    {
		string PlotFile = this->FpPlotPath + "OP";
		PLOT Ploter;
		Ploter.PlotResultGL_dy( tmpnt, PlotFile, 0 );
    }

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
    int bad_solve = 0;

    double OverlapL = 2 ;
    //cout<<"hi";
    cout<<Green(" ----- Overflow Info ------")<<endl;
    cout<<Green("      iter      ovf        ")<<endl;
    cout<<Green(" --------------------------")<<endl;


    /// solve min W(x,y) + lambda * Sum((Db-Mb)*(Db-Mb))
    for( int count = 0; count < 200; count++ )
    {
        double WLterm = 0;
        double BDterm = 0;

        if( count > 0 )
        {
            overflow_area_0 = overflow_area_1;
        }

        numCG++;
        cout<<"here1";
		this->ConjugateGradient( tmpnt );
        cout<<"here2";
		this->CalOverflowArea( tmpnt );
        cout<<"here3";
        if( count > 0 )
        {

            cout<< setw(10) << count;
            cout<< setw(10) <<100 * (float)overflow_area_1/(float)tmpnt.totalModArea<<" %";
            cout<<endl;
            if( overflow_area_1-overflow_area_0 >= 0  )
            {
                bad_solve++;
                if(bad_solve >=4 ) // Can tolerate two times bad sol
                {
                    break;
                }
            }
            else
                bad_solve = 0;
        }


        if( count > 0 && (float)overflow_area_1/(float)tmpnt.totalModArea <= InputOption.overflow_bound && overflow_area_1 - overflow_area_0 >= 0 )
        {
            break;
        }
        //=========plot==========//

		if(this->FlagPlot == true)
		{
			std::string label= UTILITY::Int_2_String( it_time );
			string path= this->FpPlotPath;
			std::string inner_label = UTILITY::Int_2_String( count );
			string dynamic_out = path+"ThermalGradient"+"_"+label+"_"+inner_label;

			PLOT Ploter;
			Ploter.PlotResultGL_dy( tmpnt, dynamic_out, 0 );
		}

        //=========plot==========//
        //=========dynamic penalty=======//
        double dk = 0;
        double wk = 0;
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            wk += sqrt( this->WG_X[i]*this->WG_X[i] + this->WG_Y[i]*this->WG_Y[i] );
            dk += sqrt( this->DG_X[i]*this->DG_X[i] + this->DG_Y[i]*this->DG_Y[i] );
        }

        double slope = (double) (wk/ ( dk * lambda   ) );

        if(slope >= 1.5)
        {
            OverlapL = 1.5;

        }
        else
        {
            OverlapL = 2;
        }


        //=========dynamic penalty=======//


        WLterm = this->CalWireGradient( tmpnt );
        BDterm = this->CalDensityGradient( tmpnt );

        lambda *= OverlapL;

        Fk_1 = lambdaW * WLterm + lambda * BDterm;  ///<++

        for( int i = 0; i < tmpnt.nMod; i++ )
        {

            this->GX_1[i] = this->WG_X[i] + lambda * this->DG_X[i];
            this->GY_1[i] = this->WG_Y[i] + lambda * this->DG_Y[i];

            this->GX_0[i] = this->GX_1[i];
            this->GY_0[i] = this->GY_1[i];
            this->DX_0[i] = 0;
            this->DY_0[i] = 0;
        }
    }
    
	this->CalculateHPWL_GP( tmpnt );

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

    end_t = clock();

    double gp_t = (end_t - start_t)/(double)CLOCKS_PER_SEC;


    ///f///0623 avoid memory error
   	this->bin3D.clear();


    cout << "*************** Done ****************" << endl;
    cout << " - CPU Time           : " << gp_t << endl;
    cout << " - HPWL               : " << tmpnt.totalWL_GP << endl;
    cout << " - Overflow Ratio     : " << overflow_area_1/tmpnt.totalModArea << endl;
    cout << " - Number of CG       : " << numCG << endl;
    cout << " - Number of Iteration: " << numIter << endl;
    //cout<<"test\n";
    cout << "*************************************" << endl;
	
    
   //cout<<"test\n";
            cout << "** Dump Global Distribution Result **" << endl;
           // cout<<"test\n";
            DumpBlocks( tmpnt, "output", "_global" );
            DumpPl( tmpnt, "output", "_global" );
            DumpNets( tmpnt, "output", "_global" );
            DumpLayer( tmpnt, "output", "_global" );
            cout << "*************************************" << endl;
                // if( outputFile != NULL)
        //{
       // }
  
}



void WLGP::CutBin( NETLIST& tmpnt )
{
    cout << " ----- Cut Bin Info -----" << endl;

    double tsvArea = pow( TSV_PITCH, 2.0 );
    double AvgModLength = sqrt((double)(tmpnt.totalModArea - tmpnt.nTSV * tsvArea )/(double)(tmpnt.nMod - tmpnt.nTSV));
    int numBin = (int)((double)tmpnt.ChipHeight/(double)AMP_PARA/AvgModLength);

	WLGP::BIN2D tempBin;

    this->nBinRow = this->nBinCol = numBin * InputOption.bin_para;

    tempBin.resize( this->nBinRow );
    for( int i = 0; i < nBinRow; i++ )
    {
        tempBin[i].resize( this->nBinCol );
    }

    this->binW = (int)((tmpnt.ChipWidth/((float)AMP_PARA)) / ((float)this->nBinCol) * AMP_PARA);
    this->binH = (int)((tmpnt.ChipHeight/((float)AMP_PARA)) / ((float)this->nBinRow) * AMP_PARA);

    cout << " - nBinRow: " << this->nBinRow << endl;
    cout << " - nBinCol: " << this->nBinCol << endl;
    cout << " - binW   : " << this->binW << endl;
    cout << " - binH   : " << this->binH << endl;

    for( int i = 0; i < this->nBinRow; i++ )
    {
        for( int j = 0; j < this->nBinCol; j++ )
        {
            tempBin[i][j].w = this->binW;
            tempBin[i][j].h = this->binH;
            tempBin[i][j].x = j * this->binW;
            tempBin[i][j].y = i * this->binH;
            tempBin[i][j].EnableValue = ((float)this->binW/(float)AMP_PARA) * ((float)this->binH/(float)AMP_PARA);
        }
    }

    for( int i = 0; i < tmpnt.nLayer; i++ )
    {
        this->bin3D[i] = tempBin;
    }
    tempBin.clear();
    BIN2D (tempBin).swap(tempBin);
    cout<<" ------------------------"<<endl;
}


double WLGP::CalWireGradient( NETLIST& tmpnt )
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

double WLGP::CalDensityGradient( NETLIST& tmpnt )
{
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        this->DG_X[i] = 0;
        this->DG_Y[i] = 0;
    }

    BIN3D::iterator bin3D_it;
    for( bin3D_it = bin3D.begin(); bin3D_it != bin3D.end(); bin3D_it++ )
    {
        for( int i = 0; i < nBinRow; i++ )
        {
            for( int j = 0; j < nBinCol; j++ )
            {
                bin3D_it->second[i][j].OccupyValue = 0;
            }
        }
    }

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
            int colR = (tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW)/binW + 2;	///< occupy bin column right
            int rowT = (tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH)/binH + 2;	///< occupy bin row top

            if( colL < 0 ) colL = 0;
            if( rowB < 0 ) rowB = 0;
            if( colR >= nBinCol ) colR = nBinCol - 1;
            if( rowT >= nBinRow ) rowT = nBinRow - 1;

            double *P_X = new double [colR - colL + 1];		///< potential x
            double *P_Y = new double [rowT - rowB + 1];		///< potential y
            double *dP_X = new double [colR - colL + 1];	///< differential potential x
            double *dP_Y = new double [rowT - rowB + 1];	///< differential potential y

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
                double disX = (double)(bin3D[modL][rowB][k].x + bin3D[modL][rowB][k].w/2 - tmpnt.mods[i].GCenterX)/(double)AMP_PARA;
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
                    this->DG_X[i] += dP_X[k-colL] * P_Y[j-rowB] * this->Cv[i] * (bin3D[modL][j][k].OccupyValue - bin3D[modL][j][k].EnableValue) * 2;
                    this->DG_Y[i] += P_X[k-colL] * dP_Y[j-rowB] * this->Cv[i] * (bin3D[modL][j][k].OccupyValue - bin3D[modL][j][k].EnableValue) * 2;
                }
            }

            delete [] P_X;
            delete [] P_Y;
            delete [] dP_X;
            delete [] dP_Y;

        }///f///0925

    }

    //overflow_area_1 = 0;

    for( int i = 0; i < tmpnt.nLayer; i++ )
    {
        for( int j = 0; j < nBinRow; j++ )
        {
            for( int k = 0; k < nBinCol; k++ )
            {
                //overflow_area_1 += MAX( bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue, 0 );
                totalBinDensity += pow( (bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue), 2.0 );
            }
        }
    }

    return totalBinDensity;
}


void WLGP::InitializeGradient( NETLIST& tmpnt)
{
    double WLterm = this->CalWireGradient( tmpnt );		///< wirelength term in the objective function
    double BDterm = this->CalDensityGradient( tmpnt );	///< density term in the objective function

    double dk = 0;
    double wk = 0;

    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        dk += sqrt( this->DG_X[i]*this->DG_X[i] + this->DG_Y[i]*this->DG_Y[i] );
        wk += sqrt( this->WG_X[i]*this->WG_X[i] + this->WG_Y[i]*this->WG_Y[i] );
    }


    this->lambdaW = 1;

    int LevelCount =(int) this->TotalLevel;

    if(this->CurrentLevel == LevelCount-1)
    {
        lambda = InputOption.InitialGradient * (wk/dk);
    }
    else
    {
        lambda = InputOption.DeclusterGradient * (wk/dk);
    }

    Fk_1 = WLterm + lambda * BDterm;

    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        this->GX_1[i] = this->WG_X[i] + lambda * this->DG_X[i];
        this->GY_1[i] = this->WG_Y[i] + lambda * this->DG_Y[i];

        this->GX_0[i] = this->GX_1[i];
        this->GY_0[i] = this->GY_1[i];
        this->DX_0[i] = 0;
        this->DY_0[i] = 0;
    }

}

void WLGP::ConjugateGradient( NETLIST& tmpnt )
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
            originX[i] = tmpnt.mods[i].GCenterX;
            originY[i] = tmpnt.mods[i].GCenterY;

            tmpnt.mods[i].GCenterX += (int)(this->alpha[i] * this->DX_1[i] * AMP_PARA);
            tmpnt.mods[i].GLeftX += (int)(this->alpha[i] * this->DX_1[i] * AMP_PARA);

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
        }

        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            this->GX_0[i] = this->GX_1[i];
            this->GY_0[i] = this->GY_1[i];
            this->DX_0[i] = this->DX_1[i];
            this->DY_0[i] = this->DY_1[i];
        }

        Fk_0 = Fk_1;

        double WLterm = CalWireGradient( tmpnt );
        double BDterm = CalDensityGradient( tmpnt );

        Fk_1 = WLterm + lambda * BDterm;

        if( Fk_1 < Fk_0 || count <= 5000 )
        {
            for( int i = 0; i < tmpnt.nMod; i++ )
            {


                    this->GX_1[i] = this->WG_X[i] + lambda * this->DG_X[i];
                    this->GY_1[i] = this->WG_Y[i] + lambda * this->DG_Y[i];



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

void WLGP::CalOverflowArea( NETLIST& tmpnt )
{
    BIN3D::iterator bin3D_it;
    for( bin3D_it = bin3D.begin(); bin3D_it != bin3D.end(); bin3D_it++ )
    {
        for( int i = 0; i < nBinRow; i++ )
        {
            for( int j = 0; j < nBinCol; j++ )
            {
                bin3D_it->second[i][j].OccupyValue = 0;
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

    overflow_area_1 = 0;
    for( int i = 0; i < tmpnt.nLayer; i++ )
    {
        for( int j = 0; j < nBinRow; j++ )
        {
            for( int k = 0; k < nBinCol; k++ )
            {
                overflow_area_1 += COM_MAX( bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue, 0 );
            }
        }
    }
}

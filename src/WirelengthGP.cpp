#include "WirelengthGP.h"
#include "tool.h"
void WLGP::GlobalStage_last_stage(NETLIST & tmpnt)
{
    //cout<<tmpnt.ChipWidth<<" "<<tmpnt.ChipHeight<<"\n";
    ///f///
    bool halt=false;
    clock_t start_tt = clock();
	this->CalculateHPWL_GP(tmpnt);
    
    cout<<BOLD(Cyan("\\\\ ====== Wirelength Mode ====== //"))<< endl;


    
    
    
    
    int layer_loop=2;
    this->Xp = new double* [layer_loop];	///< postive x-term in LSE wirelength model
    this->Xn = new double* [layer_loop];	///< negtive x-term in LSE wirelength model
    this->Yp = new double* [layer_loop];	///< postive y-term in LSE wirelength model
    this->Yn = new double* [layer_loop];	///< negtive y-term in LSE wirelength model

    this->GX_0 = new double* [layer_loop];		///< x-axis gradient directions g_k-1
    this->GX_1 = new double* [layer_loop];		///< x-axis gradient directions g_k
    this->GY_0 = new double* [layer_loop];		///< y-axis gradient directions g_k-1
    this->GY_1 = new double* [layer_loop];		///< y-axis gradient directions g_k
    this->WG_X = new double* [layer_loop];		///< x-axis wirelength gradient
    this->WG_Y = new double* [layer_loop];		///< y-axis wirelength gradient
    this->DG_X = new double* [layer_loop];		///< x-axis density gradient
    this->DG_Y = new double* [layer_loop];		///< y-axis density gradient
    this->DX_1 = new double* [layer_loop];		///< x-axis conjuagte directions d_k
    this->DX_0 = new double* [layer_loop];		///< x-axis conjugate directions d_k-1
    this->DY_1 = new double* [layer_loop];		///< y-axis conjugate directions d_k
    this->DY_0 = new double* [layer_loop];		///< y-axis conjugate directions d_k-1
    this->pure_DX=new double*[layer_loop];       ///< x-axis pure Db gradient
    this->pure_DY=new double*[layer_loop];       ///< y-axis pure Db gradient
    this->alpha_x = new double* [layer_loop];	///< step size
    this->alpha_y = new double* [layer_loop];	///< step size

    this->Cv = new double [tmpnt.nMod];		///< normalization factor
    
    for(int i=0;i<layer_loop;i++){
        this->Xp[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< postive x-term in LSE wirelength model
        this->Xn[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive x-term in LSE wirelength model
        this->Yp[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< postive y-term in LSE wirelength model
        this->Yn[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive y-term in LSE wirelength model

        this->GX_0[i] = new double [tmpnt.nMod];		///< x-axis gradient directions g_k-1
        this->GX_1[i] = new double [tmpnt.nMod];		///< x-axis gradient directions g_k
        this->GY_0[i] = new double [tmpnt.nMod];		///< y-axis gradient directions g_k-1
        this->GY_1[i] = new double [tmpnt.nMod];		///< y-axis gradient directions g_k
        this->WG_X[i] = new double [tmpnt.nMod];		///< x-axis wirelength gradient
        this->WG_Y[i] = new double [tmpnt.nMod];		///< y-axis wirelength gradient
        this->DG_X[i] = new double [tmpnt.nMod];		///< x-axis density gradient
        this->DG_Y[i] = new double [tmpnt.nMod];		///< y-axis density gradient
        this->DX_1[i] = new double [tmpnt.nMod];		///< x-axis conjuagte directions d_k
        this->DX_0[i] = new double [tmpnt.nMod];		///< x-axis conjugate directions d_k-1
        this->DY_1[i]= new double [tmpnt.nMod];		///< y-axis conjugate directions d_k
        this->DY_0[i] = new double [tmpnt.nMod];		///< y-axis conjugate directions d_k-1
        this->pure_DX[i]=new double[tmpnt.nMod];       ///< x-axis pure Db gradient
        this->pure_DY[i]=new double[tmpnt.nMod];       ///< y-axis pure Db gradient
        //this->Cv[i] = new double [tmpnt.nMod];		///< normalization factor
        this->alpha_x[i] = new double [tmpnt.nMod];	///< step size
        this->alpha_y[i] = new double [tmpnt.nMod];	///< step size
    }
    
    


    int LevelCount = this->TotalLevel;
    /// set gamma (log-sum-exp wirelength model)
    if( tmpnt.ChipWidth >= tmpnt.ChipHeight )
    {
        this->Gamma = (float)tmpnt.ChipWidth/AMP_PARA/100;
    }
    else
    {
        this->Gamma = (float)tmpnt.ChipHeight/AMP_PARA/100;
    }

    /// set modules & TSVs to the center of chip


    if(CurrentLevel == LevelCount-1){
        if(InputOption.test_mode!=22 &&InputOption.test_mode!=50){
        this->SetModToCenter( tmpnt );
        }
        else{
        this->RandomPosition( tmpnt );
        //this->SetModToCenter( tmpnt );
        }



    }
	this->CutBin( tmpnt );


    /// solve quadratic programming
    /*if(this->CurrentLevel == this->TotalLevel -1 )
	{
		this->SolveQP(tmpnt);
    }*/


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
    /*for( int i = tmpnt.nMod; i < tmpnt.nMod + tmpnt.nPad; i++ )
    {
        double tempX = tmpnt.pads[i-tmpnt.nMod].x/(double)AMP_PARA;
        double tempY = tmpnt.pads[i-tmpnt.nMod].y/(double)AMP_PARA;
        this->Xp[i] = exp( (tempX/Gamma)  );
        this->Xn[i] = exp( (-tempX/Gamma) );
        this->Yp[i] = exp( (tempY/Gamma)  );
        this->Yn[i] = exp( (-tempY/Gamma) );

    }*/

	this->CalOverflowArea( tmpnt );


	cout << " - Initial Area overflow : \n";
	cout <<"Top: "<<100 * (float)overflow_area_top_1/(float)tmpnt.topModArea<<" %" <<endl;
    cout <<"Bot: "<<100 * (float)overflow_area_bot_1/(float)tmpnt.botModArea<<" %" <<endl;



    cout << " - Solve Ojective Function by Conjugate Gradient Method" << endl;

    /// initialize gradient
    bool start_monitor_nan=false;
	this->InitializeGradient( tmpnt,start_monitor_nan);
    int bad_solve = 0;

    double OverlapL_top = 2 ;
    double OverlapL_bot = 2 ;
    cout<<Green(" ----------- Overflow Info ------------")<<endl;
    cout<<Green("      iter     ovf(top)     ovf(bot)")<<endl;
    cout<<Green(" --------------------------------------")<<endl;


    /// solve min W(x,y) + lambda * Sum((Db-Mb)*(Db-Mb))
    //for( int count = 0; count < 200; count++ )
    //5/28改成恆等式
    int count=0;
    bool start_bad_sol = false;
    double origin_lambda_top=lambda_top;
    double origin_lambda_bot=lambda_bot;
    while(true)
    {
        double* WLterm = new double[2];
        double* BDterm = new double[2];
        
        if( count > 0 )
        {
            overflow_area_top_0 = overflow_area_top_1;
            overflow_area_bot_0 = overflow_area_bot_1;
        }

        numCG++;
        //jack 7/19
        this->ConjugateGradient_last_stage( tmpnt ,start_bad_sol,start_monitor_nan,count+1,(double)start_tt,halt);
        //cout<<"here1\n";
		this->CalOverflowArea( tmpnt );
        //cout<<"here2\n";
        //cout<<"\n";
        if(halt==true){
            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
        }
        if( count > 0 )
        {
            if (start_bad_sol == true) {
                cout << setw(10) << count;
                cout << setw(10) << 100 * (float)overflow_area_top_1 / (float)tmpnt.topModArea << " %";
                cout << setw(10) << 100 * (float)overflow_area_bot_1 / (float)tmpnt.botModArea << " %";
                /*if(100 * (float)overflow_area_1 / (float)tmpnt.totalModArea<92 &&100 * (float)overflow_area_1 / (float)tmpnt.totalModArea>90){
                    start_monitor_nan=true;
                }
                else{
                    start_monitor_nan=false;
                }*/
                cout << endl;

                //8/11改終止條件，原本是>=-1持續15次，改成一變差就跳出
                if(InputOption.QP_key==false){
                    if ((100*((float)overflow_area_top_1 / (float)tmpnt.topModArea) - 100*((float)overflow_area_top_0 / (float)tmpnt.topModArea) >= 0)||(100*((float)overflow_area_bot_1 / (float)tmpnt.botModArea) - 100*((float)overflow_area_bot_0 / (float)tmpnt.botModArea) >= 0))
                    {
                        bad_solve++;
                        //cout << bad_solve << "\n";
                        if (bad_solve >= 3 ) // Can tolerate two times bad sol   **可以自己調
                        {
                            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
                        }
                        
                    }
                    else if(lambda_top>10000*origin_lambda_top &&lambda_bot>10000*origin_lambda_bot){
                            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
                    }
                    else
                        bad_solve = 0;
                }
                else{
                    if ((100*((float)overflow_area_top_1 / (float)tmpnt.topModArea) - 100*((float)overflow_area_top_0 / (float)tmpnt.topModArea) >= -1)||(100*((float)overflow_area_bot_1 / (float)tmpnt.botModArea) - 100*((float)overflow_area_bot_0 / (float)tmpnt.botModArea) >= -1))
                    {
                        bad_solve++;
                        //cout << bad_solve << "\n";
                        if (bad_solve >= 15) // Can tolerate two times bad sol   **可以自己調
                        {
                            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
                        }
                    }
                    else
                        bad_solve = 0;
                }

            }
            else{
                if(count>1000 || lambda_top>1000000000000|| lambda_bot>1000000000000){
                    for(int i=0;i<tmpnt.GetnMod();i++){
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
                                tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                            }
                            if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                            {
                                tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                            }
                    }
                    break;
                }
            }
        }


        /*if( count >  0 && (float)overflow_area_1/(float)tmpnt.totalModArea <= InputOption.overflow_bound && overflow_area_1 - overflow_area_0 >= 0 )
        {
            break;
        }*/
        //=========plot==========//

		/*if(this->FlagPlot == true)
		{
			std::string label= UTILITY::Int_2_String( it_time );
			string path= this->FpPlotPath;
			std::string inner_label = UTILITY::Int_2_String( count );
			string dynamic_out = path+"ThermalGradient"+"_"+label+"_"+inner_label;

			PLOT Ploter;
			Ploter.PlotResultGL_dy( tmpnt, dynamic_out, 0 );
		}*/

        //=========plot==========//
        //=========dynamic penalty=======//
        double dk_top= 0;
        double wk_top= 0;
        double dk_bot= 0;
        double wk_bot= 0;
        //calpuredensgradient(tmpnt);
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
           if(tmpnt.mods[i].modL==0){
                dk_top += sqrt( this->DG_X[0][i]*this->DG_X[0][i] + this->DG_Y[0][i]*this->DG_Y[0][i] );
                //dk_top += sqrt( this->pure_DX[0][i]*this->pure_DX[0][i] + this->pure_DY[0][i]*this->pure_DY[0][i] );
                wk_top += sqrt( this->WG_X[0][i]*this->WG_X[0][i] + this->WG_Y[0][i]*this->WG_Y[0][i] );
            }
            else{
                dk_bot += sqrt( this->DG_X[1][i]*this->DG_X[1][i] + this->DG_Y[1][i]*this->DG_Y[1][i] );
                //dk_bot += sqrt( this->pure_DX[1][i]*this->pure_DX[1][i] + this->pure_DY[1][i]*this->pure_DY[1][i] );
                wk_bot += sqrt( this->WG_X[1][i]*this->WG_X[1][i] + this->WG_Y[1][i]*this->WG_Y[1][i] );
            }
        }

        double slope_top = (double) (wk_top/ ( dk_top * lambda_top   ) );
        double slope_bot = (double) (wk_bot/ ( dk_bot * lambda_bot   ) );
        if(slope_top >= 1.5)
        {
            if(InputOption.test_mode==5){
                OverlapL_top = 4.5;
            }
            else{
                OverlapL_top = 1.5;
            }
            //OverlapL = 15000;

        }
        else
        {
            if(InputOption.test_mode==5){
                OverlapL_top = 6;
            }
            else{
                OverlapL_top = 2;
            }
            //OverlapL = 20000;
        }
        if(slope_bot >= 1.5)
        {
            if(InputOption.test_mode==5){
                OverlapL_bot = 4.5;
            }
            else{
                OverlapL_bot = 1.5;
            }
            //OverlapL = 15000;

        }
        else
        {
            if(InputOption.test_mode==5){
                OverlapL_bot = 6;
            }
            else{
                OverlapL_bot = 2;
            }
            //OverlapL = 20000;
        }

        //=========dynamic penalty=======//


        WLterm = this->CalWireGradient( tmpnt );
        /*cout<<" WL(top) in global stage: "<<WLterm[0]<<"\n";
        cout<<" WL(bot) in global stage: "<<WLterm[1]<<"\n";*/
        BDterm = this->CalDensityGradient( tmpnt,start_monitor_nan );
        /*cout<<" BD(top) in global stage: "<<BDterm[0]<<"\n";
        cout<<" BD(bot) in global stage: "<<BDterm[1]<<"\n";*/
        lambda_top *= OverlapL_top;
        lambda_bot *= OverlapL_bot;

        Fk_1_top = /*lambdaW * */WLterm[0] + lambda_top * BDterm[0];  ///<++
        Fk_1_bot = /*lambdaW * */WLterm[1] + lambda_bot * BDterm[1];  ///<++

        for( int i = 0; i < tmpnt.nMod; i++ )
        {

            if(tmpnt.mods[i].modL==0){
                //cout<<this->GX_1[i]<<" "<<this->WG_X[i]<<" "<<this->DG_X[i]<<"\n";
                this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];

                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = 0;
                this->DY_0[0][i] = 0;
                if(tmpnt.mods[i].flagTSV==true){
                    this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                    this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                    this->GX_0[1][i] = this->GX_1[1][i];
                    this->GY_0[1][i] = this->GY_1[1][i];
                    this->DX_0[1][i] = 0;
                    this->DY_0[1][i] = 0;
                }
            }
            else{
                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = 0;
                this->DY_0[1][i] = 0;
            }
        }

        //5/28新增
        count++;
    }
    
	this->CalculateHPWL_GP( tmpnt );

    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        tmpnt.mods[i].CenterX = tmpnt.mods[i].GCenterX;
        tmpnt.mods[i].CenterY = tmpnt.mods[i].GCenterY;
        tmpnt.mods[i].LeftX = tmpnt.mods[i].GLeftX;
        tmpnt.mods[i].LeftY = tmpnt.mods[i].GLeftY;
    }
    for(int i=0;i<layer_loop;i++){
        delete [] this->Xp[i];
        delete [] this->Xn[i];
        delete [] this->Yp[i];
        delete [] this->Yn[i];

        delete [] this->GX_0[i];
        delete [] this->GX_1[i];
        delete [] this->GY_0[i];
        delete [] this->GY_1[i];
        delete [] this->WG_X[i];
        delete [] this->WG_Y[i];
        delete [] this->DG_X[i];
        delete [] this->DG_Y[i];
        delete [] this->DX_1[i];
        delete [] this->DX_0[i];
        delete [] this->DY_1[i];
        delete [] this->DY_0[i];
        
        delete [] this->alpha_x[i];
        delete [] this->alpha_y[i];
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
    
    delete [] this->alpha_x;
    delete [] this->alpha_y;
    delete [] this->Cv;
    end_t = clock();

    double gp_t = (end_t - start_t)/(double)CLOCKS_PER_SEC;


    ///f///0623 avoid memory error
   	this->bin3D.clear();
    this->potential3D.clear();
    this->smooth_potential3D.clear();
    this->level_smoothing_potential3D.clear();

    cout << "*************** Done ****************" << endl;
    cout << " - CPU Time           : " << gp_t << endl;
    cout << " - HPWL               : " << tmpnt.totalWL_GP << endl;
    cout << " - Overflow Ratio(top)     : " << overflow_area_top_1/tmpnt.topModArea << endl;
    cout << " - Overflow Ratio(bot)     : " << overflow_area_bot_1/tmpnt.botModArea << endl;
    cout << " - Number of CG       : " << numCG << endl;
    cout << " - Number of Iteration: " << numIter << endl;
    cout << "*************************************" << endl;
    /*for(int i=0;i < tmpnt.nMod; i++){
        if(tmpnt.mods[i].GCenterX==0&&tmpnt.mods[i].GCenterY==0&&tmpnt.mods[i].GLeftX==0&&tmpnt.mods[i].GLeftY==0){
            cout<<"\n"<<i<<"\n\n   all=0\n\n\n";
        }
    }*/

    
        /*if( outputFile != NULL)
        {
            cout << "** Dump Global Distribution Result **" << endl;
            DumpBlocks( tmpnt, "output", "_global" );
            DumpPl( tmpnt, "output", "_global" );
            DumpNets( tmpnt, "output", "_global" );
            DumpLayer( tmpnt, "output", "_global" );
            cout << "*************************************" << endl;
        }*/
 
}
void WLGP::GlobalStage(NETLIST & tmpnt)
{
    //cout<<tmpnt.ChipWidth<<" "<<tmpnt.ChipHeight<<"\n";
    ///f///
	this->CalculateHPWL_GP(tmpnt);
    
    cout<<BOLD(Cyan("\\\\ ====== Wirelength Mode ====== //"))<< endl;


    start_t = clock();
    
    
    
    int layer_loop=2;
    this->Xp = new double* [layer_loop];	///< postive x-term in LSE wirelength model
    this->Xn = new double* [layer_loop];	///< negtive x-term in LSE wirelength model
    this->Yp = new double* [layer_loop];	///< postive y-term in LSE wirelength model
    this->Yn = new double* [layer_loop];	///< negtive y-term in LSE wirelength model

    this->GX_0 = new double* [layer_loop];		///< x-axis gradient directions g_k-1
    this->GX_1 = new double* [layer_loop];		///< x-axis gradient directions g_k
    this->GY_0 = new double* [layer_loop];		///< y-axis gradient directions g_k-1
    this->GY_1 = new double* [layer_loop];		///< y-axis gradient directions g_k
    this->WG_X = new double* [layer_loop];		///< x-axis wirelength gradient
    this->WG_Y = new double* [layer_loop];		///< y-axis wirelength gradient
    this->DG_X = new double* [layer_loop];		///< x-axis density gradient
    this->DG_Y = new double* [layer_loop];		///< y-axis density gradient
    this->DX_1 = new double* [layer_loop];		///< x-axis conjuagte directions d_k
    this->DX_0 = new double* [layer_loop];		///< x-axis conjugate directions d_k-1
    this->DY_1 = new double* [layer_loop];		///< y-axis conjugate directions d_k
    this->DY_0 = new double* [layer_loop];		///< y-axis conjugate directions d_k-1
    this->pure_DX=new double*[layer_loop];       ///< x-axis pure Db gradient
    this->pure_DY=new double*[layer_loop];       ///< y-axis pure Db gradient
    this->alpha_x = new double* [layer_loop];	///< step size
    this->alpha_y = new double* [layer_loop];	///< step size

    this->Cv = new double [tmpnt.nMod];		///< normalization factor
    
    for(int i=0;i<layer_loop;i++){
        this->Xp[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< postive x-term in LSE wirelength model
        this->Xn[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive x-term in LSE wirelength model
        this->Yp[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< postive y-term in LSE wirelength model
        this->Yn[i] = new double [tmpnt.nMod + tmpnt.nPad];	///< negtive y-term in LSE wirelength model

        this->GX_0[i] = new double [tmpnt.nMod];		///< x-axis gradient directions g_k-1
        this->GX_1[i] = new double [tmpnt.nMod];		///< x-axis gradient directions g_k
        this->GY_0[i] = new double [tmpnt.nMod];		///< y-axis gradient directions g_k-1
        this->GY_1[i] = new double [tmpnt.nMod];		///< y-axis gradient directions g_k
        this->WG_X[i] = new double [tmpnt.nMod];		///< x-axis wirelength gradient
        this->WG_Y[i] = new double [tmpnt.nMod];		///< y-axis wirelength gradient
        this->DG_X[i] = new double [tmpnt.nMod];		///< x-axis density gradient
        this->DG_Y[i] = new double [tmpnt.nMod];		///< y-axis density gradient
        this->DX_1[i] = new double [tmpnt.nMod];		///< x-axis conjuagte directions d_k
        this->DX_0[i] = new double [tmpnt.nMod];		///< x-axis conjugate directions d_k-1
        this->DY_1[i]= new double [tmpnt.nMod];		///< y-axis conjugate directions d_k
        this->DY_0[i] = new double [tmpnt.nMod];		///< y-axis conjugate directions d_k-1
        this->pure_DX[i]=new double[tmpnt.nMod];       ///< x-axis pure Db gradient
        this->pure_DY[i]=new double[tmpnt.nMod];       ///< y-axis pure Db gradient
        //this->Cv[i] = new double [tmpnt.nMod];		///< normalization factor
        this->alpha_x[i] = new double [tmpnt.nMod];	///< step size
        this->alpha_y[i] = new double [tmpnt.nMod];	///< step size
    }
    
    


    int LevelCount = this->TotalLevel;
    /// set gamma (log-sum-exp wirelength model)
    if( tmpnt.ChipWidth >= tmpnt.ChipHeight )
    {
        this->Gamma = (float)tmpnt.ChipWidth/AMP_PARA/100;
    }
    else
    {
        this->Gamma = (float)tmpnt.ChipHeight/AMP_PARA/100;
    }

    /// set modules & TSVs to the center of chip


    if(CurrentLevel == LevelCount-1){
        if(InputOption.test_mode!=22 &&InputOption.test_mode!=50){
        this->SetModToCenter( tmpnt );
        }
        else{
        this->RandomPosition( tmpnt );
        //this->SetModToCenter( tmpnt );
        }



    }
	this->CutBin( tmpnt );


    /// solve quadratic programming
    /*if(this->CurrentLevel == this->TotalLevel -1 )
	{
		this->SolveQP(tmpnt);
    }*/


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
    /*for( int i = tmpnt.nMod; i < tmpnt.nMod + tmpnt.nPad; i++ )
    {
        double tempX = tmpnt.pads[i-tmpnt.nMod].x/(double)AMP_PARA;
        double tempY = tmpnt.pads[i-tmpnt.nMod].y/(double)AMP_PARA;
        this->Xp[i] = exp( (tempX/Gamma)  );
        this->Xn[i] = exp( (-tempX/Gamma) );
        this->Yp[i] = exp( (tempY/Gamma)  );
        this->Yn[i] = exp( (-tempY/Gamma) );

    }*/

	this->CalOverflowArea( tmpnt );


	cout << " - Initial Area overflow : \n";
	cout <<"Top: "<<100 * (float)overflow_area_top_1/(float)tmpnt.topModArea<<" %" <<endl;
    cout <<"Bot: "<<100 * (float)overflow_area_bot_1/(float)tmpnt.botModArea<<" %" <<endl;



    cout << " - Solve Ojective Function by Conjugate Gradient Method" << endl;

    /// initialize gradient
    bool start_monitor_nan=false;
	this->InitializeGradient( tmpnt,start_monitor_nan);
    int bad_solve = 0;

    double OverlapL_top = 2 ;
    double OverlapL_bot = 2 ;
    cout<<Green(" ----------- Overflow Info ------------")<<endl;
    cout<<Green("      iter     ovf(top)     ovf(bot)")<<endl;
    cout<<Green(" --------------------------------------")<<endl;


    /// solve min W(x,y) + lambda * Sum((Db-Mb)*(Db-Mb))
    //for( int count = 0; count < 200; count++ )
    //5/28改成恆等式
    int count=0;
    bool start_bad_sol = false;
    double origin_lambda_top=lambda_top;
    double origin_lambda_bot=lambda_bot;
    while(true)
    {
        double* WLterm = new double[2];
        double* BDterm = new double[2];
        
        if( count > 0 )
        {
            overflow_area_top_0 = overflow_area_top_1;
            overflow_area_bot_0 = overflow_area_bot_1;
        }

        numCG++;
        //jack 7/19
        this->ConjugateGradient( tmpnt ,start_bad_sol,start_monitor_nan,count+1);
        //cout<<"here1\n";
		this->CalOverflowArea( tmpnt );
        //cout<<"here2\n";
        //cout<<"\n";
        if( count > 0 )
        {
            if (start_bad_sol == true) {
                cout << setw(10) << count;
                cout << setw(10) << 100 * (float)overflow_area_top_1 / (float)tmpnt.topModArea << " %";
                cout << setw(10) << 100 * (float)overflow_area_bot_1 / (float)tmpnt.botModArea << " %";
                /*if(100 * (float)overflow_area_1 / (float)tmpnt.totalModArea<92 &&100 * (float)overflow_area_1 / (float)tmpnt.totalModArea>90){
                    start_monitor_nan=true;
                }
                else{
                    start_monitor_nan=false;
                }*/
                cout << endl;

                //8/11改終止條件，原本是>=-1持續15次，改成一變差就跳出
                if(InputOption.QP_key==false){
                    if ((100*((float)overflow_area_top_1 / (float)tmpnt.topModArea) - 100*((float)overflow_area_top_0 / (float)tmpnt.topModArea) >= 0)||(100*((float)overflow_area_bot_1 / (float)tmpnt.botModArea) - 100*((float)overflow_area_bot_0 / (float)tmpnt.botModArea) >= 0))
                    {
                        bad_solve++;
                        //cout << bad_solve << "\n";
                        if (bad_solve >= 3 ) // Can tolerate two times bad sol   **可以自己調
                        {
                            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
                        }
                        
                    }
                    else if(lambda_top>10000*origin_lambda_top &&lambda_bot>10000*origin_lambda_bot){
                            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
                    }
                    else
                        bad_solve = 0;
                }
                else{
                    if ((100*((float)overflow_area_top_1 / (float)tmpnt.topModArea) - 100*((float)overflow_area_top_0 / (float)tmpnt.topModArea) >= -1)||(100*((float)overflow_area_bot_1 / (float)tmpnt.botModArea) - 100*((float)overflow_area_bot_0 / (float)tmpnt.botModArea) >= -1))
                    {
                        bad_solve++;
                        //cout << bad_solve << "\n";
                        if (bad_solve >= 15) // Can tolerate two times bad sol   **可以自己調
                        {
                            for(int i=0;i<tmpnt.GetnMod();i++){
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
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                                if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                                {
                                    tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                    tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                                }
                            }
                            break;
                        }
                    }
                    else
                        bad_solve = 0;
                }

            }
            else{
                if(count>1000 || lambda_top>1000000000000|| lambda_bot>1000000000000){
                    for(int i=0;i<tmpnt.GetnMod();i++){
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
                                tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                            }
                            if( tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH > tmpnt.ChipHeight )
                            {
                                tmpnt.mods[i].GLeftY = tmpnt.ChipHeight - tmpnt.mods[i].modH;
                                tmpnt.mods[i].GCenterY = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH/2;
                            }
                    }
                    break;
                }
            }
        }


        /*if( count >  0 && (float)overflow_area_1/(float)tmpnt.totalModArea <= InputOption.overflow_bound && overflow_area_1 - overflow_area_0 >= 0 )
        {
            break;
        }*/
        //=========plot==========//

		/*if(this->FlagPlot == true)
		{
			std::string label= UTILITY::Int_2_String( it_time );
			string path= this->FpPlotPath;
			std::string inner_label = UTILITY::Int_2_String( count );
			string dynamic_out = path+"ThermalGradient"+"_"+label+"_"+inner_label;

			PLOT Ploter;
			Ploter.PlotResultGL_dy( tmpnt, dynamic_out, 0 );
		}*/

        //=========plot==========//
        //=========dynamic penalty=======//
        double dk_top= 0;
        double wk_top= 0;
        double dk_bot= 0;
        double wk_bot= 0;
        //calpuredensgradient(tmpnt);
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
           if(tmpnt.mods[i].modL==0){
                dk_top += sqrt( this->DG_X[0][i]*this->DG_X[0][i] + this->DG_Y[0][i]*this->DG_Y[0][i] );
                //dk_top += sqrt( this->pure_DX[0][i]*this->pure_DX[0][i] + this->pure_DY[0][i]*this->pure_DY[0][i] );
                wk_top += sqrt( this->WG_X[0][i]*this->WG_X[0][i] + this->WG_Y[0][i]*this->WG_Y[0][i] );
            }
            else{
                dk_bot += sqrt( this->DG_X[1][i]*this->DG_X[1][i] + this->DG_Y[1][i]*this->DG_Y[1][i] );
                //dk_bot += sqrt( this->pure_DX[1][i]*this->pure_DX[1][i] + this->pure_DY[1][i]*this->pure_DY[1][i] );
                wk_bot += sqrt( this->WG_X[1][i]*this->WG_X[1][i] + this->WG_Y[1][i]*this->WG_Y[1][i] );
            }
        }

        double slope_top = (double) (wk_top/ ( dk_top * lambda_top   ) );
        double slope_bot = (double) (wk_bot/ ( dk_bot * lambda_bot   ) );
        if(slope_top >= 1.5)
        {
            if(InputOption.test_mode==5){
                OverlapL_top = 4.5;
            }
            else{
                OverlapL_top = 1.5;
            }
            //OverlapL = 15000;

        }
        else
        {
            if(InputOption.test_mode==5){
                OverlapL_top = 6;
            }
            else{
                OverlapL_top = 2;
            }
            //OverlapL = 20000;
        }
        if(slope_bot >= 1.5)
        {
            if(InputOption.test_mode==5){
                OverlapL_bot = 4.5;
            }
            else{
                OverlapL_bot = 1.5;
            }
            //OverlapL = 15000;

        }
        else
        {
            if(InputOption.test_mode==5){
                OverlapL_bot = 6;
            }
            else{
                OverlapL_bot = 2;
            }
            //OverlapL = 20000;
        }

        //=========dynamic penalty=======//


        WLterm = this->CalWireGradient( tmpnt );
        /*cout<<" WL(top) in global stage: "<<WLterm[0]<<"\n";
        cout<<" WL(bot) in global stage: "<<WLterm[1]<<"\n";*/
        BDterm = this->CalDensityGradient( tmpnt,start_monitor_nan );
        /*cout<<" BD(top) in global stage: "<<BDterm[0]<<"\n";
        cout<<" BD(bot) in global stage: "<<BDterm[1]<<"\n";*/
        lambda_top *= OverlapL_top;
        lambda_bot *= OverlapL_bot;

        Fk_1_top = /*lambdaW * */WLterm[0] + lambda_top * BDterm[0];  ///<++
        Fk_1_bot = /*lambdaW * */WLterm[1] + lambda_bot * BDterm[1];  ///<++

        for( int i = 0; i < tmpnt.nMod; i++ )
        {

            if(tmpnt.mods[i].modL==0){
                //cout<<this->GX_1[i]<<" "<<this->WG_X[i]<<" "<<this->DG_X[i]<<"\n";
                this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];

                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = 0;
                this->DY_0[0][i] = 0;
                if(tmpnt.mods[i].flagTSV==true){
                    this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                    this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                    this->GX_0[1][i] = this->GX_1[1][i];
                    this->GY_0[1][i] = this->GY_1[1][i];
                    this->DX_0[1][i] = 0;
                    this->DY_0[1][i] = 0;
                }
            }
            else{
                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = 0;
                this->DY_0[1][i] = 0;
            }
        }

        //5/28新增
        count++;
    }
    
	this->CalculateHPWL_GP( tmpnt );

    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        tmpnt.mods[i].CenterX = tmpnt.mods[i].GCenterX;
        tmpnt.mods[i].CenterY = tmpnt.mods[i].GCenterY;
        tmpnt.mods[i].LeftX = tmpnt.mods[i].GLeftX;
        tmpnt.mods[i].LeftY = tmpnt.mods[i].GLeftY;
    }
    for(int i=0;i<layer_loop;i++){
        delete [] this->Xp[i];
        delete [] this->Xn[i];
        delete [] this->Yp[i];
        delete [] this->Yn[i];

        delete [] this->GX_0[i];
        delete [] this->GX_1[i];
        delete [] this->GY_0[i];
        delete [] this->GY_1[i];
        delete [] this->WG_X[i];
        delete [] this->WG_Y[i];
        delete [] this->DG_X[i];
        delete [] this->DG_Y[i];
        delete [] this->DX_1[i];
        delete [] this->DX_0[i];
        delete [] this->DY_1[i];
        delete [] this->DY_0[i];
        
        delete [] this->alpha_x[i];
        delete [] this->alpha_y[i];
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
    
    delete [] this->alpha_x;
    delete [] this->alpha_y;
    delete [] this->Cv;
    end_t = clock();

    double gp_t = (end_t - start_t)/(double)CLOCKS_PER_SEC;


    ///f///0623 avoid memory error
   	this->bin3D.clear();
    this->potential3D.clear();
    this->smooth_potential3D.clear();
    this->level_smoothing_potential3D.clear();

    cout << "*************** Done ****************" << endl;
    cout << " - CPU Time           : " << gp_t << endl;
    cout << " - HPWL               : " << tmpnt.totalWL_GP << endl;
    cout << " - Overflow Ratio(top)     : " << overflow_area_top_1/tmpnt.topModArea << endl;
    cout << " - Overflow Ratio(bot)     : " << overflow_area_bot_1/tmpnt.botModArea << endl;
    cout << " - Number of CG       : " << numCG << endl;
    cout << " - Number of Iteration: " << numIter << endl;
    cout << "*************************************" << endl;
    /*for(int i=0;i < tmpnt.nMod; i++){
        if(tmpnt.mods[i].GCenterX==0&&tmpnt.mods[i].GCenterY==0&&tmpnt.mods[i].GLeftX==0&&tmpnt.mods[i].GLeftY==0){
            cout<<"\n"<<i<<"\n\n   all=0\n\n\n";
        }
    }*/

    
        /*if( outputFile != NULL)
        {
            cout << "** Dump Global Distribution Result **" << endl;
            DumpBlocks( tmpnt, "output", "_global" );
            DumpPl( tmpnt, "output", "_global" );
            DumpNets( tmpnt, "output", "_global" );
            DumpLayer( tmpnt, "output", "_global" );
            cout << "*************************************" << endl;
        }*/
 
}



void WLGP::CutBin( NETLIST& tmpnt )
{
    cout << " ----- Cut Bin Info -----" << endl;

    double tsvArea = pow( TSV_PITCH * ENLARGE_RATIO, 2.0 );
    double AvgModLength = sqrt((double)(tmpnt.totalModArea - tmpnt.nTSV * tsvArea )/(double)(tmpnt.nMod - tmpnt.nTSV));
    // int numBin = (int)((double)tmpnt.ChipHeight/(double)AMP_PARA/AvgModLength);
    int numBin =(int)sqrt(tmpnt.nMod- tmpnt.nTSV);

	WLGP::BIN2D tempBin;
    WLGP::BIN2D GuassianBin;
    this->nBinRow = this->nBinCol = numBin * InputOption.bin_para;
    //cout<<"\n\n nbin"<<this->nBinRow<<"\n";
    double subtract=(double)tmpnt.ChipHeight;
    int binn=1;
    /*while(true){
        double divide;
        double temp=subtract;
        divide=(double)tmpnt.ChipHeight/(double)binn;
        subtract=abs(divide-this->nBinRow);
        if(subtract<=temp){
            binn++;
        }
        else{
            binn--;
            divide=(double)tmpnt.ChipHeight/(double)binn;
            this->binH=binn;
            this->nBinRow=(int)divide;
            break;
        }
    }
    subtract=(double)tmpnt.ChipWidth;
    binn=1;
    while(true){
        double divide;
        double temp=subtract;
        divide=(double)tmpnt.ChipWidth/(double)binn;
        subtract=abs(divide-this->nBinCol);
        if(subtract<=temp){
            binn++;
        }
        else{
            binn--;
            divide=(double)tmpnt.ChipWidth/(double)binn;
            this->binW=binn;
            this->nBinCol=(int)divide;
            break;
        }
    }*/
    //cout << InputOption.bin_para << endl;
    //cout << tmpnt.nTSV << endl;

    tempBin.resize( this->nBinRow );

    for( int i = 0; i < nBinRow; i++ )
    {
        tempBin[i].resize( this->nBinCol );
    }
    GuassianBin.resize( this->nBinRow );

    for( int i = 0; i < nBinRow; i++ )
    {
        GuassianBin[i].resize( this->nBinCol );
    }

    this->binW = (double)((tmpnt.ChipWidth/((float)AMP_PARA)) / ((float)this->nBinCol) * AMP_PARA);
    this->binH = (double)((tmpnt.ChipHeight/((float)AMP_PARA)) / ((float)this->nBinRow) * AMP_PARA);

    cout << " - nBinRow: " << this->nBinRow << endl;
    cout << " - nBinCol: " << this->nBinCol << endl;
    cout << " - binW   : " << this->binW << endl;
    cout << " - binH   : " << this->binH << endl;
    if(InputOption.QP_key==true){
    //if(true){
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
    }
    else{
        double standard_deviation=((tmpnt.ChipWidth)+(tmpnt.ChipHeight))/8;
        double pi=3.141592653589793;
        for(int i=0;i<this->nBinRow;i++){
            for(int j=0;j<this->nBinCol;j++){
                GuassianBin[i][j].w = this->binW;
                GuassianBin[i][j].h = this->binH;
                GuassianBin[i][j].x = j*this->binW;
                GuassianBin[i][j].y = i*this->binH;
                GuassianBin[i][j].EnableValue=exp(-((GuassianBin[i][j].x*GuassianBin[i][j].x)+(GuassianBin[i][j].y*GuassianBin[i][j].y))/(2*standard_deviation*standard_deviation));//Note this is the guassain function not Mb
                GuassianBin[i][j].EnableValue/=(2*pi*(standard_deviation*standard_deviation));
            }
        }
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
            this->potential3D[i] = tempBin;
            this->smooth_potential3D[i]=tempBin;
            this->level_smoothing_potential3D[i]=tempBin;
        }
        
        for(int i=0;i < tmpnt.nLayer; i++){
            for(int j=0; j < this->nBinRow; j++){
                for( int k = 0; k < this->nBinCol; k++ ){
                    potential3D[i][j][k].EnableValue=0;
                    smooth_potential3D[i][j][k].EnableValue=0;
                    level_smoothing_potential3D[i][j][k].EnableValue=0;
                }
            }
        }
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].is_m==true){
            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
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
                if( colR >= nBinCol ) colR = nBinCol - 1;
                if( rowT >= nBinRow ) rowT = nBinRow - 1;

                double Db = 0;
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
                        //cout<<"\n\n\nelse is occuring0\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring1\n\n\n";
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
                if(Db!=0){
                    this->Cv[i] = tmpnt.mods[i].modArea/Db;
                }
                else{
                    cout<<"Db=0!\n";
                    this->Cv[i] = tmpnt.mods[i].modArea/1;
                }
                
                //cout<<"check potential:\n";
                for( int j = rowB; j <= rowT; j++ )
                {
                    
                    for( int k = colL; k <= colR; k++ )
                    {
                        bin3D[modL][j][k].EnableValue -= this->Cv[i] * P_X[k-colL] * P_Y[j-rowB];
                        
                        //jack //8/11調回來只需把potential3D寫回 bin3D 後面註解即可
                        //cout<<this->Cv[i] * P_X[k-colL] * P_Y[j-rowB]/((double)this->binW/(double)AMP_PARA) / ((double)this->binH/(double)AMP_PARA)<<" ";
                        /*potential3D[modL][j][k].EnableValue += this->Cv[i] * P_X[k-colL] * P_Y[j-rowB]/((double)this->binW/(double)AMP_PARA) / ((double)this->binH/(double)AMP_PARA);
                        smooth_potential3D[modL][j][k].EnableValue += this->Cv[i] * P_X[k-colL] * P_Y[j-rowB]/((double)this->binW/(double)AMP_PARA) / ((double)this->binH/(double)AMP_PARA);*/
                    }
                }

                delete [] P_X;
                delete [] P_Y;

            
            }
        }
        //Guassian smmoth
        /*cout<<"Guassian smooth.....\n";
        const int dx=nBinCol/2;
        const int dy=nBinRow/2;
        for(int i=0;i < tmpnt.nLayer; i++){
            
            for(int j=0; j < this->nBinRow; j++){
                for( int k = 0; k < this->nBinCol; k++ ){ 
                    double value=0;
                    for(int m=0;m<this->nBinRow;m++){
                        for(int l=0;l<this->nBinCol;l++){
                            int x=k-dx+l;
                            int y=j-dy+m;
                            if(x >= 0 && x < this->nBinCol && y >= 0 && y < this->nBinRow){
                                value+=potential3D[i][y][x].EnableValue *GuassianBin[m][l].EnableValue;
                            }
                            
                        }
                    }
                    //bin3D[i][j][k]-=()
                    smooth_potential3D[i][j][k].EnableValue=value;
                }
            }
        }*/

        /*for(int i=0;i < tmpnt.nLayer; i++){
            cout<<"\n\n\n"<<i<<"\n\n\n";
            for(int j=0; j < this->nBinRow; j++){
                    for( int k = 0; k < this->nBinCol; k++ ){
                        cout<<level_smoothing_potential3D[i][j][k].EnableValue<<" ";
                    }
                    cout<<"\n";
            }
        }*/

        //normalize
        /*cout<<"\nNormalize.....\n";
        for(int i=0;i < tmpnt.nLayer; i++){
            double min=99999999999999999;
            double max=-99999999999999999;
            for(int j=0; j < this->nBinRow; j++){
                    for( int k = 0; k < this->nBinCol; k++ ){
                        if(smooth_potential3D[i][j][k].EnableValue<min){
                            min=smooth_potential3D[i][j][k].EnableValue;
                        }
                        if(smooth_potential3D[i][j][k].EnableValue>max){
                            max=smooth_potential3D[i][j][k].EnableValue;
                        }
                    }
            }
            max-=min;
            for(int j=0; j < this->nBinRow; j++){
                    for( int k = 0; k < this->nBinCol; k++ ){
                        smooth_potential3D[i][j][k].EnableValue-=min;
                        smooth_potential3D[i][j][k].EnableValue/=max;
                    }
            }
        }*/

        //Level smooth
        /*cout<<"\nLevel smooth.....\n";
        
            for(int i=0;i < tmpnt.nLayer; i++){
                double potential_average=0;
                for(int j=0; j < this->nBinRow; j++){
                    for( int k = 0; k < this->nBinCol; k++ ){
                        potential_average+=smooth_potential3D[i][j][k].EnableValue;
                    }
                }
                potential_average/=(this->nBinRow*this->nBinCol);
                for(int j=0; j < this->nBinRow; j++){
                    for( int k = 0; k < this->nBinCol; k++ ){
                        if(smooth_potential3D[i][j][k].EnableValue>=potential_average){
                            
                            
                                //cout<<potential_average+pow((smooth_potential3D[i][j][k].EnableValue-potential_average),2.0)<<" ";
                                level_smoothing_potential3D[i][j][k].EnableValue=((double)this->binW/(double)AMP_PARA) * ((double)this->binH/(double)AMP_PARA)*(potential_average+pow((smooth_potential3D[i][j][k].EnableValue-potential_average),2.0));
                                bin3D[i][j][k].EnableValue -= level_smoothing_potential3D[i][j][k].EnableValue;
                            
                        }
                        else{
                            
                            
                                //cout<<potential_average-pow((potential_average-smooth_potential3D[i][j][k].EnableValue),2.0)<<" ";
                                level_smoothing_potential3D[i][j][k].EnableValue=((double)this->binW/(double)AMP_PARA) * ((double)this->binH/(double)AMP_PARA)*(potential_average-pow((potential_average-smooth_potential3D[i][j][k].EnableValue),2.0));
                                bin3D[i][j][k].EnableValue -= level_smoothing_potential3D[i][j][k].EnableValue;
                            
                        }
                    }
                }
            }*/
        
    }
    //plot bin info
    /*string plot_prplace="bin/"+InputOption.benName+"_"+to_string(this->CurrentLevel);
    plot_preplaced_info(plot_prplace,tmpnt);*/
    tempBin.clear();
    BIN2D (tempBin).swap(tempBin);
    cout<<" ------------------------"<<endl;
}

void WLGP::plot_preplaced_info(string filename,NETLIST& tmpnt){
    string fileName_top=filename+"_top.txt";
    ofstream fout(fileName_top.c_str());
   
    for( int j = 0; j < this->nBinRow; j++ )
    {
        for( int k = 0; k < this->nBinCol; k++ )
        {
            fout<<bin3D[0][j][k].EnableValue<<" "; 
                        
        }
        fout<<"\n";
    }
    fout.close();
    /*string fileName_bot=filename+"_bot.txt";
    ofstream ffout(fileName_bot.c_str());
    
    for( int j = 0; j <= this->nBinRow; j++ )
    {
        for( int k = 0; k <= this->nBinCol; k++ )
        {
            ffout<<bin3D[1][j][k].EnableValue<<" "; 
                        
        }
        ffout<<"\n";
    }
    ffout.close();*/
}
double* WLGP::CalWireGradient( NETLIST& tmpnt )
{
    double LSE_WL_top = 0;
    double LSE_WL_bot = 0;

    /// initial wirelength gradient
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        this->WG_X[0][i] = 0.0;
        this->WG_Y[0][i] = 0.0;
        this->WG_X[1][i] = 0.0;
        this->WG_Y[1][i] = 0.0;
    }

    /// calculate each term in LSE wirelength model for modules
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        if(tmpnt.mods[i].modL==0){
            double tempX = tmpnt.mods[i].GCenterX/(double)AMP_PARA;
            double tempY = tmpnt.mods[i].GCenterY/(double)AMP_PARA;

            this->Xp[0][i] = exp( (tempX/Gamma)  );
            this->Xn[0][i] = exp( (-tempX/Gamma) );
            this->Yp[0][i] = exp( (tempY/Gamma)  );
            this->Yn[0][i] = exp( (-tempY/Gamma) );
            if(tmpnt.mods[i].flagTSV==true){
                this->Xp[1][i] = exp( (tempX/Gamma)  );
                this->Xn[1][i] = exp( (-tempX/Gamma) );
                this->Yp[1][i] = exp( (tempY/Gamma)  );
                this->Yn[1][i] = exp( (-tempY/Gamma) );
            }
        }
        else{
            double tempX = tmpnt.mods[i].GCenterX/(double)AMP_PARA;
            double tempY = tmpnt.mods[i].GCenterY/(double)AMP_PARA;

            this->Xp[1][i] = exp( (tempX/Gamma)  );
            this->Xn[1][i] = exp( (-tempX/Gamma) );
            this->Yp[1][i] = exp( (tempY/Gamma)  );
            this->Yn[1][i] = exp( (-tempY/Gamma) );
        }
    }

    

    for( int i = 0; i < tmpnt.nNet; i++ )
    {

        double XpSum_top = 0;		///< Sum(exp(X))
        double XnSum_top = 0;		///< Sum(exp(-X))
        double YpSum_top = 0;		///< Sum(exp(Y))
        double YnSum_top = 0;		///< Sum(exp(-Y))

        double XpSumInv_top = 0;	///< 1/Sum(exp(X))
        double XnSumInv_top = 0;	///< 1/Sum(exp(-X))
        double YpSumInv_top = 0;	///< 1/Sum(exp(Y))
        double YnSumInv_top = 0;	///< 1/Sum(exp(-Y))

        double XpSum_bot = 0;		///< Sum(exp(X))
        double XnSum_bot = 0;		///< Sum(exp(-X))
        double YpSum_bot = 0;		///< Sum(exp(Y))
        double YnSum_bot = 0;		///< Sum(exp(-Y))

        double XpSumInv_bot = 0;	///< 1/Sum(exp(X))
        double XnSumInv_bot = 0;	///< 1/Sum(exp(-X))
        double YpSumInv_bot = 0;	///< 1/Sum(exp(Y))
        double YnSumInv_bot = 0;	///< 1/Sum(exp(-Y))
        bool has_top=false;
        bool has_bot=false;
        for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
        {

            int modID = tmpnt.pins[j].index;
            if(tmpnt.mods[modID].modL==0){
                has_top=true;
                XpSum_top += this->Xp[0][modID];
                XnSum_top += this->Xn[0][modID];
                YpSum_top += this->Yp[0][modID];
                YnSum_top += this->Yn[0][modID];
                if(tmpnt.mods[modID].flagTSV==true){
                    has_bot=true;
                    XpSum_bot += this->Xp[1][modID];
                    XnSum_bot += this->Xn[1][modID];
                    YpSum_bot += this->Yp[1][modID];
                    YnSum_bot += this->Yn[1][modID];
                }
            }
            else{
                has_bot=true;
                XpSum_bot += this->Xp[1][modID];
                XnSum_bot += this->Xn[1][modID];
                YpSum_bot += this->Yp[1][modID];
                YnSum_bot += this->Yn[1][modID];
            }
        }
        /*if(XpSum<=0||XnSum<=0||YpSum<=0||YnSum<=0){
            cout<<"     Sum <= 0: "<<XpSum<<" "<<XnSum<<" "<<YpSum<<" "<<YnSum<<"\n";
        }*/
        if(has_top==true)
            LSE_WL_top += ( log(XpSum_top) + log(XnSum_top) + log(YpSum_top) + log(YnSum_top) );
        if(has_bot==true)
            LSE_WL_bot += ( log(XpSum_bot) + log(XnSum_bot) + log(YpSum_bot) + log(YnSum_bot) );
        if(has_top==true){
            XpSumInv_top = 1/XpSum_top;
            XnSumInv_top = 1/XnSum_top;
            YpSumInv_top = 1/YpSum_top;
            YnSumInv_top = 1/YnSum_top;
        }
        else{
            XpSumInv_top = 0;
            XnSumInv_top = 0;
            YpSumInv_top = 0;
            YnSumInv_top = 0;
        }
        if(has_bot==true){
            XpSumInv_bot = 1/XpSum_bot;
            XnSumInv_bot = 1/XnSum_bot;
            YpSumInv_bot = 1/YpSum_bot;
            YnSumInv_bot = 1/YnSum_bot;
        }
        else{
            XpSumInv_bot = 0;
            XnSumInv_bot = 0;
            YpSumInv_bot = 0;
            YnSumInv_bot = 0;
        }

        for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
        {
            int modID = tmpnt.pins[j].index;
            int GRatio = 1;
            if( modID >= tmpnt.nMod )
                continue;


            if(tmpnt.nets[i].flagAlign)     ///<enhance align net weight
                GRatio=10;///10000000int max 2,147,483,647
            else
                GRatio=1;
            if(tmpnt.mods[modID].modL==0){
                this->WG_X[0][modID] += GRatio * XpSumInv_top * this->Xp[0][modID];
                this->WG_X[0][modID] -= GRatio * XnSumInv_top * this->Xn[0][modID];
                this->WG_Y[0][modID] += GRatio * YpSumInv_top * this->Yp[0][modID];
                this->WG_Y[0][modID] -= GRatio * YnSumInv_top * this->Yn[0][modID];
                if(tmpnt.mods[modID].flagTSV==true){
                    this->WG_X[1][modID] += GRatio * XpSumInv_bot * this->Xp[1][modID];
                    this->WG_X[1][modID] -= GRatio * XnSumInv_bot * this->Xn[1][modID];
                    this->WG_Y[1][modID] += GRatio * YpSumInv_bot * this->Yp[1][modID];
                    this->WG_Y[1][modID] -= GRatio * YnSumInv_bot * this->Yn[1][modID];
                }
            }
            else{
                this->WG_X[1][modID] += GRatio * XpSumInv_bot * this->Xp[1][modID];
                this->WG_X[1][modID] -= GRatio * XnSumInv_bot * this->Xn[1][modID];
                this->WG_Y[1][modID] += GRatio * YpSumInv_bot * this->Yp[1][modID];
                this->WG_Y[1][modID] -= GRatio * YnSumInv_bot * this->Yn[1][modID];
            }
        }


    }



    LSE_WL_top *= this->Gamma;
    LSE_WL_bot *= this->Gamma;
    double* LSE_WL=new double[2];
    LSE_WL[0]=LSE_WL_top;
    LSE_WL[1]=LSE_WL_bot;
    return LSE_WL;
}
void WLGP::calpuredensgradient(NETLIST& tmpnt){
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        this->pure_DX[0][i] = 0;
        this->pure_DY[0][i] = 0;
        this->pure_DX[1][i] = 0;
        this->pure_DY[1][i] = 0;
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
    if(InputOption.QP_key==true){
        for( int i = 0; i < tmpnt.nMod; i++ )
        {

            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
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
                        //cout<<"\n\n\nelse is occuring2\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring3\n\n\n";
                    }
                }

                for( int j = rowB; j <= rowT; j++ )
                {
                    for( int k = colL; k <= colR; k++ )
                    {
                        this->pure_DX[modL][i] += dP_X[k-colL] * P_Y[j-rowB] * this->Cv[i] ;
                        this->pure_DY[modL][i] += P_X[k-colL] * dP_Y[j-rowB] * this->Cv[i] ;
                    }
                }

                delete [] P_X;
                delete [] P_Y;
                delete [] dP_X;
                delete [] dP_Y ;

            ///f///0925
        }
    }
    else{
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].is_m==false){
            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
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
                        //cout<<"\n\n\nelse is occuring2\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring3\n\n\n";
                    }
                }

                for( int j = rowB; j <= rowT; j++ )
                {
                    for( int k = colL; k <= colR; k++ )
                    {
                        this->pure_DX[modL][i] += dP_X[k-colL] * P_Y[j-rowB] * this->Cv[i] ;
                        this->pure_DY[modL][i] += P_X[k-colL] * dP_Y[j-rowB] * this->Cv[i] ;
                    }
                }

                delete [] P_X;
                delete [] P_Y;
                delete [] dP_X;
                delete [] dP_Y ;

            ///f///0925
            }
        }
    }
}
double* WLGP::CalDensityGradient( NETLIST& tmpnt,bool & start_monitor_nan )
{
    
    
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        this->DG_X[0][i] = 0;
        this->DG_Y[0][i] = 0;
        this->DG_X[1][i] = 0;
        this->DG_Y[1][i] = 0;
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

    double totalBinDensity_top = 0;
    double totalBinDensity_bot = 0;
    if(InputOption.QP_key==true){
        /// calculate OccupyValue in each bin & Cv
        for( int i = 0; i < tmpnt.nMod; i++ )
        {

            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
                modL = tmpnt.mods[i].modL;
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
                        //cout<<"\n\n\nelse is occuring0\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring1\n\n\n";
                    }
                }

                for( int j = 0; j <= (rowT-rowB); j++ )
                {
                    for( int k = 0; k <= (colR-colL); k++ )
                    {
                        Db += (P_X[k] * P_Y[j]);
                    }
                }
                
                /*if(i==46316){
                cout<<"cchheecckk\n";
                cout<<tmpnt.mods[i].GLeftX<<" "<<tmpnt.mods[i].GCenterX<<" "<<tmpnt.mods[i].modW<<"\n";
                cout<<tmpnt.mods[i].GLeftY<<" "<<tmpnt.mods[i].GCenterY<<" "<<tmpnt.mods[i].modH<<"\n";
                }*/
                this->Cv[i] = tmpnt.mods[i].modArea/Db;
                if(Db!=0){
                    this->Cv[i] = tmpnt.mods[i].modArea/Db;
                }
                else{
                    this->Cv[i] = tmpnt.mods[i].modArea/1;
                }
                if(start_monitor_nan==true){
                    for( int j = 0; j <= (rowT-rowB); j++ )
                    {
                        for( int k = 0; k <= (colR-colL); k++ )
                        {
                            cout<<P_X[k]<<" "<<P_Y[j]<<"\n";
                        }
                    }
                }
                if(start_monitor_nan==true)
                    cout<<"   Cv[i]: "<<this->Cv[i]<<"\n";
                for( int j = rowB; j <= rowT; j++ )
                {
                    for( int k = colL; k <= colR; k++ )
                    {
                        
                        bin3D[modL][j][k].OccupyValue += this->Cv[i] * P_X[k-colL] * P_Y[j-rowB];
                        if(start_monitor_nan==true)
                            cout<<"      bin3D[modL][j][k].OccupyValue: "<<i<<" "<<j<<" "<<k<<" "<<bin3D[modL][j][k].OccupyValue<<"\n";
                        
                    }
                }

                delete [] P_X;
                delete [] P_Y;


            ///f///0925


        }



        /// calculate density gradient
        for( int i = 0; i < tmpnt.nMod; i++ )
        {

            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
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
                        //cout<<"\n\n\nelse is occuring2\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring3\n\n\n";
                    }
                }

                for( int j = rowB; j <= rowT; j++ )
                {
                    for( int k = colL; k <= colR; k++ )
                    {
                        this->DG_X[modL][i] += dP_X[k-colL] * P_Y[j-rowB] * this->Cv[i] * (bin3D[modL][j][k].OccupyValue - bin3D[modL][j][k].EnableValue) * 2;
                        this->DG_Y[modL][i] += P_X[k-colL] * dP_Y[j-rowB] * this->Cv[i] * (bin3D[modL][j][k].OccupyValue - bin3D[modL][j][k].EnableValue) * 2;
                    }
                }

                delete [] P_X;
                delete [] P_Y;
                delete [] dP_X;
                delete [] dP_Y ;

            ///f///0925

        }
    }
    else{
        /// calculate OccupyValue in each bin & Cv
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].is_m==false){
            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
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
                        //cout<<"\n\n\nelse is occuring0\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring1\n\n\n";
                    }
                }

                for( int j = 0; j <= (rowT-rowB); j++ )
                {
                    for( int k = 0; k <= (colR-colL); k++ )
                    {
                        Db += (P_X[k] * P_Y[j]);
                    }
                }
                
                /*if(i==46316){
                cout<<"cchheecckk\n";
                cout<<tmpnt.mods[i].GLeftX<<" "<<tmpnt.mods[i].GCenterX<<" "<<tmpnt.mods[i].modW<<"\n";
                cout<<tmpnt.mods[i].GLeftY<<" "<<tmpnt.mods[i].GCenterY<<" "<<tmpnt.mods[i].modH<<"\n";
                }*/
                this->Cv[i] = tmpnt.mods[i].modArea/Db;
                if(Db!=0){
                    this->Cv[i] = tmpnt.mods[i].modArea/Db;
                }
                else{
                    this->Cv[i] = tmpnt.mods[i].modArea/1;
                }
                if(start_monitor_nan==true){
                    for( int j = 0; j <= (rowT-rowB); j++ )
                    {
                        for( int k = 0; k <= (colR-colL); k++ )
                        {
                            cout<<P_X[k]<<" "<<P_Y[j]<<"\n";
                        }
                    }
                }
                if(start_monitor_nan==true)
                    cout<<"   Cv[i]: "<<this->Cv[i]<<"\n";
                for( int j = rowB; j <= rowT; j++ )
                {
                    for( int k = colL; k <= colR; k++ )
                    {
                        //cout<<this->Cv[i] * P_X[k-colL] * P_Y[j-rowB]/tmpnt.mods[i].modArea<<" ";
                        bin3D[modL][j][k].OccupyValue += this->Cv[i] * P_X[k-colL] * P_Y[j-rowB];
                        if(start_monitor_nan==true)
                            cout<<"      bin3D[modL][j][k].OccupyValue: "<<i<<" "<<j<<" "<<k<<" "<<bin3D[modL][j][k].OccupyValue<<"\n";
                        
                    }
                }

                delete [] P_X;
                delete [] P_Y;


            ///f///0925
            }

        }

        /*for( int i = 0; i < this->nBinRow; i++ )
        {
            for( int j = 0; j < this->nBinCol; j++ )
            {
                cout<<"      bin3D[i][j].OccupyValue: "<<i<<" "<<j<<" "<<bin3D[0][i][j].OccupyValue<<"\n";
            }
        }*/

        /// calculate density gradient
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].is_m==false){
            ///f///0925 stacked module has multiple layer
            
                int modL = tmpnt.mods[i].modL;
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
                        //cout<<"\n\n\nelse is occuring2\n\n\n";
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
                        //cout<<"\n\n\nelse is occuring3\n\n\n";
                    }
                }

                for( int j = rowB; j <= rowT; j++ )
                {
                    for( int k = colL; k <= colR; k++ )
                    {
                        this->DG_X[modL][i] += dP_X[k-colL] * P_Y[j-rowB] * this->Cv[i] * (bin3D[modL][j][k].OccupyValue - bin3D[modL][j][k].EnableValue) * 2;
                        this->DG_Y[modL][i] += P_X[k-colL] * dP_Y[j-rowB] * this->Cv[i] * (bin3D[modL][j][k].OccupyValue - bin3D[modL][j][k].EnableValue) * 2;
                    }
                }

                delete [] P_X;
                delete [] P_Y;
                delete [] dP_X;
                delete [] dP_Y ;

            ///f///0925
            }
        }
    }
    //overflow_area_1 = 0;

    for( int i = 0; i < tmpnt.nLayer; i++ )
    {
        for( int j = 0; j < nBinRow; j++ )
        {
            for( int k = 0; k < nBinCol; k++ )
            {
                if(i==0)
                    totalBinDensity_top += pow( (bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue), 2.0 );
                else
                    totalBinDensity_bot += pow( (bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue), 2.0 );
                
            }
        }
    }
    double* totalBinDensity=new double [2];
    totalBinDensity[0]=totalBinDensity_top;
    totalBinDensity[1]=totalBinDensity_bot;
    this->check_every_gradient(tmpnt);
    return totalBinDensity;
}

void WLGP::check_every_gradient(NETLIST tmpnt){
    
        /*for(int j=0;j<tmpnt.nMod;j++){
            
                if(tmpnt.mods[j].modL==1 && (WG_X[0][j]!=0 || WG_Y[0][j]!=0 ||DG_X[0][j]!=0 ||DG_Y[0][j]!=0||pure_DX[0][j]!=0 || pure_DY[0][j]!=0)){
                    cout<<"wrong gradient info(top)!!!\n";
                }
            
            
                if(tmpnt.mods[j].modL==0 && (WG_X[1][j]!=0 || WG_Y[1][j]!=0 ||DG_X[1][j]!=0 ||DG_Y[1][j]!=0||pure_DX[1][j]!=0 || pure_DY[1][j]!=0) &&tmpnt.mods[j].flagTSV==false){
                    cout<<"wrong gradient info(bot)!!!\n";
                }
            
        }*/
    
}

void WLGP::plot_med( NETLIST tmpnt, string filename ){
    filename="dpx/med/"+filename;
    ofstream fout_d;    ///< for display.x
    fout_d.open((filename + "_top.dpx").c_str());

    if (!fout_d)
    {
        cerr << "!error  : can't open file " << (filename + "_top.dpx") << endl;
        return;
    }

    fout_d << "COLOR black" << endl;
    fout_d << "SR  " << "0 -0 " << (double)tmpnt.ChipWidth / (double)AMP_PARA << " " << -((double)tmpnt.ChipHeight / (double)AMP_PARA) << endl;
    /*fout_d << "COLOR gray" << endl;
    fout_d << "SRF  " << Chip_bound_llx / AMP_PARA << " " << - Chip_bound_lly / AMP_PARA << " " << Chip_bound_urx / AMP_PARA << " " << -(Chip_bound_ury / AMP_PARA) << endl;*/
    /*
        if( (binH*binW) != 0 )
        {
            for( int binLeftX = 0; (binLeftX + binW) < tmpnt.ChipWidth; binLeftX += binW )
            {
                for( int binLeftY = 0; (binLeftY + binH) < tmpnt.ChipHeight; binLeftY += binH )
                {
                    fout_d << "COLOR gray" << endl;
                    fout_d << "SR  " << (float)binLeftX/(float)AMP_PARA << " "
                                     << -((float)binLeftY/(float)AMP_PARA) << " "
                                     << (float)(binLeftX + binW)/(float)AMP_PARA << " "
                                     << -((float)(binLeftY + binH)/(float)AMP_PARA) << endl;
                 }
            }
        }
    */

    for ( unsigned int i = 0; i < tmpnt.nMod; i++ )
    {
		if(tmpnt.mods[i].modL==0&&(!tmpnt.mods[i].flagTSV)){
			double llx = (double)tmpnt.mods[i].GLeftX / (double)AMP_PARA;
			double lly = (double)tmpnt.mods[i].GLeftY / (double)AMP_PARA;
			double urx = (double)tmpnt.mods[i].GLeftX / (double)AMP_PARA + (double)tmpnt.mods[i].modW / (double)AMP_PARA;
			double ury = (double)tmpnt.mods[i].GLeftY / (double)AMP_PARA + (double)tmpnt.mods[i].modH / (double)AMP_PARA;

			if (tmpnt.mods[i].is_m == 0)//cell
			{
				/*if ( tmpnt.mods[i].clustered == 1 )
					fout_d << "COLOR white" << endl;
				else*/
					fout_d << "COLOR green" << endl;

				fout_d << "SR " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				// fout_d << "COLOR black" << endl;
				// fout_d << "SR  " << llx << " " << -lly << " " << urx << " " << -ury << endl;
			}
			else//macro
			{
				/*if ( tmpnt.mods[i].clustered == 1 )
					fout_d << "COLOR orange" << endl;
				else*/
					fout_d << "COLOR blue" << endl;

				fout_d << "SR " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				// fout_d << "COLOR black" << endl;
				fout_d << "SL  " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				fout_d << "SL  " << llx << " " << -ury << " " << urx << " " << -lly << endl;
			}

			// if (filename.find("REGP_Lv") == filename.npos && filename.find("QP") == filename.npos
			//         && filename.find("Final") == filename.npos && filename.find("Center") == filename.npos )
			// {
			//     if (tmpnt.nMod < 10000)
			//     {
			//         fout_d << "COLOR red" << endl;
			//         fout_d << "SL  " << gnu_original_x[i] / (double)AMP_PARA << " " << -gnu_original_y[i] / (double)AMP_PARA << " " << tmpnt.mods[i].GCenterX / (double)AMP_PARA << " " << -tmpnt.mods[i].GCenterY / (double)AMP_PARA << endl; //plot a line
			//     }
			// }
		}
    }
    fout_d.close();
	fout_d.open((filename + "_tsv.dpx").c_str());

    if (!fout_d)
    {
        cerr << "!error  : can't open file " << (filename + "_tsv.dpx") << endl;
        return;
    }

    fout_d << "COLOR black" << endl;
    fout_d << "SR  " << "0 -0 " << (double)tmpnt.ChipWidth / (double)AMP_PARA << " " << -((double)tmpnt.ChipHeight / (double)AMP_PARA) << endl;
    /*fout_d << "COLOR gray" << endl;
    fout_d << "SRF  " << Chip_bound_llx / AMP_PARA << " " << - Chip_bound_lly / AMP_PARA << " " << Chip_bound_urx / AMP_PARA << " " << -(Chip_bound_ury / AMP_PARA) << endl;*/
    /*
        if( (binH*binW) != 0 )
        {
            for( int binLeftX = 0; (binLeftX + binW) < tmpnt.ChipWidth; binLeftX += binW )
            {
                for( int binLeftY = 0; (binLeftY + binH) < tmpnt.ChipHeight; binLeftY += binH )
                {
                    fout_d << "COLOR gray" << endl;
                    fout_d << "SR  " << (float)binLeftX/(float)AMP_PARA << " "
                                     << -((float)binLeftY/(float)AMP_PARA) << " "
                                     << (float)(binLeftX + binW)/(float)AMP_PARA << " "
                                     << -((float)(binLeftY + binH)/(float)AMP_PARA) << endl;
                 }
            }
        }
    */

    for ( unsigned int i = 0; i < tmpnt.nMod; i++ )
    {
		if(tmpnt.mods[i].modL==0&&(tmpnt.mods[i].flagTSV)){
			double llx = (double)tmpnt.mods[i].GLeftX / (double)AMP_PARA;
			double lly = (double)tmpnt.mods[i].GLeftY / (double)AMP_PARA;
			double urx = (double)tmpnt.mods[i].GLeftX / (double)AMP_PARA + (double)tmpnt.mods[i].modW / (double)AMP_PARA;
			double ury = (double)tmpnt.mods[i].GLeftY / (double)AMP_PARA + (double)tmpnt.mods[i].modH / (double)AMP_PARA;

			if (tmpnt.mods[i].is_m == 0)//cell
			{
				/*if ( tmpnt.mods[i].clustered == 1 )
					fout_d << "COLOR white" << endl;
				else*/
					fout_d << "COLOR green" << endl;

				fout_d << "SR " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				// fout_d << "COLOR black" << endl;
				// fout_d << "SR  " << llx << " " << -lly << " " << urx << " " << -ury << endl;
			}
			else//macro
			{
				/*if ( tmpnt.mods[i].clustered == 1 )
					fout_d << "COLOR orange" << endl;
				else*/
					fout_d << "COLOR blue" << endl;

				fout_d << "SR " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				// fout_d << "COLOR black" << endl;
				fout_d << "SL  " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				fout_d << "SL  " << llx << " " << -ury << " " << urx << " " << -lly << endl;
			}

			// if (filename.find("REGP_Lv") == filename.npos && filename.find("QP") == filename.npos
			//         && filename.find("Final") == filename.npos && filename.find("Center") == filename.npos )
			// {
			//     if (tmpnt.nMod < 10000)
			//     {
			//         fout_d << "COLOR red" << endl;
			//         fout_d << "SL  " << gnu_original_x[i] / (double)AMP_PARA << " " << -gnu_original_y[i] / (double)AMP_PARA << " " << tmpnt.mods[i].GCenterX / (double)AMP_PARA << " " << -tmpnt.mods[i].GCenterY / (double)AMP_PARA << endl; //plot a line
			//     }
			// }
		}
    }
    fout_d.close();
	fout_d.open((filename + "_bot.dpx").c_str());

    if (!fout_d)
    {
        cerr << "!error  : can't open file " << (filename + "_bot.dpx") << endl;
        return;
    }

    fout_d << "COLOR black" << endl;
    fout_d << "SR  " << "0 -0 " << (double)tmpnt.ChipWidth / (double)AMP_PARA << " " << -((double)tmpnt.ChipHeight / (double)AMP_PARA) << endl;
    /*fout_d << "COLOR gray" << endl;
    fout_d << "SRF  " << Chip_bound_llx / AMP_PARA << " " << - Chip_bound_lly / AMP_PARA << " " << Chip_bound_urx / AMP_PARA << " " << -(Chip_bound_ury / AMP_PARA) << endl;*/
    /*
        if( (binH*binW) != 0 )
        {
            for( int binLeftX = 0; (binLeftX + binW) < tmpnt.ChipWidth; binLeftX += binW )
            {
                for( int binLeftY = 0; (binLeftY + binH) < tmpnt.ChipHeight; binLeftY += binH )
                {
                    fout_d << "COLOR gray" << endl;
                    fout_d << "SR  " << (float)binLeftX/(float)AMP_PARA << " "
                                     << -((float)binLeftY/(float)AMP_PARA) << " "
                                     << (float)(binLeftX + binW)/(float)AMP_PARA << " "
                                     << -((float)(binLeftY + binH)/(float)AMP_PARA) << endl;
                 }
            }
        }
    */

    for ( unsigned int i = 0; i < tmpnt.nMod; i++ )
    {
		if(tmpnt.mods[i].modL==1){
			double llx = (double)tmpnt.mods[i].GLeftX / (double)AMP_PARA;
			double lly = (double)tmpnt.mods[i].GLeftY / (double)AMP_PARA;
			double urx = (double)tmpnt.mods[i].GLeftX / (double)AMP_PARA + (double)tmpnt.mods[i].modW / (double)AMP_PARA;
			double ury = (double)tmpnt.mods[i].GLeftY / (double)AMP_PARA + (double)tmpnt.mods[i].modH / (double)AMP_PARA;

			if (tmpnt.mods[i].is_m == 0)//cell
			{
				/*if ( tmpnt.mods[i].clustered == 1 )
					fout_d << "COLOR white" << endl;
				else*/
					fout_d << "COLOR green" << endl;

				fout_d << "SR " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				// fout_d << "COLOR black" << endl;
				// fout_d << "SR  " << llx << " " << -lly << " " << urx << " " << -ury << endl;
			}
			else//macro
			{
				/*if ( tmpnt.mods[i].clustered == 1 )
					fout_d << "COLOR orange" << endl;
				else*/
					fout_d << "COLOR blue" << endl;

				fout_d << "SR " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				// fout_d << "COLOR black" << endl;
				fout_d << "SL  " << llx << " " << -lly << " " << urx << " " << -ury << endl;
				fout_d << "SL  " << llx << " " << -ury << " " << urx << " " << -lly << endl;
			}

			// if (filename.find("REGP_Lv") == filename.npos && filename.find("QP") == filename.npos
			//         && filename.find("Final") == filename.npos && filename.find("Center") == filename.npos )
			// {
			//     if (tmpnt.nMod < 10000)
			//     {
			//         fout_d << "COLOR red" << endl;
			//         fout_d << "SL  " << gnu_original_x[i] / (double)AMP_PARA << " " << -gnu_original_y[i] / (double)AMP_PARA << " " << tmpnt.mods[i].GCenterX / (double)AMP_PARA << " " << -tmpnt.mods[i].GCenterY / (double)AMP_PARA << endl; //plot a line
			//     }
			// }
		}
    }
    fout_d.close();
}

void WLGP::InitializeGradient( NETLIST& tmpnt,bool & start_monitor_nan)
{
    if(InputOption.QP_key==true){
        double* WLterm = this->CalWireGradient( tmpnt );		///< wirelength term in the objective function
        //cout<<"  WL(top): "<<WLterm[0]<<"  WL(bot): "<<WLterm[1]<<" \n";
        double* BDterm = this->CalDensityGradient( tmpnt,start_monitor_nan );	///< density term in the objective function
        
        //cout<<"  BD(top): "<<BDterm[0]<<"  BD(bot): "<<BDterm[1]<<" \n";
        double dk_top = 0;
        double wk_top = 0;
        double dk_bot = 0;
        double wk_bot = 0;
        //calpuredensgradient(tmpnt);
        //8/15 dk=DG相關改成pureDX相關
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].modL==0){
                //dk_top += sqrt( this->pure_DX[0][i]*this->pure_DX[0][i] + this->pure_DY[0][i]*this->pure_DY[0][i] );
                dk_top += sqrt( this->DG_X[0][i]*this->DG_X[0][i] + this->DG_Y[0][i]*this->DG_Y[0][i] );
                wk_top += sqrt( this->WG_X[0][i]*this->WG_X[0][i] + this->WG_Y[0][i]*this->WG_Y[0][i] );
            }
            else{
                //dk_bot += sqrt( this->pure_DX[1][i]*this->pure_DX[1][i] + this->pure_DY[1][i]*this->pure_DY[1][i] );
                dk_bot += sqrt( this->DG_X[1][i]*this->DG_X[1][i] + this->DG_Y[1][i]*this->DG_Y[1][i] );
                wk_bot += sqrt( this->WG_X[1][i]*this->WG_X[1][i] + this->WG_Y[1][i]*this->WG_Y[1][i] );
            }
        }
        //cout<<dk<<" "<<wk<<"\n";

        this->lambdaW = 1;

        int LevelCount =(int) this->TotalLevel;

        if(this->CurrentLevel == LevelCount-1)
        {
            lambda_top = InputOption.InitialGradient * (wk_top/dk_top);
            lambda_bot = InputOption.InitialGradient * (wk_bot/dk_bot);
        }
        else
        {
            lambda_top = InputOption.DeclusterGradient * (wk_top/dk_top);
            lambda_bot = InputOption.DeclusterGradient * (wk_bot/dk_bot);
        }
        //cout<<lambda<<"\n";
        Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
        Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];

        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].modL==0){
                //cout<<this->GX_1[i]<<" "<<this->WG_X[i]<<" "<<this->DG_X[i]<<"\n";
                this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];

                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = 0;
                this->DY_0[0][i] = 0;
                if(tmpnt.mods[i].flagTSV==true){
                    this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                    this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                    this->GX_0[1][i] = this->GX_1[1][i];
                    this->GY_0[1][i] = this->GY_1[1][i];
                    this->DX_0[1][i] = 0;
                    this->DY_0[1][i] = 0;
                }
            }
            else{
                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = 0;
                this->DY_0[1][i] = 0;
            }
        }
        string QP_I0=InputOption.benName_sec+"QP_I0";
        //plot_med( tmpnt, QP_I0 );
    }
    else{
        double* WLterm = this->CalWireGradient( tmpnt );		///< wirelength term in the objective function
        //cout<<"  WL(top): "<<WLterm[0]<<"  WL(bot): "<<WLterm[1]<<" \n";
        double* BDterm = this->CalDensityGradient( tmpnt,start_monitor_nan );	///< density term in the objective function
        
        //cout<<"  BD(top): "<<BDterm[0]<<"  BD(bot): "<<BDterm[1]<<" \n";
        double dk_top = 0;
        double wk_top = 0;
        double dk_bot = 0;
        double wk_bot = 0;
        //calpuredensgradient(tmpnt);

        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    dk_top += sqrt( this->DG_X[0][i]*this->DG_X[0][i] + this->DG_Y[0][i]*this->DG_Y[0][i] );
                    //dk_top += sqrt( this->pure_DX[0][i]*this->pure_DX[0][i] + this->pure_DY[0][i]*this->pure_DY[0][i] );
                    wk_top += sqrt( this->WG_X[0][i]*this->WG_X[0][i] + this->WG_Y[0][i]*this->WG_Y[0][i] );
                }
                else{
                     dk_bot += sqrt( this->DG_X[1][i]*this->DG_X[1][i] + this->DG_Y[1][i]*this->DG_Y[1][i] );
                    //dk_bot += sqrt( this->pure_DX[1][i]*this->pure_DX[1][i] + this->pure_DY[1][i]*this->pure_DY[1][i] );
                    wk_bot += sqrt( this->WG_X[1][i]*this->WG_X[1][i] + this->WG_Y[1][i]*this->WG_Y[1][i] );
                }
            }
        }
        //cout<<dk<<" "<<wk<<"\n";

        this->lambdaW = 1;

        int LevelCount =(int) this->TotalLevel;

        
        if(InputOption.test_mode==201){
            if(this->CurrentLevel == LevelCount-1)
            {
                /*lambda_top = InputOption.InitialGradient * (wk_top/dk_top);
                lambda_bot = InputOption.InitialGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                /*lambda_top/=10;
                lambda_bot/=10;*/
            }
            else
            {
                /*lambda_top = InputOption.DeclusterGradient * (wk_top/dk_top);
                lambda_bot = InputOption.DeclusterGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                /*lambda_top/=10;
                lambda_bot/=10;*/
            }
        }
        else if(InputOption.test_mode==202){
            if(this->CurrentLevel == LevelCount-1)
            {
                /*lambda_top = InputOption.InitialGradient * (wk_top/dk_top);
                lambda_bot = InputOption.InitialGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                lambda_top/=10;
                lambda_bot/=10;
            }
            else
            {
                /*lambda_top = InputOption.DeclusterGradient * (wk_top/dk_top);
                lambda_bot = InputOption.DeclusterGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                lambda_top/=10;
                lambda_bot/=10;
            }
        }
        else if(InputOption.test_mode==205){
            if(this->CurrentLevel == LevelCount-1)
            {
                /*lambda_top = InputOption.InitialGradient * (wk_top/dk_top);
                lambda_bot = InputOption.InitialGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                lambda_top*=10;
                lambda_bot*=10;
            }
            else
            {
                /*lambda_top = InputOption.DeclusterGradient * (wk_top/dk_top);
                lambda_bot = InputOption.DeclusterGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                lambda_top*=10;
                lambda_bot*=10;
            }
        }
        else{
            if(this->CurrentLevel == LevelCount-1)
            {
                /*lambda_top = InputOption.InitialGradient * (wk_top/dk_top);
                lambda_bot = InputOption.InitialGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                /*lambda_top/=10;
                lambda_bot/=10;*/
            }
            else
            {
                /*lambda_top = InputOption.DeclusterGradient * (wk_top/dk_top);
                lambda_bot = InputOption.DeclusterGradient * (wk_bot/dk_bot);*/
                lambda_top=WLterm[0]/BDterm[0];
                lambda_bot=WLterm[1]/BDterm[1];
                /*lambda_top/=10;
                lambda_bot/=10;*/
            }
        }
        //cout<<lambda<<"\n";
        Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
        Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            //cout<<this->GX_1[i]<<" "<<this->WG_X[i]<<" "<<this->DG_X[i]<<"\n";
            if(tmpnt.mods[i].is_m==false){
                

                if(tmpnt.mods[i].modL==0){
                    //cout<<this->GX_1[i]<<" "<<this->WG_X[i]<<" "<<this->DG_X[i]<<"\n";
                    this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                    this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];

                    this->GX_0[0][i] = this->GX_1[0][i];
                    this->GY_0[0][i] = this->GY_1[0][i];
                    this->DX_0[0][i] = 0;
                    this->DY_0[0][i] = 0;
                    if(tmpnt.mods[i].flagTSV==true){
                        this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                        this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                        this->GX_0[1][i] = this->GX_1[1][i];
                        this->GY_0[1][i] = this->GY_1[1][i];
                        this->DX_0[1][i] = 0;
                        this->DY_0[1][i] = 0;
                    }
                }
                else{
                    this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                    this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];

                    this->GX_0[1][i] = this->GX_1[1][i];
                    this->GY_0[1][i] = this->GY_1[1][i];
                    this->DX_0[1][i] = 0;
                    this->DY_0[1][i] = 0;
                }
            }
        }
        string QP_I0=InputOption.benName_sec+"NotQP_I0";
        //plot_med( tmpnt, QP_I0 );
    }
}


void WLGP::ConjugateGradient_last_stage( NETLIST& tmpnt , bool & start_bad_sol, bool & start_monitor_nan,int countt,double start_time,bool& halt)
{
    double lambda_origin=0.0;
    int count = 0;
    bool startt=false;
    if(start_monitor_nan==true&&startt==false){
        start_monitor_nan=false;
        startt=true;
    }
    
    double *originX = new double [tmpnt.nMod];
    double *originY = new double [tmpnt.nMod];
    /*int Fk_best=9999999;
    double original_dens_1;
    double original_fk_1;
    double original_dens_2;
    double original_fk_2;
    double original_dens;
    double original_fk;*/
    
    double* WLterm_o = this->CalWireGradient( tmpnt );		///< wirelength term in the objective function
    //cout<<"  WL0(top): "<<WLterm_o[0]<<"  WL0(bot): "<<WLterm_o[1]<<" \n";
    double* BDterm_o = this->CalDensityGradient( tmpnt,start_monitor_nan );	///< density term in the objective function
    
    //cout<<"  BD0(top): "<<BDterm_o[0]<<"  BD0(bot): "<<BDterm_o[1]<<" \n\n";
    bool* do_layer=new bool[2];
    do_layer[0]=true;
    do_layer[1]=true;
    
    if(true){
        do
        {
            clock_t iter_t=clock();
            if(((double)(iter_t)-start_time)/CLOCKS_PER_SEC>1000){
                cout<<"excess run time!!\n";
                halt=true;
                break;
            }
            if(do_layer[0]==true&&do_layer[1]==true){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
            if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    beta_upper_x[0] += ( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                    beta_upper_y[0] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) );
                    
                    beta_lower_x[0] += (this->GX_0[0][i] * this->GX_0[0][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[0] +=  (this->GY_0[0][i] * this->GY_0[0][i]);  //l2norm sum(x)^2
                    if(tmpnt.mods[i].flagTSV==true){
                        beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                        beta_upper_y[1] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                        beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                        beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                    }
                }
                else{
                    beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) )*/;
                
                

                    beta_upper_y[1] += /*( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                    beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                }
            }
            }
            beta_x[0] = beta_upper_x[0]/beta_lower_x[0];
            beta_y[0] = beta_upper_y[0]/beta_lower_y[0];
            beta_x[1] = beta_upper_x[1]/beta_lower_x[1];
            beta_y[1] = beta_upper_y[1]/beta_lower_y[1];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->DX_1[0][i] = (-this->GX_1[0][i] + beta_x[0] * this->DX_0[0][i]);
                    
                    this->DY_1[0][i] = (-this->GY_1[0][i] + beta_y[0] * this->DY_0[0][i]);
                    if(tmpnt.mods[i].flagTSV==true){
                        this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                        this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                    }
                }
                else{
                    this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                    this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                }
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    alpha_lower_x[0] += this->DX_1[0][i]*this->DX_1[0][i] ;
                    alpha_lower_y[0] += this->DY_1[0][i]*this->DY_1[0][i];
                    if(tmpnt.mods[i].flagTSV==true){
                        alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                        alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                    }
                }
                else{
                    alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                    alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                }
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->alpha_x[0][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[0]/tmpnt.nMod);
                    this->alpha_y[0][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[0]/tmpnt.nMod);
                    if(tmpnt.mods[i].flagTSV==true){
                        this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                        this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                    }
                }
                else{
                    this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                    this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                }
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    
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
                else{
                    tmpnt.mods[i].GCenterX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    
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
                }
            }
            string into=InputOption.benName_sec+"NotQP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = this->DX_1[0][i];
                this->DY_0[0][i] = this->DY_1[0][i];
                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = this->DX_1[1][i];
                this->DY_0[1][i] = this->DY_1[1][i];
                }
            }

            Fk_0_top = Fk_1_top;
            Fk_0_bot = Fk_1_bot;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
            Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_top < Fk_0_top || Fk_1_bot<Fk_0_bot))
            {
                if(Fk_1_top < Fk_0_top && Fk_1_bot<Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }

                        }
                    }
                    do_layer[0]=true;
                    do_layer[1]=true;
                }
                else if(Fk_1_top < Fk_0_top && Fk_1_bot>=Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            
                        }
                        }


                    }
                    do_layer[0]=true;
                    do_layer[1]=false;
                }
                else if(Fk_1_top >= Fk_0_top && Fk_1_bot<Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }

                        }
                    }
                    do_layer[0]=false;
                    do_layer[1]=true;
                }
            }
            else{
                do_layer[0]=false;
                do_layer[1]=false;
            }
            if(do_layer[0]==false &&do_layer[1]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    
                    
                }
            }
            else if(do_layer[0]==true &&do_layer[1]==false){
                
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }
            else if(do_layer[0]==false &&do_layer[1]==true){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }

            }
            else if(do_layer[0]==false&&do_layer[1]==true){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                        beta_upper_y[1] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                        beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                        beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                    }
                }
                else{
                    beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) )*/;
                
                

                    beta_upper_y[1] += /*( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                    beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                }
                }
            }
            beta_x[1] = beta_upper_x[1]/beta_lower_x[1];
            beta_y[1] = beta_upper_y[1]/beta_lower_y[1];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                        this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                    }
                }
                else{
                    this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                    this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                }
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                        alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                    }
                }
                else{
                    alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                    alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                }
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                        this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                    }
                }
                else{
                    this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                    this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                }
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                
                    
                if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    
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
                }
            }
            string into=InputOption.benName_sec+"NotQP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = this->DX_1[1][i];
                this->DY_0[1][i] = this->DY_1[1][i];
                }
            }

            Fk_0_bot = Fk_1_bot;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_bot<Fk_0_bot))
            {
                
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }

                        }
                    }
                    do_layer[1]=true;
                
                
            }
            else{
                do_layer[1]=false;
            }
            if(do_layer[1]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }
            }
            else if(do_layer[0]==true&&do_layer[1]==false){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
            if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    beta_upper_x[0] += ( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                    beta_upper_y[0] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) );
                    
                    beta_lower_x[0] += (this->GX_0[0][i] * this->GX_0[0][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[0] +=  (this->GY_0[0][i] * this->GY_0[0][i]);  //l2norm sum(x)^2
                    
                }
            }
            }
            beta_x[0] = beta_upper_x[0]/beta_lower_x[0];
            beta_y[0] = beta_upper_y[0]/beta_lower_y[0];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->DX_1[0][i] = (-this->GX_1[0][i] + beta_x[0] * this->DX_0[0][i]);
                    
                    this->DY_1[0][i] = (-this->GY_1[0][i] + beta_y[0] * this->DY_0[0][i]);
                    
                }
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    alpha_lower_x[0] += this->DX_1[0][i]*this->DX_1[0][i] ;
                    alpha_lower_y[0] += this->DY_1[0][i]*this->DY_1[0][i];
                    
                }
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->alpha_x[0][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[0]/tmpnt.nMod);
                    this->alpha_y[0][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[0]/tmpnt.nMod);
                   
                }
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    
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
                }
            }
            string into=InputOption.benName_sec+"NotQP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = this->DX_1[0][i];
                this->DY_0[0][i] = this->DY_1[0][i];
                }
            }

            Fk_0_top = Fk_1_top;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_top < Fk_0_top ))
            {
                
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            
                        }
                        

                        }
                    }
                    do_layer[0]=true;
                
                
                
            }
            else{
                do_layer[0]=false;
            }
            if(do_layer[0]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }
            }
            
        }
    // while(count<=50);
        while( count<=3000 &&(do_layer[0]==true ||  do_layer[1]==true)); //7月12 <  ->  <=l
        //cout<<"Fk_1(top):"<<Fk_1_top<<" Fk_0(top):"<<Fk_0_top<<" lambda(top):"<<lambda_top<<" count:"<<count << "\n";
        //cout<<"Fk_1(bot):"<<Fk_1_bot<<" Fk_0(bot):"<<Fk_0_bot<<" lambda(bot):"<<lambda_bot<<" count:"<<count << "\n\n";
        if (count > 1 && start_bad_sol==false) {
            start_bad_sol = true;
        }
        
        this->numIter += count;
    }
    
    
    delete [] originX;
    delete [] originY;
}


void WLGP::ConjugateGradient( NETLIST& tmpnt , bool & start_bad_sol, bool & start_monitor_nan,int countt)
{
    double lambda_origin=0.0;
    int count = 0;
    bool startt=false;
    if(start_monitor_nan==true&&startt==false){
        start_monitor_nan=false;
        startt=true;
    }
    
    double *originX = new double [tmpnt.nMod];
    double *originY = new double [tmpnt.nMod];
    /*int Fk_best=9999999;
    double original_dens_1;
    double original_fk_1;
    double original_dens_2;
    double original_fk_2;
    double original_dens;
    double original_fk;*/
    
    double* WLterm_o = this->CalWireGradient( tmpnt );		///< wirelength term in the objective function
    //cout<<"  WL0(top): "<<WLterm_o[0]<<"  WL0(bot): "<<WLterm_o[1]<<" \n";
    double* BDterm_o = this->CalDensityGradient( tmpnt,start_monitor_nan );	///< density term in the objective function
    
    //cout<<"  BD0(top): "<<BDterm_o[0]<<"  BD0(bot): "<<BDterm_o[1]<<" \n\n";
    bool* do_layer=new bool[2];
    do_layer[0]=true;
    do_layer[1]=true;
    if(InputOption.QP_key==true){
        do
        {
            if(do_layer[0]==true&&do_layer[1]==true){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
                if(tmpnt.mods[i].modL==0){
                    beta_upper_x[0] += ( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                    beta_upper_y[0] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) );
                    
                    beta_lower_x[0] += (this->GX_0[0][i] * this->GX_0[0][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[0] +=  (this->GY_0[0][i] * this->GY_0[0][i]);  //l2norm sum(x)^2
                    if(tmpnt.mods[i].flagTSV==true){
                        beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                        beta_upper_y[1] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                        beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                        beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                    }
                }
                else{
                    beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) )*/;
                
                

                    beta_upper_y[1] += /*( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                    beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                }
            }
            beta_x[0] = beta_upper_x[0]/beta_lower_x[0];
            beta_y[0] = beta_upper_y[0]/beta_lower_y[0];
            beta_x[1] = beta_upper_x[1]/beta_lower_x[1];
            beta_y[1] = beta_upper_y[1]/beta_lower_y[1];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    this->DX_1[0][i] = (-this->GX_1[0][i] + beta_x[0] * this->DX_0[0][i]);
                    
                    this->DY_1[0][i] = (-this->GY_1[0][i] + beta_y[0] * this->DY_0[0][i]);
                    if(tmpnt.mods[i].flagTSV==true){
                        this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                        this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                    }
                }
                else{
                    this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                    this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    alpha_lower_x[0] += this->DX_1[0][i]*this->DX_1[0][i] ;
                    alpha_lower_y[0] += this->DY_1[0][i]*this->DY_1[0][i];
                    if(tmpnt.mods[i].flagTSV==true){
                        alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                        alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                    }
                }
                else{
                    alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                    alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    this->alpha_x[0][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[0]/tmpnt.nMod);
                    this->alpha_y[0][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[0]/tmpnt.nMod);
                    if(tmpnt.mods[i].flagTSV==true){
                        this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                        this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                    }
                }
                else{
                    this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                    this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    
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
                else{
                    tmpnt.mods[i].GCenterX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    
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
            }
            string into=InputOption.benName_sec+"QP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = this->DX_1[0][i];
                this->DY_0[0][i] = this->DY_1[0][i];
                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = this->DX_1[1][i];
                this->DY_0[1][i] = this->DY_1[1][i];
            }

            Fk_0_top = Fk_1_top;
            Fk_0_bot = Fk_1_bot;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
            Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_top < Fk_0_top || Fk_1_bot<Fk_0_bot))
            {
                if(Fk_1_top < Fk_0_top && Fk_1_bot<Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {

                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }


                    }
                    do_layer[0]=true;
                    do_layer[1]=true;
                }
                else if(Fk_1_top < Fk_0_top && Fk_1_bot>=Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {

                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            
                        }
                        


                    }
                    do_layer[0]=true;
                    do_layer[1]=false;
                }
                else if(Fk_1_top >= Fk_0_top && Fk_1_bot<Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {

                        if(tmpnt.mods[i].modL==0){
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }


                    }
                    do_layer[0]=false;
                    do_layer[1]=true;
                }
            }
            else{
                do_layer[0]=false;
                do_layer[1]=false;
            }
            if(do_layer[0]==false &&do_layer[1]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    
                    
                }
            }
            }
            else if(do_layer[0]==false&&do_layer[1]==true){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                        beta_upper_y[1] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                        beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                        beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                    }
                }
                else{
                    beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) )*/;
                
                

                    beta_upper_y[1] += /*( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                    beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                }
            }
            beta_x[1] = beta_upper_x[1]/beta_lower_x[1];
            beta_y[1] = beta_upper_y[1]/beta_lower_y[1];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                        this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                    }
                }
                else{
                    this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                    this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                        alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                    }
                }
                else{
                    alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                    alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                        this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                    }
                }
                else{
                    this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                    this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                
                    
                if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    
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
            }
            string into=InputOption.benName_sec+"QP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = this->DX_1[1][i];
                this->DY_0[1][i] = this->DY_1[1][i];
            }

            Fk_0_bot = Fk_1_bot;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_bot<Fk_0_bot))
            {
                
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {

                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }


                    }
                    do_layer[1]=true;
                
                
            }
            else{
                do_layer[1]=false;
            }
            if(do_layer[1]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    
                }
            }
            }
            else if(do_layer[0]==true&&do_layer[1]==false){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
                if(tmpnt.mods[i].modL==0){
                    beta_upper_x[0] += ( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                    beta_upper_y[0] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) );
                    
                    beta_lower_x[0] += (this->GX_0[0][i] * this->GX_0[0][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[0] +=  (this->GY_0[0][i] * this->GY_0[0][i]);  //l2norm sum(x)^2
                    
                }
                
            }
            beta_x[0] = beta_upper_x[0]/beta_lower_x[0];
            beta_y[0] = beta_upper_y[0]/beta_lower_y[0];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    this->DX_1[0][i] = (-this->GX_1[0][i] + beta_x[0] * this->DX_0[0][i]);
                    
                    this->DY_1[0][i] = (-this->GY_1[0][i] + beta_y[0] * this->DY_0[0][i]);
                    
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    alpha_lower_x[0] += this->DX_1[0][i]*this->DX_1[0][i] ;
                    alpha_lower_y[0] += this->DY_1[0][i]*this->DY_1[0][i];
                    
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].modL==0){
                    this->alpha_x[0][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[0]/tmpnt.nMod);
                    this->alpha_y[0][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[0]/tmpnt.nMod);
                   
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    
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
            }
            string into=InputOption.benName_sec+"QP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = this->DX_1[0][i];
                this->DY_0[0][i] = this->DY_1[0][i];
            }

            Fk_0_top = Fk_1_top;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_top < Fk_0_top ))
            {
                
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {

                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                           
                        }


                    }
                    do_layer[0]=true;
                
                
                
            }
            else{
                do_layer[0]=false;
            }
            if(do_layer[0]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    
                }
            }
            }
            
        }
    // while(count<=50);
        while( count<=3000 &&(do_layer[0]==true ||  do_layer[1]==true)); //7月12 <  ->  <=l
        //cout<<"Fk_1(top):"<<Fk_1_top<<" Fk_0(top):"<<Fk_0_top<<" lambda(top):"<<lambda_top<<" count:"<<count << "\n";
        //cout<<"Fk_1(bot):"<<Fk_1_bot<<" Fk_0(bot):"<<Fk_0_bot<<" lambda(bot):"<<lambda_bot<<" count:"<<count << "\n\n";
        if (count > 1 && start_bad_sol==false) {
            start_bad_sol = true;
        }
        
        this->numIter += count;
        
    }
    else{
        do
        {
            if(do_layer[0]==true&&do_layer[1]==true){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
            if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    beta_upper_x[0] += ( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                    beta_upper_y[0] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) );
                    
                    beta_lower_x[0] += (this->GX_0[0][i] * this->GX_0[0][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[0] +=  (this->GY_0[0][i] * this->GY_0[0][i]);  //l2norm sum(x)^2
                    if(tmpnt.mods[i].flagTSV==true){
                        beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                        beta_upper_y[1] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                        beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                        beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                    }
                }
                else{
                    beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) )*/;
                
                

                    beta_upper_y[1] += /*( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                    beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                }
            }
            }
            beta_x[0] = beta_upper_x[0]/beta_lower_x[0];
            beta_y[0] = beta_upper_y[0]/beta_lower_y[0];
            beta_x[1] = beta_upper_x[1]/beta_lower_x[1];
            beta_y[1] = beta_upper_y[1]/beta_lower_y[1];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->DX_1[0][i] = (-this->GX_1[0][i] + beta_x[0] * this->DX_0[0][i]);
                    
                    this->DY_1[0][i] = (-this->GY_1[0][i] + beta_y[0] * this->DY_0[0][i]);
                    if(tmpnt.mods[i].flagTSV==true){
                        this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                        this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                    }
                }
                else{
                    this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                    this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                }
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    alpha_lower_x[0] += this->DX_1[0][i]*this->DX_1[0][i] ;
                    alpha_lower_y[0] += this->DY_1[0][i]*this->DY_1[0][i];
                    if(tmpnt.mods[i].flagTSV==true){
                        alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                        alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                    }
                }
                else{
                    alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                    alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                }
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->alpha_x[0][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[0]/tmpnt.nMod);
                    this->alpha_y[0][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[0]/tmpnt.nMod);
                    if(tmpnt.mods[i].flagTSV==true){
                        this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                        this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                    }
                }
                else{
                    this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                    this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                }
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    
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
                else{
                    tmpnt.mods[i].GCenterX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    
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
                }
            }
            string into=InputOption.benName_sec+"NotQP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = this->DX_1[0][i];
                this->DY_0[0][i] = this->DY_1[0][i];
                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = this->DX_1[1][i];
                this->DY_0[1][i] = this->DY_1[1][i];
                }
            }

            Fk_0_top = Fk_1_top;
            Fk_0_bot = Fk_1_bot;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
            Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_top < Fk_0_top || Fk_1_bot<Fk_0_bot))
            {
                if(Fk_1_top < Fk_0_top && Fk_1_bot<Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }

                        }
                    }
                    do_layer[0]=true;
                    do_layer[1]=true;
                }
                else if(Fk_1_top < Fk_0_top && Fk_1_bot>=Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            
                        }
                        }


                    }
                    do_layer[0]=true;
                    do_layer[1]=false;
                }
                else if(Fk_1_top >= Fk_0_top && Fk_1_bot<Fk_0_bot){
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }

                        }
                    }
                    do_layer[0]=false;
                    do_layer[1]=true;
                }
            }
            else{
                do_layer[0]=false;
                do_layer[1]=false;
            }
            if(do_layer[0]==false &&do_layer[1]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    
                    
                }
            }
            else if(do_layer[0]==true &&do_layer[1]==false){
                
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }
            else if(do_layer[0]==false &&do_layer[1]==true){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }

            }
            else if(do_layer[0]==false&&do_layer[1]==true){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                        beta_upper_y[1] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                        beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                        beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                    }
                }
                else{
                    beta_upper_x[1] += ( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) /*+ ( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) )*/;
                
                

                    beta_upper_y[1] += /*( this->GX_1[1][i] * (this->GX_1[1][i]-this->GX_0[1][i]) ) + */( this->GY_1[1][i] * (this->GY_1[1][i]-this->GY_0[1][i]) );
                    
                    beta_lower_x[1] += (this->GX_0[1][i] * this->GX_0[1][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[1] +=  (this->GY_0[1][i] * this->GY_0[1][i]);  //l2norm sum(x)^2
                }
                }
            }
            beta_x[1] = beta_upper_x[1]/beta_lower_x[1];
            beta_y[1] = beta_upper_y[1]/beta_lower_y[1];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                        this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                    }
                }
                else{
                    this->DX_1[1][i] = (-this->GX_1[1][i] + beta_x[1] * this->DX_0[1][i]);
                    
                    this->DY_1[1][i] = (-this->GY_1[1][i] + beta_y[1] * this->DY_0[1][i]);
                }
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                        alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                    }
                }
                else{
                    alpha_lower_x[1] += this->DX_1[1][i]*this->DX_1[1][i] ;
                    alpha_lower_y[1] += this->DY_1[1][i]*this->DY_1[1][i];
                }
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    if(tmpnt.mods[i].flagTSV==true){
                        this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                        this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                    }
                }
                else{
                    this->alpha_x[1][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[1]/tmpnt.nMod);
                    this->alpha_y[1][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[1]/tmpnt.nMod);
                }
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                
                    
                if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[1][i] * this->DX_1[1][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[1][i] * this->DY_1[1][i] * AMP_PARA);
                    
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
                }
            }
            string into=InputOption.benName_sec+"NotQP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                this->GX_0[1][i] = this->GX_1[1][i];
                this->GY_0[1][i] = this->GY_1[1][i];
                this->DX_0[1][i] = this->DX_1[1][i];
                this->DY_0[1][i] = this->DY_1[1][i];
                }
            }

            Fk_0_bot = Fk_1_bot;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_bot = WLterm[1] + lambda_bot * BDterm[1];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_bot<Fk_0_bot))
            {
                
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            if(tmpnt.mods[i].flagTSV==true){
                                this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                                this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                            }
                        }
                        else{
                            this->GX_1[1][i] = this->WG_X[1][i] + lambda_bot * this->DG_X[1][i];
                            this->GY_1[1][i] = this->WG_Y[1][i] + lambda_bot * this->DG_Y[1][i];
                        }

                        }
                    }
                    do_layer[1]=true;
                
                
            }
            else{
                do_layer[1]=false;
            }
            if(do_layer[1]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==1){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }
            }
            else if(do_layer[0]==true&&do_layer[1]==false){
            count++;
            if(startt==true&&count>613){
                start_monitor_nan=true;
            }
            // calculate beta
            double* beta_x = new double[2];
            double* beta_upper_x = new double[2];	///< numerator of beta
            double* beta_lower_x = new double[2];	///< denominator of beta
            double* beta_y = new double[2];
            double* beta_upper_y = new double[2];	///< numerator of beta
            double* beta_lower_y = new double[2];	///< denominator of beta
            for( int i = 0; i < tmpnt.nMod; i++ )
            {//jack 8/4
            if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    beta_upper_x[0] += ( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) /*+ ( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) )*/;
                
                

                    beta_upper_y[0] += /*( this->GX_1[0][i] * (this->GX_1[0][i]-this->GX_0[0][i]) ) + */( this->GY_1[0][i] * (this->GY_1[0][i]-this->GY_0[0][i]) );
                    
                    beta_lower_x[0] += (this->GX_0[0][i] * this->GX_0[0][i]) ;  //l2norm sum(x)^2
                    beta_lower_y[0] +=  (this->GY_0[0][i] * this->GY_0[0][i]);  //l2norm sum(x)^2
                    
                }
            }
            }
            beta_x[0] = beta_upper_x[0]/beta_lower_x[0];
            beta_y[0] = beta_upper_y[0]/beta_lower_y[0];
            
            // calculate conjuagte directions d_k
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->DX_1[0][i] = (-this->GX_1[0][i] + beta_x[0] * this->DX_0[0][i]);
                    
                    this->DY_1[0][i] = (-this->GY_1[0][i] + beta_y[0] * this->DY_0[0][i]);
                    
                }
                }
            }

            // calculate alpha
            // 8/4 jack
            double* alpha_lower_x =new double[2];	///< denominator of beta
            double* alpha_lower_y =new double[2];	///< denominator of beta
            double s;
            if(InputOption.test_mode==6 ||InputOption.test_mode==50){
                s=0.3;
            }
            else if(InputOption.test_mode==7){
                s=0.4;
            }
            else if(InputOption.test_mode==8){
                s=0.5;
            }
            else if(InputOption.test_mode==9){
                s=10;
            }
            else if(InputOption.test_mode==10){
                s=0.1;
            }
            else{
                s = 0.2;			///< user-specified scaling factor
            }

            //jack 8/4
            /*for( int i = 0; i < tmpnt.nMod; i++ )
            {
                alpha_lower_x = sqrt( this->DX_1[i]*this->DX_1[i] );
                this->alpha_x[i]    = (s * binW/(double)AMP_PARA)/alpha_lower_x;
                alpha_lower_y = sqrt( this->DY_1[i]*this->DY_1[i] );
                this->alpha_y[i]    = (s * binH/(double)AMP_PARA)/alpha_lower_y;
            }*/
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    alpha_lower_x[0] += this->DX_1[0][i]*this->DX_1[0][i] ;
                    alpha_lower_y[0] += this->DY_1[0][i]*this->DY_1[0][i];
                    
                }
                }
            }

            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                if(tmpnt.mods[i].modL==0){
                    this->alpha_x[0][i]    = (s * binW/(double)AMP_PARA) / sqrt(alpha_lower_x[0]/tmpnt.nMod);
                    this->alpha_y[0][i]    = (s * binH/(double)AMP_PARA) / sqrt(alpha_lower_y[0]/tmpnt.nMod);
                   
                }
                }
            }


            // update the solution
            
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                originX[i] = tmpnt.mods[i].GCenterX;
                originY[i] = tmpnt.mods[i].GCenterY;
                if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftX += (this->alpha_x[0][i] * this->DX_1[0][i] * AMP_PARA);
                    
                    
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

                    tmpnt.mods[i].GCenterY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    tmpnt.mods[i].GLeftY += (this->alpha_y[0][i] * this->DY_1[0][i] * AMP_PARA);
                    
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
                }
            }
            string into=InputOption.benName_sec+"NotQP_I"+to_string(countt)+"_"+to_string(count);
            //plot_med( tmpnt, into );
            for( int i = 0; i < tmpnt.nMod; i++ )
            {
                if(tmpnt.mods[i].is_m==false){
                this->GX_0[0][i] = this->GX_1[0][i];
                this->GY_0[0][i] = this->GY_1[0][i];
                this->DX_0[0][i] = this->DX_1[0][i];
                this->DY_0[0][i] = this->DY_1[0][i];
                }
            }

            Fk_0_top = Fk_1_top;

            double* WLterm = CalWireGradient( tmpnt );
            //cout<<"  WL"<<count<<"(top): "<<WLterm[0]<<"  WL"<<count<<"(bot): "<<WLterm[1]<<" \n";
            double* BDterm = CalDensityGradient( tmpnt ,start_monitor_nan);
            
            //cout<<"  BD"<<count<<"(top): "<<BDterm[0]<<"  BD"<<count<<"(bot): "<<BDterm[1]<<" \n";
            
            Fk_1_top = WLterm[0] + lambda_top * BDterm[0];
            /*if(count==1||count==2){
                if(count==1){
                original_dens_1=BDterm;
                original_fk_1=Fk_1;
                }
                else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;    
                original_dens=original_dens_1-original_dens_2;
                original_fk=original_fk_1-original_fk_2;
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
                }
                
            }
            else{
                original_dens_2=BDterm;
                original_fk_2=Fk_1;
                
                if(InputOption.test_mode==3 || InputOption.test_mode==9){
                    if(original_dens_1-original_dens_2<original_dens*0.5){
                        break;
                    }
                }
                else if(InputOption.test_mode==4|| InputOption.test_mode==9){
                    if(original_fk_1-original_fk_2<original_fk*0.5 ){
                        break;
                    }
                }
                
                original_dens_1=original_dens_2;
                original_fk_1=original_fk_2;
            }*/
            if(count<=3000 && (Fk_1_top < Fk_0_top ))
            {
                
                    for( int i = 0; i < tmpnt.nMod; i++ )
                    {
                        if(tmpnt.mods[i].is_m==false){
                        if(tmpnt.mods[i].modL==0){
                            this->GX_1[0][i] = this->WG_X[0][i] + lambda_top * this->DG_X[0][i];
                            this->GY_1[0][i] = this->WG_Y[0][i] + lambda_top * this->DG_Y[0][i];
                            
                        }
                        

                        }
                    }
                    do_layer[0]=true;
                
                
                
            }
            else{
                do_layer[0]=false;
            }
            if(do_layer[0]==false){
                for( int i = 0; i < tmpnt.nMod; i++ )
                {
                    if(tmpnt.mods[i].is_m==false){
                    if(tmpnt.mods[i].modL==0){
                    tmpnt.mods[i].GCenterX = originX[i];
                    tmpnt.mods[i].GCenterY = originY[i];
                    tmpnt.mods[i].GLeftX = originX[i] - tmpnt.mods[i].modW/2;
                    tmpnt.mods[i].GLeftY = originY[i] - tmpnt.mods[i].modH/2;
                    }
                    }
                }
            }
            }
            
        }
    // while(count<=50);
        while( count<=3000 &&(do_layer[0]==true ||  do_layer[1]==true)); //7月12 <  ->  <=l
        //cout<<"Fk_1(top):"<<Fk_1_top<<" Fk_0(top):"<<Fk_0_top<<" lambda(top):"<<lambda_top<<" count:"<<count << "\n";
        //cout<<"Fk_1(bot):"<<Fk_1_bot<<" Fk_0(bot):"<<Fk_0_bot<<" lambda(bot):"<<lambda_bot<<" count:"<<count << "\n\n";
        if (count > 1 && start_bad_sol==false) {
            start_bad_sol = true;
        }
        
        this->numIter += count;
    }
    
    
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
        
            int modL = tmpnt.mods[i].modL;
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


        ///f///0925


    }

    overflow_area_top_1 = 0;
    overflow_area_bot_1 = 0;
    for( int i = 0; i < tmpnt.nLayer; i++ )
    {
        for( int j = 0; j < nBinRow; j++ )
        {
            for( int k = 0; k < nBinCol; k++ )
            {
                if(i==0)
                    overflow_area_top_1 += COM_MAX( bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue, 0 );
                else{
                    overflow_area_bot_1 += COM_MAX( bin3D[i][j][k].OccupyValue - bin3D[i][j][k].EnableValue, 0 );
                }
            }
        }
    }
}

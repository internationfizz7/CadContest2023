#include "Legalization.h"




string ILPLG::realtime()
{

    int YEAR_SET(1900),MON_SET(1);

    time_t rawtime;
    struct tm ptrnow;
    time(&rawtime);
    localtime_r(&rawtime,&ptrnow);
    int year = ptrnow.tm_year + YEAR_SET;
    int mon = ptrnow.tm_mon + MON_SET;
    int day = ptrnow.tm_mday;
    int hour = ptrnow.tm_hour;
    int min = ptrnow.tm_min;
    int sec = ptrnow.tm_sec;

    stringstream ss;
    ss<<year<<"/"<<setw(2)<<setfill('0')<<mon<<"/"<<setw(2)<<setfill('0')<<day<<"_";
    ss<<setw(2)<<setfill('0')<<hour<<":"<<setw(2)<<setfill('0')<<min<<":"<<setw(2)<<setfill('0')<<sec;

    string str;
    ss>>str;

    return str;
}


bool ILPLG::checkOverlap(NETLIST &nt)
{
    cout<<"* checkOverlap...";

    bool overlap_flag=false;
    int overlap_num=0;
    for(int l=0;l<nt.nLayer;l++)
    {
        for(unsigned int i=0;i<nt.mods.size();i++)///module i
        {

            if(nt.mods[i].flagTSV==true)
                continue;

            if(!(nt.mods[i].modL<=l && l<=nt.mods[i].modL+nt.mods[i].nLayer-1))
                continue;

            int iLLX=nt.mods[i].LeftX;
            int iLLY=nt.mods[i].LeftY;
            int iURX=nt.mods[i].LeftX+nt.mods[i].modW;
            int iURY=nt.mods[i].LeftY+nt.mods[i].modH;
            int  iID=nt.mods[i].idMod;


            for(unsigned int j=0;j<nt.mods.size();j++)///module j
            {

                if(nt.mods[j].flagTSV==true)
                    continue;

                if(!(nt.mods[j].modL<=l && l<=nt.mods[j].modL+nt.mods[j].nLayer-1))
                    continue;

                if(i==j)
                    continue;

                int jLLX=nt.mods[j].LeftX;
                int jLLY=nt.mods[j].LeftY;
                int jURX=nt.mods[j].LeftX+nt.mods[j].modW;
                int jURY=nt.mods[j].LeftY+nt.mods[j].modH;
                int  jID=nt.mods[j].idMod;


                //printf("(%d)%d\t%d\t%d\t%d\t%d / %d\t%d\t%d\t%d\t%d\n",l,i,iLLX,iLLY,iURX,iURY,j,jLLX,jLLY,jURX,jURY);
                if(jLLX<=iLLX && iLLX<jURX && jLLY<=iLLY && iLLY<jURY)/// "<=": avoid bounding error
                {
                    overlap_flag=true;
//                    cout<<"\nINCORRECT\t";
//                    cout<<"layer="<<l<<"\tiID="<<iID<<"(iLLX:"<<iLLX<<",iLLY:"<<iLLY<<") is in jID="<<jID;
//                    overlap_num++;
                }
                if(jLLX<iURX && iURX<=jURX && jLLY<=iLLY && iLLY<jURY)
                {
                    overlap_flag=true;
//                    cout<<"\nINCORRECT\t";
//                    cout<<"layer="<<l<<"\tiID="<<iID<<"(iURX:"<<iURX<<",iLLY:"<<iLLY<<") is in jID="<<jID;
//                    overlap_num++;
                }
                if(jLLX<iURX && iURX<=jURX && jLLY<iURY && iURY<=jURY)
                {
                    overlap_flag=true;
//                    cout<<"\nINCORRECT\t";
//                    cout<<"layer="<<l<<"\tiID="<<iID<<"(iURX:"<<iURX<<",iURY:"<<iURY<<") is in jID="<<jID;
//                    overlap_num++;
                }
                if(jLLX<=iLLX && iLLX<jURX && jLLY<iURY && iURY<=jURY)
                {
                    overlap_flag=true;
//                    cout<<"\nINCORRECT\t";
//                    cout<<"layer="<<l<<"\tiID="<<iID<<"(iLLX:"<<iLLX<<",iURY:"<<iURY<<") is in jID="<<jID;
//                    overlap_num++;
                }


                if(overlap_flag==true)
                {
                    cout<<"\nINCORRECT\t";
                    cout<<"layer="<<l<<"\tiID="<<iID<<": LL=("<<iLLX<<","<<iLLY<<")"<<" UR=("<<iURX<<","<<iURY<<")"
                                 <<"is in jID="<<jID<<": LL=("<<jLLX<<","<<jLLY<<")"<<" UR=("<<jURX<<","<<jURY<<")";
                    overlap_num++;
                    overlap_flag=false;
                    if(overlap_num>=10)
                    {
                        cout<<"\n...overlap_num>=10";
                        l=nt.nLayer;
                        i=nt.mods.size();
                        j=nt.mods.size();
                        break;
                    }
                }


            }
        }
    }

    if(overlap_num>0)
    {
        cout<<endl;
        return false;
    }
    else
    {
        cout<<"CORRECT\n";
        return true;
    }

}

bool ILPLG::checkOutline(NETLIST &nt)
{
    cout<<"* checkOutline...";

    int MAX_URX(0),MAX_URY(0),MIN_LLX(nt.ChipWidth),MIN_LLY(nt.ChipHeight);
    int tmp_i(0);
    for(unsigned int i=0;i<nt.mods.size();i++)
    {
        int URX=nt.mods[i].LeftX+nt.mods[i].modW;
        int URY=nt.mods[i].LeftY+nt.mods[i].modH;
        int LLX=nt.mods[i].LeftX;
        int LLY=nt.mods[i].LeftY;

        if(URX>MAX_URX)
            MAX_URX=URX;
        if(URY>MAX_URY)
            MAX_URY=URY;
        if(LLX<MIN_LLX)
            MIN_LLX=LLX;
        if(LLY<MIN_LLY)
            MIN_LLY=LLY;

        if(URX>nt.ChipWidth||URY>nt.ChipHeight||LLX<0||LLY<0)
        {
            tmp_i=i;
        }

    }

    if(nt.ChipWidth<MAX_URX||nt.ChipHeight<MAX_URY||MIN_LLX<0||MIN_LLY<0)
    {
        cout<<"INCORRECT\t";
        cout<<"(nt.ChipWidth,nt.ChipHeight)=("<<nt.ChipWidth<<","<<nt.ChipHeight<<")\t";
        cout<<"(MAX_URX,MAX_URY)=("<<MAX_URX<<","<<MAX_URY<<")\t";
        cout<<"(MIN_LLX,MIN_LLY)=("<<MIN_LLX<<","<<MIN_LLY<<")\n";

        cout<<"  mods["<<tmp_i<<"]: ";
        cout<<"LLX="<<nt.mods[tmp_i].LeftX<<", LLY="<<nt.mods[tmp_i].LeftY<<", ";
        cout<<"URX="<<nt.mods[tmp_i].LeftX+nt.mods[tmp_i].modW<<", URY="<<nt.mods[tmp_i].LeftY+nt.mods[tmp_i].modH<<endl;
        return false;
    }
    else
    {
        cout<<"CORRECT\n";
        return true;
    }

}

bool ILPLG::checkArea(NETLIST &nt)
{
    cout<<"* checkArea...";

    int global_area(0),legal_area(0);//

    for(unsigned int i=0;i<nt.mods.size();i++)
    {
        global_area+=nt.mods[i].GmodW/1000*nt.mods[i].GmodH/1000;//
        legal_area +=nt.mods[i].modW /1000*nt.mods[i].modH /1000;//

        long int W=nt.mods[i].modW;
        long int H=nt.mods[i].modH;
        long int A=nt.mods[i].modArea*1000000;
        int     ID=nt.mods[i].idMod;
        if(W*H<A)
        {
            cout<<"INCORRECT\t";
            cout<<"layer="<<nt.mods[i].modL<<"\tID="<<ID<<"  W="<<W<<"  H="<<H<<"  A="<<A<<endl;
            return false;
        }

    }
    cout<<"CORRECT\t";
    cout<<"global_area="<<global_area<<" legal_area="<<legal_area<<endl;
    return true;
}

bool ILPLG::checkRatio(NETLIST &nt)
{
    cout<<"* checkRatio...";

    for(unsigned int i=0;i<nt.mods.size();i++)
    {
        if(nt.mods[i].type=='1')//SOFT
        {
            long int W=nt.mods[i].modW;
            long int H=nt.mods[i].modH;
            int     ID=nt.mods[i].idMod;
            if(3*H<W)
            {
                cout<<"INCORRECT\t";
                cout<<"layer="<<nt.mods[i].modL<<"\tID="<<ID<<" W="<<W<<" H="<<H<<" (3*H<W)"<<endl;
                return false;
            }
            if(3*W<H)
            {
                cout<<"INCORRECT!\t";
                cout<<"layer="<<nt.mods[i].modL<<"\tID="<<ID<<"  W="<<W<<"  H="<<H<<"  (3*W<H)"<<endl;
                return false;
            }
        }
    }

    cout<<"CORRECT\n";
    return true;
}


bool ILPLG::checkPreplace(NETLIST &nt)
{
    cout<<"* checkPreplace...";

    int nonPreplace_num=0;

    for(unsigned int i=0;i<nt.mods.size();i++)
    {
        if(nt.mods[i].flagPre==true)
        {
            int GX=nt.mods[i].GLeftX;
            int GY=nt.mods[i].GLeftY;
            int GW=nt.mods[i].GmodW;
            int GH=nt.mods[i].GmodH;
            int X=nt.mods[i].LeftX;
            int Y=nt.mods[i].LeftY;
            int W=nt.mods[i].modW;
            int H=nt.mods[i].modH;
            if( GX!=X || GY!=Y || GW!=W || GH!=H )
            {
                cout<<"INCORRECT!\t";
                cout<<"layer="<<nt.mods[i].modL<<"\tID="<<nt.mods[i].idMod
                    <<"  (GX,X)=("<<GX<<","<<X<<")"
                    <<"  (GY,Y)=("<<GY<<","<<Y<<")"
                    <<"  (GW,W)=("<<GW<<","<<W<<")"
                    <<"  (GH,H)=("<<GH<<","<<H<<")\n";
                nonPreplace_num++;
                if(nonPreplace_num>=10)
                    ;//break;

            }
        }
    }

    if(nonPreplace_num>0)
    {
        cout<<"...nonPreplace_num="<<nonPreplace_num<<"\n";
        return false;
    }
    else
    {
        cout<<"CORRECT\n";
        return true;
    }

}


int ILPLG::CheckAllRule(NETLIST &nt)
{
    double start_time=double(clock())/CLOCKS_PER_SEC;
    printf("\033[1;32m/========== CheckAllRule %.2f ==========\\\033[0m\n",start_time);

    bool nonOverlap=checkOverlap(nt);
    bool inOutline=checkOutline(nt);
    bool legalArea=checkArea(nt);
    bool inRatio=checkRatio(nt);
    bool inPreplace=checkPreplace(nt);


    printf("Overlap\tOutline\tArea\tRatio\tPreplace\n");
    if(nonOverlap==true)printf("\033[1;42m _   _ \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(inOutline ==true)printf("\033[1;42m _   _ \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(legalArea ==true)printf("\033[1;42m _   _ \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(inRatio   ==true)printf("\033[1;42m _   _ \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(inPreplace==true)printf("\033[1;42m _   _ \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\n");
    if(nonOverlap==true)printf("\033[1;42m *   * \033[0m"); else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(inOutline ==true)printf("\033[1;42m *   * \033[0m"); else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(legalArea ==true)printf("\033[1;42m *   * \033[0m"); else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(inRatio   ==true)printf("\033[1;42m *   * \033[0m"); else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(inPreplace==true)printf("\033[1;42m *   * \033[0m"); else printf("\033[1;41m  * *  \033[0m");printf("\n");
    if(nonOverlap==true)printf("\033[1;42m   |   \033[0m"); else printf("\033[1;41m   *   \033[0m");printf("\t");
    if(inOutline ==true)printf("\033[1;42m   |   \033[0m"); else printf("\033[1;41m   *   \033[0m");printf("\t");
    if(legalArea ==true)printf("\033[1;42m   |   \033[0m"); else printf("\033[1;41m   *   \033[0m");printf("\t");
    if(inRatio   ==true)printf("\033[1;42m   |   \033[0m"); else printf("\033[1;41m   *   \033[0m");printf("\t");
    if(inPreplace==true)printf("\033[1;42m   |   \033[0m"); else printf("\033[1;41m   *   \033[0m");printf("\n");
    if(nonOverlap==true)printf("\033[1;42m \\___/ \033[0m");else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(inOutline ==true)printf("\033[1;42m \\___/ \033[0m");else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(legalArea ==true)printf("\033[1;42m \\___/ \033[0m");else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(inRatio   ==true)printf("\033[1;42m \\___/ \033[0m");else printf("\033[1;41m  * *  \033[0m");printf("\t");
    if(inPreplace==true)printf("\033[1;42m \\___/ \033[0m");else printf("\033[1;41m  * *  \033[0m");printf("\n");
    if(nonOverlap==true)printf("\033[1;42m       \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(inOutline ==true)printf("\033[1;42m       \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(legalArea ==true)printf("\033[1;42m       \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(inRatio   ==true)printf("\033[1;42m       \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\t");
    if(inPreplace==true)printf("\033[1;42m       \033[0m"); else printf("\033[1;41m *   * \033[0m");printf("\n");



    int return_value=0;
    if(nonOverlap==false)
        return_value=1;
    else if(inOutline==false)
        return_value=2;
    else if(legalArea==false)
        return_value=3;
    else if(inRatio==false)
        return_value=4;
    else if(inPreplace==false)
        return_value=5;
    cout<<"* return_value="<<return_value<<endl;


    double end_time=(double)(clock())/CLOCKS_PER_SEC;
    printf("\033[1;32m\\========== CheckAllRule %.2f ==========/\033[0m\n",end_time);

    return return_value;
}

double ILPLG::calculate_movement(NETLIST &nt)
{
    double movement=0;
    for(unsigned int i=0;i<nt.mods.size();i++)
    {
        movement+=sqrt( (nt.mods[i].CenterX-nt.mods[i].GCenterX)/1000*(nt.mods[i].CenterX-nt.mods[i].GCenterX)/1000+
                        (nt.mods[i].CenterY-nt.mods[i].GCenterY)/1000*(nt.mods[i].CenterY-nt.mods[i].GCenterY)/1000
                      );
    }
    //cout<<"* movement="<<movement<<endl;
    return movement;
}

double ILPLG::calculate_HPWL( NETLIST& tmpnt )///PSWu///copy in gp.cpp
{
	int maxX = 0;
	int minX = tmpnt.ChipWidth;
	int maxY = 0;
	int minY = tmpnt.ChipHeight;
	int centerX;
	int centerY;

	tmpnt.totalWL = 0.0;

	for(int i = 0; i < tmpnt.nNet; i++)
	{
		maxX = 0;
		minX = tmpnt.ChipWidth;
		maxY = 0;
		minY = tmpnt.ChipHeight;

		for(int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head+tmpnt.nets[i].nPin; j++)
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

	return tmpnt.totalWL;

}


int ILPLG::LegalizationC(NETLIST &nt)
{

    cout<<"\n";
    cout<<"\033[1;45m ##                              ####             ####  \033[0m\n";
    cout<<"\033[1;45m ##                                ##            ##  ## \033[0m\n";
    cout<<"\033[1;45m ##       ####    #####   ####     ##            ##  ## \033[0m\n";
    cout<<"\033[1;45m ##      ##  ##  ##  ##      ##    ##            ##     \033[0m\n";
    cout<<"\033[1;45m ##      ##  ##  ##  ##      ##    ##            ##     \033[0m\n";
    cout<<"\033[1;45m ##      ######  ##  ##   #####    ##            ##     \033[0m\n";
    cout<<"\033[1;45m ##      ##      ##  ##  ##  ##    ##            ##  ## \033[0m\n";
    cout<<"\033[1;45m ##      ##      ##  ##  ##  ##    ##            ##  ## \033[0m\n";
    cout<<"\033[1;45m ######   ####    #####   #####  ######           ####  \033[0m\n";
    cout<<"\033[1;45m                     ##                                 \033[0m\n";
    cout<<"\033[1;45m                     ##                                 \033[0m\n";
    cout<<"\033[1;45m                 #####                                  \033[0m\n";

    ///print start time
    double start_time=double(clock())/CLOCKS_PER_SEC;
    printf("* start_time: %.2f sec\n",start_time);
    string start_str=realtime();

    vector <int> nVertex3D;//number of modules for each layer
    vector < vector <MODULE*> > module3D;
    vector < vector < vector<int> > > matricesH3D,matricesV3D;
    int nVertex(0);
    vector <MODULE*> module;
    vector < vector<int> > matricesH,matricesV;



    ///check fixed-outline & avoid completely overlap between vertices
    cout<<"* check fixed-outline & avoid completely overlap between vertices..."<<endl;
    int totalArea(0),iter(1),moveNum(0);
    vector <int> Area3D;
    Area3D.resize(nt.nLayer);
    for(int l=0;l<nt.nLayer;l++)
        Area3D[l]=0;
    for(unsigned int i=0;i<nt.mods.size();i++)//module i
    {
        if(iter==1)
        {
            totalArea+=nt.mods[i].modArea;
            Area3D[nt.mods[i].modL]+=nt.mods[i].modArea;

            //because GmodW & GmodH are new definitions
            nt.mods[i].GmodW=nt.mods[i].modW;
            nt.mods[i].GmodH=nt.mods[i].modH;
        }

        if(nt.mods[i].flagTSV==true)
            continue;

        for(unsigned int j=i+1;j<nt.mods.size();j++)//module j
        {

            if(nt.mods[j].flagTSV==true)
                continue;

            if(nt.mods[i].GCenterX==nt.mods[j].GCenterX && nt.mods[i].GCenterY==nt.mods[j].GCenterY)
            {
                moveNum++;

                bool LorR=(nt.mods[i].GCenterX*2<nt.ChipWidth)?0:1;
                bool BorU=(nt.mods[i].GCenterY*2<nt.ChipHeight)?0:1;
                int gapX=nt.mods[i].GmodW/10+iter;
                int gapY=nt.mods[i].GmodH/10+iter;

                if(nt.mods[i].GmodW<nt.mods[i].GmodH)
                {
                    if(LorR==0)
                        nt.mods[j].GCenterX+=gapX;
                    else
                        nt.mods[j].GCenterX-=gapX;
                    nt.mods[j].GLeftX=nt.mods[j].GCenterX-nt.mods[j].GmodW/2;
                }
                else
                {
                    if(BorU==0)
                        nt.mods[j].GCenterY+=gapY;
                    else
                        nt.mods[j].GCenterY-=gapY;
                    nt.mods[j].GLeftY=nt.mods[j].GCenterY-nt.mods[j].GmodH/2;
                }
            }

        }

        if(i>=nt.mods.size()-1)
        {
            cout<<"  move iter="<<iter<<endl;
            if(moveNum>0)
            {
                moveNum=0;
                i=-1;//for loop i++, so that i=0
                iter++;
            }
        }

    }
    cout<<"* module totalArea="<<totalArea;
    for(int l=0;l<nt.nLayer;l++)
        cout<<", Area["<<l<<"]="<<Area3D[l];
    cout<<"\n* nt.ChipWidth="<<nt.ChipWidth<<", nt.ChipHeight="<<nt.ChipHeight<<endl;

	PLOT Ploter;

	

	Ploter.PlotResultGL_dy( nt, "./out/m_Global" ,0 );


    ///AGC loop
    int AGC=2;//apply AGC two times for initialization
    int return_value(0);
    while(1)
    {
        cout<<"* Apply AGC for "<<AGC<<" times..."<<endl;

        ///create constraint graph
        for(int l=0;l<nt.nLayer;l++)
        {
            nVertex=0;
            module.clear();
            for(unsigned int i=0;i<nt.mods.size();i++)
            {

                if(nt.mods[i].flagTSV==true)
                    continue;

                if(nt.mods[i].modL<=l && l<=nt.mods[i].modL+nt.mods[i].nLayer-1)
                {
                    module.push_back(&nt.mods[i]);
                    nVertex++;
                }
            }

            matricesH.clear();
            matricesV.clear();
            matricesH.resize(nVertex);
            matricesV.resize(nVertex);

            ///the matrix[i][j] is the longest path for i -> j
            ///initial edge cost: null = -1, self = 0, exist = 1
            for(int i=0;i<nVertex;i++)
            {
                matricesH[i].resize(nVertex,-1);
                matricesV[i].resize(nVertex,-1);
                for(int j=0;j<nVertex;j++)
                {
                    if(i==j)
                    {
                        matricesH[i][j]=0;
                        matricesV[i][j]=0;
                    }
                }
            }

            ///◢▆▅▄▃-Delaunay triangulation-▃▄▅▆◣f
            
			cout<<"  Layer "<<l<<":"<<endl;
            DT Triangulier;
			Triangulier.DelaunayTriangulation(nVertex,module,matricesH,matricesV,AGC);//2


            nVertex3D.push_back(nVertex);
            module3D.push_back(module);
            matricesH3D.push_back(matricesH);
            matricesV3D.push_back(matricesV);
        }
	
	


        ///◢▆▅▄▃-solve CPLEX-▃▄▅▆◣
		solveCPLEX3D(nVertex3D,module3D,matricesH3D,matricesV3D,nt);

        cout<<"  movement="<<calculate_movement(nt)<<endl;
     
		cout<<"  nt.totalWL="<<calculate_HPWL(nt)<<endl;

        ///shrink SOFT
        cout<<"* shrink soft moudle...\n";
        double shrink;
        int tmp_modW,tmp_modH;
        for(unsigned int i=0;i<nt.mods.size();i++)
        {

            if(nt.mods[i].flagTSV==true)
                continue;

            if(nt.mods[i].type=='1' && nt.mods[i].modW/1000*nt.mods[i].modH/1000>nt.mods[i].modArea )//much bigger than
            {
                shrink=sqrt( ((double)nt.mods[i].modArea+1)/((double)nt.mods[i].modW/1000*(double)nt.mods[i].modH/1000) );

                tmp_modW=(int)(nt.mods[i].modW*shrink);
                tmp_modH=(int)(nt.mods[i].modH*shrink);


                if(nt.mods[i].LeftX+tmp_modW/2<nt.mods[i].GCenterX && nt.mods[i].GCenterX<nt.mods[i].LeftX+nt.mods[i].modW-tmp_modW/2)
                    nt.mods[i].LeftX=nt.mods[i].GCenterX-tmp_modW/2;
                else if(nt.mods[i].GCenterX>nt.mods[i].CenterX)
                    nt.mods[i].LeftX+=nt.mods[i].modW-tmp_modW;

                if(nt.mods[i].LeftY+tmp_modH/2<nt.mods[i].GCenterY && nt.mods[i].GCenterY<nt.mods[i].LeftY+nt.mods[i].modH-tmp_modH/2)
                    nt.mods[i].LeftY=nt.mods[i].GCenterY-tmp_modH/2;
                else if(nt.mods[i].GCenterY>nt.mods[i].CenterY)
                    nt.mods[i].LeftY+=nt.mods[i].modH-tmp_modH;

                nt.mods[i].modW=tmp_modW;
                nt.mods[i].modH=tmp_modH;

                nt.mods[i].CenterX=nt.mods[i].LeftX+nt.mods[i].modW/2;
                nt.mods[i].CenterY=nt.mods[i].LeftY+nt.mods[i].modH/2;
            }

        }

        ///check all design rule
        return_value=CheckAllRule(nt);

        ///AGC loop
        AGC++;
        if(return_value!=1 || AGC>2)//nonloverlap
            break;
        else if(return_value==1)//overlap
            continue;//*/


    }
    cout<<"  movement="<<calculate_movement(nt)<<endl;
    cout<<"  nt.totalWL="<<calculate_HPWL(nt)<<endl;

 	Ploter.Reset();
	Ploter.PlotResultGL_dy( nt, "./out/m_Legal"  ,1 );

/*
    ///◢▆▅▄▃-packing-▃▄▅▆◣
    if(return_value==2)//out of fixed-outline
    {
        cout<<"* packing...\n";
        Packing(nt);
    }//

*/





    ///print end time
    double end_time=double(clock())/CLOCKS_PER_SEC;
    printf("* end_time: %.2f sec\n",end_time);
    string end_str=realtime();
    cout<<"* "<<start_str<<" ~ "<<end_str<<"\n";


    return return_value; 
//	return 0;
    
}


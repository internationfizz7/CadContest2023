#include "solveCPLEX.h"

ILOSTLBEGIN //#define ILOSTLBEGIN using namespace std;

int solveCPLEX3D(vector<int> nVertex3D,vector<vector <MODULE*> > module3D,vector<vector < vector<int> > > matricesH3D,vector< vector < vector<int> > > matricesV3D,NETLIST nt)///int main(int argc, char **argv)
{

    time_t rawtime;
    struct tm ptrnow;
    time(&rawtime);
    localtime_r(&rawtime,&ptrnow);
    int start_second=ptrnow.tm_mday*24*60*60+ptrnow.tm_hour*60*60+ptrnow.tm_min*60+ptrnow.tm_sec;



    double start_time=double(clock())/CLOCKS_PER_SEC;
    printf("\033[1;36m/========== solveCPLEX3D %.2f ==========\\\033[0m\n",start_time);



    ///soft module's breakpoint of area curve
    cout<<"* Soft module:\n";
    int totalSoftModArea(0),totalHardModArea(0);
    int minSoftArea(-1),maxSoftArea(-1);
    for(int i=0;i<nt.nMod;i++)
    {
        if(nt.mods[i].type=='1')
        {
            if(maxSoftArea==-1&&minSoftArea==-1)
            {
                maxSoftArea=nt.mods[i].modArea;
                minSoftArea=nt.mods[i].modArea;
            }
            else
            {
                if(nt.mods[i].modArea>maxSoftArea)maxSoftArea=nt.mods[i].modArea;
                else if(nt.mods[i].modArea<minSoftArea)minSoftArea=nt.mods[i].modArea;
            }
            totalSoftModArea+=nt.mods[i].modArea;
        }
        if(nt.mods[i].type=='0')
            totalHardModArea+=nt.mods[i].modArea;
    }
    cout<<"  nt.totalModArea = "<<nt.totalModArea<<" = "<<totalSoftModArea<<" + "<<totalHardModArea<<"\n";
    cout<<"  Soft area range : "<<minSoftArea<<"~"<<maxSoftArea<<endl;
    int nRange=10;
    vector <int> range2num(nRange,0);
    int rangeSoftArea=(maxSoftArea-minSoftArea)/nRange+1;
    for(int i=0;i<nt.nMod;i++)
    {
        if(nt.mods[i].type=='1')
            range2num[ (int)((nt.mods[i].modArea-minSoftArea)/rangeSoftArea) ]++;
    }
    cout<<"  nt.nSoft="<<nt.nSoft<<"\n";
    for(int i=0;i<(int)range2num.size();i++)
        cout<<"  ["<<i<<"]nSoft="<<range2num[i]<<" : "<<minSoftArea+i*rangeSoftArea<<"~"<<minSoftArea+(i+1)*rangeSoftArea<<"\n";





    IloEnv env3D;//construct an Cplex environment env3D, which belongs to a handle class IloEnv;
    try
    {
        IloModel model(env3D);// define the varies "env3D"

        typedef IloArray <IloNumVarArray> NumVarMatrix;//define two dimensional IloNumVarArray

        ///set locations and shapes of modules
        NumVarMatrix varsURX3D(env3D,nVertex3D.size());
        NumVarMatrix varsURY3D(env3D,nVertex3D.size());
        NumVarMatrix varsW3D(env3D,nVertex3D.size());
        NumVarMatrix varsH3D(env3D,nVertex3D.size());
        for(unsigned int l=0;l<nVertex3D.size();l++)
        {
            varsURX3D[l]=IloNumVarArray(env3D,nVertex3D[l],0,IloInfinity,ILOFLOAT);
            varsURY3D[l]=IloNumVarArray(env3D,nVertex3D[l],0,IloInfinity,ILOFLOAT);
            varsW3D[l]=IloNumVarArray(env3D,nVertex3D[l],0,IloInfinity,ILOFLOAT);
            varsH3D[l]=IloNumVarArray(env3D,nVertex3D[l],0,IloInfinity,ILOFLOAT);
        }

        ///in order to place all modules in fixed-ouline
        IloNumVar ChipW(env3D);
        IloNumVar ChipH(env3D);
        ChipW=IloNumVar(env3D,0,IloInfinity,ILOFLOAT);
        ChipH=IloNumVar(env3D,0,IloInfinity,ILOFLOAT);
        IloNumVar DiffX(env3D);
        IloNumVar DiffY(env3D);
        DiffX=IloNumVar(env3D,0,IloInfinity,ILOFLOAT);
        DiffY=IloNumVar(env3D,0,IloInfinity,ILOFLOAT);
        model.add(DiffX>=ChipW-(double)nt.ChipWidth /1000 && DiffX>=(double)nt.ChipWidth /1000-ChipW);
        model.add(DiffY>=ChipH-(double)nt.ChipHeight/1000 && DiffY>=(double)nt.ChipHeight/1000-ChipH);

        ///in order to minimize voltage island
        IloNumVarArray VI_LLX(env3D,nt.nVI);
        IloNumVarArray VI_LLY(env3D,nt.nVI);
        IloNumVarArray VI_URX(env3D,nt.nVI);
        IloNumVarArray VI_URY(env3D,nt.nVI);
        if(nt.nVI>0)
        {
            for(int v=0;v<nt.nVI;v++)
            {
                VI_LLX[v]=IloNumVar(env3D,nt.nVI,IloInfinity,ILOFLOAT);
                VI_LLY[v]=IloNumVar(env3D,nt.nVI,IloInfinity,ILOFLOAT);
                VI_URX[v]=IloNumVar(env3D,nt.nVI,IloInfinity,ILOFLOAT);
                VI_URY[v]=IloNumVar(env3D,nt.nVI,IloInfinity,ILOFLOAT);
            }
            for(unsigned int l=0;l<nVertex3D.size();l++)
            {
                for(int i=0;i<nVertex3D[l];i++)
                {
                    if(module3D[l][i]->powerMode>=nt.nVI && nt.nVI!=0)
                    {
                        cerr << "err: wrong powerMode!\t idMod:"<<module3D[l][i]->idMod<<" powerMode:"<<module3D[l][i]->powerMode<<"\n";
                        getchar();
                    }

                    model.add( VI_LLX[module3D[l][i]->powerMode]<=varsURX3D[l][i]-varsW3D[l][i] );
                    model.add( VI_LLY[module3D[l][i]->powerMode]<=varsURY3D[l][i]-varsH3D[l][i] );
                    model.add( VI_URX[module3D[l][i]->powerMode]>=varsURX3D[l][i] );
                    model.add( VI_URY[module3D[l][i]->powerMode]>=varsURY3D[l][i] );
                }
            }
        }

        ///in order to minimize voltage island in each layer
        NumVarMatrix VI_LLX3D(env3D,nVertex3D.size());
        NumVarMatrix VI_LLY3D(env3D,nVertex3D.size());
        NumVarMatrix VI_URX3D(env3D,nVertex3D.size());
        NumVarMatrix VI_URY3D(env3D,nVertex3D.size());
        if(nt.nVI>0)
        {
            for(unsigned int l=0;l<nVertex3D.size();l++)
            {
                VI_LLX3D[l]=IloNumVarArray(env3D,nt.nVI,0,IloInfinity,ILOFLOAT);
                VI_LLY3D[l]=IloNumVarArray(env3D,nt.nVI,0,IloInfinity,ILOFLOAT);
                VI_URX3D[l]=IloNumVarArray(env3D,nt.nVI,0,IloInfinity,ILOFLOAT);
                VI_URY3D[l]=IloNumVarArray(env3D,nt.nVI,0,IloInfinity,ILOFLOAT);
            }
            for(unsigned int l=0;l<nVertex3D.size();l++)
            {
                for(int i=0;i<nVertex3D[l];i++)
                {
                    if(module3D[l][i]->powerMode>=nt.nVI && nt.nVI!=0)
                    {
                        cerr << "err: wrong powerMode!\t idMod:"<<module3D[l][i]->idMod<<" powerMode:"<<module3D[l][i]->powerMode<<"\n";
                        getchar();
                    }

                    model.add( VI_LLX3D[l][module3D[l][i]->powerMode]<=varsURX3D[l][i]-varsW3D[l][i] );
                    model.add( VI_LLY3D[l][module3D[l][i]->powerMode]<=varsURY3D[l][i]-varsH3D[l][i] );
                    model.add( VI_URX3D[l][module3D[l][i]->powerMode]>=varsURX3D[l][i] );
                    model.add( VI_URY3D[l][module3D[l][i]->powerMode]>=varsURY3D[l][i] );
                }
            }
        }



        ///objective to optimize:
        IloExpr objA(env3D);///in order to expand soft module
        IloExpr objB(env3D);///in order to move preplace module
        IloExpr objC(env3D);///in order to don't move all module
        IloExpr objD(env3D);///in order to minimize voltage island
        IloExpr objE(env3D);///in order to minimize voltage island in each layer

        for(unsigned int l=0;l<nVertex3D.size();l++)
        {
            for(int i=0;i<nVertex3D[l];i++)
            {
                if(module3D[l][i]->type=='1')
                {
                    objA+=varsW3D[l][i]/nt.ChipWidth+varsH3D[l][i]/nt.ChipHeight;
                }
                if(module3D[l][i]->flagPre==true)
                {
                    objB+=IloAbs(varsURX3D[l][i]*1000-varsW3D[l][i]*1000/2-module3D[l][i]->GCenterX)+
                          IloAbs(varsURY3D[l][i]*1000-varsH3D[l][i]*1000/2-module3D[l][i]->GCenterY);
                }
                objC+=IloAbs(varsURX3D[l][i]*1000-varsW3D[l][i]*1000/2-module3D[l][i]->GCenterX)+
                      IloAbs(varsURY3D[l][i]*1000-varsH3D[l][i]*1000/2-module3D[l][i]->GCenterY);
            }
            if(nt.nVI>0)
            {
                for(int v=0;v<nt.nVI;v++)
                {
                    objE+=VI_URX3D[l][v]-VI_LLX3D[l][v]+VI_URY3D[l][v]-VI_LLY3D[l][v];
                }
            }
        }
        if(nt.nVI>0)
        {
            for(int v=0;v<nt.nVI;v++)
            {
                objD+=VI_URX[v]-VI_LLX[v]+VI_URY[v]-VI_LLY[v];
            }
        }
        ///objective function
        model.add(IloMinimize(  env3D, DiffX+DiffY +objC/10000000 ) );//-objA +objB +objC/10000000 +objD/10000 +objE/10000



        ///subject to:
        for(unsigned int l=0;l<nVertex3D.size();l++)
        {
            for(int i=0;i<nVertex3D[l];i++)
            {

                ///fixed-outline constraint:
                bool left(true),bottom(true),right(true),top(true);
                for(int j=0;j<nVertex3D[l];j++)
                {
                    if(matricesH3D[l][j][i]==1)
                        left=false;
                    if(matricesV3D[l][j][i]==1)
                        bottom=false;
                    if(matricesH3D[l][i][j]==1)
                        right=false;
                    if(matricesV3D[l][i][j]==1)
                        top=false;
                }
                if(left==true)
                    model.add(varsURX3D[l][i]>=varsW3D[l][i]);
                else if(right==true)
                    model.add(varsURX3D[l][i]<=ChipW);
                if(bottom==true)
                    model.add(varsURY3D[l][i]>=varsH3D[l][i]);
                else if(top==true)
                    model.add(varsURY3D[l][i]<=ChipH);


                ///shape constraint:
                ///preplace module
                if(module3D[l][i]->flagPre==true)
                {
                    model.add( varsW3D[l][i]*1000==module3D[l][i]->GmodW );
                    model.add( varsH3D[l][i]*1000==module3D[l][i]->GmodH );
                }
                ///hard module
                else if(module3D[l][i]->type=='0')
                {
                    if(module3D[l][i]->GmodW!=module3D[l][i]->GmodH)
                    {
                        model.add( (varsW3D[l][i]*1000==module3D[l][i]->GmodW&&varsH3D[l][i]*1000==module3D[l][i]->GmodH)
                                || (varsW3D[l][i]*1000==module3D[l][i]->GmodH&&varsH3D[l][i]*1000==module3D[l][i]->GmodW) );
                    }
                    else
                    {
                        model.add( varsW3D[l][i]*1000==module3D[l][i]->GmodW&&varsH3D[l][i]*1000==module3D[l][i]->GmodH );
                    }

                }
                ///soft module
                else if(module3D[l][i]->type=='1')
                {

                    ///aspect ratio
                    model.add( 3*0.99*varsH3D[l][i]>=varsW3D[l][i] && varsH3D[l][i]<=3*0.99*varsW3D[l][i]  );  ///<0.99 for avoid deviation


                    /*///hyperbola
                    model.add( varsH3D[l][i]*varsW3D[l][i] >= module3D[l][i]->modArea );*/

                    ///polyline graph
                    float maxAR=3;//maximum aspect ratio=3
                    int num=(module3D[l][i]->modArea-minSoftArea)/rangeSoftArea+2;//4, (module3D[l][i]->modArea-minSoftArea)/rangeSoftArea+2
                    double A=module3D[l][i]->modArea+1;
                    float tanQ1=1/maxAR;
                    float tanQ2;
                    double x1,y1,x2,y2;
                    float Qstep=( atan(maxAR)-atan(1/maxAR) )/num;

                    for( int n=0 ; n<num ; n++ )
                    {
                        tanQ2=tan( Qstep+atan(tanQ1) );
                        x1=sqrtf(A/tanQ1);
                        y1=A/x1;
                        x2=sqrtf(A/tanQ2);
                        y2=A/x2;
                        model.add( (varsH3D[l][i]-y1)>=(y1-y2)/(x1-x2)*(varsW3D[l][i]-x1) );

                        tanQ1=tanQ2;
                    }

                }
                else
                {
                    cout << "error:\tunable module type \"" << module3D[l][i]->type << "\"\n";
                    exit(1);
                }



                ///stacked module constraint
                if(module3D[l][i]->nLayer>=2)
                {
                    for(unsigned int l2=0;l2<nVertex3D.size();l2++)//nVertex3D.size()
                    {
                        for(int i2=0;i2<nVertex3D[l2];i2++)
                        {
                            ///don't change l!=l2 to l2>l , too slow?
                            if(module3D[l][i]->idMod==module3D[l2][i2]->idMod)
                            {
                                model.add( varsURX3D[l][i]==varsURX3D[l2][i2]);
                                model.add( varsURY3D[l][i]==varsURY3D[l2][i2]);
                                model.add( varsW3D[l][i]==varsW3D[l2][i2]);
                                model.add( varsH3D[l][i]==varsH3D[l2][i2]);

                            }
                        }
                    }
                }


                ///non-overlap constraint
                for(int j=0;j<nVertex3D[l];j++)
                {

                    /*//preplace module
                    if(module3D[l][i]->flagPre==true&&module3D[l][j]->flagPre==true)
                        continue;*/

                    if(matricesH3D[l][i][j]==1)
                    {
                        model.add(varsURX3D[l][i]<=varsURX3D[l][j]-varsW3D[l][j]);
                    }
                    if(matricesV3D[l][i][j]==1)
                    {
                        model.add(varsURY3D[l][i]<=varsURY3D[l][j]-varsH3D[l][j]);
                    }

                }



            }
        }

        //cout<<"getchar";getchar();




        ///expection processing, refer to "C++ try&catch"
        printf("* cplex.solve()...\n");
        IloCplex cplex(model);

        if(!cplex.solve())
        {
            env3D.error()<<"Failed to optimize LP."<<endl;
            throw(-1);
        }

        typedef IloArray<IloNumArray> NumMatrix;
        NumMatrix valsURX3D(env3D,nVertex3D.size());
        NumMatrix valsURY3D(env3D,nVertex3D.size());
        NumMatrix valsW3D(env3D,nVertex3D.size());
        NumMatrix valsH3D(env3D,nVertex3D.size());
        for(unsigned int l=0;l<nVertex3D.size();l++)
        {
            valsURX3D[l]=IloNumArray(env3D);
            valsURY3D[l]=IloNumArray(env3D);
            valsW3D[l]=IloNumArray(env3D);
            valsH3D[l]=IloNumArray(env3D);
        }
        cout<<endl;
        env3D.out() << "* Solution status = "<< cplex.getStatus() << endl;
        //env3D.out() << "* Solution value  = " << cplex.getObjective() << endl;
        for(unsigned int l=0;l<nVertex3D.size();l++)
        {
            cplex.getValues(valsURX3D[l], varsURX3D[l]);
            cplex.getValues(valsURY3D[l], varsURY3D[l]);
            cplex.getValues(valsW3D[l], varsW3D[l]);
            cplex.getValues(valsH3D[l], varsH3D[l]);
            //cout<<"layer="<<l<<endl;
            //env3D.out() << "ValuesURX3D = " << valsURX3D[l] << endl;
            //env3D.out() << "ValuesURY3D = " << valsURY3D[l] << endl;
            //env3D.out() << "ValuesW3D = " << valsW3D[l] << endl;
            //env3D.out() << "ValuesH3D = " << valsH3D[l] << endl;
        }


        printf("* checkOutput...\n");
        for(unsigned int l=0;l<nVertex3D.size();l++)//nVertex3D.size()
        {
            for(int i=0;i<nVertex3D[l];i++)
            {
                if(module3D[l][i]->type=='0')///only for hard module
                {
                    module3D[l][i]->modW=(int)((valsW3D[l][i]+0.00001)*1000);
                    module3D[l][i]->modH=(int)((valsH3D[l][i]+0.00001)*1000);
                }
                else
                {
                    module3D[l][i]->modW=(int)((valsW3D[l][i]-0.00051)*1000);///avoid deviation(round down)
                    module3D[l][i]->modH=(int)((valsH3D[l][i]-0.00051)*1000);//-0.000001
                }
                module3D[l][i]->LeftX=(int)((valsURX3D[l][i]-valsW3D[l][i]+0.00051)*1000);///avoid deviation(round off,4-5+)
                module3D[l][i]->LeftY=(int)((valsURY3D[l][i]-valsH3D[l][i]+0.00051)*1000);
                module3D[l][i]->CenterX=module3D[l][i]->LeftX+module3D[l][i]->modW/2;
                module3D[l][i]->CenterY=module3D[l][i]->LeftY+module3D[l][i]->modH/2;

                //cout<<module3D[l][i]->idMod<<"\t";
                //printf("module3D[%3d][%3d]:\tX=%9d\tY=%9d\tW=%9d\tH=%9d\t",l,i,module3D[l][i]->LeftX,module3D[l][i]->LeftY,module3D[l][i]->modW,module3D[l][i]->modH);
                //printf("(valsURX=%.5f valsW=%.5f) ",valsURX3D[l][i],valsW3D[l][i]);
                //printf("(valsURY=%.5f valsH=%.5f)\n",valsURY3D[l][i],valsH3D[l][i]);
            }
            printf("  [%d]URX: IloMin=%.4f, IloMax=%.4f\n",l,IloMin(valsURX3D[l]),IloMax(valsURX3D[l]) );
            printf("  [%d]URY: IloMin=%.4f, IloMax=%.4f\n",l,IloMin(valsURY3D[l]),IloMax(valsURY3D[l]) );

        }




    }
    catch(IloException&e)// cathch functions, refer to "C++ try&catch"
    {
        cerr << "Concert exception caught:" << e << endl;
    }
    catch(...)
    {
        cerr << "Unknown exception caught" << endl;
    }
    env3D.end();





    double end_time=(double)(clock())/CLOCKS_PER_SEC;

    time(&rawtime);
    localtime_r(&rawtime,&ptrnow);
    int end_second=ptrnow.tm_mday*24*60*60+ptrnow.tm_hour*60*60+ptrnow.tm_min*60+ptrnow.tm_sec;
    cout<<"* CPLEX real_time: "<<end_second-start_second<<" sec"<<endl;

    printf("\033[1;36m\\========== solveCPLEX3D %.2f ==========/\033[0m\n",end_time);



    return EXIT_SUCCESS;

}

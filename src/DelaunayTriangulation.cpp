#include "DelaunayTriangulation.h"




int DT::DelaunayTriangulation(int nVertex,vector <MODULE*> module,vector < vector<int> > &matricesH,vector < vector<int> > &matricesV,int addTriangleNum)///int main(int argc, char** argv)
{
    printf("\033[1;33m/========== DelaunayTriangulation %.2f ==========\\\033[0m\n",double(clock())/CLOCKS_PER_SEC);


    printf("* Number of Vertices: %d\n",nVertex);
	if (nVertex < 3)
	{
		fprintf(stderr,"Error:vertex amount should be greater than 2, but it is %d \n",nVertex);
		exit(1);
	}


    clock_t last_time, this_time;
	last_time = clock();
    MESH mesh;
    Input_module2mesh(nVertex, module, &mesh);
	IncrementalDelaunay(&mesh);
	this_time = clock();
	printf("* Elapsed Time for Incremental Delaunay: %lg sec\n",double(this_time - last_time)/CLOCKS_PER_SEC);


    ///f///
    GRAPH graph;///constraint graph structure
    ConstraintGraph(&graph,mesh);///transform the data structure: Mesh -> pGraph
    AddOverlapEdges(&graph,module,matricesH,matricesV);///add edge for close modules
    for(int i=0;i<addTriangleNum;i++)///AGC loop
    {
        printf("* addTriangleNum: %d\n",i+1);
        NeighborTriangle(nVertex,&graph);///AGC

    }
    AllPairsShortestPaths(&graph,matricesH,matricesV);///remove redundant edges
    CoutMatrices(module,matricesH,matricesV);

    ///output
    cout<<"**Dump matlab file: graph.m"<<endl;
	//Output("./out/graph.txt", &mesh);
	OutputMatlab("./out/graph.m", &mesh);
	OutputGraphHV("./out/graph",graph,matricesH,matricesV);


    printf("\033[1;33m\\========== DelaunayTriangulation %.2f ==========/\033[0m\n",double(clock())/CLOCKS_PER_SEC);

	return 0;
}


// Algorithm IncrementalDelaunay(V)
// Input: 由n个点组成的二维点集V
// Output: Delaunay三角剖分DT
//	1.add a appropriate triangle boudingbox to contain V ( such as: we can use triangle abc, a=(0, 3M), b=(-3M,-3M), c=(3M, 0), M=Max({|x1|,|x2|,|x3|,...} U {|y1|,|y2|,|y3|,...}))
//	2.initialize DT(a,b,c) as triangle abc
//	3.for i <- 1 to n
//		do (Insert(DT(a,b,c,v1,v2,...,vi-1), vi))
//	4.remove the boundingbox and relative triangle which cotains any vertex of triangle abc from DT(a,b,c,v1,v2,...,vn) and return DT(v1,v2,...,vn).
void DT::IncrementalDelaunay(MESH_PTR pMesh)
{
	// Add a appropriate triangle boudingbox to contain V
	AddBoundingBox(pMesh);

	// Get a vertex/point vi from V and Insert(vi)
	for (int i=3; i<pMesh->vertex_num+3; i++)
	{
		Insert(pMesh, i);
	}

	// Remove the bounding box
	RemoveBoundingBox(pMesh);
}


// The format of input file should be as follows:
// The First Line: amount of vertices (the amount of vertices/points needed to be triangulated)
// Other Lines: x y z (the vertices/points coordinates, z should be 0 for 2D)
// E.g.
// 4
// -1 -1 0
// -1 1 0
// 1 1 0
// 1 -1 0
void DT::Input_module2mesh(int nVertex, vector <MODULE*> &module, MESH_PTR pMesh)
{

    printf("* Input: module to pMesh...\n");

	InitMesh(pMesh, nVertex);

	for ( int j=3; j<nVertex+3; ++j)
	{
		((VERTEX2D_PTR)(pMesh->pVerArr+j))->x = module[j-3]->GCenterX;
		((VERTEX2D_PTR)(pMesh->pVerArr+j))->y = module[j-3]->GCenterY;

        ///f///
        ((VERTEX2D_PTR)(pMesh->pVerArr+j))->index = j-3;
        ((VERTEX2D_PTR)(pMesh->pVerArr+j))->idMod = module[j-3]->idMod;

        //for check module2pMesh
		/*cout<<((VERTEX2D_PTR)(pMesh->pVerArr+j))->index<<"\t"<<((VERTEX2D_PTR)(pMesh->pVerArr+j))->x<<"\t"<<((VERTEX2D_PTR)(pMesh->pVerArr+j))->y<<"\t";
		cout<<module[j-3]->modW<<"\t"<<module[j-3]->modH<<"\n";*/

	}

}


///f///transform the data structure: Mesh -> pGraph
void DT::ConstraintGraph(GRAPH_PTR pGraph, MESH Mesh)
{

    pGraph->pVerArr = (CONSTRAINT_TYP**)malloc((Mesh.vertex_num)*sizeof(CONSTRAINT_TYP*));
    pGraph->vertex_num = Mesh.vertex_num;
	if (pGraph->pVerArr == NULL)
	{
		fprintf(stderr,"Error:Allocate memory for graph failed\n");
		exit(1);
	}

    for (int i=0; i<pGraph->vertex_num; i++)
	{
	    pGraph->pVerArr[i] = (CONSTRAINT_TYP*)malloc(1*sizeof(CONSTRAINT_TYP));
	}


    for (int i=0; i<pGraph->vertex_num; i++)
	{
        pGraph->pVerArr[i]->index=((VERTEX2D_PTR)(Mesh.pVerArr+i+3))->index;
        pGraph->pVerArr[i]->idMod=((VERTEX2D_PTR)(Mesh.pVerArr+i+3))->idMod;
        pGraph->pVerArr[i]->x=(int)((VERTEX2D_PTR)(Mesh.pVerArr+i+3))->x;
        pGraph->pVerArr[i]->y=(int)((VERTEX2D_PTR)(Mesh.pVerArr+i+3))->y;
        pGraph->pVerArr[i]->connect_num=0;
        pGraph->pVerArr[i]->pConArr=(CONSTRAINT_TYP**)malloc(0*sizeof(CONSTRAINT_TYP*));
        ///address _ node index ( x , y )
        //cout<<pGraph->pVerArr+i<<"_"<<pGraph->pVerArr[i]->index;
	    //cout<<"("<<pGraph->pVerArr[i]->x<<","<<pGraph->pVerArr[i]->y<<")"<<endl;
	}


    TRIANGLE_PTR pTri = Mesh.pTriArr;
	CONSTRAINT_PTR p1,p2,p3;


    while(pTri != NULL)
	{

        p1=pGraph->pVerArr[pTri->i1-3];
        p2=pGraph->pVerArr[pTri->i2-3];
        p3=pGraph->pVerArr[pTri->i3-3];

        bool add1,add2,add3;

        add2=true;
        add3=true;
        for(int i=0;i<p1->connect_num;i++)
        {

            if( add2 && p1->pConArr[i]->index==p2->index)
            {
                add2=false;
            }

            if( add3 && p1->pConArr[i]->index==p3->index)
            {
                add3=false;
            }

        }

        if( add2 && add3 )
        {
            p1->connect_num+=2;
            p1->pConArr=(CONSTRAINT_TYP**)realloc(p1->pConArr,(p1->connect_num)*sizeof(CONSTRAINT_TYP*));
            p1->pConArr[p1->connect_num-2]=p2;
            p1->pConArr[p1->connect_num-1]=p3;
        }
        else if ( add2 )
        {
            p1->connect_num+=1;
            p1->pConArr=(CONSTRAINT_TYP**)realloc(p1->pConArr,(p1->connect_num)*sizeof(CONSTRAINT_TYP*));
            p1->pConArr[p1->connect_num-1]=p2;
        }
        else if ( add3 )
        {
            p1->connect_num+=1;
            p1->pConArr=(CONSTRAINT_TYP**)realloc(p1->pConArr,(p1->connect_num)*sizeof(CONSTRAINT_TYP*));
            p1->pConArr[p1->connect_num-1]=p3;
        }


        add1=true;
        add3=true;
        for(int i=0;i<p2->connect_num;i++)
        {

            if( add1 && p2->pConArr[i]->index==p1->index)
            {
                add1=false;
            }

            if( add3 && p2->pConArr[i]->index==p3->index)
            {
                add3=false;
            }

        }

        if( add1 && add3 )
        {
            p2->connect_num+=2;
            p2->pConArr=(CONSTRAINT_TYP**)realloc(p2->pConArr,(p2->connect_num)*sizeof(CONSTRAINT_TYP*));
            p2->pConArr[p2->connect_num-2]=p1;
            p2->pConArr[p2->connect_num-1]=p3;
        }
        else if ( add1 )
        {
            p2->connect_num+=1;
            p2->pConArr=(CONSTRAINT_TYP**)realloc(p2->pConArr,(p2->connect_num)*sizeof(CONSTRAINT_TYP*));
            p2->pConArr[p2->connect_num-1]=p1;
        }
        else if ( add3 )
        {
            p2->connect_num+=1;
            p2->pConArr=(CONSTRAINT_TYP**)realloc(p2->pConArr,(p2->connect_num)*sizeof(CONSTRAINT_TYP*));
            p2->pConArr[p2->connect_num-1]=p3;
        }


        add1=true;
        add2=true;
        for(int i=0;i<p3->connect_num;i++)
        {

            if( add1 && p3->pConArr[i]->index==p1->index)
            {
                add1=false;
            }

            if( add2 && p3->pConArr[i]->index==p2->index)
            {
                add2=false;
            }

        }

        if( add1 && add2 )
        {
            p3->connect_num+=2;
            p3->pConArr=(CONSTRAINT_TYP**)realloc(p3->pConArr,(p3->connect_num)*sizeof(CONSTRAINT_TYP*));
            p3->pConArr[p3->connect_num-2]=p1;
            p3->pConArr[p3->connect_num-1]=p2;
        }
        else if ( add1 )
        {
            p3->connect_num+=1;
            p3->pConArr=(CONSTRAINT_TYP**)realloc(p3->pConArr,(p3->connect_num)*sizeof(CONSTRAINT_TYP*));
            p3->pConArr[p3->connect_num-1]=p1;
        }
        else if ( add2 )
        {
            p3->connect_num+=1;
            p3->pConArr=(CONSTRAINT_TYP**)realloc(p3->pConArr,(p3->connect_num)*sizeof(CONSTRAINT_TYP*));
            p3->pConArr[p3->connect_num-1]=p2;
        }


        //cout<<p1<<"_"<<p1->index<<"["<<p1->connect_num<<"]\t"<<p2->index<<"["<<p2->connect_num<<"] "<<p3->index<<"["<<p3->connect_num<<"]"<<endl;///
        //cout<<p2<<"_"<<p2->index<<"["<<p2->connect_num<<"]\t"<<p1->index<<"["<<p1->connect_num<<"] "<<p3->index<<"["<<p3->connect_num<<"]"<<endl;///
        //cout<<p3<<"_"<<p3->index<<"["<<p3->connect_num<<"]\t"<<p1->index<<"["<<p1->connect_num<<"] "<<p2->index<<"["<<p2->connect_num<<"]"<<endl;///
        /*for (int i=0; i<pGraph->vertex_num; i++)
        {
            CONSTRAINT_TYP* pCon=pGraph->pVerArr[i];
            cout<<pCon<<"_"<<pCon->index<<"["<<pCon->connect_num<<"]\t";
            for(int j=0; j<pCon->connect_num; j++)
            {
                cout<<pCon->pConArr[j]->index<<"["<<pCon->pConArr[j]->connect_num<<"] ";
            }
            cout<<"\n";
        }
        cout<<"==\n";*/


		pTri = pTri->pNext;

	}


}


///f///add edge for close modules
int DT::AddOverlapEdges(GRAPH_PTR pGraph,vector <MODULE*> module,vector < vector<int> > &matricesH,vector < vector<int> > &matricesV)
{


    double amplification=0.2;

    bool overlap=false;
    for(unsigned int i=0;i<module.size();i++)
    {


        int iLLX=module[i]->GLeftX;
        int iLLY=module[i]->GLeftY;
        int iURX=module[i]->GLeftX+module[i]->modW;
        int iURY=module[i]->GLeftY+module[i]->modH;
        //int  iID=module[i]->idMod;

        iLLX-=(int)(module[i]->modW*amplification);
        iLLY-=(int)(module[i]->modH*amplification);
        iURX+=(int)(module[i]->modW*amplification);
        iURY+=(int)(module[i]->modH*amplification);



        for(unsigned int j=0;j<module.size();j++)
        {

            overlap=false;


            int jLLX=module[j]->GLeftX;
            int jLLY=module[j]->GLeftY;
            int jURX=module[j]->GLeftX+module[j]->modW;
            int jURY=module[j]->GLeftY+module[j]->modH;
            //int  jID=module[j]->idMod;


            jLLX-=(int)(module[j]->modW*amplification);
            jLLY-=(int)(module[j]->modH*amplification);
            jURX+=(int)(module[j]->modW*amplification);
            jURY+=(int)(module[j]->modH*amplification);

            //printf("(%d)%d\t%d\t%d\t%d\t%d / %d\t%d\t%d\t%d\t%d\n",l,i,iLLX,iLLY,iURX,iURY,j,jLLX,jLLY,jURX,jURY);

            if(jLLX<iLLX && iLLX<jURX && jLLY<iLLY && iLLY<jURY)
            {
                //cout<<"overlap: iID="<<iID<<"(iLLX,iLLY) is in jID="<<jID<<endl;
                overlap=true;
            }
            if(jLLX<iURX && iURX<jURX && jLLY<iLLY && iLLY<jURY)
            {
                //cout<<"overlap: iID="<<iID<<"(iURX,iLLY) is in jID="<<jID<<endl;
                overlap=true;
            }
            if(jLLX<iURX && iURX<jURX && jLLY<iURY && iURY<jURY)
            {
                //cout<<"overlap: iID="<<iID<<"(iURX,iURY) is in jID="<<jID<<endl;
                overlap=true;
            }
            if(jLLX<iLLX && iLLX<jURX && jLLY<iURY && iURY<jURY)
            {
                //cout<<"overlap: iID="<<iID<<"(iLLX,iURY) is in jID="<<jID<<endl;
                overlap=true;
            }

            //overlap=false;

            if(overlap==true)
            {

                int distanceXi_Xj=module[i]->GCenterX-module[j]->GCenterX;
                int distanceYi_Yj=module[i]->GCenterY-module[j]->GCenterY;

                if(abs(distanceXi_Xj)>=abs(distanceYi_Yj))
                {
                    if(distanceXi_Xj>0)
                    {
                        matricesH[j][i]=1;
                    }
                    else
                    {
                        matricesH[i][j]=1;
                    }
                }
                else
                {
                    if(distanceYi_Yj>0)
                    {
                        matricesV[j][i]=1;
                    }
                    else
                    {
                        matricesV[i][j]=1;
                    }
                }

            }

        }
    }



    int addnum=0;
    for(unsigned int i=0;i<module.size();i++)
    {
        for(unsigned int j=0;j<module.size();j++)
        {
            if( matricesH[i][j]==1 || matricesV[i][j]==1)
            {

                bool AddEdgei=true;
                for(int k=0;k<pGraph->pVerArr[i]->connect_num;k++)
                {
                    if(pGraph->pVerArr[i]->pConArr[k]==pGraph->pVerArr[j] )
                    {
                        AddEdgei=false;
                        break;
                    }
                }
                if(AddEdgei==true)
                {
                    //cout<<"add"<<addnum<<":"<<i<<"-"<<j<<"\t";//
                    pGraph->pVerArr[i]->connect_num+=1;
                    pGraph->pVerArr[i]->pConArr=(CONSTRAINT_TYP**)realloc(pGraph->pVerArr[i]->pConArr,(pGraph->pVerArr[i]->connect_num+1)*sizeof(CONSTRAINT_TYP*));
                    pGraph->pVerArr[i]->pConArr[pGraph->pVerArr[i]->connect_num-1]=pGraph->pVerArr[j];
                    addnum++;
                }


                bool AddEdgej=true;
                for(int k=0;k<pGraph->pVerArr[j]->connect_num;k++)
                {
                    if(pGraph->pVerArr[j]->pConArr[k]==pGraph->pVerArr[i] )
                    {
                        AddEdgej=false;
                        break;
                    }
                }
                if(AddEdgej==true)
                {
                    //cout<<"add"<<addnum<<":"<<j<<"-"<<i<<"\n";//
                    pGraph->pVerArr[j]->connect_num+=1;
                    pGraph->pVerArr[j]->pConArr=(CONSTRAINT_TYP**)realloc(pGraph->pVerArr[j]->pConArr,(pGraph->pVerArr[j]->connect_num+1)*sizeof(CONSTRAINT_TYP*));
                    pGraph->pVerArr[j]->pConArr[pGraph->pVerArr[j]->connect_num-1]=pGraph->pVerArr[i];
                    addnum++;
                }

            }
        }
    }
    printf("* AddOverlapEdges: addnum = %d\n",addnum);


    return 0;
}


///f///AGC: add edge (for modules in neighboring triangles, we may try add a geometric relation to avoid overlap)
void DT::NeighborTriangle(int nVertex,GRAPH_PTR pGraph)
{

    CONSTRAINT_PTR Con1,Con2,Con3;

    ///AddConArr[node1][node3] = how many passing way about " node1 -> x -> node3 "(crossing a node)
    vector < map<CONSTRAINT_PTR,int> > AddConArr;
    AddConArr.resize(pGraph->vertex_num);


    for(int i=0;i<pGraph->vertex_num;i++)
    {

        Con1=pGraph->pVerArr[i];

        for(int j=0;j<Con1->connect_num;j++)
        {

            Con2=Con1->pConArr[j];


            for(int k=0;k<Con2->connect_num;k++)
            {
                Con3=Con2->pConArr[k];
                AddConArr[i][Con3]++;
            }

        }

    }


    int nOverlapVertex=0;
    int nTriangleEdge=0;
    int nAddEdge=0;
    for(int i=0;i<pGraph->vertex_num;i++)
    {
        if(pGraph->pVerArr[i]->connect_num==0)
        {
            nOverlapVertex++;
            printf("Overlap idMod=%d\n",pGraph->pVerArr[i]->idMod);//so bad
        }
        else
            nTriangleEdge+=pGraph->pVerArr[i]->connect_num;

        for(map<CONSTRAINT_PTR,int>::iterator it=AddConArr[i].begin();it!=AddConArr[i].end();it++)
        {
            bool AddEdge=true;
            for(int j=0;j<pGraph->pVerArr[i]->connect_num;j++)
            {
                ///if the node3 is already in node1.pVerArr[]
                if(it->first==pGraph->pVerArr[i] || it->first==pGraph->pVerArr[i]->pConArr[j])
                {
                    AddEdge=false;
                    break;
                }
            }


            ///if AddConArr[node1][node3]>=2, add node3 in node1.pVerArr[] ;it means added a new edge
            if(AddEdge==true && it->second>=2 )
            {
                nAddEdge++;
                pGraph->pVerArr[i]->connect_num+=1;
                pGraph->pVerArr[i]->pConArr=(CONSTRAINT_TYP**)realloc(pGraph->pVerArr[i]->pConArr,(pGraph->pVerArr[i]->connect_num+1)*sizeof(CONSTRAINT_TYP*));
                pGraph->pVerArr[i]->pConArr[pGraph->pVerArr[i]->connect_num-1]=it->first;

                //cout<<"add edge: "<<pGraph->pVerArr[i]->index<<"-"<<it->first->index<<"\n";
            }

        }

    }

    printf("  Number of Overlap Vertices = %d\n",nOverlapVertex);

    nTriangleEdge=nTriangleEdge/2;
	printf("  Number of Triangle Edges = %d\n",nTriangleEdge);

    nAddEdge=nAddEdge/2;
    printf("  Number of Triangle & Added Edges = %d\n",nTriangleEdge+nAddEdge);


}


///f///remove unnecessary edge, beause the constraint graph is transitive
///[citation] Introduction to Algorithms, Second Edition, P.630
void DT::AllPairsShortestPaths(GRAPH_PTR pGraph,vector < vector<int> > &matricesH,vector < vector<int> > &matricesV)
{

    ///the matrix[i][j] is the longest path for i -> j ; initial edge cost: exist = 1 ,null = -1 , self = 0
    /*matricesH.resize(pGraph->vertex_num);//be moved to LegalizationC.cpp
    matricesV.resize(pGraph->vertex_num);
    for(int i=0;i<pGraph->vertex_num;i++)
    {
        matricesH[i].resize(pGraph->vertex_num,-1);
        matricesV[i].resize(pGraph->vertex_num,-1);
        for(int j=0;j<pGraph->vertex_num;j++)
        {
            if(i==j)
            {
                matricesH[i][j]=0;
                matricesV[i][j]=0;
            }
        }
    }*/
    CONSTRAINT_PTR pConNow,pConNext;
    for(int i=0;i<pGraph->vertex_num;i++)
    {
        pConNow=pGraph->pVerArr[i];
        for(int j=0;j<pConNow->connect_num;j++)
        {

            pConNext=pConNow->pConArr[j];
            if( abs(pConNow->x-pConNext->x)>=abs(pConNow->y-pConNext->y) )
            {
                if(pConNext->x>=pConNow->x)
                    matricesH[pConNow->index][pConNext->index]=1;
                else
                    matricesH[pConNext->index][pConNow->index]=1;
            }
            else
            {
                if(pConNext->y>=pConNow->y)
                    matricesV[pConNow->index][pConNext->index]=1;
                else
                    matricesV[pConNext->index][pConNow->index]=1;
            }
        }

    }



    ///update only one time; because we only want to update the logest path, not textbook's shorest path
    for(int i=0;i<pGraph->vertex_num;i++)
    {
        for(int j=0;j<pGraph->vertex_num;j++)
        {
            if(matricesH[i][j]<=0&&matricesV[i][j]<=0)
                continue;

            for(int k=0;k<pGraph->vertex_num;k++)
            {
                if(matricesH[i][k]>0 && matricesH[k][j]>0)
                    matricesH[i][j]=max(matricesH[i][j],matricesH[i][k]+matricesH[k][j]);
                if(matricesV[i][k]>0 && matricesV[k][j]>0)
                    matricesV[i][j]=max(matricesH[i][j],matricesV[i][k]+matricesV[k][j]);
            }
        }




    }
    ///in the end, if matricx[i][j] = 1, these edges are we wanted



    /*int countH=0;
    int countV=0;
    for(int h=0;h<pGraph->vertex_num;h++)
    {
        for(int v=0;v<pGraph->vertex_num;v++)
        {
            if(matricesH[h][v]==1)
                countH++;//cout<<++count<<":"<<i<<" "<<j<<endl;
            if(matricesV[h][v]==1)
                countV++;//cout<<++count<<":"<<i<<" "<<j<<endl;
        }
    }
    printf("* Number of Constraint Edges = %d ( H = %d, V = %d )\n",countH+countV,countH,countV);//*/



}

///f///
int DT::CoutMatrices(vector <MODULE*> module,vector < vector<int> > matricesH,vector < vector<int> > matricesV)
{
    cout<<"* CoutMatrices:"<<endl;
    ///if matricx[i][j] = 1, these edges are we wanted
    int countH=0;
    int countV=0;


    ///cout: Number of Constraint Edges
    for(unsigned int i=0;i<matricesH.size();i++)
    {
        for(unsigned int j=0;j<matricesH[i].size();j++)
        {
            if(matricesH[i][j]==1)
            {
                countH++;
            }

        }
    }
    for(unsigned int i=0;i<matricesV.size();i++)
    {
        for(unsigned int j=0;j<matricesV[i].size();j++)
        {
            if(matricesV[i][j]==1)
            {
                countV++;
            }
        }
    }
    printf("  Number of Constraint Edges = %d ( H = %d, V = %d )\n",countH+countV,countH,countV);//*/




    ///cout all value
    /*countH=0;
    countV=0;
    for(int i=0;i<matricesH.size();i++)
    {

        cout<<"(H)i="<<module[i]->idMod<<"\t:";
        for(int j=0;j<matricesH[i].size();j++)
        {
            if(matricesH[i][j]==1)
            {
                countH++;
                printf("\033[1;33m%2d\033[0m",matricesH[i][j]);
            }
            else if(matricesH[i][j]>1)
                printf("\033[1;34m%2d\033[0m",matricesH[i][j]);
            else
                printf("\033[1;30m%2d\033[0m",matricesH[i][j]);

        }
        cout<<endl;
    }
    cout<<endl;
    getchar();
    for(int i=0;i<matricesV.size();i++)
    {

        cout<<"(V)i="<<module[i]->idMod<<"\t:";
        for(int j=0;j<matricesV[i].size();j++)
        {
            if(matricesV[i][j]==1)
            {
                countV++;
                printf("\033[1;33m%2d\033[0m",matricesV[i][j]);
            }
            else if(matricesV[i][j]>1)
                printf("\033[1;34m%2d\033[0m",matricesV[i][j]);
            else
                printf("\033[1;30m%2d\033[0m",matricesV[i][j]);
        }
        cout<<endl;
    }
    printf("* Number of Constraint Edges = %d ( H = %d, V = %d )\n",countH+countV,countH,countV);//*/


    return 0;
}


// The format of output file should be as follows:
// triangle index
// x1 y1 (the coordinate of first vertex of triangle)
// x2 y2 (the coordinate of second vertex of triangle)
// x3 y3 (the coordinate of third vertex of triangle)
void DT::Output(char* pFile, MESH_PTR pMesh)
{
	FILE* fp = fopen(pFile, "w");
	if (!fp)
	{
		fprintf(stderr,"Error:%s open failed\n", pFile);

		UnInitMesh(pMesh);
		exit(1);
	}


	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int* pi;
	int vertex_index;
	int tri_index = 0;

	if(pTri == NULL)///f///
	{
	    fprintf(fp, "Triangle: 0\n");
	}

	while(pTri != NULL)
	{
		fprintf(fp, "Triangle: %d\n", ++tri_index);

		pi = &(pTri->i1);

		for (int j=0; j<3; j++)
		{
			vertex_index = *pi++;
			fprintf(fp, "%lg %lg\n", ((VERTEX2D_PTR)(pMesh->pVerArr+vertex_index))->x, ((VERTEX2D_PTR)(pMesh->pVerArr+vertex_index))->y);
		}

		pTri = pTri->pNext;
	}

	fclose(fp);

	//UnInitMesh(pMesh);
}



///f///modify Output()
void DT::OutputMatlab(char* pFile, MESH_PTR pMesh)
{
	FILE* fp = fopen(pFile, "w");
	if (!fp)
	{
		fprintf(stderr,"Error:%s open failed\n", pFile);

		UnInitMesh(pMesh);
		exit(1);
	}


	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int* pi;
	int vertex_index;
	//int tri_index = 0;


    ///f///
    fprintf(fp, "figure;\n");
	fprintf(fp, "clear;\n");
	fprintf(fp, "clf;\n");
	fprintf(fp, "axis equal;\n");


    vector < pair<double,double> > xy;///f///
    xy.resize(3);

	while(pTri != NULL)
	{
		//fprintf(fp, "Triangle: %d\n", ++tri_index);
        pair<double,double> tmpxy;///f///

		pi = &(pTri->i1);
		for (int j=0; j<3; j++)
		{
			vertex_index = *pi++;

			//fprintf(fp, "%lg %lg\n", ((VERTEX2D_PTR)(pMesh->pVerArr+vertex_index))->x, ((VERTEX2D_PTR)(pMesh->pVerArr+vertex_index))->y);

            ///f///
			tmpxy.first = ((VERTEX2D_PTR)(pMesh->pVerArr+vertex_index))->x;
			tmpxy.second = ((VERTEX2D_PTR)(pMesh->pVerArr+vertex_index))->y;
			xy[j] = tmpxy;

		}

        ///f///
        fprintf(fp, "line([%lf %lf],[%lf %lf]",xy[0].first,xy[1].first,xy[0].second,xy[1].second);
        if( abs(xy[0].first-xy[1].first)> abs(xy[0].second-xy[1].second) )
            fprintf(fp, ",'Color','b');\n");
        else
            fprintf(fp, ",'Color','b');\n");
        fprintf(fp, "line([%lf %lf],[%lf %lf]",xy[1].first,xy[2].first,xy[1].second,xy[2].second);
        if( abs(xy[1].first-xy[2].first)> abs(xy[1].second-xy[2].second) )
            fprintf(fp, ",'Color','b');\n");
        else
            fprintf(fp, ",'Color','b');\n");
        fprintf(fp, "line([%lf %lf],[%lf %lf]",xy[2].first,xy[0].first,xy[2].second,xy[0].second);
        if( abs(xy[2].first-xy[0].first)> abs(xy[2].second-xy[0].second) )
            fprintf(fp, ",'Color','b');\n");
        else
            fprintf(fp, ",'Color','b');\n");
        //fprintf(fp, "line([%lf %lf],[%lf %lf]);\n",xy[0].first,xy[1].first,xy[0].second,xy[1].second);
        //fprintf(fp, "line([%lf %lf],[%lf %lf]);\n",xy[1].first,xy[2].first,xy[1].second,xy[2].second);
        //fprintf(fp, "line([%lf %lf],[%lf %lf]);\n",xy[2].first,xy[0].first,xy[2].second,xy[0].second);

		pTri = pTri->pNext;
	}


	for (int i=3; i<pMesh->vertex_num+3; i++)
	{
		//cout<< "(" << ((VERTEX2D_PTR)(pMesh->pVerArr+i))->x << "," << ((VERTEX2D_PTR)(pMesh->pVerArr+i))->y << ")" << endl;
		fprintf(fp, "text(%lf,%lf,'%d','fontsize',20);\n",((VERTEX2D_PTR)(pMesh->pVerArr+i))->x,((VERTEX2D_PTR)(pMesh->pVerArr+i))->y,((VERTEX2D_PTR)(pMesh->pVerArr+i))->idMod);
	}


	fclose(fp);

	//UnInitMesh(pMesh);
}



///f///
void DT::OutputGraphHV(char* pFile,GRAPH pGraph,vector < vector<int> > matricesH,vector < vector<int> > matricesV)
{
	string File(pFile);
	string FileH(pFile);
	string FileV(pFile);
	string FileHV(pFile);///all path
	string FileHH(pFile);
	string FileVV(pFile);
	string FileHHVV(pFile);///shortest path

	File  =File+"0.m";
	FileH =FileH+"1_H.m";
	FileV =FileV+"2_V.m";
	FileHV=FileHV+"3_HV.m";
	FileHH=FileHH+"4_HH.m";
	FileVV=FileVV+"5_VV.m";
	FileHHVV=FileHHVV+"6_HHVV.m";

    FILE* fp   = fopen(File.c_str(), "w");
	FILE* fpH  = fopen(FileH.c_str(), "w");
	FILE* fpV  = fopen(FileV.c_str(), "w");
	FILE* fpHV = fopen(FileHV.c_str(), "w");
	FILE* fpHH = fopen(FileHH.c_str(), "w");
	FILE* fpVV = fopen(FileVV.c_str(), "w");
	FILE* fpHHVV = fopen(FileHHVV.c_str(), "w");



	if (!fp||!fpHV||!fpH||!fpV||!fpHH||!fpVV||!fpHHVV)
	{
		fprintf(stderr,"Error:%s open failed\n", pFile);
		exit(1);
	}

    fprintf(fp, "figure;\n");
	fprintf(fp, "clear;\n");
	fprintf(fp, "clf;\n");
	fprintf(fp, "axis equal;\n");
    fprintf(fpH, "figure;\n");
	fprintf(fpH, "clear;\n");
	fprintf(fpH, "clf;\n");
	fprintf(fpH, "axis equal;\n");
    fprintf(fpV, "figure;\n");
	fprintf(fpV, "clear;\n");
	fprintf(fpV, "clf;\n");
	fprintf(fpV, "axis equal;\n");
    fprintf(fpHV, "figure;\n");
	fprintf(fpHV, "clear;\n");
	fprintf(fpHV, "clf;\n");
	fprintf(fpHV, "axis equal;\n");
    fprintf(fpHH, "figure;\n");
	fprintf(fpHH, "clear;\n");
	fprintf(fpHH, "clf;\n");
	fprintf(fpHH, "axis equal;\n");
    fprintf(fpVV, "figure;\n");
	fprintf(fpVV, "clear;\n");
	fprintf(fpVV, "clf;\n");
	fprintf(fpVV, "axis equal;\n");
    fprintf(fpHHVV, "figure;\n");
	fprintf(fpHHVV, "clear;\n");
	fprintf(fpHHVV, "clf;\n");
	fprintf(fpHHVV, "axis equal;\n");

	int ax(0),ay(0),bx(0),by(0);
	for(int i=0;i<pGraph.vertex_num;i++)
	{


        ax=pGraph.pVerArr[i]->x;
        ay=pGraph.pVerArr[i]->y;
	    for(int j=0;j<pGraph.pVerArr[i]->connect_num;j++)
	    {
            bx=pGraph.pVerArr[i]->pConArr[j]->x;
            by=pGraph.pVerArr[i]->pConArr[j]->y;

            if( abs(ax-bx)>=abs(ay-by) )
            {
                fprintf(fp, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fp, ",'Color','k');\n");
                fprintf(fpH, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpH, ",'Color','r');\n");
                fprintf(fpHV, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpHV, ",'Color','r');\n");

            }
            else
            {
                fprintf(fp, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fp, ",'Color','k');\n");
                fprintf(fpV, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpV, ",'Color','b');\n");
                fprintf(fpHV, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpHV, ",'Color','b');\n");
            }


	    }
	    fprintf(fp,   "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);
		fprintf(fpH,  "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);
		fprintf(fpV,  "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);
		fprintf(fpHV, "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);
        fprintf(fpHH, "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);
        fprintf(fpVV, "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);
        fprintf(fpHHVV, "text(%d,%d,'%d','fontsize',20);\n",ax,ay,pGraph.pVerArr[i]->idMod);

	}


	for(int i=0;i<pGraph.vertex_num;i++)
	{
	    for(int j=0;j<pGraph.vertex_num;j++)
	    {

	        if(matricesH[i][j]==1)
	        {
	            for(int k=0;k<pGraph.vertex_num;k++)
	            {
	                if(pGraph.pVerArr[k]->index==i)
	                {
	                    ax=pGraph.pVerArr[k]->x;
	                    ay=pGraph.pVerArr[k]->y;
	                }
	                if(pGraph.pVerArr[k]->index==j)
	                {
	                    bx=pGraph.pVerArr[k]->x;
	                    by=pGraph.pVerArr[k]->y;
	                }
	            }
                fprintf(fpHH, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpHH, ",'Color','r');\n");
                fprintf(fpHHVV, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpHHVV, ",'Color','r');\n");
	        }
	        if(matricesV[i][j]==1)
	        {
	            for(int k=0;k<pGraph.vertex_num;k++)
	            {
	                if(pGraph.pVerArr[k]->index==i)
	                {
	                    ax=pGraph.pVerArr[k]->x;
	                    ay=pGraph.pVerArr[k]->y;
	                }
	                if(pGraph.pVerArr[k]->index==j)
	                {
	                    bx=pGraph.pVerArr[k]->x;
	                    by=pGraph.pVerArr[k]->y;
	                }
	            }
                fprintf(fpVV, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpVV, ",'Color','b');\n");
                fprintf(fpHHVV, "line([%d %d],[%d %d]",ax,bx,ay,by);
                fprintf(fpHHVV, ",'Color','b');\n");
	        }
	    }
	}

    fclose(fp);
	fclose(fpH);
	fclose(fpV);
	fclose(fpHV);
    fclose(fpHH);
    fclose(fpVV);
    fclose(fpHHVV);

}



///======================== following is unmodified source code ========================


// Allocate memory to store vertices and triangles
void DT::InitMesh(MESH_PTR pMesh, int ver_num )
{
	// Allocate memory for vertex array
	pMesh->pVerArr = (VERTEX2D_PTR)malloc((ver_num+3)*sizeof(VERTEX2D));
	if (pMesh->pVerArr == NULL)
	{
		fprintf(stderr,"Error:Allocate memory for mesh failed\n");
		exit(1);
	}

	pMesh->vertex_num = ver_num;

}

// Deallocate memory
void DT::UnInitMesh(MESH_PTR pMesh)
{
	// free vertices
	if(pMesh->pVerArr != NULL)
		free(pMesh->pVerArr);

	// free triangles
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	TRIANGLE_PTR pTemp = NULL;
	while (pTri != NULL)
	{
		pTemp = pTri->pNext;
		free(pTri);
		pTri = pTemp;
	}
}

void DT::AddBoundingBox(MESH_PTR pMesh)
{
	REAL max = 0;
	REAL max_x = 0;
	REAL max_y = 0;
	REAL t;

	for (int i=3; i<pMesh->vertex_num+3; i++)
	{
		t = abs(((VERTEX2D_PTR)(pMesh->pVerArr+i))->x);
		if (max_x < t)
		{
			max_x = t;
		}

		t = abs(((VERTEX2D_PTR)(pMesh->pVerArr+i))->y);
		if (max_y < t)
		{
			max_y = t;
		}
	}

	max = max_x > max_y ? max_x:max_y;

	//TRIANGLE box;
	//box.v1 = VERTEX2D(0, 3*max);
	//box.v2 = VERTEX2D(-3*max, 3*max);
	//box.v3 = VERTEX2D(3*max, 0);

	VERTEX2D v1 = {0, 4*max};
	VERTEX2D v2 = {-4*max, -4*max};
	VERTEX2D v3 = {4*max, 0};

	// Assign to Vertex array
	*(pMesh->pVerArr) = v1;
	*(pMesh->pVerArr + 1) = v2;
	*(pMesh->pVerArr + 2) = v3;

	// add the Triangle boundingbox
	AddTriangleNode(pMesh, NULL, 0, 1, 2);
}

void DT::RemoveBoundingBox(MESH_PTR pMesh)
{
	int statify[3]={0,0,0};
	int vertex_index;
	int* pi;
	//int k = 1;

	// Remove the first triangle-boundingbox
	//pMesh->pTriArr = pMesh->pTriArr->pNext;
	//pMesh->pTriArr->pPrev = NULL; // as head

	TRIANGLE_PTR pTri = pMesh->pTriArr;
	TRIANGLE_PTR pNext = NULL;
	while (pTri != NULL)
	{
		pNext = pTri->pNext;

		statify[0] = 0;
		statify[1] = 0;
		statify[2] = 0;

		pi = &(pTri->i1);
		for (int j=0, k = 1; j<3; j++, k*= 2)
		{
			vertex_index = *pi++;

			if(vertex_index == 0 || vertex_index == 1 || vertex_index == 2) // bounding box vertex
			{
				statify[j] = k;
			}
		}

		switch(statify[0] | statify[1] | statify[2] )
		{
		case 0: // no statify
			break;
		case 1:
		case 2:
		case 4: // 1 statify, remove 1 triangle, 1 vertex
			RemoveTriangleNode(pMesh, pTri);
			break;
		case 3:
		case 5:
		case 6: // 2 statify, remove 1 triangle, 2 vertices
			RemoveTriangleNode(pMesh, pTri);
			break;
		case 7: // 3 statify, remove 1 triangle, 3 vertices
			RemoveTriangleNode(pMesh, pTri);
			break;
		default:
			break;
		}

		// go to next item
		pTri = pNext;
	}
}


// Return a positive value if the points pa, pb, and
// pc occur in counterclockwise order; a negative
// value if they occur in clockwise order; and zero
// if they are collinear. The result is also a rough
// approximation of twice the signed area of the
// triangle defined by the three points.
REAL DT::CounterClockWise(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pc)
{
	return ((pb->x - pa->x)*(pc->y - pb->y) - (pc->x - pb->x)*(pb->y - pa->y));
}

// Adjust if the point lies in the triangle abc
REAL DT::InTriangle(MESH_PTR pMesh, VERTEX2D_PTR pVer, TRIANGLE_PTR pTri)
{
	int vertex_index;
	VERTEX2D_PTR pV1, pV2, pV3;

	vertex_index =pTri->i1;
	pV1 = (VERTEX2D_PTR)(pMesh->pVerArr+vertex_index);
	vertex_index =pTri->i2;
	pV2 = (VERTEX2D_PTR)(pMesh->pVerArr+vertex_index);
	vertex_index =pTri->i3;
	pV3 = (VERTEX2D_PTR)(pMesh->pVerArr+vertex_index);
	REAL ccw1 = CounterClockWise(pV1, pV2, pVer);
	REAL ccw2 = CounterClockWise(pV2, pV3, pVer);
	REAL ccw3 = CounterClockWise(pV3, pV1, pVer);

	REAL r = -1;
	if (ccw1>0 && ccw2>0 && ccw3>0)
	{
		r = 1;
	}
	else if(ccw1*ccw2*ccw3 == 0 && (ccw1*ccw2 > 0 || ccw1*ccw3 > 0 || ccw2*ccw3 > 0) )
	{
		r = 0;
	}

	return r;
}

// Algorithm Insert(DT(a,b,c,v1,v2,...,vi-1), vi)
// 1.find the triangle vavbvc which contains vi // FindTriangle()
// 2.if (vi located at the interior of vavbvc)
// 3.    then add triangle vavbvi, vbvcvi and vcvavi into DT // UpdateDT()
// FlipTest(DT, va, vb, vi)
// FlipTest(DT, vb, vc, vi)
// FlipTest(DT, vc, va, vi)
// 4.else if (vi located at one edge (E.g. edge vavb) of vavbvc)
// 5.    then add triangle vavivc, vivbvc, vavdvi and vivdvb into DT (here, d is the third vertex of triangle which contains edge vavb) // UpdateDT()
// FlipTest(DT, va, vd, vi)
// FlipTest(DT, vc, va, vi)
// FlipTest(DT, vd, vb, vi)
// FlipTest(DT, vb, vc, vi)
// 6.return DT(a,b,c,v1,v2,...,vi)
void DT::Insert(MESH_PTR pMesh, int ver_index)
{
	VERTEX2D_PTR pVer = (VERTEX2D_PTR)(pMesh->pVerArr+ver_index);
	TRIANGLE_PTR pTargetTri = NULL;
	TRIANGLE_PTR pEqualTri1 = NULL;
	TRIANGLE_PTR pEqualTri2 = NULL;

	int j = 0;
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	while (pTri != NULL)
	{
		REAL r = InTriangle(pMesh, pVer, pTri);
		if(r > 0) // should be in triangle
		{
			pTargetTri = pTri;
		}
		else if (r == 0) // should be on edge
		{
			if(j == 0)
			{
				pEqualTri1 = pTri;
				j++;
			}
			else
			{
				pEqualTri2 = pTri;
			}

		}

		pTri = pTri->pNext;
	}

	if (pEqualTri1 != NULL && pEqualTri2 != NULL)
	{
		InsertOnEdge(pMesh, pEqualTri1, ver_index);
		InsertOnEdge(pMesh, pEqualTri2, ver_index);
	}
	else
	{
		InsertInTriangle(pMesh, pTargetTri, ver_index);
	}
}

void DT::InsertInTriangle(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index)
{
	int index_a, index_b, index_c;
	TRIANGLE_PTR pTri = NULL;
	TRIANGLE_PTR pNewTri = NULL;

	pTri = pTargetTri;
	if(pTri == NULL)
	{
		return;
	}

	// Inset p into target triangle
	index_a = pTri->i1;
	index_b = pTri->i2;
	index_c = pTri->i3;

	// Insert edge pa, pb, pc
	for(int i=0; i<3; i++)
	{
		// allocate memory
		if(i == 0)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_a, index_b, ver_index);
		}
		else if(i == 1)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_b, index_c, ver_index);
		}
		else
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_c, index_a, ver_index);
		}

		// go to next item
		if (pNewTri != NULL)
		{
			pTri = pNewTri;
		}
		else
		{
			pTri = pTri;
		}
	}

	// Get the three sub-triangles
	pTri = pTargetTri;
	TRIANGLE_PTR pTestTri[3];
	for (int i=0; i< 3; i++)
	{
		pTestTri[i] = pTri->pNext;

		pTri = pTri->pNext;
	}

	// remove the Target Triangle
	RemoveTriangleNode(pMesh, pTargetTri);

	for (int i=0; i< 3; i++)
	{
		// Flip test
		FlipTest(pMesh, pTestTri[i]);
	}
}

void DT::InsertOnEdge(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index)
{
	int index_a, index_b, index_c;
	TRIANGLE_PTR pTri = NULL;
	TRIANGLE_PTR pNewTri = NULL;

	pTri = pTargetTri;
	if(pTri == NULL)
	{
		return;
	}

	// Inset p into target triangle
	index_a = pTri->i1;
	index_b = pTri->i2;
	index_c = pTri->i3;

	// Insert edge pa, pb, pc
	for(int i=0; i<3; i++)
	{
		// allocate memory
		if(i == 0)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_a, index_b, ver_index);
		}
		else if(i == 1)
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_b, index_c, ver_index);
		}
		else
		{
			pNewTri = AddTriangleNode(pMesh, pTri, index_c, index_a, ver_index);
		}

		// go to next item
		if (pNewTri != NULL)
		{
			pTri = pNewTri;
		}
		else
		{
			pTri = pTri;
		}
	}

	// Get the two sub-triangles
	pTri = pTargetTri;
	TRIANGLE_PTR pTestTri[2];
	for (int i=0; i< 2; i++)
	{
		pTestTri[i] = pTri->pNext;
		pTri = pTri->pNext;
	}

	// remove the Target Triangle
	RemoveTriangleNode(pMesh, pTargetTri);

	for (int i=0; i< 2; i++)
	{
		// Flip test
		FlipTest(pMesh, pTestTri[i]);
	}
}

// Precondition: the triangle satisfies CCW order
// Algorithm FlipTest(DT(a,b,c,v1,v2,...,vi), va, vb, vi)
// 1.find the third vertex (vd) of triangle which contains edge vavb // FindThirdVertex()
// 2.if(vi is in circumcircle of abd)  // InCircle()
// 3.    then remove edge vavb, add new edge vivd into DT // UpdateDT()
//		  FlipTest(DT, va, vd, vi)
//		  FlipTest(DT, vd, vb, vi)

bool DT::FlipTest(MESH_PTR pMesh, TRIANGLE_PTR pTestTri)
{
	bool flipped = false;

	int index_a = pTestTri->i1;
	int index_b = pTestTri->i2;
	int index_p = pTestTri->i3;

	int statify[3]={0,0,0};
	int vertex_index;
	int* pi;
	//int k = 1;

	// find the triangle which has edge consists of start and end
	TRIANGLE_PTR pTri = pMesh->pTriArr;

	int index_d = -1;
	while (pTri != NULL)
	{
		statify[0] = 0;
		statify[1] = 0;
		statify[2] = 0;

		pi = &(pTri->i1);
		for (int j=0, k = 1; j<3; j++, k*= 2)
		{
			vertex_index = *pi++;
			if(vertex_index == index_a || vertex_index == index_b)
			{
				statify[j] = k;
			}
		}

		switch(statify[0] | statify[1] | statify[2] )
		{
			case 3:
				if(CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr+index_a), (VERTEX2D_PTR)(pMesh->pVerArr+index_b), (VERTEX2D_PTR)(pMesh->pVerArr+pTri->i3)) < 0)
				{
					index_d = pTri->i3;
				}

				break;
			case 5:
				if(CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr+index_a), (VERTEX2D_PTR)(pMesh->pVerArr+index_b), (VERTEX2D_PTR)(pMesh->pVerArr+pTri->i2)) < 0)
				{
					index_d = pTri->i2;
				}

				break;
			case 6:
				if(CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr+index_a), (VERTEX2D_PTR)(pMesh->pVerArr+index_b), (VERTEX2D_PTR)(pMesh->pVerArr+pTri->i1)) < 0)
				{
					index_d = pTri->i1;
				}

				break;

			default:
				break;
		}

		if (index_d != -1)
		{
			VERTEX2D_PTR pa = (VERTEX2D_PTR)(pMesh->pVerArr+index_a);
			VERTEX2D_PTR pb = (VERTEX2D_PTR)(pMesh->pVerArr+index_b);
			VERTEX2D_PTR pd = (VERTEX2D_PTR)(pMesh->pVerArr+index_d);
			VERTEX2D_PTR pp = (VERTEX2D_PTR)(pMesh->pVerArr+index_p);

			if(InCircle( pa, pb, pp, pd) < 0) // not local Delaunay
			{
				flipped = true;

				// add new triangle adp,  dbp, remove abp, abd.
				// allocate memory for adp
				TRIANGLE_PTR pT1 = AddTriangleNode(pMesh, pTestTri, pTestTri->i1, index_d, pTestTri->i3);
				// allocate memory for dbp
				TRIANGLE_PTR pT2 = AddTriangleNode(pMesh, pT1, index_d, pTestTri->i2, index_p);
				// remove abp
				RemoveTriangleNode(pMesh, pTestTri);
				// remove abd
				RemoveTriangleNode(pMesh, pTri);

				FlipTest(pMesh, pT1); // pNewTestTri satisfies CCW order
				FlipTest(pMesh, pT2); // pNewTestTri2  satisfies CCW order

				break;
			}
		}

		// go to next item
		pTri = pTri->pNext;
	}

	return flipped;
}

// In circle test, use vector cross product
REAL DT::InCircle(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pp, VERTEX2D_PTR  pd)
{
	REAL det;
	REAL alift, blift, plift, bdxpdy, pdxbdy, pdxady, adxpdy, adxbdy, bdxady;

	REAL adx = pa->x - pd->x;
	REAL ady = pa->y - pd->y;

	REAL bdx = pb->x - pd->x;
	REAL bdy = pb->y - pd->y;

	REAL pdx = pp->x - pd->x;
	REAL pdy = pp->y - pd->y;

	bdxpdy = bdx * pdy;
	pdxbdy = pdx * bdy;
	alift = adx * adx + ady * ady;

	pdxady = pdx * ady;
	adxpdy = adx * pdy;
	blift = bdx * bdx + bdy * bdy;

	adxbdy = adx * bdy;
	bdxady = bdx * ady;
	plift = pdx * pdx + pdy * pdy;

	det = alift * (bdxpdy - pdxbdy)
		+ blift * (pdxady - adxpdy)
		+ plift * (adxbdy - bdxady);

	return -det;
}

// Remove a node from the triangle list and deallocate the memory
void DT::RemoveTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pTri)
{
	if (pTri == NULL)
	{
		return;
	}

	// remove from the triangle list
	if (pTri->pPrev != NULL)
	{
		pTri->pPrev->pNext = pTri->pNext;
	}
	else // remove the head, need to reset the root node
	{
		pMesh->pTriArr = pTri->pNext;
	}

	if (pTri->pNext != NULL)
	{
		pTri->pNext->pPrev = pTri->pPrev;
	}

	// deallocate memory
	free(pTri);
}

// Create a new node and add it into triangle list
DT::TRIANGLE_PTR DT::AddTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pPrevTri, int i1, int i2, int i3)
{
	// test if 3 vertices are co-linear
	if(CounterClockWise((VERTEX2D_PTR)(pMesh->pVerArr+i1), (VERTEX2D_PTR)(pMesh->pVerArr+i2), (VERTEX2D_PTR)(pMesh->pVerArr+i3)) == 0)
	{
		return NULL;
	}

	// allocate memory
	TRIANGLE_PTR pNewTestTri = (TRIANGLE_PTR)malloc(sizeof(TRIANGLE));

	pNewTestTri->i1 = i1;
	pNewTestTri->i2 = i2;
	pNewTestTri->i3 = i3;

	// insert after prev triangle
	if (pPrevTri == NULL) // add root
	{
		pMesh->pTriArr = pNewTestTri;
		pNewTestTri->pNext = NULL;
		pNewTestTri->pPrev = NULL;
	}
	else
	{
		pNewTestTri->pNext = pPrevTri->pNext;
		pNewTestTri->pPrev = pPrevTri;

		if(pPrevTri->pNext != NULL)
		{
			pPrevTri->pNext->pPrev = pNewTestTri;
		}

		pPrevTri->pNext = pNewTestTri;
	}

	return pNewTestTri;
}


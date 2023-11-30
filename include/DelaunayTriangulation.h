

/*!
 * \file 	DelannayTriangliation.h
 * \brief 	Triangliation(header)
 * \TODO: The procedure uses Double List for holding data, it can be optimized by using another data structure such as DAG, Quad-edge, etc. 
 * \author 	SoRoMan
 * \version	2.0 (3-D)
 * \date	2018.07/
 */

#ifndef _DELAUNAYTRIANGULATION_H_
#define _DELAUNAYTRIANGULATION_H_

#include "dataType.h"

// DEFINES ////////////////////////////////////////////////
#define MAX_VERTEX_NUM 4092

#ifdef SINGLE///no define SINGLE
#define REAL float
#else
#define REAL double
#endif

class DT
{
	protected:

		typedef struct VERTEX2D_TYP
		{
			REAL x;
			REAL y;

			///only can add in the end of the struct ( because ex:VERTEX2D v1 = {0, 4*max} )
			int index;
			int idMod;

		} VERTEX2D, *VERTEX2D_PTR;

		typedef struct EDGE_TYP
		{
			VERTEX2D v1;
			VERTEX2D v2;

		} EDGE, *EDGE_PTR;

		typedef struct TRIANGLE_TYP
		{
			int i1; // vertex index
			int i2;
			int i3;

			TRIANGLE_TYP* pNext;
			TRIANGLE_TYP* pPrev;

		} TRIANGLE, *TRIANGLE_PTR;

		typedef struct MESH_TYP
		{
			int vertex_num;
			int triangle_num;

			VERTEX2D_PTR pVerArr; // point to outer vertices arrary
			TRIANGLE_PTR pTriArr; // point to outer triangles arrary

		} MESH, *MESH_PTR;

		typedef struct CONSTRAINT_TYP
		{
			int x;
			int y;
			int index;
			int idMod;

			int connect_num;
			CONSTRAINT_TYP** pConArr;

		} CONSTRAINT, *CONSTRAINT_PTR;
		typedef struct GRAPH_TYP
		{
			int vertex_num;
			CONSTRAINT_TYP** pVerArr; // point to outer vertices arrary

		} GRAPH, *GRAPH_PTR;//*/
	public:

		int DelaunayTriangulation(int nVertex,vector <MODULE*> module,vector < vector<int> > &matricesH,vector < vector<int> > &matricesV,int addTriangleNum);

		// Delaunay triangulation functions
		void InitMesh(MESH_PTR pMesh, int ver_num);
		void UnInitMesh(MESH_PTR pMesh);

		void AddBoundingBox(MESH_PTR pMesh);
		void RemoveBoundingBox(MESH_PTR pMesh);;
		void IncrementalDelaunay(MESH_PTR pMesh);

		void Insert(MESH_PTR pMesh, int ver_index);
		bool FlipTest(MESH_PTR pMesh, TRIANGLE_PTR pTestTri);

		REAL InCircle(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pp, VERTEX2D_PTR  pd);
		REAL InTriangle(MESH_PTR pMesh, VERTEX2D_PTR pVer, TRIANGLE_PTR pTri);
		REAL CounterClockWise(VERTEX2D_PTR pa, VERTEX2D_PTR pb, VERTEX2D_PTR pc);

		void InsertInTriangle(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index);
		void InsertOnEdge(MESH_PTR pMesh, TRIANGLE_PTR pTargetTri, int ver_index);

		// Helper functions
		void RemoveTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pTri);
		TRIANGLE_PTR AddTriangleNode(MESH_PTR pMesh, TRIANGLE_PTR pPrevTri, int i1, int i2, int i3);

		// I/O functions
		//void Input(char* pFile, int nVertex, vector <MODULE*> &module, MESH_PTR pMesh);///no used
		void Input_module2mesh(int nVertex, vector <MODULE*> &module, MESH_PTR pMesh);
		void Output(char* pFile, MESH_PTR pMesh);

		///f///
		void ConstraintGraph(GRAPH_PTR pGraph, MESH Mesh);
		int AddOverlapEdges(GRAPH_PTR pGraph,vector <MODULE*> module,vector < vector<int> > &matricesH,vector < vector<int> > &matricesV);
		void NeighborTriangle(int nVertex,GRAPH_PTR pGraph);
		void AllPairsShortestPaths(GRAPH_PTR pGraph,vector < vector<int> > &matricesH,vector < vector<int> > &matricesV);
		int CoutMatrices(vector <MODULE*> module,vector < vector<int> > matricesH,vector < vector<int> > matricesV);
		void OutputMatlab(char* pFile, MESH_PTR pMesh);
		void OutputGraphHV(char* pFile,GRAPH pGraph,vector < vector<int> > matricesH,vector < vector<int> > matricesV);
};

#endif


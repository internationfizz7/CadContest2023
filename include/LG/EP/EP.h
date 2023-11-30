

/*!
 * \file 	EP.h
 * \brief 	File In & Out (header)
 *
 * \author 	 SEDA Lab 
 * \version	2.0 (2-D)
 * \date	2018.07/
 */

#ifndef _EP_H_
#define _EP_H_

#include "dataType.h"
#include "gp.h"




class EPLG : public GP
{
	protected:


		class GROUP
		{
			//friend ostream& operator<<(ostream &, const GROUP &);
			public:
				int idClu;				///< cluster id
				int nMod;				///< number of modules in cluster
				int nTSV;				///< number of TSVs in cluster
				vector<int> idMod;		///< modules & TSVs in cluster

				int modArea;			///< area of total modules in cluster
				int boxArea;			///< area of bounding box of the cluster
				int lbX;				///< left-bottom x-coor
				int lbY;				///< left-bottom y-coor
				int rtX;				///< right-top x-coor
				int rtY;				///< right-top y-coor
				int centerX;			///< center x-coor
				int centerY;			///< center y-coor

				int maxModW;			///< maximum width of modules
				int maxModH;			///< maximum height of modules
				int minModL;			///< minimum length of modules
				int avgModL;			///< average length of modules

				GROUP() : idClu(0), nMod(0), nTSV(0), modArea(0), boxArea(0), lbX(0), lbY(0), rtX(0), rtY(0), centerX(0), centerY(0), maxModW(0), maxModH(0), minModL(MAX_INT), avgModL(0)
			{ }

				~GROUP()
				{ }

				GROUP& operator=(const GROUP &cluster)
				{
					if(&cluster != this)
					{
						idClu = cluster.idClu;
						nMod = cluster.nMod;
						nTSV = cluster.nTSV;
						idMod.reserve( cluster.idMod.size() );
						for(unsigned int i = 0; i < cluster.idMod.size(); i++)
						{
							idMod[i] = cluster.idMod[i];
						}
						modArea = cluster.modArea;
						boxArea = cluster.boxArea;
						lbX = cluster.lbX;
						lbY = cluster.lbY;
						rtX = cluster.rtX;
						rtY = cluster.rtY;
						centerX = cluster.centerX;
						centerY = cluster.centerY;
						maxModW = cluster.maxModW;
						maxModH = cluster.maxModH;
						minModL = cluster.minModL;
						avgModL = cluster.avgModL;
					}

					return *this;
				}
		};

		/// \brief class ORDER \n
		/// order is the element of table for enumeration in EP \n
		class ORDER
		{
			public:
				int id;		///< order id
				int value;	///< order value

				ORDER() : id(0), value(0)
			{ }

				~ORDER()
				{ }

				bool operator<(const ORDER &order)
				{
					return value < order.value;
				}
		};

		/// \brief class TREE
		class TREE
		{
			//friend ostream& operator<<(ostream &, const TREE &);
			public:
				unsigned short id;		///< tree node id
				unsigned short level;	///< tree level (root: level = 0)

				/// cut direction \n
				/// V (vertically cut & horizontally merge) \n
				/// H (horizontally cut & vertically merge) \n
				/// O (bigoplus) \n
				char VHcut;

				int treeCoordX;			///< tree node x-coor
				int treeCoordY;			///< tree node y-coor
				int lbX;				///< left-bottom x-coor
				int lbY;				///< left-bottom y-coor
				int rtX;				///< right-top x-coor
				int rtY;				///< right-top y-coor

				TREE *parentTree;		///< parent tree node
				TREE *leftTree;			///< left (or bottom) tree node
				TREE *rightTree;		///< right (or top) tree node

				int nVertex;			///< number of vertices in the tree node
				vector<int> vArray;		///< vertices array storing index of modules & TSVs

				CURVE shapeCurveN;		///< shapeCurveN of the tree node

				/// flag to mark whether child modules have been swapped, \n
				/// rather than changing the actual left or right tree/curve structure \n
				/// in the swapping process
				char flagSwap;

				char flagLeaf;			///< flag to mark the leaf node

				TREE(): id(0), level(0), VHcut('O'), treeCoordX(0), treeCoordY(0), lbX(0), lbY(0), rtX(0), rtY(0), parentTree(NULL), leftTree(NULL), rightTree(NULL), nVertex(0), flagSwap('0'), flagLeaf('0')
			{ }

				~TREE()
				{ }

				TREE& operator=(const TREE &tree)
				{
					if(&tree != this)
					{
						id = tree.id;
						level = tree.level;
						VHcut = tree.VHcut;
						treeCoordX = tree.treeCoordX;
						treeCoordY = tree.treeCoordY;
						lbX = tree.lbX;
						lbY = tree.lbY;
						rtX = tree.rtX;
						rtY = tree.rtY;
						parentTree = tree.parentTree;
						leftTree = tree.leftTree;
						rightTree = tree.rightTree;
						nVertex = tree.nVertex;
						vArray.reserve(nVertex);
						for(int i = 0; i < nVertex; i++)
						{
							vArray[i] = tree.vArray[i];
						}
						shapeCurveN = tree.shapeCurveN;
						flagSwap = tree.flagSwap;
						flagLeaf = tree.flagLeaf;
					}

					return *this;
				}
		};

		/// \brief class SubTREE \n
		/// SubTREE is the tree node for the base case (tree node of EP) \n
		class SubTREE
		{
			//friend ostream& operator<<(ostream &, const SubTREE &);
			public:
				int id;					///< subtree id

				SubTREE *parentTree;	///< parent subtree node
				SubTREE *leftTree;		///< left subtree node
				SubTREE *rightTree;		///< right subtree node

				/// it points to the corresponding shapeCurveN \n
				/// for the top node in the base tree structure \n
				/// if(nodeCurve != NULL), it points to shapeCurveN \n
				CURVE *nodeCurve;

				CURVE *leftCurve;		///< left curve
				CURVE *rightCurve;		///< right curve

				/// the curve which belongs to the subtree \n
				/// if(selfCurve != NULL), it points to buffCurve \n
				CURVE *selfCurve;

				unsigned short nVertex;				///< number of modules in the base case
				unsigned short vArray[MAX_BASE];	///< vertices array

				int coordX;				///< center x-coor in the sub-floorplan module
				int coordY;				///< center y-coor in the sub-floorplan module

				/// flag to mark whether child modules have been swapped, \n
				/// rather than changing the actual left or right tree/curve structure \n
				/// in the swapping process
				char flagSwap;

				SubTREE(): id(0), parentTree(NULL), leftTree(NULL), rightTree(NULL), nodeCurve(NULL), leftCurve(NULL), rightCurve(NULL), selfCurve(NULL), nVertex(0), coordX(0), coordY(0), flagSwap('0')
			{
				for(int i = 0; i < MAX_BASE; i++)
				{
					vArray[i] = 0;
				}
			}

				~SubTREE()
				{ }


		};

		/// \brief class NODE_SLOT
		class NODE_SLOT
		{
			//	friend ostream& operator<<(ostream &, const NODE_SLOT &);
			public:
				int id;	///< slot id
				int x;	///< center x-coor
				int y;	///< center y-coor

				NODE_SLOT():id(-1), x(0), y(0)
			{ }

				~NODE_SLOT()
				{ }
		};

		/// \brief class NBBOX \m
		/// net bounding box (network-flow)
		class NBBOX
		{
			//	friend ostream& operator<<(ostream &, const NBBOX &);
			public:
				int llx;	///< lower-left x-coor
				int lly;	///< lower_left y-coor
				int urx;	///< upper_right x-coor
				int ury;	///< upper_right y-coor

				NBBOX(): llx(MAX_INT), lly(MAX_INT), urx(0), ury(0)
			{ }

				~NBBOX()
				{ }
		};

		/// \brief class NODE_TSV
		/// NODE_TSV is the TSV which has to be placed (network-flow)
		class NODE_TSV
		{
			//	friend ostream& operator<<(ostream &, const NODE_TSV &);
			public:
				int corr_id;	///< the corresponding id of TSV

				/// 1) single TSV (in layer k) is connected by two nets \n
				///    which are net in layer k and net in layer k+1 \n
				/// 2) these tow nets will form two net bounding boxes \n
				//     which are netbbox_lower and netbbox_upper
				NBBOX netbbox_lower;
				NBBOX netbbox_upper;	

				/// if netbbox_lower is composed of TSV in layer k & TSV in layer k+1, \n
				/// has_nbbox_lower = false; otherwise, true.
				bool has_nbbox_lower;
				bool has_nbbox_upper;

				vector<int> corr_slots;

				NODE_TSV(): corr_id(-1), has_nbbox_lower(false), has_nbbox_upper(false)
			{ }

				~NODE_TSV()
				{ }
		};


		/// < Grouping

		class CUTSET
		{
			public:
				int idSet;		///< cutset ID
				int boundPM;	///< the upper bound of the number of pseudo modules
				int boundTSV;	///< the upper bound of the number of TSVs in a pseudo modules

				vector<int> cutV;	///< vertical cutlines
				vector<int> cutH;	///< horizontal cutlines

				CUTSET()
				{
					idSet = 0;
					boundPM = 0;
					boundTSV = 0;
				}
				~CUTSET() {}
		};

		clock_t start_t_LG, end_t_LG;
		CURVE tempCurve[5];		///< temporary curves
		typedef vector<NETLIST> SUBNETLIST;
		SUBNETLIST sub_nt;

		int mode;				///< problem mode
		int numBase;				///< the maximum number of vertices in each cluster
		int numLayer;			///< the number of layers
		bool flagFinalResult;	///< flag to mark whether the final result is obtained
		bool flagModSoft;		///< flag to mark whether soft modules over 80%
		
		char *sub_benName;		///< sub benchmark namespace
		float soft_ratio;		///< parameter for exchanging hard mod to soft mod
		double para_pow;			///< parameter for adjusting weight of cut line

		double time_LA;			///< time of executing layer assignment
		double time_GP;			///< time of executing global distribution
		double time_LG;			///< time of executing legalization
		double time_TA;			///< time of executing TSV assignment

		double wirelength_GP;	///< wirelength after global distribution
		double wirelength_LG;	///< wirelength after legalization
		double wirelength_TA;	///< wirelength after TSV assignment




		//---- cluster info ----//
		vector<GROUP> clusters;

		//---- tree info ----//
		TREE **pointerRGST;		///< restricted generalized slicing tree pointer
		TREE *RGST;				///< restricted generalized slicing tree
		unsigned short numTree;	///< number of tree nodes in the RGST

		/// number of various subcircuit less than MAX_BASE vertices
		unsigned short numPart[MAX_BASE];


		//---- enumerative packing (EP) ----//
		/// subCurve used to store merging results from different tree structure of subcircuit
		CURVE *subCurve;
		int numSCurve;				///< number of subCurve
		int offsetSCurve;			///< offset of subCurve

		/// temporary curve used in the process of generating the curve of modules, \n
		/// for merging different kinds of subtree structure which has the same number of nodes \n
		CURVE *buffCurve;
		int numBCurve;				///< number of buffCurve
		int indexBCurve;				///< index of buffCurve
		/// for tree node containing less than or equal to MAX_BASE vertices except two vertices case
		/// used in the traceBack process to record the tree structure
		SubTREE *subTree;
		int numSTree;				///< number of subtree
		int indexSTree;				///< index of subtree

		//---- Bin Para ----//

		BIN2D bin2D;
	public:

		void EPlegal(NETLIST & tmpnt);
		void CreateSubNetlist( NETLIST &tmpnt );
		void SetModsInChip( NETLIST &tmpnt );

		bool Legalization( NETLIST &sub_nt, int shConst, char *plotFile, vector<NODE_TSV> *sub_nodeTSV, vector<NODE_SLOT> *sub_nodeSLOT );
		

		void CreateModCurve( NETLIST &tmpnt, int shConst );

		void ConstructRGST( NETLIST &tmpnt, TREE *Tree );

		void ReorderRGST( TREE *Tree, TREE **pointerTree );



		///< Grouping Fcn.


		/**
		 * clustering
		 * ---------------------------------------------------------------
		 * @param[in]		plotFile		the name of plotfile
		 * @param[in]		ori_tmpnt		original netlist (w/o clustering TSVs)
		 * @param[out]		resultPM		the result of clustering TSVs
		 * @param[out]		pm_tmpnt		new netlist (w clustering TSVs)
		 */
		void Grouping( vector<PSEUDO_MODULE> &resultPM, NETLIST &pm_tmpnt, NETLIST &ori_tmpnt, char *plotFile );

		/**
		 * generate the first cluster
		 * ---------------------------------------------------------------
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	firstC		the first cluster
		 */
		void InitialCluster( GROUP &firstC, NETLIST &tmpnt );

		/**
		 * recursively bipartition cluster
		 * ---------------------------------------------------------------
		 * @param[in]       tempC       clusters for bipartitioning
		 * @param[in]       tmpnt       original netlist (w/o clustering TSVs)
		 * @param[out]      resultC     clusters meeting the constraint
		 */
		void BipartitionC( deque<GROUP> &tempC, vector<GROUP> &resultC, NETLIST &tmpnt );

		/**
		 * select vertical cutline with minimum weight
		 * ---------------------------------------------------------------
		 * @param[in]       currentC    clusters for bipartitioning
		 * @param[in]       tmpnt       original netlist (w/o clustering TSVs)
		 * @param[out]      lchildC     left child cluster
		 * @param[out]      rchildC     right child cluster
		 * @return			the direction of cutline (V)
		 */
		char SelectCutV( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt );

		/**
		 * select horizontal cutline with minimum weight
		 * ---------------------------------------------------------------
		 * @param[in]       currentC    clusters for bipartitioning
		 * @param[in]       tmpnt       original netlist (w/o clustering TSVs)
		 * @param[out]      lchildC     bottom cluster
		 * @param[out]      rchildC     top cluster
		 * @return 			the direction of cutline (H)
		 *
		 */
		char SelectCutH( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt );

		/**
		 * select cutline with minimum weight
		 * ---------------------------------------------------------------
		 * @param[in]       currentC    clusters for bipartitioning
		 * @param[in]       tmpnt       original netlist (w/o clustering TSVs)
		 * @param[out]      lchildC     left/bottom cluster
		 * @param[out]      rchildC     right/top cluster
		 * @return			the direction of cutline (H or V)
		 */
		char SelectCutHV( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt );

		/**
		 * adjust mods in left/bottom cluster & right/top cluster (averaging modArea)
		 * ---------------------------------------------------------------
		 * @param[in]       currentC    clusters for bipartitioning
		 * @param[in]       tmpnt       original netlist (w/o clustering TSVs)
		 * @param[in]       VHcut       cut direction
		 * @param[out]      lchildC     left/bottom cluster
		 * @param[out]      rchildC     right/top cluster
		 */
		void AdjustC( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, char VHcut, NETLIST &tmpnt );

		/**
		 * construct tree
		 * ---------------------------------------------------------------
		 * @param[in]       currentC    clusters for bipartitioning
		 * @param[in]       tmpnt       original netlist (w/o clustering TSVs)
		 * @param[in]       VHcut       cut direction
		 * @param[out]      lchildC     left/bottom cluster
		 * @param[out]      rchildC     right/top cluster
		 */
		void ConstructTree( GROUP &currentC, GROUP &lchildC, GROUP &rchildC, char VHcut, NETLIST &tmpnt );

		/**
		 * redistribute modules in the cluster
		 * ---------------------------------------------------------------
		 * @param[in]       c           clusters for redistributing
		 * @param[in, out]  tmpnt       original netlist (w/o clustering TSVs)
		 */
		void RedistributeC( GROUP &c, NETLIST &tmpnt );

		/**
		 * cut bin for the cluster
		 * ---------------------------------------------------------------
		 * @param[in]       c           clusters for redistributing
		 */
		void CutBinC( GROUP &c );

		/**
		 * calculate density gradient & total bin density for the cluster
		 * ---------------------------------------------------------------
		 * @param[in]       c           clusters for redistributing
		 * @param[in, out]  tmpnt       original netlist (w/o clustering TSVs)
		 */
		double CalDensityGradientC( GROUP &c, NETLIST &tmpnt );

		/**
		 * initialize gradient for the cluster
		 * ---------------------------------------------------------------
		 * @param[in]       c           clusters for redistributing
		 * @param[in, out]  tmpnt       original netlist (w/o clustering TSVs)
		 */
		void InitializeGradientC( GROUP &c, NETLIST &tmpnt );

		/**
		 * calculate new coordinate for each module in the cluster
		 * ---------------------------------------------------------------
		 * @param[in]       c           clusters for redistributing
		 * @param[in, out]  tmpnt       original netlist (w/o clustering TSVs)
		 */
		void SolveCoordinateC( GROUP &c, NETLIST &tmpnt );

		/**
		 * update child cluster
		 * ---------------------------------------------------------------
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	lchildC		left child cluster
		 * @param[in, out]	rchildC		right child cluster
		 */
		void UpdateClusterChild( GROUP &lchildC, GROUP &rchildC, NETLIST &tmpnt );

		/**
		 * initial pseudo modules
		 * ---------------------------------------------------------------
		 * @param[in]		resultC		the result of clustering
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	tempPM		pseudo modules for bipartitioning
		 * @param[in, out]	tempCS		cutset
		 */
		void InitialPseudoMod( deque<PSEUDO_MODULE> &tempPM, vector<CUTSET> &tempCS, vector<GROUP> &resultC, NETLIST &tmpnt );

		/**
		 * recursively bipartition pseudo modules
		 * ---------------------------------------------------------------
		 * @param[in]		tempCS		cutset
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	tempPM		pseudo modules for bipartitioning
		 * @param[in, out]	resultPM	the result of bipartitioning pseudo modules
		 */
		void BipartitionPM( deque<PSEUDO_MODULE> &tempPM, vector<PSEUDO_MODULE> &resultPM, vector<CUTSET> &tempCS, NETLIST &tmpnt );

		/**
		 * calculate weights of vertical cutlines
		 * ---------------------------------------------------------------
		 * @param[in]	lengthV		length from x-coor of TSV to vertical cutline
		 * @param[in]	cutlineV	vertical cutline
		 * @param[in]	currentPM	pseudo module for bipartitioning
		 * @param[in]	tmpnt		original netlist
		 * @return 		the sum of left & right standard deviations
		 */
		float CalCutWeightV_PM( int *lengthV, int &cutlineV, PSEUDO_MODULE &currentPM, NETLIST &tmpnt );

		/**
		 * calculate weights of horizontal cutlines
		 * ---------------------------------------------------------------
		 * @param[in]	lengthH		length from y-coor of TSV to horizontal cutline
		 * @param[in]	cutlineH	horizontal cutline
		 * @param[in]	currentPM	pseudo module for bipartitioning
		 * @param[in]	tmpnt		original netlist
		 * @return 		the sum of left & right standard deviation
		 */
		float CalCutWeightH_PM( int *lengthH, int &cutlineH, PSEUDO_MODULE &currentPM, NETLIST &tmpnt );

		/**
		 * update child pseudo modules
		 * ---------------------------------------------------------------
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	lchildPM	left child pseudo module
		 * @param[in, out]	rchildPM	right child pseudo module
		 */
		void UpdatePModChild( PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, NETLIST &tmpnt );

		/**
		 * select vertical cutline with minimum weight (pseudo module)
		 * ---------------------------------------------------------------
		 * @param[in]		currentPM	pseudo module for bipartitioning
		 * @param[in]		cutset		two sets of cutlines (V & H)
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	lchildPM	left child pseudo module
		 * @param[in, out]	rchildPM	right child pseudo module
		 * @return			whether the vertical cutline had found; true: found, false: not found
		 */
		bool SelectCutV_PM( PSEUDO_MODULE &currentPM, PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, CUTSET &cutset, NETLIST &tmpnt, char &VHcut );

		/**
		 * select horizontal cutline with minimum weight (pseudo module)
		 * ---------------------------------------------------------------
		 * @param[in]		currentPM	pseudo module for bipartitioning
		 * @param[in]		cutset		two sets of cutlines (V & H)
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	lchildPM	left child pseudo module
		 * @param[in, out]	rchildPM	right child pseudo module
		 * @return			whether the horizontal cutline had found; true: found, false: not found
		 */
		bool SelectCutH_PM( PSEUDO_MODULE &currentPM, PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, CUTSET &cutset, NETLIST &tmpnt, char &VHcut );

		/**
		 * select cutline with minimum weight (pseudo module)
		 * ---------------------------------------------------------------
		 * @param[in]		currentPM	pseudo module for bipartitioning
		 * @param[in]		cutset		two sets of cutlines (V & H)
		 * @param[in]		tmpnt		original netlist
		 * @param[in, out]	VHcut		the direction of cutline
		 * @param[in, out]	lchildPM	left child pseudo module
		 * @param[in, out]	rchildPM	right child pseudo module
		 * @return			whether the had found; true: found, false: not found
		 */
		bool SelectCutHV_PM( PSEUDO_MODULE &currentPM, PSEUDO_MODULE &lchildPM, PSEUDO_MODULE &rchildPM, CUTSET &cutset, NETLIST &tmpnt, char &VHcut );

		/**
		 * create netlist with pseudo modules
		 * ---------------------------------------------------------------
		 * @param[in]		ori_tmpnt	original netlist (w/o clustering TSVs)
		 * @param[in]		resultPM	the result of clustering TSVs
		 * @param[in, out]	pm_tmpnt	new netlist with pseudo modules
		 */
		void CreatePModNetlist( NETLIST &pm_tmpnt, NETLIST &ori_tmpnt, vector<PSEUDO_MODULE> &resultPM );

		/**
		 * trace back RGST to update vArray
		 * ---------------------------------------------------------------
		 * @param[in]	leaf	leaf node
		 * @param[in]	trace	trace node
		 */
		void TraceRGST( TREE *leaf, TREE *trace );

		/**
		 * update vArray based on the result of clustering with original netlist
		 * ---------------------------------------------------------------
		 * @param[in]	ori_tmpnt	original netlist (w/o clustering TSVs)
		 * @param[in]	resultC		the result of clustering
		 */
		void UpdateVArray( NETLIST &ori_tmpnt, vector<GROUP> &resultC );

		/**
		 * update vArray based on the result of clustering with new netlist
		 * ---------------------------------------------------------------
		 * @param[in]	pm_tmpnt	new netlist with pseudo modules
		 * @param[in]	ori_tmpnt	original netlist (w/o clustering TSVs)
		 * @param[in]	resultPM	the result of clustering TSVs
		 * @param[in]	resultC		the result of clustering
		 */
		void UpdateVArray( NETLIST &pm_tmpnt, NETLIST &ori_tmpnt, vector<PSEUDO_MODULE> &resultPM, vector<GROUP> &resultC );

		void RefineSinglePMod( vector<PSEUDO_MODULE> &resultPM );

		void RefineSinglePMod( vector<PSEUDO_MODULE> &resultPM, NETLIST &tmpnt );

		void PlotResultPM( vector<GROUP> &resultC, vector<PSEUDO_MODULE> &resultPM, NETLIST &tmpnt, char *file );
		void PlotResultC( vector<GROUP> &resultC, NETLIST &tmpnt, char *file );


	   // enumerative packing 
	   // Generate the shapeCurve of the tree node containing only one Vertex
		void CreateCurveN1(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing two Vertex
		void CreateCurveN2(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing three Vertex
		void CreateCurveN3(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing four Vertex
		void CreateCurveN4(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing five Vertex
		void CreateCurveN5(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing six Vertex
		void CreateCurveN6(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing seven Vertex
		void CreateCurveN7(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing eight Vertex
		void CreateCurveN8(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing nine Vertex
		void CreateCurveN9(NETLIST &nt, TREE *t);

		// Generate the shapeCurve of the tree node containing ten Vertex
		void CreateCurveN10(NETLIST &nt, TREE *t);

		// Generate the subCurve two Vertex
		void GenBasicCurve2(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve three Vertex
		void GenBasicCurve3(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve four Vertex
		void GenBasicCurve4(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve five Vertex
		void GenBasicCurve5(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve six Vertex
		void GenBasicCurve6(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve seven Vertex
		void GenBasicCurve7(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve eight Vertex
		void GenBasicCurve8(NETLIST &nt, ORDER *leaf);

		// Generate the subCurve nine Vertex
		void GenBasicCurve9(NETLIST &nt, ORDER *leaf);

		///< Opertion for Curve


		// Allocate curve memory before curve operation
		void AllocCurveMem(CURVE *c, int n);

		// Combine cA and cB into cC without pruning
		void CombineCurve(CURVE *cA, CURVE *cB, CURVE *cC);

		// Merge two trees shapeCurveN
		void MergeTreeCurveN(TREE *tree);

		// Extend the curve based on W=H, so that upper-half curve is recovered
		void ExtendCurve(CURVE *cA, CURVE *cC);

		// Add curve horizontally
		void AddCurveH(CURVE *cA, CURVE *cB, CURVE *cC);

		// Add curve horizontally (NCKU)
		void NCKUAddCurveH(CURVE *cA, CURVE *cB, CURVE *cC);

		// Add curve vertically
		void AddCurveV(CURVE *cA, CURVE *cB, CURVE *cC);

		// Add curve vertically (NCKU)
		void NCKUAddCurveV(CURVE *cA , CURVE *cB , CURVE *cC);

		// Add curve horizontally or vertically
		void AddCurveO(CURVE *cA, CURVE *cB, int nA, int nB, CURVE *cC );

		// Merge the curve_H and curve_V in the curve combining operation
		void DeFerMergeCurveHV(CURVE *cA, CURVE *cC, int nPointL, int nPointR);

		// Merge the basic buffCurves in EP
		void MergeCurveB(CURVE *cA, CURVE *cB, CURVE *cC);

		// Use the prune parameter(beta)/WAP for pruning
		// void PrunePointsBeta(CURVE *c);
		void ShapeCurveN_test( CURVE *c);

		///< Order Table.
		  void Choose2in3(ORDER *array, ORDER(*result)[3]);
		  void Choose2in4(ORDER *array, ORDER(*result)[4]);
		  void Choose2in5(ORDER *array, ORDER(*result)[5]);
		  void Choose2in6(ORDER *array, ORDER(*result)[6]);
		  void Choose2in7(ORDER *array, ORDER(*result)[7]);
		  void Choose2in8(ORDER *array, ORDER(*result)[8]);
		  void Choose2in9(ORDER *array, ORDER(*result)[9]);
		  void Choose2in10(ORDER *array, ORDER(*result)[10]);

		// ===== 3 ===== (#2681)
		  void Choose3in4(ORDER *array, ORDER(*result)[4]);
		  void Choose3in6(ORDER *array, ORDER(*result)[6]);
		  void Choose3in7(ORDER *array, ORDER(*result)[7]);
		  void Choose3in8(ORDER *array, ORDER(*result)[8]);
		  void Choose3in9(ORDER *array, ORDER(*result)[9]);
		  void Choose3in10(ORDER *array, ORDER(*result)[10]);

		// ===== 4 ===== (#8274)
		  void Choose4in5(ORDER *array, ORDER(*result)[5]);
		  void Choose4in8(ORDER *array, ORDER(*result)[8]);
		  void Choose4in9(ORDER *array, ORDER(*result)[9]);
		  void Choose4in10(ORDER *array, ORDER(*result)[10]);

		// ===== 5 ===== (#15932)
		  void Choose5in6(ORDER *array, ORDER(*result)[6]);
		  void Choose5in10(ORDER *array, ORDER(*result)[10]);

		// ===== 6 ===== (#21052)
		  void Choose6in7(ORDER *array, ORDER(*result)[7]);

		// ===== 7 ===== (#21155)
		  void Choose7in8(ORDER *array, ORDER(*result)[8]);

		// ===== 8 ===== (#21288)
		  void Choose8in9(ORDER *array, ORDER(*result)[9]);

		// ===== 9 ===== (#21455)
		  void Choose9in10(ORDER *array, ORDER(*result)[10]);

		  ///< Trace Back Fcn.

		  bool FindValidPoint( NETLIST &tmpnt, TREE *Tree, TREE **TreeP, unsigned short &numTree );

		  /**
		   * trace back valid points to find the coordinate of modules & TSVs
		   * ---------------------------------------------------------------
		   * @brief           trace back valid points to find the coordinate of modules & TSVs
		   */
		  void TraceBack( int tIndex, NETLIST &tmpnt, TREE *Tree, TREE **TreeP, unsigned short &numTree );

		  /**
		   * trace the points from parent curve to child curve (NCKU)
		   * ---------------------------------------------------------------
		   * @brief           trace the points from parent curve to child curve (NCKU)
		   */
		  void NCKUTracePoint(TREE *t, CURVE *c);

		  /**
		   * trace the points from parent curve to child curve
		   * ---------------------------------------------------------------
		   * @brief           trace the points from parent curve to child curve
		   */
		  void TracePoint(CURVE *c);

		  /**
		   * trace the shape curve produced by EP
		   * ---------------------------------------------------------------
		   * @brief           trace the shape curve produced by EP
		   */
		  void TraceBase(CURVE *c);

		  /**
		   * transfer the trace information between the merged curve and the trace buffCurve
		   * ---------------------------------------------------------------
		   * @brief			transfer the trace information between the merged curve and the trace buffCurve
		   */
		  void TraceMergedCurve(CURVE *mergedCurve, int *tracedBCurveID);

		///< OVERLAP Fcn.

		void OptimizeWL(NETLIST &tmpnt, TREE *Tree, TREE **TreeP, unsigned short &numTree);
		void PackTreeNode(NETLIST &tmpnt, TREE *Tree, TREE **TreeP, int indexP);
		void RoughSwapBlock(NETLIST &tmpnt, TREE *Tree, TREE **TreeP, int indexP);
		void RoughSwapBase(NETLIST &tmpnt, TREE *Tree, unsigned short &numTree, SubTREE *t);
		void DetailedSwapBase(NETLIST &tmpnt, TREE *Tree, unsigned short &numTree, SubTREE *t);
		void GetDirect(CURVE *c, char *direct);
		void Get2Dim(CURVE *c, int *width, int *height, char *direct);
		void GetBuffNetWL(NETLIST &tmpnt, int nNet, double *newWL, char resetFlag);

		// Find Overlap
		void FindOverlap(NETLIST &tmpnt);

		// Find Overlap of Global Distribution
		void FindOverlap_GP(NETLIST &tmpnt);

		// Find Violate
		void FindViolate(NETLIST &tmpnt);

		void CalculateHPWL_FINAL( NETLIST &tmpnt );

		// Pack

		void PackToLeftSub( vector<MODULE*> &modArray, int bound, int &newRightBound );
		void PackToBottomSub( vector<MODULE*> &modArray, int bound, int &newTopBound );
		void PackToCenter( int &left, int &right, int &bottom, int &top, int xCenter, int yCenter, NETLIST &tmpnt );
		void PackToRight( vector<MODULE*> &array, int bound, int &newLeftBound );
		void PackToLeft( vector<MODULE*> &array, int bound, int &newRightBound );
		void PackToTop( vector<MODULE*> &array, int bound, int &newBottomBound );
		void PackToBottom( vector<MODULE*> &array, int bound, int &newTopBound );
		void GreedyMoveToLeft( vector<MODULE*> &modArray, vector<int> &coordArray, int &newLeftBound, NETLIST &tmpnt );
		void GreedyMoveToRight( vector<MODULE*> &modArray, vector<int> &coordArray, int &newRightBound, NETLIST &tmpnt );
		void GreedyMoveToBottom( vector<MODULE*> &modArray, vector<int> &coordArray, int &newBottomBound, NETLIST &tmpnt );
		void GreedyMoveToTop( vector<MODULE*> &modArray, vector<int> &coordArray, int &newTopBound, NETLIST &tmpnt);
		void GetNetBoxX( int i, int index, int &left, int &right, NETLIST &tmpnt );
		void GetNetBoxY( int i, int index, int &bottom, int &top, NETLIST &tmpnt );
		bool CheckOverlapX( MODULE* a, MODULE* b );
		bool CheckOverlapY( MODULE* a, MODULE* b );

		// TSV

		class TSV
		{
			private:
				void UpdateNodeTSV( vector<NODE_TSV> &sub_nodeTSV, NETLIST &sub_tmpnt, NETLIST &tmpnt );
				void  SolveMinCostFlow( vector<NODE_TSV> &sub_nodeTSV, vector<NODE_SLOT> &sub_nodeSLOT, NETLIST &sub_tmpnt, NETLIST &tmpnt );
				void ComputeArcCost( int &cost, NBBOX &nbbox, NODE_SLOT &slot );
				bool FindNetBoundingBox( int corr_id, int net_id, NBBOX &nbbox, NETLIST &tmpnt );
				clock_t start_t_TA, end_t_TA;
			public:

			void FindAdditionalSlots( NETLIST &sub_tmpnt, vector<NODE_SLOT> &sub_nodeSLOT );


			void AssignTSV( vector< vector<NODE_TSV> > &node_TSV, vector< vector<NODE_SLOT> > &node_SLOT, SUBNETLIST &sub_tmpnt, NETLIST &nt );
		};

};


#endif


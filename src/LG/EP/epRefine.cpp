#include "EP.h"

class OVERLAP
{
	friend ostream& operator<<(ostream &output, const OVERLAP &o);
	public:

		int id1;
		int id2;
		int X1R, X1L;
		int X2R, X2L;
		int Y1T, Y1B;
		int Y2T, Y2B;
		int ORLX; // ORLX = max{min{X1R-X2L,X2R-X1L},0}
int ORLY; // ORLY = max{min{Y1T-Y2B,Y2T-Y1B},0}
int ORL;  // ORL = ORLX + ORLY
double OA;   // OA = ORLX * ORLY
void CalcORL(void);   // Calculate ORL
void CalcOA(void);    // Calculate Overlap Area
OVERLAP(): id1(0), id2(0), X1R(0), X1L(0), X2R(0), X2L(0), Y1T(0), Y1B(0), Y2T(0), Y2B(0), ORLX(0), ORLY(0), ORL(0)
	{ }

~OVERLAP()
{ }
};

ostream& operator<<(ostream &output, const OVERLAP &o)
{
	output << "module " << o.id1 << " & " << o.id2 << endl;
	output << "ORLX = " << o.ORLX/((float)AMP_PARA) << " ORLY = " << o.ORLY/((float)AMP_PARA) << endl;
	output << "ORL = " << o.ORL/((float)AMP_PARA) << endl;
	return output;
}

vector<OVERLAP> overLap;
vector<OVERLAP> overLap_GP;

void EPLG:: OptimizeWL(NETLIST &tmpnt, TREE *Tree, TREE **TreeP, unsigned short &numTree)
{
	char tmp;
	TreeP[0]->treeCoordX = 0;
	TreeP[0]->treeCoordY = 0;

	PackTreeNode(tmpnt, Tree, TreeP, 0);
	for(int i=1; i<numTree; i++)
	{
		if((TreeP[i]->leftTree!=NULL) && (TreeP[i]->rightTree!=NULL))
		{
			PackTreeNode(tmpnt, Tree, TreeP, i);
		}
		else if(TreeP[i]->vArray.size() == 2)
		{
			RoughSwapBlock(tmpnt, Tree, TreeP, i);
		}
		else if(TreeP[i]->vArray.size() == 1)
		{
			Get2Dim(&TreeP[i]->shapeCurveN, &tmpnt.mods[TreeP[i]->vArray[0]].modW, &tmpnt.mods[TreeP[i]->vArray[0]].modH, &tmp);
			tmpnt.mods[TreeP[i]->vArray[0]].LeftX = TreeP[i]->treeCoordX;
			tmpnt.mods[TreeP[i]->vArray[0]].LeftY = TreeP[i]->treeCoordY;
		}
	}
	// Swap the subTree for the base cases
	for(int i=0; i<numSTree; i++)
	{
		RoughSwapBase(tmpnt, Tree, numTree, &subTree[i]);
	}
	// Top-Down
	int loop = 1;
	for(int i=0; i<loop; i++)
	{
		for(int i=0; i<numSTree; i++)
		{
			DetailedSwapBase(tmpnt, Tree, numTree, &subTree[i]);
		}
	}
	return;
}

void EPLG:: PackTreeNode(NETLIST &tmpnt, TREE* Tree, TREE** TreeP, int indexP)
{
	char tmp;
	int indexL;
	int indexR;
	int indexB;
	int widthL;
	int heightL;
	int widthR;
	int heightR;
	int centerLX;
	int centerLY;
	int centerRX;
	int centerRY;
	//int newtraceIndex;
	//int newnPoint;
	int index = TreeP[indexP]->id;
	//determine the Tree is be merged in O-plus or HV
	Get2Dim(Tree[index].shapeCurveN.leftCurve, &widthL, &heightL, &tmp);
	Get2Dim(Tree[index].shapeCurveN.rightCurve, &widthR, &heightR, &tmp);

	// Determine whether to swap

	/* cout << "////////////index = " << index <<"  ///////////////////"<< endl;
	   cout << "VHcut : "<< Tree[index].VHcut<<endl;
	   cout << "widthL = " << widthL << " heightL = " << heightL << endl;
	   cout << "widthR = " << widthR << " heightR = " << heightR << endl;
	 */
	indexL = Tree[index].leftTree->id;
	indexR = Tree[index].rightTree->id;
	// cout << "indexL = " << indexL << " indexR = " << indexR << endl;
	Tree[indexL].treeCoordX = Tree[index].treeCoordX;
	Tree[indexL].treeCoordY = Tree[index].treeCoordY;
	// swap the index into EP
	if(Tree[index].VHcut == 'H')
	{
		Tree[indexR].treeCoordX = Tree[indexL].treeCoordX;
		Tree[indexR].treeCoordY = Tree[indexL].treeCoordY + heightL;
	}
	else if(Tree[index].VHcut == 'V')// H
	{
		Tree[indexR].treeCoordX = Tree[indexL].treeCoordX + widthL;
		Tree[indexR].treeCoordY = Tree[indexL].treeCoordY;
	}
	else if(Tree[index].VHcut == 'O') // O
	{
		GetDirect(&(Tree[index].shapeCurveN), &tmp);
		if(tmp==0)
		{
			Tree[indexR].treeCoordX = Tree[indexL].treeCoordX + widthL;
			Tree[indexR].treeCoordY = Tree[indexL].treeCoordY;
		}
		else
		{
			Tree[indexR].treeCoordX = Tree[indexL].treeCoordX;
			Tree[indexR].treeCoordY = Tree[indexL].treeCoordY + heightL;
		}
	}

	centerLX = Tree[indexL].treeCoordX + widthL / 2;
	centerLY = Tree[indexL].treeCoordY + heightL / 2;
	centerRX = Tree[indexR].treeCoordX + widthR / 2;
	centerRY = Tree[indexR].treeCoordY + heightR / 2;

	for(int i=0; i<(int)Tree[indexL].vArray.size(); i++)
	{
		indexB = Tree[indexL].vArray[i];
		tmpnt.mods[indexB].CenterX = centerLX;
		tmpnt.mods[indexB].CenterY = centerLY;
	}
	for(int i=0; i<(int)Tree[indexR].vArray.size(); i++)
	{
		indexB = Tree[indexR].vArray[i];
		tmpnt.mods[indexB].CenterX = centerRX;
		tmpnt.mods[indexB].CenterY = centerRY;
	}
	return;
}

void EPLG:: RoughSwapBlock(NETLIST &tmpnt, TREE* Tree, TREE** TreeP, int indexP)
{
	char tmp;
	int widthL;
	int heightL;
	int widthR;
	int heightR;

	int index = TreeP[indexP]->id;
	int indexL = TreeP[indexP]->shapeCurveN.leftCurve->id;
	int indexR = TreeP[indexP]->shapeCurveN.rightCurve->id;

	Get2Dim(Tree[index].shapeCurveN.leftCurve, &widthL, &heightL, &tmp);
	Get2Dim(Tree[index].shapeCurveN.rightCurve, &widthR, &heightR, &tmp);

	tmpnt.mods[indexL].modW = widthL;
	tmpnt.mods[indexL].modH = heightL;
	tmpnt.mods[indexR].modW = widthR;
	tmpnt.mods[indexR].modH = heightR;

	tmpnt.mods[indexL].LeftX = Tree[index].treeCoordX;
	tmpnt.mods[indexL].LeftY = Tree[index].treeCoordY;

	if(Tree[index].VHcut == 'O') // O
	{
		GetDirect(&(Tree[index].shapeCurveN), &tmp);

		if(tmp==0)
		{
			tmpnt.mods[indexR].LeftX = tmpnt.mods[indexL].LeftX + tmpnt.mods[indexL].modW;
			tmpnt.mods[indexR].LeftY = tmpnt.mods[indexL].LeftY;
			tmpnt.mods[indexR].CenterX = tmpnt.mods[indexR].LeftX + (int)tmpnt.mods[indexR].modW/2;
		}
		else
		{
			tmpnt.mods[indexR].LeftX = tmpnt.mods[indexL].LeftX;
			tmpnt.mods[indexR].LeftY = tmpnt.mods[indexL].LeftY + tmpnt.mods[indexL].modH;
			tmpnt.mods[indexR].CenterY = tmpnt.mods[indexR].LeftY + (int)tmpnt.mods[indexR].modH/2;
		}
	}
	return;
}

void EPLG:: RoughSwapBase(NETLIST &tmpnt, TREE* Tree, unsigned short &numTree, SubTREE *t)
{
	char flagHV;
	char tmp;
	int indexB;
	//int indexL;
	//int indexR;
	int widthL;
	int heightL;
	int widthR;
	int heightR;
	int xValueL;
	int yValueL;
	int xValueR;
	int yValueR;
	int nBuffNet = 0;
	// unsigned short *buffNet
	// define in dataType.h and declare in dataProc.cpp
	double buffnetWL[2] = {0.0, 0.0};
	CURVE *selfC = t->selfCurve;
	CURVE *leftC = t->leftCurve;
	CURVE *rightC = t->rightCurve;
	SubTREE *leftT = t->leftTree;
	SubTREE *rightT = t->rightTree;

	GetDirect(selfC, &flagHV);

	if(t->parentTree == NULL)
	{
		t->coordX = Tree[t->nodeCurve->id].treeCoordX;
		t->coordY = Tree[t->nodeCurve->id].treeCoordY;
	}
	// cout << "*selfC : " << *selfC << endl;
	// cout << "*leftC : " << *leftC << endl;
	// cout << "*rightC : " << *rightC << endl;
	Get2Dim(leftC, &widthL, &heightL, &tmp);
	Get2Dim(rightC, &widthR, &heightR, &tmp);
	xValueL = t->coordX;
	yValueL = t->coordY;

	if(flagHV == 0)
	{
		xValueR = t->coordX + widthL;
		yValueR = t->coordY;
	}
	else
	{
		xValueR = t->coordX;
		yValueR = t->coordY + heightL;
	}

	if(leftT == NULL)
	{
		indexB = leftC->id;
		tmpnt.mods[indexB].modW = widthL;
		tmpnt.mods[indexB].modH = heightL;
		tmpnt.mods[indexB].CenterX = xValueL + widthL / 2;
		tmpnt.mods[indexB].CenterY = yValueL + heightL / 2;
		tmpnt.mods[indexB].LeftX = xValueL;
		tmpnt.mods[indexB].LeftY = yValueL;
		for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
		{
			if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
			{
				tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
				buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
				nBuffNet++;
			}
		}
	}
	else
	{
		leftT->coordX = xValueL;
		leftT->coordY = yValueL;
		for(int i=0; i<leftT->nVertex; i++)
		{
			indexB = leftT->vArray[i];
			tmpnt.mods[indexB].CenterX = leftT->coordX + widthL / 2;
			tmpnt.mods[indexB].CenterY = leftT->coordY + heightL / 2;
			tmpnt.mods[indexB].LeftX = leftT->coordX;
			tmpnt.mods[indexB].LeftY = leftT->coordY;
			for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
			{
				if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
				{
					tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
					buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
					nBuffNet++;
				}
			}
		}
	}
	if(rightT == NULL)
	{
		indexB = rightC->id;
		tmpnt.mods[indexB].modW = widthR;
		tmpnt.mods[indexB].modH = heightR;
		tmpnt.mods[indexB].CenterX = xValueR + widthR / 2;
		tmpnt.mods[indexB].CenterY = yValueR + heightR / 2;
		tmpnt.mods[indexB].LeftX = xValueR;
		tmpnt.mods[indexB].LeftY = yValueR;
		for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
		{
			if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
			{
				tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
				buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
				nBuffNet++;
			}
		}
	}
	else
	{
		rightT->coordX = xValueR;
		rightT->coordY = yValueR;
		for(int i=0; i<rightT->nVertex; i++)
		{
			indexB = rightT->vArray[i];
			tmpnt.mods[indexB].CenterX = rightT->coordX + widthR / 2;
			tmpnt.mods[indexB].CenterY = rightT->coordY + heightR / 2;
			tmpnt.mods[indexB].LeftX = rightT->coordX;
			tmpnt.mods[indexB].LeftY = rightT->coordY;
			for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
			{
				if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
				{
					tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
					buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
					nBuffNet++;
				}
			}
		}
	}

	GetBuffNetWL(tmpnt, nBuffNet, &buffnetWL[0], 0);

	leftC = t->rightCurve;
	rightC = t->leftCurve;
	leftT = t->rightTree;
	rightT = t->leftTree;

	xValueL = t->coordX;
	yValueL = t->coordY;
	if(flagHV == 0)
	{
		xValueR = t->coordX + widthR;
		yValueR = t->coordY;
	}
	else
	{
		xValueR = t->coordX;
		yValueR = t->coordY + heightR;
	}

	if(leftT == NULL)
	{
		indexB = leftC->id;
		tmpnt.mods[indexB].CenterX = xValueL + widthR / 2;
		tmpnt.mods[indexB].CenterY = yValueL + heightR / 2;
	}
	else
	{
		leftT->coordX = xValueL;
		leftT->coordY = yValueL;
		for(int i=0; i<leftT->nVertex; i++)
		{
			indexB = leftT->vArray[i];
			tmpnt.mods[indexB].CenterX = leftT->coordX + widthR / 2;
			tmpnt.mods[indexB].CenterY = leftT->coordY + heightR / 2;
		}
	}

	if(rightT == NULL)
	{
		indexB = rightC->id;
		tmpnt.mods[indexB].CenterX = xValueR + widthL / 2;
		tmpnt.mods[indexB].CenterY = yValueR + heightL / 2;
	}
	else
	{
		rightT->coordX = xValueR;
		rightT->coordY = yValueR;

		for(int i=0; i<rightT->nVertex; i++)
		{
			indexB = rightT->vArray[i];
			tmpnt.mods[indexB].CenterX = rightT->coordX + widthL / 2;
			tmpnt.mods[indexB].CenterY = rightT->coordY + heightL / 2;
		}
	}

	GetBuffNetWL(tmpnt, nBuffNet, &buffnetWL[1], 1);

	if(buffnetWL[0] < buffnetWL[1])
	{
		leftC = t->leftCurve;
		rightC = t->rightCurve;
		leftT = t->leftTree;
		rightT = t->rightTree;

		xValueL = t->coordX;
		yValueL = t->coordY;
		if(flagHV == 0)
		{
			xValueR = t->coordX + widthL;
			yValueR = t->coordY;
		}
		else
		{
			xValueR = t->coordX;
			yValueR = t->coordY + heightL;
		}

		if(leftT == NULL)
		{
			indexB = leftC->id;
			tmpnt.mods[indexB].CenterX = xValueL + widthL / 2;
			tmpnt.mods[indexB].CenterY = yValueL + heightL / 2;
			tmpnt.mods[indexB].LeftX = xValueL;
			tmpnt.mods[indexB].LeftY = yValueL;
		}
		else
		{
			leftT->coordX = xValueL;
			leftT->coordY = yValueL;

			for(int i=0; i<leftT->nVertex; i++)
			{
				indexB = leftT->vArray[i];
				tmpnt.mods[indexB].CenterX = leftT->coordX + widthL / 2;
				tmpnt.mods[indexB].CenterY = leftT->coordY + heightL / 2;
			}
		}
		if(rightT == NULL)
		{
			indexB = rightC->id;
			tmpnt.mods[indexB].CenterX = xValueR + widthR / 2;
			tmpnt.mods[indexB].CenterY = yValueR + heightR / 2;
			tmpnt.mods[indexB].LeftX = xValueR;
			tmpnt.mods[indexB].LeftY = yValueR;
		}
		else
		{
			rightT->coordX = xValueR;
			rightT->coordY = yValueR;

			for(int i=0; i<rightT->nVertex; i++)
			{
				indexB = rightT->vArray[i];
				tmpnt.mods[indexB].CenterX = rightT->coordX + widthR / 2;
				tmpnt.mods[indexB].CenterY = rightT->coordY + heightR / 2;
			}
		}
	}
	else
	{
		if(leftT == NULL)
		{
			indexB = leftC->id;
			tmpnt.mods[indexB].LeftX = xValueL;
			tmpnt.mods[indexB].LeftY = yValueL;
		}
		if(rightT == NULL)
		{
			indexB = rightC->id;
			tmpnt.mods[indexB].LeftX = xValueR;
			tmpnt.mods[indexB].LeftY = yValueR;
		}

		t->flagSwap = (t->flagSwap == 1) ? 0 : 1;
	}
	return;
}

void EPLG:: DetailedSwapBase(NETLIST &tmpnt, TREE *Tree, unsigned short &numTree, SubTREE *t)
{
	int indexB;
	int nBuffNet = 0;
	//int disL;
	//int disR;
	int widthL;
	int widthR;
	int heightL;
	int heightR;
	char flagHV;
	char tmp;
	double buffnetWL[2] = {0.0, 0.0};
	CURVE *leftC = t->leftCurve;
	CURVE *rightC = t->rightCurve;
	SubTREE *leftT = t->leftTree;
	SubTREE *rightT = t->rightTree;

	if(t->flagSwap == 0)
	{
		leftC = t->leftCurve;
		rightC = t->rightCurve;
		leftT = t->leftTree;
		rightT = t->rightTree;
	}
	else
	{
		leftC = t->rightCurve;
		rightC = t->leftCurve;
		leftT = t->rightTree;
		rightT = t->leftTree;
	}

	GetDirect(t->selfCurve, &flagHV);

	if(leftT==NULL)
	{
		indexB = leftC->id;
		widthL = tmpnt.mods[indexB].modW;
		heightL = tmpnt.mods[indexB].modH;

		for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
		{
			if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
			{
				tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
				buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
				nBuffNet++;
			}
		}
	}
	else
	{
		Get2Dim(leftC, &widthL, &heightL, &tmp);
		for(int i=0; i<leftT->nVertex; i++)
		{
			indexB = leftT->vArray[i];
			for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
			{
				if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
				{
					tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
					buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
					nBuffNet++;
				}
			}
		}
	}

	if(rightT == NULL)
	{
		indexB = rightC->id;
		widthR = tmpnt.mods[indexB].modW;
		heightR = tmpnt.mods[indexB].modH;
		for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
		{
			if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
			{
				tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
				buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
				nBuffNet++;
			}
		}
	}
	else
	{
		Get2Dim(rightC, &widthR, &heightR, &tmp);
		for(int i=0; i<rightT->nVertex; i++)
		{
			indexB = rightT->vArray[i];
			for(int j=0; j<tmpnt.mods[indexB].nNet; j++)
			{
				if(tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag == 0)
				{
					tmpnt.nets[tmpnt.mods[indexB].pNet[j]].flag = 1;
					buffNet[nBuffNet] = tmpnt.mods[indexB].pNet[j];
					nBuffNet++;
				}
			}
		}
	}

	GetBuffNetWL(tmpnt, nBuffNet, &buffnetWL[0], 0);

	if(flagHV == 0)
	{
		if(leftT == NULL)
		{
			indexB = leftC->id;
			tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX + widthR;
		}
		else
		{
			for(int i=0; i<leftT->nVertex; i++)
			{
				indexB = leftT->vArray[i];
				tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX + widthR;
			}
		}

		if(rightT == NULL)
		{
			indexB = rightC->id;
			tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX - widthL;
		}
		else
		{
			for(int i=0; i<rightT->nVertex; i++)
			{
				indexB = rightT->vArray[i];
				tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX - widthL;
			}
		}
	}
	else
	{
		if(leftT == NULL)
		{
			indexB = leftC->id;
			tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY + heightR;
		}
		else
		{
			for(int i=0; i<leftT->nVertex; i++)
			{
				indexB = leftT->vArray[i];
				tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY + heightR;
			}
		}
		if(rightT == NULL)
		{
			indexB = rightC->id;
			tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY - heightL;
		}
		else
		{
			for(int i=0; i<rightT->nVertex; i++)
			{
				indexB = rightT->vArray[i];
				tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY - heightL;
			}
		}
	}

	GetBuffNetWL(tmpnt, nBuffNet, &buffnetWL[1], 1);

	if(buffnetWL[0] < buffnetWL[1])
	{
		if(flagHV == 0)
		{
			if(leftT == NULL)
			{
				indexB = leftC->id;
				tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX - widthR;
			}
			else
			{
				for(int i=0; i<leftT->nVertex; i++)
				{
					indexB = leftT->vArray[i];
					tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX - widthR;
				}
			}

			if(rightT == NULL)
			{
				indexB = rightC->id;
				tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX + widthL;
			}
			else
			{
				for(int i=0; i<rightT->nVertex; i++)
				{
					indexB = rightT->vArray[i];
					tmpnt.mods[indexB].CenterX = tmpnt.mods[indexB].CenterX + widthL;
				}
			}
		}
		else
		{
			if(leftT == NULL)
			{
				indexB = leftC->id;
				tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY - heightR;
			}
			else
			{
				for(int i=0; i<leftT->nVertex; i++)
				{
					indexB = leftT->vArray[i];
					tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY - heightR;
				}
			}
			if(rightT == NULL)
			{
				indexB = rightC->id;
				tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY + heightL;
			}
			else
			{
				for(int i=0; i<rightT->nVertex; i++)
				{
					indexB = rightT->vArray[i];
					tmpnt.mods[indexB].CenterY = tmpnt.mods[indexB].CenterY + heightL;
				}
			}
		}
	}
	else
	{
		if(flagHV == 0)
		{
			if(leftT == NULL)
			{
				indexB = leftC->id;
				tmpnt.mods[indexB].LeftX = tmpnt.mods[indexB].LeftX + widthR;
			}
			else
			{
				for(int i=0; i<leftT->nVertex; i++)
				{
					indexB = leftT->vArray[i];
					tmpnt.mods[indexB].LeftX = tmpnt.mods[indexB].LeftX + widthR;
				}
			}
			if(rightT == NULL)
			{
				indexB = rightC->id;
				tmpnt.mods[indexB].LeftX = tmpnt.mods[indexB].LeftX - widthL;
			}
			else
			{
				for(int i=0; i<rightT->nVertex; i++)
				{
					indexB = rightT->vArray[i];
					tmpnt.mods[indexB].LeftX = tmpnt.mods[indexB].LeftX - widthL;
				}
			}
		}
		else // if(flagHV != 0)
		{
			if(leftT == NULL)
			{
				indexB = leftC->id;
				tmpnt.mods[indexB].LeftY = tmpnt.mods[indexB].LeftY + heightR;
			}
			else
			{
				for(int i=0; i<leftT->nVertex; i++)
				{
					indexB = leftT->vArray[i];
					tmpnt.mods[indexB].LeftY = tmpnt.mods[indexB].LeftY + heightR;
				}
			}
			if(rightT == NULL)
			{
				indexB = rightC->id;
				tmpnt.mods[indexB].LeftY = tmpnt.mods[indexB].LeftY - heightL;
			}
			else
			{
				for(int i=0; i<rightT->nVertex; i++)
				{
					indexB = rightT->vArray[i];
					tmpnt.mods[indexB].LeftY = tmpnt.mods[indexB].LeftY - heightL;
				}
			}
		}
		t->flagSwap = (t->flagSwap == 1) ? 0 : 1;
	}
	return;
}

void EPLG:: GetDirect(CURVE *c, char *direct)
{
	int n;
	if(c->traceIndex >= c->nPoint)
	{
		if(c->points[c->nPoint-1].x != c->points[c->nPoint-1].y)
		{
			n = 2 * c->nPoint;
		}
		else
		{
			n = 2 * c->nPoint - 1;
		}
		*direct = (c->points[n-1-c->traceIndex].traceD==1) ? 0 : 1;
	}
	else
	{
		*direct = c->points[c->traceIndex].traceD;
	}
}

void EPLG:: Get2Dim(CURVE *c, int *width, int *height, char *direct)
{
	if(c->flagHalf == '0')
	{
		//if( c->flagReverse == '0' )
		//{
		*width = c->points[c->traceIndex].x;
		*height = c->points[c->traceIndex].y;
		*direct = c->points[c->traceIndex].traceD;
		//}
		/*else
		  {
		 *width = c->points[c->traceIndex].y;
		 *height = c->points[c->traceIndex].x;
		 *direct = c->points[c->traceIndex].traceD;
		 }*/
		/*cout << "GetWidth : " << *width << "  GetHeight : " << *height <<endl;
		  cout << "==========================================" << endl;*/
		//  return;
	}
	else
	{
		int n;
		if(c->traceIndex >= c->nPoint)
		{
			if(c->points[c->nPoint-1].x != c->points[c->nPoint-1].y)
			{
				n = 2 * c->nPoint;
			}
			else
			{
				n = 2 * c->nPoint - 1;
			}
			*width = c->points[n-1-c->traceIndex].y;
			*height = c->points[n-1-c->traceIndex].x;
			*direct = (c->points[n-1-c->traceIndex].traceD==1) ? 0 : 1;
		}
		else
		{
			*width = c->points[c->traceIndex].x;
			*height = c->points[c->traceIndex].y;
			*direct = c->points[c->traceIndex].traceD;
		}

		/*if( c->flagReverse == '1' )
		  {
		  float temppo;
		  temppo=*width;
		 *width=*height;
		 *height=temppo;
		 }*/

		/*cout << "GetWidth : " << *width << "  GetHeight : " << *height <<endl;
		  cout << "==========================================" << endl;	*/
		return;
	}
}

void EPLG:: GetBuffNetWL(NETLIST &tmpnt, int nNet, double *newWL, char resetFlag)
{
	int maxX = 0;
	int minX = tmpnt.ChipWidth;
	int maxY = 0;
	int minY = tmpnt.ChipHeight;
	int centerX;
	int centerY;
	//int indexB;

	// 0: remain the flag in Nets, 1: clean the flag
	//char flag = (resetFlag==0) ? 1 : 0;

	for(int i=0; i<nNet; i++)
	{
		maxX = 0;
		minX = tmpnt.ChipWidth;
		maxY = 0;
		minY = tmpnt.ChipHeight;
		for(int j=tmpnt.nets[buffNet[i]].head; j<tmpnt.nets[buffNet[i]].head+tmpnt.nets[buffNet[i]].nPin; j++)
		{
			if(tmpnt.pins[j].index < tmpnt.nMod) // Module
			{
				centerX = tmpnt.mods[tmpnt.pins[j].index].CenterX;
				centerY = tmpnt.mods[tmpnt.pins[j].index].CenterY;
			}
			else // if(tmpnt.pins[j].index >= tmpnt.nMod) // Pad
			{
				centerX = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].x;
				centerY = tmpnt.pads[tmpnt.pins[j].index-tmpnt.nMod].y;
			}
			maxX = (centerX > maxX) ? centerX : maxX;
			minX = (centerX < minX) ? centerX : minX;
			maxY = (centerY > maxY) ? centerY : maxY;
			minY = (centerY < minY) ? centerY : minY;
		}
		*newWL = *newWL + ((maxX-minX)+(maxY-minY))/(double)AMP_PARA;
	}
	return;
}


// class OVERLAP


void OVERLAP::CalcORL(void)
{
	int x1 = X1R-X2L;
	int x2 = X2R-X1L;
	ORLX = (x1 <= x2) ? x1 : x2;
	ORLX = (ORLX > 0) ? ORLX : 0;
	int y1 = Y1T-Y2B;
	int y2 = Y2T-Y1B;
	ORLY = (y1 <= y2) ? y1 : y2;
	ORLY = (ORLY > 0) ? ORLY : 0;
	ORL = (ORLX < ORLY) ? ORLX : ORLY;
	return;
}

void OVERLAP::CalcOA(void)
{
	int x1 = X1R-X2L;
	int x2 = X2R-X1L;
	ORLX = (x1 <= x2) ? x1 : x2;
	ORLX = (ORLX > 0) ? ORLX : 0;
	int y1 = Y1T-Y2B;
	int y2 = Y2T-Y1B;
	ORLY = (y1 <= y2) ? y1 : y2;
	ORLY = (ORLY > 0) ? ORLY : 0;
	OA = (ORLX/((float)AMP_PARA)) * (ORLY/((float)AMP_PARA));
	return;
}


// Find Overlap
void EPLG:: FindOverlap(NETLIST &tmpnt)
{
	overLap.clear();
	for(int i=0; i<tmpnt.nMod; i++)
	{
		for(int j=i+1; j<tmpnt.nMod; j++)
		{
			OVERLAP overlap;
			overlap.id1 = i;
			overlap.id2 = j;
			overlap.X1L = tmpnt.mods[i].LeftX;
			overlap.X1R = tmpnt.mods[i].LeftX + tmpnt.mods[i].modW;
			overlap.Y1B = tmpnt.mods[i].LeftY;
			overlap.Y1T = tmpnt.mods[i].LeftY + tmpnt.mods[i].modH;
			overlap.X2L = tmpnt.mods[j].LeftX;
			overlap.X2R = tmpnt.mods[j].LeftX + tmpnt.mods[j].modW;
			overlap.Y2B = tmpnt.mods[j].LeftY;
			overlap.Y2T = tmpnt.mods[j].LeftY + tmpnt.mods[j].modH;
			overlap.CalcORL();
			if(overlap.ORL > 0)
				overLap.push_back(overlap);
		}
	}
}

void EPLG:: FindOverlap_GP(NETLIST &tmpnt)
{
	overLap_GP.clear();
	for(int i=0; i<tmpnt.nMod; i++)
	{
		for(int j=i+1; j<tmpnt.nMod; j++)
		{
			OVERLAP overlap;
			overlap.id1 = i;
			overlap.id2 = j;
			overlap.X1L = tmpnt.mods[i].GLeftX;
			overlap.X1R = tmpnt.mods[i].GLeftX + tmpnt.mods[i].modW;
			overlap.Y1B = tmpnt.mods[i].GLeftY;
			overlap.Y1T = tmpnt.mods[i].GLeftY + tmpnt.mods[i].modH;
			overlap.X2L = tmpnt.mods[j].GLeftX;
			overlap.X2R = tmpnt.mods[j].GLeftX + tmpnt.mods[j].modW;
			overlap.Y2B = tmpnt.mods[j].GLeftY;
			overlap.Y2T = tmpnt.mods[j].GLeftY + tmpnt.mods[j].modH;
			overlap.CalcOA();
			if(overlap.OA > 0)	overLap_GP.push_back(overlap);
		}
	}
}

// Find Violate
void EPLG:: FindViolate(NETLIST &tmpnt)
{
	FindOverlap(tmpnt);

	for(int i=0; i<(int)overLap.size(); i++)
	{
		cout << overLap[i] << endl;
	}

	for(int i=0; i<tmpnt.nMod; i++)
	{
		if(tmpnt.mods[i].type == SOFT_BLK)
		{
			float modAR = ((float)tmpnt.mods[i].modW) / ((float)tmpnt.mods[i].modH);
			if(modAR < tmpnt.mods[i].minAR)
			{
				if( (tmpnt.mods[i].minAR - modAR) < 0.001) continue;
				cout << "module " << i << " violates minAR" << endl;
				cout << "    modAR " << modAR << " minAR " << tmpnt.mods[i].minAR << endl;
				cout << "    modW " << ((float)tmpnt.mods[i].modW)/((float)AMP_PARA) << endl;
				cout << "    modH " << ((float)tmpnt.mods[i].modH)/((float)AMP_PARA) << endl;
			}
			if(modAR > tmpnt.mods[i].maxAR)
			{
				if( (modAR - tmpnt.mods[i].maxAR) < 0.001) continue;
				cout << "module " << i << " violates maxAR" << endl;
				cout << "    modAR " << modAR << " maxAR " << tmpnt.mods[i].maxAR << endl;
				cout << "    modW " << ((float)tmpnt.mods[i].modW)/((float)AMP_PARA) << endl;
				cout << "    modH " << ((float)tmpnt.mods[i].modH)/((float)AMP_PARA) << endl;
			}
		}
	}
}


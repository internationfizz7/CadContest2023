/*!
 * \file 	dataType.cpp
 * \brief 	data structures
 *
 * \author 	NCKU SEDA LAB
 * \version	2.0 (3-D) by Tai-Ting
 * \date	2018.08.08
 */

#include "dataType.h"

/*------------------------------*
 *
 * Class
 *
 *------------------------------*/

AlIGN_PAIR::AlIGN_PAIR()
{
    lowModId = 0;
    highModId = 0;

    lowLayer = -1;
    highLayer = -1;

    lowCplexIndex = -1;
    highCplexIndex = -1;
}

AlIGN_PAIR::~AlIGN_PAIR()
{}

ostream& operator<<(ostream &output, const PAD &pad)
{
	output << pad.padName << "/" << "pad" << "[" << pad.idPad << "]" << endl;
	output << "\t" << "coor: " << "(" << pad.x << ", " << pad.y << ")" << endl;
	output << "\t" << "padL: " << pad.padL << endl;

	return output;
}

PAD::PAD() : idPad(0), x(0), y(0), padL(0)
{
	strcpy(padName, "\0");
}

PAD::~PAD()
{ }

PAD& PAD::operator=(const PAD &pad)
{
	if(&pad != this)
	{
		strcpy(padName, pad.padName);
		idPad = pad.idPad;
		x = pad.x;
		y = pad.y;
		padL = pad.padL;
	}

	return *this;
}

ostream& operator<<(ostream& output, const PIN &pin)
{
	output << pin.pinName << "/" << "pin" << "[" << pin.idPin << "]" << endl;
	output << "\t" << "offset : " << "(" << pin.offsetX << ", " << pin.offsetY << ")" << endl;
	output << "\t" << "index  : " << pin.index << endl;
	output << "\t" << "pinL   : " << pin.pinL << endl;
	output << "\t" << "flagTSV: " << pin.flagTSV << endl;

	return output;
}

PIN::PIN() : idPin(0), index(0), offsetX(0), offsetY(0), pinL(0), flagTSV(false)
{
	strcpy(pinName, "\0");
}

PIN::~PIN()
{ }

PIN& PIN::operator=(const PIN &pin)
{
	if(&pin != this)
	{
		strcpy(pinName, pin.pinName);
		idPin = pin.idPin;
		index = pin.index;
		offsetX = pin.offsetX;
		offsetY = pin.offsetY;
		pinL = pin.pinL;
		flagTSV = pin.flagTSV;
	}

	return *this;
}

Lib_vector::Lib_vector()
{ }
Lib_vector::~Lib_vector()
{ }


Lib::Lib(): name("")
{ }
Lib::~Lib()
{ }

Libcell::Libcell(): id(0) , w(0), h(0), p(0),is_m(false)
{ 
strcpy(name, "\0");
}
Libcell::~Libcell()
{ }
ostream& operator<<(ostream &output, const MODULE &module)
{
	output << module.modName << "/" << "module" << "[" << module.idMod << "]" << endl;
	output << "\t" << "type        : " << module.type << endl;
	output << "\t" << "modL        : " << module.modL << endl;
	output << "\t" << "area        : " << module.modArea << endl;
	output << "\t" << "dimension   : " << "(" << module.modW << ", " << module.modH << ")" << endl;
	output << "\t" << "aspect ratio: " << module.minAR << " - " << module.maxAR << endl;
	output << "\t" << "global coorL: " << "(" << module.GLeftX << ", " << module.GLeftY << ")" << endl;
	output << "\t" << "global coorC: " << "(" << module.GCenterX << ", " << module.GCenterY << ")" << endl;
	output << "\t" << "legal coorL : " << "(" << module.LeftX << ", " << module.LeftY << ")" << endl;
	output << "\t" << "legal coorC : " << "(" << module.CenterX << ", " << module.CenterY << ")" << endl;
	output << "\t" << "nNet        : " << module.nNet << endl;
	output << "\t" << "flagTSV     : " << module.flagTSV << endl;
	output << "\t" << "flagPre     : " << module.flagPre << endl;

	return output;
}

///f///0602 modify nLayer(1),
MODULE::MODULE()
{
	is_m=false;
	idMod = -1 ;
	type = '1';
	GmodW = GmodH = 0 ;
	ShrinkH = ShrinkW = 0  ;
	modW = modH = modL = modArea=modW_bot=modH_bot=modArea_bot = 0 ;
	minAR = maxAR=minAR_bot = maxAR_bot = 0.0 ;
	nNet = 0 ;
	pNet = NULL ;
	pseudoMod = NULL;
	GLeftX = GLeftY = 0 ;
	GCenterX = GCenterY = LeftX = LeftY = CenterX = CenterY = 0;
	flagTSV = flagPre = pseudoTSV = false;
	flagAlign = false;
	flagCluster=false;
	nLayer = 1 ;
	folding = false ;
	TemperatureGroup = 2 ;
	ModuleCount = 1 ,
	Power = Pdensity = 0;
	strcpy(modName, "\0");
}

MODULE::~MODULE()
{ }

MODULE& MODULE::operator=(const MODULE &module)
{
	if(&module != this)
	{
		strcpy(modName, module.modName);
		idMod = module.idMod;
		type = module.type;
		GmodH = module.GmodH;
		GmodW = module.GmodW;
		ShrinkW = module.ShrinkW;
		ShrinkH = module.ShrinkH;
		modW = module.modW;
		modH = module.modH;
		modL = module.modL;
		modArea = module.modArea;
		minAR = module.minAR;
		maxAR = module.maxAR;
		nNet = module.nNet;
		TemperatureGroup = module.TemperatureGroup;
		Power = module.Power;
		Pdensity = module.Pdensity;
		ModuleCount = module.ModuleCount;
        InClusterMod.resize( module.InClusterMod.size() ) ;
        nLayer = module.nLayer;

        if(pNet != module.pNet)
        {
            for(int i = 0; i < module.InClusterMod.size(); i++)
            {
                InClusterMod.at(i) = module.InClusterMod[i];
            }
            if(pNet != NULL)
            {
                delete [] this->pNet;
            }
            try
            {
                pNet = new unsigned int [nNet];
            }
            catch(bad_alloc &bad)
            {
                cout << "error  : run out of memory on MODULE operator =" << endl;
                exit(EXIT_FAILURE);
            }
            for(int i = 0; i < nNet; i++)
            {
                pNet[i] = module.pNet[i];
            }
        }
		/*if(idMod==911)
		{
            cout << "911 : " << pNet[0] <<endl;
            getchar();
		}*/

		GLeftX = module.GLeftX;
		GLeftY = module.GLeftY;
		GCenterX = module.GCenterX;
		GCenterY = module.GCenterY;
		LeftX = module.LeftX;
		LeftY = module.LeftY;
		CenterX = module.CenterX;
		CenterY = module.CenterY;
		flagTSV = module.flagTSV;
		flagPre = module.flagPre;

	}

	return *this;
}

void MODULE::CreateCurveB(int totalModArea, int shConst)
{
	int delta;
	int num;

	if(type == HARD_BLK)
	{
		delta = 0;
	}
	else
	{
		delta = (int)((modArea/(float)totalModArea) * shConst) + 4;
	}

	num = 1 + delta/2;

	shapeCurveB.id = idMod;
	shapeCurveB.nPoint = num;
	shapeCurveB.leftCurve = NULL;
	shapeCurveB.rightCurve = NULL;
	shapeCurveB.flagHalf = '1';

	try
	{
		shapeCurveB.points = new POINT [num];
	}
	catch(bad_alloc &bad)
	{
		cout << "error  : run out of memory on CreateCurveB" << endl;
		exit(EXIT_FAILURE);
	}

	/// build lower-half curve
	if(type == HARD_BLK)
	{
		if(modH > modW)
		{
			shapeCurveB.points[0].x = modH;
			shapeCurveB.points[0].y = modW;
		}
		else
		{
			shapeCurveB.points[0].x = modW;
			shapeCurveB.points[0].y = modH;
		}
	}
	else
	{
		float minX;
		float maxX;
		float temp;
		maxX = sqrt(maxAR * modArea);
		minX = sqrt(modArea);
		for(int j = 0; j < num; j++)
		{
			temp = maxX - (maxX - minX) * ((float)j/(num - 1.0));
			shapeCurveB.points[j].x = (int)(temp * ((float)AMP_PARA));
			shapeCurveB.points[j].y = (int)(((float)modArea) / temp * ((float)AMP_PARA));
		}
	}
}

ostream& operator<<(ostream &output, const NET &net)
{
	output << net.netName << "/" << "net" << "[" << net.idNet << "]" << endl;
	output << "\t" << "head: " << net.head << endl;
	output << "\t" << "nPin: " << net.nPin << endl;
	output << "\t" << "netL: " << net.netL << endl;
	output << "\t" << "nTSV: " << net.nTSV << endl;
	output << "\t" << "flag: " << net.flag << endl;

	return output;
}

NET::NET() : idNet(0), head(0), nPin(0), netL(-1), nTSV(0), flag('0'), flagAlign(false)
{
	strcpy(netName, "\0");
}

NET::~NET()
{ }

NET& NET::operator=(const NET &net)
{
	if(&net != this)
	{
		strcpy(netName, net.netName);
		idNet = net.idNet;
		head = net.head;
		nPin = net.nPin;
		flag = net.flag;
		nTSV = net.nTSV;
		netL = net.netL;
	}

	return *this;
}

ostream& operator<<(ostream &output, const NETLIST &netlist)
{
	output << "***** Benchmark Characteristics *****" << endl;
	output << "-Number of modules         : " << netlist.GetnMod() << endl;
	//cout<<netlist.nMod;
	output << "-Number of pads            : " << netlist.GetnPad() << endl;
	output << "-Number of nets            : " << netlist.GetnNet() << endl;
	output << "-Number of pins            : " << netlist.GetnPin() << endl;
	output << "-Number of layers          : " << netlist.GetnLayer() << endl;
	output << "*************************************" << endl;
	output << "***** Specified Fixed-outline *******" << endl;
	output << "-Maximum whitespace (%)    : " << netlist.GetmaxWS() * 100.0 << endl;
	output << "-Fixed-outline aspect ratio: " << netlist.GetaR() << endl;
	output << "*************************************" << endl;

	return output;
}

///f///0602 modify nLayer(1)
NETLIST::NETLIST() : output_name(""),topn(0),botn(0),nHard(0), nSoft(0), nMod(0), nNet(0), nPin(0), nPad(0), nAlign(0), nTSV(0), nLayer(1), ChipWidth(0), ChipHeight(0), ChipArea(0.0), totalModArea(0),topModArea(0),botModArea(0), totalWL(0.0), totalWL_GP(0.0),totalWL_ML(0.0),totalWL_CL(0.0), maxWS(0.0), aR(0.0),row_high_top(0),NumRows_top(0),Xmin_top(0),Ymin_top(0),Xmax_top(0),Ymax_top(0),row_high_bot(0),NumRows_bot(0),Xmin_bot(0),Ymin_bot(0),Xmax_bot(0),Ymax_bot(0),toputil(0),botutil(0),toptech(""),bottech(""),terminal_w(0),terminal_h(0),terminal_space(0),terminal_cost(0)
{ }


NETLIST::~NETLIST()
{ }

void NETLIST::InitailLayer()
{
	for (int i = 0 ; i < this->nMod ; i++)
	{
		this->mods[i].modL = 0;
	}
	for(int i = 0 ; i< this->nPad ; i++)
		this->pads[i].padL = 0;
};
NETLIST& NETLIST::operator=(const NETLIST &netlist)
{
	if(&netlist != this)
	{
		nHard = netlist.nHard;
		nSoft = netlist.nSoft;

		nMod = netlist.nMod;
		mods.resize(nMod);
		for(int i = 0; i < nMod; i++)
		{
            /*if(i==911)
            {
                cout << "911 : " << netlist.mods[911].nNet <<endl;
                cout << netlist.mods[911].pNet[0] <<  endl;
                getchar();
            }*/
			mods.at(i) = netlist.mods[i];
			/*if(i==911)
            {
                cout << "911 : " << netlist.mods[911].nNet <<endl;
                cout << netlist.mods[911].pNet[0] <<  endl;
                getchar();
            }*/
		}

		nNet = netlist.nNet;
		nets.resize(nNet);
		for(int i = 0; i < nNet; i++)
		{
			nets.at(i) = netlist.nets[i];
		}

		nPin = netlist.nPin;
		pins.resize(nPin);
		for(int i = 0; i < nPin; i++)
		{
			pins.at(i) = netlist.pins[i];
		}

		nPad = netlist.nPad;
		pads.resize(nPad);
		for(int i = 0; i < nPad; i++)
		{
			pads.at(i) = netlist.pads[i];
		}

		nTSV = netlist.nTSV;
		nLayer = netlist.nLayer;
		ChipWidth = netlist.ChipWidth;
		ChipHeight = netlist.ChipHeight;
		ChipArea = netlist.ChipArea;
		FenceArea = netlist.FenceArea;
		totalModArea = netlist.totalModArea;
		totalWL = netlist.totalWL;
		totalWL_GP = netlist.totalWL_GP;
		maxWS = netlist.maxWS;
		aR = netlist.aR;

		mod_NameToID.clear();
		mod_NameToID = netlist.mod_NameToID;
		pad_NameToID.clear();
		pad_NameToID = netlist.pad_NameToID;
	}

	return *this;
}

/*------------------------------*
 *
 *   OBJ Function
 *
 *-----------------------------*/


void NETLIST::SetFixedOutline(int w,int h)
{

	ChipWidth  = w*AMP_PARA;
	ChipHeight = h*AMP_PARA;
	
	cout << ChipWidth << " " << ChipHeight<<" "<<w<<" "<<h<<"\n";

	ChipArea   = ChipWidth*ChipHeight;

	//5/19住解
	/*int *layerArea = new int [nLayer];
	for( int i = 0; i < nLayer; i++ )
	{
		layerArea[i] = 0;
	}
	for( int i = 0; i < nMod; i++ )
	{
		layerArea[ mods[i].modL ] +=mods[i].modArea;
		if(mods[i].nLayer>1)
		{
			for( int j = 1; j < mods[i].nLayer; j++)
			{
				layerArea[mods[i].modL + j ] += mods[i].modArea;
			}
		}

	}

	int maxArea = 0;
	for( int i = 0; i < nLayer; i++ )
	{
		if( layerArea[i] > maxArea )
		{
			maxArea = layerArea[i];
		}
	}
	ChipWidth  = (int)(sqrt( 1.0/(1.0 - maxWS) * maxArea / aR) * AMP_PARA);
	ChipHeight = (int)(sqrt( 1.0/(1.0 - maxWS) * maxArea * aR) * AMP_PARA);
	


	ChipArea   = 1.0/(1.0 + maxWS) * (float)maxArea; //  ??

	delete [] layerArea;
	cout<<ChipWidth<<" "<<ChipHeight<<" "<<ChipArea<<"\n";*/
};

void NETLIST::AllocatePad(bool allocate_mode )
{
	if( allocate_mode == 0 )	///< scale to boundary
	{
		int maxX = 0;
		int maxY = 0;
		for( int i = 0; i < nPad; i++ )
		{
			maxX = ( pads[i].x > maxX ) ? pads[i].x : maxX;
			maxY = ( pads[i].y > maxY ) ? pads[i].y : maxY;
		}
		for( int i = 0; i < nPad; i++ )
		{
			pads[i].x = (int)( pads[i].x / (float)maxX * ChipWidth );
			pads[i].y = (int)( pads[i].y / (float)maxY * ChipHeight );
		}

	}
	else if( allocate_mode == 1 )	///< randomly assign
	{

		// cout<<"ChipWidth/2: "<<ChipWidth/2<<"\n";
		// cout<<"ChipHeight/2: "<<ChipHeight/2<<"\n";
		//5/20 modify apply the only pad to the center
		for(int i=0;i<pads.size();i++){
			pads[i].x=ChipWidth/2;
			pads[i].y=ChipHeight/2;
			cout<<"pad"<<pads[i].x<<" "<<pads[i].y;
		}

		//5/20註解
		/*int sideNum = (int)ceil( sqrt(nPad) );
		int sideW = ChipWidth / sideNum;
		int sideH = ChipHeight / sideNum;
		int sideX = 0;
		int sideY = 0;

		vector<str_coor> padNode;
		padNode.resize( sideNum * sideNum );
		for(unsigned int i = 0; i < padNode.size(); i++)
		{
			padNode[i].x = sideX + rand() % sideW;
			padNode[i].y = sideY + rand() % sideH;

			if( (i+1) % sideNum != 0 )
			{
				sideX += sideW;
			}
			else
			{
				sideY += sideH;
				sideX = 0;
			}
		}

		random_shuffle( padNode.begin(), padNode.end() );

		for(int i = 0; i < nPad; i++)
		{
			pads[i].x = padNode[i].x;
			pads[i].y = padNode[i].y;
		}

		padNode.clear();
		vector<str_coor> (padNode).swap(padNode);*/
	}
	else
	{
		cout << "error  : unrecognized allocate_mode in AllocatePad: " << allocate_mode << endl;
		exit(EXIT_FAILURE);
	}

};

ostream &operator<<(ostream &output, const POINT &point)
{
	output << "point (" << point.x/(float)AMP_PARA << ", "
	       << point.y/(float)AMP_PARA << ") -> "
	       << "traceL: " << point.traceL << " "
	       << "traceR: " << point.traceR << " "
	       << "traceD: " << point.traceD << endl;

	return output;
}

POINT::POINT(): x(0), y(0), traceL(0), traceR(0), traceD('0')
{ }

POINT::~POINT()
{ }

POINT& POINT::operator=(const POINT &point)
{
	if(&point != this)
	{
		x = point.x;
		y = point.y;
		traceL = point.traceL;
		traceR = point.traceR;
		traceD = point.traceD;
	}

	return *this;
}

// CURVE
ostream& operator<<(ostream &output, const CURVE &curve)
{
	output << "curve" << "[" << curve.id << "]" << endl;
	output << "\t" << "flagHalf  : " << curve.flagHalf << endl;
	output << "\t" << "traceIndex: " << curve.traceIndex << endl;
	output << "\t" << "nPoint    : " << curve.nPoint << endl;
	for(int i = 0; i < curve.nPoint; i++)
	{
		output << "\t" << curve.points[i];
	}

	return output;
}

CURVE::CURVE(): leftCurve(NULL), rightCurve(NULL), id(0), nPoint(0), points(NULL), flagHalf('1'), traceIndex(0)
{ }

CURVE::~CURVE()
{ }

CURVE& CURVE::operator=(const CURVE &curve)
{
	if(&curve != this)
	{
		leftCurve = curve.leftCurve;
		rightCurve = curve.rightCurve;
		id = curve.id;
		nPoint = curve.nPoint;
		if(points != NULL)
		{
			delete [] points;
		}
		try
		{
			points = new POINT [nPoint];
		}
		catch(bad_alloc &bad)
		{
			cout << "error  : run out of memory on CURVE operator =" << endl;
			exit(EXIT_FAILURE);
		}
		for(int i = 0; i < nPoint; i++)
		{
			points[i] = curve.points[i];
		}
		flagHalf = curve.flagHalf;
		traceIndex = curve.traceIndex;
	}

	return *this;
}
ostream& operator<<(ostream &output, const OPTION &option)
{
	output << "***** Benchmark Characteristics *****" << endl;
	output << "-Program mode                : " << option.mode << endl;
	output << "-Benchmark name              : " << option.benName << endl;
	output << "-Bin paramter                : " << option.bin_para << endl;
	output << "-Is 3D IC                    : " << option.ThreeDimIC << endl;
	output << "-Thermal On                  : " <<option.ThermalOpt <<endl;
	output << "-Compare with Corblivar      : " << option.CompCorblivar << endl;

	if(option.EPorILP == false)
		output << "-Legalizer                   : ILP based legalizer"<< endl;
	else
		output << "-Legalizer                   : EP based legalizer"<< endl;

	output << "********* GP Characteristics ********" << endl;
	output << "-Overflow bound              : " << option.overflow_bound << endl;
	output << "-InitalGradient              : " <<option.InitialGradient<<endl;
	output << "-DeclusterGradient           : " <<option.DeclusterGradient<<endl;

	if(option.ThreeDimIC == true)
	{
        output << "-3D Multilevel               : " << option.ThreeDimICMulti << endl;
        output << "-3D Alignment                : " << option.ThreeDimAlign << endl;
    }

	output << "****** Cluster Characteristics ******" << endl;
	output << "-Cluster hard constraint     : " << option.ClusterArea << endl;
	output << "-ClusterTerminal             : " << option.TargetClusterNum << endl;
	output << "******** TSV Characteristics ********" << endl;
	output << "-TSV Thermal Optimize        : " << option.ThermalTsv << endl;
	output << "******* Output Characteristics ******" << endl;
	output << "-File output path            : " << option.OutputPath << endl;
	output << "*************************************" << endl;

	return output;
}

ostream& operator<<(ostream &output, const IRBIN &IRBin)
{
    output << "IRBIN" << endl;
    output << "Width: " << IRBin.width << " Height: " << IRBin.height << endl;
    output << "simLayer: " << IRBin.simLayer << endl;
    output << "Result: " << IRBin.result << " ResultWT: " << IRBin.resultWT << endl;
    output << "Power: " << endl;
    for(unsigned int i=0; i<IRBin.LayerPower.size(); i++)
        output << "    Layer " << i << ": " << IRBin.LayerPower[i] <<endl;
    output << "TSV: " << endl;
    for(unsigned int i=0; i<IRBin.LayerTSV.size(); i++)
        output << "    Layer " << i << ": " << IRBin.LayerTSV[i] <<endl;
    return output;
}
IRBIN::IRBIN()
{
    width = height = 0;
    result = resultWT = powerVariation = 0.0;
    simLayer = -1;
    simFlag = false;
}
IRBIN::~IRBIN()
{
    LayerTSV.clear();
    LayerPower.clear();
}
/*------------------------------*
 *
 * External Variables
 *
 *------------------------------*/
//---- Input Para ---//

OPTION InputOption;					///< Program Input
//---- swap info ----//
unsigned int *buffNet;


//---- exponential table ----//
double ExpTable[1400001];			///< exponential table









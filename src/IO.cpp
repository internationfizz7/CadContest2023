//4/22 註解fileRead == *4

#include "IO.h"
#include "tool.h"
void PLOT::plot_step( NETLIST &tmpnt, string filename )
{
	filename="dpx/"+filename;
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

bool SortByLayer( const PIN &a, const PIN &b )
{
	if( a.pinL == b.pinL )
	{
		if( a.flagTSV && !b.flagTSV )
		{
			return false;
		}
		else if( b.flagTSV )
		{
			return true;
		}
	}
	return ( a.pinL < b.pinL );
}//*/
void IO::IBM::ReadBlockFile (string benName, NETLIST &nt)
{
	// char fileIn[50];
	// strcpy(fileIn, blockFile);
	//strcat(fileIn, ".blocks");

	//cout << fileIn << endl;
	string fileIn = /*"temp/"+*/benName+ ".blocks";
	ifstream fileRead(fileIn.c_str());

	/*if(fileRead == NULL)
	{
		cout << "Error in opening " << fileIn << endl;
		exit(1);
	}*/



	char ReadBuffer[128];
	char *token = NULL;

	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);

	fileRead.getline(ReadBuffer, 127);
	token = strtok(ReadBuffer, " :\n\t");
	if(!strncmp(token, "NumSoftRectangularBlocks", 24))
	{
		token = strtok(NULL, " :\n\t");
		nt.nSoft = atoi(token);
	}

	fileRead.getline(ReadBuffer, 127);
	token = strtok(ReadBuffer, " :\n\t");
	if(!strncmp(token, "NumHardRectilinearBlocks", 24))
	{
		token = strtok(NULL, " :\n\t");
		nt.nHard = atoi(token);
	}
	nt.nMod = nt.nSoft + nt.nHard;

	fileRead.getline(ReadBuffer, 127);
	token = strtok(ReadBuffer, " :\n\t");
	if(!strncmp(token, "NumTerminals", 12))
	{
		token = strtok(NULL, " :\n\t");
		nt.nPad = atoi(token);
	}
	fileRead.getline(ReadBuffer, 127);
	nt.mods.resize(nt.nMod);
	//cout << "nPad : " << nt.nPad << endl;
	nt.pads.resize(nt.nPad);

	for(int i = 0; i<nt.nSoft; i++)
	{
		fileRead.getline(ReadBuffer, 127);
		token = strtok(ReadBuffer, " :\n\t"); // BLOCK_*
		//strcpy(nt.mods[i].moduleName,token);          ///< 2103-11-20 remove
		//strcpy(nt.mods[i].instanceName,token);
		strcpy(nt.mods[i].modName,token);///f///0602
		nt.mods[i].idMod = i;
		token = strtok(NULL, " :\n\t");       // softrectangular
		nt.mods[i].type = SOFT_BLK;
		token = strtok(NULL, " :\n\t");
		nt.mods[i].modArea = atoi(token);
		token = strtok(NULL, " :\n\t");
		nt.mods[i].minAR = atoi(token);
		token = strtok(NULL, " :\n\t");
		nt.mods[i].maxAR = atoi(token);

		///enlarge block
		nt.mods[i].modArea*=pow(InputOption.EnlargeBlockRatio,2);


		nt.totalModArea += nt.mods[i].modArea;
		///f///nt.flagPureSoft = 1;
		///f///0602
		nt.mods[i].nLayer = 1;
		nt.mods[i].modW=(int)sqrt(nt.mods[i].modArea)*AMP_PARA;
		nt.mods[i].modH=(int)sqrt(nt.mods[i].modArea)*AMP_PARA;
		///y///0218
		nt.mod_NameToID[nt.mods[i].modName] = i;
	}

	for(int i=nt.nSoft; i<nt.nHard + nt.nSoft; i++)
	{
		fileRead.getline(ReadBuffer, 127);
		token = strtok(ReadBuffer, " :\n\t"); // BLOCK_*
		//strcpy(nt.mods[i].moduleName,token);      ///<2013-11-20 remove
		//strcpy(nt.mods[i].instanceName,token);
		strcpy(nt.mods[i].modName,token);///f///0602
		nt.mods[i].idMod = i;
		token = strtok(NULL, " :\n\t");       // hardrectilinear
		nt.mods[i].type = HARD_BLK;
		token = strtok(NULL, " :\n\t,()");    // 4
		token = strtok(NULL, " :\n\t,()");
		token = strtok(NULL, " :\n\t,()");
		token = strtok(NULL, " :\n\t,()");
		token = strtok(NULL, " :\n\t,()");
		token = strtok(NULL, " :\n\t,()");
		nt.mods[i].modW = atoi(token);
		token = strtok(NULL, " :\n\t,()");
		nt.mods[i].modH = atoi(token);

		///enlarge block
		nt.mods[i].modW *= InputOption.EnlargeBlockRatio;
		nt.mods[i].modH *= InputOption.EnlargeBlockRatio;

		nt.mods[i].modArea = nt.mods[i].modW * nt.mods[i].modH;
		nt.mods[i].minAR = nt.mods[i].maxAR = nt.mods[i].modW / nt.mods[i].modH;
		nt.totalModArea += nt.mods[i].modArea;
		///f///nt.flagPureSoft = 0;
		///f///0602
		nt.mods[i].nLayer = 1;
		nt.mods[i].modW*=AMP_PARA;
		nt.mods[i].modH*=AMP_PARA;
		///y///0218
		nt.mod_NameToID[nt.mods[i].modName] = i;
	}


	fileRead.close();
	return;
}

void IO::IBM::ReadNetFile3D(string benName, NETLIST &tmpnt)
{

	string fileIn = benName + ".nets";
	ifstream fileRead(fileIn.c_str());


    /*if(fileRead == NULL)
    {
        cout << "Error in opening " << fileIn << endl;
        exit(1);
    }*/
    char ReadBuffer[128];
    char *token = NULL;

    vector<NET> nets;
    int nNet = 0;

    fileRead.getline(ReadBuffer, 127);
    fileRead.getline(ReadBuffer, 127);
    fileRead.getline(ReadBuffer, 127);
    fileRead.getline(ReadBuffer, 127);

    fileRead.getline(ReadBuffer, 127);
    token = strtok(ReadBuffer, " :\n\t");
    if(!strncmp(token, "NumNets", 7))
    {
        token = strtok(NULL, " :\n\t");
        nNet = atoi(token);
    }

    fileRead.getline(ReadBuffer, 127);
    token = strtok(ReadBuffer, " :\n\t");
    if(!strncmp(token, "NumPins", 7))
    {
        token = strtok(NULL, " :\n\t");
        tmpnt.nPin = atoi(token);
    }

    fileRead.getline(ReadBuffer, 127);

    nets.resize(nNet);

    int tsvCount = 0;
    int pinCount = 0;

//    tmpnt.pins.resize(tmpnt.nPin);

    // construct relationship between nt.nets and nt.pins
    for(int i = 0; i<nNet; i++)
    {
        int maxL = -1;///f///0930 maxL = 0;
        int minL = tmpnt.nLayer;///f///0930 minL = MAX_INT;
        int nPin = 0;
        int nStack=0;

        nets[i].idNet = i;
        nets[i].head = pinCount;

        fileRead.getline(ReadBuffer, 127);
        token = strtok(ReadBuffer, " :\n\t"); //NetDegree
        token = strtok(NULL, " :\n\t");
        nPin = atoi(token);
        for(int j = 0; j<nPin; j++)
        {
            fileRead.getline(ReadBuffer, 127);
            token = strtok(ReadBuffer, " :\n\t");
//            if(token[0] == 'P')  // Overlook the Nets Connecting with the boundary
//            {
//                char *n = token + 4;  // p
//                //cout << n << endl;
//                nt.pins[indexP].idPin = indexP;
//                nt.pins[indexP].index = atoi(n); // start from PAD_0
//                nt.pins[indexP].index = nt.pins[indexP].index + nt.nMod;
//
//                strcpy(nt.pins[indexP].pinName , token);
//                nt.pins[indexP].pinL = nt.pads[ indexP ].padL;      ///y////0222
//                indexP++;
//            }
//            else
//            {
//                char *n = token + 6;  // sb
//                //cout << n << endl;
//                nt.pins[indexP].idPin = indexP;
//                nt.pins[indexP].index = atoi(n) - 1; // start from BLOCK_1
//                nt.mods[atoi(n)-1].nNet++;
//                strcpy(nt.pins[indexP].pinName , token);
//                nt.pins[indexP].pinL = nt.mods[ nt.mod_NameToID[nt.pins[indexP].pinName] ].modL;      ///y////0222
//                indexP++;
//            }
            PIN pinTemp;

            strcpy(pinTemp.pinName, token);

            if( tmpnt.mod_NameToID.find(pinTemp.pinName) != tmpnt.mod_NameToID.end() )
            {

                int modID = tmpnt.mod_NameToID[pinTemp.pinName];

                if( tmpnt.mods[modID].modL > maxL )
                {
                    maxL = tmpnt.mods[modID].modL;
                }
                if( tmpnt.mods[modID].modL + tmpnt.mods[modID].nLayer -1  < minL )///f///1001add tmpnt.mods[modID].nLayer -1
                {
                    minL = tmpnt.mods[modID].modL + tmpnt.mods[modID].nLayer -1;///f///1001add tmpnt.mods[modID].nLayer -1
                }
                pinTemp.pinL = tmpnt.mods[modID].modL;


                if(tmpnt.mods[modID].nLayer!=1)///f///1025
                {
                    nStack++;
                }

            }
            else if( tmpnt.pad_NameToID.find(pinTemp.pinName) != tmpnt.pad_NameToID.end() )
            {
                int padID = tmpnt.pad_NameToID[pinTemp.pinName];
                minL = tmpnt.pads[padID].padL;
                pinTemp.pinL = tmpnt.pads[padID].padL;

            }
            else
            {
                cout << "error  : can't find connected component: " << pinTemp.pinName << endl;
                exit(EXIT_FAILURE);
            }


            tmpnt.pins.push_back( pinTemp );
            pinCount++;

        }

        if(maxL<minL)///f///1025 if this net's modules are all stack
        {
            minL=maxL;
        }


        if( nStack != 0 )///f///1025
        {
            for( int n = 0 ; n < nPin ; n++ )
            {
                int pini = tmpnt.pins.size() -1 -n;

                if( tmpnt.pins[pini].pinL < minL)
                {
                    tmpnt.pins[pini].pinL = minL;
                }
                else if( maxL < tmpnt.pins[pini].pinL )///impossible
                {
                    cerr << "error  : pin's layer is impossible great than maxmum layer in a net (dataProc.cpp)" << endl;
                    exit(EXIT_FAILURE);
                }
            }

        }

        if( (maxL - minL) > 0 )
        {
            nets[i].nTSV = maxL - minL;
            nets[i].nPin = nPin + nets[i].nTSV;
            for( int j = 0; j < nets[i].nTSV; j++)
            {
                char tsvNum[10];
                sprintf( tsvNum, "%d", tsvCount );

                PIN pinTemp;
                strcpy( pinTemp.pinName, "TSV" );
                strcat( pinTemp.pinName, tsvNum );
                pinTemp.pinL    = minL + j;
                pinTemp.flagTSV = true;

                MODULE modTemp;
                strcpy( modTemp.modName, pinTemp.pinName );
                modTemp.type    = HARD_BLK;
                modTemp.idMod   = tmpnt.nMod;
                modTemp.modW    = modTemp.modH = TSV_PITCH * AMP_PARA;
                modTemp.modL    = pinTemp.pinL;
                modTemp.modArea = (int)pow( TSV_PITCH, 2.0 );
                modTemp.minAR   = modTemp.maxAR = 1.0;
                modTemp.flagTSV = true;
				modTemp.Power	= 0;
				modTemp.Pdensity= 0;

                modTemp.nLayer = 1;///f///0926

                if(!InputOption.CompCorblivar)
                    tmpnt.totalModArea += modTemp.modArea;
                tmpnt.mods.push_back( modTemp );
                tmpnt.pins.push_back( pinTemp );
                tmpnt.mod_NameToID[ modTemp.modName ] = modTemp.idMod;
                tmpnt.nMod++;
                tmpnt.nPin++;
                tmpnt.nTSV++;
                tmpnt.nHard++;

                tsvCount++;
                pinCount++;
            }
            vector<PIN>::iterator pin_it;
            pin_it = tmpnt.pins.begin() + nets[i].head;
            sort( pin_it, tmpnt.pins.end(), SortByLayer );

        }
        else
        {
            nets[i].nTSV = 0;
            nets[i].nPin = nPin;
        }
    }

    fileRead.close();

    int netCount = 0;
    int sub_Head = 0;
    int sub_nPin = 0;



    tmpnt.nNet = nNet + tmpnt.nTSV;
    tmpnt.nets.resize( tmpnt.nNet );

    /// partition original nets into sub-nets
    for( int i = 0; i < nNet; i++ )
    {

        bool flagSubNet = false;	///< flag to mark whether the sub-net be updated
        bool flagHead   = true;		///< flag to mark whether to initialize the head for new sub-net

        for( int j = nets[i].head; j < nets[i].head + nets[i].nPin; j++ )
        {
            if( flagHead )
            {
                sub_Head = j;
                sub_nPin = 0;
                flagHead = false;
            }

            sub_nPin++;

            if( tmpnt.pins[j].flagTSV )
            {
                tmpnt.nets[netCount].nTSV++;
            }

            if( tmpnt.pins[j].flagTSV && !flagSubNet )
            {
                tmpnt.nets[netCount].idNet = netCount;
                tmpnt.nets[netCount].head  = sub_Head;
                tmpnt.nets[netCount].nPin  = sub_nPin;
                tmpnt.nets[netCount].flag  = '0';

                netCount++;
                flagSubNet = true;
                flagHead = true;
                j--; ///put TSV into next subnet
            }
            else
            {
                flagSubNet = false;
            }
        }
        tmpnt.nets[netCount].idNet = netCount;
        tmpnt.nets[netCount].head  = sub_Head;
        tmpnt.nets[netCount].nPin  = sub_nPin;
        tmpnt.nets[netCount].flag  = '0';

        netCount++;
        sub_nPin = 0;
    }
    nets.clear();
    vector<NET> (nets).swap(nets);

    /// assign pad id
    for( int i = 0; i < tmpnt.nPad; i++ )
    {
        tmpnt.pads[i].idPad += tmpnt.nMod;
    }

    /// assign pin id
    for( int i = 0; i < tmpnt.nPin; i++ )
    {
        tmpnt.pins[i].idPin = i;
    }

    /// assign pin index \n
    /// assign net layer \n
    /// count number of nets which link to mod[index] \n
    for( int i = 0; i < tmpnt.nNet; i++ )
    {
        for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
        {
            if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
            {
                tmpnt.pins[j].index = tmpnt.mod_NameToID[ tmpnt.pins[j].pinName ];
                tmpnt.mods[ tmpnt.pins[j].index ].nNet++;
            }
            else if( tmpnt.pad_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.pad_NameToID.end() )
            {
                tmpnt.pins[j].index = tmpnt.pad_NameToID[ tmpnt.pins[j].pinName ] + tmpnt.nMod;
            }
            else
            {
                cout << "error  : can't find connected component: " << tmpnt.pins[j].pinName << endl;
                exit(EXIT_FAILURE);
            }

            if( tmpnt.pins[j].pinL > tmpnt.nets[i].netL )
            {
                tmpnt.nets[i].netL = tmpnt.pins[j].pinL;
            }
        }
    }

    /// allocate the memory for nets which link to mod[index] (mod[index].pNet)
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        try
        {
            tmpnt.mods[i].pNet = new unsigned int [tmpnt.mods[i].nNet];
        }
        catch( bad_alloc &bad )
        {
            cout << "error  : run out of memory on tmpnt.mods[" << tmpnt.mods[i].idMod << "].pNet" << endl;
            exit(EXIT_FAILURE);
        }
        tmpnt.mods[i].nNet = 0;
    }

    /// construct the relationship between nets and modules
    for( int i = 0; i < tmpnt.nNet; i++ )
    {
        for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
        {
            if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
            {
                tmpnt.mods[ tmpnt.pins[j].index ].pNet[ tmpnt.mods[tmpnt.pins[j].index].nNet ] = tmpnt.nets[i].idNet;
                tmpnt.mods[ tmpnt.pins[j].index ].nNet++;
                /*if(tmpnt.mods[ tmpnt.pins[j].index ].flagTSV)
                {

                    cout <<tmpnt.pins[j].index << " " << tmpnt.mods[ tmpnt.pins[j].index ].pNet[ tmpnt.mods[tmpnt.pins[j].index].nNet ] << endl;
                    getchar();
                }*/
            }
        }
    }

    ///count the number of modules at each layer
    tmpnt.LayernMod.resize(tmpnt.nLayer);
    for( int i=0; i < tmpnt.nLayer; i++ )
        tmpnt.LayernMod[i] = 0;

    for( int i=0; i < tmpnt.nMod; i++ )
        tmpnt.LayernMod[ tmpnt.mods[i].modL ]++;

    //print number of modules at each layer
    /*for( int i=0; i < tmpnt.nLayer; i++ )
        cout << tmpnt.LayernMod[ i ] << endl;

    getchar();*/

    //test net pin
    /*for(int i=0; i<tmpnt.nNet; i++)
    {
        cout << "net id:    " << tmpnt.nets[i].idNet << endl;
        cout << "net layer: " << tmpnt.nets[i].netL << endl;
        for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
        {
            cout << "mod name:  " << tmpnt.mods[ tmpnt.pins[j].index ].modName << endl;
            cout << "pin name:  " << tmpnt.pins[j].pinName << endl;
            cout << "pin layer: " << tmpnt.pins[j].pinL << endl;

        }
        getchar();
    }*/
    /*int net_index=tmpnt.mods[911].pNet[0];
    cout << "911: " << tmpnt.mods[911].nNet << endl;
    cout << net_index << ": " << tmpnt.nets[ net_index ].nTSV << endl;
    getchar();*/

    return;
}
void IO::IBM::ReadNetFile (string benName, NETLIST &nt)
{
	// cout << fileIn << endl;
	string fileIn = benName + ".nets";
	ifstream fileRead(fileIn.c_str());


	/*if(fileRead == NULL)
	{
		cout << "Error in opening " << fileIn << endl;
		exit(1);
	}*/
	char ReadBuffer[128];
	char *token = NULL;

	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);

	fileRead.getline(ReadBuffer, 127);
	token = strtok(ReadBuffer, " :\n\t");
	if(!strncmp(token, "NumNets", 7))
	{
		token = strtok(NULL, " :\n\t");
		nt.nNet = atoi(token);
	}

	fileRead.getline(ReadBuffer, 127);
	token = strtok(ReadBuffer, " :\n\t");
	if(!strncmp(token, "NumPins", 7))
	{
		token = strtok(NULL, " :\n\t");
		nt.nPin = atoi(token);
	}

	fileRead.getline(ReadBuffer, 127);

	nt.nets.resize(nt.nNet);
	nt.pins.resize(nt.nPin);

	// construct relationship between nt.nets and nt.pins
	int indexP = 0;
	for(int i = 0; i<nt.nNet; i++)
	{
		nt.nets[i].idNet = i;
		nt.nets[i].head = indexP;
		nt.nets[i].flag = 0;
		fileRead.getline(ReadBuffer, 127);
		token = strtok(ReadBuffer, " :\n\t"); //NetDegree
		token = strtok(NULL, " :\n\t");
		nt.nets[i].nPin = atoi(token);
		for(int j = 0; j<nt.nets[i].nPin; j++)
		{
			fileRead.getline(ReadBuffer, 127);
			token = strtok(ReadBuffer, " :\n\t");
			if(token[0] == 'P')  // Overlook the Nets Connecting with the boundary
			{
				char *n = token + 4;  // p
				//cout << n << endl;
				nt.pins[indexP].idPin = indexP;
				nt.pins[indexP].index = atoi(n); // start from PAD_0
				nt.pins[indexP].index = nt.pins[indexP].index + nt.nMod;
				strcpy(nt.pins[indexP].pinName, token);
				indexP++;
			}
			else
			{
				char *n = token + 6;  // sb
				//cout << n << endl;
				nt.pins[indexP].idPin = indexP;
				nt.pins[indexP].index = atoi(n) - 1; // start from BLOCK_1
				nt.mods[atoi(n)-1].nNet++;
				strcpy(nt.pins[indexP].pinName, token);
				indexP++;
			}
		}
	}
	// construct relationship between nt.nets and nt.mods
	for(int i = 0; i<nt.nMod; i++)  // Alloc Memory for edge array in mods
	{
		nt.mods[i].pNet = (unsigned int *)malloc(sizeof(unsigned int) * nt.mods[i].nNet);
		if(nt.mods[i].pNet == NULL)
		{
			cerr << "Out of Mem on nt.mods[" << i << "].pNet" << endl;
			exit(1);
		}
		nt.mods[i].nNet = 0;  // for next step to assign nt.mods.pNet
	}

	for(int i = 0; i<nt.nNet; i++)
	{
		for(int j = nt.nets[i].head; j<nt.nets[i].head + nt.nets[i].nPin; j++)
		{
			if (nt.pins[j].index < nt.nMod)
			{
				nt.mods[nt.pins[j].index].pNet[nt.mods[nt.pins[j].index].nNet] = i;
				nt.mods[nt.pins[j].index].nNet++;
			}
		}
	}

	// buffNet used in swap process
	buffNet = (unsigned int *)malloc(sizeof(unsigned int) * nt.nNet);
	if(buffNet == NULL)
	{
		cout << "Run Out of Mem on buffNet" << endl;
	}

	///f///
	/*maxNumNetB = 0;
	  for(int i=0; i<nt.nMod; i++)
	  {
	  maxNumNetB = (nt.mods[i].nNet > maxNumNetB) ? nt.mods[i].nNet : maxNumNetB;
	  }*/


	fileRead.close();
	return;
}


void IO::IBM::ReadPlFile (string benName, NETLIST &nt)
{
	// cout << fileIn << endl;
	string fileIn = benName + ".pl";

	ifstream fileRead(fileIn.c_str());

	/*if(fileRead == NULL)
	{
		cerr << "Error in opening " << fileIn << endl;
		exit(1);
	}*/
	char ReadBuffer[128];
	char *token = NULL;

	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	fileRead.getline(ReadBuffer, 127);
	for(int i = 0; i<nt.nMod; i++)
	{
		fileRead.getline(ReadBuffer, 127);
		token = strtok(ReadBuffer, " :\n\t");
		token = strtok(NULL, " :\n\t");
		nt.mods[i].LeftX = atoi(token) * AMP_PARA;
		token = strtok(NULL, " :\n\t");
		nt.mods[i].LeftY = atoi(token) * AMP_PARA;
	}

	fileRead.getline(ReadBuffer, 127);
	for(int i = 0; i<nt.nPad; i++)
	{
		// cout << "i : " << i << endl;
		fileRead.getline(ReadBuffer, 127);
		token = strtok(ReadBuffer, " :\n\t");
		//strcpy(nt.pads[i].padName,token);       ///< 2013-11-20 remove
		strcpy(nt.pads[i].padName,token);///f///0602

		nt.pad_NameToID[nt.pads[i].padName] = i;

		nt.pads[i].idPad = i + nt.nMod;
		token = strtok(NULL, " :\n\t");
		nt.pads[i].x = (int)(atof(token) * ((float)AMP_PARA));
		token = strtok(NULL, " :\n\t");
		nt.pads[i].y = (int)(atof(token) * ((float)AMP_PARA));
		///f///nt.pads[i].padW = 10000;
		///f///nt.pads[i].padH = 10000;
		///f///nt.pads[i].padX = nt.pads[i].x;// / AMP_PARA;
		///f///nt.pads[i].padY = nt.pads[i].y;// / AMP_PARA;
		///f///nt.pads[i].orientation = 0;
	}
	fileRead.close();
	return;
}
// GSRC/CAD

void IO::CAD::ConvertBlock(string blockFile, NETLIST& tmpnt, Lib_vector& lib_vector)
{
	int num_tech=0;
	int which_lib_top=-3;
	int which_lib_bot=-3;
	string fileName_out = /*"temp/"+*/blockFile+"_"+to_string(InputOption.test_mode)+".blocks";
	string fileName_in  = blockFile+".txt";
	ifstream fin;
	ofstream fout(fileName_out.c_str());
	fin.open(fileName_in.c_str());
	if(!fin.is_open() || !fout.is_open()){
		cout<<"IO error\n";
		exit(EXIT_FAILURE);
	}
	bool read1=false; //之後註解掉
	fout<<"UCSC blocks 1.0\n# Created      : Fri Dec 08 20:11:56 PST 2000\n# User         : huaizhi@shawnee\n# Platform     : SunOS 5.8 sparc\n\nNumSoftRectangularBlocks : 0\n";
	stringstream sstemp;
	string strtemp;
	while( getline(fin, strtemp) )
	{
		sstemp << strtemp;

		sstemp >> strtemp;
		if( strtemp == "" || strtemp.substr(0, 1) == "#" )
		{
			sstemp.str( string() );
			sstemp.clear();
			continue;
		}
        else if(strtemp=="NumTechnologies"){
            
            num_tech=0;
            sstemp>>num_tech;
            //nt.tech+=num_tech;
			lib_vector.lib.resize(num_tech);
            sstemp.str( string() );
		    sstemp.clear();
            
        }
        //先假設只讀一個tech 之後要改
        //只讀了大小 其他資訊陸續補上
        else if(strtemp=="Tech"){
             
            if(num_tech>0){
            int library=0;
            sstemp>>strtemp>>library;
            lib_vector.lib[num_tech-1].name=strtemp;
            lib_vector.lib[num_tech-1].lib_cell.resize(library);
            sstemp.str( string() );
			sstemp.clear();
            
            for(int i=0;i<library;i++){
                getline(fin, strtemp);
                sstemp << strtemp;

		        sstemp >> strtemp;
                int p=0;//pin_num
                string name;
                int w=0;
                int h=0;
                if(strtemp=="LibCell"){

                   
                    sstemp >> strtemp >> name >> w >> h >> p;

					if(strtemp=="Y"){
						lib_vector.lib[num_tech-1].lib_cell[i].is_m=true;
					}
                    lib_vector.lib[num_tech-1].lib_cell[i].id=i;
                    lib_vector.lib[num_tech-1].lib_cell[i].h=h;
                    lib_vector.lib[num_tech-1].lib_cell[i].w=w;
                    lib_vector.lib[num_tech-1].lib_cell[i].p=p;
                   //--------------------HYH0904-------------------------
					/*if(lib_vector.lib[num_tech-1].lib_cell[i].is_m)
					{
						lib_vector.lib[num_tech-1].lib_cell[i].h++;
						lib_vector.lib[num_tech-1].lib_cell[i].w++;
					}*/
				   //------------------------------------------------
                    
                    strcpy( lib_vector.lib[num_tech-1].lib_cell[i].name, name.c_str() );
                    lib_vector.lib[num_tech-1].lib_NameToID[lib_vector.lib[num_tech-1].lib_cell[i].name]=i;
                    sstemp.str( string() );
			        sstemp.clear();
                    for(int j=0;j<p;j++){
                        getline(fin, strtemp);
                        sstemp.str( string() );
                        sstemp.clear();
                    }

                    //cout<<lib.lib_cell[library].w<<" "<<lib.lib_cell[library].h<<" "<<lib.lib_cell[library].p<<"haha\n";
                }
                else{
                    cout<<"error! Should be LibCell";
                }
                sstemp.str( string() );
                sstemp.clear();
                //cout<<lib.lib_cell[library].w<<" ";
            }


            num_tech--;
            }
            sstemp.str( string() );
		    sstemp.clear();
        }
		else if (strtemp == "DieSize") {
			string w_s;
			string h_s;
			sstemp >> strtemp >> strtemp >> w_s >> h_s;
			int w = stoi(w_s);
			int h = stoi(h_s);
			tmpnt.SetFixedOutline(w, h);
			//cout << w << " " << h << "\n";
			sstemp.str(string());
			sstemp.clear();
		}

		else if(strtemp=="TopDieMaxUtil"){
			double util;
			sstemp>>util;
			tmpnt.toputil=util;
			sstemp.str(string());
			sstemp.clear();
		}
		else if(strtemp=="BottomDieMaxUtil"){
			double util;
			sstemp>>util;
			tmpnt.botutil=util;
			sstemp.str(string());
			sstemp.clear();
		}
		else if(strtemp == "TopDieRows"){
			double row_high_topt;
			double NumRows_topt;
			double Xmin_topt;
			double Ymin_topt;
			double Xmax_topt;
			sstemp>>Xmin_topt>>Ymin_topt>>Xmax_topt>>row_high_topt>>NumRows_topt;
			tmpnt.row_high_top=row_high_topt;
			tmpnt.NumRows_top=NumRows_topt;
			tmpnt.Xmin_top=Xmin_topt;
			tmpnt.Ymin_top=Ymin_topt;
			tmpnt.Xmax_top=Xmax_topt;
			tmpnt.Ymax_top=Ymin_topt + row_high_topt*NumRows_topt;
			sstemp.str(string());
			sstemp.clear();

		}
		else if(strtemp == "BottomDieRows"){
			double row_high_bott;
			double NumRows_bott;
			double Xmin_bott;
			double Ymin_bott;
			double Xmax_bott;
			sstemp>>Xmin_bott>>Ymin_bott>>Xmax_bott>>row_high_bott>>NumRows_bott;
			tmpnt.row_high_bot=row_high_bott;
			tmpnt.NumRows_bot=NumRows_bott;
			tmpnt.Xmin_bot=Xmin_bott;
			tmpnt.Ymin_bot=Ymin_bott;
			tmpnt.Xmax_bot=Xmax_bott;
			tmpnt.Ymax_bot=Ymin_bott + row_high_bott*NumRows_bott;
			sstemp.str(string());
			sstemp.clear();

		}
		else if(strtemp=="TopDieTech"){
			string tech;
			sstemp>>tech;
			tmpnt.toptech=tech;
			sstemp.str(string());
			sstemp.clear();
		}
		else if(strtemp=="BottomDieTech"){
			string tech;
			sstemp>>tech;
			tmpnt.bottech=tech;
			sstemp.str(string());
			sstemp.clear();

			string tech_name=tmpnt.toptech;
			
			for(int j=0;j<lib_vector.lib.size();j++){
				if(lib_vector.lib[j].name==tech_name){
					which_lib_top=j;
					break;
				}
			}
			if(which_lib_top==-3){
				cout<<"\n\n\n\n\n\n\n\ntop lib error\n\n\n\n\n\n\n";
			}
			tech_name=tmpnt.bottech;
			for(int j=0;j<lib_vector.lib.size();j++){
				if(lib_vector.lib[j].name==tech_name){
					which_lib_bot=j;
					break;
				}
			}
			if(which_lib_bot==-3){
				cout<<"\n\n\n\n\n\n\n\nbot lib error\n\n\n\n\n\n\n";
			}
			//cout<<"top: "<<which_lib_top<<" bot: "<<which_lib_bot<<"\n";

		}
		else if(strtemp=="TerminalSize"){
			double w;
			double h;
			sstemp>>w>>h;
			tmpnt.terminal_w=w;
			tmpnt.terminal_h=h;
			sstemp.str(string());
			sstemp.clear();
		}
		else if(strtemp=="TerminalSpacing"){
			double space;
			sstemp>>space;
			tmpnt.terminal_space=space;
			sstemp.str(string());
			sstemp.clear();
		}
		else if(strtemp=="TerminalCost"){
			double cost;
			sstemp>>cost;
			tmpnt.terminal_cost=cost;
			sstemp.str(string());
			sstemp.clear();
		}
		else if( strtemp == "NumInstances" ){
			int num_instances = 0;
			sstemp >> num_instances;
			fout<<"NumHardRectilinearBlocks : "<<num_instances<<"\n";
			fout<<"NumTerminals : 0\n\n";
			sstemp.str( string() );
			sstemp.clear();
			for(int i=0;i<num_instances;i++){
				getline(fin, strtemp);
				//cout<<strtemp<<" ";
                sstemp << strtemp;

		        sstemp >> strtemp;
				
                if(strtemp=="Inst"){
					string name ;
                    string type ;
			        sstemp >> name>>type;
					if( lib_vector.lib[which_lib_top].lib_NameToID.find(type) != lib_vector.lib[which_lib_top].lib_NameToID.end() ||lib_vector.lib[which_lib_bot].lib_NameToID.find(type) != lib_vector.lib[which_lib_bot].lib_NameToID.end() ){
						if(lib_vector.lib[0].lib_cell[lib_vector.lib[0].lib_NameToID[type]].is_m)
                        	fout<<name<<" hardrectilinear 4 (0, 0) (0, "<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].h<<") ("<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].w<<", "<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].h<<") ("<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].w<<", 0) Y "<<lib_vector.lib[which_lib_bot].lib_cell[lib_vector.lib[which_lib_bot].lib_NameToID[type]].w<<" "<<lib_vector.lib[which_lib_bot].lib_cell[lib_vector.lib[which_lib_bot].lib_NameToID[type]].h<<"\n";
						else
							fout<<name<<" hardrectilinear 4 (0, 0) (0, "<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].h<<") ("<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].w<<", "<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].h<<") ("<<lib_vector.lib[which_lib_top].lib_cell[lib_vector.lib[which_lib_top].lib_NameToID[type]].w<<", 0) N "<<lib_vector.lib[which_lib_bot].lib_cell[lib_vector.lib[which_lib_bot].lib_NameToID[type]].w<<" "<<lib_vector.lib[which_lib_bot].lib_cell[lib_vector.lib[which_lib_bot].lib_NameToID[type]].h<<"\n";
                    }
                    else{
                        cout<<"error! No such Library"<<i<<"\n";
                    }
				}
				else{
					cout<<"no enough inst"<<i<<"\n";
				}
				sstemp.str( string() );
				sstemp.clear();
				
			}
			//fout<<"\np1 terminal";
			fout.close();
			sstemp.str( string() );
			sstemp.clear();
			
		}
				else if(strtemp=="NumNets"){
				string fileName_out = /*"temp/"+*/blockFile+"_"+to_string(InputOption.test_mode)+".nets";
				ofstream fout(fileName_out.c_str());
				if(!fout.is_open()){
					cout<<"output netlist file error!\n";
				}
				fout<<"UCLA nets 1.0\n# Created      : 2000\n# User         : Huaizhi Wu\n# Platform     : SunOS 5.6 sparc SUNW\n\n";
				int numnet;
				sstemp>>numnet;
				fout<<"NumNets : "<<numnet<<"\n";
				stringstream for_net;
				for_net.str( string() );
				for_net.clear();
				
				int num_pin=0;
				for(int i=0;i<numnet;i++){
					sstemp.str( string() );
					sstemp.clear();
					getline(fin, strtemp);
					sstemp << strtemp;

		        	sstemp >> strtemp;
					if(strtemp=="Net"){
						vector<string> repeat;
						int degree=0;
						string netname;
						sstemp>>netname>>degree;
						num_pin+=degree;
						//for_net<<"NetDegree : "<<degree<<"\n";
						vector<string> numbers(degree,"");
						int act_degree=degree;
						int insert_p=0;
						for(int j=0;j<degree;j++){
							sstemp.str( string() );
							sstemp.clear();
							getline(fin, strtemp);
							sstemp << strtemp;

							sstemp >> strtemp;
							if(strtemp=="Pin"){
								//不考慮pin腳位
								sstemp>>strtemp;
								string f_s("/");
								std::size_t found=strtemp.find(f_s);
								int find_pos=static_cast<int>(found);
								string fiind=strtemp.substr(0,find_pos);
								if(repeat.size()==0){
								
									repeat.push_back(fiind);
									//for_net<<fiind<<" B\n";
									string tttt=fiind+" B\n";
									numbers[insert_p]=tttt;
									insert_p++;
								}
								else{
									bool foun=false;
									for(int z=0;z<repeat.size();z++){
										if(fiind==repeat[z]){
											foun=true;
											break;
										}
									}
									if(foun==false){
										repeat.push_back(fiind);
										string tttt=fiind+" B\n";
										numbers[insert_p]=tttt;
										insert_p++;
									}
									else{
										num_pin--;
										act_degree--;
									}
								}
							}
							else{
								cout<<"error! should be pin\n";
							}
						}
						for_net<<netname<<" NetDegree : "<<act_degree<<"\n";
						for(int z=0;z<act_degree;z++){
							if(numbers[z]!=""){
								for_net<<numbers[z];
							}
							else{
								cout<<"write net file error\n";
							}
						}
						
					}
					else{
						cout<<"error! should be Net\n";
					}
				}
				fout<<"NumPins : "<<num_pin<<"\n";
				fout<<for_net.str();
				for_net.str( string() );
				for_net.clear();
				fout.close();
			}
		sstemp.str( string() );
		sstemp.clear();
	}
	
	fin.close();
	
}




void IO::GSRC::ReadBlockFile( string blockFile, NETLIST& tmpnt )
{

	string fileName = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ ".blocks";
	ifstream fin;
	fin.open( fileName.c_str() );
	if( !fin.is_open() )
	{
		cout << "error  : unable to open " << fileName << endl;
		exit(EXIT_FAILURE);
	}

	string strtemp;

	do
	{
		fin >> strtemp;
	}
	while( strtemp != "NumSoftRectangularBlocks" );

	fin >> strtemp >> tmpnt.nSoft;
	fin >> strtemp >> strtemp >> tmpnt.nHard;
	fin >> strtemp >> strtemp >> tmpnt.nPad;


	tmpnt.nMod = tmpnt.nSoft + tmpnt.nHard;
	//cout<<"\n\n\n"<<tmpnt.nMod<<"\n\n\n";
	tmpnt.mods.resize( tmpnt.nMod );
	tmpnt.pads.resize( tmpnt.nPad );

	int modCount = 0;
	int padCount = 0;
	for( int i = 0; i < tmpnt.nMod + tmpnt.nPad; i++ )
	{
		char tmpName[100] = "\0";

		fin >> tmpName >> strtemp;
		if( strtemp == "softrectangular" )
		{
			strcpy(tmpnt.mods[modCount].modName, tmpName);

			fin >> tmpnt.mods[modCount].modArea
				>> tmpnt.mods[modCount].minAR
				>> tmpnt.mods[modCount].maxAR;

            ///enlarge block
			tmpnt.mods[modCount].modArea *= pow(InputOption.EnlargeBlockRatio,2);

			tmpnt.mods[modCount].modW = tmpnt.mods[modCount].modH = (int)(sqrt(tmpnt.mods[i].modArea) * AMP_PARA);

			tmpnt.mods[modCount].idMod = modCount;
			tmpnt.mods[modCount].type = SOFT_BLK;
			tmpnt.mod_NameToID[ tmpnt.mods[modCount].modName ] = modCount;
			tmpnt.totalModArea += tmpnt.mods[modCount].modArea;
			modCount++;
		}
		else if( strtemp == "hardrectilinear" )
		{
			strcpy(tmpnt.mods[modCount].modName, tmpName);

			fin >> strtemp;				///< drop 4
			fin >> strtemp >> strtemp;	///< drop (0, 0)
			fin >> strtemp >> strtemp;	///< drop (0, H)
			fin >> strtemp;				///< read (W,
			tmpnt.mods[modCount].modW = atoi(strtemp.substr(1, strtemp.size()-2).c_str());
			fin >> strtemp;				///< read H)
			tmpnt.mods[modCount].modH = atoi(strtemp.substr(0, strtemp.size()-1).c_str());
			fin >> strtemp >> strtemp;	///< drop (W, H)
			fin>>strtemp;
			if(strtemp=="Y"){
				tmpnt.mods[modCount].is_m=true;
			}
			
			fin >>tmpnt.mods[modCount].modW_bot;
			fin >>tmpnt.mods[modCount].modH_bot;
            ///enlarge block
			tmpnt.mods[modCount].modW *= InputOption.EnlargeBlockRatio;
			tmpnt.mods[modCount].modH *= InputOption.EnlargeBlockRatio;
			tmpnt.mods[modCount].modW_bot *= InputOption.EnlargeBlockRatio;
			tmpnt.mods[modCount].modH_bot *= InputOption.EnlargeBlockRatio;

			tmpnt.mods[modCount].modArea = tmpnt.mods[modCount].modW * tmpnt.mods[modCount].modH;
			tmpnt.mods[modCount].modArea_bot = tmpnt.mods[modCount].modW_bot * tmpnt.mods[modCount].modH_bot;
			tmpnt.mods[modCount].minAR = tmpnt.mods[modCount].maxAR = (float)tmpnt.mods[modCount].modW / (float)tmpnt.mods[modCount].modH;
			tmpnt.mods[modCount].minAR_bot = tmpnt.mods[modCount].maxAR_bot = (float)tmpnt.mods[modCount].modW_bot / (float)tmpnt.mods[modCount].modH_bot;
			tmpnt.mods[modCount].modW *= AMP_PARA;
			tmpnt.mods[modCount].modH *= AMP_PARA;
			tmpnt.mods[modCount].modW_bot *= AMP_PARA;
			tmpnt.mods[modCount].modH_bot *= AMP_PARA;
			tmpnt.mods[modCount].idMod = modCount;
			tmpnt.mods[modCount].type = HARD_BLK;
			tmpnt.mod_NameToID[ tmpnt.mods[modCount].modName ] = modCount;
			tmpnt.totalModArea += tmpnt.mods[modCount].modArea;
			modCount++;
		}
		
		else if( strtemp == "terminal" )
		{
			strcpy(tmpnt.pads[padCount].padName, tmpName);
			tmpnt.pad_NameToID[ tmpnt.pads[padCount].padName ] = padCount;
			padCount++;
		}
		else
		{
			cout << "error  : unrecognized module type in .block file: " << strtemp << endl;
			exit(EXIT_FAILURE);
		}
	}
	fin.close();
}




void IO::GSRC::ReadPlFile( string plFile, NETLIST& tmpnt )
{

	string fileName = /*"temp/"+*/plFile + ".pl";
	ifstream fin;
	fin.open( fileName.c_str());
	if( !fin.is_open() )
	{
		cout << "error  : unable to open " << fileName << endl;
		exit(EXIT_FAILURE);
	}

	string strtemp;

	getline( fin, strtemp );	///< drop UCSC blocks 1.0
	getline( fin, strtemp );	///< drop # Created      : Fri Dec 08 20:11:56 PST 2000
	getline( fin, strtemp );	///< drop # User         : huaizhi@shawnee
	getline( fin, strtemp );	///< drop # Platform     : SunOS 5.8 sparc

	for( int i = 0; i < tmpnt.nMod + tmpnt.nPad; i++ )
	{
		double LeftX;
		double LeftY;

		fin >> strtemp >> LeftX >> LeftY;

		if( tmpnt.mod_NameToID.find(strtemp) != tmpnt.mod_NameToID.end() )
		{
			int modID = tmpnt.mod_NameToID[strtemp];
			tmpnt.mods[modID].LeftX = (int)(LeftX * AMP_PARA);
			tmpnt.mods[modID].LeftY = (int)(LeftY * AMP_PARA);
		}
		else if( tmpnt.pad_NameToID.find(strtemp) != tmpnt.pad_NameToID.end() )///f///1005modify
		{
			int padID = tmpnt.pad_NameToID[strtemp];
			tmpnt.pads[padID].x = (int)(LeftX * AMP_PARA);
			tmpnt.pads[padID].y = (int)(LeftY * AMP_PARA);
		}
		else
		{
			cout << "error  : unrecognized module/pad in .pl file: " << strtemp << endl;
			exit(EXIT_FAILURE);
		}
	}

	fin.close();


	//char* checkFile;
	//checkFile = strrchr( fileName, '/' );
	//checkFile++;
	//strcat( checkFile, ".check" );
	//
	//ofstream fout;
	//fout.open( checkFile );

	//fout << "UCSC blocks 1.0" << endl;
	//fout << "# Created      : Fri Dec 08 20:11:56 PST 2000" << endl;
	//fout << "# User         : huaizhi@shawnee" << endl;
	//fout << "# Platform     : SunOS 5.8 sparc" << endl;
	//fout << endl;
	//for( int i = 0; i < tmpnt.nMod; i++ )
	//{
	//	fout << tmpnt.mods[i].modName << "\t" << (float)tmpnt.mods[i].LeftX/(float)AMP_PARA << "\t" << (float)tmpnt.mods[i].LeftY/(float)AMP_PARA << endl;
	//}
	//fout << endl;
	//for( int i = 0; i < tmpnt.nPad; i++ )
	//{
	//	fout << tmpnt.pads[i].padName << "\t" << (float)tmpnt.pads[i].x/(float)AMP_PARA << "\t" << (float)tmpnt.pads[i].y/(float)AMP_PARA << endl;
	//}

	//fout.close();
}
void IO::GSRC::ConvertLayer(NETLIST_o nt_o,NETLIST & nt)
{
	nt.nLayer=2;
	nt.topn=0;
	nt.botn=0;
	//check if two netlist have same mod_num
	if(nt_o.num_mod!=nt.GetnMod()){
		cout<<"two netlist have different mod size\n";
	}
	else{
		//cout<<"two netlist have same mod size\n";
		for(int i=0;i<nt_o.num_mod;i++){
			nt.mods[i].modL=nt_o.mods[i].tier;
			nt.mods[i].modW=nt_o.mods[i].mod_w;
			nt.mods[i].modH=nt_o.mods[i].mod_h;
			nt.mods[i].modArea==nt_o.mods[i].area;
			if(nt.mods[i].modL==0){
				nt.topn++;
			}
			else{
				nt.botn++;
			}
		}
	}

}


void IO::GSRC::ReadLayerFile( string layerFile, NETLIST& tmpnt )
{

	string fileName = layerFile +"_"+to_string(InputOption.test_mode)+ ".layer";
	ifstream fin;
	fin.open( fileName.c_str() );
	if( !fin.is_open() )
	{
		cout << "error  : unable to open " << fileName << endl;
		exit(EXIT_FAILURE);
	}

	string strtemp;

	fin >> strtemp >> strtemp >> tmpnt.nLayer;


	
	//cout<<tmpnt.nLayer;
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		fin >> strtemp;
		if( tmpnt.mod_NameToID.find(strtemp) != tmpnt.mod_NameToID.end() )
		{
			int modID = tmpnt.mod_NameToID[strtemp];
			fin >> tmpnt.mods[modID].modL;
			if(tmpnt.mods[modID].modL==0){ ///找上曾
				tmpnt.mods[modID].modW = tmpnt.mods[modID].modW_bot;
				tmpnt.mods[modID].modH = tmpnt.mods[modID].modH_bot;
				tmpnt.mods[modID].modArea = tmpnt.mods[modID].modArea_bot;
				tmpnt.mods[modID].minAR = tmpnt.mods[modID].minAR_bot;
				tmpnt.mods[modID].maxAR = tmpnt.mods[modID].maxAR_bot;

			}
			else{

			}
		}
		else
		{
			cout << "error  : unrecognized module in .layer file: " << strtemp << endl;
			exit(EXIT_FAILURE);
		}
	}

	for( int i = 0; i < tmpnt.nPad; i++ )
	{
		tmpnt.pads[i].padL = 0;
	}

	fin.close();


/*
	string checkFile = "./checkLayer.txt";
	//
	ofstream fout;
	fout.open( checkFile.c_str() );

	fout << "Layer number: " << tmpnt.nLayer << endl;
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		fout << tmpnt.mods[i].modName << "\t" << tmpnt.mods[i].modL << endl;
	}

	fout.close();*/
}


/*void IO::GSRC::Transfer_data(NETLIST& nt,NETLIST_o nt_o){
	nt.nSoft=0;
	nt.nPad=0;
	nt.nHard=nt_o.num_mod;
	nt.nMod=nt.nSoft+nt.nHard;


	nt.mods.resize( nt.nMod );
	nt.pads.resize( nt.nPad );
	for(int i=0;i<nt_o.num_mod;i++){
		nt.mods[i].modName=nt_o.mods[i].modName;
		nt.mods[i].modW=nt_o.mods[i].modW;
		nt.mods[i].modH=nt_o.mods[i].modH;
		nt.mods[i].is_m=nt_o.mods[i].is_macro;



			nt.mods[i].modW *= InputOption.EnlargeBlockRatio;
			nt.mods[i].modH *= InputOption.EnlargeBlockRatio;
			nt.mods[i].modW_bot *= InputOption.EnlargeBlockRatio;
			nt.mods[i].modH_bot *= InputOption.EnlargeBlockRatio;

			nt.mods[i].modArea = nt.mods[i].modW * nt.mods[i].modH;
			nt.mods[i].modArea_bot = nt.mods[i].modW_bot * nt.mods[i].modH_bot;
			nt.mods[i].minAR = nt.mods[i].maxAR = (float)nt.mods[i].modW / (float)nt.mods[i].modH;
			nt.mods[i].minAR_bot = nt.mods[i].maxAR_bot = (float)nt.mods[i].modW_bot / (float)nt.mods[i].modH_bot;
			nt.mods[i].modW *= AMP_PARA;
			nt.mods[i].modH *= AMP_PARA;
			nt.mods[i].modW_bot *= AMP_PARA;
			nt.mods[i].modH_bot *= AMP_PARA;
			nt.mods[i].idMod = i;
			nt.mods[i].type = HARD_BLK;
			nt.mod_NameToID[ nt.mods[i].modName ] = i;
			nt.totalModArea += nt.mods[i].modArea;
	}
}*/


void IO::GSRC::ReadNetFile( string netFile, NETLIST& tmpnt )
{

	string fileName = /*"temp/"+*/netFile +"_"+to_string(InputOption.test_mode)+ ".nets";
	ifstream fin;
	fin.open( fileName.c_str() );
	if( !fin.is_open() )
	{
		cout << "error  : unable to open " << fileName << endl;
		exit(EXIT_FAILURE);
	}

	string strtemp;

	do
	{
		fin >> strtemp;
	}
	while( strtemp != "NumNets" );

	vector<NET> nets;
	int nNet = 0;

	fin >> strtemp >> nNet;
	fin >> strtemp >> strtemp >> tmpnt.nPin;

	nets.resize(nNet);

	int pinCount = 0;
	int tsvCount = 0;

	/// read nets & insert TSVs
	for( int i = 0; i < nNet; i++ )
	{
		int maxL = 0;
		int minL = MAX_INT;
		int nPin = 0;

		nets[i].idNet = i;
		nets[i].head = pinCount;
		string netname;
		fin >>netname>> strtemp >> strtemp >> nPin;
		strcpy(nets[i].netName,netname.c_str());
		for( int j = 0; j < nPin; j++ )
		{
			PIN pinTemp;

			fin >> pinTemp.pinName >> strtemp;

			if( tmpnt.mod_NameToID.find(pinTemp.pinName) != tmpnt.mod_NameToID.end() )
			{
				int modID = tmpnt.mod_NameToID[pinTemp.pinName];
				if( tmpnt.mods[modID].modL > maxL )
				{
					maxL = tmpnt.mods[modID].modL;
				}
				if( tmpnt.mods[modID].modL < minL )
				{
					minL = tmpnt.mods[modID].modL;
				}
				pinTemp.pinL = tmpnt.mods[modID].modL;
			}
			else if( tmpnt.pad_NameToID.find(pinTemp.pinName) != tmpnt.pad_NameToID.end() )
			{
				int padID = tmpnt.pad_NameToID[pinTemp.pinName];
				minL = tmpnt.pads[padID].padL;
				pinTemp.pinL = tmpnt.pads[padID].padL;
			}
			else
			{
				cout << "error  : can't find connected component: " << pinTemp.pinName << endl;
				exit(EXIT_FAILURE);
			}

			tmpnt.pins.push_back( pinTemp );
			pinCount++;
		}

		if( (maxL - minL) > 0 )  //NetList which across layer need to insert TSV
		{
			nets[i].nTSV = maxL - minL;
			nets[i].nPin = nPin + nets[i].nTSV;

			for( int j = 0; j < nets[i].nTSV; j++)
			{
				char tsvNum[10];
				sprintf( tsvNum, "%d", tsvCount );

				PIN pinTemp;
				strcpy( pinTemp.pinName, "TSV" );
				strcat( pinTemp.pinName, tsvNum );
				pinTemp.pinL    = minL + j;
				pinTemp.flagTSV = true;

				MODULE modTemp;
				strcpy( modTemp.modName, pinTemp.pinName );
				modTemp.type    = HARD_BLK;
				modTemp.idMod   = tmpnt.nMod;
				modTemp.modW    = modTemp.modH = TSV_PITCH * AMP_PARA;
				modTemp.modL    = pinTemp.pinL;
				modTemp.modArea = (int)pow( TSV_PITCH, 2.0 );
				modTemp.minAR   = modTemp.maxAR = 1.0;
				modTemp.flagTSV = true;
				///weiyi 20181029
				modTemp.Power	= 0;
				modTemp.Pdensity= 0;

                modTemp.nLayer = 1;///f///0926

                ///
                if(!InputOption.CompCorblivar)
                    tmpnt.totalModArea += modTemp.modArea;
				tmpnt.mods.push_back( modTemp );
				tmpnt.pins.push_back( pinTemp );
				tmpnt.mod_NameToID[ modTemp.modName ] = modTemp.idMod;
				tmpnt.nMod++;
				tmpnt.nPin++;
				tmpnt.nTSV++;
				tmpnt.nHard++;

				tsvCount++;
				pinCount++;
			}
			//cout<<"\n\n\n"<<tmpnt.nMod<<"\n\n\n";
			vector<PIN>::iterator pin_it;
			pin_it = tmpnt.pins.begin() + nets[i].head;
			sort( pin_it, tmpnt.pins.end(), SortByLayer );
		}
		else
		{
			nets[i].nTSV = 0;
			nets[i].nPin = nPin;
		}
	}

	fin.close();

	int netCount = 0;
	int sub_Head = 0;
	int sub_nPin = 0;

	tmpnt.nNet = nNet + tmpnt.nTSV;
	tmpnt.nets.resize( tmpnt.nNet );

	/// partition original nets into sub-nets
	for( int i = 0; i < nNet; i++ )
	{
		bool flagSubNet = false;	///< flag to mark whether the sub-net be updated
		bool flagHead   = true;		///< flag to mark whether to initialize the head for new sub-net

		for( int j = nets[i].head; j < nets[i].head + nets[i].nPin; j++ )
		{
			if( flagHead )
			{
				sub_Head = j;
				sub_nPin = 0;
				flagHead = false;
			}

			sub_nPin++;

			if( tmpnt.pins[j].flagTSV )
			{
				tmpnt.nets[netCount].nTSV++;
				//jack 7/17
				strcpy(tmpnt.nets[netCount].netName,nets[i].netName);
			}

			if( tmpnt.pins[j].flagTSV && !flagSubNet )
			{
				tmpnt.nets[netCount].idNet = netCount;
				tmpnt.nets[netCount].head  = sub_Head;
				tmpnt.nets[netCount].nPin  = sub_nPin;
				tmpnt.nets[netCount].flag  = '0';
				//jack 7/17
				strcpy(tmpnt.nets[netCount].netName,nets[i].netName);
				netCount++;
				flagSubNet = true;
				flagHead = true;
				j--;
			}
			else
			{
				flagSubNet = false;
			}
		}
		tmpnt.nets[netCount].idNet = netCount;
		tmpnt.nets[netCount].head  = sub_Head;
		tmpnt.nets[netCount].nPin  = sub_nPin;
		tmpnt.nets[netCount].flag  = '0';
		//jack 7/17
		strcpy(tmpnt.nets[netCount].netName,nets[i].netName);

		netCount++;
		sub_nPin = 0;
	}
	nets.clear();
	vector<NET> (nets).swap(nets);

	/// assign pad id
	for( int i = 0; i < tmpnt.nPad; i++ )
	{
		tmpnt.pads[i].idPad += tmpnt.nMod;
	}

	/// assign pin id
	for( int i = 0; i < tmpnt.nPin; i++ )
	{
		tmpnt.pins[i].idPin = i;
	}

	/// assign pin index \n
	/// assign net layer \n
	/// count number of nets which link to mod[index] \n
	for( int i = 0; i < tmpnt.nNet; i++ )
	{
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
			{
				tmpnt.pins[j].index = tmpnt.mod_NameToID[ tmpnt.pins[j].pinName ];
				//tmpnt.mods[ tmpnt.pins[j].index ].pNet[nNet]=tmpnt.nets[i].idNet;
				tmpnt.mods[ tmpnt.pins[j].index ].nNet++;
			}
			else if( tmpnt.pad_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.pad_NameToID.end() )
			{
				tmpnt.pins[j].index = tmpnt.pad_NameToID[ tmpnt.pins[j].pinName ] + tmpnt.nMod;
			}
			else
			{
				cout << "error  : can't find connected component: " << tmpnt.pins[j].pinName << endl;
				exit(EXIT_FAILURE);
			}

			if( tmpnt.pins[j].pinL > tmpnt.nets[i].netL )
			{
				tmpnt.nets[i].netL = tmpnt.pins[j].pinL;
			}
		}
	}

	/// allocate the memory for nets which link to mod[index] (mod[index].pNet)
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		try
		{
			tmpnt.mods[i].pNet = new unsigned int [tmpnt.mods[i].nNet];
		}
		catch( bad_alloc &bad )
		{
			cout << "error  : run out of memory on tmpnt.mods[" << tmpnt.mods[i].idMod << "].pNet" << endl;
			exit(EXIT_FAILURE);
		}
		tmpnt.mods[i].nNet = 0;
	}

	/// construct the relationship between nets and modules
	for( int i = 0; i < tmpnt.nNet; i++ )
	{
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
			{
				tmpnt.mods[ tmpnt.pins[j].index ].pNet[ tmpnt.mods[tmpnt.pins[j].index].nNet ] = tmpnt.nets[i].idNet;
				tmpnt.mods[ tmpnt.pins[j].index ].nNet++;
			}
		}
	}

	/// count number of modules at each layer
    tmpnt.LayernMod.resize(tmpnt.nLayer);
    for( int i=0; i < tmpnt.nLayer; i++ )
        tmpnt.LayernMod[i] = 0;

    for( int i=0; i < tmpnt.nMod; i++ )
        tmpnt.LayernMod[ tmpnt.mods[i].modL ]++;

    /*for( int i=0; i < tmpnt.nLayer; i++ )
        cout << tmpnt.LayernMod[ i ] << endl;

    getchar();*/

	//char* checkFile;
	//checkFile = strrchr( fileName, '/' );
	//checkFile++;
	//char checkNet[50] = "\0";
	//char checkMod[50] = "\0";
	//strcpy( checkNet, checkFile );
	//strcat( checkNet, ".checknet" );
	//strcpy( checkMod, checkFile );
	//strcat( checkMod, ".checkmod" );

	/*
	   ofstream fout;
	   fout.open( "CheckNet.txt" );

	   for( int i = 0; i < tmpnt.nNet; i++ )
	   {
	   fout << tmpnt.nets[i];
	   for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
	   {
	   fout << tmpnt.pins[j];
	   }
	   fout << endl;
	   }
	   fout.close();

	   fout.open( "CheckMod.txt" );
	   for( int i = 0; i < tmpnt.nMod; i++ )
	   {
	   fout << tmpnt.mods[i] << endl;
	   }
	   fout.close();*/
		
}
void IO::POWER::ReadPowerFile_ITRI(string benName, NETLIST &tmpnt )
{


	string fileIn= benName+".pt"; /// Read .pt file which in benchmark file.
	ifstream fin(fileIn.c_str());
	if(!fin)
	{
		cerr << "Error in opening " << fileIn << endl;
		exit(-1);
	}
	string trash;
	string unit;
	int number_blocks;

	vector <int> ModuleOrder;

	while (!fin.eof())
	{
		fin>> trash >> trash>>unit>>trash;
		fin>> trash >> trash >>trash >>trash>>number_blocks>>trash;
		fin>> trash >> trash;


		ModuleOrder.resize(number_blocks);
		for(int j=0; j< number_blocks ; j++)
		{
			string block_name ;
			fin >> block_name;
			int ModID = tmpnt.GetModID(block_name);
			ModuleOrder[j] = ModID;
		}

		fin>> trash >> trash;
		for(int j=0; j< number_blocks ; j++)
		{
			int ModID = ModuleOrder[j];
			double power ;
			fin>> power;
			if(power< 0 )
				power = 0;
			tmpnt.mods[ModID].Power  = power * InputOption.EnlargePowerRatio;
			tmpnt.mods[ModID].Pdensity = (double) tmpnt.mods[ModID].Power / tmpnt.mods[ModID].modArea;
			//cout<< " - " << tmpnt.mods[ModID].modName <<"  "<< power <<endl;
			//getchar();

		}
		fin >> trash;

		break;
	}
	fin.close();
}

void IO::ThreeDim::ReadAlignFile(string benName, NETLIST &tmpnt )
{
    string fileIn= benName+".align"; /// Read .align file which in benchmark file.
	ifstream fin(fileIn.c_str());
	if(!fin)
	{
		cerr << "Error in opening " << fileIn << endl;
		exit(-1);
	}
	string trash;

	while (!fin.eof())
	{
        fin >> trash >> trash >> trash >> tmpnt.nAlign;
		fin>> trash >> trash >> trash;  ///< low_module high_module type;


		tmpnt.aligns.resize(tmpnt.nAlign);
		for(int j=0; j< tmpnt.nAlign ; j++)
		{
            string low_mod, high_mod, type;
			fin >> low_mod >> high_mod >> type;
			int LowModID = tmpnt.GetModID(low_mod);
			int HighModID = tmpnt.GetModID(high_mod);

			if(type == "complete")
                tmpnt.aligns[j].type = '0';
            else if(type == "center")
                tmpnt.aligns[j].type = '1';

			tmpnt.mods[LowModID].flagAlign = true;
			tmpnt.mods[HighModID].flagAlign = true;

			tmpnt.aligns[j].lowModId = LowModID;
			tmpnt.aligns[j].highModId = HighModID;

			tmpnt.aligns[j].lowLayer = tmpnt.mods[LowModID].modL + tmpnt.mods[LowModID].nLayer-1;
			tmpnt.aligns[j].highLayer = tmpnt.mods[HighModID].modL + tmpnt.mods[HighModID].nLayer-1;
		}

		break;
	}
	fin.close();

	//add the pseudo_net

	int pinCount = tmpnt.nPin;
	tmpnt.nets.resize(tmpnt.nNet+tmpnt.nAlign);
	for( int i = tmpnt.nNet; i < tmpnt.nNet+tmpnt.nAlign; i++ )
	{
		int maxL = 0;
		int minL = MAX_INT;
		int nPin = 2;
		int AlignPairIndex = i-tmpnt.nNet;

		PIN LowPinTemp;
		PIN HighPinTemp;

		///Net_1

		tmpnt.nets[i].idNet = i;
		tmpnt.nets[i].head = pinCount;
		tmpnt.nets[i].flagAlign=true;

		///LOW PIN

		LowPinTemp.index = tmpnt.aligns[ AlignPairIndex ].lowModId;
		LowPinTemp.idPin = pinCount;
		LowPinTemp.pinL = tmpnt.mods[ LowPinTemp.index ].modL;
		strcpy( LowPinTemp.pinName, tmpnt.mods[ LowPinTemp.index ].modName );

		pinCount++;

		///HIGH PIN

		HighPinTemp.index = tmpnt.aligns[ AlignPairIndex ].highModId;
		//HighPinTemp.pinName = tmpnt.mods[ HighPinTemp.index ].modName;
		HighPinTemp.idPin = pinCount;
		HighPinTemp.pinL = LowPinTemp.pinL; ///set to lower layer
		strcpy( HighPinTemp.pinName, tmpnt.mods[ HighPinTemp.index ].modName );

		pinCount++;

		///push_back pin
		tmpnt.pins.push_back( LowPinTemp );
		tmpnt.pins.push_back( HighPinTemp );

		tmpnt.nPin+=2;

		///Net_2

		tmpnt.nets[i].netL = LowPinTemp.pinL;
		tmpnt.nets[i].nPin = 2;
    }

    ///update tmpnt.nNet number
    tmpnt.nNet+=tmpnt.nAlign;

    ///re-connect nets and mods
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        delete [] tmpnt.mods[i].pNet;
        tmpnt.mods[i].nNet=0;
    }

    for( int i = 0; i < tmpnt.nNet; i++ )
	{
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
			{
				tmpnt.mods[ tmpnt.pins[j].index ].nNet++;
			}
			else if( tmpnt.pad_NameToID.find( tmpnt.pins[j].pinName ) == tmpnt.pad_NameToID.end() )
			{
				cout << "error  : can't find connected component: " << tmpnt.pins[j].pinName << endl;
				exit(EXIT_FAILURE);
			}
		}
	}

	/// allocate the memory for nets which link to mod[index] (mod[index].pNet)
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		try
		{
			tmpnt.mods[i].pNet = new unsigned int [tmpnt.mods[i].nNet];
		}
		catch( bad_alloc &bad )
		{
			cout << "error  : run out of memory on tmpnt.mods[" << tmpnt.mods[i].idMod << "].pNet" << endl;
			exit(EXIT_FAILURE);
		}
		tmpnt.mods[i].nNet = 0;
	}

	/// construct the relationship between nets and modules
	for( int i = 0; i < tmpnt.nNet; i++ )
	{
        /*if(tmpnt.nets[i].flagAlign)
        {
            cout <<"here" << i<< endl;
            getchar();
        }*/
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
			{
				tmpnt.mods[ tmpnt.pins[j].index ].pNet[ tmpnt.mods[tmpnt.pins[j].index].nNet ] = tmpnt.nets[i].idNet;
				tmpnt.mods[ tmpnt.pins[j].index ].nNet++;
			}
		}
	}

}
///< PLOT

void PLOT::PlotMap ( vector <vector <double> > &Map, string MapName, int binW, int binH  )
{
	ofstream fout;
	fout.open( MapName.c_str() );
	int RowSize = (int) Map.size();
	int ColSize = (int) Map[RowSize-1].size();

	fout << "axis equal;" << endl;

	fout << "x = linspace( " << 0 << ", " << binW * RowSize << ", " << RowSize << " );" << endl;
	fout << "y = linspace( " << 0 << ", " << binH * ColSize << ", " << ColSize << " );" << endl;
	fout << "[xx,yy] = meshgrid(x, y); " << endl;
	fout << "zz = zeros (" << RowSize << ", " << ColSize << ", 'double');" << endl << endl;

	// output->result
	for(int j=0; j<RowSize; j++)
	{
		for(int k=0; k<ColSize; k++)
		{

			double data=Map[j][k];

			fout<<"zz ("<<j+1 <<", "<<k+1<<") = "<< data <<";"<<endl<<endl;
			// double data= (double) result[j*2*w_src + 2*k] / (h_src*w_src);

		}
	}

	fout<<"surf(xx, yy, zz);"<<endl;

	fout.close();

}
void PLOT::PlotResultGL_dy( NETLIST& tmpnt, string Name ,bool GorL)
{


	/*
	   if(GorL==0)
	   cout<<"**Dump matlab file: "<<file<<"_G.m"<<endl<<endl;
	   else
	   cout<<"**Dump matlab file: "<<file<<"_L.m"<<endl<<endl;*/

	char *file = new char[Name.length() + 1];

	strcpy(file, Name.c_str());


    char** fileName;
    fileName = new char* [tmpnt.nLayer+1];
    for( int i = 0; i < tmpnt.nLayer+1; i++ )
    {
		if(i!=tmpnt.nLayer){
			fileName[i] = new char [50];

			char subName[10] = "\0";
			sprintf( subName, "%d", i );

			strcpy( fileName[i], file );


			
			if(GorL==0)///f///0106
				strcat( fileName[i], "G_" );
			else
				strcat( fileName[i], "L_" );
			
			
			strcat( fileName[i], subName );
			strcat( fileName[i], ".m" );
		}
		else{
			fileName[i] = new char [50];

			char subName[10] = "\0";
			sprintf( subName, "TSV"  );

			strcpy( fileName[i], file );


			
			if(GorL==0)///f///0106
				strcat( fileName[i], "G_" );
			else
				strcat( fileName[i], "L_" );
			
			
			strcat( fileName[i], subName );
			strcat( fileName[i], ".m" );
		}
    }
	ofstream *fout = new ofstream [tmpnt.nLayer+1];

	float llx = 0.0;
	float lly = 0.0;
	float urx = 0.0;
	float ury = 0.0;
	float centerx = 0.0;
	float centery = 0.0;

	for( int i = 0; i < tmpnt.nLayer+1; i++ )
	{
		fout[i].open( fileName[i] );


		if( !fout[i].is_open() )
		{
			cout << "error  : unable to open " << fileName[i] << "for plotting the result of global distribution" << endl;
			exit(EXIT_FAILURE);
		}

		fout[i].precision(3);
		fout[i].setf( ios::fixed, ios::floatfield );

		fout[i] << "figure;" << endl;
		fout[i] << "clear;" << endl;
		fout[i] << "clf;" << endl;
		fout[i] << "axis equal;" << endl;

		llx = -tmpnt.ChipWidth/(20)/(float)AMP_PARA;
		lly = -tmpnt.ChipHeight/(20)/(float)AMP_PARA;
		urx = tmpnt.ChipWidth/((float)AMP_PARA) + tmpnt.ChipWidth/(20)/(float)AMP_PARA;
		ury = tmpnt.ChipHeight/((float)AMP_PARA) + tmpnt.ChipHeight/(20)/(float)AMP_PARA;

		fout[i] << "axis([ " << llx << " " << urx << " " << lly << " " << ury << "]);" << endl;
		fout[i] << "hold on;" << endl;
		fout[i] << "grid on;" << endl;
		fout[i] << "title('" << fileName[i] << "');" << endl;
	}

	for( int i = 0; i < tmpnt.nMod; i++ )
	{

		///f///0926 stacked module has multiple layer
		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )//+ tmpnt.mods[i].nLayer
		{
			if( modL >= tmpnt.nLayer )
			{
				cout<< modL<<"   "<<tmpnt.nLayer<<endl;
				cerr << "error  : stack's layer out of range (plot.cpp)" << endl;
				exit(EXIT_FAILURE);
			}

			if(GorL==0)///f///0106
			{
				llx = tmpnt.mods[i].GLeftX/(float)AMP_PARA;
				lly = tmpnt.mods[i].GLeftY/(float)AMP_PARA;
				urx = tmpnt.mods[i].GLeftX/(float)AMP_PARA + tmpnt.mods[i].modW/(float)AMP_PARA;
				ury = tmpnt.mods[i].GLeftY/(float)AMP_PARA + tmpnt.mods[i].modH/(float)AMP_PARA;
				centerx = tmpnt.mods[i].GCenterX/(float)AMP_PARA;
				centery = tmpnt.mods[i].GCenterY/(float)AMP_PARA;

			}
			else
			{
				llx = tmpnt.mods[i].LeftX/(float)AMP_PARA;
				lly = tmpnt.mods[i].LeftY/(float)AMP_PARA;
				urx = tmpnt.mods[i].LeftX/(float)AMP_PARA + tmpnt.mods[i].modW/(float)AMP_PARA;
				ury = tmpnt.mods[i].LeftY/(float)AMP_PARA + tmpnt.mods[i].modH/(float)AMP_PARA;
				centerx = tmpnt.mods[i].CenterX/(float)AMP_PARA;
				centery = tmpnt.mods[i].CenterY/(float)AMP_PARA;
			}

			/*if( tmpnt.mods[i].flagTSV == true )
			  continue;//*/
			// cout<<"blocks "<<llx<<" "<<lly<<" "<<urx<<" "<<ury<<endl;



			/*fout[modL] << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
				<< "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], ";*/
			if( tmpnt.mods[i].flagTSV == false )
			{
				fout[modL] << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
				<< "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], ";
				if(tmpnt.nVI==0)///f///0514
				{
					///f///0923
					if(tmpnt.mods[i].flagAlign)
					{
                        fout[modL] << "[0/255 150/255 150/255],'facealpha',0.5);" << " % aligned module(blue-green)" << i << endl;
					}
					else if( tmpnt.mods[i].nLayer > 1)
					{
						if(!tmpnt.mods[i].folding)
							fout[modL] << "[128/255 128/255 255/255],'facealpha',0.5);" << " % stack module(blue)" << i << endl;
						else
							fout[modL] << "[128/255 255/255 128/255],'facealpha',1);" << " % Folding module(green)" << i << endl;
					}
					else if( tmpnt.mods[i].type == HARD_BLK )
					{
						if(this->Coloring == true)
						{

							if(tmpnt.mods[i].TemperatureGroup == 0)//hot
								fout[modL] << "[255/255 0/255 0/255],'facealpha',0.5);" << " % hard module(green)" << i << endl;
							else if (tmpnt.mods[i].TemperatureGroup== 1 )//warm
								fout[modL] << "[255/255 255/255 128/255],'facealpha',0.5);" << " % hard module(green)" << i << endl;
							else
								fout[modL] << "[255/255 128/255 128/255],'facealpha',0.5);" << " % hard module(red)" << i << endl;
						}
						else
						{

							fout[modL] << "[255/255 128/255 128/255],'facealpha',0.5);" << " % hard module(red)" << i << endl;
						}
					}


					else if( tmpnt.mods[i].type == SOFT_BLK )
					{

						if(this->Coloring == true)
						{

							if(tmpnt.mods[i].TemperatureGroup == 0) //hot
								fout[modL] << "[255/255 0/255 0/255],'facealpha',0.5);" << " % soft module(green)" << i << endl;
							else if(tmpnt.mods[i].TemperatureGroup== 1 )//warm
								fout[modL] << "[255/255 255/255 128/255],'facealpha',0.5);" << " % soft module(green)" << i << endl;
							else
								fout[modL] << "[128/255 255/255 128/255],'facealpha',0.5);" << " % soft module(green)" << i << endl;
						}
						else
						{

							fout[modL] << "[128/255 255/255 128/255],'facealpha',0.5);" << " % soft module(green)" << i << endl;
						}
					}

					else
						fout[modL] << "[220/255 220/255 220/255],'facealpha',0.5);" << " % soft module" << endl;//*/
				}
				else
				{
					///f///0511 voltage island
					if( tmpnt.mods[i].powerMode == 0 )
						fout[modL] << "[128/255 128/255 255/255],'facealpha',0.5);" << " % powerMode=0(blue)" << i << endl;
					else if( tmpnt.mods[i].powerMode == 1 )
						fout[modL] << "[255/255 128/255 128/255],'facealpha',0.5);" << " % powerMode=1(red)" << i << endl;
					else if( tmpnt.mods[i].powerMode == 2 )
						fout[modL] << "[128/255 255/255 128/255],'facealpha',0.5);" << " % powerMode=2(green)" << i << endl;
					else if( tmpnt.mods[i].powerMode == 3 )
						fout[modL] << "[220/255 220/255   0/255],'facealpha',0.5);" << " % powerMode=3(yellow)" << i << endl;
					else
						fout[modL] << "[180/255 180/255 180/255],'facealpha',0.5);" << " % powerMode=4" << endl;//*/
				}
				fout[modL] << "plot(" << "[ " << llx << " " << urx << " " << urx << " " << llx << " " << llx << " ], "
				<< "[ " << lly << " " << lly << " " << ury << " " << ury << " " << lly << " ], "
				<< "'Color', [0/255 0/255 0/255]);" << endl;

			}
			else{
				
				float centerx_temp=centerx;
				float centery_temp=centery;
				llx=centerx_temp-(tmpnt.terminal_w/2);
				urx=centerx_temp+(tmpnt.terminal_w/2);
				lly=centery_temp-(tmpnt.terminal_h/2);
				ury=centery_temp+(tmpnt.terminal_h/2);
				fout[tmpnt.nLayer] << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
				<< "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], ";
				fout[tmpnt.nLayer] << "[217/255 144/255 88/255]);" << " % TSV" << endl;
				fout[tmpnt.nLayer] << "plot(" << "[ " << llx << " " << urx << " " << urx << " " << llx << " " << llx << " ], "
				<< "[ " << lly << " " << lly << " " << ury << " " << ury << " " << lly << " ], "
				<< "'Color', [0/255 0/255 0/255]);" << endl;
			}

			


			///f///0926 module index text
/*
			   if( tmpnt.mods[i].flagTSV == false )
			   fout[modL] << "text( " << centerx << ", " << centery << ", '" << tmpnt.mods[i].idMod << "', "
			   << "'Color', [0/255 0/255 0/255], 'fontsize', 10, 'HorizontalAlignment', 'center');" << endl;//*/

			/////20170220 plot module name
			if(this->Texting == true )
			{

			   if( tmpnt.mods[i].flagTSV == false )
				 fout[modL] << "text( " << centerx << ", " << centery << ", '" << tmpnt.mods[i].modName << "', "
				 << "'Color', [0/255 0/255 0/255], 'fontsize', 10, 'HorizontalAlignment', 'center');" << endl;
			}




		}///f///0926


	}

	for( int i = 0; i < tmpnt.nLayer+1; i++ )
	{
		llx = 0;
		lly = 0;
		urx = tmpnt.ChipWidth/((float)AMP_PARA);
		ury = tmpnt.ChipHeight/((float)AMP_PARA);

		// plot fixed-outline
		fout[i] << "% fixed-outline" << endl;
		fout[i] << "line(" << "[ " << llx << " " << llx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
		fout[i] << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << lly << " " << lly << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
		fout[i] << "line(" << "[ " << urx << " " << urx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
		fout[i] << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << ury << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;

		fout[i].close();
	}
	delete [] fout;




	///f///0924/*
	char* fileName_all;

	fileName_all = new char [50];

	char subName_all[10] = "\0";
	sprintf( subName_all, "all" );


	strcpy( fileName_all, file );
	if(GorL==0)///f///0106
		strcat( fileName_all, "G_" );
	else
		strcat( fileName_all, "L_" );
	strcat( fileName_all, subName_all );
	strcat( fileName_all, ".m" );


	ofstream gout;
	gout.open( fileName_all );

	ifstream gin;
	string strtemp;

	gout << "SS = get(0,'ScreenSize');" <<endl;
	gout << "figure('Position',[ SS(1)+SS(3)/5 SS(2)+SS(4)/5 SS(3)*3/5 SS(4)*3/5 ]);" <<endl;
	for( int i = 0; i < tmpnt.nLayer+1; i++ )
	{
		gin.open( fileName[i] );
		gout << "subplot (1,"<< tmpnt.nLayer+1 <<"," << i+1 << ");" << endl;
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




	for( int i = 0; i < tmpnt.nLayer+1; i++ )
	{
		delete [] fileName[i];
	}
	delete [] fileName;
	delete [] file;
}

void PLOT::PlotPad( NETLIST& tmpnt, string file, int allocate_mode )
{
	cout << "*Plot Locations of Pads" << endl;

	string fileName = file + "PAD.m";

	ofstream fout;
	fout.open( fileName.c_str() );
	if( !fout.is_open() )
	{
		cout << "error  : unable to open " << fileName << "for plotting pads"<< endl;
		exit(EXIT_FAILURE);
	}

	fout.precision(3);
	fout.setf( ios::fixed, ios::floatfield );

	float llx;
	float lly;
	float urx;
	float ury;

	fout << "figure;" << endl;
	fout << "clear;" << endl;
	fout << "clf;" << endl;
	fout << "axis equal;" << endl;

	llx = -tmpnt.ChipWidth/(20)/(float)AMP_PARA;
	lly = -tmpnt.ChipHeight/(20)/(float)AMP_PARA;
	urx = tmpnt.ChipWidth/((float)AMP_PARA) + tmpnt.ChipWidth/(20)/(float)AMP_PARA;
	ury = tmpnt.ChipHeight/((float)AMP_PARA) + tmpnt.ChipHeight/(20)/(float)AMP_PARA;

	fout << "axis([ " << llx << " " << urx << " " << lly << " " << ury << "]);" << endl;
	fout << "hold on;" << endl;
	fout << "grid on;" << endl;
	fout << "title('" << fileName << "');" << endl;

	llx = 0;
	lly = 0;
	urx = tmpnt.ChipWidth/((float)AMP_PARA);
	ury = tmpnt.ChipHeight/((float)AMP_PARA);

	// plot fixed-outline
	fout << "% fixed-outline" << endl;
	fout << "line(" << "[ " << llx << " " << llx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
	fout << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << lly << " " << lly << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;
	fout << "line(" << "[ " << urx << " " << urx << " ]" << ", " << "[ " << lly << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2'" << ");" << endl;
	fout << "line(" << "[ " << llx << " " << urx << " ]" << ", " << "[ " << ury << " " << ury << " ] " << ", 'Color', [0/255, 0/255, 128/255], 'Linewidth', 2" << ");" << endl;

	// plot pads
	float halfW = 1;
	float halfH = 1;
	fout << "% pads" << endl;
	if( allocate_mode == 0 )
	{
		for(int i = 0; i < tmpnt.nPad; i++)
		{
			if( tmpnt.pads[i].x == 0 )		// west
			{
				llx = tmpnt.pads[i].x/((float)AMP_PARA) - halfW;
				lly = tmpnt.pads[i].y/((float)AMP_PARA) - halfH;
				urx = llx + halfW * 2;
				ury = lly + halfH * 2;
			}
			else if( tmpnt.pads[i].y == 0 )	// south
			{
				llx = tmpnt.pads[i].x/((float)AMP_PARA) - halfH;
				lly = tmpnt.pads[i].y/((float)AMP_PARA) - halfW;
				urx = llx + halfH * 2;
				ury = lly + halfW * 2;
			}
			else if( tmpnt.pads[i].x == tmpnt.ChipWidth )	// east
			{
				llx = tmpnt.pads[i].x/((float)AMP_PARA) - halfW;
				lly = tmpnt.pads[i].y/((float)AMP_PARA) - halfH;
				urx = llx + halfW * 2;
				ury = lly + halfH * 2;
			}
			else if( tmpnt.pads[i].y == tmpnt.ChipHeight )	// north
			{
				llx = tmpnt.pads[i].x/((float)AMP_PARA) - halfH;
				lly = tmpnt.pads[i].y/((float)AMP_PARA) - halfW;
				urx = llx + halfH * 2;
				ury = lly + halfW * 2;
			}
			else
			{
				llx = tmpnt.pads[i].x/((float)AMP_PARA) - halfH;
				lly = tmpnt.pads[i].y/((float)AMP_PARA) - halfW;
				urx = llx + halfH * 2;
				ury = lly + halfW * 2;
				/*cout << "error  : " << "pad[" << i << "]" << "/" << "(" << tmpnt.pads[i].x/((float)AMP_PARA) << ", " << tmpnt.pads[i].y/((float)AMP_PARA) << ")"
				  << "does not scale to the boundary"<< endl;
				  exit(EXIT_FAILURE);*/
			}

			fout << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
				<< "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], "
				<< "[170/255 170/255 255/255]);" << endl;
			fout << "plot(" << "[ " << llx << " " << urx << " " << urx << " " << llx << " " << llx << " ], "
				<< "[ " << lly << " " << lly << " " << ury << " " << ury << " " << lly << " ], "
				<< "'Color', [0/255 0/255 0/255]);" << endl;
		}
	}
	else
	{
		for(int i = 0; i < tmpnt.nPad; i++)
		{
			llx = tmpnt.pads[i].x/((float)AMP_PARA) - halfW;
			lly = tmpnt.pads[i].y/((float)AMP_PARA) - halfH;
			urx = llx + halfW * 2;
			ury = lly + halfH * 2;

			fout << "fill(" << "[ " << llx << "; " << urx << "; " << urx << "; " << llx << " ], "
				<< "[ " << lly << "; " << lly << "; " << ury << "; " << ury << " ], "
				<< "[170/255 170/255 255/255]);" << endl;
			fout << "plot(" << "[ " << llx << " " << urx << " " << urx << " " << llx << " " << llx << " ], "
				<< "[ " << lly << " " << lly << " " << ury << " " << ury << " " << lly << " ], "
				<< "'Color', [0/255 0/255 0/255]);" << endl;
		}
	}
	fout.close();
}
// Dump FILE


void DUMP::Academic::DumpBlocks( NETLIST& tmpnt, char* file, char* subfile )
{
	cout << " - Dump Academic .blocks File" << endl;

	char fileName[100] = "\0";
	strcpy( fileName, file );
	strcat( fileName, subfile );
	strcat( fileName, ".blocks" );

	ofstream fout;
	fout.open( fileName );
	fout.precision(3);
	fout.setf( ios::fixed, ios::floatfield );

	if( strcmp( subfile, "_global" ) == 0 )
	{
		fout << "NCKU blocks after Global Distribution" << endl;
	}
	else if( strcmp( subfile, "_legal" ) == 0 )
	{
		fout << "NCKU blocks after Legalization" << endl;
	}
	else
	{
		fout << "NCKU blocks" << endl;
	}

	fout << "# Created      : " << UTILITY::CurrentTime() << endl;
	fout << "# User         : " << "NCKU SEDA LAB" << endl;
	fout << "# Platform     : " << "CentOS 5.1" << endl;
	fout << endl;
	fout << "NumSoftRectangularBlocks : " << tmpnt.nSoft << endl;
	fout << "NumHardRectilinearBlocks : " << tmpnt.nHard << endl;
	fout << "NumTerminals   : " << tmpnt.nPad << endl;
	fout << endl;
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		if( tmpnt.mods[i].type == SOFT_BLK )
		{
			fout << tmpnt.mods[i].modName << " "
				<< "softrectangular" << " "
				<< tmpnt.mods[i].modArea << " "
				<< tmpnt.mods[i].minAR << " "
				<< tmpnt.mods[i].maxAR << endl;
		}
		else if( tmpnt.mods[i].type == HARD_BLK )
		{
			fout << tmpnt.mods[i].modName << " "
				<< "hardrectilinear 4 "
				<< "(0, 0) "
				<< "(0, " << tmpnt.mods[i].modH/AMP_PARA << ") "
				<< "(" << tmpnt.mods[i].modW/AMP_PARA << ", " << tmpnt.mods[i].modH/AMP_PARA << ") "
				<< "(" << tmpnt.mods[i].modW/AMP_PARA << ", 0)"<< endl;
		}
		else
		{
			cout << "warning: unrecognized type of module " << i << "/" << tmpnt.mods[i].modName << endl;
		}
	}

	for( int i = 0; i < tmpnt.nPad; i++ )
	{
		fout << tmpnt.pads[i].padName << " terminal" << endl;
	}

	fout.close();

}
void DUMP::Academic::DumpPl( NETLIST& tmpnt, char* file, char* subfile )
{
	cout << " - Dump Academic.pl File" << endl;

	char fileName[100] = "\0";
	strcpy( fileName, file );
	strcat( fileName, subfile );
	strcat( fileName, ".pl" );

	ofstream fout;
	fout.open( fileName );

	if( strcmp( subfile, "_global" ) == 0 )
	{
		fout << "NCKU pl after Global Distribution" << endl;
	}
	else if( strcmp( subfile, "_legal" ) == 0 )
	{
		fout << "NCKU pl after Legalization" << endl;
	}
	else
	{
		fout << "NCKU pl" << endl;
	}

	fout << "# Created      : " << UTILITY::CurrentTime() << endl;
	fout << "# User         : " << "NCKU SEDA LAB" << endl;
	fout << "# Platform     : " << "CentOS 5.1" << endl;
	fout << endl;

	if( strcmp( subfile, "_global" ) == 0 )
	{
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			fout << tmpnt.mods[i].modName << "\t"
				<< tmpnt.mods[i].GLeftX/(float)AMP_PARA << "\t"
				<< tmpnt.mods[i].GLeftY/(float)AMP_PARA << "\t"
				<< ": N" << endl;
		}
	}
	else
	{
		for( int i = 0; i < tmpnt.nMod; i++ )
		{
			fout << tmpnt.mods[i].modName << "\t"
				<< tmpnt.mods[i].LeftX/(float)AMP_PARA << "\t"
				<< tmpnt.mods[i].LeftY/(float)AMP_PARA << "\t"
				<< ": N" << endl;
		}
	}

	for( int i = 0; i < tmpnt.nPad; i++ )
	{
		fout << tmpnt.pads[i].padName << "\t"
			<< tmpnt.pads[i].x/(float)AMP_PARA << "\t"
			<< tmpnt.pads[i].y/(float)AMP_PARA << "\t"
			<< ": N /FIXED" << endl;
	}

	fout.close();
}
void DUMP::Academic::DumpNets( NETLIST& tmpnt, char* file, char* subfile )
{
	cout << " - Dump Academic.nets File" << endl;

	char fileName[100] = "\0";
	strcpy( fileName, file );
	strcat( fileName, subfile );
	strcat( fileName, ".nets" );

	ofstream fout;
	fout.open( fileName );

	if( strcmp( subfile, "_global" ) == 0 )
	{
		fout << "NCKU nets after Global Distribution" << endl;
	}
	else if( strcmp( subfile, "_legal" ) == 0 )
	{
		fout << "NCKU nets after Legalization" << endl;
	}
	else
	{
		fout << "NCKU net" << endl;
	}

	fout << "# Created      : " << UTILITY::CurrentTime() << endl;
	fout << "# User         : " << "NCKU SEDA LAB" << endl;
	fout << "# Platform     : " << "CentOS 5.1" << endl;
	fout << endl;

	fout << "NumNets : " << tmpnt.nNet << endl;
	fout << "NumPins : " << tmpnt.nPin + tmpnt.nTSV << endl;

	for( int i = 0; i < tmpnt.nNet; i++ )
	{
		fout << "NetDegree : " << tmpnt.nets[i].nPin << endl;
		for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
		{
			if( tmpnt.pins[j].index < tmpnt.nMod )
			{
				int modID = tmpnt.pins[j].index;
				fout << tmpnt.mods[ modID ].modName << " B" << endl;
			}
			else
			{
				int padID = tmpnt.pins[j].index - tmpnt.nMod;
				fout << tmpnt.pads[ padID ].padName << " B" << endl;
			}
		}
	}

	fout.close();
}
void DUMP::Academic::DumpLayer( NETLIST& tmpnt, char* file, char* subfile )
{
	cout << " - Dump .layer File" << endl;
	char fileName[100] = "\0";
	strcpy( fileName, file );
	strcat( fileName, subfile );
	strcat( fileName, ".layer" );

	ofstream fout;
	fout.open( fileName );

	fout << "NumLayer : " << tmpnt.nLayer << endl;
	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		fout << tmpnt.mods[i].modName << "\t" << tmpnt.mods[i].modL << endl;
	}

	fout.close();
}
void DUMP::Academic::DumpFiles(NETLIST & tmpnt , string file , string subfile)
{
	cout<< BOLD(Cyan(" \\ ===== Dump Academic Files ===== // "))<<endl;
	cout<< " - Dump All Files : "<<endl;

	char *FileName = new char[file.length() + 1];
	strcpy(FileName, file.c_str());
	char *SubFileName = new char[subfile.length()+1];
	strcpy(SubFileName, subfile.c_str());
	DUMP::Academic::DumpLayer(tmpnt, FileName , SubFileName);
	DUMP::Academic::DumpNets(tmpnt, FileName , SubFileName);
	DUMP::Academic::DumpBlocks(tmpnt, FileName , SubFileName);
	DUMP::Academic::DumpPl(tmpnt, FileName , SubFileName);

	delete [] FileName;
	delete [] SubFileName;
}

void DUMP:: Corblivar::DumpPlHotSpot( NETLIST& tmpnt, string file , string subfile  ) //for hotspot solver
{
	cout<<" - Dump .pl File "<<endl;
	string Filename = file+subfile+".pl";
	ofstream fout(Filename.c_str());


	fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;


	for(int i=0;i<tmpnt.nMod;i++)
	{
		fout<<tmpnt.mods[i].modName<<" ";
		fout<< (double) tmpnt.mods[i].modW/AMP_PARA/this->scale<<" "<<(double) tmpnt.mods[i].modH/AMP_PARA/this->scale<<" ";
		fout<<(double) tmpnt.mods[i].LeftX/(float)AMP_PARA/this->scale<< " "<<(double) tmpnt.mods[i].LeftY/(float)AMP_PARA/this->scale<<" " ;
		fout<<"1.631e+06 "<<"0.00851064"<<endl;
	}

	fout<< "outline_1 "<<(double) tmpnt.ChipWidth/AMP_PARA/this->scale << " "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale << " "<< 0 <<" " <<0<<" 1.631e+06 0.00851064"<<endl;

	fout.close();
}

void DUMP::Corblivar::DumpPowerHotSpot( NETLIST& tmpnt, string file , string subfile  ) //for hotspot solver
{
	cout<<" - Dump .ptrace File "<<endl;
	string Filename = file+subfile+".ptrace";
	ofstream fout(Filename.c_str());

	double Amp = this->PowerAmp;
	// From mW-> W
	fout<<"active_wires_1 BEOL_1 ";
	for(int i=0;i<tmpnt.nMod;i++)
	{
		fout<<tmpnt.mods[i].modName<<" ";
	}
	fout<<"outline_1"<<endl;

	//power
	//active wire beol
	fout<<"0.0150087 0.0 ";

	for(int i=0;i<tmpnt.nMod;i++)
	{
		fout<<(double)tmpnt.mods[i].Power * 0.001 * Amp<<" ";
		//cout <<"Power : "<< tmpnt.mods[i].Power * Amp <<" (mW) "<<endl;
	}
	//outline
	fout<<"0.0"<<endl;

	fout.close();
}
void DUMP::Corblivar::DumpPowerFile_EVA( NETLIST& tmpnt, string file , string subfile  ) //for hotspot solver
{
	cout<<" - Dump .power File "<<endl;
	string Filename = file+subfile+".power";
	ofstream fout(Filename.c_str());
	fout<<"# power density in 10^6 W/m^2 = uW/um^2 end"<<endl;
	double Amp  = this->PowerAmp ;
	double Max = -1 ;
	double Min = 10000;
	double PDSum = 0;
	for(int i=0 ; i< tmpnt.nMod ; i++)
	{
            if(tmpnt.mods[i].flagTSV)
                continue;

		double PD = (double) tmpnt.mods[i].Power /tmpnt.mods[i].modArea;
		// cout<<"mod  "<< i << " "<< PD * 1000 <<endl;
		double Power = PD * 1000 * Amp ;
		if(Power > Max)
			Max= Power ;
		if(Power < Min)
			Min = Power ;

		PDSum+= Power ;
		//  getchar();
		fout<< PD * 1000 * Amp <<endl;
	}


	//cout <<" Max : "<< Max <<" Min : "<< Min<<" Avg "<< (double) PDSum / tmpnt.nMod  <<endl;

	fout.close();



}
void DUMP::Corblivar::DumpLayoutFile(NETLIST& tmpnt, string file , string subfile , string benName)
{
	cout<<" - Dump .gp File for Hotspot "<<endl;
	string Filename = file+subfile + ".gp";
	ofstream fout( Filename.c_str());
	fout<<"set title \"Thermal Map - "+ benName +", Layer 1\" noenhanced"<<endl;
	fout<<"set terminal pdfcairo enhanced font \"Gill Sans, 12\""<<endl;
	fout<<"set output \""+ benName +"_1_HotSpot.gp.pdf\""<<endl;
	fout<<"set size square"<<endl;
	fout<<"set xrange [0:64]"<<endl;
	fout<<"set yrange [0:64]"<<endl;
	fout<<"set cblabel \"Temperature [K], from HotSpot\""<<endl;
	fout<<"set tics front"<<endl;
	fout<<"set grid xtics ytics ztics"<<endl;
	fout<<"set pm3d map corners2color c1"<<endl;
	fout<<"set palette defined ( 0 \"#000090\",\\"<<endl;
	fout<<"1 \"#000fff\",\\"<<endl;
	fout<<"2 \"#0090ff\",\\"<<endl;
	fout<<"3 \"#0fffee\",\\"<<endl;
	fout<<"4 \"#90ff70\",\\"<<endl;
	fout<<"5 \"#ffee00\",\\"<<endl;
	fout<<"6 \"#ff7000\",\\"<<endl;
	fout<<"7 \"#ee0000\",\\"<<endl;
	fout<<"8 \"#7f0000\")"<<endl;

	for(int i =0; i <tmpnt.nMod ; i++)
	{
        if(tmpnt.mods[i].modL != 0)
            continue;

		fout<<"set obj rect front from ";
		// Normalize 0~64
		double llx =( (double) ( tmpnt.mods[i].LeftX )*64 / (tmpnt.ChipHeight) ) ;
		double lly =( (double) ( tmpnt.mods[i].LeftY )*64 / (tmpnt.ChipHeight) ) ;
		double urx =( (double) ( tmpnt.mods[i].LeftX + tmpnt.mods[i].modW )*64 / (tmpnt.ChipHeight) ) ;
		double ury =( (double) ( tmpnt.mods[i].LeftY +tmpnt.mods[i].modH)*64 / (tmpnt.ChipHeight) ) ;
		fout<< llx << ", "<<lly <<" to "<<urx<<", "<<ury;
		fout<<" fillstyle empty border rgb \"white\" "<<endl;

	}
	fout<<"splot \""+ benName +"_HotSpot.steady.grid.gp_data.layer_1\" using 1:2:3 notitle"<<endl;

	fout.close();


}
void DUMP::Corblivar::DumpFpInFo(NETLIST& tmpnt, string file , string subfile , string benName)
{
	cout<<" - Dump .perf File "<<endl;
	string Filename =  file+subfile+"_" + benName + ".perf";
	ofstream fout(Filename.c_str());
	fout<<"# NCKU FP Info :"<<endl;
	fout<<"# WS : "<< tmpnt.maxWS * 100 <<" % "<<endl;
	fout<<"# WL : "<< tmpnt.totalWL <<endl;
	fout<<"# Outline : "<< tmpnt.ChipHeight/AMP_PARA <<" "<< tmpnt.ChipWidth/AMP_PARA<<endl;


	fout.close();



}
void DUMP::Corblivar::DumpPassive(NETLIST& tmpnt, string file , string subfile )
{
	// Note!!
	// This File need the information of resistivity & SP
	// We are set the default

	double w = tmpnt.ChipWidth/AMP_PARA;
	double h = tmpnt.ChipHeight/AMP_PARA ;

	double width=0,height=0;
	double b_width=0,b_height=0;
	double times=1000000;
	int cut=64 ;
	width = w;
	height = h ;
	cout << " - Passive : "<< endl;
	cout<<  Red(" - R : ")<< this->SiResistivity<<endl;
	cout<<  Red(" - SP : ") << this-> SiSpecificHeat <<endl;

	b_width=(double) width/cut ;
	b_height=(double) height/cut ;


	string OutFile_passive = file + subfile  + "_passive.flp";
	ofstream fout (OutFile_passive.c_str());
	fout<<"# NCKU SEDA "<<endl;
	fout<<"# NCKU SEDA FixedOutline "<<width<<"   "<<height<<" um "<<endl;
	fout<<"# NCKU SEDA BinW,H "<<b_width<<"   "<<b_height<<" um "<<endl;
	fout<<"# NCKU SEDA "<<endl;


	for(int i=0; i<cut; i++)
	{
		for(int j=0; j<cut; j++)
		{

			if(i==cut-1)
			{
				fout<<"Si_passive_1_"<<i<<":"<<j<<"\t"<< (double) (width-b_width*i)/times;
				//cout<<i<<"   "<<width<<"   "<<b_width*i<<endl;
			}
			else
			{
				fout<<"Si_passive_1_"<<i<<":"<<j<<"\t"<< (double) b_width/times  ;
			}

			if(j==cut-1)
			{
				fout<< "\t"<< (double) (height-b_height*j)/times;
				//cout<<j<<"   "<<height<<"   "<<b_height*j<<endl;
			}
			else
			{
				fout<< "\t"<< (double) b_height/times;
			}

			fout<<"\t" << (double) b_width*i/times ;
			fout<<"\t" << (double) b_height*j/times ;
			fout<<"\t 1.631e+06"<<"\t0.00851064"<<endl;
			// cout<<"write "<<endl;
		}
	}
	fout.close();


	double specficheat = this->SiSpecificHeat;
	double resistivity = this->SiResistivity ;



	string OutFile_BEOL = file+ subfile +"_BEOL_1.flp";
	fout.open(OutFile_BEOL.c_str());
	fout<<"# Line Format: <unit-name>\t<width>\t<height>\t<left-x>\t<bottom-y>\t<specific-heat>\t<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;
	fout<<"active_wires_1 "<<w* 0.000001<<" "<<w* 0.000001<<" "<<0<< " " <<0<<" "<<specficheat<<" "<<resistivity<<endl;
	fout<<"BEOL_1 " << w* 0.000001 << " "<<h* 0.000001 << " "<<0<< " " <<0<< " "<<specficheat<< " "<<resistivity<<endl;

}

void DUMP::Corblivar::DumpPtraceHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
	cout<<" - Dump .ptrace File "<<endl;
	string Filename = tmpnt.GetoutputPath()+ benName + "_HotSpot.ptrace";
	ofstream fout(Filename.c_str());

	double Amp = this->PowerAmp;
	// From mW-> W
	for(int i=0; i<tmpnt.nLayer;i++)
	{
        fout << "active_wires_" << i << " ";
        fout << "BEOL_" << i << " ";

        fout << "outline_active" << i << " ";

        for(int j=0;j<tmpnt.nMod;j++)
        {
            if(InputOption.CompCorblivar)
            {
                if(tmpnt.mods[j].flagTSV)
                    continue;
            }


            if(tmpnt.mods[j].modL != i)
                continue;
            fout<<tmpnt.mods[j].modName<<" ";
        }

	}

	fout << endl;

	//power

    for(int i=0; i<tmpnt.nLayer;i++)
	{
        fout << "0.0150087 "; //active_wire
        fout << "0.0 "; //BEOL
        fout << "0.0 "; //outline_active
        for(int j=0;j<tmpnt.nMod;j++)
        {
           if(InputOption.CompCorblivar)
            {
                if(tmpnt.mods[j].flagTSV)
                    continue;
            }
            if(tmpnt.mods[j].modL != i)
                continue;
            fout<<(double)tmpnt.mods[j].Power * 0.001 * Amp<<" ";
            //cout <<"Power : "<< tmpnt.mods[i].Power * Amp <<" (mW) "<<endl;
        }
	}



	fout << endl;


	fout.close();
}

void DUMP::Corblivar::DumpLcfHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump Lcf File in Layer " <<endl;
    string OutFile_lcf = tmpnt.GetoutputPath()+ benName + "_HotSpot.lcf";
	ofstream fout (OutFile_lcf.c_str());

    ///command
	fout << "#Lines starting with # are used for commenting" << endl;
	fout << "##Blank lines are also ignored\n" << endl;
	fout << "#File Format:" << endl;
	fout << "#<Layer Number>" << endl;
	fout << "#<Lateral heat flow Y/N?>" << endl;
	fout << "#<Power Dissipation Y/N?>" << endl;
	fout << "#<Specific heat capacity in J/(m^3K)>" << endl;
	fout << "#<Resistivity in (m-K)/W>" << endl;
	fout << "#<Thickness in m>" << endl;
	fout << "#<floorplan file>\n" << endl;

    for(int i=0; i<tmpnt.nLayer; i++)
    {
        fout << "# BEOL (interconnects) layer " << i+1 << endl;
        fout << 4*i << endl;
        fout << "Y" << endl;
        fout << "Y" << endl;
        fout << "1.20815e+06" << endl;
        fout << "0.4444" << endl;
        fout << "1.2e-05" << endl;
        fout << benName + subfile << "_BEOL_" << i << ".flp\n" << endl;

        fout << "# Active Si layer; design layer " << i+1 << endl;
        fout << 4*i+1 << endl;
        fout << "Y" << endl;
        fout << "Y" << endl;
        fout << "1.631e+06" << endl;
        fout << "0.00851064" << endl;
        fout << "2e-06" << endl;
        fout << benName + subfile << "_active_" << i << ".flp\n" << endl;

        fout << "# Passive Si layer " << i+1 << endl;
        fout << 4*i+2 << endl;
        fout << "Y" << endl;
        fout << "N" << endl;
        fout << "1.631e+06" << endl;
        fout << "0.00851064" << endl;
        fout << "4.8e-05" << endl;
        fout << benName + subfile << "_passive_" << i << ".flp\n" << endl;

        if(i != tmpnt.nLayer-1)
        {
            fout << "# bond layer " << i+1 << "; for F2B bonding to next die " << i+1 << endl;
            fout << 4*i+3 << endl;
            fout << "Y" << endl;
            fout << "N" << endl;
            fout << "2.29854e+06" << endl;
            fout << "5" << endl;
            fout << "2e-05" << endl;
            fout << benName + subfile << "_bond_" << i << ".flp\n" << endl;
        }

    }
    fout.close();

}

void DUMP::Corblivar::DumpBeolHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump BEOL File in Layer " << this->NowNtLayer <<endl;
    //double w = (double)tmpnt.ChipWidth/AMP_PARA;
	//double h = (double)tmpnt.ChipHeight/AMP_PARA ;
    double specficheat = this->BEOLSpecificHeat;
	double resistivity = this->BEOLResistivity ;

	stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string OutFile_BEOL = file_prefix +"_BEOL_"+ str_layer + ".flp";

    ofstream fout;

	fout.open(OutFile_BEOL.c_str());
	fout<<"# Line Format: <unit-name>\t<width>\t<height>\t<left-x>\t<bottom-y>\t<specific-heat>\t<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;
	fout<<"active_wires_" << this->NowNtLayer << " "<<(double)tmpnt.ChipWidth/AMP_PARA/this->scale<<" "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale<<" "<<0<<" "<<0<<" "<<specficheat<<" "<<resistivity<<endl;
	fout<<"BEOL_" << this->NowNtLayer << " " <<(double) tmpnt.ChipWidth/AMP_PARA/this->scale << " "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale << " "<<0<< " "<<0<< " "<<specficheat<< " "<<resistivity<<endl;
    fout.close();


}

void DUMP::Corblivar::DumpActiveHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump Active File in Layer " << this->NowNtLayer <<endl;
    stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string Filename = file_prefix + "_active_" + str_layer + ".flp";

    double specficheat = this->SiSpecificHeat;
	double resistivity = this->SiResistivity ;

	ofstream fout(Filename.c_str());

	fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;

	for(int i=0;i<tmpnt.nMod;i++)
	{
        if(tmpnt.mods[i].modL == this->NowNtLayer)
        {
            fout<<tmpnt.mods[i].modName<<" ";
            fout<<(double) tmpnt.mods[i].modW/AMP_PARA/this->scale<<" "<<(double) tmpnt.mods[i].modH/AMP_PARA/this->scale<<" ";
            fout<<(double) tmpnt.mods[i].LeftX/(float)AMP_PARA/this->scale<< " "<<(double) tmpnt.mods[i].LeftY/(float)AMP_PARA/this->scale<<" " ;

            if(tmpnt.mods[i].flagTSV)
                fout<< this->CuSpecificHeat << " " << this->CuResistivity <<endl;
            else
                fout<< specficheat << " " << resistivity <<endl;
		}
	}

	fout << "outline_active" << this->NowNtLayer << " ";
	fout<<(double) tmpnt.ChipWidth/AMP_PARA/this->scale << " "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale << " ";
	fout<< 0 <<" " <<0 << " ";
	fout<< specficheat << " " << resistivity <<endl;

	fout.close();
}

void DUMP::Corblivar::DumpPassiveHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump Passive File in Layer " << this->NowNtLayer <<endl;
    stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string Filename = file_prefix + "_passive_" + str_layer + ".flp";
	ofstream fout(Filename.c_str());

    fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;

	for(int i=0;i<tmpnt.nMod;i++)
	{
        if(tmpnt.mods[i].modL == this->NowNtLayer && tmpnt.mods[i].flagTSV)
        {
            fout<<tmpnt.mods[i].modName <<"_passive ";
            fout<<(double) tmpnt.mods[i].modW/AMP_PARA/this->scale<<" "<<(double) tmpnt.mods[i].modH/AMP_PARA/this->scale<<" ";
            fout<<(double) tmpnt.mods[i].LeftX/(float)AMP_PARA/this->scale<< " "<<(double) tmpnt.mods[i].LeftY/(float)AMP_PARA/this->scale<<" " ;
            fout<< this->CuSpecificHeat << " " << this->CuResistivity <<endl;
		}
	}

    fout << "outline_passive" << this->NowNtLayer << " ";
	fout<<(double) tmpnt.ChipWidth/AMP_PARA/this->scale << " "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale << " ";
	fout<< 0 <<" " <<0 << " ";
	fout<< this->SiSpecificHeat << " " << this->SiResistivity <<endl;

	fout.close();
}

void DUMP::Corblivar::DumpBondHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump Bond File in Layer " << this->NowNtLayer <<endl;
    stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string Filename = file_prefix + "_bond_" + str_layer + ".flp";

	ofstream fout(Filename.c_str());

    fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;

	for(int i=0;i<tmpnt.nMod;i++)
	{
        if(tmpnt.mods[i].modL == this->NowNtLayer && tmpnt.mods[i].flagTSV)
        {
            fout<<tmpnt.mods[i].modName<<"_bond ";
            fout<<(double) tmpnt.mods[i].modW/AMP_PARA/this->scale<<" "<<(double) tmpnt.mods[i].modH/AMP_PARA/this->scale<<" ";
            fout<<(double) tmpnt.mods[i].LeftX/(float)AMP_PARA/this->scale<< " "<<(double) tmpnt.mods[i].LeftY/(float)AMP_PARA/this->scale<<" " ;
            fout<< this->CuSpecificHeat << " " << this->CuResistivity <<endl;
		}
	}

	fout << "outline_bond" << this->NowNtLayer << " ";
	fout<<(double) tmpnt.ChipWidth/AMP_PARA/this->scale << " "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale << " ";
	fout<< 0 <<" " <<0 << " ";
	fout<< this->BondSpecificHeat << " " << this->BondResistivity <<endl;

	fout.close();
}

void DUMP::Corblivar::DumpPassiveGridHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump Passive File in Layer " << this->NowNtLayer <<endl;

    ///open file
    stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string Filename = file_prefix + "_passive_" + str_layer + ".flp";
	ofstream fout(Filename.c_str());

	double GridWidth = (double) tmpnt.ChipWidth/(AMP_PARA*this->scale*(double)GridResolution);
	double GridHeight = (double) tmpnt.ChipHeight/(AMP_PARA*this->scale*(double)GridResolution);
	double GridArea = GridWidth*GridHeight;
	double SpecificHeat, Resistivity;

    fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;

    for(int i=0; i<this->GridResolution; i++)
    {
        for(int j=0; j<this->GridResolution; j++)
        {
            SpecificHeat = this->CuSpecificHeat*GridTSVRatioArr[i][j]+this->SiSpecificHeat*(1-GridTSVRatioArr[i][j]);
            //Resistivity = this->CuResistivity*GridTSVRatioArr[i][j]+this->SiResistivity*(1-GridTSVRatioArr[i][j]);

            if(this->GridTSVRatioArr[i][j]>=1)
                Resistivity = this->CuResistivity;
            else if(this->GridTSVRatioArr[i][j]<=0)
                Resistivity = this->SiResistivity;
            else
            {
                //Resistivity = ((this->CuResistivity*(GridTSVAreaArr[i][j]))*(this->SiResistivity*(GridArea-GridTSVAreaArr[i][j])))/((this->CuResistivity*(GridTSVAreaArr[i][j]))+(this->SiResistivity*(GridArea-GridTSVAreaArr[i][j])))*GridArea;
                //Resistivity = ((this->CuResistivity*(GridTSVRatioArr[i][j]))*(this->SiResistivity*(1-GridTSVRatioArr[i][j])))/((this->CuResistivity*(GridTSVRatioArr[i][j]))+(this->BondResistivity*(1-GridTSVRatioArr[i][j])))*100;
                Resistivity = 1/(1/(this->CuResistivity/(GridTSVRatioArr[i][j]))+1/(this->SiResistivity/(1-GridTSVRatioArr[i][j])));
            }

            double gridLeftX = GridWidth*i;
            double gridLowY = GridHeight*j;
            double ResetGridWidth = GridWidth;
            double ResetGridHeight = GridHeight;

            if(i == this->GridResolution-1)
            {
                ResetGridWidth = ((double)tmpnt.ChipWidth/(AMP_PARA*this->scale)-gridLeftX);

            }
            if(j == this->GridResolution-1)
                ResetGridHeight = (double) tmpnt.ChipHeight/(AMP_PARA*this->scale)-gridLowY;

            fout <<fixed << setprecision(8);
            fout << "passive_" << this->NowNtLayer << "_" << i << ":" << j << " ";
            fout <<setiosflags(ios::fixed)<<setprecision(10);
            fout <<ResetGridWidth<<" "<<ResetGridHeight<<" ";
            fout << gridLeftX << " "<< gridLowY <<" " ;
            fout.unsetf(std::ios_base::floatfield);
            fout << SpecificHeat << " " << Resistivity <<endl;

            //cout << i << ":" << j << " " << GridTSVRatioArr[i][j] << endl;
        }
    }
	fout.close();
}

void DUMP::Corblivar::DumpActiveGridHotspot(NETLIST& tmpnt, string file , string subfile, string benName)
{

    cout<<" - Dump Active File in Layer " << this->NowNtLayer <<endl;
    stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string Filename = file_prefix + "_active_" + str_layer + ".flp";

    double specficheat = this->SiSpecificHeat;
	double resistivity = this->SiResistivity ;

	ofstream fout(Filename.c_str());

	fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;

	for(int i=0;i<tmpnt.nMod;i++)
	{
        if(tmpnt.mods[i].modL == this->NowNtLayer)
        {
            if(tmpnt.mods[i].flagTSV)
                continue;
            fout<<tmpnt.mods[i].modName<<" ";
            fout<<(double) tmpnt.mods[i].modW/AMP_PARA/this->scale<<" "<<(double) tmpnt.mods[i].modH/AMP_PARA/this->scale<<" ";
            fout<<(double) tmpnt.mods[i].LeftX/(float)AMP_PARA/this->scale<< " "<<(double) tmpnt.mods[i].LeftY/(float)AMP_PARA/this->scale<<" " ;

            if(tmpnt.mods[i].flagTSV)
                fout<< this->CuSpecificHeat << " " << this->CuResistivity <<endl;
            else
                fout<< specficheat << " " << resistivity <<endl;
		}
	}

	fout << "outline_active" << this->NowNtLayer << " ";
	fout<<(double) tmpnt.ChipWidth/AMP_PARA/this->scale << " "<<(double) tmpnt.ChipHeight/AMP_PARA/this->scale << " ";
	fout<< 0 <<" " <<0 << " ";
	fout<< specficheat << " " << resistivity <<endl;

	fout.close();
}


void DUMP::Corblivar::DumpBondGridHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
    cout<<" - Dump Bond File in Layer " << this->NowNtLayer <<endl;
    stringstream ss;
    string str_layer;
    ss << this->NowNtLayer;
    ss >> str_layer;
	string file_prefix = tmpnt.GetoutputPath()+benName+subfile;
    string Filename = file_prefix + "_bond_" + str_layer + ".flp";

	ofstream fout(Filename.c_str());

	double GridWidth = (double) tmpnt.ChipWidth/(AMP_PARA*this->scale*(double)GridResolution);
	double GridHeight = (double) tmpnt.ChipHeight/(AMP_PARA*this->scale*(double)GridResolution);
	double GridArea = GridWidth*GridHeight;
	double SpecificHeat, Resistivity;

	//cout << GridWidth << " " << GridHeight << endl;

    fout<<"# Line Format: <unit-name>	<width>	<height>	<left-x>	<bottom-y>	<specific-heat>	<resistivity>"<<endl;
	fout<<"# all dimensions are in meters"<<endl;
	fout<<"# comment lines begin with a '#'"<<endl;
	fout<<"# comments and empty lines are ignored"<<endl;

    for(int i=0; i<this->GridResolution; i++)
    {
        for(int j=0; j<this->GridResolution; j++)
        {
            SpecificHeat = this->CuSpecificHeat*GridTSVRatioArr[i][j]+this->BondSpecificHeat*(1-GridTSVRatioArr[i][j]);
            Resistivity = this->CuResistivity*GridTSVRatioArr[i][j]+this->BondResistivity*(1-GridTSVRatioArr[i][j]);

            if(this->GridTSVRatioArr[i][j]>=1)
                Resistivity = this->CuResistivity;
            else if(this->GridTSVRatioArr[i][j]<=0)
                Resistivity = this->BondResistivity;
            else
            {
                //Resistivity = ((this->CuResistivity*(GridTSVAreaArr[i][j]))*(this->BondResistivity*(GridArea-GridTSVAreaArr[i][j])))/((this->CuResistivity*(GridTSVAreaArr[i][j]))+(this->BondResistivity*(GridArea-GridTSVAreaArr[i][j])));
                //Resistivity = ((this->CuResistivity*(GridTSVRatioArr[i][j]))*(this->BondResistivity*(1-GridTSVRatioArr[i][j])))/((this->CuResistivity*(GridTSVRatioArr[i][j]))+(this->BondResistivity*(1-GridTSVRatioArr[i][j])))*1000;
                Resistivity = 1/(1/(this->CuResistivity/(GridTSVRatioArr[i][j]))+1/(this->BondResistivity/(1-GridTSVRatioArr[i][j])));
                /*if(Resistivity<0)
                {
                    cout << "Error" << endl;
                    cout << GridTSVRatioArr[i][j] << endl;
                    getchar();
                }*/
            }
            fout<< "bond_" << this->NowNtLayer << "_" << i << ":" << j << " ";
            fout <<setiosflags(ios::fixed)<<setprecision(10);
            fout<<GridWidth<<" "<<GridHeight<<" ";

            fout<< GridWidth*i << " "<< GridHeight*j <<" " ;
            fout.unsetf(std::ios_base::floatfield);
            fout<< SpecificHeat << " " << Resistivity <<endl;

            //cout << i << ":" << j << " " << GridTSVRatioArr[i][j] << endl;
        }
    }

	fout.close();
}

void DUMP::Corblivar::CalculateGridTSVRatio(NETLIST& tmpnt)
{
    double ChipWidth = (double)tmpnt.ChipWidth;
    double ChipHeight = (double)tmpnt.ChipHeight;

    double GridWidth = ChipWidth / this->GridResolution;
    double GridHeight = ChipHeight / this->GridResolution;
    double GridArea = GridWidth*GridHeight;
    double ModLeftX;
    double ModLeftY;
    double tempurx, tempury, templlx, templly, tempArea;
    int GridLeftX, GridLeftY, GridRightX, GridRightY;

    //cout << GridWidth << "\t" << GridHeight << endl;

    ///initial array
    for(int i=0; i<this->GridResolution; i++)
        for(int j=0; j<this->GridResolution; j++)
        {
             this->GridTSVRatioArr[i][j] = 0;
             this->GridTSVAreaArr[i][j] = 0;
        }



    for(int i=0;i<tmpnt.nMod;i++)
	{
        if(tmpnt.mods[i].modL == this->NowNtLayer && tmpnt.mods[i].flagTSV)
        {
            ModLeftX = (double)tmpnt.mods[i].LeftX;
            ModLeftY = (double)tmpnt.mods[i].LeftY;

            GridLeftX = floor(ModLeftX/GridWidth);
            GridLeftY = floor(ModLeftY/GridWidth);
            GridRightX = floor((ModLeftX+tmpnt.mods[i].modW)/GridWidth);
            GridRightY = floor((ModLeftY+tmpnt.mods[i].modH)/GridWidth);

            //cout << tmpnt.mods[i].modName << "\t" << ModLeftX << "\t" <<  ModLeftY << "\t" <<  tmpnt.mods[i].modW << "\t" <<  tmpnt.mods[i].modH << endl;
            //cout << GridLeftX << "\t" << GridLeftY << "\t" << GridRightX << "\t" << GridRightY << endl;
            for(int j=GridLeftX; j<(GridRightX+1); j++)
            {
                for(int k=GridLeftY; k<(GridRightY+1); k++)
                {
                    ///
                    tempurx=((ModLeftX+tmpnt.mods[i].modW)>((double)(j+1)*GridWidth))?
                            (double)(j+1)*GridWidth : (ModLeftX+tmpnt.mods[i].modW);
                    tempury=((ModLeftY+tmpnt.mods[i].modH)>((double)(k+1)*GridHeight))?
                            (double)(k+1)*GridHeight : (ModLeftY+tmpnt.mods[i].modH);
                    templlx=(ModLeftX>((double)(j)*GridWidth))?
                            ModLeftX : (double)(j)*GridWidth;
                    templly=(ModLeftY>((double)(k)*GridHeight))?
                            ModLeftY : (double)(k)*GridHeight;

                    ///update TSV Ratio Array
                    tempArea=(tempurx-templlx)*(tempury-templly);
                    this->GridTSVRatioArr[j][k]+=(tempArea/GridArea);
                    this->GridTSVAreaArr[j][k]+=(tempArea);

                    ///test
                    /*cout << tempurx << "\t" << tempury << "\t" << templlx << "\t" << templly << endl;
                    cout << tempArea << " " << GridArea << endl;
                    getchar();*/
                }
            }
		}
	}

	///check
	/*for(int i=0; i<this->GridResolution; i++)
        for(int j=0; j<this->GridResolution; j++)
            if(this->GridTSVRatioArr[i][j] > 1)
            {
                this->GridTSVRatioArr[i][j] = 1;
                this->GridTSVAreaArr[i][j] = GridArea;

                cout << "[Error]\tTSV ratio overload at " << i << " " << j << endl;
                getchar();
            }*/
}

void DUMP::Corblivar::DumpFlpHotspot(NETLIST& tmpnt, string file, string subfile, string benName)
{
	for(int i=0; i<tmpnt.nLayer; i++)
	{
	    if(InputOption.CompCorblivar)
        {
            DUMP::Corblivar::DumpBeolHotspot(tmpnt, file, subfile, benName);
            DUMP::Corblivar::DumpActiveGridHotspot(tmpnt, file, subfile, benName);
            ///calculate TSV area Ratio in each grid
            DUMP::Corblivar::CalculateGridTSVRatio(tmpnt);

            DUMP::Corblivar::DumpPassiveGridHotspot(tmpnt, file, subfile, benName);

            if(i != tmpnt.nLayer-1)
                DUMP::Corblivar::DumpBondGridHotspot(tmpnt, file, subfile, benName);
        }
        else
        {
            DUMP::Corblivar::DumpBeolHotspot(tmpnt, file, subfile, benName);
            DUMP::Corblivar::DumpActiveHotspot(tmpnt, file, subfile, benName);
            DUMP::Corblivar::DumpPassiveHotspot(tmpnt, file, subfile, benName);

            if(i != tmpnt.nLayer-1)
                DUMP::Corblivar::DumpBondHotspot(tmpnt, file, subfile, benName);

        }


        this->NowNtLayer++;

	}
}

void DUMP::Corblivar::DumpFiles(NETLIST& tmpnt, string file, string subfile, string benName)
{
	cout<< BOLD(Cyan( "\\ ===== Dump HotSpot Solver Files ===== //" ))<<endl;
	cout<< " - Dump Files : "<<endl;
	this->HotspotLayer=0;
	this->NowNtLayer=0;
	///origin dump


    //DUMP::Corblivar::DumpPowerHotSpot(tmpnt, file , subfile);
    DUMP::Corblivar::DumpPowerFile_EVA(tmpnt, file , subfile);
    DUMP::Corblivar::DumpLayoutFile(tmpnt, file , subfile,benName);
    //DUMP::Corblivar::DumpFpInFo(tmpnt, file , subfile,benName);
    //DUMP::Corblivar::DumpPlHotSpot(tmpnt, file , subfile);
    //DUMP::Corblivar::DumpPassive(tmpnt, file , subfile);

    ///new dump
    DUMP::Corblivar::DumpPtraceHotspot(tmpnt, file, subfile, benName);
    DUMP::Corblivar::DumpLcfHotspot(tmpnt, file, subfile, benName);
    DUMP::Corblivar::DumpFlpHotspot(tmpnt, file, subfile, benName);

}

void PLOT::dump_deflef(NETLIST nt,string blockFile) {
	double chipw;
	double chiph;
	double ppa = 100;
	string fileName_in = blockFile + ".txt";
	string fileName_out;
	ifstream fin;
	ofstream fout;
	vector<string> sec;
	string temp;
	stringstream sstemp;
	string strtemp;
	
	fin.open(fileName_in.c_str());
	if (!fin.is_open() ) {
		cout << "IO error\n";
		exit(EXIT_FAILURE);
	}
	
	// 之後改掉
	bool h_read = false;
	bool reverse=false;
	string top_tech;
	string bot_tech;
	string first_tech;
	string seccond_tech;
	int first_tech_flag=0;
	int case_tech=0;
	sstemp.str(string());
	sstemp.clear();
	int lib_num = 0;
	int t_d = 0;
	vector<string> for_same;
	string for_same_temp;
	while (getline(fin, strtemp))
	{
		sstemp << strtemp;
		sstemp >> strtemp;
		if (strtemp == "NumTechnologies" ) {
			int tecnum;
			sstemp >> tecnum;
			
			if(tecnum==1)
				h_read = true;
			else{
				sstemp.str(string());
				sstemp.clear();
				while(getline(fin, strtemp))
				{
					sstemp << strtemp;
					sstemp >> strtemp;
					if(strtemp=="Tech"){
						if(first_tech_flag==0){
							first_tech_flag++;
							sstemp >> first_tech;
							//cout<<first_tech<<endl;
						}
						else{
							sstemp >> seccond_tech;
							//cout<<seccond_tech<<endl;
						}
						
					}
					else if(strtemp=="TopDieTech"){
						sstemp >> top_tech;
						//cout<<top_tech<<endl;
					}
					else if(strtemp=="BottomDieTech"){
						sstemp >> bot_tech;
						//cout<<bot_tech<<endl;
					}
					else{
						sstemp.str(string());
						sstemp.clear();
					}
					sstemp.str(string());
					sstemp.clear();
				}
				sstemp.str(string());
				sstemp.clear();
				fin.close();
				fin.open(fileName_in.c_str());
				if (!fin.is_open() ) {
					cout << "txt second open error\n";
					exit(EXIT_FAILURE);
				}
				getline(fin, strtemp);
				sstemp.str(string());
				sstemp.clear();
				if(first_tech==top_tech&&first_tech==bot_tech){
					reverse=false;
					h_read=true;
					case_tech=1;
				}
				else if(first_tech==top_tech&&seccond_tech==bot_tech){
					reverse=false;
					h_read=false;
					case_tech=2;
				}
				else if(first_tech==bot_tech&&seccond_tech==top_tech){
					reverse=true;
					h_read=false;
					case_tech=3;
				}
				else if(seccond_tech==top_tech&&seccond_tech==bot_tech){
					reverse=true;
					h_read=true;
					case_tech=4;
				}
				//cout<<"case: "<<case_tech<<endl;
			}
			
			for(int t_d=0;t_d<tecnum;t_d++){
			if(case_tech==0){
				
				fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.lef";
				
			}
			else if(case_tech==1){
				if(t_d==0)
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.lef";
				else {
					break;
				}
			}
			else if(case_tech==2){
				if(t_d==0)
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.lef";
				else {
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.lef";
				}
			}
			else if(case_tech==3){
				if(t_d==0)
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.lef";
				else {
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.lef";
				}
			}
			else if(case_tech==4){
				if(t_d==0)
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_discard.lef";
				else {
					fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.lef";
				}
			}
			else{
				cout<<"read tech error\n";
			}
			fout.open(fileName_out.c_str());
			if (!fout.is_open()) {
				cout << "IO error\n";
				exit(EXIT_FAILURE);
			}
			fout << "#\n# LEF OUT\n# User Name : m103bhyu\n# Date : Tue Nov 24 17:18:34 2015\n#\nVERSION 5.5 ;\nNAMESCASESENSITIVE ON ;\nBUSBITCHARS \"[]\" ;\nDIVIDERCHAR \" / \" ;\n\nUNITS\n";

			for_same_temp="#\n# LEF OUT\n# User Name : m103bhyu\n# Date : Tue Nov 24 17:18:34 2015\n#\nVERSION 5.5 ;\nNAMESCASESENSITIVE ON ;\nBUSBITCHARS \"[]\" ;\nDIVIDERCHAR \" / \" ;\n\nUNITS\n";
			for_same.push_back(for_same_temp);
			fout << "  DATABASE MICRONS " << ppa << " ;\nEND UNITS\nMANUFACTURINGGRID 0.005 ;\n\n";
			for_same_temp="  DATABASE MICRONS " + to_string(ppa) + " ;\nEND UNITS\nMANUFACTURINGGRID 0.005 ;\n\n";
			for_same.push_back(for_same_temp);
			fout << "LAYER ME1\n  TYPE ROUTING ;\n  DIRECTION HORIZONTAL ;\n  PITCH 0.4 ;\n  WIDTH 0.16 ;\n  AREA 0.1024 ;\n  SPACINGTABLE\n    PARALLELRUNLENGTH    9.389191e-17 \n      WIDTH 8.435379e-17 0      ;\n  MAXWIDTH 25 ;\n  MINWIDTH 0.16 ;\n  MINENCLOSEDAREA 0.3072 ;\n  MINENCLOSEDAREA 0.3072 ;\n  CAPMULTIPLIER 1 ;\n  MINIMUMDENSITY 20 ;\n  MAXIMUMDENSITY 80 ;\n  DENSITYCHECKWINDOW 200 200 ;\n  DENSITYCHECKSTEP 100 ;\nEND ME1\n\n";
			for_same_temp= "LAYER ME1\n  TYPE ROUTING ;\n  DIRECTION HORIZONTAL ;\n  PITCH 0.4 ;\n  WIDTH 0.16 ;\n  AREA 0.1024 ;\n  SPACINGTABLE\n    PARALLELRUNLENGTH    9.389191e-17 \n      WIDTH 8.435379e-17 0      ;\n  MAXWIDTH 25 ;\n  MINWIDTH 0.16 ;\n  MINENCLOSEDAREA 0.3072 ;\n  MINENCLOSEDAREA 0.3072 ;\n  CAPMULTIPLIER 1 ;\n  MINIMUMDENSITY 20 ;\n  MAXIMUMDENSITY 80 ;\n  DENSITYCHECKWINDOW 200 200 ;\n  DENSITYCHECKSTEP 100 ;\nEND ME1\n\n";
			for_same.push_back(for_same_temp);
			sstemp.str(string());
			sstemp.clear();
			getline(fin, strtemp);
			sstemp << strtemp;
			sstemp >> strtemp >> strtemp >> lib_num;
			sstemp.str(string());
			sstemp.clear();

			fout<<"MACRO he\n";
			for_same_temp="MACRO he\n";
			for_same.push_back(for_same_temp);
			fout<<"  CLASS PAD ;\n";
			for_same_temp="  CLASS PAD ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SOURCE USER ;\n";
			for_same_temp="  SOURCE USER ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  ORIGIN 0 0 ;\n";
			for_same_temp="  ORIGIN 0 0 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SIZE "<< 0.01 <<" BY "<<((double)nt.Geth()/(double)ppa)-2*((double)nt.Geth())/(1000*(double)ppa)<<" ;\n";
			for_same_temp="  SIZE "+to_string(0.01)+" BY "+to_string(((double)nt.Geth()/(double)ppa)-2*((double)nt.Geth())/(1000*(double)ppa))+" ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SYMMETRY X Y ;\n\n";
			for_same_temp="  SYMMETRY X Y ;\n\n";
			for_same.push_back(for_same_temp);
			fout<<"  PIN VSSC\n";
			for_same_temp="  PIN VSSC\n";
			for_same.push_back(for_same_temp);
			fout<<"    DIRECTION INOUT ;\n";
			for_same_temp="    DIRECTION INOUT ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    USE SIGNAL ;\n";
			for_same_temp="    USE SIGNAL ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    PORT\n";
			for_same_temp="    PORT\n";
			for_same.push_back(for_same_temp);
			fout<<"      LAYER ME1 ;\n";
			for_same_temp="      LAYER ME1 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"        RECT 0 0 0 0 ;\n";
			for_same_temp="        RECT 0 0 0 0 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    END\n";
			for_same_temp="    END\n";
			for_same.push_back(for_same_temp);
			fout<<"  END VSSC\n";
			for_same_temp="  END VSSC\n";
			for_same.push_back(for_same_temp);
			fout<<"END he\n\n";
			for_same_temp="END he\n\n";
			for_same.push_back(for_same_temp);

			fout<<"MACRO wi\n";
			for_same_temp="MACRO wi\n";
			for_same.push_back(for_same_temp);
			fout<<"  CLASS PAD ;\n";
			for_same_temp="  CLASS PAD ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SOURCE USER ;\n";
			for_same_temp="  SOURCE USER ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  ORIGIN 0 0 ;\n";
			for_same_temp="  ORIGIN 0 0 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SIZE "<<((double)nt.Getw()/(double)ppa)-2*((double)nt.Getw())/(1000*(double)ppa)<<" BY "<< 0.01 <<" ;\n";
			for_same_temp="  SIZE "+to_string(((double)nt.Getw()/(double)ppa)-2*((double)nt.Getw())/(1000*(double)ppa))+" BY "+to_string(0.01)+" ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SYMMETRY X Y ;\n\n";
			for_same_temp="  SYMMETRY X Y ;\n\n";
			for_same.push_back(for_same_temp);
			fout<<"  PIN VSSC\n";
			for_same_temp="  PIN VSSC\n";
			for_same.push_back(for_same_temp);
			fout<<"    DIRECTION INOUT ;\n";
			for_same_temp="    DIRECTION INOUT ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    USE SIGNAL ;\n";
			for_same_temp="    USE SIGNAL ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    PORT\n";
			for_same_temp="    PORT\n";
			for_same.push_back(for_same_temp);
			fout<<"      LAYER ME1 ;\n";
			for_same_temp="      LAYER ME1 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"        RECT 0 0 0 0 ;\n";
			for_same_temp="        RECT 0 0 0 0 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    END\n";
			for_same_temp="    END\n";
			for_same.push_back(for_same_temp);
			fout<<"  END VSSC\n";
			for_same_temp="  END VSSC\n";
			for_same.push_back(for_same_temp);
			fout<<"END wi\n\n";
			for_same_temp="END wi\n\n";
			for_same.push_back(for_same_temp);

			fout<<"MACRO mm\n";
			for_same_temp="MACRO mm\n";
			for_same.push_back(for_same_temp);
			fout<<"  CLASS PAD ;\n";
			for_same_temp="  CLASS PAD ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SOURCE USER ;\n";
			for_same_temp="  SOURCE USER ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  ORIGIN 0 0 ;\n";
			for_same_temp="  ORIGIN 0 0 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SIZE "<< 0.01 <<" BY "<< 0.01 <<" ;\n";
			for_same_temp="  SIZE "+to_string(0.01)+" BY "+to_string(0.01)+" ;\n";
			for_same.push_back(for_same_temp);
			fout<<"  SYMMETRY X Y ;\n\n";
			for_same_temp="  SYMMETRY X Y ;\n\n";
			for_same.push_back(for_same_temp);
			fout<<"  PIN VSSC\n";
			for_same_temp="  PIN VSSC\n";
			for_same.push_back(for_same_temp);
			fout<<"    DIRECTION INOUT ;\n";
			for_same_temp="    DIRECTION INOUT ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    USE SIGNAL ;\n";
			for_same_temp="    USE SIGNAL ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    PORT\n";
			for_same_temp="    PORT\n";
			for_same.push_back(for_same_temp);
			fout<<"      LAYER ME1 ;\n";
			for_same_temp="      LAYER ME1 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"        RECT 0 0 0 0 ;\n";
			for_same_temp="        RECT 0 0 0 0 ;\n";
			for_same.push_back(for_same_temp);
			fout<<"    END\n";
			for_same_temp="    END\n";
			for_same.push_back(for_same_temp);
			fout<<"  END VSSC\n";
			for_same_temp="  END VSSC\n";
			for_same.push_back(for_same_temp);
			fout<<"END mm\n\n";
			for_same_temp="END mm\n\n";
			for_same.push_back(for_same_temp);


			for (int i = 0; i < lib_num; i++) {
				string name;
				string is_m;
				double w;
				double h;
				int pin;
				getline(fin, strtemp);
				sstemp << strtemp;
				sstemp >> strtemp >> is_m >> name >> w >> h >> pin;
				sstemp.str(string());
				sstemp.clear();
				fout << "MACRO " << name << "\n";
				for_same_temp="MACRO " + name + "\n";
				for_same.push_back(for_same_temp);
				if (is_m == "N") {
					fout << "  CLASS CORE ;\n";
					for_same_temp="  CLASS CORE ;\n";
					for_same.push_back(for_same_temp);

				}
				else {
					fout << "  CLASS BLOCK ;\n";
					for_same_temp="  CLASS BLOCK ;\n";
					for_same.push_back(for_same_temp);
				}
				fout << "  SOURCE USER ;\n";
				for_same_temp="  SOURCE USER ;\n";
				for_same.push_back(for_same_temp);
				fout << "  ORIGIN 0 0 ;\n";
				for_same_temp="  ORIGIN 0 0 ;\n";
				for_same.push_back(for_same_temp);
				fout << "  SIZE " << w/ppa << " BY " << h/ppa << " ;\n";
				for_same_temp="  SIZE " + to_string(w/ppa) + " BY " + to_string(h/ppa) + " ;\n";
				for_same.push_back(for_same_temp);
				if (is_m == "N") {
					fout << "  SYMMETRY ;\n";
					for_same_temp="  SYMMETRY ;\n";
					for_same.push_back(for_same_temp);
				}
				else {
					fout << "  SYMMETRY R90 ;\n\n";
					for_same_temp="  SYMMETRY R90 ;\n\n";
					for_same.push_back(for_same_temp);
				}
				for (int j = 0; j < pin; j++) {
					string p_name;
					double x;
					double y;
					getline(fin, strtemp);
					sstemp << strtemp;
					sstemp >> strtemp >> p_name >> x >> y;
					sstemp.str(string());
					sstemp.clear();
					fout << "  PIN " << p_name << "\n";
					for_same_temp="  PIN " + p_name + "\n";
					for_same.push_back(for_same_temp);
					fout << "    DIRECTION INOUT ;\n    USE SIGNAL ;\n    PORT\n      LAYER ME1 ;\n        RECT " << x/ppa << " " << y/ppa << " " << x/ppa << " " << y/ppa << " ;\n    END\n";
					for_same_temp="    DIRECTION INOUT ;\n    USE SIGNAL ;\n    PORT\n      LAYER ME1 ;\n        RECT " +to_string( x/ppa) + " " + to_string(y/ppa) + " " + to_string(x/ppa) + " " + to_string(y/ppa) + " ;\n    END\n";
					for_same.push_back(for_same_temp);
					fout << "  END " << p_name << "\n\n";
					for_same_temp="  END " + p_name + "\n\n";
					for_same.push_back(for_same_temp);

				}
				fout << "END " << name << "\n";
				for_same_temp="END " + name + "\n";
				for_same.push_back(for_same_temp);
				
			}
			fout << "\nEND LIBRARY\n";
			for_same_temp="\nEND LIBRARY\n";
			for_same.push_back(for_same_temp);
			fout.close();
			if(h_read==true&&t_d==0&&reverse==true){
				for_same.clear();
			}
			}
			if (h_read == true && reverse==false) {//記得寫好

				fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.lef";
				fout.open(fileName_out.c_str());
				if (!fout.is_open()) {
					cout << "IO error\n";
					exit(EXIT_FAILURE);
				}
				for(int iij=0;iij<for_same.size();iij++){
					fout<<for_same[iij];
				}
				fout.close();
			}
			else if (h_read == true && reverse==true) {//記得寫好

				fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.lef";
				fout.open(fileName_out.c_str());
				if (!fout.is_open()) {
					cout << "IO error\n";
					exit(EXIT_FAILURE);
				}
				for(int iij=0;iij<for_same.size();iij++){
					fout<<for_same[iij];
				}
				fout.close();
			}
		}
		
		else if (strtemp == "DieSize") {
			
			fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.def";
			fout.open(fileName_out.c_str());
			if (!fout.is_open()) {
				cout << "O error\n";
				exit(EXIT_FAILURE);
			}
			

			sstemp >> strtemp >> strtemp >> chipw >> chiph;
			sstemp.str(string());
			sstemp.clear();
			fout << "#\n# SNPS write_def\n# Release      : N - 2017.09 - SP2\n# User Name : m108pclu\n# Date : Tue Apr  6 16 : 14 : 58 2021\n# \nVERSION 5.6 ;\nDIVIDERCHAR \" / \" ;\nBUSBITCHARS \"[]\" ;\nDESIGN " << blockFile << " ;\nTECHNOLOGY u110lscsp12bdr ;\nUNITS DISTANCE MICRONS " << ppa << " ;\n";
			fout << "DIEAREA ( 0 0 ) ( " << chipw << " " << chiph << " ) ;\n";
			fout << "TRACKS Y 200 DO "<<chipw/400<<" STEP 400 LAYER ME1 ;\nTRACKS X 200 DO "<<chiph/400<<" STEP 400 LAYER ME1 ;\n";
			temp= "#\n# SNPS write_def\n# Release      : N - 2017.09 - SP2\n# User Name : m108pclu\n# Date : Tue Apr  6 16 : 14 : 58 2021\n# \nVERSION 5.6 ;\nDIVIDERCHAR \" / \" ;\nBUSBITCHARS \"[]\" ;\nDESIGN " + blockFile + " ;\nTECHNOLOGY u110lscsp12bdr ;\nUNITS DISTANCE MICRONS " + to_string(ppa) + " ;\n";
			sec.push_back(temp);
			temp= "DIEAREA ( 0 0 ) ( " + to_string(chipw) + " " + to_string(chiph) + " ) ;\n";
			sec.push_back(temp);
			temp= "TRACKS Y 200 DO "+to_string(chipw/400)+" STEP 400 LAYER ME1 ;\nTRACKS X 200 DO "+to_string(chiph/400)+" STEP 400 LAYER ME1 ;\n";
			sec.push_back(temp);

		}
		else if (strtemp == "NumInstances") {
			int c_n;

			sstemp >> c_n;
			vector<string> for_top;
			int top_n = 0;
			int la1= nt.GetLayernMod(1);
			la1=la1+5;
			
			temp= "COMPONENTS " + to_string(la1) + " ;\n";
			sec.push_back(temp);
			temp=" - h1 he + FIXED ( 0 "+to_string(1.0)+" ) N ;\n";
			sec.push_back(temp);
			temp=" - h2 he + FIXED ( "+to_string((double)nt.Getw()-1.0)+" "+to_string(1.0)+" ) N ;\n";
			sec.push_back(temp);
			temp=" - w1 wi + FIXED ( "+to_string(1.0)+" 0 ) N ;\n";
			sec.push_back(temp);
			temp=" - w2 wi + FIXED ( "+to_string(1.0)+" "+to_string((double)nt.Geth()-1.0)+" ) N ;\n";
			sec.push_back(temp);
			temp=" - m1 mm + FIXED ( "+to_string((double)nt.Getw()/2)+" "+to_string((double)nt.Geth()/2)+" ) N ;\n";
			sec.push_back(temp);
			sstemp.str(string());
			sstemp.clear();
			for (int i = 0; i < c_n; i++) {
				string c_name;
				string c_type;
				getline(fin, strtemp);
				sstemp << strtemp;
				sstemp >> strtemp >> c_name >> c_type;
				sstemp.str(string());
				sstemp.clear();
				if (nt.mods[nt.GetModID(c_name)].modL == 0) {
					temp= " - " + c_name + " " + c_type + " + UNPLACED ;\n";
					for_top.push_back(temp);
					top_n++;
				}
				else {
					temp= " - " + c_name + " " + c_type + " + UNPLACED ;\n";
					sec.push_back(temp);
				}
			}
			top_n=top_n+5;
			fout<< "COMPONENTS " << top_n<< " ;\n";
			fout<<" - h1 he + FIXED ( 0 "<<to_string(1.0)<<" ) N ;\n";
			fout<<" - h2 he + FIXED ( "<<to_string((double)nt.Getw()-1.0)<<" "<<to_string(1.0)<<" ) N ;\n";
			fout<<" - w1 wi + FIXED ( "<<to_string(1.0)<<" 0 ) N ;\n";
			fout<<" - w2 wi + FIXED ( "<<to_string(1.0)<<" "<<to_string((double)nt.Geth()-1.0)<<" ) N ;\n";
			fout<<" - m1 mm + FIXED ( "<<to_string((double)nt.Getw()/2)<<" "<<to_string((double)nt.Geth()/2)<<" ) N ;\n";
			for (int i = 0; i < for_top.size(); i++) {
				fout << for_top[i];
			}
			fout << "END COMPONENTS\n\n";
			temp= "END COMPONENTS\n\n";
			sec.push_back(temp);
		}
		else if (strtemp == "NumNets") {
			int n_n;
			sstemp >> n_n;
			sstemp.str(string());
			sstemp.clear();
			//fout << "NETS " << n_n << " ;\n";
			int top_net = 0;
			int down_net = 0;
			vector<string> top_file;
			vector<string> bot_file;
			vector<int> cover(2,0);
			//int cover[2]{ 0 };
			

			for (int i = 0; i < n_n; i++) {
				cover[0] = 0;
				cover[1] = 0;
				string net_n;
				int p_n;
				getline(fin, strtemp);
				sstemp << strtemp;
				sstemp >> strtemp >> net_n >> p_n;
				//fout << " - " << net_n << "\n";
				temp= " - " + net_n + "\n";
				top_file.push_back(temp);
				temp = " - " + net_n + "\n";
				bot_file.push_back(temp);
				sstemp.str(string());
				sstemp.clear();
				for (int j = 0; j < p_n; j++) {
					string pin_name;
					string pin_cell;
					getline(fin, strtemp);
					sstemp << strtemp;
					sstemp >> strtemp >> strtemp;

					string f_s("/");
					std::size_t found = strtemp.find(f_s);
					int find_pos = static_cast<int>(found);
					string fiind1 = strtemp.substr(0, find_pos);
					string fiind2 = strtemp.substr(find_pos + 1, strtemp.size() - find_pos - 1);
					temp= "   ( " + fiind1 + " " + fiind2 + " )\n";
					if (nt.mods[nt.GetModID(fiind1)].modL == 0) {
						top_file.push_back(temp);
						cover[0]++;
					}
					else {
						bot_file.push_back(temp);
						cover[1] ++;
					}


					sstemp.str(string());
					sstemp.clear();

					

					//fout << "   ( " << fiind1 << " " << fiind2 << " )\n";
				}
				if (cover[0] <2) {
					if (cover[0] == 1) {
						top_file.pop_back();
						top_file.pop_back();
					}
					else if (cover[0] == 0) {
						top_file.pop_back();
					}


					/*for (int k = cover[0]; k >= 0; k--) {
						top_file.pop_back();
					}*/
				}
				else {
					top_net++;
					temp= "\n   + USE SIGNAL\n   ;\n";
					top_file.push_back(temp);
				}
				if (cover[1] <2) {

					if (cover[1] == 1) {
						bot_file.pop_back();
						bot_file.pop_back();
					}
					else if (cover[1] == 0) {
						bot_file.pop_back();
					}
					/*for (int k = cover[1]; k >= 0; k--) {
						bot_file.pop_back();
					}*/
				}
				else {
					down_net++;
					temp = "\n   + USE SIGNAL\n   ;\n";
					bot_file.push_back(temp);
				}
				//fout << "\n   + USE SIGNAL\n   ;\n";
			}
			//fout << "END NETS\n";
			temp= "END NETS\n";
			top_file.push_back(temp);
			bot_file.push_back(temp);
			//fout << "END DESIGN\n";
			temp= "END DESIGN\n";
			top_file.push_back(temp);
			bot_file.push_back(temp);

			fout << "NETS " << top_net << " ;\n";
			for (int i = 0; i < top_file.size(); i++) {
				fout << top_file[i];
			}
			fout.close();
			fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.def";
			fout.open(fileName_out.c_str());
			if (!fout.is_open()) {
				cout << "O error\n";
				exit(EXIT_FAILURE);
			}
			for (int i = 0; i < sec.size(); i++) {
				fout << sec[i];
			}
			fout << "NETS " << down_net << " ;\n";
			for (int i = 0; i < bot_file.size(); i++) {
				fout << bot_file[i];
			}
			fout.close();
		}
		else {
			sstemp.str(string());
			sstemp.clear();
		}
	}
}


void PLOT::dump_pl(NETLIST nt, string blockFile,int mod_num,int gp_ml) {
	if(gp_ml==0){
	//string  oout[mod_num]  { "" };
	vector<string> oout(mod_num,"");
	int p = 0;
	string fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.pl";
	ofstream fout(fileName_out.c_str());
	if (!fout.is_open()) {
		cout << "IO error\n";
		exit(EXIT_FAILURE);
	}
	fout << "UCLA pl 1.0 (by SEDALAB 2018)\n\n";
	oout[p]= "UCLA pl 1.0 (by SEDALAB 2018)\n\n";
	p++;
	for (int i = 0; i < mod_num; i++) {
		if (nt.mods[i].modL == 0) {
			if(nt.mods[i].modName[0]!='T'&&nt.mods[i].modName[1]!='S'&&nt.mods[i].modName[2]!='V'){
			fout << nt.mods[i].modName << "		" << nt.mods[i].GLeftX << "		" << nt.mods[i].GLeftY << "		:" << "	N\n";
			}
		}
		else {
			string a = nt.mods[i].modName;
			string in=  a+ "		" + to_string(nt.mods[i].GLeftX) + "		" + to_string(nt.mods[i].GLeftY) + "		:	N\n";
			oout[p] = in;
			p++;
		}
	}

	fout<<"h1               0               "<<to_string(1.0)<<"            : 	 N 	 /FIXED\n";
	fout<<"h2               "<<to_string((double)nt.Getw()-1.0)<<"         "<<to_string(1.0)<<"            : 	 N 	 /FIXED\n";
	fout<<"w1               "<<to_string(1.0)<<"            0               : 	 N 	 /FIXED\n";
	fout<<"w2               "<<to_string(1.0)<<"            "<<to_string((double)nt.Geth()-1.0)<<"         : 	 N 	 /FIXED\n";
	fout<<"m1		"<<to_string(1.0)<<"			"<<to_string((double)nt.Geth()/2)<<"         : 	 N 	 /FIXED\n";

	oout[p]="h1               0               "+to_string(1.0)+"            : 	 N 	 /FIXED\n";
	p++;
	oout[p]="h2               "+to_string((double)nt.Getw()-1.0)+"         "+to_string(1.0)+"            : 	 N 	 /FIXED\n";
	p++;
	oout[p]="w1               "+to_string(1.0)+"            0               : 	 N 	 /FIXED\n";
	p++;
	oout[p]="w2               "+to_string(1.0)+"            "+to_string((double)nt.Geth()-1.0)+"         : 	 N 	 /FIXED\n";
	p++;
	oout[p]="m1               "+to_string((double)nt.Getw()/2)+"		"+to_string((double)nt.Geth()/2)+"         : 	 N 	 /FIXED\n";
	p++;


	fout.close();
	fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.pl";
	fout.open(fileName_out.c_str());
	if (!fout.is_open()) {
		cout << "IO error\n";
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < mod_num; i++) {
		string temp = oout[i];
		if (temp[0] == '\0') {
			break;
		}
		else {
			fout << oout[i];
		}
	}
	fout.close();
	}
	else{
	vector<string> oout(mod_num,"");
	//string  oout[mod_num]  { "" };
	int p = 0;
	string fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top_ml.pl";
	ofstream fout(fileName_out.c_str());
	if (!fout.is_open()) {
		cout << "IO error\n";
		exit(EXIT_FAILURE);
	}
	fout << "UCLA pl 1.0 (by SEDALAB 2018)\n\n";
	oout[p]= "UCLA pl 1.0 (by SEDALAB 2018)\n\n";
	p++;
	for (int i = 0; i < mod_num; i++) {
		if (nt.mods[i].modL == 0) {
			if(nt.mods[i].modName[0]!='T'&&nt.mods[i].modName[1]!='S'&&nt.mods[i].modName[2]!='V'){
			fout << nt.mods[i].modName << "		" << nt.mods[i].GLeftX << "		" << nt.mods[i].GLeftY << "		:" << "	N\n";
			}
		}
		else {
			string a = nt.mods[i].modName;
			string in=  a+ "		" + to_string(nt.mods[i].GLeftX) + "		" + to_string(nt.mods[i].GLeftY) + "		:	N\n";
			oout[p] = in;
			p++;
		}
	}

	

	fout.close();
	fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot_ml.pl";
	fout.open(fileName_out.c_str());
	if (!fout.is_open()) {
		cout << "IO error\n";
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < mod_num; i++) {
		string temp = oout[i];
		if (temp[0] == '\0') {
			break;
		}
		else {
			fout << oout[i];
		}
	}
	fout.close();
	}
}
void PLOT::dump_nodes(NETLIST nt, string blockFile,int mod_num){
	vector<string> oout;
	for(int i=0;i<mod_num;i++){
		oout.push_back("");
	}
	int p = 0;
	string fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_top.nodes";
	ofstream fout(fileName_out.c_str());
	fout << "UCLA nodes 1.0 (by SEDALAB 2018)\n\n";
	

	int top_move=0;
	int top_fix=0;
	int bot_move=0;
	int bot_fix=0;
	vector<string> for_top;
	
	for (int i = 0; i < mod_num; i++) {
		if (nt.mods[i].modL == 0) {
			/*if(nt.mods[i].modName[0]!='T'&&nt.mods[i].modName[1]!='S'&&nt.mods[i].modName[2]!='V'){
			fout << nt.mods[i].modName << "		" << nt.mods[i].GLeftX << "		" << nt.mods[i].GLeftY << "		:" << "	N\n";
			}*/

			
			if(nt.mods[i].modName[0]!='T'&&nt.mods[i].modName[1]!='S'&&nt.mods[i].modName[2]!='V'){
				string n=nt.mods[i].modName;
				string aaaa= n+ "		" + to_string(nt.mods[i].modW) + "		" + to_string(nt.mods[i].modH);
				if(nt.mods[i].is_m==true){
					top_fix++;
					aaaa=aaaa+"	fixed\n";
				}
				else{
					//aaaa=aaaa+"move\n";
					top_move++;
					aaaa=aaaa+"\n";
				}
				for_top.push_back(aaaa);
			}
		}
		else {
			
			if(nt.mods[i].modName[0]!='T'&&nt.mods[i].modName[1]!='S'&&nt.mods[i].modName[2]!='V'){
				string n=nt.mods[i].modName;
				string aaaa=n + "		" + to_string(nt.mods[i].modW) + "		" + to_string(nt.mods[i].modH);
				if(nt.mods[i].is_m==true){
					bot_fix++;
					aaaa=aaaa+"	fixed\n";
				}
				else{
					bot_move++;
					aaaa=aaaa+"\n";
					//aaaa=aaaa+"move\n";
				}
				oout[p]=aaaa;
				p++;
			}
		}

	}
	
	fout<<"NumNodes	:	"<<top_move+top_fix<<"\n";
	fout<<"NumTerminals	:	"<<top_fix<<"\n\n";
	for(int i=0;i<for_top.size();i++){
		fout<<for_top[i];
	}
	fout.close();
	fileName_out = /*"temp/"+*/blockFile +"_"+to_string(InputOption.test_mode)+ "_bot.nodes";
	fout.open(fileName_out.c_str());
	if (!fout.is_open()) {
		cout << "IO error\n";
		exit(EXIT_FAILURE);
	}
	fout << "UCLA nodes 1.0 (by SEDALAB 2018)\n\n";
	fout<<"NumNodes	:	"<<bot_move+bot_fix<<"\n";
	fout<<"NumTerminals	:	"<<bot_fix<<"\n\n";
	for (int i = 0; i < mod_num; i++) {
		string temp = oout[i];
		if (temp[0] == '\0') {
			break;
		}
		else {
			fout << oout[i];
		}
	}
	fout.close();
}

void DUMP::ITRI::DumpFiles(NETLIST &tmpnt, string path )
{
	int nBinRow ;
	int nBinCol ;
	int binW ;
	int binH ;

	// Cut Bin
	double tsvArea = pow( TSV_PITCH , 2.0 );
	double AvgModLength = sqrt((double)(tmpnt.totalModArea - tmpnt.nTSV * tsvArea )/(double)(tmpnt.nMod - tmpnt.nTSV));
	int numBin = (int)((double)tmpnt.ChipHeight/(double)AMP_PARA/AvgModLength);


	nBinRow = nBinCol = numBin * InputOption.bin_para;


	binW = (int)((tmpnt.ChipWidth/((float)AMP_PARA)) / ((float)nBinCol) * AMP_PARA);
	binH = (int)((tmpnt.ChipHeight/((float)AMP_PARA)) / ((float)nBinRow) * AMP_PARA);

	cout<< BOLD(Cyan( "\\ ===== Dump ITRI Solver Files ===== //" ))<<endl;



	double **sbin;
	double **lbin;


	sbin = new double*[nBinRow];
	lbin = new double*[nBinRow];

	for(int i=0; i<nBinRow; i++)
	{
		sbin[i] = new double[nBinCol];
		lbin[i] = new double[nBinCol];
	}

	for(int i=0; i<nBinRow; i++)
	{
		for(int j=0; j<nBinCol; j++)
		{
			sbin[i][j] = 0;
			lbin[i][j] = 0;
		}
	}

	double power1=0;

	for(int i=0; i<tmpnt.nMod; i++)
	{

		for( int modL = tmpnt.mods[i].modL ; modL < tmpnt.mods[i].modL + tmpnt.mods[i].nLayer ; modL++ )
		{
			if( modL >= tmpnt.nLayer )
			{
				cerr << "error  : stack's layer out of range" << endl;
				exit(EXIT_FAILURE);
			}
			int colLeft;
			if(tmpnt.mods[i].LeftX/binW  <= 0) colLeft=0;
			else colLeft=tmpnt.mods[i].LeftX/binW ;

			int rowBottom;
			if(tmpnt.mods[i].LeftY/binH <=0) rowBottom=0;
			else rowBottom=tmpnt.mods[i].LeftY/binH ;

			int colRight;
			if( (tmpnt.mods[i].LeftX+tmpnt.mods[i].modW)/binW   >= nBinCol ) colRight=nBinCol - 1;
			else colRight=(tmpnt.mods[i].LeftX+tmpnt.mods[i].modW)/binW ;

			int rowTop;
			if( (tmpnt.mods[i].LeftY+tmpnt.mods[i].modH)/binH  >= nBinRow ) rowTop=nBinRow - 1;
			else rowTop=(tmpnt.mods[i].LeftY+tmpnt.mods[i].modH)/binH ;

			for(int j = rowBottom; j<= rowTop; j++)
			{
				for(int k = colLeft; k <= colRight; k++)
				{
					int use_width=0,use_height=0;
					int llx=binW*k;
					int lly=binH*j;

					if( (( llx <= tmpnt.mods[i].LeftX ) && ( tmpnt.mods[i].LeftX < llx + binW ))  || (k==colLeft) )
					{
						use_width = llx + binW - tmpnt.mods[i].LeftX ;
					}
					else if( (( llx <= tmpnt.mods[i].LeftX + tmpnt.mods[i].modW ) && (tmpnt.mods[i].LeftX + tmpnt.mods[i].modW < llx + binW))  || (k==colRight) )
					{
						use_width = tmpnt.mods[i].LeftX + tmpnt.mods[i].modW - llx ;
					}
					else
					{
						use_width=binW ;
						if(k==colLeft)  cout<<"error1 "<<llx<<"   "<<tmpnt.mods[i].LeftX<<"   "<<llx + binW<<endl;
						if(k==colRight) cout<<"error2 "<<llx<<"   "<<tmpnt.mods[i].LeftX + tmpnt.mods[i].modW<<"   "<<llx + binW<<endl;
					}

					if( ((lly <= tmpnt.mods[i].LeftY) && (tmpnt.mods[i].LeftY < lly + binH))  || (j==rowBottom) )
					{
						use_height = lly + binH - tmpnt.mods[i].LeftY ;
					}
					else if( ((lly <= tmpnt.mods[i].LeftY + tmpnt.mods[i].modH ) && (tmpnt.mods[i].LeftY + tmpnt.mods[i].modH < lly + binH))  || (j==rowTop) )
					{
						use_height = tmpnt.mods[i].LeftY + tmpnt.mods[i].modH - lly ;
					}
					else
					{
						use_height=binH ;
						if(j==rowBottom) cout<<"error1 "<<lly<<"   "<<tmpnt.mods[i].LeftY<<"   "<<lly + binH<<endl;
						if(j==rowTop) cout<<"error2 "<<lly<<"   "<<tmpnt.mods[i].LeftY + tmpnt.mods[i].modH<<"   "<<lly + binH<<endl;
					}

					double ttp =  ( ( (double) use_height/AMP_PARA) * ( (double) use_width/AMP_PARA) * tmpnt.mods[i].Pdensity );
					sbin[j][k] += ttp;

					//cout<<sbin[j][k]<<endl;
				}

			}
			//cout<<"5"<<endl;
		}
		power1 += tmpnt.mods[i].Power;
	}

	double all_power=0;
	double max_p = -1;
	pair <int,int> index;
	for(int i=0; i<nBinRow; i++)
	{
		for(int j=0; j<nBinCol; j++)
		{
			if(sbin[i][j] > max_p  )
			{
				index.first = i;
				index.second = j;
				max_p = sbin[i][j];
			}

			all_power += sbin[i][j] ;
			// int show_p =sbin[i][j] * 100000;
			// cout<<show_p <<"  ";
		}
		//cout<<endl;
	}
	//getchar();




	string fileName= "case_legal";
	int minDivide = 1 ;
	for(int i=2 ; i<10; i++)
	{
		if( nBinRow % i == 0 )
		{
			minDivide = i;
			break ;
		}
	}
	int shrink =  1 ;
	if(InputOption.mode == 6 ) //ibmcase need shrink
		shrink = InputOption.bin_para;
	cout<< " - nBinRow "<<nBinCol<<endl;
	cout<< " - Shrinking nBinRow : "<< nBinCol/shrink <<endl;
	int w_out=(binW/10)*10;
	int h_out=(binH/10)*10;




	fileName=path+fileName+".fp";



	cout << " - Dump File: "<<endl;
	cout << "  "<<fileName <<endl;
	ofstream fout_fp(fileName.c_str());

	if(!fout_fp)
	{
		cout << "Thermal grid can't open fileName : " << fileName << endl;
		exit(1);
	}
	fout_fp<<"size_unit: nm;"<<endl;
	fout_fp<<endl;
	fout_fp<<"design(topPop)"<<endl;
	fout_fp<<"{"<<endl;

	fout_fp<<"\tlayer_amount: 1;"<<endl;
	fout_fp<<endl;
	fout_fp<<"\tlayer(Layer_0)"<<endl;
	fout_fp<<"\t{"<<endl;
	fout_fp<<endl;
	fout_fp<<"\t\tlayer_number: 0;"<<endl;
	fout_fp<<"\t\ttechnology: t130nm;"<<endl;

	fout_fp<<"\t\tchip_length: "<<w_out*nBinCol<<";"<<endl;
	fout_fp<<"\t\tchip_width: "<<h_out*nBinRow<<";"<<endl;
	fout_fp<<"\t\tchip_thickness: 24990;"<<endl;

	double y_axis=0;
	int fp_count=0;


	for(int i=0; i<nBinRow/shrink; i++)
	{
		double x_axis=0;

		for(int j=0; j<nBinCol/shrink; j++)
		{

			if(j==0)
			{
				if(i==0)
				{
					fout_fp<<"\t\tblock(L0_NoC)"<<endl;
					fout_fp<<"\t\t{"<<endl;

					fout_fp<<"\t\tmaterial: chip;"<<endl;
					fout_fp<<"\t\tlength: "<< w_out*shrink<<";"<<endl;
					fout_fp<<"\t\twidth: "<<  h_out*shrink<<";"<<endl;
					fout_fp<<"\t\tx_axis: 0;"<<endl;
					fout_fp<<"\t\ty_axis: 0;"<<endl;

					fout_fp<<"\t\t}"<<endl;
				}
				else
				{
					x_axis=0;
					fout_fp<<"\t\tblock(L0_SYS_M3=c="<<fp_count<<")"<<endl;
					fout_fp<<"\t\t{"<<endl;
					fout_fp<<"\t\tmaterial: chip;"<<endl;
					fout_fp<<"\t\tlength: "<< w_out*shrink<<";"<<endl;
					fout_fp<<"\t\twidth: "<<  h_out*shrink<<";"<<endl;
					fout_fp<<"\t\tx_axis: "<<fixed<<setprecision(9)<<x_axis<<";"<<endl;
					fout_fp<<"\t\ty_axis: "<<fixed<<setprecision(9)<<y_axis<<";"<<endl;
					fout_fp<<"\t\t}"<<endl;

					fp_count++;
				}

			}
			else
			{
				x_axis=x_axis+w_out*shrink;

				fout_fp<<"\t\tblock(L0_SYS_M3=c="<<fp_count<<")"<<endl;
				fout_fp<<"\t\t{"<<endl;
				fout_fp<<"\t\tmaterial: chip;"<<endl;
				fout_fp<<"\t\tlength: "<< w_out*shrink<<";"<<endl;
				fout_fp<<"\t\twidth: "<<  h_out*shrink<<";"<<endl;
				fout_fp<<"\t\tx_axis: "<<fixed<<setprecision(9)<<x_axis<<";"<<endl;
				fout_fp<<"\t\ty_axis: "<<fixed<<setprecision(9)<<y_axis<<";"<<endl;
				fout_fp<<"\t\t}"<<endl;

				fp_count++;
			}

		}

		y_axis=y_axis+h_out*shrink;

	}
	fout_fp<<"\t}"<<endl;
	fout_fp<<"}"<<endl;
	fout_fp.close();


	fileName="case_legal";

	fileName=path+fileName+".pt";

	cout << "  "<< fileName <<endl;
	ofstream fout_pt(fileName.c_str());

	if(!fout_pt)
	{
		cout << "Thermal grid can't open fileName : " << fileName << endl;
		exit(1);
	}

	//fout_pt<<"power_unit: uW;"<<endl;
	fout_pt<<"power_unit: uW;"<<endl;
	fout_pt<<endl;
	fout_pt<<"application: gcc;"<<endl;
	fout_pt<<endl;
	fout_pt<<"layer(Layer_0)"<<endl;
	fout_pt<<"{"<<endl;

	int pt_count=0;


	for(int i=0; i<nBinRow/shrink; i++)
	{
		//cout<<i<<endl;
		for(int j=0; j<nBinCol/shrink; j++)
		{
			//cout<<"\t"<<j<<endl;
			double pp = 0 ;
			if(j==0)
			{
				if(i==0)
				{
					fout_pt<<"\t\tblock(L0_NoC)"<<endl;
					fout_pt<<"\t\t{"<<endl;
					for(int ii=0; ii<shrink; ii++)
					{
						for(int jj=0; jj<shrink; jj++)
						{
							pp += sbin[ shrink*i + ii ][ shrink*j + jj ];
						}
					}

					fout_pt<<"\t\t\tdynamic_power:"<<fixed<<setprecision(9)<<pp*AMP_PARA<<";"<<endl;///test
					//fout_pt<<"\t\t\tdynamic_power:"<<sbin[i][j]*1000<<";"<<endl;///test
					//fout_pt<<"\t\t\tdynamic_power:"<< sbin[2*i][2*j]*1000 + sbin[2*i+1][2*j]*1000 + sbin[2*i][2*j+1]*1000 + sbin[2*i+1][2*j+1]*1000 <<";"<<endl;///test
					//fout_pt<<"\t\t\tdynamic_power:"<<sbin[i][j]<<";"<<endl;
					fout_pt<<"\t\t\tleakage_power:0.00;"<<endl;
					fout_pt<<"\t\t}"<<endl;
				}
				else
				{
					fout_pt<<"\t\tblock(L0_SYS_M3=c=";
					fout_pt<<pt_count<<")"<<endl;
					fout_pt<<"\t\t{"<<endl;
					for(int ii=0; ii<shrink; ii++)
					{
						for(int jj=0; jj<shrink; jj++)
						{
							pp += sbin[ shrink*i + ii ][ shrink*j + jj ];
						}
					}

					fout_pt<<"\t\t\tdynamic_power:"<<fixed<<setprecision(9)<<pp*AMP_PARA<<";"<<endl;///test
					//fout_pt<<"\t\t\tdynamic_power:"<<sbin[i][j]*1000<<";"<<endl;///test
					//fout_pt<<"\t\t\tdynamic_power:"<< sbin[2*i][2*j]*1000 + sbin[2*i+1][2*j]*1000 + sbin[2*i][2*j+1]*1000 + sbin[2*i+1][2*j+1]*1000 <<";"<<endl;///test
					//fout_pt<<"\t\t\tdynamic_power:"<<sbin[i][j]<<";"<<endl;
					fout_pt<<"\t\t\tleakage_power:0.00;"<<endl;
					fout_pt<<"\t\t}"<<endl;

					pt_count++;
				}
			}
			else
			{
				fout_pt<<"\t\tblock(L0_SYS_M3=c=";
				fout_pt<<pt_count<<")"<<endl;
				fout_pt<<"\t\t{"<<endl;
				for(int ii=0; ii<shrink; ii++)
				{
					for(int jj=0; jj<shrink; jj++)
					{
						pp += sbin[ shrink*i + ii ][ shrink*j + jj ];
					}
				}


				fout_pt<<"\t\t\tdynamic_power:"<<fixed<<setprecision(9)<<pp*AMP_PARA<<";"<<endl;///test
				//fout_pt<<"\t\t\tdynamic_power:"<<sbin[i][j]*1000<<";"<<endl;///test
				//fout_pt<<"\t\t\tdynamic_power:"<< sbin[2*i][2*j]*1000 + sbin[2*i+1][2*j]*1000 + sbin[2*i][2*j+1]*1000 + sbin[2*i+1][2*j+1]*1000 <<";"<<endl;///test
				//fout_pt<<"\t\t\tdynamic_power:"<<sbin[i][j]<<";"<<endl;
				fout_pt<<"\t\t\tleakage_power:0.00;"<<endl;
				fout_pt<<"\t\t}"<<endl;

				pt_count++;
			}

		}

	}

	fout_pt<<"}"<<endl;
	fout_pt.close();


	//////////////
	for(int i=0; i<nBinRow; i++)
	{
		delete [] sbin[i];
		delete [] lbin[i];
	}

	delete [] sbin;
	delete [] lbin;

}

void IO::CAD2023::outFile(string benName, NETLIST &tmpnt){
	ofstream fout;
	fout.open(benName.c_str());
	
	int top_num=tmpnt.GetLayernMod(0);
	for(unsigned int i =0; i < tmpnt.mods.size(); i++)
	{
		if((tmpnt.mods[i].modL==0)&&(tmpnt.mods[i].flagTSV))
		{
			top_num--;
		}
	}
	fout.close();
	fout.open(benName.c_str());
	fout << "TopDiePlacement ";
	fout << top_num << endl;
	for(unsigned int i =0; i < tmpnt.mods.size(); i++)
	{
		if((tmpnt.mods[i].modL==0)&&(!tmpnt.mods[i].flagTSV))
		{
			string mod_name = tmpnt.mods[i].modName;
			fout <<"Inst " << mod_name <<" "<<(int)tmpnt.mods[i].GLeftX <<" "<< (int)tmpnt.mods[i].GLeftY<<" R0\n";
		}
	}
	
	fout << "BottomDiePlacement ";
	fout << tmpnt.GetLayernMod(1) << endl;
	for(unsigned int i =0; i < tmpnt.mods.size(); i++)
	{
		if((tmpnt.mods[i].modL==1)&&(!tmpnt.mods[i].flagTSV))
		{
			string mod_name = tmpnt.mods[i].modName;
			fout<<"Inst "<<mod_name<<" "<<(int)tmpnt.mods[i].GLeftX <<" "<< (int)tmpnt.mods[i].GLeftY<<" R0\n";
		}
	}
	
	fout<<"NumTerminals ";
	fout << tmpnt.nTSV << endl;
	for(unsigned int i=0; i<tmpnt.mods.size(); i++)
	{
		if(tmpnt.mods[i].flagTSV)
		{
			string net_name = string(tmpnt.nets[(tmpnt.mods[i].pNet[0])].netName);
			fout<<"Terminal "<<net_name<<" "<<(int)tmpnt.mods[i].CenterX <<" "<< (int)tmpnt.mods[i].CenterY <<endl;
		}	
	}
	fout.close();
}
void IO::CAD2023::outFile_01(string benName, NETLIST &tmpnt){
	ofstream fout;
	fout.open(benName.c_str());
	fout<<"TopDiePlacement 5\nInst C1 11 0 R0\nInst C2 7 10 R90\nInst C3 19 10 R0\nInst C7 19 0 R0\nInst C8 0 10 R0\nBottomDiePlacement 3\nInst C4 2 15 R0\nInst C5 2 0 R0\nInst C6 14 12 R90\n\nNumTerminals 1\nTerminal N4 29 15";
	fout.close();
}
/*!
 * \file 	gp.cpp
 * \brief 	global distribution
 *
 * \author 	NCKU SEDA LAB
 * \version	1.0 (3-D)
 * \date	2014.08.08
 */

#include "gp.h"





void GP::CreateExpTable()
{
    cout << "*Create Exponential Table" << endl;

    double value = 0.0;
    for( int i = 700000; i <= 1400000; i++ )
    {
        ExpTable[i] = exp(value) ; ///< ExpTable is Global Table
        value += 0.001;
    }
    value = -0.001;
    for( int i = 699999; i >= 0; i-- )
    {
        ExpTable[i] = exp(value);
        value -= 0.001;
    }
}

void GP::SetModToCenter( NETLIST& tmpnt )
{
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        tmpnt.mods[i].GCenterX = tmpnt.ChipWidth/2;
        tmpnt.mods[i].GCenterY = tmpnt.ChipHeight/2;
        tmpnt.mods[i].GLeftX = tmpnt.mods[i].GCenterX - tmpnt.mods[i].modW/2;
        tmpnt.mods[i].GLeftY = tmpnt.mods[i].GCenterY - tmpnt.mods[i].modH/2;
    }
}



void GP::CreateQmBv( vector<QP>& matrixQ, float* vectorB, NETLIST& tmpnt, int modeXY, map<int, int>& mapNet, map<string, int>& mapMod )
{
    int nNet = (int)mapNet.size();
    int nMod = (int)mapMod.size();

    for( int i = 0; i < nNet; i++ )
    {
        int netID = mapNet[i];								///< original net ID
        float pinWeight = (float)2/tmpnt.nets[netID].nPin;	///< weight of the net

        int starNodeID = nMod + i;	///< star pin ID
		QP::WEIGHT starNode;			///< star pin
        starNode.col = starNodeID;
        starNode.weight = pinWeight * (float)tmpnt.nets[netID].nPin * 0.5;	///< weight of star pin = weight of pin * nPin

		matrixQ[ starNodeID ].connect.push_back( starNode );

        for( int j = tmpnt.nets[netID].head; j < tmpnt.nets[netID].head + tmpnt.nets[netID].nPin; j++ )
        {


            if( tmpnt.pins[j].index >= tmpnt.nMod )	///< connect to pads : create vectorB
            {
                if( modeXY == 0 )
                    vectorB[i] += pinWeight * (float)tmpnt.pads[ tmpnt.pins[j].index-tmpnt.nMod ].x/1000/2;
                else
                    vectorB[i] += pinWeight * (float)tmpnt.pads[ tmpnt.pins[j].index-tmpnt.nMod ].y/1000/2;
            }
            else									///< connect ot modules : create matrixQ
            {
                int modID = mapMod[ tmpnt.mods[tmpnt.pins[j].index].modName ];
				QP::WEIGHT modNode;
                modNode.col = modID;
                modNode.weight = pinWeight * 0.5;


                bool flagHadAdd = false;
                for( unsigned int k = 0; k < matrixQ[modID].connect.size(); k++ )
                {
                    if( modID == matrixQ[ modID ].connect[k].col )
                    {
                        matrixQ[ modID ].connect[k].weight += pinWeight * 0.5;
                        flagHadAdd = true;
                        break;
                    }
                }
                if( !flagHadAdd )
                {
                    matrixQ[ modID ].connect.push_back( modNode );
                }
                starNode.weight = -pinWeight * 0.5;
                matrixQ[ modID ].connect.push_back( starNode );

                modNode.weight = -pinWeight * 0.5;
                matrixQ[ starNodeID ].connect.push_back( modNode );
            }
        }
    }
}

void GP::SolveQP( NETLIST& tmpnt )
{
    cout << " - Solve Quadratic Programming" << endl;
    int countNet = 0;		///< new number of nets
    map<int, int> mapNet;	///< map for new netID to original netID
    for( int i = 0; i < tmpnt.nNet; i++ )
    {
        if( tmpnt.nets[i].nPin == 0 )
        {
            cout << "net[" << i << "]" << endl;
        }
        else
        {
            mapNet[ countNet ] = i;
            countNet++;
        }
    }

    int countMod = 0;			///< new number of modules
    map<string, int> mapMod;	///< map for modName to new modID
    for( int i = 0; i < tmpnt.nMod; i++ )
    {
        if( tmpnt.mods[i].nNet == 0 )
        {
            cout << "module[" << i << "]" << endl;
        }
        else
        {
            mapMod[ tmpnt.mods[i].modName ] =  countMod;
            countMod++;
        }
    }

    int sizeQ = countMod + countNet;

    for( int modeXY = 0; modeXY <= 1; modeXY++ )	///< modeXY (0: create x matrix, 1: create y matrix)
    {
        vector<QP> matrixQ;
        matrixQ.resize( sizeQ );

        float *vectorB = new float [countNet];
        for( int i = 0; i < countNet; i++ )			///< set initial value for vectorB
        {
            vectorB[i] = 0;
        }

        this->CreateQmBv( matrixQ, vectorB, tmpnt, modeXY, mapNet, mapMod );

        QMatrix Qm;
        Vector Bv, Xv;
        Q_Constr( &Qm, "Qm", sizeQ, True, Rowws, Normal, True );

        for( int i = 0; i < sizeQ; i++ )
        {
            int entryNum = 0;
            for( unsigned int j = 0; j < matrixQ[i].connect.size(); j++ )
            {
                if( matrixQ[i].connect[j].col >= i ) entryNum++;
            }
            Q_SetLen( &Qm, i+1, entryNum );

            entryNum = 0;

            for( unsigned int j = 0; j < matrixQ[i].connect.size(); j++ )
            {
                if( matrixQ[i].connect[j].col >= i )
                {
                    Q_SetEntry( &Qm, i+1, entryNum, matrixQ[i].connect[j].col+1, matrixQ[i].connect[j].weight );
                    entryNum++;
                }
            }
        }

        V_Constr( &Xv, "Xv", sizeQ, Normal, True );
        V_SetAllCmp( &Xv, 0.0 );

        V_Constr( &Bv, "Bv", sizeQ, Normal, True );
        for( int i = 0; i < sizeQ; i++ )
        {
            if( i >= countMod )
                V_SetCmp( &Bv, i+1, vectorB[i-countMod] );
            else
                V_SetCmp( &Bv, i+1, 0.0 );
        }

        SetRTCAccuracy(1e-9);

        CGIter( &Qm, &Xv, &Bv, 100, SSORPrecond, 1.2 );

        for( int i = 0; i < tmpnt.nMod; i++ )
        {
            if( mapMod.find( tmpnt.mods[i].modName ) == mapMod.end() )
                continue;

            int modID = mapMod[ tmpnt.mods[i].modName ];
            if( modeXY == 0 )
            {
                tmpnt.mods[i].GCenterX = (int)(V_GetCmp( &Xv, modID+1) * AMP_PARA);
                tmpnt.mods[i].GLeftX = tmpnt.mods[i].GCenterX - tmpnt.mods[i].modW/2;
            }
            else
            {
                tmpnt.mods[i].GCenterY = (int)(V_GetCmp( &Xv, modID+1) * AMP_PARA);
                tmpnt.mods[i].GLeftY = tmpnt.mods[i].GCenterY - tmpnt.mods[i].modH/2;
            }
        }


        matrixQ.clear();
        vector<QP> (matrixQ).swap(matrixQ);
        delete [] vectorB;

        Q_Destr( &Qm );
        V_Destr( &Bv );
        V_Destr( &Xv );
    }
	this->CalculateHPWL_GP( tmpnt );

    cout << " - HPWL after QP: " << tmpnt.totalWL_GP << endl;
}


void GP::CalculateHPWL_GP(NETLIST & tmpnt)
{
    int maxX = 0;
    int minX = tmpnt.ChipWidth;
    int maxY = 0;
    int minY = tmpnt.ChipHeight;
    int centerX;
    int centerY;

	tmpnt.totalWL_GP = 0.0;

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
                centerX = tmpnt.mods[tmpnt.pins[j].index].GCenterX;
                centerY = tmpnt.mods[tmpnt.pins[j].index].GCenterY;
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
        tmpnt.totalWL_GP = tmpnt.totalWL_GP + (double)(((maxX - minX) + (maxY - minY))/AMP_PARA);
    }

}


/*
pair<double, double> GDistribution(vector <double> data)
{

    double mean=0;
    for(unsigned int i=0; i < data.size() ; i++)
    {

        mean += data[i] ;
    }
    mean = (double) mean /data.size()  ;
    double standard = 0;
    double sigma =0;
    double dev=0;
    for(int i=0 ; i<data.size()  ; i++)
    {
        standard = data[i] - mean;
        standard *= standard;
        sigma += standard;
    }
    sigma = (double) sigma /data.size() ;

    dev = sqrt(sigma);
    pair <double,double > temp;
    temp.first = mean;
    temp.second = dev;


    return temp;
}
*/

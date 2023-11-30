
/*!
 * \file    blockPack.cpp
 * \brief   block packing
 *
 * \author  NCKU SEDA LAB
 * \version 1.1
 * \date    2014.11.20
 */

#include "EP.h"

bool  compare_leftX( const MODULE* a, const MODULE* b )
{
	int leftA = a->LeftX;
	int leftB = b->LeftX;

	return (leftA < leftB);
}

bool  compare_rightX( const MODULE* a, const MODULE* b )
{
	int rightA = a->LeftX + a->modW;
	int rightB = b->LeftX + b->modW;

	return (rightA > rightB);
}

bool  compare_bottomY( const MODULE* a, const MODULE* b )
{
	int bottomA = a->LeftY;
	int bottomB = b->LeftY;

	return (bottomA < bottomB);
}

bool  compare_topY( const MODULE* a, const MODULE* b )
{
	int topA = a->LeftY + a->modH;
	int topB = b->LeftY + b->modH;

	return (topA > topB);
}


bool  compare_ascending( const int &a, const int &b )
{
	return (a < b);
}



void EPLG::PackToCenter( int &left, int &right, int &bottom, int &top, int xCenter, int yCenter, NETLIST &tmpnt )
{
	//int maxBoundX = 0;
	//int maxBoundY = 0;
	//int minBoundX = MAX_INT;
	//int minBOundY = MAX_INT;

	vector<MODULE*> lbArray;
	vector<MODULE*> rtArray;
	vector<int> coordArray;

	int newLeftBound;
	int newRightBound;
	int newBottomBound;
	int newTopBound;

	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		if( tmpnt.mods[i].LeftX < xCenter )
		{
			lbArray.push_back( &tmpnt.mods[i] );
		}
		if( tmpnt.mods[i].LeftX + tmpnt.mods[i].modW > xCenter )
		{
			rtArray.push_back( &tmpnt.mods[i] );
		}
	}

	PackToRight( lbArray, xCenter, newLeftBound );
	//GreedyMoveToLeft( lbArray, coordArray, newLeftBound, tmpnt );

	PackToLeft( rtArray, xCenter, newRightBound );
	//GreedyMoveToRight( rtArray, coordArray, newRightBound, tmpnt );

	lbArray.clear();
	rtArray.clear();
	coordArray.clear();

	for( int i = 0; i < tmpnt.nMod; i++ )
	{
		if( tmpnt.mods[i].LeftY < yCenter )
		{
			lbArray.push_back( &tmpnt.mods[i] );
		}
		if( tmpnt.mods[i].LeftY + tmpnt.mods[i].modH > yCenter )
		{
			rtArray.push_back( &tmpnt.mods[i] );
		}
	}

	PackToTop( lbArray, yCenter, newBottomBound );
	//GreedyMoveToBottom( lbArray, coordArray, newBottomBound, tmpnt );

	PackToBottom( rtArray, yCenter, newTopBound );
	//GreedyMoveToTop( rtArray, coordArray, newTopBound, tmpnt );

	top = newTopBound;
	bottom = newBottomBound;
	left = newLeftBound;
	right = newRightBound;

	lbArray.clear();
	rtArray.clear();
	coordArray.clear();

	//getchar();
}

void EPLG:: PackToRight( vector<MODULE*> &array, int bound, int &newLeftBound )
{
	int minValue;
	int leftBound;

	sort( array.begin(), array.end(), compare_rightX );

	for( int i = 0; i < (int)array.size(); i++ )
	{
		if( array[i]->LeftX + array[i]->modW > bound )
		{
			continue;
		}

		minValue = bound;

		for( int j = 0; j < i; j++ )
		{
			if( CheckOverlapX(array[i], array[j]) )
			{
				leftBound = array[j]->LeftX;

				if( leftBound < minValue )
				{
					minValue = leftBound;
				}
			}
		}

		array[i]->LeftX = minValue - array[i]->modW;
		array[i]->CenterX = array[i]->LeftX + array[i]->modW/2;

		leftBound = array[i]->LeftX;
		if( leftBound < newLeftBound )
		{
			newLeftBound = leftBound;
		}
	}
}

void EPLG:: PackToLeft( vector<MODULE*> &array, int bound, int &newRightBound )
{
	int maxValue;
	int rightBound;

	newRightBound = bound;

	sort( array.begin(), array.end(), compare_leftX );

	for( int i = 0; i < (int)array.size(); i++ )
	{
		if( array[i]->LeftX < bound )
		{
			continue;
		}

		maxValue = bound;
		for( int j = 0; j < i; j++ )
		{
			if( CheckOverlapX(array[i], array[j]) )
			{
				rightBound = array[j]->LeftX + array[j]->modW;
				if( rightBound > maxValue )
				{
					maxValue = rightBound;
				}
			}
		}

		array[i]->LeftX = maxValue;
		array[i]->CenterX = array[i]->LeftX + array[i]->modW/2;

		rightBound = array[i]->LeftX + array[i]->modW;
		if( rightBound > newRightBound )
		{
			newRightBound = rightBound;
		}
	}
}

void EPLG:: PackToTop( vector<MODULE*> &array, int bound, int &newBottomBound )
{
	int minValue;
	int bottomBound;

	newBottomBound = bound;

	sort( array.begin(), array.end(), compare_topY );

	for( int i = 0; i < (int)array.size(); i++ )
	{
		if( array[i]->LeftY + array[i]->modH > bound )
		{
			continue;
		}

		minValue = bound;
		for( int j = 0; j < i; j++ )
		{
			if( CheckOverlapY(array[i], array[j]) )
			{
				bottomBound = array[j]->LeftY;

				if( bottomBound < minValue )
				{
					minValue = bottomBound;
				}
			}
		}

		array[i]->LeftY = minValue - array[i]->modH;
		array[i]->CenterY = array[i]->LeftY + array[i]->modH/2;

		bottomBound = array[i]->LeftY;
		if( bottomBound < newBottomBound )
		{
			newBottomBound = bottomBound;
		}
	}
}

void EPLG:: PackToBottom( vector<MODULE*> &array, int bound, int &newTopBound )
{
	int maxValue;
	int topBound;

	newTopBound = bound;

	sort( array.begin(), array.end(), compare_bottomY );

	for( int i = 0; i < (int)array.size(); i++ )
	{
		if( array[i]->LeftY < bound )
		{
			continue;
		}

		maxValue = bound;
		for( int j = 0; j < i; j++ )
		{
			if( CheckOverlapY(array[i], array[j]) )
			{
				topBound = array[j]->LeftY + array[j]->modH;

				if( topBound > maxValue )
				{
					maxValue = topBound;
				}
			}
		}

		array[i]->LeftY = maxValue;
		array[i]->CenterY = array[i]->LeftY + array[i]->modH/2;

		topBound = array[i]->LeftY + array[i]->modH;
		if( topBound > newTopBound )
		{
			newTopBound = topBound;
		}
	}
}

void EPLG:: GreedyMoveToLeft( vector<MODULE*> &modArray, vector<int> &coordArray, int &newLeftBound, NETLIST &tmpnt )
{
	int maxValue;
	int rightBound;

	for( int i = (int)modArray.size()-1; i >= 0; i-- )
	{
		int indexN = 0;
		coordArray.resize( modArray[i]->nNet * 2 );

		for( int j = 0; j < modArray[i]->nNet; j++ )
		{
			GetNetBoxX( modArray[i]->pNet[j], modArray[i]->idMod, coordArray[indexN],
			            coordArray[indexN+1], tmpnt );
			indexN += 2;
		}

		sort( coordArray.begin(), coordArray.end(), compare_ascending );

		if( modArray[i]->CenterX > coordArray[indexN/2-1] )
		{
			maxValue = coordArray[indexN/2-1] - modArray[i]->modW/2;

			if( maxValue < 0 )
			{
				maxValue = 0;
			}

			for( int j = i+1; j < (int)modArray.size(); j++ )
			{
				if( CheckOverlapX( modArray[i], modArray[j] ) )
				{
					rightBound = modArray[j]->LeftX + modArray[j]->modW;

					if( rightBound > maxValue )
					{
						maxValue = rightBound;
					}
				}
			}

			modArray[i]->LeftX = maxValue;
			modArray[i]->CenterX = modArray[i]->LeftX + modArray[i]->modW/2;
		}

		if( modArray[i]->LeftX < newLeftBound )
		{
			newLeftBound = modArray[i]->LeftX;
		}
	}
}

void EPLG:: GreedyMoveToRight( vector<MODULE*> &modArray, vector<int> &coordArray, int &newRightBound, NETLIST &tmpnt )
{
	int minValue;
	int leftBound;

	for( int i = (int)modArray.size()-1; i >= 0; i-- )
	{
		int indexN = 0;
		coordArray.resize( modArray[i]->nNet * 2 );

		for( int j = 0; j < modArray[i]->nNet; j++ )
		{
			GetNetBoxX( modArray[i]->pNet[j], modArray[i]->idMod, coordArray[indexN],
			            coordArray[indexN+1], tmpnt );
			indexN += 2;
		}

		sort( coordArray.begin(), coordArray.end(), compare_ascending );

		if( modArray[i]->CenterX < coordArray[indexN/2] )
		{
			minValue = coordArray[indexN/2] + modArray[i]->modW/2;
			if( minValue > tmpnt.ChipWidth )
			{
				minValue = tmpnt.ChipWidth;
			}
			for( int j = i+1; j < (int)modArray.size(); j++ )
			{
				if( CheckOverlapX(modArray[i], modArray[j]) )
				{
					leftBound = modArray[j]->LeftX;

					if( leftBound < minValue )
					{
						minValue = leftBound;
					}
				}
			}

			modArray[i]->LeftX = minValue - modArray[i]->modW;
			modArray[i]->CenterX = modArray[i]->LeftX + modArray[i]->modW/2;
		}

		if( modArray[i]->LeftX + modArray[i]->modW > newRightBound )
		{
			newRightBound = modArray[i]->LeftX + modArray[i]->modW;
		}
	}
}

void EPLG:: GreedyMoveToBottom( vector<MODULE*> &modArray, vector<int> &coordArray, int &newBottomBound, NETLIST &tmpnt )
{
	int maxValue;
	int topBound;

	for( int i = (int)modArray.size()-1; i >= 0; i-- )
	{
		int indexN = 0;
		coordArray.resize( modArray[i]->nNet * 2 );

		for( int j = 0; j < modArray[i]->nNet; j++ )
		{
			GetNetBoxY( modArray[i]->pNet[j], modArray[i]->idMod, coordArray[indexN],
			            coordArray[indexN+1], tmpnt );
			indexN += 2;
		}

		sort( coordArray.begin(), coordArray.end(), compare_ascending );

		if( modArray[i]->CenterY > coordArray[indexN/2-1] )
		{
			maxValue = coordArray[indexN/2-1] - modArray[i]->modH/2;
			if( maxValue < 0 )
			{
				maxValue = 0;
			}
			for( int j = i+1; j < (int)modArray.size(); j++)
			{
				if( CheckOverlapY(modArray[i], modArray[j]) )
				{
					topBound = modArray[j]->LeftY + modArray[j]->modH;
					if( topBound > maxValue )
					{
						maxValue = topBound;
					}
				}
			}

			modArray[i]->LeftY = maxValue;
			modArray[i]->CenterY = modArray[i]->LeftY + modArray[i]->modH/2;
		}

		if( modArray[i]->LeftY < newBottomBound )
		{
			newBottomBound = modArray[i]->LeftY;
		}
	}
}

void EPLG:: GreedyMoveToTop( vector<MODULE*> &modArray, vector<int> &coordArray, int &newTopBound, NETLIST &tmpnt)
{
	int minValue;
	int bottomBound;

	for( int i = (int)modArray.size()-1; i >= 0; i-- )
	{
		int indexN = 0;
		coordArray.resize( modArray[i]->nNet * 2 );

		for( int j = 0; j < modArray[i]->nNet; j++ )
		{
			GetNetBoxY( modArray[i]->pNet[j], modArray[i]->idMod, coordArray[indexN],
			            coordArray[indexN+1], tmpnt );
			indexN += 2;
		}

		sort( coordArray.begin(), coordArray.end(), compare_ascending );

		if( modArray[i]->CenterY < coordArray[indexN/2] )
		{
			minValue = coordArray[indexN/2] + modArray[i]->modH/2;
			if( minValue > tmpnt.ChipHeight )
			{
				minValue = tmpnt.ChipHeight;
			}

			for( int j = i+1; j < (int)modArray.size(); j++ )
			{
				if( CheckOverlapY(modArray[i], modArray[j]) )
				{
					bottomBound = modArray[j]->LeftY;

					if( bottomBound < minValue )
					{
						minValue = bottomBound;
					}
				}
			}

			modArray[i]->LeftY = minValue - modArray[i]->modH;
			modArray[i]->CenterY = modArray[i]->LeftY + modArray[i]->modH/2;
		}

		if( modArray[i]->LeftY + modArray[i]->modH > newTopBound )
		{
			newTopBound = modArray[i]->LeftY + modArray[i]->modH;
		}
	}
}

void EPLG:: PackToLeftSub( vector<MODULE*> &modArray, int bound, int &newRightBound )
{
	int maxValue;
	int rightBound;

	sort( modArray.begin(), modArray.end(), compare_leftX );

	for( int i = 0; i < (int)modArray.size(); i++ )
	{
		maxValue = bound;
		for( int j = 0; j < i; j++ )
		{
			if( CheckOverlapX(modArray[i], modArray[j]) )
			{
				rightBound = modArray[j]->LeftX + modArray[j]->modW;

				if( rightBound > maxValue )
				{
					maxValue = rightBound;
				}
			}
		}

		modArray[i]->LeftX = maxValue;
		modArray[i]->CenterX = modArray[i]->LeftX + modArray[i]->modW/2;

		rightBound = modArray[i]->LeftX + modArray[i]->modW;
		if( rightBound > newRightBound )
		{
			newRightBound = rightBound;
		}
	}
}

void EPLG:: PackToBottomSub( vector<MODULE*> &modArray, int bound, int &newTopBound )
{
	int maxValue;
	int topBound;

	newTopBound = bound;

	sort( modArray.begin(), modArray.end(), compare_bottomY );

	for( int i = 0; i < (int)modArray.size(); i++ )
	{
		maxValue = bound;
		for( int j = 0; j < i; j++ )
		{
			if( CheckOverlapY(modArray[i], modArray[j]) )
			{
				topBound = modArray[j]->LeftY + modArray[j]->modH;

				if( topBound > maxValue )
				{
					maxValue = topBound;
				}
			}
		}

		modArray[i]->LeftY = maxValue;
		modArray[i]->CenterY = modArray[i]->LeftY + modArray[i]->modH/2;

		topBound = modArray[i]->LeftY + modArray[i]->modH;
		if( topBound > newTopBound )
		{
			newTopBound = topBound;
		}
	}
}

void EPLG:: GetNetBoxX( int i, int index, int &left, int &right, NETLIST &tmpnt )
{
	int maxX = 0;
	int minX = MAX_INT;

	for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
	{
		int centerX = 0;

		if( tmpnt.pins[j].index != index )
		{
			if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
			{
				centerX = tmpnt.mods[ tmpnt.mod_NameToID[tmpnt.pins[j].pinName] ].CenterX;
			}
			else if( tmpnt.pad_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.pad_NameToID.end() )
			{
				centerX = tmpnt.pads[ tmpnt.pad_NameToID[tmpnt.pins[j].pinName] ].x;
			}
			else
			{
				cout << "error:  unrecognized pin " << tmpnt.pins[j].pinName << endl;
				exit(EXIT_FAILURE);
			}

			maxX = COM_MAX( centerX, maxX );
			minX = COM_MIN( centerX, minX );
		}
	}

	left = minX;
	right = maxX;
}

void EPLG:: GetNetBoxY( int i, int index, int &bottom, int &top, NETLIST &tmpnt )
{
	int maxY = 0;
	int minY = MAX_INT;

	for( int j = tmpnt.nets[i].head; j < tmpnt.nets[i].head + tmpnt.nets[i].nPin; j++ )
	{
		int centerY = 0;

		if( tmpnt.pins[j].index != index )
		{
			if( tmpnt.mod_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.mod_NameToID.end() )
			{
				centerY = tmpnt.mods[ tmpnt.mod_NameToID[tmpnt.pins[j].pinName] ].CenterY;
			}
			else if( tmpnt.pad_NameToID.find( tmpnt.pins[j].pinName ) != tmpnt.pad_NameToID.end() )
			{
				centerY = tmpnt.pads[ tmpnt.pad_NameToID[tmpnt.pins[j].pinName] ].y;
			}
			else
			{
				cout << "error:  unrecognized pin " << tmpnt.pins[j].pinName << endl;
				exit(EXIT_FAILURE);
			}

			maxY = COM_MAX( centerY, maxY );
			minY = COM_MIN( centerY, minY );
		}
	}

	bottom = minY;
	top = maxY;
}

bool EPLG:: CheckOverlapX( MODULE* a, MODULE* b )
{
	int topA = a->LeftY + a->modH;
	int bottomA = a->LeftY;

	int topB = b->LeftY + b->modH;
	int bottomB = b->LeftY;

	if( (topA <= topB) && (topA > bottomB) )
	{
		return true;
	}
	else if( (bottomA < topB) && (bottomA >= bottomB) )
	{
		return true;
	}
	else if( (topB <= topA) && (bottomB >= bottomA) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool EPLG:: CheckOverlapY( MODULE* a, MODULE* b )
{
	int rightA = a->LeftX + a->modW;
	int leftA = a->LeftX;

	int rightB = b->LeftX + b->modW;
	int leftB = b->LeftX;

	if( (rightA <= rightB) && (rightA > leftB) )
	{
		return true;
	}
	else if( (leftA < rightB) && (leftA >= leftB) )
	{
		return true;
	}
	else if( (rightB <= rightA) && (leftB >= leftA) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


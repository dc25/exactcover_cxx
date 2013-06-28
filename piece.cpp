#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "utilities/ARRAYSIZE.h"
#include "BoolPic.h"
#include "Piece.h"

using namespace std;
using namespace boost;

Cutout::Cutout(const BoolPic& a)
{
    size_t numRow = MAXSPAN;
    size_t numCol = MAXSPAN;

    intrusive_ptr<Unit> firstUnit = NULL;
    unsigned int firstRow, firstCol;

    for (size_t row = 0; row < numRow; ++row)
    {
        for (size_t col = 0; col < numCol; ++col)
        {
            if (a[row][col])
            {
                intrusive_ptr<Unit> thisUnit= new Unit();
                m_units.push_back(thisUnit);
                if (firstUnit == NULL)
                {
                    thisUnit->m_rowOffset = 0;
                    thisUnit->m_colOffset = 0;
                    firstUnit = thisUnit;
                    firstRow = row;
                    firstCol = col;
                } else
                {
                    thisUnit->m_rowOffset = row-firstRow;
                    thisUnit->m_colOffset = col-firstCol;
                }
            }
        }
    }
}



bool isUsed(const BoolPic a)
{
    size_t numRow = MAXSPAN;
    size_t numCol = MAXSPAN;

	for (size_t row = 0; row < numRow; ++row)
    {
        for (size_t col = 0; col < numCol; ++col)
        {
            if (a[row][col])
			{
				return true;
			}
        }
    }
	return false;
}

Piece::Piece(const BoolPicSet& a, int index)
	: inUse(false)
{
	stringstream ssId;
	ssId << index;
	m_id = ssId.str();

    for (size_t pic = 0; pic < MAX_SYMMETRY; ++pic)
    {
		if(isUsed(a[pic]))
        {
            intrusive_ptr<Cutout> oneCutout= new Cutout(a[pic]);
            m_cutouts.push_back(oneCutout);
        }
    }
}



PieceSet::PieceSet (const Pentomino a[], unsigned int pieceCount)
{
    for (size_t pic = 0; pic < pieceCount; ++pic)
    {
		m_pieces.push_back(new Piece(a[pic].m_rep, pic));
    }
}


const boost::intrusive_ptr<Piece>& PieceSet::operator[](const int i) const
{
	return m_pieces[i];
}

const size_t PieceSet::size()
{
	return m_pieces.size();
}

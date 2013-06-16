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

Cutout::Cutout(const BoolPic a)
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
                intrusive_ptr<Unit> thisUnit= new Unit;
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



static int getId(const BoolPic a)
{
    size_t numRow = MAXSPAN;
    size_t numCol = MAXSPAN;

	for (size_t row = 0; row < numRow; ++row)
    {
        for (size_t col = 0; col < numCol; ++col)
        {
			int id;
            if (id = a[row][col])
			{
				return id;
			}
        }
    }
	return 0;
}

Piece::Piece(int id)
	: inUse(false)
{
	stringstream ssId;
	ssId << id;
	m_id = ssId.str();
}


void Piece::addCutout(const intrusive_ptr<Cutout>& oneCutout)
{
	m_cutouts.push_back(oneCutout);
}

PieceSet::PieceSet (const BoolPic a[], size_t numPics)
{
    for (size_t pic = 0; pic < numPics; ++pic)
    {
        intrusive_ptr<Cutout> oneCutout= new Cutout(a[pic]);
		int id = getId(a[pic]);
		m_pieces.resize(id);
		if (m_pieces[id-1] == NULL)
		{
			m_pieces[id-1] = new Piece(id);
		}
        m_pieces[id-1]->addCutout(oneCutout);
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

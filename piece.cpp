#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "utilities/ARRAYSIZE.h"
#include "BoolPic.h"
#include "Piece.h"

using namespace std;
using namespace boost;

Cutout::Cutout(BoolPic a)
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

Piece::Piece(BoolPic a[], size_t numPics, const char* id)
    : inUse(false), m_id(id)
{
    for (size_t pic = 0; pic < numPics; ++pic)
    {
        intrusive_ptr<Cutout> oneCutout= new Cutout(a[pic]);
        m_cutouts.push_back(oneCutout);
    }
}

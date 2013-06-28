#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "utilities/ARRAYSIZE.h"
#include "BoolPic.h"
#include "Piece.h"
#include "pentominos.h"
#include "Coverage.h"

using namespace std;
using namespace boost;

class Piece;
class PieceSet;

class Puzzle
{
    public:
        Puzzle();
		void initialize(const BoolPicSet a[], unsigned int pieceCount, unsigned int rowCount, unsigned int colCount);
        void solve();
        void display();
    private:
        intrusive_ptr<Piece> m_puzzleUnits[10][6];
		intrusive_ptr<PieceSet> m_pieces;
		intrusive_ptr<DancingLinks> m_coverage;

};

void Puzzle::initialize(const BoolPicSet a[], unsigned int pieceCount, unsigned int rowCount, unsigned int colCount)
{
    for (size_t p = 0; p < pieceCount; ++p)
    {
		for (size_t orientation = 0; orientation < NELEM(a[0]); ++orientation)
		{
            const BoolPic& piecePic = a[p][orientation];
			if(isUsed(piecePic))
			{
				for (unsigned int rowVal = 0; rowVal < rowCount; ++rowVal)
				{
					for (unsigned int colVal = 0; colVal < colCount; ++colVal)
					{
                        std::vector<int> rowUsage;
                        rowUsage.resize(pieceCount + rowCount * colCount); // 
                        rowUsage[p] = true;

                        auto skipRow = false;  // set to true if this piece goes out of bounds
                        for (int i = 0; i<NELEM(piecePic) && !skipRow; ++i)
                        {
                            for (int j = 0; j < NELEM(piecePic[0]) && !skipRow; ++j)
                            {
                                bool u = piecePic[i][j];
                                if (u)
                                {
                                    unsigned int row = rowVal + i;
                                    unsigned int col = colVal + j;
                                    if (row >= rowCount || col >= colCount) // bad placement
                                    {
                                        skipRow = true;
                                    } else
                                    {
                                        rowUsage[pieceCount + (col * rowCount) + row] = true;
                                    }

                                }
                            }
                        }
                        if (!skipRow)
                        {
                            m_coverage->addRow(rowUsage);
                        }
					}
				}
			}
		}
    }
}

Puzzle::Puzzle()
{

    m_pieces = new PieceSet(pentominos, NELEM(pentominos));
	m_coverage = new DancingLinks(NELEM(pentominos), 10, 6);
    initialize(pentominos, NELEM(pentominos), 10, 6);
	m_coverage->solve();

}



void Puzzle::solve()
{
    static size_t numPuzzleRow = NELEM(m_puzzleUnits);
    static size_t numPuzzleCol = NELEM(m_puzzleUnits[0]);
    static size_t pieceCount = (*m_pieces).size();

    bool emptyFound = false;
    size_t row, col;
    // find the first empty row/col on the board.
    for (size_t testRow = 0; !emptyFound && (testRow < numPuzzleRow); ++testRow)
    {
        for (size_t testCol = 0; !emptyFound && (testCol < numPuzzleCol); ++testCol)
        {
            if (m_puzzleUnits[testRow][testCol] == NULL)
            {
                row=testRow;
                col=testCol;
                emptyFound = true;
            }
        }
    }

    if (!emptyFound)
    {
        display();
        return;
    }

    // Try to place each unused piece on that location.

    for (unsigned int pieceIndex = 0; pieceIndex < pieceCount; ++pieceIndex)
    {
        auto aPiece = (*m_pieces)[pieceIndex];
        if (!aPiece->inUse)
        {
            // iterate over each cutout in the piece
            auto numCutouts = aPiece->m_cutouts.size();

            for (unsigned int cutoutIndex = 0; cutoutIndex < numCutouts; ++cutoutIndex)
            {
                auto aCutout = aPiece->m_cutouts[cutoutIndex];

                // iterate over each cutout in the piece
                auto numUnits = aCutout->m_units.size();

                for (unsigned int unitIndex = 0; unitIndex < numUnits; ++unitIndex)
                {
                    auto u0 = aCutout->m_units[unitIndex];

                    unsigned int numUnitsPlaced;
                    for (numUnitsPlaced = 0; numUnitsPlaced < numUnits; ++numUnitsPlaced)
                    {
                        auto u1 = aCutout->m_units[numUnitsPlaced];

                        int unitRow = row - u0->m_rowOffset + u1->m_rowOffset;
                        int unitCol = col - u0->m_colOffset + u1->m_colOffset;


                        if (   ( unitRow < 0 )
                            || ( unitRow >= (int)numPuzzleRow )
                            || ( unitCol < 0 )
                            || ( unitCol >= (int)numPuzzleCol )
                            || ( m_puzzleUnits[unitRow][unitCol] != NULL ) )
                        {
                            break;  
                        }

                        m_puzzleUnits[unitRow][unitCol] = aPiece ;
                    }

                    if (numUnitsPlaced == numUnits)
                    {
                        aPiece->inUse = true;
                        solve();  // solve puzzle with this piece in place
                        aPiece->inUse = false;
                    }


                    // remove the placed units of this piece
                    for ( unsigned int i = 0; i < numUnitsPlaced; ++i)
                    {
                        auto u1 = aCutout->m_units[i];

                        int unitRow = row - u0->m_rowOffset + u1->m_rowOffset;
                        int unitCol = col - u0->m_colOffset + u1->m_colOffset;

                        m_puzzleUnits[unitRow][unitCol] = NULL ;
                        
                    }

                }


            }
        }
    }
}

void Puzzle::display()
{
    static size_t numPuzzleRow = NELEM(m_puzzleUnits);
    static size_t numPuzzleCol = NELEM(m_puzzleUnits[0]);

    // find the first empty row/col on the board.
    for (size_t row = 0; row < numPuzzleRow; ++row)
    {
        for (size_t col = 0; col < numPuzzleCol; ++col)
        {
            string symbol;
            if (m_puzzleUnits[row][col] == NULL)
            {
                symbol = "* ";
            } else
            {
                symbol = m_puzzleUnits[row][col]->m_id;
            }
            cout << symbol;
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main ()
{
    Puzzle puzzle;
    // puzzle.solve();
}


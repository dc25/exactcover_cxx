#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
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

        void initialize(
                 const Pentomino a[], 
                 unsigned int pieceCount, 
                 unsigned int rowCount, 
                 unsigned int colCount, 
                 vector< vector < int > >& usage,
                 vector< string >& columns);

        void display();
    private:
        intrusive_ptr<Piece> m_puzzleUnits[10][6];
		intrusive_ptr<PieceSet> m_pieces;

};

void Puzzle::initialize(const Pentomino a[], unsigned int pieceCount, unsigned int rowCount, unsigned int colCount, vector< vector < int > >& usage, vector< string >& columns)
{
    for (size_t p = 0; p < pieceCount; ++p)
    {
		columns.push_back(string(a[p].m_name));
	}
	for (unsigned int rowVal = 0; rowVal < rowCount; ++rowVal)
	{
		for (unsigned int colVal = 0; colVal < colCount; ++colVal)
		{
			stringstream str;
			str << rowVal << "," << colVal;
			columns.push_back(str.str());
		}
	}
    for (size_t p = 0; p < pieceCount; ++p)
    {
		for (size_t orientation = 0; orientation < NELEM(a[0].m_rep); ++orientation)
		{
			const BoolPic& piecePic = a[p].m_rep[orientation];
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
                            usage.push_back(rowUsage);
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

    vector< vector< int > > usage;
    vector< string > columns;
    initialize(pentominos, NELEM(pentominos), 10, 6, usage, columns);
	intrusive_ptr<DancingLinks> coverage = new DancingLinks(NELEM(pentominos), 10, 6, usage, columns);
	coverage->solve();
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
}


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

class Piece;

class Puzzle
{
    public:
        Puzzle();
        void solve();
        void display();
    private:
        intrusive_ptr<Piece> m_puzzleUnits[10][6];
        vector<intrusive_ptr<Piece> > m_pieces;
};

Puzzle::Puzzle()
{

/* ----------------------------------- */
    BoolPic p0[] = 
    { 
        { { 1,1,1,1,1 } },

        { { 1 },
          { 1 },
          { 1 },
          { 1 },
          { 1 } },
    };

/* ----------------------------------- */
    BoolPic p1[] = 
    { 
        { 
          { 1,1,1,1 },
          { 1 },
        },

        { 
          { 1 },
          { 1 },
          { 1 },
          { 1,1 },
        },
    };

/* ----------------------------------- */


    BoolPic p2[] = 
    { 
        { 
          { 1,1,1,1 },
          { 0,1 },
        },

        { 
          { 1,1,1,1 },
          { 0,0,1},
        },

        { 
          { 0,1 },
          { 1,1,1,1 },
        },

        { 
          { 0,0,1},
          { 1,1,1,1 },
        },

        { 
          { 1 },
          { 1 },
          { 1,1 },
          { 1 },
        },

        { 
          { 1 },
          { 1,1 },
          { 1 },
          { 1 },
        },

        { 
          { 0,1 },
          { 0,1 },
          { 1,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 1,1 },
          { 0,1 },
          { 0,1 },
        },
    };

/* ----------------------------------- */
    BoolPic p3[] = 
    { 
        { 
          { 1,1,1 },
          { 1,1 },
        },

        { 
          { 1,1,1 },
          { 0,1,1},
        },

        { 
          { 1,1 },
          { 1,1,1 },
        },

        { 
          { 0,1,1},
          { 1,1,1 },
        },

        { 
          { 1 },
          { 1,1 },
          { 1,1 },
        },

        { 
          { 1,1 },
          { 1,1 },
          { 1 },
        },

        { 
          { 0,1 },
          { 1,1 },
          { 1,1 },
        },

        { 
          { 1,1 },
          { 1,1 },
          { 0,1 },
        },
    };

/* ----------------------------------- */
    BoolPic p4[] = 
    { 
        { { 1,1,1 },
          { 1,0,1 } },

        { 
          { 1,0,1 },
          { 1,1,1 },
        },

        { 
          { 1,1 },
          { 0,1 },
          { 1,1 },
        },

        { 
          { 1,1 },
          { 1,0 },
          { 1,1 },
        },
    };

/* ----------------------------------- */
    BoolPic p5[] = 
    { 
        { 
          { 1,1,1 },
          { 0,0,1,1 },
        },

        { 
          { 0,0,1,1 },
          { 1,1,1 },
        },

        { 
          { 0,1,1,1 },
          { 1,1 },
        },

        { 
          { 1,1 },
          { 0,1,1,1 },
        },

        { 
          { 1,0 },
          { 1,0 },
          { 1,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 1,1 },
          { 1,0 },
          { 1,0 },
        },

        { 
          { 0,1 },
          { 0,1 },
          { 1,1 },
          { 1,0 },
        },


        { 
          { 1,0 },
          { 1,1 },
          { 0,1 },
          { 0,1 },
        },
    };



/* ----------------------------------- */
    BoolPic p6[] = 
    { 
        { 
          { 1,1,1 },
          { 1 },
          { 1 },
        },

        { 
          { 1 },
          { 1 },
          { 1,1,1 },
        },

        { 
          { 1,1,1 },
          { 0,0,1 },
          { 0,0,1 },
        },

        { 
          { 0,0,1 },
          { 0,0,1 },
          { 1,1,1 },
        },
    };


/* ----------------------------------- */
    BoolPic p7[] = 
    { 
        { 
          { 1,1,1 },
          { 0,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 0,1 },
          { 1,1,1 },
        },

        { 
          { 1 },
          { 1,1,1 },
          { 1 },
        },

        { 
          { 0,0,1 },
          { 1,1,1 },
          { 0,0,1 },
        },
    };

/* ----------------------------------- */
    BoolPic p8[] = 
    { 
        { 
          { 1,1 },
          { 0,1,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 0,1,1 },
          { 1,1 },
        },

        { 
          { 0,1,1 },
          { 1,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 1,1 },
          { 0,1,1 },
        },

        { 
          { 1,0 },
          { 1,1,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 1,1,1 },
          { 1,0 },
        },

        { 
          { 0,0,1 },
          { 1,1,1 },
          { 0,1 },
        },

        { 
          { 0,1 },
          { 1,1,1 },
          { 0,0,1 },
        },
    };


/* ----------------------------------- */
    BoolPic p9[] = 
    { 
        { 
          { 1,1 },
          { 0,1,1 },
          { 0,0,1 },
        },

        { 
          { 0,0,1 },
          { 0,1,1 },
          { 1,1 },
        },

        { 
          { 1 },
          { 1,1 },
          { 0,1,1 },
        },

        { 
          { 0,1,1 },
          { 1,1 },
          { 1 },
        },
    };

/* ----------------------------------- */
    BoolPic p10[] = 
    { 
        { 
          { 1,1 },
          { 0,1 },
          { 0,1,1 },
        },

        { 
          { 0,1,1 },
          { 0,1 },
          { 1,1 },
        },

        { 
          { 1 },
          { 1,1,1 },
          { 0,0,1 },
        },

        { 
          { 0,0,1 },
          { 1,1,1 },
          { 1 },
        },
    };

/* ----------------------------------- */
    BoolPic p11[] = 
    { 
        { 
          { 0,1 },
          { 1,1,1 },
          { 0,1 },
        },
    };

/* ----------------------------------- */

    m_pieces.push_back(new Piece(p0, NELEM(p0), "0 "));
    m_pieces.push_back(new Piece(p1, NELEM(p1), "1 "));
    m_pieces.push_back(new Piece(p2, NELEM(p2), "2 "));
    m_pieces.push_back(new Piece(p3, NELEM(p3), "3 "));
    m_pieces.push_back(new Piece(p4, NELEM(p4), "4 "));
    m_pieces.push_back(new Piece(p5, NELEM(p5), "5 "));
    m_pieces.push_back(new Piece(p6, NELEM(p6), "6 "));
    m_pieces.push_back(new Piece(p7, NELEM(p7), "7 "));
    m_pieces.push_back(new Piece(p8, NELEM(p8), "8 "));
    m_pieces.push_back(new Piece(p9, NELEM(p9), "9 "));
    m_pieces.push_back(new Piece(p10, NELEM(p10), "10"));
    m_pieces.push_back(new Piece(p11, NELEM(p11), "11"));
}



void Puzzle::solve()
{
    static size_t numPuzzleRow = NELEM(m_puzzleUnits);
    static size_t numPuzzleCol = NELEM(m_puzzleUnits[0]);
    static size_t pieceCount = m_pieces.size();

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
        auto aPiece = m_pieces[pieceIndex];
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
    puzzle.solve();
}


#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "ARRAYSIZE.h"

using namespace std;
using namespace boost;

class Unit : public ReferenceCounted 
{
  public:
    Unit() : m_rowOffset(0), m_colOffset(0)
    {
    }

    int m_rowOffset, m_colOffset;
};


const unsigned int MAXSPAN=5;

typedef bool  BoolPic[MAXSPAN][MAXSPAN] ;

class Cutout  : public ReferenceCounted
{
  public:
    Cutout (BoolPic a);
    vector<intrusive_ptr<Unit> > m_units;
};

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

class Piece  : public ReferenceCounted
{
  public:
    Piece (BoolPic a[], size_t numPics, const char* id);
    vector<intrusive_ptr<Cutout> > m_cutouts;
    bool inUse;
    string m_id;
};

Piece::Piece(BoolPic a[], size_t numPics, const char* id)
    : inUse(false), m_id(id)
{
    for (size_t pic = 0; pic < numPics; ++pic)
    {
        intrusive_ptr<Cutout> oneCutout= new Cutout(a[pic]);
        m_cutouts.push_back(oneCutout);
    }
}

class Puzzle
{
    public:
        Puzzle();
        void solve();
        void display();
    private:
        intrusive_ptr<Piece> m_puzzleUnits[20][3];
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

    m_pieces.push_back(new Piece(p0, ARRAYSIZE(p0), "0 "));
    m_pieces.push_back(new Piece(p1, ARRAYSIZE(p1), "1 "));
    m_pieces.push_back(new Piece(p2, ARRAYSIZE(p2), "2 "));
    m_pieces.push_back(new Piece(p3, ARRAYSIZE(p3), "3 "));
    m_pieces.push_back(new Piece(p4, ARRAYSIZE(p4), "4 "));
    m_pieces.push_back(new Piece(p5, ARRAYSIZE(p5), "5 "));
    m_pieces.push_back(new Piece(p6, ARRAYSIZE(p6), "6 "));
    m_pieces.push_back(new Piece(p7, ARRAYSIZE(p7), "7 "));
    m_pieces.push_back(new Piece(p8, ARRAYSIZE(p8), "8 "));
    m_pieces.push_back(new Piece(p9, ARRAYSIZE(p9), "9 "));
    m_pieces.push_back(new Piece(p10, ARRAYSIZE(p10), "10"));
    m_pieces.push_back(new Piece(p11, ARRAYSIZE(p11), "11"));
}

void Puzzle::solve()
{
    static size_t numPuzzleRow = ARRAYSIZE(m_puzzleUnits);
    static size_t numPuzzleCol = ARRAYSIZE(m_puzzleUnits[0]);
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

    for (int pieceIndex = 0; pieceIndex < pieceCount; ++pieceIndex)
    {
        intrusive_ptr<Piece> aPiece = m_pieces[pieceIndex];
        if (!aPiece->inUse)
        {
            // iterate over each cutout in the piece
            size_t numCutouts = aPiece->m_cutouts.size();

            for (unsigned int cutoutIndex = 0; cutoutIndex < numCutouts; ++cutoutIndex)
            {
                intrusive_ptr<Cutout> aCutout = aPiece->m_cutouts[cutoutIndex];

                // iterate over each cutout in the piece
                size_t numUnits = aCutout->m_units.size();


                for (unsigned int unitIndex = 0; unitIndex < numUnits; ++unitIndex)
                {
                    intrusive_ptr<Unit> u0 = aCutout->m_units[unitIndex];

                    unsigned int numUnitsPlaced;
                    for (numUnitsPlaced = 0; numUnitsPlaced < numUnits; ++numUnitsPlaced)
                    {
                        intrusive_ptr<Unit> u1 = aCutout->m_units[numUnitsPlaced];

                        int unitRow = row - u0->m_rowOffset + u1->m_rowOffset;
                        int unitCol = col - u0->m_colOffset + u1->m_colOffset;


                        if (   ( unitRow < 0 )
                            || ( unitRow >= numPuzzleRow )
                            || ( unitCol < 0 )
                            || ( unitCol >= numPuzzleCol )
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
                        intrusive_ptr<Unit> u1 = aCutout->m_units[i];

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
    static size_t numPuzzleRow = ARRAYSIZE(m_puzzleUnits);
    static size_t numPuzzleCol = ARRAYSIZE(m_puzzleUnits[0]);

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

main ()
{
    Puzzle puzzle;
    puzzle.solve();
}


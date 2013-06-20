#include "Coverage.h"
#include "BoolPic.h"
#include "utilities/ARRAYSIZE.h"
#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <iostream>
#include <iomanip>
#include <climits>


class Coverage;


class Placement  : public ReferenceCounted 
{
	public:
        bool initialize(int pieceIndex, const BoolPic& piecePic, int rowVal, int colVal, int pieceCount, unsigned int rowCount, unsigned int colCount);
		void show( int pieceCount, unsigned int rowCount, unsigned int colCount);

    private:
		std::vector<CellPtr > m_uses;  // one entry for each piece and h*w entries for puzzle
	friend class Coverage;
};

void Placement::show( int pieceCount, unsigned int rowCount, unsigned int colCount)
{
	std::cout << std::setw(3);
	for (int i = 0; i < 12; ++i)
	{
		std::cout << std::setw(3) << i;
	}

	std::cout << std::endl;

	for (int i = 0; i < pieceCount; ++i)
	{
		std::cout << std::setw(3) << (m_uses[i] ? 1 : 0);
	}
	std::cout << std::endl << std::endl;

	for (unsigned int colVal = 0; colVal < colCount; ++colVal)
	{
		for (unsigned int rowVal = 0; rowVal < rowCount; ++rowVal)
		{
			unsigned int index = pieceCount + colVal * rowCount + rowVal;
		    std::cout << std::setw(3) << (m_uses[index] ? 1 : 0);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
	
bool Placement::initialize(int pieceIndex, const BoolPic& piecePic, int rowVal, int colVal, int pieceCount, unsigned int rowCount, unsigned int colCount)
{
	m_uses.resize(pieceCount + rowCount * colCount); // 
	m_uses[pieceIndex] = new Cell();

	for (int i = 0; i<NELEM(piecePic); ++i)
	{

		for (int j = 0; j < NELEM(piecePic[0]); ++j)
		{
			bool u = piecePic[i][j];
			if (u)
			{
				unsigned int row = rowVal + i;
				unsigned int col = colVal + j;
				if (row >= rowCount || col >= colCount)
				{
					return false;
				} else
				{
					m_uses[pieceCount + (col * rowCount) + row] = new Cell();
				}

			}
		}
	}
	// show(pieceCount, rowCount, colCount);
	return true;
}


Coverage::Coverage (const BoolPicSet a[], unsigned int pieceCount, unsigned int rowCount, unsigned int colCount)
{
    for (size_t p = 0; p < pieceCount; ++p)
    {
		for (size_t orientation = 0; orientation < NELEM(a[0]); ++orientation)
		{
			if(isUsed(a[p][orientation]))
			{
				for (unsigned int rowVal = 0; rowVal < rowCount; ++rowVal)
				{
					for (unsigned int colVal = 0; colVal < colCount; ++colVal)
					{
						boost::intrusive_ptr<Placement> placement = new Placement;
						if (placement->initialize(p, a[p][orientation], rowVal, colVal, pieceCount, rowCount, colCount))
						{
							// placement->show(pieceCount, rowCount, colCount);
							m_placements.push_back(placement);
						}
					}
				}
			}
		}
    }
}

CellPtr Coverage::ConnectLinks()
{
	for (auto p : m_placements)
	{
		CellPtr first = NULL;
		CellPtr prev = NULL;
		for (auto u : p->m_uses)
		{
			if (u)
			{
				if (!first)
				{
					first = u;
					prev = u;
					continue;
				} else
				{
					prev->right = u;
					u->left = prev;
                    prev = u;
				} 
			}
		}
		prev -> right = first;
		first -> left = prev;
	}

    auto colCount = m_placements[0]->m_uses.size();

    CellPtr root = new Cell();

    auto firstCol = root;
    auto prevCol = root;

	for (unsigned int col = 0; col < colCount; ++col)
	{
        CellPtr colHead = new Cell();
		colHead->index = col;

        prevCol->right = colHead;
        colHead->left = prevCol;
        prevCol = colHead;

		auto prev = colHead;
		for (auto p : m_placements)
		{
			auto u = p->m_uses[col];
			if (u)
			{
                prev->down = u;
                u->up = prev;
				u->col = colHead;
                ++colHead->useCount;
                prev = u;
			}
		}
		prev -> down = colHead;
		colHead -> up = prev;
		colHead -> col = colHead;
	}

    prevCol -> right = root;
    root -> left = prevCol;

	return root;
}

static void uncover(CellPtr c)
{
    for (auto r = c->up; r != c; r = r->up)
	{
        for (auto row_it = r->left; row_it != r; row_it = row_it->left)
        {
			row_it->col->useCount++;
			row_it->up->down = row_it;
			row_it->down->up = row_it;
        }

	}
	c->left->right = c;
	c->right->left = c;
}

static void cover(CellPtr c)
{
	c->right->left = c->left;
	c->left->right = c->right;
    for (auto r = c->down; r != c; r = r->down)
	{
        for (auto row_it = r->right; row_it != r; row_it = row_it->right)
        {
			row_it->down->up = row_it->up;
			row_it->up->down = row_it->down;
			row_it->col->useCount--;
        }

	}

}


void DancingLinks::showSolution( )
{

	std::vector<unsigned int> display;
	display.resize(m_rowCount * m_colCount, INT_MAX);

	// BOOST_ASSERT(m_solution.size() == m_pieceCount);

	for (auto r : m_solution)
	{
        unsigned int piece = -1;
		auto row_it = r;
		while (piece == -1)
		{
			auto index = row_it->col->index;

			if (index < m_pieceCount)
			{
				piece = index;
			}
			row_it = row_it->right;
		}

		while (true)
		{
			auto index = row_it->col->index;

			if (index < m_pieceCount)
			{
                break;
			} else
			{
				display[index-m_pieceCount] = piece;
			}
			row_it = row_it->right;
		}
	}

	for (unsigned int row = 0; row < m_colCount; ++row)
	{
		for (unsigned int col = 0; col < m_rowCount; ++col)
		{
			unsigned int displayIndex = row * m_rowCount + col;
			unsigned int displayPiece = display[displayIndex];
			BOOST_ASSERT(displayPiece != INT_MAX);
			std::cout << std::setw(3) << displayPiece;
		}
        std::cout << std::endl;
	}
    std::cout << std::endl;
}

CellPtr DancingLinks::smallestCol( )
{
	auto minUse = UINT_MAX;
    CellPtr smallest = NULL;
    for (auto col = m_root->right; col != m_root; col = col->right)
    {
        if (col->useCount < minUse)
        {
            minUse = col->useCount;
            smallest = col;
        }
    }
    return smallest;
}

void DancingLinks::searchForward()
{
    for (auto smallest = smallestCol(); smallest; smallest = smallestCol())
	{
		if (smallest == smallest->down)
		{
			break;
		}

		cover(smallest);

		auto row = smallest->down;
		m_solution.push_back(row);

		// cover all the columns that share a row with this column
 		for (auto row_it = row->right; row_it != row; row_it = row_it->right)
		{
			cover(row_it->col);
		}
	}
}

bool DancingLinks::backtrack()
{
	while (true)
	{
		// we are done using this row in this column so uncover the columns cooresponding to it.
		auto row = m_solution.back();

		for (auto row_it = row->left; row_it != row; row_it = row_it->left)
		{
			uncover(row_it->col);
		}

		m_solution.pop_back();

		row = row->down;
		if (row != row->col)
		{
			m_solution.push_back(row);
			// if there is another row in this column then use it.
			for (auto row_it = row->right; row_it != row; row_it = row_it->right)
			{
				cover(row_it->col);
			}
			return true;
		} else
		{
			// we are done with this column so uncover it.
			uncover(row->col);
		}
		if (m_solution.size() == 0)
		{
			// this happens when all the rows in all the columns have been inspected.
			return false;
		}
	}
}

bool DancingLinks::getSolution( )
{
	if (m_root == m_root->right)
	{
		if (!backtrack())
		{
			return false;
		}
	}

	while (true)
	{
		searchForward();

		if (m_root == m_root->right)
		{
			return true;
		}

		if (!backtrack())
		{
			return false;
		}
	}
    return true;
}

void DancingLinks::solve()
{
	while(getSolution())
	{
		showSolution();
	}
}

void DancingLinks::search( )
{
    auto smallest = smallestCol();

    if (!smallest)
    {
        showSolution();
        return;
    }

    cover(smallest);

    for (auto row = smallest->down; row != smallest; row = row->down)
    {
        m_solution.push_back(row);
        for (auto row_it = row->right; row_it != row; row_it = row_it->right)
        {
            cover(row_it->col);
        }
        search();
        for (auto row_it = row->left; row_it != row; row_it = row_it->left)
        {
            uncover(row_it->col);
        }
		m_solution.pop_back();
    }

    uncover(smallest);
}

DancingLinks::DancingLinks( const BoolPicSet a[], unsigned int pieceCount, unsigned int rowCount, unsigned int colCount)
	: m_pieceCount(pieceCount), m_rowCount(rowCount), m_colCount(colCount)
{
	boost::intrusive_ptr<Coverage> coverage = new Coverage(a, pieceCount, rowCount, colCount);

	m_root = coverage->ConnectLinks();

}

#include "Coverage.h"
#include "BoolPic.h"
#include "utilities/ARRAYSIZE.h"
#include "utilities/ReferenceCounted.h"
#include "boost/intrusive_ptr.hpp"
#include <vector>
#include <iostream>
#include <iomanip>
#include <climits>


class Grid;
class GridRow;

class Cell {
public:
	Cell()
		: left(0), right(0), up(0), down(0), useCount(0), index(0)
	{
	}

	CellPtr left, right, up, down, col;
	unsigned int useCount;

	unsigned int index;

	friend class Grid;
};

class Grid : public ReferenceCounted
{
	public:
		CellPtr connectLinks();
        void Grid::addRow (const std::vector<int>& usage);

	private:
	    std::vector<boost::intrusive_ptr<GridRow> > m_rows;
};



class GridRow  : public ReferenceCounted 
{
	public:
		void initialize(const std::vector<int>& usage);
 		void show( int pieceCount, unsigned int rowCount, unsigned int colCount);

    private:
		std::vector<CellPtr > m_uses;  // one entry for each piece and h*w entries for puzzle
	friend class Grid;
};

void GridRow::show( int pieceCount, unsigned int rowCount, unsigned int colCount)
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
	
void GridRow::initialize(const std::vector<int>& usage)
{
    m_uses.resize(usage.size());
	for (unsigned int i = 0; i<usage.size(); ++i)
	{
        if (usage[i])
        {
            m_uses[i] = new Cell();
        }
    }
}


void Grid::addRow (const std::vector<int>& usage)
{
    boost::intrusive_ptr<GridRow> row = new GridRow();
    row->initialize(usage);
    m_rows.push_back(row);
}


CellPtr Grid::connectLinks()
{
	for (auto p : m_rows)
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

    auto colCount = m_rows[0]->m_uses.size();

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
		for (auto p : m_rows)
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

static void linkCol(CellPtr c)
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

static void unlinkCol(CellPtr c)
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

static void unlinkRow(CellPtr row)
{
    for (auto row_it = row->right; row_it != row; row_it = row_it->right)
    {
        unlinkCol(row_it->col);
    }
}

static void linkRow(CellPtr row)
{
    for (auto row_it = row->left; row_it != row; row_it = row_it->left)
    {
        linkCol(row_it->col);
    }
}

bool DancingLinks::solved() const
{
    return (m_root == m_root->right);
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

void DancingLinks::advance()
{
    for (auto smallest = smallestCol(); smallest; smallest = smallestCol())
	{
		if (smallest == smallest->down)
		{
			break;
		}

		unlinkCol(smallest);

		auto row = smallest->down;
		m_solution.push_back(row);
        unlinkRow(row);

	}
}

bool DancingLinks::backup()
{
	while (m_solution.size())
	{
		auto row = m_solution.back();
		m_solution.pop_back();

		// we are done using this row in this column so put back the columns cooresponding to it.
        linkRow(row);

		row = row->down;
		if (row != row->col)
		{
			// if there is another row in this column then use it.
			m_solution.push_back(row);
            unlinkRow(row);
			return true;
		} else
		{
			// we are done with this column so put it back .
			linkCol(row->col);
		}
	}
	// All columns were fully advanced so there's no more work to be done.
    return false;
}

bool DancingLinks::getSolution( )
{
	if (solved())
	{
		if (!backup())
		{
			return false;
		}
	}

	while (true)
	{
		advance();

		if (solved())
		{
			return true;
		}

		if (!backup())
		{
			return false;
		}
	}
    return true;
}

void DancingLinks::solve()
{
    m_root=m_grid->connectLinks();
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

    unlinkCol(smallest);

    for (auto row = smallest->down; row != smallest; row = row->down)
    {
        m_solution.push_back(row);
        for (auto row_it = row->right; row_it != row; row_it = row_it->right)
        {
            unlinkCol(row_it->col);
        }
        search();
        for (auto row_it = row->left; row_it != row; row_it = row_it->left)
        {
            linkCol(row_it->col);
        }
		m_solution.pop_back();
    }

    linkCol(smallest);
}

DancingLinks::DancingLinks( unsigned int pieceCount, unsigned int rowCount, unsigned int colCount)
	: m_pieceCount(pieceCount), m_rowCount(rowCount), m_colCount(colCount)
{
	m_grid = new Grid();
}

void DancingLinks::addRow (const std::vector<int>& usage)
{
    m_grid->addRow(usage);
}


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
		CellPtr connectLinks( const std::vector< std::vector< int > >& usage, const std::vector< std::string >& columns);

};


CellPtr Grid::connectLinks(const std::vector< std::vector< int > >& usage, const std::vector< std::string >& columns)
{
    CellPtr root = new Cell();
	root->left = root;

	auto colCount = columns.size();
	auto rowCount = usage.size();

	// connect the column head links
	for (unsigned int col = 0; col < colCount; ++col)
	{
        CellPtr column = new Cell();
		// column->name = columns[col];
		column->index = col;
		column->col = column;

		column->up = column;

        column->right = root;
        column->left = root->left;
		column->left->right = column;
		root->left = column;
		if (!root->right)
		{
			root->right = column;
		}
	}


	// for each row ...
	for (unsigned int row = 0; row < rowCount; ++row)
	{
	    CellPtr column = root->right;
		CellPtr firstInRow = NULL;
		auto usageRow = usage[row];
		assert(colCount == usageRow.size());
		// link up a Cell for each row entry that is used.
		for (unsigned int col = 0; col < colCount; ++col)
		{
			assert(column != root);
			if (usageRow[col])
			{
				CellPtr e = new Cell();
				e->col = column;
				++(column->useCount);
				if (!firstInRow)
				{
					firstInRow = e;
					e->left = e;
				} else
				{
					e->right = firstInRow;
					e->left = firstInRow->left;
				    e->left->right = e;
					firstInRow->left = e;
					if (!firstInRow->right)
					{
						firstInRow->right = e;
					}
				}
				e->down = column;
				e->up = column->up;
				e->up->down = e;
				column->up = e;
				if (!column->down)
				{
					column->down = e;
				}
			}
			column=column->right;
			// assert(column != root);
		}
		assert (column == root);
	}

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

DancingLinks::DancingLinks(
    unsigned int pieceCount, 
    unsigned int xSize, 
    unsigned int ySize,
    const std::vector< std::vector< int > >& usage,
    const std::vector< std::string >& columns)
      : m_pieceCount(pieceCount), m_rowCount(xSize), m_colCount(ySize)
{
	boost::intrusive_ptr<Grid> grid = new Grid();
	m_root= grid->connectLinks(usage, columns);
}

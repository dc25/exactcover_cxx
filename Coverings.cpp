#include "Coverings.h"
#include <vector>
#include <climits>
#include <algorithm>
#include <string>
#include <assert.h>

using namespace std;

class Cell;
typedef Cell* CellPtr;

class Cell {
public:
	Cell()
		: left(0), right(0), up(0), down(0), useCount(0), m_name(0)
	{
	}

	CellPtr left, right, up, down, col;
	unsigned int useCount;

	char* m_name;
};

static void linkCol(Cell* c)
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

static void unlinkCol(Cell* c)
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

static void unlinkRow(Cell* row)
{
    for (auto row_it = row->right; row_it != row; row_it = row_it->right)
    {
        unlinkCol(row_it->col);
    }
}

static void linkRow(Cell* row)
{
    for (auto row_it = row->left; row_it != row; row_it = row_it->left)
    {
        linkCol(row_it->col);
    }
}

Cell* Coverings::smallestCol( ) const
{
	auto minUse = UINT_MAX;
    Cell* smallest = NULL;
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

void Coverings::advance()
{
    for (auto smallest = smallestCol(); smallest; smallest = smallestCol())
	{
		if (smallest == smallest->down)
		{
			break;
		}

		++num_searches;
		unlinkCol(smallest);

		auto row = smallest->down;
		m_solution.push_back(row);
        unlinkRow(row);
	}
}

bool Coverings::backup()
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

void Coverings::makeNameSolution()
{
	m_nameSolution.resize(0);
	m_nameSolution.resize(m_solution.size());
	unsigned int solutionIndex = 0;
    for ( auto r : m_solution )
    {
		std::vector<string> temp;
        for ( auto e = r; true;)
        {
            temp.push_back(e->col->m_name);
			e=e->right;
			if (e == r)
			{
				break;
			}
        }
        sort(temp.begin(), temp.end());

		for (auto name : temp)
		{
			m_nameSolution[solutionIndex].push_back(string(name));
		}
		solutionIndex++;
    }
}
    
const std::vector< std::vector<string> >* Coverings::getSolution() 
{
    if (m_root == m_root->right)
	{
		if (!backup())
		{
			return NULL;
		}
	}

	while (true)
	{
		advance();

        if (m_root == m_root->right)
		{
			makeNameSolution();
			return &m_nameSolution;
		}

		if (!backup())
		{
			return NULL;
		}
	}
    return NULL; // should never get here
}

Coverings::Coverings(
    const std::vector< std::vector< int > >& usage,
    const std::vector< std::string >& columns,
	unsigned int secondary)
	: num_searches(0)
{
	auto root = new Cell();
	root->left = root;
	root->right = root;

	auto colCount = columns.size();
	auto rowCount = usage.size();

	// connect the column head links
	for (unsigned int col = 0; col < colCount; ++col)
	{
        auto column = new Cell();
		column->m_name = new char[columns[col].size() + 1];
		strcpy(column->m_name, columns[col].c_str());
		column->col = column;
		column->up = column;

        column->right = root;
        column->left = root->left;
		column->right->left = column;
		column->left->right = column;

	}

	// for each row ...
	for (unsigned int row = 0; row < rowCount; ++row)
	{
	    auto column = root->right;
		Cell* firstInRow = NULL;
		auto usageRow = usage[row];
		assert(colCount == usageRow.size());
		// link up a Cell for each row entry that is used.
		for (unsigned int col = 0; col < colCount; ++col)
		{
			assert(column != root);
			if (usageRow[col])
			{
				auto e = new Cell();
				e->col = column;
				++(column->useCount);
				if (!firstInRow)
				{
					firstInRow = e;
					e->left = e;
					e->right = e;
				} else
				{

                    e->right = firstInRow;
                    e->left = firstInRow->left;
                    e->right->left = e;
                    e->left->right = e;

				}
				e->down = column;
				e->up = column->up;
				e->down->up = e;
				e->up->down = e;
			}
			column=column->right;
		}
		assert (column == root);
	}

	for (unsigned int s = 0; s < secondary; ++s)
	{
		auto column = root->left;
		root->left = column->left;
		root->left->right = root;
		column->left = column->right = column;
	}

	m_root = root;
}

#include "Coverings.h"
#include <vector>
#include <map>
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
		: m_left(0), m_right(0), m_up(0), m_down(0), m_useCount(0), m_name(0)
	{
	}

	~Cell()
	{
        delete[] m_name;
	}

	CellPtr m_left, m_right, m_up, m_down, col;
	unsigned int m_useCount;

	char* m_name;
};

static void linkCol(Cell* c)
{
    for (auto r = c->m_up; r != c; r = r->m_up)
	{
        for (auto row_it = r->m_left; row_it != r; row_it = row_it->m_left)
        {
			row_it->col->m_useCount++;
			row_it->m_up->m_down = row_it;
			row_it->m_down->m_up = row_it;
        }

	}
	c->m_left->m_right = c;
	c->m_right->m_left = c;
}

static void unlinkCol(Cell* c)
{
	c->m_right->m_left = c->m_left;
	c->m_left->m_right = c->m_right;
    for (auto r = c->m_down; r != c; r = r->m_down)
	{
        for (auto row_it = r->m_right; row_it != r; row_it = row_it->m_right)
        {
			row_it->m_down->m_up = row_it->m_up;
			row_it->m_up->m_down = row_it->m_down;
			row_it->col->m_useCount--;
        }

	}

}

static void unlinkRow(Cell* row)
{
    for (auto row_it = row->m_right; row_it != row; row_it = row_it->m_right)
    {
        unlinkCol(row_it->col);
    }
}

static void linkRow(Cell* row)
{
    for (auto row_it = row->m_left; row_it != row; row_it = row_it->m_left)
    {
        linkCol(row_it->col);
    }
}

Cell* Coverings::smallestCol( ) const
{
	auto minUse = UINT_MAX;
    Cell* smallest = NULL;
    for (auto col = m_root->m_right; col != m_root; col = col->m_right)
    {
        if (col->m_useCount < minUse)
        {
            minUse = col->m_useCount;
            smallest = col;
        }
    }
    return smallest;
}

void Coverings::advance()
{
    for (auto smallest = smallestCol(); smallest; smallest = smallestCol())
	{
		if (smallest == smallest->m_down)
		{
			break;
		}

		++num_searches;
		unlinkCol(smallest);

		auto row = smallest->m_down;
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

		row = row->m_down;
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
			e=e->m_right;
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
    if (m_root == m_root->m_right)
	{
		if (!backup())
		{
			return NULL;
		}
	}

	while (true)
	{
		advance();

        if (m_root == m_root->m_right)
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
    const std::vector< std::vector< std::string > >& usage,
    const std::vector< std::string >& columns,
	unsigned int secondary)
	: num_searches(0)
{
	auto root = new Cell();
	root->m_left = root;
	root->m_right = root;

	auto colCount = columns.size();
	auto rowCount = usage.size();

    std::map<std::string, Cell*> columnMap;

	// connect the column head links
	for (unsigned int col = 0; col < colCount; ++col)
	{
        auto column = new Cell();
		column->m_name = new char[columns[col].size() + 1];
		strcpy(column->m_name, columns[col].c_str());
        columnMap[columns[col]] = column;
		column->col = column;
		column->m_up = column;
		column->m_down = column;

        column->m_right = root;
        column->m_left = root->m_left;
		column->m_right->m_left = column;
		column->m_left->m_right = column;

	}

	// for each row ...
	for (unsigned int row = 0; row < rowCount; ++row)
	{
		Cell* firstInRow = NULL;
		auto usageRow = usage[row];
		// link up a Cell for each row entry that is used.
		for (unsigned int eIndex = 0; eIndex < usageRow.size(); ++eIndex)
		{
            auto column = columnMap[usageRow[eIndex]];
            auto e = new Cell();
            e->col = column;
            e->m_down = column;
            e->m_up = column->m_up;
            e->m_down->m_up = e;
            e->m_up->m_down = e;

            ++(column->m_useCount);
            if (!firstInRow)
            {
                firstInRow = e;
                e->m_left = e;
                e->m_right = e;
            } else
            {

                e->m_right = firstInRow;
                e->m_left = firstInRow->m_left;
                e->m_right->m_left = e;
                e->m_left->m_right = e;

            }
		}
	}

	for (unsigned int s = 0; s < secondary; ++s)
	{
		auto column = root->m_left;
		root->m_left = column->m_left;
		root->m_left->m_right = root;
		column->m_left = column->m_right = column;
	}

	m_root = root;
}

Coverings::~Coverings()
{
    for (auto col = m_root->m_right; col != m_root; )
    {
        for (auto e = col->m_down; e != col; )
        {
            auto e_next = e->m_down;
            delete e;
            e = e_next;
        }
        auto col_next = col->m_right;
        delete col;
        col = col_next;
    }
    delete m_root;
}

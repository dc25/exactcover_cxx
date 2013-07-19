/*
Dancing Links / Exact Cover library with c++ and python interfaces
Copyright (c) 2013 by David Compton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include "Coverings.h"
#include "Answer.h"
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

#include <iostream>

using namespace std;

class Cell {
public:
    Cell()
        : m_left(0), m_right(0), m_up(0), m_down(0), m_col(0), m_useCount(0), m_name(0)
    {
    }

    ~Cell()
    {
        delete[] m_name;
    }

    Cell* m_left;
    Cell* m_right;
    Cell* m_up;
    Cell* m_down;
    Cell* m_col;

	unsigned int m_useCount;

    char* m_name;  // tried using a string here but that slowed down run time noticably.
};

// Remove a column (unlink it) from its Coverings object.  
static void unlinkCol(Cell* c)
{
    // unlink from linked list of columns.
    c->m_right->m_left = c->m_left;
    c->m_left->m_right = c->m_right;
    for (auto r = c->m_down; r != c; r = r->m_down)
    {
        // unlink every row that is in this column
        for (auto row_it = r->m_right; row_it != r; row_it = row_it->m_right)
        {
            row_it->m_down->m_up = row_it->m_up;
            row_it->m_up->m_down = row_it->m_down;
            row_it->m_col->m_useCount--;
        }

    }

}

// Put a column (link it) back into its Coverings object.  
// Sequence of operations is reverse of unlink.
static void linkCol(Cell* c)
{
    for (auto r = c->m_up; r != c; r = r->m_up)
    {
        for (auto row_it = r->m_left; row_it != r; row_it = row_it->m_left)
        {
            row_it->m_col->m_useCount++;
            row_it->m_up->m_down = row_it;
            row_it->m_down->m_up = row_it;
        }

    }
    c->m_left->m_right = c;
    c->m_right->m_left = c;
}

// Remove (unlink) a row from its Coverings object.
static void unlinkRow(Cell* row)
{
    // Note that we unlink every column *except* for the one that the cell
    // that we start with is actually in.  That column was unlinked prior
    // to making this call.  This is as in Knuth's dancing links paper.
    for (auto row_it = row->m_right; row_it != row; row_it = row_it->m_right)
    {
        unlinkCol(row_it->m_col);
    }
}

// Put (link) a row back into its Coverings object.
static void linkRow(Cell* row)
{
    for (auto row_it = row->m_left; row_it != row; row_it = row_it->m_left)
    {
        linkCol(row_it->m_col);
    }
}

Cell* Coverings::smallestCol( ) const
{
    Cell* smallest = nullptr;
    for (auto col = m_root->m_right; col != m_root; col = col->m_right)
    {
        if (smallest == nullptr || smallest->m_useCount > col->m_useCount)
        {
            smallest = col;
        }
    }
    return smallest;
}

// Generate a vector of names from the existing vector of rows.
void Coverings::makeNameSolution()
{
    shared_ptr<Answer> answer( new Answer() );
    answer->resize(m_solution.size());
    unsigned int solutionIndex = 0;
    for ( auto r : m_solution )
    {
        // Sort because convention is to expect piece name
        // to be last item in each row for the solution.
        std::vector<string> temp;
        for ( auto e = r; true;)
        {
            temp.push_back(e->m_col->m_name);
            e=e->m_right;
            if (e == r)
            {
                break;
            }
        }
        sort(temp.begin(), temp.end());

        for (auto name : temp)
        {
            answer->getRow(solutionIndex).push_back(string(name));
        }
        solutionIndex++;
    }
    m_solutionQueue.push(answer);
}

// Repeat advance/backup/advance until a solution is reached.
// From row based solution, generate string based solution.
// Return pointer to string based solution or nullptr if none exists.
shared_ptr<Answer> Coverings::getSolution() 
{
    return m_solutionQueue.deque();
}

void Coverings::recursiveSearch( )
{
    if (m_root == m_root->m_right)
    {
        makeNameSolution();
        return;
    }

    auto smallest=smallestCol();
    if (smallest->m_useCount == 0) 
    {
        // No way to "cover" this location so we must backtrack
        return;
    }

    ++num_searches;
    unlinkCol(smallest);

    for (auto row = smallest->m_down; row != smallest; row = row->m_down)
    {
        m_solution.push_back(row);
        unlinkRow(row);
        recursiveSearch();
        linkRow(row);
        m_solution.pop_back();
    }

    linkCol(smallest);
}

void Coverings::search( )
{
    recursiveSearch();
    m_solutionQueue.push(shared_ptr<Answer>(nullptr));
}

// Usage matrix has a row for every possible placement of every puzzle 
// piece.  Each usage matrix row contains the name of the piece 
// and the name of every "cell" of the puzzle that this piece occupies 
// for this row's placement.  The columns vector has the names of all 
// the pieces and "cells" of the puzzle.  Really, the naming convention 
// is arbitrary but the columns should contain all of the names that 
// are used in all of the rows.
Coverings::Coverings(
    const std::vector< std::vector< std::string > >& usage,
    const std::vector< std::string >& columns,
    const std::vector< std::vector< std::string > >& startingSolution,
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
        auto bufferSize = columns[col].size() + 1;
        column->m_name = new char[bufferSize];
        strcpy_s(column->m_name, bufferSize, columns[col].c_str());
        columnMap[columns[col]] = column;  // save for lookup by name.
        column->m_col = column;
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
        Cell* firstInRow = nullptr;
        auto usageRow = usage[row];
        // link up a Cell for each row entry that is used.
        for (unsigned int eIndex = 0; eIndex < usageRow.size(); ++eIndex)
        {
            auto column = columnMap[usageRow[eIndex]]; // lookup by cell name.
            auto e = new Cell();
            e->m_col = column;
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

    // detach the "secondary" columns.
    for (unsigned int s = 0; s < secondary; ++s)
    {
        auto column = root->m_left;
        root->m_left = column->m_left;
        root->m_left->m_right = root;
        column->m_left = column->m_right = column;
    }

    m_root = root;

    // The search is on... in a new thread.
    m_worker = std::thread(&Coverings::search, this);
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

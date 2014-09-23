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
#include "Solution.h"
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

#include <iostream>

using namespace std;

//
// Objects of type Cell represent a single cell 
// in the "dancing links" sparse matrix.  Each 
// cell is part of two doubly linked lists.
//
class Cell {
public:
    Cell()
        : m_col(nullptr), m_useCount(0)
    {
        // Regard a new cell as the head of an empty linked list.  Initially all links point to self.
        m_left = this;
        m_right = this;
        m_up = this;
        m_down = this;
    }

    // append "e" to end of doubly linked list formed using m_left/m_right members.
    void horizontalAppend(Cell* e)
    {
        e->m_right = this;
        e->m_left = e->m_right->m_left;
        e->m_right->m_left = e;
        e->m_left->m_right = e;
    }

    // Append "e" to bottom of doubly linked list formed using m_up/m_down members.
    // Also set m_col member of e to point at linked list head and increment list use count.
    void verticalAppend(Cell* e)
    {
        e->m_col = this;
        e->m_down = this;
        e->m_up = e->m_down->m_up;
        e->m_down->m_up = e;
        e->m_up->m_down = e;

        ++(this->m_useCount);
    }

    Cell* m_left;
    Cell* m_right;
    Cell* m_up;
    Cell* m_down;
    Cell* m_col;

    std::shared_ptr< std::vector<std::string> > m_rowNames;  // row cells only.

	unsigned int m_useCount;  // column cells only.

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

// 
// Iterate over columns to find the one with lowest useCount (indicating 
// the fewest rows ).
//
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

//
// This method, getSolution() is called from python to get the 
// next solution in the queue.  The call blocks if there is no
// solution in the queue.  The queue itself is thread safe.
//
shared_ptr<Solution> Coverings::getSolution() 
{
    return m_solutionQueue.deque();
}

//
// Set bool to request state from solver thread.  Wait for solver thread
// to assign m_solverState and reset bool indicating that state is ready.
//
std::shared_ptr<Solution> Coverings::getState()
{
    std::unique_lock<std::mutex> lock(m_stateRequestMutex);

    if (!m_solverRunning)
    {
        // Return the oldest unprocessed solution if any.
        m_solverState = m_solutionQueue.front();
        if (m_solverState == nullptr)
        {
            // Otherwise, return the current solution in progress.
            m_solverState = make_shared<Solution>(*m_solution);
        }
        return m_solverState;
    } 

    // if solver is running, set flag and wait .
    m_stateRequest = true;
    while(m_stateRequest) 
    {
        m_stateReady.wait(lock);
    }
    return m_solverState;
}

//
// Check for a request for current state.  If request was made, then
// make the state, and notify the requester that the state is ready.
//
void Coverings::respondToStateRequest( )
{
    std::lock_guard<std::mutex> lock(m_stateRequestMutex);
    if (m_stateRequest)
    {
        // Return the oldest unprocessed solution if any.
        m_solverState = m_solutionQueue.front();
        if (m_solverState == nullptr)
        {
            // Otherwise, return the current solution in progress.
            m_solverState = make_shared<Solution>(*m_solution);
        }
        // tell waiting thread the wait is over and a state is available.
        m_stateRequest = false;
        m_stateReady.notify_one();
    }
}

//
//This function, recursiveSearch(), implements the dancing links
//algorithm to search for exact cover solutions.  
//
void Coverings::recursiveSearch(unsigned int level)
{
    // If state requested, prepare state and 
    // notify waiting thread that state is ready.
    respondToStateRequest();  

    if (m_root == m_root->m_right)
    {
        m_solutionQueue.push(make_shared<Solution>(*m_solution));
        return;
    }

    ++num_searches;
    auto smallest=smallestCol();
    if (smallest->m_useCount == 0) 
    {
        // No way to "cover" this location so we must backtrack
        return;
    }

    unlinkCol(smallest);

    for (auto row = smallest->m_down; row != smallest; row = row->m_down)
    {
        // If we have a starting solution, then fast forward loop (i.e. 
        // don't bother with recursing ) until we hit the first row in 
        // the starting solution.
        if (m_solution->size() > level)
        {
            if (m_solution->getRow(level) != *(row->m_rowNames))
            {
                continue;
            }
        } else
        {
            m_solution->push_back(row->m_rowNames);
        }
        unlinkRow(row);
        recursiveSearch(level+1);
        linkRow(row);
        m_solution->pop_back();
    }

    linkCol(smallest);
}

void Coverings::search( )
{
    m_solverRunning = true;
    recursiveSearch(0);
    m_solutionQueue.push(shared_ptr<Solution>(nullptr)); //terminator
    m_solverRunning = false;
    respondToStateRequest();  
}

//
// Rows matrix has a row for every possible placement of every puzzle 
// piece.  Each row in the rows matrix contains the name of the piece 
// and the name of every "cell" of the puzzle that this piece occupies 
// for this row's placement.  The columns vector has the names of all 
// the pieces and "cells" of the puzzle.  The startingSolution vector 
// is used when restarting.  
//
Coverings::Coverings(
    const std::vector< std::vector< std::string > >& rows,
    const std::vector< std::string >& columns,
    const std::vector< std::vector< std::string > >& startingSolution,
    unsigned int secondary)
    : num_searches(0), m_solutionQueue(128), m_stateRequest(false), m_solverRunning(false)
{
    m_root = new Cell();

    // Short term map to identify by name which column a row entry belongs to.
    std::map<std::string, Cell*> columnMap;

    // Connect the column head links
    for (auto& columnName : columns)
    {
        auto column = new Cell();
        columnMap[columnName] = column;  // save for lookup by name.
        m_root->horizontalAppend(column);
    }

    // for each row ...
    for (auto& oneRow : rows)
    {
        Cell* firstInRow = nullptr;

        auto rowNames = make_shared<vector<string> >();
        for (auto& unitName : oneRow)
        {
            rowNames->push_back(unitName);
        } 
        sort(rowNames->begin(), rowNames->end());

        // link up a Cell for each row entry that is used.
        for (auto& unitName : oneRow)
        {
            auto column = columnMap[unitName]; // lookup by cell name.
            auto e = new Cell();
            e->m_rowNames = rowNames;
            column->verticalAppend(e);
            if (!firstInRow)
            {
                firstInRow = e;
            } else
            {
                firstInRow->horizontalAppend(e);
            }
        }
    }

    // detach the "secondary" columns.
    for (unsigned int s = 0; s < secondary; ++s)
    {
        auto column = m_root->m_left;
        column->m_right->m_left = column->m_left;
        column->m_left->m_right = column->m_right;
        column->m_left = column->m_right = column;
    }

    m_solution = make_shared<Solution>(startingSolution);

    // The search is on... in a new thread.
    m_worker = std::thread(&Coverings::search, this);
}

Coverings::~Coverings()
{
    m_worker.join(); // wait for the worker thread to finish
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

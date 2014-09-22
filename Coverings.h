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

#ifndef coverings_h__
#define coverings_h__

#include <vector>
#include <memory>
#include <thread>
#include "SafeQueue.h"

class Cell;
class Solution;

/* 
 * An object of type Coverings works with "Polyform Puzzler" to solve
 * an exact cover puzzle.  When a Coverings object is created it 
 * immediately starts to solve the specified puzzle in a separate thread.
 *
 * Solutions can be retrieved using getSolution() which returns a smart pointer
 * to a Solution object.  After the last solution is found, getSolution() returns
 * a null pointer.  If the solver is not done but no solutions are queued then 
 * getSolution() blocks.
 *
 * The non-blocking method, getState(), also returns a solution (which may 
 * or may not be complete).  This is to be used when restarting the solver
 * If there is a queued solution, then getState() returns that but does not
 * remove it from the queue.  Otherwise, getState() returns the 
 * incomplete solution in progress.
 *
 * Class Coverings is accessible in python in addition to c++.  This is
 * accomplished using swig.
 */

class Coverings 
{
public:

    Coverings(
        const std::vector< std::vector< std::string > >& rows,
        const std::vector< std::string >& columns,
        const std::vector< std::vector< std::string > >& startingSolution,
        unsigned int secondary=0);

    ~Coverings();

    std::shared_ptr<Solution> getSolution();
    std::shared_ptr<Solution> getState();

    unsigned long long int num_searches;

private:
    Cell* smallestCol() const;
    void recursiveSearch(unsigned int level);
    void search();

    Cell* m_root;  // entry to the sparse matrix of cells.
    std::shared_ptr<Solution> m_solution;
    SafeQueue<std::shared_ptr<Solution> >  m_solutionQueue;

    std::thread m_worker;

    // state and state request related methods and variables.
    void respondToStateRequest( );

    std::shared_ptr<Solution> m_solverState;
    bool m_stateRequest;
    bool m_solverRunning;
    std::mutex m_stateRequestMutex;
    std::condition_variable m_stateReady;

};

#endif

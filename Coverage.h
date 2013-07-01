#ifndef coverage_h__
#define coverage_h__

#include "utilities/ReferenceCounted.h"
#include <vector>

class Cell;

class DancingLinks : public ReferenceCounted
{
public:
	DancingLinks(
        const std::vector< std::vector< int > >& usage,
        const std::vector< std::string >& columns);

    const std::vector< std::vector<std::string> >* getSolution();

private:
    void connectLinks( 
        const std::vector< std::vector< int > >& usage, 
        const std::vector< std::string >& columns);

    Cell* smallestCol() const;
	void advance();
	bool backup();

    void makeNameSolution();

	Cell* m_root;
	std::vector<Cell* > m_solution;
	std::vector < std::vector<std::string> > m_nameSolution;
};


#endif

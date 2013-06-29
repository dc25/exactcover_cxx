#ifndef coverage_h__
#define coverage_h__

#include "utilities/ReferenceCounted.h"
#include "BoolPic.h"
#include <vector>
#include "boost/intrusive_ptr.hpp"

class Cell;

class DancingLinks : public ReferenceCounted
{
public:
	DancingLinks(
        unsigned int pieceCount, 
        unsigned int xSize, 
        unsigned int ySize,
        const std::vector< std::vector< int > >& usage,
        const std::vector< std::string >& columns);

	void search( );
	void solve();
    bool getSolution();

private:
	unsigned int m_pieceCount;
    unsigned int m_rowCount; 
    unsigned int m_colCount;

    void connectLinks( 
        const std::vector< std::vector< int > >& usage, 
        const std::vector< std::string >& columns);

    Cell* smallestCol();
	void advance();
	bool backup();

	bool solved() const;

	void showSolution();
	Cell* m_root;

	std::vector<Cell* > m_solution;
};


#endif

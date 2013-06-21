#ifndef coverage_h__
#define coverate_h__

#include "utilities/ReferenceCounted.h"
#include "BoolPic.h"
#include <vector>
#include "boost/intrusive_ptr.hpp"

class Cell;
typedef Cell* CellPtr;

class DancingLinks : public ReferenceCounted
{
public:
	DancingLinks(const BoolPicSet a[], unsigned int pieceCount, unsigned int xSize, unsigned int ySize);
	void search( );
	void solve();
    bool getSolution();

private:
	unsigned int m_pieceCount;
    unsigned int m_rowCount; 
    unsigned int m_colCount;

    CellPtr smallestCol();
	void advance();
	bool backup();

	bool solved() const;

	void showSolution();
	CellPtr m_root;

	std::vector<CellPtr > m_solution;
};


#endif

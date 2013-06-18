#ifndef coverage_h__
#define coverate_h__

#include "utilities/ReferenceCounted.h"
#include "BoolPic.h"
#include <vector>
#include "boost/intrusive_ptr.hpp"

class Placement;
class Cell;

class Coverage : public ReferenceCounted
{
	public:
	    Coverage (const BoolPicSet a[], unsigned int pieceCount, unsigned int xSize, unsigned int ySize);
		boost::intrusive_ptr<Cell> ConnectLinks();
	private:
	    std::vector<boost::intrusive_ptr<Placement> > m_placements;
};


class DancingLinks : public ReferenceCounted
{
public:
	DancingLinks(const BoolPicSet a[], unsigned int pieceCount, unsigned int xSize, unsigned int ySize);
	void search( );

private:
	unsigned int m_pieceCount;
    unsigned int m_rowCount; 
    unsigned int m_colCount;

	void showSolution();
	boost::intrusive_ptr<Cell> m_root;
	std::vector<boost::intrusive_ptr<Cell> > solution;
};


#endif

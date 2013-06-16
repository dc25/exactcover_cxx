#ifndef Piece_h__
#define Piece_h__

#include "utilities/ReferenceCounted.h"
#include "BoolPic.h"
#include <string>
#include <vector>


class Unit : public ReferenceCounted 
{
  public:
    Unit() : m_rowOffset(0), m_colOffset(0)
    {
    }

    int m_rowOffset, m_colOffset;
};

class Cutout  : public ReferenceCounted
{
  public:
    Cutout (BoolPic a);
    std::vector<boost::intrusive_ptr<Unit> > m_units;
};

class Piece  : public ReferenceCounted
{
  public:
    Piece (BoolPic a[], size_t numPics, const char* id);
    std::vector<boost::intrusive_ptr<Cutout> > m_cutouts;
    bool inUse;
    std::string m_id;
};

#endif // Piece_h__
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
    Cutout::Cutout(const BoolPic& a);
    std::vector<boost::intrusive_ptr<Unit> > m_units;
};

class Piece  : public ReferenceCounted
{
  public:
	Piece(const BoolPicSet& a, int index);
    void addCutout(const boost::intrusive_ptr<Cutout>& oneCutout);

    std::vector<boost::intrusive_ptr<Cutout> > m_cutouts;
    bool inUse;
    std::string m_id;
};


class PieceSet : public ReferenceCounted
{
public:
	PieceSet (const BoolPicSet a[], size_t numPicSets);
	const boost::intrusive_ptr<Piece>& operator[](const int i) const;
    const size_t size();

private:
	std::vector<boost::intrusive_ptr<Piece> > m_pieces;
};
#endif // Piece_h__

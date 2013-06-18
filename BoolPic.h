#ifndef BoolPIC_H__
#define BoolPIC_H__

const unsigned int MAXSPAN=5;
const unsigned int MAX_SYMMETRY=8;

typedef bool BoolPic[MAXSPAN][MAXSPAN];
typedef BoolPic BoolPicSet[MAX_SYMMETRY];


extern bool isUsed(const BoolPic a);


#endif // BoolPIC_H__

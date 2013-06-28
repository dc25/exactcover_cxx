#ifndef BoolPIC_H__
#define BoolPIC_H__

const unsigned int MAXSPAN=5;
const unsigned int MAX_SYMMETRY=8;

typedef bool BoolPic[MAXSPAN][MAXSPAN];
typedef BoolPic BoolPicSet[MAX_SYMMETRY];

class Pentomino {
public:
	Pentomino(char* n, BoolPicSet& rep)
		: m_name(n)
	{
		for (unsigned int h = 0; h < MAX_SYMMETRY; ++h)
		{
			for (unsigned int i = 0; i < MAXSPAN; ++i)
			{
				for (unsigned int j = 0; j < MAXSPAN; ++j)
				{
					m_rep[h][i][j] = rep[h][i][j];
				}
			}
		}
	}

	char* m_name;
	BoolPicSet m_rep;
};


extern bool isUsed(const BoolPic a);


#endif // BoolPIC_H__

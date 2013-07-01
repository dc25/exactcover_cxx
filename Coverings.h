#ifndef coverings_h__
#define coverings_h__

#include <vector>
#include <memory>

class Cell;

class Coverings 
{
public:
	Coverings(
        const std::vector< std::vector< int > >& usage,
        const std::vector< std::string >& columns,
		unsigned int secondary=0);

	const std::vector< std::vector<std::string> >* getSolution();

	unsigned int num_searches;

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

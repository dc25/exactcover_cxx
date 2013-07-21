#ifndef CELL_H__
#define CELL_H__

class Cell {
public:
    Cell()
        : m_left(0), m_right(0), m_up(0), m_down(0), m_col(0), m_useCount(0), m_name(0)
    {
    }

    ~Cell()
    {
        delete[] m_name;
    }

    Cell* m_left;
    Cell* m_right;
    Cell* m_up;
    Cell* m_down;
    Cell* m_col;

    std::shared_ptr< std::vector<std::string> > m_nameVector;

	unsigned int m_useCount;

    char* m_name;  // tried using a string here but that slowed down run time noticably.
};

#endif // CELL_H__

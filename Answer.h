#ifndef ANSWER_H__
#define ANSWER_H__

#include <string>
#include <vector>

class Cell;

class Answer {
public:
    Answer(const std::vector<Cell* >& solution);

    Answer(const std::vector < std::vector<std::string> >& nameSolution);

    void resize(size_t s)
    {
        m_nameSolution.resize(s);
    }

    std::vector<std::string>& getRow(size_t index)
    {
        return m_nameSolution[index];
    }

    const std::vector<std::string>& getRow(size_t index) const
    {
        return m_nameSolution[index];
    }

    size_t size() const
    {
        return m_nameSolution.size();
    }

    bool matchesFirstRow(const Cell* row) const;


    void removeFirstRow() 
    {
        m_nameSolution.erase(m_nameSolution.begin());
    }

private:
    std::vector < std::vector<std::string> > m_nameSolution;
};

#endif // ANSWER_H__

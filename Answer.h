#ifndef ANSWER_H__
#define ANSWER_H__

#include <string>
#include <vector>

class Answer {
public:
    void resize(size_t s)
    {
        m_nameSolution.resize(s);
    }

    std::vector<std::string>& getRow(size_t index)
    {
        return m_nameSolution[index];
    }

    size_t size() const
    {
        return m_nameSolution.size();
    }

private:
    std::vector < std::vector<std::string> > m_nameSolution;
};

#endif // ANSWER_H__

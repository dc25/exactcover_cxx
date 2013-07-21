#ifndef ANSWER_H__
#define ANSWER_H__

#include <string>
#include <vector>
#include <memory>

class Answer {
public:
    Answer(const Answer& nameSolution);

    Answer(const std::vector< std::vector<std::string> >& vvs);

    size_t size() const
    {
        return m_nameSolution.size();
    }

    void resize(size_t s)
    {
        m_nameSolution.resize(s);
    }

    void push_back(std::shared_ptr< std::vector<std::string> > pvs)
    {
        m_nameSolution.push_back(pvs);
    }

    void pop_back()
    {
        m_nameSolution.pop_back();
    }

    const std::vector<std::string>& getRow(size_t index) const
    {
        return *(m_nameSolution[index]);
    }

private:
    std::vector < std::shared_ptr< std::vector<std::string> > > m_nameSolution;
};

#endif // ANSWER_H__

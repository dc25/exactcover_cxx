#include "Answer.h"
#include "Cell.h"
#include <algorithm>

using namespace std;


Answer::Answer(const std::vector<Cell* >& solution)
{
    resize(solution.size());
    for (size_t i = 0; i < size(); ++i)
    {
        m_nameSolution[i] = solution[i]->m_nameVector;
    }
}

Answer::Answer(const std::vector < std::vector<std::string> >& nameSolution)
{
    m_nameSolution.resize(nameSolution.size());
    for (size_t i = 0; i < nameSolution.size(); ++i)
    {
        m_nameSolution[i] = make_shared< vector <string> >();
        std::vector<std::string>& row = getRow(i);
        for ( auto s : nameSolution[i])
        {
            row.push_back(s);
        }
        // Sort because convention is to expect piece name
        // to be last item in each row for the solution.
        sort(row.begin(), row.end());
    }
}


bool Answer::matchesFirstRow(const Cell* row) const 
{
    const vector<string>& rowNames = *(row->m_nameVector);

    const vector<string>& answerNames = getRow(0);

    if (answerNames.size() != rowNames.size())
    {
        return false;
    }

    for (size_t i = 0; i < answerNames.size(); ++i)
    {
        if (answerNames[i] != rowNames[i])
        {
            return false;
        }
    }
    return true;
}



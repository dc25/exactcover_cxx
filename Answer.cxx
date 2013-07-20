#include "Answer.h"
#include "Cell.h"
#include <algorithm>

using namespace std;


static void rowToSortedString(const Cell* row, /*output*/ vector<string>& vstr)
{
    for ( auto e = row; true;)
    {
        vstr.push_back(e->m_col->m_name);
        e=e->m_right;
        if (e == row)
        {
            break;
        }
    }
    sort(vstr.begin(), vstr.end());
}



Answer::Answer(const std::vector<Cell* >& solution)
{
    resize(solution.size());
    unsigned int solutionIndex = 0;
    for ( auto r : solution )
    {
        // Sort because convention is to expect piece name
        // to be last item in each row for the solution.
        std::vector<string> temp;

        rowToSortedString(r, temp);

        for (auto name : temp)
        {
            getRow(solutionIndex).push_back(name);
        }
        solutionIndex++;
    }
}

Answer::Answer(const std::vector < std::vector<std::string> >& nameSolution)
{
    resize(nameSolution.size());
    unsigned int solutionIndex = 0;
    for ( auto r : nameSolution )
    {
        // Sort because convention is to expect piece name
        // to be last item in each row for the solution.
        std::vector<string> temp;
        for ( auto s : r)
        {
            temp.push_back(s);
        }
        sort(temp.begin(), temp.end());

        for (auto name : temp)
        {
            getRow(solutionIndex).push_back(name);
        }
        solutionIndex++;
    }
}


bool Answer::matchesFirstRow(const Cell* row) const 
{
    std::vector<string> rowNames;
    rowToSortedString(row, rowNames);

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



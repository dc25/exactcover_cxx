#include "Answer.h"
#include <algorithm>

using namespace std;

Answer::Answer(const Answer& nameSolution)
{
    for (size_t i = 0; i < nameSolution.size(); ++i)
    {
        m_nameSolution.push_back( 
            make_shared< vector <string> >(*(nameSolution.m_nameSolution[i])));
    }
}

Answer::Answer(const std::vector< std::vector<std::string> >& vvs)
{
    for (size_t i = 0; i < vvs.size(); ++i)
    {
        m_nameSolution.push_back(make_shared< vector <string> >(vvs[i]));
        std::vector<std::string>& row =  *(m_nameSolution[i]);
        // Sort because convention is to expect piece name
        // to be last item in each row for the solution.
        sort(row.begin(), row.end());
    }
}




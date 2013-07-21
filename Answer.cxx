#include "Answer.h"
#include <algorithm>

using namespace std;

Answer::Answer(const std::vector< std::vector<std::string> >& vvs)
{
    for (size_t i = 0; i < vvs.size(); ++i)
    {
        m_nameSolution.push_back(make_shared< vector <string> >(vvs[i]));
        std::vector<std::string>& row =  *(m_nameSolution[i]);
        // name vectors are sorted for vector equality testing.
        sort(row.begin(), row.end());
    }
}




#include "Coverings.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Answer.h"

using namespace std;

int main()
{
	vector<string> columns;
	std::ifstream data("data.txt");
	string line;
	while (data >> line)
	{
		if (line == string("BREAK"))
			break;
		// cout << line << endl;
		columns.push_back(line);
	}

	vector<vector<string>> usage;
	vector<vector<string>> startingSolution;

	vector<string> row;
	while (data >> line)
	{
		if (line == string("BREAK"))
		{
			usage.push_back(row);
			row.resize(0);
		} else
		{
			row.push_back(line);
		}
	}

	auto s = new Coverings(usage,columns, startingSolution, 0);
	auto solutionCount = 0;
	while (auto solution = s->getSolution())
	{
		++solutionCount;
		for (size_t r_index = 0; r_index < solution->size(); ++r_index)
		{
			auto r = solution->getRow(r_index);
			for ( auto e : r )
			{
				cout << e << ", ";
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << "Solution Count =" << solutionCount << endl;
}
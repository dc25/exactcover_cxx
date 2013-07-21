/*
Dancing Links / Exact Cover library with c++ and python interfaces
Copyright (c) 2013 by David Compton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

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

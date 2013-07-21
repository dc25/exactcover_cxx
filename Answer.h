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

#ifndef ANSWER_H__
#define ANSWER_H__

#include <string>
#include <vector>
#include <memory>

class Answer {
public:
    Answer(const std::vector< std::vector<std::string> >& vvs);

    size_t size() const
    {
        return m_nameSolution.size();
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

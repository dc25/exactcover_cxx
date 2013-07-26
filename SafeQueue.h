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

#ifndef SAFE_QUEUE_H__
#define SAFE_QUEUE_H__

#include <assert.h>
#include <queue>
#include <mutex>
#include <condition_variable>

template<class T> class SafeQueue {
public:
    void push(const T &entry)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(entry);
        m_notEmpty.notify_one();
    }

    T front()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // default constructor should be a "NULL" T type.
        T res;
        if (m_queue.empty())
        {
            assert(!res);
        } else
        {
            res = m_queue.front();
        }
        return res;
    }

    T deque()
    {

        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty()) 
        {
            m_notEmpty.wait(lock);
        }

        T res = m_queue.front();
        m_queue.pop();
        return res;
    }

private:
    std::queue<T> m_queue;
    std::mutex    m_mutex;
    std::condition_variable m_notEmpty;
};

#endif // SAFE_QUEUE_H__

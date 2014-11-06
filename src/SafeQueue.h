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

/*
 * Thread safe queue of limited size.
 */
template<class T> class SafeQueue {

public:

    // No default constructor - capacity must be specified.
    SafeQueue() = delete;

    SafeQueue(size_t capacity) 
        : m_capacity(capacity)
    {
    }

    /*
     * Make a copy of 'entry' onto the queue.  If queue
     * is full then block until there is room available.
     */
    void push(const T &entry)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.size() == m_capacity)
        {
            m_notFull.wait(lock);
        }

        m_queue.push(entry);
        m_notEmpty.notify_one();
    }

    /*
     * Copy the front of the queue to argument 'result'
     * Return false if the queue is empty.
     */
    bool front(T &result)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            return false;
        } 
        result = m_queue.front();
        return true;
    }

    /*
     * Remove and return the front queue entry.
     * If the queue is empty then block until not empty.
     */
    T deque()
    {

        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty()) 
        {
            m_notEmpty.wait(lock);
        }

        T res = m_queue.front();
        m_queue.pop();
        m_notFull.notify_one();
        return res;
    }

private:
    size_t        m_capacity;
    std::queue<T> m_queue;
    std::mutex    m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
};

#endif // SAFE_QUEUE_H__

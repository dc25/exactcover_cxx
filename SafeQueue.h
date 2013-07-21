#ifndef SAFE_QUEUE_H__
#define SAFE_QUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T> class SafeQueue {
public:
    void push(T &entry)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(entry);
        m_notEmpty.notify_one();
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

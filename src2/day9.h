#if !defined(DAY9_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   ======================================================================== */

#define DAY9_H

#include <condition_variable>
#include <thread>
#include <atomic>
#include <queue>
#include <future>
#include <functional>

#include <iostream>


using namespace std;



template <typename T>
class ThreadSafeQueue
{
public:
  ~ThreadSafeQueue(void)
  {
    invalidate();
  }

  bool
  tryPop(T& out)
  {
    lock_guard<mutex> lock{m_mutex};
    bool Result = true;
    if(m_queue.empty() || !m_valid)
    {
      Result = false;
    }
    else
    {
      out = move(m_queue.front());
      m_queue.pop();
      Result = true;
    }

    return Result;
  }

  bool
  waitPop(T& out)
  {
    unique_lock<mutex> lock{m_mutex};
    bool Result = false;

    m_condition.wait(lock, [this] {return !m_queue.empty() || !m_valid;});

    if(m_valid)
    {
      out = std::move(m_queue.front());
      m_queue.pop();
      Result = true;
    }
    else
    {
      Result = false;
    }

    return Result;
  }

  void
  push(T value)
  {
    lock_guard<mutex> lock{m_mutex};

    m_queue.push(move(value));
    m_condition.notify_one();
  }

  bool
  empty(void)
  {
    lock_guard<mutex> lock{m_mutex};
    return m_queue.empty();
  }

  void
  clear(void)
  {
    lock_guard<mutex> lock{m_mutex};
    while(!m_queue.empty())
    {
      m_queue.pop();
    }
    m_condition.notify_all();
  }

  void
  invalidate(void)
  {
    lock_guard<mutex> lock{m_mutex};
    m_valid = false;
    m_condition.notify_all();
  }

  bool
  isValid(void)
  {
    lock_guard<mutex> lock{m_mutex};
    return m_valid;
  }

private:
  atomic_bool m_valid{true};
  mutable mutex m_mutex;
  queue<T> m_queue;
  condition_variable m_condition;

};

class ThreadPool
{
private:
  class IThreadTask
  {
public:
    IThreadTask(void) = default;
    virtual
    ~IThreadTask(void) = default;
    IThreadTask(const IThreadTask& rhs) = delete;
    IThreadTask&
    operator=(const IThreadTask& rhs) = delete;
    IThreadTask(IThreadTask&& other) = default;
    IThreadTask&
    operator=(IThreadTask&& other) = default;

    virtual void
    execute() = 0;
  };

  template<typename Func>
  class ThreadTask : public IThreadTask
  {

public:
    ThreadTask(Func&& func) : m_func{move(func)} {}

    ~ThreadTask(void) override = default;
    ThreadTask(const ThreadTask& rhs) = delete;
    ThreadTask&
    operator=(const ThreadTask& rhs) = delete;
    ThreadTask(ThreadTask&& other) = default;
    ThreadTask&
    operator=(ThreadTask&& other) = default;

    void
    execute() override
    {
      m_func();
    }

private:
    Func m_func;
  };

public:

  template <typename T>
  class TaskFuture
  {
public:
    TaskFuture(future<T>&& future) : m_future{move(future)}
    {}

    TaskFuture(const TaskFuture& rhs) = delete;
    TaskFuture&
    operator=(const TaskFuture& rhs) = delete;
    TaskFuture(TaskFuture&& other) = default;
    TaskFuture&
    operator=(TaskFuture&& other) = default;

    ~TaskFuture(void)
    {
      if(m_future.valid())
      {
        m_future.get();
      }
    }

    auto
    get(void)
    {
      return m_future.get();
    }

private:
    future<T> m_future;
  };

public:
  ThreadPool(void) :
    ThreadPool{max(thread::hardware_concurrency(), 2u) - 1u}
  {}

  explicit ThreadPool(const uint32_t numThreads) :
    m_done{false},
    m_workQueue{},
    m_threads{}
  {
    std::cout << "Creating " << numThreads << " threads in pool.\n";
    try
    {
      for(uint32_t i = 0u; i < numThreads; ++i)
      {
        m_threads.emplace_back(&ThreadPool::worker, this);
      }
    }
    catch(...)
    {
      destroy();
      throw;
    }
  }

  ThreadPool(const ThreadPool& rhs) = delete;
  ThreadPool&
  operator=(const ThreadPool& rhs) = delete;

  ~ThreadPool(void)
  {
    destroy();
  }

  template <typename Func, typename ... Args>
  auto
  submit(Func&& func,
         Args&& ... args)
  {
    auto boundTask = bind(forward<Func>(func), forward<Args>(args) ...);
    using ResultType = result_of_t<decltype(boundTask)()>;
    using PackagedTask = packaged_task<ResultType()>;
    using TaskType = ThreadTask<PackagedTask>;

    PackagedTask task{move(boundTask)};
    TaskFuture<ResultType> result{task.get_future()};
    m_workQueue.push(make_unique<TaskType>(move(task)));

    return result;
  }

private:
  void
  worker(void)
  {
    while(!m_done)
    {
      unique_ptr<IThreadTask> pTask{nullptr};
      if(m_workQueue.waitPop(pTask))
      {
        pTask->execute();
      }
    }
  }

  void
  destroy(void)
  {
    m_done = true;
    m_workQueue.invalidate();
    for(auto& thread : m_threads)
    {
      if(thread.joinable())
      {
        thread.join();
      }
    }
  }
private:
  atomic_bool m_done;
  ThreadSafeQueue<unique_ptr<IThreadTask> > m_workQueue;
  vector<thread> m_threads;
};

inline ThreadPool&
getThreadPool(void)
{
  static ThreadPool defaultPool;
  return defaultPool;
}

template <typename Func, typename ... Args>
inline auto
submitJob(Func&& func,
          Args&& ... args)
{
  return getThreadPool().submit(forward<Func>(func), forward<Args>(args) ...);
}

#endif

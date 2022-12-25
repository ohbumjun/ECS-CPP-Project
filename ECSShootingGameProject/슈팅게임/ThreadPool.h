#pragma once

#include "PoolThread.h"
#include "Flag.h"


class ThreadPool
{
private:
	std::size_t m_NumThreads;
	// std::vector<std::thread> m_Workers;
	std::vector<PoolThread*> m_Workers;
	std::queue<std::function<void()>> m_Jobs;
	std::condition_variable m_CV;
	std::mutex m_Mtx;

	bool m_StopAll;
public:
	ThreadPool(size_t NumThreads);
	~ThreadPool();
public:
	bool Init();
	template<typename F, typename ...Args>
	std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
		F&& f, Args&&... args);

	template<typename F, typename ...Args>
	void SetEndFunction(int Index, F&& f, Args&&... args);

public:
	bool IsStopAll() const
	{
		return m_StopAll;
	}
	bool IsJobEmpty() const
	{
		return m_Jobs.empty();
	}
	const std::vector<PoolThread*>& GetWorkers() const
	{
		return m_Workers;
	}
	std::queue<std::function<void()>>& GetQueueJob()
	{
		return m_Jobs;
	}
};

template<typename F, typename ...Args>
inline std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(
	F&& f, Args && ...args)
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto Job = std::make_shared<std::packaged_task<return_type()>>
		(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	std::future<return_type> Job_Future = Job->get_future();

	{
		std::lock_guard<std::mutex> lock(m_Mtx);

		m_Jobs.push([Job]() {(*Job)(); });
	}

	// 자고 있는 Thread 를 하나 깨운다.
	m_CV.notify_one();

	return Job_Future;
}

template<typename F, typename ...Args>
inline void ThreadPool::SetEndFunction(int Index, F&& f, Args && ...args)
{
	if (Index >= m_NumThreads)
		assert(false);

	m_Workers[Index]->m_EndFunction = std::bind(std::forward<F>(f), 
		std::forward<Args>(args)...);
}

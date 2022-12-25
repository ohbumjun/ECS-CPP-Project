#pragma once

#include "Flag.h"

class PoolThread
{
	friend class ThreadPool;
public:
	PoolThread(std::condition_variable* CV, std::mutex* Mtx,
		class ThreadPool* Owner);
	~PoolThread();
private:
	class ThreadPool* m_OwnerPool;
	std::thread m_Thread;
	std::function<void()> m_EndFunction;

	std::condition_variable* m_CV;
	std::mutex* m_Mtx;
private:
	bool Init();
	void WorkThread();
};


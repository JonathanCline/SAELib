#pragma once

#include "SAELib_Concepts.h"

#include <queue>
#include <mutex>

namespace sae
{

	template <typename T>
	class thread_queue
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		void lock() const noexcept { return this->mtx_.lock(); };
		void unlock() const noexcept { return this->mtx_.unlock(); };

		template <cx_forward<T> _T>
		void push(_T&& _val)
		{
			this->lock();
			this->queue_.push(std::forward<_T>(_val));
			this->unlock();
		};
		
		bool empty() const noexcept
		{
			this->lock();
			auto _out = this->queue_.empty();
			this->unlock();
			return _out;
		};

		T next()
		{
			this->lock();
			auto _out = std::move(this->queue_.front());
			this->queue_.pop();
			this->unlock();
			return _out;
		};
		void ignore(int n = 1)
		{
			this->lock();
			for (int i = 0; i < n && !this->queue_.empty(); ++i)
			{
				this->queue_.pop();
			};
			this->unlock();
		};

		void clear() noexcept
		{
			this->lock();
			while (!this->queue_.empty())
			{
				this->queue_.pop();
			};
			this->unlock();
		};
		
	private:
		mutable std::mutex mtx_{};
		std::queue<T> queue_{};

	};


};

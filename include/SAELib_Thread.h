#pragma once

#include "SAELib_Singleton.h"
#include "SAELib_Timer.h"
#include "SAELib_Time.h"

#include <chrono>
#include <thread>
#include <concepts>
#include <type_traits>
#include <mutex>

namespace sae
{

	template <typename Rep, typename Period>
	static void sleep(const std::chrono::duration<Rep, Period>& _duration)
	{
		std::this_thread::sleep_for(_duration);
	};

	static void sleep_ns(double _nanoSeconds)
	{
		sleep(std::chrono::nanoseconds{ (typename std::chrono::nanoseconds::rep) _nanoSeconds });
	};
	static void sleep_us(double _microSeconds)
	{
		sleep_ns(_microSeconds * 1000.0);
	};
	static void sleep_ms(double _milliSeconds)
	{
		sleep_us(_milliSeconds * 1000.0);
	};
	static void sleep_s(double _seconds)
	{
		sleep_ms(_seconds * 1000.0);
	};


	static time_point now() noexcept(noexcept(clock().now()))
	{
		return clock().now();
	};

	static void sleep_until(time_point _time) noexcept(noexcept(std::this_thread::sleep_until(_time)))
	{
		std::this_thread::sleep_until(_time);
	};




	struct thread
	{
	public:
		bool joinable() const noexcept { return this->thread_.joinable(); };
		void join() { this->thread_.join(); };

		bool try_join()
		{
			auto _out = this->joinable();
			if (_out)
			{
				this->join();
			};
			return _out;
		};
		bool try_join(duration _timeout)
		{
			const auto _sleepDur = nanoseconds{ _timeout } / 100;
			timer _timer{ _timeout };
			while (!_timer.finished() && !this->joinable()) { sleep(_sleepDur); };
			auto _out = this->joinable();
			if (_out)
			{
				this->join();
			};
			return _out;
		};

		void join(duration _timeout)
		{
			auto _res = this->try_join(_timeout);
#ifndef NDEBUG
			if (!_res) { std::terminate(); };
#endif
		};

		explicit thread(std::thread&& _thread) :
			thread_{ std::move(_thread) }
		{};
		thread& operator=(std::thread&& _thread)
		{
			this->try_join();
			this->thread_ = std::move(_thread);
			return *this;
		};

		thread() :
			thread_{}
		{};

		template <typename FunctionT, typename... ArgTs>
		thread(FunctionT&& _function, ArgTs&&... _args) :
			thread_{ std::forward<FunctionT>(_function), std::forward<ArgTs>(_args)... }
		{};

		thread(thread&& other) noexcept : 
			thread_{ std::move(other.thread_) }
		{};
		thread& operator=(thread&& other) noexcept
		{
			this->try_join();
			this->thread_ = std::move(other.thread_);
			return *this;
		};

		~thread()
		{
			this->try_join();
		};

	private:
		std::thread thread_;
	};

	class ithread
	{
	public:
		using flag_type = bool;

		void lock() const noexcept { this->thread_mtx_.lock(); };
		void unlock() const noexcept { this->thread_mtx_.unlock(); };

		bool is_running() const noexcept { return (bool)this->run_thread_; };
		const flag_type* run_thread_flag() const noexcept { return &this->run_thread_; };

	private:
		void set_run_thread_flag(bool _to)
		{
			this->lock();
			this->run_thread_ = _to;
			this->unlock();
		};

	public:
		bool joinable() const noexcept { return this->thread_.joinable(); };
		void join()
		{ 
#ifndef NDEBUG
			if (!this->joinable())
			{
				std::terminate();
			};
#endif
			this->thread_.join(); 
		};

		void shutdown(duration _timeoutDur = 100ms)
		{
			if (this->is_running())
			{
				this->set_run_thread_flag(false);
				this->thread_.join(_timeoutDur);
			};
		};
		bool try_shutdown(duration _timeoutDur = 100ms)
		{
			auto _out = true;
			if (this->is_running())
			{
				this->set_run_thread_flag(false);
				_out = this->thread_.try_join(_timeoutDur);
			};
			return _out;
		};

		explicit ithread(thread&& _thread) :
			thread_{ std::move(_thread) }
		{};
		ithread& operator=(thread&& _thread)
		{
			this->try_shutdown();
			this->thread_ = std::move(_thread);
			return *this;
		};

		ithread() :
			thread_{}
		{};

		template <typename FunctionT, typename... ArgTs>
		ithread(FunctionT&& _function, ArgTs&&... _args) :
			thread_{ std::forward<FunctionT>(_function), std::forward<ArgTs>(_args)... }
		{};

		ithread(ithread&& other) noexcept :
			thread_{ std::move(other.thread_) }
		{};
		ithread& operator=(ithread&& other) noexcept
		{
			this->try_shutdown();
			this->thread_ = std::move(other.thread_);
			return *this;
		};

		~ithread()
		{
			this->try_shutdown();
		};

	private:
		mutable std::mutex thread_mtx_{};
		bool run_thread_ = true;
		thread thread_{};

	};


};


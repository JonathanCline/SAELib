#pragma once

#include "SAELib_Singleton.h"
#include "SAELib_Timer.h"
#include "SAELib_Time.h"
#include "SAELib_Concepts.h"

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

	struct nolock_t {};
	constexpr static nolock_t nolock{};

	template <typename T>
	class mutex_decorator
	{
	private:
		auto& get_decoratee_mtx() const noexcept { return static_cast<T*>(this)->get_mtx(); };

	public:
		void lock() const noexcept
		{
			auto& _mtx = this->get_decoratee_mtx();
#ifndef NDEBUG
			if (this->owns_mutex_)
			{
				std::terminate();
			};
			this->owns_mutex_ = true;
#endif
			_mtx.lock();
		};
		void unlock() const noexcept
		{
			auto& _mtx = this->get_decoratee_mtx();
#ifndef NDEBUG
			if (!this->owns_mutex_)
			{
				std::terminate();
			};
			this->owns_mutex_ = false;
#endif
			_mtx.unlock();
		};

	private:
#ifndef NDEBUG
		mutable bool owns_mutex_ = false;
#endif

	};

	class mutex : public mutex_decorator<mutex>
	{
	private:
		friend mutex_decorator<mutex>;
		using mtx_type = std::mutex;
		mtx_type& get_mtx() const noexcept { return this->mtx_; };

	public:
		auto acquire() const
		{
			return std::unique_lock<mtx_type>{ this->get_mtx() };
		};

		mutex() = default;

	private:
		mutable mtx_type mtx_{};
	};
	



	template <typename T, typename MtxT>
	class basic_resource_guard
	{
	public:
		using mutex_type = MtxT;

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		class const_resource_lock
		{
		public:
			using value_type = T;
			using const_pointer = const value_type*;
			using const_reference = const value_type&;

		protected:
			const_pointer get_valptr() const noexcept
			{
				return this->val_;
			};

		public:
			using mutex_type = MtxT;

			bool has_lock() const noexcept { return this->mtx_ != nullptr; };
			void release() noexcept
			{
				if (this->has_lock())
				{
					this->mtx_->unlock();
					this->mtx_ = nullptr;
					this->val_ = nullptr;
				};
			};

			const_reference value() const noexcept { return *this->get_valptr(); };
			const_reference operator*() const noexcept { return this->value(); };
			const_pointer operator->() const noexcept { return this->get_valptr(); };

			const_resource_lock(const const_resource_lock& other) = delete;
			const_resource_lock& operator=(const const_resource_lock& other) = delete;

			const_resource_lock(const_resource_lock&& other) noexcept :
				mtx_{ std::exchange(other.mtx_, nullptr) }, val_{ std::exchange(other.val_, nullptr) }
			{};
			const_resource_lock& operator=(const_resource_lock&& other) noexcept
			{
				this->release();
				this->mtx_ = std::exchange(other.mtx_, nullptr);
				this->val_ = std::exchange(other.val_, nullptr);
				return *this;
			};

			~const_resource_lock()
			{
				this->release();
			};

		private:
			static mutex_type* aquire_mtx(mutex_type& _mtx)
			{
				auto _result = _mtx.try_lock();
				return ((_result) ? &_mtx : nullptr);
			};

			friend basic_resource_guard<T, mutex_type>;

			const_resource_lock(mutex_type& _mtx, const_pointer _val) noexcept :
				mtx_{ aquire_mtx(_mtx) }, val_{ ((this->mtx_ != nullptr) ? _val : nullptr) }
			{
				this->mtx_->lock();
			};

			mutex_type* mtx_ = nullptr;
			const_pointer val_ = nullptr;

		};
		class resource_lock
		{
		public:
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;
			using const_pointer = const value_type*;
			using const_reference = const value_type&;

		protected:
			pointer get_valptr() const noexcept
			{
				return this->val_;
			};

		public:
			using mutex_type = MtxT;

			bool has_lock() const noexcept { return this->mtx_ != nullptr; };
			void release() noexcept
			{
				if (this->has_lock())
				{
					this->mtx_->unlock();
					this->mtx_ = nullptr;
					this->val_ = nullptr;
				};
			};

			reference value() noexcept { return *this->get_valptr(); };
			reference operator*() noexcept { return this->value(); };
			pointer operator->() noexcept { return this->get_valptr(); };

			const_reference value() const noexcept { return *this->get_valptr(); };
			const_reference operator*() const noexcept { return this->value(); };
			const_pointer operator->() const noexcept { return this->get_valptr(); };

			resource_lock(const resource_lock& other) = delete;
			resource_lock& operator=(const resource_lock& other) = delete;

			resource_lock(resource_lock&& other) noexcept :
				mtx_{ std::exchange(other.mtx_, nullptr) }, val_{ std::exchange(other.val_, nullptr) }
			{};
			resource_lock& operator=(resource_lock&& other) noexcept
			{
				this->release();
				this->mtx_ = std::exchange(other.mtx_, nullptr);
				this->val_ = std::exchange(other.val_, nullptr);
				return *this;
			};

			~resource_lock()
			{
				this->release();
			};

		private:
			static mutex_type* aquire_mtx(mutex_type& _mtx)
			{
				auto _result = _mtx.try_lock();
				return ((_result) ? &_mtx : nullptr);
			};

			friend basic_resource_guard<T, mutex_type>;

			resource_lock(mutex_type& _mtx, pointer _val) noexcept :
				mtx_{ aquire_mtx(_mtx) }, val_{ ((this->mtx_ != nullptr) ? _val : nullptr) }
			{
				this->mtx_->lock();
			};

			mutex_type* mtx_ = nullptr;
			pointer val_ = nullptr;

		};

	protected:
		auto& get_mtx() const noexcept { return this->mtx_; };

		auto& resource() noexcept { return this->resource_; };
		const auto& resource() const noexcept { return this->resource_; };

	public:
		void lock() const noexcept { return this->get_mtx().lock(); };
		void unlock() const noexcept { return this->get_mtx().unlock(); };

		resource_lock acquire()
		{
			return resource_lock{ this->get_mtx(), &this->resource() };
		};
		const_resource_lock acquire() const
		{
			return const_resource_lock{ this->get_mtx(), &this->resource() };
		};
		const_resource_lock cacquire() const
		{
			return const_resource_lock{ this->get_mtx(), &this->resource() };
		};

		reference value() noexcept { return this->resource_; };
		const_reference value() const noexcept { return this->resource_; };

		reference operator*() noexcept { return this->value(); };
		const_reference operator*() const noexcept { return this->value(); };

		pointer operator->() noexcept { return &this->value(); };
		const_pointer operator->() const noexcept { return &this->value(); };

		basic_resource_guard() = default;

		basic_resource_guard(const value_type& _val) :
			resource_{ _val }
		{};
		basic_resource_guard& operator=(const value_type& _val)
		{
			auto _resource = this->acquire();
			(*_resource) = _val;
			return *this;
		};

		basic_resource_guard(value_type&& _val) :
			resource_{ std::move(_val) }
		{};
		basic_resource_guard& operator=(value_type&& _val)
		{
			auto _resource = this->acquire();
			(*_resource) = std::move(_val);
			return *this;
		};

	private:
		mutable mutex_type mtx_;
		value_type resource_;

	};

	template <typename T>
	using resource_guard = basic_resource_guard<T, std::mutex>;


};


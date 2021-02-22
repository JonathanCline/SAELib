#pragma once

#include <optional>
#include <type_traits>
#include <mutex>

namespace sae
{
	namespace impl
	{
		template <typename T, typename TagT>
		extern inline std::optional<T> SINGLETON_IMPL{ std::nullopt };
	};

	namespace impl
	{
		template <typename T> requires std::is_default_constructible_v<T>
		constexpr static auto l_default_construct = [](std::optional<T>& _op) 
		{
			_op.emplace();
		};
	};

	template <typename T, typename TagT = void>
		requires requires { impl::SINGLETON_IMPL<T, TagT>; }
	static T& get_singleton()
	{
		if (impl::SINGLETON_IMPL<T, TagT>.has_value()) [[likely]]
		{
			return impl::SINGLETON_IMPL<T, TagT>.value();
		}
		else
		{
			impl::SINGLETON_IMPL<T, TagT>.emplace();
			return get_singleton<T, TagT>();
		};
	};

	template <typename T, typename TagT> requires std::is_default_constructible_v<T>
	struct ThreadsafeSingleton
	{
	public:
		void lock() const noexcept { this->mtx_.lock(); };
		void unlock() const noexcept { this->mtx_.unlock(); };

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		reference get() noexcept { return this->data_; };
		const_reference get() const noexcept { return this->data_; };

		pointer operator->() noexcept { return &this->get(); };
		const_pointer operator->() const noexcept { return &this->get(); };

		reference operator*() noexcept { return this->get();  };
		const_reference operator*() const noexcept { return this->get(); };

		ThreadsafeSingleton() = default;

		ThreadsafeSingleton(const ThreadsafeSingleton& other) = delete;
		ThreadsafeSingleton& operator=(const ThreadsafeSingleton& other) = delete;
		ThreadsafeSingleton(ThreadsafeSingleton&& other) = delete;
		ThreadsafeSingleton& operator=(ThreadsafeSingleton&& other) = delete;

	private:
		mutable std::mutex mtx_{};
		T data_{};

	};

	namespace impl
	{
		template <typename T, typename TagT> requires std::is_default_constructible_v<T>
		extern inline std::mutex TS_SINGLETON_IMPL_MTX{};

		template <typename T, typename TagT> requires std::is_default_constructible_v<T>
		extern inline std::optional<ThreadsafeSingleton<T, TagT>> TS_SINGLETON_IMPL{ std::nullopt };

	};

	template <typename T, typename TagT = void> requires requires { impl::TS_SINGLETON_IMPL<T, TagT>; }
	static ThreadsafeSingleton<T, TagT>* get_singleton_threadsafe()
	{
		if (impl::TS_SINGLETON_IMPL<T, TagT>.has_value()) [[likely]]
		{
			return &impl::TS_SINGLETON_IMPL<T, TagT>.value();
		}
		else
		{
			impl::TS_SINGLETON_IMPL_MTX<T, TagT>.lock();
			if (!impl::TS_SINGLETON_IMPL<T, TagT>.has_value())
			{
				impl::TS_SINGLETON_IMPL<T, TagT>.emplace();
			};
			impl::TS_SINGLETON_IMPL_MTX<T, TagT>.unlock();

			return get_singleton_threadsafe<T, TagT>();
		};
	};

	namespace impl
	{
		template <typename T, typename TagT> requires std::is_default_constructible_v<T>
		extern thread_local inline std::optional<T> THREAD_LOCAL_SINGLETON_IMPL{ std::nullopt };
	};

	template <typename T, typename TagT = void> requires requires { impl::THREAD_LOCAL_SINGLETON_IMPL<T, TagT>; }
	static T& get_singleton_thread_local()
	{
		if (impl::THREAD_LOCAL_SINGLETON_IMPL<T, TagT>.has_value()) [[likely]]
		{
			return impl::THREAD_LOCAL_SINGLETON_IMPL<T, TagT>.value();
		}
		else
		{
			impl::THREAD_LOCAL_SINGLETON_IMPL<T, TagT>.emplace();
			return get_singleton_thread_local<T, TagT>();
		};
	};

	template <typename T, typename TagT>
	class UniqueSingleton;

	namespace impl
	{
	
		template <typename T, typename TagT>
		struct UniqueThreadsafeSingletonOwner
		{
			void lock() const noexcept { this->mtx.lock(); };
			void unlock() const noexcept { this->mtx.unlock(); };

			void emplace() 
			{
				this->val.emplace();
			};
			void reset() 
			{ 
				this->val.reset(); 
			};

			T& value() 
			{ 
				return this->val.value();
			};
			const T& value() const
			{ 
				return this->val.value(); 
			};

			bool has_value() const noexcept 
			{ 
				return this->val.has_value();
			};

			UniqueSingleton<T, TagT> make_unique();

			std::optional<T> val{ std::nullopt };
			mutable std::mutex mtx;

		};

		template <typename T, typename TagT>
		extern inline UniqueThreadsafeSingletonOwner<T, TagT> SAE_UNIQUE_THREADSAFE_SINGLETON_IMPL{};

		template <typename T, typename TagT>
		UniqueSingleton<T, TagT> UniqueThreadsafeSingletonOwner<T, TagT>::make_unique()
		{
			return UniqueSingleton<T, TagT>{ this };
		};

	};

	template <typename T, typename TagT>
	class UniqueSingleton
	{
	public:
		bool has_lock() const noexcept { return this->ptr != nullptr; };
		void release_lock()
		{
			if (this->has_lock())
			{
				this->ptr->mtx.unlock();
				this->ptr = nullptr;
			};
		};

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		reference value() noexcept
		{
			if (this->ptr->has_value()) [[likely]]
			{
				return this->ptr->value();
			}
			else
			{
				this->ptr->emplace();
				return this->value();
			};
		};
		const_reference value() const noexcept
		{
			if (this->ptr->has_value()) [[likely]]
			{
				return this->ptr->value();
			}
			else
			{
				this->ptr->emplace();
				return this->value();
			};
		};

		reference operator*() noexcept { return this->value(); };
		const_reference operator*() const noexcept { return this->value(); };

		pointer operator->() noexcept { return &this->value(); };
		const_pointer operator->() const noexcept { return &this->value(); };



		UniqueSingleton(const UniqueSingleton& other) = delete;
		UniqueSingleton& operator=(const UniqueSingleton& other) = delete;

		UniqueSingleton(UniqueSingleton&& other) :
			ptr{ std::exchange(other.ptr, nullptr) }
		{};
		UniqueSingleton& operator=(UniqueSingleton&& other)
		{
			this->release_lock();
			this->ptr = std::exchange(other.ptr, nullptr);
			return *this;
		};

		~UniqueSingleton()
		{
			this->release_lock();
		};

	private:
		constexpr UniqueSingleton(impl::UniqueThreadsafeSingletonOwner<T, TagT>* _ptr) noexcept :
			ptr{ _ptr }
		{
			if (this->ptr)
			{
				this->ptr->lock();
			};
		};
		friend impl::UniqueThreadsafeSingletonOwner<T, TagT>;
		impl::UniqueThreadsafeSingletonOwner<T, TagT>* ptr;
	};

	template <typename T, typename TagT = void>
	static UniqueSingleton<T, TagT> get_threadsafe_unique_singleton() noexcept
	{
		return impl::SAE_UNIQUE_THREADSAFE_SINGLETON_IMPL<T, TagT>.make_unique();
	};



};
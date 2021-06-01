#pragma once

#include <SAELib_Concepts.h>

#include <type_traits>
#include <concepts>
#include <utility>
#include <cassert>

namespace sae
{
	template <typename T> requires requires (T& a) { a = NULL; }
	constexpr inline T make_null() noexcept
	{
		return NULL;
	};

	template <typename T, auto DestroyFunc, auto MakeNullValueFunc = make_null<T>> requires std::invocable<decltype(DestroyFunc), T&>
	class pimpl
	{
	private:
		using this_type = pimpl<T, DestroyFunc, MakeNullValueFunc>;

	public:
		using value_type = T;

		constexpr auto& get() noexcept { return this->val_; };
		constexpr const auto& get() const noexcept { return this->val_; };

		constexpr auto& operator*() noexcept { return this->get(); };
		constexpr const auto& operator*() const noexcept { return this->get(); };

		constexpr auto* operator->() noexcept { return &this->get(); };
		constexpr const auto* operator->() const noexcept { return &this->get(); };

		constexpr bool good() const noexcept { return !(this->get() == std::invoke(MakeNullValueFunc)); };
		constexpr explicit operator bool() const noexcept { return this->good(); };

		constexpr void release() noexcept
		{
			this->val_ = std::move(std::invoke(MakeNullValueFunc));
		};
		constexpr void reset()
		{
			if (this->good())
			{
				std::invoke(DestroyFunc, this->get());
				this->release();
			};
		};
		constexpr value_type extract() noexcept
		{
			auto _out = std::move(this->get());
			this->release();
			return _out;
		};

		constexpr bool operator==(const value_type& other) const noexcept { return this->get() == other; };
		constexpr bool operator!=(const value_type& other) const noexcept { return !(*this == other); };

		constexpr bool operator==(const pimpl& other) const noexcept { return *this == other.get(); };
		constexpr bool operator!=(const pimpl& other) const noexcept { return !(*this == other); };

		constexpr pimpl() noexcept = default;

		template <cx_forward<T> _T>
		constexpr explicit pimpl(_T&& _val) :
			val_{ std::forward<_T>(_val) }
		{};
		
		pimpl(const pimpl& other) = delete;
		pimpl& operator=(const pimpl& other) = delete;

		constexpr pimpl(pimpl&& other) noexcept :
			val_{ other.extract() }
		{};
		pimpl& operator=(pimpl&& other) noexcept
		{
			this->reset();
			this->get() = other.extract();
			return *this;
		};

		~pimpl()
		{
			this->reset();
		};

	private:
		value_type val_;

	};

	template <typename T, auto DestroyFunc> requires std::invocable<decltype(DestroyFunc), T*&>
	class pimpl_ptr 
	{
	public:
		using value_type = T;
		using pointer = value_type*;

		constexpr pointer& get() noexcept { return this->ptr_; };
		constexpr const pointer& get() const noexcept { return this->ptr_; };
		
		constexpr pointer operator->() const noexcept { return this->get(); };

		constexpr auto& operator*() noexcept { return *this->get(); };
		constexpr const auto& operator*() const noexcept { return *this->get(); };

		constexpr bool good() const noexcept { return (bool)this->get(); };
		constexpr explicit operator bool() const noexcept { return this->good(); };

		constexpr void release() noexcept
		{
			this->ptr_ = nullptr;
		};
		constexpr void reset()
		{
			if (this->good())
			{
				std::invoke(DestroyFunc, this->ptr_);
				this->release();
			};
			assert(!this->good());
		};
		constexpr pointer extract() noexcept
		{
			auto _out = this->get();
			this->release();
			return _out;
		};

		constexpr bool operator==(const pimpl_ptr& other) const { return this->get() == other.get(); };
		constexpr bool operator!=(const pimpl_ptr& other) const { return !(*this == other); };

		constexpr pimpl_ptr() noexcept = default;
		constexpr explicit pimpl_ptr(const pointer _ptr) noexcept :
			ptr_{ _ptr }
		{};

		pimpl_ptr(const pimpl_ptr& other) = delete;
		pimpl_ptr& operator=(const pimpl_ptr& other) = delete;

		constexpr pimpl_ptr(pimpl_ptr&& other) noexcept :
			ptr_{ other.extract() }
		{};
		pimpl_ptr& operator=(pimpl_ptr&& other) noexcept
		{
			this->reset();
			this->ptr_ = other.extract();
			return *this;
		};

		~pimpl_ptr()
		{
			this->reset();
		};

	private:
		pointer ptr_ = nullptr;

	};

	/**
	 * @brief Same as pimpl_ptr but provides an implicit cast to T*
	 * @tparam T Type to manage pointer to
	*/
	template <typename T, auto DestroyFunc> requires std::invocable<decltype(DestroyFunc), T*&>
	class ipimpl_ptr : public pimpl_ptr<T, DestroyFunc>
	{
	private:
		using parent_type = pimpl_ptr<T, DestroyFunc>;
	public:
		constexpr operator T* () const noexcept { return this->get(); };
		using parent_type::parent_type;
	};





	class shared_pimpl_ptr
	{
	public:
	private:
		
	public:
		


	private:
	};

};

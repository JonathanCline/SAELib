#pragma once

#include <concepts>
#include <compare>

namespace sae
{

	template <typename T, typename TagT>
	struct IntegralInvariant
	{
	public:
		using value_type = T;
		using tag_type = TagT;

	private:
		using this_type = IntegralInvariant<T, TagT>;

	public:
		constexpr value_type count() const noexcept { return this->count_; };
		constexpr auto operator<=>(const IntegralInvariant<T, TagT>& other) const noexcept = default;

		constexpr auto operator+(const IntegralInvariant& other) const noexcept
		{
			return IntegralInvariant{ this->count() + other.count() };
		};
		auto& operator+=(const IntegralInvariant& other) noexcept
		{
			this->count_ += other.count();
			return *this;
		};

		constexpr auto operator-(const IntegralInvariant& other) const noexcept
		{
			return IntegralInvariant{ this->count() - other.count() };
		};
		auto& operator-=(const IntegralInvariant& other) noexcept
		{
			this->count_ -= other.count();
			return *this;
		};

		constexpr auto operator+(T _val) const noexcept
		{
			return IntegralInvariant{ this->count() + _val };
		};
		auto& operator+=(T _val) noexcept
		{
			this->count_ += _val;
			return *this;
		};

		constexpr auto operator-(T _val) const noexcept
		{
			return IntegralInvariant{ this->count() - _val };
		};
		auto& operator-=(T _val) noexcept
		{
			this->count_ -= _val;
			return *this;
		};

		constexpr auto operator*(T _val) const noexcept
		{
			return IntegralInvariant{ this->count() * _val };
		};
		auto& operator*=(T _val) noexcept
		{
			this->count_ *= _val;
			return *this;
		};

		constexpr auto operator/(T _val) const noexcept
		{
			return IntegralInvariant{ this->count() / _val };
		};
		auto& operator/=(T _val) noexcept
		{
			this->count_ /= _val;
			return *this;
		};

		constexpr auto& operator++() noexcept
		{
			++this->count_;
			return *this;
		};
		constexpr auto operator++(int) noexcept
		{
			auto _out{ *this };
			++this->count_;
			return _out;
		};

		constexpr auto& operator--() noexcept
		{
			--this->count_;
			return *this;
		};
		constexpr auto operator--(int) noexcept
		{
			auto _out{ *this };
			--this->count_;
			return _out;
		};

		constexpr auto operator-() const noexcept
		{
			return IntegralInvariant{ -this->count() };
		};



		constexpr IntegralInvariant() noexcept = default;
		constexpr explicit IntegralInvariant(T _count) noexcept :
			count_{ _count }
		{};

	private:
		T count_{};
	};



}
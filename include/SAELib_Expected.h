#pragma once

#include <cassert>
#include <variant>
#include <string>
#include <utility>

namespace sae
{
	template <typename ExpectedT, typename AlternateT>
	class expected
	{
	public:
		using value_type = ExpectedT;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using error_type = AlternateT;

		bool good() const noexcept { return this->data_.index() == 0; };
		explicit operator bool() const noexcept { return this->good(); };

		value_type& value()
		{
			assert(this->good());
			return std::get<0>(this->data_);
		};
		const value_type& value() const
		{
			assert(this->good());
			return std::get<0>(this->data_);
		};

		error_type error() const
		{
			return (!this->good()) ? std::get<1>(this->data_) : error_type{};
		};

		constexpr reference operator*() { return this->value(); };
		constexpr const_reference operator*() const { return this->value(); };

		constexpr pointer operator->() { return &this->value(); };
		constexpr const_pointer operator->() const { return &this->value(); };

		struct err_t {};
		constexpr static err_t err{};

		constexpr expected(value_type&& _t) :
			data_{ std::move(_t) }
		{};
		constexpr explicit expected(err_t, error_type&& _t) :
			data_{ std::move(_t) }
		{};

	private:
		std::variant<value_type, error_type> data_;
	};
};
